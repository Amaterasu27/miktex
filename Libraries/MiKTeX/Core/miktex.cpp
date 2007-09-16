/* miktex.cpp:

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

#include "miktex/env.h"
#include "miktex/paths.h"

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
MIKTEXCALL
SessionImpl::GetPolicyFlags ()
{
  return (DEFAULT_MIKTEX_POLICIES);
}

/* _________________________________________________________________________

   Utils::GetExeName

   Get the application file name of the running program (e.g., tex).
   _________________________________________________________________________ */

tstring
Utils::GetExeName ()
{
  MIKTEXCHAR szName[BufferSizes::MaxPath];
  SessionImpl::GetSession()
    ->GetMyProgramFile().GetFileNameWithoutExtension (szName);
  return (szName);
}

/* _________________________________________________________________________

   SessionImpl::GetMyLocation

   Get the location of the running executable (e.g.,
   C:\texmf\miktex\bin).
   _________________________________________________________________________ */

PathName
MIKTEXCALL
SessionImpl::GetMyLocation ()
{
  return (GetMyProgramFile().RemoveFileSpec());
}

/* _________________________________________________________________________

   GetHomeDirectory
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(PathName)
GetHomeDirectory ()
{
  PathName ret;
#if defined(MIKTEX_WINDOWS)
  PathName homeDrive;
  PathName homePath;
  if (Utils::GetEnvironmentString(T_("HOMEDRIVE"), homeDrive)
      && Utils::GetEnvironmentString(T_("HOMEPATH"), homePath))
    {
      ret = homeDrive;
      ret += homePath;
      return (ret);
    }
#endif
  if (Utils::GetEnvironmentString(T_("HOME"), ret))
    {
      return (ret);
    }
#if defined(MIKTEX_WINDOWS)
  if (GetUserProfileDirectory(ret))
    {
      return (ret);
    }
  unsigned int n = GetWindowsDirectory(ret.GetBuffer(), ret.GetSize());
  if (n == 0)
    {
      FATAL_WINDOWS_ERROR (T_("GetWindowsDirectory"), 0);
    }
  else if (n >= ret.GetSize())
    {
      BUF_TOO_SMALL (T_("GetHomeDirectory"));
    }
  return (ret);
#else
  UNEXPECTED_CONDITION (T_("GetHomeDirectory"));
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
SessionImpl::MakeMakePkCommandLine (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
				    /*[in]*/ int		dpi,
				    /*[in]*/ int		baseDpi,
				    /*[in]*/ const MIKTEXCHAR *	lpszMfMode,
				    /*[out]*/ PathName &	fileName,
				    /*[out]*/ MIKTEXCHAR *	lpszArguments,
				    /*[in]*/ size_t		maxArguments)
{
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_STRING_OR_NIL (lpszMfMode);
  MIKTEX_ASSERT_CHAR_BUFFER (lpszArguments, maxArguments);

  MIKTEX_ASSERT (baseDpi != 0);

  if (! FindFile(MIKTEX_MAKEPK_EXE, FileType::EXE, fileName))
    {
      FATAL_MIKTEX_ERROR (T_("SessionImpl::MakeMakePkCommandLine"),
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

  tstring strMagStep;

  if (m == 9999)
    {
      // a+b/c
      strMagStep = NUMTOSTR(dpi / baseDpi);
      strMagStep += T_('+');
      strMagStep += NUMTOSTR(dpi % baseDpi);
      strMagStep += T_('/');
      strMagStep += NUMTOSTR(baseDpi);
    }
  else if (m >= 0)
    {
      // magstep(a.b)
      strMagStep = T_("magstep(");
      strMagStep += NUMTOSTR( m / 2);
      strMagStep += T_('.');
      strMagStep += NUMTOSTR((m & 1) * 5);
      strMagStep += T_(')');
    }
  else
    {
      // magstep(-a.b)
      strMagStep = T_("magstep(-");
      strMagStep += NUMTOSTR((-m) / 2);
      strMagStep += T_('.');
      strMagStep += NUMTOSTR((m & 1) * 5);
      strMagStep += T_(')');
    }
  
  tstring cmdline;
  cmdline.reserve (256);

  cmdline += T_(" --verbose");
  cmdline += T_(' '); cmdline += lpszFontName;
  cmdline += T_(' '); cmdline += NUMTOSTR(dpi);
  cmdline += T_(' '); cmdline += NUMTOSTR(baseDpi);
  cmdline += T_(' '); cmdline += strMagStep;

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

   Utils::GetMiKTeXVersionString
   _________________________________________________________________________ */

tstring
Utils::GetMiKTeXVersionString ()
{
  return (MIKTEX_VERSION_STR);
}

/* _________________________________________________________________________

   Utils::GetMiKTeXBannerString
   _________________________________________________________________________ */

tstring
Utils::GetMiKTeXBannerString ()
{
  return (MIKTEX_BANNER_STR);
}

/* _________________________________________________________________________

   Utils::MakeProgramVersionString
   _________________________________________________________________________ */

tstring
Utils::MakeProgramVersionString
(/*[in]*/ const MIKTEXCHAR *	lpszProgramName,
 /*[in]*/ const VersionNumber &	programVersionNumber)
{
  tstring str = lpszProgramName;
  if (tstring(MIKTEX_BANNER_STR).find(programVersionNumber.ToString())
      == tstring::npos)
    {
      str += T_(' ');
      str += programVersionNumber.ToString();
    }
  str += T_(" (") MIKTEX_BANNER_STR T_(")");
  return (str);
}

/* _________________________________________________________________________

   VersionNumber::ToString
   _________________________________________________________________________ */

tstring
MIKTEXCALL
VersionNumber::ToString ()
  const
{
  tstring str = NUMTOSTR(n1);
  str += T_('.');
  str += NUMTOSTR(n2);
  if (n3 > 0 || n4 > 0)
    {
      str += T_('.');
      str += NUMTOSTR(n3);
      if (n4 > 0)
	{
	  str += T_('.');
	  str += NUMTOSTR(n4);
	}
    }
  return (str);
}

/* _________________________________________________________________________

   VersionNumber::TryParse
   _________________________________________________________________________ */

bool
MIKTEXCALL
VersionNumber::TryParse (/*[in]*/ const MIKTEXCHAR *	lpszVersion,
			 /*[out]*/ VersionNumber &	versionNumber)
{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#  define SScanF _stscanf_s
#else
#  define SScanF sscanf
#endif
  if (SScanF(lpszVersion,
	     T_("%u.%u.%u.%u"),
	     &versionNumber.n1,
	     &versionNumber.n2,
	     &versionNumber.n3,
	     &versionNumber.n4)
      == 4)
    {
      return (true);
    }
  else if (SScanF(lpszVersion,
		  T_("%u.%u.%u"),
		  &versionNumber.n1,
		  &versionNumber.n2,
		  &versionNumber.n3)
	   == 3)
    {
      versionNumber.n4 = 0;
      return (true);
    }
  else if (SScanF(lpszVersion,
		  T_("%u.%u"),
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
MIKTEXCALL
VersionNumber::Parse (/*[in]*/ const MIKTEXCHAR * lpsz)
{
  VersionNumber versionNumber;
  if (! TryParse(lpsz, versionNumber))
    {
      FATAL_MIKTEX_ERROR (T_("VersionNumber::Parse"),
			  T_("A version number could not be parsed."),
			  lpsz);
    }
  return (versionNumber);
}

/* _________________________________________________________________________

   Fndb::Add
   _________________________________________________________________________ */

void
Fndb::Add (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  Fndb::Add (lpszPath, 0);
}

/* _________________________________________________________________________

   Fndb::Add
   _________________________________________________________________________ */

void
Fndb::Add (/*[in]*/ const MIKTEXCHAR * lpszPath,
	   /*[in]*/ const MIKTEXCHAR * lpszFileNameInfo)
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
	  UNEXPECTED_CONDITION (T_("Fndb::Add"));
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
	  UNEXPECTED_CONDITION (T_("Fndb::Add"));
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
Fndb::Enumerate (/*[in]*/ const MIKTEXCHAR *		lpszPath,
		 /*[in]*/ IEnumerateFndbCallback *	pCallback)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  FileNameDatabase * pFndb =
    SessionImpl::GetSession()->GetFileNameDatabase(lpszPath);
  
  if (pFndb == 0)
    {
      FATAL_MIKTEX_ERROR
	(T_("Fndb::Enumerate"),
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
MIKTEXCALL
Fndb::Remove (/*[in]*/ const MIKTEXCHAR *	lpszPath)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  unsigned root = SessionImpl::GetSession()->DeriveTEXMFRoot(lpszPath);

  FileNameDatabase * pFndb =
    SessionImpl::GetSession()->GetFileNameDatabase(root, TriState::False);

  if (pFndb == 0)
    {
      UNEXPECTED_CONDITION (T_("Fndb::Remove"));
    }
  
  AutoFndbRelease autoRelease (pFndb);

  pFndb->RemoveFile (lpszPath);
}

/* _________________________________________________________________________

   Fndb::FileExists
   _________________________________________________________________________ */

bool
MIKTEXCALL
Fndb::FileExists (/*[in]*/ const PathName &	path)
{
  unsigned root = SessionImpl::GetSession()->DeriveTEXMFRoot(path);

  FileNameDatabase * pFndb =
    SessionImpl::GetSession()->GetFileNameDatabase(root, TriState::False);

  if (pFndb == 0)
    {
      return (false);
    }
  
  AutoFndbRelease autoRelease (pFndb);

  return (pFndb->FileExists(path));
}

/* _________________________________________________________________________

   GetEnvironmentString

   Fast but insecure.
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(const MIKTEXCHAR *)
GetEnvironmentString (/*[in]*/ const MIKTEXCHAR * lpszName)
{
#if defined(_MSC_VER)
#  pragma warning (push)
#  pragma warning (disable: 4996)
  return (_tgetenv(lpszName));
#  pragma warning (pop)
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: GetEnvironmentString()
#else
  return (getenv(lpszName));
#endif
}

/* _________________________________________________________________________

   Utils::GetEnvironmentString
   _________________________________________________________________________ */

bool
Utils::GetEnvironmentString (/*[in]*/ const MIKTEXCHAR *	lpszName,
			     /*[out]*/ tstring &		str)
{
  bool haveValue = false;
  const MIKTEXCHAR * lpszOut = ::GetEnvironmentString(lpszName);
  if (lpszOut != 0)
    {
      str = lpszOut;
      haveValue = true;
    }
  if (SessionImpl::TryGetSession() != 0
      && SessionImpl::GetSession()->trace_env.get() != 0
      && SessionImpl::GetSession()->trace_env->IsEnabled())
    {
      SessionImpl::GetSession()->trace_env->WriteFormattedLine
	(T_("core"),
	 T_("%s => %s"),
	 lpszName,
	 (haveValue
	  ? str.c_str()
	  : T_("null")));
    }
  return (haveValue);
}

/* _________________________________________________________________________

   Utils::GetEnvironmentString
   _________________________________________________________________________ */

bool
Utils::GetEnvironmentString (/*[in]*/ const MIKTEXCHAR *	lpszName,
			     /*[out]*/ PathName &		path)
{
  return (Utils::GetEnvironmentString(lpszName,
				      path.GetBuffer(),
				      path.GetSize()));
}

/* _________________________________________________________________________

   Utils::GetEnvironmentString
   _________________________________________________________________________ */

bool
Utils::GetEnvironmentString (/*[in]*/ const MIKTEXCHAR *	lpszName,
			     /*[out]*/ MIKTEXCHAR *		lpszOut,
			     /*[in]*/ size_t			sizeOut)
{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
  size_t bufSize;
  if (_tgetenv_s(&bufSize,
		 0,
		 0,
		 lpszName)
      != 0)
    {
      return (false);
    }
  if (bufSize == 0)
    {
      return (false);
    }
  if (bufSize > sizeOut)
    {
      BUF_TOO_SMALL (T_("Utils::GetEnvironmentString"));
    }
  if (_tgetenv_s(&bufSize,
		 lpszOut,
		 bufSize,
		 lpszName)
      != 0)
    {
      FATAL_CRT_ERROR (T_("_tgetenv_s"), lpszName);
    }
  return (true);
#else
#  if defined(_MSC_VER)
  const MIKTEXCHAR * lpsz = _tgetenv(lpszName);
#  elif defined(MIKTEX_UNICODE)
#    error Unimplemented: Utils::GetEnvironmentString()
#  else
  const MIKTEXCHAR * lpsz = getenv(lpszName);
#  endif
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
HaveEnvironmentString (/*[in]*/ const MIKTEXCHAR * lpszName)
{
  return (GetEnvironmentString(lpszName) != 0);
}

/* _________________________________________________________________________

   SessionImpl::SetCWDEnv
   _________________________________________________________________________ */

void
SessionImpl::SetCWDEnv ()
{
  tstring str;
  str.reserve (256);
  for (deque<PathName>::const_iterator it = workingDirectories.begin();
       it != workingDirectories.end();
       ++ it)
    {
      if (it != workingDirectories.begin())
	{
	  str += PATH_DELIMITER;
	}
      str += it->Get();
    }
  Utils::SetEnvironmentString (MIKTEX_ENV_CWD_LIST, str.c_str());
}

/* _________________________________________________________________________

   SessionImpl::AddWorkingDirectory
   _________________________________________________________________________ */

void
SessionImpl::AddWorkingDirectory (/*[in]*/ const MIKTEXCHAR *	lpszPath,
				  /*[in]*/ bool			atEnd)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  if (! Utils::IsAbsolutePath(lpszPath))
    {
      INVALID_ARGUMENT (T_("SessionImpl::AddWorkingDirectory"), lpszPath);
    }

  // clear the search path cache
  ClearSearchVectors ();

  if (atEnd)
    {
      workingDirectories.push_back (lpszPath);
    }
  else
    {
      workingDirectories.push_front (lpszPath);
    }
}

/* _________________________________________________________________________

   SessionImpl::RemoveWorkingDirectory
   _________________________________________________________________________ */

void
SessionImpl::RemoveWorkingDirectory (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  // clear the search path cache
  ClearSearchVectors ();

  deque<PathName>::iterator it =
    find(workingDirectories.begin(),
	 workingDirectories.end(),
	 lpszPath);

  if (it == workingDirectories.end())
    {
      INVALID_ARGUMENT (T_("SessionImpl::RemoveWorkingDirectory"), lpszPath);
    }

  workingDirectories.erase (it);
}

/* _________________________________________________________________________

   SessionImpl::GetWorkingDirectory
   _________________________________________________________________________ */

bool
SessionImpl::GetWorkingDirectory (/*[in]*/ unsigned	n,
				  /*[out]*/ PathName &	path)
{
  if (n == 0)
    {
      path.SetToCurrentDirectory ();
      return (true);
    }
  -- n;
  if (n == workingDirectories.size())
    {
      return (false);
    }
  if (n > workingDirectories.size())
    {
      INVALID_ARGUMENT (T_("GetWorkingDirectory"), NUMTOSTR(n));
    }
  path = workingDirectories[n];
  return (true);
}

/* _________________________________________________________________________

   SessionImpl::SetEnvironmentVariables
   _________________________________________________________________________ */

void
SessionImpl::SetEnvironmentVariables ()
{
  Utils::SetEnvironmentString (T_("GSC"), MIKTEX_GS_EXE);
  Utils::SetEnvironmentString (T_("TEXSYSTEM"), T_("miktex"));
  Utils::SetEnvironmentString (T_("TEXMFLOCAL"),
			       GetSpecialPath(SpecialPath::DataRoot).Get());
  Utils::SetEnvironmentString (T_("TEXMFMAIN"),
			       GetSpecialPath(SpecialPath::InstallRoot).Get());

  tstring str;
  str = T_("%r");
  str += PathName::DirectoryDelimiter;
  str += T_("ghostscript");
  str += PathName::DirectoryDelimiter;
  str += T_("base");
  tstring searchPath;
  AppendToSearchPath (searchPath, str);
  str = T_("%r");
  str += PathName::DirectoryDelimiter;
  str +=  T_("fonts");
  AppendToSearchPath (searchPath, str);
  Utils::SetEnvironmentString (T_("MIKTEX_GS_LIB"), searchPath.c_str());

  PathName path = GetTempDirectory();

  if (! HaveEnvironmentString(T_("TEMPDIR")))
    {
      Utils::SetEnvironmentString (T_("TEMPDIR"), path.Get());
    }

  if (! HaveEnvironmentString(T_("TMPDIR")))
    {
      Utils::SetEnvironmentString (T_("TMPDIR"), path.Get());
    }

  if (! HaveEnvironmentString(T_("TEMP")))
    {
      Utils::SetEnvironmentString (T_("TEMP"), path.Get());
    }

  if (! HaveEnvironmentString(T_("TMP")))
    {
      Utils::SetEnvironmentString (T_("TMP"), path.Get());
    }

  if (! HaveEnvironmentString(T_("HOME")))
    {
      Utils::SetEnvironmentString (T_("HOME"), GetHomeDirectory().Get());
    }

  SetCWDEnv ();
}

/* _________________________________________________________________________

   SessionImpl::SessionImpl
   _________________________________________________________________________ */

SessionImpl::SessionImpl ()
  : initialized (false),
    installRootIndex (INVALID_ROOT_INDEX),
    triMiKTeXDirect (TriState::Undetermined),
    commonDataRootIndex (INVALID_ROOT_INDEX),
    userDataRootIndex (INVALID_ROOT_INDEX),
    commonConfigRootIndex (INVALID_ROOT_INDEX),
    userConfigRootIndex (INVALID_ROOT_INDEX),
    recordingFileNames (false),
    recordingPackageNames (false),
    refCount (0),
    runningAsAdministrator (TriState::Undetermined),
#if defined(MIKTEX_WINDOWS)
    runningAsPowerUser (TriState::Undetermined),
    isUserAPowerUser (TriState::Undetermined),
#endif
    sharedSetup (TriState::Undetermined),
    makeFonts (true),
    pInstallPackageCallback (0),
    haveStartupConfigFile (false),
    isUserAnAdministrator (TriState::Undetermined),

    // passing an empty string to the locale constructor is ok; it
    // means: "the user's preferred locale" (cf. "The C++ Programming
    // Language, Appendix D: Locales")
    defaultLocale ("")

{
}

/* _________________________________________________________________________

   SessionImpl::Initialize
   _________________________________________________________________________ */

void
SessionImpl::Initialize (/*[in]*/ const Session::InitInfo & initInfo)
{
  tstring val;

#if defined(_MSC_VER)
  if (Utils::GetEnvironmentString(T_("MIKTEX_DEBUG_ON_STD_EXCEPTION"), val))
    {
      debugOnStdException = _ttoi(val.c_str());
    }
#endif

  if (initInfo.GetSizeOfStruct() != sizeof(initInfo))
    {
      INVALID_ARGUMENT (T_("SessionImpl::Initialize"), 0);
    }

  // check system requirements
#if ! defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
  if (GetVersion() < 0x80000000)
    {
      FATAL_MIKTEX_ERROR (T_("SessionImpl::Initialize"),
			  T_("This platform is not supported."),
			  0);
    }
#endif

#if defined(MIKTEX_WINDOWS)
  if ((initInfo.GetFlags() & InitFlags::InitializeCOM) != 0)
    {
      HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
      if (FAILED(hr))
	{
	  FATAL_MIKTEX_ERROR (T_(""),
			      T_("The COM library could not be initialized."),
			      NUMTOSTR(hr));
	}
    }
#endif

  initialized = true;

  this->initInfo = initInfo;

  RegisterLibraryTraceStreams ();

  // enable trace streams
  tstring traceOptions;
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
	  traceOptions = T_("");
	}
    }
#endif
  if (! traceOptions.empty())
    {
      TraceStream::SetTraceFlags (traceOptions.c_str());
    }

  InitializeRootDirectories ();

  Utils::GetEnvironmentString (MIKTEX_ENV_PACKAGE_LIST_FILE,
			       packageHistoryFile);

  PushAppName (Utils::GetExeName().c_str());
  
  tstring miktexCwd;
  if (Utils::GetEnvironmentString(MIKTEX_ENV_CWD_LIST, miktexCwd))
    {
      for (CSVList cwd (miktexCwd.c_str(), PATH_DELIMITER);
	   cwd.GetCurrent() != 0;
	   ++ cwd)
	{
	  AddWorkingDirectory (cwd.GetCurrent(), true);
	}
    }
  
  SetEnvironmentVariables ();

  trace_core->WriteFormattedLine
    (T_("core"),
     T_("initializing MiKTeX core library version %s"),
     VER_FILEVERSION_STR);

#if defined(MIKTEX_WINDOWS) && ! defined(MIKTEX_STATIC)
  if (dynamicLoad.Get() == TriState::True)
    {
      trace_core->WriteFormattedLine (T_("core"), T_("dynamic load"));
    }
#endif
  
  trace_core->WriteFormattedLine (T_("core"),
				  T_("operating system: %s"),
				  Utils::GetOSVersionString().c_str());
  
  trace_core->WriteFormattedLine (T_("core"),
				  T_("program file: %s"),
				  Q_(GetMyProgramFile()));

  trace_config->WriteFormattedLine (T_("core"),
				    T_("session locale: %s"),
				    defaultLocale.name().c_str());
  
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
      trace_core->WriteFormattedLine (T_("core"),
				      T_("uninitializing core library"));
      CheckOpenFiles ();
      WritePackageHistory ();
      size_t n = scratchDirectoryStack.size();
      if (n > 0)
	{
	  trace_error->WriteFormattedLine
	    (T_("core"),
	     T_("there are %u scratch directories in use"),
	     static_cast<unsigned>(n));
	  while (! scratchDirectoryStack.empty())
	    {
	      scratchDirectoryStack.pop ();
	    }
	}
      workingDirectories.clear ();
      UnregisterLibraryTraceStreams ();
    }
  catch (const exception &)
    {
#if defined(MIKTEX_WINDOWS)
      if ((initInfo.GetFlags() & InitFlags::InitializeCOM) != 0)
	{
	  CoUninitialize ();
	}
#endif
      throw;
    }
#if defined(MIKTEX_WINDOWS)
  if ((initInfo.GetFlags() & InitFlags::InitializeCOM) != 0)
    {
      CoUninitialize ();
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
  const MIKTEXCHAR * MSG_CANNOT_START_SERVER =
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
	      FATAL_MIKTEX_ERROR (T_("ConnectToServer"),
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
	  bo.dwClassContext	= CLSCTX_LOCAL_SERVER;
	  HRESULT hr =
	    CoGetObject(monikerName.c_str(),
			&bo,
			__uuidof(MiKTeXSessionLib::ISession),
			reinterpret_cast<void**>(&localServer.pSession));
	  if (FAILED(hr))
	    {
	      FATAL_MIKTEX_ERROR (T_("ConnectToServer"),
				  MSG_CANNOT_START_SERVER,
				  NUMTOSTR(hr));
	    }
	}
      else
	{
	  HRESULT hr =
	    localServer.pSession.CoCreateInstance
	    (__uuidof(MiKTeXSessionLib::MAKE_CURVER_ID(MiKTeXSession)),
	     0,
	     CLSCTX_LOCAL_SERVER);
	  if (FAILED(hr))
	    {
	      FATAL_MIKTEX_ERROR (T_("ConnectToServer"),
				  MSG_CANNOT_START_SERVER,
				  NUMTOSTR(hr));
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
     && IsSharedMiKTeXSetup() == TriState::True
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
      FATAL_MIKTEX_ERROR (T_("Session::Get"),
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
      UNEXPECTED_CONDITION (T_("Session::Release"));
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

MIKTEXAPI(int)
miktex_get_miktex_version_string_ex (/*[out]*/ MIKTEXCHAR *	lpszVersion,
				     /*[in]*/ size_t		bufSize)
{
  C_FUNC_BEGIN ();
  tstring version = Utils::GetMiKTeXVersionString();
  Utils::CopyString (lpszVersion, bufSize, version.c_str());
  return (1);
  C_FUNC_END ();
}
