/* inputline.inl: inputln() implementation for TeX & Friends -*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Publstrcmpilic License
   along with this file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(GUARD_9613162233297D4BA8C4F18860291707_)
#define GUARD_9613162233297D4BA8C4F18860291707_

#if ! defined(THEAPP)
#  error THEAPP not defined
#endif

#if ! defined(THEDATA)
#  error THEDATA not defined
#endif

#include <miktex/webapp.inl>

#define MIKTEXMF_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace TeXAndFriends {

#define MIKTEXMF_END_NAMESPACE			\
    }						\
  }

MIKTEXMF_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   inputln
   _________________________________________________________________________ */

inline
bool
inputln (/*[in]*/ C4P::C4P_text &	f,
	 /*[in]*/ C4P::C4P_boolean	bypassEndOfLine = true)
{
  return (THEAPP.InputLine(f, bypassEndOfLine));
}

/* _________________________________________________________________________

   miktexclosefile
   _________________________________________________________________________ */

template<class T>
inline
void
miktexclosefile (/*[in]*/ T & f)
{
  THEAPP.CloseFile (f);
}

/* _________________________________________________________________________

   miktexopeninputfile
   _________________________________________________________________________ */

inline
bool
miktexopeninputfile (/*[in]*/ C4P::C4P_text & f)
{
  return (THEAPP.OpenInputFile(f, THEDATA(nameoffile)));
}

/* _________________________________________________________________________

   miktexopenoutputfile
   _________________________________________________________________________ */

template<class T>
inline
bool
miktexopenoutputfile (/*[in]*/ T &		f,
		      /*[in]*/ C4P::C4P_boolean	text)
{
#if 0
  MiKTeX::Core::FileShare share = MiKTeX::Core::FileShare::None;
#else
  MiKTeX::Core::FileShare share = MiKTeX::Core::FileShare::ReadWrite;
#endif
  return (THEAPP.OpenOutputFile(*reinterpret_cast<C4P::FileRoot*>(&f),
				THEDATA(nameoffile),
				share,
				text));
}

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#endif // inputline.inl
