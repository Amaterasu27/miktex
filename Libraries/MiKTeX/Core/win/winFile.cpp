/* File.cpp: file operations

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

   Directory::GetCurrentDirectory
   _________________________________________________________________________ */

PathName
Directory::GetCurrentDirectory ()
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
#if defined(_MSC_VER)
  if (_tchdir(path.Get()) != 0)
    {
      FATAL_CRT_ERROR (T_("_tchdir"), path.Get());
    }
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: ChDir()
#else
  if (chdir(path.Get()) != 0)
    {
      FATAL_CRT_ERROR (T_("chdir"), path.Get());
    }
#endif
}

/* _________________________________________________________________________

   Directory::Exists
   _________________________________________________________________________ */

bool
Directory::Exists (/*[in]*/ const PathName &	path)
{
  unsigned long attributes = GetFileAttributes(path.Get());
  if (attributes != INVALID_FILE_ATTRIBUTES)
    {
      if ((attributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
	{
	  SessionImpl::theSession->trace_access->WriteFormattedLine
	    (T_("core"),
	     T_("%s is not a directory"),
	     Q_(path));
	  return (false);
	}
      return (true);
    }
  unsigned long error = ::GetLastError();
  if (! (error == ERROR_FILE_NOT_FOUND
	 || error == ERROR_PATH_NOT_FOUND))
    {
      FATAL_WINDOWS_ERROR (T_("GetFileAttributes"), path.Get());
    }
  return (false);
}

/* _________________________________________________________________________

   Directory::Delete
   _________________________________________________________________________ */

void
Directory::Delete (/*[in]*/ const PathName &	path)
{
  SessionImpl::theSession->trace_files->WriteFormattedLine
    (T_("core"),
     T_("deleting directory %s"),
     Q_(path));

  if (! RemoveDirectory(path.Get()))
    {
      FATAL_WINDOWS_ERROR (T_("RemoveDirectory"), path.Get());
    }
}

/* _________________________________________________________________________

   File::Exists
   _________________________________________________________________________ */

bool
File::Exists (/*[in]*/ const PathName &	path)
{
  unsigned long attributes = GetFileAttributes(path.Get());
  if (attributes != INVALID_FILE_ATTRIBUTES)
    {
      if ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
	{
	  SessionImpl::theSession->trace_access->WriteFormattedLine
	    (T_("core"),
	     T_("%s is a directory"),
	     Q_(path));
	  return (false);
	}
      SessionImpl::theSession->trace_access->WriteFormattedLine
	(T_("core"),
	 T_("accessing file %s: OK"),
	 Q_(path));
      return (true);
    }
  unsigned long error = ::GetLastError();
  if (! (error == ERROR_FILE_NOT_FOUND
	 || error == ERROR_PATH_NOT_FOUND))
    {
      FATAL_WINDOWS_ERROR (T_("GetFileAttributes"), path.Get());
    }
  SessionImpl::theSession->trace_access->WriteFormattedLine
    (T_("core"),
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
  unsigned long attributes = GetFileAttributes(path.Get());

  if (attributes == INVALID_FILE_ATTRIBUTES)
    {
      FATAL_WINDOWS_ERROR (T_("GetFileAttributes"), path.Get());
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
  SessionImpl::theSession->trace_files->WriteFormattedLine
    (T_("core"),
     T_("setting new attributes (%x) on %s"),
     static_cast<int>(nativeAttributes),
     Q_(path));

  if (! SetFileAttributes(path.Get(), static_cast<DWORD>(nativeAttributes)))
    {
      FATAL_WINDOWS_ERROR (T_("SetFileAttributes"), path.Get());
    }
}

/* _________________________________________________________________________

   File::GetSize
   _________________________________________________________________________ */

size_t
File::GetSize (/*[in]*/ const PathName &	path)
{
  HANDLE h =
    CreateFile(path.Get(),
	       GENERIC_READ,
	       FILE_SHARE_READ,
	       0,
	       OPEN_EXISTING,
	       FILE_ATTRIBUTE_NORMAL,
	       0);

  if (h == INVALID_HANDLE_VALUE)
    {
      FATAL_WINDOWS_ERROR (T_("CreateFile"), path.Get());
    }

  AutoHANDLE autoClose (h);

  unsigned long fileSize = GetFileSize(h, 0);

  if (fileSize == INVALID_FILE_SIZE)
    {
      FATAL_WINDOWS_ERROR (T_("GetFileSize"), path.Get());
    }

  return (fileSize);
}

/* _________________________________________________________________________

   LocalTime
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(struct tm)
LocalTime (/*[in]*/ time_t			time)
{
  struct tm timeStruct;
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
  if (_localtime64_s(&timeStruct, &time) != 0)
    {
      FATAL_CRT_ERROR (T_("_localtime64_s"), 0);
    }
#else
  struct tm * pTm = localtime(&time);
  if (pTm == 0)
    {
      FATAL_CRT_ERROR (T_("localtime"), 0);
    }
  timeStruct = *pTm;
#endif
  return (timeStruct);
}

/* _________________________________________________________________________

   CrtTimeToSystemTime
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(SYSTEMTIME)
CrtTimeToSystemTime (/*[in]*/ time_t	time)
{
  struct tm timeStruct = LocalTime(time);
  SYSTEMTIME systemTime;
  systemTime.wYear = static_cast<WORD>(timeStruct.tm_year + 1900);
  systemTime.wMonth = static_cast<WORD>(timeStruct.tm_mon + 1);
  systemTime.wDay = static_cast<WORD>(timeStruct.tm_mday);
  systemTime.wHour = static_cast<WORD>(timeStruct.tm_hour);
  systemTime.wMinute = static_cast<WORD>(timeStruct.tm_min);
  systemTime.wSecond = static_cast<WORD>(timeStruct.tm_sec);
  systemTime.wMilliseconds = 0;
  return (systemTime);
}

/* _________________________________________________________________________

   SystemTimeToCrtTime
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(time_t)
SystemTimeToCrtTime (/*[in]*/ const SYSTEMTIME &	systemTime)
{
  struct tm timeStruct;
  timeStruct.tm_year = systemTime.wYear - 1900;
  timeStruct.tm_mon = systemTime.wMonth - 1;
  timeStruct.tm_mday = systemTime.wDay;
  timeStruct.tm_hour = systemTime.wHour;
  timeStruct.tm_min = systemTime.wMinute;
  timeStruct.tm_sec = systemTime.wSecond;
  time_t time = mktime(&timeStruct);
  if (time == static_cast<time_t>(-1))
    {
      FATAL_CRT_ERROR (T_("mktime"), 0);
    }
  return (time);
}

/* _________________________________________________________________________

   CrtTimeToFileTime
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(FILETIME)
CrtTimeToFileTime (/*[in]*/ time_t	time)
{
  SYSTEMTIME systemTime = CrtTimeToSystemTime(time);
  FILETIME localFileTime;
  if (! SystemTimeToFileTime(&systemTime, &localFileTime))
    {
      FATAL_WINDOWS_ERROR (T_("SystemTimeToFileTime"), 0);
    }
  FILETIME fileTime;
  if (! LocalFileTimeToFileTime(&localFileTime, &fileTime))
    {
      FATAL_WINDOWS_ERROR (T_("LocalFileTimeToFileTime"), 0);
    }
  return (fileTime);
}

/* _________________________________________________________________________

   FileTimeToCrtTime
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(time_t)
FileTimeToCrtTime (/*[in]*/ FILETIME	fileTime)
{
  if (fileTime.dwLowDateTime == 0 && fileTime.dwHighDateTime == 0)
    {
      return (static_cast<time_t>(-1));
    }
  FILETIME localFileTime;
  if (! FileTimeToLocalFileTime(&fileTime, &localFileTime))
    {
      FATAL_WINDOWS_ERROR (T_("FileTimeToLocalFileTime"), 0);
    }
  SYSTEMTIME systemTime;
  if (! FileTimeToSystemTime(&localFileTime, &systemTime))
    {
      FATAL_WINDOWS_ERROR (T_("FileTimeToSystemTime"), 0);
    }
  return (SystemTimeToCrtTime(systemTime));
}


/* _________________________________________________________________________

   File::SetTimes
   _________________________________________________________________________ */

#define GET_OSFHANDLE(hf) \
  reinterpret_cast<HANDLE>(_get_osfhandle(static_cast<int>(hf)))

void
File::SetTimes (/*[in]*/ FILE *			stream,
		/*[in]*/ time_t			creationTime,
		/*[in]*/ time_t			lastAccessTime,
		/*[in]*/ time_t			lastWriteTime)
{
  FILETIME creationFileTime;
  FILETIME lastAccessFileTime;
  FILETIME lastWriteFileTime;
  if (creationTime != static_cast<time_t>(-1))
    {
      creationFileTime = CrtTimeToFileTime(creationTime);
    }
  if (lastAccessTime != static_cast<time_t>(-1))
    {
      lastAccessFileTime = CrtTimeToFileTime(lastAccessTime);
    }
  if (lastWriteTime != static_cast<time_t>(-1))
    {
      lastWriteFileTime = CrtTimeToFileTime(lastWriteTime);
    }
  if (! SetFileTime(GET_OSFHANDLE(_fileno(stream)),
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
      FATAL_WINDOWS_ERROR (T_("SetFileTime"), 0);
    }
}

/* _________________________________________________________________________

   File::SetTimes
   _________________________________________________________________________ */

#define GET_OSFHANDLE(hf) \
  reinterpret_cast<HANDLE>(_get_osfhandle(static_cast<int>(hf)))

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

   File::GetTimes
   _________________________________________________________________________ */

void
File::GetTimes (/*[in]*/ const PathName &	path,
		/*[out]*/ time_t &		creationTime,
		/*[out]*/ time_t &		lastAccessTime,
		/*[out]*/ time_t &		lastWriteTime)
{
  FileStream stream (File::Open(path,
				FileMode::Open,
				FileAccess::Read,
				false));
  FILETIME creationFileTime;
  FILETIME lastAccessFileTime;
  FILETIME lastWriteFileTime;
  if (! GetFileTime(GET_OSFHANDLE(_fileno(stream.Get())),
		    &creationFileTime,
		    &lastAccessFileTime,
		    &lastWriteFileTime))
    {
      FATAL_WINDOWS_ERROR (T_("GetFileTime"), 0);
    }
  stream.Close ();
  creationTime = FileTimeToCrtTime(creationFileTime);
  lastAccessTime = FileTimeToCrtTime(lastAccessFileTime);
  lastWriteTime = FileTimeToCrtTime(lastWriteFileTime);
}

/* _________________________________________________________________________

   File::Delete
   _________________________________________________________________________ */

void
File::Delete (/*[in]*/ const PathName &	path)
{
  SessionImpl::theSession->trace_files->WriteFormattedLine (T_("core"),
							    T_("deleting %s"),
							    Q_(path));
  if (! DeleteFile(path.Get()))
    {
      FATAL_WINDOWS_ERROR (T_("DeleteFile"), path.Get());
    }
}

/* _________________________________________________________________________

   File::Move
   _________________________________________________________________________ */

void
File::Move (/*[in]*/ const PathName &	source,
	    /*[in]*/ const PathName &	dest)
{
  SessionImpl::theSession->trace_files->WriteFormattedLine
    (T_("core"),
     T_("renaming %s to %s"),
     Q_(source),
     Q_(dest));

  if (! MoveFile(source.Get(), dest.Get()))
    {
      FATAL_WINDOWS_ERROR (T_("MoveFile"), source.Get());
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

  SessionImpl::theSession->trace_files->WriteFormattedLine
    (T_("core"),
     T_("copying %s to %s"),
     Q_(source),
     Q_(dest));

  if (! CopyFile(source.Get(), dest.Get(), FALSE))
    {
      FATAL_WINDOWS_ERROR (T_("CopyFile"), source.Get());
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
  SessionImpl::theSession->trace_files->WriteFormattedLine
    (T_("core"),
     T_("opening file %s (%d 0x%x %d %d)"),
     Q_(path),
     static_cast<int>(mode.Get()),
     static_cast<int>(access.Get()),
     static_cast<int>(share.Get()),
     static_cast<int>(isTextFile));

  int flags = 0;
  tstring strFlags;

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
	  strFlags += T_("a+");
	}
      else
	{
	  strFlags += T_("r+");
	}
    }
  else if (access == FileAccess::Read)
    {
      flags |= O_RDONLY;
      strFlags += T_("r");
    }
  else if (access == FileAccess::Write)
    {
      flags |= O_WRONLY;
      if (mode == FileMode::Append)
	{
	  strFlags += T_("a");
	}
      else
	{
	  flags |= O_TRUNC;
	  strFlags += T_("w");
	}
    }

#if defined(O_SEQUENTIAL)
  flags |= O_SEQUENTIAL;
#if 0
  strFlags += T_("S");
#endif
#endif

  if (isTextFile)
    {
      flags |= O_TEXT;
      strFlags += T_("t");
    }
  else
    {
      flags |= O_BINARY;
      strFlags += T_("b");
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
#  if (_MSC_VER >= 1400)
  if (_tsopen_s(&fd,
		path.Get(),
		flags,
		shflags,
		(((flags & O_CREAT) == 0) ? 0 : S_IREAD | S_IWRITE))
      != 0)
    {
      fd = -1;
    }
#else
  fd =
    _tsopen(path.Get(),
	    flags,
	    shflags,
	    (((flags & O_CREAT) == 0) ? 0 : S_IREAD | S_IWRITE));
#endif
  if (fd < 0)
    {
      FATAL_CRT_ERROR (T_("_tsopen"), path.Get());
    }
#else
  UNUSED_ALWAYS (shflags);
  fd =
    open(path.Get(),
	 flags,
	 (((flags & O_CREAT) == 0) ? 0 : S_IREAD | S_IWRITE));
  if (fd < 0)
    {
      FATAL_CRT_ERROR (T_("open"), path.Get());
    }
#endif

  return (FdOpen(fd, strFlags.c_str()));
}
