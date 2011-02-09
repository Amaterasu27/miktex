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

#include "common.h"

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
  MIKTEX_ASSERT (pChunkerThread.get() == 0);
  MIKTEX_ASSERT (gsErr.Get() == 0);
  MIKTEX_ASSERT (pStderrReaderThread.get() == 0);
  MIKTEX_ASSERT (gsIn.Get() == 0);
  MIKTEX_ASSERT (gsOut.Get() == 0);
  MIKTEX_ASSERT (m_pGhostscript.get() == 0);
}

/* _________________________________________________________________________

   Ghostscript::Start
   _________________________________________________________________________ */

void
Ghostscript::Start ()
{
  if (IsError())
  {
    return;
  }

  // find Ghostscript
  char szGsExe[BufferSizes::MaxPath];
  unsigned long version;
  SessionWrapper(true)->GetGhostscript (szGsExe, &version);

  // check to see whether the version number is ok
  if (((version >> 16) & 0xffff) < 6)
  {
    m_bError = true;
    FATAL_MIKTEX_ERROR ("Ghostscript::Start",
      T_("At least Ghostscript version 6.00 is required."), 0);
  }

  // first version that supports .locksafe
  unsigned long dwVersion_locksafe = 0x00070004; // 7.04

  // first version that supports -sstdout=%stderr
  unsigned long dwVersion_sstdout = 0x00070002; // 7.02

  //
  // build the command-line
  //
  string strCommand ("gs");
  // - set resolution, e.g. '-r600.0x600.0'
  char szTmp[200];
  double res =
    (static_cast<double>(pDviImpl->GetResolution())
     / shrinkFactor);
  sprintf (szTmp, " -r%fx%f", res, res);
  strCommand += szTmp;
  // - set paper size
  PaperSizeInfo paperSizeInfo = pDviImpl->GetPaperSizeInfo();
  int width = paperSizeInfo.width;
  int height = paperSizeInfo.height;
  sprintf (szTmp, " -g%dx%d", width, height);
  strCommand += szTmp;
  // - set the output device
  strCommand += " -sDEVICE=bmpmono";
  // - set batch flags
  strCommand += " -q -dBATCH -dNOPAUSE";
  // - set security options
  if (version < dwVersion_locksafe)
  {
    strCommand += " -dSAFER";
  }
  else
  {
    strCommand += " -dDELAYSAFER";
  }
  // - redirect PostScript %stdout
  if (version >= dwVersion_sstdout)
  {
    strCommand += " -sstdout=%stderr";
  }
  // - anti-aliasing
  strCommand += " -dTextAlphaBits=4";
  strCommand += " -dGraphicsAlphaBits=4";
  // - image interpolation
  strCommand += " -dDOINTERPOLATE";
  // - direct bitmap output to stdout
  strCommand += " -sOutputFile=-";
  // - read from stdin
  strCommand += " -";
  tracePS->WriteFormattedLine ("libdvi", "%s", strCommand.c_str());

  ProcessStartInfo startinfo;

  startinfo.FileName = szGsExe;
  startinfo.Arguments = strCommand;
  startinfo.StandardInput = 0;
  startinfo.RedirectStandardInput = true;
  startinfo.RedirectStandardOutput = true;
  startinfo.RedirectStandardError = true;

  try
  {
    m_pGhostscript.reset (Process::Start(startinfo));
    gsIn.Attach (m_pGhostscript->get_StandardInput());
    gsOut.Attach (m_pGhostscript->get_StandardOutput());
    gsErr.Attach (m_pGhostscript->get_StandardError());
  }
  catch (exception &)
  {
    m_bError = true;
    FATAL_MIKTEX_ERROR ("Ghostscript::Start", T_("Cannot start Ghostscript."), 0);
  }

  // start chunker thread
  pChunkerThread.reset (Thread::Start(Chunker, this));

  // start stderr reader
  pStderrReaderThread.reset (Thread::Start(StderrReader, this));

  // enable file reading
  if (version >= dwVersion_locksafe)
  {
    Execute ("%s", l_szPermitFileReading);
  }
}

/* _________________________________________________________________________

   Ghostscript::Read
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
  BitmapFile bmf;

  // create a BMP file
  Utils::CopyString (bmf.szBitmapFileName,
		     BufferSizes::MaxPath,
		     PathName().SetToTempFile().Get());
  tracePS->WriteFormattedLine ("libdvi", T_("creating bitmap file %s"), Q_(bmf.szBitmapFileName));
  FILE * pfile;
#if _MSC_VER >= 1400
  if (fopen_s(&pfile, bmf.szBitmapFileName, "wb") != 0)
  {
    pfile = 0;
  }
#else
  pfile = fopen(bmf.szBitmapFileName, "wb");
#endif
  if (pfile == 0)
  {
    FATAL_CRT_ERROR ("fopen", bmf.szBitmapFileName);
  }

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
  fwrite (&header, 1, sizeof(header), pfile);

  // dump bitmap info header
  fwrite (&pBitmapInfo->bmiHeader, 1, sizeof(BITMAPINFOHEADER), pfile);

  // dump color table
  fwrite (pChunk->GetColors(), 1, uNumColors * sizeof(RGBQUAD), pfile);

  // dump bits
  fwrite (pChunk->GetBits(), 1,
	  nBytesPerLine * pBitmapInfo->bmiHeader.biHeight,
	  pfile);

  fclose (pfile);

  bmf.x = pChunk->GetX();
  bmf.y = pChunk->GetY();
  bmf.cx = pBitmapInfo->bmiHeader.biWidth;
  bmf.cy = pBitmapInfo->bmiHeader.biHeight;

  m_vecBitmapFiles.push_back (bmf);
}

/* _________________________________________________________________________

   Ghostscript::Chunker
   _________________________________________________________________________ */

void
Ghostscript::Chunker (/*[in]*/ void * pParam)
{
  Ghostscript * This = reinterpret_cast<Ghostscript*>(pParam);
  auto_ptr<DibChunker> pChunker (DibChunker::Create());
  This->m_vecBitmapFiles.clear ();
  try
  {
    const int chunkSize = 2 * 1024 * 1024;
    while (pChunker->Process(DibChunker::Default, chunkSize, This))
    {
    }
  }
  catch (const exception &)
  {
    This->m_bError = true;
    This->gsOut.Close ();
    throw;
  }
  pChunker.reset (0);
}

/* _________________________________________________________________________

   Ghostscript::StderrReader
   _________________________________________________________________________ */

void
Ghostscript::StderrReader (/*[in]*/ void * pParam)
{
  Ghostscript * This = reinterpret_cast<Ghostscript*>(pParam);
  const int chunkSize = 64;
  char buf[ chunkSize ];
  This->m_strStderr = "";
  size_t n;
  while ((n = This->gsErr.Read(buf, chunkSize)) > 0)
  {
    This->m_strStderr.append (buf, n);
  }
}

/* _________________________________________________________________________

   Ghostscript::Write
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
  if (m_pGhostscript.get() == 0)
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
  int iExitCode = -1;
  if (m_pGhostscript.get() != 0)
  {
    if (m_pGhostscript->WaitForExit(10000))
    {
      iExitCode = m_pGhostscript->get_ExitCode();
    }
    m_pGhostscript.reset ();
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

  if (! m_strStderr.empty())
  {
    tracePS->WriteFormattedLine ("libdvi", T_("Ghostscript transcript follows:\n\
==========================================================================\n\
%s\n\
=========================================================================="),
	  m_strStderr.c_str());
  }

  if (gsOut.Get() != 0)
  {
    gsOut.Close ();
  }

  if (gsErr.Get() != 0)
  {
    gsErr.Close ();
  }

  PostScript::Finalize ();

  if (iExitCode != 0)
  {
    m_bError = true;
    FATAL_MIKTEX_ERROR ("Ghostscript::Finalize",
      T_("Some PostScript specials ")
      T_("could not be rendered.\r\n\r\n")
      T_("Do you want to see the transcript ")
      T_("of the PostScript interpreter?"),
      0);
  }

  if (IsError())
  {
    for (vector<BitmapFile>::const_iterator it = m_vecBitmapFiles.begin();
      it != m_vecBitmapFiles.end();
      ++ it)
    {
      File::Delete (it->szBitmapFileName);
    }
    m_vecBitmapFiles.clear ();
  }
}
