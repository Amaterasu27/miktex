/* texmfroot.cpp: managing TEXMF root directories

   Copyright (C) 1996-2009 Christian Schenk

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

unsigned
SessionImpl::RegisterRootDirectory (/*[in]*/ const PathName & root,
				    /*[in]*/ bool	      common)
{
  unsigned idx;
  for (idx = 0; idx < rootDirectories.size(); ++ idx)
    {
      if (root == rootDirectories[idx].get_Path())
	{
	  // already registered
	  return (idx);
	}
    }
  trace_config->WriteFormattedLine ("core",
				    T_("registering %s TEXMF root: %s"),
				    common ? "common" : "user",
				    root.Get());
  RootDirectory rootDirectory (root);
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

   SessionImpl::InitializeRootDirectories

   Initialize the root directory list.
   _________________________________________________________________________ */

void
SessionImpl::InitializeRootDirectories ()
{
  if (rootDirectories.size() > 0)
  {
    return;
  }

  StartupConfig startupConfig;

  // check for MiKTeX CD/DVD
  if (IsMiKTeXDirect())
  {
    PathName myloc = GetMyLocation(true);
    PathName prefix;
    if (! Utils::GetPathNamePrefix(myloc, MIKTEX_PATH_BIN_DIR, prefix))
    {
      UNEXPECTED_CONDITION ("SessionImpl::InitializeRootDirectories");
    }
    startupConfig.commonInstallRoot = prefix;
    startupConfig.userInstallRoot = startupConfig.commonInstallRoot;
  }
  else
  {
    // evaluate init info
    MergeStartupConfig (startupConfig, initInfo.GetStartupConfig());

    // read common environment variables
    MergeStartupConfig (startupConfig, ReadEnvironment(true));

    // read user environment variables
    MergeStartupConfig (startupConfig, ReadEnvironment(false));

    // read common startup config file
    MergeStartupConfig (startupConfig, ReadStartupConfigFile(true));

    // read user startup config file
    MergeStartupConfig (startupConfig, ReadStartupConfigFile(false));

#if ! NO_REGISTRY
    // read the registry, if we don't have a startup config file
    if (! haveCommonStartupConfigFile)
    {
      MergeStartupConfig (startupConfig, ReadRegistry(true));
    }
    if (! haveUserStartupConfigFile)
    {
      MergeStartupConfig (startupConfig, ReadRegistry(false));
    }
#endif
  }

  // merge in the default settings
  MergeStartupConfig (startupConfig, DefaultConfig());

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

  // first pass through roots
  bool haveCommonInstallRoot = false;
  bool haveCommonConfigRoot = false;
  bool haveCommonDataRoot = false;
  vector<PathName> vecCommon;
  for (CSVList root (startupConfig.commonRoots.c_str(), PATH_DELIMITER);
    root.GetCurrent() != 0;
    ++ root)
  {
    if (*root.GetCurrent() == 0)
    {
      // empty
      continue;
    }
    if (find(vecCommon.begin(), vecCommon.end(), root.GetCurrent()) != vecCommon.end())
    {
      // duplicate
      continue;
    }
    if (startupConfig.commonConfigRoot == root.GetCurrent())
    {
      if ((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0)
      {
	continue;
      }
      else
      {
	haveCommonConfigRoot = true;
      }
    }
    if (startupConfig.commonDataRoot == root.GetCurrent())
    {
      if ((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0)
      {
	continue;
      }
      else
      {
	haveCommonDataRoot = true;
      }
    }
    if (startupConfig.commonInstallRoot == root.GetCurrent())
    {
      haveCommonInstallRoot = true;
    }
    vecCommon.push_back (root.GetCurrent());
  }
  bool haveUserInstallRoot = false;
  bool haveUserConfigRoot = false;
  bool haveUserDataRoot = false;
  vector<PathName> vecUser;
  for (CSVList root (startupConfig.userRoots.c_str(), PATH_DELIMITER);
    root.GetCurrent() != 0;
    ++ root)
  {
    if (*root.GetCurrent() == 0)
    {
      // empty
      continue;
    }
    if (find(vecUser.begin(), vecUser.end(), root.GetCurrent()) != vecUser.end())
    {
      // duplicate
      continue;
    }
    if (startupConfig.userConfigRoot == root.GetCurrent())
    {
      if ((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0)
      {
	continue;
      }
      else
      {
	haveUserConfigRoot = true;
      }
    }
    if (startupConfig.userDataRoot == root.GetCurrent())
    {
      if ((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0)
      {
	continue;
      }
      else
      {
	haveUserDataRoot = true;
      }
    }
    if (startupConfig.userInstallRoot == root.GetCurrent())
    {
      haveUserInstallRoot = true;
    }
    vecUser.push_back (root.GetCurrent());
  }

  // register special roots:
  //   UserConfig
  //   UserData
  //   CommonConfig
  //   CommonData
  //   UserInstall
  //   CommonInstall
  if (((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0
    || ! haveUserConfigRoot)
    && ! startupConfig.userConfigRoot.Empty())
  {
    userConfigRootIndex =
      RegisterRootDirectory(startupConfig.userConfigRoot, false);
  }
  if (((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0
    || ! haveUserDataRoot)
    && ! startupConfig.userDataRoot.Empty())
  {
    userDataRootIndex =
      RegisterRootDirectory(startupConfig.userDataRoot, false);
  }
  if (((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0
    || ! haveCommonConfigRoot)
    && ! startupConfig.commonConfigRoot.Empty())
  {
    commonConfigRootIndex =
      RegisterRootDirectory(startupConfig.commonConfigRoot, true);
  }
  if (((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0
    || ! haveCommonDataRoot)
    && ! startupConfig.commonDataRoot.Empty())
  {
    commonDataRootIndex =
      RegisterRootDirectory(startupConfig.commonDataRoot, true);
  }
  if (! haveUserInstallRoot
    && ! startupConfig.userInstallRoot.Empty())
  {
    userInstallRootIndex =
      RegisterRootDirectory(startupConfig.userInstallRoot, false);
  }
  if (! haveCommonInstallRoot
    && ! startupConfig.commonInstallRoot.Empty())
  {
    commonInstallRootIndex =
      RegisterRootDirectory(startupConfig.commonInstallRoot, true);
  }

  // second pass through roots: remember special root indexes
  for (vector<PathName>::const_iterator it = vecUser.begin();
    it != vecUser.end();
    ++ it)
  {
    unsigned idx = RegisterRootDirectory(*it, false);
    if (startupConfig.userInstallRoot == *it)
    {
      userInstallRootIndex = idx;
    }
    if (startupConfig.userConfigRoot == *it)
    {
      userConfigRootIndex = idx;
    }
    if (startupConfig.userDataRoot == *it)
    {
      userDataRootIndex = idx;
    }
  }
  for (vector<PathName>::const_iterator it = vecCommon.begin();
    it != vecCommon.end();
    ++ it)
  {
    unsigned idx = RegisterRootDirectory(*it, true);
    if (startupConfig.commonConfigRoot == *it)
    {
      commonConfigRootIndex = idx;
    }
    if (startupConfig.commonDataRoot == *it)
    {
      commonDataRootIndex = idx;
    }
    if (startupConfig.commonInstallRoot == *it)
    {
      commonInstallRootIndex = idx;
    }
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
  InitializeRootDirectories ();
      
  unsigned n = static_cast<unsigned>(rootDirectories.size());
      
  // the MPM root directory doesn't count
  MIKTEX_ASSERT (n > 1);
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
  InitializeRootDirectories ();
  return (commonInstallRootIndex);
}

/* _________________________________________________________________________

   SessionImpl::GetUserInstallRoot

   Get the index of the user installation root.
   _________________________________________________________________________ */

unsigned
SessionImpl::GetUserInstallRoot ()
{
  InitializeRootDirectories ();
  return (userInstallRootIndex);
}

/* _________________________________________________________________________

   SessionImpl::SaveRootDirectories

   Save the root directory list.
   _________________________________________________________________________ */

void
SessionImpl::SaveRootDirectories ()
{
  MIKTEX_ASSERT (! IsMiKTeXDirect());
  StartupConfig startupConfig;
  unsigned n = GetNumberOfTEXMFRoots();
  startupConfig.commonRoots.reserve (n * 30);
  startupConfig.userRoots.reserve (n * 30);
  for (unsigned idx = 0; idx < n; ++ idx)
    {
      const RootDirectory rootDirectory = this->rootDirectories[idx];
      if (rootDirectory.IsCommon())
      {
	if ((idx == commonDataRootIndex
	  || idx == commonConfigRootIndex)
	  && (GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0)
	{
	  // data/config roots are implicitly defined
	  continue;
	}
	if (! startupConfig.commonRoots.empty())
	{
	  startupConfig.commonRoots += PATH_DELIMITER;
	}
	startupConfig.commonRoots += rootDirectory.get_Path().Get();
      }
      else
      {
	if ((idx == userDataRootIndex
	  || idx == userConfigRootIndex)
	  && (GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0)
	{
	  // data/config roots are implicitly defined
	  continue;
	}
	if (! startupConfig.userRoots.empty())
	{
	  startupConfig.userRoots += PATH_DELIMITER;
	}
	startupConfig.userRoots += rootDirectory.get_Path().Get();
      }
    }
  if (commonInstallRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.commonInstallRoot =
	GetRootDirectory(commonInstallRootIndex);
    }
  if (userInstallRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.userInstallRoot = GetRootDirectory(userInstallRootIndex);
    }
  if (commonDataRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.commonDataRoot = GetRootDirectory(commonDataRootIndex);
    }
  if (userDataRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.userDataRoot = GetRootDirectory(userDataRootIndex);
    }
  if (commonConfigRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.commonConfigRoot = GetRootDirectory(commonConfigRootIndex);
    }
  if (userConfigRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.userConfigRoot = GetRootDirectory(userConfigRootIndex);
    }
#if NO_REGISTRY
  // force creation of the startup configuration file
  if (IsAdminMode())
  {
    haveCommonStartupConfigFile = true;
  }
  else
  {
    haveUserStartupConfigFile = true;
  }
#endif
  if (IsAdminMode())
  {
    if (haveCommonStartupConfigFile)
    {
      WriteStartupConfigFile (true, startupConfig);
    }
    else
    {
#if ! NO_REGISTRY
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
    if (haveUserStartupConfigFile)
    {
      WriteStartupConfigFile (false, startupConfig);
    }
    else
    {
#if ! NO_REGISTRY
      WriteRegistry (false, startupConfig);
#else
      FATAL_MIKTEX_ERROR ("SessionImpl::SaveRootDirectories",
	T_("The startup configuration cannot be saved."),
	0);
#endif
    }
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
      HRESULT hr =
	localServer.pSession->RegisterRootDirectories(_bstr_t(roots.c_str()));
      if (FAILED(hr))
	{
	  MiKTeXSessionLib::ErrorInfo errorInfo;
	  HRESULT hr2 = localServer.pSession->GetErrorInfo(&errorInfo);
	  if (FAILED(hr2))
	    {
	      FATAL_MIKTEX_ERROR ("SessionImpl::RegisterRootDirectories",
				  T_("sessionsvc failed for some reason."),
				  NUMTOSTR(hr));
	    }
	  AutoSysString a (errorInfo.message);
	  AutoSysString b (errorInfo.info);
	  AutoSysString c (errorInfo.sourceFile);
	  Session::FatalMiKTeXError
	    ("SessionImpl::RegisterRootDirectories",
	     CW2CT(errorInfo.message),
	     CW2CT(errorInfo.info),
	     CW2CT(errorInfo.sourceFile),
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
  RegisterRootDirectories (startupConfig, false);
}

/* _______________________________________________________________________
   
   SessionImpl::RegisterRootDirectories

   Configure MiKTeX root directories.
   _______________________________________________________________________ */

void
SessionImpl::RegisterRootDirectories
(/*[in]*/ const StartupConfig &	startupConfig,
 /*[in]*/ bool			sessionOnly)
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

  if (! sessionOnly)
  {
    // save the information
    SaveRootDirectories ();
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
  InitializeRootDirectories ();
  return (commonDataRootIndex);
}

/* _________________________________________________________________________

   SessionImpl::GetUserDataRoot
   _________________________________________________________________________ */

unsigned
SessionImpl::GetUserDataRoot ()
{
  InitializeRootDirectories ();
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
  InitializeRootDirectories ();
  return (commonConfigRootIndex);
}

/* _________________________________________________________________________

   SessionImpl::GetUserConfigRoot
   _________________________________________________________________________ */

unsigned
SessionImpl::GetUserConfigRoot ()
{
  InitializeRootDirectories ();
  return (userConfigRootIndex);
}

/* _________________________________________________________________________

   SessionImpl::IsTeXMFReadOnly

   Check to see whether a TEXMF root is read-only.
   _________________________________________________________________________ */

bool
SessionImpl::IsTeXMFReadOnly (/*[in]*/ unsigned r)
{
  return (IsMiKTeXDirect() && r == GetInstallRoot()
    || rootDirectories[r].IsCommon() && ! IsAdminMode());
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
  string fndbFileName = MIKTEX_PATH_MIKTEX_CONFIG_DIR;
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
  InitializeRootDirectories ();
  
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
  path += MIKTEX_PATH_MD_INI;
  if (! File::Exists(path))
    {
      return (false);
    }
  FileAttributes attributes = File::GetAttributes(path);
  return ((attributes & FileAttributes::ReadOnly) != 0);
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
