/* scratchdir.cpp: managing scratch directories

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

/* _________________________________________________________________________

   IsGoodTempDirectory
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
IsGoodTempDirectory (/*[in]*/ const char *	lpszPath)
{
  return (Utils::IsAbsolutePath(lpszPath) && Directory::Exists(lpszPath));
}

/* _________________________________________________________________________

   SessionImpl::GetTempDirectory
   _________________________________________________________________________ */

PathName
SessionImpl::GetTempDirectory ()
{
  string tempDirectory;

  // try MiKTeX temp directory
  if ((GetSessionValue(MIKTEX_REGKEY_CORE,
		       MIKTEX_REGVAL_TEMPDIR,
		       tempDirectory,
		       0)
       != 0)
      && IsGoodTempDirectory(tempDirectory.c_str()))
    {
      return (tempDirectory);
    }

  PathName path;

#if defined(MIKTEX_WINDOWS)
  // get system drive (usually C:)
  string systemDrive;
  if (IsWindowsNT())
    {
      if (! Utils::GetEnvironmentString("SystemDrive", systemDrive))
	{
	  TraceError (T_("SystemDrive environment variable not defined"));
	}
    }
  if (systemDrive.length() == 0)
    {
      systemDrive = "C:";
    }

  // try "C:\MiKTeXTemp"
  path.Set (systemDrive.c_str(), MIKTEX_TEMP_DIR, 0, 0);
  if (IsGoodTempDirectory(path.Get()))
    {
      return (path);
    }
#endif
  
  // try designated temp directory
  path.SetToTempDirectory ();
  if (IsGoodTempDirectory(path.Get()))
    {
      return (path);
    }

  //
  // it would be unusual to arrive at this point
  //

#if defined(MIKTEX_WINDOWS)
  // try C:\temp
  path.Set (systemDrive.c_str(), "\\temp", 0, 0);
  if (IsGoodTempDirectory(path.Get()))
    {
      return (path);
    }

  // try C:\tmp
  path.Set (systemDrive.c_str(), "\\tmp", 0, 0);
  if (IsGoodTempDirectory(path.Get()))
    {
      return (path);
    }
#endif

  // try local root
  path = GetRootDirectory(GetUserDataRoot());
  if (IsGoodTempDirectory(path.Get()))
    {
      return (path);
    }

#if defined(MIKTEX_WINDOWS)
  // as a last resort: try "C:\"
  path.Set (systemDrive.c_str(), "\\", 0, 0);
  if (IsGoodTempDirectory(path.Get()))
    {
      return (path);
    }
#endif

  FATAL_MIKTEX_ERROR ("SessionImpl::GetTempDirectory",
		      T_("No suitable temp directory found."),
		      0);
}

/* _________________________________________________________________________

   NameExists
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
NameExists (/*[in]*/ const char *	lpszName)
{
#if defined(MIKTEX_WINDOWS)
  unsigned long attributes = GetFileAttributes(lpszName);
  return (attributes != INVALID_FILE_ATTRIBUTES);
#else
  struct stat statbuf;
  return (stat(lpszName, &statbuf) == 0);
#endif
}

/* _________________________________________________________________________

   InitializeRand
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(void)
InitializeRand ()
{
  static bool done = false;
  if (done)
    {
      return;
    }
  // <fixme>calling srand()</fixme>
  srand (static_cast<unsigned>(time(0)));
  done = true;
}

/* _________________________________________________________________________

   ScratchDirectory::Create
   _________________________________________________________________________ */

namespace {
  MIKTEX_DEFINE_LOCK(ScratchDirectory);
}

PathName &
ScratchDirectory::Create (/*[out]*/ PathName & path)
{
  // get temp directory
  PathName pathTempDir = SessionImpl::GetSession()->GetTempDirectory();
  
  InitializeRand ();
  
  // make a unique sub-directory name
  for (size_t maxrounds = 5; maxrounds > 0; -- maxrounds)
    {
      string fileName = "mik";
      MIKTEX_LOCK (ScratchDirectory)
	{
	  unsigned unique =
	    static_cast<unsigned>((static_cast<double>(rand())
				   / RAND_MAX)
				  * 0xffff);
	  fileName += NUMTOSTR(unique);
	  PathName pathScratchDirectory (pathTempDir.Get(),
					 fileName.c_str(),
					 0);
	  if (! NameExists(pathScratchDirectory.Get()))
	    {
	      // create scratch directory
	      path = pathScratchDirectory;
	      Directory::Create (path);
	      return (path);
	    }
	  else
	    {
	      SessionImpl::GetSession()->trace_tempfile->WriteFormattedLine
		("core",
		 T_("\"%s\" already exists"),
		 pathScratchDirectory.Get());
	    }
	}
      MIKTEX_UNLOCK ();
    }

  FATAL_MIKTEX_ERROR ("ScratchDirectory::Create",
		      T_("Could not create a unique sub-directory."),
		      pathTempDir.Get());
}

/* _________________________________________________________________________

   ScratchDirectory::ScratchDirectory
   _________________________________________________________________________ */

ScratchDirectory::ScratchDirectory ()
  : entered (false)
{
}

/* _________________________________________________________________________

   ScratchDirectory::~ScratchDirectory
   _________________________________________________________________________ */

ScratchDirectory::~ScratchDirectory ()
{
  try
    {
      if (entered)
	{
	  Leave ();
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   ScratchDirectory::Enter
   _________________________________________________________________________ */

void
ScratchDirectory::Enter (/*[in]*/ const char *	lpszPrefix)
{
  UNUSED_ALWAYS (lpszPrefix);

  SessionImpl::ScratchDirectoryInfo scratchDirectoryInfo;

  // get current working directory
  scratchDirectoryInfo.previous = PathName().SetToCurrentDirectory().Get();

  // create a scratch directory
  PathName scratchDirectory;
  scratchDirectoryInfo.scratchDirectory = Create(scratchDirectory).Get();

  SessionImpl::GetSession()
    ->scratchDirectoryStack.push (scratchDirectoryInfo);
  
  SessionImpl::GetSession()->trace_tempfile->WriteFormattedLine
    ("core",
     T_("entering scratch directory \"%s\""),
     scratchDirectoryInfo.scratchDirectory.c_str());
  
  // change working directory
  Directory::SetCurrentDirectory
    (scratchDirectoryInfo.scratchDirectory.c_str());
  
  // we're in
  entered = true;
  
  // add the previous directory to the input path
  SessionImpl::GetSession()
    ->AddWorkingDirectory (scratchDirectoryInfo.previous.c_str(), false);
}

/* _________________________________________________________________________

   ScratchDirectory::Leave
   _________________________________________________________________________ */

void
ScratchDirectory::Leave ()
{
  if (! entered)
    {
      return;
    }

  if (SessionImpl::GetSession()->scratchDirectoryStack.empty())
    {
      FATAL_MIKTEX_ERROR ("ScratchDirectory::Leave",
			  T_("The scratch directory could not be left."),
			  T_("empty stack"));
    }

  SessionImpl::ScratchDirectoryInfo scratchdir =
    SessionImpl::GetSession()->scratchDirectoryStack.top();
  SessionImpl::GetSession()->scratchDirectoryStack.pop ();

  // remove the previous directory from the input path
  SessionImpl::GetSession()
    ->RemoveWorkingDirectory (scratchdir.previous.c_str());

  SessionImpl::GetSession()->trace_tempfile->WriteFormattedLine
    ("core",
     T_("leaving scratch directory \"%s\""),
     scratchdir.scratchDirectory.c_str());
  
  // sanity check
  PathName pathCwd;
  pathCwd.SetToCurrentDirectory ();
  PathName pathScratchDir (scratchdir.scratchDirectory.c_str());
  if (PathName::Compare(pathCwd, pathScratchDir) != 0)
    {
      FATAL_MIKTEX_ERROR ("ScratchDirectory::Leave",
			  T_("The scratch directory could not be left."),
			  pathScratchDir.Get());
    }
  
  // change directory
  Directory::SetCurrentDirectory (scratchdir.previous.c_str());

  entered = false;
  
  // remove scratch directory
  Directory::Delete (scratchdir.scratchDirectory.c_str(), true);
}
