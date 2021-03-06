/* miktex/TeXAndFriends/TeXApp.inl:				-*- C++ -*-

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

#if ! defined(GUARD_C3A59E4996D60046B59B00D793120222_)
#define GUARD_C3A59E4996D60046B59B00D793120222_

#if ! defined(THEAPP)
#  error THEAPP not defined
#endif

#if ! defined(THEDATA)
#  error THEDATA not defined
#endif

#include <miktex/TeXAndFriends/TeXMFApp.inl>

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

   miktexclosedvifile
   _________________________________________________________________________ */

inline
void
miktexclosedvifile (/*[in]*/ bytefile & f)
{
  THEAPP.CloseFile (f);
}

/* _________________________________________________________________________

   miktexclosepdffile
   _________________________________________________________________________ */

inline
void
miktexclosepdffile (/*[in]*/ bytefile & f)
{
  THEAPP.CloseFile (f);
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

   miktexinsertsrcspecialauto
   _________________________________________________________________________ */

inline
bool
miktexinsertsrcspecialauto ()
{
  return (THEAPP.IsSourceSpecialOn(SourceSpecials::Auto));
}

/* _________________________________________________________________________

   miktexinsertsrcspecialeverycr
   _________________________________________________________________________ */

inline
bool
miktexinsertsrcspecialeverycr ()
{
  return (THEAPP.IsSourceSpecialOn(SourceSpecials::CarriageReturn));
}

/* _________________________________________________________________________

   miktexinsertsrcspecialeverydisplay
   _________________________________________________________________________ */

inline
bool
miktexinsertsrcspecialeverydisplay ()
{
  return (THEAPP.IsSourceSpecialOn(SourceSpecials::Display));
}

/* _________________________________________________________________________

   miktexinsertsrcspecialeveryhbox
   _________________________________________________________________________ */

inline
bool
miktexinsertsrcspecialeveryhbox ()
{
  return (THEAPP.IsSourceSpecialOn(SourceSpecials::HorizontalBox));
}

/* _________________________________________________________________________

   miktexinsertsrcspecialeverymath
   _________________________________________________________________________ */

inline
bool
miktexinsertsrcspecialeverymath ()
{
  return (THEAPP.IsSourceSpecialOn(SourceSpecials::Math));
}

/* _________________________________________________________________________

   miktexinsertsrcspecialeverypar
   _________________________________________________________________________ */

inline
bool
miktexinsertsrcspecialeverypar ()
{
  return (THEAPP.IsSourceSpecialOn(SourceSpecials::Paragraph));
}

/* _________________________________________________________________________

   miktexinsertsrcspecialeveryparend
   _________________________________________________________________________ */

inline
bool
miktexinsertsrcspecialeveryparend ()
{
  return (THEAPP.IsSourceSpecialOn(SourceSpecials::ParagraphEnd));
}

/* _________________________________________________________________________

   miktexinsertsrcspecialeveryvbox
   _________________________________________________________________________ */

inline
bool
miktexinsertsrcspecialeveryvbox ()
{
  return (THEAPP.IsSourceSpecialOn(SourceSpecials::VerticalBox));
}

/* _________________________________________________________________________

   miktexisnewsource
   _________________________________________________________________________ */

inline
bool
miktexisnewsource (/*[in]*/ int fileName,
		   /*[in]*/ int lineNo)
{
  return (THEAPP.IsNewSource(fileName, lineNo));
}

/* _________________________________________________________________________

   miktexmakesrcspecial
   _________________________________________________________________________ */

inline int
miktexmakesrcspecial (/*[in]*/ int fileName,
		      /*[in]*/ int lineNo)
{
  return (THEAPP.MakeSrcSpecial(fileName, lineNo));
}

/* _________________________________________________________________________

   miktexopendvifile
   _________________________________________________________________________ */

template<class FileType>
inline
bool
miktexopendvifile (/*[in]*/ FileType & f)
{
  MiKTeX::Core::PathName outPath;
  bool done = THEAPP.OpenOutputFile(*reinterpret_cast<C4P::FileRoot*>(&f),
				THEAPP.GetNameOfFile().Get(),
				MiKTeX::Core::FileShare::Read,
				false,
				outPath);
  if (done)
  {
    THEAPP.SetNameOfFile (THEAPP.MangleNameOfFile(outPath.Get()));
  }
  return (done);
}

/* _________________________________________________________________________

   miktexopenpdffile
   _________________________________________________________________________ */

template<class FileType>
inline
bool
miktexopenpdffile (/*[in]*/ FileType & f)
{
  MiKTeX::Core::PathName outPath;
  bool done = THEAPP.OpenOutputFile(*reinterpret_cast<C4P::FileRoot*>(&f),
				THEAPP.GetNameOfFile().Get(),
				MiKTeX::Core::FileShare::Read,
				false,
				outPath);
  if (done)
  {
    THEAPP.SetNameOfFile (THEAPP.MangleNameOfFile(outPath.Get()));
  }
  return (done);
}

/* _________________________________________________________________________

   miktexopenformatfile
   _________________________________________________________________________ */

template<class FileType>
inline bool
miktexopenformatfile (/*[in]*/ FileType &	f,
		      /*[in]*/ bool		renew = false)
{
  return (THEAPP.OpenMemoryDumpFile(f, renew));
}

/* _________________________________________________________________________

   miktexremembersourceinfo
   _________________________________________________________________________ */

inline void
miktexremembersourceinfo (/*[in]*/ int fileName,
			  /*[in]*/ int lineNo)
{
  THEAPP.RememberSourceInfo (fileName, lineNo);
}

/* _________________________________________________________________________

   miktexwrite18
   _________________________________________________________________________ */

inline
int
miktexwrite18 (/*[in]*/ const TEXMFCHAR * lpszCommand)
{
  int exitCode;
  return (THEAPP.Write18(lpszCommand, exitCode).Get());
}

/* _________________________________________________________________________

   miktexwrite18p
   _________________________________________________________________________ */

inline
bool
miktexwrite18p ()
{
  return (THEAPP.Write18P());
}

/* _________________________________________________________________________

   miktexenctexp
   _________________________________________________________________________ */

inline
bool
miktexenctexp ()
{
  return (THEAPP.EncTeXP());
}

/* _________________________________________________________________________

   miktexmltexp
   _________________________________________________________________________ */

inline
bool
miktexmltexp ()
{
  return (THEAPP.MLTeXP());
}

/* _________________________________________________________________________

   miktexgetsynchronizationoptions
   _________________________________________________________________________ */

inline
int
miktexgetsynchronizationoptions ()
{
  return (THEAPP.GetSynchronizationOptions());
}

/* _________________________________________________________________________

   Web2C compatibility names
   _________________________________________________________________________ */

#if 1

inline
bool
restrictedshell ()
{
  return (THEAPP.GetWrite18Mode() == TeXApp::Write18Mode::PartiallyEnabled
    || THEAPP.GetWrite18Mode() == TeXApp::Write18Mode::Query);
}

inline
bool
shellenabledp ()
{
  return (miktexwrite18p());
}

template<class CharType>
int
runsystem (/*[in]*/ const CharType * lpszCommand)
{
  return (miktexwrite18(reinterpret_cast<const TEXMFCHAR *>(lpszCommand)));
}

template<class CharType>
const CharType *
conststringcast (/*[in]*/ CharType * lpsz)
{
  return (const_cast<const CharType *>(lpsz));
}

#endif

MIKTEXMF_END_NAMESPACE;

#undef MIKTEXMF_BEGIN_NAMESPACE
#undef MIKTEXMF_END_NAMESPACE

#endif	// texapp.inl
