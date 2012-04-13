/* texmfroot.cpp: managing TEXMF root directories

   Copyright (C) 1996-2012 Christian Schenk

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

#include "fnamedb.h"
#include "miktex/Core/Version"

// index of the hidden MPM root
#define MPM_ROOT static_cast<unsigned>(GetNumberOfTEXMFRoots())

/* _________________________________________________________________________

   Local Variables
   _________________________________________________________________________ */

namespace {
  MIKTEX_DEFINE_LOCK(fndb);
}

/* _________________________________________________________________________

   RootDirectory::~RootDirectory
   _________________________________________________________________________ */

RootDirectory::~RootDirectory ()
{
  try
    {
      SetFndb (0);
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   RootDirectory::SetFndb
   _________________________________________________________________________ */

void
RootDirectory::SetFndb (/*[in]*/ FileNameDatabase * pFndb)
{
  if (this->pFndb != 0)
    {
      this->pFndb->Release ();
    }
  this->pFndb = pFndb;
  if (pFndb != 0)
    {
      pFndb->AddRef ();
    }
}

/* _________________________________________________________________________

   SessionImpl::RegisterRootDirectory

   Register a TEXMF root.
   _________________________________________________________________________ */

static
string
ExpandEnvironmentVariables (/*[in]*/ const char *	lpszToBeExpanded)
{
  const char * lpsz = lpszToBeExpanded;
  string valueName;
  string expansion;
  expansion.reserve (strlen(lpsz));
  for (; *lpsz != 0; ++ lpsz)
  {
    if (lpsz[0] == '<')
    {
      const char * lpszBegin = lpsz;
      const char endChar = '>';
      valueName = "";
      for (lpsz += 1; *lpsz != 0 && *lpsz != endChar; ++ lpsz)
      {
	valueName += *lpsz;
      }
      if (*lpsz != endChar)
      {
	FATAL_MIKTEX_ERROR ("ExpandEnvironmentVariables", T_("Missing value name delimiter."), 0);
      }
      if (valueName.empty())
      {
	FATAL_MIKTEX_ERROR ("ExpandEnvironmentVariables", T_("Missing value name."), 0);
      }
      string value;
      if (! Utils::GetEnvironmentString(valueName.c_str(), value))
      {
	FATAL_MIKTEX_ERROR ("ExpandEnvironmentVariables", T_("Environment variable not defined."), valueName.c_str());
      }
      expansion += value;
    }
    else
    {
      expansion += lpsz[0];
    }
  }
  return (expansion);
}

unsigned
SessionImpl::RegisterRootDirectory (/*[in]*/ const PathName & root,
				    /*[in]*/ bool	      common)
{
  unsigned idx;
  for (idx = 0; idx < rootDirectories.size(); ++ idx)
    {
      if (root == rootDirectories[idx].get_UnexpandedPath())
	{
	  // already registered
	  if (common && ! rootDirectories[idx].IsCommon())
	  {
	    trace_config->WriteFormattedLine ("core",
	      T_("now a common TEXMF root: %s"),
				    root.Get());
	    rootDirectories[idx].set_Common (common);
	  }
	  return (idx);
	}
    }
  trace_config->WriteFormattedLine ("core",
				    T_("registering %s TEXMF root: %s"),
				    common ? "common" : "user",
				    root.Get());
  RootDirectory rootDirectory (root, ExpandEnvironmentVariables(root.Get()));
  rootDirectory.set_Common (common);
  rootDirectories.reserve (10);
  rootDirectories.push_back (rootDirectory);
  return (idx);
}

/* _________________________________________________________________________

   MergeStartupConfig
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(void)
MergeStartupConfig (/*[in,out]*/ StartupConfig &	startupConfig,
		    /*[in]*/ const StartupConfig &	defaults)
{
  if (startupConfig.config == MiKTeXConfiguration::None)
  {
    startupConfig.config = defaults.config;
  }
  if (startupConfig.commonRoots.empty())
    {
      startupConfig.commonRoots = defaults.commonRoots;
    }
  if (startupConfig.userRoots.empty())
    {
      startupConfig.userRoots = defaults.userRoots;
    }
  if (startupConfig.commonInstallRoot.Empty())
    {
      startupConfig.commonInstallRoot = defaults.commonInstallRoot;
    }
  if (startupConfig.userInstallRoot.Empty())
    {
      startupConfig.userInstallRoot = defaults.userInstallRoot;
    }
  if (startupConfig.commonDataRoot.Empty())
    {
      startupConfig.commonDataRoot = defaults.commonDataRoot;
    }
  if (startupConfig.userDataRoot.Empty())
    {
      startupConfig.userDataRoot = defaults.userDataRoot;
    }
  if (startupConfig.commonConfigRoot.Empty())
    {
      startupConfig.commonConfigRoot = defaults.commonConfigRoot;
    }
  if (startupConfig.userConfigRoot.Empty())
    {
      startupConfig.userConfigRoot = defaults.userConfigRoot;
    }
}

/* _________________________________________________________________________

   SessionImpl::DoStartupConfig
   _________________________________________________________________________ */

void
SessionImpl::DoStartupConfig ()
{
  // evaluate init info
  MergeStartupConfig (startupConfig, initInfo.GetStartupConfig());

  // read common environment variables
  MergeStartupConfig (startupConfig, ReadEnvironment(true));

  // read user environment variables
  MergeStartupConfig (startupConfig, ReadEnvironment(false));

  PathName commonStartupConfigFile;

  bool haveCommonStartupConfigFile =
    FindStartupConfigFile(true, commonStartupConfigFile);

  PathName commonPrefix;

  if (haveCommonStartupConfigFile)
  {
    PathName dir (commonStartupConfigFile);
    dir.RemoveFileSpec ();
    Utils::GetPathNamePrefix (dir, MIKTEX_PATH_MIKTEX_CONFIG_DIR, commonPrefix);
  }

  PathName userStartupConfigFile;

  bool haveUserStartupConfigFile =
    FindStartupConfigFile(false, userStartupConfigFile);

  PathName userPrefix;

  if (haveUserStartupConfigFile)
  {
    PathName dir (userStartupConfigFile);
    dir.RemoveFileSpec ();
    Utils::GetPathNamePrefix (dir, MIKTEX_PATH_MIKTEX_CONFIG_DIR, userPrefix);
  }

  // read common startup config file
  if (haveCommonStartupConfigFile)
  {
    MergeStartupConfig (startupConfig,
      ReadStartupConfigFile(true, commonStartupConfigFile));
  }

  // read user startup config file
  if (haveUserStartupConfigFile)
  {
    MergeStartupConfig (startupConfig,
      ReadStartupConfigFile(false, userStartupConfigFile));
  }

#if ! NO_REGISTRY
  if (startupConfig.config != MiKTeXConfiguration::Portable)
  {
    // read the registry, if we don't have a startup config file
    if (! haveCommonStartupConfigFile)
    {
      MergeStartupConfig (startupConfig, ReadRegistry(true));
    }
    if (! haveUserStartupConfigFile)
    {
      MergeStartupConfig (startupConfig, ReadRegistry(false));
    }
  }
#endif

  // merge in the default settings
  MergeStartupConfig (startupConfig,
    DefaultConfig(startupConfig.config, commonPrefix, userPrefix));
}

/* _________________________________________________________________________

   SessionImpl::InitializeRootDirectories

   Initialize the root directory list.
   _________________________________________________________________________ */

void
SessionImpl::InitializeRootDirectories ()
{
  InitializeRootDirectories (startupConfig);
}

/* _________________________________________________________________________

   SessionImpl::InitializeRootDirectories

   Initialize the root directory list.
   _________________________________________________________________________ */

void
SessionImpl::InitializeRootDirectories
(/*[in]*/ const StartupConfig & startupConfig)
{
  rootDirectories.clear ();

  commonInstallRootIndex = INVALID_ROOT_INDEX;
  userInstallRootIndex = INVALID_ROOT_INDEX;
  commonDataRootIndex = INVALID_ROOT_INDEX;
  userDataRootIndex = INVALID_ROOT_INDEX;
  commonConfigRootIndex = INVALID_ROOT_INDEX;
  userConfigRootIndex = INVALID_ROOT_INDEX;

  // UserConfig
  if (! startupConfig.userConfigRoot.Empty())
  {
    userConfigRootIndex =
      RegisterRootDirectory(startupConfig.userConfigRoot, false);
  }

  // UserData
  if (! startupConfig.userDataRoot.Empty())
  {
    userDataRootIndex =
      RegisterRootDirectory(startupConfig.userDataRoot, false);
  }

  // UserRoots
  for (CSVList root (startupConfig.userRoots.c_str(), PATH_DELIMITER);
       root.GetCurrent() != 0;
       ++ root)
  {
    if (*root.GetCurrent() != 0)
    {
      RegisterRootDirectory(root.GetCurrent(), false);
    }
  }

  // UserInstall
  if (! startupConfig.userInstallRoot.Empty())
  {
    userInstallRootIndex =
      RegisterRootDirectory(startupConfig.userInstallRoot, false);
  }

  // CommonConfig
  if (! startupConfig.commonConfigRoot.Empty())
  {
    commonConfigRootIndex =
      RegisterRootDirectory(startupConfig.commonConfigRoot, true);
  }

  // CommonData
  if (! startupConfig.commonDataRoot.Empty())
  {
    commonDataRootIndex =
      RegisterRootDirectory(startupConfig.commonDataRoot, true);
  }

  // CommonRoots
  for (CSVList root (startupConfig.commonRoots.c_str(), PATH_DELIMITER);
       root.GetCurrent() != 0;
       ++ root)
  {
    if (*root.GetCurrent() != 0)
    {
      RegisterRootDirectory(root.GetCurrent(), true);
    }
  }

  // CommonInstall
  if (! startupConfig.commonInstallRoot.Empty())
  {
    commonInstallRootIndex =
      RegisterRootDirectory(startupConfig.commonInstallRoot, true);
  }

  if (rootDirectories.size() == 0)
  {
    UNEXPECTED_CONDITION ("SessionImpl::InitializeRootDirectories");
  }

  if (commonDataRootIndex == INVALID_ROOT_INDEX)
  {
    commonDataRootIndex = 0;
  }

  if (userDataRootIndex == INVALID_ROOT_INDEX)
  {
    userDataRootIndex = 0;
  }

  if (commonConfigRootIndex == INVALID_ROOT_INDEX)
  {
    commonConfigRootIndex = commonDataRootIndex;
  }

  if (userConfigRootIndex == INVALID_ROOT_INDEX)
  {
    userConfigRootIndex = userDataRootIndex;
  }

  if (commonInstallRootIndex == INVALID_ROOT_INDEX)
  {
    commonInstallRootIndex = commonConfigRootIndex;
  }

  if (userInstallRootIndex == INVALID_ROOT_INDEX)
  {
    userInstallRootIndex = userConfigRootIndex;
  }

  RegisterRootDirectory (MPM_ROOT_PATH, IsAdminMode());

  trace_config->WriteFormattedLine ("core",
    "UserData: %s",
    GetRootDirectory(userDataRootIndex).Get());

  trace_config->WriteFormattedLine ("core",
    "UserConfig: %s",
    GetRootDirectory(userConfigRootIndex).Get());

  trace_config->WriteFormattedLine ("core",
    "UserInstall: %s",
    GetRootDirectory(userInstallRootIndex).Get());

  trace_config->WriteFormattedLine ("core",
    "CommonData: %s",
    GetRootDirectory(commonDataRootIndex).Get());

  trace_config->WriteFormattedLine ("core",
    "CommonConfig: %s",
    GetRootDirectory(commonConfigRootIndex).Get());

  trace_config->WriteFormattedLine ("core",
    "CommonInstall: %s",
    GetRootDirectory(commonInstallRootIndex).Get());
}

/* _________________________________________________________________________

   SessionImpl::GetNumberOfTEXMFRoots

   Count the registered TEXMF root directories.
   _________________________________________________________________________ */

unsigned
SessionImpl::GetNumberOfTEXMFRoots ()
{
  unsigned n = static_cast<unsigned>(rootDirectories.size());

  MIKTEX_ASSERT (n > 1);

  if (n <= 1)
  {
    UNEXPECTED_CONDITION ("SessionImpl::GetNumberOfTEXMFRoots");
  }
      
  // the MPM root directory doesn't count
  return (n - 1);
}

/* _________________________________________________________________________

   SessionImpl::GetRootDirectory

   Get a TEXMF root directory by index.
   _________________________________________________________________________ */

PathName
SessionImpl::GetRootDirectory (/*[in]*/ unsigned	r)
{
  unsigned n = GetNumberOfTEXMFRoots();
  if (r == INVALID_ROOT_INDEX || r >= n)
    {
      INVALID_ARGUMENT ("SessionImpl::GetRootDirectory", NUMTOSTR(r));
    }
  return (rootDirectories[r].get_Path());
}

/* _________________________________________________________________________

   SessionImpl::IsCommonRootDirectory
   _________________________________________________________________________ */

bool
SessionImpl::IsCommonRootDirectory (/*[in]*/ unsigned	r)
{
  unsigned n = GetNumberOfTEXMFRoots();
  if (r == INVALID_ROOT_INDEX || r >= n)
    {
      INVALID_ARGUMENT ("SessionImpl::IsCommonRootDirectory", NUMTOSTR(r));
    }
  return (rootDirectories[r].IsCommon());
}


/* _________________________________________________________________________

   SessionImpl::GetMpmRoot
   _________________________________________________________________________ */

unsigned
SessionImpl::GetMpmRoot ()
{
  return (MPM_ROOT);
}

/* _________________________________________________________________________

   SessionImpl::GetInstallRoot
   _________________________________________________________________________ */

unsigned
SessionImpl::GetInstallRoot ()
{
  if (IsAdminMode())
    {
      return (GetCommonInstallRoot());
    }
  else
    {
      return (GetUserInstallRoot());
    }
}

/* _________________________________________________________________________

   SessionImpl::GetCommonInstallRoot

   Get the index of the common installation root.
   _________________________________________________________________________ */

unsigned
SessionImpl::GetCommonInstallRoot ()
{
  return (commonInstallRootIndex);
}

/* _________________________________________________________________________

   SessionImpl::GetUserInstallRoot

   Get the index of the user installation root.
   _________________________________________________________________________ */

unsigned
SessionImpl::GetUserInstallRoot ()
{
  return (userInstallRootIndex);
}

/* _________________________________________________________________________

   SessionImpl::GetDistRoot
   _________________________________________________________________________ */

unsigned
SessionImpl::GetDistRoot ()
{
  PathName myloc = GetMyLocation(true);
  RemoveDirectoryDelimiter (myloc.GetBuffer());
  PathName internalBindir (MIKTEX_PATH_INTERNAL_BIN_DIR);
  RemoveDirectoryDelimiter (internalBindir.GetBuffer());
  PathName prefix;
  if (Utils::GetPathNamePrefix(myloc, internalBindir, prefix))
  {
    return (TryDeriveTEXMFRoot(prefix));
  }
  PathName bindir (MIKTEX_PATH_BIN_DIR);
  RemoveDirectoryDelimiter (bindir.GetBuffer());
  if (Utils::GetPathNamePrefix(myloc, bindir, prefix))
  {
    return (TryDeriveTEXMFRoot(prefix));
  }
  return (INVALID_ROOT_INDEX);
}

/* _________________________________________________________________________

   SessionImpl::SaveRootDirectories

   Save the root directory list.
   _________________________________________________________________________ */

void
SessionImpl::SaveRootDirectories (
#if defined(MIKTEX_WINDOWS)
				  /*[in]*/ bool noRegistry
#endif
				  )
{
#if ! defined(MIKTEX_WINDOWS)
  bool noRegistry = true;
#endif
  MIKTEX_ASSERT (! IsMiKTeXDirect());
  StartupConfig startupConfig;
  startupConfig.config =
    (IsMiKTeXPortable()
	? MiKTeXConfiguration::Portable
	: MiKTeXConfiguration::Regular);
  unsigned n = GetNumberOfTEXMFRoots();
  startupConfig.commonRoots.reserve (n * 30);
  startupConfig.userRoots.reserve (n * 30);
  for (unsigned idx = 0; idx < n; ++ idx)
    {
      const RootDirectory rootDirectory = this->rootDirectories[idx];
      if (rootDirectory.IsCommon())
      {
	if (idx == commonDataRootIndex
	    || idx == commonConfigRootIndex
	    || idx == commonInstallRootIndex)
	{
	  // implicitly defined
	  continue;
	}
	if (! startupConfig.commonRoots.empty())
	{
	  startupConfig.commonRoots += PATH_DELIMITER;
	}
	startupConfig.commonRoots += rootDirectory.get_UnexpandedPath().Get();
      }
      else
      {
	if (idx == userDataRootIndex
	    || idx == userConfigRootIndex
	    || idx == userInstallRootIndex)
	{
	  // implicitly defined
	  continue;
	}
	if (! startupConfig.userRoots.empty())
	{
	  startupConfig.userRoots += PATH_DELIMITER;
	}
	startupConfig.userRoots += rootDirectory.get_UnexpandedPath().Get();
      }
    }
  if (commonInstallRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.commonInstallRoot =
	this->rootDirectories[commonInstallRootIndex].get_UnexpandedPath();
    }
  if (userInstallRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.userInstallRoot = this->rootDirectories[userInstallRootIndex].get_UnexpandedPath();
    }
  if (commonDataRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.commonDataRoot = this->rootDirectories[commonDataRootIndex].get_UnexpandedPath();
    }
  if (userDataRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.userDataRoot = this->rootDirectories[userDataRootIndex].get_UnexpandedPath();
    }
  if (commonConfigRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.commonConfigRoot = this->rootDirectories[commonConfigRootIndex].get_UnexpandedPath();
    }
  if (userConfigRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.userConfigRoot = this->rootDirectories[userConfigRootIndex].get_UnexpandedPath();
    }
  if (IsAdminMode())
  {
    PathName commonStartupConfigFile;
    bool haveCommonStartupConfigFile =
      FindStartupConfigFile(true, commonStartupConfigFile);
    if (haveCommonStartupConfigFile || noRegistry)
    {
      WriteStartupConfigFile (true, startupConfig);
    }
    else
    {
#if defined(MIKTEX_WINDOWS)
      WriteRegistry (true, startupConfig);
#else
      FATAL_MIKTEX_ERROR ("SessionImpl::SaveRootDirectories",
	T_("The startup configuration cannot be saved."),
	0);
#endif
    }
  }
  else
  {
    PathName userStartupConfigFile;
    bool haveUserStartupConfigFile =
      FindStartupConfigFile(false, userStartupConfigFile);
    if (haveUserStartupConfigFile || noRegistry)
    {
      WriteStartupConfigFile (false, startupConfig);
    }
    else
    {
#if defined(MIKTEX_WINDOWS)
      WriteRegistry (false, startupConfig);
#else
      FATAL_MIKTEX_ERROR ("SessionImpl::SaveRootDirectories",
	T_("The startup configuration cannot be saved."),
	0);
#endif
    }
  }
  time_t now = time(0);
  string nowStr = NUMTOSTR(now);
  if (IsAdminMode())
  {
    SetConfigValue (MIKTEX_REGKEY_CORE, MIKTEX_REGVAL_LAST_ADMIN_MAINTENANCE, nowStr.c_str());
  }
  else
  {
    SetConfigValue (MIKTEX_REGKEY_CORE, MIKTEX_REGVAL_LAST_USER_MAINTENANCE, nowStr.c_str());
  }
}

/* _______________________________________________________________________
   
   SessionImpl::RegisterRootDirectories

   Configure MiKTeX root directories.
   _______________________________________________________________________ */

void
SessionImpl::RegisterRootDirectories (/*[in]*/ const string &	roots)
{
#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
  if (UseLocalServer())
    {
      ConnectToServer ();
      HResult hr =
	localServer.pSession->RegisterRootDirectories(_bstr_t(roots.c_str()));
      if (hr.Failed())
	{
	  MiKTeXSessionLib::ErrorInfo errorInfo;
	  HResult hr2 = localServer.pSession->GetErrorInfo(&errorInfo);
	  if (hr2.Failed())
	    {
	      FATAL_MIKTEX_ERROR ("SessionImpl::RegisterRootDirectories",
				  T_("sessionsvc failed for some reason."),
				  hr.GetText());
	    }
	  AutoSysString a (errorInfo.message);
	  AutoSysString b (errorInfo.info);
	  AutoSysString c (errorInfo.sourceFile);
	  Session::FatalMiKTeXError
	    ("SessionImpl::RegisterRootDirectories",
	     CW2A(errorInfo.message),
	     CW2A(errorInfo.info),
	     CW2A(errorInfo.sourceFile),
	     errorInfo.sourceLine);
	}
      return;
    }
#endif

  StartupConfig startupConfig;
  if (IsAdminMode())
  {
    startupConfig.commonRoots = roots;
  }
  else
  {
    startupConfig.userRoots = roots;
  }
  unsigned flags = 0;
#if defined(MIKTEX_WINDOWS)
  if (GetConfigValue(MIKTEX_REGKEY_CORE, MIKTEX_REGVAL_NO_REGISTRY, NO_REGISTRY ? true : false))
  {
    flags |= RegisterRootDirectoriesFlags::NoRegistry;
  }
#endif
  RegisterRootDirectories (startupConfig, flags);
}

/* _______________________________________________________________________
   
   SessionImpl::RegisterRootDirectories

   Configure MiKTeX root directories.
   _______________________________________________________________________ */

void
SessionImpl::RegisterRootDirectories
(/*[in]*/ const StartupConfig &	startupConfig,
 /*[in]*/ unsigned		flags)
{
  if (IsMiKTeXDirect())
  {
    UNEXPECTED_CONDITION ("SessionImpl::RegisterRootDirectories");
  }

  // clear the search path cache
  ClearSearchVectors ();

  triMiKTeXDirect = TriState::Undetermined;

  StartupConfig startupConfig_ = startupConfig;

  if (startupConfig_.commonInstallRoot.Empty()
    && commonInstallRootIndex != INVALID_ROOT_INDEX)
  {
    startupConfig_.commonInstallRoot =
      GetRootDirectory(commonInstallRootIndex);
  }
  if (startupConfig_.commonDataRoot.Empty()
    && commonDataRootIndex != INVALID_ROOT_INDEX)
  {
    startupConfig_.commonDataRoot = GetRootDirectory(commonDataRootIndex);
  }
  if (startupConfig_.commonConfigRoot.Empty()
    && commonConfigRootIndex != INVALID_ROOT_INDEX)
  {
    startupConfig_.commonConfigRoot =
      GetRootDirectory(commonConfigRootIndex);
  }

  if (startupConfig_.userInstallRoot.Empty()
    && userInstallRootIndex != INVALID_ROOT_INDEX)
  {
    startupConfig_.userInstallRoot = GetRootDirectory(userInstallRootIndex);
  }

  if (startupConfig_.userDataRoot.Empty()
    && userDataRootIndex != INVALID_ROOT_INDEX)
  {
    startupConfig_.userDataRoot = GetRootDirectory(userDataRootIndex);
  }

  if (startupConfig_.userConfigRoot.Empty()
    && userConfigRootIndex != INVALID_ROOT_INDEX)
  {
    startupConfig_.userConfigRoot =
      GetRootDirectory(userConfigRootIndex);
  }

  MergeStartupConfig (startupConfig_, DefaultConfig());

  InitializeRootDirectories (startupConfig_);

  if ((flags & RegisterRootDirectoriesFlags::Temporary) == 0)
  {
    // save the information
#if defined(MIKTEX_WINDOWS)
    SaveRootDirectories ((flags & RegisterRootDirectoriesFlags::NoRegistry) != 0);
#else
    SaveRootDirectories ();
#endif
  }
}

/* _________________________________________________________________________

   SessionImpl::GetDataRoot
   _________________________________________________________________________ */

unsigned
SessionImpl::GetDataRoot ()
{
  if (IsAdminMode())
    {
      return (GetCommonDataRoot());
    }
  else
    {
      return (GetUserDataRoot());
    }
}

/* _________________________________________________________________________

   SessionImpl::GetCommonDataRoot
   _________________________________________________________________________ */

unsigned
SessionImpl::GetCommonDataRoot ()
{
  return (commonDataRootIndex);
}

/* _________________________________________________________________________

   SessionImpl::GetUserDataRoot
   _________________________________________________________________________ */

unsigned
SessionImpl::GetUserDataRoot ()
{
  return (userDataRootIndex);
}

/* _________________________________________________________________________

   SessionImpl::GetConfigRoot
   _________________________________________________________________________ */

unsigned
SessionImpl::GetConfigRoot ()
{
  if (IsAdminMode())
    {
      return (GetCommonConfigRoot());
    }
  else
    {
      return (GetUserConfigRoot());
    }
}

/* _________________________________________________________________________

   SessionImpl::GetCommonConfigRoot
   _________________________________________________________________________ */

unsigned
SessionImpl::GetCommonConfigRoot ()
{
  return (commonConfigRootIndex);
}

/* _________________________________________________________________________

   SessionImpl::GetUserConfigRoot
   _________________________________________________________________________ */

unsigned
SessionImpl::GetUserConfigRoot ()
{
  return (userConfigRootIndex);
}

/* _________________________________________________________________________

   SessionImpl::IsTeXMFReadOnly

   Check to see whether a TEXMF root is read-only.
   _________________________________________________________________________ */

bool
SessionImpl::IsTeXMFReadOnly (/*[in]*/ unsigned r)
{
  return (
    ! IsMiKTeXPortable()
    && (IsMiKTeXDirect() && r == GetInstallRoot()
        || rootDirectories[r].IsCommon() && ! IsAdminMode()));
}

/* _________________________________________________________________________

   SessionImpl::FindFilenameDatabase

   Find the file name database for a given TEXMF root.
   _________________________________________________________________________ */

bool
SessionImpl::FindFilenameDatabase (/*[in]*/ unsigned		r,
				   /*[out]*/ PathName &		path)
{
  if (! (r < GetNumberOfTEXMFRoots() || r == MPM_ROOT))
    {
      INVALID_ARGUMENT ("SessionImpl::FindFilenameDatabase", NUMTOSTR(r));
    }

  vector<PathName> fndbFiles = GetFilenameDatabasePathNames(r);

  for (vector<PathName>::const_iterator it = fndbFiles.begin();
       it != fndbFiles.end();
       ++ it)
    {
      if (File::Exists(*it))
	{
	  path = *it;
	  return (true);
	}
    }

  return (false);
}

/* _________________________________________________________________________

   SessionImpl::GetFilenameDatabasePathName

   Determine the prefered fndb path for a given TEXMF root.
   _________________________________________________________________________ */

PathName
SessionImpl::GetFilenameDatabasePathName (/*[in]*/ unsigned	r)
{
  return (GetFilenameDatabasePathNames(r)[0]);
}

/* _________________________________________________________________________

   SessionImpl::GetFilenameDatabasePathNames

   Determine the possible fndb paths for a given TEXMF root.
   _________________________________________________________________________ */

vector<PathName>
SessionImpl::GetFilenameDatabasePathNames (/*[in]*/ unsigned r)
{
  vector<PathName> result;

  if (! IsMiKTeXPortable())
  {
    // preferred pathname
    PathName path = rootDirectories[r].get_Path();
    if (rootDirectories[r].IsCommon())
    {
      path = GetSpecialPath(SpecialPath::CommonDataRoot);
    }
    else
    {
      path = GetSpecialPath(SpecialPath::UserDataRoot);
    }
    path += GetRelativeFilenameDatabasePathName(r);
    result.push_back (path);
  }

  PathName path;

  // alternative pathname
  if (r == MPM_ROOT)
    {
      // INSTALL\miktex\conig\mpm.fndb
      if (GetInstallRoot() == INVALID_ROOT_INDEX)
	{
	  UNEXPECTED_CONDITION ("SessionImpl::FindFilenameDatabase");
	}
      path.Set (rootDirectories[GetInstallRoot()].get_Path(),
		MIKTEX_PATH_MPM_FNDB);
    }
  else
    {
      // ROOT\miktex\conig\texmf.fndb
      path.Set (rootDirectories[r].get_Path(), MIKTEX_PATH_TEXMF_FNDB);
    }
  result.push_back (path);
  
  return (result);
}

/* _________________________________________________________________________

   SessionImpl::GetMpmDatabasePathName

   Get the path name of mpm.fndb.
   _________________________________________________________________________ */

PathName
SessionImpl::GetMpmDatabasePathName ()
{
  return (GetFilenameDatabasePathName(MPM_ROOT));
}

/* _________________________________________________________________________

   SessionImpl::GetMpmRootPath
   _________________________________________________________________________ */

PathName
SessionImpl::GetMpmRootPath ()
{
  return (MPM_ROOT_PATH);
}

/* _________________________________________________________________________

   SessionImpl::GetRelativeFilenameDatabasePathName
   _________________________________________________________________________ */

PathName
SessionImpl::GetRelativeFilenameDatabasePathName (/*[in]*/  unsigned	r)
{
  string fndbFileName = MIKTEX_PATH_FNDB_DIR;
  fndbFileName += PathName::DirectoryDelimiter;
  PathName root (rootDirectories[r].get_Path());
  root.Normalize ();
  MD5Builder md5Builder;
  md5Builder.Update (root.Get(), root.GetLength());
  md5Builder.Final ();
  fndbFileName += md5Builder.GetMD5().ToString();
  fndbFileName += MIKTEX_FNDB_FILE_SUFFIX;
  return (fndbFileName);
}

/* _________________________________________________________________________

   SessionImpl::GetFileNameDatabase

   Get the fndb handle for a TEXMF root.
   _________________________________________________________________________ */

FileNameDatabase *
SessionImpl::GetFileNameDatabase (/*[in]*/ unsigned r)
{
  return (GetFileNameDatabase(r, TriState::Undetermined));
}

/* _________________________________________________________________________

   SessionImpl::GetFileNameDatabase

   Get the fndb handle of a TEXMF root.
   _________________________________________________________________________ */

FileNameDatabase *
SessionImpl::GetFileNameDatabase (/*[in]*/ unsigned	r,
				  /*[in]*/ TriState	triReadOnly)
{
  if (r != MPM_ROOT && r >= GetNumberOfTEXMFRoots())
    {
      INVALID_ARGUMENT ("SessionImpl::GetFileNameDatabase", NUMTOSTR(r));
    }
  
  bool readOnly;

  if (triReadOnly == TriState::True)
    {
      readOnly = true;
    }
  else if (triReadOnly == TriState::False)
    {
      readOnly = false;
    }
  else			// triReadOnly == TriState::Undetermined
    {
      readOnly = IsTeXMFReadOnly(r);
    }
  
  MIKTEX_LOCK(fndb)
    {
      RootDirectory & root = rootDirectories[r];
      
      if (root.GetFndb() != 0)
	{
	  if (triReadOnly == TriState::False && root.GetFndb()->IsInvariable())
	    {
	      // we say we indend to modify the fndb; but the fndb is opened readonly
	      // => we have to reload the database
	      if (! UnloadFilenameDatabaseInternal(r, false))
		{
		  return (0);
		}
	    }
	  else
	    {
	      root.GetFndb()->AddRef ();
	      return (root.GetFndb());
	    }
	}

#if 0
      if (root.get_NoFndb())
	{
	  // don't try to load the file name database
	  return (0);
	}
#endif
      
      PathName fqFndbFileName;
      
      bool fndbFileExists = FindFilenameDatabase(r, fqFndbFileName);
      
      if (! fndbFileExists)
	{
	  TraceError (T_("there is no fndb file for %s"), Q_(root.get_Path()));
#if 0
	  root.set_NoFndb (true);
#endif
	  return (0);
	}
      
      trace_fndb->WriteFormattedLine ("core",
				      T_("loading fndb%s: %s"),
				      (readOnly
				       ? T_(" read-only")
				       : ""),
				      fqFndbFileName.Get());
      
      FileNameDatabase * pFndb =
	FileNameDatabase::Create(fqFndbFileName.Get(),
				 root.get_Path().Get(),
				 readOnly);

      root.SetFndb (pFndb);
      
      return (pFndb);
    }
  MIKTEX_UNLOCK();
}

/* _________________________________________________________________________

   SessionImpl::GetFileNameDatabase

   Get the fndb handle of a TEXMF root (by path).
   _________________________________________________________________________ */

FileNameDatabase *
SessionImpl::GetFileNameDatabase (/*[in]*/ const char *	lpszPath)
{
  unsigned root = TryDeriveTEXMFRoot(lpszPath);
  if (root == INVALID_ROOT_INDEX)
    {
      return (0);
    }
  return (GetFileNameDatabase(root, TriState::Undetermined));
}

/* _________________________________________________________________________

   SessionImpl::TryDeriveTEXMFRoot

   Return the TEXMF root for a given path.
   _________________________________________________________________________ */

unsigned
SessionImpl::TryDeriveTEXMFRoot (/*[in]*/ const PathName & path)
{
  if (! Utils::IsAbsolutePath(path.Get()))
    {
      INVALID_ARGUMENT ("SessionImpl::DeriveRootDirectory", path.Get());
    }
      
  if (IsMpmFile(path.Get()))
    {
      return (MPM_ROOT);
    }

  unsigned rootDirectoryIndex = INVALID_ROOT_INDEX;

  unsigned n = GetNumberOfTEXMFRoots();

  for (unsigned idx = 0; idx < n; ++ idx)
    {
      PathName pathRoot = GetRootDirectory(idx);
      size_t rootlen = pathRoot.GetLength();
      if (PathName::Compare(pathRoot, path, rootlen) == 0
	  && (pathRoot.EndsWithDirectoryDelimiter()
	      || path[rootlen] == 0
	      || IsDirectoryDelimiter(path[rootlen])))
	{
	  if (rootDirectoryIndex == INVALID_ROOT_INDEX)
	    {
	      rootDirectoryIndex = idx;
	    }
	  else if (GetRootDirectory(rootDirectoryIndex).GetLength() < rootlen)
	    {
	      rootDirectoryIndex = idx;
	    }
	}
    }
      
  return (rootDirectoryIndex);
}

/* _________________________________________________________________________

   SessionImpl::DeriveTEXMFRoot

   Return the TEXMF root for a given path.
   _________________________________________________________________________ */

unsigned
SessionImpl::DeriveTEXMFRoot (/*[in]*/ const PathName & path)
{
  unsigned root = TryDeriveTEXMFRoot(path);
  if (root == INVALID_ROOT_INDEX)
    {
      FATAL_MIKTEX_ERROR ("SessionImpl::DeriveTEXMFRoot",
			  T_("Not a TEXMF root directory."),
			  path.Get());
    }
  return (root);
}

/* _________________________________________________________________________

   SessionImpl::UnloadFilenameDatabaseInternal

   Unload/remove a file name database file.
   _________________________________________________________________________ */

bool
SessionImpl::UnloadFilenameDatabaseInternal (/*[in]*/ unsigned	r,
					     /*[in]*/ bool	remove)
{
  trace_fndb->WriteFormattedLine
    ("core",
     T_("going to unload file name database %u"),
     r);
  MIKTEX_LOCK(fndb)
    {
      if (rootDirectories[r].GetFndb() != 0)
	{
	  // check the reference count
	  if (rootDirectories[r].GetFndb()->GetRefCount() != 1)
	    {
	      trace_fndb->WriteFormattedLine ("core",
		     T_("cannot unload fndb #%u: still in use"),
		     r);
	      return (false);
	    }
      
	  // release the database file
	  rootDirectories[r].SetFndb (0);
	}

      if (remove && r < GetNumberOfTEXMFRoots())
	{
	  // remove the database file
	  PathName path = GetFilenameDatabasePathName(r);
	  if (File::Exists(path))
	    {
	      File::Delete (path, true);
	    }
	}
    }
  MIKTEX_UNLOCK();

  return (true);
}

/* _________________________________________________________________________

   SessionImpl::UnloadFilenameDatabase

   Unload the file name database.
   _________________________________________________________________________ */

bool
SessionImpl::UnloadFilenameDatabase ()
{
  bool done = true;

  for (unsigned r = 0; r < rootDirectories.size(); ++ r)
    {
      if (! UnloadFilenameDatabaseInternal(r, false))
	{
	  done = false;
	}
    }

  return (done);
}

/* _________________________________________________________________________

   SessionImpl::IsTEXMFFile

   Check to see whether a fully qualified path is a part of the MiKTeX
   system.
   _________________________________________________________________________ */

bool
SessionImpl::IsTEXMFFile (/*[in]*/ const char *	lpszPath,
			  /*[out]*/ char *	lpszRelPath,
			  /*[out]*/ unsigned *	pRootIndex)
{
  for (unsigned r = 0; r < GetNumberOfTEXMFRoots(); ++ r)
    {
      PathName pathRoot = GetRootDirectory(r);
      size_t cchRoot = pathRoot.GetLength();
      if (PathName::Compare(pathRoot, lpszPath, cchRoot) == 0
	  && (lpszPath[cchRoot] == 0
	      || IsDirectoryDelimiter(lpszPath[cchRoot])))
	{
	  if (lpszRelPath != 0)
	    {
	      const char * lpsz = &lpszPath[cchRoot];
	      if (IsDirectoryDelimiter(*lpsz))
		{
		  ++ lpsz;
		}
	      Utils::CopyString (lpszRelPath, BufferSizes::MaxPath, lpsz);
	    }
	  if (pRootIndex != 0)
	    {
	      *pRootIndex = r;
	    }
	  return (true);
	}
    }
  return (false);
}

/* _________________________________________________________________________

   SessionImpl::SplitTEXMFPath

   Split a TEXMF path into its components: a fully qualified path to
   the root directory + a relative path to a sub-directory.
   _________________________________________________________________________ */

unsigned
SessionImpl::SplitTEXMFPath (/*[in]*/  const PathName &		path,
			     /*[in]*/  PathName &		root,
			     /*[out]*/ PathName &		relative)
{
  for (unsigned r = 0; r < GetNumberOfTEXMFRoots(); ++ r)
    {
      PathName rootDir = GetRootDirectory(r);
      size_t rootDirLen = rootDir.GetLength();
      if (PathName::Compare(rootDir, path, rootDirLen) == 0
	  && (path[rootDirLen] == 0
	      || IsDirectoryDelimiter(path[rootDirLen])))
	{
	  CopyString2 (root.GetBuffer(),
		       BufferSizes::MaxPath,
		       rootDir.Get(),
		       rootDirLen);
	  const char * lpsz = &path[0] + rootDirLen;
	  if (IsDirectoryDelimiter(*lpsz))
	    {
	      ++ lpsz;
	    }
	  relative = lpsz;
	  return (r);
	}
    }

  return (INVALID_ROOT_INDEX);
}

/* _________________________________________________________________________

   SessionImpl::IsManagedRoot
   _________________________________________________________________________ */

bool
SessionImpl::IsManagedRoot (/*[in]*/ unsigned root)
{
  return (root == GetUserInstallRoot()
	  || root == GetUserConfigRoot()
	  || root == GetUserDataRoot()
	  || root == GetCommonInstallRoot()
	  || root == GetCommonConfigRoot()
	  || root == GetCommonDataRoot());
}

/* _________________________________________________________________________

   IsMpmFile

   Check to see if a path starts with "\\MiKTeX\[MPM]\"
   _________________________________________________________________________ */

bool
SessionImpl::IsMpmFile (/*[in]*/ const char * lpszPath)
{
  return ((PathName::Compare(MPM_ROOT_PATH,
			     lpszPath,
			     static_cast<unsigned long>(MPM_ROOT_PATH_LEN))
	   == 0)
	  && (lpszPath[MPM_ROOT_PATH_LEN] == 0
	      || IsDirectoryDelimiter(lpszPath[MPM_ROOT_PATH_LEN])));
}

/* _________________________________________________________________________

   Utils::IsMiKTeXDirectRoot
   _________________________________________________________________________ */

bool
Utils::IsMiKTeXDirectRoot (/*[in]*/ const PathName &	root)
{
  PathName path (root);
  path += MIKTEXDIRECT_PREFIX_DIR;
  path += MIKTEX_PATH_STARTUP_CONFIG_FILE;
  if (! File::Exists(path))
  {
    return (false);
  }
  FileAttributes attributes = File::GetAttributes(path);
  if (((attributes & FileAttributes::ReadOnly) != 0) == 0)
  {
    return (false);
  }
  SmartPointer<Cfg> pcfg (Cfg::Create());      
  pcfg->Read (path);
  string str;
  return (pcfg->TryGetValue("Auto", "Config", str) && str == "Direct");
}

/* _________________________________________________________________________

   miktex_get_number_of_texmf_roots
   _________________________________________________________________________ */

MIKTEXCEEAPI(unsigned)
miktex_get_number_of_texmf_roots ()
{
  C_FUNC_BEGIN ();
  return (SessionImpl::GetSession()->GetNumberOfTEXMFRoots());
  C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_get_root_directory
   _________________________________________________________________________ */

MIKTEXCEEAPI(char *)
miktex_get_root_directory (/*[in]*/  unsigned		r,
			   /*[out]*/ char *	lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  Utils::CopyString (lpszPath,
		     BufferSizes::MaxPath,
		     SessionImpl::GetSession()->GetRootDirectory(r).Get());
  return (lpszPath);
  C_FUNC_END ();
}
