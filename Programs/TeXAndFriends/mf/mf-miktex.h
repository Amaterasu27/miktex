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

#if defined(MIKTEX_TRAPMF)
#  include "trapmfdefs.h"
#define THEDATA(x) TRAPMFDATA.m_##x
#else
#  include "mfdefs.h"
#define THEDATA(x) MFDATA.m_##x
#endif

#include "mf.rc"
#include <MiKTeX/TeXAndFriends/MetafontApp>

namespace mf {
#include "mf.defaults.h"
}

#include "screen.h"

#if ! defined(MIKTEXHELP_MF)
#  include <MiKTeX/Core/Help>
#endif

using namespace MiKTeX::Core;
using namespace MiKTeX::TeXAndFriends;
using namespace std;

#if defined(MIKTEX_TRAPMF)
class TRAPMFCLASS
#else
class MFCLASS
#endif

  : public MetafontApp

{
public:
#if defined(MIKTEX_TRAPMF)
  TRAPMFCLASS ()
#else
  MFCLASS ()
#endif
  {
    inputFileType = FileType::MF;
  }

public:
  virtual
  void
  MIKTEXTHISCALL
  Init (/*[in]*/ const char * lpszProgramInvocationName)
  {
    MetafontApp::Init (lpszProgramInvocationName);
    SetProgramInfo ("mf", 0, 0, 0);
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
  MIKTEXTHISCALL
  AddOptions ()
  {
    MetafontApp::AddOptions ();
    AddOption ("base", "undump");
    AddOption (MIKTEXTEXT("max-wiggle\0Set max_wiggle to N."),
	       OPT_MAX_WIGGLE,
	       required_argument,
	       "N");
    AddOption (MIKTEXTEXT("move-size\0Set move_size to N."),
	       OPT_MOVE_SIZE,
	       required_argument,
	       "N");
    AddOption (MIKTEXTEXT("screen\0Enable screen output."),
	       OPT_SCREEN);
  }

public:
  virtual
  bool
  MIKTEXTHISCALL
  ProcessOption (/*[in]*/ int			opt,
		 /*[in]*/ const char *	lpszOptArg)
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

    GETPARAMCHECK (m_max_wiggle,
		   maxwiggle,
		   max_wiggle,
		   mf::mf::max_wiggle());

    GETPARAMCHECK (m_move_size,
		   movesize,
		   move_size,
		   mf::mf::move_size());

    Allocate ("after", THEDATA(after), THEDATA(maxwiggle));
    Allocate ("before", THEDATA(before), THEDATA(maxwiggle));
    Allocate ("envmove", THEDATA(envmove), THEDATA(movesize));
    Allocate ("move", THEDATA(move), THEDATA(movesize));
    Allocate ("nodetoround", THEDATA(nodetoround), THEDATA(maxwiggle));
  }

public:
  virtual
  int
  MIKTEXTHISCALL
  GetFormatIdent ()
    const
  {
    return (THEDATA(baseident));
  }

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetMemoryDumpFileName ()
    const
  {
    return ("mf.base");
  }

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetPoolFileName ()
    const
  {
    return ("mf.pool");
  }

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetInitProgramName ()
    const
  {
    return ("inimf");
  }

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetVirginProgramName ()
    const
  {
    return ("virmf");
  }

public:
  virtual
  const char *
  MIKTEXTHISCALL
  TheNameOfTheGame ()
    const
  {
    return ("METAFONT");
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
  MIKTEXTHISCALL
  GetHelpId ()
    const
  {
    return (MIKTEXHELP_MF);
  }
};

#if defined(MIKTEX_TRAPMF)
extern TRAPMFCLASS TRAPMFAPP;
#define THEAPP TRAPMFAPP
#else
extern MFCLASS MFAPP;
#define THEAPP MFAPP
#endif

inline
bool
miktexopenbasefile (/*[in]*/ wordfile &		f,
		    /*[in]*/ bool		renew = false)
{
  return (THEAPP.OpenMemoryDumpFile(f, renew));
}

#include <MiKTeX/TeXAndFriends/MetafontApp.inliners>
