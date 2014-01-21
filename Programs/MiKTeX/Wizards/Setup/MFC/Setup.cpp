/* Setup.cpp:

   Copyright (C) 1999-2014 Christian Schenk

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

// this runs the wizard
SetupWizardApplication theApp;

/* _________________________________________________________________________

   SetupCommandLineInfo
   _________________________________________________________________________ */

class SetupCommandLineInfo : public CCommandLineInfo
{
public:
  SetupCommandLineInfo() :
    optAllowUnattendedReboot(false),
    optDryRun(false),
#if ENABLE_ADDTEXMF
    optNoAddTEXMFDirs(false),
#endif
    optNoRegistry(false),
    optPrivate(false),
    optPortable(false),
    optShared(false),
    optUnattended(false),
    packageLevel(PackageLevel::None),
#if FEATURE_1874934
    installOnTheFly(TriState::Undetermined),
#endif
    task(SetupTask::None)
  {
  }

public:
  string folderName;

public:
  StartupConfig startupConfig;

public:
  PathName localPackageRepository;

public:
  string remotePackageRepository;

public:
  SetupTask task;

public:
  bool optAllowUnattendedReboot;

public:
  bool optDryRun;

#if ENABLE_ADDTEXMF
public:
  bool optNoAddTEXMFDirs;
#endif

public:
  bool optNoRegistry;

public:
  bool optShared;

public:
  bool optPrivate;

public:
  bool optPortable;

public:
  bool optUnattended;

public:
  PackageLevel packageLevel;

#if FEATURE_1874934
public:
  TriState installOnTheFly;
#endif

#if FEATURE_1874934
public:
  string paperSize;
#endif
};

/* _________________________________________________________________________

   AddArgument
   _________________________________________________________________________ */

void AddArgument (const string & argument, /*[out]*/ int & argc, /*[out]*/ char ** & argv, /*[out]*/ int & argMax)
{
  if (argc == argMax)
  {
    argMax += 10;
    argv = reinterpret_cast<char**>(realloc(argv, argMax * sizeof(argv[0])));
  }
  argv[ argc++ ] = strdup(argument.c_str());
}

/* _________________________________________________________________________

   GetArguments
   _________________________________________________________________________ */

void GetArguments (const char * lpszCommandLine, const char * lpszExeName, /*[out]*/ int & argc, /*[out]*/ char ** & argv)
{
  argc = 0;
  argv = 0;

  int argMax = 0;

  AddArgument(string(lpszExeName), argc, argv, argMax);

  string arg;

  bool copying = false;
  bool inQuotedArg = false;

  const char * lpsz = lpszCommandLine;

  while (*lpsz != 0)
  {
    if (*lpsz == ' ' && ! inQuotedArg)
    {
      if (copying)
      {
	AddArgument(arg, argc, argv, argMax);
	arg = "";
	copying = false;
      }
    }
    else if (*lpsz == '"')
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
    AddArgument(arg, argc, argv, argMax);
  }
}

/* _________________________________________________________________________

   FreeArguments
   _________________________________________________________________________ */

void FreeArguments (int argc, /*[out]*/ char ** & argv)
{
  for (int i = 0; i < argc; ++ i)
  {
    free(argv[i]);
    argv[i] = 0;
  }
  free(argv);
  argv = 0;
}

/* _________________________________________________________________________

   long_options / short_options
   _________________________________________________________________________ */

enum {
  OPT_AAA = 1000,
  OPT_ALLOW_UNATTENDED_REBOOT,
#if FEATURE_1874934
  OPT_AUTO_INSTALL,
#endif
  OPT_COMMON_CONFIG,
  OPT_COMMON_DATA,
  OPT_COMMON_INSTALL,
  OPT_COMMON_ROOTS,
  OPT_DOWNLOAD_ONLY,
  OPT_DRY_RUN,
  OPT_HELP,
  OPT_INSTALL_FROM_LOCAL_REPOSITORY,
  OPT_LOCAL_PACKAGE_REPOSITORY,
  OPT_NO_ADDITIONAL_ROOTS,
  OPT_NO_REGISTRY,
  OPT_PACKAGE_SET,
#if FEATURE_1874934
  OPT_PAPER_SIZE,
#endif
  OPT_PORTABLE,
  OPT_PRIVATE,
  OPT_PROGRAM_FOLDER,
  OPT_REMOTE_PACKAGE_REPOSITORY,
  OPT_SHARED,
  OPT_UNATTENDED,
  OPT_USER_CONFIG,
  OPT_USER_DATA,
  OPT_USER_INSTALL,
  OPT_USER_ROOTS,
};

const struct option long_options[] =
{
  { "allow-unattended-reboot", no_argument, 0, OPT_ALLOW_UNATTENDED_REBOOT },
#if FEATURE_1874934
  { "auto-install", required_argument, 0, OPT_AUTO_INSTALL },
#endif
  { "common-config", required_argument, 0, OPT_COMMON_CONFIG },
  { "common-data", required_argument, 0, OPT_COMMON_DATA },
  { "common-install", required_argument, 0, OPT_COMMON_INSTALL },
  { "common-roots", required_argument, 0, OPT_COMMON_ROOTS },
  { "download-only", no_argument, 0, OPT_DOWNLOAD_ONLY },
  { "dry-run", no_argument, 0, OPT_DRY_RUN },
  { "help", no_argument, 0, OPT_HELP },
  { "install-from-local-repository", no_argument, 0, OPT_INSTALL_FROM_LOCAL_REPOSITORY },
  { "local-package-repository", required_argument, 0, OPT_LOCAL_PACKAGE_REPOSITORY },
  { "no-additional-roots", no_argument, 0, OPT_NO_ADDITIONAL_ROOTS },
  { "no-registry", no_argument, 0, OPT_NO_REGISTRY },
  { "package-set", required_argument, 0, OPT_PACKAGE_SET },
#if FEATURE_1874934
  { "paper-size", required_argument, 0, OPT_PAPER_SIZE },
#endif
  { "portable", no_argument, 0, OPT_PORTABLE },
  { "private", no_argument, 0, OPT_PRIVATE },
  { "program-folder", required_argument, 0, OPT_PROGRAM_FOLDER },
  { "remote-package-repository", required_argument, 0, OPT_REMOTE_PACKAGE_REPOSITORY },
  { "shared", no_argument, 0, OPT_SHARED },
  { "unattended", no_argument, 0, OPT_UNATTENDED },
  { "user-config", required_argument, 0, OPT_USER_CONFIG },
  { "user-data", required_argument, 0, OPT_USER_DATA },
  { "user-install", required_argument, 0, OPT_USER_INSTALL },
  { "user-roots", required_argument, 0, OPT_USER_ROOTS },
  { 0, no_argument, 0, 0 }
};

/* _________________________________________________________________________

   ShowHelpAndExit
   _________________________________________________________________________ */

void ShowHelpAndExit(int retCode = 0)
{
  AfxMessageBox(T_(_T("Usage: setupwiz [OPTIONS]\r\n\
\r\n\
Options:\r\n\r\n\
  --allow-unattended-reboot\r\n"))
#if FEATURE_1874934
		T_(_T("\
  --auto-install=yes\r\n\
  --auto-install=no\r\n\
  --auto-install=ask\r\n"))
#endif
		T_(_T("\
  --common-config=DIR\r\n\
  --common-data=DIR\r\n\
  --common-install=DIR\r\n\
  --common-roots=DIRS\r\n\
  --download-only\r\n\
  --dry-run\r\n\
  --help\r\n\
  --install-from-local-repository\r\n\
  --local-package-repository=DIR\r\n\
  --no-additional-roots\r\n\
  --no-registry\r\n\
  --package-set=SET\r\n"))
#if FEATURE_1874934
		T_(_T("\
  --paper-size=A4\r\n\
  --paper-size=Letter\r\n"))
#endif
		T_(_T("\
  --portable\r\n\
  --private\r\n\
  --program-folder=NAME\r\n\
  --remote-package-repository=URL\r\n\
  --shared\r\n\
  --unattended\r\n\
  --user-config=DIR\r\n\
  --user-data=DIR\r\n\
  --user-install=DIR\r\n\
  --user-roots=DIRS\r\n\
\r\n\
setupwiz reads its arguments from setupwiz.opt, if such a file exists.\r\n\
See the MiKTeX Manual for more information.")));
  exit(retCode);
}

/* _________________________________________________________________________

   CheckStartupConfig
   _________________________________________________________________________ */

void CheckStartupConfig(/*[out]*/ StartupConfig & startupConfig)
{
#if 1
  string commonRoots;
  for (CSVList tok (startupConfig.commonRoots.c_str(), ';'); tok.GetCurrent() != 0; ++ tok)
  {
    PathName path (tok.GetCurrent());
    if (path.Empty())
    {
      continue;
    }
    if (startupConfig.commonConfigRoot == path)
    {
      FATAL_MIKTEX_ERROR("CheckStartupConfig",
	T_("Improper options: --common-roots value collides with --common-config value."),
	path.Get());
    }
    if (startupConfig.commonDataRoot == path)
    {
      FATAL_MIKTEX_ERROR("CheckStartupConfig",
	T_("Improper options: --common-roots value collides with --common-data value."),
	path.Get());
    }
    if (startupConfig.commonInstallRoot == path)
    {
      FATAL_MIKTEX_ERROR("CheckStartupConfig",
	T_("Improper options: --common-roots value collides with --common-install value."),
	path.Get());
    }
    if (startupConfig.userConfigRoot == path)
    {
      FATAL_MIKTEX_ERROR("CheckStartupConfig",
	T_("Improper options: --common-roots value collides with --user-config value."),
	path.Get());
    }
    if (startupConfig.userDataRoot == path)
    {
      FATAL_MIKTEX_ERROR("CheckStartupConfig",
	T_("Improper options: --common-roots value collides with --user-data value."),
	path.Get());
    }
    if (startupConfig.userInstallRoot == path)
    {
      FATAL_MIKTEX_ERROR("CheckStartupConfig",
	T_("Improper options: --common-roots value collides with --user-install value."),
	path.Get());
    }
    if (! commonRoots.empty())
    {
      commonRoots += ';';
    }
    commonRoots += path.Get();
  }
  startupConfig.commonRoots = commonRoots;

  string userRoots;
  for (CSVList tok (startupConfig.userRoots.c_str(), ';'); tok.GetCurrent() != 0; ++ tok)
  {
    PathName path (tok.GetCurrent());
    if (path.Empty())
    {
      continue;
    }
    if (Utils::Contains(startupConfig.commonRoots.c_str(), path.Get(), ";", true))
    {
      FATAL_MIKTEX_ERROR("CheckStartupConfig",
	T_("Improper options: --user-roots value collides with --common-roots value."),
	path.Get());
    }
    if (startupConfig.commonConfigRoot == path)
    {
      FATAL_MIKTEX_ERROR("CheckStartupConfig",
	T_("Improper options: --user-roots value collides with --common-config value."),
	path.Get());
    }
    if (startupConfig.commonDataRoot == path)
    {
      FATAL_MIKTEX_ERROR("CheckStartupConfig",
	T_("Improper options: --user-roots value collides with --common-data value."),
	path.Get());
    }
    if (startupConfig.commonInstallRoot == path)
    {
      FATAL_MIKTEX_ERROR("CheckStartupConfig",
	T_("Improper options: --user-roots value collides with --common-install value."),
	path.Get());
    }
    if (startupConfig.userConfigRoot == path)
    {
      FATAL_MIKTEX_ERROR("CheckStartupConfig",
	T_("Improper options: --user-roots value collides with --user-config value."),
	path.Get());
    }
    if (startupConfig.userDataRoot == path)
    {
      FATAL_MIKTEX_ERROR("CheckStartupConfig",
	T_("Improper options: --user-roots value collides with --user-data value."),
	path.Get());
    }
    if (startupConfig.userInstallRoot == path)
    {
      FATAL_MIKTEX_ERROR("CheckStartupConfig",
	T_("Improper options: --user-roots value collides with --user-install value."),
	path.Get());
    }
    if (! userRoots.empty())
    {
      userRoots += ';';
    }
    userRoots += path.Get();
  }
  startupConfig.userRoots = userRoots;
#endif
}

/* _________________________________________________________________________

   ParseSetupCommandLine
   _________________________________________________________________________ */

void ParseSetupCommandLine(int argc, char ** argv, /*[out]*/ SetupCommandLineInfo & cmdinfo)
{
  int iOptIdx = 0;
  int c;

  cmdinfo.m_nShellCommand = CCommandLineInfo::FileNothing;

  optind = 0;

  while ((c = getopt_long_only(argc, argv, "", long_options, &iOptIdx)) != EOF)
  {
    switch (c)
    {

    case OPT_ALLOW_UNATTENDED_REBOOT:
      cmdinfo.optAllowUnattendedReboot = true;
      break;

#if FEATURE_1874934
    case OPT_AUTO_INSTALL:
      if (StringCompare(optarg, "yes", true) == 0)
      {
	cmdinfo.installOnTheFly = TriState::True;
      }
      else if (StringCompare(optarg, "no", true) == 0)
      {
	cmdinfo.installOnTheFly = TriState::False;
      }
      else if (StringCompare(optarg, "ask", true) == 0)
      {
	cmdinfo.installOnTheFly = TriState::Undetermined;
      }
      else
      {
	FATAL_MIKTEX_ERROR ("ParseSetupCommandLine", T_("Value must be one of: yes, no, ask."), 0);
      }
      break;
#endif

    case OPT_COMMON_CONFIG:
      if (! (SessionWrapper(true)->RunningAsAdministrator() || SessionWrapper(true)->RunningAsPowerUser()))
      {
	FATAL_MIKTEX_ERROR("ParseSetupCommandLine", T_("You must have administrator privileges to set up a common configuration directory."), 0);
      }
      cmdinfo.startupConfig.commonConfigRoot = optarg;
      break;

    case OPT_COMMON_DATA:
      if (! (SessionWrapper(true)->RunningAsAdministrator() || SessionWrapper(true)->RunningAsPowerUser()))
      {
	FATAL_MIKTEX_ERROR("ParseSetupCommandLine", T_("You must have administrator privileges to set up a common data directory."), 0);
      }
      cmdinfo.startupConfig.commonDataRoot = optarg;
      break;

    case OPT_COMMON_INSTALL:
      if (! (SessionWrapper(true)->RunningAsAdministrator() || SessionWrapper(true)->RunningAsPowerUser()))
      {
	FATAL_MIKTEX_ERROR("ParseSetupCommandLine", T_("You must have administrator privileges to set up a common installation directory."), 0);
      }
      cmdinfo.startupConfig.commonInstallRoot = optarg;
      break;

    case OPT_COMMON_ROOTS:
      if (! (SessionWrapper(true)->RunningAsAdministrator() || SessionWrapper(true)->RunningAsPowerUser()))
      {
	FATAL_MIKTEX_ERROR("ParseSetupCommandLine", T_("You must have administrator privileges to set up common root directories."), 0);
      }
      cmdinfo.startupConfig.commonRoots = optarg;
      break;

    case OPT_DOWNLOAD_ONLY:
      cmdinfo.task = SetupTask::Download;
      break;

    case OPT_DRY_RUN:
      cmdinfo.optDryRun = true;
      break;

    case OPT_HELP:
      FreeArguments(argc, argv);
      ShowHelpAndExit();
      break;

    case OPT_INSTALL_FROM_LOCAL_REPOSITORY:
      cmdinfo.task = SetupTask::InstallFromLocalRepository;
      break;

    case OPT_LOCAL_PACKAGE_REPOSITORY:
      cmdinfo.localPackageRepository = optarg;
      break;

    case OPT_NO_ADDITIONAL_ROOTS:
#if ENABLE_ADDTEXMF
      cmdinfo.optNoAddTEXMFDirs = true;
#endif
      break;

    case OPT_NO_REGISTRY:
      cmdinfo.optNoRegistry = true;
      break;

#if FEATURE_1874934
    case OPT_PAPER_SIZE:
      if (StringCompare(optarg, "A4", true) == 0)
      {
	cmdinfo.paperSize = "A4";
      }
      else if (StringCompare(optarg, "Letter", true) == 0)
      {
	cmdinfo.paperSize = "Letter";
      }
      else
      {
	FATAL_MIKTEX_ERROR("ParseSetupCommandLine", T_("Value must be one of: A4, Letter."), 0);
      }
      break;
#endif

    case OPT_PACKAGE_SET:
      if (StringCompare(optarg, "essential") == 0)
      {
	cmdinfo.packageLevel = PackageLevel::Essential;
      }
      else if (StringCompare(optarg, "basic") == 0)
      {
	cmdinfo.packageLevel = PackageLevel::Basic;
      }
      else if (StringCompare(optarg, "advanced") == 0)
      {
	cmdinfo.packageLevel = PackageLevel::Advanced;
      }
      else if (StringCompare(optarg, "complete") == 0)
      {
	cmdinfo.packageLevel = PackageLevel::Complete;
      }
      else
      {
	FATAL_MIKTEX_ERROR("ParseSetupCommandLine", T_("Invalid package set."), 0);
      }
      break;

    case OPT_PORTABLE:
      cmdinfo.optPortable = true;
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

    case OPT_SHARED:
      if (! (SessionWrapper(true)->RunningAsAdministrator() || SessionWrapper(true)->RunningAsPowerUser()))
      {
	FATAL_MIKTEX_ERROR("ParseSetupCommandLine", T_("You must have administrator privileges to set up a shared MiKTeX system."), 0);
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

    case OPT_USER_INSTALL:
      cmdinfo.startupConfig.userInstallRoot = optarg;
      break;

    case OPT_USER_ROOTS:
      cmdinfo.startupConfig.userRoots = optarg;
      break;

    default:
      FreeArguments(argc, argv);
      ShowHelpAndExit(1);
      break;
    }
  }
}

/* _________________________________________________________________________

   FindFile
   _________________________________________________________________________ */

bool FindFile(const PathName &	fileName, /*[out]*/ PathName & result)
{
  // try my directory
  result = SessionWrapper(true)->GetMyLocation();
  result += fileName;
  if (File::Exists(result))
  {
    return true;
  }

  // try the current directory
  result.SetToCurrentDirectory();
  result += fileName;
  if (File::Exists(result))
  {
    return true;
  }

  return false;
}

/* _________________________________________________________________________

   ReadSetupWizIni
   _________________________________________________________________________ */

bool ReadSetupWizIni(/*[out]*/ SetupCommandLineInfo & cmdinfo)
{
  PathName fileName;
  if (! FindFile("setupwiz.opt", fileName))
  {
    return false;
  }
  StreamReader reader(fileName);
  string commandLine;
  string line;
  while (reader.ReadLine(line))
  {
    commandLine += ' ';
    commandLine += line;
  }
  reader.Close();
  int argc;
  char ** argv;
  GetArguments(commandLine.c_str(), TU_(AfxGetAppName()), argc, argv);
  ParseSetupCommandLine(argc, argv, cmdinfo);
  FreeArguments(argc, argv);
  return true;
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

SetupWizardApplication::SetupWizardApplication() :
  prefabricatedPackageLevel(PackageLevel::None),
  mustReboot(false),
  showLogFileOnExit(false)
{
  SetAppID(UT_("MiKTeXorg.MiKTeX.Setup." MIKTEX_COMPONENT_VERSION_STR));
}

/* _________________________________________________________________________

   CheckAddTEXMFDirs
   _________________________________________________________________________ */


#if ENABLE_ADDTEXMF
void
CheckAddTEXMFDirs (/*[in,out]*/ string &	directories,
		   /*[out]*/ vector<PathName> &	vec)
{
  CSVList path (directories.c_str(), ';');

  vec.clear ();
  directories = "";

  for (; path.GetCurrent() != 0; ++ path)
    {
      if (! (theApp.GetStartupConfig().userDataRoot == path.GetCurrent()
	     || theApp.GetStartupConfig().userConfigRoot == path.GetCurrent()
	     || theApp.GetStartupConfig().userInstallRoot == path.GetCurrent()
	     || theApp.GetStartupConfig().commonDataRoot == path.GetCurrent()
	     || theApp.GetStartupConfig().commonConfigRoot == path.GetCurrent()
	     || theApp.GetStartupConfig().commonInstallRoot == path.GetCurrent()))
	{
	  if (vec.size() > 0)
	    {
	      directories += ';';
	    }
	  vec.push_back (path.GetCurrent());
	  directories += path.GetCurrent();
	}
    }
}
#endif

/* _________________________________________________________________________

   IsMiKTeXDirect
   _________________________________________________________________________ */

bool
IsMiKTeXDirectRoot (/*[out]*/ PathName & MiKTeXDirectRoot)
{
  // check ..\texmf\miktex\config\miktexstartup.ini
  MiKTeXDirectRoot = SessionWrapper(true)->GetMyLocation();
  MiKTeXDirectRoot += "..";
  MiKTeXDirectRoot.MakeAbsolute ();
  PathName pathStartupConfig = MiKTeXDirectRoot;
  pathStartupConfig += "texmf";
  pathStartupConfig += MIKTEX_PATH_STARTUP_CONFIG_FILE;
  if (! File::Exists(pathStartupConfig))
  {
    return (false);
  }
  FileAttributes attributes = File::GetAttributes(pathStartupConfig);
  if (((attributes & FileAttributes::ReadOnly) != 0) == 0)
  {
    return (false);
  }
  SmartPointer<Cfg> pcfg (Cfg::Create());      
  pcfg->Read (pathStartupConfig);
  string str;
  if (! pcfg->TryGetValue("Auto", "Config", str) || str != "Direct")
  {
    return (false);
  }
  theApp.pSetupService->Log(T_("started from MiKTeXDirect location\n"));
  return (true);
}

/* _________________________________________________________________________

   SetupGlobalVars
   _________________________________________________________________________ */

void
SetupGlobalVars (/*[in]*/ const SetupCommandLineInfo &	cmdinfo)
{
  SetupOptions options = theApp.pSetupService->GetOptions();

  options.IsPrefabricated = false;
  options.IsRegisterPathEnabled = true;

  options.IsDryRun = cmdinfo.optDryRun;
  options.PackageLevel = cmdinfo.packageLevel;
  options.IsPortable = cmdinfo.optPortable;

  theApp.allowUnattendedReboot = cmdinfo.optAllowUnattendedReboot;
  theApp.unattended = cmdinfo.optUnattended;

  // check to see whether setup is started from a MiKTeXDirect location
  theApp.isMiKTeXDirect = IsMiKTeXDirectRoot(options.MiKTeXDirectRoot);

  // startup configuration
  options.Config = cmdinfo.startupConfig;
  if (options.Config.commonInstallRoot.Empty())
  {
    options.Config.commonInstallRoot = SetupService::GetDefaultCommonInstallDir();
  }

  options.IsRegistryEnabled = ! cmdinfo.optNoRegistry;

  // shared setup
  options.IsCommonSetup = SessionWrapper(true)->RunningAsAdministrator() ||
     cmdinfo.optShared ||
     ! cmdinfo.startupConfig.commonRoots.empty() ||
     ! cmdinfo.startupConfig.commonInstallRoot.Empty() ||
     ! cmdinfo.startupConfig.commonDataRoot.Empty() ||
     ! cmdinfo.startupConfig.commonConfigRoot.Empty();

  // auto install
#if FEATURE_1874934
  if (cmdinfo.installOnTheFly != TriState::Undetermined)
  {
    options.IsInstallOnTheFlyEnabled = cmdinfo.installOnTheFly;
  }
#endif

  // paper size
#if FEATURE_1874934
  if (! cmdinfo.paperSize.empty())
  {
    options.PaperSize = cmdinfo.paperSize;
  }
#endif

  if (! cmdinfo.folderName.empty())
  {
    options.FolderName = cmdinfo.folderName;
  }

  // local package repository
  if (! cmdinfo.localPackageRepository.Empty())
  {
    options.LocalPackageRepository = cmdinfo.localPackageRepository;
    if (cmdinfo.task != SetupTask::Download)
    {
      PackageLevel foundPackageLevel = SetupService::TestLocalRepository(options.LocalPackageRepository, options.PackageLevel);
      if (foundPackageLevel == PackageLevel::None)
      {
	FATAL_MIKTEX_ERROR ("SetupGlobalVars",
	  T_("The specified local repository does not exist."),
	  0);
      }
      if (options.PackageLevel == PackageLevel::None)
      {
	options.PackageLevel = foundPackageLevel;
      }
    }
  }
  else
  {
    PackageLevel foundPackageLevel = SetupService::SearchLocalRepository(options.LocalPackageRepository, options.PackageLevel, options.IsPrefabricated);
    if (foundPackageLevel != PackageLevel::None)
    {
      if (options.PackageLevel == PackageLevel::None)
      {
	options.PackageLevel = foundPackageLevel;
      }
      if (options.IsPrefabricated)
      {
	theApp.prefabricatedPackageLevel = foundPackageLevel;
      }
    }
    else
    {
      // check the default location
      options.LocalPackageRepository = SetupService::GetDefaultLocalRepository();
      PackageLevel foundPackageLevel = SetupService::TestLocalRepository(options.LocalPackageRepository, options.PackageLevel);
      if (options.PackageLevel == PackageLevel::None)
      {
	options.PackageLevel = foundPackageLevel;
      }
    }
  }

  // setup task
  options.Task = cmdinfo.task;
  if (options.Task == SetupTask::None)
  {
    if (theApp.isMiKTeXDirect)
    {
      options.Task = SetupTask::InstallFromCD;
    }
    else if (! options.LocalPackageRepository.Empty() && options.PackageLevel != PackageLevel::None)
    {
      options.Task = SetupTask::InstallFromLocalRepository;
    }
  }

  // remote package repository
  options.RemotePackageRepository = cmdinfo.remotePackageRepository;
  if (options.RemotePackageRepository.empty())
  {
    string str;
    if (theApp.pManager->TryGetRemotePackageRepository(str))
    {
      options.RemotePackageRepository = str.c_str();
    }
  }

  // check variables, if started in unattended mode
  if (theApp.unattended)
  {
    if (options.Task == SetupTask::None)
    {
      FATAL_MIKTEX_ERROR("SetupGlobalVars", T_("No setup task has been specified."), 0);
    }
    if (options.PackageLevel == PackageLevel::None)
    {
      FATAL_MIKTEX_ERROR("SetupGlobalVars", T_("No package set has been specified."), 0);
    }
    if (options.Task == SetupTask::InstallFromLocalRepository || options.Task == SetupTask::Download)
    {
      if (options.LocalPackageRepository.Empty())
      {
	FATAL_MIKTEX_ERROR("SetupGlobalVars", T_("No local package repository has been specified."), 0);
      }
    }
    if (options.Task == SetupTask::InstallFromRemoteRepository || options.Task == SetupTask::Download)
    {
      if (options.RemotePackageRepository.empty())
      {
	FATAL_MIKTEX_ERROR("SetupGlobalVars", T_("No remote package repository has been specified."), 0);
      }
    }
  }

  options = theApp.pSetupService->SetOptions(options);
}

/* _________________________________________________________________________

   ExtractFiles
   _________________________________________________________________________ */

bool
ExtractFiles (/*[in,out]*/ ScratchDirectory &	sfxDir)
{
  _TCHAR szPath[BufferSizes::MaxPath];
  if (GetModuleFileNameW(0, szPath, BufferSizes::MaxPath) == 0)
    {
      FATAL_WINDOWS_ERROR ("GetModuleFileName", 0);
    }
  FileStream myImage (File::Open(szPath,
				 FileMode::Open,
				 FileAccess::Read,
				 false));
  char magic[16];
  while (myImage.Read(magic, 16) == 16)
  {
    static char const MAGIC3[3] = { 'T', 'A', 'R' };
    if (memcmp(magic, MAGIC3, 3) == 0
      && memcmp(magic + 3, MAGIC3, 3) == 0
      && memcmp(magic + 6, MAGIC3, 3) == 0
      && memcmp(magic + 9, MAGIC3, 3) == 0
      && memcmp(magic + 12, MAGIC3, 3) == 0
      && memcmp(magic + 15, MAGIC3, 1) == 0)
    {
      sfxDir.Enter ();
      auto_ptr<MiKTeX::Extractor::Extractor>
	pExtractor
	(MiKTeX::Extractor::Extractor::CreateExtractor
	(MiKTeX::Extractor::ArchiveFileType::Tar));
      pExtractor->Extract (&myImage,
	Directory::GetCurrentDirectoryA(),
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
      AfxMessageBox (T_(_T("The application could not be initialized (1).")),
		     MB_ICONSTOP | MB_OK);
      return (FALSE);
    }

  if (FAILED(CoInitialize(0)))
    {
      AfxMessageBox (T_(_T("The application could not be initialized (2).")),
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
      startupConfig.userInstallRoot = scratchRoot.Get();
      startupConfig.userDataRoot = scratchRoot.Get();
      startupConfig.userConfigRoot = scratchRoot.Get();
      startupConfig.commonDataRoot = scratchRoot.Get();
      startupConfig.commonConfigRoot = scratchRoot.Get();
      startupConfig.commonInstallRoot = scratchRoot.Get();
      Session::InitInfo initInfo ("setup",
				  Session::InitFlags::NoConfigFiles);
      initInfo.SetStartupConfig (startupConfig);
      SessionWrapper pSession (initInfo);

      // extract package archive files
      ScratchDirectory sfxDir;
      bool selfExtractor = ExtractFiles(sfxDir);

      // create package manager
      pManager = PackageManager::Create();

      // create setup service
      pSetupService = SetupService::Create();
      SetupOptions options = pSetupService->GetOptions();
      CString banner;
      if (! banner.LoadString(IDS_SETUPWIZ))
      {
	UNEXPECTED_CONDITION ("InitInstance");
      }
      options.Banner = TU_(banner);
      options.Version = MIKTEX_COMPONENT_VERSION_STR;
      options = pSetupService->SetOptions(options);

      // set trace options
      traceStream = auto_ptr<TraceStream>(TraceStream::Open("setup"));
      TraceStream::SetTraceFlags
	("error,extractor,mpm,process,config,setup");
      
      // get command-line arguments
      int argc;
      char ** argv;
      GetArguments (TU_(m_lpCmdLine), TU_(AfxGetAppName()), argc, argv);
      SetupCommandLineInfo cmdinfo;
      ReadSetupWizIni (cmdinfo);
      ParseSetupCommandLine (argc, argv, cmdinfo);
      FreeArguments (argc, argv);
      CheckStartupConfig (cmdinfo.startupConfig);
      if (cmdinfo.optPrivate && cmdinfo.optShared)
	{
	  FATAL_MIKTEX_ERROR
	    ("SetupWizardApplication::InitInstance",
	     T_("You cannot specify --private along with --shared."),
	     0);
	}
      if (cmdinfo.optPrivate
	  && ! (cmdinfo.startupConfig.commonDataRoot.Empty()
		&& cmdinfo.startupConfig.commonRoots.empty()
		&& cmdinfo.startupConfig.commonInstallRoot.Empty()
		&& cmdinfo.startupConfig.commonConfigRoot.Empty()))
	{
	  FATAL_MIKTEX_ERROR
	    ("SetupWizardApplication::InitInstance",
	     T_("You cannot specify --private along with \
--common-config, common-data, --common-install or --common-roots."),
	     0);
	}
      SetupGlobalVars (cmdinfo);

      // open the log file
      pSetupService->OpenLog();

      INT_PTR dlgRet;
      
      // run the wizard
      {
	SetupWizard dlg (pManager.Get());
	m_pMainWnd = &dlg;
	dlgRet = dlg.DoModal ();
      }
      
      // clean up
      PathName pathLogFile = pSetupService->CloseLog(dlgRet == IDCANCEL);
      if (theApp.showLogFileOnExit && ! pathLogFile.Empty())
      {
	INT_PTR r = reinterpret_cast<INT_PTR>(
	  ShellExecuteW(0, L"open", pathLogFile.ToWideCharString().c_str(), 0, 0, SW_SHOWNORMAL));
	if (r <= 32)
	{
	  Process::Start("notepad.exe", pathLogFile.Get());
	}
      }
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
  _TCHAR szShortPath1[BufferSizes::MaxPath];
  _TCHAR szShortPath2[BufferSizes::MaxPath];

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
  if (isalpha(str[0]) && str[1] == ':' && IsDirectoryDelimiter(str[2]))
    {
      CString driveRoot = str.Left(3);
      if (! Directory::Exists(PathName(driveRoot)))
	{
	  CString message;
	  message.Format (T_(_T("The specified path is invalid because the \
root directory %s does not exist.")),
			  static_cast<LPCTSTR>(driveRoot));
	  AfxMessageBox (message, MB_ICONEXCLAMATION);
	  message.Empty ();
	  pDX->Fail ();
	}
      str2 = str.GetString() + 3;
    }
  else
    {
      PathName uncRoot;
      if (! Utils::GetUncRootFromPath(TU_(str), uncRoot))
	{
	  CString message;
	  message.Format (T_(_T("The specified path is invalid because it is not \
fully qualified.")));
	  AfxMessageBox (message, MB_ICONEXCLAMATION);
	  message.Empty ();
	  pDX->Fail ();
	}
      if (! Directory::Exists(uncRoot))
	{
	  CString message;
	  message.Format (T_(_T("The specified path is invalid because the UNC \
root directory %s does not exist.")),
			  uncRoot.Get());
	  AfxMessageBox (message, MB_ICONEXCLAMATION);
	  message.Empty ();
	  pDX->Fail ();
	}
      str2 = str;
    }
  int i = str2.FindOneOf(_T(":*?\"<>|;="));
  if (i >= 0)
    {
      CString message;
      message.Format (T_(_T("The specified path is invalid because it contains \
an invalid character (%c).")),
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
      string str;
      str =
	T_("The operation could not be completed for the following reason: ");
      str += "\n\n";
      str += e.what();
      if (! e.GetInfo().empty())
	{
	  str += "\n\n";
	  str += T_("Details: ");
	  str += e.GetInfo();
	}
      AfxMessageBox (UT_(str.c_str()), MB_OK | MB_ICONSTOP);
      theApp.pSetupService->Log(T_("\nAn error occurred:\n"));
      theApp.pSetupService->Log(T_("  source file: %s\n"), e.GetSourceFile().c_str());
      theApp.pSetupService->Log(T_("  source line: %d\n"), e.GetSourceLine());
      theApp.pSetupService->Log(T_("  message: %s\n"), e.what());
      theApp.pSetupService->Log(T_("  info: %s\n"), e.GetInfo().c_str());
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
      string str;
      str =
	T_("The operation could not be completed for the following reason: ");
      str += "\n\n";
      str += e.what();
      theApp.pSetupService->Log("\n%s\n", str.c_str());
      AfxMessageBox (UT_(str.c_str()), MB_OK | MB_ICONSTOP);
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   SplitUrl
   _________________________________________________________________________ */

void
SplitUrl (/*[in]*/ const string &	url,
	  /*[out]*/ string &		protocol,
	  /*[out]*/ string &		host)
{
  wchar_t szProtocol[200];
  wchar_t szHost[200];
  URL_COMPONENTSW url_comp = { 0 };
  url_comp.dwStructSize = sizeof(url_comp);
  url_comp.lpszScheme = szProtocol;
  url_comp.dwSchemeLength = 200;
  url_comp.lpszHostName = szHost;
  url_comp.dwHostNameLength = 200;
  if (! InternetCrackUrlW(UW_(url.c_str()), 0, 0, &url_comp))
    {
      FATAL_WINDOWS_ERROR ("InternetCrackUrlW", 0);
    }
  protocol = WU_(szProtocol);
  host = WU_(szHost);
}
