/* wrapper.cpp: wrap a main function

   Copyright (C) 2004-2011 Christian Schenk

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
   USA.  */

#if defined(HAVE_STDAFX_H)
#  include "StdAfx.h"
#endif

#if defined(HAVE_CONFIG_H)
#  include "config.h"
#endif

#include <cstdlib>

#include <miktex/Core/Core>
#include <miktex/App/Application>

#include <vector>

#if ! defined(stringify_)
#  define stringify__(x) #x
#  define stringify_(x) stringify__(x)
#endif

#if defined(APPTAG)
#  define APPTAGSTR stringify_(APPTAG)
#endif

#if ! defined(MAINFUNC)
#  define MAINFUNC Main
#endif

#if ! defined(CPLUSPLUSMAIN)
extern "C"
#endif
int
MIKTEXCEECALL
MAINFUNC (/*[in]*/ int		argc,
	  /*[in]*/ char **	argv);

// Keep the application object in the global scope (C functions might
// call exit())
MiKTeX::App::Application app;

#if defined(main) && ! defined(_UNICODE)
#  undef main
#endif

int
MIKTEXCEECALL
#if defined(_UNICODE)
wmain (/*[in]*/ int		argc,
       /*[in]*/ wchar_t *	argv[])
#else
main (/*[in]*/ int		argc,
      /*[in]*/ char *		argv[])
#endif
{
  try
    {
#if defined(MIKTEX_WINDOWS)
      std::vector<std::string> utf8args;
      utf8args.reserve (argc);
#endif
      std::vector<char *> args;
      args.reserve (argc + 1);
      for (int idx = 0; idx < argc; ++ idx)
      {
#if defined(MIKTEX_WINDOWS)
#if defined(_UNICODE)
	utf8args.push_back (MiKTeX::Core::Utils::WideCharToUTF8(argv[idx]));
#else
	utf8args.push_back (MiKTeX::Core::Utils::AnsiToUTF8(argv[idx]));
#endif
	args.push_back (const_cast<char*>(utf8args[idx].c_str()));
#else
	args.push_back (argv[idx]);
#endif
      }
      args.push_back (0);

      app.Init (args);

#if defined(APPTAGSTR)
      app.GetSession()->PushBackAppName (APPTAGSTR);
#endif
      
#if defined(DISABLE_INSTALLER)
      app.EnableInstaller (MiKTeX::Core::TriState::False);
#endif

#if defined(BEQUIET)
      app.SetQuietFlag (true);
#endif

      int exitCode = MAINFUNC(args.size() - 1, &args[0]);
      
      app.Finalize ();

      return (exitCode);
    }
  catch (const MiKTeX::Core::MiKTeXException & e)
    {
      MiKTeX::Core::Utils::PrintException (e);
      return (EXIT_FAILURE);
    }
  catch (const std::exception & e)
    {
      MiKTeX::Core::Utils::PrintException (e);
      return (EXIT_FAILURE);
    }
  catch (int exitCode)
    {
      return (exitCode);
    }
}
