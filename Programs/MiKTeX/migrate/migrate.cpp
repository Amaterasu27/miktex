/* migrate.cpp: MiKTeX migration tool

   Copyright (C) 2006 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.
   
   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with This file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  */

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <atlbase.h>

#include <miktex/core.h>
#include <miktex/reg.h>

using namespace std;
using namespace MiKTeX::Core;

#define UNUSED_ALWAYS(x) (x)

#define T_(x) MIKTEXTEXT(x)

#if defined(MIKTEX_UNICODE)
#  define tcout wcout
#  define tcerr wcerr
#else
#  define tcout cout
#  define tcerr cerr
#endif

/* _________________________________________________________________________

   RegistrySettings24
   _________________________________________________________________________ */

struct RegistrySettings24
{
  bool sharedMiKTeX;
  tstring roots;
  tstring installRoot;
  tstring localRoot;
};

/* _________________________________________________________________________

   ReadRegistry24
   _________________________________________________________________________ */

RegistrySettings24
ReadRegistry24 ()
{
  RegistrySettings24 settings;

  const size_t MAX_VALUE = 8192;
  MIKTEXCHAR szValue[MAX_VALUE];
  ULONG valueSize;

  CRegKey hklmMiKTeX;
  CRegKey hkcuMiKTeX;

  bool haveHklmMiKTeX =
    (hklmMiKTeX.Open(HKEY_LOCAL_MACHINE,
		     T_("SOFTWARE\\MiK\\MiKTeX\\CurrentVersion\\MiKTeX"),
		     KEY_READ)
     == ERROR_SUCCESS);

  bool haveHkcuMiKTeX =
    (hkcuMiKTeX.Open(HKEY_CURRENT_USER,
		     T_("Software\\MiK\\MiKTeX\\CurrentVersion\\MiKTeX"),
		     KEY_READ)
     == ERROR_SUCCESS);

  valueSize = MAX_VALUE;

  settings.sharedMiKTeX =
    (haveHklmMiKTeX
     && (hklmMiKTeX.QueryStringValue(T_("SharedSetup"),
				     szValue,
				     &valueSize)
	 == ERROR_SUCCESS)
     && _tcscmp(szValue, T_("1")) == 0);
  
  if (settings.sharedMiKTeX)
    {
      valueSize = MAX_VALUE;
      if (! (hklmMiKTeX.QueryStringValue(T_("TeXMF Root Directories"),
					 szValue,
					 &valueSize)
	     == ERROR_SUCCESS))
	{
	  throw T_("texmf root directories key could not be read");
	}
      settings.roots = szValue;
      valueSize = MAX_VALUE;
      if (! (hklmMiKTeX.QueryStringValue(T_("Install Root"),
					 szValue,
					 &valueSize)
	     == ERROR_SUCCESS))
	{
	  throw T_("install root key could not be read");
	}
      settings.installRoot = szValue;
      valueSize = MAX_VALUE;
      if (! (hklmMiKTeX.QueryStringValue(T_("Local Root"),
					 szValue,
					 &valueSize)
	     == ERROR_SUCCESS))
	{
	  throw T_("local root key could not be read");
	}
      settings.localRoot = szValue;
    }
  else if (! haveHkcuMiKTeX)
    {
      throw T_("no registry settings");
    }
  else
    {
      valueSize = MAX_VALUE;
      if (! (hkcuMiKTeX.QueryStringValue(T_("TeXMF Root Directories"),
					 szValue,
					 &valueSize)
	     == ERROR_SUCCESS))
	{
	  throw T_("texmf root directories value could not be read");
	}
      settings.roots = szValue;
      valueSize = MAX_VALUE;
      if (! (hkcuMiKTeX.QueryStringValue(T_("Install Root"),
					 szValue,
					 &valueSize)
	     == ERROR_SUCCESS))
	{
	  throw T_("install root value could not be read");
	}
      settings.installRoot = szValue;
      valueSize = MAX_VALUE;
      if (! (hkcuMiKTeX.QueryStringValue(T_("Local Root"),
					 szValue,
					 &valueSize)
	     == ERROR_SUCCESS))
	{
	  throw T_("local root key could not be read");
	}
      settings.localRoot = szValue;
    }
  return (settings);
}

/* _________________________________________________________________________

   TryMigrateRegistry24
   _________________________________________________________________________ */

bool
TryMigrateRegistry24 ()
{
  CRegKey root25;

  if ((root25.Open(HKEY_LOCAL_MACHINE, MIKTEX_REGPATH_SERIES, KEY_READ)
       == ERROR_SUCCESS)
      || (root25.Open(HKEY_CURRENT_USER, MIKTEX_REGPATH_SERIES, KEY_READ)
	  == ERROR_SUCCESS))
    {
      return (false);
    }

  root25.Close ();

  RegistrySettings24 settings24 = ReadRegistry24();
  
  Session::InitInfo initInfo (T_("migrate"),
			      Session::InitFlags::NoConfigFiles);
  
  SessionWrapper pSession (initInfo);
  
  StartupConfig startupConfig;
  
  startupConfig.roots = settings24.roots;
  startupConfig.installRoot = settings24.installRoot;
  
  bool doNothing = false;

  if (settings24.sharedMiKTeX)
    {
      pSession->SharedMiKTeXSetup (settings24.sharedMiKTeX, doNothing);
    }

  pSession->RegisterRootDirectories (startupConfig, doNothing);

  if (root25.Create((settings24.sharedMiKTeX
		     ? HKEY_LOCAL_MACHINE
		     : HKEY_CURRENT_USER),
		    MIKTEX_REGPATH_SERIES MIKTEX_REGKEY_MIGRATE)
      != ERROR_SUCCESS)
    {
      throw T_("the registry key could not be created");
    }

  if (root25.SetQWORDValue(MIKTEX_REGVAL_TIMESTAMP,
			   static_cast<ULONGLONG>(time(0)))
      != ERROR_SUCCESS)
    {
      throw T_("the registry key could not be written");
    }

  if (root25.SetDWORDValue(MIKTEX_REGVAL_VERSION,
			   VersionNumber(T_("2.4")).GetHighWord())
    != ERROR_SUCCESS)
    {
      throw T_("the registry key could not be written");
    }

  root25.Close ();

  return (true);
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int			argc,
      /*[int]*/ const MIKTEXCHAR **	argv)
{
  UNUSED_ALWAYS (argc);
  UNUSED_ALWAYS (argv);
  int successCode = 1;
  try
    {
      TryMigrateRegistry24 ();
      successCode = 0;
    }
  catch (const MIKTEXCHAR * lpszMessage)
    {
      tcerr << T_("error: ") << lpszMessage << endl;
    }
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
    }
  catch (const exception &)
    {
    }
  catch (int sc)
    {
      successCode = sc;
    }
  return (successCode);
}
