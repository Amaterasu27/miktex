/* runperl.cpp: the MiKTeX Perl runner

   Copyright (C) 2001-2011 Christian Schenk

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

#include <miktex/App/Application>

#include <vector>
#include <string>

using namespace MiKTeX::App;
using namespace MiKTeX::Core;

using namespace std;

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
#if defined(_UNICODE)
wmain (/*[in]*/ int			argc,
       /*[in]*/ const wchar_t **	argv)
#else
int
main (/*[in]*/ int			argc,
      /*[in]*/ const char **		argv)
#endif
{
  try
    {
      vector<string> utf8args;
      utf8args.reserve (argc);
      vector<const char *> newargv;
      newargv.reserve (argc + 1);
      for (int idx = 0; idx < argc; ++ idx)
      {
#if defined(_UNICODE)
	utf8args.push_back (Utils::WideCharToUTF8(argv[idx]));
#else
	utf8args.push_back (Utils::AnsiToUTF8(argv[idx]));
#endif
	newargv.push_back (utf8args[idx].c_str());
      }
      newargv.push_back (0);
      Application app;
      app.Init (newargv[0]);
      int exitCode = app.GetSession()->RunPerl(argc, &newargv[0]);
      app.Finalize ();
      return (exitCode);
    }
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      return (1);
    }
  catch (const std::exception & e)
    {
      Utils::PrintException (e);
      return (1);
    }
}
