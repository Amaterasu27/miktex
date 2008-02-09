/* win.cpp:

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
#include "core-version.h"

#include "miktex/Core/Registry"
#include "win/winRegistry.h"

#include <miktex/Core/win/DllProc>

#define SET_SECURITY 1

/* _________________________________________________________________________

   Utils::GetFolderPath
   _________________________________________________________________________ */

PathName
Utils::GetFolderPath (/*[in]*/ int	nFolder,
		      /*[in]*/ int	nFallbackFolder,
		      /*[in]*/ bool	getCurrentPath)
{
  static DllProc5<HRESULT, HWND, int, HANDLE, DWORD, char *>
    pFunc ("shfolder.dll", "SHGetFolderPathA");
  PathName ret;
  DWORD flags =
    ( 0
      | (getCurrentPath
	 ? SHGFP_TYPE_CURRENT
	 : SHGFP_TYPE_DEFAULT)
      | 0);
  HRESULT hr =
    pFunc(0,
	  nFolder | CSIDL_FLAG_CREATE,
	  0,
	  flags,
	  ret.GetBuffer());
  if (hr == E_INVALIDARG && (nFolder != nFallbackFolder))
    {
      hr =
	pFunc(0,
	      nFallbackFolder | CSIDL_FLAG_CREATE,
	      0,
	      flags,
	      ret.GetBuffer());
    }
  if (hr != S_OK)
    {
      FATAL_MIKTEX_ERROR ("Utils::GetFolderPath",
			  T_("The file system path could not be retrieved."),
			  NUMTOSTR(nFolder));
    }
  return (ret);
}

/* _________________________________________________________________________

   MyGetFolderPath
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(PathName)
MyGetFolderPath (/*[in]*/ int	nFolder,
		 /*[in]*/ bool	getCurrentPath)
{
  return (Utils::GetFolderPath(nFolder, nFolder, getCurrentPath));
}

/* _________________________________________________________________________

   SessionImpl::GetMyProgramFile

   Get the fully qualified file name of the running executable (e.g.,
   C:\texmf\miktex\bin\tex.exe).
   _________________________________________________________________________ */

PathName
SessionImpl::GetMyProgramFile ()
{
  PathName path;
  if (GetModuleFileNameA(0, path.GetBuffer(), BufferSizes::MaxPath) == 0)
    {
      FATAL_WINDOWS_ERROR ("GetModuleFileNameA", 0);
    }
  return (path);
}

/* _________________________________________________________________________

   SessionImpl::DefaultConfig
   _________________________________________________________________________ */

StartupConfig
SessionImpl::DefaultConfig (/*[in]*/ bool sharedSetup)
{
  StartupConfig ret;
  string product = (IsMiKTeXDirect() ? "MiKTeXDirect" : "MiKTeX");
  if (sharedSetup
      || ! IsWindowsNT()
      || IsUserAnAdministrator()
      || IsUserAPowerUser())
    {
      ret.installRoot = MyGetFolderPath(CSIDL_PROGRAM_FILES, true);
      ret.installRoot += "MiKTeX" " " MIKTEX_SERIES_STR;
    }
  else
    {
      ret.installRoot =
	Utils::GetFolderPath(CSIDL_LOCAL_APPDATA, CSIDL_APPDATA, true);
      ret.installRoot += "MiKTeX" " " MIKTEX_SERIES_STR;
    }
  if (sharedSetup)
    {
      ret.commonDataRoot = MyGetFolderPath(CSIDL_COMMON_APPDATA, true);
      ret.commonDataRoot += product;
      ret.commonDataRoot += MIKTEX_SERIES_STR;
      ret.commonConfigRoot = ret.commonDataRoot;
    }
  ret.userDataRoot =
    Utils::GetFolderPath(CSIDL_LOCAL_APPDATA, CSIDL_APPDATA, true);
  ret.userDataRoot += product;
  ret.userDataRoot += MIKTEX_SERIES_STR;
  ret.userConfigRoot =
    Utils::GetFolderPath(CSIDL_APPDATA, CSIDL_APPDATA, true);
  ret.userConfigRoot += product;
  ret.userConfigRoot += MIKTEX_SERIES_STR;
  return (ret);
}

/* _________________________________________________________________________

   SessionImpl::ReadRegistry

   Read path info from the Windows registry.
   _________________________________________________________________________ */

StartupConfig
SessionImpl::ReadRegistry ()
{
  MIKTEX_ASSERT (! IsMiKTeXDirect());

  StartupConfig ret;

  string str;

  if (winRegistry::TryGetRegistryValue(IsSharedMiKTeXSetup(),
				       MIKTEX_REGKEY_CORE,
				       MIKTEX_REGVAL_ROOTS,
				       str,
				       0))
    {
      ret.roots = str;
    }

  if (winRegistry::TryGetRegistryValue(IsSharedMiKTeXSetup(),
				       MIKTEX_REGKEY_CORE,
				       MIKTEX_REGVAL_INSTALL,
				       str,
				       0))
    {
      ret.installRoot = str;
    }
  
  if (winRegistry::TryGetRegistryValue(IsSharedMiKTeXSetup(),
				       MIKTEX_REGKEY_CORE,
				       MIKTEX_REGVAL_COMMON_DATA,
				       str,
				       0))
    {
      ret.commonDataRoot = str;
    }

  if (winRegistry::TryGetRegistryValue(IsSharedMiKTeXSetup(),
				       MIKTEX_REGKEY_CORE,
				       MIKTEX_REGVAL_USER_DATA,
				       str,
				       0))
    {
      ret.userDataRoot = str;
    }

  if (winRegistry::TryGetRegistryValue(IsSharedMiKTeXSetup(),
				       MIKTEX_REGKEY_CORE,
				       MIKTEX_REGVAL_COMMON_CONFIG,
				       str,
				       0))
    {
      ret.commonConfigRoot = str;
    }

  if (winRegistry::TryGetRegistryValue(IsSharedMiKTeXSetup(),
				       MIKTEX_REGKEY_CORE,
				       MIKTEX_REGVAL_USER_CONFIG,
				       str,
				       0))
    {
      ret.userConfigRoot = str;
    }

  return (ret);
}

/* _________________________________________________________________________

   SessionImpl::WriteRegistry

   Write path info into the registry.
   _________________________________________________________________________ */

void
SessionImpl::WriteRegistry (/*[in]*/ const StartupConfig & startupConfig)
{
  MIKTEX_ASSERT (! IsMiKTeXDirect());

  StartupConfig defaultConfig = DefaultConfig(true);

  // clean registry values in HKLM and HKCU
  winRegistry::TryDeleteRegistryValue (TriState::Undetermined,
				       MIKTEX_REGKEY_CORE,
				       MIKTEX_REGVAL_ROOTS);
  winRegistry::TryDeleteRegistryValue (TriState::Undetermined,
				       MIKTEX_REGKEY_CORE,
				       MIKTEX_REGVAL_INSTALL);
  winRegistry::TryDeleteRegistryValue (TriState::Undetermined,
				       MIKTEX_REGKEY_CORE,
				       MIKTEX_REGVAL_COMMON_DATA);
  winRegistry::TryDeleteRegistryValue (TriState::Undetermined,
				       MIKTEX_REGKEY_CORE,
				       MIKTEX_REGVAL_USER_DATA);
  winRegistry::TryDeleteRegistryValue (TriState::Undetermined,
				       MIKTEX_REGKEY_CORE,
				       MIKTEX_REGVAL_COMMON_CONFIG);
  winRegistry::TryDeleteRegistryValue (TriState::Undetermined,
				       MIKTEX_REGKEY_CORE,
				       MIKTEX_REGVAL_USER_CONFIG);

  // try very hard to keep the registry clean

  if (! startupConfig.roots.empty()
      && startupConfig.roots != startupConfig.installRoot)
    {
      winRegistry::SetRegistryValue (TriState::Undetermined,
				     MIKTEX_REGKEY_CORE,
				     MIKTEX_REGVAL_ROOTS,
				     startupConfig.roots.c_str());
    }

  if (! startupConfig.installRoot.Empty()
      && startupConfig.installRoot != defaultConfig.installRoot)
    {
      winRegistry::SetRegistryValue (TriState::Undetermined,
				     MIKTEX_REGKEY_CORE,
				     MIKTEX_REGVAL_INSTALL,
				     startupConfig.installRoot.Get());
    }

  if (! startupConfig.commonDataRoot.Empty()
      && startupConfig.commonDataRoot != defaultConfig.commonDataRoot)
    {
      winRegistry::SetRegistryValue (TriState::Undetermined,
				     MIKTEX_REGKEY_CORE,
				     MIKTEX_REGVAL_COMMON_DATA,
				     startupConfig.commonDataRoot.Get());
    }

  if (! startupConfig.userDataRoot.Empty()
       && startupConfig.userDataRoot != defaultConfig.userDataRoot)
   {
      winRegistry::SetRegistryValue (TriState::Undetermined,
				     MIKTEX_REGKEY_CORE,
				     MIKTEX_REGVAL_USER_DATA,
				     startupConfig.userDataRoot.Get());
    }

  if (! startupConfig.commonConfigRoot.Empty()
      && startupConfig.commonConfigRoot != defaultConfig.commonConfigRoot)
    {
      winRegistry::SetRegistryValue (TriState::Undetermined,
				     MIKTEX_REGKEY_CORE,
				     MIKTEX_REGVAL_COMMON_CONFIG,
				     startupConfig.commonConfigRoot.Get());
    }

  if (! startupConfig.userConfigRoot.Empty()
      && startupConfig.userConfigRoot != defaultConfig.userConfigRoot)
    {
      winRegistry::SetRegistryValue (TriState::Undetermined,
				     MIKTEX_REGKEY_CORE,
				     MIKTEX_REGVAL_USER_CONFIG,
				     startupConfig.userConfigRoot.Get());
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

      AutoHKEY hkey;
      
      const wchar_t * ACRORD32 =
	(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion"
	 L"\\App Paths\\AcroRd32.exe");
  
      LONG res =
	RegOpenKeyExW(HKEY_LOCAL_MACHINE,
		      ACRORD32,
		      0,
		      KEY_READ,
		      &hkey);

      if (res != ERROR_SUCCESS)
	{
	  if (res != ERROR_FILE_NOT_FOUND)
	    {
	      TraceWindowsError ("RegOpenKeyEx",
				 res,
				 "Acrobat",
				 __FILE__,
				 __LINE__);
	    }
	  return (false);
	}

      DWORD type;
      PathName pathExe;
      DWORD len =
	static_cast<DWORD>(pathExe.GetCapacity() * sizeof(pathExe[0]));
      res =
	RegQueryValueExA
	(hkey.Get(),
	 "",
	 0,
	 &type,
	 reinterpret_cast<unsigned char *>(pathExe.GetBuffer()),
	 &len);
      if (res != ERROR_SUCCESS)
	{
	  if (res != ERROR_FILE_NOT_FOUND)
	    {
	      TraceWindowsError ("RegQueryValueEx",
				 res,
				 0,
				 __FILE__,
				 __LINE__);
	    }
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
      
      AutoHKEY hkey;
      
      LONG res =
	RegOpenKeyExW(HKEY_LOCAL_MACHINE,
		      L"SOFTWARE\\Adobe\\Adobe Type Manager\\Setup",
		      0,
		      KEY_READ,
		      &hkey);

      if (res != ERROR_SUCCESS)
	{
	  if (res != ERROR_FILE_NOT_FOUND)
	    {
	      TraceWindowsError ("RegOpenKeyEx",
				 res,
				 "ATM",
				 __FILE__,
				 __LINE__);
	    }
	  return (false);
	}

      DWORD type;
      PathName fontDir;
      DWORD len =
	static_cast<DWORD>(fontDir.GetCapacity() * sizeof(fontDir[0]));
      res =
	RegQueryValueExA
	(hkey.Get(),
	 "PFB_DIR",
	 0,
	 &type,
	 reinterpret_cast<unsigned char *>(fontDir.GetBuffer()),
	 &len);
      if (res != ERROR_SUCCESS)
	{
	  if (res != ERROR_FILE_NOT_FOUND)
	    {
	      TraceWindowsError ("RegQueryValueEx",
				 res,
				 0,
				 __FILE__,
				 __LINE__);
	    }
	  return (false);
	}
  
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
  char szWinDir[BufferSizes::MaxPath];

  if (GetWindowsDirectoryA(szWinDir, BufferSizes::MaxPath) == 0)
    {
      FATAL_WINDOWS_ERROR ("GetWindowsDirectoryA", 0);
    }

  char szWinDrive[BufferSizes::MaxPath];

  PathName::Split (szWinDir,
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
  if (GetWindowsDirectoryA(path.GetBuffer(),
			   static_cast<UINT>(path.GetCapacity()))
      == 0)
    {
      FATAL_WINDOWS_ERROR ("GetWindowsDirectoryA", 0);
    }
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
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		  0,
		  functionResult,
		  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		  reinterpret_cast<char *>(&pMessageBuffer),
		  0,
		  0);
  if (len == 0)
    {
      TraceError (T_("FormatMessage() failed for some reason"));
      return (false);
    }
  AutoLocalMemory autoFree (pMessageBuffer);
  errorMessage = reinterpret_cast<char *>(pMessageBuffer);
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
  WIN32_FIND_DATA finddata;
  HANDLE hnd = FindFirstFileA(lpszPath, &finddata);
  if (hnd == INVALID_HANDLE_VALUE)
    {
      FATAL_WINDOWS_ERROR ("FindFirstFileA", lpszPath);
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
#  if _MSC_VER >= 1400
  if (_tmakepath_s(lpszPath,
		   sizePath,
		   lpszDrive,
		   lpszAbsPath,
		   lpszRelPath,
		   lpszExtension)
      != 0)
    {
      FATAL_CRT_ERROR ("_tmakepath_s", 0);
    }
#  else
  char szPath[BufferSizes::MaxPath];
  _tmakepath (szPath, lpszDrive, lpszAbsPath, lpszRelPath, lpszExtension);
  Utils::CopyString (lpszPath, sizePath, szPath);
#  endif
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
#  if _MSC_VER >= 1400
  if (_tsplitpath_s(lpszPath,
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
#  else
  _tsplitpath (lpszPath, lpszDrive, lpszDir, lpszName, lpszExtension);
#  endif
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
  unsigned a[4] = { VER_FILEVERSION };
  pdvi->dwMajorVersion = a[0];
  pdvi->dwMinorVersion = a[1];
  pdvi->dwBuildNumber = a[2];
  pdvi->dwPlatformID = DLLVER_PLATFORM_WINDOWS;
  return (S_OK);
}

/* _________________________________________________________________________

   Utils::GetOSVersionString
   _________________________________________________________________________ */

typedef void (WINAPI * PGNSI) (/*[out]*/ LPSYSTEM_INFO);
typedef BOOL (WINAPI * PGPI) (/*[in]*/ DWORD,
			      /*[in]*/ DWORD,
			      /*[in]*/ DWORD,
			      /*[in]*/ DWORD,
			      /*[out]*/ DWORD *);

#if ! defined(PRODUCT_UNLICENSED)
#  define PRODUCT_UNLICENSED 0xABCDABCD
#endif

#if ! defined(SM_SERVERR2)
#  define SM_SERVERR2 89
#endif

#if ! defined(PRODUCT_BUSINESS)
#  define PRODUCT_BUSINESS 0x00000006
#endif

#if ! defined(PRODUCT_BUSINESS_N)
#  define PRODUCT_BUSINESS_N 0x00000010
#endif

#if ! defined(PRODUCT_CLUSTER_SERVER)
#  define PRODUCT_CLUSTER_SERVER 0x00000012
#endif

#if ! defined(PRODUCT_DATACENTER_SERVER)
#  define PRODUCT_DATACENTER_SERVER 0x00000008
#endif

#if ! defined(PRODUCT_DATACENTER_SERVER_CORE)
#  define PRODUCT_DATACENTER_SERVER_CORE 0x0000000C
#endif

#if ! defined(PRODUCT_ENTERPRISE)
#  define PRODUCT_ENTERPRISE 0x00000004
#endif

#if ! defined(PRODUCT_ENTERPRISE_SERVER)
#  define PRODUCT_ENTERPRISE_SERVER 0x0000000A
#endif

#if ! defined(PRODUCT_ENTERPRISE_SERVER_CORE)
#  define PRODUCT_ENTERPRISE_SERVER_CORE 0x0000000E
#endif

#if ! defined(PRODUCT_ENTERPRISE_SERVER_IA64)
#  define PRODUCT_ENTERPRISE_SERVER_IA64 0x0000000F
#endif

#if ! defined(PRODUCT_HOME_BASIC)
#  define PRODUCT_HOME_BASIC 0x00000002
#endif

#if ! defined(PRODUCT_HOME_BASIC_N)
#  define PRODUCT_HOME_BASIC_N 0x00000005
#endif

#if ! defined(PRODUCT_HOME_PREMIUM)
#  define PRODUCT_HOME_PREMIUM 0x00000003
#endif

#if ! defined(PRODUCT_HOME_SERVER)
#  define PRODUCT_HOME_SERVER 0x00000013
#endif

#if ! defined(PRODUCT_SERVER_FOR_SMALLBUSINESS)
#  define PRODUCT_SERVER_FOR_SMALLBUSINESS 0x00000018
#endif

#if ! defined(PRODUCT_SMALLBUSINESS_SERVER)
#  define PRODUCT_SMALLBUSINESS_SERVER 0x00000009
#endif

#if ! defined(PRODUCT_SMALLBUSINESS_SERVER_PREMIUM)
#  define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM 0x00000019
#endif

#if ! defined(PRODUCT_STANDARD_SERVER)
#  define PRODUCT_STANDARD_SERVER 0x00000007
#endif

#if ! defined(PRODUCT_STANDARD_SERVER_CORE )
#  define PRODUCT_STANDARD_SERVER_CORE  0x0000000D
#endif

#if ! defined(PRODUCT_STARTER)
#  define PRODUCT_STARTER 0x0000000B
#endif

#if ! defined(PRODUCT_STORAGE_ENTERPRISE_SERVER)
#  define PRODUCT_STORAGE_ENTERPRISE_SERVER 0x00000017
#endif

#if ! defined(PRODUCT_STORAGE_EXPRESS_SERVER)
#  define PRODUCT_STORAGE_EXPRESS_SERVER 0x00000014
#endif

#if ! defined(PRODUCT_STORAGE_STANDARD_SERVER)
#  define PRODUCT_STORAGE_STANDARD_SERVER 0x00000015
#endif

#if ! defined(PRODUCT_STORAGE_WORKGROUP_SERVER)
#  define PRODUCT_STORAGE_WORKGROUP_SERVER 0x00000016
#endif

#if ! defined(PRODUCT_UNDEFINED)
#  define PRODUCT_UNDEFINED 0x00000000
#endif

#if ! defined(PRODUCT_ULTIMATE)
#  define PRODUCT_ULTIMATE 0x00000001
#endif

#if ! defined(PRODUCT_WEB_SERVER)
#  define PRODUCT_WEB_SERVER 0x00000011
#endif

string
Utils::GetOSVersionString ()
{
  // get Windows version information
  OSVERSIONINFOEX osvi;
  ZeroMemory (&osvi, sizeof(OSVERSIONINFOEX));
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
  BOOL haveOsVersionInfoEx =
    GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&osvi));
  if (! haveOsVersionInfoEx)
    {
      osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
      if (! GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&osvi)))
	{
	  FATAL_WINDOWS_ERROR ("GetVersionEx", 0);
	}
    }
  
  SYSTEM_INFO si;
  ZeroMemory (&si, sizeof(si));
  bool haveSystemInfo = false;

  // build version string
  string str;
  switch (osvi.dwPlatformId)
     {
     case VER_PLATFORM_WIN32_NT: // Windows NT product family
       if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
	 {
	   if (osvi.wProductType == VER_NT_WORKSTATION)
	     {
	       str += "Microsoft Windows Vista ";
	     }
	   else
	     {
	       str += "Microsoft Windows Server \"Longhorn\" ";
	     }
	   PGPI pGPI =
	     reinterpret_cast<PGPI>
	     (GetProcAddress(GetModuleHandle("kernel32.dll"),
			     "GetProductInfo"));
	   DWORD dwProductType;
	   if (pGPI != 0 && pGPI(6, 0, 0, 0, &dwProductType))
	     {
	       switch (dwProductType)
		 {
		 case PRODUCT_UNLICENSED:
		   str += T_("(not activated, grace period expired) ");
		   break;
		 case PRODUCT_BUSINESS:
		   str += "Business Edition ";
		   break;
		 case PRODUCT_BUSINESS_N:
		   str += "Business N Edition ";
		   break;
		 case PRODUCT_CLUSTER_SERVER:
		   str += "Cluster Server Edition ";
		   break;
		 case PRODUCT_DATACENTER_SERVER:
		   str += "Server Datacenter Edition (full installation) ";
		   break;
		 case PRODUCT_DATACENTER_SERVER_CORE:
		   str += "Server Datacenter Edition (core installation) ";
		   break;
		 case PRODUCT_ENTERPRISE:
		   str += "Enterprise Edition ";
		   break;
		 case PRODUCT_ENTERPRISE_SERVER:
		   str += "Server Enterprise Edition (full installation) ";
		   break;
		 case PRODUCT_ENTERPRISE_SERVER_CORE:
		   str += "Server Enterprise Edition (core installation) ";
		   break;
		 case PRODUCT_ENTERPRISE_SERVER_IA64:
		   str +=
		     ("Server Enterprise Edition "
		      "for Itanium-based Systems ");
		   break;
		 case PRODUCT_HOME_BASIC:
		   str += "Home Basic Edition ";
		   break;
		 case PRODUCT_HOME_BASIC_N:
		   str += "Home Basic N Edition ";
		   break;
		 case PRODUCT_HOME_PREMIUM:
		   str += "Home Premium Edition ";
		   break;
		 case PRODUCT_HOME_SERVER:
		   str += "Home Server Edition ";
		   break;
		 case PRODUCT_SERVER_FOR_SMALLBUSINESS:
		   str += "Server for Small Business Edition ";
		   break;
		 case PRODUCT_SMALLBUSINESS_SERVER:
		   str += "Small Business Server ";
		   break;
		 case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
		   str += "Small Business Server Premium Edition ";
		   break;
		 case PRODUCT_STANDARD_SERVER:
		   str +=
		     "Server Standard Edition (full installation) ";
		   break;
		 case PRODUCT_STANDARD_SERVER_CORE :
		   str +=
		     "Server Standard Edition (core installation) ";
		   break;
		 case PRODUCT_STARTER:
		   str += "Starter Edition ";
		   break;
		 case PRODUCT_STORAGE_ENTERPRISE_SERVER:
		   str += "Storage Server Enterprise Edition ";
		   break;
		 case PRODUCT_STORAGE_EXPRESS_SERVER:
		   str += "Storage Server Express Edition ";
		   break;
		 case PRODUCT_STORAGE_STANDARD_SERVER:
		   str += "Storage Server Standard Edition ";
		   break;
		 case PRODUCT_STORAGE_WORKGROUP_SERVER:
		   str += "Storage Server Workgroup Edition ";
		   break;
		 case PRODUCT_UNDEFINED:
		   str += T_("(unknown product) ");
		   break;
		 case PRODUCT_ULTIMATE:
		   str += "Ultimate Edition ";
		   break;
		 case PRODUCT_WEB_SERVER:
		   str += "Web Server Edition ";
		   break;
		 }
	     }
	 }
       else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
	 {
	   // use GetProcAddress to avoid load issues on Windows 2000
	   PGNSI pGNSI =
	     reinterpret_cast<PGNSI>
	     (GetProcAddress(GetModuleHandle("kernel32.dll"),
			     "GetNativeSystemInfo"));
	   if (pGNSI != 0)
	     {
	       pGNSI (&si);
	       haveSystemInfo = true;
	     }
	   if (GetSystemMetrics(SM_SERVERR2))
	     {
	       str += "Microsoft Windows Server 2003 \"R2\" ";
	     }
	   else if (osvi.wProductType == VER_NT_WORKSTATION
		    && haveSystemInfo
		    && (si.wProcessorArchitecture
			== PROCESSOR_ARCHITECTURE_AMD64))
	     {
	       str += "Microsoft Windows XP Professional x64 Edition ";
	     }
	   else
	     {
	       str += "Microsoft Windows Server 2003 ";
	     }
	 }
       else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
	 {
	   str += "Microsoft Windows XP ";
	 }
       else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
	 {
	   str += "Microsoft Windows 2000 ";
	 }
       else if (osvi.dwMajorVersion <= 4)
	 {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
	   str += "Microsoft Windows NT ";
#else
	   UNIMPLEMENTED ("Utils::GetOSVersionString");
#endif
	 }
       if (haveOsVersionInfoEx)
	 {
	   // test for the workstation type
	   if (osvi.dwMajorVersion < 6
	       && osvi.wProductType == VER_NT_WORKSTATION
	       && ! (haveSystemInfo
		     && (si.wProcessorArchitecture
			 == PROCESSOR_ARCHITECTURE_AMD64)))
	     {
	       if (osvi.dwMajorVersion == 4)
		 {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
		   str += "Workstation 4.0 ";
#else
		   UNIMPLEMENTED ("Utils::GetOSVersionString");
#endif
		 }
	       else if ((osvi.wSuiteMask & VER_SUITE_PERSONAL) != 0)
		 {
		   str += "Home Edition ";
		 }
	       else
		 {
		   str += T_("Professional " );
		 }
	     }

	   // test for the server type
	   else if (osvi.wProductType == VER_NT_SERVER
		    || osvi.wProductType == VER_NT_DOMAIN_CONTROLLER)
	     {
	       if (osvi.dwMajorVersion == 6)
		 {
		   // see above
		 }
	       else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
		 {
		   if (haveSystemInfo
		       && (si.wProcessorArchitecture
			   == PROCESSOR_ARCHITECTURE_IA64))
		     {
		       if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) != 0)
			 {
			   str +=
			     "Datacenter Edition"
			     " for Itanium-based Systems ";
			 }
		       else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
			 {
			   str +=
			     "Enterprise Edition"
			     " for Itanium-based Systems ";
			 }
		     }
		   else if (haveSystemInfo
			    && (si.wProcessorArchitecture
				== PROCESSOR_ARCHITECTURE_AMD64))
		     {
		       if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) != 0)
			 {
			   str += "Datacenter x64 Edition ";
			 }
		       else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
			 {
			   str += "Enterprise x64 Edition ";
			 }
		       else
			 {
			   str += "Standard x64 Edition ";
			 }
		     }
		   else
		     {
		       if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) != 0)
			 {
			   str += "Datacenter Edition ";
			 }
		       else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
			 {
			   str+= "Enterprise Edition ";
			 }
		       else if ((osvi.wSuiteMask == VER_SUITE_BLADE) != 0)
			 {
			   str += "Web Edition ";
			 }
		       else
			 {
			   str += "Standard Edition ";
			 }
		     }
		 }
	       else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
		 {
		   if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) != 0)
		     {
		       str += "Datacenter Server ";
		     }
		   else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
		     {
		       str += "Advanced Server ";
		     }
		   else
		     {
		       str += "Server ";
		     }
		 }
	       else		// Windows NT 4.0
		 {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
		   if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
		     {
		       str += T_("Server 4.0, Enterprise Edition " );
		     }
		   else
		     {
		       str += "Server 4.0 ";
		     }
#else
		   UNIMPLEMENTED ("Utils::GetOSVersionString");
#endif
		 }
	     }
	 }
       else  // test for specific product on Windows NT 4.0 SP5 and earlier
	 {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
	   AutoHKEY hKey;
	   LONG lRet;
	   lRet =
	     RegOpenKeyExA(HKEY_LOCAL_MACHINE,
			   ("SYSTEM\\CurrentControlSet\\Control"
			    "\\ProductOptions"),
			   0,
			   KEY_QUERY_VALUE,
			   &hKey);
	   if (lRet != ERROR_SUCCESS)
	     {
	       FATAL_WINDOWS_ERROR ("RegOpenKeyExA", 0);
	     }
	   char szProductType[80];
	   unsigned long dwBufLen = ARRAY_SIZE(szProductType);
	   lRet =
	     RegQueryValueExA(hKey.Get(),
			      "ProductType",
			      0,
			      0,
			      reinterpret_cast<LPBYTE>(szProductType),
			      &dwBufLen);
	   if (lRet != ERROR_SUCCESS)
	     {
	       FATAL_WINDOWS_ERROR ("RegQueryValueExA", 0);
	     }
	   if (dwBufLen > ARRAY_SIZE(szProductType))
	     {
	       BUF_TOO_SMALL ("GetWindowsVersion");
	     }
	   else if (_tcsicmp("WINNT", szProductType) == 0)
	     {
	       str += "Workstation ";
	     }
	   else if (_tcsicmp("LANMANNT", szProductType) == 0)
	     {
	       str += "Server ";
	     }
	   else if (_tcsicmp ("SERVERNT", szProductType) == 0)
	     {
	       str += "Advanced Server ";
	     }
	   str += NUMTOSTR(osvi.dwMajorVersion);
	   str += '.';
	   str += NUMTOSTR(osvi.dwMinorVersion);
	   str += ' ';
#else
	   UNIMPLEMENTED ("Utils::GetOSVersionString");
#endif
	 }
     
       // get service pack (if any) and build number
       if (osvi.dwMajorVersion == 4
	   && _tcsicmp(osvi.szCSDVersion, "Service Pack 6") == 0)
	 {
	   HKEY hKey;
	   LONG lRet;
	 
	   // test for SP6 versus SP6a
	   lRet =
	     RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			  ("SOFTWARE\\Microsoft\\Windows NT"
			   "\\CurrentVersion\\Hotfix\\Q246009"),
			  0,
			  KEY_QUERY_VALUE,
			  &hKey);
	   if (lRet == ERROR_SUCCESS)
	     {
	       RegCloseKey (hKey);
	       str += "Service Pack 6a (Build ";
	       str += NUMTOSTR(osvi.dwBuildNumber & 0xFFFF);
	       str += ')';
	     }
	   else
	     {
	       str += osvi.szCSDVersion;
	       str += " (Build ";
	       str += NUMTOSTR(osvi.dwBuildNumber & 0xFFFF);
	       str += ')';
	     }
	 }
       else // Windows NT 3.51 and earlier or Windows 2000 and later
	 {
	   if (osvi.szCSDVersion[0] != 0)
	     {
	       str += osvi.szCSDVersion;
	       str += ' ';
	     }
	   str += "(Build ";
	   str += NUMTOSTR(osvi.dwBuildNumber & 0xFFFF);
	   str += ')';
	 }
       break;
     
     case VER_PLATFORM_WIN32_WINDOWS: // Windows 95 product family
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
       if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
         {
	   str += "Microsoft Windows 95";
	   if (osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B')
	     {
	       str += " OSR2";
	     }
         }
       else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
         {
	   str += "Microsoft Windows 98";
	   if ( osvi.szCSDVersion[1] == 'A')
	     {
	       str += " SE";
	     }
         } 
       else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
         {
	   str += "Microsoft Windows Millennium Edition";
         } 
       break;
#else
       UNIMPLEMENTED ("Utils::GetOSVersionString");
#endif
     }

  return (str);
}

/* _________________________________________________________________________

   Utils::GetDefPrinter

   See Q246772.
   _________________________________________________________________________ */

bool
Utils::GetDefPrinter (/*[out]*/ char *		pPrinterName,
		      /*[in,out]*/ size_t *	pBufferSize)
{
  OSVERSIONINFO osv;
  osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx (&osv);
  if (osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
    {
      unsigned long dwNeeded, dwReturned;
      EnumPrinters (PRINTER_ENUM_DEFAULT,
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
      PRINTER_INFO_2 * ppi2 =
	reinterpret_cast<PRINTER_INFO_2*>(hMem.Get());
      if (! EnumPrinters(PRINTER_ENUM_DEFAULT,
			 0,
			 2,
			 reinterpret_cast<LPBYTE>(ppi2),
			 dwNeeded,
			 &dwNeeded,
			 &dwReturned))
	{
	  return (false);
	}
      unsigned long l = static_cast<unsigned long>(StrLen(ppi2->pPrinterName));
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
	  DllProc2<BOOL, char *, LPDWORD>
	    getDefaultPrinterA ("winspool.drv", "GetDefaultPrinterA");
	  DWORD dwBufferSize = static_cast<DWORD>(*pBufferSize);
	  BOOL bDone = getDefaultPrinterA(pPrinterName, &dwBufferSize);
	  if (! bDone)
	    {
	      if (::GetLastError() == ERROR_FILE_NOT_FOUND)
		{
		  return (false);
		}
	      else
		{
		  FATAL_WINDOWS_ERROR ("GetDefaultPrinterA", 0);
		}
	    }
	  else
	    {
	      *pBufferSize = dwBufferSize;
	      return (true);
	    }
	}
      else
	{
	  char cBuffer[4096];
	  if (GetProfileString("windows",
			       "device",
			       ",,,",
			       cBuffer,
			       4096)
	      <= 0)
	    {
	      return (false);
	    }
	  Tokenizer tok (cBuffer, ",");
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
  HWND hwnd = HtmlHelp(hWnd, pathHelpFile.Get(), HH_HELP_CONTEXT, topic);
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
  unsigned long n =
    GetTempPathA(static_cast<DWORD>(GetCapacity()), GetBuffer());
  if (n == 0)
    {
      FATAL_WINDOWS_ERROR ("GetTempPathA", 0);
    }
  if (n >= GetCapacity())
    {
      UNEXPECTED_CONDITION ("PathName::SetToTempDirectory");
    }
  return (*this);
}

/* _________________________________________________________________________

   PathName::SetToTempFile
   _________________________________________________________________________ */

PathName &
PathName::SetToTempFile ()
{
  PathName pathTempDir = SessionImpl::GetSession()->GetTempDirectory();

  UINT n = GetTempFileNameA(pathTempDir.Get(), "mik", 0, GetBuffer());

  if (n == 0)
    {
      FATAL_WINDOWS_ERROR ("GetTempFileNameA", pathTempDir.Get());
    }

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
Windows function %s failed for the following reason:\n	\
%s\n\
Result: %u\n\
Info: %s\n\
Source: %s\n\
Line: %d"),
     lpszWindowsFunction,
     errorMessage.c_str(),
     static_cast<unsigned>(functionResult),
     (lpszInfo == 0 ? "" : lpszInfo),
     lpszSourceFile,
     sourceLine);
}

/* _________________________________________________________________________

   AddEventSource
   _________________________________________________________________________ */

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
    CreateFile(lpszFileName,
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
  trace_files->WriteFormattedLine
    ("core",
     T_("scheduling removal of %s"),
     Q_(lpszFileName));
  if (IsWindowsNT())
    {
      if (! MoveFileExA(lpszFileName, 0, MOVEFILE_DELAY_UNTIL_REBOOT))
	{
	  FATAL_WINDOWS_ERROR ("MoveFileExA", lpszFileName);
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
  return (GetDriveType(pathRootName.Get()));
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

  EXPLICIT_ACCESS ea[1];
  ZeroMemory (&ea, 1 * sizeof(EXPLICIT_ACCESS));
  ea[0].grfAccessPermissions = FILE_ALL_ACCESS;
  ea[0].grfAccessMode = GRANT_ACCESS;
  ea[0].grfInheritance= SUB_CONTAINERS_AND_OBJECTS_INHERIT;
  ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
  ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
  ea[0].Trustee.ptstrName =
    reinterpret_cast<char *>(pEveryoneSID.Get());
  
  if (SetEntriesInAcl(1, ea, 0, &pACL) != ERROR_SUCCESS)
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

  if (! CreateDirectoryA(lpszPath, &sa))
    {
      FATAL_WINDOWS_ERROR ("CreateDirectoryA", lpszPath);
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
      && pSession->IsSharedMiKTeXSetup() == TriState::True
      && (pSession->GetSpecialPath(SpecialPath::CommonConfigRoot) == lpszPath
	  || pSession->GetSpecialPath(SpecialPath::CommonDataRoot) == lpszPath)
      && pSession->RunningAsAdministrator())
    {
      CreateDirectoryForEveryone (lpszPath);
    }
  else if (! CreateDirectoryA(lpszPath, 0))
    {
      FATAL_WINDOWS_ERROR ("CreateDirectoryA", lpszPath);
    }
#else
  if (! CreateDirectoryA(lpszPath, 0))
    {
      FATAL_WINDOWS_ERROR ("CreateDirectoryA", lpszPath);
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
  if (_tputenv_s(lpszValueName, lpszValue) != 0)
    {
      FATAL_CRT_ERROR ("_tputenv_s", lpszValueName);
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

BOOL ShellExecuteURLExInternal(LPSHELLEXECUTEINFO lpExecInfo)
{
    BOOL bRet;
    DWORD dwErr;
    HRESULT hr;
    PARSEDURL pu;
    TCHAR szSchemeBuffer[INTERNET_MAX_SCHEME_LENGTH + 1];
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
        hr = ParseURL(lpExecInfo->lpFile, &pu);
 
        /* Is the URL really, unambiguously an URL? */
        if
        (
            SUCCEEDED(hr) &&
            pu.pszProtocol == lpExecInfo->lpFile &&
            pu.pszProtocol[pu.cchProtocol] == TEXT(':')
        )
        {
            /* We need the scheme name NUL-terminated, so we copy it */
            hr = StringCbCopyN
            (
                szSchemeBuffer,
                sizeof(szSchemeBuffer),
                pu.pszProtocol,
                pu.cchProtocol * sizeof(TCHAR)
            );
 
            if(SUCCEEDED(hr))
            {
                /* Is the URL scheme a registered ProgId? */
                hr = AssocQueryKey
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
                    dwErr = RegQueryValueEx
                    (
                        hkeyClass,
                        TEXT("URL Protocol"),
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
                        bRet = ShellExecuteEx(lpExecInfo);
 
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
 
BOOL ShellExecuteURLEx(LPSHELLEXECUTEINFO lpExecInfo)
{
    BOOL bRet;
    SHELLEXECUTEINFO ExecInfo;
 
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
    LPCTSTR lpOperation,
    LPCTSTR lpFile,
    LPCTSTR lpParameters,
    LPCTSTR lpDirectory,
    INT nShowCmd
)
{
    SHELLEXECUTEINFO ExecuteInfo;
 
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

   Utils::RegisterMiKTeXUser
   _________________________________________________________________________ */

void
Utils::RegisterMiKTeXUser ()
{
  HINSTANCE hInst =
    ShellExecuteURL(0,
		    0,
		    MIKTEX_URL_WWW_GIVE_BACK,
		    0,
		    0,
		    SW_SHOWNORMAL);
  if (reinterpret_cast<int>(hInst) <= 32)
    {
      FATAL_MIKTEX_ERROR ("Utils::RegisterMiKTeXUser",
			  T_("The web browser could not be started."),
			  NUMTOSTR(reinterpret_cast<int>(hInst)));
    }
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
      argv.push_back (StrDup("foo"));
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
	      argv.push_back (StrDup(arg.c_str()));
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

   Utils::UTF8ToWideChar
   _________________________________________________________________________ */

wstring
Utils::UTF8ToWideChar (/*[in]*/ const char * lpszUtf8)
{
  MIKTEX_ASSERT (IsUTF8(lpszUtf8));
  if (*lpszUtf8 == 0)
    {
      return (L"");
    }
  int len =
    MultiByteToWideChar(CP_UTF8,
			MB_ERR_INVALID_CHARS,
			lpszUtf8,
			-1,
			0,
			0);
  if (len <= 0)
    {
      FATAL_WINDOWS_ERROR ("MultiByteToWideChar", 0);
    }
  CharBuffer<wchar_t, 200> buf (len + 1);
  len =
    MultiByteToWideChar(CP_UTF8,
			MB_ERR_INVALID_CHARS,
			lpszUtf8,
			-1,
			buf.GetBuffer(),
			buf.GetCapacity());
  if (len <= 0)
    {
      FATAL_WINDOWS_ERROR ("MultiByteToWideChar", 0);
    }
  return (buf.Get());
}

/* _________________________________________________________________________

   Utils::WideCharToAnsi
   _________________________________________________________________________ */

string
Utils::WideCharToAnsi (/*[in]*/ const wchar_t * lpszWideChar)
{
  if (*lpszWideChar == 0)
    {
      return ("");
    }
  CharBuffer<char, 512> buf (wcslen(lpszWideChar) + 1);
  int n = WideCharToMultiByte
    (CP_ACP,
     WC_NO_BEST_FIT_CHARS,
     lpszWideChar,
     -1,
     buf.GetBuffer(),
     buf.GetCapacity(),
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
  return (buf.Get());
}

/* _________________________________________________________________________

   Utils::AnsiToWideChar
   _________________________________________________________________________ */

wstring
Utils::AnsiToWideChar (/*[in]*/ const char * lpszAnsi)
{
  if (*lpszAnsi == 0)
    {
      return (L"");
    }
  CharBuffer<wchar_t, 512> buf (strlen(lpszAnsi) + 1);
  int n = MultiByteToWideChar
    (CP_ACP,
     0,
     lpszAnsi,
     -1,
     buf.GetBuffer(),
     buf.GetCapacity());
  if (n == 0)
    {
      FATAL_WINDOWS_ERROR ("MultiByteToWideChar", 0);
    }
  if (n < 0)
    {
      UNEXPECTED_CONDITION ("Utils::AnsiToWideChar");
    }
  return (buf.Get());
}
