/* unxMemoryMappedFile.cpp:

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

#if ! defined(HAVE_MMAP) && defined(__CYGWIN__)
#  define HAVE_MMAP
#endif

#if ! defined(HAVE_MMAP)
#  error this system does not provide the mmap() function
#endif

#include "unx/unxMemoryMappedFile.h"

/* _________________________________________________________________________

   MemoryMappedFile::Create
   _________________________________________________________________________ */

MemoryMappedFile *
MemoryMappedFile::Create ()
{
  return (new unxMemoryMappedFile);
}

/* _________________________________________________________________________

   unxMemoryMappedFile::unxMemoryMappedFile
   _________________________________________________________________________ */

unxMemoryMappedFile::unxMemoryMappedFile ()
  : readWrite (false),
    size (0),
    filedes (-1),
    ptr (0)
{
}

/* _________________________________________________________________________

   unxMemoryMappedFile::~unxMemoryMappedFile
   _________________________________________________________________________ */

unxMemoryMappedFile::~unxMemoryMappedFile ()
{
  try
    {
      DestroyMapping ();
      CloseFile ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   unxMemoryMappedFile::Open
   _________________________________________________________________________ */

void *
unxMemoryMappedFile::Open (/*[in]*/ const MIKTEXCHAR *	lpszPath,
			   /*[in]*/ bool		readWrite)
{
  path = lpszPath;
  this->readWrite = readWrite;
  OpenFile ();
  CreateMapping (0);
  return (ptr);
}

/* _________________________________________________________________________

   unxMemoryMappedFile::Close
   _________________________________________________________________________ */

void
unxMemoryMappedFile::Close ()
{
  DestroyMapping ();
  CloseFile ();
}

/* _________________________________________________________________________

   unxMemoryMappedFile::Resize
   _________________________________________________________________________ */

void *
unxMemoryMappedFile::Resize (/*[in]*/ size_t newSize)
{
  DestroyMapping ();
  if (ftruncate(filedes, newSize) != 0)
    {
      FATAL_CRT_ERROR (T_("ftruncate"), path.c_str());
    }
  CreateMapping (newSize);
  return (ptr);
}

/* _________________________________________________________________________

   unxMemoryMappedFile::OpenFile
   _________________________________________________________________________ */

void
unxMemoryMappedFile::OpenFile ()
{
  int oflag (readWrite ? O_RDWR : O_RDONLY);
  filedes = open(path.c_str(), oflag);
  if (filedes < 0)
    {
      FATAL_CRT_ERROR (T_("open"), path.c_str());
    }
}

/* _________________________________________________________________________

   winMemoryMappedFile::CreateMapping
   _________________________________________________________________________ */

void
unxMemoryMappedFile::CreateMapping (/*[in]*/ size_t maximumFileSize)
{
  struct stat statbuf;

  if (fstat(filedes, &statbuf) != 0)
    {
      FATAL_CRT_ERROR (T_("fstat"), path.c_str());
    }

  if (maximumFileSize == 0)
    {
      maximumFileSize = statbuf.st_size;
    }
  
  size = maximumFileSize;
  
  ptr =
    mmap(0,
	 size,
	 (readWrite
	  ? (PROT_READ | PROT_WRITE)
	  : PROT_READ),
	 MAP_SHARED,
	 filedes,
	 0);

  if (ptr == MAP_FAILED)
    {
      FATAL_CRT_ERROR (T_("mmap"), path.c_str());
    }
}

/* _________________________________________________________________________

   unxMemoryMappedFile::CloseFile
   _________________________________________________________________________ */

void
unxMemoryMappedFile::CloseFile ()
{
  if (this->filedes < 0)
    {
      return;
    }
  int filedes = this->filedes;
  this->filedes = -1;
  if (close(filedes) < 0)
    {
      FATAL_CRT_ERROR (T_("close"), path.c_str());
    }
}

/* _________________________________________________________________________

   unxMemoryMappedFile::DestroyMapping
   _________________________________________________________________________ */

void
unxMemoryMappedFile::DestroyMapping ()
{
  if (ptr == 0)
    {
      return;
    }
  void * ptr = this->ptr;
  this->ptr = 0;
  if (munmap(ptr, size) != 0)
    {
      FATAL_CRT_ERROR (T_("munmap"), path.c_str());
    }
}

/* _________________________________________________________________________

   unxMemoryMappedFile::Flush
   _________________________________________________________________________ */

void
unxMemoryMappedFile::Flush ()
{
  int err = msync(ptr, size, MS_SYNC);
  if (err != 0)
    {
      FATAL_CRT_ERROR (T_("msync"), path.c_str());
    }
}
