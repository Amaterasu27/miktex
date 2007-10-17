/* texapp.inl: inliners for METAFONT, MetaPost			-*- C++ -*-

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

#if ! defined(GUARD_DE57699544C3AA4398A72098F31ED9CD_)
#define GUARD_DE57699544C3AA4398A72098F31ED9CD_

#if ! defined(THEAPP)
#  error THEAPP not defined
#endif

#if ! defined(THEDATA)
#  error THEDATA not defined
#endif

#include <MiKTeX/TeXAndFriends/TeXMFApp.inliners>

#define MIKTEXMF_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace TeXAndFriends {

#define MIKTEXMF_END_NAMESPACE			\
    }						\
  }

MIKTEXMF_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   miktexallocatememory
   _________________________________________________________________________ */

inline
void
miktexallocatememory ()
{
  THEAPP.AllocateMemory ();
}

/* _________________________________________________________________________

   miktexfreememory
   _________________________________________________________________________ */

inline
void
miktexfreememory ()
{
  THEAPP.FreeMemory ();
}

/* _________________________________________________________________________

   makefraction
   _________________________________________________________________________ */

#if 1
inline
C4P_integer
makefraction (/*[in]*/ C4P_integer p,
	      /*[in]*/ C4P_integer q)
{
  return (MakeFraction(p, q, THEDATA(aritherror)));
}
#endif

/* _________________________________________________________________________

   makescaled
   _________________________________________________________________________ */

#if 0
inline
C4P_integer
makescaled (/*[in]*/ C4P_integer p,
	    /*[in]*/ C4P_integer q)
{
  return (MakeScaled(p, q, THEDATA(aritherror)));
}
#endif

/* _________________________________________________________________________

   takefraction
   _________________________________________________________________________ */

#if 1
inline
C4P_integer
takefraction (/*[in]*/ C4P_integer p,
	      /*[in]*/ C4P_integer q)
{
  return (TakeFraction(p, q, THEDATA(aritherror)));
}
#endif

/* _________________________________________________________________________

   takescaled
   _________________________________________________________________________ */

#if 1
inline
C4P_integer
takescaled (/*[in]*/ C4P_integer p,
	    /*[in]*/ C4P_integer q)
{
  return (TakeScaled(p, q, THEDATA(aritherror)));
}
#endif

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#endif	// mfapp.inl
