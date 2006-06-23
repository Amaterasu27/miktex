/* mf-miktex.h:							-*- C++ -*-

   Copyright (C) 1991-2006 Christian Schenk

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

#define THEDATA(x) METAFONTDATA.m_##x

#if defined(TRAPMF)
#  include "trapmfdefs.h"
#else
#  include "mfdefs.h"
#endif

#include "mf.rc"
#include <miktex/mfapp.h>

namespace mf {
#include "mf.defaults.h"
}

#include "screen.h"

#if ! defined(MIKTEXHELP_MF)
#  include <miktex/help.h>
#endif

using namespace MiKTeX::Core;
using namespace MiKTeX::TeXAndFriends;
using namespace std;

class METAFONTCLASS

  : public MetafontApp

{
public:
  METAFONTCLASS ()
  {
    inputFileType = FileType::MF;
  }

public:
  virtual
  void
  MIKTEXMFCALL
  Init (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName)
  {
    MetafontApp::Init (lpszProgramInvocationName);
    SetProgramInfo (MIKTEXTEXT("mf"), 0, 0, 0);
#ifdef IMPLEMENT_TCX
    EnableFeature (Feature::TCX);
#endif
    m_bScreen = false;
    m_max_wiggle = -1;
    m_move_size = -1;
  }

  enum {
    OPT_MAX_WIGGLE = 10000,
    OPT_MOVE_SIZE,
    OPT_SCREEN,
  };

public:
  virtual
  void
  MIKTEXMFCALL
  AddOptions ()
  {
    MetafontApp::AddOptions ();
    AddOption (MIKTEXTEXT("base"), MIKTEXTEXT("undump"));
    AddOption (MIKTEXTEXT("max-wiggle\0Set max_wiggle to N."),
	       OPT_MAX_WIGGLE,
	       required_argument,
	       MIKTEXTEXT("N"));
    AddOption (MIKTEXTEXT("move-size\0Set move_size to N."),
	       OPT_MOVE_SIZE,
	       required_argument,
	       MIKTEXTEXT("N"));
    AddOption (MIKTEXTEXT("screen\0Enable screen output."),
	       OPT_SCREEN);
  }

public:
  virtual
  bool
  MIKTEXMFCALL
  ProcessOption (/*[in]*/ int			opt,
		 /*[in]*/ const MIKTEXCHAR *	lpszOptArg)
  {
    bool done = true;
    switch (opt)
      {
      case OPT_MAX_WIGGLE:
	m_max_wiggle = atoi(lpszOptArg);
	break;
      case OPT_MOVE_SIZE:
	m_move_size = atoi(lpszOptArg);
	break;
      case OPT_SCREEN:
	m_bScreen = true;
	break;
      default:
	done = MetafontApp::ProcessOption(opt, lpszOptArg);
	break;
      }
    return (done);
  }

public:
  void
  FreeMemory ()
  {
    MetafontApp::FreeMemory ();
    Free (THEDATA(after));
    Free (THEDATA(before));
    Free (THEDATA(envmove));
    Free (THEDATA(move));
    Free (THEDATA(nodetoround));
  }

public:
  void
  AllocateMemory ()
  {
    MetafontApp::AllocateMemory ();

    GETPARAM (m_max_wiggle,
	      maxwiggle,
	      max_wiggle,
	      mf::mf::max_wiggle());

    GETPARAM (m_move_size,
	      movesize,
	      move_size,
	      mf::mf::move_size());

    Allocate (THEDATA(after), THEDATA(maxwiggle) + 1);
    Allocate (THEDATA(before), THEDATA(maxwiggle) + 1);
    Allocate (THEDATA(envmove), THEDATA(movesize) + 1);
    Allocate (THEDATA(move), THEDATA(movesize) + 1);
    Allocate (THEDATA(nodetoround), THEDATA(maxwiggle) + 1);
  }

public:
  virtual
  int
  MIKTEXMFCALL
  GetFormatIdent ()
    const
  {
    return (THEDATA(baseident));
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetMemoryDumpFileName ()
    const
  {
    return (MIKTEXTEXT("mf.base"));
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetPoolFileName ()
    const
  {
    return (MIKTEXTEXT("mf.pool"));
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetInitProgramName ()
    const
  {
    return (MIKTEXTEXT("inimf"));
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetVirginProgramName ()
    const
  {
    return (MIKTEXTEXT("virmf"));
  }

public:
  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  TheNameOfTheGame ()
    const
  {
    return (MIKTEXTEXT("METAFONT"));
  }

public:
  bool
  ScreenEnabled ()
    const
  {
    return (m_bScreen);
  }

private:
  bool m_bScreen;

private:
  int m_max_wiggle;

private:
  int m_move_size;

public:
  virtual
  unsigned long
  MIKTEXMFCALL
  GetHelpId ()
    const
  {
    return (MIKTEXHELP_MF);
  }
};

extern METAFONTCLASS METAFONTAPP;
#define THEAPP METAFONTAPP

inline
bool
miktexopenbasefile (/*[in]*/ wordfile &		f,
		    /*[in]*/ bool		renew = false)
{
  return (THEAPP.OpenMemoryDumpFile(f, renew));
}

#include <miktex/mfapp.inl>
