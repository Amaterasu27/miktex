/* mpm.cpp:

   Copyright (C) 2008 Christian Schenk

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

using namespace MiKTeX::Core;
using namespace std;

int
main (int	argc,
      char **	argv)
{
  QApplication application (argc, argv);
  int ret = 0;
  try
    {
      SessionWrapper pSession;
      Session::InitInfo initInfo;
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
