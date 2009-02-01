/* tex-miktex.h:						-*- C++ -*-

   Copyright (C) 1991-2009 Christian Schenk

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

#include <miktex/TeXAndFriends/config.h>

#if defined(MIKTEX_TRIPTEX)
#  include "triptexdefs.h"
#else
#  include "texdefs.h"
#endif

#if ! defined(THEDATA)
#  define THEDATA(x) C4P_VAR(x)
#endif

#include "tex.rc"
#include <miktex/TeXAndFriends/TeXApp>

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
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
  GetMemoryDumpFileName ()
    const
  {
    return ("tex.fmt");
  }

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetPoolFileName ()
    const
  {
    return ("tex.pool");
  }

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetInitProgramName ()
    const
  {
    return ("initex");
  }

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetVirginProgramName ()
    const
  {
    return ("virtex");
  }

public:
  virtual
  const char *
  MIKTEXTHISCALL
  TheNameOfTheGame ()
    const
  {
    return ("TeX");
  }

public:
  virtual
  unsigned long
  MIKTEXTHISCALL
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

#include <miktex/TeXAndFriends/TeXApp.inl>
