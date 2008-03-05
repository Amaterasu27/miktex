/* unx.cpp:

   Copyright (C) 1996-2008 Christian Schenk

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

   SessionImpl::GetMyProgramFile

   Get the fully qualified file name of the running executable (e.g.,
   /usr/local/bin/tex).
   _________________________________________________________________________ */

PathName
SessionImpl::GetMyProgramFile ()
{
  // we do this once
  if (myProgramFile.GetLength() == 0)
    {
      string invocationName = initInfo.GetProgramInvocationName();
      if (invocationName.length() == 0)
	{
	  FATAL_MIKTEX_ERROR ("SessionImpl::GetMyProgramFile",
			      T_("No invocation name has been set."),
			      0);
	}
      if (Utils::IsAbsolutePath(invocationName.c_str()))
	{
	  myProgramFile = invocationName;
	}
      else if (invocationName.length() > 3
	       && (invocationName.substr(0, 2) == "./"
		   || invocationName.substr(0, 3) == "../"))
	{
	  myProgramFile = GetFullPath(invocationName.c_str());
	}
      else
	{
	  string path;
	  if (! Utils::GetEnvironmentString("PATH", path))
	    {
	      FATAL_MIKTEX_ERROR ("SessionImpl::GetMyProgramFile",
				  T_("\
The environment variable PATH is not defined."),
				  0);
	    }
	  if (path.find(RECURSION_INDICATOR) != string::npos
	      || path.find(TEXMF_PLACEHOLDER) != string::npos)
	    {
	      FATAL_MIKTEX_ERROR ("SessionImpl::GetMyProgramFile",
				  T_("\
The environment variable PATH has an obscure value."),
				  path.c_str());
	    }
	  if (! SlowFindFile(invocationName.c_str(),
			     path.c_str(),
			     myProgramFile))
	    {
	      FATAL_MIKTEX_ERROR ("SessionImpl::GetMyProgramFile",
				  T_("\
The invoked program could not be found in the PATH."),
				  0);
	    }
	}
    }
  return (myProgramFile);
}

/* _________________________________________________________________________

   SessionImpl::DefaultConfig
   _________________________________________________________________________ */

StartupConfig
SessionImpl::DefaultConfig (/*[in]*/ bool shared)
{
  UNUSED_ALWAYS (shared);
  StartupConfig ret;
  string home;
  if (! Utils::GetEnvironmentString("HOME", home))
    {
      FATAL_MIKTEX_ERROR ("SessionImpl::DefaultConfig",
			  T_("Environment variable HOME is not set."),
			  0);
    }
  PathName defaultPath (home);
  defaultPath += ".miktex";
#if defined(MIKTEX_ROOTS)
  ret.roots = MIKTEX_ROOTS;
#endif
#if defined(MIKTEX_INSTALLROOT)
  ret.installRoot = MIKTEX_INSTALLROOT;
#endif
  if (ret.installRoot.Empty())
    {
      ret.installRoot = defaultPath;
    }
#if defined(MIKTEX_DATAROOT)
  ret.userDataRoot = MIKTEX_DATAROOT;
#endif
  if (ret.userDataRoot.Empty())
    {
      ret.userDataRoot = defaultPath;
    }
#if defined(MIKTEX_CONFIGROOT)
  ret.userConfigRoot = MIKTEX_CONFIGROOT;
#endif
  if (ret.userConfigRoot.Empty())
    {
      ret.userConfigRoot = defaultPath;
    }
  return (ret);
}

/* _________________________________________________________________________

   SessionImpl::GetPsFontDirs
   _________________________________________________________________________ */

bool
SessionImpl::GetPsFontDirs (/*[out]*/ string &	psFontDirs)
{
#  warning Unimplemented: SessionImpl::GetPsFontDirs
  return (false);
}

/* _________________________________________________________________________

   SessionImpl::GetTTFDirs
   _________________________________________________________________________ */

bool
SessionImpl::GetTTFDirs (/*[out]*/ string &	ttfDirs)
{
#  warning Unimplemented: SessionImpl::GetTTFDirs
  return (false);
}

/* _________________________________________________________________________

   SessionImpl::GetOTFDirs
   _________________________________________________________________________ */

bool
SessionImpl::GetOTFDirs (/*[out]*/ string &	otfDirs)
{
#  warning Unimplemented: SessionImpl::GetOTFDirs
  return (false);
}

/* _________________________________________________________________________

   Utils::GetOSVersionString
   _________________________________________________________________________ */

string
Utils::GetOSVersionString ()
{
  string version;
#if defined(HAVE_UNAME_SYSCALL)
  struct utsname buf;
  if (uname(&buf) < 0)
    {
      FATAL_CRT_ERROR ("uname", 0);
    }
  version = buf.sysname;
  version += ' ';
  version += buf.release;
  version += ' ';
  version += buf.version;
  version += ' ';
  version += buf.machine;
#else
#  warning Unimplemented: Utils::GetOSVersionString
  version = "UnkOS 0.1";
#endif
  return (version);
}

/* _________________________________________________________________________

   PathName::SetToCurrentDirectory
   _________________________________________________________________________ */

PathName &
PathName::SetToCurrentDirectory ()
{
  if (getcwd(GetBuffer(), GetCapacity()) == 0)
    {
      FATAL_CRT_ERROR ("getcwd", 0);
    }
  return (*this);
}

/* _________________________________________________________________________

   PathName::SetToTempDirectory
   _________________________________________________________________________ */

PathName &
PathName::SetToTempDirectory ()
{
  if (! Utils::GetEnvironmentString("TMPDIR", GetBuffer(), GetCapacity()))
    {
#if defined(P_tmpdir)
      Utils::CopyString (GetBuffer(), GetCapacity(), P_tmpdir);
#else
      Utils::CopyString (GetBuffer(), GetCapacity(), "/tmp");
#endif
    }
  return (*this);
}

/* _________________________________________________________________________

   PathName::SetToTempFile
   _________________________________________________________________________ */

PathName &
PathName::SetToTempFile ()
{
  *this = SessionImpl::theSession->GetTempDirectory();

  AppendComponent ("mikXXXXXX");

  int fd = mkstemp(GetBuffer());

  if (fd < 0)
    {
      FATAL_CRT_ERROR ("mkstemp", 0);
    }

  close (fd);

  SessionImpl::theSession->trace_tempfile->WriteFormattedLine
    ("core",
     T_("created temporary file %s"),
     Q_(GetBuffer()));

  return (*this);
}

/* _________________________________________________________________________

   FileIsOnROMedia
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(bool)
FileIsOnROMedia (/*[in]*/ const char * lpszPath)
{
#if defined(HAVE_STATVFS)
  struct statvfs buf;
  if (statvfs(lpszPath, &buf) < 0)
    {
      FATAL_CRT_ERROR ("statvfs", lpszPath);
    }
  return ((buf.f_flag & ST_RDONLY) != 0);
#else
#  warning Unimplemented: FileIsOnROMedia()
  UNUSED_ALWAYS (lpszPath);
  return (false);
#endif
}

/* _________________________________________________________________________

   CreateDirectoryPath

   Create a directory path.
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(void)
CreateDirectoryPath (/*[in]*/ const char *	lpszPath,
		     /*[in]*/ mode_t			mode)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  if (! Utils::IsAbsolutePath(lpszPath))
    {
      PathName path (lpszPath);
      path.MakeAbsolute ();
      CreateDirectoryPath (path.Get(), mode);
    }

  // do nothing, if the directory already exists
  if (Directory::Exists(lpszPath))
    {
      return;
    }

  // create the parent directory
  char szDir[BufferSizes::MaxPath];
  char szFname[BufferSizes::MaxPath];
  char szExt[BufferSizes::MaxPath];
  PathName::Split (lpszPath,
		   szDir, BufferSizes::MaxPath,
		   szFname, BufferSizes::MaxPath,
		   szExt, BufferSizes::MaxPath);
  PathName pathParent (szDir, 0, 0);
  RemoveDirectoryDelimiter (pathParent.GetBuffer());
  // <recursivecall>
  CreateDirectoryPath (pathParent.GetBuffer(), mode);
  // </recursivecall>

  // we're done, if szFname is empty (this happens when lpszPath ends
  // with a directory delimiter)
  if (szFname[0] == 0 && szExt[0] == 0)
    {
      return;
    }

  SessionImpl::theSession->trace_config->WriteFormattedLine
    ("core",
     T_("creating directory %s..."),
     Q_(lpszPath));

  // create the directory itself
  if (mkdir(lpszPath, mode) != 0)
    {
      FATAL_CRT_ERROR ("mkdir", lpszPath);
    }
}

/* _________________________________________________________________________

   CreateDirectoryPath

   Create a directory path for the current user.
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(void)
CreateDirectoryPath (/*[in]*/ const char *	lpszPath)
{
  ::CreateDirectoryPath (lpszPath,
			 S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
}

/* _________________________________________________________________________

   CreateDirectoryPathForEveryone

   Create a directory path for the world group.
   _________________________________________________________________________ */

#if 0
MIKTEXINTERNALFUNC(void)
CreateDirectoryPathForEveryone (/*[in]*/ const char * lpszPath)
{
  ::CreateDirectoryPath (lpszPath,
			 S_IRWXU | S_IRWXG | S_IRWXO);
}
#endif

/* _________________________________________________________________________

   Utils::SetEnvironmentString
   _________________________________________________________________________ */

void
Utils::SetEnvironmentString (/*[in]*/ const char *	lpszValueName,
			     /*[in]*/ const char *	lpszValue)
{
  const char * lpszOldValue = ::GetEnvironmentString(lpszValueName);
  if (lpszOldValue != 0 && StringCompare(lpszOldValue, lpszValue, false) == 0)
    {
      return;
    }
  SessionImpl::theSession->trace_config->WriteFormattedLine
    ("core",
     T_("setting env %s=%s"),
     lpszValueName,
     lpszValue);
  if (setenv(lpszValueName, lpszValue, 1) != 0)
    {
      FATAL_CRT_ERROR ("setenv", lpszValueName);
    }
}

/* _________________________________________________________________________

   SessionImpl::RunningAsAdministrator
   _________________________________________________________________________ */

bool
SessionImpl::RunningAsAdministrator ()
{
  return (geteuid() == 0);
}

/* _________________________________________________________________________

   SessionImpl::IsUserAnAdministrator
   _________________________________________________________________________ */

bool
SessionImpl::IsUserAnAdministrator ()
{
  return (getuid() == 0 || geteuid() == 0);
}

/* _________________________________________________________________________

   Utils::CheckHeap
   _________________________________________________________________________ */

void
Utils::CheckHeap ()
{
}
