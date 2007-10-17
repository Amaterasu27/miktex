/* mfapp.h: METAFONT, MetaPost					-*- C++ -*-

   Copyright (C) 1996-2007 Christian Schenk

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

#include <MiKTeX/TeXAndFriends/TeXMFApp>

namespace mfapp {
#include <miktex/mfapp.defaults.h>
}

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
     
     ProcessOption
     _______________________________________________________________________ */

protected:

  virtual
  MIKTEXMFAPI(bool)
  ProcessOption (/*[in]*/ int			opt,
		 /*[in]*/ const MIKTEXCHAR *	lpszOptArg);

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
     
     GetMemoryDumpFileExtension
     _______________________________________________________________________ */

public:

  virtual
  const char *
  MIKTEXMFCALL
  GetMemoryDumpFileExtension ()
    const
  {
    return (".base");
  }

  /* _______________________________________________________________________
     
     GetMemoryDumpFileType
     _______________________________________________________________________ */

public:

  virtual
  MiKTeX::Core::FileType
  MIKTEXMFCALL
  GetMemoryDumpFileType ()
    const
  {
    return (MiKTeX::Core::FileType::BASE);
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
     
     AllocateMemory
     _______________________________________________________________________ */

public:

#if defined(THEDATA)
  void
  AllocateMemory ()
  {
    TeXMFApp::AllocateMemory ();

    GETPARAMCHECK (param_bistack_size,
		   bistacksize,
		   bistack_size,
		   mfapp::mfapp::bistack_size());
    GETPARAMCHECK (param_lig_table_size,
		   ligtablesize,
		   lig_table_size,
		   mfapp::mfapp::lig_table_size());
    GETPARAMCHECK (param_path_size,
		   pathsize,
		   path_size,
		   mfapp::mfapp::path_size());

    Allocate ("bisectstack", THEDATA(bisectstack), THEDATA(bistacksize));
    Allocate ("delta", THEDATA(delta), THEDATA(pathsize));
    Allocate ("deltax", THEDATA(deltax), THEDATA(pathsize));
    Allocate ("deltay", THEDATA(deltay), THEDATA(pathsize));
    Allocate ("ligkern", THEDATA(ligkern), THEDATA(ligtablesize));
    Allocate ("psi", THEDATA(psi), THEDATA(pathsize));
    if (IsInitProgram()	|| AmI(MIKTEXTEXT("mf")))
      {
	Allocate ("strref", THEDATA(strref), THEDATA(maxstrings));
      }
    Allocate ("theta", THEDATA(theta), THEDATA(pathsize));
    Allocate ("uu", THEDATA(uu), THEDATA(pathsize));
    Allocate ("vv", THEDATA(vv), THEDATA(pathsize));
    Allocate ("ww", THEDATA(ww), THEDATA(pathsize));

#if defined(TRAPMF)
    Allocate ("c4p", THEDATA(c4p_free), THEDATA(memmax));
    Allocate ("wasfree", THEDATA(wasfree), THEDATA(memmax));
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
  int param_bistack_size;
  
private:
  int param_lig_table_size;
  
private:
  int param_path_size;

private:
  int optBase;
};

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#endif	// mfapp.h
