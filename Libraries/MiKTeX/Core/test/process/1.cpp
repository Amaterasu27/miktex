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
  PathName pathExe =
    pSession->GetSpecialPath(SpecialPath::BinDirectory);
  pathExe += "1-1" MIKTEX_EXE_FILE_SUFFIX;
  int exitCode;
  TEST (Process::Run(pathExe.Get(),
		     "a.txt",
		     0,
		     &exitCode,
		     0));
  TEST (exitCode == 0);
  TEST (File::Exists("a.txt"));
  TESTX (File::Delete("a.txt", true));
}
END_TEST_FUNCTION();

string outputBuffer;

BEGIN_TEST_FUNCTION(2);
{
  PathName pathExe =
    pSession->GetSpecialPath(SpecialPath::BinDirectory);
  pathExe += "1-2" MIKTEX_EXE_FILE_SUFFIX;
  int exitCode;
  ProcessOutput processOutput;
  TEST (Process::Run(pathExe.Get(),
		     "hello world!",
		     &processOutput,
		     &exitCode,
		     0));
  TEST (exitCode == 0);
  TEST (processOutput.GetOutput() == "hello\nworld!\n");
}
END_TEST_FUNCTION();

BEGIN_TEST_FUNCTION(3);
{
  PathName pathExe = 
    pSession->GetSpecialPath(SpecialPath::BinDirectory);
  pathExe += "1-2" MIKTEX_EXE_FILE_SUFFIX;
  int exitCode;
  char buf[100];
  size_t n = 10;
  TEST (Process::Run(pathExe.Get(),
		     "01234567890123456789",
		     buf,
		     &n,
		     &exitCode));
  TEST (exitCode == 0);
  TEST (n == 10);
  std::string str (buf, n);
  TEST (str == "0123456789");
  n = 100;
  TEST (Process::Run(pathExe.Get(),
		     "01234567890123456789",
		     buf,
		     &n,
		     &exitCode));
  TEST (exitCode == 0);
  TEST (n == 21);
  str.assign (buf, n);
  TEST (str == "01234567890123456789\n");
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
