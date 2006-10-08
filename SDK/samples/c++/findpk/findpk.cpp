/* findpk.cpp: find a PK file
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

#include <cstdio>
#include <miktex/app.h>

using namespace MiKTeX::App;
using namespace MiKTeX::Core;

int
main (/*[in]*/ int			argc,
      /*[in]*/ const MIKTEXCHAR **	argv)
{
  if (argc != 4)
    {
      puts ("Usage: findpk FONTNAME MODE DPI");
      puts (" e.g.: findpk cmr10 ljfour 600");
      return (1);
    }

  try
    {
      // initialize MiKTeX
      Application app;
      app.Init (argv[0]);

      PathName result;
      int exitCode;

      // find the PK file
      if (app.GetSession()->FindPkFile(argv[1],
				       argv[2],
				       atoi(argv[3]),
				       result))
	{
	  puts (result.Get());
	  exitCode = 0;
	}
      else
	{
	  exitCode = 1;
	}

      // uninitialize MiKTeX
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
