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

#include <io.h> // FIXME: see _pipe()
#include <fcntl.h> // FIXME: see _pipe()
#include <process.h> // FIXME: see _beginthreadex()

/* _________________________________________________________________________

   CPostScript::CPostScript
   _________________________________________________________________________ */

CPostScript::CPostScript ()
  : m_bError (false),
    m_bOpen (false),
    m_bPageBegun (false),
    pDviImpl (0),
    tracePS (TraceStream::Open(MIKTEX_TRACE_DVIPAGE))	 // FIXME
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
  return (true);		// FIXME
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
  string strCommand;
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
AllowShellCommand (/*[in]*/ const char * lpszCommand)
{
  return (false); // todo
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
	  FATAL_MIKTEX_ERROR ("CPostScript::ExecuteEncapsulatedPostScript",
	    T_("Cannot convert to EPS."), lpszFileName);
	}	  
    }
  else
    {
#if _MSC_VER >= 1400
      if (fopen_s(&pfile, lpszFileName, "rb") != 0)
      {
	pfile = 0;
      }
#else
      pfile = fopen(lpszFileName, "rb");
#endif
      if (pfile == 0)
	{
	  m_bError = true;
	  FATAL_MIKTEX_ERROR ("CPostScript::ExecuteEncapsulatedPostScript",
	    T_("Cannot open PostScript file."), lpszFileName);
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

  AutoFILE xxx (pfile);

  if (fseek(pfile, start, SEEK_SET) != 0)
    {
      tracePS->WriteFormattedLine ("libdvi", T_("fseek() failed for some reason"));
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
  MIKTEX_ASSERT (pv != 0);
  ThreadArg * parg = reinterpret_cast<ThreadArg*>(pv);
  MIKTEX_ASSERT (parg->filein != 0 && parg->fileout != 0);
  bmeps_cfg ("c");
  int rc = bmeps_run(parg->fileout, parg->filein, parg->pathFile.GetBuffer());
  if (rc == 0)
    {
#if 0 // FIXME
      tracePS->WriteFormattedLine ("libdvi", T_("bmeps_run() failed for some reason"));
#endif
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
  FILE * pFilePipeRead = fdopen(aHandles[0], "rb");
  if (pFilePipeRead == 0)
    {
      _close (aHandles[0]);
      return (0);
    }
  FILE * pFilePipeWrite = fdopen(aHandles[1], "wb");
  if (pFilePipeWrite == 0)
    {
      fclose (pFilePipeRead);
      _close (aHandles[1]);
      return (0);
    }
  FILE * pFileIn;
#if _MSC_VER >= 1400
  if (fopen_s(&pFileIn, lpszFileName, "rb") != 0)
  {
    pFileIn = 0;
  }
#else
  pFileIn = fopen(lpszFileName, "rb");
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
      FATAL_MIKTEX_ERROR (T_("CPostScript::SendHeader"),
	T_("Cannot find PostScript header file."), lpszHeaderName);
    }
  tracePS->WriteFormattedLine ("libdvi", T_("Sending %s..."), Q_(fileName));
  bool bDone = ExecuteBatch(fileName.Get());
  if (! bDone)
    {
      if (! m_bError)
	{
	  m_bError = true;
	  FATAL_MIKTEX_ERROR ("CPostScript::SendHeader",
	    T_("Cannot send PostScript header file."), fileName.Get());
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
CPostScript::DoSpecial (/*[in]*/ PsfileSpecialImpl * ppsfilespecial)
{
  if (m_bError)
    {
      return (false);
    }
  PathName pathFileName;
  bool bFileExists =
    FindGraphicsFile(ppsfilespecial->GetFileName(), pathFileName.GetBuffer());
  if (! bFileExists)
    {
      m_bError = true;
      FATAL_MIKTEX_ERROR ("CPostScript::DoSpecial",
	T_("Cannot find file."), ppsfilespecial->GetFileName());
    }

  bool bDone = true;

  bDone = Execute("%d %d a\n",
    ppsfilespecial->GetX() - pDviImpl->GetResolution(),
    ppsfilespecial->GetY() - pDviImpl->GetResolution());
  
  bDone = bDone && Execute("@beginspecial\n");
  
  if (bDone && ppsfilespecial->HasHsize())
    {
      bDone = Execute("%d @hsize\n", ppsfilespecial->GetHsize());
    }
  
  if (bDone && ppsfilespecial->HasVsize())
    {
      bDone = Execute("%d @vsize\n", ppsfilespecial->GetVsize());
    }
  
  if (bDone && ppsfilespecial->HasHoffset())
    {
      bDone = Execute("%d @hoffset\n", ppsfilespecial->GetHoffset());
    }
  
  if (bDone && ppsfilespecial->HasVoffset())
    {
      bDone = Execute("%d @voffset\n", ppsfilespecial->GetVoffset());
    }
  
  if (bDone && ppsfilespecial->HasHscale())
    {
      bDone = Execute("%d @hscale\n", ppsfilespecial->GetHscale());
    }
  
  if (bDone && ppsfilespecial->HasVscale())
    {
      bDone = Execute("%d @vscale\n", ppsfilespecial->GetVscale());
    }
  
  if (bDone && ppsfilespecial->HasAngle())
    {
      bDone = Execute("%d @angle\n", ppsfilespecial->GetAngke());
    }
  
  if (bDone && ppsfilespecial->HasLlx())
    {
      bDone = Execute("%d @llx\n", ppsfilespecial->GetLlx());
    }
  
  if (bDone && ppsfilespecial->HasLly())
    {
      bDone = Execute("%d @lly\n", ppsfilespecial->GetLly());
    }
  
  if (bDone && ppsfilespecial->HasUrx())
    {
      bDone = Execute("%d @urx\n", ppsfilespecial->GetUrx());
    }
  
  if (bDone && ppsfilespecial->HasUry())
    {
      bDone = Execute("%d @ury\n", ppsfilespecial->GetUry());
    }
  
  if (bDone && ppsfilespecial->HasRwi())
    {
      bDone = Execute("%d @rwi\n", ppsfilespecial->GetRwi());
    }
  
  if (bDone && ppsfilespecial->HasRhi())
    {
      bDone = Execute("%d @rhi\n", ppsfilespecial->GetRhi());
    }
  
  if (bDone && ppsfilespecial->HasClipFlag())
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
CPostScript::AddDefinition (/*[in]*/ PsdefSpecialImpl * ppsdefspecial)
{
  if (ppsdefspecial->GetDef())
    {
      if (find(m_setDefinitions.begin(), m_setDefinitions.end(),
	       ppsdefspecial->GetDef())
	  == m_setDefinitions.end())
	{
	  m_setDefinitions.push_back (ppsdefspecial->GetDef());
	}
    }
  else if (ppsdefspecial->GetFileName())
    {
      AddHeader (ppsdefspecial->GetFileName());
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
CPostScript::DoSpecial (/*[in]*/ DvipsSpecialImpl * pdvipsspecial)
{
  if (m_bError)
    {
      return (false);
    }
  bool bDone =
    Execute("%d %d a\n",
	    pdvipsspecial->GetX() - pDviImpl->GetResolution(),
	    pdvipsspecial->GetY() - pDviImpl->GetResolution());
  if (pdvipsspecial->GetProtection())
    {
      bDone = bDone && Execute("@beginspecial\n");
      bDone = bDone && Execute("@setspecial\n");
    }
  if (pdvipsspecial->GetString())
    {
      bDone = bDone && Execute("%s\n", pdvipsspecial->GetString());
    }
  else if (pdvipsspecial->GetFileName())
    {
      char filename[_MAX_PATH];
      if (! FindGraphicsFile(pdvipsspecial->GetFileName(), filename))
	{
	  m_bError = true;
	  bDone = false;
	  FATAL_MIKTEX_ERROR ("CPostScript::DoSpecial",
	    T_("Cannot find file."), pdvipsspecial->GetFileName());
	}
      else
	{
	  bDone = bDone && ExecuteEncapsulatedPostScript(filename);
	}
    }
  if (pdvipsspecial->GetProtection())
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
  this->pDviImpl = pDviImpl;

  int width, height;

  // calculate device dimensions
  PaperSizeInfo size = pDviImpl->GetPaperSizeInfo();
    {
      if (pDviImpl->Landscape())
	{
	  height = pt2sp(size.width);
	  width = pt2sp(size.height);
	}
      else
	{
	  width = pt2sp(size.width);
	  height = pt2sp(size.height);
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
  MIKTEX_ASSERT (! m_bPageBegun);
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
  MIKTEX_ASSERT (! m_bPageBegun);
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
  MIKTEX_ASSERT (m_bPageBegun);
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
				       /*[out]*/ char *		lpszResult)
{
  tracePS->WriteFormattedLine ("libdvi", T_("Searching file %s..."), Q_(lpszFileName));
  MIKTEX_ASSERT (pDviImpl != 0);
  MIKTEX_ASSERT (pDviImpl->GetDviFileName().GetLength() != 0);
  PathName path = pDviImpl->GetDviFileName();
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
      FATAL_MIKTEX_ERROR ("CPostScript::Uncompress",
	T_("Cannot find file."), lpszFileName);
    }
  PathName temp;
  Utils::UncompressFile (szPath, temp);
  Utils::CopyString (lpszTempFileName, _MAX_PATH, temp.Get());
  pDviImpl->RememberTempFile(lpszFileName, lpszTempFileName);
  return (true);
}

/* _________________________________________________________________________

   IsZFileName
   _________________________________________________________________________ */

bool
IsZFileName (/*[in]*/ const PathName & fileName)
{
  return (fileName.HasExtension(".gz") || fileName.HasExtension(".bz2"));
}

/* _________________________________________________________________________

   CPostScript::FindGraphicsFile

   Find a graphics file.  Here we will handle backtics and compressed
   files.
   _________________________________________________________________________ */

bool
CPostScript::FindGraphicsFile (/*[in]*/ const char *	lpszFileName,
			       /*[out]*/ char *		lpszResult)
{
  if (*lpszFileName == '`')
    {
      PathName tempFile;
      if (pDviImpl->TryGetTempFile(lpszFileName, tempFile))
	{
	  Utils::CopyString (lpszResult, _MAX_PATH, tempFile.Get());
	  return (true);
	}
      else if (strncmp(lpszFileName + 1, "gunzip -c ", 10) == 0)
	{
	  return (Uncompress(lpszFileName + 11, lpszResult));
	}
      else if (strncmp(lpszFileName + 1, "bunzip2 -c ", 11) == 0)
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
	  string command;
	  command = lpszFileName + 1;
	  command += " > ";
	  command += Q_(szTempFileName);
	  MIKTEX_ASSERT (pDviImpl != 0);
	  PathName docDir = pDviImpl->GetDviFileName();
	  docDir.RemoveFileSpec();
	  StdoutReader reader;
	  bool bDone =
	    Process::ExecuteSystemCommand(command.c_str(), 0,
			   &reader, docDir.Get());
	  if (! bDone)
	    {
	      // <fixme>hard-coded string</fixme>
	      m_bError = true;
	      File::Delete (szTempFileName);
	      FATAL_MIKTEX_ERROR ("CPostScript::FindGraphicsFile",
		T_("Execution of an embedded shell ")
		T_("command failed for some reason!"),
		0);
	    }
	  pDviImpl->RememberTempFile (lpszFileName + 1, szTempFileName);
	  Utils::CopyString (lpszResult, _MAX_PATH, szTempFileName);
	  return (true);
	}
    }
  else if (IsZFileName(lpszFileName))
    {
      PathName tempFile;
      if (pDviImpl->TryGetTempFile(lpszFileName, tempFile))
	{
	  Utils::CopyString (lpszResult, _MAX_PATH, tempFile.Get());
	  return (true);
	}
      return (Uncompress(lpszFileName, lpszResult));
    }
  else
    {
      return (InternalFindGraphicsFile(lpszFileName, lpszResult));
    }
}
