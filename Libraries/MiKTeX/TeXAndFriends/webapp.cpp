/* webapp.cpp:

   Copyright (C) 1996-2010 Christian Schenk
 
   This file is part of the MiKTeX TeXMF Library.

   The MiKTeX TeXMF Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX TeXMF Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX TeXMF Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

#include <miktex/Core/Version>

/* _______________________________________________________________________
   
   WebApp::FatalError
   _______________________________________________________________________ */

void
WebApp::FatalError (/*[in]*/ const char *	lpszFormat,
		    /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  cerr << endl;
  cerr << Utils::GetExeName() << ": "
       << Utils::FormatString(lpszFormat, arglist)
       << endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________
     
   WebApp::Init
   _________________________________________________________________________ */

void
WebApp::Init (/*[in]*/ const char * lpszProgramInvocationName)
{
  Application::Init (lpszProgramInvocationName, TheNameOfTheGame());
  theNameOfTheGame = Utils::GetExeName();
  enable8BitChars = false;
}

/* _________________________________________________________________________
   
   WebApp::Finalize
   _________________________________________________________________________ */

void
WebApp::Finalize ()
{
  if (! packageListFileName.Empty())
    {
      FileStream stream
	(File::Open(packageListFileName,
		    FileMode::Create,
		    FileAccess::Write));
      vector<FileInfoRecord> fileInfoRecords = pSession->GetFileInfoRecords();
      set<string> packages;
      for (vector<FileInfoRecord>::const_iterator it = fileInfoRecords.begin();
	   it != fileInfoRecords.end();
	   ++ it)
	{
	  if (it->packageName.length() > 0)
	    {
	      packages.insert (it->packageName);
	    }
	}
      for (set<string>::const_iterator it2 = packages.begin();
	   it2 != packages.end();
	   ++ it2)
	{
	  fprintf (stream.Get(), "%s\n", it2->c_str());
	}
      stream.Close ();
    }
  features.reset ();
  copyright = "";
  packageListFileName = "";
  programName = "";
  tcxFileName = "";
  trademarks = "";
  version = "";
  options.clear ();
  theNameOfTheGame = "";
  Application::Finalize ();
}

/* _________________________________________________________________________

   WebApp::ShowHelp
   _________________________________________________________________________ */

void
WebApp::ShowHelp (/*[in]*/ bool usageOnly)
  const
{
  if (options.size() == 0 || usageOnly || popt == 0)
    {
      return;
    }
  if (usageOnly)
    {
      popt.PrintUsage ();
    }
  else
    {
      popt.PrintHelp ();
    }
}

/* _________________________________________________________________________

   WebApp::BadUsage
   _________________________________________________________________________ */

void
WebApp::BadUsage ()
  const
{
  cerr << T_("Invalid command-line. Try this:\n")
       << Utils::GetExeName() << " -help"
       << endl;
  throw (1);
}

/* _________________________________________________________________________

   WebApp::AddOption
   _________________________________________________________________________ */

void
WebApp::AddOption (/*[in]*/ const char *	lpszNameAndHelp,
		   /*[in]*/ int			val,
		   /*[in]*/ int			argInfo,
		   /*[in]*/ const char *	lpszArgDescription,
		   /*[in]*/ void *		pArg,
		   /*[in]*/ char		shortName)
{
#if defined(MIKTEX_DEBUG)
  if (lpszNameAndHelp != 0)
    {
      for (vector<poptOption>::const_iterator it = options.begin();
	   it != options.end();
	   ++ it)
	{
	  if (StringCompare(lpszNameAndHelp, it->longName, false) == 0)
	    {
	      FATAL_MIKTEX_ERROR ("WebApp::AddOption",
				  T_("Option already added."),
				  lpszNameAndHelp);
	    }
	}
    }
#endif
  poptOption opt;
  memset (&opt, 0, sizeof(opt));
  opt.longName = lpszNameAndHelp;
  opt.shortName = shortName;
  opt.argInfo = argInfo | POPT_ARGFLAG_ONEDASH;
  if (val == OPT_UNSUPPORTED || val == OPT_NOOP)
    {
      opt.argInfo |= POPT_ARGFLAG_DOC_HIDDEN;
    }
  opt.arg = pArg;
  opt.val = val;
  if (lpszNameAndHelp != 0
      && val != OPT_UNSUPPORTED
      && val != OPT_NOOP
      && ! (argInfo & POPT_ARGFLAG_DOC_HIDDEN))
    {
      opt.descrip = lpszNameAndHelp + StrLen(lpszNameAndHelp) + 1;
    }
  else
    {
      opt.descrip = 0;
    }
  opt.argDescrip = lpszArgDescription;
  options.push_back (opt);
}

/* _________________________________________________________________________

   WebApp::AddOption
   _________________________________________________________________________ */

void
WebApp::AddOption (/*[in]*/ const char *	lpszAliasName,
		   /*[in]*/ const char *	lpszName)
{
  for (vector<poptOption>::const_iterator it = options.begin();
       it != options.end();
       ++ it)
    {
      if (StringCompare(lpszName, it->longName, false) == 0)
	{
	  poptOption opt = *it;
	  opt.longName = lpszAliasName;
	  opt.argInfo |= POPT_ARGFLAG_DOC_HIDDEN;
	  options.push_back (opt);
	  return;
	}
    }
  FATAL_MIKTEX_ERROR ("WebApp::AddOption",
		      T_("Unknown command-line option."),
		      lpszName);
}

/* _________________________________________________________________________

   WebApp::AddOptions
   _________________________________________________________________________ */

enum {
  OPT_ALIAS,
  OPT_DISABLE_INSTALLER,
  OPT_ENABLE_INSTALLER,
  OPT_HELP,
  OPT_HHELP,
  OPT_INCLUDE_DIRECTORY,
  OPT_RECORD_PACKAGE_USAGES,
  OPT_TRACE,
  OPT_VERBOSE,
  OPT_VERSION,
};

void
WebApp::AddOptions ()
{
  options.reserve (50);

  optBase = static_cast<int>(GetOptions().size());

  AddOption (T_("alias\0\
Pretend to be APP.  This affects both the format used and the search path."),
	     FIRST_OPTION_VAL + optBase + OPT_ALIAS,
	     POPT_ARG_STRING,
	     T_("APP"));

  AddOption (T_("disable-installer\0\
Disable the package installer.  Missing files will not be installed."),
	     FIRST_OPTION_VAL + optBase + OPT_DISABLE_INSTALLER);

  AddOption (T_("enable-installer\0\
Enable the package installer.  Missing files will be installed."),
	     FIRST_OPTION_VAL + optBase + OPT_ENABLE_INSTALLER);

  AddOption (T_("help\0\
Show this help screen and exit."),
	     FIRST_OPTION_VAL + optBase + OPT_HELP);

  AddOption (T_("include-directory\0\
Prefix DIR to the input search path."),
	     FIRST_OPTION_VAL + optBase + OPT_INCLUDE_DIRECTORY,
	     POPT_ARG_STRING,
	     T_("DIR"));

  AddOption (T_("kpathsea-debug\0"),
	     OPT_UNSUPPORTED,
	     POPT_ARG_STRING);

  AddOption (T_("record-package-usages\0\
Enable the package usage recorder.  Output is written to FILE."),
	     FIRST_OPTION_VAL + optBase + OPT_RECORD_PACKAGE_USAGES,
	     POPT_ARG_STRING,
	     T_("FILE"));

  AddOption (T_("trace\0\
Turn tracing on.  OPTIONS must be a comma-separated list of trace options. \
  See the manual, for more information."),
	     FIRST_OPTION_VAL + optBase + OPT_TRACE,
	     POPT_ARG_STRING,
	     T_("OPTIONS"));

  AddOption ("verbose\0", OPT_UNSUPPORTED);

  AddOption (T_("version\0\
Print version information and exit."),
	     FIRST_OPTION_VAL + optBase + OPT_VERSION);

#if defined(MIKTEX_WINDOWS)
  if (GetHelpId() > 0)
    {
      AddOption (T_("hhelp\0\
Show the manual page in an HTMLHelp window and exit when the\
 window is closed."),
		 FIRST_OPTION_VAL + optBase + OPT_HHELP);
    }
#endif
}

/* _________________________________________________________________________

   WebApp::ProcessOption
   _________________________________________________________________________ */

bool
WebApp::ProcessOption (/*[in]*/ int		opt,
		       /*[in]*/ const char *	lpszArg)
{
  if (opt == OPT_UNSUPPORTED)
    {
      FATAL_MIKTEX_ERROR ("WebApp::ProcessOption",
			  T_("Unsupported command-line option."),
			  0);
    }
  else if (opt == OPT_NOOP)
    {
      return (true);
    }
  bool done = true;
  switch (opt - FIRST_OPTION_VAL - optBase)
    {
    case OPT_ALIAS:
      pSession->PushAppName (lpszArg);
      break;
    case OPT_DISABLE_INSTALLER:
      EnableInstaller (TriState::False);
      break;
    case OPT_ENABLE_INSTALLER:
      EnableInstaller (TriState::True);
      break;
    case OPT_INCLUDE_DIRECTORY:
      if (Directory::Exists(lpszArg))
	{
	  PathName path (lpszArg);
	  path.MakeAbsolute ();
	  pSession->AddInputDirectory (path.Get(), true);
	}
      break;
    case OPT_HELP:
      ShowHelp ();
      throw (0);
#if defined(MIKTEX_WINDOWS)
    case OPT_HHELP:
      MIKTEX_ASSERT (GetHelpId() > 0);
      pSession->ShowManualPageAndWait (0, GetHelpId());
      throw (0);
#endif
    case OPT_RECORD_PACKAGE_USAGES:
      pSession->StartFileInfoRecorder (true);
      packageListFileName = lpszArg;
      break;
    case OPT_TRACE:
      MiKTeX::Core::TraceStream::SetTraceFlags (lpszArg);
      break;
    case OPT_VERSION:
      ShowProgramVersion ();
      throw (0);
    default:
      done = false;
      break;
    }
  return (done);
}

/* _________________________________________________________________________

   WebApp::ProcessCommandLineOptions
   _________________________________________________________________________ */

inline
bool
operator< (/*[in]*/ const poptOption &	opt1,
	   /*[in]*/ const poptOption &	opt2)
{
  MIKTEX_ASSERT (opt1.longName != 0);
  MIKTEX_ASSERT (opt2.longName != 0);
  return (StringCompare(opt1.longName, opt2.longName, false) < 0);
}

void
WebApp::ProcessCommandLineOptions ()
{
  int argc = C4P::GetArgC();
  const char ** argv = C4P::GetArgV();
  
  if (options.size() == 0)
    {
      AddOptions ();
      sort (options.begin(), options.end());
      AddOption (0, 0);
    }

  popt.Construct (argc, argv, &options[0]);
  popt.SetOtherOptionHelp (GetUsage());

  int opt;

  while ((opt = popt.GetNextOpt()) >= 0)
    {
      if (! ProcessOption(opt, popt.GetOptArg()))
	{
	  FATAL_MIKTEX_ERROR ("WebApp::ProcessCommandLineOptions",
			      T_("Unprocessed command-line option."),
			      0);
	}
    }

  if (opt != -1)
    {
      FATAL_MIKTEX_ERROR ("WebApp::ProcessCommandLineOptions",
			  T_("Invalid command-line option"),
			  popt.Strerror(opt));
    }
      
  argv = popt.GetArgs();

  if (argv == 0)
    {
      C4P::MakeCommandLine (0, 0);
    }
  else
    {
      argc = 0;
      for (; argv[argc] != 0; ++ argc)
	{
	  ;
	}
      C4P::MakeCommandLine (argc, argv);
    }
}

/* _________________________________________________________________________

   WebApp::TheNameOfTheGame
   _________________________________________________________________________ */

const char *
WebApp::TheNameOfTheGame ()
  const
{
  return (theNameOfTheGame.c_str());
}

/* _________________________________________________________________________

   WebApp::ShowProgramversion
   _________________________________________________________________________ */

void
WebApp::ShowProgramVersion ()
  const
{
  cout << "MiKTeX" << '-' << TheNameOfTheGame()
       << ' ' << version
       << " (" << Utils::GetMiKTeXBannerString() << ')' << '\n'
       << copyright << '\n';
  if (! trademarks.empty())
    {
      cout << trademarks << '\n';
    }
  cout << flush;
}

/* _________________________________________________________________________
   
   CWebApp::SetProgramInfo
   _________________________________________________________________________ */

void
WebApp::SetProgramInfo (/*[in]*/ const char * lpszProgramName,
			/*[in]*/ const char * lpszVersion,
			/*[in]*/ const char * lpszCopyright,
			/*[in]*/ const char * lpszTrademarks)
{
  if (lpszProgramName != 0)
    {
      programName = lpszProgramName;
    }
  if (lpszVersion != 0)
    {
      version = lpszVersion;
    }
  if (lpszCopyright != 0)
    {
      copyright = lpszCopyright;
    }
  if (lpszTrademarks != 0)
    {
      trademarks = lpszTrademarks;
    }
}
