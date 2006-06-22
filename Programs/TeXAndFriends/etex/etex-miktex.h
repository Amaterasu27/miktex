/* etex-miktex.h:					-*- C++ -*-

   Copyright (C) 1998-2006 Christian Schenk

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

#if ! defined(THEDATA)
#  define THEDATA(x) ETEXDATA.m_##x
#endif

#if defined(MIKTEX_ETEX)
#  if defined(ETRIPTEX)
#    include "etriptexdefs.h"
#  else
#    include "etexdefs.h"
#  endif
#endif

#if defined(MIKTEX_ETEX)
#  include "etex.rc"
#endif

#include <miktex/texapp.h>

#if ! defined(MIKTEX_VIRTUAL_TEXAPP)
#  define MIKTEX_VIRTUAL_TEXAPP
#endif

#if ! defined(MIKTEXHELP_ETEX)
#  include <miktex/help.h>
#endif

using namespace MiKTeX::TeXAndFriends;

class ETEXCLASS

  : MIKTEX_VIRTUAL_TEXAPP public TeXApp

{
public:
  void
  AllocateMemory ()
  {
    TeXApp::AllocateMemory ();
    Allocate (THEDATA(eofseen), THEDATA(maxinopen));
    Allocate (THEDATA(grpstack), THEDATA(maxinopen)+1);
    Allocate (THEDATA(ifstack), THEDATA(maxinopen)+1);
  }

public:
  void
  FreeMemory ()
  {
    TeXApp::FreeMemory ();
    Free (THEDATA(eofseen));
    Free (THEDATA(grpstack));
    Free (THEDATA(ifstack));
  }

public:
  virtual
  void
  MIKTEXMFCALL
  Init (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName)
  {
    TeXApp::Init (lpszProgramInvocationName);
#if defined(IMPLEMENT_TCX)
    EnableFeature (Feature::TCX);
#endif
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetMemoryDumpFileExtension ()
    const
  {
    return (MIKTEX_FORMAT_FILE_SUFFIX);
  }

public:
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetMemoryDumpFileName ()
    const
  {
    return (MIKTEXTEXT("etex") MIKTEX_FORMAT_FILE_SUFFIX);
  }

public:
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetPoolFileName ()
    const
  {
    return (MIKTEXTEXT("etex") MIKTEX_POOL_FILE_SUFFIX);
  }

public:
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetInitProgramName ()
    const
  {
    return (MIKTEXTEXT("einitex"));
  }

public:
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetVirginProgramName ()
    const
  {
    return (MIKTEXTEXT("evirtex"));
  }

public:
  const MIKTEXCHAR *
  MIKTEXMFCALL
  TheNameOfTheGame ()
    const
  {
    return (MIKTEXTEXT("e-TeX"));
  }

public:
  unsigned long
  MIKTEXMFCALL
  GetHelpId ()
    const
  {
    return (MIKTEXHELP_ETEX);
  }
};

#if defined(MIKTEX_ETEX)
extern ETEXCLASS ETEXAPP;
#define THEAPP ETEXAPP
#include <miktex/texapp.inl>
#endif
