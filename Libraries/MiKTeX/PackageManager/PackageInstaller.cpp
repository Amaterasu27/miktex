/* PackageInstaller.cpp:

   Copyright (C) 2001-2007 Christian Schenk

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

#include "Extractor.h"
#include "TpmParser.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace std;

#define LF T_("\n")

/* _________________________________________________________________________

   PackageInstallerImpl::MakeUrl
   _________________________________________________________________________ */

tstring
PackageInstallerImpl::MakeUrl (/*[in]*/ const MIKTEXCHAR * lpszBase,
			       /*[in]*/ const MIKTEXCHAR * lpszRel)
{
  tstring url (lpszBase);
  size_t l = url.length();
  if (l == 0)
    {
      FATAL_MPM_ERROR (T_("MakeUrl"), T_("Invalid base URL."), lpszBase);
    }
  if (url[l - 1] != T_('/'))
    {
      url += T_('/');
    }
  if (lpszRel[0] == T_('/'))
    {
      FATAL_MPM_ERROR (T_("MakeUrl"), T_("Invalid relative URL."), lpszRel);
    }
  url += lpszRel;
  return (url);
}

/* _________________________________________________________________________

   PackageInstallerImpl::MakeUrl
   _________________________________________________________________________ */

tstring
PackageInstallerImpl::MakeUrl (/*[in]*/ const MIKTEXCHAR * lpszRel)
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
IsPureContainer (/*[in]*/ const MIKTEXCHAR * lpszDeploymentName)
{
  return (StrNCmp(lpszDeploymentName, T_("_miktex-"), 8) == 0);
}

/* _________________________________________________________________________

   IsMiKTeXPackage
   _________________________________________________________________________ */

MPMSTATICFUNC(bool)
IsMiKTeXPackage (/*[in]*/ const MIKTEXCHAR * lpszDeploymentName)
{
  return (StrNCmp(lpszDeploymentName, T_("miktex-"), 7) == 0);
}

/* _________________________________________________________________________

   PrefixedPackageDefinitionFile

   Get the prefixed path to a package definition file.
   _________________________________________________________________________ */

MPMSTATICFUNC(PathName)
PrefixedPackageDefinitionFile (/*[in]*/ const tstring & deploymentName)
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
    taskPackageLevel (PackageLevel::None),
    repositoryType (RepositoryType::Unknown),
    pManager (pManager),
    trace_error (TraceStream::Open(MIKTEX_TRACE_ERROR)),
    trace_mpm (TraceStream::Open(MIKTEX_TRACE_MPM))

{
  MIKTEX_ASSERT (PackageLevel::None < PackageLevel::Essential
		 && PackageLevel::Essential < PackageLevel::Basic
		 && PackageLevel::Basic < PackageLevel::Advanced
		 && PackageLevel::Advanced < PackageLevel::Complete);
  
  // get the default remote repository URL
  tstring repository;
  if (pManager->TryGetDefaultPackageRepository(repositoryType, repository))
    {
      trace_mpm->WriteFormattedLine
	(T_("libmpm"),
	 T_("using default package repository: %s"),
	 repository.c_str());
      SetRepository (repository);
    }

  // get the MiKTeX installation directory
  destinationDirectory =
    SessionWrapper(true)->GetSpecialPath(SpecialPath::InstallRoot);
}

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
PackageInstallerImpl::Download (/*[in]*/ const tstring &	url,
				/*[in]*/ const PathName &	dest,
				/*[in]*/ size_t			expectedSize)
{
  trace_mpm->WriteFormattedLine
    (T_("libmpm"),
     T_("going to download: %s => %s"),
     Q_(url),
     Q_(dest));
  
  if (expectedSize > 0)
    {
      ReportLine (T_("downloading %s (%u bytes)..."),
		  Q_(url),
		  static_cast<unsigned>(expectedSize));
    }
  else
    {
      ReportLine (T_("downloading %s..."), Q_(url));
    }

  // open the remote file
  auto_ptr<WebFile>
    webFile (pManager->GetWebSession()->OpenUrl(url.c_str(), this));

  // open the local file
  TempFile downloadedFile;
  FileStream
    destStream (File::Open(dest,
			   FileMode::Create,
			   FileAccess::Write,
			   false));
  downloadedFile = dest;

  // receive the data
  trace_mpm->WriteFormattedLine (T_("libmpm"),
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
  trace_mpm->WriteFormattedLine (T_("libmpm"),
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
(/*[in]*/ const MIKTEXCHAR *	lpszFileName,
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
(/*[in]*/ const MIKTEXCHAR *	lpszFileName,
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
      progressInfo.fileName = T_("");
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
PackageInstallerImpl::OnError (/*[in]*/ const MIKTEXCHAR *	lpszMessage)
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
  auto_ptr<Extractor>
    pExtractor (Extractor::CreateExtractor(archiveFileType));
  pExtractor->Extract (archiveFileName,
		       destinationDirectory,
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

  ReportLine (T_("opening repository %s..."), Q_(repository));
  ReportLine (T_("repository type: %s"),
	      (repositoryType == RepositoryType::Remote
	       ? T_("remote package repository")
	       : (repositoryType == RepositoryType::Local
		  ? T_("local package repository")
		  : T_("MiKTeXDirect"))));
  
  // path to config dir
  PathName pathConfigDir (destinationDirectory, MIKTEX_PATH_MIKTEX_CONFIG_DIR);

  if (repositoryType == RepositoryType::Remote
      || repositoryType == RepositoryType::Local)
    {
      // we need a scratch directory when we download from the
      // Internet
      ScratchDirectory scratchDirectory;

      ReportLine (T_("loading lightweight database file..."));

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
      auto_ptr<Extractor>
	pExtractor (Extractor::CreateExtractor(DB_ARCHIVE_FILE_TYPE));
      pExtractor->Extract (pathZzdb1,
			   pathConfigDir,
			   false,
			   0,
			   0);
      pExtractor->Dispose ();
    }
  else
    {
      MIKTEX_ASSERT (repositoryType == RepositoryType::MiKTeXDirect);
      PathName pathMpmIniSrc (repository);
      pathMpmIniSrc += PrefixedPath(MIKTEX_PATH_MPM_INI);
      PathName pathMpmIniDst (destinationDirectory);
      pathMpmIniDst += MIKTEX_PATH_MPM_INI;
      size_t size;
      MyCopyFile (pathMpmIniSrc, pathMpmIniDst, size);
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
  PathName pathMpmIni (destinationDirectory, MIKTEX_PATH_MPM_INI);

  // install if necessary
  if (download || ! File::Exists(pathMpmIni))
    {
      InstallDbLight ();
    }

  // load mpm.ini
  dbLight.Read (pathMpmIni);
  
  // report digest
  MD5 md5 = MD5::FromFile(pathMpmIni.Get());
  ReportLine (T_("DBlight digest: %s"),
	      dbLight.GetDigest().ToString().c_str());
}

/* _________________________________________________________________________

   CompareVersions
   _________________________________________________________________________ */

int
CompareVersions (/*[in]*/ const tstring &	ver1,
		 /*[in]*/ const tstring &	ver2)
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

   PackageInstallerImpl::FindUpdates
   _________________________________________________________________________ */

void
MPMCALL
PackageInstallerImpl::FindUpdates ()
{
  trace_mpm->WriteLine (T_("libmpm"), T_("searching for updated packages"));

  // force a download of the lightweight database
  LoadDbLight (true);

  updates.clear ();

  // read lightweight database
  MIKTEXCHAR szPackage[BufferSizes::MaxPackageName];
  for (const MIKTEXCHAR * lpszPackage = dbLight.FirstPackage(szPackage);
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
	      tstring version = dbLight.GetPackageVersion(lpszPackage);
	      int verCmp = CompareVersions(version, MIKTEX_SERIES_STR);
	      if (verCmp == 0)
		{
		  trace_mpm->WriteFormattedLine (T_("libmpm"),
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
	    (T_("libmpm"),
	     T_("%s: server has a different version"),
	     lpszPackage);
	  trace_mpm->WriteFormattedLine
	    (T_("libmpm"),
	     T_("server digest: %s"),
	     md5.ToString().c_str());
	  trace_mpm->WriteFormattedLine
	    (T_("libmpm"),
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
		(T_("libmpm"),
		 T_("%s: server has updated package"),
		 lpszPackage);
	    }
	  else
	    {
	      // server has a newer version
	      trace_mpm->WriteFormattedLine
		(T_("libmpm"),
		 T_("%s: server has newer version"),
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
MIKTEXCALLBACK
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
	  FATAL_MPM_ERROR (T_("PackageInstallerImpl::FindUpdatesThread"),
			   T_("Cannot start updater thread."),
			   NUMTOSTR(hr));
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
(/*[in]*/ const vector<tstring> &	toBeRemoved,
 /*[in]*/ bool				silently)
{
  for (vector<tstring>::const_iterator it = toBeRemoved.begin();
       it != toBeRemoved.end();
       ++ it)
    {
      Notify ();

      // only consider texmf files
      tstring fileName;
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
      PathName path (destinationDirectory, fileName);
      
      // only delete if the reference count reached zero
      if (pInstalledFileInfo != 0 && pInstalledFileInfo->refCount > 0)
	{
	  trace_mpm->WriteFormattedLine
	    (T_("libmpm"),
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
	    (T_("libmpm"),
	     T_("file %s does not exist"),
	     Q_(path));
	  done = true;
	}
      
	  
      // remove from MPM FNDB
#if 0
      if (autoFndbSync && Fndb::Exists(PathName(MPM_ROOT_PATH, fileName)))
	{
	  Fndb::Remove (PathName(MPM_ROOT_PATH, fileName));
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
      Notify ();
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::RemovePackage
   _________________________________________________________________________ */

void
PackageInstallerImpl::RemovePackage (/*[in]*/ const tstring &	deploymentName)
{
  trace_mpm->WriteFormattedLine (T_("libmpm"),
				 T_("going to remove %s"),
				 Q_(deploymentName));

  // notify client
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
    (T_("libmpm"),
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
  trace_mpm->WriteFormattedLine (T_("libmpm"),
				 T_("going to remove %u file(s)"),
				 static_cast<unsigned>(nTotal));
  RemoveFiles (pPackageInfo->runFiles);
  RemoveFiles (pPackageInfo->docFiles);
  RemoveFiles (pPackageInfo->sourceFiles);

  trace_mpm->WriteFormattedLine (T_("libmpm"),
				 T_("package %s successfully removed"),
				 Q_(deploymentName));
  
  // update progress info
  MIKTEX_LOCK(ProgressIndicator)
    {
      progressInfo.cPackagesRemoveCompleted += 1;
    }
  MIKTEX_UNLOCK();

  // notify client
  Notify ();
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
	  otstringstream text;
	  text << T_("The file") LF
	       << LF
	       << T_("  ") << dest.Get() << LF
	       << LF
	       << T_("could not be opened for the following reason:") LF
	       << LF
	       << T_("  ") << e.what() << LF
	       << LF
	       << T_("Make sure that no other application uses")
	       << T_(" the file and that you have write permission on the")
	       << T_(" file.");
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
PackageInstallerImpl::CopyFiles (/*[in]*/ const vector<tstring> & fileList)
{
  PathName pathSourceRoot (repository, TEXMF_PREFIX_DIRECTORY);

  for (vector<tstring>::const_iterator it = fileList.begin();
       it != fileList.end();
       ++ it)
    {
      Notify ();

      // only consider texmf files
      tstring fileName;
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

      PathName pathDest (destinationDirectory, fileName);

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
	  progressInfo.fileName = T_("");
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
(/*[in,out]*/ vector<tstring> &		fileList,
 /*[in]*/ const PathName &		fileName)
  const
{
  // avoid duplicates
  for (vector<tstring>::const_iterator it = fileList.begin();
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
(/*[in,out]*/ vector<tstring> &	fileList,
 /*[in]*/ const PathName &	fileName)
  const
{
  for (vector<tstring>::iterator it = fileList.begin();
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
PackageInstallerImpl::CopyPackage (/*[in]*/ const tstring & deploymentName)
{
  // parse the package definition file
  PathName pathPackageFile = repository;
  pathPackageFile += TEXMF_PREFIX_DIRECTORY;
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
  CopyFiles (packageInfo.runFiles);
  CopyFiles (packageInfo.docFiles);
  CopyFiles (packageInfo.sourceFiles);
}

/* _________________________________________________________________________

   GetFiles
   _________________________________________________________________________ */

#if defined(USE_HASH_SET)
typedef hash_set<tstring> StringSet;
#else
typedef set<tstring> StringSet;
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
(/*[in]*/ const vector<tstring> &	installedFiles,
 /*[in]*/ const vector<tstring> &	removedFiles,
 /*[in]*/ const MIKTEXCHAR *		lpszPackageName)
{
#if 0
  ReportLine (T_("updating MPM file name database:"));
  ReportLine (T_("  %u records to be added"), installedFiles.size());
  ReportLine (T_("  %u records to be removed"), removedFiles.size());
#endif
  vector<tstring>::const_iterator it;
  for (it = installedFiles.begin(); it != installedFiles.end(); ++ it)
    {
      PathName path (MPM_ROOT_PATH, *it);
      if (! Fndb::FileExists(path))
	{
	  Fndb::Add (path, lpszPackageName);
	}
      else
	{
	  trace_mpm->WriteFormattedLine (T_("libmpm"),
					 T_("%s already exists in mpm fndb"),
					 Q_(path));
	}
     }
  for (it = removedFiles.begin(); it != removedFiles.end(); ++ it)
    {
      PathName path (MPM_ROOT_PATH, *it);
      if (Fndb::FileExists(path))
	{
	  Fndb::Remove (path);
	}
      else
	{
	  trace_mpm->WriteFormattedLine (T_("libmpm"),
					 T_("%s does not exist in mpm fndb"),
					 Q_(path));
	}
    }
}

/* _________________________________________________________________________

   PackageInstallerImpl::InstallPackage
   _________________________________________________________________________ */

void
PackageInstallerImpl::InstallPackage (/*[in]*/ const tstring &	deploymentName)
{
  trace_mpm->WriteFormattedLine (T_("libmpm"),
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
      trace_mpm->WriteFormattedLine (T_("libmpm"),
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
  else
    {
      // copy from CD
      MIKTEX_ASSERT (repositoryType == RepositoryType::MiKTeXDirect);
      CopyPackage (deploymentName);
    }

  // parse the new package definition file
  PathName pathPackageFile
    (PathName(destinationDirectory, MIKTEX_PATH_PACKAGE_DEFINITION_DIR),
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
  vector<tstring> recycledFiles;
  for (it = set1.begin(); it != set1.end(); ++ it)
    {
      if (set2.find(*it) == set2.end())
	{
	  tstring str;
	  if (PackageManager::StripTeXMFPrefix(*it, str))
	    {
	      recycledFiles.push_back (str);
	    }
	}
    }
  vector<tstring> newFiles;
  for (it = set2.begin(); it != set2.end(); ++ it)
    {
      if (set1.find(*it) == set1.end())
	{
	  tstring str;
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
PackageInstallerImpl::DownloadPackage (/*[in]*/ const tstring & deploymentName)
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
		    PathName(destinationDirectory, pathArchiveFile),
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
  vector<tstring>::const_iterator it;

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
MIKTEXCALL
PackageInstallerImpl::ReadDirectory (/*[in]*/ const MIKTEXCHAR * lpszPath,
				     /*[out]*/ MIKTEXCHAR * * ppSubDirNames,
				     /*[out]*/ MIKTEXCHAR * * ppFileNames,
				     /*[out]*/ MIKTEXCHAR * * ppFileNameInfos)

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
MIKTEXCALL
PackageInstallerImpl::OnProgress (/*[in]*/ unsigned		level,
				  /*[in]*/ const MIKTEXCHAR *	lpszDirectory)
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
(/*[in]*/ const MIKTEXCHAR *	lpszPackage,
 /*[in]*/ const PathName &	archiveFileName,
 /*[in]*/ bool			mustBeOk)
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
  const MIKTEXCHAR * MSG_CANNOT_START_SERVER =
    T_("Cannot start MiKTeX package manager.");
  if (localServer.pInstaller == 0)
    {
      if (localServer.pManager == 0)
	{
	  if (IsWindowsVista())
	    {
	      WCHAR wszCLSID[50];
	      if (StringFromGUID2
		  (__uuidof(MiKTeXPackageManagerLib::PackageManager),
			    wszCLSID, 
			    sizeof(wszCLSID) / sizeof(wszCLSID[0]))
		  < 0)
		{
		  FATAL_MPM_ERROR (T_("ConnectToServer"),
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
			    __uuidof(MiKTeXPackageManagerLib::IPackageManager),
			    reinterpret_cast<void**>(&localServer.pManager));
	      if (FAILED(hr))
		{
		  FATAL_MPM_ERROR (T_("ConnectToServer"),
				   MSG_CANNOT_START_SERVER,
				   NUMTOSTR(hr));
		}
	    }
	  else
	    {
	      HRESULT hr =
		localServer.pManager.CoCreateInstance
		(__uuidof(MiKTeXPackageManagerLib::PackageManager),
		 0,
		 CLSCTX_LOCAL_SERVER);
	      if (FAILED(hr))
		{
		  FATAL_MPM_ERROR (T_("ConnectToServer"),
				   MSG_CANNOT_START_SERVER,
				   NUMTOSTR(hr));
		}
	    }
	}
      HRESULT hr =
	localServer.pManager->CreateInstaller(&localServer.pInstaller);
      if (FAILED(hr))
	{
	  localServer.pManager.Release ();
	  FATAL_MPM_ERROR (T_("ConnectToServer"),
			   MSG_CANNOT_START_SERVER,
			   NUMTOSTR(hr));
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
(/*[in]*/ bool				doRegister,
 /*[in]*/ const PathName &		path)
{
  ReportLine (T_("%s %s"),
	      (doRegister
	       ? T_("registering")
	       : T_("unregistering")),
	      path.Get());
  PathName regExe;
  CommandLineBuilder cmdLine;
  if (path.HasExtension(MIKTEX_SHARED_LIB_FILE_SUFFIX))
    {
      regExe = T_("regsvr32.exe");
      cmdLine.AppendOption (T_("/s"));
      if (! doRegister)
	{
	  cmdLine.AppendOption (T_("/u"));
	}
      cmdLine.AppendArgument (path);
    }
  else
    {
      regExe = path;
      cmdLine.AppendOption (doRegister
			    ? T_("/RegServer")
			    : T_("/UnregServer"));
    }
  Process::Run (regExe, cmdLine.Get());
}

#endif

/* _________________________________________________________________________

   PackageInstallerImpl::RegisterComponents
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)

static const MIKTEXCHAR * const components[] = {
  MIKTEX_PATH_MPM_DLL,
  MIKTEX_PATH_MPM_PS_DLL,
};

void
PackageInstallerImpl::RegisterComponents
(/*[in]*/ bool				doRegister,
 /*[in]*/ const vector<tstring> &	packages)
{
  for (vector<tstring>::const_iterator it = packages.begin();
       it != packages.end();
       ++ it)
    {
      PackageInfo * pPackageInfo =
	pManager->TryGetPackageInfo(it->c_str());
      if (pPackageInfo == 0)
	{
	  FatalError (ERROR_UNKNOWN_PACKAGE, it->c_str());
	}
      for (vector<tstring>::const_iterator it2 =
	     pPackageInfo->runFiles.begin();
	   it2 != pPackageInfo->runFiles.end();
	   ++ it2)
	{
	  tstring fileName;
	  if (! PackageManager::StripTeXMFPrefix(*it2, fileName))
	    {
	      continue;
	    }
	  for (size_t idx = 0;
	       idx < sizeof(components) / sizeof(components[0]);
	       ++ idx)
	    {
	      if (PathName(fileName) != components[idx])
		{
		  continue;
		}
	      PathName path (destinationDirectory);
	      path += components[idx];
	      if (File::Exists(path))
		{
		  RegisterComponent (doRegister, path);
		}
	    }
	}
    }
}

#endif

/* _________________________________________________________________________

   PackageInstallerImpl::RegisterComponents
   _________________________________________________________________________ */

void
MPMCALL
PackageInstallerImpl::RegisterComponents (/*[in]*/ bool doRegister)
{
#if defined(MIKTEX_WINDOWS)
  for (size_t idx = 0;
       idx < sizeof(components) / sizeof(components[0]);
       ++ idx)
    {
      PathName path (destinationDirectory);
      path += components[idx];
      if (File::Exists(path))
	{
	  RegisterComponent (doRegister, path);
	}
    }
#endif
}

/* _________________________________________________________________________

   PackageInstallerImpl::InstallRemove
   _________________________________________________________________________ */

void
MPMCALL
PackageInstallerImpl::InstallRemove ()
{
#if USE_LOCAL_SERVER
  if (UseLocalServer())
    {
      HRESULT hr;
      ConnectToServer ();
      for (vector<tstring>::const_iterator it = toBeInstalled.begin();
	   it != toBeInstalled.end();
	   ++ it)
	{
	  hr =
	    localServer.pInstaller->Add(_bstr_t(it->c_str()), TRUE);
	  if (FAILED(hr))
	    {
	      FATAL_MPM_ERROR (T_("PackageInstallerImpl::InstallRemove"),
			       T_("Cannot communicate with mpmsvc."),
			       NUMTOSTR(hr));
	    }
	}
      for (vector<tstring>::const_iterator it = toBeRemoved.begin();
	   it != toBeRemoved.end();
	   ++ it)
	{
	  HRESULT hr =
	    localServer.pInstaller->Add(_bstr_t(it->c_str()), FALSE);
	  if (FAILED(hr))
	    {
	      FATAL_MPM_ERROR (T_("PackageInstallerImpl::InstallRemove"),
			       T_("Cannot communicate with mpmsvc."),
			       NUMTOSTR(hr));
	    }
	}
      localServer.pInstaller->SetCallback(this);
      hr = localServer.pInstaller->InstallRemove();
      localServer.pInstaller->SetCallback(0);
      if (FAILED(hr))
	{
	  MiKTeXPackageManagerLib::ErrorInfo errorInfo;
	  HRESULT hr2 = localServer.pInstaller->GetErrorInfo(&errorInfo);
	  if (FAILED(hr2))
	    {
	      FATAL_MPM_ERROR (T_("PackageInstallerImpl::InstallRemove"),
			       T_("mpmsvc failed for some reason."),
			       NUMTOSTR(hr));
	    }
	  AutoSysString a (errorInfo.message);
	  AutoSysString b (errorInfo.info);
	  AutoSysString c (errorInfo.sourceFile);
	  Session::FatalMiKTeXError
	    (T_("PackageInstallerImpl::InstallRemove"),
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

  if (repositoryType == RepositoryType::Remote && installing)
    {
      pManager->VerifyPackageRepository (repository);
    }

  if (repositoryType == RepositoryType::Unknown && installing)
    {
      // we must have a package repository
      repository = pManager->PickRepositoryUrl();
      repositoryType = RepositoryType::Remote;
    }

  ReportLine (T_("starting package maintenance..."));
  ReportLine (T_("installation directory: %s"), Q_(destinationDirectory));
  ReportLine (T_("package repository: %s"), Q_(repository));

  autoFndbSync = true;

  // make sure that mpm.fndb exists
  if (autoFndbSync
      && ! File::Exists(SessionWrapper(true)->GetMpmDatabasePathName()))
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
      
      MIKTEXCHAR szPackage[BufferSizes::MaxPackageName];
      if (dbLight.FirstPackage(szPackage) == 0)
	{
	  FATAL_MPM_ERROR (T_("PackageInstallerImpl::InstallRemove"),
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

  // calculate total size and more
  CalculateExpenditure ();
  
  vector<tstring>::const_iterator it;

#if defined(MIKTEX_WINDOWS)
  if (SessionWrapper(true)->RunningAsAdministrator())
    {
      RegisterComponents (false, toBeInstalled, toBeRemoved);
    }
#endif

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

#if defined(MIKTEX_WINDOWS)
  if (SessionWrapper(true)->RunningAsAdministrator())
    {
      RegisterComponents (true, toBeInstalled);
    }
#endif

  if (! autoFndbSync)
    {
      // refresh file name database now
      ReportLine (T_("refreshing file name database..."));
      if (! Fndb::Refresh(destinationDirectory, this))
	{
	  throw OperationCancelledException ();
	}
      pManager->CreateMpmFndb ();
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
MIKTEXCALLBACK
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
	  FATAL_MPM_ERROR (T_("PackageInstallerImpl::InstallRemoveThread"),
			   T_("Cannot start installer thread."),
			   NUMTOSTR(hr));
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
	    PathName(destinationDirectory, fileName),
	    expectedSize);
}

/* _________________________________________________________________________

   PackageInstallerImpl::Download
   _________________________________________________________________________ */

void
MPMCALL
PackageInstallerImpl::Download ()
{
  if (repositoryType == RepositoryType::Unknown)
    {
      repository = pManager->PickRepositoryUrl();
      repositoryType = RepositoryType::Remote;
    }

  MIKTEX_ASSERT (repositoryType == RepositoryType::Remote);

  ReportLine (T_("starting download..."));
  ReportLine (T_("URL: %s"), Q_(repository));
  ReportLine (T_("local directory: %s"), Q_(destinationDirectory));

  // download and load the lightweight database
  LoadDbLight (true);
 
  // collect required packages
  MIKTEXCHAR szPackage[BufferSizes::MaxPackageName];
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
	    (destinationDirectory,
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
  for (vector<tstring>::const_iterator it = toBeInstalled.begin();
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
MIKTEXCALLBACK
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
	  FATAL_MPM_ERROR (T_("PackageInstallerImpl::DownloadThread"),
			   T_("Cannot start downloader thread."),
			   NUMTOSTR(hr));
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
  auto_ptr<Extractor>
    pExtractor (Extractor::CreateExtractor(DB_ARCHIVE_FILE_TYPE));
  pExtractor->Extract (pathDatabase,
		       directory,
		       false,
		       0,
		       0);
  pExtractor->Dispose ();
}

/* _________________________________________________________________________

   PackageInstallerImpl::HandleObsoletePackageDefinitionFiles
   _________________________________________________________________________ */

void
PackageInstallerImpl::HandleObsoletePackageDefinitionFiles
(/*[in]*/ const PathName & temporaryDirectory)
{
  PathName pathPackageDir (destinationDirectory,
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

      MIKTEXCHAR szDeploymentName[BufferSizes::MaxPackageName];
      MIKTEX_ASSERT
	(PathName(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX)
	 == (PathName(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX).GetExtension()));
      name.GetFileNameWithoutExtension (szDeploymentName);

      // check to see whether the obsolete package is installed
      if (pManager->GetTimeInstalled(szDeploymentName) == 0
	  || IsPureContainer(szDeploymentName))
	{
	  // not installed: remove the package definition file
	  trace_mpm->WriteFormattedLine (T_("libmpm"),
					 T_("removing obsolete %s"),
					 Q_(name));
	  File::Delete (PathName(pathPackageDir, name), true);
	}
      else
	{
	  // installed: declare the package as obsolete (we wont
	  // uninstall obsolete packages)
	  trace_mpm->WriteFormattedLine (T_("libmpm"),
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
MPMCALL
PackageInstallerImpl::UpdateDb ()
{
#if USE_LOCAL_SERVER
  if (UseLocalServer())
    {
      ConnectToServer ();
      HRESULT hr = localServer.pInstaller->UpdateDb();
      if (FAILED(hr))
	{
	  MiKTeXPackageManagerLib::ErrorInfo errorInfo;
	  HRESULT hr2 = localServer.pInstaller->GetErrorInfo(&errorInfo);
	  if (FAILED(hr2))
	    {
	      FATAL_MPM_ERROR (T_("PackageInstallerImpl::UpdateDb"),
			       T_("The service failed for some reason."),
			       NUMTOSTR(hr));
	    }
	  AutoSysString a (errorInfo.message);
	  AutoSysString b (errorInfo.info);
	  AutoSysString c (errorInfo.sourceFile);
	  Session::FatalMiKTeXError (T_("PackageInstallerImpl::UpdateDb"),
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
  else
    {
      // installing from the CD
      MIKTEX_ASSERT (repositoryType == RepositoryType::MiKTeXDirect);
      tempDir.Set (repository,
		   PrefixedPath(MIKTEX_PATH_PACKAGE_DEFINITION_DIR));
    }

  // handle obsolete package definition files
  HandleObsoletePackageDefinitionFiles (tempDir);

  // update the package definition directory
  PathName packageDefinitionDir (destinationDirectory,
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
      MIKTEXCHAR szDeploymentName[BufferSizes::MaxPackageName];
      MIKTEX_ASSERT
	(PathName(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX)
	 == (PathName(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX).GetExtension()));
      name.GetFileNameWithoutExtension (szDeploymentName);

      // build name of current package definition file
      PathName currentPackageDefinitionfile (packageDefinitionDir, name);

      // ignore package, if package is already installed
      if (! IsPureContainer(szDeploymentName)
	  && pManager->IsPackageInstalled(szDeploymentName)
	  && File::Exists(currentPackageDefinitionfile))
	{
	  continue;
	}

      // parse new package definition file
      PathName newPackageDefinitionFile (tempDir, name);
      tpmparser.Parse (newPackageDefinitionFile);
      
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
MIKTEXCALLBACK
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
  autoFndbSync = false;
  pThread = Thread::Start(pFunc, this);
}

/* _________________________________________________________________________

   PackageInstallerImpl::WaitForCompletion
   _________________________________________________________________________ */

void
MPMCALL
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
  otstringstream message;
  const MIKTEXCHAR * lpszArg1;
  const MIKTEXCHAR * lpszArg2;
  const MIKTEXCHAR * lpszArg3;
  switch (error)
    {
    case ERROR_UNKNOWN_PACKAGE:
      message << T_("The package") << LF
	      << LF
	      << T_("  ") << va_arg(marker, const MIKTEXCHAR *) << LF
	      << LF
	      << T_("is unknown.");
      break;
    case ERROR_DOWNLOAD:
      lpszArg1 = va_arg(marker, const MIKTEXCHAR *);
      lpszArg2 = va_arg(marker, const MIKTEXCHAR *);
      message << T_("The file") << LF
	      << LF
	      << T_("  ") << lpszArg1 << LF
	      << LF
	      << T_("could not be downloaded for the following reason:") << LF
	      << LF
	      << lpszArg2;
      break;
    case ERROR_PACKAGE_NOT_INSTALLED:
      message << T_("The package") << LF
	      << LF
	      << T_("  ") << va_arg(marker, const MIKTEXCHAR *) << LF
	      << LF
	      << T_("is not installed.");
      break;
    case ERROR_CANNOT_DELETE:
      lpszArg1 = va_arg(marker, const MIKTEXCHAR *);
      lpszArg2 = va_arg(marker, const MIKTEXCHAR *);
      message << T_("The file") << LF
	      << LF
	      << T_("  ") << lpszArg1 << LF
	      << LF
	      << T_("could not be be deleted for the following reason:") << LF
	      << LF
	      << lpszArg2;
      break;
    case ERROR_MISSING_PACKAGE:
      message << T_("The package file") << LF
	      << LF
	      << T_("  ") << va_arg(marker, const MIKTEXCHAR *) << LF
	      << LF
	      << T_("is not available.");
      break;
    case ERROR_CORRUPTED_PACKAGE:
      lpszArg1 = va_arg(marker, const MIKTEXCHAR *);
      lpszArg2 = va_arg(marker, const MIKTEXCHAR *);
      lpszArg3 = va_arg(marker, const MIKTEXCHAR *);
      message
	<< T_("The package file") << LF
	<< LF
	<< T_("  ") << lpszArg1 << LF
	<< LF
	<< T_("failed verification due to a MD5 checksum mismatch:") << LF
	<< LF
	<< T_("  ") << lpszArg2 << T_(" (expected MD5 checksum)") << LF
	<< T_("  ") << lpszArg3 << T_(" (actual MD5 checksum)") << LF
	<< LF
	<< T_("This can be the result of a failed download operation.")
	<< T_(" Another reason might be that the local package database")
	<< T_(" is outdated.");
      break;
    case ERROR_SOURCE_FILE_NOT_FOUND:
      message << T_("The file") << LF
	      << LF
	      << T_("  ") << va_arg(marker, const MIKTEXCHAR *) << LF
	      << LF
	      << T_("is not available.");
      break;
    case ERROR_SIZE_MISMATCH:
      {
	lpszArg1 = va_arg(marker, const MIKTEXCHAR *);
	size_t size1 = va_arg(marker, size_t);
	size_t size2 = va_arg(marker, size_t);
	message
	  << T_("The package file") << LF
	  << LF
	  << T_("  ") << lpszArg1 << LF
	  << LF
	  << T_("failed verification due to a size mismatch:") << LF
	  << LF
	  << T_("  ") << size1 << T_(" (expected size)") << LF
	  << T_("  ") << size2 << T_(" (actual size)") << LF
	  << LF
	  << T_("This can be the result of a failed download operation.")
	  << T_(" Another reason might be that the local package database")
	  << T_(" is outdated.");
	break;
      }
    default:
      // this shouldn't happen
      message << T_("An error (") << error << T_(") occurred.");
      break;
    }
  va_end (marker);
  MIKTEX_LOCK(This)
    {
      progressInfo.numErrors += 1;
    }
  MIKTEX_UNLOCK();
  trace_error->WriteLine (T_("libmpm"), message.str().c_str());
  FATAL_MIKTEX_ERROR (0, message.str().c_str(), 0);
}

/* _________________________________________________________________________

   PackageInstallerImpl::ReportLine
   _________________________________________________________________________ */

void
PackageInstallerImpl::ReportLine (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
				  /*[in]*/			...)
{
  if (pCallback == 0)
    {
      return;
    }
  va_list marker;
  va_start (marker, lpszFormat);
  tstring str = Utils::FormatString(lpszFormat, marker);
  va_end (marker);
  pCallback->ReportLine (str.c_str());
}

/* _________________________________________________________________________

   PackageInstallerImpl::Dispose
   _________________________________________________________________________ */

void
MPMCALL
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
}

/* _________________________________________________________________________

   PackageInstallerImpl::UseLocalServer
   _________________________________________________________________________ */

#if USE_LOCAL_SERVER

bool
PackageInstallerImpl::UseLocalServer ()
{
  if (PackageManagerImpl::localServer)
    {
      // already running as local server
      return (false);
    }
#if defined(MIKTEX_WINDOWS)
  bool elevationRequired =
    (IsWindowsVista()
     && ! SessionWrapper(true)->RunningAsAdministrator());
  bool forceLocalServer =
    SessionWrapper(true)->GetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
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

#if USE_LOCAL_SERVER

HRESULT
__stdcall
PackageInstallerImpl::QueryInterface (/*[in]*/ REFIID		riid,
				      /*[out]*/ LPVOID *	ppvObj)
{
  using namespace MiKTeXPackageManagerLib;
  if (trace_mpm->IsEnabled())
    {    
      WCHAR szRiid[100];
      if (StringFromGUID2(riid, szRiid, 100) > 0)
	{
	  trace_mpm->WriteFormattedLine (T_("libmpm"), T_("QI %S"), szRiid);
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

#if USE_LOCAL_SERVER

ULONG
__stdcall
PackageInstallerImpl::AddRef ()
{
  return (1);
}

#endif

/* _________________________________________________________________________

   PackageInstallerImpl::Release
   _________________________________________________________________________ */

#if USE_LOCAL_SERVER

ULONG
__stdcall
PackageInstallerImpl::Release ()
{
  return (1);
}

#endif

/* _________________________________________________________________________

   PackageInstallerImpl::ReportLine
   _________________________________________________________________________ */

#if USE_LOCAL_SERVER

HRESULT
__stdcall
PackageInstallerImpl::ReportLine (/*[in]*/ BSTR line)
{
  try
    {
      if (pCallback != 0)
	{
	  _bstr_t bstr (line, false);
	  pCallback->ReportLine (static_cast<const MIKTEXCHAR *>(bstr));
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

#if USE_LOCAL_SERVER

HRESULT
__stdcall
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
	    (pCallback->OnRetryableError(static_cast<const MIKTEXCHAR *>(bstr))
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

#if USE_LOCAL_SERVER

HRESULT
__stdcall
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
