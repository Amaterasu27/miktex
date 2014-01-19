/* SetupService.cpp:

   Copyright (C) 2013-2014 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.
   
   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include <miktex/Core/Core>
#include <miktex/Core/Paths>
#include <miktex/Core/Registry>
#include <miktex/PackageManager/PackageManager>

#if defined(MIKTEX_WINDOWS)
#  include <ShlObj.h>
#endif

#include "internal.h"

#if defined(MIKTEX_WINDOWS)
#  include "win/winSetupService.h"
#endif

#include "setup-version.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace MiKTeX::Setup;
using namespace std;

#define LICENSE_FILE "LICENSE.TXT"
#define DOWNLOAD_INFO_FILE "README.TXT"

#define BASIC_MIKTEX "\"Basic MiKTeX\""
#define BASIC_MIKTEX_LEGACY "\"Small MiKTeX\""
#define COMPLETE_MIKTEX "\"Complete MiKTeX\""
#define COMPLETE_MIKTEX_LEGACY "\"Total MiKTeX\""
#define ESSENTIAL_MIKTEX "\"Essential MiKTeX\""

/* _________________________________________________________________________

   ComparePaths
   _________________________________________________________________________ */

SETUPSTATICFUNC(int) ComparePaths(const PathName & path1, const PathName & path2, bool shortify)
{
  wchar_t szShortPath1[BufferSizes::MaxPath];
  wchar_t szShortPath2[BufferSizes::MaxPath];

  if (shortify
      && (GetShortPathNameW(path1.ToWideCharString().c_str(),
			    szShortPath1,
			    BufferSizes::MaxPath)
	  > 0)
      && (GetShortPathNameW(path2.ToWideCharString().c_str(),
			    szShortPath2,
			    BufferSizes::MaxPath)
	  > 0))
    {
      return (PathName::Compare(szShortPath1, szShortPath2));
    }
  else
    {
      return (PathName::Compare(path1, path2));
    }
}

/* _________________________________________________________________________

   SetupService::~SetupService
   _________________________________________________________________________ */

SetupService::~SetupService()
{
}

/* _________________________________________________________________________

   SetupServiceImpl::SetupServiceImpl
   _________________________________________________________________________ */

SetupServiceImpl::SetupServiceImpl()
  : refCount (0), logging(false), pCallback(0)
{
  traceStream = auto_ptr<TraceStream>(TraceStream::Open("setup"));
  TraceStream::SetTraceFlags("error,extractor,mpm,process,config,setup");
  pManager = PackageManager::Create();

  string path;
  if (SessionWrapper(true)->TryGetConfigValue(MIKTEX_REGKEY_CORE, MIKTEX_REGVAL_COMMON_INSTALL, path))
  {
    options.Config.commonInstallRoot = path;
  }
  else
  {
#if defined(MIKTEX_WINDOWS)
    // default location: "C:\Program Files\MiKTeX X.Y"
    options.Config.commonInstallRoot = Utils::GetFolderPath(
      CSIDL_PROGRAM_FILES,
      CSIDL_PROGRAM_FILES,
      true);
    options.Config.commonInstallRoot += MIKTEX_PRODUCTNAME_STR " " MIKTEX_SERIES_STR;
#endif
  }

}

/* _________________________________________________________________________

   SetupServiceImpl::~SetupServiceImpl
   _________________________________________________________________________ */

SetupServiceImpl::~SetupServiceImpl()
{
  try
  {
  }
  catch (const exception &)
  {
  }
}

/* _________________________________________________________________________

   SetupService::Create
   _________________________________________________________________________ */

SetupService * SetupService::Create()
{
#if defined(MIKTEX_WINDOWS)
  return new winSetupServiceImpl();
#endif
}

/* _________________________________________________________________________

   SetupServiceImpl::AddRef
   _________________________________________________________________________ */

void SetupServiceImpl::AddRef()
{
  ++ refCount;
}

/* _________________________________________________________________________

   SetupServiceImpl::Release
   _________________________________________________________________________ */

void SetupServiceImpl::Release()
{
  -- refCount;
  if (refCount == 0)
  {
    delete this;
  }
}

/* _________________________________________________________________________

   SetupService::GetDefaultLocalRepository
   _________________________________________________________________________ */

PathName SetupService::GetDefaultLocalRepository()
{
  PathName ret;
  string val;
  if (SessionWrapper(true)->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER, MIKTEX_REGVAL_LOCAL_REPOSITORY, val))
  {
    ret = val;
  }
  else
    {
#if defined(MIKTEX_WINDOWS)
      // try Internet Explorer download directory
      AutoHKEY hkey;
      LONG res = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Internet Explorer", 0, KEY_READ, &hkey);
      if (res == ERROR_SUCCESS)
      {
	DWORD type;
	DWORD len = static_cast<DWORD>(ret.GetCapacity() * sizeof(ret[0]));
	res = RegQueryValueExA(hkey.Get(), "Download Directory", 0, &type, reinterpret_cast<unsigned char *>(ret.GetBuffer()), &len);
      }
      if (res != ERROR_SUCCESS || ! Directory::Exists(ret))
      {
	ret = "";
      }
#endif
      if (ret.Empty())
      {
	// default is "%TEMP%\MiKTeX X.Y Packages"
	ret.SetToTempDirectory();
      }
      ret += MIKTEX_PRODUCTNAME_STR " " MIKTEX_SERIES_STR " ";
      ret.Append (T_("Setup"), false);
  }
  return ret;
}

/* _________________________________________________________________________

   SetupService::SearchLocalRepository
   _________________________________________________________________________ */

PackageLevel SetupService::SearchLocalRepository(/*[out]*/ PathName & localRepository, PackageLevel requestedPackageLevel, /*[out]*/ bool & prefabricated)
{
  PackageLevel packageLevel_ = PackageLevel::None;

  // try current directory
  localRepository.SetToCurrentDirectory();
  packageLevel_ = SetupService::TestLocalRepository(localRepository, requestedPackageLevel);
  if (packageLevel_ != PackageLevel::None)
  {
    prefabricated = true;
    return packageLevel_;
  }

  // try my directory
  localRepository = SessionWrapper(true)->GetMyLocation();
  packageLevel_ = SetupService::TestLocalRepository(localRepository, requestedPackageLevel);
  if (packageLevel_ != PackageLevel::None)
  {
    prefabricated = true;
    return packageLevel_;
  }

  // try ..\tm\packages
  localRepository = SessionWrapper(true)->GetMyLocation();
  localRepository += "..";
  localRepository += "tm";
  localRepository += "packages";
  localRepository.MakeAbsolute ();
  packageLevel_ = SetupService::TestLocalRepository(localRepository, requestedPackageLevel);
  if (packageLevel_ != PackageLevel::None)
  {
    prefabricated = true;
    return packageLevel_;
  }

  // try last directory
  if (PackageManager::TryGetLocalPackageRepository(localRepository))
  {
    packageLevel_ = SetupService::TestLocalRepository(localRepository, requestedPackageLevel);
    if (packageLevel_ != PackageLevel::None)
    {
      prefabricated = false;
      return packageLevel_;
    }
  }

  return PackageLevel::None;
}

/* _________________________________________________________________________

   SetupService::TestLocalRepository
   _________________________________________________________________________ */

PackageLevel SetupService::TestLocalRepository(const PathName & pathRepository, PackageLevel requestedPackageLevel)
{
  PathName pathInfoFile (pathRepository, DOWNLOAD_INFO_FILE);
  if (! File::Exists(pathInfoFile))
  {
    return PackageLevel::None;
  }
  StreamReader stream(pathInfoFile);
  string firstLine;
  bool haveFirstLine = stream.ReadLine(firstLine);
  stream.Close();
  if (! haveFirstLine)
  {
    return PackageLevel::None;
  }
  PackageLevel packageLevel_ = PackageLevel::None;
  if (firstLine.find(ESSENTIAL_MIKTEX) != string::npos)
  {
    packageLevel_ = PackageLevel::Essential;
  }
  else if (firstLine.find(BASIC_MIKTEX) != string::npos)
  {
    packageLevel_ = PackageLevel::Basic;
  }
  else if (firstLine.find(COMPLETE_MIKTEX) != string::npos
    || firstLine.find(COMPLETE_MIKTEX_LEGACY) != string::npos)
  {
    packageLevel_ = PackageLevel::Complete;
  }
  else
  {
    // README.TXT doesn't look right
    return PackageLevel::None;
  }
  if (requestedPackageLevel > packageLevel_)
  {
    // doesn't have the requested package set
    return PackageLevel::None;
  }
  return packageLevel_;
}

/* _________________________________________________________________________

   SetupServiceImpl::SetOptions
   _________________________________________________________________________ */

SetupOptions SetupServiceImpl::SetOptions(const SetupOptions & options)
{
  this->options = options;
  return this->options;
}

/* _________________________________________________________________________

   SetupServiceImpl::OpenLog
   _________________________________________________________________________ */

void SetupServiceImpl::OpenLog()
{
  if (logStream.IsOpen())
  {
    return ;
  }

  // make the intermediate log file name
  intermediateLogFile.SetToTempFile();
  
  // open the intermediate log file
  MIKTEX_LOCK(logStream)
  {
    logStream.Attach(File::Open(intermediateLogFile, FileMode::Create, FileAccess::Write));
    logStream.WriteLine();
    logStream.WriteLine();
  }
  MIKTEX_UNLOCK();
}

/* _________________________________________________________________________

   SetupServiceImpl::CloseLog
   _________________________________________________________________________ */

PathName SetupServiceImpl::CloseLog(bool cancel)
{
  // we must have an intermediate log file
  if (! logStream.IsOpen())
  {
    return "";
  }

  // close the intermediate log file
  logStream.Close();

  if (cancel)
  {
    File::Delete(intermediateLogFile);
    return "";
  }

  // determine the final log directory
  PathName pathLogDir;
  if (options.IsDryRun || options.Task == SetupTask::PrepareMiKTeXDirect)
  {
    pathLogDir.SetToTempDirectory();
  }
  else
  {
    if (options.Task == SetupTask::InstallFromCD || options.Task == SetupTask::InstallFromLocalRepository || options.Task == SetupTask::InstallFromRemoteRepository)
    {
      if (Directory::Exists(GetInstallRoot()))
      {
	pathLogDir.Set(GetInstallRoot(), MIKTEX_PATH_MIKTEX_CONFIG_DIR);
      }
      else
      {
	pathLogDir.SetToTempDirectory();
      }
    }
    else if (options.Task == SetupTask::Download)
    {
      if (Directory::Exists(options.LocalPackageRepository))
      {
	pathLogDir = options.LocalPackageRepository;
      }
      else
      {
	pathLogDir.SetToTempDirectory();
      }
    }
    else
    {
      // remove the intermediate log file
      File::Delete(intermediateLogFile);
      return "";
    }
  }

  // create the log directory
  Directory::Create(pathLogDir);

  // make the final log path name
  PathName pathLogFile(pathLogDir);
  time_t t = time(0);
  char dateTime[128];
  strftime(dateTime, 128, "%Y-%m-%d-%H-%M", localtime(&t));
  string fileName;
  if (options.Task == SetupTask::Download)
  {
    fileName = "download";
  }
  else
  {
    fileName = "setup";
  }
  fileName += "-";
  fileName += dateTime;
  pathLogFile += TU_(fileName);
  pathLogFile.SetExtension(".log");

  // install the log file
  // <todo>add the log file to the uninstall script</todo>
  File::Copy(intermediateLogFile, pathLogFile);

  // remove the intermediate log file
  File::Delete(intermediateLogFile);

  return pathLogFile;
}

/* _________________________________________________________________________

   SetupServiceImpl::LogHeader
   _________________________________________________________________________ */

void SetupServiceImpl::LogHeader()
{
  Log(T_("%s %s Report\n\n"), static_cast<const char *>(TU_(options.Banner)), options.Version);
  time_t t = time(0);
  struct tm * pTm = localtime(&t);
  char dateString[128];
  strftime(dateString, 128, "%A, %B %d, %Y", pTm);
  char timeString[128];
  strftime(timeString, 128, "%H:%M:%S", pTm);
  Log(T_("Date: %s\n"), dateString);
  Log(T_("Time: %s\n"), timeString);
  Log(T_("OS version: %s\n"), Utils::GetOSVersionString().c_str());
#if defined(MIKTEX_WINDOWS)
  if (IsWindowsNT())
  {
    Log("SystemAdmin: %s\n", (SessionWrapper(true)->RunningAsAdministrator() ? "yes" : "false"));
    Log("PowerUser: %s\n", (SessionWrapper(true)->RunningAsPowerUser() ? "yes" : "false"));
  }
#endif
  if (options.Task != SetupTask::Download)
  {
    Log("SharedSetup: %s\n", (options.IsCommonSetup ? "yes" : "false"));
  }
  wchar_t szSetupPath[BufferSizes::MaxPath];
  if (GetModuleFileNameW(0, szSetupPath, BufferSizes::MaxPath) == 0)
  {
    FATAL_WINDOWS_ERROR("GetModuleFileNameW", 0);
  }
  Log(T_("Setup path: %s\n"), WU_(szSetupPath));
  if (options.Task != SetupTask::Download)
  {
    Log("UserRoots: %s\n", (options.Config.userRoots.empty() ? T_("<none specified>") : options.Config.userRoots.c_str()));
    Log("UserData: %s\n", (options.Config.userDataRoot.Empty() ? T_("<none specified>") : options.Config.userDataRoot.Get()));
    Log("UserConfig: %s\n", (options.Config.userConfigRoot.Empty() ? T_("<none specified>") : options.Config.userConfigRoot.Get()));
    Log("CommonRoots: %s\n", (options.Config.commonRoots.empty() ? T_("<none specified>") : options.Config.commonRoots.c_str()));
    Log("CommonData: %s\n", (options.Config.commonDataRoot.Empty() ? T_("<none specified>") : options.Config.commonDataRoot.Get()));
    Log("CommonConfig: %s\n", (options.Config.commonConfigRoot.Empty() ? T_("<none specified>") : options.Config.commonConfigRoot.Get()));
    Log("\nInstallation: %s\n", GetInstallRoot().Get());
  }
}

/* _________________________________________________________________________

   SetupServiceImpl::Log
   _________________________________________________________________________ */

void SetupServiceImpl::Log(const char * lpszFormat, ...)
{
  va_list argList;
  va_start(argList, lpszFormat);
  LogV(lpszFormat, argList);
  va_end(argList);
}

/* _________________________________________________________________________

   SetupServiceImpl::LogV
   _________________________________________________________________________ */

void SetupServiceImpl::LogV(const char * lpszFormat, va_list argList)
{
  MIKTEX_LOCK(logStream);
  if (! logging)
  {
    logging = true;
    LogHeader();
  }
  string formatted = Utils::FormatString(lpszFormat, argList);
  static string currentLine;
  for (const char * lpsz = formatted.c_str(); *lpsz != 0; ++ lpsz)
  {
    if (lpsz[0] == '\n' || (lpsz[0] == '\r' && lpsz[1] == '\n'))
    {
      traceStream->WriteFormattedLine("setup", "%s", currentLine.c_str());
      if (logStream.IsOpen())
      {
	logStream.WriteLine(currentLine);
      }
      currentLine = "";
      if (lpsz[0] == '\r')
      {
	++ lpsz;
      }
    }
    else
    {
      currentLine += *lpsz;
    }
  }
  MIKTEX_UNLOCK();
}

/* _________________________________________________________________________

   SetupServiceImpl::ULogOpen
   _________________________________________________________________________ */

void
SetupServiceImpl::ULogOpen()
{
  PathName uninstLog(GetULogFileName());
  FileMode mode = (File::Exists(uninstLog) ? FileMode::Append : FileMode::Create);
  uninstStream.Attach(File::Open(uninstLog, mode, FileAccess::Write));;
  section = None;
}

/* _________________________________________________________________________

   SetupServiceImpl::GetULogFileName
   _________________________________________________________________________ */

PathName
SetupServiceImpl::GetULogFileName()
{
  PathName ret;
  if (options.IsDryRun || options.Task == SetupTask::PrepareMiKTeXDirect)
  {
    ret.SetToTempDirectory();
  }
  else
  {
    ret = GetInstallRoot();
    ret += MIKTEX_PATH_MIKTEX_CONFIG_DIR;
  }
  Directory::Create(ret);
  ret += MIKTEX_UNINSTALL_LOG;
  return (ret);
}

/* _________________________________________________________________________

   SetupServiceImpl::ULogClose
   _________________________________________________________________________ */

void
SetupServiceImpl::ULogClose(bool finalize)
{
  if (! uninstStream.IsOpen())
  {
    return;
  }

  try
  {
    if (finalize)
    {
      ULogAddFile(GetULogFileName());
      if (! options.IsPortable)
      {
	RegisterUninstaller();
      }
    }
  }
  catch (const exception &)
  {
    uninstStream.Close();
    throw;
  }

  uninstStream.Close();
}

/* _________________________________________________________________________

   SetupServiceImpl::ULogAddFile
   _________________________________________________________________________ */

void SetupServiceImpl::ULogAddFile (const PathName & path)
{
  if (! uninstStream.IsOpen())
  {
    return;
  }
  if (section != Files)
  {
    uninstStream.WriteLine("[files]");
    section = Files;
  }
  PathName absolutePath(path);
  absolutePath.MakeAbsolute();
#if defined(MIKTEX_WINDOWS)
  absolutePath.ToDos();
#endif
  uninstStream.WriteLine(absolutePath.Get());
}

/* _________________________________________________________________________

   SetupServiceImpl::SetCallback
   _________________________________________________________________________ */

void SetupServiceImpl::SetCallback(SetupServiceCallback * pCallback)
{
  this->pCallback = pCallback;
}

/* _________________________________________________________________________

   SetupServiceImpl::Run
   _________________________________________________________________________ */

void SetupServiceImpl::Run()
{
  Initialize();
  switch (options.Task.Get())
  {
  case SetupTask::Download:
    DoTheDownload();
    break;
  case SetupTask::PrepareMiKTeXDirect:
    DoPrepareMiKTeXDirect();
    break;
  case SetupTask::InstallFromCD:
  case SetupTask::InstallFromLocalRepository:
    DoTheInstallation();
    break;
  default:
    UNEXPECTED_CONDITION("SetupServiceImpl::Run");
  }
}
  
/* _________________________________________________________________________

   SetupServiceImpl::Initialize
   _________________________________________________________________________ */

void SetupServiceImpl::Initialize()
{
  pInstaller = auto_ptr<PackageInstaller>(pManager->CreateInstaller());
  pInstaller->SetNoPostProcessing(true);
  pInstaller->SetNoLocalServer(true);
  cancelled = false;

  // complete options
  if (options.Task == SetupTask::Download || options.Task == SetupTask::InstallFromLocalRepository)
  {
    if (options.LocalPackageRepository.Empty())
    {
      PackageLevel foundPackageLevel = SearchLocalRepository(options.LocalPackageRepository, options.PackageLevel, options.IsPrefabricated);
      if (foundPackageLevel == PackageLevel::None)
      {
	// check the default location
	options.LocalPackageRepository = SetupService::GetDefaultLocalRepository();
	foundPackageLevel = SetupService::TestLocalRepository(options.LocalPackageRepository, options.PackageLevel);
      }
      if (options.Task == SetupTask::InstallFromLocalRepository)
      {
	if (foundPackageLevel < options.PackageLevel)
	{
	  FATAL_MIKTEX_ERROR("SetupServiceImp::Initialize", "no local package directory found", 0);
	}
      }
      if (options.PackageLevel == PackageLevel::None)
      {
	options.PackageLevel = foundPackageLevel;
      }
    }
  }
  if (options.PackageLevel == PackageLevel::None)
  {
    options.PackageLevel = PackageLevel::Complete;
  }
  if (options.Config.userInstallRoot.Empty())
  {
    if (options.Task == SetupTask::InstallFromCD ||
      options.Task == SetupTask::InstallFromLocalRepository ||
      options.Task == SetupTask::InstallFromRemoteRepository)
    {
#if defined(MIKTEX_WINDOWS)
      // FIXME
      options.Config.userInstallRoot = "C:\\MiKTeX";
#endif
    }
  }
  if (options.RemotePackageRepository.empty() &&
    (options.Task == SetupTask::Download || options.Task == SetupTask::InstallFromRemoteRepository))
  {
    if (! pManager->TryGetRemotePackageRepository(options.RemotePackageRepository))
    {
      options.RemotePackageRepository = pManager->PickRepositoryUrl();
    }
  }

  // initialize installer
  if (options.Task == SetupTask::InstallFromCD)
  {
    pInstaller->SetRepository(options.MiKTeXDirectRoot.Get());
  }
  else if (options.Task == SetupTask::Download)
  {
    pInstaller->SetRepository(options.RemotePackageRepository);
    pInstaller->SetDownloadDirectory(options.LocalPackageRepository);
    // remember local repository folder
    SessionWrapper(true)->SetConfigValue(
      MIKTEX_REGKEY_PACKAGE_MANAGER,
      MIKTEX_REGVAL_LOCAL_REPOSITORY,
      options.LocalPackageRepository.Get());
    // create the local repository directory
    Directory::Create(options.LocalPackageRepository);
  }
  else if (options.Task == SetupTask::InstallFromLocalRepository)
  {
    pInstaller->SetRepository(options.LocalPackageRepository.Get());
    // remember local repository folder
    if (! options.IsPrefabricated)
    {
      pManager->SetLocalPackageRepository(options.LocalPackageRepository);
    }
  }
  pInstaller->SetPackageLevel(options.PackageLevel);
  pInstaller->SetCallback(this);
}

/* _________________________________________________________________________

   SetupServiceImpl::DoTheDownload
   _________________________________________________________________________ */

void
SetupServiceImpl::DoTheDownload()
{
  // start downloader in the background
  pInstaller->DownloadAsync();

  // wait for downloader thread
  pInstaller->WaitForCompletion ();

  if (cancelled)
  {
    return;
  }

  // copy the license file
  PathName licenseFile;
  if (FindFile(LICENSE_FILE, licenseFile))
  {
    PathName licenseFileDest (options.LocalPackageRepository, LICENSE_FILE);
    if (ComparePaths(licenseFile.Get(), licenseFileDest.Get(), true) != 0)
    {
      File::Copy(licenseFile, licenseFileDest);
    }
  }

  // now copy the setup program
  wchar_t szSetupPath[BufferSizes::MaxPath];
  if (GetModuleFileNameW(0, szSetupPath, BufferSizes::MaxPath) == 0)
  {
    FATAL_WINDOWS_ERROR("GetModuleFileNameW", 0);
  }
  char szFileName[BufferSizes::MaxPath];
  char szExt[BufferSizes::MaxPath];
  PathName::Split(
    WU_(szSetupPath),
    0, 0,
    0, 0,
    szFileName, BufferSizes::MaxPath,
    szExt, BufferSizes::MaxPath);
  PathName pathDest(options.LocalPackageRepository, szFileName, szExt);
  if (ComparePaths(WU_(szSetupPath), pathDest.Get(), true) != 0)
  {
    File::Copy(WU_(szSetupPath), pathDest);
  }

  // create info file
  CreateInfoFile ();
}

/* _________________________________________________________________________

   SetupServiceImpl::DoPrepareMiKTeXDirect
   _________________________________________________________________________ */

void SetupServiceImpl::DoPrepareMiKTeXDirect()
{
  PathName installRoot(options.MiKTeXDirectRoot);
  installRoot += "texmf";
  if (options.IsCommonSetup)
  {
    options.Config.commonInstallRoot = installRoot;
  }
  else
  {
    options.Config.userInstallRoot = installRoot;
  }

  // open the uninstall script
  ULogOpen();
#if 0				// <fixme/>
  ULogAddFile(g_strLogFile);
#endif

  // run IniTeXMF
  ConfigureMiKTeX();

  // create shell links
  if (! options.IsPortable)
  {
    CreateProgramIcons();
  }

  // register path
  if (! options.IsPortable && options.IsRegisterPathEnabled)
  {
    Utils::CheckPath(true);
  }
}

/* _________________________________________________________________________

   SetupServiceImpl::DoTheInstallation
   _________________________________________________________________________ */

void SetupServiceImpl::DoTheInstallation()
{
  // register installation directory
  StartupConfig startupConfig;
  if (options.IsCommonSetup)
  {
    startupConfig.commonInstallRoot = options.Config.commonInstallRoot;
  }
  startupConfig.userInstallRoot = options.Config.userInstallRoot;
  SessionWrapper(true)->RegisterRootDirectories(
    startupConfig,
    RegisterRootDirectoriesFlags::Temporary | RegisterRootDirectoriesFlags::NoRegistry);

  // parse package definition files
  PathName pathDB;
  if (options.Task == SetupTask::InstallFromCD)
  {
    pathDB = options.MiKTeXDirectRoot;
    pathDB += "texmf";
    pathDB += MIKTEX_PATH_PACKAGE_DEFINITION_DIR;
  }
  else
  {
    pathDB = options.LocalPackageRepository;
    pathDB += MIKTEX_MPM_DB_FULL_FILE_NAME;
  }
  if (pCallback != 0)
  {
    pCallback->ReportLine(T_("Loading package database..."));
  }
  pManager->LoadDatabase(pathDB);

  // create the destination directory
  Directory::Create(GetInstallRoot());

  // open the uninstall script
  ULogOpen();
#if 0				// <fixme/>
  ULogAddFile(g_strLogFile);
#endif

  // run installer
  pInstaller->InstallRemove();

  if (cancelled)
  {
    return;
  }

  // install package definition files
  pManager->UnloadDatabase();
  pInstaller->UpdateDb();

  if (cancelled)
  {
    return;
  }

  // run IniTeXMF
  ConfigureMiKTeX();

  if (cancelled)
  {
    return;
  }

  // remove obsolete files
#if 0
  RemoveObsoleteFiles();
#endif

  if (cancelled)
  {
    return;
  }

  // create shell links
  if (! options.IsPortable)
  {
    CreateProgramIcons();
  }

  if (cancelled)
  {
    return;
  }

  // register path
  if (! options.IsPortable && options.IsRegisterPathEnabled)
  {
    Utils::CheckPath(true);
  }

  if (options.IsPortable)
  {
    PathName fileName(options.Config.commonInstallRoot);
    fileName += "miktex-portable.cmd";
    StreamWriter starter(fileName);
    starter.WriteLine("@echo off");
    starter.WriteLine("cd /d %~dp0");
    starter.WriteLine("miktex\\bin\\miktex-taskbar-icon.exe");
    starter.Close();
  }
}

/* _________________________________________________________________________

   SetupServiceImpl::ConfigureMiKTeX
   _________________________________________________________________________ */

void SetupServiceImpl::ConfigureMiKTeX()
{
  PathName initexmf(GetInstallRoot());
  initexmf += MIKTEX_PATH_BIN_DIR;
  initexmf += MIKTEX_INITEXMF_EXE;

  if (pCallback != 0 && ! pCallback->OnProgress(MiKTeX::Setup::Notification::ConfigureBegin))
  {
    cancelled = true;
    return;
  }
  
  CommandLineBuilder cmdLine;

  if (options.Task != SetupTask::PrepareMiKTeXDirect)
  {
    // define roots & remove old fndbs
    cmdLine.Clear ();
    if (options.IsPortable)
    {
      cmdLine.AppendOption("--portable=", options.Config.commonInstallRoot);
    }
    else
    {
      if (! options.Config.userInstallRoot.Empty())
      {
	cmdLine.AppendOption("--user-install=", options.Config.userInstallRoot);
      }
      if (! options.Config.userDataRoot.Empty())
      {
	cmdLine.AppendOption("--user-data=", options.Config.userDataRoot);
      }
      if (! options.Config.userConfigRoot.Empty())
      {
	cmdLine.AppendOption("--user-config=", options.Config.userConfigRoot);
      }
      if (! options.Config.commonDataRoot.Empty())
      {
	cmdLine.AppendOption("--common-data=", options.Config.commonDataRoot);
      }
      if (! options.Config.commonConfigRoot.Empty())
      {
	cmdLine.AppendOption("--common-config=", options.Config.commonConfigRoot);
      }
      if (! options.Config.commonInstallRoot.Empty())
      {
	cmdLine.AppendOption("--common-install=", options.Config.commonInstallRoot);
      }
      if (! options.IsRegistryEnabled)
      {
	cmdLine.AppendOption("--no-registry");
	cmdLine.AppendOption("--create-config-file=", MIKTEX_PATH_MIKTEX_INI);
	cmdLine.AppendOption("--set-config-value=", "[" MIKTEX_REGKEY_CORE "]" MIKTEX_REGVAL_NO_REGISTRY "=1");
      }
    }
    if (! options.Config.commonRoots.empty())
    {
      cmdLine.AppendOption("--common-roots=", options.Config.commonRoots);
    }
    if (! options.Config.userRoots.empty())
    {
      cmdLine.AppendOption("--user-roots=", options.Config.userRoots);
    }
    cmdLine.AppendOption("--rmfndb");
    RunIniTeXMF(cmdLine);
    if (cancelled)
    {
      return;
    }

    // register components, configure files
    RunMpm("--register-components");

    // create filename database files
    cmdLine.Clear();
    cmdLine.AppendOption("--update-fndb");
    RunIniTeXMF(cmdLine);
    if (cancelled)
    {
      return;
    }

    // create latex.exe, ...
    RunIniTeXMF(CommandLineBuilder("--force", "--mklinks"));
    if (cancelled)
    {
      return;
    }

    // create font map files and language.dat
    RunIniTeXMF(CommandLineBuilder("--mkmaps", "--mklangs"));
    if (cancelled)
    {
      return;
    }
  }

  // set paper size
  if (! options.PaperSize.empty())
  {
    cmdLine.Clear();
    if (StringCompare(options.PaperSize.c_str(), "a4", true) == 0)
    {
      cmdLine.AppendOption("--default-paper-size=", "A4size");
    }
    else
    {
    }
  }

  // set auto-install
  string valueSpec = "[" MIKTEX_REGKEY_PACKAGE_MANAGER "]";
  valueSpec += MIKTEX_REGVAL_AUTO_INSTALL;
  valueSpec += "=";
  valueSpec += NUMTOSTR(options.IsInstallOnTheFlyEnabled.Get());
  cmdLine.Clear();
  cmdLine.AppendOption("--set-config-value=", valueSpec.c_str());
  RunIniTeXMF(cmdLine);

  if (options.Task != SetupTask::PrepareMiKTeXDirect)
  {
    // refresh file name database again
    RunIniTeXMF("--update-fndb");
    if (cancelled)
    {
      return;
    }
  }

  if (! options.IsPortable)
  {
    RunIniTeXMF("--register-shell-file-types");
  }
      
  if (! options.IsPortable && options.IsRegisterPathEnabled)
  {
    RunIniTeXMF("--modify-path");
  }

  // create report
  RunIniTeXMF("--report");
  if (cancelled)
  {
    return;
  }
}

/* _________________________________________________________________________

   SetupServiceImpl::RunIniTeXMF
   _________________________________________________________________________ */

void SetupServiceImpl::RunIniTeXMF(const CommandLineBuilder & cmdLine1)
{
  // make absolute exe path name
  PathName exePath;
  exePath = GetInstallRoot();
  exePath += MIKTEX_PATH_BIN_DIR;
  exePath += MIKTEX_INITEXMF_EXE;

  // make command line
  CommandLineBuilder cmdLine (cmdLine1);
  if (options.IsCommonSetup)
  {
    cmdLine.AppendOption("--admin");
  }
  cmdLine.AppendOption("--log-file=", GetULogFileName());
  cmdLine.AppendOption("--verbose");

  // run initexmf.exe
  if (! options.IsDryRun)
  {
    Log("%s %s:\n", Q_(exePath.Get()), cmdLine.Get());
    ULogClose(false);
    SessionWrapper(true)->UnloadFilenameDatabase();
    Process::Run(exePath.Get(), cmdLine.Get(), this);
    ULogOpen();
  }
}

/* _________________________________________________________________________

   SetupServiceImpl::RunMpm
   _________________________________________________________________________ */

void SetupServiceImpl::RunMpm(const CommandLineBuilder & cmdLine1)
{
  // make absolute exe path name
  PathName exePath;
  exePath = GetInstallRoot();
  exePath += MIKTEX_PATH_BIN_DIR;
  exePath += MIKTEX_MPM_EXE;

  // make command line
  CommandLineBuilder cmdLine (cmdLine1);
  if (options.IsCommonSetup)
  {
    cmdLine.AppendOption("--admin");
  }
  cmdLine.AppendOption("--verbose");

  // run mpm.exe
  if (! options.IsDryRun)
  {
    Log("%s %s:\n", Q_(exePath.Get()), cmdLine.Get());
    ULogClose(false);
    SessionWrapper(true)->UnloadFilenameDatabase();
    Process::Run(exePath.Get(), cmdLine.Get(), this);
    ULogOpen();
  }
}

/* _________________________________________________________________________

   SetupServiceImpl::CreateInfoFile
   _________________________________________________________________________ */

void SetupServiceImpl::CreateInfoFile()
{
  StreamWriter stream(PathName(options.LocalPackageRepository, DOWNLOAD_INFO_FILE));
  const char * lpszPackageSet;
  switch (options.PackageLevel.Get())
  {
  case PackageLevel::Essential:
    lpszPackageSet = ESSENTIAL_MIKTEX;
    break;
  case PackageLevel::Basic:
    lpszPackageSet = BASIC_MIKTEX;
    break;
  case PackageLevel::Complete:
    lpszPackageSet = COMPLETE_MIKTEX;
    break;
  default:
    MIKTEX_ASSERT(false);
    __assume(false);
  }
  wchar_t szSetupPath[BufferSizes::MaxPath];
  if (GetModuleFileNameW(0, szSetupPath, BufferSizes::MaxPath) == 0)
  {
    FATAL_WINDOWS_ERROR("GetModuleFileNameW", 0);
  }
  PathName setupExe(szSetupPath);
  setupExe.RemoveDirectorySpec();
  stream.WriteFormattedLine (
    T_("\
This folder contains the %s package set.\n\
\n\
To install MiKTeX, run %s.\n\
\n\
For more information, visit the MiKTeX project page at\n\
http://miktex.org.\n"),
    lpszPackageSet,
    setupExe.Get());
  stream.Close();
  RepositoryInfo repositoryInfo;
  if (pManager->TryGetRepositoryInfo(options.RemotePackageRepository, repositoryInfo))
  {
    StreamWriter stream(PathName(options.LocalPackageRepository, "pr.ini"));
    stream.WriteFormattedLine("[repository]");
    stream.WriteFormattedLine("date=%d", static_cast<int>(repositoryInfo.timeDate));
    stream.WriteFormattedLine("version=%u", static_cast<unsigned>(repositoryInfo.version));
    stream.Close();
  }
}

/* _________________________________________________________________________

   SetupServiceImpl::GetProgressInfo
   _________________________________________________________________________ */

SetupService::ProgressInfo SetupServiceImpl::GetProgressInfo()
{
  PackageInstaller::ProgressInfo pi =  pInstaller->GetProgressInfo();
  ProgressInfo progressInfo;
  progressInfo.deploymentName = pi.deploymentName;
  progressInfo.displayName = pi.displayName;
  progressInfo.fileName = pi.fileName;
  progressInfo.cFilesRemoveCompleted = pi.cFilesRemoveCompleted;
  progressInfo.cFilesRemoveTotal = pi.cFilesRemoveTotal;
  progressInfo.cPackagesRemoveCompleted = pi.cPackagesRemoveCompleted;
  progressInfo.cPackagesRemoveTotal = pi.cPackagesRemoveTotal;
  progressInfo.cbPackageDownloadCompleted = pi.cbPackageDownloadCompleted;
  progressInfo.cbPackageDownloadTotal = pi.cbPackageDownloadTotal;
  progressInfo.cbDownloadCompleted = pi.cbDownloadCompleted;
  progressInfo.cbDownloadTotal = pi.cbDownloadTotal;
  progressInfo.cFilesPackageInstallCompleted = pi.cFilesPackageInstallCompleted;
  progressInfo.cFilesPackageInstallTotal = pi.cFilesPackageInstallTotal;
  progressInfo.cFilesInstallCompleted = pi.cFilesInstallCompleted;
  progressInfo.cFilesInstallTotal = pi.cFilesInstallTotal;
  progressInfo.cPackagesInstallCompleted = pi.cPackagesInstallCompleted;
  progressInfo.cPackagesInstallTotal = pi.cPackagesInstallTotal;
  progressInfo.cbPackageInstallCompleted = pi.cbPackageInstallCompleted;
  progressInfo.cbPackageInstallTotal = pi.cbPackageInstallTotal;
  progressInfo.cbInstallCompleted = pi.cbInstallCompleted;
  progressInfo.cbInstallTotal = pi.cbInstallTotal;
  progressInfo.bytesPerSecond = pi.bytesPerSecond;
  progressInfo.timeRemaining = pi.timeRemaining;
  progressInfo.ready = pi.ready;
  progressInfo.numErrors = pi.numErrors;
  progressInfo.cancelled = pi.cancelled;
  return progressInfo;
}

/* _________________________________________________________________________

   SetupServiceImpl::OnProcessOutput
   _________________________________________________________________________ */

bool SetupServiceImpl::OnProcessOutput(const void * pOutput, size_t n)
{
  if (pCallback != 0 && ! pCallback->OnProcessOutput(pOutput, n))
  {
    cancelled = true;
    return false;
  }
  return true;
}

/* _________________________________________________________________________

   SetupServiceImpl::ReportLine
   _________________________________________________________________________ */

void SetupServiceImpl::ReportLine(const char * lpszLine)
{
  if (pCallback != 0)
  {
    pCallback->ReportLine(lpszLine);
  }
}

/* _________________________________________________________________________

   SetupServiceImpl::OnRetryableError
   _________________________________________________________________________ */

bool SetupServiceImpl::OnRetryableError(const char * lpszMessage)
{
  if (pCallback != 0 && ! pCallback->OnRetryableError(lpszMessage))
  {
    cancelled = true;
    return false;
  }
  return true;
}

/* _________________________________________________________________________

   SetupServiceImpl::OnProgress
   _________________________________________________________________________ */

bool SetupServiceImpl::OnProgress(MiKTeX::Packages::Notification nf)
{
  if (pCallback != 0)
  {
    MiKTeX::Setup::Notification setupNotification(Setup::Notification::None);
    switch (nf.Get())
    {
    case MiKTeX::Packages::Notification::DownloadPackageStart:
      setupNotification = MiKTeX::Setup::Notification::DownloadPackageStart; break;
    case MiKTeX::Packages::Notification::DownloadPackageEnd:
      setupNotification = MiKTeX::Setup::Notification::DownloadPackageEnd; break;
    case MiKTeX::Packages::Notification::InstallFileStart:
      setupNotification = MiKTeX::Setup::Notification::InstallFileStart; break;
    case MiKTeX::Packages::Notification::InstallFileEnd:
      setupNotification = MiKTeX::Setup::Notification::InstallFileEnd; break;
    case MiKTeX::Packages::Notification::InstallPackageStart:
      setupNotification = MiKTeX::Setup::Notification::InstallPackageStart; break;
    case MiKTeX::Packages::Notification::InstallPackageEnd:
      setupNotification = MiKTeX::Setup::Notification::InstallPackageEnd; break;
    case MiKTeX::Packages::Notification::RemoveFileStart:
      setupNotification = MiKTeX::Setup::Notification::RemoveFileStart; break;
    case MiKTeX::Packages::Notification::RemoveFileEnd:
      setupNotification = MiKTeX::Setup::Notification::RemoveFileEnd; break;
    case MiKTeX::Packages::Notification::RemovePackageStart:
      setupNotification = MiKTeX::Setup::Notification::RemovePackageStart; break;
    case MiKTeX::Packages::Notification::RemovePackageEnd:
      setupNotification = MiKTeX::Setup::Notification::RemovePackageEnd; break;
    }
    if (! pCallback->OnProgress(setupNotification))
    {
      cancelled = true;
      return false;
    }
  }
  return true;
}

/* _________________________________________________________________________

   SetupServiceImpl::Expand
   _________________________________________________________________________ */

wstring & SetupServiceImpl::Expand(const char * lpszSource, wstring & dest)
{
  dest = Utils::UTF8ToWideChar(lpszSource);
  wstring::size_type pos;
  while ((pos = dest.find(L"%MIKTEX_INSTALL%")) != wstring::npos)
  {
    dest = dest.replace(pos, 16, GetInstallRoot().ToWideCharString());
  }
  return dest;
}

/* _________________________________________________________________________

   SetupServiceImpl::FindFile
   _________________________________________________________________________ */

bool SetupServiceImpl::FindFile(const PathName & fileName, PathName & result)
{
  // try my directory
  result = SessionWrapper(true)->GetMyLocation();
  result += fileName;
  if (File::Exists(result))
  {
    return true;
  }
  
  // try the current directory
  result.SetToCurrentDirectory ();
  result += fileName;
  if (File::Exists(result))
  {
    return true;
  }

  return false;
}

