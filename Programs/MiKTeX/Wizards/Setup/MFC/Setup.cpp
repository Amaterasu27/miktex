/* Setup.cpp:

   Copyright (C) 1999-2009 Christian Schenk

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
#if ENABLE_ADDTEXMF
      optNoAddTEXMFDirs (false),
#endif
      optNoRegistry (false),
      optPrivate (false),
      optPortable (false),
      optShared (false),
      optUnattended (false),
      packageLevel (PackageLevel::None),
#if FEATURE_1874934
      installOnTheFly (TriState::Undetermined),
#endif
      task (SetupTask::None)
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

void
AddArgument (/*[in]*/ const CString &	argument,
	     /*[in,out]*/ int &		argc,
	     /*[in,out]*/ char ** &	argv,
	     /*[in,out]*/ int &		argMax)
{
  if (argc == argMax)
    {
      argMax += 10;
      argv =
	reinterpret_cast<char**>(realloc(argv,
					 argMax * sizeof(argv[0])));
    }
  argv[ argc++ ] = _tcsdup(argument);
}

/* _________________________________________________________________________

   GetArguments
   _________________________________________________________________________ */

void
GetArguments (/*[in]*/ const char *		lpszCommandLine,
	      /*[in]*/ const char *		lpszExeName,
	      /*[in,out]*/ int &		argc,
	      /*[in,out]*/ char ** &		argv)
{
  argc = 0;
  argv = 0;

  int argMax = 0;

  AddArgument (lpszExeName, argc, argv, argMax);

  CString arg;

  bool copying = false;
  bool inQuotedArg = false;

  const char * lpsz = lpszCommandLine;

  while (*lpsz != 0)
    {
      if (*lpsz == ' ' && ! inQuotedArg)
	{
	  if (copying)
	    {
	      AddArgument (arg, argc, argv, argMax);
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
      AddArgument (arg, argc, argv, argMax);
    }
}

/* _________________________________________________________________________

   FreeArguments
   _________________________________________________________________________ */

void
FreeArguments (/*[in]*/ int		argc,
	       /*[in,out]*/ char ** & 	argv)
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
  { "allow-unattended-reboot", no_argument, 0,
    OPT_ALLOW_UNATTENDED_REBOOT },
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
  { "install-from-local-repository", no_argument, 0,
    OPT_INSTALL_FROM_LOCAL_REPOSITORY },
  { "local-package-repository", required_argument, 0,
    OPT_LOCAL_PACKAGE_REPOSITORY },
  { "no-additional-roots", no_argument, 0, OPT_NO_ADDITIONAL_ROOTS },
  { "no-registry", no_argument, 0, OPT_NO_REGISTRY },
  { "package-set", required_argument, 0, OPT_PACKAGE_SET },
#if FEATURE_1874934
  { "paper-size", required_argument, 0, OPT_PAPER_SIZE },
#endif
  { "portable", no_argument, 0, OPT_PORTABLE },
  { "private", no_argument, 0, OPT_PRIVATE },
  { "program-folder", required_argument, 0, OPT_PROGRAM_FOLDER },
  { "remote-package-repository", required_argument, 0,
    OPT_REMOTE_PACKAGE_REPOSITORY },
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

void
ShowHelpAndExit (/*[in]*/ int retCode = 0)
{
  AfxMessageBox (T_("Usage: setupwiz [OPTIONS]\r\n\
\r\n\
Options:\r\n\r\n\
  --allow-unattended-reboot\r\n")
#if FEATURE_1874934
		 T_("\
  --auto-install=yes\r\n\
  --auto-install=no\r\n\
  --auto-install=ask\r\n")
#endif
		 T_("\
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
  --package-set=SET\r\n")
#if FEATURE_1874934
		 T_("\
  --paper-size=A4\r\n\
  --paper-size=Letter\r\n")
#endif
		 T_("\
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
See the MiKTeX Manual for more information."));
  exit (retCode);
}

/* _________________________________________________________________________

   CheckStartupConfig
   _________________________________________________________________________ */

void
CheckStartupConfig (/*[in,out]*/ StartupConfig & startupConfig)
{
#if 1
  string commonRoots;
  for (CSVList tok (startupConfig.commonRoots.c_str(), ';');
    tok.GetCurrent() != 0;
    ++ tok)
  {
    PathName path (tok.GetCurrent());
    if (path.Empty())
    {
      continue;
    }
    path.MakeAbsolute ();
    if (startupConfig.commonConfigRoot == path)
    {
      FATAL_MIKTEX_ERROR ("CheckStartupConfig",
	T_("\
Improper options: \
--common-roots value collides with --common-config value."),
	path.Get());
    }
    if (startupConfig.commonDataRoot == path)
    {
      FATAL_MIKTEX_ERROR ("CheckStartupConfig",
	T_("\
Improper options: \
--common-roots value collides with --common-data value."),
	path.Get());
    }
    if (startupConfig.commonInstallRoot == path)
    {
      FATAL_MIKTEX_ERROR ("CheckStartupConfig",
	T_("\
Improper options: \
--common-roots value collides with --common-install value."),
	path.Get());
    }
    if (startupConfig.userConfigRoot == path)
    {
      FATAL_MIKTEX_ERROR ("CheckStartupConfig",
	T_("\
Improper options: \
--common-roots value collides with --user-config value."),
	path.Get());
    }
    if (startupConfig.userDataRoot == path)
    {
      FATAL_MIKTEX_ERROR ("CheckStartupConfig",
	T_("\
Improper options: \
--common-roots value collides with --user-data value."),
	path.Get());
    }
    if (startupConfig.userInstallRoot == path)
    {
      FATAL_MIKTEX_ERROR ("CheckStartupConfig",
	T_("\
Improper options: \
--common-roots value collides with --user-install value."),
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
  for (CSVList tok (startupConfig.userRoots.c_str(), ';');
    tok.GetCurrent() != 0;
    ++ tok)
  {
    PathName path (tok.GetCurrent());
    if (path.Empty())
    {
      continue;
    }
    path.MakeAbsolute ();
    if (Utils::Contains(startupConfig.commonRoots.c_str(), path.Get(), ";", true))
    {
      FATAL_MIKTEX_ERROR ("CheckStartupConfig",
	T_("\
Improper options: \
--user-roots value collides with --common-roots value."),
	path.Get());
    }
    if (startupConfig.commonConfigRoot == path)
    {
      FATAL_MIKTEX_ERROR ("CheckStartupConfig",
	T_("\
Improper options: \
--user-roots value collides with --common-config value."),
	path.Get());
    }
    if (startupConfig.commonDataRoot == path)
    {
      FATAL_MIKTEX_ERROR ("CheckStartupConfig",
	T_("\
Improper options: \
--user-roots value collides with --common-data value."),
	path.Get());
    }
    if (startupConfig.commonInstallRoot == path)
    {
      FATAL_MIKTEX_ERROR ("CheckStartupConfig",
	T_("\
Improper options: \
--user-roots value collides with --common-install value."),
	path.Get());
    }
    if (startupConfig.userConfigRoot == path)
    {
      FATAL_MIKTEX_ERROR ("CheckStartupConfig",
	T_("\
Improper options: \
--user-roots value collides with --user-config value."),
	path.Get());
    }
    if (startupConfig.userDataRoot == path)
    {
      FATAL_MIKTEX_ERROR ("CheckStartupConfig",
	T_("\
Improper options: \
--user-roots value collides with --user-data value."),
	path.Get());
    }
    if (startupConfig.userInstallRoot == path)
    {
      FATAL_MIKTEX_ERROR ("CheckStartupConfig",
	T_("\
Improper options: \
--user-roots value collides with --user-install value."),
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

void
ParseSetupCommandLine (/*[in]*/ int				argc,
		       /*[in]*/ char **				argv,
		       /*[in,out]*/ SetupCommandLineInfo &	cmdinfo)
{
  int iOptIdx = 0;
  int c;

  cmdinfo.m_nShellCommand = CCommandLineInfo::FileNothing;

  optind = 0;

  while ((c = getopt_long_only(argc, argv, "", long_options, &iOptIdx))
	 != EOF)
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
	      FATAL_MIKTEX_ERROR
		("ParseSetupCommandLine",
		 T_("Value must be one of: yes, no, ask."),
		 0);
	    }
	  break;
#endif

	case OPT_COMMON_CONFIG:
	  if (IsWindowsNT()
	      && ! (SessionWrapper(true)->RunningAsAdministrator()
		    || SessionWrapper(true)->RunningAsPowerUser()))
	    {
	      FATAL_MIKTEX_ERROR
		("ParseSetupCommandLine",
		 T_("You must have administrator privileges to set up \
a common configuration directory."),
		 0);
	    }
	  cmdinfo.startupConfig.commonConfigRoot = optarg;
	  cmdinfo.startupConfig.commonConfigRoot.MakeAbsolute ();
	  break;

	case OPT_COMMON_DATA:
	  if (IsWindowsNT()
	      && ! (SessionWrapper(true)->RunningAsAdministrator()
		    || SessionWrapper(true)->RunningAsPowerUser()))
	    {
	      FATAL_MIKTEX_ERROR
		("ParseSetupCommandLine",
		 T_("You must have administrator privileges to set up \
a common data directory."),
		 0);
	    }
	  cmdinfo.startupConfig.commonDataRoot = optarg;
	  cmdinfo.startupConfig.commonDataRoot.MakeAbsolute ();
	  break;

	case OPT_COMMON_INSTALL:
	  if (IsWindowsNT()
	      && ! (SessionWrapper(true)->RunningAsAdministrator()
		    || SessionWrapper(true)->RunningAsPowerUser()))
	    {
	      FATAL_MIKTEX_ERROR
		("ParseSetupCommandLine",
		 T_("You must have administrator privileges to set up \
a common installation directory."),
		 0);
	    }
	  cmdinfo.startupConfig.commonInstallRoot = optarg;
	  cmdinfo.startupConfig.commonInstallRoot.MakeAbsolute ();
	  break;

	case OPT_COMMON_ROOTS:
	  if (IsWindowsNT()
	      && ! (SessionWrapper(true)->RunningAsAdministrator()
		    || SessionWrapper(true)->RunningAsPowerUser()))
	    {
	      FATAL_MIKTEX_ERROR
		("ParseSetupCommandLine",
		 T_("You must have administrator privileges to set up \
common root directories."),
		 0);
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
	  FreeArguments (argc, argv);
	  ShowHelpAndExit ();
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
	      FATAL_MIKTEX_ERROR
		("ParseSetupCommandLine",
		 T_("Value must be one of: A4, Letter."),
		 0);
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
	      FATAL_MIKTEX_ERROR
		("ParseSetupCommandLine",
		 T_("Invalid package set."),
		 0);
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
	  if (IsWindowsNT()
	      && ! (SessionWrapper(true)->RunningAsAdministrator()
		    || SessionWrapper(true)->RunningAsPowerUser()))
	    {
	      FATAL_MIKTEX_ERROR
		("ParseSetupCommandLine",
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
	  cmdinfo.startupConfig.userConfigRoot.MakeAbsolute ();
	  break;

	case OPT_USER_DATA:
	  cmdinfo.startupConfig.userDataRoot = optarg;
	  cmdinfo.startupConfig.userDataRoot.MakeAbsolute ();
	  break;

	case OPT_USER_INSTALL:
	  cmdinfo.startupConfig.userInstallRoot = optarg;
	  cmdinfo.startupConfig.userInstallRoot.MakeAbsolute ();
	  break;

	case OPT_USER_ROOTS:
	  cmdinfo.startupConfig.userRoots = optarg;
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
  if (! FindFile("setupwiz.opt", fileName))
    {
      return (false);
    }
  StreamReader reader (fileName);
  CString commandLine;
  string line;
  while (reader.ReadLine(line))
    {
      commandLine += ' ';
      commandLine += line.c_str();
    }
  reader.Close ();
  int argc;
  char ** argv;
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
    prefabricatedPackageLevel (PackageLevel::None),
    installOnTheFly (TriState::Undetermined),
    paperSize ("A4"),
    setupTask (SetupTask::None)
{
}

/* _________________________________________________________________________

   FindCommonInstallDir
   _________________________________________________________________________ */

PathName
FindCommonInstallDir ()
{
  // probe the registry
  string path;
  if (SessionWrapper(true)
      ->TryGetConfigValue(MIKTEX_REGKEY_CORE,
			  MIKTEX_REGVAL_COMMON_INSTALL,
			  path))
    {
      return (path);
    }
  else
    {
      // return the default location: "C:\Program Files\MiKTeX X.Y"
      PathName path =
	    Utils::GetFolderPath(CSIDL_PROGRAM_FILES,
				 CSIDL_PROGRAM_FILES,
				 true);
      path += MIKTEX_PRODUCTNAME_STR " " MIKTEX_SERIES_STR;
      return (path);
    }
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
      if (! (theApp.startupConfig.userDataRoot == path.GetCurrent()
	     || theApp.startupConfig.userConfigRoot == path.GetCurrent()
	     || theApp.startupConfig.userInstallRoot == path.GetCurrent()
	     || theApp.startupConfig.commonDataRoot == path.GetCurrent()
	     || theApp.startupConfig.commonConfigRoot == path.GetCurrent()
	     || theApp.startupConfig.commonInstallRoot == path.GetCurrent()))
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

   TestLocalRepository
   _________________________________________________________________________ */

PackageLevel
TestLocalRepository (/*[in]*/ const PathName &		pathRepository,
		     /*[in]*/ PackageLevel		requestedPackageLevel)
{
  PathName pathInfoFile (pathRepository, DOWNLOAD_INFO_FILE);
  if (! File::Exists(pathInfoFile))
    {
      return (PackageLevel::None);
    }
  StreamReader stream (pathInfoFile);
  string firstLine;
  bool haveFirstLine = stream.ReadLine(firstLine);
  stream.Close ();
  if (! haveFirstLine)
    {
      return (PackageLevel::None);
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
      return (PackageLevel::None);
    }
  if (requestedPackageLevel > packageLevel_)
    {
      // doesn't have the requested package set
      return (PackageLevel::None);
    }
  return (packageLevel_);
}

/* _________________________________________________________________________

   SearchLocalRepository
   _________________________________________________________________________ */

PackageLevel
SearchLocalRepository (/*[out]*/ PathName &	localRepository,
		       /*[in]*/ PackageLevel	requestedPackageLevel,
		       /*[out]*/ bool &		prefabricated)
{
  PackageLevel packageLevel_ = PackageLevel::None;

  // try current directory
  localRepository.SetToCurrentDirectory ();
  packageLevel_ = TestLocalRepository(localRepository, requestedPackageLevel);
  if (packageLevel_ != PackageLevel::None)
    {
      prefabricated = true;
      return (packageLevel_);
    }

  // try my directory
  localRepository = SessionWrapper(true)->GetMyLocation();
  packageLevel_ = TestLocalRepository(localRepository, requestedPackageLevel);
  if (packageLevel_ != PackageLevel::None)
    {
      prefabricated = true;
      return (packageLevel_);
    }

  // try ..\tm\packages
  localRepository = SessionWrapper(true)->GetMyLocation();
  localRepository += "..";
  localRepository += "tm";
  localRepository += "packages";
  localRepository.MakeAbsolute ();
  packageLevel_ = TestLocalRepository(localRepository, requestedPackageLevel);
  if (packageLevel_ != PackageLevel::None)
    {
      prefabricated = true;
      return (packageLevel_);
    }

  // try last directory
  if (PackageManager::TryGetLocalPackageRepository(localRepository))
    {
      packageLevel_ = TestLocalRepository(localRepository, requestedPackageLevel);
      if (packageLevel_ != PackageLevel::None)
      {
	prefabricated = false;
	return (packageLevel_);
      }
    }

  return (PackageLevel::None);
}

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
  string val;
  if (SessionWrapper(true)
      ->TryGetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			  MIKTEX_REGVAL_LOCAL_REPOSITORY,
			  val))
    {
      ret = val;
    }
  else
    {
      // try Internet Explorer download directory
      AutoHKEY hkey;
      LONG res =
	RegOpenKeyExW(HKEY_CURRENT_USER,
		      L"SOFTWARE\\Microsoft\\Internet Explorer",
		      0,
		      KEY_READ,
		      &hkey);
      if (res == ERROR_SUCCESS)
	{
	  DWORD type;
	  DWORD len = static_cast<DWORD>(ret.GetCapacity() * sizeof(ret[0]));
	  res =
	    RegQueryValueExA
	    (hkey.Get(),
	     "Download Directory",
	     0,
	     &type,
	     reinterpret_cast<unsigned char *>(ret.GetBuffer()),
	     &len);
	}
      if (res != ERROR_SUCCESS || ! Directory::Exists(ret))
	{
	  // default is "%TEMP%\MiKTeX X.Y Packages"
	  ret.SetToTempDirectory ();
	}
      ret += MIKTEX_PRODUCTNAME_STR " " MIKTEX_SERIES_STR " ";
      ret.Append (T_("Setup"), false);
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
  theApp.portable = cmdinfo.optPortable;

  // check to see whether setup is started from a MiKTeXDirect location
  theApp.isMiKTeXDirect = IsMiKTeXDirectRoot(theApp.MiKTeXDirectRoot);
  if (theApp.isMiKTeXDirect)
    {
      theApp.MiKTeXDirectTeXMFRoot = theApp.MiKTeXDirectRoot;
      theApp.MiKTeXDirectTeXMFRoot += "texmf";
    }

  // startup configuration
  theApp.startupConfig = cmdinfo.startupConfig;
  if (theApp.startupConfig.commonInstallRoot.Empty())
    {
      theApp.startupConfig.commonInstallRoot = FindCommonInstallDir();
    }

  theApp.noRegistry = cmdinfo.optNoRegistry;

  // shared setup
  theApp.commonUserSetup =
    ((IsWindowsNT() && SessionWrapper(true)->RunningAsAdministrator())
     || cmdinfo.optShared
     || ! cmdinfo.startupConfig.commonRoots.empty()
     || ! cmdinfo.startupConfig.commonInstallRoot.Empty()
     || ! cmdinfo.startupConfig.commonDataRoot.Empty()
     || ! cmdinfo.startupConfig.commonConfigRoot.Empty());

  // auto install
#if FEATURE_1874934
  if (cmdinfo.installOnTheFly != TriState::Undetermined)
    {
      theApp.installOnTheFly = cmdinfo.installOnTheFly;
    }
#endif

  // paper size
#if FEATURE_1874934
  if (! cmdinfo.paperSize.empty())
    {
      theApp.paperSize = cmdinfo.paperSize;
    }
#endif

  // startup menu item (default: "MiKTeX X.Y")
  theApp.folderName = cmdinfo.folderName;
  if (theApp.folderName.empty())
    {
      theApp.folderName =
	MIKTEX_PRODUCTNAME_STR " " MIKTEX_SERIES_STR;
    }

  // local package repository
  if (! cmdinfo.localPackageRepository.Empty())
    {
      theApp.localPackageRepository = cmdinfo.localPackageRepository;
      if (cmdinfo.task != SetupTask::Download)
	{
	  PackageLevel foundPackageLevel =
	    TestLocalRepository(theApp.localPackageRepository,
				theApp.packageLevel);
	  if (foundPackageLevel == PackageLevel::None)
	  {
	    FATAL_MIKTEX_ERROR ("SetupGlobalVars",
	      T_("The specified local repository does not exist."),
	      0);
	  }
	  if (theApp.packageLevel == PackageLevel::None)
	  {
	    theApp.packageLevel = foundPackageLevel;
	  }
	}
    }
  else
  {
    PackageLevel foundPackageLevel =
      SearchLocalRepository(theApp.localPackageRepository,
			    theApp.packageLevel,
			    theApp.prefabricated);
    if (foundPackageLevel != PackageLevel::None)
    {
      if (theApp.packageLevel == PackageLevel::None)
      {
	theApp.packageLevel = foundPackageLevel;
      }
      if (theApp.prefabricated)
      {
	theApp.prefabricatedPackageLevel = foundPackageLevel;
      }
    }
    else
    {
      // check the default location
      theApp.localPackageRepository = GetDefaultLocalRepository();
      PackageLevel foundPackageLevel =
	TestLocalRepository(theApp.localPackageRepository,
			    theApp.packageLevel);
      if (theApp.packageLevel == PackageLevel::None)
      {
	theApp.packageLevel = foundPackageLevel;
      }
    }
  }

  // setup task
  theApp.setupTask = cmdinfo.task;
  if (theApp.setupTask == SetupTask::None)
    {
      if (theApp.isMiKTeXDirect)
	{
	  theApp.setupTask = SetupTask::InstallFromCD;
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
      string str;
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
	    ("SetupGlobalVars",
	     T_("No setup task has been specified."),
	     0);
	}
      if (theApp.packageLevel == PackageLevel::None)
	{
	  FATAL_MIKTEX_ERROR
	    ("SetupGlobalVars",
	     T_("No package set has been specified."),
	     0);
	}
      if (theApp.setupTask == SetupTask::InstallFromLocalRepository
	  || theApp.setupTask == SetupTask::Download)
	{
	  if (theApp.localPackageRepository.Empty())
	    {
	      FATAL_MIKTEX_ERROR
		("SetupGlobalVars",
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
		("SetupGlobalVars",
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
	  if (Directory::Exists(theApp.GetInstallRoot()))
	    {
	      pathLogDir.Set (theApp.GetInstallRoot(),
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
      pathLogFile += t.Format("download-%Y-%m-%d-%H-%M");
    }
  else
    {
      pathLogFile += t.Format("setup-%Y-%m-%d-%H-%M");
    }
  pathLogFile.SetExtension (".log");

  // install the log file
  // <todo>add the log file to the uninstall script</todo>
  File::Copy (theApp.intermediateLogFile, pathLogFile);

  // remove the intermediate log file
  File::Delete (theApp.intermediateLogFile);

  if (theApp.showLogFileOnExit)
    {
      INT_PTR r =
	reinterpret_cast<INT_PTR>(ShellExecute(0,
					       "open",
					       pathLogFile.Get(),
					       0,
					       0,
					       SW_SHOWNORMAL));
      if (r <= 32)
	{
	  Process::Start ("notepad.exe", pathLogFile.Get());
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
      FATAL_WINDOWS_ERROR ("GetModuleFileName", 0);
    }
  FileStream myImage (File::Open(path,
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

      // set trace options
      traceStream = auto_ptr<TraceStream>(TraceStream::Open("setup"));
      TraceStream::SetTraceFlags
	("error,extractor,mpm,process,config,setup");
      
      // get command-line arguments
      int argc;
      char ** argv;
      GetArguments (m_lpCmdLine, AfxGetAppName(), argc, argv);
      SetupCommandLineInfo cmdinfo;
      if (GetModuleFileName(0, setupPath.GetBuffer(), BufferSizes::MaxPath)
	  == 0)
	{
	  FATAL_WINDOWS_ERROR ("GetModuleFileName", 0);
	}
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
ContainsBinDir (/*[in]*/ const char *	lpszPath)
{
  PathName pathBinDir;
  if (theApp.setupTask == SetupTask::PrepareMiKTeXDirect)
    {
      pathBinDir.Set (theApp.MiKTeXDirectTeXMFRoot, MIKTEX_PATH_BIN_DIR);
    }
  else
    {
      pathBinDir.Set (theApp.GetInstallRoot(), MIKTEX_PATH_BIN_DIR);
    }
  CSVList bindir (lpszPath, ';');
  for (; bindir.GetCurrent() != 0; ++ bindir)
    {
      size_t l = StrLen(bindir.GetCurrent());
      if (l == 0)
	{
	  continue;
	}
      PathName pathBinDir2;
      if (bindir.GetCurrent()[0] == '"'
	  && l > 2
	  && bindir.GetCurrent()[l - 1] == '"')
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
		  /*[in]*/ const char *	lpszKey)
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
			"Path",
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
			    "\\Control\\Session Manager\\Environment")))
	{
	  return (true);
	}
      if (IsPathRegistered(HKEY_CURRENT_USER, "Environment"))
			   
	{
	  return (true);
	}
    }
  else
    {
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
      string value;
      if (Utils::GetEnvironmentString("PATH", value))
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
      ret = theApp.GetInstallRoot();
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
  MIKTEX_PRODUCTNAME_STR " " MIKTEX_VERSION_STR

#define UNINST_DISPLAY_NAME_MIKTEXDIRECT \
  "MiKTeXDirect" " " MIKTEX_VERSION_STR

#define UNINST_REG_PATH							\
    (theApp.setupTask == SetupTask::PrepareMiKTeXDirect			\
     ? REGSTR_PATH_UNINSTALL "\\" UNINST_DISPLAY_NAME_MIKTEXDIRECT	\
     : REGSTR_PATH_UNINSTALL "\\" UNINST_DISPLAY_NAME)

#define UNINST_HKEY_ROOT			\
  (theApp.commonUserSetup			\
   ? HKEY_LOCAL_MACHINE				\
   : HKEY_CURRENT_USER)

void
AddUninstallerRegValue (/*[in]*/ HKEY			hkey,
			/*[in]*/ const char *	lpszValueName,
			/*[in]*/ const char *	lpszValue)
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
      FATAL_WINDOWS_ERROR_2 ("RegSetValueEx", result, 0);
    }
  
  ULogAddRegValue (UNINST_HKEY_ROOT, UNINST_REG_PATH, lpszValueName);
}

void
AddUninstallerRegValue (/*[in]*/ HKEY			hkey,
			/*[in]*/ const char *	lpszValueName,
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
      FATAL_WINDOWS_ERROR_2 ("RegSetValueEx", result, 0);
    }
  
  ULogAddRegValue (UNINST_HKEY_ROOT, UNINST_REG_PATH, lpszValueName);
}

/* _________________________________________________________________________

   RegisterUninstaller
   _________________________________________________________________________ */

#define UNINST_HELP_LINK "http://miktex.org/support"
#define UNINST_PUBLISHER MIKTEX_COMP_COMPANY_STR
#define UNINST_DISPLAY_VERSION MIKTEX_VERSION_STR
#define UNINST_DISPLAY_STRING				\
  (theApp.setupTask == SetupTask::PrepareMiKTeXDirect	\
   ? UNINST_DISPLAY_NAME_MIKTEXDIRECT			\
   : UNINST_DISPLAY_NAME)
#define UNINST_ABOUT_URL "http://miktex.org/about"
#define UNINST_UPDATE_URL "http://miktex.org"
#define UNINST_COMMENT T_("Uninstall MiKTeX")
#define UNINST_README MIKTEX_URL_WWW_KNOWN_ISSUES

void
RegisterUninstaller ()
{
  // make uninstall command line
  string commandLine;
  if (theApp.setupTask != SetupTask::PrepareMiKTeXDirect)
    {
      PathName pathCopyStart (theApp.GetInstallRoot(),
			      (theApp.commonUserSetup
			       ? MIKTEX_PATH_INTERNAL_COPYSTART_ADMIN_EXE
			       : MIKTEX_PATH_INTERNAL_COPYSTART_EXE));
      commandLine += Q_(pathCopyStart.Get());
      commandLine += " ";
    }
  PathName pathUninstallDat (theApp.GetInstallRoot(),
			     (theApp.commonUserSetup
			      ? MIKTEX_PATH_INTERNAL_UNINSTALL_ADMIN_EXE
			      : MIKTEX_PATH_INTERNAL_UNINSTALL_EXE));
  commandLine += Q_(pathUninstallDat.Get());

  // make icon path
  PathName iconPath (theApp.GetInstallRoot());
  iconPath += MIKTEX_PATH_BIN_DIR;
  iconPath += MIKTEX_MO_EXE;
  iconPath.Append (",0", false);

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
      FATAL_WINDOWS_ERROR_2 ("RegCreateKeyEx", result, 0);
    }
  AutoHKEY autoHKEY (hkey);
  
  // set values
  PathName installRoot (theApp.GetInstallRoot());
  AddUninstallerRegValue (hkey, "Comment", UNINST_COMMENT);
  AddUninstallerRegValue (hkey, "DisplayIcon", iconPath.Get());
  AddUninstallerRegValue (hkey, "DisplayName", UNINST_DISPLAY_STRING); 
  AddUninstallerRegValue (hkey, "DisplayVersion", UNINST_DISPLAY_VERSION);
  AddUninstallerRegValue (hkey, "HelpLink", UNINST_HELP_LINK);
  AddUninstallerRegValue (hkey, "InstallLocation", installRoot.Get());
  AddUninstallerRegValue (hkey, "NoModify", 1);
  AddUninstallerRegValue (hkey, "NoRepair", 1);
  AddUninstallerRegValue (hkey, "Publisher", UNINST_PUBLISHER);
  AddUninstallerRegValue (hkey, "Readme", UNINST_README);
  AddUninstallerRegValue (hkey, "UninstallString", commandLine.c_str());
  AddUninstallerRegValue (hkey, "UrlInfoAbout", UNINST_ABOUT_URL);
  AddUninstallerRegValue (hkey, "UrlUpdateInfo", UNINST_UPDATE_URL);
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
	  if (! theApp.portable)
	  {
	    RegisterUninstaller ();
	  }
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
      theApp.uninstStream.WriteLine ("[files]");
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
      theApp.uninstStream.WriteLine ("[hklm]");
      section = HKLM;
    }
  else if (hkey == HKEY_CURRENT_USER && section != HKCU)
    {
      theApp.uninstStream.WriteLine ("[hkcu]");
      section = HKCU;
    }
  theApp.uninstStream.WriteFormattedLine
    ("%s;%s", lpszSubKey, lpszValueName);
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
      pathBinDir.Set (theApp.GetInstallRoot(), MIKTEX_PATH_BIN_DIR);
    }

  Log (T_("Registering bin dir: %s\n"), Q_(pathBinDir));

#define REGSTR_KEY_ENVIRONMENT_COMMON \
   REGSTR_PATH_CURRENTCONTROLSET "\\Control\\Session Manager\\Environment"
#define REGSTR_KEY_ENVIRONMENT_USER "Environment"

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
      FATAL_WINDOWS_ERROR_2 ("RegOpenKey", result, 0);
    }

  AutoHKEY autoHKEY (hkey);

  DWORD type;
  CharBuffer<char> value (32 * 1024);
  DWORD valueSize = static_cast<DWORD>(value.GetCapacity());

  result =
    RegQueryValueEx(hkey,
		    "Path",
		    0,
		    &type,
		    reinterpret_cast<LPBYTE>(value.GetBuffer()),
		    &valueSize);
 
  bool havePath = (result == ERROR_SUCCESS);

  if (! havePath)
    {
      if (result != ERROR_FILE_NOT_FOUND)
	{
	  FATAL_WINDOWS_ERROR_2 ("RegQueryValueEx", result, 0);
	}
    }

  string newPath (pathBinDir.Get());

  if (havePath)
    {
      newPath += ';';
      newPath += value.Get();
    }

  result =
    RegSetValueEx(hkey,
		  "Path",
		  0,
		  (havePath ? type : REG_SZ),
		  reinterpret_cast<const BYTE *>(newPath.c_str()),
		  static_cast<DWORD>(STR_BYT_SIZ(newPath.c_str())));

  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 ("RegSetValueEx", result, 0);
    }

  DWORD sendMessageResult;

  if (SendMessageTimeout(HWND_BROADCAST,
			 WM_SETTINGCHANGE,
			 0,
			 reinterpret_cast<LPARAM>("Environment"),
			 SMTO_ABORTIFHUNG,
			 5000,
			 &sendMessageResult)
      == 0)
    {
      CHECK_WINDOWS_ERROR ("SendMessageTimeout", 0);
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
      pathBinDir.Set (theApp.GetInstallRoot(), MIKTEX_PATH_BIN_DIR);
    }
  Utils::RemoveBlanksFromPathName (pathBinDir);
  StreamWriter writer (File::Open("c:\\autoexec.bat", // <fixme/>
				  FileMode::Append,
				  FileAccess::Write));
  writer.WriteFormattedLine ("set PATH=%s;%%PATH%%", pathBinDir.Get());
  writer.Close ();
  Log ("set PATH=%s;%%PATH%%\n", pathBinDir.Get());
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

   Expand
   _________________________________________________________________________ */

CString &
Expand (/*[in]*/ const char *	lpszSource,
	/*[out]*/ CString &	dest)
{
  dest = lpszSource;
  int pos;
  while ((pos = dest.Find("%MIKTEX_INSTALL%")) >= 0)
    {
      dest.Delete (pos, 16);
      dest.Insert (pos, theApp.GetInstallRoot().Get());
    }
  return (dest);
}

/* _________________________________________________________________________

   CreateInternetShortcut
   _________________________________________________________________________ */

void
CreateInternetShortcut (/*[in]*/ const PathName &	path,
			/*[in]*/ const char *	lpszUrl)
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
      UNEXPECTED_CONDITION ("CreateInternetShortcut");
    }

  hr = pURL->SetURL(lpszUrl, 0);

  if (FAILED(hr))
    {
      Log (T_("IUniformResourceLocator::SetURL() failed (%08x)\n"), hr);
      UNEXPECTED_CONDITION ("CreateInternetShortcut");
    }

  IPersistFilePtr pPF;

  hr = pURL->QueryInterface(IID_IPersistFile, reinterpret_cast<void **>(&pPF));

  if (FAILED(hr))
    {
      Log (T_("IPersistFile could not be created (%08x)\n"), hr);
      UNEXPECTED_CONDITION ("CreateInternetShortcut");
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
      FATAL_WINDOWS_ERROR ("MultiByteToWideChar", 0);
    }
  hr = pPF->Save(wszPath, TRUE);
#endif

  if (FAILED(hr))
    {
      Log (T_("IPersistFile::Save() failed (%08x)\n"), hr);
      UNEXPECTED_CONDITION ("CreateInternetShortcut");
    }
}

/* _________________________________________________________________________

   CreateShellLink
   _________________________________________________________________________ */

void
CreateShellLink (/*[in]*/ const PathName &		pathFolder,
		 /*[in]*/ const ShellLinkData &		ld)
{
  if ((ld.flags & LD_IFCOMMON) != 0 && ! theApp.commonUserSetup)
  {
    // ignore system-wide command if this is a per-user setup
    return;
  }

  PathName pathLink;

  if (ld.subFolderID > 0)
    {
      CString subFolder;
      if (! subFolder.LoadString(ld.subFolderID))
	{
	  UNEXPECTED_CONDITION ("CreateShellLink");
	}
      PathName pathSubFolder (pathFolder,
			      static_cast<const char *>(subFolder));
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
      UNEXPECTED_CONDITION ("CreateShellLink");
    }

  pathLink += strItemName;
  pathLink.SetExtension (ld.isUrl ? ".url" : ".lnk");

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
	  UNEXPECTED_CONDITION ("CreateShellLink");
	}
      
      CString str;
      
      hr = psl->SetPath(Expand(ld.lpszPathName, str));

      if (FAILED(hr))
	{
	  Log (T_("IShellLink::SetPath() failed (%08x)\n"), hr);
	  UNEXPECTED_CONDITION ("CreateShellLink");
	}
      
      if ((ld.flags & LD_USEARGS) != 0)
	{
	  hr = psl->SetArguments(Expand(ld.lpszArgs, str));
	  if (FAILED(hr))
	    {
	      Log (T_("IShellLink::SetArguments() failed (%08x)\n"), hr);
	      UNEXPECTED_CONDITION ("CreateShellLink");
	    }
	}
      
      if ((ld.flags & LD_USEDESC) != 0)
	{
	  hr = psl->SetDescription(ld.lpszDescription);
	  if (FAILED(hr))
	    {
	      Log (T_("IShellLink::SetDescription() failed (%08x)\n"), hr);
	      UNEXPECTED_CONDITION ("CreateShellLink");
	    }
	}
      
      if ((ld.flags & LD_USEICON) != 0)
	{
	  hr =
	    psl->SetIconLocation(Expand(ld.lpszIconPath, str), ld.iconIndex);
	  if (FAILED(hr))
	    {
	      Log (T_("IShellLink::SetIconLocation() failed (%08x)\n"), hr);
	      UNEXPECTED_CONDITION ("CreateShellLink");
	    }
	}
      
      if ((ld.flags & LD_USEWORKDIR) != 0)
	{
	  hr = psl->SetWorkingDirectory(Expand(ld.lpszWorkingDir, str));
	  if (FAILED(hr))
	    {
	      Log (T_("IShellLink::SetWorkingDirectory() failed (%08x)\n"),
		   hr);
	      UNEXPECTED_CONDITION ("CreateShellLink");
	    }
	}

      if ((ld.flags & LD_USESHOWCMD) != 0)
	{
	  hr = psl->SetShowCmd(ld.showCmd);
	  if (FAILED(hr))
	    {
	      Log (T_("IShellLink::SetShowCmd() failed (%08x)\n"), hr);
	      UNEXPECTED_CONDITION ("CreateShellLink");
	    }
	}
      
      if ((ld.flags & LD_USEHOTKEY) != 0)
	{
	  hr = psl->SetHotkey (ld.hotKey);
	  if (FAILED(hr))
	    {
	      Log (T_("IShellLink::SetHotkey() failed (%08x)\n"), hr);
	      UNEXPECTED_CONDITION ("CreateShellLink");
	    }
	}
      
      IPersistFilePtr ppf;

      hr =
	psl->QueryInterface(IID_IPersistFile, reinterpret_cast<void **>(&ppf));

      if (FAILED(hr))
	{
	  Log (T_("IPersistFile could not be created (%08x)\n"), hr);
	  UNEXPECTED_CONDITION ("CreateShellLink");
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
	  FATAL_WINDOWS_ERROR ("MultiByteToWideChar", 0);
	}
      hr = ppf->Save(wszPath, TRUE);
#endif

      if (FAILED(hr))
	{
	  Log (T_("IShellLink::Save() failed (%08x)\n"), hr);
	  UNEXPECTED_CONDITION ("CreateShellLink");
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
LogV (/*[in]*/ const char *	lpszFormat,
      /*[in]*/ va_list		argList)
{
  CSingleLock singleLock (&theApp.criticalSectionMonitorLogStream, TRUE);
  string formatted = Utils::FormatString(lpszFormat, argList);
  static string currentLine;
  for (const char * lpsz = formatted.c_str();
       *lpsz != 0;
       ++ lpsz)
    {
      if (lpsz[0] == '\n'
	  || (lpsz[0] == '\r' && lpsz[1] == '\n'))
	{
	  theApp.traceStream->WriteFormattedLine ("setup",
						  "%s",
						  currentLine.c_str());
	  if (theApp.logStream.IsOpen())
	    {
	      theApp.logStream.WriteLine (currentLine);
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
}

/* _________________________________________________________________________

   Log
   _________________________________________________________________________ */

void
Log (/*[in]*/ const char *	lpszFormat,
     /*[in]*/			...)
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
      UNEXPECTED_CONDITION ("LogHeader");
    }
  Log (T_("%s %s Report\n\n"),
       static_cast<const char *>(banner),
       MIKTEX_COMPONENT_VERSION_STR);
  CTime t = CTime::GetCurrentTime();
  Log (T_("Date: %s\n"), t.Format(T_("%A, %B %d, %Y")));
  Log (T_("Time: %s\n"), t.Format(T_("%H:%M:%S")));
  Log (T_("OS version: %s\n"), Utils::GetOSVersionString().c_str());
  if (IsWindowsNT())
    {
      Log ("SystemAdmin: %s\n",
	   (SessionWrapper(true)->RunningAsAdministrator()
	    ? "yes"
	    : "false"));
      Log ("PowerUser: %s\n",
	   (SessionWrapper(true)->RunningAsPowerUser()
	    ? "yes"
	    : "false"));
    }
  if (theApp.setupTask != SetupTask::Download)
    {
      Log ("SharedSetup: %s\n",
	   (theApp.commonUserSetup ? "yes" : "false"));
    }
  Log (T_("Setup path: %s\n"), theApp.setupPath.Get());
  if (theApp.setupTask != SetupTask::Download)
    {
      Log ("UserRoots: %s\n",
	  (theApp.startupConfig.userRoots.empty()
	    ? T_("<none specified>")
	    : theApp.startupConfig.userRoots.c_str()));
      Log ("UserData: %s\n",
	   (theApp.startupConfig.userDataRoot.Empty()
	    ? T_("<none specified>")
	    : theApp.startupConfig.userDataRoot.Get()));
      Log ("UserConfig: %s\n",
	   (theApp.startupConfig.userConfigRoot.Empty()
	    ? T_("<none specified>")
	    : theApp.startupConfig.userConfigRoot.Get()));
      Log ("CommonRoots: %s\n",
	  (theApp.startupConfig.commonRoots.empty()
	    ? T_("<none specified>")
	    : theApp.startupConfig.commonRoots.c_str()));
      Log ("CommonData: %s\n",
	   (theApp.startupConfig.commonDataRoot.Empty()
	    ? T_("<none specified>")
	    : theApp.startupConfig.commonDataRoot.Get()));
      Log ("CommonConfig: %s\n",
	   (theApp.startupConfig.commonConfigRoot.Empty()
	    ? T_("<none specified>")
	    : theApp.startupConfig.commonConfigRoot.Get()));
      Log ("\nInstallation: %s\n", theApp.GetInstallRoot().Get());
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
      FATAL_WINDOWS_ERROR ("GetFileVersionInfoSize", path.Get());
    }
  CharBuffer<char> buf (cchver);
  if (! GetFileVersionInfo(path.Get(), dwHandle, cchver, buf.GetBuffer()))
    {
      FATAL_WINDOWS_ERROR ("GetFileVersionInfo", path.Get());
    }
  UINT uLen;
  void * pVer;
  if (! VerQueryValue(buf.GetBuffer(),
		      "\\",
		      &pVer,
		      &uLen))
    {
      FATAL_WINDOWS_ERROR ("VerQueryValue", path.Get());
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
  if (isalpha(str[0]) && str[1] == ':' && IsDirectoryDelimiter(str[2]))
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
  int i = str2.FindOneOf(":*?\"<>|;=");
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
      string str;
      str =
	T_("The operation could not be completed for the following reason: ");
      str += "\n\n";
      str += e.what();
      Log ("\n%s\n", str.c_str());
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
SplitUrl (/*[in]*/ const string &	url,
	  /*[out]*/ string &		protocol,
	  /*[out]*/ string &		host)
{
  char szProtocol[200];
  char szHost[200];
  URL_COMPONENTS url_comp = { 0 };
  url_comp.dwStructSize = sizeof(url_comp);
  url_comp.lpszScheme = szProtocol;
  url_comp.dwSchemeLength = 200;
  url_comp.lpszHostName = szHost;
  url_comp.dwHostNameLength = 200;
  if (! InternetCrackUrl(url.c_str(), 0, 0, &url_comp))
    {
      FATAL_WINDOWS_ERROR ("InternetCrackUrl", 0);
    }
  protocol = szProtocol;
  host = szHost;
}
