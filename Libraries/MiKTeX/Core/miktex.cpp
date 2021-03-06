/* miktex.cpp:

   Copyright (C) 1996-2015 Christian Schenk

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

#include "miktex/Core/Environment"
#include "miktex/Core/Paths"

#if defined(MIKTEX_WINDOWS)
#  include "win/winRegistry.h"
#endif

#include "core-version.h"
#include "fnamedb.h"

/* _________________________________________________________________________

   SessionImpl::theSession
   _________________________________________________________________________ */

SessionImpl * SessionImpl::theSession = 0;

/* _________________________________________________________________________

   SessionImpl::runningAsLocalServer
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
bool SessionImpl::runningAsLocalServer = false;
#endif

/* _________________________________________________________________________

   SessionImpl::GetPolicyFlags
   _________________________________________________________________________ */

#if ! defined(DEFAULT_MIKTEX_POLICIES)
#  define DEFAULT_MIKTEX_POLICIES (PolicyFlags::DataRootHighestPriority)
#endif

PolicyFlags
SessionImpl::GetPolicyFlags ()
{
  return (DEFAULT_MIKTEX_POLICIES);
}

/* _________________________________________________________________________

   Utils::GetExeName

   Get the application file name of the running program (e.g., tex).
   _________________________________________________________________________ */

string
Utils::GetExeName ()
{
  char szName[BufferSizes::MaxPath];
  SessionImpl::GetSession()
    ->GetMyProgramFile(false).GetFileNameWithoutExtension (szName);
  return (szName);
}

/* _________________________________________________________________________

   SessionImpl::GetMyLocation

   Get the location of the running executable (e.g.,
   C:\texmf\miktex\bin).
   _________________________________________________________________________ */

PathName
SessionImpl::GetMyLocation (/*[in]*/ bool canonicalized)
{
#if defined(MIKTEX_WINDOWS) && defined(MIKTEX_CORE_SHARED)
  return (GetDllPathName(canonicalized).RemoveFileSpec());
#else
  return (GetMyProgramFile(canonicalized).RemoveFileSpec());
#endif
}

/* _________________________________________________________________________

   GetHomeDirectory
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(PathName)
GetHomeDirectory ()
{
#if defined(MIKTEX_WINDOWS)
  string homeDrive;
  string homePath;
  if (Utils::GetEnvironmentString("HOMEDRIVE", homeDrive)
      && Utils::GetEnvironmentString("HOMEPATH", homePath))
    {
      return homeDrive + homePath;
    }
#endif
  PathName ret;
  if (Utils::GetEnvironmentString("HOME", ret))
    {
      return (ret);
    }
#if defined(MIKTEX_WINDOWS)
  if (GetUserProfileDirectory(ret))
    {
      return (ret);
    }
  wchar_t szWinDir[_MAX_PATH];
  unsigned int n = GetWindowsDirectoryW(szWinDir, _MAX_PATH);
  if (n == 0)
    {
      FATAL_WINDOWS_ERROR ("GetWindowsDirectoryW", 0);
    }
  else if (n >= _MAX_PATH)
    {
      BUF_TOO_SMALL ("GetHomeDirectory");
    }
  ret = szWinDir;
  return (ret);
#else
  UNEXPECTED_CONDITION ("GetHomeDirectory");
#endif
}

/* _________________________________________________________________________

   magstep
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(int)
magstep (/*[in]*/ int	n,
	 /*[in]*/ int	bdpi)
{
  double t;
  int neg = 0;
  if (n < 0)
    {
      neg = 1;
      n = -n;
    }
  if (n & 1)
    {
      n &= ~1;
      t = 1.095445115;
    }
  else
    {
      t = 1.0;
    }
  while (n > 8)
    {
      n -= 8;
      t = t * 2.0736;
    }
  while (n > 0)
    {
      n -= 2 ;
      t = t * 1.2;
    }
  if (neg)
    {
      return (static_cast<int>(0.5 + bdpi / t));
    }
  else
    {
      return (static_cast<int>(0.5 + bdpi * t));
    }
}

/* _________________________________________________________________________

   SessionImplMakeMakePkCommandLine
   _________________________________________________________________________ */

void
SessionImpl::MakeMakePkCommandLine (/*[in]*/ const char *	lpszFontName,
				    /*[in]*/ int		dpi,
				    /*[in]*/ int		baseDpi,
				    /*[in]*/ const char *	lpszMfMode,
				    /*[out]*/ PathName &	fileName,
				    /*[out]*/ char *		lpszArguments,
				    /*[in]*/ size_t		maxArguments)
{
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszMfMode);
  MIKTEX_ASSERT_CHAR_BUFFER (lpszArguments, maxArguments);

  MIKTEX_ASSERT (baseDpi != 0);

  if (! FindFile(MIKTEX_MAKEPK_EXE, FileType::EXE, fileName))
    {
      FATAL_MIKTEX_ERROR ("SessionImpl::MakeMakePkCommandLine",
			  T_("The MakePk utility could not be found."),
			  MIKTEX_MAKEPK_EXE);
    }

  int m = 0;
  int n;

  if (dpi < baseDpi)
    {
      for (;;)
	{
	  -- m;
	  n = magstep(m, baseDpi);
	  if (n == dpi)
	    {
	      break;
	    }
	  if (n < dpi || m < -40)
	    {
	      m = 9999;
	      break;
	    }
	}
    }
  else if (dpi > baseDpi)
    {
      for (;;)
	{
	  ++ m;
	  n = magstep(m, baseDpi);
	  if (n == dpi)
	    {
	      break;
	    }
	  if (n > dpi || m > 40)
	    {
	      m = 9999;
	      break;
	    }
	}
    }

  string strMagStep;

  if (m == 9999)
    {
      // a+b/c
      strMagStep = NUMTOSTR(dpi / baseDpi);
      strMagStep += '+';
      strMagStep += NUMTOSTR(dpi % baseDpi);
      strMagStep += '/';
      strMagStep += NUMTOSTR(baseDpi);
    }
  else if (m >= 0)
    {
      // magstep(a.b)
      strMagStep = "magstep(";
      strMagStep += NUMTOSTR( m / 2);
      strMagStep += '.';
      strMagStep += NUMTOSTR((m & 1) * 5);
      strMagStep += ')';
    }
  else
    {
      // magstep(-a.b)
      strMagStep = "magstep(-";
      strMagStep += NUMTOSTR((-m) / 2);
      strMagStep += '.';
      strMagStep += NUMTOSTR((m & 1) * 5);
      strMagStep += ')';
    }
  
  string cmdline;
  cmdline.reserve (256);

  cmdline += " --verbose";
  cmdline += ' '; cmdline += lpszFontName;
  cmdline += ' '; cmdline += NUMTOSTR(dpi);
  cmdline += ' '; cmdline += NUMTOSTR(baseDpi);
  cmdline += ' '; cmdline += strMagStep;

  if (lpszMfMode != 0)
    {
      cmdline += ' '; cmdline += lpszMfMode;
    }

  Utils::CopyString (lpszArguments, maxArguments, cmdline.c_str());
}

/* _________________________________________________________________________

   SessionnImpl::EnableFontMaker
   _________________________________________________________________________ */

bool
SessionImpl::EnableFontMaker (/*[in]*/ bool	enable)
{

  bool prev = makeFonts;
  makeFonts = enable;
  return (prev);
}

/* _________________________________________________________________________

   SessionImpl::GetMakeFontsFlag
   _________________________________________________________________________ */

bool
SessionImpl::GetMakeFontsFlag ()
{
  return (makeFonts);
}

/* _________________________________________________________________________

   SessionImpl::TryGetMiKTeXUserInfo
   _________________________________________________________________________ */

#if HAVE_MIKTEX_USER_INFO
bool
SessionImpl::TryGetMiKTeXUserInfo (/*[out]*/ MiKTeXUserInfo & info)
{
  static TriState haveResult = TriState::Undetermined;
  static MiKTeXUserInfo result;
  if (haveResult == TriState::Undetermined)
  {
    haveResult = TriState::False;
    string userInfoFile;
    if (! TryGetConfigValue(0, MIKTEX_REGVAL_USERINFO_FILE, userInfoFile))
    {
      return (false);
    }
    if (! File::Exists(userInfoFile))
    {
      return (false);
    }
    SmartPointer<Cfg> pcfg (Cfg::Create());
    pcfg->Read (userInfoFile, true);
    if (! pcfg->TryGetValue("user", "id", result.id))
    {
      return (false);
    }
    if (! pcfg->TryGetValue("user", "name", result.name))
    {
      return (false);
    }
    if (! pcfg->TryGetValue("user", "organization", result.organization))
    {
      result.organization = "";
    }
    if (! pcfg->TryGetValue("user", "email", result.email))
    {
      result.email = "";
    }
    string str;
    if (pcfg->TryGetValue("membership", "expirationdate", str))
    {
      int year, month, day;
      if (sscanf(str.c_str(), "%d-%d-%d", &year, &month, &day) == 3
	  && year >= 1970
	  && month >= 1 && month <= 12
	  && day >= 1 || day <= 31)
      {
	struct tm date;
	memset (&date, 0, sizeof(date));
	date.tm_year = year - 1900;
	date.tm_mon = month - 1;
	date.tm_mday = day;
	date.tm_hour = 23;
	date.tm_min = 59;
	date.tm_sec = 59;
	date.tm_isdst = -1;
	result.expirationDate = mktime(&date);
      }
    }
    if (pcfg->TryGetValue("membership", "level", str))
    {
      if (StringCompare(str.c_str(), "individual", true) == 0)
      {
	result.level = MiKTeXUserInfo::Individual;
      }
      else
      {
	result.level = atoi(str.c_str());
      }
    }
    if (pcfg->TryGetValue("membership", "role", str))
    {
      if (StringCompare(str.c_str(), "developer", true) == 0)
      {
	result.role = MiKTeXUserInfo::Developer;
      }
      else if (StringCompare(str.c_str(), "contributor", true) == 0)
      {
	result.role = MiKTeXUserInfo::Contributor;
      }
      else if (StringCompare(str.c_str(), "sponsor", true) == 0)
      {
	result.role = MiKTeXUserInfo::Sponsor;
      }
      else if (StringCompare(str.c_str(), "knownuser", true) == 0)
      {
	result.role = MiKTeXUserInfo::KnownUser;
      }
      else
      {
	result.role = atoi(str.c_str());
      }
    }
    haveResult = TriState::True;
  }
  if (haveResult == TriState::True)
  {
    info = result;
    return (true);
  }
  return (false);
}
#endif

/* _________________________________________________________________________

   SessionImpl::TryGetMiKTeXUserInfo
   _________________________________________________________________________ */

#if HAVE_MIKTEX_USER_INFO
MiKTeXUserInfo
SessionImpl::RegisterMiKTeXUser (/*[in]*/ const MiKTeXUserInfo & info)
{
  Utils::ShowWebPage (MIKTEX_URL_WWW_GIVE_BACK);
  // TODO
  throw new OperationCancelledException();
}
#endif

/* _________________________________________________________________________

   Utils::IsRegisteredMiKTeXUser
   _________________________________________________________________________ */

#if ! HAVE_MIKTEX_USER_INFO
bool
Utils::IsRegisteredMiKTeXUser ()
{
  return (false);
}
#endif

/* _________________________________________________________________________

   Utils::RegisterMiKTeXUser
   _________________________________________________________________________ */

#if ! HAVE_MIKTEX_USER_INFO
void
Utils::RegisterMiKTeXUser ()
{
  ShowWebPage (MIKTEX_URL_WWW_GIVE_BACK);
}
#endif

/* _________________________________________________________________________

   Utils::GetMiKTeXVersionString
   _________________________________________________________________________ */

string
Utils::GetMiKTeXVersionString ()
{
  return (MIKTEX_VERSION_STR);
}

/* _________________________________________________________________________

   Utils::GetMiKTeXBannerString
   _________________________________________________________________________ */

string
Utils::GetMiKTeXBannerString ()
{
  return (MIKTEX_BANNER_STR);
}

/* _________________________________________________________________________

   Utils::MakeProgramVersionString
   _________________________________________________________________________ */

string
Utils::MakeProgramVersionString
(/*[in]*/ const char *		lpszProgramName,
 /*[in]*/ const VersionNumber &	programVersionNumber)
{
  string str = lpszProgramName;
  if (string(MIKTEX_BANNER_STR).find(programVersionNumber.ToString())
      == string::npos)
    {
      str += ' ';
      str += programVersionNumber.ToString();
    }
  str += " (" MIKTEX_BANNER_STR ")";
  return (str);
}

/* _________________________________________________________________________

   VersionNumber::ToString
   _________________________________________________________________________ */

string
VersionNumber::ToString ()
  const
{
  string str = NUMTOSTR(n1);
  str += '.';
  str += NUMTOSTR(n2);
  if (n3 > 0 || n4 > 0)
    {
      str += '.';
      str += NUMTOSTR(n3);
      if (n4 > 0)
	{
	  str += '.';
	  str += NUMTOSTR(n4);
	}
    }
  return (str);
}

/* _________________________________________________________________________

   VersionNumber::TryParse
   _________________________________________________________________________ */

bool
VersionNumber::TryParse (/*[in]*/ const char *		lpszVersion,
			 /*[out]*/ VersionNumber &	versionNumber)
{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#  define SScanF sscanf_s
#else
#  define SScanF sscanf
#endif
  if (SScanF(lpszVersion,
	     "%u.%u.%u.%u",
	     &versionNumber.n1,
	     &versionNumber.n2,
	     &versionNumber.n3,
	     &versionNumber.n4)
      == 4)
    {
      return (true);
    }
  else if (SScanF(lpszVersion,
		  "%u.%u.%u",
		  &versionNumber.n1,
		  &versionNumber.n2,
		  &versionNumber.n3)
	   == 3)
    {
      versionNumber.n4 = 0;
      return (true);
    }
  else if (SScanF(lpszVersion,
		  "%u.%u",
		  &versionNumber.n1,
		  &versionNumber.n2)
	   == 2)
    {
      versionNumber.n3 = 0;
      versionNumber.n4 = 0;
      return (true);
    }
  else
    {
      return (false);
    }
#undef SScanF
}

/* _________________________________________________________________________

   VersionNumber::Parse
   _________________________________________________________________________ */

VersionNumber
VersionNumber::Parse (/*[in]*/ const char * lpsz)
{
  VersionNumber versionNumber;
  if (! TryParse(lpsz, versionNumber))
    {
      FATAL_MIKTEX_ERROR ("VersionNumber::Parse",
			  T_("A version number could not be parsed."),
			  lpsz);
    }
  return (versionNumber);
}

/* _________________________________________________________________________

   Fndb::Add
   _________________________________________________________________________ */

void
Fndb::Add (/*[in]*/ const char * lpszPath)
{
  Fndb::Add (lpszPath, 0);
}

/* _________________________________________________________________________

   Fndb::Add
   _________________________________________________________________________ */

void
Fndb::Add (/*[in]*/ const char * lpszPath,
	   /*[in]*/ const char * lpszFileNameInfo)
{
  MIKTEX_ASSERT_STRING (lpszPath);

#if 0
  MIKTEX_ASSERT (File::Exists(lpszPath));
#endif

  unsigned root = SessionImpl::GetSession()->DeriveTEXMFRoot(lpszPath);

  PathName pathFqFndbFileName;

  if (SessionImpl::GetSession()->FindFilenameDatabase(root, pathFqFndbFileName))
    {
      FileNameDatabase * pFndb =
	SessionImpl::GetSession()->GetFileNameDatabase(root, TriState::False);
      if (pFndb == 0)
	{
	  UNEXPECTED_CONDITION ("Fndb::Add");
	}
      AutoFndbRelease autoRelease (pFndb);
      pFndb->AddFile (lpszPath, lpszFileNameInfo);
    }
  else
    {
      // create the fndb file
      // <fixme>the file name info hasn't been added, if the file
      // exists</fixme>
      PathName pathFndbFile
	= SessionImpl::GetSession()->GetFilenameDatabasePathName(root);
      if (! Fndb::Create(pathFndbFile.Get(),
		 SessionImpl::GetSession()->GetRootDirectory(root).Get(),
			 0))
	{
	  UNEXPECTED_CONDITION ("Fndb::Add");
	}
      if (! File::Exists(lpszPath))
	{
	  // the file hasn't been added yet
	  // <recursivecall>
	  Add (lpszPath, lpszFileNameInfo);
	  // </recursivecall>
	}
    }
}

/* _________________________________________________________________________

   Fndb::Enumerate
   _________________________________________________________________________ */

bool
Fndb::Enumerate (/*[in]*/ const char *		lpszPath,
		 /*[in]*/ IEnumerateFndbCallback *	pCallback)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  FileNameDatabase * pFndb =
    SessionImpl::GetSession()->GetFileNameDatabase(lpszPath);
  
  if (pFndb == 0)
    {
      FATAL_MIKTEX_ERROR
	("Fndb::Enumerate",
	 T_("The path is not covered by the file name database."),
	 lpszPath);
    }

  AutoFndbRelease autoRelease (pFndb);
  
  return (pFndb->Enumerate(lpszPath, pCallback));
}

/* _________________________________________________________________________

   Fndb::Remove
   _________________________________________________________________________ */

void
Fndb::Remove (/*[in]*/ const char *	lpszPath)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  unsigned root = SessionImpl::GetSession()->DeriveTEXMFRoot(lpszPath);

  FileNameDatabase * pFndb =
    SessionImpl::GetSession()->GetFileNameDatabase(root, TriState::False);

  if (pFndb == 0)
    {
      UNEXPECTED_CONDITION ("Fndb::Remove");
    }
  
  AutoFndbRelease autoRelease (pFndb);

  pFndb->RemoveFile (lpszPath);
}

/* _________________________________________________________________________

   Fndb::FileExists
   _________________________________________________________________________ */

bool
Fndb::FileExists (/*[in]*/ const PathName &	path)
{
  unsigned root = SessionImpl::GetSession()->DeriveTEXMFRoot(path);

  FileNameDatabase * pFndb =
    SessionImpl::GetSession()->GetFileNameDatabase(root, TriState::True);

  if (pFndb == 0)
    {
      return (false);
    }
  
  AutoFndbRelease autoRelease (pFndb);

  return (pFndb->FileExists(path));
}

/* _________________________________________________________________________

   Fndb::Search
   _________________________________________________________________________ */

bool
Fndb::Search (/*[in]*/ const char *		lpszFileName,
	      /*[in]*/ const char *		lpszPathPattern,
	      /*[in]*/ bool			firstMatchOnly,
	      /*[out]*/ PathNameArray &		result,
	      /*[out]*/ vector<string> &	fileNameInfo)
{
  unsigned root = SessionImpl::GetSession()->DeriveTEXMFRoot(lpszPathPattern);

  FileNameDatabase * pFndb =
    SessionImpl::GetSession()->GetFileNameDatabase(root, TriState::True);

  if (pFndb == 0)
  {
    return (false);
  }
  
  AutoFndbRelease autoRelease (pFndb);

  return (pFndb->Search(lpszFileName, lpszPathPattern, firstMatchOnly, result, fileNameInfo));
}

/* _________________________________________________________________________

   GetEnvironmentString
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(bool)
GetEnvironmentString (/*[in]*/ const char * lpszName,
		      /*[in]*/ string &	    value)
{
#if defined(MIKTEX_WINDOWS)
  wchar_t * lpszValue = _wgetenv(UW_(lpszName));
  if (lpszValue == 0)
  {
    return (false);
  }
  else
  {
    value = WU_(lpszValue);
    return (true);
  }
#else
  const char * lpszValue = getenv(lpszName);
  if (lpszValue == 0)
  {
    return (false);
  }
  else
  {
    value = lpszValue;
    return (true);
  }
#endif
}

/* _________________________________________________________________________

   Utils::GetEnvironmentString
   _________________________________________________________________________ */

bool
Utils::GetEnvironmentString (/*[in]*/ const char *	lpszName,
			     /*[out]*/ string &		str)
{
  bool haveValue = ::GetEnvironmentString(lpszName, str);
  if (SessionImpl::TryGetSession() != 0
      && SessionImpl::GetSession()->trace_env.get() != 0
      && SessionImpl::GetSession()->trace_env->IsEnabled())
    {
      SessionImpl::GetSession()->trace_env->WriteFormattedLine
	("core",
	 "%s => %s",
	 lpszName,
	 (haveValue
	  ? str.c_str()
	  : "null"));
    }
  return (haveValue);
}

/* _________________________________________________________________________

   Utils::GetEnvironmentString
   _________________________________________________________________________ */

bool
Utils::GetEnvironmentString (/*[in]*/ const char *		lpszName,
			     /*[out]*/ PathName &		path)
{
  return (Utils::GetEnvironmentString(lpszName,
				      path.GetBuffer(),
				      path.GetCapacity()));
}

/* _________________________________________________________________________

   Utils::GetEnvironmentString
   _________________________________________________________________________ */

bool
Utils::GetEnvironmentString (/*[in]*/ const char *	lpszName,
			     /*[out]*/ char *		lpszOut,
			     /*[in]*/ size_t		sizeOut)
{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
  size_t bufSize;
  if (_wgetenv_s(&bufSize, 0, 0, UW_(lpszName)) != 0)
  {
    return (false);
  }
  if (bufSize == 0)
  {
    return (false);
  }
  CharBuffer<wchar_t> buf (bufSize);
  if (_wgetenv_s(&bufSize, buf.GetBuffer(), bufSize, UW_(lpszName)) != 0)
  {
    FATAL_CRT_ERROR ("_wgetenv_s", lpszName);
  }
  Utils::CopyString (lpszOut, sizeOut, buf.Get());
  return (true);
#else
  const char * lpsz = getenv(lpszName);
  if (lpsz == 0)
    {
      return (false);
    }
  Utils::CopyString (lpszOut, sizeOut, lpsz);
  return (true);
#endif
}

/* _________________________________________________________________________

   HaveEnvironmentString
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(bool)
HaveEnvironmentString (/*[in]*/ const char * lpszName)
{
  string value;
  return (GetEnvironmentString(lpszName, value));
}

/* _________________________________________________________________________

   LoadPublicKey
   _________________________________________________________________________ */

namespace {
#define PUBLIC_KEY_NAME DC13376B_CCAB_4B4B_B795_6AB245A77596
#define miktex_der PUBLIC_KEY_NAME
#include "miktex.der.h"
}

MIKTEXINTERNALFUNC(Botan::Public_Key *)
LoadPublicKey ()
{
  return (Botan::X509::load_key(
    Botan::MemoryVector<Botan::byte>(&PUBLIC_KEY_NAME[0], sizeof(PUBLIC_KEY_NAME))));
}

/* _________________________________________________________________________

   SessionImpl::SetCWDEnv
   _________________________________________________________________________ */

void
SessionImpl::SetCWDEnv ()
{
  string str;
  str.reserve (256);
  for (deque<PathName>::const_iterator it = inputDirectories.begin();
       it != inputDirectories.end();
       ++ it)
    {
      if (it != inputDirectories.begin())
	{
	  str += PATH_DELIMITER;
	}
      str += it->Get();
    }
  Utils::SetEnvironmentString (MIKTEX_ENV_CWD_LIST, str.c_str());
}

/* _________________________________________________________________________

   SessionImpl::AddInputDirectory
   _________________________________________________________________________ */

void
SessionImpl::AddInputDirectory (/*[in]*/ const char *	lpszPath,
				/*[in]*/ bool		atEnd)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  if (! Utils::IsAbsolutePath(lpszPath))
    {
      INVALID_ARGUMENT ("SessionImpl::AddInputDirectory", lpszPath);
    }

  // clear the search path cache
  ClearSearchVectors ();

  if (atEnd)
    {
      inputDirectories.push_back (lpszPath);
    }
  else
    {
      inputDirectories.push_front (lpszPath);
    }

#if 1
  SetCWDEnv ();
#endif
}

/* _________________________________________________________________________

   SessionImpl::GetWorkingDirectory
   _________________________________________________________________________ */

bool
SessionImpl::GetWorkingDirectory (/*[in]*/ unsigned	n,
				  /*[out]*/ PathName &	path)
{
  if (n == scratchDirectories.size() + inputDirectories.size())
  {
    return (false);
  }
  if (n > scratchDirectories.size() + inputDirectories.size())
  {
    INVALID_ARGUMENT ("GetWorkingDirectory", NUMTOSTR(n));
  }
  if (n < scratchDirectories.size())
  {
    path = scratchDirectories[scratchDirectories.size() - n - 1];
  }
  else
  {
    path = inputDirectories[n - scratchDirectories.size()];
  }
  return (true);
}

/* _________________________________________________________________________

   SessionImpl::SetEnvironmentVariables
   _________________________________________________________________________ */

void
SessionImpl::SetEnvironmentVariables ()
{
#if MIKTEX_WINDOWS
  string str;

  // Ghostscript
  Utils::SetEnvironmentString ("GSC", MIKTEX_GS_EXE);
  PathName root1 = GetSpecialPath(SpecialPath::CommonInstallRoot);
  PathName root2 = GetSpecialPath(SpecialPath::UserInstallRoot);
  PathName gsbase1 = root1;
  gsbase1 += "ghostscript";
  gsbase1 += "base";
  PathName gsbase2 = root2;
  gsbase2 += "ghostscript";
  gsbase2 += "base";
  str = gsbase1.Get();
  if (gsbase1 != gsbase2)
  {
    str += PATH_DELIMITER;
    str += gsbase2.Get();
  }
  PathName fonts1 = root1;
  fonts1 += "fonts";
  PathName fonts2 = root2;
  fonts2 += "fonts";
  str += PATH_DELIMITER;
  str += fonts1.Get();
  if (fonts1 != fonts2)
  {
    str += PATH_DELIMITER;
    str += fonts2.Get();
  }
  Utils::SetEnvironmentString ("MIKTEX_GS_LIB", str.c_str());
#endif

  PathName path = GetTempDirectory();

  if (! HaveEnvironmentString("TEMPDIR") || IsMiKTeXPortable())
    {
      Utils::SetEnvironmentString ("TEMPDIR", path.Get());
    }

  if (! HaveEnvironmentString("TMPDIR") || IsMiKTeXPortable())
    {
      Utils::SetEnvironmentString ("TMPDIR", path.Get());
    }

  if (! HaveEnvironmentString("TEMP") || IsMiKTeXPortable())
    {
      Utils::SetEnvironmentString ("TEMP", path.Get());
    }

  if (! HaveEnvironmentString("TMP") || IsMiKTeXPortable())
    {
      Utils::SetEnvironmentString ("TMP", path.Get());
    }

  if (! HaveEnvironmentString("HOME"))
    {
      Utils::SetEnvironmentString ("HOME", GetHomeDirectory().Get());
    }

  SetCWDEnv ();
}

/* _________________________________________________________________________

   SessionImpl::SessionImpl
   _________________________________________________________________________ */

SessionImpl::SessionImpl ()
  : initialized (false),
    commonInstallRootIndex (INVALID_ROOT_INDEX),
    userInstallRootIndex (INVALID_ROOT_INDEX),
    triMiKTeXDirect (TriState::Undetermined),
    commonDataRootIndex (INVALID_ROOT_INDEX),
    userDataRootIndex (INVALID_ROOT_INDEX),
    commonConfigRootIndex (INVALID_ROOT_INDEX),
    userConfigRootIndex (INVALID_ROOT_INDEX),
    recordingFileNames (false),
    recordingPackageNames (false),
    refCount (0),
    runningAsAdministrator (TriState::Undetermined),
    adminMode (false),
#if defined(MIKTEX_WINDOWS)
    runningAsPowerUser (TriState::Undetermined),
    isUserAPowerUser (TriState::Undetermined),
    numCoInitialize (0),
#endif
    makeFonts (true),
    pInstallPackageCallback (0),
    isUserAnAdministrator (TriState::Undetermined),

    // passing an empty string to the locale constructor is ok; it
    // means: "the user's preferred locale" (cf. "The C++ Programming
    // Language, Appendix D: Locales")
    defaultLocale ("")

{
}

/* _________________________________________________________________________

   SessionImpl::MyCoInitialize
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
void
SessionImpl::MyCoInitialize ()
{
  HResult hr = CoInitializeEx(0, COINIT_MULTITHREADED);
  if (hr.Failed())
    {
      FATAL_MIKTEX_ERROR ("SessionImpl::MyCoInitialize",
			  T_("The COM library could not be initialized."),
			  hr.GetText());
    }
  ++ numCoInitialize;
}
#endif

/* _________________________________________________________________________

   SessionImpl::MyCoUninitialize
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
void
SessionImpl::MyCoUninitialize ()
{
  if (numCoInitialize == 0)
    {
      UNEXPECTED_CONDITION ("SessionImpl::MyCoUninitialize");
    }
  CoUninitialize ();
  -- numCoInitialize;
}
#endif

/* _________________________________________________________________________

   SessionImpl::Initialize
   _________________________________________________________________________ */

void
SessionImpl::Initialize (/*[in]*/ const Session::InitInfo & initInfo)
{
  string val;

  PathName programInvocationName = initInfo.GetProgramInvocationName();
  programInvocationName = programInvocationName.GetFileNameWithoutExtension();
  const char * lpsz = strstr(programInvocationName.Get(), MIKTEX_ADMIN_SUFFIX);
  bool forceAdminMode =
    (lpsz != 0 && strlen(lpsz) == strlen(MIKTEX_ADMIN_SUFFIX));

#if defined(MIKTEX_WINDOWS)
  if (! forceAdminMode)
  {
    programInvocationName =
      GetMyProgramFile(false).Normalize().GetFileNameWithoutExtension().Get();
    lpsz = strstr(programInvocationName.Get(), MIKTEX_ADMIN_SUFFIX);
    forceAdminMode =
      (lpsz != 0 && strlen(lpsz) == strlen(MIKTEX_ADMIN_SUFFIX));      
  }
#endif

  bool oldAdminMode = adminMode;

  if (forceAdminMode)
  {
    adminMode = true;
  }
  else
  {
    adminMode = ((initInfo.GetFlags() & InitFlags::AdminMode) != 0);
  }

#if defined(_MSC_VER)
  if (Utils::GetEnvironmentString("MIKTEX_DEBUG_ON_STD_EXCEPTION", val))
    {
      debugOnStdException = atoi(val.c_str());
    }
#endif

  // check minimum system requirements (which is Windows 2000 atm)
#if defined(MIKTEX_WINDOWS) && ! defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
  DWORD version = GetVersion();
  bool nt = version < 0x80000000;
  DWORD major = LOBYTE(LOWORD(version));
  DWORD minor = HIBYTE(LOWORD(version));
  if ((! nt)			// Windows 9x/Me
      || (major < 5))		// Windows NT 3.x/4.x
    {
      string msg = T_("Unsupported operation system version (");
      if (nt)
	{
	  msg += "NT ";
	}
      msg += NUMTOSTR(major);
      msg += ".";
      msg += NUMTOSTR(minor);
      msg += ").";
      FATAL_MIKTEX_ERROR ("SessionImpl::Initialize",
			  msg.c_str(),
			  0);
    }
#endif

  if (initInfo.GetSizeOfStruct() != sizeof(initInfo))
    {
      INVALID_ARGUMENT ("SessionImpl::Initialize", 0);
    }

#if defined(MIKTEX_WINDOWS)
  if ((initInfo.GetFlags() & InitFlags::InitializeCOM) != 0)
    {
      MyCoInitialize ();
    }
#endif

  Botan::LibraryInitializer::initialize ();

  initialized = true;

  this->initInfo = initInfo;

  theNameOfTheGame = initInfo.GetTheNameOfTheGame();

  RegisterLibraryTraceStreams ();

  // enable trace streams
  string traceOptions;
  traceOptions = initInfo.GetTraceFlags();
  if (traceOptions.empty())
    {
      Utils::GetEnvironmentString (MIKTEX_ENV_TRACE, traceOptions);
    }
#if defined(MIKTEX_WINDOWS)
  if (traceOptions.empty()
      && (initInfo.GetFlags() & InitFlags::NoConfigFiles) == 0)
    {
      if (! winRegistry::TryGetRegistryValue(TriState::False,
					     MIKTEX_REGKEY_CORE,
					     MIKTEX_REGVAL_TRACE,
					     traceOptions,
					     0))
	{
	  traceOptions = "";
	}
    }
#endif
  if (! traceOptions.empty())
    {
      TraceStream::SetTraceFlags (traceOptions.c_str());
    }

  if (! forceAdminMode)
  {
    adminMode = oldAdminMode;
    SetAdminMode ((initInfo.GetFlags() & InitFlags::AdminMode) != 0);
  }

  DoStartupConfig ();

  InitializeRootDirectories ();

  Utils::GetEnvironmentString (MIKTEX_ENV_PACKAGE_LIST_FILE,
			       packageHistoryFile);

  PushAppName (Utils::GetExeName().c_str());
  
  // the process start directory is the initial scratch directory
  scratchDirectories.push_back (PathName().SetToCurrentDirectory());

  string miktexCwd;
  if (Utils::GetEnvironmentString(MIKTEX_ENV_CWD_LIST, miktexCwd))
    {
      for (CSVList cwd (miktexCwd.c_str(), PATH_DELIMITER);
	   cwd.GetCurrent() != 0;
	   ++ cwd)
	{
	  AddInputDirectory (cwd.GetCurrent(), true);
	}
    }
  
  SetEnvironmentVariables ();

  trace_core->WriteFormattedLine
    ("core",
     T_("initializing MiKTeX core library version %s"),
     MIKTEX_COMPONENT_VERSION_STR);

#if defined(MIKTEX_WINDOWS) && defined(MIKTEX_CORE_SHARED)
  if (dynamicLoad.Get() == TriState::True)
    {
      trace_core->WriteFormattedLine ("core", T_("dynamic load"));
    }
#endif
  
  trace_core->WriteFormattedLine ("core",
				  T_("operating system: %s"),
				  Utils::GetOSVersionString().c_str());
  
  trace_core->WriteFormattedLine ("core",
				  T_("program file: %s"),
				  GetMyProgramFile(true).Get());

  trace_config->WriteFormattedLine ("core",
				    T_("session locale: %s"),
				    defaultLocale.name().c_str());
  
}

/* _________________________________________________________________________

   SessionImpl::SetTheNameOfTheGame
   _________________________________________________________________________ */

void
SessionImpl::SetTheNameOfTheGame (/*[in]*/ const char * lpszTheNameOfTheGame)
{
  MIKTEX_ASSERT_STRING (lpszTheNameOfTheGame);
  fileTypes.clear ();
  theNameOfTheGame = lpszTheNameOfTheGame;
}

/* _________________________________________________________________________

   SessionImpl::~SessionImpl
   _________________________________________________________________________ */

SessionImpl::~SessionImpl ()
{
  try
    {
      Uninitialize ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   SessionImpl::Uninitialize
   _________________________________________________________________________ */

void
SessionImpl::Uninitialize ()
{
  if (! initialized)
    {
      return;
    }
  try
    {
      initialized = false;
      trace_core->WriteFormattedLine ("core",
				      T_("uninitializing core library"));
      CheckOpenFiles ();
      WritePackageHistory ();
      scratchDirectories.clear ();
      inputDirectories.clear ();
      UnregisterLibraryTraceStreams ();
      configurationSettings.clear ();
      Botan::LibraryInitializer::deinitialize ();
    }
  catch (const exception &)
    {
#if defined(MIKTEX_WINDOWS)
      while (numCoInitialize > 0)
	{
	  MyCoUninitialize ();
	}
#endif
      throw;
    }
#if defined(MIKTEX_WINDOWS)
  while (numCoInitialize > 0)
    {
      MyCoUninitialize ();
    }
#endif
}

/* _________________________________________________________________________

   SessionImpl::ConnectToServer
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER

void
SessionImpl::ConnectToServer ()
{
  const char * MSG_CANNOT_START_SERVER =
    T_("Cannot start MiKTeX session.");
  if (localServer.pSession == 0)
    {
      if (IsWindowsVista())
	{
	  WCHAR wszCLSID[50];
	  if (StringFromGUID2
	      (__uuidof(MiKTeXSessionLib::MAKE_CURVER_ID(MiKTeXSession)),
	       wszCLSID, 
	       sizeof(wszCLSID) / sizeof(wszCLSID[0]))
	      < 0)
	    {
	      FATAL_MIKTEX_ERROR ("ConnectToServer",
				  MSG_CANNOT_START_SERVER,
				  0);
	    }
	  wstring monikerName;
	  monikerName = L"Elevation:Administrator!new:";
	  monikerName += wszCLSID;
	  BIND_OPTS3 bo;
	  memset (&bo, 0, sizeof(bo));
	  bo.cbStruct = sizeof(bo);
	  bo.hwnd = GetForegroundWindow();
	  bo.dwClassContext = CLSCTX_LOCAL_SERVER;
	  HResult hr =
	    CoGetObject(monikerName.c_str(),
			&bo,
			__uuidof(MiKTeXSessionLib::ISession),
			reinterpret_cast<void**>(&localServer.pSession));
	  if (hr == CO_E_NOTINITIALIZED)
	    {
	      MyCoInitialize ();
	      hr =
		CoGetObject(monikerName.c_str(),
			    &bo,
			    __uuidof(MiKTeXSessionLib::ISession),
			    reinterpret_cast<void**>(&localServer.pSession));
	    }
	  if (hr.Failed())
	    {
	      FATAL_MIKTEX_ERROR ("ConnectToServer",
				  MSG_CANNOT_START_SERVER,
				  hr.GetText());
	    }
	}
      else
	{
	  HResult hr =
	    localServer.pSession.CoCreateInstance
	    (__uuidof(MiKTeXSessionLib::MAKE_CURVER_ID(MiKTeXSession)),
	     0,
	     CLSCTX_LOCAL_SERVER);
	  if (hr == CO_E_NOTINITIALIZED)
	    {
	      MyCoInitialize ();
	      hr =
		localServer.pSession.CoCreateInstance
		(__uuidof(MiKTeXSessionLib::MAKE_CURVER_ID(MiKTeXSession)),
		 0,
		 CLSCTX_LOCAL_SERVER);
	    }
	  if (hr.Failed())
	    {
	      FATAL_MIKTEX_ERROR ("ConnectToServer",
				  MSG_CANNOT_START_SERVER,
				  hr.GetText());
	    }
	}
    }
}

#endif
      
/* _________________________________________________________________________

   SessionImpl::UseLocalServer
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER

bool
SessionImpl::UseLocalServer ()
{
  if (SessionImpl::runningAsLocalServer)
    {
      // already running as local server
      return (false);
    }
#if defined(MIKTEX_WINDOWS)
  bool elevationRequired =
    (IsWindowsVista()
     && IsAdminMode()
     && ! SessionWrapper(true)->RunningAsAdministrator());
  return (elevationRequired);
#else
  return (false);
#endif
}

#endif

/* _________________________________________________________________________

   Session::Get

   Return the global session object.
   _________________________________________________________________________ */

Session *
Session::Get ()
{
  SessionImpl::GetSession()->refCount += 1;
  return (SessionImpl::GetSession());
}

/* _________________________________________________________________________

   Session::TryGet

   Return the global session object.
   _________________________________________________________________________ */

Session *
Session::TryGet ()
{
  if (SessionImpl::theSession != 0)
    {
      SessionImpl::theSession->refCount += 1;
    }
  return (SessionImpl::theSession);
}

/* _________________________________________________________________________

   Session::Get

   Initialize the global session object.
   _________________________________________________________________________ */

Session *
Session::Get (/*[in]*/ const Session::InitInfo & initInfo)
{
  if (SessionImpl::theSession != 0)
    {
      FATAL_MIKTEX_ERROR ("Session::Get",
			  T_("The core library is already initialized."),
			  0);
    }
  SessionImpl::theSession = new SessionImpl;
  try
    {
      SessionImpl::theSession->Initialize (initInfo);
      SessionImpl::theSession->refCount += 1;
    }
  catch (const exception &)
    {
      delete SessionImpl::theSession;
      SessionImpl::theSession = 0;
      throw;
    }
  return (SessionImpl::theSession);
}

/* _________________________________________________________________________

   Session::Release

   Release the global session object.
   _________________________________________________________________________ */

void
Session::Release (/*[in]*/ Session * pSession)
{
  if (pSession != SessionImpl::theSession)
    {
      UNEXPECTED_CONDITION ("Session::Release");
    }
  SessionImpl::theSession->refCount -= 1;
  if (SessionImpl::theSession->refCount == 0)
    {
      SessionImpl::theSession->Uninitialize ();
      delete SessionImpl::theSession;
      SessionImpl::theSession = 0;
    }
}

/* _________________________________________________________________________

   Session::~Session

   _________________________________________________________________________ */

Session::~Session ()
{
}

/* _________________________________________________________________________

   miktex_get_miktex_version_string_ex
   _________________________________________________________________________ */

MIKTEXCEEAPI(int)
miktex_get_miktex_version_string_ex (/*[out]*/ char *	lpszVersion,
				     /*[in]*/ size_t	bufSize)
{
  C_FUNC_BEGIN ();
  string version = Utils::GetMiKTeXVersionString();
  Utils::CopyString (lpszVersion, bufSize, version.c_str());
  return (1);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   DllMain
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && defined(MIKTEX_CORE_SHARED)

#if defined(_MANAGED)
#  pragma managed(push, off)
#endif

#if ! defined(MIKTEX_PREVENT_DYNAMIC_LOADS)
#  define MIKTEX_PREVENT_DYNAMIC_LOADS 0
#endif

HINSTANCE SessionImpl::hinstDLL = 0;
TriState SessionImpl::dynamicLoad = TriState::Undetermined;

extern "C"
BOOL
WINAPI
DllMain (/*[in]*/ HINSTANCE	hInstance,
	 /*[in]*/ DWORD		reason,
	 /*[in]*/ LPVOID	lpReserved)
{
  BOOL retCode = TRUE;

  switch (reason)
    {
      // initialize primary thread
    case DLL_PROCESS_ATTACH:
      SessionImpl::dynamicLoad =
	(lpReserved == 0 ? TriState::True : TriState::False);
#if MIKTEX_PREVENT_DYNAMIC_LOADS
      if (SessionImpl::dynamicLoad == TriState::True)
	{
	  retCode = FALSE;
	}
#endif
      SessionImpl::hinstDLL = hInstance;
      break;

      // finalize primary thread
    case DLL_PROCESS_DETACH:
      SessionImpl::dynamicLoad = TriState::Undetermined;
      SessionImpl::hinstDLL = 0;
      break;
    }

#if defined(HAVE_ATLBASE_H) && defined(MIKTEX_CORE_SHARED)
  if (retCode)
    {
      retCode = SessionImpl::AtlDllMain(reason, lpReserved);
    }
#endif

  return (retCode);
}

#if defined(_MANAGED)
#  pragma managed(pop)
#endif

#endif
