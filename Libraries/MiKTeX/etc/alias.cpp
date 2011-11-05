/* alias.cpp: MiKTeX .exe alias

   Copyright (C) 1999-2011 Christian Schenk

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
#include <MiKTeX/Core/Core>

#if ! IN_PROCESS
#  include <process.h>
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

   MyMain (in process)
   _________________________________________________________________________ */

#if IN_PROCESS
static
int
MyMain (/*[in]*/ int		argc,
        /*[in]*/ const char **	argv)
{
  return (DLLMAIN(argc, argv));
}
#endif

/* _________________________________________________________________________

   MyMain (out of process)
   _________________________________________________________________________ */

#if ! IN_PROCESS
static
int
MyMain (/*[in]*/ int		argc,
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
      pSession.Reset ();
      std::vector<std::wstring> wargs;
      wargs.reserve (argc);
      for (int idx = 0; idx < argc; ++ idx)
	{
	  wargs.push_back (MiKTeX::Core::Utils::UTF8ToWideChar(argv[idx]));
	}
      std::vector<const wchar_t *> wargv;
      wargv.reserve (wargs.size() + 1);
      for (std::vector<std::wstring>::const_iterator it = wargs.begin(); it != wargs.end(); ++ it)
	{
	  wargv.push_back (it->c_str());
	}
      wargv.push_back (0);
      int exitCode = _wspawnv(_P_WAIT, prog.ToWideCharString().c_str(), &wargv[0]);
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

/* _________________________________________________________________________

   [w]main
   _________________________________________________________________________ */


int
MIKTEXCEECALL
#if defined(_UNICODE)
wmain (/*[in]*/ int			argc,
       /*[in]*/ const wchar_t **	argv)
#else
main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)
#endif
{
  std::vector<std::string> utf8args;
#if defined(PRE_ARGV)
  utf8args.reserve (argc + pre_argc);
#else
  utf8args.reserve (argc);
#endif
#if defined(_UNICODE)
  utf8args.push_back (MiKTeX::Core::Utils::WideCharToUTF8(argv[0]));
#else
  utf8args.push_back (MiKTeX::Core::Utils::AnsiToUTF8(argv[0]));
#endif
#if defined(PRE_ARGV)
  for (int idx = 0; idx < pre_argc; ++ idx)
    {
      utf8args.push_back (pre_argv[idx]);
    }
#endif
  for (int idx = 1; idx < argc; ++ idx)
    {
#if defined(_UNICODE)
      utf8args.push_back (MiKTeX::Core::Utils::WideCharToUTF8(argv[idx]));
#else
      utf8args.push_back (MiKTeX::Core::Utils::AnsiToUTF8(argv[idx]));
#endif
    }
  std::vector<const char *> args;
  args.reserve (utf8args.size() + 1);
  for (std::vector<std::string>::const_iterator it = utf8args.begin(); it != utf8args.end(); ++ it)
    {
      args.push_back (it->c_str());
    }
  args.push_back (0);
  return (MyMain(utf8args.size(), &args[0]));
}
