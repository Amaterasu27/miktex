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

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <locale>

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

enum TASK { None, ComputeDigest, PrintClasses };

/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 1000,
  OPT_COMPUTE_DIGEST,
  OPT_PRINT_CLASSES,
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
    T_("print-classes"), 0, POPT_ARG_NONE, 0, OPT_PRINT_CLASSES,
    T_("Print C++ class definitions."), 0,
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

   
   _________________________________________________________________________ */

MIKTEXCHAR
DownChar (/*[in]*/ MIKTEXCHAR c) 
{ 
  const static locale loc;
  if (isalnum(c, loc))
    {
      return (tolower(c, loc));
    }
  else
    {
      return (T_('_'));
    }
}

/* _________________________________________________________________________

   
   _________________________________________________________________________ */

tstring
ToLower (/*[in]*/ const tstring & s)
{ 
  tstring result (s);
  transform (result.begin(),
	     result.end(),
	     result.begin(),
	     DownChar);
  return (result);
}

/* _________________________________________________________________________

   
   _________________________________________________________________________ */

tstring
ToStr (/*[in]*/ const tstring & s)
{
  tstring result;
  for (tstring::const_iterator it = s.begin(); it != s.end(); ++ it)
    {
      switch (*it)
	{
	case T_('\\'):
	  result += T_("\\\\");
	  break;
	case T_('"'):
	  result += T_("\\\"");
	  break;
	default:
	  result += *it;
	  break;
	}
    }
  return (result);
}

/* _________________________________________________________________________

   DoPrintClasses
   _________________________________________________________________________ */

void
DoPrintClasses (/*[in]*/ Cfg * pCfg)
{
  MIKTEXCHAR szKey[BufferSizes::MaxCfgName];
  for (MIKTEXCHAR * lpszKey = pCfg->FirstKey(szKey, BufferSizes::MaxCfgName);
       lpszKey != 0;
       lpszKey = pCfg->NextKey(szKey, BufferSizes::MaxCfgName))
    {
      tcout << T_("class ") << ToLower(lpszKey) << T_(" {") << endl;
      MIKTEXCHAR szValueName[BufferSizes::MaxCfgName];
      for (MIKTEXCHAR * lpszValueName =
	     pCfg->FirstValue(lpszKey, szValueName, BufferSizes::MaxCfgName);
	   lpszValueName != 0;
	   lpszValueName =
	     pCfg->NextValue(szValueName, BufferSizes::MaxCfgName))
	{
	  tstring value = pCfg->GetValue(lpszKey, lpszValueName);
	  MIKTEXCHAR * endptr = 0;
	  strtol (value.c_str(), &endptr, 0);
	  bool isString = (endptr != 0 && *endptr != 0);
	  tcout << T_("  public: static ");
	  if (isString)
	    {
	      tcout << T_("std::string");
	    }
	  else
	    {
	      tcout << T_("int");
	    }
	  tcout << T_(" ") << ToLower(lpszValueName)
		<< T_(" () { return (");
	  if (isString)
	    {
	      tcout << T_('"') << ToStr(value) << T_('"');
	    }
	  else
	    {
	      tcout << value;
	    }
	  tcout << T_("); }") << endl;
	}
      tcout << T_("};") << endl;
    }
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
	case OPT_PRINT_CLASSES:
	  task = PrintClasses;
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
      else if (task == PrintClasses)
	{
	  DoPrintClasses (pCfg.Get());
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
