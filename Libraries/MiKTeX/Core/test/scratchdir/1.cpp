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

#include <miktex/test.h>

BEGIN_TEST_SCRIPT();

BEGIN_TEST_FUNCTION(2);
{
  TEST (FindFile(T_("a.txt"), T_(".")));
  TEST (RunSystemCommand(T_("1-1")) == 0);
}
END_TEST_FUNCTION();

void
EnterMany (/*[in]*/ int n)
{
  MiKTeX::Core::ScratchDirectory scratchDir;
  scratchDir.Enter (T_("y"));
  if (n > 0)
    {
      EnterMany (n - 1);
    }
  CALL_TEST_FUNCTION(2);
}

BEGIN_TEST_FUNCTION(1);
{
  MiKTeX::Core::PathName cwdOrig;
  cwdOrig.SetToCurrentDirectory ();
  MiKTeX::Core::ScratchDirectory scratchDir;
  scratchDir.Enter (T_("x"));
  MiKTeX::Core::PathName scratch1;
  scratch1.SetToCurrentDirectory ();
  TEST (MiKTeX::Core::PathName::Compare(scratch1, cwdOrig) != 0);
  MiKTeX::Core::PathName dir (T_("./x/y/z"));
  dir.MakeAbsolute ();
  MiKTeX::Core::Directory::Create (dir);
  Touch (T_("x/y/z/a.txt"));
  pSession->AddWorkingDirectory (dir.Get(), true);
  EnterMany (21);
  scratchDir.Leave ();
  TEST (! MiKTeX::Core::Directory::Exists(scratch1));
  MiKTeX::Core::PathName cwd;
  cwd.SetToCurrentDirectory ();
  TEST (MiKTeX::Core::PathName::Compare(cwd, cwdOrig) == 0);
}
END_TEST_FUNCTION();

BEGIN_TEST_PROGRAM();
{
  CALL_TEST_FUNCTION (1);
}
END_TEST_PROGRAM();

END_TEST_SCRIPT();

RUN_TEST_SCRIPT ();
