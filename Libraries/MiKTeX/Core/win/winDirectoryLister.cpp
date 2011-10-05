/* winDirectoryLister.cpp: directory lister (Windows)

   Copyright (C) 1996-2011 Christian Schenk

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

#include "win/winDirectoryLister.h"

/* _________________________________________________________________________

   DirectoryLister::Open
   _________________________________________________________________________ */

DirectoryLister *
DirectoryLister::Open (/*[in]*/ const PathName & directory)
{
  return (new winDirectoryLister (directory, 0, Options::None));
}

/* _________________________________________________________________________

   DirectoryLister::Open
   _________________________________________________________________________ */

DirectoryLister *
DirectoryLister::Open (/*[in]*/ const PathName &	directory,
		       /*[in]*/ const char *		lpszPattern)
{
  return (new winDirectoryLister (directory, lpszPattern, Options::None));
}

/* _________________________________________________________________________

   DirectoryLister::Open
   _________________________________________________________________________ */

DirectoryLister *
DirectoryLister::Open (/*[in]*/ const PathName &	directory,
		       /*[in]*/ const char *		lpszPattern,
		       /*[in]*/ Options			options)
{
  return (new winDirectoryLister (directory, lpszPattern, options));
}

/* _________________________________________________________________________

   winDirectoryLister::winDirectoryLister
   _________________________________________________________________________ */

winDirectoryLister::winDirectoryLister (/*[in]*/ const PathName &	directory,
					/*[in]*/ const char *		lpszPattern,
					/*[in]*/ Options		options)
  : directory (directory),
    pattern (lpszPattern == 0 ? "" : lpszPattern),
    options (options),
    handle (INVALID_HANDLE_VALUE)
{
}

/* _________________________________________________________________________

   winDirectoryLister::~winDirectoryLister
   _________________________________________________________________________ */

winDirectoryLister::~winDirectoryLister ()
{
  try
    {
      Close ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   winDirectoryLister::Close
   _________________________________________________________________________ */

void
winDirectoryLister::Close ()
{
  HANDLE handle = this->handle;
  if (handle == INVALID_HANDLE_VALUE)
    {
      return;
    }
  this->handle = INVALID_HANDLE_VALUE;
  if (! FindClose(handle))
    {
      FATAL_WINDOWS_ERROR ("FindClose", 0);
    }
}

/* _________________________________________________________________________

   winDirectoryLister::GetNext
   _________________________________________________________________________ */

bool
winDirectoryLister::GetNext (/*[out]*/ DirectoryEntry & direntry)
{
  DirectoryEntry2 direntry2;
  if (! GetNext(direntry2))
    {
      return (false);
    }
  direntry.name = direntry2.name;
  direntry.wname = direntry2.wname;
  direntry.isDirectory = direntry2.isDirectory;
  return (true);
}

/* _________________________________________________________________________

   IsDotDirectory
   _________________________________________________________________________ */

inline
bool
IsDotDirectory (/*[in]*/ const wchar_t * lpszDirectory)
{
  if (lpszDirectory[0] != L'.')
  {
    return (false);
  }
  if (lpszDirectory[1] == 0)
  {
    return (true);
  }
  return (lpszDirectory[1] == L'.' && lpszDirectory[2] == 0);
}

/* _________________________________________________________________________

   winDirectoryLister::GetNext
   _________________________________________________________________________ */

bool
winDirectoryLister::GetNext (/*[out]*/ DirectoryEntry2 & direntry2)
{
  WIN32_FIND_DATAW ffdat;
  do
    {
      if (handle == INVALID_HANDLE_VALUE)
	{
	  PathName pathPattern (directory);
	  if (pattern.empty())
	    {
	      pathPattern += "*";
	    }
	  else
	    {
	      pathPattern += pattern.c_str();
	    }
	  handle = FindFirstFileExW(
	    UW_(pathPattern.Get()),
	    IsWindows7() ? FindExInfoBasic : FindExInfoStandard,
	    &ffdat,
	    (options & Options::DirectoriesOnly) != 0 ? FindExSearchLimitToDirectories : FindExSearchNameMatch,
	    0,
	    IsWindows7() ? FIND_FIRST_EX_LARGE_FETCH : 0);
	  if (handle == INVALID_HANDLE_VALUE)
	    {
	      if (::GetLastError() != ERROR_FILE_NOT_FOUND)
		{
		  FATAL_WINDOWS_ERROR ("FindFirstFileExW", directory.Get());
		}
	      return (false);
	    }
	}
      else
	{
	  if (! FindNextFileW(handle, &ffdat))
	    {
	      if (::GetLastError() != ERROR_NO_MORE_FILES)
		{
		  FATAL_WINDOWS_ERROR ("FindNextFileExW", directory.Get());
		}
	      return (false);
	    }
	}
    }
  while ((((ffdat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
          && IsDotDirectory(ffdat.cFileName))
	 || (((options & Options::DirectoriesOnly) != 0)
	     && ((ffdat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0))
	 || (((options & Options::FilesOnly) != 0)
	     && ((ffdat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)));
  direntry2.wname = ffdat.cFileName;
  direntry2.name = WU_(ffdat.cFileName);
  direntry2.isDirectory =
    ((ffdat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
  direntry2.size = ffdat.nFileSizeLow;
  if (ffdat.nFileSizeHigh != 0)
    {
      UNEXPECTED_CONDITION ("winDirectoryLister::GetNext");
    }
  return (true);
}
