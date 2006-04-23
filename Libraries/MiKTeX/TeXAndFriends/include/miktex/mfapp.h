/* mfapp.h: METAFONT, MetaPost					-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

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

#if ! defined(GUARD_6024E96164A684468B745C9A039DF171_)
#define GUARD_6024E96164A684468B745C9A039DF171_

#include <miktex/texmfapp.h>

#define MIKTEXMF_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace TeXAndFriends {

#define MIKTEXMF_END_NAMESPACE			\
    }						\
  }

MIKTEXMF_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   MetafontApp
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
MetafontApp

  : public TeXMFApp

{

  /* _______________________________________________________________________
     
     Init
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFAPI(void)
  Init (/*[in]*/ const MIKTEXCHAR * lpszProgramInvocationName);

  /* _______________________________________________________________________
     
     Finalize
     _______________________________________________________________________ */

public:

  virtual
  MIKTEXMFAPI(void)
  Finalize ();

  /* _______________________________________________________________________
     
     AddOptions
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFAPI(void)
  AddOptions ();
  
  /* _______________________________________________________________________
     
     GetDumpExtension
     _______________________________________________________________________ */

public:

  virtual
  const MIKTEXCHAR *
  MIKTEXMFCALL
  GetDumpExtension ()
    const
  {
    return (MIKTEXTEXT(".base"));
  }

  /* _______________________________________________________________________
     
     GetDumpFileType
     _______________________________________________________________________ */

public:

  virtual
  MiKTeX::Core::FileType
  MIKTEXMFCALL
  GetDumpFileType ()
    const
  {
    return (MiKTeX::Core::FileType::BASE);
  }

  /* _______________________________________________________________________
     
     GetInputFileType
     _______________________________________________________________________ */

public:

  virtual
  MiKTeX::Core::FileType
  MIKTEXMFCALL
  GetInputFileType ()
    const
  {
    return (MiKTeX::Core::FileType::MF);
  }

  /* _______________________________________________________________________
     
     GetPoolFileType
     _______________________________________________________________________ */

public:

  virtual
  MiKTeX::Core::FileType
  MIKTEXMFCALL
  GetPoolFileType ()
    const
  {
    return (MiKTeX::Core::FileType::MFPOOL);
  }

  /* _______________________________________________________________________
     
     ProcessOption
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFAPI(bool)
  ProcessOption (/*[in]*/ int			opt,
		 /*[in]*/ const MIKTEXCHAR *	lpszOptArg);

  /* _______________________________________________________________________
     
     AllocateMemory
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  void
  AllocateMemory ()
  {
    TeXMFApp::AllocateMemory ();

    GETPARAM (m_bistack_size, bistacksize, bistack_size, 1500);
    GETPARAM (m_lig_table_size, ligtablesize, lig_table_size, 15000);
    GETPARAM (m_path_size, pathsize, path_size, 10000);

    Allocate (THEDATA(bisectstack), THEDATA(bistacksize) + 1);
    Allocate (THEDATA(delta), THEDATA(pathsize) + 1);
    Allocate (THEDATA(deltax), THEDATA(pathsize) + 1);
    Allocate (THEDATA(deltay), THEDATA(pathsize) + 1);
    Allocate (THEDATA(ligkern), THEDATA(ligtablesize) + 1);
    Allocate (THEDATA(psi), THEDATA(pathsize));
    Allocate (THEDATA(strref), THEDATA(maxstrings) + 1);
    Allocate (THEDATA(theta), THEDATA(pathsize) + 1);
    Allocate (THEDATA(uu), THEDATA(pathsize) + 1);
    Allocate (THEDATA(vv), THEDATA(pathsize) + 1);
    Allocate (THEDATA(ww), THEDATA(pathsize) + 1);

#if defined(TRAPMF)
    Allocate (THEDATA(c4p_free), THEDATA(memmax) + 1);
    Allocate (THEDATA(wasfree), THEDATA(memmax) + 1);
#endif
  }
#endif // THEDATA

  /* _______________________________________________________________________
     
     FreeMemory
     _______________________________________________________________________ */

public:
  
#if defined(THEDATA)
  void
  FreeMemory ()
  {
    TeXMFApp::FreeMemory ();

    Free (THEDATA(bisectstack));
    Free (THEDATA(delta));
    Free (THEDATA(deltax));
    Free (THEDATA(deltay));
    Free (THEDATA(ligkern));
    Free (THEDATA(psi));
    Free (THEDATA(strref));
    Free (THEDATA(theta));
    Free (THEDATA(uu));
    Free (THEDATA(vv));
    Free (THEDATA(ww));

#if defined(TRAPMF)
    Free (THEDATA(c4p_free));
    Free (THEDATA(wasfree));
#endif
  }
#endif // THEDATA

  /* _______________________________________________________________________
     
     Variables
     _______________________________________________________________________ */

private:
  int m_bistack_size;
  
private:
  int m_lig_table_size;
  
private:
  int m_path_size;
};

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#endif	// mfapp.h
