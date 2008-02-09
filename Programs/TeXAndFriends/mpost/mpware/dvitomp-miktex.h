/* dvitomp-miktex.h:						-*- C++ -*-

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

#define THEDATA(x) DVITOMPDATA.m_##x

#include "dvitomp.rc"
#include <miktex/TeXAndFriends/WebApp>

using namespace MiKTeX::Core;
using namespace MiKTeX::TeXAndFriends;

class DVITOMPCLASS

  : public WebApp

{
public:
  virtual
  const char *
  MIKTEXTHISCALL
  GetUsage ()
    const
  {
    return (MIKTEXTEXT("[OPTION...] DVIFILE MPXFILE"));
  }

public:
  virtual
  void
  MIKTEXTHISCALL
  ProcessCommandLineOptions ()
  {
    WebApp::ProcessCommandLineOptions ();
    if (C4P::GetArgC() != 3)
      {
	BadUsage ();
      }
  }
};

extern DVITOMPCLASS DVITOMPAPP;
#define THEAPP DVITOMPAPP

#include <miktex/TeXAndFriends/WebApp.inliners>

#include <miktex/KPSE/Emulation>

/* _________________________________________________________________________

   xmallocchararray
   _________________________________________________________________________ */

inline
char *
xmallocchararray (/*[in]*/ size_t size)
{
  return (reinterpret_cast<char*>(xmalloc(size + 1)));
}

/* _________________________________________________________________________

   stringcast
   _________________________________________________________________________ */

template<typename TYPE>
inline
char *
stringcast (/*[in]*/ TYPE * p)
{
  return (reinterpret_cast<char*>(p));
}

/* _________________________________________________________________________

   libcfree
   _________________________________________________________________________ */

inline
void
libcfree (/*[in]*/ void * ptr)
{
  free (ptr);
}

