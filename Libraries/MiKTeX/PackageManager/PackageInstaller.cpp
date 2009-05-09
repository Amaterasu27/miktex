/* PackageInstaller.cpp:

   Copyright (C) 2001-2009 Christian Schenk

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
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

#include "TpmParser.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Extractor;
using namespace MiKTeX::Packages;
using namespace std;

#define LF "\n"

/* _________________________________________________________________________

   PackageInstallerImpl::MakeUrl
   _________________________________________________________________________ */

string
PackageInstallerImpl::MakeUrl (/*[in]*/ const char * lpszBase,
			       /*[in]*/ const char * lpszRel)
{
  string url (lpszBase);
  size_t l = url.length();
  if (l == 0)
    {
      FATAL_MPM_ERROR ("MakeUrl", T_("Invalid base URL."), lpszBase);
    }
  if (url[l - 1] != '/')
    {
      url += '/';
    }
  if (lpszRel[0] == '/')
    {
      FATAL_MPM_ERROR ("MakeUrl", T_("Invalid relative URL."), lpszRel);
    }
  url += lpszRel;
  return (url);
}

/* _________________________________________________________________________

   PackageInstallerImpl::MakeUrl
   _________________________________________________________________________ */

string
PackageInstallerImpl::MakeUrl (/*[in]*/ const char * lpszRel)
{
  return (MakeUrl(repository.c_str(), lpszRel));
}

/* _________________________________________________________________________

   PackageInstaller::~PackageInstaller
   _________________________________________________________________________ */

PackageInstaller::~PackageInstaller ()
{
}

/* _________________________________________________________________________

   IsPureContainer
   _________________________________________________________________________ */

MPMSTATICFUNC(bool)
IsPureContainer (/*[in]*/ const char * lpszDeploymentName)
{
  return (strncmp(lpszDeploymentName, "_miktex-", 8) == 0);
}

/* _________________________________________________________________________

   IsMiKTeXPackage
   _________________________________________________________________________ */

MPMSTATICFUNC(bool)
IsMiKTeXPackage (/*[in]*/ const char * lpszDeploymentName)
{
  return (strncmp(lpszDeploymentName, "miktex-", 7) == 0);
}

/* _________________________________________________________________________

   PrefixedPackageDefinitionFile

   Get the prefixed path to a package definition file.
   _________________________________________________________________________ */

MPMSTATICFUNC(PathName)
PrefixedPackageDefinitionFile (/*[in]*/ const string & deploymentName)
{
  PathName path (TEXMF_PREFIX_DIRECTORY);
  path += MIKTEX_PATH_PACKAGE_DEFINITION_DIR;
  path += deploymentName;
  path.AppendExtension (MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX);
  return (path);
}

/* _________________________________________________________________________

   PackageInstallerImpl::PackageInstallerImpl
   _________________________________________________________________________ */

PackageInstallerImpl::PackageInstallerImpl
    (/*[in]*/ PackageManagerImpl * pManager)

  : pThread (0),
    pCallback (0),
    noPostProcessing (false),
    noLocalServer (false),
    taskPackageLevel (PackageLevel::None),
    repositoryType (RepositoryType::Unknown),
    pManager (pManager),
    pSession (true),
#if defined(MIKTEX_WINDOWS)
    numCoInitialize (0),
#endif
    trace_error (TraceStream::Open(MIKTEX_TRACE_ERROR)),
    trace_mpm (TraceStream::Open(MIKTEX_TRACE_MPM))

{
  MIKTEX_ASSERT (PackageLevel::None < PackageLevel::Essential
		 && PackageLevel::Essential < PackageLevel::Basic
		 && PackageLevel::Basic < PackageLevel::Advanced
		 && PackageLevel::Advanced < PackageLevel::Complete);
  
  // get the default remote repository URL
  string repository;
  if (pManager->TryGetDefaultPackageRepository(repositoryType, repository))
    {
      trace_mpm->WriteFormattedLine
	("libmpm",
	 T_("using default package repository: %s"),
	 repository.c_str());
      SetRepository (repository);
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::MyCoInitialize
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
void
PackageInstallerImpl::MyCoInitialize ()
{
  HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
  if (FAILED(hr))
    {
      FATAL_MIKTEX_ERROR ("PackageInstallerImpl::MyCoInitialize",
			  T_("The COM library could not be initialized."),
			  NUMTOSTR(hr));
    }
  ++ numCoInitialize;
}
#endif

/* _________________________________________________________________________

   PackageInstallerImpl::MyCoUninitialize
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
void
PackageInstallerImpl::MyCoUninitialize ()
{
  if (numCoInitialize == 0)
    {
      UNEXPECTED_CONDITION ("PackageInstallerImpl::MyCoUninitialize");
    }
  CoUninitialize ();
  -- numCoInitialize;
}
#endif

/* _________________________________________________________________________

   PackageInstallerImpl::SetCallback
   _________________________________________________________________________ */

void
PackageInstallerImpl::SetCallback
(/*[in]*/ PackageInstallerCallback * pCallback)
{
  this->pCallback = pCallback;
}

/* _________________________________________________________________________

   PackageInstallerImpl::~PackageInstallerImpl
   _________________________________________________________________________ */

PackageInstallerImpl::~PackageInstallerImpl ()
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

   PackageInstallerImpl::OnProgress
   _________________________________________________________________________ */

void
PackageInstallerImpl::OnProgress ()
{
  Notify ();
}

/* _________________________________________________________________________

   PackageInstallerImpl::Download

   Download a file.
   _________________________________________________________________________ */

void
PackageInstallerImpl::Download (/*[in]*/ const string &		url,
				/*[in]*/ const PathName &	dest,
				/*[in]*/ size_t			expectedSize)
{
  trace_mpm->WriteFormattedLine
    ("libmpm",
     T_("going to download: %s => %s"),
     Q_(url),
     Q_(dest));
  
  if (expectedSize > 0)
    {
      ReportLine (T_("downloading %s (expecting %u bytes)..."),
		  Q_(url),
		  static_cast<unsigned>(expectedSize));
    }
  else
    {
      ReportLine (T_("downloading %s..."), Q_(url));
    }

  // open the remote file
  auto_ptr<WebFile>
    webFile (pManager->GetWebSession()->OpenUrl(url.c_str()));

  // open the local file
  TempFile downloadedFile;
  FileStream
    destStream (File::Open(dest,
			   FileMode::Create,
			   FileAccess::Write,
			   false));
  downloadedFile = dest;

  // receive the data
  trace_mpm->WriteFormattedLine ("libmpm",
				 T_("start writing on %s"),
				 Q_(dest));
  char buf[1024];
  size_t n;
  size_t received = 0;
  clock_t start = clock();
  for (clock_t start1 = start;
       (n = webFile->Read(buf, sizeof(buf))) > 0;
       start1 = clock())
    {
      clock_t end1 = clock();

      destStream.Write (buf, n);

      received += n;

      // update progress info
      MIKTEX_LOCK(ProgressIndicator)
	{
	  progressInfo.cbPackageDownloadCompleted += n;
	  progressInfo.cbDownloadCompleted += n;
	  if (end1 > start1)
	    {
	      progressInfo.bytesPerSecond =
		static_cast<unsigned long>
		((static_cast<double>(n) / (end1 - start1))
		 * CLOCKS_PER_SEC);
	    }
	  double timePassed = clock() - timeStarted;
	  double timeTotal =
	    ((timePassed / progressInfo.cbDownloadCompleted)
	     * progressInfo.cbDownloadTotal);
	  progressInfo.timeRemaining =
	    static_cast<unsigned long>
	    ((timeTotal - timePassed) / CLOCKS_PER_SEC);
	}
      MIKTEX_UNLOCK();

      Notify ();
    }

  // close files
  destStream.Close ();
  webFile->Close ();

  clock_t end = clock();

  if (start == end)
    {
      ++ end;
    }

  // report statistics
  trace_mpm->WriteFormattedLine ("libmpm",
				 T_("downloaded %u bytes in %u milliseconds"),
				 received,
				 ((end - start) * CLOCKS_PER_SEC) / 1000);
  ReportLine (T_("%u bytes, %.2f KB/Sec"),
	      received,
	      ((((static_cast<double>(received)
		  / static_cast<double>(end - start)))
		* CLOCKS_PER_SEC)
	       / 1024));
  
  if (expectedSize > 0 && expectedSize != received)
    {
      FatalError (ERROR_SIZE_MISMATCH,
		  dest.Get(),
		  expectedSize,
		  received);
    }

  // keep the downloaded file
  downloadedFile = 0;
}

/* _________________________________________________________________________

   PackageInstallerImpl::OnBeginFileExtraction
   _________________________________________________________________________ */

void
PackageInstallerImpl::OnBeginFileExtraction
  (/*[in]*/ const char *	lpszFileName,
   /*[in]*/ size_t		uncompressedSize)
{
  UNUSED_ALWAYS (uncompressedSize);

  // update progress info
  MIKTEX_LOCK(ProgressIndicator)
    {
      progressInfo.fileName = lpszFileName;
    }
  MIKTEX_UNLOCK();

  // update file name database
  if (autoFndbSync && lpszFileName != 0)
    {
      if (! Fndb::FileExists(lpszFileName))
	{
	  Fndb::Add (lpszFileName);
	}
    }

  // notify client: beginning of file extraction
  Notify (Notification::InstallFileStart);
}

/* _________________________________________________________________________

   PackageInstallerImpl::OnEndFileExtraction
   _________________________________________________________________________ */

void
PackageInstallerImpl::OnEndFileExtraction
  (/*[in]*/ const char *	lpszFileName,
   /*[in]*/ size_t		uncompressedSize)
{
  // update file name database
  if (autoFndbSync && lpszFileName != 0)
    {
      if (! Fndb::FileExists(lpszFileName))
	{
	  Fndb::Add (lpszFileName);
	}
    }

  // update progress info
  MIKTEX_LOCK(ProgressIndicator)
    {
      progressInfo.fileName = "";
      progressInfo.cFilesPackageInstallCompleted += 1;
      progressInfo.cFilesInstallCompleted += 1;
      progressInfo.cbPackageInstallCompleted += uncompressedSize;
      progressInfo.cbInstallCompleted += uncompressedSize;
    }
  MIKTEX_UNLOCK();

  // notify client: end of file extraction
  Notify (Notification::InstallFileEnd);
}

/* _________________________________________________________________________

   PackageInstallerImpl::OnError
   _________________________________________________________________________ */

bool
PackageInstallerImpl::OnError (/*[in]*/ const char *	lpszMessage)
{
  // we have a problem: let the client decide how to proceed
  return (! AbortOrRetry(lpszMessage));
}

/* _________________________________________________________________________

   PackageInstallerImpl::ExtractFiles
   _________________________________________________________________________ */

void
PackageInstallerImpl::ExtractFiles
  (/*[in]*/ const PathName &	archiveFileName,
   /*[in]*/ ArchiveFileType	archiveFileType)
{
  auto_ptr<MiKTeX::Extractor::Extractor>
    pExtractor
    (MiKTeX::Extractor::Extractor::CreateExtractor(archiveFileType));
  pExtractor->Extract (archiveFileName,
		       pSession->GetSpecialPath(SpecialPath::InstallRoot),
		       true,
		       this,
		       TEXMF_PREFIX_DIRECTORY);
  pExtractor->Dispose ();
}

/* _________________________________________________________________________

   PackageInstallerImpl::InstallDbLight

   Download the lightweight database file from the package repository
   and write mpm.ini.
   _________________________________________________________________________ */

void
PackageInstallerImpl::InstallDbLight ()
{
  // we must have a package repository
  if (repositoryType == RepositoryType::Unknown)
    {
      repository = pManager->PickRepositoryUrl();
      repositoryType = RepositoryType::Remote;
    }

  ReportLine (T_("visiting repository %s..."), Q_(repository));
  ReportLine (T_("repository type: %s"),
	      (repositoryType == RepositoryType::Remote
	       ? T_("remote package repository")
	       : (repositoryType == RepositoryType::Local
		  ? T_("local package repository")
		  : "MiKTeXDirect")));
  
  // path to config dir
  PathName pathConfigDir (
    pSession->GetSpecialPath(SpecialPath::InstallRoot),
    MIKTEX_PATH_MIKTEX_CONFIG_DIR);

  if (repositoryType == RepositoryType::Remote
      || repositoryType == RepositoryType::Local)
    {
      // we need a scratch directory when we download from the
      // Internet
      ScratchDirectory scratchDirectory;

      ReportLine (T_("loading lightweight database..."));

      // full path to the database file
      PathName pathZzdb1;

      // pick up the database file
      if (repositoryType == RepositoryType::Remote)
	{
	  // create a scratch directory and make it current
	  scratchDirectory.Enter ();

	  PathName curDir;
	  curDir.SetToCurrentDirectory ();
	  
	  pathZzdb1.Set (curDir, MIKTEX_MPM_DB_LIGHT_FILE_NAME);
	  
	  // update progress indicator
	  MIKTEX_LOCK(ProgressIndicator)
	    {
	      progressInfo.deploymentName =
		MIKTEX_MPM_DB_LIGHT_FILE_NAME_NO_SUFFIX;
	      progressInfo.displayName = T_("Lightweight package database");
	      progressInfo.cbPackageDownloadCompleted = 0;
	      progressInfo.cbPackageDownloadTotal = MPM_APSIZE_DB_LIGHT;
	    }
	  MIKTEX_UNLOCK();
	  
	  // download the database file
	  Download (MakeUrl(MIKTEX_MPM_DB_LIGHT_FILE_NAME), pathZzdb1);
	}
      else
	{
	  MIKTEX_ASSERT (repositoryType == RepositoryType::Local);
	  pathZzdb1.Set (repository, MIKTEX_MPM_DB_LIGHT_FILE_NAME);
	}

      // unpack database
      auto_ptr<MiKTeX::Extractor::Extractor>
	pExtractor
	(MiKTeX::Extractor::Extractor::CreateExtractor(DB_ARCHIVE_FILE_TYPE));
      pExtractor->Extract (pathZzdb1,
			   pathConfigDir,
			   false,
			   0,
			   0);
      pExtractor->Dispose ();
    }
  else if (repositoryType == RepositoryType::MiKTeXDirect)
    {
      PathName pathMpmIniSrc (repository);
      pathMpmIniSrc += MIKTEXDIRECT_PREFIX_DIR;
      pathMpmIniSrc += MIKTEX_PATH_MPM_INI;
      PathName pathMpmIniDst =
	pSession->GetSpecialPath(SpecialPath::InstallRoot);
      pathMpmIniDst += MIKTEX_PATH_MPM_INI;
      size_t size;
      MyCopyFile (pathMpmIniSrc, pathMpmIniDst, size);
    }
  else
    {
      UNEXPECTED_CONDITION ("PackageInstallerImpl::InstallDbLight");
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::LoadDbLight
   _________________________________________________________________________ */

void
PackageInstallerImpl::LoadDbLight (/*[in]*/ bool download)
{
  dbLight.Clear ();

  // path to mpm.ini
  PathName pathMpmIni (
    pSession->GetSpecialPath(SpecialPath::InstallRoot),
    MIKTEX_PATH_MPM_INI);

  // install (if necessary)
  if (download || ! File::Exists(pathMpmIni))
    {
      InstallDbLight ();
    }

  // load mpm.ini
  dbLight.Read (pathMpmIni);
  
  // report digest
  MD5 md5 = MD5::FromFile(pathMpmIni.Get());
  ReportLine (T_("lightweight database digest: %s"),
	      dbLight.GetDigest().ToString().c_str());
}

/* _________________________________________________________________________

   CompareVersions
   _________________________________________________________________________ */

int
CompareVersions (/*[in]*/ const string &	ver1,
		 /*[in]*/ const string &	ver2)
{
  if (ver1.empty() || ver2.empty())
    {
      return (0);
    }
  VersionNumber verNum1;
  VersionNumber verNum2;
  if (VersionNumber::TryParse(ver1.c_str(), verNum1)
      && VersionNumber::TryParse(ver2.c_str(), verNum2))
    {
      return (verNum1.CompareTo(verNum2));
    }
  else
    {
      return (-1);
    }
}

/* _________________________________________________________________________

   CompareSerieses
   _________________________________________________________________________ */

int
CompareSerieses (/*[in]*/ const string &	ver1,
		 /*[in]*/ const string &	ver2)
{
  if (ver1.empty() || ver2.empty())
    {
      return (0);
    }
  VersionNumber verNum1;
  VersionNumber verNum2;
  if (VersionNumber::TryParse(ver1.c_str(), verNum1)
      && VersionNumber::TryParse(ver2.c_str(), verNum2))
    {
      verNum1.n3 = 0;
      verNum1.n4 = 0;
      verNum2.n3 = 0;
      verNum2.n4 = 0;
      return (verNum1.CompareTo(verNum2));
    }
  else
    {
      return (-1);
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::FindUpdates
   _________________________________________________________________________ */

void
PackageInstallerImpl::FindUpdates ()
{
  trace_mpm->WriteLine ("libmpm", T_("searching for updateable packages"));

  // force a download of the lightweight database
  LoadDbLight (true);

  updates.clear ();

  // package iteration
  char szPackage[BufferSizes::MaxPackageName];
  for (const char * lpszPackage = dbLight.FirstPackage(szPackage);
       lpszPackage != 0;
       lpszPackage = dbLight.NextPackage(szPackage))
    {
      Notify ();

      // initialize update info
      UpdateInfo updateInfo;
      updateInfo.deploymentName = lpszPackage;
      updateInfo.timePackaged = dbLight.GetTimePackaged(lpszPackage);

      // get local package definition
      const PackageInfo * pPackageInfo
	= pManager->TryGetPackageInfo(lpszPackage);
      if (pPackageInfo == 0 || ! pManager->IsPackageInstalled(lpszPackage))
	{
#if defined(MIKTEX_WINDOWS)
	  // package not known/installed => must be installed if it is
	  // an essential MiKTeX package in the current series
	  if ((dbLight.GetPackageLevel(lpszPackage).Get()
	       <= PackageLevel::Essential)
	      && IsMiKTeXPackage(lpszPackage))
	    {
	      string version = dbLight.GetPackageVersion(lpszPackage);
	      int verCmp = CompareSerieses(version, MIKTEX_SERIES_STR);
	      if (verCmp == 0)
		{
		  trace_mpm->WriteFormattedLine ("libmpm",
						 T_("%s: new MiKTeX package"),
						 lpszPackage);
		  updates.push_back (updateInfo);
		}
	    }
#endif
	}
      else
	{
	  // check the integrity of installed MiKTeX packages
	  if (IsMiKTeXPackage(lpszPackage)
	      && ! pManager->TryVerifyInstalledPackage(lpszPackage))
	    {
	      // the package has been tampered with
	      updateInfo.timePackaged = static_cast<time_t>(-1);
	      updates.push_back (updateInfo);
	      continue;
	    }

	  // compare digests, version numbers and time stamps
	  MD5 md5 = dbLight.GetPackageDigest(lpszPackage);
	  if (md5 == pPackageInfo->digest)
	    {
	      // digests do match => no update necessary
	      continue;
	    }
	  trace_mpm->WriteFormattedLine
	    ("libmpm",
	     T_("%s: server has a different version"),
	     lpszPackage);
	  trace_mpm->WriteFormattedLine
	    ("libmpm",
	     T_("server digest: %s"),
	     md5.ToString().c_str());
	  trace_mpm->WriteFormattedLine
	    ("libmpm",
	     T_("local digest: %s"),
	     pPackageInfo->digest.ToString().c_str());
	  int verCmp =
	    CompareVersions(dbLight.GetPackageVersion(lpszPackage),
			    pPackageInfo->version);
	  if (verCmp < 0)
	    {
	      // server has a previous version => no update necessary
	      continue;
	    }
	  else if (verCmp == 0)
	    {
	      // the version numbers are equal => compare time stamps
	      time_t timePackaged = dbLight.GetTimePackaged(lpszPackage);
	      if (timePackaged <= pPackageInfo->timePackaged)
		{
		  // server has an older package => no update
		  // necessary
		  continue;
		}
	      // server has a newer package
	      trace_mpm->WriteFormattedLine
		("libmpm",
		 T_("%s: server has new version"),
		 lpszPackage);
	    }
	  else
	    {
	      // server has a newer version
	      trace_mpm->WriteFormattedLine
		("libmpm",
		 T_("%s: server has new version"),
		 lpszPackage);
	    }
	  updates.push_back (updateInfo);
	}
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::FindUpdatesAsync
   _________________________________________________________________________ */

void
PackageInstallerImpl::FindUpdatesAsync ()
{
  StartTaskThread (FindUpdatesThread);
}

/* _________________________________________________________________________

   PackageInstallerImpl::FindUpdatesThread
   _________________________________________________________________________ */

void
PackageInstallerImpl::FindUpdatesThread (/*[in]*/ void * pv)
{
  PackageInstallerImpl * This = static_cast<PackageInstallerImpl*>(pv);
#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
  HRESULT hr = E_FAIL;
#endif
  try
    {
#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
      hr = CoInitializeEx(0, COINIT_MULTITHREADED);
      if (FAILED(hr))
	{
	  FATAL_MPM_ERROR ("PackageInstallerImpl::FindUpdatesThread",
			   T_("Cannot start updater thread."),
			   NUMTOHEXSTR(hr));
	}
#endif
      This->FindUpdates ();
      This->progressInfo.ready = true;
      This->Notify ();
    }
  catch (const OperationCancelledException & e)
    {
      This->progressInfo.ready = true;
      This->progressInfo.cancelled = true;
      This->threadMiKTeXException = e;
    }
  catch (const MiKTeXException & e)
    {
      This->progressInfo.ready = true;
      This->progressInfo.numErrors += 1;
      This->threadMiKTeXException = e;
    }
#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
  if (SUCCEEDED(hr))
    {
      CoUninitialize ();
    }
#endif
}

/* _________________________________________________________________________

   PackageInstallerImpl::RemoveFiles
   _________________________________________________________________________ */

void
PackageInstallerImpl::RemoveFiles
  (/*[in]*/ const vector<string> &	toBeRemoved,
   /*[in]*/ bool			silently)
{
  for (vector<string>::const_iterator it = toBeRemoved.begin();
       it != toBeRemoved.end();
       ++ it)
    {
      Notify (Notification::RemoveFileStart);

      // only consider texmf files
      string fileName;
      if (! PackageManager::StripTeXMFPrefix(*it, fileName))
	{
	  continue;
	}

      bool done = false;
      
      // get information about the installed file
      InstalledFileInfo * pInstalledFileInfo
	= pManager->GetInstalledFileInfo(it->c_str());
      
      // decrement the file reference counter
      if (pInstalledFileInfo != 0 && pInstalledFileInfo->refCount > 0)
	{
	  pInstalledFileInfo->refCount -= 1;
	}
      
      // make an absolute path name
      PathName path (
	pSession->GetSpecialPath(SpecialPath::InstallRoot),
	fileName);
      
      // only delete if the reference count reached zero
      if (pInstalledFileInfo != 0 && pInstalledFileInfo->refCount > 0)
	{
	  trace_mpm->WriteFormattedLine
	    ("libmpm",
	     T_("will not delete %s (ref count is %u)"),
	     Q_(path),
	     pInstalledFileInfo->refCount);
	  done = true;
	}
      else if (File::Exists(path))
	{
	  // remove the file
	  try
	    {
	      File::Delete (path, true, autoFndbSync);
	      done = true;
	    }
	  catch (const MiKTeXException & e)
	    {
	      done = false;
	      if (! silently)
		{
		  FatalError (ERROR_CANNOT_DELETE,
			      path.Get(),
			      e.what());
		}
	    }
	}
      else
	{
	  trace_mpm->WriteFormattedLine
	    ("libmpm",
	     T_("file %s does not exist"),
	     Q_(path));
	  done = true;
	}
      
	  
      // remove from MPM FNDB
#if 0
      if (autoFndbSync
	    && Fndb::Exists(PathName(pSession->GetMpmRootPath(), fileName)))
	{
	  Fndb::Remove (PathName(pSession->GetMpmRootPath(), fileName));
	}
#endif
      
      // update progress info
      if (done && ! silently)
	{
	  MIKTEX_LOCK(ProgressIndicator)
	    {
	      progressInfo.cFilesRemoveCompleted += 1;
	    }
	  MIKTEX_UNLOCK();
	}
      
      // notify client
      Notify (Notification::RemoveFileEnd);
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::RemovePackage
   _________________________________________________________________________ */

void
PackageInstallerImpl::RemovePackage (/*[in]*/ const string &	deploymentName)
{
  trace_mpm->WriteFormattedLine ("libmpm",
				 T_("going to remove %s"),
				 Q_(deploymentName));

  // notify client
  Notify (Notification::RemovePackageStart);
  ReportLine (T_("removing package %s..."), Q_(deploymentName));

  // get package info
  PackageInfo * pPackageInfo =
    pManager->TryGetPackageInfo(deploymentName.c_str());
  if (pPackageInfo == 0)
    {
      FatalError (ERROR_UNKNOWN_PACKAGE, deploymentName.c_str());
    }

  // check to see whether it is installed
  if (pManager->GetTimeInstalled(deploymentName.c_str()) == 0)
    {
      FatalError (ERROR_PACKAGE_NOT_INSTALLED, deploymentName.c_str());
    }

  // clear the installTime value => package is not installed
  trace_mpm->WriteFormattedLine
    ("libmpm",
     T_("removing %s from the variable package table"),
     Q_(deploymentName));
  pManager->SetTimeInstalled (deploymentName.c_str(), 0);
  pManager->FlushVariablePackageTable ();
  pPackageInfo->timeInstalled = 0;

  if (pManager->IsPackageObsolete(deploymentName.c_str()))
    {
      // it's an obsolete package: make sure that the package
      // definition file gets removed too
      AddToFileList (pPackageInfo->runFiles,
		     PrefixedPackageDefinitionFile(deploymentName));
    }
  else
    {
      // make sure that the package definition file does not get removed
      RemoveFromFileList (pPackageInfo->runFiles,
			  PrefixedPackageDefinitionFile(deploymentName));
    }

  // remove the files
  size_t nTotal = (pPackageInfo->runFiles.size()
		   + pPackageInfo->docFiles.size()
		   + pPackageInfo->sourceFiles.size());
  trace_mpm->WriteFormattedLine ("libmpm",
				 T_("going to remove %u file(s)"),
				 static_cast<unsigned>(nTotal));
  RemoveFiles (pPackageInfo->runFiles);
  RemoveFiles (pPackageInfo->docFiles);
  RemoveFiles (pPackageInfo->sourceFiles);

  trace_mpm->WriteFormattedLine ("libmpm",
				 T_("package %s successfully removed"),
				 Q_(deploymentName));
  
  // update progress info
  MIKTEX_LOCK(ProgressIndicator)
    {
      progressInfo.cPackagesRemoveCompleted += 1;
    }
  MIKTEX_UNLOCK();

  // notify client
  Notify (Notification::RemovePackageEnd);
}

/* _________________________________________________________________________

   MyCopyFile
   _________________________________________________________________________ */

void
PackageInstallerImpl::MyCopyFile (/*[in]*/ const PathName &	source,
				  /*[in]*/ const PathName &	dest,
				  /*[out]*/ size_t &		size)
{
  // reset the read-only attribute, if the destination file exists
  if (File::Exists(dest))
    {
      FileAttributes attributesOld = File::GetAttributes(dest);
      FileAttributes attributesNew = attributesOld;
      if ((attributesOld & FileAttributes::ReadOnly) != 0)
	{
	  attributesNew &= ~ FileAttributes(FileAttributes::ReadOnly);
	}
#if defined(MIKTEX_WINDOWS)
      if ((attributesOld & FileAttributes::Hidden) != 0)
	{
	  attributesNew &= ~ FileAttributes(FileAttributes::Hidden);
	}
#endif
      if (attributesOld.Get() != attributesNew.Get())
	{
	  File::SetAttributes (dest, attributesNew);
	}
    }

  FILE * pfileTo;

  // open the destination file
  do
    {
      try
	{
	  pfileTo =
	    File::Open(dest,
		       FileMode::Create,
		       FileAccess::Write,
		       false);
	}
      catch (const MiKTeXException & e)
	{
	  ostringstream text;
	  text << T_("\
The following file could not be written:")
	       << LF
	       << LF
	       << "  " << dest.Get()
	       << LF
	       << LF
	       << T_("\
The write operation failed for the following reason:")
	       << LF
	       << LF
	       << "  " << e.what()
	       << LF
	       << LF
	       << T_("\
Make sure that no other application uses the file and that \
you have write permission on the file.");
	  if (AbortOrRetry(text.str().c_str()))
	    {
	      throw;
	    }
	  pfileTo = 0;
	}
    }
  while (pfileTo == 0);

  FileStream toStream (pfileTo);

  // open the source file
  FileStream fromStream (File::Open(source,
				    FileMode::Open,
				    FileAccess::Read,
				    false));

  // copy the file
  char buffer[4096];
  size_t n;
  size = 0;
  while ((n = fromStream.Read(buffer, 4096)) > 0)
    {
      toStream.Write (buffer, n);
      size += n;
    }

  fromStream.Close ();
  toStream.Close ();

  if (autoFndbSync)
    {
      if (! Fndb::FileExists(dest))
	{
	  Fndb::Add (dest);
	}
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::CopyFiles
   _________________________________________________________________________ */

void
PackageInstallerImpl::CopyFiles (/*[in]*/ const PathName & pathSourceRoot,
				 /*[in]*/ const vector<string> & fileList)
{
  for (vector<string>::const_iterator it = fileList.begin();
       it != fileList.end();
       ++ it)
    {
      Notify ();

      // only consider texmf files
      string fileName;
      if (! (PackageManager::StripTeXMFPrefix(*it, fileName)))
	{
	  continue;
	}

      // make sure the source file exists
      PathName pathSource (pathSourceRoot, fileName);
      if (! File::Exists(pathSource))
	{
	  FatalError (ERROR_SOURCE_FILE_NOT_FOUND, pathSource.Get());
	}

      PathName pathDest (pSession->GetSpecialPath(SpecialPath::InstallRoot), fileName);

      PathName pathDestFolder (pathDest);
      pathDestFolder.RemoveFileSpec ();
      
      // notify client: beginning of file copy operation
      Notify (Notification::InstallFileStart);

      // create the destination folder
      Directory::Create (pathDestFolder);

      MIKTEX_LOCK(ProgressIndicator)
	{
	  progressInfo.fileName = pathDest;
	}
      MIKTEX_UNLOCK();

      size_t size;

      // copy the file
      MyCopyFile (pathSource, pathDest, size);

      // update progress info
      MIKTEX_LOCK(ProgressIndicator)
	{
	  progressInfo.fileName = "";
	  progressInfo.cFilesPackageInstallCompleted += 1;
	  progressInfo.cFilesInstallCompleted += 1;
	  progressInfo.cbPackageInstallCompleted += size;
	  progressInfo.cbInstallCompleted += size;
	}
      MIKTEX_UNLOCK();

      // notify client: end of file copy operation
      Notify (Notification::InstallFileEnd);
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::AddToFileList
   _________________________________________________________________________ */

void
PackageInstallerImpl::AddToFileList
(/*[in,out]*/ vector<string> &		fileList,
 /*[in]*/ const PathName &		fileName)
  const
{
  // avoid duplicates
  for (vector<string>::const_iterator it = fileList.begin();
       it != fileList.end();
       ++ it)
    {
      if (PathName::Compare(it->c_str(), fileName.Get()) == 0)
	{
	  return;
	}
    }
  fileList.push_back (fileName.Get());
}

/* _________________________________________________________________________

   PackageInstallerImpl::RemoveFromFileList
   _________________________________________________________________________ */

void
PackageInstallerImpl::RemoveFromFileList
(/*[in,out]*/ vector<string> &	fileList,
 /*[in]*/ const PathName &	fileName)
  const
{
  for (vector<string>::iterator it = fileList.begin();
       it != fileList.end();
       ++ it)
    {
      if (PathName::Compare(it->c_str(), fileName.Get()) == 0)
	{
	  fileList.erase (it);
	  return;
	}
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::CopyPackage
   _________________________________________________________________________ */

void
PackageInstallerImpl::CopyPackage (/*[in]*/ const PathName & pathSourceRoot,
				   /*[in]*/ const string & deploymentName)
{
  // parse the package definition file
  PathName pathPackageFile = pathSourceRoot;
  pathPackageFile += MIKTEX_PATH_PACKAGE_DEFINITION_DIR;
  pathPackageFile += deploymentName;
  pathPackageFile.AppendExtension (MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX);
  TpmParser tpmparser;
  tpmparser.Parse (pathPackageFile);

  // get the package info from the parser; set the package name
  PackageInfo packageInfo = tpmparser.GetPackageInfo();
  packageInfo.deploymentName = deploymentName;

  // make sure that the package definition file is included in the
  // file list
  AddToFileList (packageInfo.runFiles,
		 PrefixedPackageDefinitionFile(deploymentName));

  // copy the files
  CopyFiles (pathSourceRoot, packageInfo.runFiles);
  CopyFiles (pathSourceRoot, packageInfo.docFiles);
  CopyFiles (pathSourceRoot, packageInfo.sourceFiles);
}

/* _________________________________________________________________________

   GetFiles
   _________________________________________________________________________ */

#if defined(USE_HASH_SET)
typedef hash_set<string> StringSet;
#else
typedef set<string> StringSet;
#endif

MPMSTATICFUNC(void)
GetFiles (/*[in]*/ const PackageInfo &	packageInfo,
	  /*[out]*/ StringSet &		files)
{
  files.insert (packageInfo.runFiles.begin(), packageInfo.runFiles.end());
  files.insert (packageInfo.docFiles.begin(), packageInfo.docFiles.end());
  files.insert (packageInfo.sourceFiles.begin(),
		packageInfo.sourceFiles.end());
}

/* _________________________________________________________________________

   PackageInstallerImpl::UpdateMpmFndb
   _________________________________________________________________________ */

void
PackageInstallerImpl::UpdateMpmFndb
  (/*[in]*/ const vector<string> &	installedFiles,
   /*[in]*/ const vector<string> &	removedFiles,
   /*[in]*/ const char *		lpszPackageName)
{
#if 0
  ReportLine (T_("updating MPM file name database:"));
  ReportLine (T_("  %u records to be added"), installedFiles.size());
  ReportLine (T_("  %u records to be removed"), removedFiles.size());
#endif
  vector<string>::const_iterator it;
  for (it = installedFiles.begin(); it != installedFiles.end(); ++ it)
    {
      PathName path (pSession->GetMpmRootPath(), *it);
      if (! Fndb::FileExists(path))
	{
	  Fndb::Add (path, lpszPackageName);
	}
      else
	{
	  trace_mpm->WriteFormattedLine ("libmpm",
					 T_("%s already exists in mpm fndb"),
					 Q_(path));
	}
     }
  for (it = removedFiles.begin(); it != removedFiles.end(); ++ it)
    {
      PathName path (pSession->GetMpmRootPath(), *it);
      if (Fndb::FileExists(path))
	{
	  Fndb::Remove (path);
	}
      else
	{
	  trace_mpm->WriteFormattedLine ("libmpm",
					 T_("%s does not exist in mpm fndb"),
					 Q_(path));
	}
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::InstallPackage
   _________________________________________________________________________ */

void
PackageInstallerImpl::InstallPackage (/*[in]*/ const string &	deploymentName)
{
  trace_mpm->WriteFormattedLine ("libmpm",
				 T_("installing package %s"),
				 Q_(deploymentName));
  
  // search the package table
  PackageInfo * pPackageInfo =
    pManager->TryGetPackageInfo(deploymentName.c_str());
  if (pPackageInfo == 0)
    {
      FatalError (ERROR_UNKNOWN_PACKAGE, deploymentName.c_str());
    }

  // initialize progress info
  MIKTEX_LOCK(ProgressIndicator)
    {
      progressInfo.deploymentName = deploymentName;
      progressInfo.displayName = pPackageInfo->displayName;
      progressInfo.cFilesPackageInstallCompleted = 0;
      progressInfo.cFilesPackageInstallTotal = pPackageInfo->GetNumFiles();
      progressInfo.cbPackageInstallCompleted = 0;
      progressInfo.cbPackageInstallTotal = pPackageInfo->GetSize();
      if (repositoryType == RepositoryType::Remote)
	{
	  progressInfo.cbPackageDownloadCompleted = 0;
	  progressInfo.cbPackageDownloadTotal =
	    dbLight.GetArchiveFileSize(deploymentName.c_str());
	}
    }
  MIKTEX_UNLOCK();

  // notify client: beginning of package installation
  Notify (Notification::InstallPackageStart);

  PathName pathArchiveFile;
  ArchiveFileType aft = dbLight.GetArchiveFileType(deploymentName.c_str());
  TempFile downloadedFile;

  // get hold of the archive file
  if (repositoryType == RepositoryType::Remote
      || repositoryType == RepositoryType::Local)
    {
      PathName
	packageFileName (0,
			 deploymentName,
			 ArchiveFileType::GetFileNameExtension(aft.Get()));

      if (repositoryType == RepositoryType::Remote)
	{
	  // take hold of the package
	  pathArchiveFile.SetToTempFile ();
	  Download (MakeUrl(packageFileName.Get()), pathArchiveFile);
	  downloadedFile = pathArchiveFile;
	}
      else
	{
	  MIKTEX_ASSERT (repositoryType == RepositoryType::Local);
	  pathArchiveFile.Set
	    (repository,
	     deploymentName,
	     ArchiveFileType::GetFileNameExtension(aft.Get()));
	}
      
      // check to see whether the digest is good
      if (! CheckArchiveFile(deploymentName.c_str(),
			     pathArchiveFile,
			     false))
	{
	  LoadDbLight (true);
	  CheckArchiveFile (deploymentName.c_str(),
			    pathArchiveFile,
			    true);
	}
    }

  // silently uninstall the package (this also decrements the file
  // reference counts)
  if (pManager->IsPackageInstalled(deploymentName.c_str()))
    {
      trace_mpm->WriteFormattedLine ("libmpm",
				     T_("%s: removing old files"),
				     deploymentName.c_str());
      // make sure that the package info file does not get removed
      RemoveFromFileList (pPackageInfo->runFiles,
			  PrefixedPackageDefinitionFile(deploymentName));
      RemoveFiles (pPackageInfo->runFiles, true);
      RemoveFiles (pPackageInfo->docFiles, true);
      RemoveFiles (pPackageInfo->sourceFiles, true);
      // temporarily set the status to "not installed"
      pManager->SetTimeInstalled (deploymentName.c_str(), 0);
      pManager->FlushVariablePackageTable ();
    }

  if (repositoryType == RepositoryType::Remote
      || repositoryType == RepositoryType::Local)
    {
      // unpack the archive file
      ReportLine
	(T_("extracting files from %s..."),
	 Q_(PathName(0,
		     deploymentName,
		     ArchiveFileType::GetFileNameExtension(aft.Get()))));
      ExtractFiles (pathArchiveFile, aft);
      downloadedFile.Delete ();
    }
  else if (repositoryType == RepositoryType::MiKTeXDirect)
    {
      // copy from CD
      PathName pathSourceRoot (repository);
      pathSourceRoot += MIKTEXDIRECT_PREFIX_DIR;
      CopyPackage (pathSourceRoot, deploymentName);
    }
  else if (repositoryType == RepositoryType::MiKTeXInstallation)
    {
      // import from another MiKTeX installation
      ReportLine (T_("importing package %s..."), deploymentName.c_str());
      PathName pathSourceRoot (repository);
      CopyPackage (pathSourceRoot, deploymentName);
    }
  else
    {
      UNEXPECTED_CONDITION ("PackageInstallerImpl::InstallPackage");
    }

  // parse the new package definition file
  PathName pathPackageFile
    (PathName(pSession->GetSpecialPath(SpecialPath::InstallRoot),
		MIKTEX_PATH_PACKAGE_DEFINITION_DIR),
     deploymentName,
     MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX);
  TpmParser tpmparser;
  tpmparser.Parse (pathPackageFile);

  // get new package info
  PackageInfo packageInfo = tpmparser.GetPackageInfo();
  packageInfo.deploymentName = deploymentName;

  // find recycled and brand new files
  StringSet set1;
  GetFiles (*pPackageInfo, set1);
  StringSet set2;
  GetFiles (packageInfo, set2);
  StringSet::const_iterator it;
  vector<string> recycledFiles;
  for (it = set1.begin(); it != set1.end(); ++ it)
    {
      if (set2.find(*it) == set2.end())
	{
	  string str;
	  if (PackageManager::StripTeXMFPrefix(*it, str))
	    {
	      recycledFiles.push_back (str);
	    }
	}
    }
  vector<string> newFiles;
  for (it = set2.begin(); it != set2.end(); ++ it)
    {
      if (set1.find(*it) == set1.end())
	{
	  string str;
	  if (PackageManager::StripTeXMFPrefix(*it, str))
	    {
	      newFiles.push_back (str);
	    }
	}
    }

  // update the MPM file name database
  if (autoFndbSync)
    {
      UpdateMpmFndb (newFiles, recycledFiles, deploymentName.c_str());
    }

  // set the timeInstalled value => package is installed
  packageInfo.timeInstalled = time(0);
  pManager->SetTimeInstalled (deploymentName.c_str(),
			      packageInfo.timeInstalled);
  pManager->FlushVariablePackageTable ();

  // update package info table
  *pPackageInfo = packageInfo;

  // increment file ref counts
  pManager->IncrementFileRefCounts (deploymentName);

  // update progress info
  MIKTEX_LOCK(ProgressIndicator)
    {
      progressInfo.cPackagesInstallCompleted += 1;
    }
  MIKTEX_UNLOCK();

  // notify client: end of package installation
  Notify (Notification::InstallPackageEnd);
}

/* _________________________________________________________________________

   PackageInstallerImpl::DownloadPackage
   _________________________________________________________________________ */

void
PackageInstallerImpl::DownloadPackage (/*[in]*/ const string & deploymentName)
{
  size_t expectedSize;

  // update progress info
  MIKTEX_LOCK(ProgressIndicator)
    {
      progressInfo.deploymentName = deploymentName;
      progressInfo.displayName = deploymentName;
      MIKTEX_ASSERT (repositoryType == RepositoryType::Remote);
      progressInfo.cbPackageDownloadCompleted = 0;
      progressInfo.cbPackageDownloadTotal =
	dbLight.GetArchiveFileSize(deploymentName.c_str());
      expectedSize = progressInfo.cbPackageDownloadTotal;
    }
  MIKTEX_UNLOCK();

  // notify client: beginning of package download
  Notify (Notification::DownloadPackageStart);
  
  // make the archive file name
  ArchiveFileType aft = dbLight.GetArchiveFileType(deploymentName.c_str());
  PathName pathArchiveFile (0,
			    deploymentName,
			    ArchiveFileType::GetFileNameExtension(aft.Get()));
  
  // download the archive file
  Download (pathArchiveFile, expectedSize);

  // check to see whether the archive file is ok
  CheckArchiveFile (deploymentName.c_str(),
		    PathName(pSession->GetSpecialPath(SpecialPath::InstallRoot), pathArchiveFile),
		    true);

  // notify client: end of package download
  Notify (Notification::DownloadPackageEnd);
}

/* _________________________________________________________________________

   PackageInstallerImpl::CalculateExpenditure
   _________________________________________________________________________ */

void
PackageInstallerImpl::CalculateExpenditure (/*[in]*/ bool downloadOnly)
{
  vector<string>::const_iterator it;

  ProgressInfo packageInfo;

  if (! downloadOnly)
    {
      packageInfo.cPackagesInstallTotal =
	static_cast<unsigned long>(toBeInstalled.size());
    }

  for (it = toBeInstalled.begin(); it != toBeInstalled.end(); ++ it)
    {
      if (! downloadOnly)
	{
	  PackageInfo * pPackageInfo =
	    pManager->TryGetPackageInfo(it->c_str());
	  if (pPackageInfo == 0)
	    {
	      FatalError (ERROR_UNKNOWN_PACKAGE, it->c_str());
	    }
	  packageInfo.cFilesInstallTotal += pPackageInfo->GetNumFiles();
	  packageInfo.cbInstallTotal += pPackageInfo->GetSize();
	}
      if (repositoryType == RepositoryType::Remote)
	{
	  int iSize = dbLight.GetArchiveFileSize(it->c_str());
	  if (iSize == 0)
	    {
	      LoadDbLight (true);
	      if ((iSize = dbLight.GetArchiveFileSize(it->c_str())) == 0)
		{
		  FatalError (ERROR_UNKNOWN_PACKAGE, it->c_str());
		}
	    }
	  packageInfo.cbDownloadTotal += iSize;
	}
    }

  if (packageInfo.cbDownloadTotal > 0)
    {
      ReportLine (T_("going to download %u bytes"),
		  packageInfo.cbDownloadTotal);
    }

  if (! downloadOnly && toBeInstalled.size() > 0)
    {
      ReportLine (T_("going to install %u file(s) (%u package(s))"),
		  packageInfo.cFilesInstallTotal,
		  packageInfo.cPackagesInstallTotal);
    }

  if (! downloadOnly && toBeRemoved.size() > 0)
    {
      packageInfo.cPackagesRemoveTotal =
	static_cast<unsigned long>(toBeRemoved.size());
      
      for (it = toBeRemoved.begin(); it != toBeRemoved.end(); ++ it)
	{
	  PackageInfo * pPackageInfo =
	    pManager->TryGetPackageInfo(it->c_str());
	  if (pPackageInfo == 0)
	    {
	      FatalError (ERROR_UNKNOWN_PACKAGE, it->c_str());
	    }
	  packageInfo.cFilesRemoveTotal += pPackageInfo->GetNumFiles();
	}
      
      ReportLine (T_("going to remove %u file(s) (%u package(s))"),
		  packageInfo.cFilesRemoveTotal,
		  packageInfo.cPackagesRemoveTotal);
    }

    MIKTEX_LOCK(ProgressIndicator)
      {
	progressInfo = packageInfo;
      }
    MIKTEX_UNLOCK();
}

/* _________________________________________________________________________

   PackageInstallerImpl::ReadDirectory
   _________________________________________________________________________ */

bool
PackageInstallerImpl::ReadDirectory (/*[in]*/ const char * lpszPath,
				     /*[out]*/ char * * ppSubDirNames,
				     /*[out]*/ char * * ppFileNames,
				     /*[out]*/ char * * ppFileNameInfos)

{
  UNUSED_ALWAYS (lpszPath);
  UNUSED_ALWAYS (ppSubDirNames);
  UNUSED_ALWAYS (ppFileNames);
  UNUSED_ALWAYS (ppFileNameInfos);
  return (false);
}

/* _________________________________________________________________________

   PackageInstallerImpl::OnProgress
   _________________________________________________________________________ */

bool
PackageInstallerImpl::OnProgress (/*[in]*/ unsigned		level,
				  /*[in]*/ const char *	lpszDirectory)
{
  UNUSED_ALWAYS (level);
  UNUSED_ALWAYS (lpszDirectory);
  try
    {
      Notify ();
      return (true);
    }
  catch (const OperationCancelledException &)
    {
      return (false);
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::CheckArchiveFile
   _________________________________________________________________________ */

bool
PackageInstallerImpl::CheckArchiveFile
  (/*[in]*/ const char *	lpszPackage,
   /*[in]*/ const PathName &	archiveFileName,
   /*[in]*/ bool		mustBeOk)
{
  if (! File::Exists(archiveFileName))
    {
      FatalError (ERROR_MISSING_PACKAGE, archiveFileName.Get());
    }
  MD5 digest1 = dbLight.GetArchiveFileDigest(lpszPackage);
  MD5 digest2 = MD5::FromFile(archiveFileName.Get());
  bool ok = (digest1 == digest2);
  if (! ok && mustBeOk)
    {
      FatalError (ERROR_CORRUPTED_PACKAGE,
		  archiveFileName.Get(),
		  digest1.ToString().c_str(),
		  digest2.ToString().c_str());
    }
  return (ok);
}

/* _________________________________________________________________________

   PackageInstallerImpl::ConnectToServer
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER

void
PackageInstallerImpl::ConnectToServer ()
{
  const char * MSG_CANNOT_START_SERVER =
    T_("Cannot start MiKTeX package manager.");
  if (localServer.pInstaller == 0)
    {
      if (localServer.pManager == 0)
	{
	  if (IsWindowsVista())
	    {
	      WCHAR wszCLSID[50];
	      if (StringFromGUID2
		  (__uuidof(MiKTeXPackageManagerLib
			    ::MAKE_CURVER_ID(PackageManager)),
			    wszCLSID, 
			    sizeof(wszCLSID) / sizeof(wszCLSID[0]))
		  < 0)
		{
		  FATAL_MPM_ERROR ("ConnectToServer",
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
			    __uuidof(MiKTeXPackageManagerLib
				     ::IPackageManager),
			    reinterpret_cast<void**>(&localServer.pManager));
	      if (hr == CO_E_NOTINITIALIZED)
		{
		  MyCoInitialize ();
		  hr =
		    CoGetObject(monikerName.c_str(),
				&bo,
				__uuidof(MiKTeXPackageManagerLib
					 ::IPackageManager),
			    reinterpret_cast<void**>(&localServer.pManager));
		}
	      if (FAILED(hr))
		{
		  FATAL_MPM_ERROR ("ConnectToServer",
				   MSG_CANNOT_START_SERVER,
				   NUMTOHEXSTR(hr));
		}
	    }
	  else
	    {
	      HRESULT hr =
		localServer.pManager.CoCreateInstance
		(__uuidof(MiKTeXPackageManagerLib::
			  MAKE_CURVER_ID(PackageManager)),
		 0,
		 CLSCTX_LOCAL_SERVER);
	      if (hr == CO_E_NOTINITIALIZED)
		{
		  MyCoInitialize ();
		  hr =
		    localServer.pManager.CoCreateInstance
		    (__uuidof(MiKTeXPackageManagerLib::
			      MAKE_CURVER_ID(PackageManager)),
		     0,
		     CLSCTX_LOCAL_SERVER);
		}
	      if (FAILED(hr))
		{
		  FATAL_MPM_ERROR ("ConnectToServer",
				   MSG_CANNOT_START_SERVER,
				   NUMTOHEXSTR(hr));
		}
	    }
	}
      HRESULT hr =
	localServer.pManager->CreateInstaller(&localServer.pInstaller);
      if (FAILED(hr))
	{
	  localServer.pManager.Release ();
	  FATAL_MPM_ERROR ("ConnectToServer",
			   MSG_CANNOT_START_SERVER,
			   NUMTOHEXSTR(hr));
	}
    }
}

#endif
      
/* _________________________________________________________________________

   PackageInstallerImpl::RegisterComponent
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)

void
PackageInstallerImpl::RegisterComponent
  (/*[in]*/ bool			doRegister,
   /*[in]*/ const PathName &		path,
   /*[in]*/ bool			mustSucceed)
{
  MIKTEX_ASSERT (! pSession->IsMiKTeXPortable());
  ReportLine ("%s %s",
	      (doRegister
	       ? "registering"
	       : "unregistering"),
	      path.Get());
  PathName regExe;
  CommandLineBuilder cmdLine;
  if (path.HasExtension(MIKTEX_SHARED_LIB_FILE_SUFFIX))
    {
      regExe = "regsvr32.exe";
      cmdLine.AppendOption ("/s");
      if (! doRegister)
	{
	  cmdLine.AppendOption ("/u");
	}
      cmdLine.AppendArgument (path);
    }
  else
    {
      regExe = path;
      cmdLine.AppendOption (doRegister
			    ? "/RegServer"
			    : "/UnregServer");
    }
  int exitCode;
  if (! Process::Run(regExe, cmdLine.Get(), 0, &exitCode, 0))
    {
      UNEXPECTED_CONDITION ("PackageInstallerImpl::RegisterComponent");
    }
  if (exitCode != 0 && mustSucceed)
    {
      FATAL_MPM_ERROR ("PackageInstallerImpl::RegisterComponent",
		       T_("regsvr failed for some reason."),
		       path.Get());
    }
}

#endif

/* _________________________________________________________________________

   PackageInstallerImpl::RegisterComponents
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)

static const char * const components[] = {
  MIKTEX_PATH_CORE_DLL,
  MIKTEX_PATH_CORE_PS_DLL,
  MIKTEX_PATH_MPM_DLL,
  MIKTEX_PATH_MPM_PS_DLL,
};

#endif

static const char * const toBeConfigured[] = {
  MIKTEX_PATH_FONTCONFIG_CONFIG_FILE,
};

void
PackageInstallerImpl::RegisterComponents
  (/*[in]*/ bool			doRegister,
   /*[in]*/ const vector<string> &	packages)
{
  for (vector<string>::const_iterator it = packages.begin();
       it != packages.end();
       ++ it)
    {
      PackageInfo * pPackageInfo =
	pManager->TryGetPackageInfo(it->c_str());
      if (pPackageInfo == 0)
	{
	  FatalError (ERROR_UNKNOWN_PACKAGE, it->c_str());
	}
      for (vector<string>::const_iterator it2 =
	     pPackageInfo->runFiles.begin();
	   it2 != pPackageInfo->runFiles.end();
	   ++ it2)
	{
	  string fileName;
	  if (! PackageManager::StripTeXMFPrefix(*it2, fileName))
	    {
	      continue;
	    }
	  if (doRegister)
	    {
	      for (size_t idx = 0;
		   idx < sizeof(toBeConfigured) / sizeof(toBeConfigured[0]);
		   ++ idx)
		{
		  PathName relPath (toBeConfigured[idx]);
		  PathName relPathIn (relPath);
		  relPathIn.AppendExtension (".in");
		  if (PathName(fileName) != relPathIn)
		    {
		      continue;
		    }
		  PathName pathIn =
		    pSession->GetSpecialPath(SpecialPath::InstallRoot);
		  pathIn += relPathIn;
		  if (File::Exists(pathIn))
		    {
		      ReportLine (T_("configuring %s"), relPath.Get());
		      pSession->ConfigureFile (relPath);
		    }
		  else
		    {
		      ReportLine (T_("problem: %s does not exist"),
				  pathIn.Get());
		    }
		}
	    }
#if defined(MIKTEX_WINDOWS)
	  if (! pSession->IsMiKTeXPortable()
	      && (! IsWindowsNT()
	          || pSession->RunningAsAdministrator()
	          || pSession->RunningAsPowerUser()))
	    {
	      for (size_t idx = 0;
		   idx < sizeof(components) / sizeof(components[0]);
		   ++ idx)
		{
		  if (PathName(fileName) != components[idx])
		    {
		      continue;
		    }
		  PathName path =
		    pSession->GetSpecialPath(SpecialPath::InstallRoot);
		  path += components[idx];
		  if (File::Exists(path))
		    {
		      RegisterComponent (doRegister, path, doRegister);
		    }
		  else
		    {
		      ReportLine (T_("problem: %s does not exist"),
				  path.Get());
		    }
		}
	    }
#endif
	}
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::RegisterComponents
   _________________________________________________________________________ */

void
PackageInstallerImpl::RegisterComponents (/*[in]*/ bool doRegister)
{
  if (doRegister)
    {
      for (size_t idx = 0;
	   idx < sizeof(toBeConfigured) / sizeof(toBeConfigured[0]);
	   ++ idx)
	{
	  PathName relPath (toBeConfigured[idx]);
	  PathName pathIn (pSession->GetSpecialPath(SpecialPath::InstallRoot));
	  pathIn += relPath;
	  pathIn.AppendExtension (".in");
	  if (File::Exists(pathIn))
	    {
	      ReportLine (T_("configuring %s"), relPath.Get());
	      pSession->ConfigureFile (relPath);
	    }
	  else
	    {
	      ReportLine (T_("problem: %s does not exist"), pathIn.Get());
	    }
	}
    }
#if defined(MIKTEX_WINDOWS)
  if (! pSession->IsMiKTeXPortable()
      && (! IsWindowsNT()
          || pSession->RunningAsAdministrator()
          || pSession->RunningAsPowerUser()))
    {
      for (size_t idx = 0;
	   idx < sizeof(components) / sizeof(components[0]);
	   ++ idx)
	{
	  PathName path (pSession->GetSpecialPath(SpecialPath::InstallRoot));
	  path += components[idx];
	  if (File::Exists(path))
	    {
	      RegisterComponent (doRegister, path, doRegister);
	    }
	  else
	    {
	      ReportLine (T_("problem: %s does not exist"), path.Get());
	    }
	}
    }
#endif
}

/* _________________________________________________________________________

   PackageInstallerImpl::RunIniTeXMF
   _________________________________________________________________________ */

void
PackageInstallerImpl::RunIniTeXMF (/*[in]*/ const char *	lpszArguments)
{
#if defined(MIKTEX_WINDOWS)
  // find initexmf
  PathName exe;
  if (! pSession->FindFile(MIKTEX_INITEXMF_EXE,
				       FileType::EXE,
				       exe))
    {
      FATAL_MPM_ERROR (
	"PackageInstallerImpl::RunIniTeXMF",
	(T_("\
The MiKTeX configuration utility could not be found.")),
	0);
    }

  // run initexmf.exe
  string arguments = lpszArguments;
  Process::Run (exe, arguments.c_str());
#else
  UNUSED_ALWAYS (lpszArguments);
#  warning Unimplemented: PackageInstallerImpl::RunIniTeXMF
#endif
}

/* _________________________________________________________________________

   PackageInstallerImpl::CheckDependencies
   _________________________________________________________________________ */

void
PackageInstallerImpl::CheckDependencies
  (/*[in,out]*/ set<string> &	packages,
   /*[in]*/ const string &	deploymentName,
   /*[in]*/ bool		force,
   /*[in]*/ int			level)
{
  if (level > 10)
    {
      FATAL_MPM_ERROR ("PackageInstallerImpl::CheckDependencies",
		       T_("Cyclic dependencies detected."),
		       0);
    }
  PackageInfo * pPackageInfo =
    pManager->TryGetPackageInfo(deploymentName.c_str());
  if (pPackageInfo != 0)
    {
      for (vector<string>::const_iterator it =
	     pPackageInfo->requiredPackages.begin();
	   it != pPackageInfo->requiredPackages.end();
	   ++ it)
	{
	  CheckDependencies (packages, *it, force, level + 1);
	}
    }
  if (force || ! pManager->IsPackageInstalled(deploymentName.c_str()))
    {
      packages.insert (deploymentName);
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::InstallRemove
   _________________________________________________________________________ */

void
PackageInstallerImpl::InstallRemove ()
{
#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
  if (UseLocalServer())
    {
      HRESULT hr;
      ConnectToServer ();
      for (vector<string>::const_iterator it = toBeInstalled.begin();
	   it != toBeInstalled.end();
	   ++ it)
	{
	  hr =
	    localServer.pInstaller->Add(_bstr_t(it->c_str()), VARIANT_TRUE);
	  if (FAILED(hr))
	    {
	      FATAL_MPM_ERROR ("PackageInstallerImpl::InstallRemove",
			       T_("Cannot communicate with mpmsvc."),
			       NUMTOHEXSTR(hr));
	    }
	}
      for (vector<string>::const_iterator it = toBeRemoved.begin();
	   it != toBeRemoved.end();
	   ++ it)
	{
	  HRESULT hr =
	    localServer.pInstaller->Add(_bstr_t(it->c_str()), VARIANT_FALSE);
	  if (FAILED(hr))
	    {
	      FATAL_MPM_ERROR ("PackageInstallerImpl::InstallRemove",
			       T_("Cannot communicate with mpmsvc."),
			       NUMTOHEXSTR(hr));
	    }
	}
      localServer.pInstaller->SetCallback(this);
      localServer.pInstaller->SetRepository(_bstr_t(repository.c_str()));
      hr = localServer.pInstaller->InstallRemove();
      localServer.pInstaller->SetCallback(0);
      if (FAILED(hr))
	{
	  MiKTeXPackageManagerLib::ErrorInfo errorInfo;
	  HRESULT hr2 = localServer.pInstaller->GetErrorInfo(&errorInfo);
	  if (FAILED(hr2))
	    {
	      FATAL_MPM_ERROR ("PackageInstallerImpl::InstallRemove",
			       T_("mpmsvc failed for some reason."),
			       NUMTOHEXSTR(hr));
	    }
	  AutoSysString a (errorInfo.message);
	  AutoSysString b (errorInfo.info);
	  AutoSysString c (errorInfo.sourceFile);
	  Session::FatalMiKTeXError
	    ("PackageInstallerImpl::InstallRemove",
	     CW2CT(errorInfo.message),
	     CW2CT(errorInfo.info),
	     CW2CT(errorInfo.sourceFile),
	     errorInfo.sourceLine);
	}
      return;
    }
#endif

  bool upgrade = (toBeInstalled.size() == 0 && toBeRemoved.size() == 0);
  bool installing = (upgrade || toBeInstalled.size() > 0);

  if (installing)
    {
      if (repositoryType == RepositoryType::Unknown)
	{
	  // we must have a package repository
	  repository = pManager->PickRepositoryUrl();
	  repositoryType = RepositoryType::Remote;
	}
      else if (repositoryType == RepositoryType::Remote)
	{
	  pManager->VerifyPackageRepository (repository);
	}
    }

  ReportLine (T_("starting package maintenance..."));
  ReportLine (T_("installation directory: %s"),
    Q_(pSession->GetSpecialPath(SpecialPath::InstallRoot)));
  if (installing)
    {
      ReportLine (T_("package repository: %s"), Q_(repository));
    }

  SetAutoFndbSync (true);

  // make sure that mpm.fndb exists
  if (autoFndbSync
      && ! File::Exists(pSession->GetMpmDatabasePathName()))
    {
      pManager->CreateMpmFndb ();
    }

  if (toBeInstalled.size() > 1 || toBeRemoved.size() > 0)
    {
      pManager->NeedInstalledFileInfoTable ();
    }

  // collect all packages, if no packages were specified by the caller
  if (upgrade)
    {
      LoadDbLight (true);
      
      char szPackage[BufferSizes::MaxPackageName];
      if (dbLight.FirstPackage(szPackage) == 0)
	{
	  FATAL_MPM_ERROR ("PackageInstallerImpl::InstallRemove",
			   T_("No packages on server."),
			   0);
	}
      do
	{
	  // search dblight
	  PackageLevel lvl = dbLight.GetPackageLevel(szPackage);
	  if (lvl.Get() > taskPackageLevel.Get())
	    {
	      // not found or not required
	      continue;
	    }

	  if (repositoryType == RepositoryType::Local
	      || repositoryType == RepositoryType::Remote)
	    {
	      // ignore pure containers
	      if (IsPureContainer(szPackage))
		{
		  continue;
		}

	      // check to see whether the archive file exists
	      ArchiveFileType aft = dbLight.GetArchiveFileType(szPackage);
	      PathName
		pathLocalArchiveFile
		(repository,
		 szPackage,
		 ArchiveFileType::GetFileNameExtension(aft.Get()));
	      if (! File::Exists(pathLocalArchiveFile))
		{
		  FatalError (ERROR_MISSING_PACKAGE,
			      pathLocalArchiveFile.Get());
		}
	      
	      // check to see if the archive file is valid
	      CheckArchiveFile (szPackage, pathLocalArchiveFile, true);
	    }

	  // collect the package
	  toBeInstalled.push_back (szPackage);
	}
      while (dbLight.NextPackage(szPackage) != 0);
    }
  else if (toBeInstalled.size() > 0)
    {
      // we need mpm.ini, if packages are to be installed
      LoadDbLight (false);
    }

  // check dependencies
  set<string> tmp;
  for (vector<string>::const_iterator it = toBeInstalled.begin();
       it != toBeInstalled.end();
       ++ it)
    {
      CheckDependencies (tmp, *it, true, 0);
    }
  toBeInstalled.assign (tmp.begin(), tmp.end());

  // calculate total size and more
  CalculateExpenditure ();
  
  vector<string>::const_iterator it;

  RegisterComponents (false, toBeInstalled, toBeRemoved);

  // install packages
  for (it = toBeInstalled.begin(); it != toBeInstalled.end(); ++ it)
    {
      InstallPackage (*it);
    }

  // remove packages
  for (it = toBeRemoved.begin(); it != toBeRemoved.end(); ++it)
    {
      RemovePackage (*it);
    }

  // check dependencies (install missing required packages)
  tmp.clear ();
  for (vector<string>::const_iterator it = toBeInstalled.begin();
       it != toBeInstalled.end();
       ++ it)
    {
      CheckDependencies (tmp, *it, false, 0);
    }
  for (set<string>::const_iterator it = tmp.begin(); it != tmp.end(); ++ it)
    {
      InstallPackage (*it);
    }

  if (! noPostProcessing)
    {
      RegisterComponents (true, toBeInstalled);
    }

  if (! autoFndbSync)
    {
      // refresh file name database now
      ReportLine (T_("refreshing file name database..."));
      if (! Fndb::Refresh(
	      pSession->GetSpecialPath(SpecialPath::InstallRoot), this))
	{
	  throw OperationCancelledException ();
	}
      pManager->CreateMpmFndb ();
    }

  if (! noPostProcessing)
    {
      RunIniTeXMF ("--mklinks --mkmaps");
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::InstallRemoveAsync
   _________________________________________________________________________ */

void
PackageInstallerImpl::InstallRemoveAsync ()
{
  StartTaskThread (InstallRemoveThread);
}

/* _________________________________________________________________________

   PackageInstallerImpl::InstallRemoveThread
   _________________________________________________________________________ */

void
PackageInstallerImpl::InstallRemoveThread (/*[in]*/ void * pv)
{
  PackageInstallerImpl * This = static_cast<PackageInstallerImpl*>(pv);
#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
  HRESULT hr = E_FAIL;
#endif
  try
    {
#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
      hr = CoInitializeEx(0, COINIT_MULTITHREADED);
      if (FAILED(hr))
	{
	  FATAL_MPM_ERROR ("PackageInstallerImpl::InstallRemoveThread",
			   T_("Cannot start installer thread."),
			   NUMTOHEXSTR(hr));
	}
#endif
      This->InstallRemove ();
      This->progressInfo.ready = true;
      This->Notify ();
    }
  catch (const OperationCancelledException & e)
    {
      This->progressInfo.ready = true;
      This->progressInfo.cancelled = true;
      This->threadMiKTeXException = e;
    }
  catch (const MiKTeXException & e)
    {
      This->progressInfo.ready = true;
      This->progressInfo.numErrors += 1;
      This->threadMiKTeXException = e;
    }
#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
  if (SUCCEEDED(hr))
    {
      CoUninitialize ();
    }
#endif
}

/* _________________________________________________________________________

   PackageInstallerImpl::Download

   Download a file from the server.
   _________________________________________________________________________ */

void
PackageInstallerImpl::Download (/*[in]*/ const PathName &	fileName,
				/*[in]*/ size_t			expectedSize)
{
  Download (MakeUrl(fileName.Get()).c_str(),
	    PathName(downloadDirectory, fileName),
	    expectedSize);
}

/* _________________________________________________________________________

   PackageInstallerImpl::Download
   _________________________________________________________________________ */

void
PackageInstallerImpl::Download ()
{
  if (repositoryType == RepositoryType::Remote)
    {
      pManager->VerifyPackageRepository (repository);
    }

  if (repositoryType == RepositoryType::Unknown)
    {
      repository = pManager->PickRepositoryUrl();
      repositoryType = RepositoryType::Remote;
    }

  MIKTEX_ASSERT (repositoryType == RepositoryType::Remote);

  ReportLine (T_("starting download..."));
  ReportLine (T_("repository: %s"), Q_(repository));
  ReportLine (T_("download directory: %s"), Q_(downloadDirectory));

  // download and load the lightweight database
  LoadDbLight (true);
 
  // collect required packages
  char szPackage[BufferSizes::MaxPackageName];
  if (dbLight.FirstPackage(szPackage) != 0)
    {
      do
	{
	  // don't add pure containers
	  if (IsPureContainer(szPackage))
	    {
	      continue;
	    }

	  // check package level
	  if (taskPackageLevel < dbLight.GetPackageLevel(szPackage))
	    {
	      // package is not required
	      continue;
	    }

	  // check to see whether the file was downloaded previously
	  ArchiveFileType aft = dbLight.GetArchiveFileType(szPackage);
	  PathName
	    pathLocalArchiveFile
	    (downloadDirectory,
	     szPackage,
	     ArchiveFileType::GetFileNameExtension(aft.Get()));
	  if (File::Exists(pathLocalArchiveFile))
	    {
	      // the archive file exists;  check to see if it is valid
	      MD5 digest1 = dbLight.GetArchiveFileDigest(szPackage);
	      MD5 digest2 = MD5::FromFile(pathLocalArchiveFile.Get());
	      if (digest1 == digest2)
		{
		  // valid => don't download again
		  ReportLine (T_("%s already exists - keep it"),
			      Q_(pathLocalArchiveFile));
		  continue;
		}
	      ReportLine (T_("%s already exists but seems to be damaged"),
			  Q_(pathLocalArchiveFile));
	    }

	  // pick up the package
	  toBeInstalled.push_back (szPackage);
	}
      while (dbLight.NextPackage(szPackage) != 0);
    }

  // count bytes
  CalculateExpenditure (true);

  // download dblight & dbfull
  ReportLine (T_("downloading package database..."));
  MIKTEX_LOCK(ProgressIndicator)
    {
      progressInfo.deploymentName = MIKTEX_MPM_DB_LIGHT_FILE_NAME_NO_SUFFIX;
      progressInfo.displayName = T_("Lightweight package database");
      progressInfo.cbPackageDownloadCompleted = 0;
      progressInfo.cbPackageDownloadTotal = MPM_APSIZE_DB_LIGHT;
    }
  MIKTEX_UNLOCK();
  Download (MIKTEX_MPM_DB_LIGHT_FILE_NAME);
  MIKTEX_LOCK(ProgressIndicator)
    {
      progressInfo.deploymentName = MIKTEX_MPM_DB_FULL_FILE_NAME_NO_SUFFIX;
      progressInfo.displayName = T_("complete package database");
      progressInfo.cbPackageDownloadCompleted = 0;
      progressInfo.cbPackageDownloadTotal = MPM_APSIZE_DB_FULL;
    }
  MIKTEX_UNLOCK();
  Download (MIKTEX_MPM_DB_FULL_FILE_NAME);

  // download archive files
  for (vector<string>::const_iterator it = toBeInstalled.begin();
       it != toBeInstalled.end();
       ++ it)
    {
      DownloadPackage (it->c_str());
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::DownloadAsync
   _________________________________________________________________________ */

void
PackageInstallerImpl::DownloadAsync ()
{
  StartTaskThread (DownloadThread);
}

/* _________________________________________________________________________

   PackageInstallerImpl::DownloadThread
   _________________________________________________________________________ */

void
PackageInstallerImpl::DownloadThread (/*[in]*/ void * pv)
{
  PackageInstallerImpl * This = static_cast<PackageInstallerImpl*>(pv);
#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
  HRESULT hr = E_FAIL;
#endif
  try
    {
#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
      hr = CoInitializeEx(0, COINIT_MULTITHREADED);
      if (FAILED(hr))
	{
	  FATAL_MPM_ERROR ("PackageInstallerImpl::DownloadThread",
			   T_("Cannot start downloader thread."),
			   NUMTOHEXSTR(hr));
	}
#endif
      This->Download ();
      This->progressInfo.ready = true;
      This->Notify ();
    }
  catch (const OperationCancelledException & e)
    {
      This->progressInfo.ready = true;
      This->progressInfo.cancelled = true;
      This->threadMiKTeXException = e;
    }
  catch (const MiKTeXException & e)
    {
      This->progressInfo.ready = true;
      This->progressInfo.numErrors += 1;
      This->threadMiKTeXException = e;
    }
#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
  if (SUCCEEDED(hr))
    {
      CoUninitialize ();
    }
#endif
}

/* _________________________________________________________________________

   PackageInstallerImpl::SetUpPackageDefinitionFiles
   _________________________________________________________________________ */

void
PackageInstallerImpl::SetUpPackageDefinitionFiles
  (/*[in]*/ const PathName &	directory)
{
  // path to the database file
  PathName pathDatabase;
  
  if (repositoryType == RepositoryType::Remote)
    {
      // download the database file
      pathDatabase.Set (directory, MIKTEX_MPM_DB_FULL_FILE_NAME);
      Download (MakeUrl(MIKTEX_MPM_DB_FULL_FILE_NAME), pathDatabase);
    }
  else
    {
      MIKTEX_ASSERT (repositoryType == RepositoryType::Local);
      pathDatabase.Set (repository, MIKTEX_MPM_DB_FULL_FILE_NAME);
    }
  
  // extract package defintion files
  auto_ptr<MiKTeX::Extractor::Extractor>
    pExtractor
    (MiKTeX::Extractor::Extractor::CreateExtractor(DB_ARCHIVE_FILE_TYPE));
  pExtractor->Extract (pathDatabase,
		       directory,
		       false,
		       0,
		       0);
  pExtractor->Dispose ();
}

/* _________________________________________________________________________

   PackageInstallerImpl::CleanUpUserDatabase
   _________________________________________________________________________ */

void
PackageInstallerImpl::CleanUpUserDatabase ()
{
  PathName userDir
    (pSession->GetSpecialPath(SpecialPath::UserInstallRoot),
      MIKTEX_PATH_PACKAGE_DEFINITION_DIR);

  PathName commonDir
    (pSession->GetSpecialPath(SpecialPath::CommonInstallRoot),
      MIKTEX_PATH_PACKAGE_DEFINITION_DIR);

  if (! Directory::Exists(userDir) || ! Directory::Exists(commonDir))
  {
    return;
  }

  if (userDir.Canonicalize() == commonDir.Canonicalize())
  {
    return;
  }

  vector<PathName> toBeRemoved;

  // check all package definition files
  auto_ptr<DirectoryLister> pLister (DirectoryLister::Open(userDir));
  DirectoryEntry direntry;
  while (pLister->GetNext(direntry))
  {
    PathName name (direntry.name);

    if (direntry.isDirectory
      || ! name.HasExtension(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX))
    {
      continue;
    }

    // check to see whether the system-wide file exists
    PathName commonPackageDefinitionFile (commonDir, name);
    if (! File::Exists(commonPackageDefinitionFile))
    {
      continue;
    }

    // compare files
    PathName userPackageDefinitionFile (userDir, name);
    if (File::GetSize(userPackageDefinitionFile)
	  == File::GetSize(commonPackageDefinitionFile)
	&& MD5::FromFile(userPackageDefinitionFile.Get())
	  == MD5::FromFile(commonPackageDefinitionFile.Get()))
    {
      // files are identical; remove user file later
      toBeRemoved.push_back (userPackageDefinitionFile);
    }
  }
  pLister->Close ();

  // remove redundant user package definition files
  for (vector<PathName>::const_iterator it = toBeRemoved.begin();
    it != toBeRemoved.end();
    ++ it)
  {
    trace_mpm->WriteFormattedLine ("libmpm",
      T_("removing redundant package definition file: %s"),
      Q_(*it));
    File::Delete (*it, true);
  }
}

/* _________________________________________________________________________

   PackageInstallerImpl::HandleObsoletePackageDefinitionFiles
   _________________________________________________________________________ */

void
PackageInstallerImpl::HandleObsoletePackageDefinitionFiles
  (/*[in]*/ const PathName & temporaryDirectory)
{
  PathName pathPackageDir (pSession->GetSpecialPath(SpecialPath::InstallRoot),
			   MIKTEX_PATH_PACKAGE_DEFINITION_DIR);

  if (! Directory::Exists(pathPackageDir))
    {
      return;
    }

  auto_ptr<DirectoryLister> pLister (DirectoryLister::Open(pathPackageDir));
  DirectoryEntry direntry;
  while (pLister->GetNext(direntry))
    {
      PathName name (direntry.name);

      if (direntry.isDirectory
	  || ! name.HasExtension(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX))
	{
	  continue;
	}

      // it's not an obsolete package if the temporary directory
      // contains a corresponding package definition file
      if (File::Exists(PathName(temporaryDirectory, name)))
	{
	  continue;
	}

      // now we know that the package is obsolete

      char szDeploymentName[BufferSizes::MaxPackageName];
      MIKTEX_ASSERT
	(PathName(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX)
	 == (PathName(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX).GetExtension()));
      name.GetFileNameWithoutExtension (szDeploymentName);

      // check to see whether the obsolete package is installed
      if (pManager->GetTimeInstalled(szDeploymentName) == 0
	  || IsPureContainer(szDeploymentName))
	{
	  // not installed: remove the package definition file
	  trace_mpm->WriteFormattedLine ("libmpm",
					 T_("removing obsolete %s"),
					 Q_(name));
	  File::Delete (PathName(pathPackageDir, name), true);
	}
      else
	{
	  // installed: declare the package as obsolete (we wont
	  // uninstall obsolete packages)
	  trace_mpm->WriteFormattedLine ("libmpm",
					 T_("declaring %s obsolete"),
					 Q_(szDeploymentName));
	  pManager->DeclarePackageObsolete (szDeploymentName, true);
	}
    }
  
  pLister->Close ();

  pManager->FlushVariablePackageTable ();
}

/* _________________________________________________________________________

   PackageInstallerImpl::UpdateDb
   _________________________________________________________________________ */

void
PackageInstallerImpl::UpdateDb ()
{
#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
  if (UseLocalServer())
    {
      ConnectToServer ();
      localServer.pInstaller->SetRepository(_bstr_t(repository.c_str()));
      HRESULT hr = localServer.pInstaller->UpdateDb();
      if (FAILED(hr))
	{
	  MiKTeXPackageManagerLib::ErrorInfo errorInfo;
	  HRESULT hr2 = localServer.pInstaller->GetErrorInfo(&errorInfo);
	  if (FAILED(hr2))
	    {
	      FATAL_MPM_ERROR ("PackageInstallerImpl::UpdateDb",
			       T_("The service failed for some reason."),
			       NUMTOHEXSTR(hr));
	    }
	  AutoSysString a (errorInfo.message);
	  AutoSysString b (errorInfo.info);
	  AutoSysString c (errorInfo.sourceFile);
	  Session::FatalMiKTeXError ("PackageInstallerImpl::UpdateDb",
				     CW2CT(errorInfo.message),
				     CW2CT(errorInfo.info),
				     CW2CT(errorInfo.sourceFile),
				     errorInfo.sourceLine);
	}
      return;
    }
#endif

  if (repositoryType == RepositoryType::Unknown)
    {
      repository = pManager->PickRepositoryUrl();
      repositoryType = RepositoryType::Remote;
    }
  else if (repositoryType == RepositoryType::Remote)
    {
      pManager->VerifyPackageRepository (repository);
    }

  // we might need a scratch directory
  ScratchDirectory scratchDirectory;

  // path to the temporary source directory
  PathName tempDir;

  // copy the new package definition files into a temporary directory
  if (repositoryType == RepositoryType::Remote
      || repositoryType == RepositoryType::Local)
    {
      scratchDirectory.Enter ();
      tempDir.SetToCurrentDirectory ();
      SetUpPackageDefinitionFiles (tempDir);
    }
  else if (repositoryType == RepositoryType::MiKTeXDirect)
    {
      // installing from the CD
      tempDir = repository;
      tempDir += MIKTEXDIRECT_PREFIX_DIR;
      tempDir += MIKTEX_PATH_PACKAGE_DEFINITION_DIR;
    }
  else
    {
      UNEXPECTED_CONDITION ("PackageInstallerImpl::UpdateDb");
    }

  // handle obsolete package definition files
  HandleObsoletePackageDefinitionFiles (tempDir);

  // update the package definition directory
  PathName packageDefinitionDir (
    pSession->GetSpecialPath(SpecialPath::InstallRoot),
    MIKTEX_PATH_PACKAGE_DEFINITION_DIR);
  ReportLine (T_("updating package definition directory (%s)..."),
	      Q_(packageDefinitionDir));
  size_t count = 0;
  auto_ptr<DirectoryLister> pLister (DirectoryLister::Open(tempDir));
  DirectoryEntry direntry;
  TpmParser tpmparser;
  while (pLister->GetNext(direntry))
    {
      Notify ();

      PathName name (direntry.name);

      if (direntry.isDirectory
	  || ! name.HasExtension(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX))
	{
	  continue;
	}

      // get external package name
      char szDeploymentName[BufferSizes::MaxPackageName];
      MIKTEX_ASSERT
	(PathName(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX)
	 == (PathName(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX).GetExtension()));
      name.GetFileNameWithoutExtension (szDeploymentName);

      // build name of current package definition file
      PathName currentPackageDefinitionfile (packageDefinitionDir, name);

      // ignore package, if package is already installed
      if (! IsPureContainer(szDeploymentName)
	  && pManager->IsPackageInstalled(szDeploymentName))
	{
#if 0
	  if (File::Exists(currentPackageDefinitionfile))
#endif
	  continue;
	}

      // parse new package definition file
      PathName newPackageDefinitionFile (tempDir, name);
      tpmparser.Parse (newPackageDefinitionFile);

      PackageInfo currentPackageInfo;
      if (pManager->TryGetPackageInfo(szDeploymentName, currentPackageInfo)
	&& tpmparser.GetPackageInfo().digest == currentPackageInfo.digest)
      {
	// nothing new
	continue;
      }
      
      // move the new package definition file into the package
      // definition directory
      Directory::Create (packageDefinitionDir);
      if (File::Exists(currentPackageDefinitionfile))
	{
	  // move the current file out of the way
	  File::Delete (currentPackageDefinitionfile, true);
	}
      File::Copy (newPackageDefinitionFile, currentPackageDefinitionfile);
      
      // update the database
      pManager->DefinePackage (szDeploymentName, tpmparser.GetPackageInfo());

      ++ count;
    }

  pLister->Close ();

  ReportLine (T_("installed %u package definition files"),
	      static_cast<unsigned>(count));

  // remove the temporary directory
  if (repositoryType == RepositoryType::Remote
      || repositoryType == RepositoryType::Local)
    {
      scratchDirectory.Leave ();
    }

  // clean up the user database
  if (! pSession->IsAdminMode())
  {
    CleanUpUserDatabase ();
  }

  // install mpm.ini
  InstallDbLight ();

  // force a reload of the database
  dbLight.Clear ();
  pManager->ClearAll ();

  // create the MPM file name database
  pManager->CreateMpmFndb ();
}

/* _________________________________________________________________________

   PackageInstallerImpl::UpdateDbAsync
   _________________________________________________________________________ */

void
PackageInstallerImpl::UpdateDbAsync ()
{
  StartTaskThread (UpdateDbThread);
}

/* _________________________________________________________________________

   PackageInstallerImpl::UpdateDTbhread
   _________________________________________________________________________ */

void
PackageInstallerImpl::UpdateDbThread (/*[in]*/ void * pv)
{
  PackageInstallerImpl * This = static_cast<PackageInstallerImpl*>(pv);
  try
    {
      This->UpdateDb ();
      This->progressInfo.ready = true;
      This->Notify ();
    }
  catch (const OperationCancelledException & e)
    {
      This->progressInfo.ready = true;
      This->progressInfo.cancelled = true;
      This->threadMiKTeXException = e;
    }
  catch (const MiKTeXException & e)
    {
      This->progressInfo.ready = true;
      This->progressInfo.numErrors += 1;
      This->threadMiKTeXException = e;
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::StartTaskThread
   _________________________________________________________________________ */

void
PackageInstallerImpl::StartTaskThread
  (/*[in]*/ void (MIKTEXCALLBACK * pFunc) (void *))
{
  progressInfo = ProgressInfo ();
  timeStarted = clock();
  SetAutoFndbSync (false);
  pThread = Thread::Start(pFunc, this);
}

/* _________________________________________________________________________

   PackageInstallerImpl::WaitForCompletion
   _________________________________________________________________________ */

void
PackageInstallerImpl::WaitForCompletion ()
{
  pThread->Join ();
  if (progressInfo.numErrors > 0 || progressInfo.cancelled)
    {
      throw threadMiKTeXException;
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::FatalError
   _________________________________________________________________________ */

MIKTEXNORETURN
void
PackageInstallerImpl::FatalError (/*[in]*/ ErrorCode	error,
				  /*[in]*/		...)
{
  va_list marker;
  va_start (marker, error);
  ostringstream message;
  const char * lpszArg1;
  const char * lpszArg2;
  const char * lpszArg3;
  size_t size1;
  size_t size2;
  switch (error)
    {
    case ERROR_UNKNOWN_PACKAGE:
      lpszArg1 = va_arg(marker, const char *);
      message
	<< T_("\
The operation could not be completed because the following \
package is unknown:")
	<< LF
	<< LF
	<< lpszArg1;
      break;
    case ERROR_DOWNLOAD:
      lpszArg1 = va_arg(marker, const char *);
      lpszArg2 = va_arg(marker, const char *);
      message
	<< T_("\
The download operation could not be completed for the following reason:")
	<< LF
	<< LF
	<< lpszArg2;
      break;
    case ERROR_PACKAGE_NOT_INSTALLED:
      message
	<< T_("\
The operation could not be completed because the following \
package is not installed:")
	<< LF
	<< LF
	<< va_arg(marker, const char *);
      break;
    case ERROR_CANNOT_DELETE:
      lpszArg1 = va_arg(marker, const char *);
      lpszArg2 = va_arg(marker, const char *);
      message
	<< T_("\
The operation could not be completed because the removal of the \
following file did not succeed:")
	<< LF
	<< LF
	<< lpszArg1
	<< LF
	<< LF
	<< T_("\
The removal failed for the following reason:")
	<< LF
	<< LF
	<< lpszArg2;
      break;
    case ERROR_MISSING_PACKAGE:
      lpszArg1 = va_arg(marker, const char *);
      message << T_("\
The operation could not be completed because a required \
file does not exist.");
      break;
    case ERROR_CORRUPTED_PACKAGE:
      lpszArg1 = va_arg(marker, const char *);
      lpszArg2 = va_arg(marker, const char *);
      lpszArg3 = va_arg(marker, const char *);
      message
	<< T_("\
The operation could not be completed because the following file failed \
verification:")
	<< LF
	<< LF
	<< lpszArg1;
      break;
    case ERROR_SOURCE_FILE_NOT_FOUND:
      lpszArg1 = va_arg(marker, const char *);
      message
	<< T_("\
The operation could not be completed because the following file \
does not exist:")
	<< LF
	<< LF
	<< lpszArg1;
      break;
    case ERROR_SIZE_MISMATCH:
      lpszArg1 = va_arg(marker, const char *);
      size1 = va_arg(marker, size_t);
      size2 = va_arg(marker, size_t);
      message
	<< T_("\
The operation could not be completed because the following file failed \
verification:")
	<< LF
	<< LF
	<< lpszArg1;
      break;
    default:
      // this shouldn't happen
      message
	<< T_("\
The operation could not be completed.");
	break;
    }
  va_end (marker);
  MIKTEX_LOCK(This)
    {
      progressInfo.numErrors += 1;
    }
  MIKTEX_UNLOCK();
  trace_error->WriteLine ("libmpm", message.str().c_str());
  FATAL_MIKTEX_ERROR (0, message.str().c_str(), 0);
}

/* _________________________________________________________________________

   PackageInstallerImpl::ReportLine
   _________________________________________________________________________ */

void
PackageInstallerImpl::ReportLine (/*[in]*/ const char *	lpszFormat,
				  /*[in]*/		...)
{
  if (pCallback == 0)
    {
      return;
    }
  va_list marker;
  va_start (marker, lpszFormat);
  string str = Utils::FormatString(lpszFormat, marker);
  va_end (marker);
  pCallback->ReportLine (str.c_str());
}

/* _________________________________________________________________________

   PackageInstallerImpl::Dispose
   _________________________________________________________________________ */

void
PackageInstallerImpl::Dispose ()
{
  if (pThread != 0)
    {
      delete pThread;
      pThread = 0;
    }
  if (trace_mpm.get() != 0)
    {
      trace_mpm->Close ();
      trace_mpm.reset ();
    }
  if (trace_error.get() != 0)
    {
      trace_error->Close ();
      trace_error.reset ();
    }
#if defined(MIKTEX_WINDOWS)
  while (numCoInitialize > 0)
    {
      MyCoUninitialize ();
    }
#endif
}

/* _________________________________________________________________________

   PackageInstallerImpl::UseLocalServer
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER

bool
PackageInstallerImpl::UseLocalServer ()
{
  if (PackageManagerImpl::localServer)
    {
      // already running as local server
      return (false);
    }
  if (noLocalServer)
    {
      return (false);
    }
  if (! pSession->IsAdminMode())
    {
      return (false);
    }
#if defined(MIKTEX_WINDOWS)
  bool elevationRequired =
    (IsWindowsVista()
     && ! pSession->RunningAsAdministrator());
  bool forceLocalServer =
    pSession->GetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
					 MIKTEX_REGVAL_FORCE_LOCAL_SERVER,
					 false);
  return (elevationRequired || forceLocalServer);
#else
  return (false);
#endif
}

#endif

/* _________________________________________________________________________

   PackageInstallerImpl::QueryInterface
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER

HRESULT
PackageInstallerImpl::QueryInterface (/*[in]*/ REFIID		riid,
				      /*[out]*/ LPVOID *	ppvObj)
{
  using namespace MiKTeXPackageManagerLib;
  if (trace_mpm->IsEnabled())
    {    
      WCHAR szRiid[100];
      if (StringFromGUID2(riid, szRiid, 100) > 0)
	{
	  trace_mpm->WriteFormattedLine ("libmpm", "QI %S", szRiid);
	}
    }
  if (riid == __uuidof(IUnknown))
    {
      *ppvObj = static_cast<IUnknown*>(this);
    }
  else if (riid == __uuidof(IPackageInstallerCallback))

    {
      *ppvObj = static_cast<IPackageInstallerCallback*>(this);
    }
  else
    {
      *ppvObj = 0;
      return (E_NOINTERFACE);
    }
  AddRef ();
  return (S_OK);
}

#endif

/* _________________________________________________________________________

   PackageInstallerImpl::AddRef
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER

ULONG
PackageInstallerImpl::AddRef ()
{
  return (1);
}

#endif

/* _________________________________________________________________________

   PackageInstallerImpl::Release
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER

ULONG
PackageInstallerImpl::Release ()
{
  return (1);
}

#endif

/* _________________________________________________________________________

   PackageInstallerImpl::ReportLine
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER

HRESULT
PackageInstallerImpl::ReportLine (/*[in]*/ BSTR line)
{
  try
    {
      if (pCallback != 0)
	{
	  _bstr_t bstr (line, false);
	  pCallback->ReportLine (static_cast<const char *>(bstr));
	}
      return (S_OK);
    }
  catch (const exception &)
    {
      return (E_FAIL);
    }
}

#endif

/* _________________________________________________________________________

   PackageInstallerImpl::OnRetryableError
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER

HRESULT
PackageInstallerImpl::OnRetryableError
(/*[in]*/ BSTR				message,
 /*[out,retval]*/ VARIANT_BOOL *	pDoContinue)
{
  try
    {
      if (pCallback != 0)
	{
	  _bstr_t bstr (message, false);
	  *pDoContinue =
	    (pCallback->OnRetryableError(static_cast<const char *>(bstr))
	     ? VARIANT_TRUE
	     : VARIANT_FALSE);
	}
      else
	{
	  *pDoContinue = VARIANT_FALSE;
	}
      return (S_OK);
    }
  catch (const exception &)
    {
      return (E_FAIL);
    }
}

#endif

/* _________________________________________________________________________

   PackageInstallerImpl::OnProgress
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER

HRESULT
PackageInstallerImpl::OnProgress
(/*[in]*/ LONG				nf,
 /*[out,retval]*/ VARIANT_BOOL *	pDoContinue)
{
  try
    {
      if (pCallback != 0)
	{
	  Notification notification (static_cast<Notification::EnumType>(nf));
	  *pDoContinue =
	    (pCallback->OnProgress(notification)
	     ? VARIANT_TRUE
	     : VARIANT_FALSE);
	}
      else
	{
	  *pDoContinue = VARIANT_TRUE;
	}
      return (S_OK);
    }
  catch (const exception &)
    {
      return (E_FAIL);
    }
}

#endif
