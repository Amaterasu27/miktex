/* PostScript.cpp:

   Copyright (C) 1996-2004 Christian Schenk

   This file is part of Yap.

   Yap is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   Yap is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with Yap; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include "StdAfx.h"

#include "yap.h"

#include "DviDoc.h"
#include "PostScript.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

/* _________________________________________________________________________

   CPostScript::CPostScript
   _________________________________________________________________________ */

CPostScript::CPostScript ()
  : m_bError (false),
    m_bOpen (false),
    m_bPageBegun (false),
    m_pDviDoc (0)
{
}

/* _________________________________________________________________________

   CPostScript::~CPostScript
   _________________________________________________________________________ */

CPostScript::~CPostScript ()
{
  Clear ();
}

/* _________________________________________________________________________

   CPostScript::Initialize
   _________________________________________________________________________ */

bool
CPostScript::Initialize ()
{
  AddHeader (_T("tex.pro"));
#if 0
  AddHeader (_T("finclude.pro"));
#endif
  AddHeader (_T("special.pro"));
  return (true);
}

/* _________________________________________________________________________

   CPostScript::IsEnabled
   _________________________________________________________________________ */

bool
CPostScript::IsEnabled ()
{
  return (! g_pYapConfig->m_bIgnorePostScriptSpecials);
}

/* _________________________________________________________________________

   CPostScript::ExecuteBatch

   Process a PostScript file. This is done with the help of 'run',
   which seems to be a Ghostscript extension.
   
   <fixme>This should be a virtual function.</fixme>
   _________________________________________________________________________ */

bool
CPostScript::ExecuteBatch (/*[in]*/ LPCTSTR lpszFileName)
{
  tstring strCommand;
  strCommand.reserve (_MAX_PATH + 10);
  strCommand = _T('(');
  PathName pathUnx (lpszFileName);
  pathUnx.Convert (ConvertPathNameFlags(ConvertPathNameFlags::ToUnix)
		   | ConvertPathNameFlags::MakeAbsolute);
  strCommand += pathUnx.Get();
  strCommand += _T(") run\n");
  return (Execute(strCommand.c_str()));
}

/* _________________________________________________________________________

   AllowShellCommand
   _________________________________________________________________________ */

bool
AllowShellCommand (/*[in]*/ const CString & strCommand)
{
  switch (g_pYapConfig->m_nEnableCommands)
    {
    default:
    case CYapConfig::SEC_DISABLE_COMMANDS:
      return (false);
    case CYapConfig::SEC_ASK_USER:
      {
	CString message;
	// <fixme>hard-coded string</fixme>
	message.Format ((_T("The following command is embedded in the ")
			 _T("DVI file:\n\n%s\n\n")
			 _T("Do you allow to execute this command?")),
			static_cast<LPCTSTR>(strCommand));
	return (AfxMessageBox(message, MB_YESNO | MB_ICONQUESTION) == IDYES);
      }
    case CYapConfig::SEC_SECURE_COMMANDS:
      {
	CString strName = strCommand.SpanExcluding(_T(" "));
	for (Tokenizer tok (g_pYapConfig->m_strSecureCommands, _T(" ,;"));
	     tok.GetCurrent() != 0;
	     ++ tok)
	  {
	    if (PathName::Compare(tok.GetCurrent(), strName) == 0)
	      {
		return (true);
	      }
	  }
	return (false);
      }
    case CYapConfig::SEC_ENABLE_ALL_COMMANDS:
      return (true);
    }
}

/* _________________________________________________________________________

   CPostScript::CopyFile
   _________________________________________________________________________ */

bool
CPostScript::CopyFile (/*[in]*/ FILE *		pfile,
		       /*[in]*/ unsigned	length)
{
  size_t n;
  char buf[4096];
  if (length == 0)
    {
      length = UINT_MAX;
    }
  while ((n = fread(buf, 1, min(4096, length), pfile)) > 0)
    {
      if (! Write(buf, static_cast<unsigned>(n)))
	{
	  return (false);
	}
      length -= static_cast<unsigned>(n);
    }

  return (true);
}

/* _________________________________________________________________________

   CPostScript::ExecuteEncapsulatedPostScript

   Load an .eps file.
   _________________________________________________________________________ */

bool
CPostScript::ExecuteEncapsulatedPostScript (/*[in]*/ LPCTSTR lpszFileName)
{
  if (m_bError)
    {
      return (false);
    }

#if 0 // <todo/>
  StatusBarMessage (_T("Loading PostScript file \"%s\"..."), lpszFileName);
#endif

  FILE * pfile = 0;

  unsigned length = 0;
  unsigned start = 0;

  if (bmeps_can_handle(const_cast<LPTSTR>(lpszFileName)) != 0)
    {
      YapLog (_T("Trying to convert \"%s\"..."), lpszFileName);
      pfile = ConvertToEPS(lpszFileName);
      if (pfile == 0)
	{
	  YapError (_T("Cannot convert \"%1\" to EPS."), lpszFileName);
	  m_bError = true;
	  return (false);
	}	  
    }
  else
    {
#if _MSC_VER >= 1400
      if (_tfopen_s(&pfile, lpszFileName, _T("rb")) != 0)
      {
	pfile = 0;
      }
#else
      pfile = _tfopen(lpszFileName, _T("rb"));
#endif
      if (pfile == 0)
	{
	  YapError (_T("Cannot open PostScript file \"%1\"."), lpszFileName);
	  m_bError = true;
	  return (false);
	}
      struct
      {
	unsigned char magic[4];
	unsigned char start[4];
	unsigned char length[4];
      } epsfheader;
      if (fread(&epsfheader, sizeof(epsfheader), 1, pfile) == 1
	  && epsfheader.magic[0] == _T('E') + 0x80
	  && epsfheader.magic[1] == _T('P') + 0x80
	  && epsfheader.magic[2] == _T('S') + 0x80
	  && epsfheader.magic[3] == _T('F') + 0x80)
	{
	  start = epsfheader.start[0];
	  start += epsfheader.start[1] * 256;
	  start += epsfheader.start[2] * 256 * 256;
	  start += epsfheader.start[3] * 256 * 256 * 256;
	  length = epsfheader.length[0];
	  length += epsfheader.length[1] * 256;
	  length += epsfheader.length[2] * 256 * 256;
	  length += epsfheader.length[3] * 256 * 256 * 256;
	  YapLog (_T("EPS has a binary header"));
	  YapLog (_T("start: %u"), static_cast<unsigned>(start));
	  YapLog (_T("length: %u"), static_cast<unsigned>(length));
	}
    }

  autofclose xxx (pfile);

  if (fseek(pfile, start, SEEK_SET) != 0)
    {
      YapLog (_T("fseek() failed for some reason"));
      fclose (pfile);
    }

  return (CopyFile(pfile, length));
}

/* _________________________________________________________________________

   ConvertToEPSThread
   _________________________________________________________________________ */

struct ThreadArg
{
  FILE *	filein;
  FILE *	fileout;
  PathName		pathFile;
};

unsigned __stdcall
CPostScript::ConvertToEPSThread (/*[in]*/ LPVOID pv)
{
  assert (pv != 0);
  ThreadArg * parg = reinterpret_cast<ThreadArg*>(pv);
  assert (parg->filein != 0 && parg->fileout != 0);
  bmeps_cfg (_T("c"));
  int rc = bmeps_run(parg->fileout, parg->filein, parg->pathFile.GetBuffer());
  if (rc == 0)
    {
      YapLog (_T("bmeps_run() failed for some reason"));
    }
  fclose (parg->filein);
  fclose (parg->fileout);
  delete parg;
  return (0);
}

/* _________________________________________________________________________
   
   ConvertToEPS
   _________________________________________________________________________ */

FILE *
CPostScript::ConvertToEPS (/*[in]*/ LPCTSTR lpszFileName)
{
  int aHandles[2];
  if (_pipe(aHandles, 4096, _O_BINARY) != 0)
    {
      return (0);
    }
  FILE * pFilePipeRead = _tfdopen(aHandles[0], _T("rb"));
  if (pFilePipeRead == 0)
    {
      _close (aHandles[0]);
      return (0);
    }
  FILE * pFilePipeWrite = _tfdopen(aHandles[1], _T("wb"));
  if (pFilePipeWrite == 0)
    {
      fclose (pFilePipeRead);
      _close (aHandles[1]);
      return (0);
    }
  FILE * pFileIn;
#if _MSC_VER >= 1400
  if (_tfopen_s(&pFileIn, lpszFileName, _T("rb")) != 0)
  {
    pFileIn = 0;
  }
#else
  pFileIn = _tfopen(lpszFileName, _T("rb"));
#endif
  if (pFileIn == 0)
    {
      fclose (pFilePipeRead);
      fclose (pFilePipeWrite);
      return (0);
    }
  ThreadArg * pthreadarg = new ThreadArg;
  pthreadarg->filein = pFileIn;
  pthreadarg->fileout = pFilePipeWrite;
  pthreadarg->pathFile = lpszFileName;
  unsigned threadID;
  HANDLE hThread =
    reinterpret_cast<HANDLE>
    (_beginthreadex(0, 0, ConvertToEPSThread, pthreadarg, 0, &threadID));
  if (hThread == 0)
    {
      fclose (pFilePipeRead);
      fclose (pFilePipeWrite);
      fclose (pFileIn);
      return (0);
    }
  CloseHandle (hThread);
  return (pFilePipeRead);
}

/* _________________________________________________________________________

   CPostScript::SendHeader

   Send a PostScript header file.
   _________________________________________________________________________ */

bool
CPostScript::SendHeader (/*[in]*/ LPCTSTR lpszHeaderName)
{
  if (m_bError)
    {
      return (false);
    }
  PathName fileName;
  if (! SessionWrapper(true)->FindFile(lpszHeaderName, FileType::PSHEADER, fileName))
    {
      YapError (_T("Cannot find PostScript header file \"%1\"."),
		lpszHeaderName);
      m_bError = true;
      return (false);
    }
  YapLog (_T("Sending \"%s\"..."), fileName.Get());
  bool bDone = ExecuteBatch(fileName.Get());
  if (! bDone)
    {
      if (! m_bError)
	{
	  YapError (_T("Cannot send PostScript header file \"%1\"."),
		    fileName.Get());
	  m_bError = true;
	}
    }
  return (bDone);
}

/* _________________________________________________________________________

   CPostScript::DoProlog

   Initialize the PostScript device: send headers & define macros.
   _________________________________________________________________________ */

bool
CPostScript::DoProlog ()
{
  bool bNoError = true;
  vector<string>::iterator it;
  for (it = m_setHeaders.begin(); bNoError && it != m_setHeaders.end(); ++ it)
    {
      bNoError = SendHeader(it->c_str());
    }
  if (! bNoError)
    {
      return (false);
    }
  return (DoDefinitions());
}

/* _________________________________________________________________________

   CPostScript::DoDefinitions
   _________________________________________________________________________ */

bool
CPostScript::DoDefinitions ()
{
  if (m_setDefinitions.empty())
    {
      return (true);
    }
  bool bNoError = true;
  if (! Execute(_T("\nTeXDict begin @defspecial\n")))
    {
      return (false);
    }
  vector<string>::iterator it;
  for (it = m_setDefinitions.begin();
       bNoError && it != m_setDefinitions.end();
       ++ it)
    {
      bNoError = Execute(_T("%s\n"), it->c_str());
    }
  if (! bNoError)
    {
      return (false);
    }
  return (Execute(_T("\n@fedspecial end\n")));
}

/* _________________________________________________________________________

   CPostScript::DoSpecial

   Send a psfile special.
   _________________________________________________________________________ */

bool
CPostScript::DoSpecial (/*[in]*/ const psfilespecial * ppsfilespecial)
{
  if (m_bError)
    {
      return (false);
    }
  PathName pathFileName;
  bool bFileExists =
    FindGraphicsFile(ppsfilespecial->getfilename(), pathFileName.GetBuffer());
  if (! bFileExists)
    {
      YapError (_T("Cannot find file \"%1\"."), ppsfilespecial->getfilename());
      m_bError = true;
      return (false);
    }

  bool bDone = true;

  bDone = Execute(_T("%d %d a\n"),
		  ppsfilespecial->getx() - m_pDviDoc->GetResolution(),
		  ppsfilespecial->gety() - m_pDviDoc->GetResolution());
  
  bDone = bDone && Execute(_T("@beginspecial\n"));
  
  if (bDone && ppsfilespecial->has_hsize())
    {
      bDone = Execute(_T("%d @hsize\n"), ppsfilespecial->gethsize());
    }
  
  if (bDone && ppsfilespecial->has_vsize())
    {
      bDone = Execute(_T("%d @vsize\n"), ppsfilespecial->getvsize());
    }
  
  if (bDone && ppsfilespecial->has_hoffset())
    {
      bDone = Execute(_T("%d @hoffset\n"), ppsfilespecial->gethoffset());
    }
  
  if (bDone && ppsfilespecial->has_voffset())
    {
      bDone = Execute(_T("%d @voffset\n"), ppsfilespecial->getvoffset());
    }
  
  if (bDone && ppsfilespecial->has_hscale())
    {
      bDone = Execute(_T("%d @hscale\n"), ppsfilespecial->gethscale());
    }
  
  if (bDone && ppsfilespecial->has_vscale())
    {
      bDone = Execute(_T("%d @vscale\n"), ppsfilespecial->getvscale());
    }
  
  if (bDone && ppsfilespecial->has_angle ())
    {
      bDone = Execute(_T("%d @angle\n"), ppsfilespecial->getangle());
    }
  
  if (bDone && ppsfilespecial->has_llx())
    {
      bDone = Execute(_T("%d @llx\n"), ppsfilespecial->getllx());
    }
  
  if (bDone && ppsfilespecial->has_lly())
    {
      bDone = Execute(_T("%d @lly\n"), ppsfilespecial->getlly());
    }
  
  if (bDone && ppsfilespecial->has_urx())
    {
      bDone = Execute(_T("%d @urx\n"), ppsfilespecial->geturx());
    }
  
  if (bDone && ppsfilespecial->has_ury())
    {
      bDone = Execute(_T("%d @ury\n"), ppsfilespecial->getury());
    }
  
  if (bDone && ppsfilespecial->has_rwi())
    {
      bDone = Execute(_T("%d @rwi\n"), ppsfilespecial->getrwi());
    }
  
  if (bDone && ppsfilespecial->has_rhi())
    {
      bDone = Execute(_T("%d @rhi\n"), ppsfilespecial->getrhi());
    }
  
  if (bDone && ppsfilespecial->has_clip())
    {
      bDone = Execute(_T("@clip\n"));
    }
  
  bDone = bDone && Execute(_T("@setspecial\n"));
  bDone = bDone && ExecuteEncapsulatedPostScript(pathFileName.Get());
  bDone = bDone && Execute(_T("@endspecial\n"));

  return (bDone);
}

/* _________________________________________________________________________

   CPostScript::AddDefinition

   Remember a psdef special for later.
   _________________________________________________________________________ */

void
CPostScript::AddDefinition (/*[in]*/
			    const psdefspecial * ppsdefspecial)
{
  if (ppsdefspecial->getdef())
    {
      if (find(m_setDefinitions.begin(), m_setDefinitions.end(),
	       ppsdefspecial->getdef())
	  == m_setDefinitions.end())
	{
	  m_setDefinitions.push_back (ppsdefspecial->getdef());
	}
    }
  else if (ppsdefspecial->getfilename())
    {
      AddHeader (ppsdefspecial->getfilename());
    }
}

/* _________________________________________________________________________

   CPostScript::AddHeader

   Remember a psheader file for later.
   _________________________________________________________________________ */

void
CPostScript::AddHeader (/*[in]*/ LPCTSTR lpszFileName)
{
  if (find(m_setHeaders.begin(), m_setHeaders.end(), lpszFileName)
      == m_setHeaders.end())
    {
      m_setHeaders.push_back (lpszFileName);
    }
}

/* _________________________________________________________________________

   CPostScript::DoSpecial

   Send a ps special.
   _________________________________________________________________________ */

bool
CPostScript::DoSpecial (/*[in]*/ const dvipsspecial * pdvipsspecial)
{
  if (m_bError)
    {
      return (false);
    }
  bool bDone =
    Execute(_T("%d %d a\n"),
	    pdvipsspecial->getx() - m_pDviDoc->GetResolution(),
	    pdvipsspecial->gety() - m_pDviDoc->GetResolution());
  if (pdvipsspecial->getprotection())
    {
      bDone = bDone && Execute(_T("@beginspecial\n"));
      bDone = bDone && Execute(_T("@setspecial\n"));
    }
  if (pdvipsspecial->getstring())
    {
      bDone = bDone && Execute(_T("%s\n"), pdvipsspecial->getstring());
    }
  else if (pdvipsspecial->getfilename())
    {
      _TCHAR filename[_MAX_PATH];
      if (! FindGraphicsFile(pdvipsspecial->getfilename(), filename))
	{
	  YapError (_T("Cannot find file \"%1\"."),
		    pdvipsspecial->getfilename());
	  m_bError = true;
	  bDone = false;
	}
      else
	{
	  bDone = bDone && ExecuteEncapsulatedPostScript(filename);
	}
    }
  if (pdvipsspecial->getprotection())
    {
      bDone = bDone && Execute(_T("@endspecial\n"));
    }
  if (! bDone)
    {
      return (false);
    }
  return (true);
}

/* _________________________________________________________________________

   pt2sp

   Convert PostScript point to scaled point.
   _________________________________________________________________________ */

inline
int
pt2sp (/*[in]*/ int pt)
{
  return (static_cast<int>((pt * (65536 * 72.27)) / 72));
}

/* _________________________________________________________________________

   CPostScript::Open

   Open the PostScript device.
   _________________________________________________________________________ */

bool
CPostScript::Open (/*[in]*/ CDviDoc * pDviDoc)
{
  m_pDviDoc = pDviDoc;

  int width, height;

  // calculate device dimensions
  PaperSizeInfo size;
  if (! Utils::GetPaperSize(-1, g_pYapConfig->m_strPaperSize, &size))
    {
      if (m_pDviDoc->IsPortrait())
	{
	  width = 39158280;
	  height = 55380996;
	}
      else
	{
	  height = 39158280;
	  width = 55380996;
	}
    }
  else
    {
      if (m_pDviDoc->IsPortrait())
	{
	  width = pt2sp(size.width);
	  height = pt2sp(size.height);
	}
      else
	{
	  height = pt2sp(size.width);
	  width = pt2sp(size.height);
	}
    }

  // initialize
  m_bOpen = (Initialize()
	     && DoProlog()
	     && Execute(_T("TeXDict begin\n"))
	     && Execute(_T("%d %d %d %d %d (test.dvi) @start\n"),
			width,
			height,
			m_pDviDoc->GetMagnification(),
			m_pDviDoc->GetResolution(),
			m_pDviDoc->GetResolution()));
  
  return (m_bOpen);
}

/* _________________________________________________________________________

   CPostScript::Close

   Close the PostScript device.
   _________________________________________________________________________ */

bool
CPostScript::Close ()

{
  ASSERT (! m_bPageBegun);
  Execute (_T("end\n"));
  Finalize();
  m_bOpen = false;
  return (true);
}

/* _________________________________________________________________________

   CPostScript::BeginPage

   Start a new page.
   _________________________________________________________________________ */

bool
CPostScript::BeginPage ()
{
  ASSERT (! m_bPageBegun);
  m_bPageBegun = Execute(_T("1 0 bop\n"));
  return (m_bPageBegun);
}

/* _________________________________________________________________________

   CPostScript::EndPage

   Close the current page.
   _________________________________________________________________________ */

bool
CPostScript::EndPage ()
{
  ASSERT (m_bPageBegun);
  bool ret = Execute(_T("eop\n"));
  m_bPageBegun = false;
  return (ret);
}

/* _________________________________________________________________________

   CPostScript::Clear
   _________________________________________________________________________ */

void
CPostScript::Clear ()
{
  m_setHeaders.clear ();
  m_setDefinitions.clear ();
}

/* _________________________________________________________________________

   CPostScript::Finalize
   _________________________________________________________________________ */

bool
CPostScript::Finalize ()
{
  return (true);
}

/* _________________________________________________________________________

   CPostScript::InternalFindGraphicsFile

   Find a graphics file.
   _________________________________________________________________________ */

bool
CPostScript::InternalFindGraphicsFile (/*[in]*/ LPCTSTR	lpszFileName,
				       /*[out]*/ LPTSTR	lpszResult)
{
  YapLog (_T("Searching file \"%s\"..."), lpszFileName);
  ASSERT (m_pDviDoc != 0);
  ASSERT (m_pDviDoc->GetPathName().GetLength() != 0);
  PathName path = m_pDviDoc->GetPathName();
  path.RemoveFileSpec ();
  path += lpszFileName;
  bool bFound = File::Exists(path);
  if (! bFound)
    {
      bFound = SessionWrapper(true)->FindFile(lpszFileName, FileType::GRAPHICS, path);
    }
  if (bFound)
    {
      Utils::CopyString (lpszResult, _MAX_PATH, path.Get());
      YapLog (_T("Found \"%s\""), lpszResult);
    }
  return (bFound);
}

/* _________________________________________________________________________

   CPostScript::Uncompress

   Uncompress a graphics file.
   _________________________________________________________________________ */

bool
CPostScript::Uncompress (/*[in]*/ LPCTSTR	lpszFileName,
			 /*[out]*/  LPTSTR	lpszTempFileName)
{
  if (m_bError)
    {
      return (false);
    }
  _TCHAR szPath[_MAX_PATH];
  if (! InternalFindGraphicsFile(lpszFileName, szPath))
    {
      YapError (_T("Cannot find file \"%1\"."), lpszFileName);
      m_bError = true;
      return (false);
    }
  PathName temp;
  Utils::UncompressFile (szPath, temp);
  Utils::CopyString (lpszTempFileName, _MAX_PATH, temp.Get());
  m_pDviDoc->GetTempFiles()[lpszFileName] = lpszTempFileName;
  return (true);
}

/* _________________________________________________________________________

   IsZFileName
   _________________________________________________________________________ */

bool
IsZFileName (/*[in]*/ const CString & strFileName)
{
  return (_tcsicmp(strFileName.Right(3), _T(".gz")) == 0
	  || _tcsicmp(strFileName.Right(4), _T(".bz2")) == 0);
}

/* _________________________________________________________________________

   CPostScript::FindGraphicsFile

   Find a graphics file.  Here we will handle backtics and compressed
   files.
   _________________________________________________________________________ */

bool
CPostScript::FindGraphicsFile (/*[in]*/ LPCTSTR	lpszFileName,
			       /*[out]*/ LPTSTR	lpszResult)
{
  if (*lpszFileName == _T('`'))
    {
      map<tstring, TempFile>::const_iterator it =
	m_pDviDoc->GetTempFiles().find(lpszFileName + 1);
      if (it != m_pDviDoc->GetTempFiles().end())
	{
	  Utils::CopyString (lpszResult, _MAX_PATH, it->second.Get());
	  return (true);
	}
      else if (_tcsncmp(lpszFileName + 1, _T("gunzip -c "), 10) == 0)
	{
	  return (Uncompress(lpszFileName + 11, lpszResult));
	}
      else if (_tcsncmp(lpszFileName + 1, _T("bunzip2 -c "), 11) == 0)
	{
	  return (Uncompress(lpszFileName + 12, lpszResult));
	}
      else if (! AllowShellCommand(lpszFileName + 1))
	{
	  YapLog (_T("Ignoring shell command \"%s\""), lpszFileName + 1);
	  return (false);
	}
      else
	{
	  _TCHAR szTempFileName[_MAX_PATH];
	  Utils::CopyString (szTempFileName,
			     _MAX_PATH,
			     PathName().SetToTempFile().Get());
	  CString strCommand;
	  strCommand.Format (_T("%s > \"%s\""),
			     lpszFileName + 1,
			     szTempFileName);
	  ASSERT (m_pDviDoc != 0);
	  CString strDocDir = m_pDviDoc->GetDocDir();
	  StdoutReader reader (0);
	  bool bDone =
	    Process::ExecuteSystemCommand(static_cast<LPCTSTR>(strCommand), 0,
			   &reader, strDocDir);
	  if (! bDone)
	    {
	      // <fixme>hard-coded string</fixme>
	      YapError (_T("Execution of an embedded shell ")
			_T("command failed for some reason!"));
	      DeleteFile (szTempFileName);
	      m_bError = true;
	      return (false);
	    }
	  m_pDviDoc->GetTempFiles()[lpszFileName + 1] = szTempFileName;
	  Utils::CopyString (lpszResult, _MAX_PATH, szTempFileName);
	  return (true);
	}
    }
  else if (IsZFileName(lpszFileName))
    {
      map<tstring, TempFile>::const_iterator it =
	m_pDviDoc->GetTempFiles().find(lpszFileName);
      if (it != m_pDviDoc->GetTempFiles().end())
	{
	  Utils::CopyString (lpszResult, _MAX_PATH, it->second.Get());
	  return (true);
	}
      return (Uncompress(lpszFileName, lpszResult));
    }
  else
    {
      return (InternalFindGraphicsFile(lpszFileName, lpszResult));
    }
}
