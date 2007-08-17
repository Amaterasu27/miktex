/* debug.h: MiKTeX debug macros					-*- C++ -*-

   Copyright (C) 1996-2007 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(ADD61B45737D4F4ABCD5725E989A2EDA_)
#define ADD61B45737D4F4ABCD5725E989A2EDA_

#define MIKTEX_DEBUG_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace Debug {

#define MIKTEX_DEBUG_END_NAMESPACE		\
    }						\
  }

MIKTEX_DEBUG_BEGIN_NAMESPACE;

MIKTEXEXPORT
void
MIKTEXNORETURN
MIKTEXCALL
FatalMiKTeXError (/*[in]*/ const MIKTEXCHAR *	lpszMiktexFunction,
		  /*[in]*/ const MIKTEXCHAR *	lpszMessage,
		  /*[in]*/ const MIKTEXCHAR *	lpszInfo,
		  /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		  /*[in]*/ int			lpszSourceLine);

MIKTEX_DEBUG_END_NAMESPACE;

/* _________________________________________________________________________

   MIKTEX_ASSERT
   _________________________________________________________________________ */

#if defined(MIKTEX_DEBUG)

#  define MIKTEX_ASSERT(expr)			\
  static_cast<void>				\
   ((expr)					\
    ? 0						\
    : (MiKTeX::Debug::FatalMiKTeXError		\
           (0,					\
            MIKTEXTEXT("Assertion failed."),	\
            #expr,				\
            MIKTEXTEXT(__FILE__),		\
            __LINE__)))

#else

#  define MIKTEX_ASSERT(expr)

#endif

/* _________________________________________________________________________

   MIKTEX_ASSERT_BUFFER
   _________________________________________________________________________ */

#if defined(MIKTEX_DEBUG)

#  define MIKTEX_ASSERT_BUFFER(buf, n) MiKTeX::Debug::AssertValidBuf (buf, n)

#else

#  define MIKTEX_ASSERT_BUFFER(buf, n)

#endif

/* _________________________________________________________________________

   MIKTEX_ASSERT_BUFFER_OR_NIL
   _________________________________________________________________________ */

#if defined(MIKTEX_DEBUG)

#  define MIKTEX_ASSERT_BUFFER_OR_NIL(buf, n)	\
  if (buf != 0)					\
    {						\
      MiKTeX::Debug::AssertValidBuf (buf, n);	\
    }

#else

#  define MIKTEX_ASSERT_BUFFER_OR_NIL(buf, n)

#endif

/* _________________________________________________________________________

   MIKTEX_ASSERT_CHAR_BUFFER
   _________________________________________________________________________ */

#if defined(MIKTEX_DEBUG)

#  define MIKTEX_ASSERT_CHAR_BUFFER(buf, n)			\
  MiKTeX::Debug::AssertValidBuf (buf, sizeof(buf[0]) * (n))

#else

#  define MIKTEX_ASSERT_CHAR_BUFFER(buf, n)

#endif

/* _________________________________________________________________________

   MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL
   _________________________________________________________________________ */

#if defined(MIKTEX_DEBUG)

#  define MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL(buf, n)			\
  if (buf != 0)								\
    {									\
      MiKTeX::Debug::AssertValidBuf (buf, sizeof(buf[0]) * (n));	\
    }

#else

#  define MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL(buf, n)

#endif

/* _________________________________________________________________________

   MIKTEX_ASSERT_STRING
   _________________________________________________________________________ */

#if defined(MIKTEX_DEBUG)

#  define MIKTEX_ASSERT_STRING(str) MiKTeX::Debug::AssertValidString (str)

#else

#  define MIKTEX_ASSERT_STRING(str)

#endif

/* _________________________________________________________________________

   MIKTEX_ASSERT_STRING_OR_NIL
   _________________________________________________________________________ */

#if defined(MIKTEX_DEBUG)

#  define MIKTEX_ASSERT_STRING_OR_NIL(str)	\
  if (str != 0)					\
    {						\
      MiKTeX::Debug::AssertValidString (str);	\
    }

#else

#  define MIKTEX_ASSERT_STRING_OR_NIL(str)

#endif

/* _________________________________________________________________________

   MIKTEX_ASSERT_CHAR_BUFFER
   _________________________________________________________________________ */

#  define MIKTEX_ASSERT_PATH_BUFFER(buf) \
  MIKTEX_ASSERT_CHAR_BUFFER (buf, MiKTeX::Core::BufferSizes::MaxPath)

/* _________________________________________________________________________

   MIKTEX_ASSERT_PATH_BUFFER_OR_NIL
   _________________________________________________________________________ */

#  define MIKTEX_ASSERT_PATH_BUFFER_OR_NIL(buf) \
  MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL (buf, MiKTeX::Core::BufferSizes::MaxPath)

/* _________________________________________________________________________

   MIKTEX_ASSERT_FNAME_BUFFER
   _________________________________________________________________________ */

#  define MIKTEX_ASSERT_FNAME_BUFFER(buf) \
  MIKTEX_ASSERT_CHAR_BUFFER (buf, MiKTeX::Core::BufferSizes::MaxPath)

/* _________________________________________________________________________

   AssertValidBuf
   _________________________________________________________________________ */

MIKTEX_DEBUG_BEGIN_NAMESPACE;

inline
void
AssertValidBuf (/*[in]*/ void *		lp,
		/*[in]*/ size_t		n)
{
  lp;
  n;
  MIKTEX_ASSERT (lp != 0);
#if defined(MIKTEX_WINDOWS)
  MIKTEX_ASSERT (! IsBadWritePtr(lp, n));
#endif
}

MIKTEX_DEBUG_END_NAMESPACE;

/* _________________________________________________________________________

   AssertValidString
   _________________________________________________________________________ */

MIKTEX_DEBUG_BEGIN_NAMESPACE;

inline
void
AssertValidString (/*[in]*/ const char *	lp,
		   /*[in]*/ size_t		n = 4096)
{
  lp;
  n;
  MIKTEX_ASSERT (lp != 0);
#if defined(MIKTEX_WINDOWS)
  MIKTEX_ASSERT (! IsBadStringPtrA(lp, n));
#endif
}

inline
void
AssertValidString (/*[in]*/ const wchar_t *	lp,
		   /*[in]*/ size_t		n = 4096)
{
  lp;
  n;
  MIKTEX_ASSERT (lp != 0);
#if defined(MIKTEX_WINDOWS)
  MIKTEX_ASSERT (! IsBadStringPtrW(lp, n));
#endif
}

MIKTEX_DEBUG_END_NAMESPACE;

#endif
