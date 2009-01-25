/* miktex/TeXAndFriends/ETeXApp.h:				-*- C++ -*-

   Copyright (C) 1996-2009 Christian Schenk

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

#include <miktex/TeXAndFriends/TeXApp>

#define MIKTEXMF_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace TeXAndFriends {

#define MIKTEXMF_END_NAMESPACE			\
    }						\
  }

MIKTEXMF_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   ETeXApp
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
MIKTEXMFTYPEAPI(ETeXApp)

  : public TeXApp

{
public:
  MIKTEXMFEXPORT
  MIKTEXTHISCALL
  ETeXApp ();

  /* _______________________________________________________________________
     
     Init
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFTHISAPI(void)
  Init (/*[in]*/ const char * lpszProgramInvocationName);

  /* _______________________________________________________________________
     
     Finalize
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFTHISAPI(void)
  Finalize ();

  /* _______________________________________________________________________
     
     AddOptions
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFTHISAPI(void)
  AddOptions ();
  
  /* _______________________________________________________________________
     
     ProcessOption
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFTHISAPI(bool)
  ProcessOption (/*[in]*/ int		c,
		 /*[in]*/ const char *	lpszOptArg);

  /* _______________________________________________________________________
     
     OnTeXMFStartJob
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFTHISAPI(void)
  OnTeXMFStartJob ();

  /* _______________________________________________________________________
     
     CheckMemory
     _______________________________________________________________________ */

#if defined(MIKTEX_DEBUG)
public:
  virtual
  void
  CheckMemory ()
  {
#if defined(THEDATA)
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(eofseen));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(grpstack));
    MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL (THEDATA(ifstack));
#endif
    TeXMFApp::CheckMemory ();
  }
#endif

  /* _______________________________________________________________________
     
     AllocateMemory
     _______________________________________________________________________ */

#if defined(THEDATA)
public:
  void
  AllocateMemory ()
  {
    TeXApp::AllocateMemory ();

    Allocate ("eofseen", THEDATA(eofseen), THEDATA(maxinopen));
    Allocate ("grpstack", THEDATA(grpstack), THEDATA(maxinopen));
    Allocate ("ifstack", THEDATA(ifstack), THEDATA(maxinopen));
  }
#endif

  /* _______________________________________________________________________
     
     FreeMemory
     _______________________________________________________________________ */

#if defined(THEDATA)
public:
  void
  FreeMemory ()
  {
    TeXApp::FreeMemory ();

    Free ("eofseen", THEDATA(eofseen));
    Free ("grpstack", THEDATA(grpstack));
    Free ("ifstack", THEDATA(ifstack));
  }
#endif

  /* _______________________________________________________________________
     
     ETeXP
     _______________________________________________________________________ */

public:

  bool
  ETeXP ()
    const
  {
    return (enableETeX);
  }

  /* _______________________________________________________________________
     
     Variables
     _______________________________________________________________________ */

private:
  bool enableETeX;

private:
  int optBase;
};

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE
