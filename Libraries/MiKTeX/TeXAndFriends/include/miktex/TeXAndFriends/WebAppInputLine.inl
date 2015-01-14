/* miktex/TeXAndFriends/WebAppInputLine.inl:			-*- C++ -*-

   Copyright (C) 1996-2015 Christian Schenk

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

#include <miktex/TeXAndFriends/WebApp.inl>

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

template<class FileType>
inline
bool
inputln (/*[in]*/ FileType &		f,
	 /*[in]*/ C4P::C4P_boolean	bypassEndOfLine = true)
{
  return (THEAPP.InputLine(f, bypassEndOfLine));
}

/* _________________________________________________________________________

   miktexclosefile
   _________________________________________________________________________ */

template<class FileType>
inline
void
miktexclosefile (/*[in]*/ FileType & f)
{
  THEAPP.CloseFile (f);
}

/* _________________________________________________________________________

   miktexopeninputfile
   _________________________________________________________________________ */

template<class FileType>
inline
bool
miktexopeninputfile (/*[in]*/ FileType & f)
{
  bool done = (THEAPP.OpenInputFile(*static_cast<C4P::FileRoot*>(&f),
			       THEAPP.GetNameOfFile().Get()));
#if 1
  // bug #1733 bad \input implementation in TeX breaks SyncTeX Move Edit
  // http://sourceforge.net/p/miktex/bugs/1733/
  if (done)
  {
    MiKTeX::Core::PathName cwd;
    cwd.SetToCurrentDirectory ();
    const char * lpszRel = MiKTeX::Core::Utils::GetRelativizedPath(THEAPP.GetFQNameOfFile(), cwd.Get());
    if (lpszRel == 0)
    {
      THEAPP.SetNameOfFile (THEAPP.MangleNameOfFile(THEAPP.GetFQNameOfFile()));
    }
    else
    {
      THEAPP.SetNameOfFile (THEAPP.MangleNameOfFile(lpszRel));
    }
  }
#endif
  return (done);
}

/* _________________________________________________________________________

   miktexallownameoffile
   _________________________________________________________________________ */

inline
bool
miktexallownameoffile (/*[in]*/ C4P::C4P_boolean forInput)
{
  return (THEAPP.AllowFileName(THEAPP.GetNameOfFile().Get(),
			       forInput));
}

/* _________________________________________________________________________

   miktexopenoutputfile
   _________________________________________________________________________ */

template<class FileType>
inline
bool
miktexopenoutputfile (/*[in]*/ FileType &		f,
		      /*[in]*/ C4P::C4P_boolean		text)
{
  // must open with read/write sharing flags
  // cf. bug 2006511
  MiKTeX::Core::FileShare share = MiKTeX::Core::FileShare::ReadWrite;
  MiKTeX::Core::PathName outPath;
  bool done = THEAPP.OpenOutputFile(*static_cast<C4P::FileRoot*>(&f),
				THEAPP.GetNameOfFile().Get(),
				share,
				text,
				outPath);
  if (done)
  {
    THEAPP.SetNameOfFile (THEAPP.MangleNameOfFile(outPath.Get()));
  }
  return (done);
}

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#endif
