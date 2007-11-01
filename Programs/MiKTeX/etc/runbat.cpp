/* runbat.cpp: the MiKTeX batch file runner

   Copyright (C) 2004-2007 Christian Schenk

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

#include <miktex/app.h>

using namespace MiKTeX::App;
using namespace MiKTeX::Core;

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int			argc,
      /*[in]*/ const MIKTEXCHAR **	argv)
{
  try
    {
      Application app;
      app.Init (argv[0]);
      const char * lpszArguments = GetCommandLineA();
      bool inQuote = false;
      for (;
	   (*lpszArguments != 0
	    && (inQuote
		|| (*lpszArguments != ' '
		    && *lpszArguments != '\t')));
	   ++ lpszArguments)
	{
	  if (*lpszArguments == '"')
	    {
	      inQuote = ! inQuote;
	    }
	}
      for (;
	   *lpszArguments == ' ' || *lpszArguments == '\t';
	   ++ lpszArguments)
	{
	}
      char szName[BufferSizes::MaxPath];
      PathName::Split (argv[0],
		       0, 0,
		       szName, BufferSizes::MaxPath,
		       0, 0);
      Session * pSession = app.GetSession();
      Session2 * pSession2 = dynamic_cast<Session2*>(pSession);
      int exitCode;
      if (pSession == 0)
	{
	  fprintf (stderr, "Invalid MiKTeX configuration\n");
	  exitCode = 1;
	}
      else
	{
	  exitCode = pSession2->RunBatch(szName, lpszArguments);
	}
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
