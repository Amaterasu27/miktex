/* wrapper.cpp: wrap a main function

   Copyright (C) 2004-2006 Christian Schenk

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

#include <cstdlib>

#include <MiKTeX/Core/Core>
#include <MiKTeX/App/Application>

#if ! defined(MAINFUNC)
#  define MAINFUNC Main
#endif

#if defined(_MSC_VER)
#  define MAINAPI __cdecl
#  define main _tmain
#else
#  define MAINAPI
#endif

#if ! defined(CPLUSPLUSMAIN)
extern "C"
#endif
int
MAINAPI
MAINFUNC (/*[in]*/ int			argc,
	  /*[in]*/ MIKTEXCHAR **	argv);

// Keep the application object in the global scope (C functions might
// call exit())
MiKTeX::App::Application app;

int
MAINAPI
main (/*[in]*/ int		argc,
      /*[in]*/ MIKTEXCHAR **	argv)
{
  try
    {
#if defined(APPNAMEx) // <fixme/>
      MiKTeX::Core::PushAppName (MIKTEXTEXT(APPNAME));
#endif

      app.Init (argv[0]);
      
#if defined(DISABLE_INSTALLER)
      app.EnableInstaller (MiKTeX::Core::TriState::False);
#endif

#if defined(BEQUIET)
      app.SetQuietFlag (true);
#endif

      int exitCode = MAINFUNC(argc, argv);
      
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
