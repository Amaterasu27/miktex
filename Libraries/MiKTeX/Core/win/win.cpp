/* win.cpp:

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

#include "miktex/version.h"

#include "internal.h"
#include "core-version.h"

#include "miktex/reg.h"
#include "win/winRegistry.h"

#define SET_SECURITY 1
#define REPORT_EVENTS 0

/* _________________________________________________________________________

   Utils::GetFolderPath
   _________________________________________________________________________ */

PathName
MIKTEXCALL
Utils::GetFolderPath (/*[in]*/ int	nFolder,
		      /*[in]*/ int	nFallbackFolder,
		      /*[in]*/ bool	getCurrentPath)
{
  typedef HRESULT (__stdcall * PFUNC) (HWND hwndOwner,
				       int nFolder,
				       HANDLE hToken,
				       DWORD dwFlags,
				       LPTSTR lpszPath);
  MIKTEXPERMANENTVAR(PFUNC) pFunc = 0;
  if (pFunc == 0)
    {
      HMODULE hModSHFolder = LoadLibrary(T_("shfolder.dll"));
      if (hModSHFolder == 0)
	{
	  FATAL_MIKTEX_ERROR (T_("Utils::GetFolderPath"),
			      T_("SHFolder.dll is not installed."),
			      0);
	}
#if defined(MIKTEX_UNICODE)
      pFunc =
	reinterpret_cast<PFUNC>(GetProcAddress(hModSHFolder,
					       "SHGetFolderPathW"));
#else
      pFunc =
	reinterpret_cast<PFUNC>(GetProcAddress(hModSHFolder,
					       "SHGetFolderPathA"));
#endif
      if (pFunc == 0)
	{
	  FATAL_MIKTEX_ERROR (T_("Utils::GetFolderPath"),
			      T_("SHFolder.dll is outdated."),
			      0);
	}
    }
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
      FATAL_MIKTEX_ERROR (T_("Utils::GetFolderPath"),
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
  if (GetModuleFileName(0, path.GetBuffer(), BufferSizes::MaxPath) == 0)
    {
      FATAL_WINDOWS_ERROR (T_("GetModuleFileName"), 0);
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
  ret.installRoot = MyGetFolderPath(CSIDL_PROGRAM_FILES, true);
  ret.installRoot += T_("MiKTeX") T_(" ") T_(MIKTEX_SERIES_STR);
  if (sharedSetup)
    {
      ret.commonDataRoot = MyGetFolderPath(CSIDL_COMMON_APPDATA, true);
      ret.commonDataRoot += T_("MiKTeX");
      ret.commonDataRoot += T_(MIKTEX_SERIES_STR);
      ret.commonConfigRoot = ret.commonDataRoot;
    }
  ret.userDataRoot =
    Utils::GetFolderPath(CSIDL_LOCAL_APPDATA, CSIDL_APPDATA, true);
  ret.userDataRoot += T_("MiKTeX");
  ret.userDataRoot += T_(MIKTEX_SERIES_STR);
  ret.userConfigRoot =
    Utils::GetFolderPath(CSIDL_APPDATA, CSIDL_APPDATA, true);
  ret.userConfigRoot += T_("MiKTeX");
  ret.userConfigRoot += T_(MIKTEX_SERIES_STR);
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

  tstring str;

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
SessionImpl::GetAcrobatFontDir (/*[out]*/ MIKTEXCHAR *	lpszPath)
{
  if (flags.test(Flags::CachedAcrobatFontDir))
    {
      if (acrobatFontDir.length() == 0)
	{
	  return (false);
	}
      Utils::CopyString (lpszPath,
			 BufferSizes::MaxPath,
			 acrobatFontDir.c_str());
      return (true);
    }

  flags.set (Flags::CachedAcrobatFontDir);
  acrobatFontDir = T_("");

  AutoHKEY hkey;

  const MIKTEXCHAR * ACRORD32 =
    (T_("SOFTWARE\\Microsoft\\Windows\\CurrentVersion")
     T_("\\App Paths\\AcroRd32.exe"));
  
  LONG res =
    RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		 ACRORD32,
		 0,
		 KEY_READ,
		 &hkey);

  if (res != ERROR_SUCCESS)
    {
      if (res != ERROR_FILE_NOT_FOUND)
	{
	  TraceWindowsError (T_("RegOpenKeyEx"),
			     res,
			     ACRORD32,
			     T_(__FILE__),
			     __LINE__);
	}
      return (false);
    }

  DWORD type;
  PathName pathExe;
  DWORD len = static_cast<DWORD>(pathExe.GetSize());
  res =
    RegQueryValueEx(hkey.Get(),
		    T_(""),
		    0,
		    &type,
		    reinterpret_cast<unsigned char *>(pathExe.GetBuffer()),
		    &len);
  if (res != ERROR_SUCCESS)
    {
      if (res != ERROR_FILE_NOT_FOUND)
	{
	  TraceWindowsError (T_("RegQueryValueEx"),
			     res,
			     0,
			     T_(__FILE__),
			     __LINE__);
	}
      return (false);
    }
  
  PathName dir (pathExe);
  dir.RemoveFileSpec ();
  
  PathName fontDir;
  
  // try Acrobat Reader 3.0
  fontDir.Set (dir.Get(), T_("FONTS"), 0);
  if (! Directory::Exists(fontDir))
    {
      // try Acrobat Reader 4.0
      fontDir.Set (dir.Get(), T_("..\\Resource\\Font"), 0);
      if (! Directory::Exists(fontDir))
	{
	  return (false);
	}
    }

  acrobatFontDir = GetFullPath(fontDir.Get()).ToString();

  Utils::CopyString (lpszPath, BufferSizes::MaxPath, acrobatFontDir.c_str());

  return (true);
}

/* _________________________________________________________________________
   
   SessionImpl::GetATMFontDir
   _________________________________________________________________________ */

bool
SessionImpl::GetATMFontDir (/*[out]*/ MIKTEXCHAR *	lpszPath)
{
  if (flags.test(Flags::CachedAtmFontDir))
    {
      if (atmFontDir.length() == 0)
	{
	  return (false);
	}
      Utils::CopyString (lpszPath, BufferSizes::MaxPath, atmFontDir.c_str());
      return (true);
    }

  flags.set (Flags::CachedAtmFontDir);
  atmFontDir = T_("");

  AutoHKEY hkey;

  const MIKTEXCHAR * ATM = T_("SOFTWARE\\Adobe\\Adobe Type Manager\\Setup");

  LONG res =
    RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		 ATM,
		 0,
		 KEY_READ,
		 &hkey);

  if (res != ERROR_SUCCESS)
    {
      if (res != ERROR_FILE_NOT_FOUND)
	{
	  TraceWindowsError (T_("RegOpenKeyEx"),
			     res,
			     ATM,
			     T_(__FILE__),
			     __LINE__);
	}
      return (false);
    }

  DWORD type;
  PathName fontDir;
  DWORD len = static_cast<DWORD>(fontDir.GetSize());
  res = RegQueryValueEx(hkey.Get(),
			T_("PFB_DIR"),
			0,
			&type,
			reinterpret_cast<unsigned char *>(fontDir.GetBuffer()),
			&len);
  if (res != ERROR_SUCCESS)
    {
      if (res != ERROR_FILE_NOT_FOUND)
	{
	  TraceWindowsError (T_("RegQueryValueEx"),
			     res,
			     0,
			     T_(__FILE__),
			     __LINE__);
	}
      return (false);
    }
  
  if (! Directory::Exists(fontDir))
    {
      return (false);
    }

  RemoveDirectoryDelimiter (fontDir.GetBuffer());

  atmFontDir = GetFullPath(fontDir.Get()).ToString();

  Utils::CopyString (lpszPath, BufferSizes::MaxPath, atmFontDir.c_str());

  return (true);
}

/* _________________________________________________________________________

   GetPsFontsDirectory
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
GetPsFontDirectory (/*[out]*/ MIKTEXCHAR *	lpszDir)
{
  MIKTEXCHAR szWinDir[BufferSizes::MaxPath];

  if (GetWindowsDirectory(szWinDir, BufferSizes::MaxPath) == 0)
    {
      FATAL_WINDOWS_ERROR (T_("GetWindowsDirectory"), 0);
    }

  MIKTEXCHAR szWinDrive[BufferSizes::MaxPath];

  PathName::Split (szWinDir,
		   szWinDrive, BufferSizes::MaxPath,
		   0, 0,
		   0, 0,
		   0, 0);

  PathName::Combine (lpszDir,
		     BufferSizes::MaxPath,
		     szWinDrive,
		     T_("\\psfonts"),
		     0,
		     0);

  return (Directory::Exists(lpszDir));
}

/* _________________________________________________________________________

   SessionImpl::GetPsFontDirs
   _________________________________________________________________________ */

bool
SessionImpl::GetPsFontDirs (/*[out]*/ tstring &	psFontDirs)
{
  if (! flags.test(Flags::CachedPsFontDirs))
    {
      flags.set (Flags::CachedPsFontDirs);
      PathName path;
      if (GetATMFontDir(path.GetBuffer())
	  || GetPsFontDirectory(path.GetBuffer()))
	{
	  if (this->psFontDirs.length() > 0)
	    {
	      this->psFontDirs += PATH_DELIMITER;
	    }
	  this->psFontDirs += path.Get();
	}
      if (GetAcrobatFontDir(path.GetBuffer()))
	{
	  if (this->psFontDirs.length() > 0)
	    {
	      this->psFontDirs += PATH_DELIMITER;
	    }
	  this->psFontDirs += path.Get();
	}
    }

  if (this->psFontDirs.length() == 0)
    {
      return (false);
    }

  psFontDirs = this->psFontDirs;

  return (true);
}

/* _________________________________________________________________________

   GetWindowsFontsDirectory
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
GetWindowsFontsDirectory (/*[out]*/ PathName & path)
{
  if (GetWindowsDirectory(path.GetBuffer(),
			  static_cast<UINT>(path.GetSize()))
      == 0)
    {
      FATAL_WINDOWS_ERROR (T_("GetWindowsDirectory"), 0);
    }
  path += T_("Fonts");
  return (Directory::Exists(path));
}

/* _________________________________________________________________________

   SessionImpl::GetTTFDirs
   _________________________________________________________________________ */

bool
SessionImpl::GetTTFDirs (/*[out]*/ tstring &	ttfDirs)
{
  if (! flags.test(Flags::CachedTtfDirs))
    {
      flags.set (Flags::CachedTtfDirs);
      PathName path;
      if (GetWindowsFontsDirectory(path))
	{
	  if (this->ttfDirs.length() > 0)
	    {
	      this->ttfDirs += PATH_DELIMITER;
	    }
	  this->ttfDirs = path.ToString();
	}
    }

  if (this->ttfDirs.length() == 0)
    {
      return (false);
    }

  ttfDirs = this->ttfDirs;

  return (true);
}

/* _________________________________________________________________________

   GetWindowsErrorMessage
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
GetWindowsErrorMessage (/*[in]*/ unsigned long	functionResult,
			/*[out]*/ tstring &	errorMessage)
{
  void * pMessageBuffer;
  unsigned long len =
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		  0,
		  functionResult,
		  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		  reinterpret_cast<LPTSTR>(&pMessageBuffer),
		  0,
		  0);
  if (len == 0)
    {
      TraceError (T_("FormatMessage() failed for some reason"));
      return (false);
    }
  AutoLocalMemory autoFree (pMessageBuffer);
  errorMessage = reinterpret_cast<MIKTEXCHAR *>(pMessageBuffer);
  return (true);
}

/* _________________________________________________________________________

   Session::FatalWindowsError
   _________________________________________________________________________ */

void
MIKTEXNORETURN
MIKTEXCALL
Session::FatalWindowsError (/*[in]*/ const MIKTEXCHAR *	lpszWindowsFunction,
			    /*[in]*/ unsigned long	errorCode,
			    /*[in]*/ const MIKTEXCHAR *	lpszInfo,
			    /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
			    /*[in]*/ int		sourceLine)
{
  tstring programInvocationName;
  if (SessionImpl::theSession != 0)
    {
      TraceStream::TraceLastWin32Error (lpszWindowsFunction,
					lpszInfo,
					lpszSourceFile,
					sourceLine);
      programInvocationName =
	SessionImpl::theSession->initInfo.GetProgramInvocationName();
    }
  tstring errorMessage = T_("Windows API error ");
  errorMessage += NUMTOSTR(errorCode);
  tstring windowsErrorMessage;
  if (GetWindowsErrorMessage(errorCode, windowsErrorMessage))
    {
      errorMessage += T_(": ");
      errorMessage += windowsErrorMessage;
    }
  else
    {
      errorMessage += T_('.');
    }
#if 1
  tstring env;
  if (Utils::GetEnvironmentString(T_("MIKTEX_DEBUG_BREAK"), env)
      && env == T_("1"))
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
MIKTEXNORETURN
MIKTEXCALL
Session::FatalWindowsError (/*[in]*/ const MIKTEXCHAR *	lpszWindowsFunction,
			    /*[in]*/ const MIKTEXCHAR *	lpszInfo,
			    /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
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
(/*[in]*/ const MIKTEXCHAR *	lpszProgramInvocationName,
 /*[in]*/ const MIKTEXCHAR *	lpszMessage,
 /*[in]*/ const MIKTEXCHAR *	lpszInfo,
 /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
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
GetAlternate (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	      /*[out]*/ MIKTEXCHAR *		lpszAlternate)
{
  WIN32_FIND_DATA finddata;
  HANDLE hnd = FindFirstFile(lpszPath, &finddata);
  if (hnd == INVALID_HANDLE_VALUE)
    {
      FATAL_WINDOWS_ERROR (T_("FindFirstFile"), lpszPath);
    }
  if (! FindClose(hnd))
    {
      FATAL_WINDOWS_ERROR (T_("FindClose"), 0);
    }
  if (finddata.cAlternateFileName[0] == 0)
    {
      FATAL_MIKTEX_ERROR (T_("GetAlternate"),
			  T_("no alternate file name"),
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
  MIKTEXCHAR * lpszEnd = temp.GetBuffer() + temp.GetLength();

  // back iterator
  MIKTEXCHAR * lpsz = lpszEnd;

  // points to the last '/' found (if any)
  MIKTEXCHAR * lpszSlash = 0;

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
      if (*lpsz == T_(' '))
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
  MIKTEXCHAR szAlternate[BufferSizes::MaxPath];

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
  MIKTEX_ASSERT (StrChr(szAlternate, T_(' ')) == 0);

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
      MIKTEX_ASSERT (StrChr(temp.Get(), T_(' ')) == 0);
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
PathName::Combine (/*[out]*/ MIKTEXCHAR *	lpszPath,
		   /*[in]*/ size_t		sizePath,
		   /*[in]*/ const MIKTEXCHAR *	lpszDrive,
		   /*[in]*/ const MIKTEXCHAR *	lpszAbsPath,
		   /*[in]*/ const MIKTEXCHAR *	lpszRelPath,
		   /*[in]*/ const MIKTEXCHAR *	lpszExtension)
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
      FATAL_CRT_ERROR (T_("_tmakepath_s"), 0);
    }
#  else
  MIKTEXCHAR szPath[BufferSizes::MaxPath];
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
PathName::Split (/*[in]*/ const MIKTEXCHAR *	lpszPath,
		 /*[out]*/ MIKTEXCHAR *		lpszDrive,
		 /*[in]*/ size_t		sizeDrive,
		 /*[out]*/ MIKTEXCHAR *		lpszDir,
		 /*[in]*/ size_t		sizeDir,
		 /*[out]*/ MIKTEXCHAR *		lpszName,
		 /*[in]*/ size_t		sizeName,
		 /*[out]*/ MIKTEXCHAR *		lpszExtension,
		 /*[in]*/ size_t		sizeExtension)
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
      FATAL_CRT_ERROR (T_("_tsplitpath_s"), 0);
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

MIKTEXINTERNALFUNC(MIKTEXCHAR *)
GetUserProfileDirectory (/*[out]*/ MIKTEXCHAR * lpszPath)
{
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);

  bool found = false;

  const MIKTEXCHAR * lpszProfRecon =
    T_("Software\\Microsoft\\Windows\\CurrentVersion\\ProfileReconciliation");

  AutoHKEY hkey;

  LONG res =
    RegOpenKeyEx(HKEY_CURRENT_USER,
		 lpszProfRecon,
		 0,
		 KEY_READ,
		 &hkey);

  if (res == ERROR_SUCCESS)
    {
      // Windows 98
      unsigned long len = BufferSizes::MaxPath;
      unsigned long type =  REG_SZ;
      res =
	RegQueryValueEx(hkey.Get(),
			T_("ProfileDirectory"),
			0,
			&type,
			reinterpret_cast<LPBYTE>(lpszPath),
			&len);
      if (res == ERROR_SUCCESS)
	{
	  found = true;
	}
      else if (res != ERROR_FILE_NOT_FOUND)
	{
	  FATAL_WINDOWS_ERROR (T_("ProfileDirectory"), 0);
	}
    }
  else if (res != ERROR_FILE_NOT_FOUND)
    {
      FATAL_WINDOWS_ERROR (T_("ProfileDirectory"), lpszProfRecon);
    }

  if (! found)
    {
      tstring userProfile;
      if (Utils::GetEnvironmentString(T_("USERPROFILE"), userProfile)
	  && userProfile.length() > 0
	  && userProfile.length() < BufferSizes::MaxPath)
	{
	  Utils::CopyString (lpszPath,
			     BufferSizes::MaxPath,
			     userProfile.c_str());
	  found = true;
	}
    }

  return (found ? lpszPath : 0);
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

#define SM_SERVERR2 89
typedef void (WINAPI * PGNSI) (/*[out]*/ LPSYSTEM_INFO);

tstring
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
	  FATAL_WINDOWS_ERROR (T_("GetVersionEx"), 0);
	}
    }
  
  SYSTEM_INFO si;
  ZeroMemory (&si, sizeof(si));
  bool haveSystemInfo = false;

  // build version string
  tstring str;
  switch (osvi.dwPlatformId)
     {
     case VER_PLATFORM_WIN32_NT: // Windows NT product family
       if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
	 {
	   if (osvi.wProductType == VER_NT_WORKSTATION)
	     {
	       str += T_("Microsoft Windows Vista ");
	     }
	   else
	     {
	       str += T_("Windows Server \"Longhorn\" ");
	     }
	 }
       else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
	 {
	   // use GetProcAddress to avoid load issues on Windows 2000
	   PGNSI pGNSI =
	     reinterpret_cast<PGNSI>
	     (GetProcAddress(GetModuleHandle(T_("kernel32.dll")),
			     T_("GetNativeSystemInfo")));
	   if (pGNSI != 0)
	     {
	       pGNSI (&si);
	       haveSystemInfo = true;
	     }
	   if (GetSystemMetrics(SM_SERVERR2))
	     {
	       str += T_("Microsoft Windows Server 2003 \"R2\" ");
	     }
	   else if (osvi.wProductType == VER_NT_WORKSTATION
		    && haveSystemInfo
		    && (si.wProcessorArchitecture
			== PROCESSOR_ARCHITECTURE_AMD64))
	     {
	       str += T_("Microsoft Windows XP Professional x64 Edition ");
	     }
	   else
	     {
	       str += T_("Microsoft Windows Server 2003 ");
	     }
	 }
       else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
	 {
	   str += T_("Microsoft Windows XP ");
	 }
       else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
	 {
	   str += T_("Microsoft Windows 2000 ");
	 }
       else if (osvi.dwMajorVersion <= 4)
	 {
	   str += T_("Microsoft Windows NT ");
	 }
       if (haveOsVersionInfoEx)
	 {
	   // test for the workstation type
	   if (osvi.wProductType == VER_NT_WORKSTATION
	       && ! (haveSystemInfo
		     && (si.wProcessorArchitecture
			 == PROCESSOR_ARCHITECTURE_AMD64)))
	     {
	       if (osvi.dwMajorVersion == 4)
		 {
		   str += T_("Workstation 4.0 ");
		 }
	       else if ((osvi.wSuiteMask & VER_SUITE_PERSONAL) != 0)
		 {
		   str += T_("Home Edition ");
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
	       if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
		 {
		   if (haveSystemInfo
		       && (si.wProcessorArchitecture
			   == PROCESSOR_ARCHITECTURE_IA64))
		     {
		       if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) != 0)
			 {
			   str +=
			     T_("Datacenter Edition")
			     T_(" for Itanium-based Systems ");
			 }
		       else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
			 {
			   str +=
			     T_("Enterprise Edition")
			     T_(" for Itanium-based Systems ");
			 }
		     }
		   else if (haveSystemInfo
			    && (si.wProcessorArchitecture
				== PROCESSOR_ARCHITECTURE_AMD64))
		     {
		       if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) != 0)
			 {
			   str += T_("Datacenter x64 Edition ");
			 }
		       else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
			 {
			   str += T_("Enterprise x64 Edition ");
			 }
		       else
			 {
			   str += T_("Standard x64 Edition ");
			 }
		     }
		   else
		     {
		       if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) != 0)
			 {
			   str += T_("Datacenter Edition ");
			 }
		       else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
			 {
			   str+= T_("Enterprise Edition ");
			 }
		       else if ((osvi.wSuiteMask == VER_SUITE_BLADE) != 0)
			 {
			   str += T_("Web Edition ");
			 }
		       else
			 {
			   str += T_("Standard Edition ");
			 }
		     }
		 }
	       else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
		 {
		   if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) != 0)
		     {
		       str += T_("Datacenter Server ");
		     }
		   else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
		     {
		       str += T_("Advanced Server ");
		     }
		   else
		     {
		       str += T_("Server ");
		     }
		 }
	       else		// Windows NT 4.0
		 {
		   if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
		     {
		       str += T_("Server 4.0, Enterprise Edition " );
		     }
		   else
		     {
		       str += T_("Server 4.0 ");
		     }
		 }
	     }
	 }
       else  // test for specific product on Windows NT 4.0 SP5 and earlier
	 {
	   AutoHKEY hKey;
	   LONG lRet;
	   lRet =
	     RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			  (T_("SYSTEM\\CurrentControlSet\\Control")
			   T_("\\ProductOptions")),
			  0,
			  KEY_QUERY_VALUE,
			  &hKey);
	   if (lRet != ERROR_SUCCESS)
	     {
	       FATAL_WINDOWS_ERROR (T_("RegOpenKeyEx"), 0);
	     }
	   MIKTEXCHAR szProductType[80];
	   unsigned long dwBufLen = ARRAY_SIZE(szProductType);
	   lRet =
	     RegQueryValueEx(hKey.Get(),
			     T_("ProductType"),
			     0,
			     0,
			     reinterpret_cast<LPBYTE>(szProductType),
			     &dwBufLen);
	   if (lRet != ERROR_SUCCESS)
	     {
	       FATAL_WINDOWS_ERROR (T_("RegQueryValueEx"), 0);
	     }
	   if (dwBufLen > ARRAY_SIZE(szProductType))
	     {
	       BUF_TOO_SMALL (T_("GetWindowsVersion"));
	     }
	   else if (_tcsicmp(T_("WINNT"), szProductType) == 0)
	     {
	       str += T_("Workstation ");
	     }
	   else if (_tcsicmp(T_("LANMANNT"), szProductType) == 0)
	     {
	       str += T_("Server ");
	     }
	   else if (_tcsicmp (T_("SERVERNT"), szProductType) == 0)
	     {
	       str += T_("Advanced Server ");
	     }
	   str += NUMTOSTR(osvi.dwMajorVersion);
	   str += T_('.');
	   str += NUMTOSTR(osvi.dwMinorVersion);
	   str += T_(' ');
	 }
     
       // get service pack (if any) and build number
       if (osvi.dwMajorVersion == 4
	   && _tcsicmp(osvi.szCSDVersion, T_("Service Pack 6")) == 0)
	 {
	   HKEY hKey;
	   LONG lRet;
	 
	   // test for SP6 versus SP6a
	   lRet =
	     RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			  (T_("SOFTWARE\\Microsoft\\Windows NT")
			   T_("\\CurrentVersion\\Hotfix\\Q246009")),
			  0,
			  KEY_QUERY_VALUE,
			  &hKey);
	   if (lRet == ERROR_SUCCESS)
	     {
	       RegCloseKey (hKey);
	       str += T_("Service Pack 6a (Build ");
	       str += NUMTOSTR(osvi.dwBuildNumber & 0xFFFF);
	       str += T_(')');
	     }
	   else
	     {
	       str += osvi.szCSDVersion;
	       str += T_(" (Build ");
	       str += NUMTOSTR(osvi.dwBuildNumber & 0xFFFF);
	       str += T_(')');
	     }
	 }
       else // Windows NT 3.51 and earlier or Windows 2000 and later
	 {
	   str += osvi.szCSDVersion;
	   str += T_(" (Build ");
	   str += NUMTOSTR(osvi.dwBuildNumber & 0xFFFF);
	   str += T_(')');
	 }
       break;
     
     case VER_PLATFORM_WIN32_WINDOWS: // Windows 95 product family
       if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
         {
	   str += T_("Microsoft Windows 95");
	   if (osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B')
	     {
	       str += T_(" OSR2");
	     }
         }
       else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
         {
	   str += T_("Microsoft Windows 98");
	   if ( osvi.szCSDVersion[1] == 'A')
	     {
	       str += T_(" SE");
	     }
         } 
       else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
         {
	   str += T_("Microsoft Windows Millennium Edition");
         } 
       break;
     }

  return (str);
}

/* _________________________________________________________________________

   Utils::GetDefPrinter

   See Q246772.
   _________________________________________________________________________ */

MIKTEXAPI(bool)
Utils::GetDefPrinter (/*[out]*/ MIKTEXCHAR *		pPrinterName,
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
	  OUT_OF_MEMORY (T_("Utils::GetDefPrinter"));
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
#if defined(WINVER) && (WINVER >= 0x0500)
  if (osv.dwPlatformId == VER_PLATFORM_WIN32_NT && osv.dwMajorVersion >= 5)
    {
      return (GetDefaultPrinter(pPrinterName, pBufferSize) ? true : false);
    }
#endif
  TCHAR cBuffer[4096];
  if (GetProfileString(T_("windows"), T_("device"), T_(",,,"), cBuffer, 4096)
      <= 0)
    {
      return (false);
    }
  Tokenizer tok (cBuffer, T_(","));
  if (tok.GetCurrent() == 0)
    {
      return (false);
    }
  unsigned long l = static_cast<unsigned long>(StrLen(tok.GetCurrent()));
  if (l >= *pBufferSize)
    {
      *pBufferSize = l + 1;
      return (false);
    }
  Utils::CopyString (pPrinterName, *pBufferSize, tok.GetCurrent());
  *pBufferSize = l + 1;
  return (true);
}

/* _________________________________________________________________________

   SessionImpl::ShowManualPageAndWait
   _________________________________________________________________________ */

bool
SessionImpl::ShowManualPageAndWait (/*[in]*/ HWND		hWnd,
				    /*[in]*/ unsigned long	topic)
{
  PathName pathHelpFile;
  if (! FindFile(T_("miktex.chm"), T_("%R\\doc\\miktex//"), pathHelpFile))
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
  if (_tgetcwd(buffer, static_cast<int>(GetSize())) == 0)
    {
      FATAL_CRT_ERROR (T_("_tgetcwd"), 0);
    }
  return (*this);
}

/* _________________________________________________________________________

   PathName::SetToTempDirectory
   _________________________________________________________________________ */

PathName &
PathName::SetToTempDirectory ()
{
  unsigned long n = GetTempPath(static_cast<DWORD>(GetSize()), buffer);
  if (n == 0)
    {
      FATAL_WINDOWS_ERROR (T_("GetTempPath"), 0);
    }
  if (n >= GetSize())
    {
      UNEXPECTED_CONDITION (T_("PathName::SetToTempDirectory"));
    }
  return (*this);
}

/* _________________________________________________________________________

   PathName::SetToTempFile
   _________________________________________________________________________ */

PathName &
PathName::SetToTempFile ()
{
  PathName pathTempDir = SessionImpl::theSession->GetTempDirectory();

  UINT n = GetTempFileName(pathTempDir.Get(), T_("mik"), 0, buffer);

  if (n == 0)
    {
      FATAL_WINDOWS_ERROR (T_("GetTempFileName"), pathTempDir.Get());
    }

  SessionImpl::theSession->trace_tempfile->WriteFormattedLine
    (T_("core"),
     T_("created temporary file %s"),
     Q_(buffer));

  return (*this);
}

/* _________________________________________________________________________

   TraceWindowsError
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(void)
TraceWindowsError (/*[in]*/ const MIKTEXCHAR *	lpszWindowsFunction,
		   /*[in]*/ unsigned long	functionResult,
		   /*[in]*/ const MIKTEXCHAR *	lpszInfo,
		   /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		   /*[in]*/ int			sourceLine)
{
  tstring errorMessage;
  if (! GetWindowsErrorMessage(functionResult, errorMessage))
    {
      return;
    }
  SessionImpl::theSession->trace_error->WriteFormattedLine (T_("core"),
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
	 (lpszInfo == 0 ? T_("") : lpszInfo),
	 lpszSourceFile,
	 sourceLine);
}

/* _________________________________________________________________________

   AddEventSource
   _________________________________________________________________________ */

#if REPORT_EVENTS

#define SOURCE T_("MiKTeX")

#define EVTLOGAPP \
  T_("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application")

#if defined(MIKTEX_DLL)
MIKTEXSTATICFUNC(bool)
AddEventSource ()
{
  if (! IsWindowsNT())
    {
      return (false);
    }

  tstring registryPath = EVTLOGAPP;
  registryPath += T_('\\');
  registryPath += SOURCE;

  AutoHKEY hkey;
  unsigned long disp;
  long res =
    RegCreateKeyEx(HKEY_LOCAL_MACHINE,
		   registryPath.c_str(),
		   0,
		   T_(""),
		   REG_OPTION_NON_VOLATILE,
		   KEY_ALL_ACCESS,
		   0,
		   &hkey,
		   &disp);
  if (res != ERROR_SUCCESS)
    {
      TraceWindowsError (T_(""),
			 res,
			 registryPath.c_str(),
			 __FILE__,
			 __LINE__);
      return (false);
    }
  
  // set the name of the message file
  MIKTEXCHAR szModule[BufferSizes::MaxPath];
  if (GetModuleFileName(SessionImpl::hinstDLL, szModule, BufferSizes::MaxPath)
      == 0)
    {
      TraceStream::TraceLastWin32Error (T_("GetModuleFileName"),
					0,
					T_(__FILE__),
					__LINE__);
      return (false);
    }
  res =
    RegSetValueEx(hkey.Get(),
		  T_("EventMessageFile"),
		  0,
		  REG_EXPAND_SZ,
		  reinterpret_cast<unsigned char *>(&szModule),
		  static_cast<unsigned long>((StrLen(szModule) + 1)
					     * sizeof(MIKTEXCHAR)));
  if (res != ERROR_SUCCESS)
    {
      TraceWindowsError (T_("RegSetValueEx"),
			 res,
			 T_("EventMessageFile"),
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
		  T_("TypesSupported"),
		  0,
		  REG_DWORD,
		  reinterpret_cast<unsigned char *>(&dwData),
		  sizeof(unsigned long));
  
  if (res != ERROR_SUCCESS)
    {
      TraceWindowsError (T_("RegSetValueEx"),
			 res,
			 T_("TypesSupported"),
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

#if defined(MIKTEX_DLL)
MIKTEXSTATICFUNC(bool)
RemoveEventSource ()
{
  if (! IsWindowsNT())
    {
      return (false);
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
      TraceWindowsError (T_("RemoveEventSource"),
			 res,
			 T_("EventLog\\Application"),
			 __FILE__,
			 __LINE__);
      return (false);
    }
  res = RegDeleteKey(hkey.Get(), SOURCE);
  if (res != ERROR_SUCCESS)
    {
      TraceWindowsError (T_("RemoveEventSource"),
			 res,
			 T_("EventLog\\Application\\MiKTeX"),
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
      return (false);
    }

  vector<const MIKTEXCHAR *> vecStrings;
  va_list marker;
  va_start (marker, eventId);
  for (const MIKTEXCHAR * lpsz = va_arg(marker, const MIKTEXCHAR *);
       lpsz != 0;
       lpsz = va_arg(marker, const MIKTEXCHAR *))
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
      OUT_OF_MEMORY (T_("ReportMiKTeXEvent"));
    }
  MIKTEXCHAR szAccountName[BufferSizes::MaxPath];
  unsigned long n = BufferSizes::MaxPath;
  if (GetUserName(szAccountName, &n))
    {
      unsigned long sidSize = static_cast<unsigned long>(bufSize);
      pSid = reinterpret_cast<PSID>(pBuf.Get());
      MIKTEXCHAR szDomainName[BufferSizes::MaxPath];
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
SessionImpl::IsFileAlreadyOpen (/*[in]*/ const MIKTEXCHAR * lpszFileName)
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
	  FATAL_WINDOWS_ERROR (T_("CloseHandle"), lpszFileName);
	}
    }

  return ((hFile == INVALID_HANDLE_VALUE)
	  && (error == ERROR_SHARING_VIOLATION));
}

/* _________________________________________________________________________
   
   SessionImpl::ScheduleFileRemoval
   _________________________________________________________________________ */

void
SessionImpl::ScheduleFileRemoval (/*[in]*/ const MIKTEXCHAR * lpszFileName)
{
  MIKTEX_ASSERT_STRING (lpszFileName);
  trace_files->WriteFormattedLine
    (T_("core"),
     T_("scheduling removal of %s"),
     Q_(lpszFileName));
  if (IsWindowsNT())
    {
      if (! MoveFileEx(lpszFileName, 0, MOVEFILE_DELAY_UNTIL_REBOOT))
	{
	  FATAL_WINDOWS_ERROR (T_("MoveFileEx"), lpszFileName);
	}
    }
  else
    {
      MIKTEXCHAR szWinDir[BufferSizes::MaxPath];
      if (GetWindowsDirectory(szWinDir, BufferSizes::MaxPath) == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("GetWindowsDirectory"), 0);
	}
      PathName pathWinInitIni (szWinDir, T_("wininit"), T_(".ini"));
      vector<tstring> lines;
      lines.reserve (20);
      MIKTEXCHAR szShortPathName[BufferSizes::MaxPath];
      unsigned long n =
	GetShortPathName(lpszFileName, szShortPathName, BufferSizes::MaxPath);
      if (n == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("GetShortPathName"), lpszFileName);
	}
      else if (n >= BufferSizes::MaxPath)
	{
	  BUF_TOO_SMALL (T_("SessionImpl::ScheduleFileRemoval"));
	}
      tstring deleteStatement = T_("NUL=");
      deleteStatement += szShortPathName;
      bool hasDeleteStatement = false;
      bool hasRenameSection = false;
      bool inRenameSection = false;
      if (File::Exists(pathWinInitIni))
	{
	  StreamReader reader (pathWinInitIni);
	  tstring line;
	  while (reader.ReadLine(line))
	    {
	      if (line.length() > 0
		  && line[0] == T_('[')
		  && ! hasDeleteStatement)
		{
		  if (_tcsnicmp(line.c_str(), T_("[rename]"), 8) == 0)
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
	      lines.push_back (T_("[rename]"));
	    }
	  lines.push_back (deleteStatement);
	}
      StreamWriter writer(pathWinInitIni);
      for (vector<tstring>::const_iterator it = lines.begin();
	   it != lines.end();
	   ++ it)
	{
	  writer.WriteLine  (*it);
	}
      writer.Close ();
    }
}

/* _________________________________________________________________________

   SessionImpl::RunningAs

   See MSDN article "Windows NT Security" by Christopher Nefcy.
   _________________________________________________________________________ */

bool
SessionImpl::RunningAs (/*[in]*/ DWORD localGroup)
{
  if (! IsWindowsNT())
    {
      UNEXPECTED_CONDITION (T_("SessionImpl::RunningAs"));
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

   SessionImpl::RunningAsAdministrator
   _________________________________________________________________________ */

bool
MIKTEXCALL
SessionImpl::RunningAsAdministrator ()
{
  if (runningAsAdministrator == TriState::Undetermined)
    {
      runningAsAdministrator =
	(RunningAs(DOMAIN_ALIAS_RID_ADMINS)
	 ? TriState::True
	 : TriState::False);
    }
  return (runningAsAdministrator == TriState::True ? true : false);
}

/* _________________________________________________________________________

   SessionImpl::RunningAsPowerUser
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
bool
MIKTEXCALL
SessionImpl::RunningAsPowerUser ()
{
  if (runningAsPowerUser == TriState::Undetermined)
    {
      runningAsPowerUser =
	(RunningAs(DOMAIN_ALIAS_RID_POWER_USERS)
	 ? TriState::True
	 : TriState::False);
    }
  return (runningAsPowerUser == TriState::True ? true : false);
}
#endif

/* _________________________________________________________________________

   GetMediaType
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(unsigned int)
GetMediaType (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  PathName pathRootName;
  if (IsAlpha(lpszPath[0])
      && PathName::IsVolumeDelimiter(lpszPath[1])
      && PathName::IsDirectoryDelimiter(lpszPath[2]))
    {
      CopyString2 (pathRootName.GetBuffer(),
		   pathRootName.GetSize(),
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
FileIsOnROMedia (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  return (GetMediaType(lpszPath) == DRIVE_CDROM);
}

/* _________________________________________________________________________

   CreateDirectoryForEveryone

   Create a directory for the world group.
   _________________________________________________________________________ */

#if SET_SECURITY
MIKTEXSTATICFUNC(void)
CreateDirectoryForEveryone (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  if (! IsWindowsNT())
    {
      if (! CreateDirectory (lpszPath, 0))
	{
	  FATAL_WINDOWS_ERROR (T_("CreateDirectory"), lpszPath);
	}
      return;
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
      FATAL_WINDOWS_ERROR (T_("AllocateAndInitializeSid"), 0);
    }

  EXPLICIT_ACCESS ea[1];
  ZeroMemory (&ea, 1 * sizeof(EXPLICIT_ACCESS));
  ea[0].grfAccessPermissions = FILE_ALL_ACCESS;
  ea[0].grfAccessMode = GRANT_ACCESS;
  ea[0].grfInheritance= SUB_CONTAINERS_AND_OBJECTS_INHERIT;
  ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
  ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
  ea[0].Trustee.ptstrName =
    reinterpret_cast<MIKTEXCHAR *>(pEveryoneSID.Get());
  
  if (SetEntriesInAcl(1, ea, 0, &pACL) != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR (T_("SetEntriesInAcl"), 0);
    }

  AutoLocalMemory xxx (reinterpret_cast<void*>(pACL));
  
  PSECURITY_DESCRIPTOR pSD =
    reinterpret_cast<PSECURITY_DESCRIPTOR>
    (_alloca(SECURITY_DESCRIPTOR_MIN_LENGTH));

  if (! InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION)) 
    {
      FATAL_WINDOWS_ERROR (T_("InitializeSecurityDescriptor"), 0);
    } 
 
  if (! SetSecurityDescriptorDacl(pSD,
				  TRUE,
				  pACL,
				  FALSE))
    {
      FATAL_WINDOWS_ERROR (T_("SetSecurityDescriptorDacl"), 0);
    } 

  SECURITY_ATTRIBUTES sa;
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor = pSD;
  sa.bInheritHandle = FALSE;

  if (! CreateDirectory (lpszPath, &sa))
    {
      FATAL_WINDOWS_ERROR (T_("CreateDirectory"), lpszPath);
    }
}
#endif

/* _________________________________________________________________________

   CreateDirectoryPath

   Create a directory path.
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(void)
CreateDirectoryPath (/*[in]*/ const MIKTEXCHAR *	lpszPath)
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
  else if (! CreateDirectory(lpszPath, 0))
    {
      FATAL_WINDOWS_ERROR (T_("CreateDirectory"), lpszPath);
    }
#else
  if (! CreateDirectory(lpszPath, 0))
    {
      FATAL_WINDOWS_ERROR (T_("CreateDirectory"), lpszPath);
    }
#endif
}

/* _________________________________________________________________________

   PathName::AppendAltDirectoryDelimiter
   _________________________________________________________________________ */

PathName &
PathName::AppendAltDirectoryDelimiter ()
{
  size_t l = StrLen(buffer);
  if (l == 0 || ! IsDirectoryDelimiter(buffer[l - 1]))
    {
      if (l >= BufferSizes::MaxPath - 1)
	{
	  BUF_TOO_SMALL (T_("PathName::AppendAltDirectoryDelimiter"));
	}
      buffer[l] = static_cast<MIKTEXCHAR>(AltDirectoryDelimiter);
      buffer[l + 1] = 0;
    }
  return (*this);
}

/* _________________________________________________________________________

   ResolveSymbolicLinks
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(PathName)
ResolveSymbolicLinks (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  // <todo>follow junction points</todo>
  return (lpszPath);
}

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
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
  if (_tputenv_s(lpszValueName, lpszValue) != 0)
    {
      FATAL_CRT_ERROR (T_("_tputenv_s"), lpszValueName);
    }
#else
  tstring str = lpszValueName;
  str += T_('=');
  str += lpszValue;
#  if defined(_MSC_VER)
  if (_tputenv(str.c_str()) != 0)
    {
      FATAL_CRT_ERROR (T_("_tputenv"), str.c_str());
    }
#  elif defined(MIKTEX_UNICODE)
#      error Unimplemented: Utils::SetEnvironmentString()
#  else
  if (putenv(str.c_str()) != 0)
    {
      FATAL_CRT_ERROR (T_("putenv"), str.c_str());
    }
#  endif
#endif
}

/* _________________________________________________________________________

   Utils::RegisterMiKTeXUser
   _________________________________________________________________________ */

void
Utils::RegisterMiKTeXUser ()
{
  ShellExecute (0,
		T_("open"),
		T_("http://miktex.org/Registration.aspx"),
		0,
		0,
		SW_SHOW);
}

/* _________________________________________________________________________

   Argv::Build

   Mimic the behaviour of CommandLineToArgvW().
   _________________________________________________________________________ */

void
MIKTEXCALL
Argv::Build (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
	     /*[in]*/ const MIKTEXCHAR *	lpszArguments)
{
  argv.clear ();

  argv.push_back (StrDup(lpszFileName));

  for (const MIKTEXCHAR * lpsz = lpszArguments; *lpsz != 0; )
    {
      // skip white space
      for (; *lpsz == T_(' ') || *lpsz == T_('\t'); ++ lpsz)
	{
	}

      if (*lpsz == 0)
	{
	  break;
	}


      // get the next argument
      tstring arg;
      bool inQuotation = false;
      for (; ; ++ lpsz)
	{
	  // count backslashes
	  size_t nBackslashes = 0;
	  for (; *lpsz == T_('\\'); ++ lpsz)
	    {
	      ++ nBackslashes;
	    }

	  bool quoteOrUnquote = false;

	  // quotation mark madness
	  if (lpsz[0] == T_('"'))
	    {
	      if (nBackslashes % 2 == 0)
		{
		  if (lpsz[1] == T_('"') && inQuotation)
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
	      arg += T_('\\');
	    }

	  // check to see if we have the complete argument
	  if (*lpsz == 0
	      || ((*lpsz == T_(' ') || *lpsz == T_('\t'))
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

   DllMain

   DLL entry/exit routine.
   _________________________________________________________________________ */

#if defined(MIKTEX_DLL)

HINSTANCE SessionImpl::hinstDLL = 0;

int
WINAPI
DllMain (/*[in]*/ HINSTANCE		hinstDLL,
	 /*[in]*/ unsigned long		reason,
	 /*[in]*/ void *			)
{
  switch (reason)
    {
      // initialize primary thread
    case DLL_PROCESS_ATTACH:
      SessionImpl::hinstDLL = hinstDLL;
      break;

      // finalize primary thread
    case DLL_PROCESS_DETACH:
      SessionImpl::hinstDLL = 0;
      break;
    }

  return (1);
}

#endif

/* _________________________________________________________________________

   DllRegisterServer
   _________________________________________________________________________ */

#if defined(MIKTEX_DLL)
extern "C"
__declspec(dllexport)
HRESULT
__stdcall
DllRegisterServer (void)
{
#if REPORT_EVENTS
  if (! AddEventSource())
    {
      return (E_FAIL);
    }
#endif
  return (S_OK);
}
#endif

/* _________________________________________________________________________

   DllUnregisterServer
   _________________________________________________________________________ */

#if defined(MIKTEX_DLL)
extern "C"
__declspec(dllexport)
HRESULT
__stdcall
DllUnregisterServer (void)
{
#if REPORT_EVENTS
  if (RemoveEventSource())
    {
      return (E_FAIL);
    }
#endif
  return (S_OK);
}
#endif
