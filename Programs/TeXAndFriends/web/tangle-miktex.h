/* tangle-miktex.h:						-*- C++ -*-

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

#if ! defined(THEDATA)
#  define THEDATA(x) C4P_VAR(x)
#endif

#include "tangle-version.h"
#include <miktex/TeXAndFriends/WebApp>

using namespace MiKTeX::TeXAndFriends;
using namespace MiKTeX::Core;

#define OPT_OMEGA 1000

class TANGLECLASS

  : public MiKTeX::TeXAndFriends::WebApp

{
public:
  virtual
  void
  MIKTEXTHISCALL
  Init (/*[in]*/ const char * lpszProgramInvocationName)
  {
    MiKTeX::TeXAndFriends::WebApp::Init (lpszProgramInvocationName);
    THEDATA(maxchar) = 255;
  }

public:
  virtual
  void
  MIKTEXTHISCALL
  AddOptions ()
  {
    AddOption (MIKTEXTEXT("omega\0Create Omega compatible output file."),
	       OPT_OMEGA);
    WebApp::AddOptions ();
  }

public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetUsage ()
    const
  {
    return (MIKTEXTEXT("\
[OPTION...] INPUTFILE CHANGEFILE OUTPUTFILE POOLFILE"));
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
      case OPT_OMEGA:
	THEDATA(maxchar) = 0xffff;
	break;
      default:
	done = WebApp::ProcessOption(opt, lpszOptArg);
	break;
      }
    return (done);
  }

public:
  void
  MIKTEXTHISCALL
  ProcessCommandLineOptions ()
  {
    WebApp::ProcessCommandLineOptions ();
    if (C4P::GetArgC() != 5)
      {
	BadUsage ();
      }
  }
};

extern TANGLECLASS TANGLEAPP;
#define THEAPP TANGLEAPP

#include <miktex/TeXAndFriends/WebApp.inl>
