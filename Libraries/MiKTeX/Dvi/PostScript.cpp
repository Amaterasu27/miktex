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

#include <io.h> // FIXME: see _pipe()
#include <fcntl.h> // FIXME: see _pipe()
#include <process.h> // FIXME: see _beginthreadex()

/* _________________________________________________________________________

   PostScript::PostScript
   _________________________________________________________________________ */

PostScript::PostScript ()
  : m_bError (false),
    m_bOpen (false),
    m_bPageBegun (false),
    pDviImpl (0),
    tracePS (TraceStream::Open(MIKTEX_TRACE_DVIPAGE))	 // FIXME
{
}

/* _________________________________________________________________________

   PostScript::~PostScript
   _________________________________________________________________________ */

PostScript::~PostScript ()
{
  Clear ();
}

/* _________________________________________________________________________

   PostScript::Initialize
   _________________________________________________________________________ */

void
PostScript::Initialize ()
{
  AddHeader ("tex.pro");
#if 0
  AddHeader ("finclude.pro");
#endif
  AddHeader ("special.pro");
}

/* _________________________________________________________________________

   PostScript::IsEnabled
   _________________________________________________________________________ */

bool
PostScript::IsEnabled ()
{
  return (true);		// TODO
}

/* _________________________________________________________________________

   PostScript::ExecuteBatch

   Process a PostScript file. This is done with the help of 'run',
   which seems to be a Ghostscript extension.
   
   <fixme>This should be a virtual function.</fixme>
   _________________________________________________________________________ */

void
PostScript::ExecuteBatch (/*[in]*/ const char * lpszFileName)
{
  string command;
  command.reserve (BufferSizes::MaxPath + 10);
  command = '(';
  PathName pathUnx (lpszFileName);
  pathUnx.Convert (ConvertPathNameFlags(ConvertPathNameFlags::ToUnix)
		   | ConvertPathNameFlags::MakeAbsolute);
  command += pathUnx.Get();
  command += ") run\n";
  Execute (command.c_str());
}

/* _________________________________________________________________________

   AllowShellCommand
   _________________________________________________________________________ */

bool
AllowShellCommand (/*[in]*/ const char * lpszCommand)
{
  return (false); // TODO
}

/* _________________________________________________________________________

   PostScript::CopyFile
   _________________________________________________________________________ */

void
PostScript::CopyFile (/*[in]*/ FILE *		pfile,
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
    Write (buf, static_cast<unsigned>(n));
    length -= static_cast<unsigned>(n);
  }
}

/* _________________________________________________________________________

   PostScript::ExecuteEncapsulatedPostScript

   Load an .eps file.
   _________________________________________________________________________ */

void
PostScript::ExecuteEncapsulatedPostScript (/*[in]*/ const char * lpszFileName)
{
  if (m_bError)
  {
    return;
  }

#if 0 // TODO
  StatusBarMessage (T_("Loading PostScript file %s..."), Q_(lpszFileName));
#endif

  FILE * pfile = 0;

  unsigned length = 0;
  unsigned start = 0;

  if (bmeps_can_handle(PathName(lpszFileName).GetBuffer()) != 0)
  {
    tracePS->WriteFormattedLine ("libdvi", T_("Trying to convert %s..."), Q_(lpszFileName));
    pfile = ConvertToEPS(lpszFileName);
    if (pfile == 0)
    {
      m_bError = true;
      FATAL_MIKTEX_ERROR ("PostScript::ExecuteEncapsulatedPostScript",
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
      FATAL_MIKTEX_ERROR ("PostScript::ExecuteEncapsulatedPostScript",
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

  CopyFile (pfile, length);
}

/* _________________________________________________________________________

   ConvertToEPSThread
   _________________________________________________________________________ */

struct ThreadArg
{
  FILE *	filein;
  FILE *	fileout;
  PathName	pathFile;
};

void
MIKTEXCALLBACK
PostScript::ConvertToEPSThread (/*[in]*/ void * pv)
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
}

/* _________________________________________________________________________
   
   ConvertToEPS
   _________________________________________________________________________ */

FILE *
PostScript::ConvertToEPS (/*[in]*/ const char * lpszFileName)
{
  int aHandles[2];
  if (_pipe(aHandles, 4096, _O_BINARY) != 0)
  {
    FATAL_CRT_ERROR ("pipe", 0);
  }
  FILE * pFilePipeRead = fdopen(aHandles[0], "rb");
  if (pFilePipeRead == 0)
  {
    FATAL_CRT_ERROR ("fdopen", 0);
  }
  FILE * pFilePipeWrite = fdopen(aHandles[1], "wb");
  if (pFilePipeWrite == 0)
  {
    fclose (pFilePipeRead);
    _close (aHandles[1]);
    FATAL_CRT_ERROR ("fdopen", 0);
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
    FATAL_CRT_ERROR ("fopen", 0);
  }
  ThreadArg * pthreadarg = new ThreadArg;
  pthreadarg->filein = pFileIn;
  pthreadarg->fileout = pFilePipeWrite;
  pthreadarg->pathFile = lpszFileName;
  auto_ptr<Thread> pConverterThread;
  try
  {
    pConverterThread.reset (Thread::Start(ConvertToEPSThread, pthreadarg));;
  }
  catch (const MiKTeXException &)
  {
    fclose (pFilePipeRead);
    fclose (pFilePipeWrite);
    fclose (pFileIn);
    delete pthreadarg;
    throw;
  }
  return (pFilePipeRead);
}

/* _________________________________________________________________________

   PostScript::SendHeader

   Send a PostScript header file.
   _________________________________________________________________________ */

void
PostScript::SendHeader (/*[in]*/ const char * lpszHeaderName)
{
  if (m_bError)
  {
    return;
  }
  PathName fileName;
  if (! SessionWrapper(true)->FindFile(lpszHeaderName, FileType::PSHEADER, fileName))
  {
    m_bError = true;
    FATAL_MIKTEX_ERROR (T_("PostScript::SendHeader"),
      T_("Cannot find PostScript header file."), lpszHeaderName);
  }
  tracePS->WriteFormattedLine ("libdvi", T_("Sending %s..."), Q_(fileName));
  ExecuteBatch (fileName.Get());
}

/* _________________________________________________________________________

   PostScript::DoProlog

   Initialize the PostScript device: send headers & define macros.
   _________________________________________________________________________ */

void
PostScript::DoProlog ()
{
  vector<string>::iterator it;
  for (it = m_setHeaders.begin(); it != m_setHeaders.end(); ++ it)
  {
    SendHeader (it->c_str());
  }
  DoDefinitions ();
}

/* _________________________________________________________________________

   PostScript::DoDefinitions
   _________________________________________________________________________ */

void
PostScript::DoDefinitions ()
{
  if (m_setDefinitions.empty())
  {
    return;
  }
  Execute ("\nTeXDict begin @defspecial\n");
  for (vector<string>::iterator it = m_setDefinitions.begin();
       it != m_setDefinitions.end();
       ++ it)
  {
    Execute ("%s\n", it->c_str());
  }
  Execute ("\n@fedspecial end\n");
}

/* _________________________________________________________________________

   PostScript::DoSpecial

   Send a psfile special.
   _________________________________________________________________________ */

void
PostScript::DoSpecial (/*[in]*/ PsfileSpecialImpl * ppsfilespecial)
{
  if (m_bError)
  {
    return;
  }
  PathName pathFileName;
  bool bFileExists =
    FindGraphicsFile(ppsfilespecial->GetFileName(), pathFileName.GetBuffer());
  if (! bFileExists)
  {
    m_bError = true;
    FATAL_MIKTEX_ERROR ("PostScript::DoSpecial",
      T_("Cannot find file."), ppsfilespecial->GetFileName());
  }

  Execute ("%d %d a\n",
    ppsfilespecial->GetX() - pDviImpl->GetResolution(),
    ppsfilespecial->GetY() - pDviImpl->GetResolution());
  
  Execute ("@beginspecial\n");
  
  if (ppsfilespecial->HasHsize())
  {
    Execute ("%d @hsize\n", ppsfilespecial->GetHsize());
  }

  if (ppsfilespecial->HasVsize())
  {
    Execute ("%d @vsize\n", ppsfilespecial->GetVsize());
  }

  if (ppsfilespecial->HasHoffset())
  {
    Execute ("%d @hoffset\n", ppsfilespecial->GetHoffset());
  }

  if (ppsfilespecial->HasVoffset())
  {
    Execute ("%d @voffset\n", ppsfilespecial->GetVoffset());
  }

  if (ppsfilespecial->HasHscale())
  {
    Execute ("%d @hscale\n", ppsfilespecial->GetHscale());
  }

  if (ppsfilespecial->HasVscale())
  {
    Execute ("%d @vscale\n", ppsfilespecial->GetVscale());
  }

  if (ppsfilespecial->HasAngle())
  {
    Execute ("%d @angle\n", ppsfilespecial->GetAngke());
  }

  if (ppsfilespecial->HasLlx())
  {
    Execute ("%d @llx\n", ppsfilespecial->GetLlx());
  }

  if (ppsfilespecial->HasLly())
  {
    Execute ("%d @lly\n", ppsfilespecial->GetLly());
  }

  if (ppsfilespecial->HasUrx())
  {
    Execute ("%d @urx\n", ppsfilespecial->GetUrx());
  }

  if (ppsfilespecial->HasUry())
  {
    Execute ("%d @ury\n", ppsfilespecial->GetUry());
  }

  if (ppsfilespecial->HasRwi())
  {
    Execute ("%d @rwi\n", ppsfilespecial->GetRwi());
  }

  if (ppsfilespecial->HasRhi())
  {
    Execute ("%d @rhi\n", ppsfilespecial->GetRhi());
  }

  if (ppsfilespecial->HasClipFlag())
  {
    Execute ("@clip\n");
  }
  
  Execute ("@setspecial\n");
  ExecuteEncapsulatedPostScript (pathFileName.Get());
  Execute ("@endspecial\n");
}

/* _________________________________________________________________________

   PostScript::AddDefinition

   Remember a psdef special for later.
   _________________________________________________________________________ */

void
PostScript::AddDefinition (/*[in]*/ PsdefSpecialImpl * ppsdefspecial)
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

   PostScript::AddHeader

   Remember a psheader file for later.
   _________________________________________________________________________ */

void
PostScript::AddHeader (/*[in]*/ const char * lpszFileName)
{
  if (find(m_setHeaders.begin(), m_setHeaders.end(), lpszFileName) == m_setHeaders.end())
  {
    m_setHeaders.push_back (lpszFileName);
  }
}

/* _________________________________________________________________________

   PostScript::DoSpecial

   Send a ps special.
   _________________________________________________________________________ */

void
PostScript::DoSpecial (/*[in]*/ DvipsSpecialImpl * pdvipsspecial)
{
  if (m_bError)
  {
    return;
  }
  Execute ("%d %d a\n",
    pdvipsspecial->GetX() - pDviImpl->GetResolution(),
    pdvipsspecial->GetY() - pDviImpl->GetResolution());
  if (pdvipsspecial->GetProtection())
  {
    Execute ("@beginspecial\n");
    Execute ("@setspecial\n");
  }
  if (pdvipsspecial->GetString())
  {
    Execute ("%s\n", pdvipsspecial->GetString());
  }
  else if (pdvipsspecial->GetFileName())
  {
    char filename[_MAX_PATH];
    if (! FindGraphicsFile(pdvipsspecial->GetFileName(), filename))
    {
      m_bError = true;
      FATAL_MIKTEX_ERROR ("PostScript::DoSpecial",
	T_("Cannot find file."), pdvipsspecial->GetFileName());
    }
    else
    {
      ExecuteEncapsulatedPostScript (filename);
    }
  }
  if (pdvipsspecial->GetProtection())
  {
    Execute("@endspecial\n");
  }
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

   PostScript::Open

   Open the PostScript device.
   _________________________________________________________________________ */

void
PostScript::Open (/*[in]*/ DviImpl * pDviImpl,
		   /*[in]*/ int shrinkFactor)
{
  this->pDviImpl = pDviImpl;
  this->shrinkFactor = shrinkFactor;

  int width, height;

  // calculate device dimensions
  PaperSizeInfo size = pDviImpl->GetPaperSizeInfo();
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

  // initialize
  Initialize ();
  DoProlog ();
  Execute ("TeXDict begin\n");
  Execute ("%d %d %d %d %d (test.dvi) @start\n",
    width,
    height,
    pDviImpl->GetMagnification(),
    pDviImpl->GetResolution(),
    pDviImpl->GetResolution());

  m_bOpen = true;
}

/* _________________________________________________________________________

   PostScript::Close

   Close the PostScript device.
   _________________________________________________________________________ */

void
PostScript::Close ()

{
  MIKTEX_ASSERT (! m_bPageBegun);
  m_bOpen = false;
  Execute ("end\n");
  Finalize ();
}

/* _________________________________________________________________________

   PostScript::BeginPage

   Start a new page.
   _________________________________________________________________________ */

void
PostScript::BeginPage ()
{
  MIKTEX_ASSERT (! m_bPageBegun);
  Execute ("1 0 bop\n");
  m_bPageBegun = true;
}

/* _________________________________________________________________________

   PostScript::EndPage

   Close the current page.
   _________________________________________________________________________ */

void
PostScript::EndPage ()
{
  MIKTEX_ASSERT (m_bPageBegun);
  Execute ("eop\n");
  m_bPageBegun = false;
}

/* _________________________________________________________________________

   PostScript::Clear
   _________________________________________________________________________ */

void
PostScript::Clear ()
{
  m_setHeaders.clear ();
  m_setDefinitions.clear ();
}

/* _________________________________________________________________________

   PostScript::Finalize
   _________________________________________________________________________ */

void
PostScript::Finalize ()
{
}

/* _________________________________________________________________________

   PostScript::InternalFindGraphicsFile

   Find a graphics file.
   _________________________________________________________________________ */

bool
PostScript::InternalFindGraphicsFile (/*[in]*/ const char *	lpszFileName,
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

   PostScript::Uncompress

   Uncompress a graphics file.
   _________________________________________________________________________ */

void
PostScript::Uncompress (/*[in]*/ const char *	lpszFileName,
			 /*[out]*/  char *	lpszTempFileName)
{
  if (m_bError)
  {
    return;
  }
  char szPath[_MAX_PATH];
  if (! InternalFindGraphicsFile(lpszFileName, szPath))
  {
    m_bError = true;
    FATAL_MIKTEX_ERROR ("PostScript::Uncompress",
      T_("Cannot find file."), lpszFileName);
  }
  PathName temp;
  Utils::UncompressFile (szPath, temp);
  Utils::CopyString (lpszTempFileName, _MAX_PATH, temp.Get());
  pDviImpl->RememberTempFile(lpszFileName, lpszTempFileName);
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

   PostScript::FindGraphicsFile

   Find a graphics file.  Here we will handle backtics and compressed
   files.
   _________________________________________________________________________ */

bool
PostScript::FindGraphicsFile (/*[in]*/ const char *	lpszFileName,
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
      Uncompress (lpszFileName + 11, lpszResult);
      return (true);
    }
    else if (strncmp(lpszFileName + 1, "bunzip2 -c ", 11) == 0)
    {
      Uncompress (lpszFileName + 12, lpszResult);
      return (true);
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
	FATAL_MIKTEX_ERROR ("PostScript::FindGraphicsFile",
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
    Uncompress (lpszFileName, lpszResult);
    return (true);
  }
  else
  {
    return (InternalFindGraphicsFile(lpszFileName, lpszResult));
  }
}
