/* unxFile.cpp: file operations

   Copyright (C) 1996-2007 Christian Schenk

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
  if (chdir(path.Get()) != 0)
    {
      FATAL_CRT_ERROR ("chdir", path.Get());
    }
}

/* _________________________________________________________________________

   Directory::Exists
   _________________________________________________________________________ */

bool
Directory::Exists (/*[in]*/ const PathName & path)
{
  struct stat statbuf;
  if (stat(path.Get(), &statbuf) == 0)
    {
      if (S_ISDIR(statbuf.st_mode) == 0)
	{
	  SessionImpl::theSession->trace_access->WriteFormattedLine
	    ("core",
	     T_("%s is not a directory"),
	     Q_(path));
	  return (false);
	}
      return (true);
    }
  int error = errno;
  if (error != ENOENT)
    {
      FATAL_CRT_ERROR ("stat", path.Get());
    }
  return (false);
}

/* _________________________________________________________________________

   Directory::Delete
   _________________________________________________________________________ */

void
Directory::Delete (/*[in]*/ const PathName & path)
{
  SessionImpl::theSession->trace_files->WriteFormattedLine
    ("core",
     T_("deleting directory %s"),
     Q_(path));

  if (rmdir(path.Get()) != 0)
    {
      FATAL_CRT_ERROR ("rmdir", path.Get());
    }
}

/* _________________________________________________________________________

   File::Exists
   _________________________________________________________________________ */

bool
File::Exists (/*[in]*/ const PathName & path)
{
  struct stat statbuf;
  if (stat(path.Get(), &statbuf) == 0)
    {
      if (S_ISDIR(statbuf.st_mode) != 0)
	{
	  SessionImpl::theSession->trace_access->WriteFormattedLine
	    ("core",
	     T_("%s is a directory"),
	     Q_(path));
	  return (false);
	}
      SessionImpl::theSession->trace_access->WriteFormattedLine
	("core",
	 T_("accessing file %s: OK"),
	 Q_(path));
      return (true);
    }
  int error = errno;
  if (error != ENOENT)
    {
      FATAL_CRT_ERROR ("stat", path.Get());
    }
  SessionImpl::theSession->trace_access->WriteFormattedLine
    ("core",
     T_("accessing file %s: NOK"),
     Q_(path));
  return (false);
}

/* _________________________________________________________________________

   File::GetAttributes
   _________________________________________________________________________ */

FileAttributes
File::GetAttributes (/*[in]*/ const PathName & path)
{
  mode_t attributes = static_cast<mode_t>(GetNativeAttributes(path));

  FileAttributes result (FileAttributes::Normal);

  if (S_ISDIR(attributes) != 0)
    {
      result |= FileAttributes::Directory;
    }

  if (((attributes & S_IWUSR) == 0)
      || ((attributes & S_IWGRP) == 0)
      || ((attributes & S_IWOTH) == 0))
    {
      result |= FileAttributes::ReadOnly;
    }

  return (result);
}

/* _________________________________________________________________________

   File::GetNativeAttributes
   _________________________________________________________________________ */

unsigned long
File::GetNativeAttributes (/*[in]*/ const PathName & path)
{
  struct stat statbuf;

  if (stat(path.Get(), &statbuf) != 0)
    {
      FATAL_CRT_ERROR ("stat", path.Get());
    }

  return (static_cast<unsigned long>(statbuf.st_mode));
}

/* _________________________________________________________________________

   File::SetAttributes
   _________________________________________________________________________ */

void
File::SetAttributes (/*[in]*/ const PathName &		path,
		     /*[in]*/ FileAttributes		attributes)
{
  mode_t oldAttributes = static_cast<mode_t>(GetNativeAttributes(path));

  mode_t newAttributes = oldAttributes;

  bool writable =
    (((oldAttributes & S_IWUSR) != 0)
     && ((oldAttributes & S_IWGRP) != 0)
     && ((oldAttributes & S_IWOTH) != 0));
	 
  if ((attributes & FileAttributes::ReadOnly) != 0 && writable)
    {
      newAttributes &= ~ (S_IWUSR | S_IWGRP | S_IWOTH);
    }
  else if ((attributes & FileAttributes::ReadOnly) == 0 && ! writable)
    {
      newAttributes |= (S_IWUSR | S_IWGRP | S_IWOTH);
    }

  if (newAttributes == oldAttributes)
    {
      return;
    }

  SetNativeAttributes (path, static_cast<unsigned long>(newAttributes));
}

/* _________________________________________________________________________

   File::SetNativeAttributes
   _________________________________________________________________________ */

void
File::SetNativeAttributes (/*[in]*/ const PathName &	path,
			   /*[in]*/ unsigned long	nativeAttributes)
{
  SessionImpl::theSession->trace_files->WriteFormattedLine
    ("core",
     T_("setting new attributes (%x) on %s"),
     static_cast<int>(nativeAttributes),
     Q_(path));

  if (chmod(path.Get(), static_cast<mode_t>(nativeAttributes)) != 0)
    {
      FATAL_CRT_ERROR ("chmod", path.Get());
    }
}

/* _________________________________________________________________________

   File::GetSize
   _________________________________________________________________________ */

size_t
File::GetSize (/*[in]*/ const PathName &	path)
{
  struct stat statbuf;
  if (stat(path.Get(), &statbuf) != 0)
    {
      FATAL_CRT_ERROR ("stat", path.Get());
    }
  return (statbuf.st_size);
}

/* _________________________________________________________________________

   File::SetTimes
   _________________________________________________________________________ */

void
File::SetTimes (/*[in]*/ int			fd,
		/*[in]*/ time_t			creationTime,
		/*[in]*/ time_t			lastAccessTime,
		/*[in]*/ time_t			lastWriteTime)
{
  UNUSED_ALWAYS (fd);
  UNUSED_ALWAYS (creationTime);
  UNUSED_ALWAYS (lastAccessTime);
  UNUSED_ALWAYS (lastWriteTime);
  UNIMPLEMENTED ("File::SetTimes");
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
  UNUSED_ALWAYS (stream);
  UNUSED_ALWAYS (creationTime);
  UNUSED_ALWAYS (lastAccessTime);
  UNUSED_ALWAYS (lastWriteTime);
  UNIMPLEMENTED ("File::SetTimes");
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
  UNUSED_ALWAYS (creationTime);
  utimbuf times;
  time_t now = time(0);
  if (lastAccessTime == static_cast<time_t>(-1))
    {
      lastAccessTime = now;
    }
  if (lastWriteTime == static_cast<time_t>(-1))
    {
      lastWriteTime = now;
    }
  times.actime = lastAccessTime;
  times.modtime = lastWriteTime;
  if (utime(path.Get(), &times) != 0)
    {
      FATAL_CRT_ERROR ("utimes", path.Get());
    }
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
  struct stat stat_;
  if (stat(path.Get(), &stat_) != 0)
    {
      FATAL_CRT_ERROR ("stat", path.Get());
    }
  creationTime = stat_.st_ctime;
  lastAccessTime = stat_.st_atime;
  lastWriteTime = stat_.st_mtime;
}

/* _________________________________________________________________________

   File::Delete
   _________________________________________________________________________ */

void
File::Delete (/*[in]*/ const PathName & path)
{
  SessionImpl::theSession->trace_files->WriteFormattedLine ("core",
							    T_("deleting %s"),
							    Q_(path));

  if (remove(path.Get()) != 0)
    {
      FATAL_CRT_ERROR ("remove", path.Get());
    }
}

/* _________________________________________________________________________

   File::Move
   _________________________________________________________________________ */

void
File::Move (/*[in]*/ const PathName &	source,
	    /*[in]*/ const PathName &	dest)
{
  struct stat sourceStat;
  if (stat(source.Get(), &sourceStat) != 0)
    {
      FATAL_CRT_ERROR ("stat", source.Get());
    }

  PathName destDir (dest);
  destDir.MakeAbsolute ();
  destDir.RemoveFileSpec ();
  struct stat destStat;
  if (stat(destDir.Get(), &destStat) != 0)
    {
      FATAL_CRT_ERROR ("stat", destDir.Get());
    }
  
  bool sameDevice = (sourceStat.st_dev == destStat.st_dev);

  if (sameDevice)
    {
      SessionImpl::theSession->trace_files->WriteFormattedLine
	("core",
	 T_("renaming %s to %s"),
	 Q_(source),
	 Q_(dest));
      if (rename(source.Get(), dest.Get()) != 0)
	{
	  FATAL_CRT_ERROR ("rename", source.Get());
	}
    }
  else
    {
      Copy (source, dest);
      try
	{
	  Delete (source);
	}
      catch (const MiKTeXException &)
	{
	  try
	    {
	      if (Exists(source))
		{
		  Delete (dest);
		}
	    }
	  catch (const MiKTeXException &)
	    {
	    }
	  throw;
	}
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
  SessionImpl::theSession->trace_files->WriteFormattedLine
    ("core",
     T_("copying %s to %s"),
     Q_(source),
     Q_(dest));

  struct stat sourceStat;

  if (preserveAttributes)
    {
      if (stat(source.Get(), &sourceStat) != 0)
	{
	  FATAL_CRT_ERROR ("stat", source.Get());
	}
    }
  
  FileStream sourceStream (File::Open(source,
				      FileMode::Open,
				      FileAccess::Read,
				      false));

  try
    {
      FileStream destStream (File::Open(dest,
					FileMode::Create,
					FileAccess::Write,
					false));
      
      char buffer[4096];
      size_t n;
      while ((n = sourceStream.Read(buffer, 4096)) > 0)
	{
	  destStream.Write (buffer, n);
	}
      
      sourceStream.Close ();
      destStream.Close ();

      if (preserveAttributes)
	{
	  SetNativeAttributes (dest,
			       static_cast<unsigned long>(sourceStat.st_mode));
	  
#if defined(HAVE_CHOWN)
	  if (chown(dest.Get(), sourceStat.st_uid, sourceStat.st_gid) != 0)
	    {
	      FATAL_CRT_ERROR ("chown", dest.Get());
	    }
#endif
	  
	  SetTimes (dest.Get(),
		    sourceStat.st_ctime,
		    sourceStat.st_atime,
		    sourceStat.st_mtime);
	}
    }
  catch (const MiKTeXException &)
    {
      try
	{
	  if (Exists(dest))
	    {
	      Delete (dest, true);
	    }
	}
      catch (const MiKTeXException &)
	{
	}
      throw;
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
  UNUSED_ALWAYS (isTextFile);
  UNUSED_ALWAYS (share);

  SessionImpl::theSession->trace_files->WriteFormattedLine
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

  int fd;

  fd =
    open(path.Get(),
	 flags,
	 (((flags & O_CREAT) == 0)
	  ? 0
	  : (0
	     | S_IRUSR | S_IWUSR
	     | S_IRGRP
	     | S_IROTH
	     | 0)));

  if (fd < 0)
    {
      FATAL_CRT_ERROR ("open", path.Get());
    }

  try
    {
      return (FdOpen(fd, strFlags.c_str()));
    }
  catch (const exception &)
    {
      close (fd);
      throw;
    }
}
