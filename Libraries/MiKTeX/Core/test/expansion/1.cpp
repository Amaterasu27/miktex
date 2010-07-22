/* 1.cpp:

   Copyright (C) 1996-2010 Christian Schenk

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

class VarExpand : public MiKTeX::Core::IExpandCallback
{
public:
  VarExpand (Session * pSession)
    : pSession (pSession)
  {
  }
private:
  Session * pSession;
public:
  virtual
  bool
  MIKTEXTHISCALL
  GetValue (/*[in]*/ const char *   lpszValueName,
	    /*[out]*/ std::string & varValue)
  {
    if (strcmp(lpszValueName, "jkl") == 0)
    {
      varValue = "mno";
      return (true);
    }
    else if (strcmp(lpszValueName, "rst") == 0)
    {
      varValue = pSession->Expand("x${abc}x");
      return (true);
    }
    else
    {
      return (false);
    }
  }
};

BEGIN_TEST_SCRIPT();

BEGIN_TEST_FUNCTION(1);
{
  TEST (pSession->Expand("$$") == "$");
  putenv ("abc=def");
  TEST (pSession->Expand("$abc") == "def");
  TEST (pSession->Expand("$(abc)") == "def");
  TEST (pSession->Expand("${abc}") == "def");
  putenv ("xyz=x$(abc)x");
  TEST (pSession->Expand("$xyz") == "xdefx");
  TEST (pSession->Expand("$xyz", &VarExpand(pSession.GetSession())) == "xdefx");
  TEST (pSession->Expand("$jkl", &VarExpand(pSession.GetSession())) == "mno");
  TEST (pSession->Expand("$rst", &VarExpand(pSession.GetSession())) == "xdefx");
}
END_TEST_FUNCTION();

BEGIN_TEST_FUNCTION(2);
{
  std::string str1 = pSession->Expand("%R/tex//", ExpandFlags::PathPatterns, 0);
  std::string str2 = pSession->Expand("%R/tex//base", ExpandFlags::PathPatterns, 0);
  std::string str3 = pSession->Expand("ab{cd,ef}gh{ij,kl}mn", ExpandFlags::Braces, 0);
  TEST (str3 == "abcdghijmn;abcdghklmn;abefghijmn;abefghklmn");
  std::string str4 = pSession->Expand("x{a,b{c,d}e}y", ExpandFlags::Braces, 0);
  TEST (str4 == "xay;xbcey;xbdey");
  std::string str5 = pSession->Expand("/texmf/tex/{plain,generic,}//", ExpandFlags::Braces, 0);
  TEST (str5 == "/texmf/tex/plain//;/texmf/tex/generic//;/texmf/tex///", 0);
  putenv ("abc={d,e}");
  std::string str6 = pSession->Expand("a{b,c}$abc", ExpandFlags::Braces | ExpandFlags::Values, 0);
  TEST (str6 == "abd;abe;acd;ace");
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
