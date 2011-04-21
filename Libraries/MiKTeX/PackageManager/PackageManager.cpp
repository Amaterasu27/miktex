/* PackageManager.cpp: MiKTeX Package Manager

   Copyright (C) 2001-2011 Christian Schenk

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

#include "TpmParser.h"

#include "../WebServiceClients/Repository/repositoryRepositorySoapProxy.h"
#include "../WebServiceClients/Repository/RepositorySoap.nsmap"

#define WEBSVCURL "http://api.miktex.org/Repository.asmx"

#include "mpm-version.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Extractor;
using namespace MiKTeX::Packages;
using namespace std;

string PackageManagerImpl::proxyUser;
string PackageManagerImpl::proxyPassword;

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
bool PackageManagerImpl::localServer = false;
#endif

class MyRepositorySoapProxy : public RepositorySoapProxy
{
public:
  MyRepositorySoapProxy ()
  {
    this->soap_endpoint = WEBSVCURL;
  }
};

/* _________________________________________________________________________

   ClientInfo
   _________________________________________________________________________ */

template<class Base>
struct ClientInfo : public Base
{
  ClientInfo ()
    : version_ (MIKTEX_COMPONENT_VERSION_STR)
  {
#if 1
    vector<string> invokers = Process2::GetInvokerNames();
    for (vector<string>::const_iterator it = invokers.begin(); it != invokers.end(); ++ it)
    {
      name_ += *it;
      name_ += "/";
    }
    name_ += Utils::GetExeName();
    name_ += "/";
#endif
    name_ += "MPM";
    Base::Name = &name_;
    Base::Version = &version_;
  };
private:
  string name_;
  string version_;
};

/* _________________________________________________________________________

   FatalSoapError
   _________________________________________________________________________ */

void
FatalSoapError (/*[in]*/ soap *		pSoap,
		/*[in]*/ const char *	lpszFile,
		/*[in]*/ int		line)
{
  if (soap_check_state(pSoap))
    {
      UNEXPECTED_CONDITION ("FatalSoapError");
    }
  else if (pSoap->error != SOAP_OK)
    {
      const char ** ppText = soap_faultstring(pSoap);
      const char ** ppDetail = soap_faultdetail(pSoap);
      switch (pSoap->error)
	{
	case 403:
	case 503:
	  FATAL_MIKTEX_ERROR (0,
			      T_("The MiKTeX web service has rejected your \
request. Possible reason: too many requests arriving from your IP \
address."),
			      (ppDetail != 0 ? *ppDetail : 0));
	default:
	  Session::FatalMiKTeXError (0,
				     (ppText != 0 ? *ppText : 0),
				     (ppDetail != 0 ? *ppDetail : 0),
				     lpszFile,
				     line);
	}
    }
  else
    {
      UNEXPECTED_CONDITION ("FatalSoapError");
    }
}

/* _________________________________________________________________________

   PackageManager::~PackageManager
   _________________________________________________________________________ */

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
    webSession (WebSession::Create(this))
{
  trace_mpm->WriteFormattedLine ("libmpm",
				 T_("initializing MPM library version %s"),
				 MIKTEX_COMPONENT_VERSION_STR);
}

/* _________________________________________________________________________

   PackageManagerImpl::~PackageManagerImpl
   _________________________________________________________________________ */

PackageManagerImpl::~PackageManagerImpl ()
{
  try
    {
      Dispose ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   PackageManager::Create
   _________________________________________________________________________ */

PackageManager *
PackageManager::Create ()
{
  PackageManager * pManager = new PackageManagerImpl ();
  return (pManager);
}

/* _________________________________________________________________________

   PackageManagerImpl::AddRef
   _________________________________________________________________________ */

void
PackageManagerImpl::AddRef ()
{
  ++ refCount;
}

/* _________________________________________________________________________

   PackageManagerImpl::Release
   _________________________________________________________________________ */

void
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
PackageManagerImpl::CreateInstaller ()
{
  return (new PackageInstallerImpl (this));
}

/* _________________________________________________________________________

   PackageManagerImpl::CreateIterator
   _________________________________________________________________________ */

PackageIterator *
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
  if (commonVariablePackageTable.Get() != 0)
    {
      return;
    }
 
  commonVariablePackageTable = Cfg::Create();

  PathName pathCommonPackagesIni
    (pSession->GetSpecialPath(SpecialPath::CommonInstallRoot),
     MIKTEX_PATH_PACKAGES_INI,
     0);
  
  if (File::Exists(pathCommonPackagesIni))
    {
      trace_mpm->WriteFormattedLine
	("libmpm",
	 T_("loading common variable package table (%s)"),
	 Q_(pathCommonPackagesIni));
      commonVariablePackageTable->Read (pathCommonPackagesIni);
    }

  commonVariablePackageTable->SetModified (false);

  PathName pathUserPackagesIni
    (pSession->GetSpecialPath(SpecialPath::UserInstallRoot),
     MIKTEX_PATH_PACKAGES_INI,
     0);

  if (! pSession->IsAdminMode()
      && (pathCommonPackagesIni.Canonicalize()
	  != pathUserPackagesIni.Canonicalize()))
  {
    userVariablePackageTable = Cfg::Create();
    if (File::Exists(pathUserPackagesIni))
    {	  
      trace_mpm->WriteFormattedLine
	("libmpm",
	 T_("loading user variable package table (%s)"),
	 Q_(pathUserPackagesIni));
      userVariablePackageTable->Read (pathUserPackagesIni);
    }
    userVariablePackageTable->SetModified (false);
  }
}

/* _________________________________________________________________________

   PackageManagerImpl::FlushVariablePackageTable

   Write packages.ini.
   _________________________________________________________________________ */

void
PackageManagerImpl::FlushVariablePackageTable ()
{
  if (commonVariablePackageTable.Get() != 0
      && commonVariablePackageTable->IsModified())
    {
      PathName pathPackagesIni
	(pSession->GetSpecialPath(SpecialPath::CommonInstallRoot),
	 MIKTEX_PATH_PACKAGES_INI,
	 0);
      trace_mpm->WriteFormattedLine
	("libmpm",
	 T_("flushing common variable package table (%s)"),
	 Q_(pathPackagesIni));
      commonVariablePackageTable->Write (pathPackagesIni);
    }
  if (userVariablePackageTable.Get() != 0
      && userVariablePackageTable->IsModified())
    {
      PathName pathPackagesIni
	(pSession->GetSpecialPath(SpecialPath::UserInstallRoot),
	 MIKTEX_PATH_PACKAGES_INI,
	 0);
      trace_mpm->WriteFormattedLine
	("libmpm",
	 T_("flushing user variable package table (%s)"),
	 Q_(pathPackagesIni));
      userVariablePackageTable->Write (pathPackagesIni);
    }
}

/* _________________________________________________________________________

   PackageManagerImpl::IsRemovable
   _________________________________________________________________________ */

bool
PackageManagerImpl::IsRemovable (/*[in]*/ const char * lpszDeploymentName)
{
  bool ret;
  LoadVariablePackageTable ();
  string str;
  if (pSession->IsAdminMode())
  {
    // administrator can remove system-wide packages
    ret = (GetCommonTimeInstalled(lpszDeploymentName) != 0);
  }
  else
  {
    // user can remove private packages
    if (pSession->GetSpecialPath(SpecialPath::CommonInstallRoot).Canonicalize()
      == pSession->GetSpecialPath(SpecialPath::UserInstallRoot).Canonicalize())
    {
      ret = (GetTimeInstalled(lpszDeploymentName) != 0);
    }
    else
    {
      ret = (GetUserTimeInstalled(lpszDeploymentName) != 0);
    }
  }
  return (ret);
}

/* _________________________________________________________________________

   PackageManagerImpl::GetUserTimeInstalled
   _________________________________________________________________________ */

time_t
PackageManagerImpl::GetUserTimeInstalled (/*[in]*/ const char * lpszDeploymentName)
{
  if (pSession->IsAdminMode())
  {
    UNEXPECTED_CONDITION ("PackageManagerImpl::GetUserTimeInstalled");
  }
  LoadVariablePackageTable ();
  string str;
  if (userVariablePackageTable.Get() != 0
      && userVariablePackageTable->TryGetValue(lpszDeploymentName,
						"TimeInstalled",
						str))
  {
    return (atoi(str.c_str()));
  }
  else
  {
    return (0);
  }
}

/* _________________________________________________________________________

   PackageManagerImpl::GetCommonTimeInstalled
   _________________________________________________________________________ */

time_t
PackageManagerImpl::GetCommonTimeInstalled (/*[in]*/ const char * lpszDeploymentName)
{
  LoadVariablePackageTable ();
  string str;
  if (commonVariablePackageTable.Get() != 0
      && commonVariablePackageTable->TryGetValue(lpszDeploymentName,
						 "TimeInstalled",
						 str))
  {
    return (atoi(str.c_str()));
  }
  else
  {
    return (0);
  }
}

/* _________________________________________________________________________

   PackageManagerImpl::GetTimeInstalled

   Returns the time when the package was installed.  Returns zero, if
   the package is not installed.
   _________________________________________________________________________ */

time_t
PackageManagerImpl::GetTimeInstalled (/*[in]*/ const char * lpszDeploymentName)
{
  LoadVariablePackageTable ();
  string str;
  if ((! pSession->IsAdminMode()
       && userVariablePackageTable.Get() != 0
       && userVariablePackageTable->TryGetValue(lpszDeploymentName,
						"TimeInstalled",
						str))
      || commonVariablePackageTable->TryGetValue(lpszDeploymentName,
						 "TimeInstalled",
						 str))
    {
      return (atoi(str.c_str()));
    }
  else
    {
      return (0);
    }
}

/* _________________________________________________________________________

   PackageManagerImpl::IsPackageInstalled
   _________________________________________________________________________ */

bool
PackageManagerImpl::IsPackageInstalled
  (/*[in]*/ const char * lpszDeploymentName)
{
  return (GetTimeInstalled(lpszDeploymentName) > 0);
}

/* _________________________________________________________________________

   PackageManagerImpl::IsPackageObsolete
   _________________________________________________________________________ */

bool
PackageManagerImpl::IsPackageObsolete
  (/*[in]*/ const char * lpszDeploymentName)
{
  LoadVariablePackageTable ();
  string str;
  if ((! pSession->IsAdminMode()
       && userVariablePackageTable.Get() != 0
       && userVariablePackageTable->TryGetValue(lpszDeploymentName,
						"Obsolete",
						str))
      || commonVariablePackageTable->TryGetValue(lpszDeploymentName,
					         "Obsolete",
						 str))
    {
      return (atoi(str.c_str()) != 0);
    }
  else
    {
      return (false);
    }
}

/* _________________________________________________________________________

   PackageManagerImpl::DeclarePackageObsolete
   _________________________________________________________________________ */

void
PackageManagerImpl::DeclarePackageObsolete
  (/*[in]*/ const char *	lpszDeploymentName,
   /*[in]*/ bool		obsolete)
{
  LoadVariablePackageTable ();
  if (pSession->IsAdminMode()
      || userVariablePackageTable.Get() == 0)
    {
      commonVariablePackageTable->PutValue (lpszDeploymentName,
					    "Obsolete",
					    (obsolete ? "1" : "0"));
    }
  else
    {
      userVariablePackageTable->PutValue (lpszDeploymentName,
					  "Obsolete",
					  (obsolete ? "1" : "0"));
    }
}

/* _________________________________________________________________________

   PackageManagerImpl::SetTimeInstalled
   _________________________________________________________________________ */

void
PackageManagerImpl::SetTimeInstalled
  (/*[in]*/ const char *	lpszDeploymentName,
   /*[in]*/ time_t		timeInstalled)
{
  LoadVariablePackageTable ();
  if (pSession->IsAdminMode()
    || userVariablePackageTable.Get() == 0)
  {
    if (timeInstalled == 0)
    {
      commonVariablePackageTable->DeleteKey (lpszDeploymentName);
    }
    else
    {
      commonVariablePackageTable->PutValue (lpszDeploymentName,
	"TimeInstalled",
	NUMTOSTR(timeInstalled));
    }
  }
  else
  {
    if (timeInstalled == 0)
    {
      userVariablePackageTable->DeleteKey (lpszDeploymentName);
    }
    else
    {
      userVariablePackageTable->PutValue (lpszDeploymentName,
	"TimeInstalled",
	NUMTOSTR(timeInstalled));
    }
  }
}

/* _________________________________________________________________________

   PackageManagerImpl::IncrementFileRefCounts
   _________________________________________________________________________ */

void
PackageManagerImpl::IncrementFileRefCounts
  (/*[in]*/ const vector<string> & files)
{
  for ( vector<string>::const_iterator it = files.begin();
	it != files.end();
	++ it)
    {
      ++ installedFileInfoTable[*it].refCount;
#if POLLUTE_THE_DEBUG_STREAM
      if (installedFileInfoTable[*it].refCount >= 2)
	{
	  trace_mpm->WriteFormattedLine ("libmpm",
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
  (/*[in]*/ const string &	deploymentName)
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
  (/*[in]*/ const string &	deploymentName,
   /*[in]*/ const PackageInfo &	packageInfo)
{
  pair<PackageDefinitionTable::iterator, bool> p =
    packageTable.insert
    (make_pair<string, PackageInfo>(deploymentName, packageInfo));
  p.first->second.deploymentName = deploymentName;
  if (pSession->IsMiKTeXDirect())
  {
    // installed from the start
    p.first->second.isRemovable = false;
    p.first->second.isObsolete = false;
    p.first->second.timeInstalled = packageInfo.timePackaged;
  }
  else
  {
    p.first->second.isRemovable = IsRemovable(deploymentName.c_str());
    p.first->second.isObsolete = IsPackageObsolete(deploymentName.c_str());
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
  trace_mpm->WriteFormattedLine
    ("libmpm",
     T_("searching %s for package definition files"),
     Q_(directory));

  if (! Directory::Exists(directory))
    {
      trace_mpm->WriteFormattedLine
	("libmpm",
	 T_("package definition directory (%s) does not exist"),
	 Q_(directory));
      return;
    }

  auto_ptr<DirectoryLister>
    pLister (DirectoryLister::Open(directory,
				   "*" MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX));

  // create parser object
  TpmParser tpmParser;

  // parse package definition files
  unsigned count = 0;
  DirectoryEntry direntry;
  while (pLister->GetNext(direntry))
    {
      PathName name (direntry.name.c_str());

      // get deployment name
      char szDeploymentName[BufferSizes::MaxPackageName];
      name.GetFileNameWithoutExtension (szDeploymentName);

      // ignore redefinition
      if (packageTable.find(szDeploymentName) != packageTable.end())
	{
#if 0
	  trace_mpm->WriteFormattedLine ("libmpm",
					 T_("%s: ignoring redefinition"),
					 szDeploymentName);
#endif
	  continue;
	}

      // parse package definition file
      tpmParser.Parse (PathName(directory, name, 0));

#if IGNORE_OTHER_SYSTEMS
      string targetSystems =
	tpmParser.GetPackageInfo().targetSystem;
      if (targetSystems != ""
	&& ! Utils::Contains(targetSystems.c_str(), MIKTEX_SYSTEM_TAG))
      {
	trace_mpm->WriteFormattedLine ("libmpm",
	  T_("%s: ignoring %s package"),
	  szDeploymentName, targetSystems.c_str());
	continue;
      }
#endif

#if POLLUTE_THE_DEBUG_STREAM
      trace_mpm->WriteFormattedLine ("libmpm",
				     T_("  adding %s"),
				     szDeploymentName);
#endif

      count += 1;
      
      // insert into database
      PackageInfo * pPi =
	DefinePackage(szDeploymentName, tpmParser.GetPackageInfo());

      // increment file ref counts, if package is installed
      if (pPi->timeInstalled > 0)
	{
	  IncrementFileRefCounts (pPi->runFiles);
	  IncrementFileRefCounts (pPi->docFiles);
	  IncrementFileRefCounts (pPi->sourceFiles);
	}
    }

  pLister->Close ();

  trace_mpm->WriteFormattedLine ("libmpm",
				 T_("found %u package definition files"),
				 static_cast<unsigned>(count));
  
  // determine dependencies
  for (PackageDefinitionTable::iterator it = packageTable.begin();
       it != packageTable.end();
       ++ it)
    {
      time_t timeInstalledMin = static_cast<time_t>(0xffffffffffffffffULL);
      time_t timeInstalledMax = 0;
      for (vector<string>::const_iterator
	     it2 = it->second.requiredPackages.begin();
	   it2 != it->second.requiredPackages.end();
	   ++ it2)
	{
	  PackageDefinitionTable::iterator it3 = packageTable.find(*it2);
	  if (it3 == packageTable.end())
	    {
	      trace_mpm->WriteFormattedLine
		("libmpm",
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
  piObsolete.deploymentName = "_miktex-obsolete";
  piObsolete.displayName = T_("Obsolete");
  piObsolete.title = T_("Obsolete packages");
  piObsolete.description =
    T_("Packages that were removed from the MiKTeX package repository.");
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
  piOther.deploymentName = "_miktex-all-the-rest";
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
      // we do this once
      return;
    }
  PathName userInstallRoot =
    pSession->GetSpecialPath(SpecialPath::UserInstallRoot);
  PathName commonInstallRoot =
    pSession->GetSpecialPath(SpecialPath::CommonInstallRoot);
  if (! pSession->IsAdminMode())
    {
      ParseAllPackageDefinitionFilesInDirectory
	(PathName(userInstallRoot,
		  MIKTEX_PATH_PACKAGE_DEFINITION_DIR,
		  0));
      if (userInstallRoot.Canonicalize() == commonInstallRoot.Canonicalize())
      {
	parsedAllPackageDefinitionFiles = true;
	return;
      }
    }
  ParseAllPackageDefinitionFilesInDirectory
    (PathName(commonInstallRoot,
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
      // enter the scratch directory
      scratchDirectory.Enter ();

      pathPackageInfoDir.SetToCurrentDirectory ();
      
      // unpack the package definition files
      auto_ptr<MiKTeX::Extractor::Extractor>
	pExtractor
	(MiKTeX::Extractor::Extractor::CreateExtractor(DB_ARCHIVE_FILE_TYPE));
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
  if (commonVariablePackageTable.Get() != 0)
    {
      commonVariablePackageTable.Release ();
    }
  if (userVariablePackageTable.Get() != 0)
    {
      userVariablePackageTable.Release ();
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
PackageManagerImpl::TryGetPackageInfo (/*[in]*/ const string & deploymentName)
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
  PathName pathPackageDefinitionFile;
  bool havePackageInfoFile = false;
  if (! pSession->IsAdminMode())
    {
      pathPackageDefinitionFile =
	pSession->GetSpecialPath(SpecialPath::UserInstallRoot);
      pathPackageDefinitionFile += MIKTEX_PATH_PACKAGE_DEFINITION_DIR;
      pathPackageDefinitionFile += deploymentName;
      pathPackageDefinitionFile.AppendExtension
	(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX);
      havePackageInfoFile = File::Exists(pathPackageDefinitionFile);
    }
  if (! havePackageInfoFile)
    {
      pathPackageDefinitionFile = 
	pSession->GetSpecialPath(SpecialPath::CommonInstallRoot);
      pathPackageDefinitionFile += MIKTEX_PATH_PACKAGE_DEFINITION_DIR;
      pathPackageDefinitionFile += deploymentName;
      pathPackageDefinitionFile.AppendExtension
	(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX);
      havePackageInfoFile = File::Exists(pathPackageDefinitionFile);
    }
  if (! havePackageInfoFile)
    {
      return (0);
    }
  TpmParser tpmParser;
  tpmParser.Parse (pathPackageDefinitionFile);
#if IGNORE_OTHER_SYSTEMS
  string targetSystems =
    tpmParser.GetPackageInfo().targetSystem;
  if (targetSystems != ""
    && ! Utils::Contains(targetSystems.c_str(), MIKTEX_SYSTEM_TAG))
  {
    return (0);
  }
#endif
  return (DefinePackage(deploymentName, tpmParser.GetPackageInfo()));
}

/* _________________________________________________________________________

   PackageManagerImpl::TryGetPackageInfo
   _________________________________________________________________________ */

bool
PackageManagerImpl::TryGetPackageInfo (/*[in]*/ const string & deploymentName,
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
PackageManagerImpl::GetPackageInfo (/*[in]*/ const string & deploymentName)
{
  const PackageInfo * pPackageInfo = TryGetPackageInfo(deploymentName);
  if (pPackageInfo == 0)
    {
      FATAL_MPM_ERROR ("MiKTeX::Packages::GetPackageInfo",
		       T_("Unknown package."),
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
PackageManager::TryGetRemotePackageRepository (/*[out]*/ string & url)
{
  if (SessionWrapper(true)
      ->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			  MIKTEX_REGVAL_REMOTE_REPOSITORY,
			  url))
    {
      return (true);
    }
  return (Utils::GetEnvironmentString(MIKTEX_ENV_REPOSITORY, url)
	  && (PackageManagerImpl::DetermineRepositoryType(url)
	      == RepositoryType::Remote));
}

/* _________________________________________________________________________

   PackageManager::GetRemotePackageRepository
   _________________________________________________________________________ */

string
PackageManager::GetRemotePackageRepository ()
{
  string url;
  if (! TryGetRemotePackageRepository(url))
    {
      FATAL_MIKTEX_ERROR ("PackageManager::GetRemotePackageRepository",
			  T_("Invalid MiKTeX configuration."),
			  0);
    }
  return (url);
}

/* _________________________________________________________________________

   IsUrl
   _________________________________________________________________________ */

MPMSTATICFUNC(bool)
IsUrl (/*[in]*/ const string & url)
{
  string::size_type pos = url.find("://");
  if (pos == string::npos)
    {
      return (false);
    }
  string scheme = url.substr(0, pos);
  for (string::const_iterator it = scheme.begin(); it != scheme.end(); ++ it)
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
  (/*[in]*/ const string & repository)
{
  if (IsUrl(repository))
    {
      return (RepositoryType::Remote);
    }

  if (! Utils::IsAbsolutePath(repository.c_str()))
    {
      FATAL_MPM_ERROR ("PackageManagerImpl::DetermineRepositoryType",
		       T_("Invalid MiKTeX configuration."),
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

  PathName path (repository);
  path += MIKTEX_PATH_PACKAGES_INI;
  if (File::Exists(path))
    {
      return (RepositoryType::MiKTeXInstallation);
    }

  FATAL_MPM_ERROR ("PackageManagerImpl::DetermineRepositoryType",
		   T_("Not a package repository."),
		   repository.c_str());
}

/* _________________________________________________________________________

   PackageManager::SetRemotePackageRepository
   _________________________________________________________________________ */

void
PackageManager::SetRemotePackageRepository (/*[in]*/ const string & url)
{
  SessionWrapper(true)
    ->SetConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
		      MIKTEX_REGVAL_REMOTE_REPOSITORY,
		      url.c_str());
}

/* _________________________________________________________________________

   PackageManager::TryGetLocalPackageRepository
   _________________________________________________________________________ */

bool
PackageManager::TryGetLocalPackageRepository (/*[out]*/ PathName & path)
{
  string str;
  if (SessionWrapper(true)
      ->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			  MIKTEX_REGVAL_LOCAL_REPOSITORY,
			  str))
    {
      path = str;
      return (true);
    }
  else if (Utils::GetEnvironmentString(MIKTEX_ENV_REPOSITORY, str)
	   && (PackageManagerImpl::DetermineRepositoryType(str)
	       == RepositoryType::Local))
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
PackageManager::GetLocalPackageRepository ()
{
  PathName path;
  if (! TryGetLocalPackageRepository(path))
    {
      FATAL_MIKTEX_ERROR ("PackageManager::GetLocalPackageRepository",
			  T_("Invalid MiKTeX configuration."),
			  0);
    }
  return (path);
}

/* _________________________________________________________________________

   PackageManager::SetLocalPackageRepository
   _________________________________________________________________________ */

void
PackageManager::SetLocalPackageRepository (/*[in]*/ const PathName & path)
{
  SessionWrapper(true)
    ->SetConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
		      MIKTEX_REGVAL_LOCAL_REPOSITORY,
		      path.Get());
}

/* _________________________________________________________________________

   PackageManager::TryGetMiKTeXDirectRoot
   _________________________________________________________________________ */

bool
PackageManager::TryGetMiKTeXDirectRoot (/*[out]*/ PathName & path)
{
  string str;
  if (SessionWrapper(true)
      ->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			  MIKTEX_REGVAL_MIKTEXDIRECT_ROOT,
			  str))
    {
      path = str;
      return (true);
    }
  else if (Utils::GetEnvironmentString(MIKTEX_ENV_REPOSITORY, str)
	   && (PackageManagerImpl::DetermineRepositoryType(str)
	       == RepositoryType::MiKTeXDirect))
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
PackageManager::GetMiKTeXDirectRoot ()
{
  PathName path;
  if (! TryGetMiKTeXDirectRoot(path))
    {
      FATAL_MIKTEX_ERROR ("PackageManager::GetMiKTeXDirectRoot",
			  T_("Invalid MiKTeX configuration."),
			  0);
    }
  return (path);
}

/* _________________________________________________________________________

   PackageManager::SetMiKTeXDirectRoot
   _________________________________________________________________________ */

void
PackageManager::SetMiKTeXDirectRoot (/*[in]*/ const PathName & path)
{
  SessionWrapper(true)
    ->SetConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
		      MIKTEX_REGVAL_MIKTEXDIRECT_ROOT,
		      path.Get());
}

/* _________________________________________________________________________

   PackageManager::TryGetDefaultPackageRepository
   _________________________________________________________________________ */

bool
PackageManager::TryGetDefaultPackageRepository
  (/*[out]*/ RepositoryType &	repositoryType,
   /*[out]*/ string &		urlOrPath)
{
  string str;
  if (SessionWrapper(true)->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
					      MIKTEX_REGVAL_REPOSITORY_TYPE,
					      str))
    {
      if (str == "remote")
	{
	  urlOrPath = GetRemotePackageRepository();
	  repositoryType = RepositoryType::Remote;
	}
      else if (str == "local")
	{
	  urlOrPath = GetLocalPackageRepository().Get();
	  repositoryType = RepositoryType::Local;
	}
      else if (str == "direct")
	{
	  urlOrPath = GetMiKTeXDirectRoot().Get();
	  repositoryType = RepositoryType::MiKTeXDirect;
	}
      else
	{
	  FATAL_MIKTEX_ERROR
	    ("PackageManager::TryGetDefaultPackageRepository",
	     T_("Invalid MiKTeX configuration."),
	     str.c_str());
	}
      return (true);
    }
  else if (Utils::GetEnvironmentString(MIKTEX_ENV_REPOSITORY, urlOrPath))
    {
      repositoryType = PackageManagerImpl::DetermineRepositoryType(urlOrPath);
      return (true);
    }
  else
    {
      return (false);
    }
}

/* _________________________________________________________________________

   PackageManager::GetDefaultPackageRepository
   _________________________________________________________________________ */

RepositoryType
PackageManager::GetDefaultPackageRepository (/*[out]*/ string &	urlOrPath)
{
  RepositoryType repositoryType (RepositoryType::Unknown);
  if (! TryGetDefaultPackageRepository(repositoryType, urlOrPath))
    {
      FATAL_MIKTEX_ERROR ("PackageManager::GetDefaultPackageRepository",
			  T_("Invalid MiKTeX configuration."),
			  0);
    }
  return (repositoryType);
}

/* _________________________________________________________________________

   PackageManager::SetDefaultPackageRepository
   _________________________________________________________________________ */

void
PackageManager::SetDefaultPackageRepository
  (/*[in]*/ RepositoryType	repositoryType,
   /*[in]*/ const string &	urlOrPath)
{
  if (repositoryType == RepositoryType::Unknown)
    {
      repositoryType = PackageManagerImpl::DetermineRepositoryType(urlOrPath);
    }
  string repositoryTypeStr;
  switch (repositoryType.Get())
    {
    case RepositoryType::MiKTeXDirect:
      repositoryTypeStr = "direct";
      SetMiKTeXDirectRoot (urlOrPath);
      break;
    case RepositoryType::Local:
      repositoryTypeStr = "local";
      SetLocalPackageRepository (urlOrPath);
      break;
    case RepositoryType::Remote:
      repositoryTypeStr = "remote";
      SetRemotePackageRepository (urlOrPath);
      break;
    default:
      UNEXPECTED_CONDITION ("PackageManager::SetDefaultPackageRepository");
    }
  SessionWrapper(true)
    ->SetConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
		      MIKTEX_REGVAL_REPOSITORY_TYPE,
		      repositoryTypeStr.c_str());
}

/* _________________________________________________________________________

   MakeRepositoryInfo
   _________________________________________________________________________ */

template<class RepositoryInfo_>
MPMSTATICFUNC(RepositoryInfo)
MakeRepositoryInfo (/*[in]*/ const RepositoryInfo_ * pMwsRepositoryInfo)
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

/* _________________________________________________________________________

   PackageManagerImpl::DownloadRepositoryList
   _________________________________________________________________________ */

void
PackageManagerImpl::DownloadRepositoryList ()
{
  repositories.clear ();

  MyRepositorySoapProxy repositorySoapProxy;
  ProxySettings proxySettings;
  if (TryGetProxy(WEBSVCURL, proxySettings) && proxySettings.useProxy)
    {
      repositorySoapProxy.proxy_host = proxySettings.proxy.c_str();
      repositorySoapProxy.proxy_port = proxySettings.port;
      if (proxySettings.authenticationRequired)
	{
	  repositorySoapProxy.proxy_userid = proxySettings.user.c_str();
	  repositorySoapProxy.proxy_passwd = proxySettings.password.c_str();
	}
    }
  ClientInfo<mtrep3__ClientInfo> clientInfo;
  _mtrep3__GetRepositories2 arg;
  arg.clientInfo = &clientInfo;
  arg.onlyOnline = true;
  arg.noCorrupted = true;
  arg.maxDelay = -1;
  _mtrep3__GetRepositories2Response resp;
  if (repositorySoapProxy.GetRepositories2(&arg, &resp) != SOAP_OK)
    {
      FATAL_SOAP_ERROR (&repositorySoapProxy);
    }
  for (vector<mtrep3__RepositoryInfo*>::const_iterator it = 
	 resp.GetRepositories2Result->RepositoryInfo.begin();
       it != resp.GetRepositories2Result->RepositoryInfo.end();
       ++ it)
    {
      repositories.push_back (MakeRepositoryInfo(*it));
    }
}

/* _________________________________________________________________________

   PackageManagerImpl::PickRepositoryUrl
   _________________________________________________________________________ */

string
PackageManagerImpl::PickRepositoryUrl ()
{
  MyRepositorySoapProxy repositorySoapProxy;
  ProxySettings proxySettings;
  if (TryGetProxy(WEBSVCURL, proxySettings) && proxySettings.useProxy)
    {
      repositorySoapProxy.proxy_host = proxySettings.proxy.c_str();
      repositorySoapProxy.proxy_port = proxySettings.port;
      if (proxySettings.authenticationRequired)
	{
	  repositorySoapProxy.proxy_userid = proxySettings.user.c_str();
	  repositorySoapProxy.proxy_passwd = proxySettings.password.c_str();
	}
    }
  ClientInfo<mtrep3__ClientInfo> clientInfo;
  _mtrep3__PickRepository2 arg;
  arg.clientInfo = &clientInfo;
  _mtrep3__PickRepository2Response resp;
  if (repositorySoapProxy.PickRepository2(&arg, &resp) != SOAP_OK)
    {
      FATAL_SOAP_ERROR (&repositorySoapProxy);
    }
  if (resp.PickRepository2Result->Url == 0)
    {
      UNEXPECTED_CONDITION ("PackageManagerImpl::PickRepositoryUrl");
    }
  return (*resp.PickRepository2Result->Url);
}

/* _________________________________________________________________________

   PackageManagerImpl::TraceError
   _________________________________________________________________________ */

void
PackageManagerImpl::TraceError (/*[in]*/ const char *	lpszFormat,
				/*[in]*/			...)
{
  va_list marker;
  va_start (marker, lpszFormat);
  trace_error->VTrace ("libmpm", lpszFormat, marker);
  va_end (marker);
}

/* _________________________________________________________________________

   RememberFileNameInfo
   _________________________________________________________________________ */

namespace {

#if defined(HAVE_UNORDERED_SET)
  typedef tr1::unordered_set<string, hash_path, equal_path> SubDirectoryTable;
#else
  typedef set<string, less_path> SubDirectoryTable;
#endif
  
  struct DirectoryInfo
  {
    SubDirectoryTable subDirectoryNames;
    string fileNames;
    string packageNames;
  };

  
#if defined(HAVE_UNORDERED_MAP)
  typedef
  tr1::unordered_map<string, DirectoryInfo, hash_path, equal_path>
  DirectoryInfoTable;
#else
  typedef
  map<string, DirectoryInfo, less_path>
  DirectoryInfoTable;
#endif  

  // directory info table: written by
  // PackageManagerImpl::CreateMpmFndb(); read by
  // PackageManagerImpl::ReadDirectory()
  DirectoryInfoTable directoryInfoTable;

}

MPMSTATICFUNC(void)
RememberFileNameInfo (/*[in]*/ const string &	prefixedFileName,
		      /*[in]*/ const string &	packageName)
{
  string fileName;

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
  PathName path = SessionWrapper(true)->GetMpmRootPath();
  //  path += CURRENT_DIRECTORY;

  // lpsz1: current path name component
  const char * lpsz1 = pathtok.GetCurrent();

  // lpszName: file name component
  const char * lpszName = lpsz1;

  for (const char * lpsz2 = ++ pathtok; lpsz2 != 0; lpsz2 = ++ pathtok)
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
  directoryInfo.fileNames += '\0';
  directoryInfo.packageNames += packageName;
  directoryInfo.packageNames += '\0';
}

/* _________________________________________________________________________

   PackageManagerImpl::ReadDirectory
   _________________________________________________________________________ */

bool
PackageManagerImpl::ReadDirectory (/*[in]*/ const char *	lpszPath,
				   /*[out]*/ char ** ppSubDirectoryNames,
				   /*[out]*/ char ** ppFileNames,
				   /*[out]*/ char ** ppFileNameInfos)
{
  MIKTEX_ASSERT_STRING (lpszPath);

  // get the directory info for the given path
  DirectoryInfo & directoryInfo = directoryInfoTable[lpszPath];

  string subDirectoryNames;
  for (SubDirectoryTable::const_iterator
	 it = directoryInfo.subDirectoryNames.begin();
       it != directoryInfo.subDirectoryNames.end();
       ++ it)
    {
      subDirectoryNames += *it;
      subDirectoryNames += '\0';
    }
  subDirectoryNames += '\0';
  MIKTEX_ASSERT (ppSubDirectoryNames != 0);
  *ppSubDirectoryNames =
    static_cast<char *>(malloc(subDirectoryNames.length()));
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
  subDirectoryNames._Copy_s (*ppSubDirectoryNames,
			     subDirectoryNames.length(),
			     subDirectoryNames.length());
#else
  subDirectoryNames.copy (*ppSubDirectoryNames, subDirectoryNames.length());
#endif
  MIKTEX_ASSERT (ppFileNames != 0);
  directoryInfo.fileNames += '\0';
  *ppFileNames =
    static_cast<char *>(malloc(directoryInfo.fileNames.length()));
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
  directoryInfo.fileNames._Copy_s (*ppFileNames,
				   directoryInfo.fileNames.length(),
				   directoryInfo.fileNames.length());
#else
  directoryInfo.fileNames.copy (*ppFileNames,
				directoryInfo.fileNames.length());
#endif
  MIKTEX_ASSERT (ppFileNameInfos != 0);
  directoryInfo.packageNames += '\0';
  *ppFileNameInfos =
    static_cast<char *>(malloc(directoryInfo.packageNames.length()));
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
PackageManagerImpl::OnProgress (/*[in]*/ unsigned	level,
				/*[in]*/ const char *	lpszDirectory)
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
      vector<string>::const_iterator it2;
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
	        pSession->GetMpmRootPath().Get(),
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
  (/*[in]*/ const char * lpszPath)
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
PackageManager::IsLocalPackageRepository (/*[in]*/ const PathName & path)
{
  if (! Directory::Exists(path))
    {
      return (false);
    }

  // local mirror of remote package repository?
  if (File::Exists(PathName(path, MIKTEX_MPM_DB_LIGHT_FILE_NAME))
      && File::Exists(PathName(path, MIKTEX_MPM_DB_FULL_FILE_NAME)))
    {
      return (true);
    }

  return (false);
}

/* _________________________________________________________________________

   PackageManager::ReadPackageDefinitionFile
   _________________________________________________________________________ */

PackageInfo
PackageManager::ReadPackageDefinitionFile
  (/*[in]*/ const PathName &	path,
   /*[in]*/ const char *	lpszTeXMFPrefix)
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
    FPutS ("<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n", stream);
  }

public:
  void
  StartElement (/*[in]*/ const char *	lpszName)
  {
    if (freshElement)
      {
	FPutC ('>', stream);
      }
    FPutC ('<', stream);
    FPutS (lpszName, stream);
    freshElement = true;
    elements.push (lpszName);
  }

public:
  void
  AddAttribute (/*[in]*/ const char *	lpszAttributeName,
		/*[in]*/ const char *	lpszAttributeValue)
  {
    FPutC (' ', stream);
    FPutS (lpszAttributeName, stream);
    FPutS ("=\"", stream);
    FPutS (lpszAttributeValue, stream);
    FPutC ('"', stream);
  }

public:
  void
  EndElement ()
  {
    if (elements.empty())
      {
	FATAL_MIKTEX_ERROR ("XmlWriter::EndElement",
			    T_("No elements on the stack."),
			    0);
      }
    if (freshElement)
      {
	FPutS ("/>", stream);
	freshElement = false;
      }
    else
      {
	FPutS ("</", stream);
	FPutS (elements.top().c_str(), stream);
	FPutC ('>', stream);
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
  Text (/*[in]*/ const string & text )
  {
    if (freshElement)
      {
	FPutC ('>', stream);
	freshElement = false;
      }
    for (const char * lpszText = text.c_str();
	 *lpszText != 0;
	 ++ lpszText)
      {
	switch (*lpszText)
	  {
	  case '&':
	    FPutS ("&amp;", stream);
	    break;
	  case '<':
	    FPutS ("&lt;", stream);
	    break;
	  case '>':
	    FPutS ("&gt;", stream);
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
  stack<string> elements;

private:
  bool freshElement;
};

/* _________________________________________________________________________

   PackageManager::WritePackageDefinitionFile
   _________________________________________________________________________ */

void
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
  xml.StartElement ("rdf:RDF");
  xml.AddAttribute ("xmlns:rdf",
		    "http://www.w3.org/1999/02/22-rdf-syntax-ns#");
  xml.AddAttribute ("xmlns:TPM", "http://texlive.dante.de/");
  xml.StartElement ("rdf:Description");
  string about ("http://www.miktex.org/packages/");
  about += packageInfo.deploymentName;
  xml.AddAttribute ("about", about.c_str());

  // create "TPM:Name" node
  xml.StartElement ("TPM:Name");
  xml.Text (packageInfo.displayName);
  xml.EndElement ();

  // create "TPM:Creator" node
  xml.StartElement ("TPM:Creator");
  xml.Text ("mpc");
  xml.EndElement ();
  
  // create "TPM:Title" node
  xml.StartElement ("TPM:Title");
  xml.Text (packageInfo.title);
  xml.EndElement ();

  // create "TPM:Version" node
  xml.StartElement ("TPM:Version");
  xml.Text (packageInfo.version);
  xml.EndElement ();

  // create "TPM:TargetSystem" node
  xml.StartElement ("TPM:TargetSystem");
  xml.Text (packageInfo.targetSystem);
  xml.EndElement ();

  // create "TPM:Description" node
  xml.StartElement ("TPM:Description");
  xml.Text (packageInfo.description);
  xml.EndElement ();
  

  // create "TPM:RunFiles" node
  if (packageInfo.runFiles.size() > 0)
    {
      xml.StartElement ("TPM:RunFiles");
      xml.AddAttribute
	("size",
	 NUMTOSTR(static_cast<unsigned>(packageInfo.sizeRunFiles)));
      for (vector<string>::const_iterator it = packageInfo.runFiles.begin();
	   it != packageInfo.runFiles.end();
	   ++ it)
	{
	  if (it != packageInfo.runFiles.begin())
	    {
	      xml.Text (" ");
	    }
	  xml.Text (*it);
	}
      xml.EndElement ();
    }

  // create "TPM:DocFiles" node
  if (packageInfo.docFiles.size() > 0)
    {
      xml.StartElement ("TPM:DocFiles");
      xml.AddAttribute
	("size",
	 NUMTOSTR(static_cast<unsigned>(packageInfo.sizeDocFiles)));
      for (vector<string>::const_iterator it = packageInfo.docFiles.begin();
	   it != packageInfo.docFiles.end();
	   ++ it)
	{
	  if (it != packageInfo.docFiles.begin())
	    {
	      xml.Text (" ");
	    }
	  xml.Text (*it);
	}
      xml.EndElement ();
    }

  // create "TPM:SourceFiles" node
  if (packageInfo.sourceFiles.size())
    {
      xml.StartElement ("TPM:SourceFiles");
      xml.AddAttribute
	("size",
	 NUMTOSTR(static_cast<unsigned>(packageInfo.sizeSourceFiles)));
      for (vector<string>::const_iterator it
	     = packageInfo.sourceFiles.begin();
	   it != packageInfo.sourceFiles.end();
	   ++ it)
	{
	  if (it != packageInfo.sourceFiles.begin())
	    {
	      xml.Text (" ");
	    }
	  xml.Text (*it);
	}
      xml.EndElement ();
    }

  // create "TPM:Requires" node
  if (packageInfo.requiredPackages.size() > 0)
    {
      xml.StartElement ("TPM:Requires");
      for (vector<string>::const_iterator it
	     = packageInfo.requiredPackages.begin();
	   it != packageInfo.requiredPackages.end();
	   ++ it)
	{
	  xml.StartElement ("TPM:Package");
	  xml.AddAttribute ("name", it->c_str());
	  xml.EndElement ();
	}
      xml.EndElement ();
    }

  // create "TPM:TimePackaged" node
  if (timePackaged != 0)
    {
      xml.StartElement ("TPM:TimePackaged");
      xml.Text (NUMTOSTR(timePackaged));
      xml.EndElement ();
    }

  // create "TPM:MD5" node
  xml.StartElement ("TPM:MD5");
  xml.Text (packageInfo.digest.ToString());
  xml.EndElement ();

#if MIKTEX_EXTENDED_PACKAGEINFO
  if (! packageInfo.ctanPath.empty())
  {
    xml.StartElement ("TPM:CTAN");
    xml.AddAttribute ("path", packageInfo.ctanPath.c_str());
    xml.EndElement ();
  }

  if (! (packageInfo.copyrightOwner.empty() && packageInfo.copyrightYear.empty()))
  {
    xml.StartElement ("TPM:Copyright");
    xml.AddAttribute ("owner", packageInfo.copyrightOwner.c_str());
    xml.AddAttribute ("year", packageInfo.copyrightYear.c_str());
    xml.EndElement ();
  }

  if (! packageInfo.licenseType.empty())
  {
    xml.StartElement ("TPM:License");
    xml.AddAttribute ("type", packageInfo.licenseType.c_str());
    xml.EndElement ();
  }
#endif

  xml.EndAllElements ();

  stream.Close ();
}

/* _________________________________________________________________________

   PackageManager::StripTeXMFPrefix
   _________________________________________________________________________ */

bool
PackageManager::StripTeXMFPrefix (/*[in]*/ const string &	str,
				  /*[out]*/ string &		result)
{
  if (StripPrefix(str, TEXMF_PREFIX_DIRECTORY, result))
    {
      return (true);
    }
  PathName prefix2 (".");
  prefix2 += TEXMF_PREFIX_DIRECTORY;
  return (StripPrefix(str, prefix2.Get(), result));
}

/* _________________________________________________________________________

   PackageManager::SetProxy
   _________________________________________________________________________ */

void
PackageManager::SetProxy (/*[in]*/ const ProxySettings & proxySettings)
{
#if defined(MIKTEX_WINDOWS)
  SessionWrapper(true)
    ->SetConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
		      MIKTEX_REGVAL_USE_PROXY,
		      proxySettings.useProxy);
  SessionWrapper(true)
    ->SetConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
		      MIKTEX_REGVAL_PROXY_HOST,
		      proxySettings.proxy.c_str());
  SessionWrapper(true)
    ->SetConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
		      MIKTEX_REGVAL_PROXY_PORT,
		      proxySettings.port);
  SessionWrapper(true)
    ->SetConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
		      MIKTEX_REGVAL_PROXY_AUTH_REQ,
		      proxySettings.authenticationRequired);
#endif
  PackageManagerImpl::proxyUser = proxySettings.user;
  PackageManagerImpl::proxyPassword = proxySettings.password;
}

/* _________________________________________________________________________

   PackageManager::TryGetProxy
   _________________________________________________________________________ */

bool
PackageManager::TryGetProxy (/*[in]*/ const char *	lpszDestUrl,
			     /*[out]*/ ProxySettings &	proxySettings)
{
  string proxyEnv;
  if (lpszDestUrl != 0)
    {
      Uri uri (lpszDestUrl);
      string scheme = uri.GetScheme();
      string envName;
      if (scheme == "http")
	{
	  envName = "http_proxy";
	}
      else if (scheme == "ftp")
	{
	  envName = "FTP_PROXY";
	}
      else
	{
	  UNEXPECTED_CONDITION ("PackageManager::TryGetProxy");
	}
      Utils::GetEnvironmentString (envName.c_str(), proxyEnv);
    }
  if (proxyEnv.empty())
    {
      Utils::GetEnvironmentString("ALL_PROXY", proxyEnv);
    }
  if (! proxyEnv.empty())
    {
      Uri uri (proxyEnv.c_str());
      proxySettings.useProxy = true;
      proxySettings.proxy = uri.GetHost();
      proxySettings.port = uri.GetPort();
      string userInfo = uri.GetUserInfo();
      if (! userInfo.empty())
	{
	  proxySettings.authenticationRequired = true;
	  string::size_type idx = userInfo.find_first_of(":");
	  if (idx == string::npos)
	    {
	      proxySettings.user = userInfo;
	      proxySettings.password = "";
	    }
	  else
	    {
	      proxySettings.user = userInfo.substr(0, idx);
	      proxySettings.password = userInfo.substr(idx + 1);
	    }
	}
      return (true);
    }
  string str;
  if (! (SessionWrapper(true)
	 ->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			     MIKTEX_REGVAL_USE_PROXY,
			     str)))
    {
      return (false);
    }
  proxySettings.useProxy = (str == "t");
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
  proxySettings.port = atoi(str.c_str());
  if (! (SessionWrapper(true)
	 ->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			     MIKTEX_REGVAL_PROXY_AUTH_REQ,
			     str)))
    {
      return (false);
    }
  proxySettings.authenticationRequired = (str == "t");
  proxySettings.user = PackageManagerImpl::proxyUser;
  proxySettings.password = PackageManagerImpl::proxyPassword;
  return (true);
}

/* _________________________________________________________________________

   PackageManager::TryGetProxy
   _________________________________________________________________________ */

bool
PackageManager::TryGetProxy (/*[out]*/ ProxySettings & proxySettings)
{
  return (TryGetProxy(0, proxySettings));
}

/* _________________________________________________________________________

   PackageManager::GetProxy
   _________________________________________________________________________ */

ProxySettings
PackageManager::GetProxy (/*[in]*/ const char * lpszDestUrl)
{
  ProxySettings proxySettings;
  if (! TryGetProxy(lpszDestUrl, proxySettings))
    {
      FATAL_MIKTEX_ERROR ("PackageManager::GetProxy",
			  T_("No proxy host is configured."),
			  0);
    }
  return (proxySettings);
}

/* _________________________________________________________________________

   PackageManager::GetProxy
   _________________________________________________________________________ */

ProxySettings
PackageManager::GetProxy ()
{
  return (GetProxy(0));
}

/* _________________________________________________________________________

   PackageManagerImpl::OnProgress
   _________________________________________________________________________ */

void
PackageManagerImpl::OnProgress ()
{
}

/* _________________________________________________________________________

   PackageManagerImpl::TryGetRepositoryInfo
   _________________________________________________________________________ */

bool
PackageManagerImpl::TryGetRepositoryInfo
  (/*[in]*/ const string &	url,
   /*[out]*/ RepositoryInfo &	repositoryInfo)
{
  MyRepositorySoapProxy repositorySoapProxy;
  ProxySettings proxySettings;
  if (TryGetProxy(WEBSVCURL, proxySettings) && proxySettings.useProxy)
    {
      repositorySoapProxy.proxy_host = proxySettings.proxy.c_str();
      repositorySoapProxy.proxy_port = proxySettings.port;
      if (proxySettings.authenticationRequired)
	{
	  repositorySoapProxy.proxy_userid = proxySettings.user.c_str();
	  repositorySoapProxy.proxy_passwd = proxySettings.password.c_str();
	}
    }
  ClientInfo<mtrep3__ClientInfo> clientInfo;
  string url2 = url;
  _mtrep3__TryGetRepositoryInfo2 arg;
  arg.clientInfo = &clientInfo;
  arg.url = &url2;
  _mtrep3__TryGetRepositoryInfo2Response resp;
  if (repositorySoapProxy.TryGetRepositoryInfo2(&arg, &resp) != SOAP_OK)
    {
      FATAL_SOAP_ERROR (&repositorySoapProxy);
    }
  if (resp.TryGetRepositoryInfo2Result)
    {
      repositoryInfo = MakeRepositoryInfo(resp.repositoryInfo);
    }
  return (resp.TryGetRepositoryInfo2Result);
}

/* _________________________________________________________________________

   PackageManagerImpl::VerifyPackageRepository
   _________________________________________________________________________ */

RepositoryInfo
PackageManagerImpl::VerifyPackageRepository (/*[in]*/ const string & url)
{
  for (vector<RepositoryInfo>::const_iterator it = repositories.begin();
       it != repositories.end();
       ++ it)
    {
      if (it->url == url)
	{
	  return (*it);
	}
    }
  RepositoryInfo repositoryInfo;
  MyRepositorySoapProxy repositorySoapProxy;
  ProxySettings proxySettings;
  if (TryGetProxy(WEBSVCURL, proxySettings) && proxySettings.useProxy)
    {
      repositorySoapProxy.proxy_host = proxySettings.proxy.c_str();
      repositorySoapProxy.proxy_port = proxySettings.port;
      if (proxySettings.authenticationRequired)
	{
	  repositorySoapProxy.proxy_userid = proxySettings.user.c_str();
	  repositorySoapProxy.proxy_passwd = proxySettings.password.c_str();
	}
    }
  string url2 = url;
  ClientInfo<mtrep4__ClientInfo> clientInfo;
  _mtrep4__VerifyRepository arg;
  arg.clientInfo = &clientInfo;
  arg.url = &url2;
  _mtrep4__VerifyRepositoryResponse resp;
  if (repositorySoapProxy.VerifyRepository(&arg, &resp) != SOAP_OK)
    {
      FATAL_SOAP_ERROR (&repositorySoapProxy);
    }
  if (! resp.VerifyRepositoryResult)
    {
      if (resp.repositoryInfo == 0)
	{
	  FATAL_MPM_ERROR
	    ("PackageManagerImpl::VerifyPackageRepository",
	     T_("\
The remote package repository is not registered. \
You have to choose another repository."),
	     url.c_str());
	}
      else if (resp.repositoryInfo->Status != RepositoryStatus::Online)
	{
	  FATAL_MPM_ERROR
	    ("PackageManagerImpl::VerifyPackageRepository",
	     T_("\
The remote package repository is not online. \
You have to choose another repository."),
	     url.c_str());
	}
      else if (resp.repositoryInfo->Integrity == RepositoryIntegrity::Corrupted)
	{
	  FATAL_MPM_ERROR
	    ("PackageManagerImpl::VerifyPackageRepository",
	     T_("\
The remote package repository is corrupted. \
You have to choose another repository."),
	     url.c_str());
	}
      else
	{
	  FATAL_MPM_ERROR
	    ("PackageManagerImpl::VerifyPackageRepository",
	     T_("\
The remote package repository is outdated. \
You have to choose another repository."),
	     url.c_str());
	}
    }
  repositoryInfo = MakeRepositoryInfo(resp.repositoryInfo);
  repositories.push_back (repositoryInfo);
  return (repositoryInfo);
}

/* _________________________________________________________________________

   PackageManagerImpl::TryVerifyInstalledPackageHelper
   _________________________________________________________________________ */

bool
PackageManagerImpl::TryVerifyInstalledPackageHelper
  (/*[in]*/ const PathName &	prefix,
   /*[in]*/ const string &	fileName,
   /*[out]*/ bool &		haveDigest,
   /*[out]*/ MD5 &		digest)
{
  string unprefixed;
  if (! StripTeXMFPrefix(fileName, unprefixed))
  {
    return (true);
  }
  PathName path = prefix;
  path += unprefixed;
  if (! File::Exists(path))
  {
    trace_mpm->WriteFormattedLine
      ("libmpm",
       T_("package verification failed: file %s does not exist"),
       Q_(path));
    return (false);
  }
  if (path.HasExtension(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX))
  {
    haveDigest = false;
    return (true);
  }
  digest = MD5::FromFile(path.Get());
  haveDigest = true;
  return (true);
}

/* _________________________________________________________________________

   PackageManagerImpl::TryVerifyInstalledPackage
   _________________________________________________________________________ */

bool
PackageManagerImpl::TryVerifyInstalledPackage
  (/*[in]*/ const string & deploymentName)
{
  PackageInfo packageInfo = GetPackageInfo(deploymentName);

  PathName prefix;

  if (! pSession->IsAdminMode()
      && GetUserTimeInstalled(deploymentName.c_str()) != static_cast<time_t>(0))
  {
    prefix = pSession->GetSpecialPath(SpecialPath::UserInstallRoot);
  }

  if (prefix.Empty())
  {
    prefix = pSession->GetSpecialPath(SpecialPath::CommonInstallRoot);
  }

  FileDigestTable fileDigests;

  for (vector<string>::const_iterator it = packageInfo.runFiles.begin();
       it != packageInfo.runFiles.end();
       ++ it)
    {
      bool haveDigest;
      MD5 digest;
      if (! TryVerifyInstalledPackageHelper(prefix, *it, haveDigest, digest))
	{
	  return (false);
	}
      if (haveDigest)
	{
	  fileDigests[*it] = digest;
	}
    }

  for (vector<string>::const_iterator it = packageInfo.docFiles.begin();
       it != packageInfo.docFiles.end();
       ++ it)
    {
      bool haveDigest;
      MD5 digest;
      if (! TryVerifyInstalledPackageHelper(prefix, *it, haveDigest, digest))
	{
	  return (false);
	}
      if (haveDigest)
	{
	  fileDigests[*it] = digest;
	}
    }

  for (vector<string>::const_iterator it = packageInfo.sourceFiles.begin();
       it != packageInfo.sourceFiles.end();
       ++ it)
    {
      bool haveDigest;
      MD5 digest;
      if (! TryVerifyInstalledPackageHelper(prefix, *it, haveDigest, digest))
	{
	  return (false);
	}
      if (haveDigest)
	{
	  fileDigests[*it] = digest;
	}
    }

  MD5Builder md5Builder;

  for (FileDigestTable::const_iterator it = fileDigests.begin();
       it != fileDigests.end();
       ++ it)
    {
      PathName path (it->first);
      // we must dosify the path name for backward compatibility
      path.ToDos ();
      md5Builder.Update (path.Get(), path.GetLength());
      md5Builder.Update (it->second.GetBits(), sizeof(MD5));
    }

  bool ok = (md5Builder.Final() == packageInfo.digest);

  if (! ok)
    {
      trace_mpm->WriteFormattedLine
	("libmpm",
	 T_("package %s verification failed: some files have been modified"),
	 Q_(deploymentName));
    }

  return (ok);
}

/* _________________________________________________________________________

   PackageManagerImpl::GetContainerPath
   _________________________________________________________________________ */

string
PackageManagerImpl::GetContainerPath (/*[in]*/ const string &	deploymentName,
				      /*[in]*/ bool	useDisplayNames)
{
  string path;
  PackageInfo packageInfo = GetPackageInfo(deploymentName);
  for (size_t idx = 0; idx < packageInfo.requiredBy.size(); ++ idx)
    {
      PackageInfo packageInfo2 = GetPackageInfo(packageInfo.requiredBy[idx]);
      if (packageInfo2.IsPureContainer())
	{
	  // <recursivecall>
	  path =
	    GetContainerPath(packageInfo.requiredBy[idx], useDisplayNames);
	  // </recursivecall>
	  path += PathName::DirectoryDelimiter;
	  if (useDisplayNames)
	    {
	      path += packageInfo2.displayName;
	    }
	  else
	    {
	      path += packageInfo2.deploymentName;
	    }
	  break;
	}
    }
  return (path);
}
