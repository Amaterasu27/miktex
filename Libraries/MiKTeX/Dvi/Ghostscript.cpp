/* Ghostscript.cpp:

   Copyright (C) 1996-2011 Christian Schenk

   This file is part of the MiKTeX DVI Library.

   The MiKTeX DVI Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2, or (at your option) any later version.

   The MiKTeX DVI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the MiKTeX DVI Library; if not, write to the
   Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139,
   USA.  */

#include "StdAfx.h"

#include "internal.h"

#include "Ghostscript.h"

namespace {
  // make file reading 'wide-open'
  const char * const l_szPermitFileReading = "\
{ << /PermitFileReading [ (*) ] /PermitFileWriting [ ] PermitFileControl [ ]\
 >> setuserparams .locksafe } stopped pop\
";
}

/* _________________________________________________________________________

   Ghostscript::Ghostscript
   _________________________________________________________________________ */

Ghostscript::Ghostscript ()
{
}

/* _________________________________________________________________________

   Ghostscript::~Ghostscript
   _________________________________________________________________________ */

Ghostscript::~Ghostscript ()
{
  MIKTEX_ASSERT (pProcess.get() == 0);
  MIKTEX_ASSERT (pChunkerThread.get() == 0);
  MIKTEX_ASSERT (pStderrReaderThread.get() == 0);
  MIKTEX_ASSERT (gsIn.Get() == 0);
  MIKTEX_ASSERT (gsOut.Get() == 0);
  MIKTEX_ASSERT (gsErr.Get() == 0);
}

/* _________________________________________________________________________

   Ghostscript::Start
   _________________________________________________________________________ */

void
Ghostscript::Start ()
{
  // find Ghostscript
  char szGsExe[BufferSizes::MaxPath];
  unsigned long version;
  SessionWrapper(true)->GetGhostscript (szGsExe, &version);

  // check to see whether the version number is ok
  if (((version >> 16) & 0xffff) < 8)
  {
    FATAL_MIKTEX_ERROR ("Ghostscript::Start",
      T_("At least Ghostscript version 8.00 is required."), 0);
  }

  // make Ghostscript command line
  CommandLineBuilder arguments;
  string res =
    NUMTOSTR(static_cast<double>(pDviImpl->GetResolution())
	     / shrinkFactor);
  arguments.AppendOption ("-r", res + 'x' + res);
  PaperSizeInfo paperSizeInfo = pDviImpl->GetPaperSizeInfo();
  int width = paperSizeInfo.width;
  int height = paperSizeInfo.height;
  if (pDviImpl->Landscape())
  {
    swap (width, height);
  }
  width = 
    static_cast<int>(((pDviImpl->GetResolution() * width) / 72.0)
		     / shrinkFactor);
  height = 
    static_cast<int>(((pDviImpl->GetResolution() * height) / 72.0)
		     / shrinkFactor);
  arguments.AppendOption ("-g",
			    (string(NUMTOSTR(width))
			     + 'x'
			     + NUMTOSTR(height)));
  arguments.AppendOption ("-sDEVICE=", "bmp16m");
  arguments.AppendOption ("-q");
  arguments.AppendOption ("-dBATCH");
  arguments.AppendOption ("-dNOPAUSE");
  arguments.AppendOption ("-dDELAYSAFER");
  arguments.AppendOption ("-sstdout=", "%stderr");
  arguments.AppendOption ("-dTextAlphaBits=", "4");
  arguments.AppendOption ("-dGraphicsAlphaBits=", "4");
  arguments.AppendOption ("-dDOINTERPOLATE");
  arguments.AppendOption ("-sOutputFile=", "-");
  arguments.AppendArgument ("-");

  tracePS->WriteFormattedLine ("libdvi", "%s", arguments.Get());

  ProcessStartInfo startinfo;

  startinfo.Arguments = arguments.Get();
  startinfo.FileName = szGsExe;
  startinfo.StandardInput = 0;
  startinfo.RedirectStandardInput = true;
  startinfo.RedirectStandardOutput = true;
  startinfo.RedirectStandardError = true;

  pProcess.reset (Process::Start(startinfo));

  gsIn.Attach (pProcess->get_StandardInput());
  gsOut.Attach (pProcess->get_StandardOutput());
  gsErr.Attach (pProcess->get_StandardError());

  // start chunker thread
  pChunkerThread.reset (Thread::Start(Chunker, this));

  // start stderr reader
  pStderrReaderThread.reset (Thread::Start(StderrReader, this));

  // enable file reading
  Execute ("%s", l_szPermitFileReading);
}

/* _________________________________________________________________________

   Ghostscript::Read

   Read Ghostscript output.
   _________________________________________________________________________ */

size_t
Ghostscript::Read (/*[out]*/ void *	pBuf,
		   /*[in]*/ size_t	size)
{
  return (gsOut.Read(pBuf, size));
}

/* _________________________________________________________________________

   Ghostscript::OnNewChunk

   Process bitmap chunks.
   _________________________________________________________________________ */

void
Ghostscript::OnNewChunk (/*[in]*/ DibChunk *	pChunk)
{
  PathName fileName;

  // create a BMP file
  fileName.SetToTempFile();
  tracePS->WriteFormattedLine ("libdvi", T_("creating bitmap file %s"), Q_(fileName));
  FileStream stream (File::Open(fileName, FileMode::Create, FileAccess::Write, false));

  const BITMAPINFO * pBitmapInfo = pChunk->GetBitmapInfo();

  size_t nBytesPerLine =
    ((((pBitmapInfo->bmiHeader.biWidth
	* pBitmapInfo->bmiHeader.biBitCount)
       + 31)
      & ~31)
     >> 3);

  unsigned uNumColors =
    (pBitmapInfo->bmiHeader.biBitCount == 24
     ? 0
     : 1 << pBitmapInfo->bmiHeader.biBitCount);

  // dump bitmap file header
  BITMAPFILEHEADER header;
  header.bfType = 0x4d42;
  header.bfSize =
    (sizeof(header)
     + sizeof(BITMAPINFOHEADER)
     + uNumColors * sizeof(RGBQUAD)
     + (static_cast<DWORD>(nBytesPerLine)
	* pBitmapInfo->bmiHeader.biHeight));
  header.bfReserved1 = 0;
  header.bfReserved2 = 0;
  header.bfOffBits =
    (sizeof(header)
     + sizeof(BITMAPINFOHEADER)
     + uNumColors * sizeof(RGBQUAD));
  stream.Write (&header, sizeof(header));

  // dump bitmap info header
  stream.Write (&pBitmapInfo->bmiHeader, sizeof(BITMAPINFOHEADER));

  // dump color table
  stream.Write (pChunk->GetColors(), uNumColors * sizeof(RGBQUAD));

  // dump bits
  stream.Write (pChunk->GetBits(), nBytesPerLine * pBitmapInfo->bmiHeader.biHeight);

  stream.Close ();

  graphicsInclusions.push_back (new GraphicsInclusionImpl(
    ImageType::DIB,
    fileName,
    true,
    pChunk->GetX(),
    pChunk->GetY(),
    pBitmapInfo->bmiHeader.biWidth,
    pBitmapInfo->bmiHeader.biHeight));
}

/* _________________________________________________________________________

   Ghostscript::Chunker
   _________________________________________________________________________ */

void
Ghostscript::Chunker (/*[in]*/ void * pParam)
{
  Ghostscript * This = reinterpret_cast<Ghostscript*>(pParam);
  auto_ptr<DibChunker> pChunker (DibChunker::Create());
  try
  {
    const int chunkSize = 2 * 1024 * 1024;
    while (pChunker->Process(DibChunker::Default, chunkSize, This))
    {
    }
  }
  catch (const exception &)
  {
    This->gsOut.Close ();
    throw;
  }
  pChunker.reset ();
}

/* _________________________________________________________________________

   Ghostscript::StderrReader

   Read Ghostscript transcript.
   _________________________________________________________________________ */

void
Ghostscript::StderrReader (/*[in]*/ void * pParam)
{
  Ghostscript * This = reinterpret_cast<Ghostscript*>(pParam);
  const int chunkSize = 64;
  char buf[ chunkSize ];
  size_t n;
  while ((n = This->gsErr.Read(buf, chunkSize)) > 0)
  {
    This->stderrBuffer.append (buf, n);
  }
}

/* _________________________________________________________________________

   Ghostscript::Write

   Feed Ghostscript with PostScript.
   _________________________________________________________________________ */

void
Ghostscript::Write (/*[in]*/ const void *	p,
		    /*[in]*/ unsigned		n)
{
  gsIn.Write (p, n);
}

/* _________________________________________________________________________

   Ghostscript::Execute
   _________________________________________________________________________ */

void
Ghostscript::Execute (/*[in]*/ const char *	lpszFormat,
		      /*[in]*/			...)
{
  if (pProcess.get() == 0)
  {
    Start ();
  }
  va_list argptr;
  va_start (argptr, lpszFormat);
  string str = Utils::FormatString(lpszFormat, argptr);
  va_end (argptr);
  Write (str.c_str(), str.length());
}

/* _________________________________________________________________________

   Ghostscript::Finalize
   _________________________________________________________________________ */

void
Ghostscript::Finalize ()
{
  // close Ghostscript's input stream
  if (gsIn.Get() != 0)
  {
    gsIn.Close ();
  }

  // wait for Ghostscript to finish
  int gsExitCode = -1;
  if (pProcess.get() != 0)
  {
    if (pProcess->WaitForExit(10000))
    {
      gsExitCode = pProcess->get_ExitCode();
    }
    pProcess.reset ();
  }

  // wait for the chunker to finish
  if (pChunkerThread.get() != 0)
  {
    pChunkerThread->Join ();
    pChunkerThread.reset ();
  }

  // wait for stderr reader to finish
  if (pStderrReaderThread.get() != 0)
  {
    pStderrReaderThread->Join ();
    pStderrReaderThread.reset (0);
  }

  // write the transcript to the debug stream
  if (! stderrBuffer.empty())
  {
    tracePS->WriteFormattedLine ("libdvi", T_("Ghostscript transcript follows:\n\
==========================================================================\n\
%s\n\
=========================================================================="),
	  stderrBuffer.c_str());
  }

  // close Ghostscript's output stream
  if (gsOut.Get() != 0)
  {
    gsOut.Close ();
  }

  // close Ghostscript's error stream
  if (gsErr.Get() != 0)
  {
    gsErr.Close ();
  }

  PostScript::Finalize ();

  // check Ghostscript's exit code
  if (gsExitCode != 0)
  {
    FATAL_MIKTEX_ERROR ("Ghostscript::Finalize",
      T_("Some PostScript specials could not be rendered."), 0);
  }
}
