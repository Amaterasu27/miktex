/* texmfroot.cpp: managing TEXMF root directories

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

#include "miktex/env.h"

#include "fnamedb.h"
#include "miktex/version.h"

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
SessionImpl::RegisterRootDirectory (/*[in]*/ const PathName & root)
{
  unsigned idx;
  for (idx = 0; idx < rootDirectories.size(); ++ idx)
    {
      if (root == rootDirectories[idx].get_Path())
	{
	  return (idx);
	}
    }
  trace_config->WriteFormattedLine (T_("core"),
				    T_("registering TEXMF root: %s"),
				    root.Get());
  RootDirectory rootDirectory (root);
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
  if (startupConfig.roots.empty())
    {
      startupConfig.roots = defaults.roots;
    }
  if (startupConfig.installRoot.Empty())
    {
      startupConfig.installRoot = defaults.installRoot;
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
      startupConfig.installRoot = GetMyLocation();
#if defined(MIKTEX_WINDOWS_32)
      startupConfig.installRoot += PARENT_PARENT_DIRECTORY;
#else
      startupConfig.installRoot += PARENT_PARENT_PARENT_DIRECTORY;
#endif
      startupConfig.installRoot.MakeAbsolute ();
    }
  else
    {
      // evaluate init info
      MergeStartupConfig (startupConfig, initInfo.GetStartupConfig());
      
      // read environment variables
      MergeStartupConfig (startupConfig, ReadEnvironment());
      
      // read startup config file
      MergeStartupConfig (startupConfig, ReadStartupConfigFile());

#if 1
      if (haveStartupConfigFile)
	{
	  SharedMiKTeXSetup (false, true);
	}
#endif
      
#if defined(MIKTEX_WINDOWS)
      // read the registry, if we don't have a startup config file
      if (! haveStartupConfigFile)
	{
	  MergeStartupConfig (startupConfig, ReadRegistry());
	}
#endif
    }

  // merge in the default settings
  MergeStartupConfig (startupConfig,
		      DefaultConfig(IsSharedMiKTeXSetup() == TriState::True));

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

  installRootIndex = INVALID_ROOT_INDEX;
  commonDataRootIndex = INVALID_ROOT_INDEX;
  userDataRootIndex = INVALID_ROOT_INDEX;
  commonConfigRootIndex = INVALID_ROOT_INDEX;
  userConfigRootIndex = INVALID_ROOT_INDEX;

  // first pass through roots
  bool haveCommonConfigRoot = false;
  bool haveCommonDataRoot = false;
  bool haveInstallRoot = false;
  bool haveUserConfigRoot = false;
  bool haveUserDataRoot = false;
  vector<PathName> vec;
  CSVList root (startupConfig.roots.c_str(), PATH_DELIMITER);
  for (unsigned u = 0; root.GetCurrent() != 0; ++ u, ++ root)
    {
      if (StrLen(root.GetCurrent()) == 0)
	{
	  UNEXPECTED_CONDITION
	    (T_("SessionImpl::InitializeRootDirectories"));
	}
      if (find(vec.begin(), vec.end(), root.GetCurrent()) != vec.end())
	{
	  UNEXPECTED_CONDITION
	    (T_("SessionImpl::InitializeRootDirectories"));
	}
      if (startupConfig.commonConfigRoot == root.GetCurrent())
	{
	  if ((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0)
	    {
	      UNEXPECTED_CONDITION
		(T_("SessionImpl::InitializeRootDirectories"));
	    }
	  haveCommonConfigRoot = true;
	}
      if (startupConfig.commonDataRoot == root.GetCurrent())
	{
	  if ((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0)
	    {
	      UNEXPECTED_CONDITION
		(T_("SessionImpl::InitializeRootDirectories"));
	    }
	  haveCommonDataRoot = true;
	}
      if (startupConfig.installRoot == root.GetCurrent())
	{
	  haveInstallRoot = true;
	}
      if (startupConfig.userConfigRoot == root.GetCurrent())
	{
	  if ((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0)
	    {
	      UNEXPECTED_CONDITION
		(T_("SessionImpl::InitializeRootDirectories"));
	    }
	  haveUserConfigRoot = true;
	}
      if (startupConfig.userDataRoot == root.GetCurrent())
	{
	  if ((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0)
	    {
	      UNEXPECTED_CONDITION
		(T_("SessionImpl::InitializeRootDirectories"));
	    }
	  haveUserDataRoot = true;
	}
      vec.push_back (root.GetCurrent());
    }

  // register special roots
  if (((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0
       || ! haveUserConfigRoot)
      && ! startupConfig.userConfigRoot.Empty())
    {
      userConfigRootIndex =
	RegisterRootDirectory (startupConfig.userConfigRoot);
    }
  if (((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0
       || ! haveUserDataRoot)
      && ! startupConfig.userDataRoot.Empty())
    {
      userDataRootIndex =
	RegisterRootDirectory (startupConfig.userDataRoot);
    }
  if (((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0
       || ! haveCommonConfigRoot)
      && ! startupConfig.commonConfigRoot.Empty())
    {
      commonConfigRootIndex =
	RegisterRootDirectory (startupConfig.commonConfigRoot);
    }
  if (((GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0
       || ! haveCommonDataRoot)
      && ! startupConfig.commonDataRoot.Empty())
    {
      commonDataRootIndex =
	RegisterRootDirectory (startupConfig.commonDataRoot);
    }
  if (! haveInstallRoot
      && ! startupConfig.installRoot.Empty())
    {
      installRootIndex =
	RegisterRootDirectory (startupConfig.installRoot);
    }

  // second pass through roots
  for (vector<PathName>::const_iterator it = vec.begin();
       it != vec.end();
       ++ it)
    {
      unsigned idx = RegisterRootDirectory(*it);
      if (startupConfig.commonConfigRoot == *it)
	{
	  commonConfigRootIndex = idx;
	}
      if (startupConfig.commonDataRoot == *it)
	{
	  commonDataRootIndex = idx;
	}
      if (startupConfig.installRoot == *it)
	{
	  installRootIndex = idx;
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

  if (rootDirectories.size() == 0)
    {
      UNEXPECTED_CONDITION (T_("SessionImpl::InitializeRootDirectories"));
    }

  if (installRootIndex == INVALID_ROOT_INDEX)
    {
      installRootIndex = 0;
    }
  
  if (commonDataRootIndex == INVALID_ROOT_INDEX
      && IsSharedMiKTeXSetup() == TriState::True)
    {
      commonDataRootIndex = 0;
    }

  if (userDataRootIndex == INVALID_ROOT_INDEX)
    {
      userDataRootIndex = 0;
    }

  if (commonConfigRootIndex == INVALID_ROOT_INDEX
      && IsSharedMiKTeXSetup() == TriState::True)
    {
      commonConfigRootIndex = commonDataRootIndex;
    }

  if (userConfigRootIndex == INVALID_ROOT_INDEX)
    {
      userConfigRootIndex = userDataRootIndex;
    }

  RegisterRootDirectory (MPM_ROOT_PATH);

  trace_config->WriteFormattedLine (T_("core"),
				    T_("UserData: %s"),
				    GetRootDirectory(userDataRootIndex).Get());

  trace_config->WriteFormattedLine
    (T_("core"),
     T_("UserConfig: %s"),
     GetRootDirectory(userConfigRootIndex).Get());

  if (IsSharedMiKTeXSetup() == TriState::True)
    {
      trace_config->WriteFormattedLine
	(T_("core"),
	 T_("CommonData: %s"),
	 GetRootDirectory(commonDataRootIndex).Get());
      trace_config->WriteFormattedLine
	(T_("core"),
	 T_("CommonConfig: %s"),
	 GetRootDirectory(commonConfigRootIndex).Get());
    }

  trace_config->WriteFormattedLine (T_("core"),
				    T_("Install: %s"),
				    GetRootDirectory(installRootIndex).Get());
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
      INVALID_ARGUMENT (T_("SessionImpl::GetRootDirectory"), NUMTOSTR(r));
    }
  return (rootDirectories[r].get_Path());
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

   Get the index of the main (install) TEXMF root.
   _________________________________________________________________________ */

unsigned
SessionImpl::GetInstallRoot ()
{
  InitializeRootDirectories ();
  return (installRootIndex);
}

/* _________________________________________________________________________

   SessionImpl::SaveRootDirectories

   Save the root directory list somewhere.
   _________________________________________________________________________ */

void
SessionImpl::SaveRootDirectories ()
{
  MIKTEX_ASSERT (! IsMiKTeXDirect());
  StartupConfig startupConfig;
  unsigned n = GetNumberOfTEXMFRoots();
  startupConfig.roots.reserve (n * 30);
  for (unsigned idx = 0; idx < n; ++ idx)
    {
      if ((idx == commonDataRootIndex
	   || idx == userDataRootIndex
	   || idx == commonConfigRootIndex
	   || idx == userConfigRootIndex)
	  && (GetPolicyFlags() & PolicyFlags::DataRootHighestPriority) != 0)
	{
	  // data/config roots are implicitly defined
	  continue;
	}
      if (! startupConfig.roots.empty())
	{
	  startupConfig.roots += PATH_DELIMITER;
	}
      startupConfig.roots += this->rootDirectories[idx].get_Path().Get();
    }
  if (installRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.installRoot = GetRootDirectory(installRootIndex);
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
  if (haveStartupConfigFile)
    {
      WriteStartupConfigFile (startupConfig);
    }
  else
    {
#if defined(MIKTEX_WINDOWS)
      WriteRegistry (startupConfig);
#else
      FATAL_MIKTEX_ERROR (T_("SessionImpl::SaveRootDirectories"),
			  T_("The startup configuration cannot be saved."),
			  0);
#endif
    }
}

/* _______________________________________________________________________
   
   SessionImpl::RegisterRootDirectories

   Configure MiKTeX root directories.
   _______________________________________________________________________ */

void
SessionImpl::RegisterRootDirectories (/*[in]*/ const tstring &	roots)
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
	      FATAL_MIKTEX_ERROR (T_("SessionImpl::RegisterRootDirectories"),
				  T_("sessionsvc failed for some reason."),
				  NUMTOSTR(hr));
	    }
	  AutoSysString a (errorInfo.message);
	  AutoSysString b (errorInfo.info);
	  AutoSysString c (errorInfo.sourceFile);
	  Session::FatalMiKTeXError
	    (T_("SessionImpl::RegisterRootDirectories"),
	     CW2CT(errorInfo.message),
	     CW2CT(errorInfo.info),
	     CW2CT(errorInfo.sourceFile),
	     errorInfo.sourceLine);
	}
      return;
    }
#endif

  StartupConfig startupConfig;
  startupConfig.roots = roots;
  RegisterRootDirectories (startupConfig, false);
}

/* _______________________________________________________________________
   
   SessionImpl::RegisterRootDirectories

   Configure MiKTeX root directories.
   _______________________________________________________________________ */

void
SessionImpl::RegisterRootDirectories
(/*[in]*/ const StartupConfig &	startupConfig_,
 /*[in]*/ bool			sessionOnly)
{
  if (IsMiKTeXDirect())
    {
      UNEXPECTED_CONDITION (T_("SessionImpl::RegisterRootDirectories"));
    }

  // clear the search path cache
  ClearSearchVectors ();

  triMiKTeXDirect = TriState::Undetermined;

  StartupConfig startupConfig = startupConfig_;

  if (startupConfig.installRoot.Empty()
      && installRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.installRoot = GetRootDirectory(installRootIndex);
    }

  if (IsSharedMiKTeXSetup() == TriState::True)
    {
      if (startupConfig.commonDataRoot.Empty()
	  && commonDataRootIndex != INVALID_ROOT_INDEX)
	{
	  startupConfig.commonDataRoot = GetRootDirectory(commonDataRootIndex);
	}
      if (startupConfig.commonConfigRoot.Empty()
	  && commonConfigRootIndex != INVALID_ROOT_INDEX)
	{
	  startupConfig.commonConfigRoot =
	    GetRootDirectory(commonConfigRootIndex);
	}
    }
  else
    {
      MIKTEX_ASSERT (startupConfig.commonDataRoot.Empty());
      MIKTEX_ASSERT (startupConfig.commonConfigRoot.Empty());
    }

  if (startupConfig.userDataRoot.Empty()
      && userDataRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.userDataRoot = GetRootDirectory(userDataRootIndex);
    }

  if (startupConfig.userConfigRoot.Empty()
      && userConfigRootIndex != INVALID_ROOT_INDEX)
    {
      startupConfig.userConfigRoot =
	GetRootDirectory(userConfigRootIndex);
    }
  
  MergeStartupConfig (startupConfig,
		      DefaultConfig(IsSharedMiKTeXSetup() == TriState::True));

  InitializeRootDirectories (startupConfig);

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
  if (IsSharedMiKTeXSetup() == TriState::True)
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
  MIKTEX_ASSERT (IsSharedMiKTeXSetup() == TriState::True);
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
  if (IsSharedMiKTeXSetup() == TriState::True)
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
  MIKTEX_ASSERT (IsSharedMiKTeXSetup() == TriState::True);
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
  return (IsMiKTeXDirect() && r == GetInstallRoot());
}

/* _________________________________________________________________________

   SessionImpl::FindFilenameDatabase

   Find the file name database for a given TEXMF root.
   _________________________________________________________________________ */

bool
SessionImpl::FindFilenameDatabase (/*[in]*/ unsigned		r,
				   /*[out]*/ PathName &		path)
{
  if (r == MPM_ROOT)
    {
      path = GetMpmDatabasePathName();
    }
  else if (r >= GetNumberOfTEXMFRoots())
    {
      INVALID_ARGUMENT (T_("SessionImpl::FindFilenameDatabase"), NUMTOSTR(r));
    }
  else
    {
      path = GetFilenameDatabasePathName(r);
    }

  if (File::Exists(path))
    {
      return (true);
    }

  if (r == MPM_ROOT)
    {
      // try INSTALL\miktex\conig\mpm.fndb
      if (GetInstallRoot() == INVALID_ROOT_INDEX)
	{
	  UNEXPECTED_CONDITION (T_("SessionImpl::FindFilenameDatabase"));
	}
      path.Set (rootDirectories[GetInstallRoot()].get_Path(),
		MIKTEX_PATH_MPM_FNDB);
    }
  else
    {
      // try ROOT\miktex\conig\texmf.fndb
      path.Set (rootDirectories[r].get_Path(), MIKTEX_PATH_TEXMF_FNDB);
    }

  return (File::Exists(path));
}

/* _________________________________________________________________________

   SessionImpl::GetFilenameDatabasePathName

   Determine the fndb path for a given TEXMF root.
   _________________________________________________________________________ */

PathName
SessionImpl::GetFilenameDatabasePathName (/*[in]*/ unsigned	r)
{
  PathName path;
  if (IsSharedMiKTeXSetup() == TriState::True
      && r != userDataRootIndex
      && r != userConfigRootIndex)
    {
      path = GetSpecialPath(SpecialPath::CommonDataRoot);
    }
  else
    {
      path = GetSpecialPath(SpecialPath::UserDataRoot);
    }
  path += GetRelativeFilenameDatabasePathName(r);
  return (path);
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

   SessionImpl::GetRelativeFilenameDatabasePathName
   _________________________________________________________________________ */

PathName
SessionImpl::GetRelativeFilenameDatabasePathName (/*[in]*/  unsigned	r)
{
  tstring fndbFileName = MIKTEX_PATH_MIKTEX_CONFIG_DIR;
  fndbFileName += PathName::DirectoryDelimiter;
  PathName root (rootDirectories[r].get_Path());
  root.Normalize ();
  MD5Builder md5Builder;
#if defined(MIKTEX_UNICODE)
#  error Unimplemented: SessionImpl::GetRelativeFilenameDatabasePathName()
#endif
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
      INVALID_ARGUMENT (T_("SessionImpl::GetFileNameDatabase"), NUMTOSTR(r));
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
	      // we have to reload the database
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
      
      trace_fndb->WriteFormattedLine (T_("core"),
				      T_("loading fndb%s: %s"),
				      (readOnly
				       ? T_(" read-only")
				       : T_("")),
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
SessionImpl::GetFileNameDatabase (/*[in]*/ const MIKTEXCHAR *	lpszPath)
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
      INVALID_ARGUMENT (T_("SessionImpl::DeriveRootDirectory"), path.Get());
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
      FATAL_MIKTEX_ERROR (T_("SessionImpl::DeriveTEXMFRoot"),
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
    (T_("core"),
     T_("going to unload file name database %u"),
     r);
  MIKTEX_LOCK(fndb)
    {
      if (rootDirectories[r].GetFndb() != 0)
	{
	  // check the reference count
	  if (rootDirectories[r].GetFndb()->GetRefCount() != 1)
	    {
	      trace_fndb->WriteFormattedLine (T_("core"),
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
SessionImpl::IsTEXMFFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
			  /*[out]*/ MIKTEXCHAR *	lpszRelPath,
			  /*[out]*/ unsigned *		pRootIndex)
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
	      const MIKTEXCHAR * lpsz = &lpszPath[cchRoot];
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
	  const MIKTEXCHAR * lpsz = &path[0] + rootDirLen;
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
  return (root == GetInstallRoot()
	  || root == GetUserConfigRoot()
	  || root == GetUserDataRoot()
	  || (IsSharedMiKTeXSetup() == TriState::True
	      && (root == GetCommonConfigRoot()
		  || root == GetCommonDataRoot())));
}

/* _________________________________________________________________________

   IsMpmFile

   Check to see if a path starts with "\\MiKTeX\[MPM]\"
   _________________________________________________________________________ */

MIKTEXINTERNALFUNC(bool)
IsMpmFile (/*[in]*/ const MIKTEXCHAR * lpszPath)
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

MIKTEXAPI(bool)
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

MIKTEXAPI(unsigned)
miktex_get_number_of_texmf_roots ()
{
  C_FUNC_BEGIN ();
  return (SessionImpl::GetSession()->GetNumberOfTEXMFRoots());
  C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_get_root_directory
   _________________________________________________________________________ */

MIKTEXAPI(MIKTEXCHAR *)
miktex_get_root_directory (/*[in]*/  unsigned		r,
			   /*[out]*/ MIKTEXCHAR *	lpszPath)
{
  C_FUNC_BEGIN ();
  MIKTEX_ASSERT_PATH_BUFFER (lpszPath);
  Utils::CopyString (lpszPath,
		     BufferSizes::MaxPath,
		     SessionImpl::GetSession()->GetRootDirectory(r).Get());
  return (lpszPath);
  C_FUNC_END ();
}
