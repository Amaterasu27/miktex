/* mpm.cpp:

   Copyright (C) 2008-2009 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include <QApplication>
#include <miktex/Core/Core>

#include "MainWindow.h"

#include <popt-miktex.h>

using namespace MiKTeX::Core;
using namespace std;

enum {
  OPT_AAA = 1,

  OPT_ADMIN,
};

namespace {
  struct poptOption const aoption[] = {
    {
      "admin", 0,
      POPT_ARG_NONE, 0,
      OPT_ADMIN,
      "Run in administration mode.", 0
    },
    POPT_TABLEEND
  };
}

int
main (int	argc,
      char **	argv)
{
  QApplication application (argc, argv);
  int ret = 0;
  try
    {
      Session::InitInfo initInfo;
      Cpopt popt (argc,
		  const_cast<const char**>(argv),
		  aoption);
      int option;
      while ((option = popt.GetNextOpt()) >= 0)
	{
	  const char * lpszOptArg = popt.GetOptArg();
	  switch (option)
	    {
	    case OPT_ADMIN:
	      initInfo.SetFlags(initInfo.GetFlags()
				| Session::InitFlags::AdminMode);
	      break;
	    }
	}
      SessionWrapper pSession;
      initInfo.SetProgramInvocationName (argv[0]);
      pSession.CreateSession (initInfo);
      MainWindow mainWindow;
      mainWindow.show ();
      ret = application.exec();
    }
  catch (const MiKTeXException & e)
    {
      ret = 1;
    }
  catch (const exception & e)
    {
      ret = 1;
    }
  return (ret);
}
