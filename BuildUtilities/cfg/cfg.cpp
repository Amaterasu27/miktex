/* cfg.cpp:

   Copyright (C) 2006 Christian Schenk

   This file is part of cfg.

   cfg is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   cfg is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with cfg; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include <miktex/core.h>
#include <popt-miktex.h>
#include "cfg-version.h"

#include <iomanip>
#include <iostream>

using namespace MiKTeX::Core;
using namespace std;

#define T_(x) MIKTEXTEXT(x)

#if defined(MIKTEX_UNICODE)
#  define tcout wcout
#  define tcerr wcerr
#else
#  define tcout cout
#  define tcerr cerr
#endif

/* _________________________________________________________________________

   TASK
   _________________________________________________________________________ */

enum TASK { None, ComputeDigest };

/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 1000,
  OPT_COMPUTE_DIGEST,
  OPT_VERSION,
};

/* _________________________________________________________________________

   aoption
   _________________________________________________________________________ */

const struct poptOption aoption[] = {
  {
    T_("compute-digest"), 0, POPT_ARG_NONE, 0, OPT_COMPUTE_DIGEST,
    T_("Compute the MD5."), 0,
  },
  {
    T_("version"), 0, POPT_ARG_NONE, 0, OPT_VERSION,
    T_("Show version information and exit."), 0
  },
  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   FatalError
   _________________________________________________________________________ */

void
FatalError (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	    /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcerr << Utils::GetExeName() << T_(": ")
	<< Utils::FormatString(lpszFormat, arglist)
	<< endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________

   PrintDigest
   _________________________________________________________________________ */

void
PrintDigest (/*[in]*/ const MD5 & md5)
{
  tcout << T_('@') <<  md5.ToString() << endl;
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

void
Main (/*[in]*/ int			argc,
      /*[in]*/ const MIKTEXCHAR **	argv)
{
  int option;

  Cpopt popt (argc, argv, aoption);

  popt.SetOtherOptionHelp (T_("[OPTION...] [DIRECTORY...]"));

  TASK task;

  task = ComputeDigest;

  while ((option = popt.GetNextOpt()) >= 0)
    {
      switch (option)
	{
	case OPT_COMPUTE_DIGEST:
	  task = ComputeDigest;
	  break;
	case OPT_VERSION:
	  tcout <<
	    Utils::MakeProgramVersionString(Utils::GetExeName().c_str(),
					    VersionNumber(VER_FILEVERSION))
		<< T_("\n\
Copyright (C) 2006 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
		<< endl;
	  return;
	}
    }

  if (option != -1)
    {
      tstring msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += T_(": ");
      msg += popt.Strerror(option);
      FatalError (T_("%s"), msg.c_str());
    }
      
  const MIKTEXCHAR ** leftovers = popt.GetArgs();

  if (leftovers == 0)
    {
      FatalError (T_("no file name arguments"));
    }
  
  for (int i = 0; *leftovers != 0; ++ leftovers, ++ i)
    {
      SmartPointer<Cfg> pCfg (Cfg::Create());
      pCfg->Read (*leftovers);
      if (task == ComputeDigest)
	{
	  PrintDigest (pCfg->GetDigest());
	}
    }
}

int
main (/*[in]*/ int			argc,
      /*[in]*/ const MIKTEXCHAR **	argv)
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
