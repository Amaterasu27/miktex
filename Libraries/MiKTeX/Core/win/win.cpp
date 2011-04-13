/* win.cpp:

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
#include "core-version.h"

#include "miktex/Core/Registry"
#include "win/winRegistry.h"

#include <miktex/Core/win/DllProc>

#include <regstr.h>

#define SET_SECURITY 1

/* _________________________________________________________________________

   Utils::GetFolderPath
   _________________________________________________________________________ */

PathName
Utils::GetFolderPath (/*[in]*/ int	nFolder,
		      /*[in]*/ int	nFallbackFolder,
		      /*[in]*/ bool	getCurrentPath)
{
  DWORD flags =
    ( 0
      | (getCurrentPath
	 ? SHGFP_TYPE_CURRENT
	 : SHGFP_TYPE_DEFAULT)
      | 0);
  wchar_t szPath[MAX_PATH];
  HRESULT hr =
    SHGetFolderPathW(0,
	  nFolder | CSIDL_FLAG_CREATE,
	  0,
	  flags,
	  szPath);
  if (hr == E_INVALIDARG && (nFolder != nFallbackFolder))
    {
      hr =
	SHGetFolderPathW(0,
	      nFallbackFolder | CSIDL_FLAG_CREATE,
	      0,
	      flags,
	      szPath);
    }
  if (hr == E_FAIL)
    {
      FATAL_MIKTEX_ERROR ("Utils::GetFolderPath",
			  T_("A required file system folder does not exist."),
			  NUMTOSTR(nFolder));
    }
  if (hr == E_INVALIDARG)
    {
      FATAL_MIKTEX_ERROR ("Utils::GetFolderPath",
			  T_("Unsupported Windows product."),
			  NUMTOSTR(nFolder));
    }
  if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
  {
    FATAL_MIKTEX_ERROR ("Utils::GetFolderPath",
      T_("A required file system folder cannot be accessed."),
      NUMTOSTR(nFolder));
  }
  if (hr != S_OK)
    {
      FATAL_MIKTEX_ERROR ("Utils::GetFolderPath",
			  T_("A required file system path could not be retrieved."),
			  (string(NUMTOSTR(nFolder)) + " (hr=" + NUMTOHEXSTR(hr) + ")").c_str());
    }
  return (szPath);
}

/* _________________________________________________________________________

   SessionImpl::GetMyProgramFile

   Get the fully qualified file name of the running executable (e.g.,
   C:\texmf\miktex\bin\tex.exe).
   _________________________________________________________________________ */

PathName
SessionImpl::GetMyProgramFile (/*[in]*/ bool canonicalized)
{
  // we do this once
  if (myProgramFile.GetLength() == 0)
    {
      wchar_t szPath[BufferSizes::MaxPath];
      DWORD n =
	GetModuleFileNameW(0,
			   szPath,
			   BufferSizes::MaxPath);
      if (n == 0)
	{
	  FATAL_WINDOWS_ERROR ("GetModuleFileNameW", 0);
	}
      if (n == BufferSizes::MaxPath)
      {
	BUF_TOO_SMALL ("SessionImpl::GetMyProgramFile");
      }
      myProgramFile = szPath;
      myProgramFileCanon = myProgramFile;
      myProgramFileCanon.Canonicalize ();
    }
  if (canonicalized)
    {
      return (myProgramFileCanon);
    }
  else
    {
      return (myProgramFile);
    }
}

/* _________________________________________________________________________

   SessionImpl::GetDllPathName
   _________________________________________________________________________ */

#if ! defined(MIKTEX_STATIC)
PathName
SessionImpl::GetDllPathName (/*[in]*/ bool canonicalized)
{
  // we do this once
  if (dllPathName.GetLength() == 0)
    {
      wchar_t szPath[BufferSizes::MaxPath];
      DWORD n = GetModuleFileNameW(
	hinstDLL,
	szPath,
	BufferSizes::MaxPath);
      if (n == 0)
	{
	  FATAL_WINDOWS_ERROR ("GetModuleFileNameW", 0);
	}
      if (n == BufferSizes::MaxPath)
      {
	BUF_TOO_SMALL ("SessionImpl::GetDllPathName");
      }
      dllPathName = szPath;
      dllPathNameCanon = dllPathName;
      dllPathNameCanon.Canonicalize ();
    }
  if (canonicalized)
    {
      return (dllPathNameCanon);
    }
  else
    {
      return (dllPathName);
    }
}

#endif

/* _________________________________________________________________________

   SessionImpl::DefaultConfig

   UserInstall:	  %USERPROFILE%\AppData\Roaming\MiKTeX\X.Y\
   UserConfig:	  %USERPROFILE%\AppData\Roaming\MiKTeX\X.Y\
   UserData:	  %USERPROFILE%\AppData\Local\MiKTeX\X.Y\
   CommonInstall: C:\Program Files\MiKTeX X.Y\
   CommonConfig:  C:\ProgramData\MiKTeX\X.Y\
   CommonData:	  C:\ProgramData\MiKTeX\X.Y\
   _________________________________________________________________________ */

StartupConfig
SessionImpl::DefaultConfig (/*[in]*/ MiKTeXConfiguration config,
			    /*[in]*/ const PathName &    commonPrefixArg,
			    /*[in]*/ const PathName &    userPrefixArg)
{
  StartupConfig ret;
  if (config == MiKTeXConfiguration::None)
  {
    config = MiKTeXConfiguration::Regular;
  }
  ret.config = config;
  if (config == MiKTeXConfiguration::Portable)
  {
    PathName commonPrefix (commonPrefixArg);
    PathName userPrefix (userPrefixArg);
    if (commonPrefix.Empty() && ! userPrefix.Empty())
    {
      commonPrefix = userPrefix;
    }
    else if (userPrefix.Empty() && ! commonPrefix.Empty())
    {
      userPrefix = commonPrefix;
    }
    if (! commonPrefix.Empty())
    {
      ret.commonConfigRoot = commonPrefix;
      ret.commonDataRoot = commonPrefix;
      ret.commonInstallRoot = commonPrefix;
    }
    if (! userPrefix.Empty())
    {
      ret.userConfigRoot = userPrefix;
      ret.userDataRoot = userPrefix;
      ret.userInstallRoot = userPrefix;
    }
    if (userPrefix.Empty() || commonPrefix.Empty())
    {
      PathName myloc (GetMyLocation(false));
      PathName prefix;
      if (Utils::GetPathNamePrefix(myloc, MIKTEX_PATH_INTERNAL_BIN_DIR, prefix)
	|| Utils::GetPathNamePrefix(myloc, MIKTEX_PATH_BIN_DIR, prefix)
	|| Utils::GetPathNamePrefix(myloc, MIKTEX_PATH_MIKTEX_TEMP_DIR, prefix))
      {
	if (commonPrefix.Empty())
	{
	  ret.commonConfigRoot = prefix;
	  ret.commonDataRoot = prefix;
	  ret.commonInstallRoot = prefix;
	}
	if (userPrefix.Empty())
	{
	  ret.userConfigRoot = prefix;
	  ret.userDataRoot = prefix;
	  ret.userInstallRoot = prefix;
	}
      }
    }
  }
  else
  {
    string product;
    if (config == MiKTeXConfiguration::Direct)
    {
      product = "MiKTeXDirect";
      PathName myloc (GetMyLocation(false));
      PathName prefix;
      if (! Utils::GetPathNamePrefix(myloc, MIKTEX_PATH_BIN_DIR, prefix))
      {
	UNEXPECTED_CONDITION ("SessionImpl::DefaultConfig");
      }
      ret.commonInstallRoot = prefix;
    }
    else
    {
      product = "MiKTeX";
      wchar_t szProgramFiles[MAX_PATH];
      if (SHGetFolderPathW(0, CSIDL_PROGRAM_FILES, 0, SHGFP_TYPE_CURRENT, szProgramFiles) == S_OK)
      {
	ret.commonInstallRoot = szProgramFiles;
	ret.commonInstallRoot += "MiKTeX" " " MIKTEX_SERIES_STR;
      }
    }
    wchar_t szPath[MAX_PATH];
    if (SHGetFolderPathW(0, CSIDL_COMMON_APPDATA, 0, SHGFP_TYPE_CURRENT, szPath) == S_OK)
    {
      ret.commonDataRoot =szPath;
      ret.commonDataRoot += product;
      ret.commonDataRoot += MIKTEX_SERIES_STR;
    }
    ret.commonConfigRoot = ret.commonDataRoot;
    if (SHGetFolderPathW(0, CSIDL_LOCAL_APPDATA, 0, SHGFP_TYPE_CURRENT, szPath) == S_OK
        || SHGetFolderPathW(0, CSIDL_APPDATA, 0, SHGFP_TYPE_CURRENT, szPath) == S_OK)
    {
      ret.userDataRoot = szPath;
      ret.userDataRoot += product;
      ret.userDataRoot += MIKTEX_SERIES_STR;
    }
    if (SHGetFolderPathW(0, CSIDL_APPDATA, 0, SHGFP_TYPE_CURRENT, szPath) == S_OK)
    {
      ret.userConfigRoot = szPath;
      ret.userConfigRoot += product;
      ret.userConfigRoot += MIKTEX_SERIES_STR;
    }
    ret.userInstallRoot = ret.userConfigRoot;
  }
  return (ret);
}

/* _________________________________________________________________________

   SessionImpl::ReadRegistry

   Read path info from the Windows registry.
   _________________________________________________________________________ */

StartupConfig
SessionImpl::ReadRegistry (/*[in]*/ bool common)
{
  MIKTEX_ASSERT (! IsMiKTeXDirect());

  StartupConfig ret;

  string str;

  if (common)
  {
    if (winRegistry::TryGetRegistryValue(TriState::True,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_COMMON_ROOTS,
      str,
      0))
    {
      ret.commonRoots = str;
    }
    if (winRegistry::TryGetRegistryValue(TriState::True,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_COMMON_INSTALL,
      str,
      0))
    {
      ret.commonInstallRoot = str;
    }
    if (winRegistry::TryGetRegistryValue(TriState::True,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_COMMON_DATA,
      str,
      0))
    {
      ret.commonDataRoot = str;
    }
    if (winRegistry::TryGetRegistryValue(TriState::True,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_COMMON_CONFIG,
      str,
      0))
    {
      ret.commonConfigRoot = str;
    }
  }
  if (! common || AdminControlsUserConfig())
  {
    TriState shared =
      (AdminControlsUserConfig()
       ? TriState::Undetermined 
       : TriState::False);
    if (winRegistry::TryGetRegistryValue(shared,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_USER_ROOTS,
      str,
      0))
    {
      ret.userRoots = str;
    }
    if (winRegistry::TryGetRegistryValue(shared,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_USER_INSTALL,
      str,
      0))
    {
      ret.userInstallRoot = str;
    }
    if (winRegistry::TryGetRegistryValue(shared,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_USER_DATA,
      str,
      0))
    {
      ret.userDataRoot = str;
    }
    if (winRegistry::TryGetRegistryValue(shared,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_USER_CONFIG,
      str,
      0))
    {
      ret.userConfigRoot = str;
    }
  }

  return (ret);
}

/* _________________________________________________________________________

   SessionImpl::WriteRegistry

   Write path info into the registry.
   _________________________________________________________________________ */

void
SessionImpl::WriteRegistry (/*[in]*/ bool		    common,
			    /*[in]*/ const StartupConfig &  startupConfig)
{
  MIKTEX_ASSERT (! IsMiKTeXDirect());

  StartupConfig defaultConfig = DefaultConfig();

  // clean registry values
  if (common)
  {
    winRegistry::TryDeleteRegistryValue (TriState::True,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_COMMON_ROOTS);
    winRegistry::TryDeleteRegistryValue (TriState::True,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_COMMON_INSTALL);
    winRegistry::TryDeleteRegistryValue (TriState::True,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_COMMON_DATA);
    winRegistry::TryDeleteRegistryValue (TriState::True,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_COMMON_CONFIG);
#if ADMIN_CONTROLS_USER_CONFIG
    if (AdminControlsUserConfig())
      {
	winRegistry::TryDeleteRegistryValue (TriState::True,
					     MIKTEX_REGKEY_CORE,
					     MIKTEX_REGVAL_USER_ROOTS);
	winRegistry::TryDeleteRegistryValue (TriState::True,
					     MIKTEX_REGKEY_CORE,
					     MIKTEX_REGVAL_USER_INSTALL);
	winRegistry::TryDeleteRegistryValue (TriState::True,
					     MIKTEX_REGKEY_CORE,
					     MIKTEX_REGVAL_USER_DATA);
	winRegistry::TryDeleteRegistryValue (TriState::True,
					     MIKTEX_REGKEY_CORE,
					     MIKTEX_REGVAL_USER_CONFIG);
      }
#endif
  }
  else
  {
    winRegistry::TryDeleteRegistryValue (TriState::False,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_USER_ROOTS);
    winRegistry::TryDeleteRegistryValue (TriState::False,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_USER_INSTALL);
    winRegistry::TryDeleteRegistryValue (TriState::False,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_USER_DATA);
    winRegistry::TryDeleteRegistryValue (TriState::False,
      MIKTEX_REGKEY_CORE,
      MIKTEX_REGVAL_USER_CONFIG);
  }

  if (common)
  {
    if (! startupConfig.commonRoots.empty()
      && startupConfig.commonRoots != startupConfig.commonInstallRoot)
    {
      winRegistry::SetRegistryValue (TriState::True,
	MIKTEX_REGKEY_CORE,
	MIKTEX_REGVAL_COMMON_ROOTS,
	startupConfig.commonRoots.c_str());
    }
    if (! startupConfig.commonInstallRoot.Empty()
      && startupConfig.commonInstallRoot != defaultConfig.commonInstallRoot)
    {
      winRegistry::SetRegistryValue (TriState::True,
	MIKTEX_REGKEY_CORE,
	MIKTEX_REGVAL_COMMON_INSTALL,
	startupConfig.commonInstallRoot.Get());
    }
    if (! startupConfig.commonDataRoot.Empty()
      && startupConfig.commonDataRoot != defaultConfig.commonDataRoot)
    {
      winRegistry::SetRegistryValue (TriState::True,
	MIKTEX_REGKEY_CORE,
	MIKTEX_REGVAL_COMMON_DATA,
	startupConfig.commonDataRoot.Get());
    }
    if (! startupConfig.commonConfigRoot.Empty()
      && startupConfig.commonConfigRoot != defaultConfig.commonConfigRoot)
    {
      winRegistry::SetRegistryValue (TriState::True,
	MIKTEX_REGKEY_CORE,
	MIKTEX_REGVAL_COMMON_CONFIG,
	startupConfig.commonConfigRoot.Get());
    }
  }
  if (! common || AdminControlsUserConfig())
  {
    TriState shared =
      (AdminControlsUserConfig()
       ? TriState::Undetermined 
       : TriState::False);
    if (! startupConfig.userRoots.empty()
      && startupConfig.userRoots != startupConfig.userInstallRoot)
    {
      winRegistry::SetRegistryValue (shared,
	MIKTEX_REGKEY_CORE,
	MIKTEX_REGVAL_USER_ROOTS,
	startupConfig.userRoots.c_str());
    }
    if (! startupConfig.userInstallRoot.Empty()
      && startupConfig.userInstallRoot != defaultConfig.userInstallRoot)
    {
      winRegistry::SetRegistryValue (shared,
	MIKTEX_REGKEY_CORE,
	MIKTEX_REGVAL_USER_INSTALL,
	startupConfig.userInstallRoot.Get());
    }
    if (! startupConfig.userDataRoot.Empty()
      && startupConfig.userDataRoot != defaultConfig.userDataRoot)
    {
      winRegistry::SetRegistryValue (shared,
	MIKTEX_REGKEY_CORE,
	MIKTEX_REGVAL_USER_DATA,
	startupConfig.userDataRoot.Get());
    }
    if (! startupConfig.userConfigRoot.Empty()
      && startupConfig.userConfigRoot != defaultConfig.userConfigRoot)
    {
      winRegistry::SetRegistryValue (shared,
	MIKTEX_REGKEY_CORE,
	MIKTEX_REGVAL_USER_CONFIG,
	startupConfig.userConfigRoot.Get());
    }
  }
}

/* _________________________________________________________________________

   SessionImpl::GetAcrobatFontDir
   _________________________________________________________________________ */

bool
SessionImpl::GetAcrobatFontDir (/*[out]*/ PathName &	path)
{
  if (! flags.test(Flags::CachedAcrobatFontDir))
  {
    flags.set (Flags::CachedAcrobatFontDir);

    const wchar_t * const ACRORD32 =
      L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\AcroRd32.exe";

    wstring pathExe;

    if (! winRegistry::TryGetRegistryValue(
      HKEY_LOCAL_MACHINE,
      ACRORD32,
      L"",
      pathExe,
      0))
    {
      return (false);
    }

    PathName dir (pathExe);
    dir.RemoveFileSpec ();

    PathName fontDir;

    // try Acrobat Reader 3.0
    fontDir.Set (dir.Get(), "FONTS", 0);
    if (! Directory::Exists(fontDir))
    {
      // try Acrobat Reader 4.0
      fontDir.Set (dir.Get(), "..\\Resource\\Font", 0);
      if (! Directory::Exists(fontDir))
      {
	return (false);
      }
    }

    RemoveDirectoryDelimiter (fontDir.GetBuffer());

    acrobatFontDir = GetFullPath(fontDir.Get());
  }

  if (acrobatFontDir.GetLength() == 0)
  {
    return (false);
  }

  path = acrobatFontDir;

  return (true);
}

/* _________________________________________________________________________
   
   SessionImpl::GetATMFontDir
   _________________________________________________________________________ */

bool
SessionImpl::GetATMFontDir (/*[out]*/ PathName &	path)
{
  if (! flags.test(Flags::CachedAtmFontDir))
  {
    flags.set (Flags::CachedAtmFontDir);

    const wchar_t * const ATMSETUP =
      L"SOFTWARE\\Adobe\\Adobe Type Manager\\Setup";

    wstring pfbDir;

    if (! winRegistry::TryGetRegistryValue(
      HKEY_LOCAL_MACHINE,
      ATMSETUP,
      L"PFB_DIR",
      pfbDir,
      0))
    {
      return (false);
    }

    PathName fontDir (pfbDir);

    if (! Directory::Exists(fontDir))
    {
      return (false);
    }

    RemoveDirectoryDelimiter (fontDir.GetBuffer());

    atmFontDir = GetFullPath(fontDir.Get());
  }

  if (atmFontDir.GetLength() == 0)
  {
    return (false);
  }

  path = atmFontDir;

  return (true);
}

/* _________________________________________________________________________

   GetPsFontsDirectory
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
GetPsFontDirectory (/*[out]*/ PathName & path)
{
  wchar_t szWinDir[BufferSizes::MaxPath];

  if (GetWindowsDirectoryW(szWinDir, BufferSizes::MaxPath) == 0)
    {
      FATAL_WINDOWS_ERROR ("GetWindowsDirectoryW", 0);
    }

  char szWinDrive[BufferSizes::MaxPath];

  PathName::Split (PathName(szWinDir).Get(),
		   szWinDrive, BufferSizes::MaxPath,
		   0, 0,
		   0, 0,
		   0, 0);

  PathName path_ (szWinDrive, "\\psfonts", 0, 0);

  if (! Directory::Exists(path_))
    {
      return (false);
    }

  path = path_;

  return (true);
}

/* _________________________________________________________________________

   SessionImpl::GetPsFontDirs
   _________________________________________________________________________ */

bool
SessionImpl::GetPsFontDirs (/*[out]*/ string &	psFontDirs)
{
  if (! flags.test(Flags::CachedPsFontDirs))
    {
      flags.set (Flags::CachedPsFontDirs);
      PathName path;
      if (GetATMFontDir(path) || GetPsFontDirectory(path))
	{
	  if (! this->psFontDirs.empty())
	    {
	      this->psFontDirs += PathName::PathNameDelimiter;
	    }
	  this->psFontDirs += path.Get();
	}
      if (GetAcrobatFontDir(path))
	{
	  if (! this->psFontDirs.empty())
	    {
	      this->psFontDirs += PathName::PathNameDelimiter;
	    }
	  this->psFontDirs += path.Get();
	}
    }

  if (this->psFontDirs.empty())
    {
      return (false);
    }

  psFontDirs = this->psFontDirs;

  return (true);
}

/* _________________________________________________________________________

   GetWindowsFontsDirectory
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(bool)
GetWindowsFontsDirectory (/*[out]*/ PathName & path)
{
  wchar_t szWinDir[BufferSizes::MaxPath];
  if (GetWindowsDirectoryW(szWinDir,
			   BufferSizes::MaxPath)
      == 0)
    {
      FATAL_WINDOWS_ERROR ("GetWindowsDirectoryW", 0);
    }
  path = szWinDir;
  path += "Fonts";
  return (Directory::Exists(path));
}

/* _________________________________________________________________________

   SessionImpl::GetTTFDirs
   _________________________________________________________________________ */

bool
SessionImpl::GetTTFDirs (/*[out]*/ string &	ttfDirs)
{
  if (! flags.test(Flags::CachedTtfDirs))
    {
      flags.set (Flags::CachedTtfDirs);
      PathName path;
      if (GetWindowsFontsDirectory(path))
	{
	  if (! this->ttfDirs.empty())
	    {
	      this->ttfDirs += PathName::PathNameDelimiter;;
	    }
	  this->ttfDirs += path.Get();
	}
    }

  if (this->ttfDirs.empty())
    {
      return (false);
    }

  ttfDirs = this->ttfDirs;

  return (true);
}

/* _________________________________________________________________________

   SessionImpl::GetOTFDirs
   _________________________________________________________________________ */

bool
SessionImpl::GetOTFDirs (/*[out]*/ string &	otfDirs)
{
  if (! flags.test(Flags::CachedOtfDirs))
    {
      flags.set (Flags::CachedOtfDirs);
      PathName path;
      if (GetWindowsFontsDirectory(path))
	{
	  if (! this->otfDirs.empty())
	    {
	      this->otfDirs += PathName::PathNameDelimiter;
	    }
	  this->otfDirs += path.Get();
	}
    }

  if (this->otfDirs.empty())
    {
      return (false);
    }

  otfDirs = this->otfDirs;

  return (true);
}

/* _________________________________________________________________________

   GetWindowsErrorMessage
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
GetWindowsErrorMessage (/*[in]*/ unsigned long	functionResult,
			/*[out]*/ string &	errorMessage)
{
  void * pMessageBuffer;
  unsigned long len =
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		   0,
		   functionResult,
		   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		   reinterpret_cast<wchar_t *>(&pMessageBuffer),
		   0,
		   0);
  if (len == 0)
    {
      TraceError (T_("FormatMessageW() failed for some reason"));
      return (false);
    }
  AutoLocalMemory autoFree (pMessageBuffer);
  errorMessage =
    Utils::WideCharToAnsi(reinterpret_cast<wchar_t *>(pMessageBuffer));
  return (true);
}

/* _________________________________________________________________________

   Session::FatalWindowsError
   _________________________________________________________________________ */

void
Session::FatalWindowsError (/*[in]*/ const char *	lpszWindowsFunction,
			    /*[in]*/ unsigned long	errorCode,
			    /*[in]*/ const char *	lpszInfo,
			    /*[in]*/ const char *	lpszSourceFile,
			    /*[in]*/ int		sourceLine)
{
  lpszSourceFile = GetShortSourceFile(lpszSourceFile);
  string programInvocationName;
  if (SessionImpl::TryGetSession() != 0)
    {
      TraceStream::TraceLastWin32Error (lpszWindowsFunction,
					lpszInfo,
					lpszSourceFile,
					sourceLine);
      programInvocationName =
	SessionImpl::GetSession()->initInfo.GetProgramInvocationName();
    }
  string errorMessage = T_("Windows API error ");
  errorMessage += NUMTOSTR(errorCode);
  string windowsErrorMessage;
  if (GetWindowsErrorMessage(errorCode, windowsErrorMessage))
    {
      errorMessage += ": ";
      errorMessage += windowsErrorMessage;
    }
  else
    {
      errorMessage += '.';
    }
#if 1
  string env;
  if (Utils::GetEnvironmentString("MIKTEX_DEBUG_BREAK", env)
      && env == "1")
    {
      DEBUG_BREAK ();
    }
#endif
  switch (errorCode)
    {
    case ERROR_ACCESS_DENIED:
      throw UnauthorizedAccessException (programInvocationName.c_str(),
					 errorMessage.c_str(),
					 lpszInfo,
					 lpszSourceFile,
					 sourceLine);
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
      throw FileNotFoundException (programInvocationName.c_str(),
				   errorMessage.c_str(),
				   lpszInfo,
				   lpszSourceFile,
				   sourceLine);
    case ERROR_SHARING_VIOLATION:
      throw SharingViolationException (programInvocationName.c_str(),
				       errorMessage.c_str(),
				       lpszInfo,
				       lpszSourceFile,
				       sourceLine);
    default:
      {
	throw MiKTeXException (programInvocationName.c_str(),
			       errorMessage.c_str(),
			       lpszInfo,
			       lpszSourceFile,
			       sourceLine);
      }
    }
}

/* _________________________________________________________________________

   Session::FatalWindowsError
   _________________________________________________________________________ */

void
Session::FatalWindowsError (/*[in]*/ const char *	lpszWindowsFunction,
			    /*[in]*/ const char *	lpszInfo,
			    /*[in]*/ const char *	lpszSourceFile,
			    /*[in]*/ int		sourceLine)
{
  FatalWindowsError (lpszWindowsFunction,
		     GetLastError(),
		     lpszInfo,
		     lpszSourceFile,
		     sourceLine);
}

/* _________________________________________________________________________

   SharingViolationException::SharingViolationException
   _________________________________________________________________________ */

SharingViolationException::SharingViolationException ()
{
}

/* _________________________________________________________________________

   SharingViolationException::SharingViolationException
   _________________________________________________________________________ */

SharingViolationException::SharingViolationException
(/*[in]*/ const char *	lpszProgramInvocationName,
 /*[in]*/ const char *	lpszMessage,
 /*[in]*/ const char *	lpszInfo,
 /*[in]*/ const char *	lpszSourceFile,
 /*[in]*/ int			sourceLine)
  : IOException (lpszProgramInvocationName,
		 lpszMessage,
		 lpszInfo,
		 lpszSourceFile,
		 sourceLine)
{
}

/* _________________________________________________________________________

   GetAlternate
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(void)
GetAlternate (/*[in]*/ const char *	lpszPath,
	      /*[out]*/ char *		lpszAlternate)
{
  MIKTEX_ASSERT_STRING (lpszPath);
  MIKTEX_ASSERT_PATH_BUFFER (lpszAlternate);
  WIN32_FIND_DATAW finddata;
  HANDLE hnd = FindFirstFileW(PathName(lpszPath).ToWideCharString().c_str(), &finddata);
  if (hnd == INVALID_HANDLE_VALUE)
    {
      FATAL_WINDOWS_ERROR ("FindFirstFileW", lpszPath);
    }
  if (! FindClose(hnd))
    {
      FATAL_WINDOWS_ERROR ("FindClose", 0);
    }
  if (finddata.cAlternateFileName[0] == 0)
    {
      FATAL_MIKTEX_ERROR ("GetAlternate",
			  T_("No alternate file name found."),
			  lpszPath);
    }
  Utils::CopyString (lpszAlternate,
		     BufferSizes::MaxPath,
		     finddata.cAlternateFileName);
}

/* _________________________________________________________________________

   Utils::RemoveBlanksFromPathName 
   _________________________________________________________________________ */

void
Utils::RemoveBlanksFromPathName (/*[in,out]*/ PathName &	path)
{
  // make a working copy
  PathName temp (path);

  // points to the null char
  char * lpszEnd = temp.GetBuffer() + temp.GetLength();

  // back iterator
  char * lpsz = lpszEnd;

  // points to the last '/' found (if any)
  char * lpszSlash = 0;

  // true, if lpsz points to a char sequence that includes a blank
  // char
  bool containsBlanks = false;

  // find the last slash
  while (lpsz != temp.Get())
    {
      -- lpsz;
      if (IsDirectoryDelimiter(*lpsz) && lpsz + 1 != lpszEnd)
	{
	  lpszSlash = lpsz;
	  break;
	}
      if (*lpsz == ' ')
	{
	  containsBlanks = true;
	}
    }

  // done, if we're at the beginning of the path and if there is no
  // blank
  if (! containsBlanks && lpsz == temp.Get())
    {
      return;
    }

  // lpszSlash points to the last '/'
  MIKTEX_ASSERT (lpszSlash == 0 || IsDirectoryDelimiter(*lpszSlash));
  
  // buffer for file name w/o blanks
  char szAlternate[BufferSizes::MaxPath];

  if (containsBlanks)
    {
      // get short file name (8.3 w/o blanks)
      GetAlternate (temp.Get(), szAlternate);
    }
  else
    {
      // no blanks found in original file name, i.e., keep the
      // original file name
      Utils::CopyString (szAlternate,
			 BufferSizes::MaxPath,
			 (lpsz == lpszSlash ? lpsz + 1 : lpsz));
    }

  // the new file name doesn't contain a space
  MIKTEX_ASSERT (StrChr(szAlternate, ' ') == 0);

  if (lpszSlash == temp.Get()	// "/foo.bar"
      || (lpszSlash == temp.Get() + 1 // "//foo.bar"
	  && PathName::IsDirectoryDelimiter(temp[0]))	
      || (lpszSlash == temp.Get() + 2	// "C:/foo.bar"
	  && IsAlpha(temp[0])
	  && PathName::IsVolumeDelimiter(temp[1])))
    {
      // we have reached the beginning of the path; separate original
      // file name from path; then combine path with new file name
      lpszSlash[1] = 0;
      path = temp;
      path += szAlternate;
    }
  else if (lpszSlash != 0)
    {
      // separate file name from path
      *lpszSlash = 0;
      // still have to remove blanks from path
      // <recursivecall>
      RemoveBlanksFromPathName (temp);
      // </recursivecall>
      // path now hasn't any blanks; combine path with new file name
      MIKTEX_ASSERT (StrChr(temp.Get(), ' ') == 0);
      path = temp;
      path += szAlternate;
    }
  else
    {
      // original path was something like 'foo bar'; simply turn back
      // the new version (something like 'FOOBAR~1')
      path = szAlternate;
    }
}

/* _________________________________________________________________________

   PathName::Combine
   _________________________________________________________________________ */

void
PathName::Combine (/*[out]*/ char *		lpszPath,
		   /*[in]*/ size_t		sizePath,
		   /*[in]*/ const char *	lpszDrive,
		   /*[in]*/ const char *	lpszAbsPath,
		   /*[in]*/ const char *	lpszRelPath,
		   /*[in]*/ const char *	lpszExtension)
{
#if defined(_MSC_VER)
  if (_makepath_s(lpszPath,
		  sizePath,
		  lpszDrive,
		  lpszAbsPath,
		  lpszRelPath,
		  lpszExtension)
      != 0)
    {
      FATAL_CRT_ERROR ("_makepath_s", 0);
    }
#else  // not Microsoft C++
#  error Unimplemented: PathName::Combine()
#endif	// not Microsoft C++
}

/* _________________________________________________________________________

   PathName::Split
   _________________________________________________________________________ */

void
PathName::Split (/*[in]*/ const char *	lpszPath,
		 /*[out]*/ char *	lpszDrive,
		 /*[in]*/ size_t	sizeDrive,
		 /*[out]*/ char *	lpszDir,
		 /*[in]*/ size_t	sizeDir,
		 /*[out]*/ char *	lpszName,
		 /*[in]*/ size_t	sizeName,
		 /*[out]*/ char *	lpszExtension,
		 /*[in]*/ size_t	sizeExtension)
{
  MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL (lpszDrive, sizeDrive);
  MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL (lpszDir, sizeDir);
  MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL (lpszName, sizeName);
  MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL (lpszExtension, sizeExtension);
#if defined(_MSC_VER)
  if (_splitpath_s(lpszPath,
		   lpszDrive,
		   sizeDrive,
		   lpszDir,
		   sizeDir,
		   lpszName,
		   sizeName,
		   lpszExtension,
		   sizeExtension)
      != 0)
    {
      FATAL_CRT_ERROR ("_tsplitpath_s", 0);
    }
#else  // not Microsoft C++
#  error Unimplemented: PathName::Split()
#endif	// not Microsoft C++
}

/* _________________________________________________________________________

   GetUserProfileDirectory
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(bool)
GetUserProfileDirectory (/*[out]*/ PathName & path)
{
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
  if (! IsWindowsNT())
    {
      const char * lpszProfRecon =
	T_("\
Software\\Microsoft\\Windows\\CurrentVersion\\ProfileReconciliation");
      
      AutoHKEY hkey;
      
      LONG res =
	RegOpenKeyExA(HKEY_CURRENT_USER,
		      lpszProfRecon,
		      0,
		      KEY_READ,
		      &hkey);
      
      if (res == ERROR_SUCCESS)
	{
	  unsigned long len = path.GetCapacity();
	  unsigned long type = REG_SZ;
	  res =
	    RegQueryValueExA(hkey.Get(),
			     "ProfileDirectory",
			     0,
			     &type,
			     reinterpret_cast<LPBYTE>(path.GetBuffer()),
			     &len);
	  if (res == ERROR_SUCCESS)
	    {
	      return (true);
	    }
	  else if (res != ERROR_FILE_NOT_FOUND)
	    {
	      FATAL_WINDOWS_ERROR ("RegQueryValueExA", 0);
	    }
	}
      else if (res != ERROR_FILE_NOT_FOUND)
	{
	  FATAL_WINDOWS_ERROR ("RegOpenKeyExA", lpszProfRecon);
	}
    }
#endif

  return (Utils::GetEnvironmentString("USERPROFILE", path));
}

/* _________________________________________________________________________

   DllGetVersion
   _________________________________________________________________________ */

extern "C"
__declspec(dllexport)
HRESULT
CALLBACK
DllGetVersion (/*[out]*/ DLLVERSIONINFO * pdvi)
{
  MIKTEX_ASSERT (pdvi != 0);
  if (pdvi->cbSize != sizeof(*pdvi))
    {
      return (E_INVALIDARG);
    }
  unsigned a[4] = {
    MIKTEX_MAJOR_VERSION,
    MIKTEX_MINOR_VERSION,
    MIKTEX_COMP_J2000_VERSION,
    0
  };
  pdvi->dwMajorVersion = a[0];
  pdvi->dwMinorVersion = a[1];
  pdvi->dwBuildNumber = a[2];
  pdvi->dwPlatformID = DLLVER_PLATFORM_WINDOWS;
  return (S_OK);
}

/* _________________________________________________________________________

   Utils::GetOSVersionString

   We assume that the minimum system requirements have been checked.
   _________________________________________________________________________ */

typedef void (WINAPI * PGNSI) (/*[out]*/ LPSYSTEM_INFO);
typedef BOOL (WINAPI * PGPI) (/*[in]*/ DWORD,
			      /*[in]*/ DWORD,
			      /*[in]*/ DWORD,
			      /*[in]*/ DWORD,
			      /*[out]*/ DWORD *);

inline
void
NeedBlank (/*[in,out]*/ string & str)
{
  if (! str.empty() && str[str.length() - 1] != ' ')
    {
      str += ' ';
    }
}

inline
HMODULE
GetKernel32Module ()
{
  HMODULE h = GetModuleHandleW(L"kernel32.dll");
  if (h == 0)
    {
      UNEXPECTED_CONDITION ("GetKernel32Module");
    }
  return (h);
}

string
Utils::GetOSVersionString ()
{
  // get Windows version information
  OSVERSIONINFOEXW osvi;
  ZeroMemory (&osvi, sizeof(osvi));
  osvi.dwOSVersionInfoSize = sizeof(osvi);
  if (! GetVersionExW(reinterpret_cast<OSVERSIONINFOW*>(&osvi)))
    {
      UNSUPPORTED_PLATFORM ();
    }

  // get system information
  SYSTEM_INFO si;
  ZeroMemory (&si, sizeof(si));
  PGNSI pGNSI =
    reinterpret_cast<PGNSI>
    (GetProcAddress(GetKernel32Module(), "GetNativeSystemInfo"));
  if (pGNSI != 0)
    {
      pGNSI (&si);
    }
  else
    {
      // Windows 2000
      GetSystemInfo (&si);
    }

  // build version string
  string str;
  switch (osvi.dwPlatformId)
     {
     case VER_PLATFORM_WIN32_NT:
       // Windows NT product family
       str += "Microsoft Windows";
       NeedBlank (str);
       if (osvi.dwMajorVersion == 6
	   && (osvi.dwMinorVersion == 0
	       || osvi.dwMinorVersion == 1))
	 {
	   if (osvi.wProductType == VER_NT_WORKSTATION)
	     {
	       if (osvi.dwMinorVersion == 0)
		 {
		   str += "Vista";
		 }
	       else if (osvi.dwMinorVersion == 1)
		 {
		   str += "7";
		 }
	     }
	   else
	     {
	       if (osvi.dwMinorVersion == 0)
		 {
		   str += "Server 2008";
		 }
	       else if (osvi.dwMinorVersion == 1)
		 {
		   str += "Server 2008 R2";
		 }
	     }
	   PGPI pGPI =
	     reinterpret_cast<PGPI>
	     (GetProcAddress(GetKernel32Module(), "GetProductInfo"));
	   if (pGPI == 0)
	     {
	       UNEXPECTED_CONDITION ("Utils::GetOSVersionString");
	     }
	   DWORD productType;
	   if (! pGPI(6, 0, 0, 0, &productType))
	     {
	       UNEXPECTED_CONDITION ("Utils::GetOSVersionString");
	     }
	   NeedBlank (str);
	   switch (productType)
	     {
	     case PRODUCT_UNLICENSED:
	       str += T_("(not activated, grace period expired)");
	       break;
	     case PRODUCT_BUSINESS:
	       str += "Business Edition";
	       break;
	     case PRODUCT_BUSINESS_N:
	       str += "Business N Edition";
	       break;
	     case PRODUCT_CLUSTER_SERVER:
	       str += "Cluster Server Edition";
	       break;
	     case PRODUCT_DATACENTER_SERVER:
	       str += "Server Datacenter Edition";
	       break;
	     case PRODUCT_DATACENTER_SERVER_CORE:
	       str += "Server Datacenter Edition (core installation)";
	       break;
	     case PRODUCT_ENTERPRISE:
	       str += "Enterprise Edition";
	       break;
	     case PRODUCT_ENTERPRISE_SERVER:
	       str += "Server Enterprise Edition";
	       break;
	     case PRODUCT_ENTERPRISE_SERVER_CORE:
	       str += "Server Enterprise Edition (core installation)";
	       break;
	     case PRODUCT_ENTERPRISE_SERVER_IA64:
	       str += "Server Enterprise Edition for Itanium-based Systems";
	       break;
	     case PRODUCT_HOME_BASIC:
	       str += "Home Basic Edition";
	       break;
	     case PRODUCT_HOME_BASIC_N:
	       str += "Home Basic N Edition";
	       break;
	     case PRODUCT_HOME_PREMIUM:
	       str += "Home Premium Edition";
	       break;
	     case PRODUCT_HOME_SERVER:
	       str += "Home Server Edition";
	       break;
	     case PRODUCT_SERVER_FOR_SMALLBUSINESS:
	       str += "Server for Small Business Edition";
	       break;
	     case PRODUCT_SMALLBUSINESS_SERVER:
	       str += "Small Business Server";
	       break;
	     case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
	       str += "Small Business Server Premium Edition";
	       break;
	     case PRODUCT_STANDARD_SERVER:
	       str += "Server Standard Edition";
	       break;
	     case PRODUCT_STANDARD_SERVER_CORE :
	       str += "Server Standard Edition (core installation)";
	       break;
	     case PRODUCT_STARTER:
	       str += "Starter Edition";
	       break;
	     case PRODUCT_STORAGE_ENTERPRISE_SERVER:
	       str += "Storage Server Enterprise Edition";
	       break;
	     case PRODUCT_STORAGE_EXPRESS_SERVER:
	       str += "Storage Server Express Edition";
	       break;
	     case PRODUCT_STORAGE_STANDARD_SERVER:
	       str += "Storage Server Standard Edition";
	       break;
	     case PRODUCT_STORAGE_WORKGROUP_SERVER:
	       str += "Storage Server Workgroup Edition";
	       break;
	     case PRODUCT_UNDEFINED:
	       str += T_("(unknown product)");
	       break;
	     case PRODUCT_ULTIMATE:
	       str += "Ultimate Edition";
	       break;
	     case PRODUCT_WEB_SERVER:
	       str += "Web Server Edition";
	       break;
	     }
	   if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
	     {
	       str += ", 64-bit";
	     }
	   else
	     {
	       str += ", 32-bit";
	     }
	 }
       else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
	 {
	   if (GetSystemMetrics(SM_SERVERR2))
	     {
	       str += "Server 2003 R2";
	     }
	   else if (osvi.wSuiteMask == VER_SUITE_STORAGE_SERVER)
	     {
	       str += "Storage Server 2003";
	     }
	   else if (osvi.wProductType == VER_NT_WORKSTATION
		    && (si.wProcessorArchitecture
			== PROCESSOR_ARCHITECTURE_AMD64))
	     {
	       str += "XP Professional x64 Edition";
	     }
	   else
	     {
	       str += "Server 2003";
	     }

         // test for the server type
         if (osvi.wProductType != VER_NT_WORKSTATION)
	   {
	     str += ", ";
	     if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	       {
		 if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) != 0)
		   {
		     str += "Datacenter Edition for Itanium-based Systems";
		   }
		 else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
		   {
		     str += "Enterprise Edition for Itanium-based Systems";
		   }
	       }
	     else if (si.wProcessorArchitecture
		      == PROCESSOR_ARCHITECTURE_AMD64)
	       {
		 if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) != 0)
		   {
		     str += "Datacenter x64 Edition";
		   }
		 else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
		   {
		     str += "Enterprise x64 Edition";
		   }
		 else
		   {
		     str += "Standard x64 Edition";
		   }
	       }
	     else if ((osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER) != 0)
	       {
		 str += "Compute Cluster Edition";
	       }
	     else if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) != 0)
	       {
		 str += "Datacenter Edition";
	       }
	     else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
	       {
		 str += "Enterprise Edition";
	       }
	     else if ((osvi.wSuiteMask & VER_SUITE_BLADE) != 0)
	       {
		 str += "Web Edition";
	       }
	     else
	       {
		 str += "Standard Edition";
	       }
	   }
         }
       else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
	 {
	   str += "XP";
	   NeedBlank (str);
	   if ((osvi.wSuiteMask & VER_SUITE_PERSONAL) != 0)
	     {
	       str += "Home Edition";
	     }
	   else
	     {
	       str += "Professional";
	     }
	 }
       else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
	 {
	   str += "2000";
	   NeedBlank (str);
	   if (osvi.wProductType == VER_NT_WORKSTATION)
	     {
	       str +="Professional";
	     }
	   else if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) != 0)
	     {
               str += "Datacenter Server";
	     }
	   else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
	     {
	       str += "Advanced Server";
	     }
	   else
	     {
	       str += "Server";
	     }
         }
       else
	 {
	   // unknown Windows version
	   MIKTEX_ASSERT (false);
	   str += NUMTOSTR(osvi.dwMajorVersion);
	   str += ".";
	   str += NUMTOSTR(osvi.dwMinorVersion);
	 }

       // service pack
       if (osvi.szCSDVersion[0] != 0)
	 {
	   NeedBlank (str);
	   str += Utils::WideCharToAnsi(osvi.szCSDVersion);
	 }

       // build number
       NeedBlank (str);
       str += "(build";
       NeedBlank (str);
       str += NUMTOSTR(osvi.dwBuildNumber);
       str += ")";

       break;
     
     case VER_PLATFORM_WIN32s:
       // nearly impossible
       UNEXPECTED_CONDITION ("Utils::GetOSVersionString");

     case VER_PLATFORM_WIN32_WINDOWS:
       // Windows 95 product family
       UNSUPPORTED_PLATFORM ();

     default:
       // unknown platform
       MIKTEX_ASSERT (false);
       str += "unknown platform (";
       str += NUMTOSTR(osvi.dwPlatformId);
       str += ")";
       break;
     }

  return (str);
}

/* _________________________________________________________________________

   Utils::RunningOnAServer
   _________________________________________________________________________ */

bool
Utils::RunningOnAServer ()
{
  OSVERSIONINFOEXW osvi;
  ZeroMemory (&osvi, sizeof(osvi));
  osvi.dwOSVersionInfoSize = sizeof(osvi);
  if (! GetVersionExW(reinterpret_cast<OSVERSIONINFOW*>(&osvi)))
  {
    UNSUPPORTED_PLATFORM ();
  }
  return (osvi.wProductType != VER_NT_WORKSTATION);
}

/* _________________________________________________________________________

   Utils::GetDefPrinter

   See Q246772.
   _________________________________________________________________________ */

bool
Utils::GetDefPrinter (/*[out]*/ char *		pPrinterName,
		      /*[in,out]*/ size_t *	pBufferSize)
{
  OSVERSIONINFOW osv;
  osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionExW (&osv);
  if (osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
    {
      unsigned long dwNeeded, dwReturned;
      EnumPrintersW (PRINTER_ENUM_DEFAULT,
		     0,
		     2,
		     0,
		     0,
		     &dwNeeded,
		     &dwReturned);
      if (dwNeeded == 0)
	{
	  return (false);
	}
      AutoGlobalMemory hMem (GlobalAlloc(GPTR, dwNeeded));
      if (hMem.Get() == 0)
	{
	  OUT_OF_MEMORY ("Utils::GetDefPrinter");
	}
      PRINTER_INFO_2W * ppi2 =
	reinterpret_cast<PRINTER_INFO_2W *>(hMem.Get());
      if (! EnumPrintersW(PRINTER_ENUM_DEFAULT,
			  0,
			  2,
			  reinterpret_cast<LPBYTE>(ppi2),
			  dwNeeded,
			  &dwNeeded,
			  &dwReturned))
	{
	  return (false);
	}
      size_t l = StrLen(ppi2->pPrinterName);
      if (l >= *pBufferSize)
	{
	  *pBufferSize = l + 1;
	  return (false);
	}
      Utils::CopyString (pPrinterName, *pBufferSize, ppi2->pPrinterName);
      *pBufferSize = l + 1;
      return (true);
    }
  else
    {
      if (osv.dwPlatformId != VER_PLATFORM_WIN32_NT)
	{
	  UNEXPECTED_CONDITION ("Utils::GetDefPrinter");
	}
      if (osv.dwMajorVersion >= 5)
	{
	  DllProc2<BOOL, wchar_t *, LPDWORD>
	    getDefaultPrinterW ("winspool.drv", "GetDefaultPrinterW");
	  DWORD dwBufferSize = static_cast<DWORD>(*pBufferSize);
	  CharBuffer<wchar_t> printerName (*pBufferSize);
	  BOOL bDone = getDefaultPrinterW(printerName.GetBuffer(), &dwBufferSize);
	  if (! bDone)
	    {
	      if (::GetLastError() == ERROR_FILE_NOT_FOUND)
		{
		  return (false);
		}
	      else
		{
		  FATAL_WINDOWS_ERROR ("GetDefaultPrinterW", 0);
		}
	    }
	  else
	    {
	      Utils::CopyString (pPrinterName, *pBufferSize, printerName.Get());
	      *pBufferSize = dwBufferSize;
	      return (true);
	    }
	}
      else
	{
	  wchar_t cBuffer[4096];
	  if (GetProfileStringW(L"windows",
			        L"device",
			        L",,,",
			        cBuffer,
			        4096)
	      <= 0)
	    {
	      return (false);
	    }
	  Tokenizer tok (Utils::WideCharToAnsi(cBuffer).c_str(), ",");
	  if (tok.GetCurrent() == 0)
	    {
	      return (false);
	    }
	  unsigned long l =
	    static_cast<unsigned long>(StrLen(tok.GetCurrent()));
	  if (l >= *pBufferSize)
	    {
	      *pBufferSize = l + 1;
	      return (false);
	    }
	  Utils::CopyString (pPrinterName, *pBufferSize, tok.GetCurrent());
	  *pBufferSize = l + 1;
	  return (true);
	}
    }
}

/* _________________________________________________________________________

   SessionImpl::ShowManualPageAndWait
   _________________________________________________________________________ */

bool
SessionImpl::ShowManualPageAndWait (/*[in]*/ HWND		hWnd,
				    /*[in]*/ unsigned long	topic)
{
  PathName pathHelpFile;
  if (! FindFile("miktex.chm", "%R\\doc\\miktex//", pathHelpFile))
    {
      return (false);
    }
  HWND hwnd = HtmlHelpW(hWnd, pathHelpFile.ToWideCharString().c_str(), HH_HELP_CONTEXT, topic);
  if (hwnd == 0)
    {
      return (false);
    }
  while (IsWindow(hwnd))
    {
      Sleep (20);
    }
  return (true);
}

/* _________________________________________________________________________

   PathName::SetToCurrentDirectory
   _________________________________________________________________________ */

PathName &
PathName::SetToCurrentDirectory ()
{
  if (getcwd(GetBuffer(), static_cast<int>(GetCapacity())) == 0)
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
  wchar_t szTemp[BufferSizes::MaxPath];
  unsigned long n =
    GetTempPathW(static_cast<DWORD>(BufferSizes::MaxPath), szTemp);
  if (n == 0)
    {
      FATAL_WINDOWS_ERROR ("GetTempPathW", 0);
    }
  if (n >= GetCapacity())
    {
      UNEXPECTED_CONDITION ("PathName::SetToTempDirectory");
    }
  *this = szTemp;
  return (*this);
}

/* _________________________________________________________________________

   PathName::SetToTempFile
   _________________________________________________________________________ */

PathName &
PathName::SetToTempFile ()
{
  PathName pathTempDir = SessionImpl::GetSession()->GetTempDirectory();

  wchar_t szTemp[MAX_PATH];

  UINT n = GetTempFileNameW(pathTempDir.ToWideCharString().c_str(), L"mik", 0, szTemp);

  if (n == 0)
    {
      FATAL_WINDOWS_ERROR ("GetTempFileNameW", pathTempDir.Get());
    }

  *this = szTemp;

  SessionImpl::GetSession()->trace_tempfile->WriteFormattedLine
    ("core",
     T_("created temporary file %s"),
     Q_(Get()));

  return (*this);
}

/* _________________________________________________________________________

   TraceWindowsError
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(void)
TraceWindowsError (/*[in]*/ const char *	lpszWindowsFunction,
		   /*[in]*/ unsigned long	functionResult,
		   /*[in]*/ const char *	lpszInfo,
		   /*[in]*/ const char *	lpszSourceFile,
		   /*[in]*/ int			sourceLine)
{
  string errorMessage;
  if (! GetWindowsErrorMessage(functionResult, errorMessage))
    {
      return;
    }
  SessionImpl::GetSession()->trace_error->WriteFormattedLine
    ("core",
     T_("\
%s\n\
Function: %s\n\
Result: %u\n\
Data: %s\n\
Source: %s:%d"),
     errorMessage.c_str(),
     lpszWindowsFunction,
     static_cast<unsigned>(functionResult),
     (lpszInfo == 0 ? "<no data>" : lpszInfo),
     lpszSourceFile,
     sourceLine);
}

/* _________________________________________________________________________

   AddEventSource
   _________________________________________________________________________ */

#define REPORT_EVENTS 0
#if REPORT_EVENTS

#define SOURCE "MiKTeX"

#define EVTLOGAPP \
  "SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application"

#if ! defined(MIKTEX_STATIC)
MIKTEXSTATICFUNC(bool)
AddEventSource ()
{
  if (! IsWindowsNT())
    {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
      return (false)
#else
      UNSUPPORTED_PLATFORM ();
#endif
    }

  string registryPath = EVTLOGAPP;
  registryPath += '\\';
  registryPath += SOURCE;

  AutoHKEY hkey;
  unsigned long disp;
  long res =
    RegCreateKeyEx(HKEY_LOCAL_MACHINE,
		   registryPath.c_str(),
		   0,
		   "",
		   REG_OPTION_NON_VOLATILE,
		   KEY_ALL_ACCESS,
		   0,
		   &hkey,
		   &disp);
  if (res != ERROR_SUCCESS)
    {
      TraceWindowsError ("",
			 res,
			 registryPath.c_str(),
			 __FILE__,
			 __LINE__);
      return (false);
    }
  
  // set the name of the message file
  char szModule[BufferSizes::MaxPath];
  if (GetModuleFileName(SessionImpl::hinstDLL, szModule, BufferSizes::MaxPath)
      == 0)
    {
      TraceStream::TraceLastWin32Error ("GetModuleFileName",
					0,
					__FILE__,
					__LINE__);
      return (false);
    }
  res =
    RegSetValueEx(hkey.Get(),
		  "EventMessageFile",
		  0,
		  REG_EXPAND_SZ,
		  reinterpret_cast<unsigned char *>(&szModule),
		  static_cast<unsigned long>((StrLen(szModule) + 1)
					     * sizeof(char)));
  if (res != ERROR_SUCCESS)
    {
      TraceWindowsError ("RegSetValueEx",
			 res,
			 "EventMessageFile",
			 __FILE__,
			 __LINE__);
      return (false);
    }
 
  // set the supported event types in the TypesSupported subkey
  unsigned long dwData = (EVENTLOG_ERROR_TYPE
			  | EVENTLOG_WARNING_TYPE
			  | EVENTLOG_INFORMATION_TYPE);
  res =
    RegSetValueEx(hkey.Get(),
		  "TypesSupported",
		  0,
		  REG_DWORD,
		  reinterpret_cast<unsigned char *>(&dwData),
		  sizeof(unsigned long));
  
  if (res != ERROR_SUCCESS)
    {
      TraceWindowsError ("RegSetValueEx",
			 res,
			 "TypesSupported",
			 __FILE__,
			 __LINE__);
      return (false);
    }

  return (true);
} 
#endif

#endif

/* _________________________________________________________________________

   RemoveEventSource
   _________________________________________________________________________ */

#if REPORT_EVENTS

#if ! defined(MIKTEX_STATIC)
MIKTEXSTATICFUNC(bool)
RemoveEventSource ()
{
  if (! IsWindowsNT())
    {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
      return (false)
#else
      UNSUPPORTED_PLATFORM ();
#endif
    }
  AutoHKEY hkey;
  LONG res =
    RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		 EVTLOGAPP,
		 0,
		 KEY_SET_VALUE,
		 &hkey);
  if (res != ERROR_SUCCESS)
    {
      TraceWindowsError ("RemoveEventSource",
			 res,
			 "EventLog\\Application",
			 __FILE__,
			 __LINE__);
      return (false);
    }
  res = RegDeleteKey(hkey.Get(), SOURCE);
  if (res != ERROR_SUCCESS)
    {
      TraceWindowsError ("RemoveEventSource",
			 res,
			 "EventLog\\Application\\MiKTeX",
			 __FILE__,
			 __LINE__);
      return (false);
    }
  return (true);
} 
#endif

#endif

/* _________________________________________________________________________

   ReportMiKTeXEvent
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(bool)
ReportMiKTeXEvent (/*[in]*/ unsigned short	eventType,
		   /*[in]*/ unsigned long	eventId,
		   /*[in]*/			...)
{
#if ! REPORT_EVENTS
  UNUSED_ALWAYS (eventType);
  UNUSED_ALWAYS (eventId);
  return (false);
#else
  if (! IsWindowsNT())
    {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
      return (false)
#else
      UNSUPPORTED_PLATFORM ();
#endif
    }

  vector<const char *> vecStrings;
  va_list marker;
  va_start (marker, eventId);
  for (const char * lpsz = va_arg(marker, const char *);
       lpsz != 0;
       lpsz = va_arg(marker, const char *))
    {
      vecStrings.push_back (lpsz);
    }
  va_end (marker);

  HANDLE hEventSource = RegisterEventSource(0, SOURCE);
  if (hEventSource == 0)
    {
      return (false);
    }

  PSID pSid = 0;
  size_t bufSize = 8192;
  AutoMemoryPointer pBuf (malloc(bufSize));
  if (pBuf.Get() == 0)
    {
      OUT_OF_MEMORY ("ReportMiKTeXEvent");
    }
  char szAccountName[BufferSizes::MaxPath];
  unsigned long n = BufferSizes::MaxPath;
  if (GetUserName(szAccountName, &n))
    {
      unsigned long sidSize = static_cast<unsigned long>(bufSize);
      pSid = reinterpret_cast<PSID>(pBuf.Get());
      char szDomainName[BufferSizes::MaxPath];
      unsigned long domainNameSize = BufferSizes::MaxPath;
      SID_NAME_USE use;
      if (! LookupAccountName(0,
			      szAccountName,
			      pSid,
			      &sidSize,
			      szDomainName,
			      &domainNameSize,
			      &use))
	{
	  pSid = 0;
	}
    }
  BOOL done =
    ReportEvent(hEventSource,
		eventType,
		0,
		eventId,
		pSid,
		static_cast<unsigned short>(vecStrings.size()),
		0,
		&vecStrings[0],
		0);
  DeregisterEventSource (hEventSource);

  return (done ? true : false);
#endif
}

/* _________________________________________________________________________

   SessionImpl::IsFileAlreadyOpen
   _________________________________________________________________________ */

bool
SessionImpl::IsFileAlreadyOpen (/*[in]*/ const char * lpszFileName)
{
  MIKTEX_ASSERT_STRING (lpszFileName);

  unsigned long error = NO_ERROR;

  HANDLE hFile =
    CreateFileW(PathName(lpszFileName).ToWideCharString().c_str(),
	        GENERIC_READ,
	        FILE_SHARE_READ,
	        0,
	        OPEN_EXISTING,
	        FILE_ATTRIBUTE_NORMAL,
	        0);

  if (hFile == INVALID_HANDLE_VALUE)
    {
      error = ::GetLastError();
    }
  else
    {
      if (! CloseHandle(hFile))
	{
	  FATAL_WINDOWS_ERROR ("CloseHandle", lpszFileName);
	}
    }

  return ((hFile == INVALID_HANDLE_VALUE)
	  && (error == ERROR_SHARING_VIOLATION));
}

/* _________________________________________________________________________
   
   SessionImpl::ScheduleFileRemoval
   _________________________________________________________________________ */

void
SessionImpl::ScheduleFileRemoval (/*[in]*/ const char * lpszFileName)
{
  MIKTEX_ASSERT_STRING (lpszFileName);
  if (IsMiKTeXPortable())
  {
    // todo
    return;
  }
  trace_files->WriteFormattedLine
    ("core",
     T_("scheduling removal of %s"),
     Q_(lpszFileName));
  if (IsWindowsNT())
    {
      if (! MoveFileExW(PathName(lpszFileName).ToWideCharString().c_str(), 0, MOVEFILE_DELAY_UNTIL_REBOOT))
	{
	  FATAL_WINDOWS_ERROR ("MoveFileExW", lpszFileName);
	}
    }
  else
    {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
      char szWinDir[BufferSizes::MaxPath];
      if (GetWindowsDirectoryA(szWinDir, BufferSizes::MaxPath) == 0)
	{
	  FATAL_WINDOWS_ERROR ("GetWindowsDirectoryA", 0);
	}
      PathName pathWinInitIni (szWinDir, "wininit", ".ini");
      vector<string> lines;
      lines.reserve (20);
      char szShortPathName[BufferSizes::MaxPath];
      unsigned long n =
	GetShortPathNameA(lpszFileName, szShortPathName, BufferSizes::MaxPath);
      if (n == 0)
	{
	  FATAL_WINDOWS_ERROR ("GetShortPathNameA", lpszFileName);
	}
      else if (n >= BufferSizes::MaxPath)
	{
	  BUF_TOO_SMALL ("SessionImpl::ScheduleFileRemoval");
	}
      string deleteStatement = "NUL=";
      deleteStatement += szShortPathName;
      bool hasDeleteStatement = false;
      bool hasRenameSection = false;
      bool inRenameSection = false;
      if (File::Exists(pathWinInitIni))
	{
	  StreamReader reader (pathWinInitIni);
	  string line;
	  while (reader.ReadLine(line))
	    {
	      if (line.length() > 0
		  && line[0] == '['
		  && ! hasDeleteStatement)
		{
		  if (_tcsnicmp(line.c_str(), "[rename]", 8) == 0)
		    {
		      hasRenameSection = true;
		      inRenameSection = true;
		    }
		  else if (inRenameSection)
		    {
		      lines.push_back (deleteStatement);
		      hasDeleteStatement = true;
		      inRenameSection = false;
		    }
		}
	      else
		{
		  if (inRenameSection
		      && (_tcsicmp(line.c_str(), deleteStatement.c_str())
			  == 0))
		    {
		      hasDeleteStatement = true;
		    }
		}
	      lines.push_back (line);
	    }
	  reader.Close ();
	}
      if (! hasDeleteStatement)
	{
	  if (! inRenameSection)
	    {
	      lines.push_back ("[rename]");
	    }
	  lines.push_back (deleteStatement);
	}
      StreamWriter writer(pathWinInitIni);
      for (vector<string>::const_iterator it = lines.begin();
	   it != lines.end();
	   ++ it)
	{
	  writer.WriteLine  (*it);
	}
      writer.Close ();
#else
      UNSUPPORTED_PLATFORM ();
#endif
    }
}

/* _________________________________________________________________________

   SessionImpl::IsUserMemberOfGroup

   See MSDN article "Windows NT Security" by Christopher Nefcy.
   _________________________________________________________________________ */

bool
SessionImpl::IsUserMemberOfGroup (/*[in]*/ DWORD localGroup)
{
  if (! IsWindowsNT())
    {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
      return (false)
#else
      UNSUPPORTED_PLATFORM ();
#endif
    }

  HANDLE hThread;

  if (! OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, FALSE, &hThread))
    {
      if (GetLastError() == ERROR_NO_TOKEN)
	{
	  if (! OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hThread))
	    {
	      return (false);
	    }
	}
      else
	{
	  return (false);
	}
    }

  AutoHANDLE autoClose (hThread);

  DWORD cbTokenGroups;

  if (GetTokenInformation(hThread, TokenGroups, 0, 0, &cbTokenGroups))
    {
      return (false);
    }

  if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
      return (false);
    }

  TOKEN_GROUPS *
    ptg = reinterpret_cast<TOKEN_GROUPS *>(_alloca(cbTokenGroups));

  if (ptg == 0)
    {
      return (false);
    }

  if (! GetTokenInformation(hThread,
			    TokenGroups,
			    ptg,
			    cbTokenGroups,
			    &cbTokenGroups))
    {
      return (false);
    }

  SID_IDENTIFIER_AUTHORITY SystemSidAuthority = SECURITY_NT_AUTHORITY;

  PSID psidAdmin;

  if (! AllocateAndInitializeSid(&SystemSidAuthority,
				 2,
				 SECURITY_BUILTIN_DOMAIN_RID,
				 localGroup,
				 0, 0, 0, 0, 0, 0,
				 &psidAdmin))
    {
      return (false);
    }

  AutoSid autoFree (psidAdmin);

  for (size_t i = 0; i < ptg->GroupCount; ++ i)
    {
      if (EqualSid(ptg->Groups[i].Sid, psidAdmin))
	{
	  return (true);
	}
    }

  return (false);
}

/* _________________________________________________________________________

   SessionImpl::IsUserAnAdministrator
   _________________________________________________________________________ */

bool
SessionImpl::IsUserAnAdministrator ()
{
  if (isUserAnAdministrator == TriState::Undetermined)
    {
      if (IsUserMemberOfGroup(DOMAIN_ALIAS_RID_ADMINS))
	{
	  isUserAnAdministrator = TriState::True;
	}
      else
	{
	  isUserAnAdministrator = TriState::False;
	}
    }
  return (isUserAnAdministrator == TriState::True ? true : false);
}

/* _________________________________________________________________________

   SessionImpl::IsUserAPowerUser
   _________________________________________________________________________ */

bool
SessionImpl::IsUserAPowerUser ()
{
  if (isUserAPowerUser == TriState::Undetermined)
    {
      if (IsUserMemberOfGroup(DOMAIN_ALIAS_RID_POWER_USERS))
	{
	  isUserAPowerUser = TriState::True;
	}
      else
	{
	  isUserAPowerUser = TriState::False;
	}
    }
  return (isUserAPowerUser == TriState::True ? true : false);
}

/* _________________________________________________________________________

   SessionImpl::RunningElevated
   _________________________________________________________________________ */

bool
SessionImpl::RunningElevated ()
{
  HANDLE hToken;
  if (! OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &hToken))
    {
      FATAL_WINDOWS_ERROR ("OpenProcessToken", 0);
    }
  DWORD infoLen;
  TOKEN_ELEVATION_TYPE elevationType;
  if (! GetTokenInformation(hToken,
			    TokenElevationType,
			    &elevationType,
			    sizeof(elevationType),
			    &infoLen))
    {
      FATAL_WINDOWS_ERROR ("GetTokenInformation", 0);
    }
  TOKEN_ELEVATION elevation;
  if (! GetTokenInformation(hToken,
			    TokenElevation,
			    &elevation,
			    sizeof(elevation),
			    &infoLen))
    {
      FATAL_WINDOWS_ERROR ("GetTokenInformation", 0);
    }
  switch (elevationType)
    {
    case TokenElevationTypeDefault:
      return (elevation.TokenIsElevated == 0 ? false : true);
    case TokenElevationTypeFull:
      return (true);
    case TokenElevationTypeLimited:
      return (false);
    default:
      UNEXPECTED_CONDITION ("SessionImpl::RunningElevated");
    }
}

/* _________________________________________________________________________

   SessionImpl::RunningAsAdministrator
   _________________________________________________________________________ */

bool
SessionImpl::RunningAsAdministrator ()
{
  if (runningAsAdministrator == TriState::Undetermined)
    {
      if (IsUserAnAdministrator())
	{
	  if (IsWindowsVista() && ! RunningElevated())
	    {
	      runningAsAdministrator = TriState::False;
	    }
	  else
	    {
	      runningAsAdministrator = TriState::True;
	    }
	}
      else
	{
	  runningAsAdministrator = TriState::False;
	}
    }
  return (runningAsAdministrator == TriState::True ? true : false);
}

/* _________________________________________________________________________

   SessionImpl::RunningAsPowerUser
   _________________________________________________________________________ */

bool
SessionImpl::RunningAsPowerUser ()
{
  if (runningAsPowerUser == TriState::Undetermined)
    {
      if (IsUserAPowerUser())
	{
	  if (IsWindowsVista() && ! RunningElevated())
	    {
	      runningAsPowerUser = TriState::False;
	    }
	  else
	    {
	      runningAsPowerUser = TriState::True;
	    }
	}
      else
	{
	  runningAsPowerUser = TriState::False;
	}
    }
  return (runningAsPowerUser == TriState::True ? true : false);
}

/* _________________________________________________________________________

   GetMediaType
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(unsigned int)
GetMediaType (/*[in]*/ const char * lpszPath)
{
  PathName pathRootName;
  if (IsAlpha(lpszPath[0])
      && PathName::IsVolumeDelimiter(lpszPath[1])
      && PathName::IsDirectoryDelimiter(lpszPath[2]))
    {
      CopyString2 (pathRootName.GetBuffer(),
		   pathRootName.GetCapacity(),
		   lpszPath,
		   3);
    }
  else if (! Utils::GetUncRootFromPath(lpszPath, pathRootName))
    {
      return (DRIVE_UNKNOWN);
    }
  return (GetDriveTypeW(pathRootName.ToWideCharString().c_str()));
}

/* _________________________________________________________________________

   FileIsOnROMedia
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(bool)
FileIsOnROMedia (/*[in]*/ const char * lpszPath)
{
  return (GetMediaType(lpszPath) == DRIVE_CDROM);
}

/* _________________________________________________________________________

   CreateDirectoryForEveryone

   Create a directory for the world group.
   _________________________________________________________________________ */

#if SET_SECURITY
MIKTEXSTATICFUNC(void)
CreateDirectoryForEveryone (/*[in]*/ const char * lpszPath)
{
  if (! IsWindowsNT())
    {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
      if (! CreateDirectoryA(lpszPath, 0))
	{
	  FATAL_WINDOWS_ERROR ("CreateDirectoryA", lpszPath);
	}
      return;
#else
      UNSUPPORTED_PLATFORM ();
#endif
    }

  AutoSid pEveryoneSID;
  PACL pACL = 0;

  SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
  if (! AllocateAndInitializeSid(&SIDAuthWorld,
				 1,
				 SECURITY_WORLD_RID,
				 0,
				 0,
				 0,
				 0,
				 0,
				 0,
				 0,
				 &pEveryoneSID))
    {
      FATAL_WINDOWS_ERROR ("AllocateAndInitializeSid", 0);
    }

  EXPLICIT_ACCESSW ea[1];
  ZeroMemory (&ea, sizeof(ea));
  ea[0].grfAccessPermissions = FILE_ALL_ACCESS;
  ea[0].grfAccessMode = GRANT_ACCESS;
  ea[0].grfInheritance= SUB_CONTAINERS_AND_OBJECTS_INHERIT;
  ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
  ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
  ea[0].Trustee.ptstrName =
    reinterpret_cast<wchar_t *>(pEveryoneSID.Get());
  
  if (SetEntriesInAclW(1, ea, 0, &pACL) != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR ("SetEntriesInAcl", 0);
    }

  AutoLocalMemory xxx (reinterpret_cast<void*>(pACL));
  
  PSECURITY_DESCRIPTOR pSD =
    reinterpret_cast<PSECURITY_DESCRIPTOR>
    (_alloca(SECURITY_DESCRIPTOR_MIN_LENGTH));

  if (! InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION)) 
    {
      FATAL_WINDOWS_ERROR ("InitializeSecurityDescriptor", 0);
    } 
 
  if (! SetSecurityDescriptorDacl(pSD,
				  TRUE,
				  pACL,
				  FALSE))
    {
      FATAL_WINDOWS_ERROR ("SetSecurityDescriptorDacl", 0);
    } 

  SECURITY_ATTRIBUTES sa;
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor = pSD;
  sa.bInheritHandle = FALSE;

  if (! CreateDirectoryW(PathName(lpszPath).ToWideCharString().c_str(), &sa))
    {
      FATAL_WINDOWS_ERROR ("CreateDirectoryW", lpszPath);
    }
}
#endif

/* _________________________________________________________________________

   CreateDirectoryPath

   Create a directory path.
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(void)
CreateDirectoryPath (/*[in]*/ const char *	lpszPath)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  if (! Utils::IsAbsolutePath(lpszPath))
    {
      PathName path (lpszPath);
      path.MakeAbsolute ();
      CreateDirectoryPath (path.Get());
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
  CreateDirectoryPath (pathParent.Get());
  // </recursivecall>

  // we're done, if szFname is empty (this happens when lpszPath ends
  // with a directory delimiter)
  if (szFname[0] == 0 && szExt[0] == 0)
    {
      return;
    }

  SessionWrapper pSession (true);

#if SET_SECURITY
  // create the directory itself
  if (IsWindowsNT()
      && pSession->IsAdminMode()
      && (pSession->GetSpecialPath(SpecialPath::CommonConfigRoot) == lpszPath
	  || pSession->GetSpecialPath(SpecialPath::CommonDataRoot) == lpszPath)
      && pSession->RunningAsAdministrator())
    {
      CreateDirectoryForEveryone (lpszPath);
    }
  else if (! CreateDirectoryW(PathName(lpszPath).ToWideCharString().c_str(), 0))
    {
      FATAL_WINDOWS_ERROR ("CreateDirectoryW", lpszPath);
    }
#else
  if (! CreateDirectoryW(PathName(lpszPath).ToWideCharString().c_str(), 0))
    {
      FATAL_WINDOWS_ERROR ("CreateDirectoryW", lpszPath);
    }
#endif
}

/* _________________________________________________________________________

   PathName::AppendAltDirectoryDelimiter
   _________________________________________________________________________ */

PathName &
PathName::AppendAltDirectoryDelimiter ()
{
  size_t l = GetLength();
  if (l == 0 || ! IsDirectoryDelimiter(CharBuffer::operator[](l - 1)))
    {
      CharBuffer::Append (AltDirectoryDelimiter);
    }
  return (*this);
}

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
  SessionImpl::GetSession()->trace_config->WriteFormattedLine
    ("core",
     T_("setting env %s=%s"),
     lpszValueName,
     lpszValue);
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
  if (_putenv_s(lpszValueName, lpszValue) != 0)
    {
      FATAL_CRT_ERROR ("_putenv_s", lpszValueName);
    }
#else
  string str = lpszValueName;
  str += '=';
  str += lpszValue;
  if (putenv(str.c_str()) != 0)
    {
      FATAL_CRT_ERROR ("putenv", str.c_str());
    }
#endif
}

/* _________________________________________________________________________

   ShellExecuteURL
   
   See MSDN community content:

   Using lpExecInfo->lpClass and lpExecInfo->hkeyClass (KJK_Hyperion)

   If you are afraid ShellExecute(Ex) might misidentify the file type,
   because the path might be ambiguous or if you use a non-standard
   type system, you can force one by filling the lpClass or hkeyClass
   fields in lpExecInfo. You set lpClass to either the extension
   (e.g. ".txt") or the URL scheme (e.g. "http") or the OLE
   ProgId/ClassId; better yet, you use AssocQueryKey to retrieve the
   appropriate hkeyClass for the given class name and verb. For
   example, this is how you safely execute URLs with ShellExecute(Ex):
   _________________________________________________________________________ */

BOOL ShellExecuteURLExInternal(LPSHELLEXECUTEINFOW lpExecInfo)
{
    BOOL bRet;
    DWORD dwErr;
    HRESULT hr;
    PARSEDURLW pu;
    wchar_t szSchemeBuffer[INTERNET_MAX_SCHEME_LENGTH + 1];
    HKEY hkeyClass;
 
    /* Default error codes */
    bRet = FALSE;
    dwErr = ERROR_INVALID_PARAMETER;
    hr = S_OK;
 
    lpExecInfo->hInstApp =
        (HINSTANCE)UlongToHandle(SE_ERR_ACCESSDENIED);
 
    /* Validate parameters */
    if
    (
        lpExecInfo->cbSize == sizeof(*lpExecInfo) &&
        lpExecInfo->lpFile != NULL &&
        (lpExecInfo->fMask & SEE_MASK_INVOKEIDLIST) == 0 &&
        (lpExecInfo->fMask & SEE_MASK_CLASSNAME) == 0 &&
        (lpExecInfo->fMask & 0x00400000) == 0 /* SEE_MASK_FILEANDURL */
    )
    {
        /* Extract the scheme out of the URL */
        pu.cbSize = sizeof(pu);
        hr = ParseURLW(lpExecInfo->lpFile, &pu);
 
        /* Is the URL really, unambiguously an URL? */
        if
        (
            SUCCEEDED(hr) &&
            pu.pszProtocol == lpExecInfo->lpFile &&
            pu.pszProtocol[pu.cchProtocol] == L':'
        )
        {
            /* We need the scheme name NUL-terminated, so we copy it */
            hr = StringCbCopyNW
            (
                szSchemeBuffer,
                sizeof(szSchemeBuffer),
                pu.pszProtocol,
                pu.cchProtocol * sizeof(wchar_t)
            );
 
            if(SUCCEEDED(hr))
            {
                /* Is the URL scheme a registered ProgId? */
                hr = AssocQueryKeyW
                (
                    ASSOCF_INIT_IGNOREUNKNOWN,
                    ASSOCKEY_CLASS,
                    szSchemeBuffer,
                    NULL,
                    &hkeyClass
                );
 
                if(SUCCEEDED(hr))
                {
                    /* Is the ProgId really an URL scheme? */
                    dwErr = RegQueryValueExW
                    (
                        hkeyClass,
                        L"URL Protocol",
                        NULL,
                        NULL,
                        NULL,
                        NULL
                    );
 
                    /* All clear! */
                    if(dwErr == NO_ERROR || dwErr == ERROR_MORE_DATA)
                    {
                        /* Don't let ShellExecuteEx guess */
                        lpExecInfo->fMask |= SEE_MASK_CLASSKEY;
                        lpExecInfo->lpClass = NULL;
                        lpExecInfo->hkeyClass = hkeyClass;
 
                        /* Finally, execute the damn URL */
                        bRet = ShellExecuteExW(lpExecInfo);
 
                        /* To preserve ShellExecuteEx's last error */
                        dwErr = NO_ERROR;
                    }
 
                    RegCloseKey(hkeyClass);
                }
            }
        }
    }
 
    /* Last error was a HRESULT */
    if(FAILED(hr))
    {
        /* Try to dissect it */
        if(HRESULT_FACILITY(hr) == FACILITY_WIN32)
            dwErr = HRESULT_CODE(hr);
        else
            dwErr = hr;
    }
 
    /* We have a last error to set */
    if(dwErr)
        SetLastError(dwErr);
 
    return bRet;
}
 
BOOL ShellExecuteURLEx(LPSHELLEXECUTEINFOW lpExecInfo)
{
    BOOL bRet;
    SHELLEXECUTEINFOW ExecInfo;
 
    /* We use a copy of the parameters, because you never know */
    CopyMemory(&ExecInfo, lpExecInfo, sizeof(ExecInfo));
 
    /* Do the magic */
    bRet = ShellExecuteURLExInternal(&ExecInfo);
 
    /* These need to be copied back */
    lpExecInfo->hInstApp = ExecInfo.hInstApp;
    lpExecInfo->hProcess = ExecInfo.hProcess;
    return bRet;
}
 
HINSTANCE ShellExecuteURL
(
    HWND hwnd,
    const wchar_t * lpOperation,
    const wchar_t * lpFile,
    const wchar_t * lpParameters,
    const wchar_t * lpDirectory,
    INT nShowCmd
)
{
    SHELLEXECUTEINFOW ExecuteInfo;
 
    ExecuteInfo.fMask = SEE_MASK_FLAG_NO_UI; /* Odd but true */
    ExecuteInfo.hwnd = hwnd;
    ExecuteInfo.cbSize = sizeof(ExecuteInfo);
    ExecuteInfo.lpVerb = lpOperation;
    ExecuteInfo.lpFile = lpFile;
    ExecuteInfo.lpParameters = lpParameters;
    ExecuteInfo.lpDirectory = lpDirectory;
    ExecuteInfo.nShow = nShowCmd;
 
    ShellExecuteURLExInternal(&ExecuteInfo);
 
    return ExecuteInfo.hInstApp;
}

// Note that we use ParseURL instead of more sophisticated functions
// like InternetCrackUrl because it's much more forgiving, and we only
// need to extract the scheme anyway

/* _________________________________________________________________________

   Utils::ShowWebPage
   _________________________________________________________________________ */

void
Utils::ShowWebPage (/*[in]*/ const char * lpszUrl)
{
  HINSTANCE hInst =
    ShellExecuteURL(0,
		    0,
		    Utils::AnsiToWideChar(lpszUrl).c_str(),
		    0,
		    0,
		    SW_SHOWNORMAL);
  if (reinterpret_cast<int>(hInst) <= 32)
    {
      FATAL_MIKTEX_ERROR ("Utils::ShowWebPage",
			  T_("The web browser could not be started."),
			  NUMTOSTR(reinterpret_cast<int>(hInst)));
    }
}

/* _________________________________________________________________________

   Utils::RegisterMiKTeXUser
   _________________________________________________________________________ */

void
Utils::RegisterMiKTeXUser ()
{
  ShowWebPage (MIKTEX_URL_WWW_GIVE_BACK);
}

/* _________________________________________________________________________

   Utils::IsRegisteredMiKTeXUser
   _________________________________________________________________________ */

bool
Utils::IsRegisteredMiKTeXUser ()
{
  return (false);
}

/* _________________________________________________________________________

   Argv::Append

   Mimic the behaviour of CommandLineToArgvW().
   _________________________________________________________________________ */

void
Argv::Append (/*[in]*/ const char *	lpszArguments)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszArguments);
  MIKTEX_ASSERT (argv.size() > 0);

  argv.pop_back ();

  if (argv.size() == 0)
    {
      argv.push_back (MIKTEX_STRDUP("foo"));
    }

  for (const char * lpsz = lpszArguments; *lpsz != 0; )
    {
      // skip white space
      for (; *lpsz == ' ' || *lpsz == '\t'; ++ lpsz)
	{
	}

      if (*lpsz == 0)
	{
	  break;
	}

      // get the next argument
      string arg;
      bool inQuotation = false;
      for (; ; ++ lpsz)
	{
	  // count backslashes
	  size_t nBackslashes = 0;
	  for (; *lpsz == '\\'; ++ lpsz)
	    {
	      ++ nBackslashes;
	    }

	  bool quoteOrUnquote = false;

	  // quotation mark madness
	  if (lpsz[0] == '"')
	    {
	      if (nBackslashes % 2 == 0)
		{
		  if (lpsz[1] == '"' && inQuotation)
		    {
		      // two quotation marks in quotation => one
		      // quotation mark
		      ++ lpsz;
		    }
		  else
		    {
		      // start/end of quotation
		      inQuotation = ! inQuotation;
		      quoteOrUnquote = true;
		    }
		}
	      nBackslashes /= 2;
	    }

	  // write backslashes
	  for (; nBackslashes > 0; -- nBackslashes)
	    {
	      arg += '\\';
	    }

	  // check to see if we have the complete argument
	  if (*lpsz == 0
	      || ((*lpsz == ' ' || *lpsz == '\t')
		  && ! inQuotation))
	    {
	      argv.push_back (MIKTEX_STRDUP(arg.c_str()));
	      break;
	    }
	  else if (! quoteOrUnquote)
	    {
	      arg += *lpsz;
	    }
	}
    }

  argv.push_back (0);
}

/* _________________________________________________________________________

   UTF8ToWideChar
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(wchar_t*)
UTF8ToWideChar (/*[in]*/ const char * lpszUtf8,
		/*[in,out]*/ size_t & sizeWideChar,
		/*[out]*/ wchar_t *   lpszWideChar)
{
  MIKTEX_ASSERT (Utils::IsUTF8(lpszUtf8));
  MIKTEX_ASSERT (sizeWideChar == 0 || lpszWideChar != 0);
  MIKTEX_ASSERT (sizeWideChar != 0 || lpszWideChar == 0);
  MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL (lpszWideChar, sizeWideChar);
  if (*lpszUtf8 == 0)
    {
      if (lpszWideChar != 0 && sizeWideChar > 0)
      {
	*lpszWideChar = 0;
      }
      sizeWideChar = 0;
      return (lpszWideChar);
    }
  else if (sizeWideChar == 0)
  {
    int len = MultiByteToWideChar(
      CP_UTF8,
      MB_ERR_INVALID_CHARS,
      lpszUtf8,
      -1,
      0,
      0);
    if (len <= 0)
    {
      FATAL_WINDOWS_ERROR ("MultiByteToWideChar", 0);
    }
    sizeWideChar = len;
    return (0);
  }
  else
  {
    int len = MultiByteToWideChar(
      CP_UTF8,
      MB_ERR_INVALID_CHARS,
      lpszUtf8,
      -1,
      lpszWideChar,
      sizeWideChar);
    if (len <= 0)
    {
      FATAL_WINDOWS_ERROR ("MultiByteToWideChar", 0);
    }
    sizeWideChar = len;
    return (lpszWideChar);
  }
}

/* _________________________________________________________________________

   WideCharToUTF8
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(char*)
WideCharToUTF8 (/*[in]*/ const wchar_t *  lpszWideChar,
		/*[in,out]*/ size_t &	  sizeUtf8,
		/*[out]*/ char *	  lpszUtf8)
{
  MIKTEX_ASSERT (sizeUtf8 == 0 || lpszUtf8 != 0);
  MIKTEX_ASSERT (sizeUtf8 != 0 || lpszUtf8 == 0);
  MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL (lpszUtf8, sizeUtf8);
  if (*lpszWideChar == 0)
    {
      if (lpszUtf8 != 0 && sizeUtf8 > 0)
      {
	*lpszUtf8 = 0;
      }
      sizeUtf8 = 0;
      return (lpszUtf8);
    }
  else if (sizeUtf8 == 0)
  {
    int len = WideCharToMultiByte(
      CP_UTF8,
      0,
      lpszWideChar,
      -1,
      0,
      0,
      0,
      0);
    if (len <= 0)
    {
      FATAL_WINDOWS_ERROR ("WideCharToMultiByte", 0);
    }
    sizeUtf8 = len;
    return (0);
  }
  else
  {
    int len = WideCharToMultiByte(
      CP_UTF8,
      0,
      lpszWideChar,
      -1,
      lpszUtf8,
      sizeUtf8,
      0,
      0);
    if (len <= 0)
    {
      FATAL_WINDOWS_ERROR ("WideCharToMultiByte", 0);
    }
    sizeUtf8 = len;
    return (lpszUtf8);
  }
}

/* _________________________________________________________________________

   Utils::UTF8ToWideChar
   _________________________________________________________________________ */

wstring
Utils::UTF8ToWideChar (/*[in]*/ const char * lpszUtf8)
{
  size_t len = 0;
  ::UTF8ToWideChar (lpszUtf8, len, 0);
  CharBuffer<wchar_t, 200> buf (len);
  ::UTF8ToWideChar (lpszUtf8, len, buf.GetBuffer());
  return (buf.Get());
}

/* _________________________________________________________________________

   miktex_utf8_to_wide_char
   _________________________________________________________________________ */

MIKTEXCEEAPI(wchar_t*)
miktex_utf8_to_wide_char (/*[in]*/ const char *	lpszUtf8,
			  /*[in]*/ size_t	sizeWideChar,
			  /*[out]*/ wchar_t *	lpszWideChar)
{
  C_FUNC_BEGIN();
  return (UTF8ToWideChar(lpszUtf8, sizeWideChar, lpszWideChar));
  C_FUNC_END();
}

/* _________________________________________________________________________

   Utils::WideCharToUTF8
   _________________________________________________________________________ */

string
Utils::WideCharToUTF8 (/*[in]*/ const wchar_t * lpszWideChar)
{
  size_t len = 0;
  ::WideCharToUTF8 (lpszWideChar, len, 0);
  CharBuffer<char, 200> buf (len);
  ::WideCharToUTF8 (lpszWideChar, len, buf.GetBuffer());
  return (buf.Get());
}

/* _________________________________________________________________________

   miktex_wide_char_to_utf8
   _________________________________________________________________________ */

MIKTEXCEEAPI(char*)
miktex_wide_char_to_utf8 (/*[in]*/ const wchar_t *  lpszWideChar,
			  /*[in]*/ size_t	    sizeUtf8,
			  /*[out]*/ char *	    lpszUtf8)
{
  C_FUNC_BEGIN();
  return (WideCharToUTF8(lpszWideChar, sizeUtf8, lpszUtf8));
  C_FUNC_END();
}

/* _________________________________________________________________________

   Utils::WideCharToAnsi
   _________________________________________________________________________ */

char *
Utils::WideCharToAnsi (/*[in]*/ const wchar_t * lpszWideChar,
		       /*[out]*/ char *		lpszAnsi,
		       /*[in]*/ size_t		size)
{
  if (*lpszWideChar == 0)
    {
      return ("");
    }
  int n = WideCharToMultiByte
    (CP_ACP,
     WC_NO_BEST_FIT_CHARS,
     lpszWideChar,
     -1,
     lpszAnsi,
     size,
     0,
     FALSE);
  if (n == 0)
    {
      FATAL_WINDOWS_ERROR ("WideCharToMultiByte", 0);
    }
  if (n < 0)
    {
      UNEXPECTED_CONDITION ("Utils::WideCharToAnsi");
    }
  return (lpszAnsi);
}

/* _________________________________________________________________________

   Utils::WideCharToAnsi
   _________________________________________________________________________ */

string
Utils::WideCharToAnsi (/*[in]*/ const wchar_t * lpszWideChar)
{
  CharBuffer<char, 512> buf (wcslen(lpszWideChar) + 1);
  return (WideCharToAnsi(lpszWideChar, buf.GetBuffer(), buf.GetCapacity()));
}

/* _________________________________________________________________________

   Utils::AnsiToWideChar
   _________________________________________________________________________ */

wchar_t *
Utils::AnsiToWideChar (/*[in]*/ const char *	lpszAnsi,
		       /*[out]*/ wchar_t *	lpszWideChar,
			/*[in]*/ size_t		size)
{
  if (*lpszAnsi == 0)
    {
      return (L"");
    }
  int n = MultiByteToWideChar
    (CP_ACP,
     0,
     lpszAnsi,
     -1,
     lpszWideChar,
     size);
  if (n == 0)
    {
      FATAL_WINDOWS_ERROR ("MultiByteToWideChar", lpszAnsi);
    }
  if (n < 0)
    {
      UNEXPECTED_CONDITION ("Utils::AnsiToWideChar");
    }
  return (lpszWideChar);
}

/* _________________________________________________________________________

   Utils::AnsiToWideChar
   _________________________________________________________________________ */

wstring
Utils::AnsiToWideChar (/*[in]*/ const char * lpszAnsi)
{
  CharBuffer<wchar_t, 512> buf (strlen(lpszAnsi) + 1);
  return (AnsiToWideChar(lpszAnsi, buf.GetBuffer(), buf.GetCapacity()));
}

/* _________________________________________________________________________

   Utils::CheckHeap
   _________________________________________________________________________ */

void
Utils::CheckHeap ()
{
  int heapStatus = _heapchk();
  switch (heapStatus)
    {
    case _HEAPOK:
      break;
    case _HEAPEMPTY:
      break;
    case _HEAPBADBEGIN:
      FATAL_MIKTEX_ERROR ("Utils::CheckHeap",
			  T_("The heap is corrupted (_HEAPBADBEGIN)."),
			  0);
      break;
    case _HEAPBADNODE:
      FATAL_MIKTEX_ERROR ("Utils::CheckHeap",
			  T_("The heap is corrupted (_HEAPBADNODE)."),
			  0);
      break;
    case _HEAPBADPTR:
      FATAL_MIKTEX_ERROR ("Utils::CheckHeap",
			  T_("The heap is corrupted (_HEAPBADPTR)."),
			  0);
      break;
    }
#if defined(_DEBUG)
  if (_CrtCheckMemory() == 0)
    {
      FATAL_MIKTEX_ERROR ("Utils::CheckHeap",
			  T_("The heap is corrupted."),
			  0);
    }
#endif
}

/* _________________________________________________________________________

   miktex_ansi_to_utf8 
   _________________________________________________________________________ */

MIKTEXCEEAPI(char*)
miktex_ansi_to_utf8 (/*[in]*/ const char *	lpszAnsi,
		     /*[in]*/ size_t		sizeUtf8,
		     /*[out]*/ char *		lpszUtf8)
{
  C_FUNC_BEGIN ();
  string utf8 = Utils::AnsiToUTF8(lpszAnsi);
  Utils::CopyString (lpszUtf8, sizeUtf8, utf8.c_str());
  return (lpszUtf8);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_utf8_to_ansi
   _________________________________________________________________________ */

MIKTEXCEEAPI(char*)
miktex_utf8_to_ansi (/*[in]*/ const char *	lpszUtf8,
		     /*[in]*/ size_t		sizeAnsi,
		     /*[out]*/ char *		lpszAnsi)
{
  C_FUNC_BEGIN ();
  string ansi = Utils::UTF8ToAnsi(lpszUtf8);
  Utils::CopyString (lpszAnsi, sizeAnsi, ansi.c_str());
  return (lpszAnsi);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   CheckPath
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
CheckPath (/*[in]*/ const string &	oldPath,
	   /*[in]*/ const PathName &	binDirArg,
	   /*[out]*/ string &		newPath,
	   /*[out]*/ bool &		competition)
{
  bool modified = false;
  bool found = false;
  competition = false;
  newPath = "";
  PathName binDir = binDirArg;
  binDir.AppendDirectoryDelimiter ();
  for (CSVList entry (oldPath.c_str(), PathName::PathNameDelimiter);
       entry.GetCurrent() != 0;
       ++ entry)
    {
      PathName dir (entry.GetCurrent());
      dir.AppendDirectoryDelimiter ();
      if (binDir == dir)
	{
	  if (found)
	    {
	      // prevent duplicates
	      continue;
	    }
	  found = true;
	}
      else
	{
	  PathName otherPdfTeX (dir);
	  otherPdfTeX += "pdftex.exe";
	  if (! found && File::Exists(otherPdfTeX))
	    {
	      int exitCode;
	      CharBuffer<char> versionInfo (4096);
	      size_t n = static_cast<unsigned>(versionInfo.GetCapacity());
	      bool isOtherPdfTeX = true;
	      if (Process::Run(otherPdfTeX.Get(),
			       "--version",
			       versionInfo.GetBuffer(),
			       &n,
			       &exitCode)
		  && exitCode == 0)
		{
		  versionInfo.GetBuffer()[versionInfo.GetCapacity() - 1] = 0;
		  if (strstr(versionInfo.Get(), "MiKTeX") != 0)
		    {
		      isOtherPdfTeX = false;
		    }
		}
	      if (isOtherPdfTeX)
		{
		  // another TeX system is in our way; push it out
		  // from this place
		  if (! newPath.empty())
		    {
		      newPath += PathName::PathNameDelimiter;
		    }
		  newPath += binDir.Get();
		  found = true;
		  modified = true;
		  competition = true;
		}
	    }
	}
      if (! newPath.empty())
	{
	  newPath += PathName::PathNameDelimiter;
	}
      newPath += entry.GetCurrent();
    }
  if (! found)
    {
      // MiKTeX is not yet in the PATH
      if (! newPath.empty())
	{
	  newPath += PathName::PathNameDelimiter;
	}
      newPath += binDir.Get();
      modified = true;
    }
  return (! modified);
}

/* _________________________________________________________________________

   Utils::CheckPath
   _________________________________________________________________________ */

bool
Utils::CheckPath (/*[in]*/ bool repair)
{
#define REGSTR_KEY_ENVIRONMENT_COMMON \
   L"System\\CurrentControlSet\\Control\\Session Manager\\Environment"

#define REGSTR_KEY_ENVIRONMENT_USER L"Environment"
  
  SessionWrapper pSession (true);

  wstring systemPath;

  if (! winRegistry::TryGetRegistryValue(HKEY_LOCAL_MACHINE,
				         REGSTR_KEY_ENVIRONMENT_COMMON,
				         L"Path",
					 systemPath,
					 0))
  {
    systemPath = L"";
  }

  wstring userPath;

  if (! pSession->IsAdminMode())
  {
    if (! winRegistry::TryGetRegistryValue(HKEY_CURRENT_USER,
					   REGSTR_KEY_ENVIRONMENT_USER,
					   L"Path",
					   userPath,
					   0))
    {
      userPath = L"";
    }
  }

  PathName commonBinDir = pSession->GetSpecialPath(SpecialPath::CommonInstallRoot);
  commonBinDir += MIKTEX_PATH_BIN_DIR;

  string repairedSystemPath;

  bool systemPathCompetition;

  bool systemPathOkay = (
    ! Directory::Exists(commonBinDir)
    || ::CheckPath(WA_(systemPath), commonBinDir, repairedSystemPath, systemPathCompetition));

  bool repaired = false;

  bool userPathOkay = true;

  if (pSession->IsAdminMode())
  {
    if (! systemPathOkay && ! repair)
    {
      SessionImpl::GetSession()->trace_error->WriteLine
	("core",
	T_("Something is wrong with the system PATH:"));
      SessionImpl::GetSession()->trace_error->WriteLine
	("core",
	WA_(systemPath.c_str()));
    }
    else if (! systemPathOkay && repair)
    {
      SessionImpl::GetSession()->trace_error->WriteLine
	("core",
	T_("Setting new system PATH:"));
      SessionImpl::GetSession()->trace_error->WriteLine
	("core",
	repairedSystemPath.c_str());
      systemPath = AW_(repairedSystemPath.c_str());
      winRegistry::SetRegistryValue (HKEY_LOCAL_MACHINE,
				     REGSTR_KEY_ENVIRONMENT_COMMON,
				     L"Path",
				     systemPath.c_str());
      systemPathOkay = true;
      repaired = true;
    }
  }
  else
  {
    if (! systemPathOkay && ! systemPathCompetition)
    {
      string repairedUserPath;
      bool userPathCompetition;
      systemPathOkay = ::CheckPath(WA_(userPath), commonBinDir, repairedUserPath, userPathCompetition);
      if (! systemPathOkay && repair)
      {
	SessionImpl::GetSession()->trace_error->WriteLine
	  ("core",
	  T_("Setting new user PATH:"));
	SessionImpl::GetSession()->trace_error->WriteLine
	  ("core",
	  repairedUserPath.c_str());
	userPath = AW_(repairedUserPath);
	winRegistry::SetRegistryValue (HKEY_CURRENT_USER,
				       REGSTR_KEY_ENVIRONMENT_USER,
				       L"Path",
				       userPath.c_str());
	systemPathOkay = true;
	repaired = true;
      }
    }
    PathName userBinDir = pSession->GetSpecialPath(SpecialPath::UserInstallRoot);
    userBinDir += MIKTEX_PATH_BIN_DIR;
    string repairedUserPath;
    bool userPathCompetition;
    userPathOkay = (
      ! Directory::Exists(userBinDir)
      || ::CheckPath(WA_(userPath), userBinDir, repairedUserPath, userPathCompetition));
    if (! userPathOkay && repair)
    {
      SessionImpl::GetSession()->trace_error->WriteLine
	("core",
	T_("Setting new user PATH:"));
      SessionImpl::GetSession()->trace_error->WriteLine
	("core",
	repairedUserPath.c_str());
      userPath = AW_(repairedUserPath);
      winRegistry::SetRegistryValue (HKEY_CURRENT_USER,
				     REGSTR_KEY_ENVIRONMENT_USER,
				     L"Path",
				     userPath.c_str());
      userPathOkay = true;
      repaired = true;
    }
  }

  if (repaired)
  {
    DWORD_PTR sendMessageResult;
    if (SendMessageTimeoutW(HWND_BROADCAST,
			    WM_SETTINGCHANGE,
			    0,
			    reinterpret_cast<LPARAM>(L"Environment"),
			    SMTO_ABORTIFHUNG,
			    5000,
			    &sendMessageResult)
	== 0)
    {
      if (::GetLastError() != ERROR_SUCCESS)
      {
	FATAL_WINDOWS_ERROR ("SendMessageTimeoutW", 0);
      }
    }
  }

  return (repaired || (systemPathOkay && userPathOkay));
}

/* _________________________________________________________________________

   Utils::CanonicalizePathName
   _________________________________________________________________________ */

void
Utils::CanonicalizePathName (/*[in,out]*/ PathName & path)
{
  wchar_t szFullPath[BufferSizes::MaxPath];
  DWORD n = GetFullPathNameW(path.ToWideCharString().c_str(),
			     BufferSizes::MaxPath,
			     szFullPath,
			     0);
  if (n == 0)
    {
      FATAL_WINDOWS_ERROR ("GetFullPathNameW", path.Get());
    }
  if (n >= BufferSizes::MaxPath)
    {
      BUF_TOO_SMALL ("Utils::CanonicalizePathName");
    }
  path = szFullPath;
}

/* _________________________________________________________________________

   Utils::RegisterShellFileAssoc
   _________________________________________________________________________ */

void
Utils::RegisterShellFileAssoc (/*[in]*/ const char * lpszExtension,
			       /*[in]*/ const char * lpszProgId,
			       /*[in]*/ bool	     takeOwnership)
{
  MIKTEX_ASSERT_STRING (lpszExtension);
  MIKTEX_ASSERT_STRING (lpszProgId);
  PathName regPath ("Software\\Classes");
  regPath += lpszExtension;
  string otherProgId;
  bool haveOtherProgId = false;
  if (! SessionWrapper(true)->IsAdminMode())
  {
    haveOtherProgId = winRegistry::TryGetRegistryValue(
      HKEY_CURRENT_USER,
      regPath.Get(),
      "",
      otherProgId,
      0);
    haveOtherProgId =
      (haveOtherProgId && StringCompare(lpszProgId, otherProgId.c_str(), true) != 0);
  }
  if (! haveOtherProgId)
  {
    haveOtherProgId = winRegistry::TryGetRegistryValue(
      HKEY_LOCAL_MACHINE,
      regPath.Get(),
      "",
      otherProgId,
      0);
    haveOtherProgId =
      (haveOtherProgId && StringCompare(lpszProgId, otherProgId.c_str(), true) != 0);
  }
  HKEY hkeyRoot =
    (SessionWrapper(true)->IsAdminMode() ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER);
  PathName openWithProgIds (regPath);
  openWithProgIds += "OpenWithProgIds";
  if (haveOtherProgId)
  {
    winRegistry::SetRegistryValue (
      hkeyRoot,
      openWithProgIds.Get(),
      otherProgId.c_str(),
      "");
    winRegistry::SetRegistryValue (
      hkeyRoot,
      openWithProgIds.Get(),
      lpszProgId,
      "");
  }
  if (! haveOtherProgId || takeOwnership)
  {
    if (haveOtherProgId)
    {
      winRegistry::SetRegistryValue (
	hkeyRoot,
	regPath.Get(),
	"MiKTeX." MIKTEX_SERIES_STR ".backup",
	otherProgId.c_str());
    }
    winRegistry::SetRegistryValue (
      hkeyRoot,
      regPath.Get(),
      "",
      lpszProgId);
  }
}

/* _________________________________________________________________________

   Utils::UnregisterShellFileAssoc
   _________________________________________________________________________ */

void
Utils::UnregisterShellFileAssoc (/*[in]*/ const char * lpszExtension,
				 /*[in]*/ const char * lpszProgId)
{
  MIKTEX_ASSERT_STRING (lpszExtension);
  HKEY hkeyRoot =
    (SessionWrapper(true)->IsAdminMode() ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER);
  PathName regPath ("Software\\Classes");
  regPath += lpszExtension;
  string progId;
  if (! winRegistry::TryGetRegistryValue(
    hkeyRoot,
    regPath.Get(),
    "",
    progId,
    0))
  {
    return;
  }
  string backupProgId;
  bool haveBackupProgId = winRegistry::TryGetRegistryValue(
    hkeyRoot,
    regPath.Get(),
    "MiKTeX." MIKTEX_SERIES_STR ".backup",
    backupProgId,
    0);
  if (haveBackupProgId || StringCompare(progId.c_str(), lpszProgId, true) != 0)
  {
    if (haveBackupProgId)
    {
      winRegistry::SetRegistryValue (
	hkeyRoot,
	regPath.Get(),
	"",
	backupProgId.c_str());
      winRegistry::TryDeleteRegistryValue (
	hkeyRoot,
	regPath.Get(), 
	"MiKTeX." MIKTEX_SERIES_STR ".backup");
    }
    PathName openWithProgIds (regPath);
    openWithProgIds += "OpenWithProgIds";
    winRegistry::TryDeleteRegistryValue (
      hkeyRoot,
      openWithProgIds.Get(),
      lpszProgId);
  }
  else
  {
    winRegistry::TryDeleteRegistryKey (hkeyRoot, regPath.Get());
  }
}

/* _________________________________________________________________________

   Utils::RegisterShellFileType
   _________________________________________________________________________ */

void
Utils::RegisterShellFileType (/*[in]*/ const char * lpszProgId,
			      /*[in]*/ const char * lpszUserFriendlyName,
			      /*[in]*/ const char * lpszIconPath)
{
  MIKTEX_ASSERT_STRING (lpszProgId);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszUserFriendlyName);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszIconPath);
  HKEY hkeyRoot =
    (SessionWrapper(true)->IsAdminMode() ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER);
  PathName regPath ("Software\\Classes");
  regPath += lpszProgId;
  if (lpszUserFriendlyName != 0)
  {
    winRegistry::SetRegistryValue (
      hkeyRoot,
      regPath.Get(),
      "",
      lpszUserFriendlyName);
  }
  if (lpszIconPath != 0)
  {
    PathName defaultIcon (regPath);
    defaultIcon += "DefaultIcon";
    winRegistry::SetRegistryValue (
      hkeyRoot,
      defaultIcon.Get(),
      "",
      lpszIconPath);
  }
}

/* _________________________________________________________________________

   Utils::UnregisterShellFileType
   _________________________________________________________________________ */

void
Utils::UnregisterShellFileType (/*[in]*/ const char * lpszProgId)
{
  MIKTEX_ASSERT_STRING (lpszProgId);
  HKEY hkeyRoot =
    (SessionWrapper(true)->IsAdminMode() ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER);
  PathName regPath ("Software\\Classes");
  regPath += lpszProgId;
  winRegistry::TryDeleteRegistryKey (hkeyRoot, regPath.Get());
}

/* _________________________________________________________________________

   Utils::RegisterShellVerb
   _________________________________________________________________________ */

void
Utils::RegisterShellVerb (/*[in]*/ const char * lpszProgId,
			  /*[in]*/ const char * lpszVerb,
			  /*[in]*/ const char * lpszCommand,
			  /*[in]*/ const char * lpszDdeExec)
{
  MIKTEX_ASSERT_STRING (lpszProgId);
  MIKTEX_ASSERT_STRING (lpszVerb);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszCommand);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszDdeExec);
  HKEY hkeyRoot =
    (SessionWrapper(true)->IsAdminMode() ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER);
  PathName regPath ("Software\\Classes");
  regPath += lpszProgId;
  if (lpszCommand != 0)
  {
    PathName path (regPath);
    path += "shell";
    path += lpszVerb;
    path += "command";
    winRegistry::SetRegistryValue (
      hkeyRoot,
      path.Get(),
      "",
      lpszCommand);
  }
  if (lpszDdeExec != 0)
  {
    PathName path (regPath);
    path += "shell";
    path += lpszVerb;
    path += "ddeexec";
    winRegistry::SetRegistryValue (
      hkeyRoot,
      path.Get(),
      "",
      lpszDdeExec);
  }
}

/* _________________________________________________________________________

   Utils::MakeProgId
   _________________________________________________________________________ */

string
Utils::MakeProgId (/*[in]*/ const char * lpszComponent)
{
  string progId ("MiKTeX.");
  progId += lpszComponent;
  progId += "." MIKTEX_SERIES_STR;
  return (progId);
}

/* _________________________________________________________________________

   HResult::~HResult
   _________________________________________________________________________ */

HResult::~HResult ()
{
  try
  {
    if (lpszMessage != 0)
    {
      LocalFree (reinterpret_cast<HLOCAL>(lpszMessage));
    }
  }
  catch (const exception &)
  {
  }
}

/* _________________________________________________________________________

   HResult::ToString
   _________________________________________________________________________ */

string
HResult::ToString ()
  const
{
  string ret;
  ret += NUMTOSTR(GetSeverity());
  ret += ':';
  ret += NUMTOSTR(GetFacility());
  ret += ':';
  ret += NUMTOSTR(GetCode());
  return (ret);
}

/* _________________________________________________________________________

   HResult::GetText
   _________________________________________________________________________ */

const char *
HResult::GetText ()
{
  if (lpszMessage == 0)
  {
    // FIXME: use Unicode version
    FormatMessageA (FORMAT_MESSAGE_ALLOCATE_BUFFER|
      FORMAT_MESSAGE_FROM_SYSTEM|
      FORMAT_MESSAGE_IGNORE_INSERTS,
      0,
      hr,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      reinterpret_cast<char*>(&lpszMessage),
      0,
      0);
    if (lpszMessage == 0)
    {
      string str = ToString();
      size_t size = str.length() + 1;
      lpszMessage = reinterpret_cast<char*>
	(LocalAlloc(0,(size * sizeof(char))));
      if (lpszMessage != 0)
      {
	Utils::CopyString (lpszMessage, size, str.c_str());
      }
    }
  }
  return (lpszMessage);
}
