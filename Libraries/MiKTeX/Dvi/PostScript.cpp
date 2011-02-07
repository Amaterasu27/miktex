/* PostScript.cpp:

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

#include "PostScript.h"

/* _________________________________________________________________________

   CPostScript::CPostScript
   _________________________________________________________________________ */

CPostScript::CPostScript ()
  : m_bError (false),
    m_bOpen (false),
    m_bPageBegun (false),
    pDviImpl (0),
    tracePage (TraceStream::Open(MIKTEX_TRACE_DVIPAGE))	 // FIXME
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
  AddHeader ("tex.pro");
#if 0
  AddHeader ("finclude.pro");
#endif
  AddHeader ("special.pro");
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
CPostScript::ExecuteBatch (/*[in]*/ const char * lpszFileName)
{
  tstring strCommand;
  strCommand.reserve (_MAX_PATH + 10);
  strCommand = '(';
  PathName pathUnx (lpszFileName);
  pathUnx.Convert (ConvertPathNameFlags(ConvertPathNameFlags::ToUnix)
		   | ConvertPathNameFlags::MakeAbsolute);
  strCommand += pathUnx.Get();
  strCommand += ") run\n";
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
	message.Format ((T_("The following command is embedded in the ")
			 T_("DVI file:\n\n%s\n\n")
			 T_("Do you allow to execute this command?")),
			static_cast<const char *>(strCommand));
	return (AfxMessageBox(message, MB_YESNO | MB_ICONQUESTION) == IDYES);
      }
    case CYapConfig::SEC_SECURE_COMMANDS:
      {
	CString strName = strCommand.SpanExcluding(" ");
	for (Tokenizer tok (g_pYapConfig->m_strSecureCommands, " ,;");
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
CPostScript::ExecuteEncapsulatedPostScript (/*[in]*/ const char * lpszFileName)
{
  if (m_bError)
    {
      return (false);
    }

#if 0 // <todo/>
  StatusBarMessage (T_("Loading PostScript file %s..."), Q_(lpszFileName));
#endif

  FILE * pfile = 0;

  unsigned length = 0;
  unsigned start = 0;

  if (bmeps_can_handle(const_cast<char *>(lpszFileName)) != 0)
    {
      tracePS->WriteFormattedLine ("libdvi", T_("Trying to convert %s..."), Q_(lpszFileName));
      pfile = ConvertToEPS(lpszFileName);
      if (pfile == 0)
	{
	  m_bError = true;
	  YapError (T_("Cannot convert %1 to EPS."), Q_(lpszFileName));
	  return (false);
	}	  
    }
  else
    {
#if _MSC_VER >= 1400
      if (_tfopen_s(&pfile, lpszFileName, "rb") != 0)
      {
	pfile = 0;
      }
#else
      pfile = _tfopen(lpszFileName, "rb");
#endif
      if (pfile == 0)
	{
	  m_bError = true;
	  YapError (T_("Cannot open PostScript file %1."), Q_(lpszFileName));
	  return (false);
	}
      struct
      {
	unsigned char magic[4];
	unsigned char start[4];
	unsigned char length[4];
      } epsfheader;
      if (fread(&epsfheader, sizeof(epsfheader), 1, pfile) == 1
	  && epsfheader.magic[0] == 'E' + 0x80
	  && epsfheader.magic[1] == 'P' + 0x80
	  && epsfheader.magic[2] == 'S' + 0x80
	  && epsfheader.magic[3] == 'F' + 0x80)
	{
	  start = epsfheader.start[0];
	  start += epsfheader.start[1] * 256;
	  start += epsfheader.start[2] * 256 * 256;
	  start += epsfheader.start[3] * 256 * 256 * 256;
	  length = epsfheader.length[0];
	  length += epsfheader.length[1] * 256;
	  length += epsfheader.length[2] * 256 * 256;
	  length += epsfheader.length[3] * 256 * 256 * 256;
	  tracePS->WriteFormattedLine ("libdvi", T_("EPS has a binary header"));
	  tracePS->WriteFormattedLine ("libdvi", T_("start: %u"), static_cast<unsigned>(start));
	  tracePS->WriteFormattedLine ("libdvi", T_("length: %u"), static_cast<unsigned>(length));
	}
    }

  autofclose xxx (pfile);

  if (fseek(pfile, start, SEEK_SET) != 0)
    {
      tracePS->WriteFormattedLine ("libdvi", T_("fseek() failed for some reason"));
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
  bmeps_cfg ("c");
  int rc = bmeps_run(parg->fileout, parg->filein, parg->pathFile.GetBuffer());
  if (rc == 0)
    {
      tracePS->WriteFormattedLine ("libdvi", T_("bmeps_run() failed for some reason"));
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
CPostScript::ConvertToEPS (/*[in]*/ const char * lpszFileName)
{
  int aHandles[2];
  if (_pipe(aHandles, 4096, _O_BINARY) != 0)
    {
      return (0);
    }
  FILE * pFilePipeRead = _tfdopen(aHandles[0], "rb");
  if (pFilePipeRead == 0)
    {
      _close (aHandles[0]);
      return (0);
    }
  FILE * pFilePipeWrite = _tfdopen(aHandles[1], "wb");
  if (pFilePipeWrite == 0)
    {
      fclose (pFilePipeRead);
      _close (aHandles[1]);
      return (0);
    }
  FILE * pFileIn;
#if _MSC_VER >= 1400
  if (_tfopen_s(&pFileIn, lpszFileName, "rb") != 0)
  {
    pFileIn = 0;
  }
#else
  pFileIn = _tfopen(lpszFileName, "rb");
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
CPostScript::SendHeader (/*[in]*/ const char * lpszHeaderName)
{
  if (m_bError)
    {
      return (false);
    }
  PathName fileName;
  if (! SessionWrapper(true)->FindFile(lpszHeaderName, FileType::PSHEADER, fileName))
    {
      m_bError = true;
      YapError (T_("Cannot find PostScript header file %1."),
		Q_(lpszHeaderName));
      return (false);
    }
  tracePS->WriteFormattedLine ("libdvi", T_("Sending %s..."), Q_(fileName));
  bool bDone = ExecuteBatch(fileName.Get());
  if (! bDone)
    {
      if (! m_bError)
	{
	  m_bError = true;
	  YapError (T_("Cannot send PostScript header file %1."),
		    Q_(fileName));
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
  if (! Execute("\nTeXDict begin @defspecial\n"))
    {
      return (false);
    }
  vector<string>::iterator it;
  for (it = m_setDefinitions.begin();
       bNoError && it != m_setDefinitions.end();
       ++ it)
    {
      bNoError = Execute("%s\n", it->c_str());
    }
  if (! bNoError)
    {
      return (false);
    }
  return (Execute("\n@fedspecial end\n"));
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
      m_bError = true;
      YapError (T_("Cannot find file %1."), Q_(ppsfilespecial->getfilename()));
      return (false);
    }

  bool bDone = true;

  bDone = Execute("%d %d a\n",
		  ppsfilespecial->getx() - pDviImpl->GetResolution(),
		  ppsfilespecial->gety() - pDviImpl->GetResolution());
  
  bDone = bDone && Execute("@beginspecial\n");
  
  if (bDone && ppsfilespecial->has_hsize())
    {
      bDone = Execute("%d @hsize\n", ppsfilespecial->gethsize());
    }
  
  if (bDone && ppsfilespecial->has_vsize())
    {
      bDone = Execute("%d @vsize\n", ppsfilespecial->getvsize());
    }
  
  if (bDone && ppsfilespecial->has_hoffset())
    {
      bDone = Execute("%d @hoffset\n", ppsfilespecial->gethoffset());
    }
  
  if (bDone && ppsfilespecial->has_voffset())
    {
      bDone = Execute("%d @voffset\n", ppsfilespecial->getvoffset());
    }
  
  if (bDone && ppsfilespecial->has_hscale())
    {
      bDone = Execute("%d @hscale\n", ppsfilespecial->gethscale());
    }
  
  if (bDone && ppsfilespecial->has_vscale())
    {
      bDone = Execute("%d @vscale\n", ppsfilespecial->getvscale());
    }
  
  if (bDone && ppsfilespecial->has_angle ())
    {
      bDone = Execute("%d @angle\n", ppsfilespecial->getangle());
    }
  
  if (bDone && ppsfilespecial->has_llx())
    {
      bDone = Execute("%d @llx\n", ppsfilespecial->getllx());
    }
  
  if (bDone && ppsfilespecial->has_lly())
    {
      bDone = Execute("%d @lly\n", ppsfilespecial->getlly());
    }
  
  if (bDone && ppsfilespecial->has_urx())
    {
      bDone = Execute("%d @urx\n", ppsfilespecial->geturx());
    }
  
  if (bDone && ppsfilespecial->has_ury())
    {
      bDone = Execute("%d @ury\n", ppsfilespecial->getury());
    }
  
  if (bDone && ppsfilespecial->has_rwi())
    {
      bDone = Execute("%d @rwi\n", ppsfilespecial->getrwi());
    }
  
  if (bDone && ppsfilespecial->has_rhi())
    {
      bDone = Execute("%d @rhi\n", ppsfilespecial->getrhi());
    }
  
  if (bDone && ppsfilespecial->has_clip())
    {
      bDone = Execute("@clip\n");
    }
  
  bDone = bDone && Execute("@setspecial\n");
  bDone = bDone && ExecuteEncapsulatedPostScript(pathFileName.Get());
  bDone = bDone && Execute("@endspecial\n");

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
CPostScript::AddHeader (/*[in]*/ const char * lpszFileName)
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
    Execute("%d %d a\n",
	    pdvipsspecial->getx() - pDviImpl->GetResolution(),
	    pdvipsspecial->gety() - pDviImpl->GetResolution());
  if (pdvipsspecial->getprotection())
    {
      bDone = bDone && Execute("@beginspecial\n");
      bDone = bDone && Execute("@setspecial\n");
    }
  if (pdvipsspecial->getstring())
    {
      bDone = bDone && Execute("%s\n", pdvipsspecial->getstring());
    }
  else if (pdvipsspecial->getfilename())
    {
      char filename[_MAX_PATH];
      if (! FindGraphicsFile(pdvipsspecial->getfilename(), filename))
	{
	  m_bError = true;
	  bDone = false;
	  YapError (T_("Cannot find file %1."),
		    Q_(pdvipsspecial->getfilename()));
	}
      else
	{
	  bDone = bDone && ExecuteEncapsulatedPostScript(filename);
	}
    }
  if (pdvipsspecial->getprotection())
    {
      bDone = bDone && Execute("@endspecial\n");
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
CPostScript::Open (/*[in]*/ DviImpl * pDviImpl)
{
  pDviImpl = pDviImpl;

  int width, height;

  // calculate device dimensions
  PaperSizeInfo size;
  if (! Utils::GetPaperSize(-1, g_pYapConfig->m_strPaperSize, &size))
    {
      if (pDviImpl->IsPortrait())
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
      if (pDviImpl->IsPortrait())
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
	     && Execute("TeXDict begin\n")
	     && Execute("%d %d %d %d %d (test.dvi) @start\n",
			width,
			height,
			pDviImpl->GetMagnification(),
			pDviImpl->GetResolution(),
			pDviImpl->GetResolution()));
  
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
  Execute ("end\n");
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
  m_bPageBegun = Execute("1 0 bop\n");
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
  bool ret = Execute("eop\n");
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
CPostScript::InternalFindGraphicsFile (/*[in]*/ const char *	lpszFileName,
				       /*[out]*/ char *	lpszResult)
{
  tracePS->WriteFormattedLine ("libdvi", T_("Searching file %s..."), Q_(lpszFileName));
  ASSERT (pDviImpl != 0);
  ASSERT (pDviImpl->GetPathName().GetLength() != 0);
  PathName path = pDviImpl->GetPathName();
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
      tracePS->WriteFormattedLine ("libdvi", T_("Found %s"), Q_(lpszResult));
    }
  return (bFound);
}

/* _________________________________________________________________________

   CPostScript::Uncompress

   Uncompress a graphics file.
   _________________________________________________________________________ */

bool
CPostScript::Uncompress (/*[in]*/ const char *	lpszFileName,
			 /*[out]*/  char *	lpszTempFileName)
{
  if (m_bError)
    {
      return (false);
    }
  char szPath[_MAX_PATH];
  if (! InternalFindGraphicsFile(lpszFileName, szPath))
    {
      m_bError = true;
      YapError (T_("Cannot find file %1."), Q_(lpszFileName));
      return (false);
    }
  PathName temp;
  Utils::UncompressFile (szPath, temp);
  Utils::CopyString (lpszTempFileName, _MAX_PATH, temp.Get());
  pDviImpl->GetTempFiles()[lpszFileName] = lpszTempFileName;
  return (true);
}

/* _________________________________________________________________________

   IsZFileName
   _________________________________________________________________________ */

bool
IsZFileName (/*[in]*/ const CString & strFileName)
{
  return (_tcsicmp(strFileName.Right(3), ".gz") == 0
	  || _tcsicmp(strFileName.Right(4), ".bz2") == 0);
}

/* _________________________________________________________________________

   CPostScript::FindGraphicsFile

   Find a graphics file.  Here we will handle backtics and compressed
   files.
   _________________________________________________________________________ */

bool
CPostScript::FindGraphicsFile (/*[in]*/ const char *	lpszFileName,
			       /*[out]*/ char *	lpszResult)
{
  if (*lpszFileName == '`')
    {
      map<tstring, TempFile>::const_iterator it =
	pDviImpl->GetTempFiles().find(lpszFileName + 1);
      if (it != pDviImpl->GetTempFiles().end())
	{
	  Utils::CopyString (lpszResult, _MAX_PATH, it->second.Get());
	  return (true);
	}
      else if (_tcsncmp(lpszFileName + 1, "gunzip -c ", 10) == 0)
	{
	  return (Uncompress(lpszFileName + 11, lpszResult));
	}
      else if (_tcsncmp(lpszFileName + 1, "bunzip2 -c ", 11) == 0)
	{
	  return (Uncompress(lpszFileName + 12, lpszResult));
	}
      else if (! AllowShellCommand(lpszFileName + 1))
	{
	  tracePS->WriteFormattedLine ("libdvi", T_("Ignoring shell command %s"), Q_(lpszFileName + 1));
	  return (false);
	}
      else
	{
	  char szTempFileName[_MAX_PATH];
	  Utils::CopyString (szTempFileName,
			     _MAX_PATH,
			     PathName().SetToTempFile().Get());
	  CString strCommand;
	  strCommand.Format ("%s > %s",
			     Q_(lpszFileName + 1),
			     szTempFileName);
	  ASSERT (pDviImpl != 0);
	  CString strDocDir = pDviImpl->GetDocDir();
	  StdoutReader reader (0);
	  bool bDone =
	    Process::ExecuteSystemCommand(static_cast<const char *>(strCommand), 0,
			   &reader, strDocDir);
	  if (! bDone)
	    {
	      // <fixme>hard-coded string</fixme>
	      m_bError = true;
	      YapError (T_("Execution of an embedded shell ")
			T_("command failed for some reason!"));
	      DeleteFile (szTempFileName);
	      return (false);
	    }
	  pDviImpl->GetTempFiles()[lpszFileName + 1] = szTempFileName;
	  Utils::CopyString (lpszResult, _MAX_PATH, szTempFileName);
	  return (true);
	}
    }
  else if (IsZFileName(lpszFileName))
    {
      map<tstring, TempFile>::const_iterator it =
	pDviImpl->GetTempFiles().find(lpszFileName);
      if (it != pDviImpl->GetTempFiles().end())
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
