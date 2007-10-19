/* tex-miktex.h:						-*- C++ -*-

   Copyright (C) 1991-2007 Christian Schenk

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
   USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if defined(MIKTEX_TRIPTEX)
#  include "triptexdefs.h"
#  define THEDATA(x) TRIPTEXDATA.m_##x
#else
#  include "texdefs.h"
#  define THEDATA(x) TEXDATA.m_##x
#endif

#include "tex.rc"
#include <MiKTeX/TeXAndFriends/TeXApp>

#if ! defined(MIKTEXHELP_TEX)
#  include <MiKTeX/Core/Help>
#endif

using namespace MiKTeX::TeXAndFriends;

#if defined(MIKTEX_TRIPTEX)
class TRIPTEXCLASS
#else
class TEXCLASS
#endif

  : public TeXApp

{
public:
  virtual
  void
  MIKTEXMFCALL
  Init (/*[in]*/ const char * lpszProgramInvocationName)
  {
    TeXApp::Init (lpszProgramInvocationName);
#if defined(IMPLEMENT_TCX)
    EnableFeature (Feature::EightBitChars);
    EnableFeature (Feature::TCX);
#endif
  }

public:
  virtual
  const char *
  MIKTEXMFCALL
  GetMemoryDumpFileName ()
    const
  {
    return ("tex.fmt");
  }

public:
  virtual
  const char *
  MIKTEXMFCALL
  GetPoolFileName ()
    const
  {
    return ("tex.pool");
  }

public:
  virtual
  const char *
  MIKTEXMFCALL
  GetInitProgramName ()
    const
  {
    return ("initex");
  }

public:
  virtual
  const char *
  MIKTEXMFCALL
  GetVirginProgramName ()
    const
  {
    return ("virtex");
  }

public:
  virtual
  const char *
  MIKTEXMFCALL
  TheNameOfTheGame ()
    const
  {
    return ("TeX");
  }

public:
  virtual
  unsigned long
  MIKTEXMFCALL
  GetHelpId ()
    const
  {
    return (MIKTEXHELP_TEX);
  }
};

#if defined(MIKTEX_TRIPTEX)
extern TRIPTEXCLASS TRIPTEXAPP;
#define THEAPP TRIPTEXAPP
#else
extern TEXCLASS TEXAPP;
#define THEAPP TEXAPP
#endif

#include <MiKTeX/TeXAndFriends/TeXApp.inliners>
