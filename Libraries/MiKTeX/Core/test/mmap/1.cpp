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

#include <MiKTeX/Core/Test>

BEGIN_TEST_SCRIPT();

BEGIN_TEST_FUNCTION(1);
{
  StreamWriter writer (T_("test1"));
  writer.WriteLine (T_("hello, world!"));
  writer.Close ();
  auto_ptr<MemoryMappedFile> mmap (MemoryMappedFile::Create());
  TESTX (mmap->Open(T_("test1"), true));
  MIKTEXCHAR * lpsz = (reinterpret_cast<MIKTEXCHAR *>(mmap->GetPtr()));
  TEST (memcmp(lpsz, T_("hello, world!"), 13 * sizeof(MIKTEXCHAR)) == 0);
  memcpy (lpsz, T_("uryyb, jbeyq!"), 13 * sizeof(MIKTEXCHAR));
  TESTX (mmap->Close());
  StreamReader reader (T_("test1"));
  tstring line;
  TEST (reader.ReadLine(line));
  reader.Close ();
  TEST (memcmp(line.c_str(), T_("uryyb, jbeyq!"), 13 * sizeof(MIKTEXCHAR))
	== 0);
  TESTX (File::Delete(T_("test1")));
}
END_TEST_FUNCTION();

BEGIN_TEST_FUNCTION(2);
{
  FileStream stream;
  stream.Attach (File::Open(T_("test2"),
			    FileMode::Create,
			    FileAccess::Write,
			    false));
  char buf[1024];
  memset (buf, 0xcc, 1024);
  stream.Write (buf, 1024);
  stream.Close ();
  auto_ptr<MemoryMappedFile> mmap (MemoryMappedFile::Create());
  TEST (mmap->Open(T_("test2"), true) != 0);
  TEST (mmap->Resize(2048) != 0);
  memcpy (static_cast<char*>(mmap->GetPtr()) + 1024, buf, 1024);
  TESTX (mmap->Close());
  TEST (File::GetSize(T_("test2")) == 2048);
  TEST (MD5::FromFile(T_("test2"))
	== MD5::Parse(T_("e36f492c295ce23fdcfbf63fd665072b")));
  TESTX (File::Delete(T_("test2")));
}
END_TEST_FUNCTION();

BEGIN_TEST_PROGRAM();
{
  CALL_TEST_FUNCTION (1);
  CALL_TEST_FUNCTION (2);
}
END_TEST_PROGRAM();

END_TEST_SCRIPT();

RUN_TEST_SCRIPT ();
