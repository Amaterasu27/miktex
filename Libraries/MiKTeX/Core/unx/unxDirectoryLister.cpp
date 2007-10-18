/* unxDirectoryLister.cpp: directory lister

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

#include "unx/unxDirectoryLister.h"

/* _________________________________________________________________________

   DirectoryLister::Open
   _________________________________________________________________________ */

DirectoryLister *
MIKTEXCALL
DirectoryLister::Open (/*[in]*/ const PathName & directory)
{
  return (new unxDirectoryLister (directory, 0));
}

/* _________________________________________________________________________

   DirectoryLister::Open
   _________________________________________________________________________ */

DirectoryLister *
MIKTEXCALL
DirectoryLister::Open (/*[in]*/ const PathName &	directory,
		       /*[in]*/ const char *	lpszPattern)
{
  return (new unxDirectoryLister (directory, lpszPattern));
}

/* _________________________________________________________________________

   unxDirectoryLister::unxDirectoryLister
   _________________________________________________________________________ */

unxDirectoryLister::unxDirectoryLister
(/*[in]*/ const PathName &	directory,
 /*[in]*/ const char *	lpszPattern)
  : directory (directory),
    pattern (lpszPattern == 0 ? "" : lpszPattern),
    pDir (0)
{
}

/* _________________________________________________________________________

   unxDirectoryLister::~unxDirectoryLister
   _________________________________________________________________________ */

MIKTEXCALL
unxDirectoryLister::~unxDirectoryLister ()
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

   unxDirectoryLister::Close
   _________________________________________________________________________ */

void
MIKTEXCALL
unxDirectoryLister::Close ()
{
  DIR * pDir = this->pDir;
  if (pDir == 0)
    {
      return;
    }
  this->pDir = 0;
  if (closedir(pDir) != 0)
    {
      FATAL_CRT_ERROR ("closedir", 0);
    }
}

/* _________________________________________________________________________

   unxDirectoryLister::GetNext
   _________________________________________________________________________ */

bool
MIKTEXCALL
unxDirectoryLister::GetNext (/*[out]*/ DirectoryEntry & direntry)
{
  DirectoryEntry2 direntry2;
  if (! GetNext(direntry2, true))
    {
      return (false);
    }
  direntry.name = direntry2.name;
  direntry.isDirectory = direntry2.isDirectory;
  return (true);
}

/* _________________________________________________________________________

   unxDirectoryLister::GetNext
   _________________________________________________________________________ */

bool
MIKTEXCALL
unxDirectoryLister::GetNext (/*[out]*/ DirectoryEntry2 & direntry2)
{
  return (GetNext(direntry2, false));
}

/* _________________________________________________________________________

   unxDirectoryLister::GetNext
   _________________________________________________________________________ */

bool
unxDirectoryLister::GetNext (/*[out]*/ DirectoryEntry2 &	direntry2,
			     /*[in]*/ bool			simple)
{
  if (pDir == 0)
    {
      pDir = opendir(directory.Get());
      if (pDir == 0)
	{
	  FATAL_CRT_ERROR ("opendir", directory.Get());
	}
    }
  struct dirent * pDirent;
  do
    {
      int olderrno = errno;
      pDirent = readdir(pDir);
      if (pDirent == 0)
	{
	  if (errno != olderrno)
	    {
	      FATAL_CRT_ERROR ("readdir", directory.Get());
	    }
	  return (false);
	}
    }
  while (((StringCompare(pDirent->d_name, CURRENT_DIRECTORY) == 0)
	  || (StringCompare(pDirent->d_name, PARENT_DIRECTORY) == 0))
	 || (pattern.length() > 0
	     && ! PathName::Match(pattern.c_str(), pDirent->d_name)));
  direntry2.name = pDirent->d_name;
  bool mustStat = true;
#if defined(HAVE_STRUCT_DIRENT_D_TYPE)
  if (pDirent->d_type != DT_UNKNOWN)
    {
      direntry2.isDirectory = (pDirent->d_type == DT_DIR);
      mustStat = false;
    }
#endif
  if (mustStat || ! simple)
    {
      struct stat statbuf;
      PathName path (directory.Get(), pDirent->d_name, 0);
      if (lstat(path.Get(), &statbuf) != 0)
	{
	  FATAL_CRT_ERROR ("lstat", path.Get());
	}
      direntry2.isDirectory = (S_ISDIR(statbuf.st_mode) != 0);
      direntry2.size = statbuf.st_size;
    }
  return (true);
}
