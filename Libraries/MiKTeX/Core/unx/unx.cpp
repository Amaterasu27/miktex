/* unx.cpp:

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
      tstring invocationName = initInfo.GetProgramInvocationName();
      if (invocationName.length() == 0)
	{
	  FATAL_MIKTEX_ERROR (T_("SessionImpl::GetMyProgramFile"),
			      T_("No invocation name has been set."),
			      0);
	}
      if (Utils::IsAbsolutePath(invocationName.c_str()))
	{
	  myProgramFile = invocationName;
	}
      else if (invocationName.length() > 3
	       && (invocationName.substr(0, 2) == T_("./")
		   || invocationName.substr(0, 3) == T_("../")))
	{
	  myProgramFile = GetFullPath(invocationName.c_str());
	}
      else
	{
	  tstring path;
	  if (! Utils::GetEnvironmentString(T_("PATH"), path))
	    {
	      FATAL_MIKTEX_ERROR (T_("SessionImpl::GetMyProgramFile"),
				  T_("\
The environment variable PATH is not defined."),
				  0);
	    }
	  if (path.find(RECURSION_INDICATOR) != tstring::npos
	      || path.find(TEXMF_PLACEHOLDER) != tstring::npos)
	    {
	      FATAL_MIKTEX_ERROR (T_("SessionImpl::GetMyProgramFile"),
				  T_("\
The environment variable PATH has an obscure value."),
				  path.c_str());
	    }
	  if (! SlowFindFile(invocationName.c_str(),
			     path.c_str(),
			     myProgramFile))
	    {
	      FATAL_MIKTEX_ERROR (T_("SessionImpl::GetMyProgramFile"),
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
  tstring home;
  if (! Utils::GetEnvironmentString(T_("HOME"), home))
    {
      FATAL_MIKTEX_ERROR (T_("SessionImpl::DefaultConfig"),
			  T_("Environment variable HOME is not set."),
			  0);
    }
  PathName defaultPath (home);
  defaultPath += T_(".miktex");
#if defined(MIKTEX_ROOTS)
  ret.roots = MIKTEX_ROOTS;
#endif
#if defined(MIKTEX_INSTALLROOT)
  ret.installRoot = T_(MIKTEX_INSTALLROOT);
#endif
  if (ret.installRoot.Empty())
    {
      ret.installRoot = defaultPath;
    }
#if defined(MIKTEX_DATAROOT)
  ret.userDataRoot = T_(MIKTEX_DATAROOT);
#endif
  if (ret.userDataRoot.Empty())
    {
      ret.userDataRoot = defaultPath;
    }
#if defined(MIKTEX_CONFIGROOT)
  ret.userConfigRoot = T_(MIKTEX_CONFIGROOT);
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
SessionImpl::GetPsFontDirs (/*[out]*/ tstring &	psFontDirs)
{
#  warning Unimplemented: SessionImpl::GetPsFontDirs
  return (false);
}

/* _________________________________________________________________________

   SessionImpl::GetTTFDirs
   _________________________________________________________________________ */

bool
SessionImpl::GetTTFDirs (/*[out]*/ tstring &	ttfDirs)
{
#  warning Unimplemented: SessionImpl::GetPsTtfDirs
  return (false);
}

/* _________________________________________________________________________

   Utils::GetOSVersionString
   _________________________________________________________________________ */

tstring
Utils::GetOSVersionString ()
{
  tstring version;
#if defined(HAVE_UNAME_SYSCALL)
  struct utsname buf;
  if (uname(&buf) < 0)
    {
      FATAL_CRT_ERROR (T_("uname"), 0);
    }
  version = buf.sysname;
  version += T_(' ');
  version += buf.release;
  version += T_(' ');
  version += buf.version;
  version += T_(' ');
  version += buf.machine;
#else
#  warning Unimplemented: Utils::GetOSVersionString
  version = T_("UnkOS 0.1");
#endif
  return (version);
}

/* _________________________________________________________________________

   PathName::SetToCurrentDirectory
   _________________________________________________________________________ */

PathName &
PathName::SetToCurrentDirectory ()
{
  if (getcwd(buffer, GetSize()) == 0)
    {
      FATAL_CRT_ERROR (T_("getcwd"), 0);
    }
  return (*this);
}

/* _________________________________________________________________________

   PathName::SetToTempDirectory
   _________________________________________________________________________ */

PathName &
PathName::SetToTempDirectory ()
{
  if (! Utils::GetEnvironmentString(T_("TMPDIR"), buffer, GetSize()))
    {
#if defined(P_tmpdir)
      Utils::CopyString (buffer, GetSize(), P_tmpdir);
#else
      Utils::CopyString (buffer, GetSize(), T_("/tmp"));
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

  AppendComponent (T_("mikXXXXXX"));

  int fd = mkstemp(buffer);

  if (fd < 0)
    {
      FATAL_CRT_ERROR (T_("mkstemp"), 0);
    }

  close (fd);

  SessionImpl::theSession->trace_tempfile->WriteFormattedLine
    (T_("core"),
     T_("created temporary file \"%s\""),
     buffer);

  return (*this);
}

/* _________________________________________________________________________

   FileIsOnROMedia
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(bool)
FileIsOnROMedia (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
#if defined(HAVE_STATVFS)
  struct statvfs buf;
  if (statvfs(lpszPath, &buf) < 0)
    {
      FATAL_CRT_ERROR (T_("statvfs"), lpszPath);
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
CreateDirectoryPath (/*[in]*/ const MIKTEXCHAR *	lpszPath,
		     /*[in]*/ mode_t			mode)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  if (! Utils::IsAbsolutePath(lpszPath))
    {
      INVALID_ARGUMENT (T_("CreateDirectoryPath"), lpszPath);
    }

  // do nothing, if the directory already exists
  if (Directory::Exists(lpszPath))
    {
      return;
    }

  // create the parent directory
  MIKTEXCHAR szDir[BufferSizes::MaxPath];
  MIKTEXCHAR szFname[BufferSizes::MaxPath];
  MIKTEXCHAR szExt[BufferSizes::MaxPath];
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
    (T_("core"),
     T_("creating directory %s..."),
     Q_(lpszPath));

  // create the directory itself
  if (mkdir(lpszPath, mode) != 0)
    {
      FATAL_CRT_ERROR (T_("mkdir"), lpszPath);
    }
}

/* _________________________________________________________________________

   CreateDirectoryPath

   Create a directory path for the current user.
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(void)
CreateDirectoryPath (/*[in]*/ const MIKTEXCHAR *	lpszPath)
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
CreateDirectoryPathForEveryone (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  ::CreateDirectoryPath (lpszPath,
			 S_IRWXU | S_IRWXG | S_IRWXO);
}
#endif

/* _________________________________________________________________________

   Utils::SetEnvironmentString
   _________________________________________________________________________ */

void
Utils::SetEnvironmentString (/*[in]*/ const MIKTEXCHAR *	lpszValueName,
			     /*[in]*/ const MIKTEXCHAR *	lpszValue)
{
  SessionImpl::theSession->trace_config->WriteFormattedLine
    (T_("core"),
     T_("setting env %s=%s"),
     lpszValueName,
     lpszValue);
  if (setenv(lpszValueName, lpszValue, 1) != 0)
    {
      FATAL_CRT_ERROR (T_("setenv"), lpszValueName);
    }
}

/* _________________________________________________________________________

   SessionImpl::RunningAsAdministrator
   _________________________________________________________________________ */

bool
MIKTEXCALL
SessionImpl::RunningAsAdministrator ()
{
  return (geteuid() == 0);
}

/* _________________________________________________________________________

   SessionImpl::IsUserAnAdministrator
   _________________________________________________________________________ */

bool
MIKTEXCALL
SessionImpl::IsUserAnAdministrator ()
{
  return (getuid() == 0 || geteuid() == 0);
}
