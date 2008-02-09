/* 1.cpp:

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include <miktex/Core/Test>

#include <miktex/Core/Paths>

BEGIN_TEST_SCRIPT();

BEGIN_TEST_FUNCTION(1);
{
  PathName localRoot =
    pSession->GetSpecialPath(SpecialPath::UserDataRoot);

  PathName installRoot =
    pSession->GetSpecialPath(SpecialPath::InstallRoot);
  
  PathName pathConfigDir (localRoot);
  pathConfigDir += MIKTEX_PATH_MIKTEX_CONFIG_DIR;
  TESTX (Directory::Create(pathConfigDir));

  TEST (Directory::Exists(localRoot));

  PathName fndb0 = pSession->GetFilenameDatabasePathName(0);
  TEST (Fndb::Create(fndb0.Get(), localRoot.Get(), 0));
  TEST (File::Exists(fndb0));

  PathName fndb1 = pSession->GetFilenameDatabasePathName (1);
  TEST (Fndb::Create(fndb1.Get(), installRoot.Get(), 0));
  TEST (File::Exists(fndb1));
}
END_TEST_FUNCTION();

BEGIN_TEST_FUNCTION(2);
{
  PathName path;
  TEST (pSession->FindFile("test.tex", "%R/tex//", path));
  if (File::Exists("./test.tex"))
    {
      File::Delete ("./test.tex");
    }
  TEST (! pSession->FindFile("./test.tex", "%R/tex//", path));
  Touch ("./test.tex");
  TEST (pSession->FindFile("./test.tex", "%R/tex//", path));
  path.MakeAbsolute ();
  PathName path2;
  path2.SetToCurrentDirectory();
  path2 += "test.tex";
  TEST (path == path2);
  File::Delete ("./test.tex");
}
END_TEST_FUNCTION();

BEGIN_TEST_PROGRAM();
{
  CALL_TEST_FUNCTION (1) ;
  CALL_TEST_FUNCTION (2) ;
}
END_TEST_PROGRAM();

END_TEST_SCRIPT();

RUN_TEST_SCRIPT ();

