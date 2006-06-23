/* pdfetex-miktex.h:						-*- C++ -*-

   Copyright (C) 2001-2006 Christian Schenk

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

#include "pdfetexdefs.h"

#if ! defined(C4PEXTERN)
#  define C4PEXTERN extern
#endif

#include "pdfetexd.h"

#define THEDATA(x) PDFETEXDATA.m_##x

#define MIKTEX_VIRTUAL_TEXAPP virtual

#include "pdfetex.rc"

#include <pdftex-miktex.h>
#include <etex-miktex.h>

#if defined(__cplusplus)

using namespace MiKTeX::TeXAndFriends;
using namespace MiKTeX::Core;

class PDFETEXCLASS

  : public CpdfTeXApp,
    public CeTeXApp

{
public:
  virtual
  void
  MIKTEXMFCALL
  Init (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName)
  {
    CeTeXApp::Init (lpszProgramInvocationName);
  }

public:
  void
  AllocateMemory ()
  {
    CpdfTeXApp::AllocateMemory ();
    CeTeXApp::AllocateMemory ();
  }

public:
  void
  FreeMemory ()
  {
    CpdfTeXApp::FreeMemory ();
    CeTeXApp::FreeMemory ();
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
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetMemoryDumpFileName ()
    const
  {
    return (MIKTEXTEXT("pdfetex") MIKTEX_FORMAT_FILE_SUFFIX);
  }
  
public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetPoolFileName ()
    const
  {
    return (MIKTEXTEXT("pdfetex") MIKTEX_POOL_FILE_SUFFIX);
  }
  
public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetInitProgramName ()
    const
  {
    return (MIKTEXTEXT("pdfeinitex"));
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetVirginProgramName ()
    const
  {
    return (MIKTEXTEXT("pdfevirtex"));
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  TheNameOfTheGame ()
    const
  {
    return (MIKTEXTEXT("pdfetex"));
  }

public:
  virtual
  unsigned long
  MIKTEXMFCALL
  GetHelpId ()
    const
  {
    return (0);
  }
};

extern PDFETEXCLASS PDFETEXAPP;
#define THEAPP PDFETEXAPP
#include <miktex/texapp.inl>

#endif // __cplusplus
