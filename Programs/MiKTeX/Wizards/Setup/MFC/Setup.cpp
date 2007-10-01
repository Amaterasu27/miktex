/* Setup.cpp:

   Copyright (C) 1999-2007 Christian Schenk

   This file is part of MiKTeX Setup Wizard.

   MiKTeX Setup Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Setup Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX Setup Wizard; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"
#include "Setup.h"

#include "SetupWizard.h"
#include "ShellLinkData.h"

// this runs the wizard
SetupWizardApplication theApp;

/* _________________________________________________________________________

   SetupCommandLineInfo
   _________________________________________________________________________ */

class SetupCommandLineInfo : public CCommandLineInfo
{
public:
  SetupCommandLineInfo ()
    : optAllowUnattendedReboot (false),
      optDryRun (false),
      optNoAddTEXMFDirs (false),
      optPrivate (false),
      optShared (false),
      optUnattended (false),
      packageLevel (PackageLevel::None),
      task (SetupTask::None)
  {
  }

public:
  tstring folderName;

public:
  StartupConfig startupConfig;

public:
  PathName localPackageRepository;

public:
  tstring remotePackageRepository;

public:
  SetupTask task;

public:
  bool optAllowUnattendedReboot;

public:
  bool optDryRun;

public:
  bool optNoAddTEXMFDirs;

public:
  bool optShared;

public:
  bool optPrivate;

public:
  bool optUnattended;

public:
  PackageLevel packageLevel;
};

/* _________________________________________________________________________

   AddArgument
   _________________________________________________________________________ */

void
AddArgument (/*[in]*/ const CString &		argument,
	     /*[in,out]*/ int &			argc,
	     /*[in,out]*/ MIKTEXCHAR ** &	argv,
	     /*[in,out]*/ int &			argMax)
{
  if (argc == argMax)
    {
      argMax += 10;
      argv =
	reinterpret_cast<MIKTEXCHAR**>(realloc(argv,
					       argMax * sizeof(argv[0])));
    }
  argv[ argc++ ] = _tcsdup(argument);
}

/* _________________________________________________________________________

   GetArguments
   _________________________________________________________________________ */

void
GetArguments (/*[in]*/ const MIKTEXCHAR *		lpszCommandLine,
	      /*[in]*/ const MIKTEXCHAR *		lpszExeName,
	      /*[in,out]*/ int &			argc,
	      /*[in,out]*/ MIKTEXCHAR ** &		argv)
{
  argc = 0;
  argv = 0;

  int argMax = 0;

  AddArgument (lpszExeName, argc, argv, argMax);

  CString arg;

  bool copying = false;
  bool inQuotedArg = false;

  const MIKTEXCHAR * lpsz = lpszCommandLine;

  while (*lpsz != 0)
    {
      if (*lpsz == T_(' ') && ! inQuotedArg)
	{
	  if (copying)
	    {
	      AddArgument (arg, argc, argv, argMax);
	      arg = T_("");
	      copying = false;
	    }
	}
      else if (*lpsz == T_('"'))
	{
	  inQuotedArg = ! inQuotedArg;
	  copying = true;
	}
      else
	{
	  arg += *lpsz;
	  copying = true;
	}
      ++ lpsz;
    }

  if (copying)
    {
      AddArgument (arg, argc, argv, argMax);
    }
}

/* _________________________________________________________________________

   FreeArguments
   _________________________________________________________________________ */

void
FreeArguments (/*[in]*/ int			argc,
	       /*[in,out]*/ MIKTEXCHAR ** & 	argv)
{
  for (int i = 0; i < argc; ++ i)
    {
      free (argv[i]);
      argv[i] = 0;
    }
  free (argv);
  argv = 0;
}

/* _________________________________________________________________________

   long_options / short_options
   _________________________________________________________________________ */

enum {
  OPT_AAA = 1000,
  OPT_ALLOW_UNATTENDED_REBOOT,
  OPT_COMMON_CONFIG,
  OPT_COMMON_DATA,
  OPT_DOWNLOAD_ONLY,
  OPT_DRY_RUN,
  OPT_HELP,
  OPT_INSTALL_FROM_LOCAL_REPOSITORY,
  OPT_INSTALL_ROOT,
  OPT_LOCAL_PACKAGE_REPOSITORY,
  OPT_NO_ADDITIONAL_ROOTS,
  OPT_PACKAGE_SET,
  OPT_PRIVATE,
  OPT_PROGRAM_FOLDER,
  OPT_REMOTE_PACKAGE_REPOSITORY,
  OPT_ROOTS,
  OPT_SHARED,
  OPT_UNATTENDED,
  OPT_USER_CONFIG,
  OPT_USER_DATA,
};

const struct option long_options[] =
{
  { T_("allow-unattended-reboot"), no_argument, 0,
    OPT_ALLOW_UNATTENDED_REBOOT },
  { T_("common-config"), required_argument, 0, OPT_COMMON_CONFIG },
  { T_("common-data"), required_argument, 0, OPT_COMMON_DATA },
  { T_("download-only"), no_argument, 0, OPT_DOWNLOAD_ONLY },
  { T_("dry-run"), no_argument, 0, OPT_DRY_RUN },
  { T_("help"), no_argument, 0, OPT_HELP },
  { T_("install-from-local-repository"), no_argument, 0,
    OPT_INSTALL_FROM_LOCAL_REPOSITORY },
  { T_("install-root"), required_argument, 0, OPT_INSTALL_ROOT },
  { T_("local-package-repository"), required_argument, 0,
    OPT_LOCAL_PACKAGE_REPOSITORY },
  { T_("no-additional-roots"), no_argument, 0, OPT_NO_ADDITIONAL_ROOTS },
  { T_("package-set"), required_argument, 0, OPT_PACKAGE_SET },
  { T_("private"), no_argument, 0, OPT_PRIVATE },
  { T_("program-folder"), required_argument, 0, OPT_PROGRAM_FOLDER },
  { T_("remote-package-repository"), required_argument, 0,
    OPT_REMOTE_PACKAGE_REPOSITORY },
  { T_("roots"), required_argument, 0, OPT_ROOTS },
  { T_("shared"), no_argument, 0, OPT_SHARED },
  { T_("unattended"), no_argument, 0, OPT_UNATTENDED },
  { T_("user-config"), required_argument, 0, OPT_USER_CONFIG },
  { T_("user-data"), required_argument, 0, OPT_USER_DATA },
  { 0, no_argument, 0, 0 }
};

/* _________________________________________________________________________

   ShowHelpAndExit
   _________________________________________________________________________ */

void
ShowHelpAndExit (/*[in]*/ int retCode = 0)
{
  AfxMessageBox (T_("Usage: setupwiz [OPTIONS]\r\n\r\n\
Options:\r\n\r\n\
  --allow-unattended-reboot\r\n\
  --common-config=DIR\r\n\
  --common-data=DIR\r\n\
  --download-only\r\n\
  --dry-run\r\n\
  --help\r\n\
  --install-from-local-repository\r\n\
  --install-root=DIR\r\n\
  --local-package-repository=DIR\r\n\
  --no-additional-roots\r\n\
  --package-set=SET\r\n\
  --private\r\n\
  --program-folder=NAME\r\n\
  --remote-package-repository=URL\r\n\
  --roots=DIRS\r\n\
  --shared\r\n\
  --unattended\r\n\
  --user-config=DIR\r\n\
  --user-data=DIR")
		 T_("\r\n\r\n\
setupwiz reads its arguments from setupwiz.opt, if such a file exists.\r\n\
See the MiKTeX Manual for more information."));
  exit (retCode);
}

/* _________________________________________________________________________

   ParseSetupCommandLine
   _________________________________________________________________________ */

void
ParseSetupCommandLine (/*[in]*/ int				argc,
		       /*[in]*/ MIKTEXCHAR **			argv,
		       /*[in,out]*/ SetupCommandLineInfo &	cmdinfo)
{
  int iOptIdx = 0;
  int c;

  cmdinfo.m_nShellCommand = CCommandLineInfo::FileNothing;

  optind = 0;

  while ((c = getopt_long_only(argc, argv, T_(""), long_options, &iOptIdx))
	 != EOF)
    {
      switch (c)
	{

	case OPT_ALLOW_UNATTENDED_REBOOT:
	  cmdinfo.optAllowUnattendedReboot = true;
	  break;

	case OPT_COMMON_CONFIG:
	  if (IsWindowsNT()
	      && ! (SessionWrapper(true)->RunningAsAdministrator()
		    || SessionWrapper(true)->RunningAsPowerUser()))
	    {
	      FATAL_MIKTEX_ERROR
		(T_("ParseSetupCommandLine"),
		 T_("You must have administrator privileges to set up \
a common configuration directory."),
		 0);
	    }
	  cmdinfo.startupConfig.commonConfigRoot = optarg;
	  break;

	case OPT_COMMON_DATA:
	  if (IsWindowsNT()
	      && ! (SessionWrapper(true)->RunningAsAdministrator()
		    || SessionWrapper(true)->RunningAsPowerUser()))
	    {
	      FATAL_MIKTEX_ERROR
		(T_("ParseSetupCommandLine"),
		 T_("You must have administrator privileges to set up \
a common data directory."),
		 0);
	    }
	  cmdinfo.startupConfig.commonDataRoot = optarg;
	  break;

	case OPT_DOWNLOAD_ONLY:
	  cmdinfo.task = SetupTask::Download;
	  break;

	case OPT_DRY_RUN:
	  cmdinfo.optDryRun = true;
	  break;

	case OPT_HELP:
	  FreeArguments (argc, argv);
	  ShowHelpAndExit ();
	  break;

	case OPT_INSTALL_ROOT:
	  cmdinfo.startupConfig.installRoot = optarg;
	  break;

	case OPT_INSTALL_FROM_LOCAL_REPOSITORY:
	  cmdinfo.task = SetupTask::InstallFromLocalRepository;
	  break;

	case OPT_LOCAL_PACKAGE_REPOSITORY:
	  cmdinfo.localPackageRepository = optarg;
	  break;

	case OPT_NO_ADDITIONAL_ROOTS:
	  cmdinfo.optNoAddTEXMFDirs = true;
	  break;

	case OPT_PACKAGE_SET:
	  if (StringCompare(optarg, T_("essential")) == 0)
	    {
	      cmdinfo.packageLevel = PackageLevel::Essential;
	    }
	  else if (StringCompare(optarg, T_("basic")) == 0)
	    {
	      cmdinfo.packageLevel = PackageLevel::Basic;
	    }
	  else if (StringCompare(optarg, T_("advanced")) == 0)
	    {
	      cmdinfo.packageLevel = PackageLevel::Advanced;
	    }
	  else if (StringCompare(optarg, T_("complete")) == 0)
	    {
	      cmdinfo.packageLevel = PackageLevel::Complete;
	    }
	  else
	    {
	      FATAL_MIKTEX_ERROR
		(T_("ParseSetupCommandLine"),
		 T_("Invalid package set."),
		 0);
	    }
	  break;

	case OPT_PRIVATE:
	  cmdinfo.optPrivate = true;
	  break;

	case OPT_PROGRAM_FOLDER:
	  cmdinfo.folderName = optarg;
	  break;
	  
	case OPT_REMOTE_PACKAGE_REPOSITORY:
	  cmdinfo.remotePackageRepository = optarg;
	  break;

	case OPT_ROOTS:
	  cmdinfo.startupConfig.roots = optarg;
	  break;

	case OPT_SHARED:
	  if (IsWindowsNT()
	      && ! (SessionWrapper(true)->RunningAsAdministrator()
		    || SessionWrapper(true)->RunningAsPowerUser()))
	    {
	      FATAL_MIKTEX_ERROR
		(T_("ParseSetupCommandLine"),
		 T_("You must have administrator privileges to set up \
a shared MiKTeX system."),
		 0);
	    }
	  cmdinfo.optShared = true;
	  break;

	case OPT_UNATTENDED:
	  cmdinfo.optUnattended = true;
	  break;

	case OPT_USER_CONFIG:
	  cmdinfo.startupConfig.userConfigRoot = optarg;
	  break;

	case OPT_USER_DATA:
	  cmdinfo.startupConfig.userDataRoot = optarg;
	  break;

	default:
	  FreeArguments (argc, argv);
	  ShowHelpAndExit (1);
	  break;
	}
    }
}

/* _________________________________________________________________________

   FindFile
   _________________________________________________________________________ */

bool
FindFile (/*[in]*/ const PathName &	fileName,
	  /*[out]*/ PathName &		result)
{
  // try my directory
  result = SessionWrapper(true)->GetMyLocation();
  result += fileName;
  if (File::Exists(result))
    {
      return (true);
    }
  
  // try the current directory
  result.SetToCurrentDirectory ();
  result += fileName;
  if (File::Exists(result))
    {
      return (true);
    }

  return (false);
}

/* _________________________________________________________________________

   ReadSetupWizIni
   _________________________________________________________________________ */

bool
ReadSetupWizIni (/*[in,out]*/ SetupCommandLineInfo &	cmdinfo)
{
  PathName fileName;
  if (! FindFile(T_("setupwiz.opt"), fileName))
    {
      return (false);
    }
  StreamReader reader (fileName);
  CString commandLine;
  tstring line;
  while (reader.ReadLine(line))
    {
      commandLine += T_(' ');
      commandLine += line.c_str();
    }
  reader.Close ();
  int argc;
  MIKTEXCHAR ** argv;
  GetArguments (commandLine, AfxGetAppName(), argc, argv);
  ParseSetupCommandLine (argc, argv, cmdinfo);
  FreeArguments (argc, argv);
  return (true);
}

/* _________________________________________________________________________

   SetupWizardApplication Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(SetupWizardApplication, CWinApp)
#if 0
  ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
#endif
END_MESSAGE_MAP();

/* _________________________________________________________________________

   SetupWizardApplication::SetupWizardApplication
   _________________________________________________________________________ */

SetupWizardApplication::SetupWizardApplication ()
  : packageLevel (PackageLevel::None),
    installOnTheFly (TriState::Undetermined),
    setupTask (SetupTask::None)
{
}

/* _________________________________________________________________________

   FindInstallDir
   _________________________________________________________________________ */

PathName
FindInstallDir ()
{
  // probe the registry
  tstring path;
  if (SessionWrapper(true)
      ->TryGetConfigValue(MIKTEX_REGKEY_CORE,
			  MIKTEX_REGVAL_INSTALL,
			  path))
    {
      return (path);
    }
  else
    {
      // return the default location (usually "C:\Program Files\MiKTeX X.Y")
      PathName path;
      if (! IsWindowsNT()
	  || SessionWrapper(true)->IsUserAnAdministrator()
	  || SessionWrapper(true)->IsUserAPowerUser())
	{
	  path =
	    Utils::GetFolderPath(CSIDL_PROGRAM_FILES,
				 CSIDL_PROGRAM_FILES,
				 true);
	}
      else
	{
	  path =
	    Utils::GetFolderPath(CSIDL_LOCAL_APPDATA,
				 CSIDL_APPDATA,
				 true);
	}
      path += T_(MIKTEX_PRODUCTNAME_STR) T_(" ") T_(MIKTEX_SERIES_STR);
      return (path);
    }
}

/* _________________________________________________________________________

   FindAddTEXMFDirs
   _________________________________________________________________________ */

tstring
FindAddTEXMFDirs ()
{
  tstring directories;
  if (! SessionWrapper(true)
      ->TryGetConfigValue(MIKTEX_REGKEY_CORE,
			  MIKTEX_REGVAL_ROOTS,
			  directories))
    {
      directories = T_("");
    }
  return (directories);
}

/* _________________________________________________________________________

   CheckAddTEXMFDirs
   _________________________________________________________________________ */

void
CheckAddTEXMFDirs (/*[in,out]*/ tstring &	directories,
		   /*[out]*/ vector<PathName> &	vec)
{
  CSVList path (directories.c_str(), T_(';'));

  vec.clear ();
  directories = T_("");

  for (; path.GetCurrent() != 0; ++ path)
    {
      if (! (theApp.startupConfig.userDataRoot == path.GetCurrent()
	     || theApp.startupConfig.userConfigRoot == path.GetCurrent()
	     || theApp.startupConfig.commonDataRoot == path.GetCurrent()
	     || theApp.startupConfig.commonConfigRoot == path.GetCurrent()
	     || theApp.startupConfig.installRoot == path.GetCurrent()))
	{
	  if (vec.size() > 0)
	    {
	      directories += T_(';');
	    }
	  vec.push_back (path.GetCurrent());
	  directories += path.GetCurrent();
	}
    }
}

/* _________________________________________________________________________

   TestLocalRepository

   Check to see whether README.TXT exists in the local repository.
   _________________________________________________________________________ */

bool
TestLocalRepository (/*[in]*/ const PathName &		pathRepository,
		     /*[in,out]*/ PackageLevel &	packageLevel)
{
  PathName pathInfoFile (pathRepository, DOWNLOAD_INFO_FILE);
  if (! File::Exists(pathInfoFile))
    {
      return (false);
    }
  StreamReader stream (pathInfoFile);
  tstring firstLine;
  bool haveFirstLine = stream.ReadLine(firstLine);
  stream.Close ();
  if (! haveFirstLine)
    {
      return (false);
    }
  PackageLevel packageLevel_ = PackageLevel::None;
  if (firstLine.find(ESSENTIAL_MIKTEX) != tstring::npos)
    {
      packageLevel_ = PackageLevel::Essential;
    }
  else if (firstLine.find(BASIC_MIKTEX) != tstring::npos)
    {
      packageLevel_ = PackageLevel::Basic;
    }
  else if (firstLine.find(COMPLETE_MIKTEX) != tstring::npos
	   || firstLine.find(COMPLETE_MIKTEX_LEGACY) != tstring::npos)
    {
      packageLevel_ = PackageLevel::Complete;
    }
  else
    {
      // README.TXT doesn't look right
      return (false);
    }
  if (packageLevel > packageLevel_)
    {
      // doesn't have the requested package set
      return (false);
    }
  if (packageLevel == PackageLevel::None)
    {
      packageLevel = packageLevel_;
    }
  return (true);
}

/* _________________________________________________________________________

   SearchLocalRepository
   _________________________________________________________________________ */

bool
SearchLocalRepository (/*[out]*/ PathName &		localRepository,
		       /*[in,out]*/ PackageLevel &	packageLevel,
		       /*[out]*/ bool &			prefabricated)
{
  // try current directory
  localRepository.SetToCurrentDirectory ();
  if (TestLocalRepository(localRepository, packageLevel))
    {
      prefabricated = true;
      return (true);
    }

  // try my directory
  localRepository = SessionWrapper(true)->GetMyLocation();
  if (TestLocalRepository(localRepository, packageLevel))
    {
      prefabricated = true;
      return (true);
    }

  // try ..\tm\packages
  localRepository = SessionWrapper(true)->GetMyLocation();
  localRepository += T_("..");
  localRepository += T_("tm");
  localRepository += T_("packages");
  localRepository.MakeAbsolute ();
  if (TestLocalRepository(localRepository, packageLevel))
    {
      prefabricated = true;
      return (true);
    }

  // try last directory
  if (PackageManager::TryGetLocalPackageRepository(localRepository)
      && TestLocalRepository(localRepository, packageLevel))
    {
      prefabricated = false;
      return (true);
    }

  return (false);
}

/* _________________________________________________________________________

   IsMiKTeXDirect
   _________________________________________________________________________ */

bool
IsMiKTeXDirectRoot (/*[out]*/ PathName & MiKTeXDirectRoot)
{
  // check ..\texmf\miktex\config\md.ini
  MiKTeXDirectRoot = SessionWrapper(true)->GetMyLocation();
  MiKTeXDirectRoot += T_("..");
  MiKTeXDirectRoot.MakeAbsolute ();
  PathName pathMdIni = MiKTeXDirectRoot;
  pathMdIni += T_("texmf");
  pathMdIni += MIKTEX_PATH_MD_INI;
  if (! File::Exists(pathMdIni))
    {
      return (false);
    }
  Log (T_("started from MiKTeXDirect location\n"));
  return (true);
}

/* _________________________________________________________________________

   GetDefaultLocalRepository
   _________________________________________________________________________ */

PathName
GetDefaultLocalRepository ()
{
  PathName ret;
  tstring val;
  if (SessionWrapper(true)
      ->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			  MIKTEX_REGVAL_LOCAL_REPOSITORY,
			  val))
    {
      ret = val;
    }
  else
    {
      // default is "%TEMP%\MiKTeX X.Y Packages"
      ret.SetToTempDirectory ();
      ret +=
	(T_(MIKTEX_PRODUCTNAME_STR)
	 T_(" ") T_(MIKTEX_SERIES_STR)
	 T_(" Packages"));
    }
  return (ret);
}

/* _________________________________________________________________________

   SetupGlobalVars
   _________________________________________________________________________ */

void
SetupGlobalVars (/*[in]*/ const SetupCommandLineInfo &	cmdinfo)
{
  theApp.allowUnattendedReboot = cmdinfo.optAllowUnattendedReboot;
  theApp.dryRun = cmdinfo.optDryRun;
  theApp.mustReboot = false;
  theApp.prefabricated = false;
  theApp.registerPath = true;
  theApp.showLogFileOnExit = false;
  theApp.unattended = cmdinfo.optUnattended;
  theApp.packageLevel = cmdinfo.packageLevel;

  // check to see whether setup is started from a MiKTeXDirect location
  theApp.isMiKTeXDirect = IsMiKTeXDirectRoot(theApp.MiKTeXDirectRoot);
  if (theApp.isMiKTeXDirect)
    {
      theApp.MiKTeXDirectTeXMFRoot = theApp.MiKTeXDirectRoot;
      theApp.MiKTeXDirectTeXMFRoot += T_("texmf");
    }

  // startup configuration
  theApp.startupConfig = cmdinfo.startupConfig;
  if (theApp.startupConfig.installRoot.Empty())
    {
      theApp.startupConfig.installRoot = FindInstallDir();
    }
  if (theApp.startupConfig.roots.empty())
    {
      theApp.startupConfig.roots = FindAddTEXMFDirs();
    }
  if (! theApp.startupConfig.roots.empty())
    {
      CheckAddTEXMFDirs (theApp.startupConfig.roots, theApp.addTEXMFDirs);
    }

  theApp.noAddTEXMFDirs = cmdinfo.optNoAddTEXMFDirs;

  // shared setup
  theApp.commonUserSetup =
    ((IsWindowsNT() && SessionWrapper(true)->RunningAsAdministrator())
     || cmdinfo.optShared
     || ! cmdinfo.startupConfig.commonDataRoot.Empty()
     || ! cmdinfo.startupConfig.commonConfigRoot.Empty());

  // startup menu item (default: "MiKTeX X.Y")
  theApp.folderName = cmdinfo.folderName;
  if (theApp.folderName.empty())
    {
      theApp.folderName =
	T_(MIKTEX_PRODUCTNAME_STR) T_(" ") T_(MIKTEX_SERIES_STR);
    }

  // local package repository
  if (! cmdinfo.localPackageRepository.Empty())
    {
      theApp.localPackageRepository = cmdinfo.localPackageRepository;
      if (cmdinfo.task != SetupTask::Download
	  && ! TestLocalRepository(theApp.localPackageRepository,
				   theApp.packageLevel))
	{
	  FATAL_MIKTEX_ERROR
	    (T_("SetupGlobalVars"),
	     T_("The specified local repository does not exist."),
	     0);
	}
    }
  else if (! SearchLocalRepository(theApp.localPackageRepository,
				   theApp.packageLevel,
				   theApp.prefabricated))
    {
      // check the default location
      theApp.localPackageRepository = GetDefaultLocalRepository();
      TestLocalRepository (theApp.localPackageRepository, theApp.packageLevel);
    }

  // setup task
  theApp.setupTask = cmdinfo.task;
  if (theApp.setupTask == SetupTask::None)
    {
      if (theApp.isMiKTeXDirect)
	{
	  theApp.setupTask = SetupTask::PrepareMiKTeXDirect;
	}
      else if (! theApp.localPackageRepository.Empty()
	       && theApp.packageLevel != PackageLevel::None)
	{
	  theApp.setupTask = SetupTask::InstallFromLocalRepository;
	}
    }

  // remote package repository
  theApp.remotePackageRepository = cmdinfo.remotePackageRepository;
  if (theApp.remotePackageRepository.empty())
    {
      tstring str;
      if (theApp.pManager->TryGetRemotePackageRepository(str))
	{
	  theApp.remotePackageRepository = str.c_str();
	}
    }

  // check variables, if started in unattended mode
  if (theApp.unattended)
    {
      if (theApp.setupTask == SetupTask::None)
	{
	  FATAL_MIKTEX_ERROR
	    (T_("SetupGlobalVars"),
	     T_("No setup task has been specified."),
	     0);
	}
      if (theApp.packageLevel == PackageLevel::None)
	{
	  FATAL_MIKTEX_ERROR
	    (T_("SetupGlobalVars"),
	     T_("No package set has been specified."),
	     0);
	}
      if (theApp.setupTask == SetupTask::InstallFromLocalRepository
	  || theApp.setupTask == SetupTask::Download)
	{
	  if (theApp.localPackageRepository.Empty())
	    {
	      FATAL_MIKTEX_ERROR
		(T_("SetupGlobalVars"),
		 T_("No local package repository has been specified."),
		 0);
	    }
	}
      if (theApp.setupTask == SetupTask::InstallFromRemoteRepository
	  || theApp.setupTask == SetupTask::Download)
	{
	  if (theApp.remotePackageRepository.empty())
	    {
	      FATAL_MIKTEX_ERROR
		(T_("SetupGlobalVars"),
		 T_("No remote package repository has been specified."),
		 0);
	    }
	}
    }
}

/* _________________________________________________________________________

   OpenLog
   _________________________________________________________________________ */

void
OpenLog ()
{
  if (theApp.logStream.IsOpen())
    {
      return ;
    }

  // make the intermediate log file name
  theApp.intermediateLogFile.SetToTempFile ();
  
  // open the intermediate log file
  {
    CSingleLock singleLock (&theApp.criticalSectionMonitorLogStream, TRUE);
    theApp.logStream.Attach (File::Open(theApp.intermediateLogFile,
					FileMode::Create,
					FileAccess::Write));
    theApp.logStream.WriteLine ();
    theApp.logStream.WriteLine ();
  }
}

/* _________________________________________________________________________

   CloseLog
   _________________________________________________________________________ */

void
CloseLog (/*[in]*/ bool cancel)
{
  // we must have an intermediate log file
  if (! theApp.logStream.IsOpen())
    {
      return;
    }

  // close the intermediate log file
  theApp.logStream.Close ();

  if (cancel)
    {
      File::Delete (theApp.intermediateLogFile);
      return;
    }

  // determine the final log directory
  PathName pathLogDir;
  if (theApp.dryRun || theApp.setupTask == SetupTask::PrepareMiKTeXDirect)
    {
      pathLogDir.SetToTempDirectory ();
    }
  else
    {
      if (theApp.setupTask == SetupTask::InstallFromCD
	  || theApp.setupTask == SetupTask::InstallFromLocalRepository
	  || theApp.setupTask == SetupTask::InstallFromRemoteRepository)
	{
	  if (Directory::Exists(theApp.startupConfig.installRoot))
	    {
	      pathLogDir.Set (theApp.startupConfig.installRoot,
			      MIKTEX_PATH_MIKTEX_CONFIG_DIR);
	    }
	  else
	    {
	      pathLogDir.SetToTempDirectory ();
	    }
	}
      else if (theApp.setupTask == SetupTask::Download)
	{
	  if (Directory::Exists(theApp.localPackageRepository))
	    {
	      pathLogDir = theApp.localPackageRepository;
	    }
	  else
	    {
	      pathLogDir.SetToTempDirectory ();
	    }
	}
      else
	{
	  // remove the intermediate log file
	  File::Delete (theApp.intermediateLogFile);
	  return;
	}
    }

  // create the log directory
  Directory::Create (pathLogDir);

  // make the final log path name
  PathName pathLogFile (pathLogDir);
  CTime t = CTime::GetCurrentTime();
  if (theApp.setupTask == SetupTask::Download)
    {
      pathLogFile += t.Format(T_("download-%Y-%m-%d-%H-%M"));
    }
  else
    {
      pathLogFile += t.Format(T_("setup-%Y-%m-%d-%H-%M"));
    }
  pathLogFile.SetExtension (T_(".log"));

  // install the log file
  // <todo>add the log file to the uninstall script</todo>
  File::Copy (theApp.intermediateLogFile, pathLogFile);

  // remove the intermediate log file
  File::Delete (theApp.intermediateLogFile);

  if (theApp.showLogFileOnExit)
    {
      INT_PTR r =
	reinterpret_cast<INT_PTR>(ShellExecute(0,
					       T_("open"),
					       pathLogFile.Get(),
					       0,
					       0,
					       SW_SHOWNORMAL));
      if (r <= 32)
	{
	  Process::Start (T_("notepad.exe"), pathLogFile.Get());
	}
    }
}

/* _________________________________________________________________________

   ExtractFiles
   _________________________________________________________________________ */

bool
ExtractFiles (/*[in,out]*/ ScratchDirectory &	sfxDir)
{
  PathName path;
  if (GetModuleFileName(0, path.GetBuffer(), BufferSizes::MaxPath) == 0)
    {
      FATAL_WINDOWS_ERROR (T_("GetModuleFileName"), 0);
    }
  FileStream myImage (File::Open(path,
				 FileMode::Open,
				 FileAccess::Read,
				 false));
  char magic[16];
  while (myImage.Read(magic, 16) == 16)
    {
      if (memcmp(magic, "TARTARTARTARTART", 16) == 0)
	{
	  sfxDir.Enter ();
	  auto_ptr<MiKTeX::Extractor::Extractor>
	    pExtractor
	    (MiKTeX::Extractor::Extractor::CreateExtractor
	     (MiKTeX::Extractor::ArchiveFileType::Tar));
	  pExtractor->Extract (&myImage,
			       Directory::GetCurrentDirectory(),
			       true,
			       0,
			       0);
	  pExtractor->Dispose ();
	  return (true);
	}
      else
	{
	  myImage.Seek (512 - 16, SeekOrigin::Current);
	}
    }
  return (false);
}

/* _________________________________________________________________________

   SetupWizardApplication::InitInstance
   _________________________________________________________________________ */

BOOL
SetupWizardApplication::InitInstance ()
{
  INITCOMMONCONTROLSEX initCtrls;

  initCtrls.dwSize = sizeof(initCtrls);
  initCtrls.dwICC = ICC_WIN95_CLASSES;

  if (! InitCommonControlsEx(&initCtrls))
    {
      AfxMessageBox (T_("The application could not be initialized (1)."),
		     MB_ICONSTOP | MB_OK);
      return (FALSE);
    }

  if (FAILED(CoInitialize(0)))
    {
      AfxMessageBox (T_("The application could not be initialized (2)."),
		     MB_ICONSTOP | MB_OK);
      return (FALSE);
    }

  AfxInitRichEdit2 ();

  try
    {
      // create a scratch root directory
      TempDirectory scratchRoot;

      // create a MiKTeX session
      StartupConfig startupConfig;
      startupConfig.userDataRoot = scratchRoot.Get();
      startupConfig.userConfigRoot = scratchRoot.Get();
      startupConfig.commonDataRoot = scratchRoot.Get();
      startupConfig.commonConfigRoot = scratchRoot.Get();
      startupConfig.installRoot = scratchRoot.Get();
      Session::InitInfo initInfo (T_("setup"),
				  Session::InitFlags::NoConfigFiles);
      initInfo.SetStartupConfig (startupConfig);
      SessionWrapper pSession (initInfo);

      // extract package archive files
      ScratchDirectory sfxDir;
      bool selfExtractor = ExtractFiles(sfxDir);

      // create package manager
      pManager = PackageManager::Create();

      // set trace options
      traceStream = auto_ptr<TraceStream>(TraceStream::Open(T_("setup")));
      TraceStream::SetTraceFlags
	(T_("error,extractor,mpm,process,config,setup"));
      
      // get command-line arguments
      int argc;
      MIKTEXCHAR ** argv;
      GetArguments (m_lpCmdLine, AfxGetAppName(), argc, argv);
      SetupCommandLineInfo cmdinfo;
      if (GetModuleFileName(0, setupPath.GetBuffer(), BufferSizes::MaxPath)
	  == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("GetModuleFileName"), 0);
	}
      ReadSetupWizIni (cmdinfo);
      ParseSetupCommandLine (argc, argv, cmdinfo);
      FreeArguments (argc, argv);
      if (cmdinfo.optPrivate && cmdinfo.optShared)
	{
	  FATAL_MIKTEX_ERROR
	    (T_("SetupWizardApplication::InitInstance"),
	     T_("You cannot specify --private along with --shared."),
	     0);
	}
      if (cmdinfo.optPrivate
	  && ! (cmdinfo.startupConfig.commonDataRoot.Empty()
		&& cmdinfo.startupConfig.commonConfigRoot.Empty()))
	{
	  FATAL_MIKTEX_ERROR
	    (T_("SetupWizardApplication::InitInstance"),
	     T_("You cannot specify --private along with \
--common-data and/or --common-config."),
	     0);
	}
      SetupGlobalVars (cmdinfo);

      // open the log file
      OpenLog ();

      INT_PTR dlgRet;
      
      // run the wizard
      {
	SetupWizard dlg (pManager.Get());
	m_pMainWnd = &dlg;
	dlgRet = dlg.DoModal ();
      }
      
      // clean up
      CloseLog (dlgRet == IDCANCEL);
      traceStream.reset ();
      pManager->UnloadDatabase ();
      pManager.Release ();
      pSession->UnloadFilenameDatabase ();
      if (selfExtractor)
	{
	  sfxDir.Leave ();
	}
      scratchRoot.Delete ();
      pSession.Reset ();
    }

  catch (const MiKTeXException & e)
    {
      ReportError (e);
    }
  catch (const exception & e)
    {
      ReportError (e);
    }

  CoUninitialize ();

  return (FALSE);
}

/* _________________________________________________________________________

   Reboot
   _________________________________________________________________________ */

#if 0
bool
Reboot ()
{
  if (IsWindowsNT())
    {
      HANDLE hToken;
      if (! OpenProcessToken(GetCurrentProcess(),
			     TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
			     &hToken))
	{
	  return (false);
	}
      TOKEN_PRIVILEGES tkp;
      LookupPrivilegeValue (0, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
      tkp.PrivilegeCount = 1;
      tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
      AdjustTokenPrivileges (hToken, FALSE, &tkp, 0, 0, 0);
      if (GetLastError() != ERROR_SUCCESS)
	{
	  return (false);
	}
    }
  if (! ExitWindowsEx(EWX_REBOOT, 0))
    {
      return (false);
    }
  return (true);
}
#endif

/* _________________________________________________________________________

   ComparePaths
   _________________________________________________________________________ */

int
ComparePaths (/*[in]*/ const PathName &	path1,
	      /*[in]*/ const PathName &	path2,
	      /*[in]*/ bool		shortify)
{
  PathName shortPath1;
  PathName shortPath2;

  if (shortify
      && (GetShortPathName(path1.Get(),
			   shortPath1.GetBuffer(),
			   static_cast<DWORD>(shortPath1.GetCapacity()))
	  > 0)
      && (GetShortPathName(path2.Get(),
			   shortPath2.GetBuffer(),
			   static_cast<DWORD>(shortPath2.GetCapacity()))
	  > 0))
    {
      return (PathName::Compare(shortPath1, shortPath2));
    }
  else
    {
      return (PathName::Compare(path1, path2));
    }
}

/* _________________________________________________________________________

   ContainsBinDir
   _________________________________________________________________________ */

bool
ContainsBinDir (/*[in]*/ const MIKTEXCHAR *	lpszPath)
{
  PathName pathBinDir;
  if (theApp.setupTask == SetupTask::PrepareMiKTeXDirect)
    {
      pathBinDir.Set (theApp.MiKTeXDirectTeXMFRoot, MIKTEX_PATH_BIN_DIR);
    }
  else
    {
      pathBinDir.Set (theApp.startupConfig.installRoot, MIKTEX_PATH_BIN_DIR);
    }
  CSVList bindir (lpszPath, T_(';'));
  for (; bindir.GetCurrent() != 0; ++ bindir)
    {
      size_t l = StrLen(bindir.GetCurrent());
      if (l == 0)
	{
	  continue;
	}
      PathName pathBinDir2;
      if (bindir.GetCurrent()[0] == T_('"')
	  && l > 2
	  && bindir.GetCurrent()[l - 1] == T_('"'))
	{
	  pathBinDir2 = bindir.GetCurrent() + 1;
	  pathBinDir2[l - 2] = 0;
	}
      else
	{
	  pathBinDir2 = bindir.GetCurrent();
	}
      if (ComparePaths(pathBinDir, pathBinDir2, true) == 0)
	{
	  return (true);
	}
    }
  return (false);
}

/* _________________________________________________________________________

   IsPathRegistered
   _________________________________________________________________________ */

bool
IsPathRegistered (/*[in]*/ HKEY			hkeyRoot,
		  /*[in]*/ const MIKTEXCHAR *	lpszKey)
{
  HKEY hkey;
  LONG result = RegOpenKeyEx(hkeyRoot, lpszKey, 0, KEY_QUERY_VALUE, &hkey);
  bool found = false;
  if (result == ERROR_SUCCESS)
    {
      CharBuffer<char> value (1024 * 32);
      DWORD type;
      DWORD valueSize = static_cast<DWORD>(value.GetCapacity());
      result =
	RegQueryValueEx(hkey,
			T_("Path"),
			0,
			&type,
			reinterpret_cast<LPBYTE>(value.GetBuffer()),
			&valueSize);
      RegCloseKey (hkey);
      if (result == ERROR_SUCCESS)
	{
	  found = ContainsBinDir(value.Get());
	}
    }
  return (found);
}

/* _________________________________________________________________________

   IsPathRegistered
   _________________________________________________________________________ */

bool
IsPathRegistered ()
{
  if (IsWindowsNT())
    {
      if (IsPathRegistered(HKEY_LOCAL_MACHINE,
			   (REGSTR_PATH_CURRENTCONTROLSET
			    T_("\\Control\\Session Manager\\Environment"))))
	{
	  return (true);
	}
      if (IsPathRegistered(HKEY_CURRENT_USER, T_("Environment")))
			   
	{
	  return (true);
	}
    }
  else
    {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
      tstring value;
      if (Utils::GetEnvironmentString(T_("PATH"), value))
	{
	  return (ContainsBinDir(value.c_str()));
	}
#else
      UNSUPPORTED_PLATFORM ();
#endif
    }
  return (false);
}

/* _________________________________________________________________________

   GetLogFileName
   _________________________________________________________________________ */

PathName
GetLogFileName ()
{
  PathName ret;
  if (theApp.dryRun || theApp.setupTask == SetupTask::PrepareMiKTeXDirect)
    {
      ret.SetToTempDirectory ();
    }
  else
    {
      ret = theApp.startupConfig.installRoot;
      ret += MIKTEX_PATH_MIKTEX_CONFIG_DIR;
    }
  Directory::Create (ret);
  ret += MIKTEX_UNINSTALL_LOG;
  return (ret);
}

/* _________________________________________________________________________

   ULogOpen
   _________________________________________________________________________ */

enum Section { None, Files, HKLM, HKCU };

namespace {
  Section section = None;
}

void
ULogOpen ()
{
  PathName uninstLog (GetLogFileName());
  FileMode mode =
    (File::Exists(uninstLog) ? FileMode::Append : FileMode::Create);
  theApp.uninstStream.Attach (File::Open(uninstLog, mode, FileAccess::Write));;
  section = None;
}

/* _________________________________________________________________________

   AddUninstallerRegValue
   _________________________________________________________________________ */

#define UNINST_DISPLAY_NAME \
  MIKTEX_PRODUCTNAME_STR T_(" ") MIKTEX_VERSION_STR

#define UNINST_DISPLAY_NAME_MIKTEXDIRECT \
  "MiKTeXDirect" T_(" ") MIKTEX_VERSION_STR

#define UNINST_REG_PATH							\
    (theApp.setupTask == SetupTask::PrepareMiKTeXDirect			\
     ? REGSTR_PATH_UNINSTALL T_("\\") UNINST_DISPLAY_NAME_MIKTEXDIRECT	\
     : REGSTR_PATH_UNINSTALL T_("\\") UNINST_DISPLAY_NAME)

#define UNINST_HKEY_ROOT			\
  (theApp.commonUserSetup			\
   ? HKEY_LOCAL_MACHINE				\
   : HKEY_CURRENT_USER)

void
AddUninstallerRegValue (/*[in]*/ HKEY			hkey,
			/*[in]*/ const MIKTEXCHAR *	lpszValueName,
			/*[in]*/ const MIKTEXCHAR *	lpszValue)
{
  LONG result =
    RegSetValueEx
    (hkey,
     lpszValueName,
     0,
     REG_SZ,
     reinterpret_cast<const BYTE *>(lpszValue),
     static_cast<DWORD>(STR_BYT_SIZ(lpszValue)));
  
  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 (T_("RegSetValueEx"), result, 0);
    }
  
  ULogAddRegValue (UNINST_HKEY_ROOT, UNINST_REG_PATH, lpszValueName);
}

void
AddUninstallerRegValue (/*[in]*/ HKEY			hkey,
			/*[in]*/ const MIKTEXCHAR *	lpszValueName,
			/*[in]*/ DWORD			value)
{
  LONG result =
    RegSetValueEx
    (hkey,
     lpszValueName,
     0,
     REG_DWORD,
     reinterpret_cast<const BYTE *>(&value),
     static_cast<DWORD>(sizeof(value)));
  
  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 (T_("RegSetValueEx"), result, 0);
    }
  
  ULogAddRegValue (UNINST_HKEY_ROOT, UNINST_REG_PATH, lpszValueName);
}

/* _________________________________________________________________________

   RegisterUninstaller
   _________________________________________________________________________ */

#define UNINST_HELP_LINK T_("http://miktex.org/Support.aspx")
#define UNINST_PUBLISHER MIKTEX_COMPANYNAME_STR
#define UNINST_DISPLAY_VERSION MIKTEX_VERSION_STR
#define UNINST_DISPLAY_STRING				\
  (theApp.setupTask == SetupTask::PrepareMiKTeXDirect	\
   ? UNINST_DISPLAY_NAME_MIKTEXDIRECT			\
   : UNINST_DISPLAY_NAME)
#define UNINST_ABOUT_URL T_("http://miktex.org/About.aspx")
#define UNINST_UPDATE_URL T_("http://miktex.org")
#define UNINST_COMMENT T_("Uninstall MiKTeX")
#define UNINST_README MIKTEX_URL_WWW_KNOWN_ISSUES

void
RegisterUninstaller ()
{
  // make uninstall command line
  PathName pathCopyStart (theApp.startupConfig.installRoot,
			  MIKTEX_PATH_COPYSTART_ADMIN_EXE);
  tstring commandLine;
  commandLine += T_('"');
  commandLine += pathCopyStart.Get();
  commandLine += T_("\" \"");
  PathName pathUninstallDat (theApp.startupConfig.installRoot,
			     MIKTEX_PATH_UNINSTALL_DAT);
  commandLine += pathUninstallDat.Get();
  commandLine += T_('"');

  // make icon path
  PathName iconPath (theApp.startupConfig.installRoot);
  iconPath += MIKTEX_PATH_BIN_DIR;
  iconPath += MIKTEX_MO_EXE;
  iconPath.Append (T_(",0"), false);

  // create registry key
  HKEY hkey;
  DWORD disp;
  LONG result =
    RegCreateKeyEx(UNINST_HKEY_ROOT,
		   UNINST_REG_PATH,
		   0,
		   0,
		   REG_OPTION_NON_VOLATILE,
		   KEY_ALL_ACCESS,
		   0,
		   &hkey,
		   &disp);
  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 (T_("RegCreateKeyEx"), result, 0);
    }
  AutoHKEY autoHKEY (hkey);
  
  // set values
  PathName installRoot (theApp.startupConfig.installRoot);
  AddUninstallerRegValue (hkey, T_("Comment"), UNINST_COMMENT);
  AddUninstallerRegValue (hkey, T_("DisplayIcon"), iconPath.Get());
  AddUninstallerRegValue (hkey, T_("DisplayName"), UNINST_DISPLAY_STRING); 
  AddUninstallerRegValue (hkey, T_("DisplayVersion"), UNINST_DISPLAY_VERSION);
  AddUninstallerRegValue (hkey, T_("HelpLink"), UNINST_HELP_LINK);
  AddUninstallerRegValue (hkey, T_("InstallLocation"), installRoot.Get());
  AddUninstallerRegValue (hkey, T_("NoModify"), 1);
  AddUninstallerRegValue (hkey, T_("NoRepair"), 1);
  AddUninstallerRegValue (hkey, T_("Publisher"), UNINST_PUBLISHER);
  AddUninstallerRegValue (hkey, T_("Readme"), UNINST_README);
  AddUninstallerRegValue (hkey, T_("UninstallString"), commandLine.c_str());
  AddUninstallerRegValue (hkey, T_("UrlInfoAbout"), UNINST_ABOUT_URL);
  AddUninstallerRegValue (hkey, T_("UrlUpdateInfo"), UNINST_UPDATE_URL);
}

/* _________________________________________________________________________

   ULogClose
   _________________________________________________________________________ */

void
ULogClose (/*[in]*/ bool finalize)
{
  if (! theApp.uninstStream.IsOpen())
    {
      return;
    }
  
  try
    {
      if (finalize)
	{
	  ULogAddFile (GetLogFileName());
	  RegisterUninstaller ();
	}
    }
  catch (const exception &)
    {
      theApp.uninstStream.Close ();
      throw;
    }

  theApp.uninstStream.Close ();
}

/* _________________________________________________________________________

   ULogAddFile
   _________________________________________________________________________ */

void
ULogAddFile (/*[in]*/ const PathName & path)
{
  if (! theApp.uninstStream.IsOpen())
    {
      return;
    }
  if (section != Files)
    {
      theApp.uninstStream.WriteLine (T_("[files]"));
      section = Files;
    }
  PathName absolutePath (path);
  absolutePath.MakeAbsolute ();
  absolutePath.ToDos ();
  theApp.uninstStream.WriteLine (absolutePath.Get());
}

/* _________________________________________________________________________

   ULogAddRegValue
   _________________________________________________________________________ */

void
ULogAddRegValue (/*[in]*/ HKEY		hkey,
		 /*[in]*/ LPCSTR	lpszSubKey,
		 /*[in]*/ LPCSTR	lpszValueName)
{
  if (! theApp.uninstStream.IsOpen())
    {
      return;
    }
  if (hkey == HKEY_LOCAL_MACHINE && section != HKLM)
    {
      theApp.uninstStream.WriteLine (T_("[hklm]"));
      section = HKLM;
    }
  else if (hkey == HKEY_CURRENT_USER && section != HKCU)
    {
      theApp.uninstStream.WriteLine (T_("[hkcu]"));
      section = HKCU;
    }
  theApp.uninstStream.WriteFormattedLine
    (T_("%s;%s"), lpszSubKey, lpszValueName);
}

/* _________________________________________________________________________

   RegisterPathNT
   _________________________________________________________________________ */

void
RegisterPathNT ()
{
  PathName pathBinDir;

  if (theApp.setupTask == SetupTask::PrepareMiKTeXDirect)
    {
      pathBinDir.Set (theApp.MiKTeXDirectTeXMFRoot, MIKTEX_PATH_BIN_DIR);
    }
  else
    {
      pathBinDir.Set (theApp.startupConfig.installRoot, MIKTEX_PATH_BIN_DIR);
    }

  Log (T_("Registering bin dir: %s\n"), Q_(pathBinDir));

#define REGSTR_KEY_ENVIRONMENT_COMMON \
   REGSTR_PATH_CURRENTCONTROLSET T_("\\Control\\Session Manager\\Environment")
#define REGSTR_KEY_ENVIRONMENT_USER T_("Environment")

  HKEY hkey;

  LONG result =
    RegOpenKeyEx((theApp.commonUserSetup
		  ? HKEY_LOCAL_MACHINE
		  : HKEY_CURRENT_USER),
		 (theApp.commonUserSetup
		  ? REGSTR_KEY_ENVIRONMENT_COMMON
		  : REGSTR_KEY_ENVIRONMENT_USER),
		 0,
		 KEY_QUERY_VALUE | KEY_SET_VALUE,
		 &hkey);

  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 (T_("RegOpenKey"), result, 0);
    }

  AutoHKEY autoHKEY (hkey);

  DWORD type;
  CharBuffer<char> value (32 * 1024);
  DWORD valueSize = static_cast<DWORD>(value.GetCapacity());

  result =
    RegQueryValueEx(hkey,
		    T_("Path"),
		    0,
		    &type,
		    reinterpret_cast<LPBYTE>(value.GetBuffer()),
		    &valueSize);
 
  bool havePath = (result == ERROR_SUCCESS);

  if (! havePath)
    {
      if (result != ERROR_FILE_NOT_FOUND)
	{
	  FATAL_WINDOWS_ERROR_2 (T_("RegQueryValueEx"), result, 0);
	}
    }

  tstring newPath (pathBinDir.Get());

  if (havePath)
    {
      newPath += T_(';');
      newPath += value.Get();
    }

  result =
    RegSetValueEx(hkey,
		  T_("Path"),
		  0,
		  (havePath ? type : REG_SZ),
		  reinterpret_cast<const BYTE *>(newPath.c_str()),
		  static_cast<DWORD>(STR_BYT_SIZ(newPath.c_str())));

  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 (T_("RegSetValueEx"), result, 0);
    }

  DWORD sendMessageResult;

  if (SendMessageTimeout(HWND_BROADCAST,
			 WM_SETTINGCHANGE,
			 0,
			 reinterpret_cast<LPARAM>(T_("Environment")),
			 SMTO_ABORTIFHUNG,
			 5000,
			 &sendMessageResult)
      == 0)
    {
      CHECK_WINDOWS_ERROR (T_("SendMessageTimeout"), 0);
    }
}

/* _________________________________________________________________________

   RegisterPath95
   _________________________________________________________________________ */

#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
void
RegisterPath95 ()
{
  PathName pathBinDir;
  if (theApp.setupTask == SetupTask::PrepareMiKTeXDirect)
    {
      pathBinDir.Set (theApp.MiKTeXDirectTeXMFRoot, MIKTEX_PATH_BIN_DIR);
    }
  else
    {
      pathBinDir.Set (theApp.startupConfig.installRoot, MIKTEX_PATH_BIN_DIR);
    }
  Utils::RemoveBlanksFromPathName (pathBinDir);
  StreamWriter writer (File::Open(T_("c:\\autoexec.bat"), // <fixme/>
				  FileMode::Append,
				  FileAccess::Write));
  writer.WriteFormattedLine (T_("set PATH=%s;%%PATH%%"), pathBinDir.Get());
  writer.Close ();
  Log (T_("set PATH=%s;%%PATH%%\n"), pathBinDir.Get());
  theApp.mustReboot = true;
}
#endif

/* _________________________________________________________________________

   CreateProgramFolder
   _________________________________________________________________________ */

PathName
CreateProgramFolder ()
{
  int cidl =
    (theApp.commonUserSetup && IsWindowsNT()
     ? CSIDL_COMMON_PROGRAMS
     : CSIDL_PROGRAMS);
  PathName path = Utils::GetFolderPath(cidl, cidl, true);
  path += theApp.folderName;
  Directory::Create (path);
  return (path);
}

/* _________________________________________________________________________

   RegisterMiKTeXFileTypes
   _________________________________________________________________________ */

void
RegisterMiKTeXFileTypes ()
{
  if (! theApp.dryRun
      && (! IsWindowsNT()
	  || SessionWrapper(true)->RunningAsAdministrator()
	  || SessionWrapper(true)->RunningAsPowerUser()))
    {
      PathName yap (theApp.startupConfig.installRoot);
      yap += MIKTEX_PATH_BIN_DIR;
      yap += MIKTEX_YAP_EXE;
      Process::Run (yap.Get(), T_("--register"));
    }
}

/* _________________________________________________________________________

   Expand
   _________________________________________________________________________ */

CString &
Expand (/*[in]*/ const MIKTEXCHAR *	lpszSource,
	/*[out]*/ CString &		dest)
{
  dest = lpszSource;
  int pos;
  while ((pos = dest.Find(T_("%MIKTEX_INSTALL%"))) >= 0)
    {
      dest.Delete (pos, 16);
      dest.Insert (pos, theApp.startupConfig.installRoot.Get());
    }
  return (dest);
}

/* _________________________________________________________________________

   CreateInternetShortcut
   _________________________________________________________________________ */

void
CreateInternetShortcut (/*[in]*/ const PathName &	path,
			/*[in]*/ const MIKTEXCHAR *	lpszUrl)
{
  _COM_SMARTPTR_TYPEDEF (IUniformResourceLocator,
			 IID_IUniformResourceLocator);

  IUniformResourceLocatorPtr pURL;

  HRESULT hr =
    CoCreateInstance(CLSID_InternetShortcut,
		     0, 
		     CLSCTX_INPROC_SERVER,
		     IID_IUniformResourceLocator,
		     reinterpret_cast<void **>(&pURL));

  if (FAILED(hr))
    {
      Log (T_("IUniformResourceLocator could not be created (%08x)\n"), hr);
      UNEXPECTED_CONDITION (T_("CreateInternetShortcut"));
    }

  hr = pURL->SetURL(lpszUrl, 0);

  if (FAILED(hr))
    {
      Log (T_("IUniformResourceLocator::SetURL() failed (%08x)\n"), hr);
      UNEXPECTED_CONDITION (T_("CreateInternetShortcut"));
    }

  IPersistFilePtr pPF;

  hr = pURL->QueryInterface(IID_IPersistFile, reinterpret_cast<void **>(&pPF));

  if (FAILED(hr))
    {
      Log (T_("IPersistFile could not be created (%08x)\n"), hr);
      UNEXPECTED_CONDITION (T_("CreateInternetShortcut"));
    }

#if defined(MIKTEX_UNICODE)
  hr = pPF->Save(path.Get(), TRUE);
#else
  WCHAR wszPath[BufferSizes::MaxPath];
  if (MultiByteToWideChar(CP_ACP,
			  0,
			  path.Get(),
			  -1,
			  wszPath,
			  BufferSizes::MaxPath)
      == 0)
    {
      FATAL_WINDOWS_ERROR (T_("MultiByteToWideChar"), 0);
    }
  hr = pPF->Save(wszPath, TRUE);
#endif

  if (FAILED(hr))
    {
      Log (T_("IPersistFile::Save() failed (%08x)\n"), hr);
      UNEXPECTED_CONDITION (T_("CreateInternetShortcut"));
    }
}

/* _________________________________________________________________________

   CreateShellLink
   _________________________________________________________________________ */

void
CreateShellLink (/*[in]*/ const PathName &		pathFolder,
		 /*[in]*/ const ShellLinkData &		ld)
{
  PathName pathLink;

  if (ld.subFolderID > 0)
    {
      CString subFolder;
      if (! subFolder.LoadString(ld.subFolderID))
	{
	  UNEXPECTED_CONDITION (T_("CreateShellLink"));
	}
      PathName pathSubFolder (pathFolder,
			      static_cast<const MIKTEXCHAR *>(subFolder));
      Directory::Create (pathSubFolder);
      pathLink = pathSubFolder;
    }
  else
    {
      pathLink = pathFolder;
    }
  
  CString strItemName;

  if (! strItemName.LoadString(ld.nameID))
    {
      UNEXPECTED_CONDITION (T_("CreateShellLink"));
    }

  pathLink += strItemName;
  pathLink.SetExtension (ld.isUrl ? T_(".url") : T_(".lnk"));

  if (File::Exists(pathLink))
    {
      Log (T_("removing %s...\n"), Q_(pathLink));
      if (! theApp.dryRun)
	{
	  File::Delete (pathLink);
	}
    }
  
  if (ld.lpszPathName == 0)
    {
      return;
    }
  
  if (ld.isUrl)
    {
      Log (T_("creating internet shortcut %s...\n"), Q_(pathLink));
    }
  else
    {
      Log (T_("creating shell link %s...\n"), Q_(pathLink));
    }

  if (theApp.dryRun)
    {
      return;
    }

  if (ld.isUrl)
    {
      CreateInternetShortcut (pathLink, ld.lpszPathName);
    }
  else
    {
      _COM_SMARTPTR_TYPEDEF (IShellLink, IID_IShellLink);

      IShellLinkPtr psl;

      HRESULT hr =
	CoCreateInstance(CLSID_ShellLink,
			 0,
			 CLSCTX_INPROC_SERVER,
			 IID_IShellLink,
			 reinterpret_cast<void **>(&psl));

      if (FAILED(hr))
	{
	  Log (T_("IShellLink could not be created (%08x)\n"), hr);
	  UNEXPECTED_CONDITION (T_("CreateShellLink"));
	}
      
      CString str;
      
      hr = psl->SetPath(Expand(ld.lpszPathName, str));

      if (FAILED(hr))
	{
	  Log (T_("IShellLink::SetPath() failed (%08x)\n"), hr);
	  UNEXPECTED_CONDITION (T_("CreateShellLink"));
	}
      
      if ((ld.flags & LD_USEARGS) != 0)
	{
	  hr = psl->SetArguments(Expand(ld.lpszArgs, str));
	  if (FAILED(hr))
	    {
	      Log (T_("IShellLink::SetArguments() failed (%08x)\n"), hr);
	      UNEXPECTED_CONDITION (T_("CreateShellLink"));
	    }
	}
      
      if ((ld.flags & LD_USEDESC) != 0)
	{
	  hr = psl->SetDescription(ld.lpszDescription);
	  if (FAILED(hr))
	    {
	      Log (T_("IShellLink::SetDescription() failed (%08x)\n"), hr);
	      UNEXPECTED_CONDITION (T_("CreateShellLink"));
	    }
	}
      
      if ((ld.flags & LD_USEICON) != 0)
	{
	  hr =
	    psl->SetIconLocation(Expand(ld.lpszIconPath, str), ld.iconIndex);
	  if (FAILED(hr))
	    {
	      Log (T_("IShellLink::SetIconLocation() failed (%08x)\n"), hr);
	      UNEXPECTED_CONDITION (T_("CreateShellLink"));
	    }
	}
      
      if ((ld.flags & LD_USEWORKDIR) != 0)
	{
	  hr = psl->SetWorkingDirectory(Expand(ld.lpszWorkingDir, str));
	  if (FAILED(hr))
	    {
	      Log (T_("IShellLink::SetWorkingDirectory() failed (%08x)\n"),
		   hr);
	      UNEXPECTED_CONDITION (T_("CreateShellLink"));
	    }
	}

      if ((ld.flags & LD_USESHOWCMD) != 0)
	{
	  hr = psl->SetShowCmd(ld.showCmd);
	  if (FAILED(hr))
	    {
	      Log (T_("IShellLink::SetShowCmd() failed (%08x)\n"), hr);
	      UNEXPECTED_CONDITION (T_("CreateShellLink"));
	    }
	}
      
      if ((ld.flags & LD_USEHOTKEY) != 0)
	{
	  hr = psl->SetHotkey (ld.hotKey);
	  if (FAILED(hr))
	    {
	      Log (T_("IShellLink::SetHotkey() failed (%08x)\n"), hr);
	      UNEXPECTED_CONDITION (T_("CreateShellLink"));
	    }
	}
      
      IPersistFilePtr ppf;

      hr =
	psl->QueryInterface(IID_IPersistFile, reinterpret_cast<void **>(&ppf));

      if (FAILED(hr))
	{
	  Log (T_("IPersistFile could not be created (%08x)\n"), hr);
	  UNEXPECTED_CONDITION (T_("CreateShellLink"));
	}

#if defined(MIKTEX_UNICODE)
      hr = ppf->Save(pathLink.Get(), TRUE);
#else
      WCHAR wszPath[_MAX_PATH] = { 0 };
      if (MultiByteToWideChar(CP_ACP,
			      0,
			      pathLink.Get(),
			      static_cast<int>(pathLink.GetLength()),
			      wszPath,
			      static_cast<int>(pathLink.GetCapacity()))
	  == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("MultiByteToWideChar"), 0);
	}
      hr = ppf->Save(wszPath, TRUE);
#endif

      if (FAILED(hr))
	{
	  Log (T_("IShellLink::Save() failed (%08x)\n"), hr);
	  UNEXPECTED_CONDITION (T_("CreateShellLink"));
	}
    }

  ULogAddFile (pathLink.Get());
}

/* _________________________________________________________________________

   CreateProgramIcons
   _________________________________________________________________________ */

void
CreateProgramIcons ()
{
  PathName path = CreateProgramFolder();
  for (size_t i = 0; i < nShellLinks; ++ i)
    {
      CreateShellLink (path, shellLinks[i]);
    }
}

/* _________________________________________________________________________

   LogV
   _________________________________________________________________________ */

void
LogV (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
      /*[in]*/ va_list			argList)
{
  CSingleLock singleLock (&theApp.criticalSectionMonitorLogStream, TRUE);
  tstring formatted = Utils::FormatString(lpszFormat, argList);
  static tstring currentLine;
  for (const MIKTEXCHAR * lpsz = formatted.c_str();
       *lpsz != 0;
       ++ lpsz)
    {
      if (lpsz[0] == T_('\n')
	  || (lpsz[0] == T_('\r') && lpsz[1] == T_('\n')))
	{
	  theApp.traceStream->WriteFormattedLine (T_("setup"),
						  T_("%s"),
						  currentLine.c_str());
	  if (theApp.logStream.IsOpen())
	    {
	      theApp.logStream.WriteLine (currentLine);
	    }
	  currentLine = T_("");
	  if (lpsz[0] == T_('\r'))
	    {
	      ++ lpsz;
	    }
	}
      else
	{
	  currentLine += *lpsz;
	}
    }
}

/* _________________________________________________________________________

   Log
   _________________________________________________________________________ */

void
Log (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
     /*[in]*/				...)
{
  va_list argList;
  va_start (argList, lpszFormat);
  LogV (lpszFormat, argList);
  va_end (argList);
}

/* _________________________________________________________________________

   LogHeader
   _________________________________________________________________________ */

void
LogHeader ()
{
  CString banner;
  if (! banner.LoadString(IDS_SETUPWIZ))
    {
      UNEXPECTED_CONDITION (T_("LogHeader"));
    }
  Log (T_("%s %s Report\n\n"),
       static_cast<const MIKTEXCHAR *>(banner),
       VER_FILEVERSION_STR);
  CTime t = CTime::GetCurrentTime();
  Log (T_("Date: %s\n"), t.Format(T_("%A, %B %d, %Y")));
  Log (T_("Time: %s\n"), t.Format(T_("%H:%M:%S")));
  Log (T_("OS version: %s\n"), Utils::GetOSVersionString().c_str());
  if (IsWindowsNT())
    {
      Log (T_("SystemAdmin: %s\n"),
	   (SessionWrapper(true)->RunningAsAdministrator()
	    ? T_("yes")
	    : T_("false")));
      Log (T_("PowerUser: %s\n"),
	   (SessionWrapper(true)->RunningAsPowerUser()
	    ? T_("yes")
	    : T_("false")));
    }
  if (theApp.setupTask != SetupTask::Download)
    {
      Log (T_("SharedSetup: %s\n"),
	   (theApp.commonUserSetup ? T_("yes") : T_("false")));
    }
  Log (T_("Setup path: %s\n"), theApp.setupPath.Get());
  if (theApp.setupTask != SetupTask::Download)
    {
      Log (T_("Roots: %s\n"),
	   (theApp.noAddTEXMFDirs || theApp.startupConfig.roots.empty()
	    ? T_("<none specified>")
	    : theApp.startupConfig.roots.c_str()));
      Log (T_("UserData: %s\n"),
	   (theApp.startupConfig.userDataRoot.Empty()
	    ? T_("<none specified>")
	    : theApp.startupConfig.userDataRoot.Get()));
      Log (T_("UserConfig: %s\n"),
	   (theApp.startupConfig.userConfigRoot.Empty()
	    ? T_("<none specified>")
	    : theApp.startupConfig.userConfigRoot.Get()));
      Log (T_("CommonData: %s\n"),
	   (theApp.startupConfig.commonDataRoot.Empty()
	    ? T_("<none specified>")
	    : theApp.startupConfig.commonDataRoot.Get()));
      Log (T_("CommonConfig: %s\n"),
	   (theApp.startupConfig.commonConfigRoot.Empty()
	    ? T_("<none specified>")
	    : theApp.startupConfig.commonConfigRoot.Get()));
      Log (T_("\nInstallation: %s\n"), theApp.startupConfig.installRoot.Get());
    }
}

/* _________________________________________________________________________

   GetFileVersion
   _________________________________________________________________________ */

VersionNumber
GetFileVersion (/*[in]*/ const PathName &	path)
{
  DWORD dwHandle;
  DWORD cchver = GetFileVersionInfoSize(path.Get(), &dwHandle);
  if (cchver == 0)
    {
      FATAL_WINDOWS_ERROR (T_("GetFileVersionInfoSize"), path.Get());
    }
  CharBuffer<char> buf (cchver);
  if (! GetFileVersionInfo(path.Get(), dwHandle, cchver, buf.GetBuffer()))
    {
      FATAL_WINDOWS_ERROR (T_("GetFileVersionInfo"), path.Get());
    }
  UINT uLen;
  void * pVer;
  if (! VerQueryValue(buf.GetBuffer(),
		      T_("\\"),
		      &pVer,
		      &uLen))
    {
      FATAL_WINDOWS_ERROR (T_("VerQueryValue"), path.Get());
    }
  return
    (VersionNumber
     (reinterpret_cast<VS_FIXEDFILEINFO*>(pVer)->dwFileVersionMS,
      reinterpret_cast<VS_FIXEDFILEINFO*>(pVer)->dwFileVersionLS));
}

/* _________________________________________________________________________

   DDV_Path
   _________________________________________________________________________ */

void
DDV_Path (/*[in]*/ CDataExchange *	pDX,
	  /*[in]*/ const CString &	str)
{
  if (! pDX->m_bSaveAndValidate)
    {
      return;
    }
  CString str2;
  if (isalpha(str[0]) && str[1] == T_(':') && IsDirectoryDelimiter(str[2]))
    {
      CString driveRoot = str.Left(3);
      if (! Directory::Exists(PathName(driveRoot)))
	{
	  CString message;
	  message.Format (T_("The specified path is invalid because the \
root directory %s does not exist."),
			  driveRoot.GetString());
	  AfxMessageBox (message, MB_ICONEXCLAMATION);
	  message.Empty ();
	  pDX->Fail ();
	}
      str2 = str.GetString() + 3;
    }
  else
    {
      PathName uncRoot;
      if (! Utils::GetUncRootFromPath(str, uncRoot))
	{
	  CString message;
	  message.Format (T_("The specified path is invalid because it is not \
fully qualified."));
	  AfxMessageBox (message, MB_ICONEXCLAMATION);
	  message.Empty ();
	  pDX->Fail ();
	}
      if (! Directory::Exists(uncRoot))
	{
	  CString message;
	  message.Format (T_("The specified path is invalid because the UNC \
root directory %s does not exist."),
			  uncRoot.Get());
	  AfxMessageBox (message, MB_ICONEXCLAMATION);
	  message.Empty ();
	  pDX->Fail ();
	}
      str2 = str;
    }
  int i = str2.FindOneOf(T_(":*?\"<>|;="));
  if (i >= 0)
    {
      CString message;
      message.Format (T_("The specified path is invalid because it contains \
an invalid character (%c)."),
		      str2[i]);
      AfxMessageBox (message, MB_ICONEXCLAMATION);
      message.Empty ();
      pDX->Fail ();
    }
}

/* _________________________________________________________________________

   ReportError
   _________________________________________________________________________ */

void
ReportError (/*[in]*/ const MiKTeXException & e)
{
  try
    {
      tstring str;
      str =
	T_("The operation could not be completed for the following reason: ");
      str += T_("\n\n");
      str += e.what();
      if (! e.GetInfo().empty())
	{
	  str += T_("\n\n");
	  str += T_("Details: ");
	  str += e.GetInfo();
	}
      AfxMessageBox (str.c_str(), MB_OK | MB_ICONSTOP);
      Log (T_("\nAn error occurred:\n"));
      Log (T_("  source file: %s\n"), e.GetSourceFile().c_str());
      Log (T_("  source line: %d\n"), e.GetSourceLine());
      Log (T_("  message: %s\n"), e.what());
      Log (T_("  info: %s\n"), e.GetInfo().c_str());
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   ReportError
   _________________________________________________________________________ */

void
ReportError (/*[in]*/ const exception & e)
{
  try
    {
      tstring str;
      str =
	T_("The operation could not be completed for the following reason: ");
      str += T_("\n\n");
      str += e.what();
      Log (T_("\n%s\n"), str.c_str());
      AfxMessageBox (str.c_str(), MB_OK | MB_ICONSTOP);
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   SplitUrl
   _________________________________________________________________________ */

void
SplitUrl (/*[in]*/ const tstring &	url,
	  /*[out]*/ tstring &		protocol,
	  /*[out]*/ tstring &		host)
{
  MIKTEXCHAR szProtocol[200];
  MIKTEXCHAR szHost[200];
  URL_COMPONENTS url_comp = { 0 };
  url_comp.dwStructSize = sizeof(url_comp);
  url_comp.lpszScheme = szProtocol;
  url_comp.dwSchemeLength = 200;
  url_comp.lpszHostName = szHost;
  url_comp.dwHostNameLength = 200;
  if (! InternetCrackUrl(url.c_str(), 0, 0, &url_comp))
    {
      FATAL_WINDOWS_ERROR (T_("InternetCrackUrl"), 0);
    }
  protocol = szProtocol;
  host = szHost;
}
