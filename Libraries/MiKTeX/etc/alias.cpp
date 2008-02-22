/* alias.cpp: MiKTeX .exe alias

   Copyright (C) 1999-2008 Christian Schenk

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

#include <memory>

#if defined(PRE_ARGV)
#  include <crtdbg.h>
#  include <malloc.h>
#endif

#if defined(REAL_NAME)
#  define IN_PROCES 0
#else
#  define IN_PROCESS 1
#endif

#include <miktex/Core/Definitions>

#if ! IN_PROCESS
#  include <process.h>
#  include <MiKTeX/Core/Core>
#  include <cerrno>
#  include <cstdio>
#endif

#if IN_PROCESS

#  if ! defined(DLLMAIN)
#    define DLLMAIN c4pmain
#  endif

#  if ! defined(DLLAPI)
#    define DLLAPI MIKTEXCEECALL
#  endif

extern
"C"
int
DLLAPI
DLLMAIN (/*[in]*/ int		argc,
	 /*[in]*/ const char **	argv);

#endif // IN_PROCESS

#if defined(PRE_ARGV)
namespace {
  const char * const pre_argv[] = { PRE_ARGV };
  const size_t pre_argc = (sizeof(pre_argv) / sizeof(pre_argv[0]));
}
#endif

/* _________________________________________________________________________

   main (in process)
   _________________________________________________________________________ */

#if IN_PROCESS
int
MIKTEXCEECALL
main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)
{
#if defined(PRE_ARGV)
  int newargc = (argc + 1/*NULL*/) + pre_argc;
  char ** newargv =
    reinterpret_cast<char**>(_alloca(sizeof(char *) * newargc));
  newargv[0] = const_cast<char*>(argv[0]);
  size_t idx;
  for (idx = 0; idx < pre_argc; ++ idx)
    {
      newargv[idx + 1] = const_cast<char*>(pre_argv[idx]);
    }
  for (idx = 1; idx <= argc; ++ idx)
    {
      newargv[idx + pre_argc] = const_cast<char*>(argv[idx]);
    }
#else // ! PRE_ARGV
#  define newargc argc
#  define newargv argv
#endif // ! PRE_ARGV
  return (DLLMAIN(newargc, newargv));
}
#endif

/* _________________________________________________________________________

   main (out of process)
   _________________________________________________________________________ */

#if ! IN_PROCESS
int
MIKTEXCEECALL
main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)
{
  try
    {
      MiKTeX::Core::SessionWrapper pSession;
      pSession.CreateSession (MiKTeX::Core::Session::InitInfo(argv[0]));

      MiKTeX::Core::PathName prog;
      if (! pSession->FindFile(REAL_NAME,
			       MiKTeX::Core::FileType::EXE,
			       prog))
	{
	  fprintf (stderr,
		   "The %s executable could not be found.\n",
		   REAL_NAME);
	  throw (1);
	}

#if defined(PRE_ARGV)
      int newargc = (argc + 1/*NULL*/) + pre_argc;
      char ** newargv =
	reinterpret_cast<char**>(_alloca(sizeof(char *) * newargc));
      newargv[0] = const_cast<char*>(argv[0]);
      size_t idx;
      for (idx = 0; idx < pre_argc; ++ idx)
	{
	  newargv[idx + 1] = const_cast<char*>(pre_argv[idx]);
	}
      for (idx = 1; idx <= argc; ++ idx)
	{
	  newargv[idx + pre_argc] = const_cast<char*>(argv[idx]);
	}
#else // ! PRE_ARGV
#  define newargc argc
#  define newargv argv
#endif // ! PRE_ARGV

      pSession.Reset ();

      int exitCode = _spawnv(_P_WAIT, prog.Get(), newargv);

      if (exitCode < 0 && errno > 0)
	{
	  perror (prog.Get());
	  throw (1);
	}

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
#endif
