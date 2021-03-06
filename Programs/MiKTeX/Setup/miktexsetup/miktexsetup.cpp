/* miktexsetup.cpp:
   
   Copyright (C) 2014 Christian Schenk

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

#include "StdAfx.h"

#include "internal.h"

#if ! defined(THE_NAME_OF_THE_GAME)
#  define THE_NAME_OF_THE_GAME T_("MiKTeX Setup Utility")
#endif

const char * DEFAULT_TRACE_STREAMS =
  MIKTEX_TRACE_CORE ","
  MIKTEX_TRACE_CURL ","
  MIKTEX_TRACE_ERROR ","
  MIKTEX_TRACE_FNDB ","
  MIKTEX_TRACE_MPM
  ;

/* _________________________________________________________________________

   Application
   _________________________________________________________________________ */

class Application
  : SetupServiceCallback
{
private:
  void InstallSignalHandler(int sig)
  {
    void (* oldHandlerFunc) (int);
    oldHandlerFunc = signal(sig, Application::SignalHandler);
    if (oldHandlerFunc == SIG_ERR)
    {
      Error(T_("signal() failed for some reason."));
    }
    if (oldHandlerFunc != SIG_DFL)
    {
      if (signal(sig, oldHandlerFunc) == SIG_ERR)
      {
	Error(T_("signal() failed for some reason."));
      }
    }
  }

public:
  Application () :
    verbose(false),
    quiet(false)
  {
    InstallSignalHandler(SIGINT);
    InstallSignalHandler(SIGTERM);
  }

public:
  virtual void ReportLine(/*[in]*/ const char * lpszLine);

public:
  virtual bool MIKTEXTHISCALL OnRetryableError(const char * lpszMessage);

public:
  virtual bool MIKTEXTHISCALL OnProgress(MiKTeX::Setup::Notification nf);

public:
  virtual bool MIKTEXTHISCALL OnProcessOutput(const void * pOutput, size_t n);

public:
  void Main(int argc, const char ** argv);

private:
  void Verbose(const char * lpszFormat, ...);

private:
  void Message(const char * lpszFormat, ...);

private:
  MIKTEXNORETURN void Error(const char * lpszFormat, ...);

private:
  static void SignalHandler(int sig);

private:
  SetupServicePtr pSetupService;

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
};

/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 1,
  OPT_ADMIN,
  OPT_PACKAGE_LEVEL,
  OPT_QUIET,
  OPT_TRACE,
  OPT_VERBOSE,
  OPT_VERSION,
};

/* _________________________________________________________________________

   Application::aoption
   _________________________________________________________________________ */

const struct poptOption Application::aoption[] = {

  {
    "admin", 0, POPT_ARG_NONE, 0, OPT_ADMIN,
    T_("Run in administrative mode."), 0,
  },

  {
    "quiet", 0, POPT_ARG_NONE, 0, OPT_QUIET,
    T_("Suppress all output (except errors)."), 0
  },

  {
    "package-level", 0, POPT_ARG_STRING, 0, OPT_PACKAGE_LEVEL,
    T_("Set the package level (essential, basic, ...)."), 0
  },

  {
    "trace", 0, POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0, OPT_TRACE,
    T_("\
Turn on tracing.\
  TRACESTREAMS, if specified, is a comma-separated list of trace stream names\
 (see the MiKTeX manual)."),
    T_("TRACESTREAMS")
  },

  {
    "verbose", 0, POPT_ARG_NONE, 0, OPT_VERBOSE,
    T_("Turn on verbose output mode."), 0
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

void Application::Message(const char *	lpszFormat, ...)
{
  if (quiet)
  {
    return;
  }
  va_list arglist;
  va_start(arglist, lpszFormat);
  printf("%s", Utils::FormatString(lpszFormat, arglist).c_str());
  va_end(arglist);
}

/* _________________________________________________________________________

   Application::Verbose
   _________________________________________________________________________ */

void Application::Verbose(const char * lpszFormat, ...)
{
  if (! verbose)
  {
    return;
  }
  va_list arglist;
  va_start (arglist, lpszFormat);
  printf ("%s", Utils::FormatString(lpszFormat, arglist).c_str());
  va_end (arglist);
}

/* _________________________________________________________________________

   Application::Error
   _________________________________________________________________________ */

MIKTEXNORETURN void Application::Error(const char * lpszFormat, ...)
{
  va_list arglist;
  va_start(arglist, lpszFormat);
  fprintf(stderr, "miktexsetup: %s\n", Utils::FormatString(lpszFormat, arglist).c_str());
  va_end(arglist);
  throw 1;
}

/* _________________________________________________________________________

   Application::ReportLine
   _________________________________________________________________________ */

void Application::ReportLine(const char * lpszLine)
{
  Verbose("%s\n", lpszLine);
}

/* _________________________________________________________________________

   Application::OnRetryableError
   _________________________________________________________________________ */

bool Application::OnRetryableError (const char * lpszMessage)
{
  return false;
}

/* _________________________________________________________________________

   Application::OnProgress
   _________________________________________________________________________ */

bool Application::OnProgress(MiKTeX::Setup::Notification nf)
{
  return ! interrupted;
}

/* _________________________________________________________________________

   Application::OnProcessOutput
   _________________________________________________________________________ */

bool Application::OnProcessOutput(const void * pOutput, size_t n)
{
  Verbose ("%.*s", n, reinterpret_cast<const char *>(pOutput));
  return ! interrupted;
}

/* _________________________________________________________________________

   Application::Main
   _________________________________________________________________________ */

void Application::Main(int argc, const char ** argv)
{
  Session::InitInfo initInfo;
  initInfo.SetProgramInvocationName(argv[0]);

  bool optAdmin = false;
  bool optVersion = false;
  PackageLevel optPackageLevel = PackageLevel::None;
  Cpopt popt(argc, argv, aoption);
  int option;

  while ((option = popt.GetNextOpt()) >= 0)
  {
    const char * lpszOptArg = popt.GetOptArg();
    switch (option)
    {
    case OPT_ADMIN:
      optAdmin = true;
      break;
    case OPT_PACKAGE_LEVEL:
      if (StringCompare(lpszOptArg, "essential") == 0)
      {
	optPackageLevel = PackageLevel::Essential;
      }
      else if (StringCompare(lpszOptArg, "basic") == 0)
      {
	optPackageLevel = PackageLevel::Basic;
      }
      else if (StringCompare(lpszOptArg, "advanced") == 0)
      {
	optPackageLevel = PackageLevel::Advanced;
      }
      else if (StringCompare(lpszOptArg, "complete") == 0)
      {
	optPackageLevel = PackageLevel::Complete;
      }
      else
      {
	Error("Application::Main", T_("Invalid package level."), 0);
      }
      break;
    case OPT_QUIET:
      if (verbose)
      {
	Error(T_("Cannot be --verbose and --quiet at the same time."));
      }
      quiet = true;
      break;
    case OPT_TRACE:
      if (lpszOptArg == 0)
      {
	initInfo.SetTraceFlags(DEFAULT_TRACE_STREAMS);
      }
      else
      {
	initInfo.SetTraceFlags(lpszOptArg);
      }
      break;
    case OPT_VERBOSE:
      if (quiet)
      {
	Error(T_("Cannot be --verbose and --quiet at the same time."));
      }
      verbose = true;
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
    Error("%s", msg.c_str());
  }
      
  if (optVersion)
  {
    printf ("%s\n", Utils::MakeProgramVersionString
      (THE_NAME_OF_THE_GAME,
      VersionNumber(MIKTEX_MAJOR_VERSION,
      MIKTEX_MINOR_VERSION,
      MIKTEX_COMP_J2000_VERSION,
      0)).c_str());
    printf (T_("\
Copyright (C) 2014 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"));
    return;
  }

  const char ** leftovers = popt.GetArgs();

  int argCount = popt.GetArgCount();

  if (argCount == 0)
  {
    Error(T_("Missing arguments."));
  }

  if (argCount > 1)
  {
    Error(T_("Too many arguments."));
  }

  pSession.CreateSession(initInfo);

  pSetupService.Create ();
  SetupOptions setupOptions = pSetupService->GetOptions();

  if (strcmp("download", leftovers[0]) == 0)
  {
    setupOptions.Task = SetupTask::Download;
  }
  else if (strcmp("install", leftovers[0]) == 0)
  {
    setupOptions.Task = SetupTask::InstallFromLocalRepository;
  }
  else
  {
    Error(T_("Unknown/unsupported setup task: %s"), leftovers[0]);
  }

  if (optPackageLevel != PackageLevel::None)
  {
    setupOptions.PackageLevel = optPackageLevel;
  }

  pSetupService->SetOptions(setupOptions);

  pSetupService->SetCallback(this);

  if (optAdmin)
  {
    if (! pSession->RunningAsAdministrator())
    {
#if defined(MIKTEX_WINDOWS)
      Error(T_("Option --admin requires administrator privileges."));
#else
      Error(T_("Option --admin requires root privileges."));
#endif
    }
    pSession->SetAdminMode(true);
  }

  pSetupService->Run();

  pSetupService.Release ();
  pSession.Reset ();
}

/* _________________________________________________________________________

   Application::SignalHandler
   _________________________________________________________________________ */

extern "C" void Application::SignalHandler(int signalToBeHandled)
{
  switch (signalToBeHandled)
  {
  case SIGINT:
  case SIGTERM:
    signal(SIGINT, SIG_IGN);
    interrupted = true;
  }
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
#if defined(_UNICODE)
wmain(int argc, const wchar_t ** argv)
#else
main(int argc, const char ** argv)
#endif
{
  int retCode = 0;
  try
  {
    vector<string> utf8args;
    utf8args.reserve(argc);
    vector<const char *> newargv;
    newargv.reserve(argc + 1);
    for (int idx = 0; idx < argc; ++ idx)
    {
#if defined(_UNICODE)
      utf8args.push_back(Utils::WideCharToUTF8(argv[idx]));
#else
      utf8args.push_back(Utils::AnsiToUTF8(argv[idx]));
#endif
      newargv.push_back(utf8args[idx].c_str());
    }
    newargv.push_back(0);
    Application app;
    app.Main(argc, &newargv[0]);
  }
  catch (const MiKTeXException & e)
  {
    Utils::PrintException(e);
    retCode = 1;
  }
  catch (const exception & e)
  {
    Utils::PrintException(e);
    retCode = 1;
  }
  catch (int rc)
  {
    retCode = rc;
  }
  return retCode;
}
