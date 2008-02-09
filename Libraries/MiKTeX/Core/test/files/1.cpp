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

BEGIN_TEST_SCRIPT();

BEGIN_TEST_FUNCTION(1);
{
  TEST (! MiKTeX::Core::File::Exists("xxx.zzz"));
  Touch ("xxx.zzz");
  TEST (MiKTeX::Core::File::Exists("xxx.zzz"));
  TESTX (MiKTeX::Core::File::SetAttributes("xxx.zzz",
				     MiKTeX::Core::FileAttributes::ReadOnly));
  if (verbose)
    {
      system ("ls -l xxx.zzz");
    }
  MiKTeX::Core::FileAttributes attributes =
    MiKTeX::Core::File::GetAttributes("xxx.zzz");
  TEST ((attributes & MiKTeX::Core::FileAttributes::ReadOnly) != 0);
  TESTX (MiKTeX::Core::File::SetAttributes("xxx.zzz",
				     MiKTeX::Core::FileAttributes::Normal));
  attributes = MiKTeX::Core::File::GetAttributes("xxx.zzz");
  TEST ((attributes & MiKTeX::Core::FileAttributes::ReadOnly) == 0);
  TESTX (MiKTeX::Core::File::Move("xxx.zzz", "zzz.xxx"));
  TESTX (MiKTeX::Core::File::Delete("zzz.xxx"));
}
END_TEST_FUNCTION();

BEGIN_TEST_FUNCTION(2);
{
  FILE * stream =
    MiKTeX::Core::File::Open("abc.def",
		       MiKTeX::Core::FileMode::Create,
		       MiKTeX::Core::FileAccess::Write,
		       true,
		       MiKTeX::Core::FileShare::None);
  TEST (stream != 0);
  fprintf (stream, "hello, world!\n");
  fclose (stream);
  TESTX (MiKTeX::Core::File::Delete("abc.def"));
}
END_TEST_FUNCTION();

BEGIN_TEST_FUNCTION(3);
{
  MiKTeX::Core::PathName dir;
  dir.SetToCurrentDirectory ();
  dir += "d";
  MiKTeX::Core::PathName subdir (dir);
  subdir += "dd/ddd";
  TESTX (MiKTeX::Core::Directory::Create(subdir));
  TEST (MiKTeX::Core::Directory::Exists(subdir));
  TEST (! MiKTeX::Core::File::Exists(subdir));
  MiKTeX::Core::FileAttributes attributes = MiKTeX::Core::File::GetAttributes(subdir);
  TEST ((attributes & MiKTeX::Core::FileAttributes::Directory) != 0);
  MiKTeX::Core::PathName file = subdir;
  file += "file.txt";
  Touch (file.Get());
  TEST (MiKTeX::Core::File::Exists(file));
  TESTX (MiKTeX::Core::Directory::Delete(dir, true));
}
END_TEST_FUNCTION();

BEGIN_TEST_PROGRAM();
{
  CALL_TEST_FUNCTION (1);
  CALL_TEST_FUNCTION (2);
  CALL_TEST_FUNCTION (3);
}
END_TEST_PROGRAM();

END_TEST_SCRIPT();

RUN_TEST_SCRIPT ();
