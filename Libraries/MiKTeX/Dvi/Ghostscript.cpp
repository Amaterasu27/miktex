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

#include "Ghostscript.h"

namespace {
  // make file reading 'wide-open'
  const char * const l_szPermitFileReading = "\
{ << /PermitFileReading [ (*) ] /PermitFileWriting [ ] PermitFileControl [ ]\
 >> setuserparams .locksafe } stopped pop\
";
}

/* _________________________________________________________________________

   CGhostscript::CGhostscript
   _________________________________________________________________________ */

CGhostscript::CGhostscript ()
  : m_pStderr (0),
    m_pStdin (0),
    m_pStdout (0)
{
}

/* _________________________________________________________________________

   CGhostscript::~CGhostscript
   _________________________________________________________________________ */

CGhostscript::~CGhostscript ()
{
  MIKTEX_ASSERT (pChunkerThread.get() == 0);
  MIKTEX_ASSERT (m_pStderr == 0);
  MIKTEX_ASSERT (pStderrReaderThread.get() == 0);
  MIKTEX_ASSERT (m_pStdin == 0);
  MIKTEX_ASSERT (m_pStdout == 0);
  MIKTEX_ASSERT (m_pGhostscript.get() == 0);
}

/* _________________________________________________________________________

   CGhostscript::Start
   _________________________________________________________________________ */

void
CGhostscript::Start ()
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
  if (HIWORD(version) < 6)
  {
    m_bError = true;
    FATAL_MIKTEX_ERROR ("CGhostscript::Start",
      T_("At least Ghostscript version 6.0 is required."), 0);
  }

  // first version that supports .locksafe
  unsigned long dwVersion_locksafe = MAKELONG(4, 7); // 7.04

  // first version that supports -sstdout=%stderr
  unsigned long dwVersion_sstdout = MAKELONG(2, 7); // 7.02

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
    m_pGhostscript = auto_ptr<Process>(Process::Start(startinfo));
    m_pStdin = m_pGhostscript->get_StandardInput();
    m_pStdout = m_pGhostscript->get_StandardOutput();
    m_pStderr = m_pGhostscript->get_StandardError();
  }
  catch (exception &)
  {
    m_bError = true;
    FATAL_MIKTEX_ERROR ("CGhostscript::Start", T_("Cannot start Ghostscript."), 0);
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

   CGhostscript::Read
   _________________________________________________________________________ */

size_t
CGhostscript::Read (/*[out]*/ void *	pBuf,
		   /*[in]*/ size_t	size)
{
  size_t bytesRead = fread(pBuf, 1, size, m_pStdout);
  return (bytesRead);
}

/* _________________________________________________________________________

   CGhostscript::OnNewChunk

   Process bitmap chunks.
   _________________________________________________________________________ */

void
CGhostscript::OnNewChunk (/*[in]*/ DibChunk *	pChunk)
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

   CGhostscript::Chunker
   _________________________________________________________________________ */

void
CGhostscript::Chunker (/*[in]*/ void * pParam)
{
  CGhostscript * This = reinterpret_cast<CGhostscript*>(pParam);
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
    fclose (This->m_pStdout);
    This->m_pStdout = 0;
    throw;
  }
  pChunker.reset (0);
}

/* _________________________________________________________________________

   CGhostscript::StderrReader
   _________________________________________________________________________ */

void
CGhostscript::StderrReader (/*[in]*/ void * pParam)
{
  CGhostscript * This = reinterpret_cast<CGhostscript*>(pParam);
#define CHUNK_SIZE 64
  char buf[ CHUNK_SIZE ];
  This->m_strStderr = "";
  size_t n;
  while ((n = fread(buf, 1, CHUNK_SIZE, This->m_pStderr)) > 0)
  {
    for (size_t i = 0; i < n; ++ i)
    {
      if (buf[i] == '\n')
      {
	This->m_strStderr += "\r\n";
      }
      else
      {
	This->m_strStderr += buf[i];
      }
    }
  }
  int err = ferror(This->m_pStderr);
  if (err != 0 && err != EPIPE)
  {
    This->tracePS->WriteFormattedLine ("libdvi", T_("ReadFile failed on Ghostscript %%stderr"));
  }
}

/* _________________________________________________________________________

   CGhostscript::Write
   _________________________________________________________________________ */

void
CGhostscript::Write (/*[in]*/ const void *	p,
		     /*[in]*/ unsigned		n)
{
  size_t nwritten = fwrite(p, 1, n, m_pStdin);
  int err = ferror(m_pStdin);
  if (n != nwritten || (err != 0 && err != EPIPE))
  {
    tracePS->WriteFormattedLine ("libdvi", T_("Ghostscript finished prematurely"));
    FATAL_MIKTEX_ERROR ("CGhostscript::Write",
      T_("Ghostscript finished prematurely."), 0);
  }
}

/* _________________________________________________________________________

   CGhostscript::Execute
   _________________________________________________________________________ */

void
CGhostscript::Execute (/*[in]*/ const char *	lpszFormat,
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

   CGhostscript::Finalize
   _________________________________________________________________________ */

void
CGhostscript::Finalize ()
{
  // close Ghostscript's input stream
  if (m_pStdin != 0)
    {
      fclose (m_pStdin);
      m_pStdin = 0;
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
    pChunkerThread.reset (0);
  }

  // wait for stderr reader to finish
  if (pStderrReaderThread.get() != 0)
  {
    pStderrReaderThread->Join ();
    pStderrReaderThread.reset (0);
  }

  if (! m_strStderr.empty())
  {
    tracePS->WriteFormattedLine ("libdvi", T_("Ghostscript transcript follows:\r\n\
==========================================================================\r\n\
%s\r\n\
=========================================================================="),
	  m_strStderr.c_str());
  }

  if (m_pStdout != 0)
  {
    fclose (m_pStdout);
    m_pStdout = 0;
  }

  if (m_pStderr != 0)
  {
    fclose (m_pStderr);
    m_pStderr = 0;
  }

  CPostScript::Finalize ();

  if (iExitCode != 0)
  {
    m_bError = true;
    FATAL_MIKTEX_ERROR ("CGhostscript::Finalize",
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
