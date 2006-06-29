/* mpm.cpp: MiKTeX Package Manager

   Copyright (C) 2001-2006 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"

#include "internal.h"

#include "Extractor.h"
#include "TpmParser.h"

#if USE_WEB_SERVICE
#  include "../WebServiceClients/Repository/repositoryRepositorySoapProxy.h"
#  include "../WebServiceClients/Repository/RepositorySoap.nsmap"
#endif

#if ! USE_WEB_SERVICE
#  define PACKAGE_REPOSITORIES_CSV \
  T_("http://miktex.sourceforge.net/download/tm/package-repositories.csv")
#  define SF_LOGO \
  T_("http://sourceforge.net/sflogo.php?group_id=10783&type=1")
#endif

#include "mpm-version.h"

tstring PackageManagerImpl::proxyUser;
tstring PackageManagerImpl::proxyPassword;

/* _________________________________________________________________________

   FatalSoapError
   _________________________________________________________________________ */

#if USE_WEB_SERVICE
void
FatalSoapError (/*[in]*/ soap *			pSoap,
		/*[in]*/ const MIKTEXCHAR *	lpszFile,
		/*[in]*/ int			line)
{
  if (soap_check_state(pSoap))
    {
      UNEXPECTED_CONDITION (T_("FatalSoapError"));
    }
  else if (pSoap->error != SOAP_OK)
    {
      const MIKTEXCHAR ** pp = soap_faultstring(pSoap);
      Session::FatalMiKTeXError (0,
				 (pp != 0 ? *pp : 0),
				 0,
				 lpszFile,
				 line);
    }
  else
    {
      UNEXPECTED_CONDITION (T_("FatalSoapError"));
    }
}
#endif

/* _________________________________________________________________________

   PackageManager::~PackageManager
   _________________________________________________________________________ */

MPMCALL
PackageManager::~PackageManager ()
{
}

/* _________________________________________________________________________

   PackageManagerImpl::PackageManagerImpl
   _________________________________________________________________________ */

PackageManagerImpl::PackageManagerImpl ()
  : refCount (0),
    parsedAllPackageDefinitionFiles (false),
    trace_error (TraceStream::Open(MIKTEX_TRACE_ERROR)),
    trace_mpm (TraceStream::Open(MIKTEX_TRACE_MPM)),
    pSession (true),
    webSession (WebSession::Create())
{
  trace_mpm->WriteFormattedLine (T_("libmpm"),
				 T_("initializing MPM library version %s"),
				 VER_FILEVERSION_STR);
}

/* _________________________________________________________________________

   PackageManagerImpl::~PackageManagerImpl
   _________________________________________________________________________ */

MPMCALL
PackageManagerImpl::~PackageManagerImpl ()
{
  try
    {
      Dispose ();
    }
  catch (const MiKTeXException &)
    {
    }
}

/* _________________________________________________________________________

   PackageManager::Create
   _________________________________________________________________________ */

PackageManager *
MPMCALL
PackageManager::Create ()
{
  PackageManager * pManager = new PackageManagerImpl ();
  return (pManager);
}

/* _________________________________________________________________________

   PackageManagerImpl::AddRef
   _________________________________________________________________________ */

void
MPMCALL
PackageManagerImpl::AddRef ()
{
  ++ refCount;
}

/* _________________________________________________________________________

   PackageManagerImpl::Release
   _________________________________________________________________________ */

void
MPMCALL
PackageManagerImpl::Release ()
{
  -- refCount;
  if (refCount == 0)
    {
      Dispose ();
      delete this;
    }
}

/* _________________________________________________________________________

   PackageManagerImpl::Dispose
   _________________________________________________________________________ */

void
PackageManagerImpl::Dispose ()
{
  ClearAll ();
  if (webSession.get() != 0)
    {
      webSession->Dispose ();
      webSession.reset ();
    }
}

/* _________________________________________________________________________

   PackageManagerImpl::CreateInstaller
   _________________________________________________________________________ */

PackageInstaller *
MPMCALL
PackageManagerImpl::CreateInstaller ()
{
  return (new PackageInstallerImpl (this));
}

/* _________________________________________________________________________

   PackageManagerImpl::CreateIterator
   _________________________________________________________________________ */

PackageIterator *
MPMCALL
PackageManagerImpl::CreateIterator ()
{
  return (new PackageIteratorImpl (this));
}

/* _________________________________________________________________________

   PackageManagerImpl::LoadVariablePackageTable

   Read packages.ini.
   _________________________________________________________________________ */

void
PackageManagerImpl::LoadVariablePackageTable ()
{
  // only load once
  if (variablePackageTable.Get() != 0)
    {
      return;
    }

  variablePackageTable = Cfg::Create();

  PathName pathPackagesIni
    (pSession->GetSpecialPath(SpecialPath::InstallRoot),
     MIKTEX_PATH_PACKAGES_INI,
     0);

  if (! File::Exists(pathPackagesIni))
    {
      trace_mpm->WriteFormattedLine (T_("libmpm"),
				     T_("file %s does not (yet) exist"),
				     Q_(pathPackagesIni));
      variablePackageTable->SetModified (false);
      return;
    }

  trace_mpm->WriteFormattedLine (T_("libmpm"),
				 T_("reading %s"),
				 Q_(pathPackagesIni));

  variablePackageTable->Read (pathPackagesIni);
  variablePackageTable->SetModified (false);
}

/* _________________________________________________________________________

   PackageManagerImpl::FlushVariablePackageTable

   Write packages.ini.
   _________________________________________________________________________ */

void
PackageManagerImpl::FlushVariablePackageTable ()
{
  if (variablePackageTable.Get() == 0
      || ! variablePackageTable->IsModified())
    {
      return;
    }

  PathName pathPackagesIni
    (pSession->GetSpecialPath(SpecialPath::InstallRoot),
     MIKTEX_PATH_PACKAGES_INI,
     0);

  trace_mpm->WriteFormattedLine (T_("libmpm"),
				 T_("writing %s"),
				 Q_(pathPackagesIni));

  variablePackageTable->Write (pathPackagesIni);
}

/* _________________________________________________________________________

   PackageManagerImpl::GetTimeInstalled

   Returns the time when the package was installed.  Returns zero, if
   the package is not installed.
   _________________________________________________________________________ */

time_t
PackageManagerImpl::GetTimeInstalled
(/*[in]*/ const MIKTEXCHAR * lpszDeploymentName)
{
  LoadVariablePackageTable ();
  tstring str;
  if (! variablePackageTable->TryGetValue(lpszDeploymentName,
				       T_("TimeInstalled"),
				       str))
    {
      return (0);
    }
  return (AToI(str.c_str()));
}

/* _________________________________________________________________________

   PackageManagerImpl::IsPackageInstalled
   _________________________________________________________________________ */

bool
PackageManagerImpl::IsPackageInstalled
(/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName)
{
  return (GetTimeInstalled(lpszDeploymentName) > 0);
}

/* _________________________________________________________________________

   PackageManagerImpl::IsPackageObsolete
   _________________________________________________________________________ */

bool
PackageManagerImpl::IsPackageObsolete
(/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName)
{
  LoadVariablePackageTable ();
  tstring str;
  if (! variablePackageTable->TryGetValue(lpszDeploymentName,
				       T_("Obsolete"),
				       str))
    {
      return (false);
    }
  return (AToI(str.c_str()) != 0);
}

/* _________________________________________________________________________

   PackageManagerImpl::DeclarePackageObsolete
   _________________________________________________________________________ */

void
PackageManagerImpl::DeclarePackageObsolete
(/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName,
 /*[in]*/ bool			obsolete)
{
  LoadVariablePackageTable ();
  variablePackageTable->PutValue (lpszDeploymentName,
				  T_("Obsolete"),
				  (obsolete ? T_("1") : T_("0")));
}

/* _________________________________________________________________________

   PackageManagerImpl::SetTimeInstalled
   _________________________________________________________________________ */

void
PackageManagerImpl::SetTimeInstalled
(/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName,
 /*[in]*/ time_t		timeInstalled)
{
  LoadVariablePackageTable ();
  variablePackageTable->PutValue (lpszDeploymentName,
				  T_("TimeInstalled"),
				  NUMTOSTR(timeInstalled));
}

/* _________________________________________________________________________

   PackageManagerImpl::IncrementFileRefCounts
   _________________________________________________________________________ */

void
PackageManagerImpl::IncrementFileRefCounts
(/*[in]*/ const vector<tstring> & files)
{
  for ( vector<tstring>::const_iterator it = files.begin();
	it != files.end();
	++ it)
    {
      ++ installedFileInfoTable[*it].refCount;
#if POLLUTE_THE_DEBUG_STREAM
      if (installedFileInfoTable[*it].refCount >= 2)
	{
	  trace_mpm->WriteFormattedLine (T_("libmpm"),
					 T_("%s: ref count > 1"),
					 Q_(*it));
	}
#endif
    }
}

/* _________________________________________________________________________

   PackageManagerImpl::IncrementFileRefCounts
   _________________________________________________________________________ */

void
PackageManagerImpl::IncrementFileRefCounts
(/*[in]*/ const tstring &	deploymentName)
{
  NeedInstalledFileInfoTable ();
  const PackageInfo & pi = packageTable[deploymentName];
  IncrementFileRefCounts (pi.runFiles);
  IncrementFileRefCounts (pi.docFiles);
  IncrementFileRefCounts (pi.sourceFiles);
}

/* _________________________________________________________________________

   PackageManagerImpl::DefinePackage
   _________________________________________________________________________ */

PackageInfo *
PackageManagerImpl::DefinePackage
(/*[in]*/ const tstring &	deploymentName,
 /*[in]*/ const PackageInfo &	packageInfo)
{
  pair<PackageDefinitionTable::iterator, bool> p =
    packageTable.insert
    (make_pair<tstring, PackageInfo>(deploymentName, packageInfo));
  p.first->second.deploymentName = deploymentName;
  if (pSession->IsMiKTeXDirect())
    {
      // installed from the start
      p.first->second.timeInstalled = packageInfo.timePackaged;
    }
  else
    {
      p.first->second.timeInstalled = GetTimeInstalled(deploymentName.c_str());
    }
  return (&(p.first->second));
}

/* _________________________________________________________________________

   PackageManagerImpl::ParseAllPackageDefinitionFilesInDirectory

   Parse all package definition files in a directory.
   _________________________________________________________________________ */

void
PackageManagerImpl::ParseAllPackageDefinitionFilesInDirectory
(/*[in]*/ const PathName &	directory)
{
  trace_mpm->WriteFormattedLine (T_("libmpm"),
			 T_("searching %s for package definition files"),
				 Q_(directory));

  if (! Directory::Exists(directory))
    {
      trace_mpm->WriteFormattedLine (T_("libmpm"),
				     T_("directory %s does not exist"),
				     Q_(directory));
      return;
    }

  auto_ptr<DirectoryLister>
    pLister (DirectoryLister::Open(directory,
			   T_("*") MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX));

  // create parser object
  TpmParser tpmParser;

  // parse package definition files
  unsigned count = 0;
  DirectoryEntry direntry;
  while (pLister->GetNext(direntry))
    {
      PathName name (direntry.name.c_str());

      // get external package name
      MIKTEXCHAR szExternalName[BufferSizes::MaxPackageName];
      name.GetFileNameWithoutExtension (szExternalName);

      // ignore redefinition
      if (packageTable.find(szExternalName) != packageTable.end())
	{
	  trace_mpm->WriteFormattedLine (T_("libmpm"),
					 T_("%s: ignoring redefinition"),
					 szExternalName);
	  continue;
	}


      // parse package definition file
      tpmParser.Parse (PathName(directory, name, 0));

#if POLLUTE_THE_DEBUG_STREAM
      trace_mpm->WriteFormattedLine (T_("libmpm"),
				     T_("  adding %s"),
				     szExternalName);
#endif

      count += 1;
      
      // insert into database
      PackageInfo * pPi =
	DefinePackage(szExternalName, tpmParser.GetPackageInfo());

      // increment file ref counts, if package is installed
      if (pPi->timeInstalled > 0)
	{
	  IncrementFileRefCounts (pPi->runFiles);
	  IncrementFileRefCounts (pPi->docFiles);
	  IncrementFileRefCounts (pPi->sourceFiles);
	}
    }

  pLister->Close ();

  trace_mpm->WriteFormattedLine (T_("libmpm"),
				 T_("found %u package definition files"),
				 static_cast<unsigned>(count));
  
  // determine dependencies
  for (PackageDefinitionTable::iterator it = packageTable.begin();
       it != packageTable.end();
       ++ it)
    {
      time_t timeInstalledMin = static_cast<time_t>(0xffffffffffffffffULL);
      time_t timeInstalledMax = 0;
      for (vector<tstring>::const_iterator
	     it2 = it->second.requiredPackages.begin();
	   it2 != it->second.requiredPackages.end();
	   ++ it2)
	{
	  PackageDefinitionTable::iterator it3 = packageTable.find(*it2);
	  if (it3 == packageTable.end())
	    {
	      trace_mpm->WriteFormattedLine (T_("libmpm"),
				T_("dependancy problem: %s is required by %s"),
					     it2->c_str(),
					     it->second.deploymentName.c_str());
	    }
	  else
	    {
	      it3->second.requiredBy.push_back (it->second.deploymentName);
	      if (it3->second.timeInstalled < timeInstalledMin)
		{
		  timeInstalledMin = it3->second.timeInstalled;
		}
	      if (it3->second.timeInstalled > timeInstalledMax)
		{
		  timeInstalledMax = it3->second.timeInstalled;
		}
	    }
	}
      if (timeInstalledMin > 0)
	{
	  if (it->second.IsPureContainer()
	      || (it->second.IsInstalled()
		  && it->second.timeInstalled < timeInstalledMax))
	    {
	      it->second.timeInstalled = timeInstalledMax;
	    }
	}
    }

  PackageDefinitionTable::iterator it2;

  // create "Obsolete" container
  PackageInfo piObsolete;
  piObsolete.deploymentName = T_("_miktex-obsolete");
  piObsolete.displayName = T_("Obsolete");
  piObsolete.title = T_("Obsolete packages");
  piObsolete.description =
    T_("Packages that were removed from the package repository.");
  for (it2 = packageTable.begin(); it2 != packageTable.end(); ++ it2)
    {
      if (! it2->second.IsContained()
	  && ! it2->second.IsContainer()
	  && IsPackageObsolete(it2->second.deploymentName.c_str()))
	{
	  piObsolete.requiredPackages.push_back (it2->second.deploymentName);
	  it2->second.requiredBy.push_back (piObsolete.deploymentName);
	}
    }
  if (piObsolete.requiredPackages.size() > 0)
    {
      // insert "Obsolete" into the database
      DefinePackage (piObsolete.deploymentName, piObsolete);
    }

  // create "Other" container
  PackageInfo piOther;
  piOther.deploymentName = T_("_miktex-all-the-rest");
  piOther.displayName = T_("Uncategorized");
  piOther.title = T_("Uncategorized packages");
  for (it2 = packageTable.begin(); it2 != packageTable.end(); ++ it2)
    {
      if (! it2->second.IsContained() && ! it2->second.IsContainer())
	{
	  piOther.requiredPackages.push_back (it2->second.deploymentName);
	  it2->second.requiredBy.push_back (piOther.deploymentName);
	}
    }
  if (piOther.requiredPackages.size() > 0)
    {
      // insert "Other" into the database
      DefinePackage (piOther.deploymentName, piOther);
    }
}

/* _________________________________________________________________________
   
   PackageManagerImpl::ParseAllPackageDefinitionFiles

   Parse all package info files in the default package info directory.
   _________________________________________________________________________ */

void
PackageManagerImpl::ParseAllPackageDefinitionFiles ()
{
  if (parsedAllPackageDefinitionFiles)
    {
      return;
    }
  ParseAllPackageDefinitionFilesInDirectory
    (PathName(pSession->GetSpecialPath(SpecialPath::InstallRoot),
	      MIKTEX_PATH_PACKAGE_DEFINITION_DIR,
	      0));
  parsedAllPackageDefinitionFiles = true;
}

/* _________________________________________________________________________
   
   PackageManagerImpl::LoadDatabase
   _________________________________________________________________________ */

void
PackageManagerImpl::LoadDatabase (/*[in]*/ const PathName & path)
{
  // get the full path name
  PathName absPath (path);
  absPath.MakeAbsolute ();

  // check to see whether it is an archive file or a directory
  bool isDirectory = Directory::Exists(absPath);
  
  ScratchDirectory scratchDirectory;

  PathName pathPackageInfoDir;

  if (isDirectory)
    {
      pathPackageInfoDir = absPath;
    }
  else
    {
      absPath.SetExtension
	(ArchiveFileType::GetFileNameExtension(DB_ARCHIVE_FILE_TYPE));

      // enter the scratch directory
      scratchDirectory.Enter ();

      pathPackageInfoDir.SetToCurrentDirectory ();
      
      // unpack the package definition files
      auto_ptr<Extractor>
	pExtractor (Extractor::CreateExtractor(DB_ARCHIVE_FILE_TYPE));
      pExtractor->Extract (absPath,
			   pathPackageInfoDir,
			   false,
			   0,
			   0);
      pExtractor->Dispose ();
    }

  // read package definition files
  ParseAllPackageDefinitionFilesInDirectory (pathPackageInfoDir);

  parsedAllPackageDefinitionFiles = true;
}

/* _________________________________________________________________________

   PackageManagerImpl::ClearAll
   _________________________________________________________________________ */

void
PackageManagerImpl::ClearAll ()
{
  packageTable.clear ();
  installedFileInfoTable.clear ();
  if (variablePackageTable.Get() != 0)
    {
      variablePackageTable.Release ();
    }
  parsedAllPackageDefinitionFiles = false;
}

/* _________________________________________________________________________

   PackageManagerImpl::UnloadDatabase
   _________________________________________________________________________ */

void
PackageManagerImpl::UnloadDatabase ()
{
  ClearAll ();
}

/* _________________________________________________________________________

   PackageManagerImpl::TryGetPackageInfo
   _________________________________________________________________________ */

PackageInfo *
PackageManagerImpl::TryGetPackageInfo (/*[in]*/ const tstring & deploymentName)
{
  PackageDefinitionTable::iterator it = packageTable.find(deploymentName);
  if (it != packageTable.end())
    {
      return (&it->second);
    }
  if (parsedAllPackageDefinitionFiles)
    {
      return (0);
    }
  PathName
    pathPackageDefinitionFile
    (PathName(pSession->GetSpecialPath(SpecialPath::InstallRoot),
	      MIKTEX_PATH_PACKAGE_DEFINITION_DIR),
     deploymentName,
     MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX);
  if (! File::Exists(pathPackageDefinitionFile))
    {
      return (0);
    }
  TpmParser tpmParser;
  tpmParser.Parse (pathPackageDefinitionFile);
  return (DefinePackage(deploymentName, tpmParser.GetPackageInfo()));
}

/* _________________________________________________________________________

   PackageManagerImpl::TryGetPackageInfo
   _________________________________________________________________________ */

bool
MPMCALL
PackageManagerImpl::TryGetPackageInfo (/*[in]*/ const tstring & deploymentName,
				       /*[out]*/ PackageInfo &	packageInfo)
{
  PackageInfo * pPackageInfo = TryGetPackageInfo(deploymentName);
  if (pPackageInfo == 0)
    {
      return (false);
    }
  else
    {
      packageInfo = *pPackageInfo;
      return (true);
    }
}

/* _________________________________________________________________________

   PackageManagerImpl::::GetPackageInfo
   _________________________________________________________________________ */

PackageInfo
PackageManagerImpl::GetPackageInfo (/*[in]*/ const tstring & deploymentName)
{
  const PackageInfo * pPackageInfo = TryGetPackageInfo(deploymentName);
  if (pPackageInfo == 0)
    {
      FATAL_MPM_ERROR (T_("MiKTeX::Packages::GetPackageInfo"),
		       T_("The package is unknown."),
		       deploymentName.c_str());
    }
  return (*pPackageInfo);
}

/* _________________________________________________________________________

   PackageManagerImpl::GetFileRefCount
   _________________________________________________________________________ */

unsigned long
PackageManagerImpl::GetFileRefCount (/*[in]*/ const PathName & path)
{
  NeedInstalledFileInfoTable ();
  InstalledFileInfoTable::const_iterator it =
    installedFileInfoTable.find(path.Get());
  if (it == installedFileInfoTable.end())
    {
      return (0);
    }
  return (it->second.refCount);
}

/* _________________________________________________________________________

   PackageManagerImpl::NeedInstalledFileInfoTable
   _________________________________________________________________________ */

void
PackageManagerImpl::NeedInstalledFileInfoTable ()
{
  ParseAllPackageDefinitionFiles ();
}

/* _________________________________________________________________________

   PackageManager::TryGetRemotePackageRepository
   _________________________________________________________________________ */

bool
MPMCALL
PackageManager::TryGetRemotePackageRepository (/*[out]*/ tstring & url)
{
  return (SessionWrapper(true)
	  ->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			      MIKTEX_REGVAL_REMOTE_REPOSITORY,
			      url));
}

/* _________________________________________________________________________

   PackageManager::GetRemotePackageRepository
   _________________________________________________________________________ */

tstring
MPMCALL
PackageManager::GetRemotePackageRepository ()
{
  tstring url;
  if (! TryGetRemotePackageRepository(url))
    {
      FATAL_MIKTEX_ERROR (T_("PackageManager::GetRemotePackageRepository"),
			  T_("No remote package repository configured."),
			  0);
    }
  return (url);
}

/* _________________________________________________________________________

   IsUrl
   _________________________________________________________________________ */

MPMSTATICFUNC(bool)
IsUrl (/*[in]*/ const tstring & url)
{
  tstring::size_type pos = url.find(T_("://"));
  if (pos == tstring::npos)
    {
      return (false);
    }
  tstring scheme = url.substr(0, pos);
  for (tstring::const_iterator it = scheme.begin(); it != scheme.end(); ++ it)
    {
      if (! isalpha(*it, locale()))
	{
	  return (false);
	}
    }
  return (true);
}

/* _________________________________________________________________________

   PackageManagerImpl::DetermineRepositoryType
   _________________________________________________________________________ */

RepositoryType
PackageManagerImpl::DetermineRepositoryType
(/*[in]*/ const tstring & repository)
{
  if (IsUrl(repository))
    {
      return (RepositoryType::Remote);
    }

  if (! Utils::IsAbsolutePath(repository.c_str()))
    {
      FATAL_MPM_ERROR (T_("PackageManagerImpl::DetermineRepositoryType"),
		       T_("Invalid package repository."),
		       repository.c_str());
    }

  if (PackageManager::IsLocalPackageRepository(repository))
    {
      return (RepositoryType::Local);
    }

  if (Utils::IsMiKTeXDirectRoot(repository.c_str()))
    {
      return (RepositoryType::MiKTeXDirect);
    }

  FATAL_MPM_ERROR (T_("PackageManagerImpl::DetermineRepositoryType"),
		   T_("Not a valid installation source."),
		   repository.c_str());
}

/* _________________________________________________________________________

   PackageManager::SetRemotePackageRepository
   _________________________________________________________________________ */

void
MPMCALL
PackageManager::SetRemotePackageRepository (/*[in]*/ const tstring & url)
{
  SessionWrapper(true)
    ->SetUserConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
			  MIKTEX_REGVAL_REMOTE_REPOSITORY,
			  url.c_str());
}

/* _________________________________________________________________________

   PackageManager::TryGetLocalPackageRepository
   _________________________________________________________________________ */

bool
MPMCALL
PackageManager::TryGetLocalPackageRepository (/*[out]*/ PathName & path)
{
  tstring str;
  if (SessionWrapper(true)
      ->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			  MIKTEX_REGVAL_LOCAL_REPOSITORY,
			  str))
    {
      path = str;
      return (true);
    }
  else
    {
      return (false);
    }
}

/* _________________________________________________________________________

   PackageManager::GetLocalPackageRepository
   _________________________________________________________________________ */

PathName
MPMCALL
PackageManager::GetLocalPackageRepository ()
{
  PathName path;
  if (! TryGetLocalPackageRepository(path))
    {
      FATAL_MIKTEX_ERROR (T_("PackageManager::GetLocalPackageRepository"),
			  T_("No local package repository configured."),
			  0);
    }
  return (path);
}

/* _________________________________________________________________________

   PackageManager::SetLocalPackageRepository
   _________________________________________________________________________ */

void
MPMCALL
PackageManager::SetLocalPackageRepository (/*[in]*/ const PathName & path)
{
  SessionWrapper(true)
    ->SetUserConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
			  MIKTEX_REGVAL_LOCAL_REPOSITORY,
			  path.Get());
}

/* _________________________________________________________________________

   PackageManager::TryGetMiKTeXDirectRoot
   _________________________________________________________________________ */

bool
MPMCALL
PackageManager::TryGetMiKTeXDirectRoot (/*[out]*/ PathName & path)
{
  tstring str;
  if (SessionWrapper(true)
      ->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			  MIKTEX_REGVAL_MIKTEXDIRECT_ROOT,
			  str))
    {
      path = str;
      return (true);
    }
  else
    {
      return (false);
    }
}

/* _________________________________________________________________________

   PackageManager::GetMiKTeXDirectRoot
   _________________________________________________________________________ */

PathName
MPMCALL
PackageManager::GetMiKTeXDirectRoot ()
{
  PathName path;
  if (! TryGetLocalPackageRepository(path))
    {
      FATAL_MIKTEX_ERROR (T_("PackageManager::GetMiKTeXDirectRoot"),
			  T_("No MiKTeXDirect root configured."),
			  0);
    }
  return (path);
}

/* _________________________________________________________________________

   PackageManager::SetMiKTeXDirectRoot
   _________________________________________________________________________ */

void
MPMCALL
PackageManager::SetMiKTeXDirectRoot (/*[in]*/ const PathName & path)
{
  SessionWrapper(true)
    ->SetUserConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
			  MIKTEX_REGVAL_MIKTEXDIRECT_ROOT,
			  path.Get());
}

/* _________________________________________________________________________

   PackageManager::TryGetDefaultPackageRepository
   _________________________________________________________________________ */

bool
MPMCALL
PackageManager::TryGetDefaultPackageRepository
(/*[out]*/ RepositoryType &	repositoryType,
 /*[out]*/ tstring &		urlOrPath)
{
  if (Utils::GetEnvironmentString(MIKTEX_ENV_REPOSITORY, urlOrPath))
    {
      repositoryType = PackageManagerImpl::DetermineRepositoryType(urlOrPath);
    }
  else
    {
      tstring str;
      if (! (SessionWrapper(true)
	     ->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
				 MIKTEX_REGVAL_REPOSITORY_TYPE,
				 str)))
	{
	  return (false);
	}
      if (str == T_("remote"))
	{
	  urlOrPath = GetRemotePackageRepository();
	  repositoryType = RepositoryType::Remote;
	}
      else if (str == (T_("local")))
	{
	  urlOrPath = GetLocalPackageRepository().Get();
	  repositoryType = RepositoryType::Local;
	}
      else if (str == T_("direct"))
	{
	  urlOrPath = GetMiKTeXDirectRoot().Get();
	  repositoryType = RepositoryType::MiKTeXDirect;
	}
      else
	{
	  FATAL_MIKTEX_ERROR
	    (T_("PackageManager::TryGetDefaultPackageRepository"),
	     T_("Invalid registry settings."),
	     0);
	}
    }
  return (true);
}

/* _________________________________________________________________________

   PackageManager::GetDefaultPackageRepository
   _________________________________________________________________________ */

RepositoryType
MPMCALL
PackageManager::GetDefaultPackageRepository
(/*[out]*/ tstring &	urlOrPath)
{
  RepositoryType repositoryType (RepositoryType::Unknown);
  if (! TryGetDefaultPackageRepository(repositoryType, urlOrPath))
    {
      FATAL_MIKTEX_ERROR (T_("PackageManager::GetDefaultPackageRepository"),
			  T_("No package repository configured."),
			  0);
    }
  return (repositoryType);
}

/* _________________________________________________________________________

   PackageManager::SetDefaultPackageRepository
   _________________________________________________________________________ */

void
MPMCALL
PackageManager::SetDefaultPackageRepository
(/*[in]*/ RepositoryType	repositoryType,
 /*[in]*/ const tstring &	urlOrPath)
{
  if (repositoryType == RepositoryType::Unknown)
    {
      repositoryType = PackageManagerImpl::DetermineRepositoryType(urlOrPath);
    }
  tstring repositoryTypeStr;
  switch (repositoryType.Get())
    {
    case RepositoryType::MiKTeXDirect:
      repositoryTypeStr = T_("direct");
      SetMiKTeXDirectRoot (urlOrPath);
      break;
    case RepositoryType::Local:
      repositoryTypeStr = T_("local");
      SetLocalPackageRepository (urlOrPath);
      break;
    case RepositoryType::Remote:
      repositoryTypeStr = T_("remote");
      SetRemotePackageRepository (urlOrPath);
      break;
    default:
      UNEXPECTED_CONDITION (T_("PackageManager::SetDefaultPackageRepository"));
    }
  SessionWrapper(true)
    ->SetUserConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
			  MIKTEX_REGVAL_REPOSITORY_TYPE,
			  repositoryTypeStr.c_str());
}

/* _________________________________________________________________________

   ParseLine

   Parse a package repository definition.
   _________________________________________________________________________ */

#if ! USE_WEB_SERVICE
MPMSTATICFUNC(bool)
ParseLine (/*[in]*/ const tstring &	str,
	   /*[out]*/ RepositoryInfo &	repositoryInfo)
{
  CSVList tok (str.c_str(), T_(';'));
  if (tok.GetCurrent() == 0)
    {
      return (false);
    }
  repositoryInfo.packageLevel = PackageLevel::None;
  repositoryInfo.version = 0;
  repositoryInfo.timeDate = 0;
  repositoryInfo.url = tok.GetCurrent();
  ++ tok;
  if (tok.GetCurrent() == 0)
    {
      return (true);
    }
  repositoryInfo.description = tok.GetCurrent();
  ++ tok;
  if (tok.GetCurrent() == 0)
    {
      return (true);
    }
  repositoryInfo.country = tok.GetCurrent();
  ++ tok;
  if (tok.GetCurrent() == 0)
    {
      return (true);
    }
  repositoryInfo.town = tok.GetCurrent();
  ++ tok;
  if (tok.GetCurrent() == 0)
    {
      return (true);
    }
  repositoryInfo.packageLevel = DbLight::CharToPackageLevel(*tok.GetCurrent());
  ++ tok;
  if (tok.GetCurrent() == 0)
    {
      return (true);
    }
  repositoryInfo.timeDate = AToI(tok.GetCurrent());
  ++ tok;
  if (tok.GetCurrent() == 0)
    {
      return (true);
    }
  repositoryInfo.version = AToI(tok.GetCurrent());
  ++ tok;
  if (tok.GetCurrent() == 0)
    {
      return (true);
    }
  if (StringCompare(tok.GetCurrent(), T_("online")) == 0)
    {
      repositoryInfo.status = RepositoryStatus::Online;
    }
  else if (StringCompare(tok.GetCurrent(), T_("offline")) == 0)
    {
      repositoryInfo.status = RepositoryStatus::Offline;
    }
  else
    {
      ;
    }
  ++ tok;
  if (tok.GetCurrent() == 0)
    {
      return (true);
    }
  if (StringCompare(tok.GetCurrent(), T_("unknown")) == 0)
    {
      repositoryInfo.integrity = RepositoryIntegrity::Unknown;
    }
  else if (StringCompare(tok.GetCurrent(), T_("intact")) == 0)
    {
      repositoryInfo.integrity = RepositoryIntegrity::Intact;
    }
  else if (StringCompare(tok.GetCurrent(), T_("corrupted")) == 0)
    {
      repositoryInfo.integrity = RepositoryIntegrity::Corrupted;
    }
  else
    {
      ;
    }
  ++ tok;
  if (tok.GetCurrent() == 0)
    {
      return (true);
    }
  repositoryInfo.delay = AToI(tok.GetCurrent());
  return (true);
}
#endif

/* _________________________________________________________________________

   HiSourceForge
   _________________________________________________________________________ */

#if ! USE_WEB_SERVICE
MPMSTATICFUNC(void)
HiSourceForge (/*[in]*/ WebSession * webSession)
{
#if 1
  UNUSED_ALWAYS (webSession);
#else
  try
    {
      // open sflogo.php
      auto_ptr<WebFile> webFile (webSession->OpenUrl(SF_LOGO, this));
      
      const size_t BUFSIZE = 4096;
      char buffer[BUFSIZE];
      
      // download the data
      while (webFile->Read(buffer, BUFSIZE) > 0)
	{
	}
      
      webFile->Close ();
    }
  catch (const MiKTeXException &)
    {
    }
#endif
}
#endif

/* _________________________________________________________________________

   PackageManagerImpl::DownloadRepositoryListCSV
   _________________________________________________________________________ */

#if ! USE_WEB_SERVICE
void
PackageManagerImpl::DownloadRepositoryListCSV ()
{
  tstring url =
    (SessionWrapper(true)
     ->GetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
		      T_("MIKTEX_PKGREPLIST"),
		      PACKAGE_REPOSITORIES_CSV));

  auto_ptr<WebFile> webFile (webSession->OpenUrl(url.c_str(), this));

  // enter a scratch directory
  ScratchDirectory scratchDirectory;
  scratchDirectory.Enter ();

  // download the package repository list
  {
    FileStream stream (File::Open(T_("rep.txt"),
				  FileMode::Create,
				  FileAccess::Write,
				  false));
    const size_t BUFSIZE = 4096;
    char buffer[BUFSIZE];
    size_t n;
    while ((n = webFile->Read(buffer, BUFSIZE)) > 0)
      {
	stream.Write (buffer, n);
      }
    stream.Close ();
  }

  webFile->Close ();

  // read the list
  {
    FileStream stream (File::Open(T_("rep.txt"),
				  FileMode::Open,
				  FileAccess::Read,
				  false));
    tstring line;
    while (Utils::ReadUntilDelim(line, T_('\n'), stream.Get()))
      {
	RepositoryInfo repositoryInfo;
	ParseLine (line, repositoryInfo);
	repositories.push_back (repositoryInfo);
      }
    stream.Close ();
  }

  HiSourceForge (webSession.get());

  // clean-up
  scratchDirectory.Leave ();
}
#endif

/* _________________________________________________________________________

   MakeRepositoryInfo
   _________________________________________________________________________ */

#if USE_WEB_SERVICE
MPMSTATICFUNC(RepositoryInfo)
MakeRepositoryInfo (/*[in]*/ const mtrep__RepositoryInfo * pMwsRepositoryInfo)
{
  RepositoryInfo repositoryInfo;
  if (pMwsRepositoryInfo->Country != 0)
    {
      repositoryInfo.country = *pMwsRepositoryInfo->Country;
    }
  repositoryInfo.timeDate = pMwsRepositoryInfo->Date;
  repositoryInfo.delay = pMwsRepositoryInfo->Delay;
  if (pMwsRepositoryInfo->Description != 0)
    {
      repositoryInfo.description = *pMwsRepositoryInfo->Description;
    }
  switch (pMwsRepositoryInfo->Integrity)
    {
    case mtrep__Integrities__Corrupted:
      repositoryInfo.integrity = RepositoryIntegrity::Corrupted;
      break;
    case mtrep__Integrities__Intact:
      repositoryInfo.integrity = RepositoryIntegrity::Intact;
      break;
    case mtrep__Integrities__Unknown:
      repositoryInfo.integrity = RepositoryIntegrity::Unknown;
      break;
    }
  switch (pMwsRepositoryInfo->Level)
    {
    case mtrep__Levels__Essential:
      repositoryInfo.packageLevel = PackageLevel::Essential;
      break;
    case mtrep__Levels__Basic:
      repositoryInfo.packageLevel = PackageLevel::Basic;
      break;
    case mtrep__Levels__Advanced:
      repositoryInfo.packageLevel = PackageLevel::Advanced;
      break;
    case mtrep__Levels__Complete:
      repositoryInfo.packageLevel = PackageLevel::Complete;
      break;
    }
  switch (pMwsRepositoryInfo->Status)
    {
    case mtrep__RepositoryStatus__Online:
      repositoryInfo.status = RepositoryStatus::Online;
      break;
    case mtrep__RepositoryStatus__Offline:
      repositoryInfo.status = RepositoryStatus::Offline;
      break;
    case mtrep__RepositoryStatus__Unknown:
      repositoryInfo.status = RepositoryStatus::Unknown;
      break;
    }
  if (pMwsRepositoryInfo->Url != 0)
    {
      repositoryInfo.url = *pMwsRepositoryInfo->Url;
    }
  repositoryInfo.version = pMwsRepositoryInfo->Version;
  return (repositoryInfo);
}
#endif

/* _________________________________________________________________________

   PackageManagerImpl::DownloadRepositoryListWS
   _________________________________________________________________________ */

#if USE_WEB_SERVICE
void
PackageManagerImpl::DownloadRepositoryListWS ()
{
  RepositorySoapProxy repositorySoapProxy;
  ProxySettings proxySettings;
  if (TryGetProxy(proxySettings) && proxySettings.useProxy)
    {
      repositorySoapProxy.proxy_host = proxySettings.proxy.c_str();
      repositorySoapProxy.proxy_port = proxySettings.port;
      if (proxySettings.authenticationRequired)
	{
	  repositorySoapProxy.proxy_userid = proxySettings.user.c_str();
	  repositorySoapProxy.proxy_passwd = proxySettings.password.c_str();
	}
    }
  _mtrep__GetRepositories arg;
  arg.onlyOnline = true;
  arg.noCorrupted = true;
  arg.maxDelay = 10;
  _mtrep__GetRepositoriesResponse resp;
  if (repositorySoapProxy.GetRepositories(&arg, &resp) != SOAP_OK)
    {
      FATAL_SOAP_ERROR (&repositorySoapProxy);
    }
  for (vector<mtrep__RepositoryInfo*>::const_iterator it = 
	 resp.GetRepositoriesResult->RepositoryInfo.begin();
       it != resp.GetRepositoriesResult->RepositoryInfo.end();
       ++ it)
    {
      repositories.push_back (MakeRepositoryInfo(*it));
    }
}
#endif

/* _________________________________________________________________________

   PackageManagerImpl::DownloadRepositoryList
   _________________________________________________________________________ */

void
MPMCALL
PackageManagerImpl::DownloadRepositoryList ()
{
  repositories.clear ();

#if USE_WEB_SERVICE
  DownloadRepositoryListWS ();
#else
  DownloadRepositoryListCSV ();
#endif
}

/* _________________________________________________________________________

   PackageManagerImpl::PickRepositoryUrl
   _________________________________________________________________________ */

#if ! USE_WEB_SERVICE
class Rand
{
public:
  Rand ()
  {
    srand (static_cast<unsigned int>(time(0)));
  }
public:
  size_t
  operator() (/*[in]*/ size_t n)
    const
  {
    return (static_cast<size_t>(rand()) % n);
  }
};
#endif

tstring
MPMCALL
PackageManagerImpl::PickRepositoryUrl ()
{
#if USE_WEB_SERVICE
  RepositorySoapProxy repositorySoapProxy;
  ProxySettings proxySettings;
  if (TryGetProxy(proxySettings) && proxySettings.useProxy)
    {
      repositorySoapProxy.proxy_host = proxySettings.proxy.c_str();
      repositorySoapProxy.proxy_port = proxySettings.port;
      if (proxySettings.authenticationRequired)
	{
	  repositorySoapProxy.proxy_userid = proxySettings.user.c_str();
	  repositorySoapProxy.proxy_passwd = proxySettings.password.c_str();
	}
    }
  _mtrep__PickRepository arg;
  _mtrep__PickRepositoryResponse resp;
  if (repositorySoapProxy.PickRepository(&arg, &resp) != SOAP_OK)
    {
      FATAL_SOAP_ERROR (&repositorySoapProxy);
    }
  if (resp.PickRepositoryResult->Url == 0)
    {
      UNEXPECTED_CONDITION (T_("PackageManagerImpl::PickRepositoryUrl"));
    }
  return (*resp.PickRepositoryResult->Url);
#else
  if (repositories.size() == 0)
    {
      DownloadRepositoryList ();
    }
  if (repositories.size() == 0)
    {
      FATAL_MIKTEX_ERROR (T_("PackageManagerImpl::PickRepositoryUrl"),
			  T_("There are no package repositories."),
			  0);
    }
  unsigned bestVersion = 0;
  for (vector<RepositoryInfo>::const_iterator
	 it = repositories.begin();
       it != repositories.end();
       ++ it)
    {
      if (it->version > bestVersion)
	{
	  bestVersion = it->version;
	}
    }
  vector<RepositoryInfo> bestRepositories;
  for (vector<RepositoryInfo>::const_iterator
	 it = repositories.begin();
       it != repositories.end();
       ++ it)
    {
      if (it->version == bestVersion)
	{
	  bestRepositories.push_back (*it);
	}
    }
  Rand rand;
  random_shuffle (bestRepositories.begin(),
		  bestRepositories.end(),
		  rand);
  return (bestRepositories[0].url);
#endif
}

/* _________________________________________________________________________

   PackageManagerImpl::TraceError
   _________________________________________________________________________ */

void
PackageManagerImpl::TraceError (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
				/*[in]*/			...)
{
  va_list marker;
  va_start (marker, lpszFormat);
  trace_error->VTrace (T_("libmpm"), lpszFormat, marker);
  va_end (marker);
}

/* _________________________________________________________________________

   RememberFileNameInfo
   _________________________________________________________________________ */

namespace {

#if defined(USE_HASH_SET)
  typedef hash_set<tstring, hash_compare_path> SubDirectoryTable;
#else
  typedef set<tstring, hash_compare_path> SubDirectoryTable;
#endif
  
  struct DirectoryInfo
  {
    SubDirectoryTable subDirectoryNames;
    tstring fileNames;
    tstring packageNames;
  };

  
#if defined(USE_HASH_MAP)
  typedef
  hash_map<tstring, DirectoryInfo, hash_compare_path>
  DirectoryInfoTable;
#else
  typedef
  map<tstring, DirectoryInfo, hash_compare_path>
  DirectoryInfoTable;
#endif  

  // directory info table: written by
  // PackageManagerImpl::CreateMpmFndb(); read by
  // PackageManagerImpl::ReadDirectory()
  DirectoryInfoTable directoryInfoTable;

}

MPMSTATICFUNC(void)
RememberFileNameInfo (/*[in]*/ const tstring &	prefixedFileName,
		      /*[in]*/ const tstring &	packageName)
{
  tstring fileName;

  // ignore non-texmf files
  if (! PackageManager::StripTeXMFPrefix(prefixedFileName, fileName))
    {
      return;
    }

  PathNameParser pathtok (fileName.c_str());

  if (pathtok.GetCurrent() == 0)
    {
      return;
    }

  // initialize root path: "//MiKTeX/[MPM]"
  PathName path (MPM_ROOT_PATH);
  //  path += CURRENT_DIRECTORY;

  // lpsz1: current path name component
  const MIKTEXCHAR * lpsz1 = pathtok.GetCurrent();

  // lpszName: file name component
  const MIKTEXCHAR * lpszName = lpsz1;

  for (const MIKTEXCHAR * lpsz2 = ++ pathtok; lpsz2 != 0; lpsz2 = ++ pathtok)
    {
      directoryInfoTable[path.Get()].subDirectoryNames.insert (lpsz1);
      lpszName = lpsz2;
#if defined(MIKTEX_WINDOWS)
      // make sure the the rest of the path contains slashes (not
      // backslashes)
      path.AppendAltDirectoryDelimiter ();
#else
      path.AppendDirectoryDelimiter ();
#endif
      path += lpsz1;
      lpsz1 = lpsz2;
    }

  DirectoryInfo & directoryInfo = directoryInfoTable[path.Get()];
  directoryInfo.fileNames += lpszName;
  directoryInfo.fileNames += T_('\0');
  directoryInfo.packageNames += packageName;
  directoryInfo.packageNames += T_('\0');
}

/* _________________________________________________________________________

   PackageManagerImpl::ReadDirectory
   _________________________________________________________________________ */

bool
MIKTEXCALL
PackageManagerImpl::ReadDirectory (/*[in]*/ const MIKTEXCHAR *	lpszPath,
				   /*[out]*/ MIKTEXCHAR ** ppSubDirectoryNames,
				   /*[out]*/ MIKTEXCHAR ** ppFileNames,
				   /*[out]*/ MIKTEXCHAR ** ppFileNameInfos)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  // get the directory info for the given path
  DirectoryInfo & directoryInfo = directoryInfoTable[lpszPath];

  tstring subDirectoryNames;
  for (SubDirectoryTable::const_iterator
	 it = directoryInfo.subDirectoryNames.begin();
       it != directoryInfo.subDirectoryNames.end();
       ++ it)
    {
      subDirectoryNames += *it;
      subDirectoryNames += T_('\0');
    }
  subDirectoryNames += T_('\0');
  assert (ppSubDirectoryNames != 0);
  *ppSubDirectoryNames =
    reinterpret_cast<MIKTEXCHAR *>(malloc(subDirectoryNames.length()));
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
  subDirectoryNames._Copy_s (*ppSubDirectoryNames,
			     subDirectoryNames.length(),
			     subDirectoryNames.length());
#else
  subDirectoryNames.copy (*ppSubDirectoryNames, subDirectoryNames.length());
#endif
  assert (ppFileNames != 0);
  directoryInfo.fileNames += T_('\0');
  *ppFileNames =
    reinterpret_cast<MIKTEXCHAR *>(malloc(directoryInfo.fileNames.length()));
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
  directoryInfo.fileNames._Copy_s (*ppFileNames,
				   directoryInfo.fileNames.length(),
				   directoryInfo.fileNames.length());
#else
  directoryInfo.fileNames.copy (*ppFileNames,
				directoryInfo.fileNames.length());
#endif
  assert (ppFileNameInfos != 0);
  directoryInfo.packageNames += T_('\0');
  *ppFileNameInfos =
    reinterpret_cast<MIKTEXCHAR*>(malloc(directoryInfo.packageNames.length()));
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
  directoryInfo.packageNames._Copy_s (*ppFileNameInfos,
				      directoryInfo.packageNames.length(),
				      directoryInfo.packageNames.length());
#else
  directoryInfo.packageNames.copy (*ppFileNameInfos,
				   directoryInfo.packageNames.length());
#endif
  return (true);
}

/* _________________________________________________________________________

   PackageManagerImpl::OnProgress
   _________________________________________________________________________ */

bool
MIKTEXCALL
PackageManagerImpl::OnProgress (/*[in]*/ unsigned		level,
				/*[in]*/ const MIKTEXCHAR *	lpszDirectory)
{
  UNUSED_ALWAYS (level);
  UNUSED_ALWAYS (lpszDirectory);
  return (true);
}

/* _________________________________________________________________________

   PackageManagerImpl::CreateMpmFndb

   Create the MPM file name database (mpm.fndb).

   This function is not reentrant.
   _________________________________________________________________________ */

void
PackageManagerImpl::CreateMpmFndb ()
{
  ParseAllPackageDefinitionFiles ();

  // collect the file names
  for (PackageDefinitionTable::const_iterator it = packageTable.begin();
       it != packageTable.end();
       ++ it)
    {
      const PackageInfo & pi = it->second;
      vector<tstring>::const_iterator it2;
      for (it2 = pi.runFiles.begin();
	   it2 != pi.runFiles.end();
	   ++ it2)
	{
	  RememberFileNameInfo (*it2, pi.deploymentName);
	}
      for (it2 = pi.docFiles.begin();
	   it2 != pi.docFiles.end();
	   ++ it2)
	{
	  RememberFileNameInfo (*it2, pi.deploymentName);
	}
      for (it2 = pi.sourceFiles.begin();
	   it2 != pi.sourceFiles.end();
	   ++ it2)
	{
	  RememberFileNameInfo (*it2, pi.deploymentName);
	}
    }

  // create the database
  Fndb::Create (pSession->GetMpmDatabasePathName().Get(),
	        MPM_ROOT_PATH,
		this,
		true,
		true);

  // free memory
  directoryInfoTable.clear ();
}

/* _________________________________________________________________________

   PackageManagerImpl::GetAllPackageDefinitions
   _________________________________________________________________________ */

void
PackageManagerImpl::GetAllPackageDefinitions
(/*[out]*/ vector<PackageInfo> & packages)
{
  ParseAllPackageDefinitionFiles ();
  for (PackageDefinitionTable::const_iterator it = packageTable.begin();
       it != packageTable.end();
       ++ it)
    {
      packages.push_back (it->second);
    }
}

/* _________________________________________________________________________

   PackageManagerImpl::GetInstalledFileInfo
   _________________________________________________________________________ */

InstalledFileInfo *
PackageManagerImpl::GetInstalledFileInfo
(/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  ParseAllPackageDefinitionFiles ();
  InstalledFileInfoTable::iterator it = installedFileInfoTable.find(lpszPath);
  if (it == installedFileInfoTable.end())
    {
      return (0);
    }
  return (&it->second);
}

/* _________________________________________________________________________

   PackageManager::IsLocalPackageRepository
   _________________________________________________________________________ */

bool
MPMCALL
PackageManager::IsLocalPackageRepository (/*[in]*/ const PathName & path)
{
  if (! Directory::Exists(path))
    {
      return (false);
    }

  // local mirror of remote package repository?
  if (File::Exists(PathName(path, ZZDB1_FILE_NAME))
      && File::Exists(PathName(path, ZZDB2_FILE_NAME)))
    {
      return (true);
    }

  return (false);
}

/* _________________________________________________________________________

   PackageManager::ReadPackageDefinitionFile
   _________________________________________________________________________ */

PackageInfo
MPMCALL
PackageManager::ReadPackageDefinitionFile
(/*[in]*/ const PathName &	path,
 /*[in]*/ const MIKTEXCHAR *	lpszTeXMFPrefix)
{
  TpmParser tpmParser;
  tpmParser.Parse (path, lpszTeXMFPrefix);
  return (tpmParser.GetPackageInfo());
}

/* _________________________________________________________________________

   XmlWrite
   _________________________________________________________________________ */

class XmlWriter
{
public:
  XmlWriter (/*[in]*/ FILE * stream)
    : stream (stream),
      freshElement (false)
  {
    FPutS (T_("<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n"), stream);
  }

public:
  void
  StartElement (/*[in]*/ const MIKTEXCHAR *	lpszName)
  {
    if (freshElement)
      {
	FPutC (T_('>'), stream);
      }
    FPutC (T_('<'), stream);
    FPutS (lpszName, stream);
    freshElement = true;
    elements.push (lpszName);
  }

public:
  void
  AddAttribute (/*[in]*/ const MIKTEXCHAR *	lpszAttributeName,
		/*[in]*/ const MIKTEXCHAR *	lpszAttributeValue)
  {
    FPutC (T_(' '), stream);
    FPutS (lpszAttributeName, stream);
    FPutS (T_("=\""), stream);
    FPutS (lpszAttributeValue, stream);
    FPutC (T_('"'), stream);
  }

public:
  void
  EndElement ()
  {
    if (elements.empty())
      {
	FATAL_MIKTEX_ERROR (T_("XmlWriter::EndElement"),
			    T_("No elements on the stack."),
			    0);
      }
    if (freshElement)
      {
	FPutS (T_("/>"), stream);
	freshElement = false;
      }
    else
      {
	FPutS (T_("</"), stream);
	FPutS (elements.top().c_str(), stream);
	FPutC (T_('>'), stream);
      }
    elements.pop ();
  }

public:
  void
  EndAllElements ()
  {
    while (! elements.empty())
      {
	EndElement ();
      }
  }

public:
  void
  Text (/*[in]*/ const tstring & text )
  {
    if (freshElement)
      {
	FPutC (T_('>'), stream);
	freshElement = false;
      }
    for (const MIKTEXCHAR * lpszText = text.c_str();
	 *lpszText != 0;
	 ++ lpszText)
      {
	switch (*lpszText)
	  {
	  case T_('&'):
	    FPutS (T_("&amp;"), stream);
	    break;
	  case T_('<'):
	    FPutS (T_("&lt;"), stream);
	    break;
	  case T_('>'):
	    FPutS (T_("&gt;"), stream);
	    break;
	  default:
	    FPutC (*lpszText, stream);
	    break;
	  }
      }
  }

private:
  FILE * stream;

private:
  stack<tstring> elements;

private:
  bool freshElement;
};

/* _________________________________________________________________________

   PackageManager::WritePackageDefinitionFile
   _________________________________________________________________________ */

void
MPMCALL
PackageManager::WritePackageDefinitionFile
(/*[in]*/ const PathName &	path,
 /*[in]*/ const PackageInfo &	packageInfo,
 /*[in]*/ time_t		timePackaged)
{
  FileStream stream (File::Open(path,
				FileMode::Create,
				FileAccess::Write,
				false));

  XmlWriter xml (stream.Get());

  // create "rdf:Description" node
  xml.StartElement (T_("rdf:RDF"));
  xml.AddAttribute (T_("xmlns:rdf"),
		    T_("http://www.w3.org/1999/02/22-rdf-syntax-ns#"));
  xml.AddAttribute (T_("xmlns:TPM"),
		    T_("http://texlive.dante.de/"));
  xml.StartElement (T_("rdf:Description"));
  tstring about (T_("http://www.miktex.org/packages/"));
  about += packageInfo.deploymentName;
  xml.AddAttribute (T_("about"), about.c_str());

  // create "TPM:Name" node
  xml.StartElement (T_("TPM:Name"));
  xml.Text (packageInfo.displayName);
  xml.EndElement ();

  // create "TPM:Creator" node
  xml.StartElement (T_("TPM:Creator"));
  xml.Text (T_("mpc"));
  xml.EndElement ();
  
  // create "TPM:Title" node
  xml.StartElement (T_("TPM:Title"));
  xml.Text (packageInfo.title);
  xml.EndElement ();

  // create "TPM:Version" node
  xml.StartElement (T_("TPM:Version"));
  xml.Text (packageInfo.version);
  xml.EndElement ();

  // create "TPM:Description" node
  xml.StartElement (T_("TPM:Description"));
  xml.Text (packageInfo.description);
  xml.EndElement ();
  

  // create "TPM:RunFiles" node
  if (packageInfo.runFiles.size() > 0)
    {
      xml.StartElement (T_("TPM:RunFiles"));
      xml.AddAttribute
	(T_("size"),
	 NUMTOSTR(static_cast<unsigned>(packageInfo.sizeRunFiles)));
      for (vector<tstring>::const_iterator it = packageInfo.runFiles.begin();
	   it != packageInfo.runFiles.end();
	   ++ it)
	{
	  if (it != packageInfo.runFiles.begin())
	    {
	      xml.Text (T_(" "));
	    }
	  xml.Text (*it);
	}
      xml.EndElement ();
    }

  // create "TPM:DocFiles" node
  if (packageInfo.docFiles.size() > 0)
    {
      xml.StartElement (T_("TPM:DocFiles"));
      xml.AddAttribute
	(T_("size"),
	 NUMTOSTR(static_cast<unsigned>(packageInfo.sizeDocFiles)));
      for (vector<tstring>::const_iterator it = packageInfo.docFiles.begin();
	   it != packageInfo.docFiles.end();
	   ++ it)
	{
	  if (it != packageInfo.docFiles.begin())
	    {
	      xml.Text (T_(" "));
	    }
	  xml.Text (*it);
	}
      xml.EndElement ();
    }

  // create "TPM:SourceFiles" node
  if (packageInfo.sourceFiles.size())
    {
      xml.StartElement (T_("TPM:SourceFiles"));
      xml.AddAttribute
	(T_("size"),
	 NUMTOSTR(static_cast<unsigned>(packageInfo.sizeSourceFiles)));
      for (vector<tstring>::const_iterator it
	     = packageInfo.sourceFiles.begin();
	   it != packageInfo.sourceFiles.end();
	   ++ it)
	{
	  if (it != packageInfo.sourceFiles.begin())
	    {
	      xml.Text (T_(" "));
	    }
	  xml.Text (*it);
	}
      xml.EndElement ();
    }

  // create "TPM:Requires" node
  if (packageInfo.requiredPackages.size() > 0)
    {
      xml.StartElement (T_("TPM:Requires"));
      for (vector<tstring>::const_iterator it
	     = packageInfo.requiredPackages.begin();
	   it != packageInfo.requiredPackages.end();
	   ++ it)
	{
	  xml.StartElement (T_("TPM:Package"));
	  xml.AddAttribute (T_("name"), it->c_str());
	  xml.EndElement ();
	}
      xml.EndElement ();
    }

  // create "TPM:TimePackaged" node
  if (timePackaged != 0)
    {
      xml.StartElement (T_("TPM:TimePackaged"));
      xml.Text (NUMTOSTR(timePackaged));
      xml.EndElement ();
    }

  // create "TPM:MD5" node
  xml.StartElement (T_("TPM:MD5"));
  xml.Text (packageInfo.digest.ToString());

  xml.EndAllElements ();

  stream.Close ();
}

/* _________________________________________________________________________

   PackageManager::StripTeXMFPrefix
   _________________________________________________________________________ */

bool
MPMCALL
PackageManager::StripTeXMFPrefix (/*[in]*/ const tstring &	str,
				  /*[out]*/ tstring &		result)
{
  if (StripPrefix(str, TEXMF_PREFIX_DIRECTORY, result))
    {
      return (true);
    }
  PathName prefix2 (T_("."));
  prefix2 += TEXMF_PREFIX_DIRECTORY;
  return (StripPrefix(str, prefix2.Get(), result));
}

/* _________________________________________________________________________

   PackageManager::SetProxy
   _________________________________________________________________________ */

void
MPMCALL
PackageManager::SetProxy (/*[in]*/ const ProxySettings & proxySettings)
{
  SessionWrapper(true)
    ->SetUserConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
			  MIKTEX_REGVAL_USE_PROXY,
			  proxySettings.useProxy);
  SessionWrapper(true)
    ->SetUserConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
			  MIKTEX_REGVAL_PROXY_HOST,
			  proxySettings.proxy.c_str());
  SessionWrapper(true)
    ->SetUserConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
			  MIKTEX_REGVAL_PROXY_PORT,
			  proxySettings.port);
  SessionWrapper(true)
    ->SetUserConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
			  MIKTEX_REGVAL_PROXY_AUTH_REQ,
			  proxySettings.authenticationRequired);
  PackageManagerImpl::proxyUser = proxySettings.user;
  PackageManagerImpl::proxyPassword = proxySettings.password;
}

/* _________________________________________________________________________

   PackageManager::TryGetProxy
   _________________________________________________________________________ */

bool
MPMCALL
PackageManager::TryGetProxy (/*[out]*/ ProxySettings & proxySettings)
{
  tstring str;
  if (! (SessionWrapper(true)
	 ->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			     MIKTEX_REGVAL_USE_PROXY,
			     str)))
    {
      return (false);
    }
  proxySettings.useProxy = (str == T_("t"));
  if (! (SessionWrapper(true)
	 ->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			     MIKTEX_REGVAL_PROXY_HOST,
			     proxySettings.proxy)))
    {
      return (false);
    }
  if (! (SessionWrapper(true)
	 ->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			     MIKTEX_REGVAL_PROXY_PORT,
			     str)))
    {
      return (false);
    }
  proxySettings.port = AToI(str.c_str());
  if (! (SessionWrapper(true)
	 ->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			     MIKTEX_REGVAL_PROXY_AUTH_REQ,
			     str)))
    {
      return (false);
    }
  proxySettings.authenticationRequired = (str == T_("t"));
  proxySettings.user = PackageManagerImpl::proxyUser;
  proxySettings.password = PackageManagerImpl::proxyPassword;
  return (true);
}

/* _________________________________________________________________________

   PackageManager::GetProxy
   _________________________________________________________________________ */

ProxySettings
MPMCALL
PackageManager::GetProxy ()
{
  ProxySettings proxySettings;
  if (! TryGetProxy(proxySettings))
    {
      FATAL_MIKTEX_ERROR (T_("PackageManager::GetProxy"),
			  T_("No proxy configuration settings."),
			  0);
    }
  return (proxySettings);
}

/* _________________________________________________________________________

   PackageManagerImpl::OnProgress
   _________________________________________________________________________ */

void
PackageManagerImpl::OnProgress ()
{
}
