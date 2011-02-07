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

#include "yap.h"

#include "DviDoc.h"
#include "Ghostscript.h"
#include "yaphelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
  // make file reading 'wide-open'
  LPCTSTR const l_szPermitFileReading = _T("\
{ << /PermitFileReading [ (*) ] /PermitFileWriting [ ] PermitFileControl [ ]\
 >> setuserparams .locksafe } stopped pop\
");
}

/* _________________________________________________________________________

   CGhostscript::CGhostscript
   _________________________________________________________________________ */

CGhostscript::CGhostscript ()
  : m_hChunker (0),
    m_pStderr (0),
    m_hStderrReader (0),
    m_pStdin (0),
    m_pStdout (0)
{
}

/* _________________________________________________________________________

   CGhostscript::~CGhostscript
   _________________________________________________________________________ */

CGhostscript::~CGhostscript ()
{
  ASSERT (m_hChunker == 0);
  ASSERT (m_pStderr == 0);
  ASSERT (m_hStderrReader == 0);
  ASSERT (m_pStdin == 0);
  ASSERT (m_pStdout == 0);
  ASSERT (m_pGhostscript.get() == 0);
}

/* _________________________________________________________________________

   CGhostscript::Start
   _________________________________________________________________________ */

bool
CGhostscript::Start ()
{
  if (IsError())
    {
      return (false);
    }

  // find Ghostscript
  _TCHAR szGsExe[_MAX_PATH];
  DWORD version;
  SessionWrapper(true)->GetGhostscript (szGsExe, &version);

  // check to see whether the version number is ok
  if (HIWORD(version) < 6)
    {
      YapErrorHelp (IDP_GS_OUTDATED);
      m_bError = true;
      return (false);
    }

  // first version that supports .locksafe
  DWORD dwVersion_locksafe = MAKELONG(4, 7); // 7.04

  // first version that supports -sstdout=%stderr
  DWORD dwVersion_sstdout = MAKELONG(2, 7); // 7.02

  //
  // build the command-line
  //
  CString strCommand (_T("gs"));
  // - set resolution, e.g. '-r600.0x600.0'
  CString strTmp;
  double res =
    (static_cast<double>(m_pDviDoc->GetResolution())
     / m_pDviDoc->GetShrinkFactor());
  strTmp.Format (_T(" -r%fx%f"), res, res);
  strCommand += strTmp;
  // - set paper size
  CSize size = m_pDviDoc->GetPaperSize();
  strTmp.Format (_T(" -g%dx%d"), size.cx, size.cy);
  strCommand += strTmp;
  // - set the output device, e.g. '-sDEVICE=bmp16m'
  switch (g_pYapConfig->m_nPostScriptQuality)
    {
    case CYapConfig::Monochrome:
      strCommand += _T(" -sDEVICE=bmpmono");
      break;
    case CYapConfig::Color256:
      strCommand += _T(" -sDEVICE=bmp256");
      break;
    default:
    case CYapConfig::Color16M:
      strCommand += _T(" -sDEVICE=bmp16m");
      break;
    }
  // - set batch flags
  strCommand += _T(" -q -dBATCH -dNOPAUSE");
  // - set security options
  if (version < dwVersion_locksafe)
    {
      strCommand += _T(" -dSAFER");
    }
  else
    {
      strCommand += _T(" -dDELAYSAFER");
    }
  // - redirect PostScript %stdout
  if (version >= dwVersion_sstdout)
    {
      strCommand += _T(" -sstdout=%stderr");
    }
  if (g_pYapConfig->m_nPostScriptQuality == CYapConfig::Color16M)
    {
      // - anti-aliasing
      strCommand += _T(" -dTextAlphaBits=4");
      strCommand += _T(" -dGraphicsAlphaBits=4");
      // - image interpolation
      strCommand += _T(" -dDOINTERPOLATE");
    }
  // - direct bitmap output to stdout
  strCommand += _T(" -sOutputFile=-");
  // - read from stdin
  strCommand += _T(" -");
  YapLog (_T("%s"), static_cast<LPCTSTR>(strCommand));

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
      YapErrorHelp (IDP_CANNOT_START_GHOSTSCRIPT);
      m_bError = true;
      return (false);
    }

  // start chunker thread
  CWinThread * pThread =
    AfxBeginThread(Chunker,
		   this,
		   THREAD_PRIORITY_NORMAL,
		   0,
		   CREATE_SUSPENDED);
  ASSERT (pThread != 0);
  VERIFY (DuplicateHandle(GetCurrentProcess(),
			  pThread->m_hThread,
			  GetCurrentProcess(),
			  &m_hChunker,
			  0,
			  FALSE,
			  DUPLICATE_SAME_ACCESS));
  pThread->ResumeThread ();

  // start stderr reader
  pThread =
    AfxBeginThread(StderrReader,
		   this,
		   THREAD_PRIORITY_NORMAL,
		   0,
		   CREATE_SUSPENDED);
  ASSERT (pThread != 0);
  VERIFY (DuplicateHandle(GetCurrentProcess(),
			  pThread->m_hThread,
			  GetCurrentProcess(),
			  &m_hStderrReader,
			  0,
			  FALSE,
			  DUPLICATE_SAME_ACCESS));
  pThread->ResumeThread ();

  // enable file reading
  if (version >= dwVersion_locksafe)
    {
      if (! Execute(_T("%s"), l_szPermitFileReading))
	{
	  return (false);
	}
    }
			  
  return (true);
}

/* _________________________________________________________________________

   CGhostscript::OnNewChunk

   Process bitmap chunks.
   _________________________________________________________________________ */

bool __stdcall
CGhostscript::OnNewChunk (/*[in]*/ LPVOID			pv,
			  /*[in]*/ const CDibChunker::CHUNK *	pChunk)
{
  CGhostscript * This = reinterpret_cast<CGhostscript*>(pv);

  BitmapFile bmf;

  // create a BMP file
  Utils::CopyString (bmf.szBitmapFileName,
		     BufferSizes::MaxPath,
		     PathName().SetToTempFile().Get());
  YapLog (_T("creating bitmap file %s"), bmf.szBitmapFileName);
  FILE * pfile;
#if _MSC_VER >= 1400
  if (_tfopen_s(&pfile, bmf.szBitmapFileName, _T("wb")) != 0)
  {
    pfile = 0;
  }
#else
  pfile = _tfopen(bmf.szBitmapFileName, _T("wb"));
#endif
  if (pfile == 0)
    {
      throw (_T("A temporary bitmap file could not be opened"));
    }

  size_t nBytesPerLine =
    ((((pChunk->lpBitmapInfoHeader->biWidth
	* pChunk->lpBitmapInfoHeader->biBitCount)
       + 31)
      & ~31)
     >> 3);

  unsigned uNumColors =
    (pChunk->lpBitmapInfoHeader->biBitCount == 24
     ? 0
     : 1 << pChunk->lpBitmapInfoHeader->biBitCount);

  // dump bitmap file header
  BITMAPFILEHEADER header;
  header.bfType = 0x4d42;
  header.bfSize =
    (sizeof(header)
     + sizeof(BITMAPINFOHEADER)
     + uNumColors * sizeof(RGBQUAD)
     + (static_cast<DWORD>(nBytesPerLine)
	* pChunk->lpBitmapInfoHeader->biHeight));
  header.bfReserved1 = 0;
  header.bfReserved2 = 0;
  header.bfOffBits =
    (sizeof(header)
     + sizeof(BITMAPINFOHEADER)
     + uNumColors * sizeof(RGBQUAD));
  fwrite (&header, 1, sizeof(header), pfile);

  // dump bitmap info header
  fwrite (pChunk->lpBitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), pfile);

  // dump color table
  fwrite (pChunk->lpColors, 1, uNumColors * sizeof(RGBQUAD), pfile);

  // dump bits
  fwrite (pChunk->lpBits, 1,
	  nBytesPerLine * pChunk->lpBitmapInfoHeader->biHeight,
	  pfile);

  fclose (pfile);

  bmf.x = pChunk->x;
  bmf.y = pChunk->y;
  bmf.cx = pChunk->lpBitmapInfoHeader->biWidth;
  bmf.cy = pChunk->lpBitmapInfoHeader->biHeight;

  This->m_vecBitmapFiles.push_back (bmf);

  return (true);
}

/* _________________________________________________________________________

   CGhostscript::Chunker
   _________________________________________________________________________ */

UINT
CGhostscript::Chunker (/*[in]*/ LPVOID pParam)
{
  CGhostscript * This = reinterpret_cast<CGhostscript*>(pParam);
  CDibChunker * pChunker = CDibChunker::Create();
  UINT ret = 0;
  This->m_vecBitmapFiles.clear ();
  try
    {
      const int CHUNK_SIZE = 2 * 1024 * 1024;
      pChunker->Process (This->m_pStdout,
			 CDibChunker::Default,
			 CHUNK_SIZE,
			 This,
			 &OnNewChunk);
    }
  catch (LPCTSTR	lpszMessage)
    {
      YapLog (_T("%s"), lpszMessage);
      This->m_bError = true;
      ret = 1;
      fclose (This->m_pStdout);
      This->m_pStdout = 0;
    }
  CDibChunker::Destroy (pChunker);
  return (ret);
}

/* _________________________________________________________________________

   CGhostscript::StderrReader
   _________________________________________________________________________ */

UINT
CGhostscript::StderrReader (/*[in]*/ LPVOID pParam)
{
  CGhostscript * This = reinterpret_cast<CGhostscript*>(pParam);
#define CHUNK_SIZE 64
  _TCHAR buf[ CHUNK_SIZE ];
  This->m_strStderr = _T("");
  size_t n;
  while ((n = fread(buf, 1, CHUNK_SIZE, This->m_pStderr)) > 0)
    {
      for (size_t i = 0; i < n; ++ i)
	{
	  if (buf[i] == _T('\n'))
	    {
	      This->m_strStderr += _T("\r\n");
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
      YapLog (_T("ReadFile failed on Ghostscript %%stderr"));
    }
  return (0);
}

/* _________________________________________________________________________

   CGhostscript::Write
   _________________________________________________________________________ */

bool
CGhostscript::Write (/*[in]*/ const void *	p,
		     /*[in]*/ unsigned		n)
{
  size_t nwritten = fwrite(p, 1, n, m_pStdin);
  int err = ferror(m_pStdin);
  if (n != nwritten || (err != 0 && err != EPIPE))
    {
      YapLog (_T("Ghostscript finished prematurely"));
      return (false);
    }
  return (true);
}

/* _________________________________________________________________________

   CGhostscript::Execute
   _________________________________________________________________________ */

bool
CGhostscript::Execute (/*[in]*/ LPCTSTR	lpszFormat,
		       /*[in]*/		...)
{
  if (m_pGhostscript.get() == 0)
    {
      Start ();
    }
  if (m_pGhostscript.get() == 0)
    {
      return (false);
    }
  va_list argptr;
  va_start (argptr, lpszFormat);
  CString str;
  str.FormatV (lpszFormat, argptr);
  va_end (argptr);
  return (Write(static_cast<LPCTSTR>(str), str.GetLength()));
}

/* _________________________________________________________________________

   CGhostscript::Finalize
   _________________________________________________________________________ */

bool
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
  if (m_hChunker != 0)
    {
      WaitForSingleObject (m_hChunker, INFINITE);
      CloseHandle (m_hChunker);
      m_hChunker = 0;
    }

  // wait for stderr reader to finish
  if (m_hStderrReader != 0)
    {
      WaitForSingleObject (m_hStderrReader, INFINITE);
      CloseHandle (m_hStderrReader);
      m_hStderrReader = 0;
    }

  if (m_strStderr.GetLength() > 0)
    {
      YapLog (_T("Ghostscript transcript follows:\r\n\
==========================================================================\r\n\
%s\r\n\
=========================================================================="),
	      static_cast<LPCTSTR>(m_strStderr));
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
      if (AfxMessageBox(_T("Some PostScript specials ")
			_T("could not be rendered.\r\n\r\n")
			_T("Do you want to see the transcript ")
			_T("of the PostScript interpreter?"),
			MB_YESNO | MB_ICONSTOP)
	  == IDYES)
	{
	  TextViewerDialog::DoModal (0,
				   _T("PostScript Interpreter Transcript"),
				   m_strStderr);
	}
    }

  if (IsError())
    {
      for (vector<BitmapFile>::const_iterator it = m_vecBitmapFiles.begin();
	   it != m_vecBitmapFiles.end();
	   ++ it)
	{
	  DeleteFile (it->szBitmapFileName);
	}
      m_vecBitmapFiles.clear ();
    }

  return (true);
}
