/* winMemoryMappedFile.cpp:

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

#include "win/winMemoryMappedFile.h"

/* _________________________________________________________________________

   MemoryMappedFile::Create
   _________________________________________________________________________ */

MemoryMappedFile *
MemoryMappedFile::Create ()
{
  return (new winMemoryMappedFile);
}

/* _________________________________________________________________________

   winMemoryMappedFile::winMemoryMappedFile
   _________________________________________________________________________ */

winMemoryMappedFile::winMemoryMappedFile ()
  : readWrite (false),
    size (0),
    hFile (INVALID_HANDLE_VALUE),
    hMapping (0),
    ptr (0),
    traceStream (TraceStream::Open(MIKTEX_TRACE_MMAP))
{
}

/* _________________________________________________________________________

   winMemoryMappedFile::~winMemoryMappedFile
   _________________________________________________________________________ */

winMemoryMappedFile::~winMemoryMappedFile ()
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

   winMemoryMappedFile::Open
   _________________________________________________________________________ */

void *
winMemoryMappedFile::Open (/*[in]*/ const MIKTEXCHAR *	lpszPath,
			   /*[in]*/ bool		readWrite)
{
  path = lpszPath;
  this->readWrite = readWrite;

  // create a unique object name
  name = T_("");
  name.reserve (BufferSizes::MaxPath);
  for (size_t i = 0; lpszPath[i] != 0; ++ i)
    {
      if (IsDirectoryDelimiter(lpszPath[i])
	  || lpszPath[i] == T_(':'))
	{
	  continue;
	}
      name += ToLower(lpszPath[i]);
    }
  
  // try to open an existing file mapping
  hMapping =
    OpenFileMapping(readWrite ? FILE_MAP_WRITE : FILE_MAP_READ,
		    FALSE,
		    name.c_str());

  if (hMapping != 0)
    {
      traceStream->WriteFormattedLine
	(T_("core"),
	 T_("using existing file mapping object %s"),
	 Q_(name));

      // map existing file view into memory
      ptr =
	MapViewOfFile(hMapping,
		      (readWrite
		       ? FILE_MAP_WRITE
		       : FILE_MAP_READ),
		      0,
		      0,
		      0);
      if (ptr == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("MapViewOfFile"), lpszPath);
	}

      // get the size
#if defined(_MSC_VER)
      struct _stat statbuf;
      if (_tstat(lpszPath, &statbuf) != 0)
	{
	  FATAL_CRT_ERROR (T_("_tstat"), lpszPath);
	}
      size = statbuf.st_size;
#else
#  error Unimplemented: winMemoryMappedFile::Open()
#endif
    }
  else
    {
      traceStream->WriteFormattedLine
	(T_("core"),
	 T_("creating new file mapping object %s"),
	 Q_(name));

      // create a new file mapping
      OpenFile ();
      CreateMapping (0);
    }

  return (ptr);
}

/* _________________________________________________________________________

   winMemoryMappedFile::Close
   _________________________________________________________________________ */

void
winMemoryMappedFile::Close ()
{
  DestroyMapping ();
  CloseFile ();
}

/* _________________________________________________________________________

   winMemoryMappedFile::Resize
   _________________________________________________________________________ */

void *
winMemoryMappedFile::Resize (/*[in]*/ size_t dwNewSize)
{
  DestroyMapping ();
  CreateMapping (dwNewSize);
  return (ptr);
}

/* _________________________________________________________________________

   winMemoryMappedFile::OpenFile
   _________________________________________________________________________ */

void
winMemoryMappedFile::OpenFile ()
{
  unsigned long desiredAccess = GENERIC_READ;
  unsigned long shareMode = FILE_SHARE_READ;

  if (readWrite)
    {
      desiredAccess |= GENERIC_WRITE;
      shareMode |= FILE_SHARE_WRITE;
    }

  traceStream->WriteFormattedLine
    (T_("core"),
     T_("opening memory-mapped file %s for %s"),
     Q_(path),
     (readWrite ? T_("reading/writing") : T_("reading")));
    
  hFile =
    CreateFile(path.c_str(),
	       desiredAccess,
	       shareMode,
	       0,
	       OPEN_EXISTING,
	       FILE_FLAG_RANDOM_ACCESS,
	       0);

  if (hFile == INVALID_HANDLE_VALUE)
    {
      FATAL_WINDOWS_ERROR (T_("CreateFile"), path.c_str());
    }
}

/* _________________________________________________________________________

   winMemoryMappedFile::CreateMapping
   _________________________________________________________________________ */

void
winMemoryMappedFile::CreateMapping (/*[in]*/ size_t maximumFileSize)
{
  if (maximumFileSize == 0)
    {
      unsigned long fileSizeHigh;
      maximumFileSize =
	GetFileSize(hFile, &fileSizeHigh);
      if (maximumFileSize == INVALID_FILE_SIZE)
	{
	  FATAL_WINDOWS_ERROR (T_("GetFileSize"), path.c_str());
	}
      if (fileSizeHigh != 0)
	{
	  UNEXPECTED_CONDITION (T_("winMemoryMappedFile::CreateMapping"));
	}
      if (maximumFileSize == 0)
	{
	  UNEXPECTED_CONDITION (T_("winMemoryMappedFile::CreateMapping"));
	}
    }
  
  size = maximumFileSize;
  
  // create file-mapping object
  hMapping =
    ::CreateFileMapping(hFile,
			0,
			(readWrite
			 ? PAGE_READWRITE
			 : PAGE_READONLY),
			0,
			static_cast<DWORD>(maximumFileSize),
			name.c_str());
  if (hMapping == 0)
    {
      FATAL_WINDOWS_ERROR (T_("CreateFileMapping"), name.c_str());
    }
  
  // map file view into memory
  ptr =
    MapViewOfFile(hMapping,
		  (readWrite ? FILE_MAP_WRITE : FILE_MAP_READ),
		  0,
		  0,
		  maximumFileSize);
  if (ptr == 0)
    {
      FATAL_WINDOWS_ERROR (T_("CreateFileMapping"), name.c_str());
    }
}

/* _________________________________________________________________________

   winMemoryMappedFile::CloseFile
   _________________________________________________________________________ */

void
winMemoryMappedFile::CloseFile ()
{
  if (this->hFile == INVALID_HANDLE_VALUE)
    {
      return;
    }
  HANDLE hFile = this->hFile;
  this->hFile = INVALID_HANDLE_VALUE;
  traceStream->WriteFormattedLine
    (T_("core"),
     T_("closing memory-mapped file %s"),
     Q_(path));
  if (! CloseHandle(hFile))
    {
      FATAL_WINDOWS_ERROR (T_("CloseHandle"), 0);
    }
}

/* _________________________________________________________________________

   winMemoryMappedFile::DestroyMapping
   _________________________________________________________________________ */

void
winMemoryMappedFile::DestroyMapping ()
{
  if (ptr != 0)
    {
      void * ptr = this->ptr;
      this->ptr = 0;
      if (! UnmapViewOfFile(ptr))
	{
	  FATAL_WINDOWS_ERROR (T_("UnmapViewOfFile"), 0);
	}
    }
  if (hMapping != 0)
    {
      HANDLE h = hMapping;
      hMapping = 0;
      if (! CloseHandle(h))
	{
	  FATAL_WINDOWS_ERROR (T_("CloseHandle"), 0);
	}
    }
}

/* _________________________________________________________________________

   winMemoryMappedFile::Flush
   _________________________________________________________________________ */

void
winMemoryMappedFile::Flush ()
{
  if (! FlushViewOfFile(GetPtr(), 0))
    {
      FATAL_WINDOWS_ERROR (T_("FlushViewOfFile"), 0);
    }
}
