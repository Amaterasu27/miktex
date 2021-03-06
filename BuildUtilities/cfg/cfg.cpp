/* cfg.cpp:

   Copyright (C) 2006-2011 Christian Schenk

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

#if defined(_MSC_VER)
#  include <conio.h>
#endif

#include <miktex/Core/Core>
#include <popt-miktex.h>
#include "cfg-version.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <locale>

using namespace MiKTeX::Core;
using namespace std;

#define T_(x) MIKTEXTEXT(x)

/* _________________________________________________________________________

   TASK
   _________________________________________________________________________ */

enum TASK { None, ComputeDigest, PrintClasses, Sign };

/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 1000,
  OPT_COMPUTE_DIGEST,
  OPT_PRINT_CLASSES,
  OPT_PRIVATE_KEY_FILE,
  OPT_SIGN,
  OPT_VERSION,
};

/* _________________________________________________________________________

   aoption
   _________________________________________________________________________ */

const struct poptOption aoption[] = {
  {
    "compute-digest", 0, POPT_ARG_NONE, 0, OPT_COMPUTE_DIGEST,
    T_("Compute the MD5."), 0,
  },
  {
    "print-classes", 0, POPT_ARG_NONE, 0, OPT_PRINT_CLASSES,
    T_("Print C++ class definitions."), 0,
  },
  {
    "private-key-file", 0, POPT_ARG_STRING, 0, OPT_PRIVATE_KEY_FILE,
    T_("The private key file used for signing."), T_("FILE"),
  },
  {
    "sign", 0, POPT_ARG_NONE, 0, OPT_SIGN,
    T_("Sign the cfg file."), 0,
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

   PrintDigest
   _________________________________________________________________________ */

void
PrintDigest (/*[in]*/ const MD5 & md5)
{
  cout << md5.ToString() << endl;
}

/* _________________________________________________________________________

   
   _________________________________________________________________________ */

char
DownChar (/*[in]*/ char c) 
{ 
  const static locale loc;
  if (isalnum(c, loc))
    {
      return (tolower(c, loc));
    }
  else
    {
      return ('_');
    }
}

/* _________________________________________________________________________

   
   _________________________________________________________________________ */

string
ToLower (/*[in]*/ const string & s)
{ 
  string result (s);
  transform (result.begin(),
	     result.end(),
	     result.begin(),
	     DownChar);
  return (result);
}

/* _________________________________________________________________________

   
   _________________________________________________________________________ */

string
ToStr (/*[in]*/ const string & s)
{
  string result;
  for (string::const_iterator it = s.begin(); it != s.end(); ++ it)
    {
      switch (*it)
	{
	case '\\':
	  result += "\\\\";
	  break;
	case '"':
	  result += "\\\"";
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
  char szKey[BufferSizes::MaxCfgName];
  for (char * lpszKey = pCfg->FirstKey(szKey, BufferSizes::MaxCfgName);
       lpszKey != 0;
       lpszKey = pCfg->NextKey(szKey, BufferSizes::MaxCfgName))
    {
      cout << T_("class ") << ToLower(lpszKey) << " {" << endl;
      char szValueName[BufferSizes::MaxCfgName];
      for (char * lpszValueName =
	     pCfg->FirstValue(lpszKey, szValueName, BufferSizes::MaxCfgName);
	   lpszValueName != 0;
	   lpszValueName =
	     pCfg->NextValue(szValueName, BufferSizes::MaxCfgName))
	{
	  string value = pCfg->GetValue(lpszKey, lpszValueName);
	  char * endptr = 0;
	  strtol (value.c_str(), &endptr, 0);
	  bool isString = (endptr != 0 && *endptr != 0);
	  cout << T_("  public: static ");
	  if (isString)
	    {
	      cout << "std::string";
	    }
	  else
	    {
	      cout << T_("int");
	    }
	  cout << " " << ToLower(lpszValueName)
	       << T_(" () { return (");
	  if (isString)
	    {
	      cout << '"' << ToStr(value) << '"';
	    }
	  else
	    {
	      cout << value;
	    }
	  cout << "); }" << endl;
	}
      cout << "};" << endl;
    }
}

/* _________________________________________________________________________

   PrivateKeyProvider
   _________________________________________________________________________ */


class PrivateKeyProvider : public IPrivateKeyProvider
{
public:
  PrivateKeyProvider (/*[in]*/ const PathName & privateKeyFile)
    : privateKeyFile (privateKeyFile)
  {
  }
public:
  virtual
  PathName
  MIKTEXTHISCALL
  GetPrivateKeyFile ()
  {
    return (privateKeyFile);
  }
public:
  virtual
  bool
  GetPassphrase (/*[out]*/ std::string & passphrase)
  {
    fputs (T_("Passphrase: "), stdout);
#if defined(MIKTEX_WINDOWS)
    const int EOL = '\r';
#else
    const inT EOL = '\n';
#endif
    passphrase = "";
    int ch;
    while ((ch = getch()) != EOL)
    {
      passphrase += ch;
    }
    putchar ('\n');
    return (true);
  }
private:
  PathName privateKeyFile;
};

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

void
Main (/*[in]*/ int			argc,
      /*[in]*/ const char **	argv)
{
  int option;

  Cpopt popt (argc, argv, aoption);

  popt.SetOtherOptionHelp (T_("[OPTION...] CFGFILE..."));

  TASK task = ComputeDigest;

  PathName privateKeyFile;

  while ((option = popt.GetNextOpt()) >= 0)
    {
      const char * lpszOptArg = popt.GetOptArg();
      switch (option)
	{
	case OPT_COMPUTE_DIGEST:
	  task = ComputeDigest;
	  break;
	case OPT_PRINT_CLASSES:
	  task = PrintClasses;
	  break;
	case OPT_PRIVATE_KEY_FILE:
	  privateKeyFile = lpszOptArg;
	  break;
	case OPT_SIGN:
	  task = Sign;
	  break;
	case OPT_VERSION:
	  cout <<
	    Utils::MakeProgramVersionString(Utils::GetExeName().c_str(),
					    VersionNumber(MIKTEX_MAJOR_VERSION,
							  MIKTEX_MINOR_VERSION,
							  MIKTEX_COMP_J2000_VERSION,
							  0))
	       << T_("\n\
Copyright (C) 2006-2011 Christian Schenk\n\
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
    else if (task == Sign)
    {
      PrivateKeyProvider privateKeyProvider (privateKeyFile);
      pCfg->Write (*leftovers, 0, &privateKeyProvider);
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
  catch (const exception & e)
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
