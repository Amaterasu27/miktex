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
  TEST (PathName::Compare (T_("//aBc/[e]/ghi.jkl"),
			   T_("//aBc/[e]/qwe.rty"),
			   10)
	== 0);
  TEST (PathName::Compare (T_("/abc/def/ghi.jkl"),
			   T_("/abc/def/qwe.rty"),
			   10)
	!= 0);
  MIKTEXCHAR szDir[BufferSizes::MaxPath];
  MIKTEXCHAR szName[BufferSizes::MaxPath];
  MIKTEXCHAR szExt[BufferSizes::MaxPath];
  PathName::Split (T_("/abc/def/ghi.jkl"),
		   szDir, BufferSizes::MaxPath,
		   szName, BufferSizes::MaxPath,
		   szExt, BufferSizes::MaxPath);
  TEST (PathName::Compare(szDir, T_("/abc/def")) != 0);
  TEST (PathName::Compare(szDir, T_("/abc/def/")) == 0);
  TEST (PathName::Compare(szName, T_("ghi")) == 0);
  TEST (PathName::Compare(szExt, T_(".jkl")) == 0);
  TEST (PathName::Compare(szExt, T_("jkl")) != 0);
}
END_TEST_FUNCTION();

BEGIN_TEST_FUNCTION(2);
{
#if defined(MIKTEX_WINDOWS)
  PathNameParser component (T_("C:/abc/def/ghi.jkl"));
  TEST (PathName::Compare(component.GetCurrent(), T_("C:")) == 0);
  ++ component;
  TEST (PathName::Compare(component.GetCurrent(), T_("abc")) == 0);
  ++ component;
  TEST (PathName::Compare(component.GetCurrent(), T_("def")) == 0);
  ++ component;
  TEST (PathName::Compare(component.GetCurrent(), T_("ghi.jkl")) == 0);
  ++ component;
  TEST (component.GetCurrent() == 0);
#endif
  PathNameParser component2 (T_("/abc/def/ghi.jkl"));
  TEST (PathName::Compare(component2.GetCurrent(), T_("/abc")) == 0);
  ++ component2;
  TEST (PathName::Compare(component2.GetCurrent(), T_("def")) == 0);
  ++ component2;
  TEST (PathName::Compare(component2.GetCurrent(), T_("ghi.jkl"))
	== 0);
  ++ component2;
  TEST (component2.GetCurrent() == 0);
}
END_TEST_FUNCTION();

BEGIN_TEST_FUNCTION(3);
{
  PathNameParser component (T_("//abc/def/ghi.jkl"));
  TEST (PathName::Compare(component.GetCurrent(), T_("//abc")) == 0);
  ++ component;
  TEST (PathName::Compare(component.GetCurrent(), T_("def")) == 0);
  ++ component;
  TEST (PathName::Compare(component.GetCurrent(), T_("ghi.jkl")) == 0);
  ++ component;
  TEST (component.GetCurrent() == 0);
}
END_TEST_FUNCTION();

BEGIN_TEST_FUNCTION(4);
{
  PathNameParser component (T_("/abc///def/ghi.jkl"));
  TEST (PathName::Compare(component.GetCurrent(), T_("/abc")) == 0);
  ++ component;
  TEST (PathName::Compare(component.GetCurrent(), T_("def")) == 0);
  ++ component;
  TEST (PathName::Compare(component.GetCurrent(), T_("ghi.jkl")) == 0);
  ++ component;
  TEST (component.GetCurrent() == 0);
}
END_TEST_FUNCTION();

BEGIN_TEST_FUNCTION(5);
{
  PathName path;

#if defined(MIKTEX_WINDOWS)
  path = T_("C:/abc/def/../ghi.jkl");
  path.MakeAbsolute ();
  TEST (PathName::Compare(path.Get(), T_("C:/abc/ghi.jkl")) == 0);
#endif

  path = T_("/abc/def/../ghi.jkl");
  path.MakeAbsolute ();
  TEST (PathName::Compare(path.Get(), T_("/abc/ghi.jkl")) == 0);

  PathName path2;

  path = T_("abc/./def/../ghi.jkl");
  path.MakeAbsolute ();

  path2.SetToCurrentDirectory ();
  path2 += T_("abc/ghi.jkl");

  TEST (PathName::Compare(path.Get(), path2) == 0);
}
END_TEST_FUNCTION();

BEGIN_TEST_FUNCTION(6);
{
#if defined(MIKTEX_WINDOWS)
  PathName path (T_("C:/abc/def/ghi.jkl/"));
  TEST (path.CutOffLastComponent() == T_("C:/abc/def"));
  TEST (path.CutOffLastComponent() == T_("C:/abc"));
  TEST (path.CutOffLastComponent() == T_("C:/"));
  TEST (path.CutOffLastComponent() == T_("C:/"));
#endif
  PathName path2 (T_("/abc/def/ghi.jkl/"));
  TEST (path2.CutOffLastComponent() == T_("/abc/def"));
  TEST (path2.CutOffLastComponent() == T_("/abc"));
  TEST (path2.CutOffLastComponent() == T_("/"));
  TEST (path2.CutOffLastComponent() == T_("/"));
}
END_TEST_FUNCTION();

BEGIN_TEST_FUNCTION(7)
{
  PathName path (T_("/abc/def/ghi.jkl/mno.pqr.stu"));
  TEST (PathName::Compare(path.GetExtension(), T_(".stu")) == 0);
  TEST (path.HasExtension(T_(".stu")));
  MIKTEXCHAR szName[BufferSizes::MaxPath];
  TEST (PathName::Compare(path.GetFileNameWithoutExtension(szName),
			  T_("mno.pqr"))
	== 0);
  MIKTEXCHAR szExt[BufferSizes::MaxPath];
  PathName::Split (path.Get(),
		   0, 0,
		   szName, BufferSizes::MaxPath,
		   szExt, BufferSizes::MaxPath);
  TEST (PathName::Compare(szName, T_("mno.pqr")) == 0);
  TEST (PathName::Compare(szExt, T_(".stu")) == 0);
  path.SetExtension (T_(".vwx"));
  TEST (path.HasExtension(T_(".vwx")));
}
END_TEST_FUNCTION();

BEGIN_TEST_PROGRAM();
{
  CALL_TEST_FUNCTION (1);
  CALL_TEST_FUNCTION (2);
  CALL_TEST_FUNCTION (3);
  CALL_TEST_FUNCTION (4);
  CALL_TEST_FUNCTION (5);
  CALL_TEST_FUNCTION (6);
  CALL_TEST_FUNCTION (7);
}
END_TEST_PROGRAM();

END_TEST_SCRIPT();

RUN_TEST_SCRIPT ();
