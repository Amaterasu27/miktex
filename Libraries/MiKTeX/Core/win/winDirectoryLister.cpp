/* winDirectoryLister.cpp: directory lister (Windows)

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

#include "win/winDirectoryLister.h"

/* _________________________________________________________________________

   DirectoryLister::Open
   _________________________________________________________________________ */

DirectoryLister *
MIKTEXCALL
DirectoryLister::Open (/*[in]*/ const PathName & directory)
{
  return (new winDirectoryLister (directory, 0));
}

/* _________________________________________________________________________

   DirectoryLister::Open
   _________________________________________________________________________ */

DirectoryLister *
MIKTEXCALL
DirectoryLister::Open (/*[in]*/ const PathName &	directory,
		       /*[in]*/ const char *		lpszPattern)
{
  return (new winDirectoryLister (directory, lpszPattern));
}

/* _________________________________________________________________________

   winDirectoryLister::winDirectoryLister
   _________________________________________________________________________ */

winDirectoryLister::winDirectoryLister
(/*[in]*/ const PathName &	directory,
 /*[in]*/ const char *	lpszPattern)
  : directory (directory),
    pattern (lpszPattern == 0 ? "" : lpszPattern),
    handle (INVALID_HANDLE_VALUE)
{
}

/* _________________________________________________________________________

   winDirectoryLister::~winDirectoryLister
   _________________________________________________________________________ */

MIKTEXCALL
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
MIKTEXCALL
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
MIKTEXCALL
winDirectoryLister::GetNext (/*[out]*/ DirectoryEntry & direntry)
{
  DirectoryEntry2 direntry2;
  if (! GetNext(direntry2))
    {
      return (false);
    }
  direntry.name = direntry2.name;
  direntry.isDirectory = direntry2.isDirectory;
  return (true);
}

/* _________________________________________________________________________

   winDirectoryLister::GetNext
   _________________________________________________________________________ */

bool
MIKTEXCALL
winDirectoryLister::GetNext (/*[out]*/ DirectoryEntry2 & direntry2)
{
  WIN32_FIND_DATA ffdat;
  do
    {
      if (handle == INVALID_HANDLE_VALUE)
	{
	  PathName pathPattern (directory);
	  if (pattern.length() == 0)
	    {
	      pathPattern += "*";
	    }
	  else
	    {
	      pathPattern += pattern.c_str();
	    }
	  handle = FindFirstFileA(pathPattern.Get(), &ffdat);
	  if (handle == INVALID_HANDLE_VALUE)
	    {
	      if (::GetLastError() != ERROR_FILE_NOT_FOUND)
		{
		  FATAL_WINDOWS_ERROR ("FindFirstFileA", directory.Get());
		}
	      return (false);
	    }
	}
      else
	{
	  if (! FindNextFileA(handle, &ffdat))
	    {
	      if (::GetLastError() != ERROR_NO_MORE_FILES)
		{
		  FATAL_WINDOWS_ERROR ("FindNextFileA", directory.Get());
		}
	      return (false);
	    }
	}
    }
  while (((ffdat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
	 && ((MiKTeX::Core::StringCompare(ffdat.cFileName, CURRENT_DIRECTORY)
	      == 0)
	     || (MiKTeX::Core::StringCompare(ffdat.cFileName, PARENT_DIRECTORY)
		 == 0)));
  direntry2.name = ffdat.cFileName;
  direntry2.isDirectory =
    ((ffdat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
  direntry2.size = ffdat.nFileSizeLow;
  if (ffdat.nFileSizeHigh != 0)
    {
      UNEXPECTED_CONDITION ("winDirectoryLister::GetNext");
    }
  return (true);
}
