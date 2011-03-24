/* File.cpp: file operations

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

/* _________________________________________________________________________

   Directory::GetCurrentDirectoryA
   _________________________________________________________________________ */

PathName
Directory::GetCurrentDirectoryA ()
{
  PathName cd;
  cd.SetToCurrentDirectory ();
  return (cd);
}

/* _________________________________________________________________________

   Directory::SetCurrentDirectory
   _________________________________________________________________________ */

void
Directory::SetCurrentDirectory (/*[in]*/ const PathName &	path)
{
  if (chdir(path.Get()) != 0)
    {
      FATAL_CRT_ERROR ("chdir", path.Get());
    }
}

/* _________________________________________________________________________

   Directory::Exists
   _________________________________________________________________________ */

bool
Directory::Exists (/*[in]*/ const PathName &	path)
{
  unsigned long attributes = GetFileAttributesW(path.ToWideCharString().c_str());
  if (attributes != INVALID_FILE_ATTRIBUTES)
    {
      if ((attributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
	{
	  SessionImpl::GetSession()->trace_access->WriteFormattedLine
	    ("core",
	     T_("%s is not a directory"),
	     Q_(path));
	  return (false);
	}
      return (true);
    }
  unsigned long error = ::GetLastError();
  if (! (error == ERROR_FILE_NOT_FOUND
	 || error == ERROR_INVALID_NAME
	 || error == ERROR_BAD_NETPATH
	 || error == ERROR_PATH_NOT_FOUND))
    {
      FATAL_WINDOWS_ERROR ("GetFileAttributesW", path.Get());
    }
  return (false);
}

/* _________________________________________________________________________

   Directory::Delete
   _________________________________________________________________________ */

void
Directory::Delete (/*[in]*/ const PathName &	path)
{
  SessionImpl::GetSession()->trace_files->WriteFormattedLine
    ("core",
     T_("deleting directory %s"),
     Q_(path));

  if (! RemoveDirectoryW(AW_(path.Get())))
    {
      FATAL_WINDOWS_ERROR ("RemoveDirectoryW", path.Get());
    }
}

/* _________________________________________________________________________

   File::Exists
   _________________________________________________________________________ */

bool
File::Exists (/*[in]*/ const PathName &	path)
{
  unsigned long attributes = GetFileAttributesW(path.ToWideCharString().c_str());
  if (attributes != INVALID_FILE_ATTRIBUTES)
    {
      if ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
	{
	  SessionImpl::GetSession()->trace_access->WriteFormattedLine
	    ("core",
	     T_("%s is a directory"),
	     Q_(path));
	  return (false);
	}
      SessionImpl::GetSession()->trace_access->WriteFormattedLine
	("core",
	 T_("accessing file %s: OK"),
	 Q_(path));
      return (true);
    }
  unsigned long error = ::GetLastError();
  if (! (error == ERROR_FILE_NOT_FOUND
	 || error == ERROR_INVALID_NAME
	 || error == ERROR_PATH_NOT_FOUND))
    {
      FATAL_WINDOWS_ERROR ("GetFileAttributesW", path.Get());
    }
  SessionImpl::GetSession()->trace_access->WriteFormattedLine
    ("core",
     T_("accessing file %s: NOK"),
     Q_(path));
  return (false);
}

/* _________________________________________________________________________

   File::GetAttributes
   _________________________________________________________________________ */

FileAttributes
File::GetAttributes (/*[in]*/ const PathName &	path)
{
  unsigned long attributes = GetNativeAttributes(path);

  FileAttributes result (FileAttributes::Normal);

  if ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
    {
      result |= FileAttributes::Directory;
    }

  if ((attributes & FILE_ATTRIBUTE_READONLY) != 0)
    {
      result |= FileAttributes::ReadOnly;
    }

  if ((attributes & FILE_ATTRIBUTE_HIDDEN) != 0)
    {
      result |= FileAttributes::Hidden;
    }

  return (result);
}

/* _________________________________________________________________________

   File::GetNativeAttributes
   _________________________________________________________________________ */

unsigned long
File::GetNativeAttributes (/*[in]*/ const PathName & path)
{
  unsigned long attributes = GetFileAttributesW(path.ToWideCharString().c_str());

  if (attributes == INVALID_FILE_ATTRIBUTES)
    {
      FATAL_WINDOWS_ERROR ("GetFileAttributesW", path.Get());
    }

  return (attributes);
}

/* _________________________________________________________________________

   File::SetAttributes
   _________________________________________________________________________ */

void
File::SetAttributes (/*[in]*/ const PathName &	path,
		     /*[in]*/ FileAttributes	attributes)
{
  unsigned long attributesOld = GetNativeAttributes(path);

  unsigned long attributesNew = attributesOld;

  if ((attributes & FileAttributes::ReadOnly) != 0)
    {
      attributesNew |= FILE_ATTRIBUTE_READONLY;
    }
  else
    {
      attributesNew &= ~ FILE_ATTRIBUTE_READONLY;
    }

  if ((attributes & FileAttributes::Hidden) != 0)
    {
      attributesNew |= FILE_ATTRIBUTE_HIDDEN;
    }
  else
    {
      attributesNew &= ~ FILE_ATTRIBUTE_HIDDEN;
    }

  if (attributesNew == attributesOld)
    {
      return;
    }

  SetNativeAttributes (path, attributesNew);
}

/* _________________________________________________________________________

   File::SetNativeAttributes
   _________________________________________________________________________ */

void
File::SetNativeAttributes (/*[in]*/ const PathName &	path,
			   /*[in]*/ unsigned long	nativeAttributes)
{
  SessionImpl::GetSession()->trace_files->WriteFormattedLine
    ("core",
     T_("setting new attributes (%x) on %s"),
     static_cast<int>(nativeAttributes),
     Q_(path));

  if (! SetFileAttributesW(path.ToWideCharString().c_str(), static_cast<DWORD>(nativeAttributes)))
    {
      FATAL_WINDOWS_ERROR ("SetFileAttributesW", path.Get());
    }
}

/* _________________________________________________________________________

   File::GetSize
   _________________________________________________________________________ */

size_t
File::GetSize (/*[in]*/ const PathName &	path)
{
  HANDLE h =
    CreateFileW(path.ToWideCharString().c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

  if (h == INVALID_HANDLE_VALUE)
    {
      FATAL_WINDOWS_ERROR ("CreateFileW", path.Get());
    }

  AutoHANDLE autoClose (h);

  unsigned long fileSize = GetFileSize(h, 0);

  if (fileSize == INVALID_FILE_SIZE)
    {
      FATAL_WINDOWS_ERROR ("GetFileSize", path.Get());
    }

  return (fileSize);
}

/* _________________________________________________________________________

   UniversalCrtTimeToFileTime
   _________________________________________________________________________ */

/*
 * Number of 100 nanosecond units from 1/1/1601 to 1/1/1970
 */
const LONGLONG EPOCH_BIAS = 116444736000000000;


// 1601-01-01 00:00:00 as Unx time
const LONGLONG MIN_TIME_T = -11644473600;

// 30827-12-31 23:59:59 as Unx time
const LONGLONG MAX_TIME_T = 910670515199;

MIKTEXSTATICFUNC(FILETIME)
UniversalCrtTimeToFileTime (/*[in]*/ time_t time)
{
  FILETIME fileTime;
  if (time == static_cast<time_t>(-1)
      || time < MIN_TIME_T
      || time > MAX_TIME_T)
    
    {
      fileTime.dwLowDateTime = 0;
      fileTime.dwHighDateTime = 0;
    }
  else
    {
      LONGLONG ll = static_cast<LONGLONG>(time) * 10000000 + EPOCH_BIAS;
      fileTime.dwLowDateTime = static_cast<DWORD>(ll);
      fileTime.dwHighDateTime = ll >> 32;
    }
  return (fileTime);
}

/* _________________________________________________________________________

   FileTimeToUniversalCrtTime
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(time_t)
FileTimeToUniversalCrtTime (/*[in]*/ FILETIME	fileTime)
{
  if (fileTime.dwLowDateTime == 0 && fileTime.dwHighDateTime == 0)
    {
      return (static_cast<time_t>(-1));
    }
  ULARGE_INTEGER uli;
  uli.LowPart = fileTime.dwLowDateTime;
  uli.HighPart = fileTime.dwHighDateTime;
  return ((uli.QuadPart / 10000000) - (EPOCH_BIAS / 10000000));
}

/* _________________________________________________________________________

   SetTimesInternal
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(void)
SetTimesInternal (/*[in]*/ HANDLE		handle,
		  /*[in]*/ time_t		creationTime,
		  /*[in]*/ time_t		lastAccessTime,
		  /*[in]*/ time_t		lastWriteTime)
{
  FILETIME creationFileTime;
  FILETIME lastAccessFileTime;
  FILETIME lastWriteFileTime;
  if (creationTime != static_cast<time_t>(-1))
    {
      creationFileTime = UniversalCrtTimeToFileTime(creationTime);
    }
  if (lastAccessTime != static_cast<time_t>(-1))
    {
      lastAccessFileTime = UniversalCrtTimeToFileTime(lastAccessTime);
    }
  if (lastWriteTime != static_cast<time_t>(-1))
    {
      lastWriteFileTime = UniversalCrtTimeToFileTime(lastWriteTime);
    }
  if (! SetFileTime(handle,
		    (creationTime != static_cast<time_t>(-1)
		     ? &creationFileTime
		     : 0),
		    (lastAccessTime != static_cast<time_t>(-1)
		     ? &lastAccessFileTime
		     : 0),
		    (lastWriteTime != static_cast<time_t>(-1)
		     ? &lastWriteFileTime
		     : 0)))
    {
      FATAL_WINDOWS_ERROR ("SetFileTime", 0);
    }
}

/* _________________________________________________________________________

   File::SetTimes
   _________________________________________________________________________ */

#define GET_OSFHANDLE(hf) \
  reinterpret_cast<HANDLE>(_get_osfhandle(static_cast<int>(hf)))

void
File::SetTimes (/*[in]*/ int		fd,
		/*[in]*/ time_t		creationTime,
		/*[in]*/ time_t		lastAccessTime,
		/*[in]*/ time_t		lastWriteTime)
{
  SetTimesInternal (GET_OSFHANDLE(fd),
		    creationTime,
		    lastAccessTime,
		    lastWriteTime);
}

/* _________________________________________________________________________

   File::SetTimes
   _________________________________________________________________________ */

void
File::SetTimes (/*[in]*/ FILE *			stream,
		/*[in]*/ time_t			creationTime,
		/*[in]*/ time_t			lastAccessTime,
		/*[in]*/ time_t			lastWriteTime)
{
  SetTimes (_fileno(stream), creationTime, lastAccessTime, lastWriteTime);
}

/* _________________________________________________________________________

   File::SetTimes
   _________________________________________________________________________ */

void
File::SetTimes (/*[in]*/ const PathName &	path,
		/*[in]*/ time_t			creationTime,
		/*[in]*/ time_t			lastAccessTime,
		/*[in]*/ time_t			lastWriteTime)
{
  FileStream stream (File::Open(path,
				FileMode::Open,
				FileAccess::ReadWrite,
				false));
  SetTimes (stream.Get(), creationTime, lastAccessTime, lastWriteTime);
  stream.Close ();
}

/* _________________________________________________________________________

   Directory::SetTimes
   _________________________________________________________________________ */

void
Directory::SetTimes (/*[in]*/ const PathName &	path,
		     /*[in]*/ time_t		creationTime,
		     /*[in]*/ time_t		lastAccessTime,
		     /*[in]*/ time_t		lastWriteTime)
{
  HANDLE h =
    CreateFileW(path.ToWideCharString().c_str(),
		FILE_WRITE_ATTRIBUTES,
		0,
		0,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		0);
  if (h == INVALID_HANDLE_VALUE)
    {
      FATAL_WINDOWS_ERROR ("CreateFileW", path.Get());
    }
  AutoHANDLE autoClose (h);
  SetTimesInternal (h, creationTime, lastAccessTime, lastWriteTime);
}

/* _________________________________________________________________________

   File::GetTimes
   _________________________________________________________________________ */

void
File::GetTimes (/*[in]*/ const PathName &	path,
		/*[out]*/ time_t &		creationTime,
		/*[out]*/ time_t &		lastAccessTime,
		/*[out]*/ time_t &		lastWriteTime)
{
  WIN32_FIND_DATAW findData;
  HANDLE findHandle = FindFirstFileW(path.ToWideCharString().c_str(), &findData);
  if (findHandle == INVALID_HANDLE_VALUE)
  {
    FATAL_WINDOWS_ERROR ("FindFirstFileW", path.Get());
  }
  if (! FindClose(findHandle))
  {
    FATAL_WINDOWS_ERROR ("FindClose", 0);
  }
  creationTime = FileTimeToUniversalCrtTime(findData.ftCreationTime);
  lastAccessTime = FileTimeToUniversalCrtTime(findData.ftLastAccessTime);
  lastWriteTime = FileTimeToUniversalCrtTime(findData.ftLastWriteTime);
}

/* _________________________________________________________________________

   File::Delete
   _________________________________________________________________________ */

void
File::Delete (/*[in]*/ const PathName &	path)
{
  SessionImpl::GetSession()->trace_files->WriteFormattedLine
    ("core",
     T_("deleting %s"),
     Q_(path));
  if (! DeleteFileW(path.ToWideCharString().c_str()))
    {
      FATAL_WINDOWS_ERROR ("DeleteFileW", path.Get());
    }
}

/* _________________________________________________________________________

   File::Move
   _________________________________________________________________________ */

void
File::Move (/*[in]*/ const PathName &	source,
	    /*[in]*/ const PathName &	dest)
{
  SessionImpl::GetSession()->trace_files->WriteFormattedLine
    ("core",
     T_("renaming %s to %s"),
     Q_(source),
     Q_(dest));

  if (! MoveFileW(source.ToWideCharString().c_str(), dest.ToWideCharString().c_str()))
    {
      FATAL_WINDOWS_ERROR ("MoveFileW", source.Get());
    }
}

/* _________________________________________________________________________

   File::Copy
   _________________________________________________________________________ */

void
File::Copy (/*[in]*/ const PathName &	source,
	    /*[in]*/ const PathName &	dest,
	    /*[in]*/ bool		preserveAttributes)
{
  UNUSED_ALWAYS (preserveAttributes);

  SessionImpl::GetSession()->trace_files->WriteFormattedLine
    ("core",
     T_("copying %s to %s"),
     Q_(source),
     Q_(dest));

  if (File::Exists(dest))
    {
      unsigned long destAttributes = GetNativeAttributes(dest);
      unsigned long destAttributes2 = destAttributes;
      destAttributes2 &= ~ FILE_ATTRIBUTE_READONLY;
      destAttributes2 &= ~ FILE_ATTRIBUTE_HIDDEN;
      if (destAttributes != destAttributes2)
	{
	  SetNativeAttributes (dest, destAttributes2);
	}
    }

  if (! CopyFileW(source.ToWideCharString().c_str(), dest.ToWideCharString().c_str(), FALSE))
    {
      FATAL_WINDOWS_ERROR ("CopyFileW", source.Get());
    }
}

/* _________________________________________________________________________

   File::Open
   _________________________________________________________________________ */

FILE *
File::Open (/*[in]*/ const PathName &	path,
	    /*[in]*/ FileMode		mode,
	    /*[in]*/ FileAccess		access,
	    /*[in]*/ bool		isTextFile,
	    /*[in]*/ FileShare		share)
{
  SessionImpl::GetSession()->trace_files->WriteFormattedLine
    ("core",
     T_("opening file %s (%d 0x%x %d %d)"),
     Q_(path),
     static_cast<int>(mode.Get()),
     static_cast<int>(access.Get()),
     static_cast<int>(share.Get()),
     static_cast<int>(isTextFile));

  int flags = 0;
  string strFlags;

  if (mode == FileMode::Create)
    {
      flags |= O_CREAT;
    }
  else if (mode == FileMode::Append)
    {
      flags |= O_APPEND;
    }

  if (access == FileAccess::ReadWrite)
    {
      flags |= O_RDWR;
      if (mode == FileMode::Append)
	{
	  strFlags += "a+";
	}
      else
	{
	  strFlags += "r+";
	}
    }
  else if (access == FileAccess::Read)
    {
      flags |= O_RDONLY;
      strFlags += "r";
    }
  else if (access == FileAccess::Write)
    {
      flags |= O_WRONLY;
      if (mode == FileMode::Append)
	{
	  strFlags += "a";
	}
      else
	{
	  flags |= O_TRUNC;
	  strFlags += "w";
	}
    }

#if defined(O_SEQUENTIAL)
  flags |= O_SEQUENTIAL;
#if 0
  strFlags += "S";
#endif
#endif

  if (isTextFile)
    {
      flags |= O_TEXT;
      strFlags += "t";
    }
  else
    {
      flags |= O_BINARY;
      strFlags += "b";
    }

  int fd;

#if defined(_MSC_VER)
  int shflags = 0;
  if (share == FileShare::None)
    {
      shflags = SH_DENYRW;
    }
  else if (share == FileShare::Read)
    {
      shflags |= SH_DENYWR;
    }
  if (share == FileShare::Write)
    {
      shflags |= SH_DENYRD;
    }
  else if (share == FileShare::ReadWrite)
    {
      shflags |= SH_DENYNO;
    }
  if (mode == FileMode::Create)
    {
      PathName dir (path);
      dir.MakeAbsolute ();
      dir.RemoveFileSpec();
      if (! Directory::Exists(dir))
	{
	  Directory::Create (dir);
	}
    }
  if (_wsopen_s(&fd,
		path.ToWideCharString().c_str(),
		flags,
		shflags,
		(((flags & O_CREAT) == 0) ? 0 : S_IREAD | S_IWRITE))
      != 0)
    {
      fd = -1;
    }
  if (fd < 0)
    {
      FATAL_CRT_ERROR ("_wsopen_s", path.Get());
    }
#else
  UNUSED_ALWAYS (shflags);
  fd =
    open(path.Get(),
	 flags,
	 (((flags & O_CREAT) == 0) ? 0 : S_IREAD | S_IWRITE));
  if (fd < 0)
    {
      FATAL_CRT_ERROR ("open", path.Get());
    }
#endif

  return (FdOpen(fd, strFlags.c_str()));
}
