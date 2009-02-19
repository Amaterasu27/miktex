/* mpm.cpp: MiKTeX Package Manager (console version)

   Copyright (C) 2003-2009 Christian Schenk

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

#if ! defined(THE_NAME_OF_THE_GAME)
#  define THE_NAME_OF_THE_GAME T_("MiKTeX Package Manager")
#endif

#if defined(MIKTEX_WINDOWS)
#  define ENABLE_OPT_INSTALL_ROOT 0
#else
#  define ENABLE_OPT_INSTALL_ROOT 1
#endif

#define MYPKG "miktex-bin"

#if defined(MIKTEX_WINDOWS)
const char PATH_DELIMITER = ';';
#define PATH_DELIMITER_STRING ";"
#else
const char PATH_DELIMITER = ':';
#define PATH_DELIMITER_STRING ":"
#endif

const char * DEFAULT_TRACE_STREAMS =
  MIKTEX_TRACE_CORE ","
  MIKTEX_TRACE_CURL ","
  MIKTEX_TRACE_ERROR ","
  MIKTEX_TRACE_FNDB ","
  MIKTEX_TRACE_MPM
  ;

/* _________________________________________________________________________

   OutputFormat
   _________________________________________________________________________ */

class OutputFormatEnum
{
public:
  enum EnumType {
    None = 0,
    Listing,
    CSV,
    DeploymentNames,
  };
};

typedef EnumWrapper<OutputFormatEnum> OutputFormat;

/* _________________________________________________________________________

   SortKey
   _________________________________________________________________________ */

class SortKeyEnum
{
public:
  enum EnumType {
    None = 0,
    DeploymentName,
    InstalledOn,
    PackagedOn,
  };
};

typedef EnumWrapper<SortKeyEnum> SortKey;

/* _________________________________________________________________________

   PackageInfoComparer
   _________________________________________________________________________ */

class PackageInfoComparer
{
public:
  bool
  operator() (/*[in]*/ const PackageInfo & pi1,
	      /*[in]*/ const PackageInfo & pi2)
    const
  {
    bool cmp;
    switch (sortKey.Get())
      {
      case SortKey::DeploymentName:
	cmp = (PathName::Compare(pi1.deploymentName, pi2.deploymentName) < 0);
	break;
      case SortKey::InstalledOn:
	cmp = (pi1.timeInstalled < pi2.timeInstalled);
	break;
      case SortKey::PackagedOn:
	cmp = (pi1.timePackaged < pi2.timePackaged);
	break;
      default:
	cmp = false;
	break;
      }
    return (reverse ? ! cmp : cmp);
  }
public:
  static SortKey sortKey;
public:
  static bool reverse;
};

SortKey PackageInfoComparer::sortKey (SortKey::DeploymentName);
bool PackageInfoComparer::reverse = false;

/* _________________________________________________________________________

   UpdateInfoComparer
   _________________________________________________________________________ */

class UpdateInfoComparer
{
public:
  bool
  operator() (/*[in]*/ const PackageInstaller::UpdateInfo & ui1,
	      /*[in]*/ const PackageInstaller::UpdateInfo & ui2)
    const
  {
    return (PathName::Compare(ui1.deploymentName, ui2.deploymentName) < 0);
  }
};

/* _________________________________________________________________________

   Application
   _________________________________________________________________________ */

class Application
  : public PackageInstallerCallback
{
private:
  void
  InstallSignalHandler (/*[in]*/ int sig)
  {
    void (* oldHandlerFunc) (int);
    oldHandlerFunc = signal(sig, Application::SignalHandler);
    if (oldHandlerFunc == SIG_ERR)
      {
	Error (T_("signal() failed for some reason."));
      }
    if (oldHandlerFunc != SIG_DFL)
      {
	if (signal(sig, oldHandlerFunc) == SIG_ERR)
	  {
	    Error (T_("signal() failed for some reason."));
	  }
      }
  }

public:
  Application ()
    : verbose (false),
      quiet (false)
  {
    InstallSignalHandler (SIGINT);
    InstallSignalHandler (SIGTERM);
  }

  // PackageInstallerCallback impl
public:
  virtual
  void
  MIKTEXTHISCALL
  ReportLine (/*[in]*/ const char * lpszLine);

  // PackageInstallerCallback impl
public:
  virtual
  bool
  MIKTEXTHISCALL
  OnRetryableError (/*[in]*/ const char * lpszMessage);

  // PackageInstallerCallback impl
public:
  virtual
  bool
  MIKTEXTHISCALL
  OnProgress (/*[in]*/ Notification	nf);

public:
  void
  Main (/*[in]*/ int			argc,
	/*[in]*/ const char **	argv);

private:
  void
  Verbose (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);

private:
  void
  Message (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);

private:
  MIKTEXNORETURN
  void
  Error (/*[in]*/ const char *	lpszFormat,
	 /*[in]*/			...);

private:
  void
  UpdateDb ();

private:
  void
  Install (/*[in]*/ const vector<string> &	toBeInstalled,
	   /*[in]*/ const vector<string> &	toBeRemoved);

#if defined(MIKTEX_WINDOWS)
private:
  void
  RegisterComponents (/*[in]*/ bool doRegister);
#endif

private:
  void
  Verify (/*[in]*/ const vector<string> &	toBeVerified);

private:
  void
  VerifyMiKTeX ();

private:
  void
  FindConflicts ();

private:
  void
  ImportPackage (/*[in]*/ /*[in]*/ const string & deploymentName,
		 /*[in,out]*/ vector<string> &	toBeinstalled);

private:
  void
  ImportPackages (/*[in,out]*/ vector<string> & toBeinstalled);

private:
  void
  FindUpdates ();

private:
  void
  Update (/*[in]*/ const vector<string> &	updates);

private:
  string
  GetDirectories (/*[in]*/ const string &	deploymentName);

private:
  void
  List (/*[in]*/ OutputFormat	outputFormat,
	/*[in]*/ int		maxCount);

private:
  void
  ListRepositories (/*[in]*/ OutputFormat outputFormat);

private:
  void
  PickRepositoryUrl ();

private:
  void
  PrintFiles (/*[in]*/ const vector<string> & files);

private:
  void
  PrintPackageInfo (/*[in]*/ const string & deploymentName);

private:
  void
  RestartWindowed ();

private:
  void
  ReadFileList (/*[in]*/ const PathName &	path,
		/*[in,out]*/ vector<string> &	files);

private:
  static
  void
  SignalHandler (/*[in]*/ int sig);

private:
  PackageManagerPtr pPackageManager;

private:
  SessionWrapper pSession;

private:
  static const struct poptOption aoption[];

private:
  static volatile sig_atomic_t interrupted;

private:
  bool verbose;

private:
  bool quiet;

private:
  string repository;
};

/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 1,
  OPT_ADMIN,
  OPT_CSV,			// deprecated
  OPT_FIND_CONFLICTS,		// internal
  OPT_FIND_UPDATES,
  OPT_HHELP,
  OPT_IMPORT,
  OPT_IMPORT_ALL,
  OPT_INSTALL,
  OPT_INSTALL_ROOT,		// deprecated
  OPT_INSTALL_SOME,
  OPT_LIST,
  OPT_LIST_PACKAGE_NAMES,
  OPT_LIST_REPOSITORIES,
  OPT_MAX_COUNT,		// experimental
  OPT_OUTPUT_FORMAT,		// experimental
  OPT_PICK_REPOSITORY_URL,
  OPT_PRINT_PACKAGE_INFO,
  OPT_PROXY,			// experimental
  OPT_PROXY_PASSWORD,		// experimental
  OPT_PROXY_USER,		// experimental
  OPT_QUIET,
  OPT_REGISTER_COMPONENTS,	// experimental
  OPT_REPOSITORY,
  OPT_REVERSE,			// experimental
  OPT_SET_REPOSITORY,
  OPT_SORT,			// experimental
  OPT_TRACE,
  OPT_UNINSTALL,
  OPT_UNREGISTER_COMPONENTS,	// experimental
  OPT_UPDATE,
  OPT_UPDATE_ALL,		// experimental
  OPT_UPDATE_DB,
  OPT_UPDATE_FNDB,		// experimental
  OPT_UPDATE_SOME,
  OPT_VERBOSE,
  OPT_VERIFY,
  OPT_VERIFY_MIKTEX,
  OPT_VERSION,
};

/* _________________________________________________________________________

   Application::aoption
   _________________________________________________________________________ */

const struct poptOption Application::aoption[] = {

  {
    "admin", 0, POPT_ARG_NONE, 0, OPT_ADMIN,
    T_("Run in administration mode."), 0,
  },

  {				// deprecated
    "csv", 0, POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0, OPT_CSV,
    T_("Output comma-separated value lists."), 0,
  },

  {				// internal
    "find-conflicts", 0, POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN,
    0, OPT_FIND_CONFLICTS,
    T_("Find file conflicts."), 0,
  },

  {
    "find-updates", 0, POPT_ARG_NONE, 0, OPT_FIND_UPDATES,
    T_("Test the package repository for updates, then print the list of\
 updateable packages."), 0,
  },

#if defined(MIKTEX_WINDOWS)
  {
    "hhelp", 0, POPT_ARG_NONE, 0, OPT_HHELP,
    T_("Show the manual page in an HTMLHelp window and \
exit when the window is closed."), 0
  },
#endif

  {
    "import", 0, POPT_ARG_STRING, 0, OPT_IMPORT,
    T_("Import the specified package from another MiKTeX installation."),
    T_("PACKAGE")
  },

  {
    "import-all", 0, POPT_ARG_NONE, 0, OPT_IMPORT_ALL,
    T_("Import all installed packages from another MiKTeX installation."),
    0
  },

  {
    "install", 0, POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0, OPT_INSTALL,
    T_("Install the specified package."),
    T_("PACKAGE")
  },

#if ENABLE_OPT_INSTALL_ROOT
  {				// deprecated
    "install-root", 0, POPT_ARG_STRING, 0, OPT_INSTALL_ROOT,
    T_("\
Use the specified directory as the installation destination."),
    T_("DIR")
  },
#endif

  {
    "install-some", 0, POPT_ARG_STRING, 0, OPT_INSTALL_SOME,
    T_("Install packages listed (line-by-line) in the specified file."),
    T_("FILE")
  },

  {
    "list", 0, POPT_ARG_NONE, 0, OPT_LIST,
    T_("List the contents of the package database:\
 for each package, print the installation status, the number of files,\
 the size, and the name."),
    0
  },

  {
    "list-package-names", 0, POPT_ARG_NONE, 0, OPT_LIST_PACKAGE_NAMES,
    T_("List the package names."), 0
  },

  {
    "list-repositories", 0, POPT_ARG_NONE, 0, OPT_LIST_REPOSITORIES,
    T_("\
Download the list of known package repository URLs from\
 the MiKTeX project server, then print the list."),
    0
  },

  {
    "max-count", 0, POPT_ARG_STRING, 0, OPT_MAX_COUNT,
    T_("Stop after NUM packages."),
    T_("NUM")
  },

  {				// experimental
    "output-format", 0, POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_OUTPUT_FORMAT,
    T_("Set the output format."),
    T_("FORMAT"),
  },
    
  {
    "pick-repository-url", 0, POPT_ARG_NONE, 0, OPT_PICK_REPOSITORY_URL,
    T_("\
Pick a suitable package repository URL and print it."),
    0
  },

  {
    "print-package-info", 0, POPT_ARG_STRING, 0, OPT_PRINT_PACKAGE_INFO,
    T_("Print detailed information about the specified package."),
    T_("PACKAGE")
  },

  {				// experimental
    "proxy", 0, POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0, OPT_PROXY,
    T_("Use the specified proxy host[:port]."),
    T_("HOST[:PORT]")
  },

  {				// experimental
    "proxy-password", 0, POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_PROXY_PASSWORD,
    T_("Use the specified password for proxy authentication."),
    T_("PASSWORD")
  },

  {				// experimental
    "proxy-user", 0, POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_PROXY_USER,
    T_("Use the specified user for proxy authentication."),
    T_("USER")
  },

  {				// experimental
    "sort", 0, POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_SORT,
    T_("Sort the package list."),
    T_("KEY")
  },

  {
    "quiet", 0, POPT_ARG_NONE, 0, OPT_QUIET,
    T_("Suppress all output (except errors)."), 0
  },

#if defined(MIKTEX_WINDOWS)
  {
    "register-components", 0, POPT_ARG_NONE, 0, OPT_REGISTER_COMPONENTS,
    T_("Register COMponents."), 0
  },
#endif

  {
    "repository", 0, POPT_ARG_STRING, 0, OPT_REPOSITORY,
    T_("\
Use the specified location as the package repository.\
  The location can be either a fully qualified path name\
 (a local package repository) or an URL (a remote package repository)."),
    T_("LOCATION")
  },

  {
    "reverse", 0, POPT_ARG_NONE, 0, OPT_REVERSE,
    T_("\
Reverse the result of comparisons."),
    0
  },

#if defined(MIKTEX_WINDOWS)
  {
    "set-repository", 0, POPT_ARG_STRING, 0, OPT_SET_REPOSITORY,
    T_("Register the location of the default package repository.\
  The location can be either a fully qualified path name\
 (a local package repository) or an URL (a remote package repository)."),
    T_("LOCATION")
  },
#endif

  {
    "trace", 0, POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0, OPT_TRACE,
    T_("\
Turn on tracing.\
  TRACESTREAMS, if specified, is a comma-separated list of trace stream names\
 (see the MiKTeX manual)."),
    T_("TRACESTREAMS")
  },

  {
    "uninstall", 0, POPT_ARG_STRING, 0, OPT_UNINSTALL,
    T_("Uninstall the specified package."),
    T_("PACKAGE")
  },

#if defined(MIKTEX_WINDOWS)
  {
    "unregister-components", 0, POPT_ARG_NONE, 0,
    OPT_UNREGISTER_COMPONENTS,
    T_("Unregister COMponents."),
    0
  },
#endif

  {
    "update", 0, POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0, OPT_UPDATE,
    T_("Update the specified package, if an updated version\
 is available in the package repository.  Install all updateable\
 packages, if the package name is omitted."),
    T_("PACKAGE")
  },

  {				// experimental
    "update-all", 0, POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN,
    0, OPT_UPDATE_ALL,
    T_("Test the package repository for updates, then install\
 all updateable packages."),
    0,
  },

  {
    "update-db", 0, POPT_ARG_NONE, 0, OPT_UPDATE_DB,
    T_("Synchronize the local package database with\
 the package repository."), 0
  },

  {				// experimental
    "update-fndb", 0, POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN,
    0, OPT_UPDATE_FNDB,
    T_("Update mpm.fndb."),
    0
  },

  {
    "update-some", 0, POPT_ARG_STRING, 0, OPT_UPDATE_SOME,
    T_("Update packages listed (line-by-line) in the specified file."),
    T_("FILE")
  },

  {
    "verbose", 0, POPT_ARG_NONE, 0, OPT_VERBOSE,
    T_("Turn on verbose output mode."), 0
  },

  {
    "verify", 0, POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL,
    0, OPT_VERIFY,
    T_("Verify the integrity of the installed packages."),
    T_("PACKAGE")
  },

  {				// experimental
    "verify-miktex", 0, POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN,
    0, OPT_VERIFY_MIKTEX,
    T_("Verify the integrity of the installed MiKTeX packages."),
    0
  },

  {
    "version", 0, POPT_ARG_NONE, 0, OPT_VERSION,
    T_("Show version information and exit."),
    0
  },

  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   Application::interrupted
   _________________________________________________________________________ */

volatile sig_atomic_t Application::interrupted = false;

/* _________________________________________________________________________

   Application::Message
   _________________________________________________________________________ */

void
Application::Message (/*[in]*/ const char *	lpszFormat,
		      /*[in]*/				...)
{
  if (quiet)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  cout << Utils::FormatString(lpszFormat, arglist);
  va_end (arglist);
}

/* _________________________________________________________________________

   Application::Verbose
   _________________________________________________________________________ */

void
Application::Verbose (/*[in]*/ const char *	lpszFormat,
		      /*[in]*/				...)
{
  if (! verbose)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  cout << Utils::FormatString(lpszFormat, arglist);
  va_end (arglist);
}

/* _________________________________________________________________________

   Application::Error
   _________________________________________________________________________ */

MIKTEXNORETURN
void
Application::Error (/*[in]*/ const char *		lpszFormat,
		    /*[in]*/				...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  cerr << "mpm" << ": "
       << Utils::FormatString(lpszFormat, arglist) << endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________

   Application::ReportLine
   _________________________________________________________________________ */

void
Application::ReportLine (/*[in]*/ const char * lpszLine)
{
  Verbose ("%s\n", lpszLine);
}

/* _________________________________________________________________________

   Application::OnRetryableError
   _________________________________________________________________________ */

bool
Application::OnRetryableError (/*[in]*/ const char * lpszMessage)
{
  lpszMessage;
  return (false);
}

/* _________________________________________________________________________

   Application::OnProgress
   _________________________________________________________________________ */

bool
Application::OnProgress (/*[in]*/ Notification		nf)
{
  nf;
  return (interrupted == 0);
}

/* _________________________________________________________________________

   Application::UpdateDb
   _________________________________________________________________________ */

void
Application::UpdateDb ()
{
  auto_ptr<PackageInstaller> pInstaller (pPackageManager->CreateInstaller());
  if (! repository.empty())
    {
      pInstaller->SetRepository (repository);
    }
  pInstaller->SetCallback (this);
  pInstaller->UpdateDb ();
  pInstaller->Dispose ();
}

/* _________________________________________________________________________

   Application::Install
   _________________________________________________________________________ */

void
Application::Install (/*[in]*/ const vector<string> &	toBeInstalled,
		      /*[in]*/ const vector<string> &	toBeRemoved)
{
  for (vector<string>::const_iterator it = toBeInstalled.begin();
       it != toBeInstalled.end();
       ++ it)
    {
      PackageInfo packageInfo = pPackageManager->GetPackageInfo(*it);
      if (packageInfo.IsInstalled())
	{
	  Error (T_("Package \"%s\" is already installed."), it->c_str());
	}
    }

  for (vector<string>::const_iterator it = toBeRemoved.begin();
       it != toBeRemoved.end();
       ++ it)
    {
      PackageInfo packageInfo = pPackageManager->GetPackageInfo(*it);
      if (! packageInfo.IsInstalled())
	{
	  Error (T_("Package \"%s\" is not installed."), it->c_str());
	}
    }
       
  auto_ptr<PackageInstaller> pInstaller (pPackageManager->CreateInstaller());

  if (! repository.empty())
    {
      pInstaller->SetRepository (repository);
    }

  pInstaller->SetCallback (this);
  pInstaller->SetFileLists (toBeInstalled, toBeRemoved);
  pInstaller->InstallRemove ();
  pInstaller->Dispose ();
  if (toBeInstalled.size() == 1)
    {
      Message (T_("Package \"%s\" has been successfully installed.\n"),
	       toBeInstalled[0].c_str());
    }
  else if (toBeInstalled.size() > 1)
    {
      Message (T_("%u packages have been successfully installed.\n"),
	       toBeInstalled.size());
    }
  if (toBeRemoved.size() == 1)
    {
      Message (T_("Package \"%s\" has been successfully removed.\n"),
	       toBeRemoved[0].c_str());
    }
  else if (toBeRemoved.size() > 1)
    {
      Message (T_("%u packages have been successfully removed.\n"),
	       toBeRemoved.size());
    }
}

/* _________________________________________________________________________

   Application::RegisterComponents
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)

void
Application::RegisterComponents (/*[in]*/ bool doRegister)
{
  auto_ptr<PackageInstaller> pInstaller (pPackageManager->CreateInstaller());
  pInstaller->SetCallback (this);
  pInstaller->RegisterComponents (doRegister);
  pInstaller->Dispose ();
}

#endif

/* _________________________________________________________________________

   Application::FindConflicts
   _________________________________________________________________________ */

void
Application::FindConflicts ()
{
  map<string, vector<string> > filesAndPackages;
  auto_ptr<PackageIterator> pIter (pPackageManager->CreateIterator());
  PackageInfo packageInfo;
  for (int idx = 0; pIter->GetNext(packageInfo); ++ idx)
    {
      for (vector<string>::const_iterator it = packageInfo.runFiles.begin();
	   it != packageInfo.runFiles.end();
	   ++ it)
	{
	  PathName file (*it);
	  file.Normalize ();
	  filesAndPackages[file.Get()].push_back (packageInfo.deploymentName);
	}
      for (vector<string>::const_iterator it = packageInfo.docFiles.begin();
	   it != packageInfo.docFiles.end();
	   ++ it)
	{
	  PathName file (*it);
	  file.Normalize ();
	  filesAndPackages[file.Get()].push_back (packageInfo.deploymentName);
	}
      for (vector<string>::const_iterator it =
	     packageInfo.sourceFiles.begin();
	   it != packageInfo.sourceFiles.end();
	   ++ it)
	{
	  PathName file (*it);
	  file.Normalize ();
	  filesAndPackages[file.Get()].push_back (packageInfo.deploymentName);
	}
    }
  for (map<string, vector<string> >::const_iterator it =
	 filesAndPackages.begin();
       it != filesAndPackages.end();
       ++ it)
    {
      if (it->second.size() > 1)
	{
	  cout << it->first << ":" << endl;
	  for (vector<string>::const_iterator it2 = it->second.begin();
	       it2 != it->second.end();
	       ++ it2)
	    {
	      cout << "  " << *it2 << endl;
	    }
	}
    }
  pIter->Dispose ();
}

/* _________________________________________________________________________

   Application::verifyMiKTeX
   _________________________________________________________________________ */

void
Application::VerifyMiKTeX ()
{
  vector<string> toBeVerified;
  auto_ptr<PackageIterator> pIter (pPackageManager->CreateIterator());
  PackageInfo packageInfo;
  for (int idx = 0; pIter->GetNext(packageInfo); ++ idx)
    {
      if (! packageInfo.IsPureContainer()
	  && packageInfo.IsInstalled()
	  && packageInfo.deploymentName.compare (0, 7, "miktex-") == 0)
	{
	  toBeVerified.push_back (packageInfo.deploymentName);
	}
    }
  pIter->Dispose ();
  Verify (toBeVerified);
}

/* _________________________________________________________________________

   Application::verify
   _________________________________________________________________________ */

void
Application::Verify (/*[in]*/ const vector<string> &	toBeVerifiedArg)
{
  vector<string> toBeVerified = toBeVerifiedArg;
  bool verifyAll = (toBeVerified.size() == 0);
  if (verifyAll)
    {
      auto_ptr<PackageIterator> pIter (pPackageManager->CreateIterator());
      PackageInfo packageInfo;
      for (int idx = 0; pIter->GetNext(packageInfo); ++ idx)
	{
	  if (! packageInfo.IsPureContainer()
	      && packageInfo.IsInstalled())
	    {
	      toBeVerified.push_back (packageInfo.deploymentName);
	    }
	}
      pIter->Dispose ();
    }
  bool ok = true;
  for (vector<string>::const_iterator it = toBeVerified.begin();
       it != toBeVerified.end();
       ++ it)
    {
      if (! pPackageManager->TryVerifyInstalledPackage(*it))
	{
	  Message (T_("%s: this package needs to be reinstalled.\n"),
		   it->c_str());
	  ok = false;
	}
    }
  if (ok)
    {
      if (verifyAll)
	{
	  Message (T_("All packages are correctly installed.\n"));
	}
      else
	{
	  if (toBeVerified.size() == 1)
	    {
	      Message (T_("Package %s is correctly installed.\n"),
		       toBeVerified[0].c_str());
	    }
	  else
	    {
	      Message (T_("The packages are correctly installed.\n"));
	    }
	}
    }
  else
    {
      Error (T_("Some packages need to be reinstalled."));
    }
}

/* _________________________________________________________________________

   Application::ImportPackage
   _________________________________________________________________________ */

void
Application::ImportPackage (/*[in]*/ /*[in]*/ const string & deploymentName,
			    /*[in,out]*/ vector<string> & toBeinstalled)
{
  if (repository.length() == 0)
    {
      Error (T_("You have to use --repository=/PATH/TO/MIKTEX."));
    }
  PathName packagesIni (repository);
  packagesIni += MIKTEX_PATH_PACKAGES_INI;
  if (! File::Exists(packagesIni))
    {
      Error (T_("Not a MiKTeX installation directory: %s"),
	     repository.c_str());
    }
  SmartPointer<Cfg> pCfg = Cfg::Create();
  pCfg->Read (packagesIni);
  if (strncmp(deploymentName.c_str(), "miktex-", 7) == 0)
    {
      Error (T_("Cannot import package %s."), deploymentName.c_str());
    }
  string str;
  if (! pCfg->TryGetValue(deploymentName.c_str(), "TimeInstalled", str)
      || str == ""
      || str == "0")
    {
      Error (T_("Package %s is not installed."), deploymentName.c_str());
    }
  if (pCfg->TryGetValue(deploymentName.c_str(), T_("Obsolete"), str)
      && str == "1")
    {
      Error (T_("Package %s is obsolete."), deploymentName.c_str());
    }
  PackageInfo packageInfo;
  if (! pPackageManager->TryGetPackageInfo(deploymentName.c_str(),
					   packageInfo))
    {
      Error (T_("Unknown package: %s."), deploymentName.c_str());
    }
  if (packageInfo.IsInstalled())
    {
      Error (T_("Package %s is already installed."), deploymentName.c_str());
    }
  toBeinstalled.push_back (deploymentName);
}

/* _________________________________________________________________________

   Application::ImportPackages
   _________________________________________________________________________ */

void
Application::ImportPackages (/*[in,out]*/ vector<string> & toBeinstalled)
{
  if (repository.length() == 0)
    {
      Error (T_("You have to use --repository=/PATH/TO/MIKTEX."));
    }
  PathName packagesIni (repository);
  packagesIni += MIKTEX_PATH_PACKAGES_INI;
  if (! File::Exists(packagesIni))
    {
      Error (T_("Not a MiKTeX installation directory: %s"),
	     repository.c_str());
    }
  SmartPointer<Cfg> pCfg = Cfg::Create();
  pCfg->Read (packagesIni);
  char szKey[BufferSizes::MaxCfgName];
  for (char * lpszKey = pCfg->FirstKey(szKey, BufferSizes::MaxCfgName);
       lpszKey != 0;
       lpszKey = pCfg->NextKey(szKey, BufferSizes::MaxCfgName))
    {
      if (strncmp(lpszKey, "miktex-", 7) == 0)
	{
	  continue;
	}
      string str;
      if (! pCfg->TryGetValue(lpszKey, "TimeInstalled", str)
	  || str == ""
	  || str == "0")
	{
	  continue;
	}
      if (pCfg->TryGetValue(lpszKey, "Obsolete", str)
	  && str == "1")
	{
	  continue;
	}
      PackageInfo packageInfo;
      if (! pPackageManager->TryGetPackageInfo(lpszKey, packageInfo)
	  || packageInfo.IsInstalled())
	{
	  continue;
	}
      toBeinstalled.push_back (lpszKey);
    } 
}

/* _________________________________________________________________________

   Application::FindUpdates
   _________________________________________________________________________ */

void
Application::FindUpdates ()
{
  auto_ptr<PackageInstaller> pInstaller (pPackageManager->CreateInstaller());
  if (repository.length() > 0)
    {
      pInstaller->SetRepository (repository);
    }
  pInstaller->SetCallback (this);
  pInstaller->FindUpdates ();
  vector<PackageInstaller::UpdateInfo> updates = pInstaller->GetUpdates();
  pInstaller->Dispose ();
  if (updates.size() == 0)
    {
      Message (T_("There are currently no updates available.\n"));
    }
  else
    {
      sort (updates.begin(), updates.end(), UpdateInfoComparer());
      for (vector<PackageInstaller::UpdateInfo>::const_iterator
	     it = updates.begin();
	   it != updates.end();
	   ++ it)
	{
	  cout << it->deploymentName << '\n';
	}
    }
}

/* _________________________________________________________________________

   Application::Update
   _________________________________________________________________________ */

void
Application::Update (/*[in]*/ const vector<string> &	updates)
{
  auto_ptr<PackageInstaller> pInstaller (pPackageManager->CreateInstaller());
  if (repository.length() > 0)
    {
      pInstaller->SetRepository (repository);
    }
  pInstaller->SetCallback (this);
  pInstaller->FindUpdates ();
  vector<PackageInstaller::UpdateInfo> serverUpdates
    = pInstaller->GetUpdates();
  vector<string> updates2;
  for (vector<PackageInstaller::UpdateInfo>::const_iterator it =
	 serverUpdates.begin();
       it != serverUpdates.end();
       ++ it)
    {
      updates2.push_back (it->deploymentName);
    }
  sort (updates2.begin(), updates2.end());
  vector<string> toBeInstalled;
  if (updates.size() == 0)
    {
      if (updates2.size() == 0)
	{
	  Message (T_("There are currently no updates available.\n"));
	}
      toBeInstalled = updates2;
    }
  else
    {
      for (vector<string>::const_iterator it = updates.begin();
	   it != updates.end();
	   ++ it)
	{
	  PackageInfo packageInfo = pPackageManager->GetPackageInfo(*it);
	  if (! packageInfo.IsInstalled())
	    {
	      Error (T_("Package \"%s\" is not installed."), it->c_str());
	    }
	  if (binary_search(updates2.begin(), updates2.end(), *it))
	    {
	      toBeInstalled.push_back (*it);
	    }
	  else
	    {
	      Message (T_("Package \"%s\" is up to date.\n"), it->c_str());
	    }
	}
    }
  if (toBeInstalled.size() == 0)
    {
      return;
    }
  if (binary_search(updates2.begin(), updates2.end(), MYPKG)
      && ! (toBeInstalled.size() == 1 && toBeInstalled[0] == MYPKG))
    {
      Error (T_("Package \"%s\" must be updated separately."), MYPKG);
    }
  pInstaller->SetFileLists (toBeInstalled, vector<string>());
  pInstaller->InstallRemove ();
  pInstaller->Dispose ();
  if (toBeInstalled.size() == 1)
    {
      Message (T_("Package \"%s\" has been successfully updated.\n"),
	       toBeInstalled[0].c_str());
    }
  else if (toBeInstalled.size() > 1)
    {
      Message (T_("%u packages have been successfully updated.\n"),
	       toBeInstalled.size());
    }
}

/* _________________________________________________________________________

   Application::GetDirectories
   _________________________________________________________________________ */

string
Application::GetDirectories (/*[in]*/ const string &	deploymentName)
{
  string ret;
  set<string> setDirectories;
  PackageInfo pi = pPackageManager->GetPackageInfo(deploymentName);
  for (vector<string>::const_iterator it = pi.runFiles.begin();
       it != pi.runFiles.end();
       ++ it)
    {
      PathName path (*it);
      if (! path.HasExtension(MIKTEX_PACKAGE_DEFINITION_FILE_SUFFIX))
	{
	  setDirectories.insert (path.RemoveFileSpec().Get());
	}
    }
  for (set<string>::const_iterator it = setDirectories.begin();
       it != setDirectories.end();
       ++ it)
    {
      if (it != setDirectories.begin())
	{
	  ret += PATH_DELIMITER;
	}
      ret += *it;
    }
  return (ret);
}

/* _________________________________________________________________________

   Application::List
   _________________________________________________________________________ */

void
Application::List (/*[in]*/ OutputFormat	outputFormat,
		   /*[in]*/ int			maxCount)
{
  auto_ptr<PackageIterator> pIter (pPackageManager->CreateIterator());
  PackageInfo packageInfo;
  set<PackageInfo, PackageInfoComparer> setPi;
  for (int idx = 0; pIter->GetNext(packageInfo); ++ idx)
    {
      if (packageInfo.IsPureContainer())
	{
	  continue;
	}
      setPi.insert (packageInfo);
    }
  if (setPi.size() == 0)
    {
      Message (T_("The package database files have not been installed.\n"));
    }
  int count = 0;
  for (set<PackageInfo, PackageInfoComparer>::const_iterator
	 it = setPi.begin();
       it != setPi.end() && count < maxCount;
       ++ it, ++ count)
    {
      if (outputFormat == OutputFormat::Listing)
	{
	  cout << (it->IsInstalled() ? 'i' : '-')
	       << "  " << setw(5) << static_cast<int>(it->GetNumFiles())
	       << " "
	       << setw(10) << left << static_cast<int>(it->GetSize())
	       << resetiosflags(ios_base::left)
	       << "  " << it->deploymentName
	       << endl;
	}
      else if (outputFormat == OutputFormat::CSV)
	{
	  string path =
	    pPackageManager->GetContainerPath(it->deploymentName, false);
	  string directories = GetDirectories(it->deploymentName);
	  cout << path << '\\' << it->deploymentName << ','
	       << directories << '\n';
	}
      else if (outputFormat == OutputFormat::DeploymentNames)
	{
	  cout << it->deploymentName
	       << endl;
	}
    }
  pIter->Dispose ();
}

/* _________________________________________________________________________

   Application::ListRepositories
   _________________________________________________________________________ */

void
Application::ListRepositories (/*[in]*/ OutputFormat outputFormat)
{
  outputFormat;
  pPackageManager->DownloadRepositoryList ();
  vector<RepositoryInfo> repositories = pPackageManager->GetRepositories();
  if (repositories.size() == 0)
    {
      Message (T_("No package repositories are currently available.\n"));
    }
  for (vector<RepositoryInfo>::const_iterator
	 it = repositories.begin();
       it != repositories.end();
       ++ it)
    {
      cout << it->url << '\n';
    }
}

/* _________________________________________________________________________

   Application::PickRepositoryUrl
   _________________________________________________________________________ */

void
Application::PickRepositoryUrl ()
{
  cout << pPackageManager->PickRepositoryUrl() << endl;
}

/* _________________________________________________________________________

   Application::PrintFiles
   _________________________________________________________________________ */

void
Application::PrintFiles (/*[in]*/ const vector<string> & files)
{
  for (vector<string>::const_iterator it = files.begin();
       it != files.end();
       ++ it)
    {
      PathName path = pSession->GetSpecialPath(SpecialPath::InstallRoot);
      string disp;
      if (! PackageManager::StripTeXMFPrefix(*it, disp))
	{
	  disp = *it;
	}
      path += disp;
      cout << "  " << path.ToString() << '\n';
    }
}

/* _________________________________________________________________________

   Application::PrintPackageInfo
   _________________________________________________________________________ */

void
Application::PrintPackageInfo (/*[in]*/ const string & deploymentName)
{
  PackageInfo packageInfo = pPackageManager->GetPackageInfo(deploymentName);
  cout << T_("name: ") << packageInfo.deploymentName << '\n'
       << T_("title: ") << packageInfo.title << '\n';
  if (packageInfo.runFiles.size() > 0)
    {
      cout << T_("run-time files:\n");
      PrintFiles (packageInfo.runFiles);
    }
  if (packageInfo.docFiles.size() > 0)
    {
      cout << T_("documentation files:\n");
      PrintFiles (packageInfo.docFiles);
    }
  if (packageInfo.sourceFiles.size() > 0)
    {
      cout << T_("source files:\n");
      PrintFiles (packageInfo.sourceFiles);
    }
}

/* _________________________________________________________________________

   Application::RestartWindowed
   _________________________________________________________________________ */

void
Application::RestartWindowed ()
{
  Verbose (T_("Restarting in windowed mode...\n"));

  PathName mpmgui;

#if defined(MIKTEX_ATLMFC)
  // locate mpm_mfc
  if (! SessionWrapper(true)->FindFile(MIKTEX_MPM_MFC_EXE,
				       FileType::EXE,
				       mpmgui))
    {
      Error (T_("Could not restart in windowed mode."));
    }
#else
  // locate mpm_qt
  if (! SessionWrapper(true)->FindFile(MIKTEX_MPM_QT_EXE,
				       FileType::EXE,
				       mpmgui))
    {
      Error (T_("Could not restart in windowed mode."));
    }
#endif

  if (pSession->IsAdminMode())
    {
      Process::Start (mpmgui, "--admin");
    }
  else
    {
      Process::Start (mpmgui);
    }
}

/* _________________________________________________________________________

   Application::ReadFileList
   _________________________________________________________________________ */

void
Application::ReadFileList (/*[in]*/ const PathName &		path,
			   /*[in,out]*/ vector<string> &	files)
{
  StreamReader reader (path);
  string line;
  while (reader.ReadLine(line))
    {
      Tokenizer tok (line.c_str(), " \t\n\r");
      if (tok.GetCurrent() != 0)
	{
	  files.push_back (tok.GetCurrent());
	}
    }
  reader.Close ();
}

/* _________________________________________________________________________

   Application::Main
   _________________________________________________________________________ */

void
Application::Main (/*[in]*/ int			argc,
		   /*[in]*/ const char **	argv)
{
  StartupConfig startupConfig;
  Session::InitInfo initInfo;
  initInfo.SetProgramInvocationName (argv[0]);

  bool optAdmin = false;
  bool optFindConflicts = false;
  bool optFindUpdates = false;
  bool optImport = false;
  bool optImportAll = false;
  bool optList = false;
  bool optListRepositories = false;
  bool optPickRepositoryUrl = false;
  bool optPrintPackageInfo = false;
  bool optRegisterComponents = false;
  bool optSetRepository = false;
  bool optSort = false;
  bool optUnregisterComponents = false;
  bool optUpdate = false;
  bool optUpdateAll = false;
  bool optUpdateDb = false;
  bool optUpdateFndb = false;
  bool optVerify = false;
  bool optVerifyMiKTeX = false;
  bool optVersion = false;
  int optMaxCount = INT_MAX;
  int optProxyPort = -1;
  OutputFormat outputFormat (OutputFormat::Listing);
  string deploymentName;
  string optProxy;
  string optProxyPassword;
  string optProxyUser;
  string toBeImported;
  vector<string> installSome;
  vector<string> toBeInstalled;
  vector<string> toBeRemoved;
  vector<string> toBeVerified;
  vector<string> updateSome;
  vector<string> updates;

  bool changeProxy = false;

  Cpopt popt (argc, argv, aoption);

  //  popt.SetOtherOptionHelp (T_("[OPTION...]"));

  // process command-line options
  int option;
  while ((option = popt.GetNextOpt()) >= 0)
    {
      const char * lpszOptArg = popt.GetOptArg();
      switch (option)
	{
	case OPT_ADMIN:
	  optAdmin = true;
	  break;
	case OPT_CSV:
	  outputFormat = OutputFormat::CSV;
	  break;
	case OPT_FIND_CONFLICTS:
	  optFindConflicts = true;
	  break;
	case OPT_FIND_UPDATES:
	  optFindUpdates = true;
	  break;
#if defined (MIKTEX_WINDOWS)
	case OPT_HHELP:
	  {
	    pSession.CreateSession (initInfo);
	    pSession->ShowManualPageAndWait (0, MIKTEXHELP_MPMCON);
	    pSession.Reset ();
	    return;
	  }
#endif
	case OPT_IMPORT:
	  toBeImported = lpszOptArg;
	  optImport = true;
	  break;
	case OPT_IMPORT_ALL:
	  optImportAll = true;
	  break;
	case OPT_INSTALL:
	  toBeInstalled.push_back (lpszOptArg);
	  break;
	case OPT_INSTALL_ROOT:
	  startupConfig.commonInstallRoot = lpszOptArg;
	  startupConfig.userInstallRoot = lpszOptArg;
	  break;
	case OPT_INSTALL_SOME:
	  installSome.push_back (lpszOptArg);
	  break;
	case OPT_LIST:
	  optList = true;
	  break;
	case OPT_LIST_PACKAGE_NAMES:
	  optList = true;
	  outputFormat = OutputFormat::DeploymentNames;
	  break;
	case OPT_LIST_REPOSITORIES:
	  optListRepositories = true;
	  break;
	case OPT_PICK_REPOSITORY_URL:
	  optPickRepositoryUrl = true;
	  break;
	case OPT_PRINT_PACKAGE_INFO:
	  optPrintPackageInfo = true;
	  deploymentName = lpszOptArg;
	  break;
	case OPT_PROXY:
	  {
	    changeProxy = true;
	    Tokenizer tok (lpszOptArg, ":");
	    optProxy = tok.GetCurrent();
	    ++ tok;
	    if (tok.GetCurrent() != 0)
	      {
		optProxyPort = atoi(tok.GetCurrent());
	      }
	  }
	  break;
	case OPT_SORT:
	  optSort = true;
	  if (StringCompare(lpszOptArg, "deploymentname", true) == 0)
	    {
	      PackageInfoComparer::sortKey = SortKey::DeploymentName;
	    }
	  else if (StringCompare(lpszOptArg, "installedon", true) == 0)
	    {
	      PackageInfoComparer::sortKey = SortKey::InstalledOn;
	    }
	  else if (StringCompare(lpszOptArg, "packagedon", true) == 0)
	    {
	      PackageInfoComparer::sortKey = SortKey::PackagedOn;
	    }
	  else
	    {
	      Error (T_("Unknown sort key."));
	    }
	  break;
	case OPT_PROXY_USER:
	  changeProxy = true;
	  optProxyUser = lpszOptArg;
	  break;
	case OPT_PROXY_PASSWORD:
	  changeProxy = true;
	  optProxyPassword = lpszOptArg;
	  break;
	case OPT_QUIET:
	  if (verbose)
	    {
	      Error (T_("Cannot be --verbose and --quiet at the same time."));
	    }
	  quiet = true;
	  break;
#if defined (MIKTEX_WINDOWS)
	case OPT_REGISTER_COMPONENTS:
	  optRegisterComponents = true;
	  break;
#endif
	case OPT_MAX_COUNT:
	  optMaxCount = atoi(lpszOptArg);
	  break;
	case OPT_OUTPUT_FORMAT:
	  {
	    if (StringCompare(lpszOptArg, "listing", true) == 0)
	    {
	      outputFormat = OutputFormat::Listing;
	    }
	  else if (StringCompare(lpszOptArg, "csv", true) == 0)
	    {
	      outputFormat = OutputFormat::CSV;
	    }
	  else if (StringCompare(lpszOptArg, "deploymentnames", true) == 0)
	    {
	      outputFormat = OutputFormat::DeploymentNames;
	    }
	  else
	    {
	      Error (T_("Unknown output format."));
	    }
	  }
	case OPT_REPOSITORY:
	  repository = lpszOptArg;
	  break;
	case OPT_REVERSE:
	  PackageInfoComparer::reverse = true;
	  break;
	case OPT_SET_REPOSITORY:
	  optSetRepository = true;
	  repository = lpszOptArg;
	  break;
	case OPT_TRACE:
	  if (lpszOptArg == 0)
	    {
	      initInfo.SetTraceFlags (DEFAULT_TRACE_STREAMS);
	    }
	  else
	    {
	      initInfo.SetTraceFlags (lpszOptArg);
	    }
	  break;
	case OPT_UNINSTALL:
	  toBeRemoved.push_back (lpszOptArg);
	  break;
#if defined (MIKTEX_WINDOWS)
	case OPT_UNREGISTER_COMPONENTS:
	  optUnregisterComponents = true;
	  break;
#endif
	case OPT_UPDATE:
	  if (lpszOptArg != 0)
	    {
	      if (optUpdateAll)
		{
		  Error (T_("Already updating all packages."));
		}
	      optUpdate = true;
	      updates.push_back (lpszOptArg);
	    }
	  else
	    {
	      if (optUpdate)
		{
		  Error (T_("Already updating selected packages."));
		}
	      optUpdateAll = true;
	    }
	  break;
	case OPT_UPDATE_ALL:
	  if (optUpdate)
	    {
	      Error (T_("Already updating selected packages."));
	    }
	  optUpdateAll = true;
	  break;
	case OPT_UPDATE_DB:
	  optUpdateDb = true;
	  break;
	case OPT_UPDATE_FNDB:
	  optUpdateFndb = true;
	  break;
	case OPT_UPDATE_SOME:
	  updateSome.push_back (lpszOptArg);
	  break;
	case OPT_VERBOSE:
	  if (quiet)
	    {
	      Error (T_("Cannot be --verbose and --quiet at the same time."));
	    }
	  verbose = true;
	  break;
	case OPT_VERIFY:
	  if (lpszOptArg != 0)
	    {
	      toBeVerified.push_back (lpszOptArg);
	    }
	  optVerify = true;
	  break;
	case OPT_VERIFY_MIKTEX:
	  optVerifyMiKTeX = true;
	  break;
	case OPT_VERSION:
	  optVersion = true;
	  break;
	}
    }

  if (option != -1)
    {
      string msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += ": ";
      msg += popt.Strerror(option);
      Error ("%s", msg.c_str());
    }
      
  if (popt.GetArgs() != 0)
    {
      Error (T_("This utility does not accept non-option arguments."));
    }

  if (optVersion)
    {
      cout
	<< (Utils::MakeProgramVersionString
	    (THE_NAME_OF_THE_GAME,
	     VersionNumber(MIKTEX_MAJOR_VERSION,
			   MIKTEX_MINOR_VERSION,
			   MIKTEX_COMP_J2000_VERSION,
			   0)))
	<< T_("\n\
Copyright (C) 2005-2009 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
	<< endl;
      return;
    }

  if (optAdmin)
    {
      initInfo.SetFlags (initInfo.GetFlags() | Session::InitFlags::AdminMode);
    }

  initInfo.SetStartupConfig (startupConfig);

  pSession.CreateSession (initInfo);

  pPackageManager.Create ();

  if (changeProxy)
    {
      ProxySettings proxySettings;
      ProxySettings temp;
      if (PackageManager::TryGetProxy(temp))
	{
	  proxySettings = temp;
	}
      proxySettings.useProxy = true;
      if (! optProxy.empty())
	{
	  proxySettings.proxy = optProxy;
	}
      if (optProxyPort >= 0)
	{
	  proxySettings.port = optProxyPort;
	}
      if (! optProxyUser.empty())
	{
	  proxySettings.authenticationRequired = true;
	  proxySettings.user = optProxyUser;
	}
      if (! optProxyPassword.empty())
	{
	  proxySettings.password = optProxyPassword;
	}
      pPackageManager->SetProxy (proxySettings);
    }

  bool restartWindowed = true;

  if (optSetRepository)
    {
      pPackageManager->SetDefaultPackageRepository (RepositoryType::Unknown,
						    repository);
      restartWindowed = false;
    }

  if (optUpdateFndb && ! optUpdateDb)
    {
      pPackageManager->CreateMpmFndb ();
      restartWindowed = false;
    }

  if (optUpdateDb)
    {
      UpdateDb ();
      restartWindowed = false;
    }

  if (optImport)
    {
      ImportPackage (toBeImported, toBeInstalled);
      restartWindowed = false;
    }

  if (optImportAll)
    {
      ImportPackages (toBeInstalled);
      restartWindowed = false;
    }

  if (optFindUpdates)
    {
      FindUpdates ();
      restartWindowed = false;
    }

  vector<string>::const_iterator it;

  for (it = installSome.begin(); it != installSome.end(); ++ it)
    {
      ReadFileList (*it, toBeInstalled);
    }

  if (toBeInstalled.size() > 0 || toBeRemoved.size() > 0)
    {
      Install (toBeInstalled, toBeRemoved);
      restartWindowed = false;
    }

#if defined(MIKTEX_WINDOWS)
  if (optRegisterComponents || optUnregisterComponents)
    {
      if (optRegisterComponents && optUnregisterComponents)
	{
	  Error (T_("--register-components conflicts \
with --unregister-components."));
	}
      RegisterComponents (optRegisterComponents);
      restartWindowed = false;
    }
#endif

  for (it = updateSome.begin(); it != updateSome.end(); ++ it)
    {
      ReadFileList (*it, updates);
    }

  if (optUpdateAll || updates.size() > 0)
    {
      Update (updates);
      restartWindowed = false;
    }

  if (optFindConflicts)
    {
      FindConflicts ();
      restartWindowed = false;
    }

  if (optVerifyMiKTeX)
    {
      VerifyMiKTeX ();
      restartWindowed = false;
    }

  if (optVerify)
    {
      Verify (toBeVerified);
      restartWindowed = false;
    }

  if (optList)
    {
      List (outputFormat, optMaxCount);
      restartWindowed = false;
    }

  if (optListRepositories)
    {
      ListRepositories (outputFormat);
      restartWindowed = false;
    }

  if (optPickRepositoryUrl)
    {
      PickRepositoryUrl ();
      restartWindowed = false;
    }

  if (optPrintPackageInfo)
    {
      PrintPackageInfo (deploymentName);
      restartWindowed = false;
    }
  
  if (restartWindowed)
    {
      RestartWindowed ();
    }
  
  pPackageManager.Release ();
  pSession.Reset ();
}

/* _________________________________________________________________________

   Application::SignalHandler
   _________________________________________________________________________ */

extern "C"
void
Application::SignalHandler (/*[in]*/ int signalToBeHandled)
{
  switch (signalToBeHandled)
    {
    case SIGINT:
    case SIGTERM:
      signal (SIGINT, SIG_IGN);
      interrupted = true;
    }
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)
{
#if defined(MIKTEX_WINDOWS)
  HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
  if (FAILED(hr))
    {
      cerr << "mpm" << ": "
	   << T_("The COM library could not be initialized.")
	   << endl;
      return (1);
    }
#endif
  int retCode = 0;
  try
    {
      Application app;
      app.Main (argc, argv);
    }
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      retCode = 1;
    }
  catch (const exception & e)
    {
      Utils::PrintException (e);
      retCode = 1;
    }
  catch (int rc)
    {
      retCode = rc;
    }
#if defined(MIKTEX_WINDOWS)
  CoUninitialize ();
#endif
  return (retCode);
}
