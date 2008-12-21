/* touch.cpp:

   Copyright (C) 2008 Christian Schenk

   This file is part of miktex-touch.

   miktex-touch is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   miktex-touch is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with miktex-touch; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include <miktex/Core/Core>
#include <popt-miktex.h>

#include "touch-version.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <locale>

using namespace MiKTeX::Core;
using namespace std;

#define T_(x) MIKTEXTEXT(x)

/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 1000,
  OPT_TIME_STAMP,
  OPT_VERSION,
};

/* _________________________________________________________________________

   aoption
   _________________________________________________________________________ */

const struct poptOption aoption[] = {
  {
    "time-stamp", 0, POPT_ARG_STRING, 0, OPT_TIME_STAMP,
    T_("\
Use TIMESTAMP (elapsed seconds since 1970) instead of current time."),
    T_("TIMESTAMP")
  },
  
  {
    "version", 0, POPT_ARG_NONE, 0, OPT_VERSION,
    T_("Show version information and exit."), 0
  },
  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   FatalError
   _________________________________________________________________________ */

void
FatalError (/*[in]*/ const char *	lpszFormat,
	    /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  cerr << Utils::GetExeName() << ": "
       << Utils::FormatString(lpszFormat, arglist)
       << endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

void
Main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)
{
  int option;

  time_t timeStamp = time(0);

  Cpopt popt (argc, argv, aoption);

  popt.SetOtherOptionHelp (T_("[OPTION...] [FILE...]"));

  while ((option = popt.GetNextOpt()) >= 0)
    {
      const char * lpszOptArg = popt.GetOptArg();
      switch (option)
	{
	case OPT_TIME_STAMP:
	  {
	    long long t;
	    if (sscanf(lpszOptArg, "%lld", &t) != 1)
	      {
		FatalError (T_("Invalid time-stamp."));
	      }
	    timeStamp = static_cast<time_t>(t);
	    break;
	  }
	case OPT_VERSION:
	  cout <<
	    Utils::MakeProgramVersionString(Utils::GetExeName().c_str(),
					    VersionNumber(MIKTEX_MAJOR_VERSION,
							  MIKTEX_MINOR_VERSION,
							  MIKTEX_COMP_J2000_VERSION,
							  0))
	       << T_("\n\
Copyright (C) 2008 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
	       << endl;
	  return;
	}
    }

  if (option != -1)
    {
      string msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += ": ";
      msg += popt.Strerror(option);
      FatalError ("%s", msg.c_str());
    }
      
  const char ** leftovers = popt.GetArgs();

  if (leftovers == 0)
    {
      FatalError (T_("no file name arguments"));
    }
  
  for (int i = 0; *leftovers != 0; ++ leftovers, ++ i)
    {
      if (Directory::Exists(*leftovers))
	{
	  Directory::SetTimes (*leftovers, timeStamp, timeStamp, timeStamp);
	}
      else
	{
	  File::SetTimes (*leftovers, timeStamp, timeStamp, timeStamp);
	}
    }
}

int
main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)
{
  int exitCode;
  try
    {
      SessionWrapper pSession;
      Session::InitInfo initInfo;
      initInfo.SetProgramInvocationName (argv[0]);
      pSession.CreateSession (initInfo);
      Main (argc, argv);
      exitCode = 0;
    }
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      exitCode = 1;
    }
  catch (int r)
    {
      exitCode = r;
    }
  return (exitCode);
}
