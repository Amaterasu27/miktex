/* miktex/Core/Debug.h: MiKTeX debug macros			-*- C++ -*-

   Copyright (C) 1996-2008 Christian Schenk

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

#if ! defined(MIKTEXCOREEXPORT)
#  error Core must be included before Debug
#endif

#include <miktex/Core/Text>

#if defined(__cplusplus)
#  include <cstdlib>
#else
#  include <stdlib.h>
#endif

#define MIKTEX_DEBUG_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace Debug {

#define MIKTEX_DEBUG_END_NAMESPACE		\
    }						\
  }

#if defined(__cplusplus)
MIKTEX_DEBUG_BEGIN_NAMESPACE;

MIKTEXCOREEXPORT
void
MIKTEXNORETURN
MIKTEXCEECALL
FatalMiKTeXError (/*[in]*/ const char *	lpszMiktexFunction,
		  /*[in]*/ const char *	lpszMessage,
		  /*[in]*/ const char *	lpszInfo,
		  /*[in]*/ const char *	lpszSourceFile,
		  /*[in]*/ int		lpszSourceLine);

MIKTEXCORECEEAPI(void *)
Malloc (/*[in]*/ size_t		size,
	/*[in]*/ const char *	lpszFileName,
	/*[in]*/ int		line);

MIKTEXCORECEEAPI(void *)
Realloc (/*[in]*/ void *	ptr,
	 /*[in]*/ size_t	size,
	 /*[in]*/ const char *	lpszFileName,
	 /*[in]*/ int		line);

MIKTEXCORECEEAPI(void *)
Calloc (/*[in]*/ size_t		num,
	/*[in]*/ size_t		size,
	/*[in]*/ const char *	lpszFileName,
	/*[in]*/ int		line);

#if defined(_MSC_VER)
#  pragma push_macro("StrDup")
#  undef StrDup
#endif

MIKTEXCORECEEAPI(char *)
StrDup (/*[in]*/ const char *	lpsz,
	/*[in]*/ const char *	lpszFileName,
	/*[in]*/ int		line);

#if defined(_MSC_VER)
#  pragma pop_macro("StrDup")
#endif

MIKTEXCORECEEAPI(void)
Free (/*[in]*/ void *		ptr,
      /*[in]*/ const char *	lpszFileName,
      /*[in]*/ int		line);

MIKTEX_DEBUG_END_NAMESPACE;
#endif /* C++ */

MIKTEX_BEGIN_EXTERN_C_BLOCK;

MIKTEXCORECEEAPI(void *)
miktex_core_malloc (/*[in]*/ size_t		size,
		    /*[in]*/ const char *	lpszFileName,
		    /*[in]*/ int		line);

MIKTEXCORECEEAPI(void *)
miktex_core_realloc (/*[in]*/ void *		ptr,
		     /*[in]*/ size_t		size,
		     /*[in]*/ const char *	lpszFileName,
		     /*[in]*/ int		line);

MIKTEXCORECEEAPI(void *)
miktex_core_calloc (/*[in]*/ size_t		num,
		    /*[in]*/ size_t		size,
		    /*[in]*/ const char *	lpszFileName,
		    /*[in]*/ int		line);

MIKTEXCORECEEAPI(char *)
miktex_core_strdup (/*[in]*/ const char *	s,
		    /*[in]*/ const char *	lpszFileName,
		    /*[in]*/ int		line);

MIKTEXCORECEEAPI(void)
miktex_core_free (/*[in]*/ void * 	ptr,
		  /*[in]*/ const char *	lpszFileName,
		  /*[in]*/ int		line);

MIKTEX_END_EXTERN_C_BLOCK;

/* _________________________________________________________________________

   MIKTEX_MALLOC
   _________________________________________________________________________ */

#define MIKTEX_MALLOC(size)				\
  MiKTeX::Debug::Malloc((size), __FILE__, __LINE__)

/* _________________________________________________________________________

   MIKTEX_REALLOC
   _________________________________________________________________________ */

#define MIKTEX_REALLOC(ptr, size)				\
  MiKTeX::Debug::Realloc((ptr), (size), __FILE__, __LINE__)

/* _________________________________________________________________________

   MIKTEX_CALLOC
   _________________________________________________________________________ */

#define MIKTEX_CALLOC(num, size)				\
  MiKTeX::Debug::Calloc(num, size, __FILE__, __LINE__)

/* _________________________________________________________________________

   MIKTEX_STRDUP
   _________________________________________________________________________ */

#  define MIKTEX_STRDUP(str)					\
  MiKTeX::Debug::StrDup((str), __FILE__, __LINE__)

/* _________________________________________________________________________

   MIKTEX_FREE
   _________________________________________________________________________ */

#define MIKTEX_FREE(ptr)				\
  MiKTeX::Debug::Free((ptr), __FILE__, __LINE__)

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
            __FILE__,				\
            __LINE__),				\
       0))

#else

#  define MIKTEX_ASSERT(expr) (static_cast<void>(0))

#endif

/* _________________________________________________________________________

   MIKTEX_ASSUME
   _________________________________________________________________________ */

#if defined(_MSC_VER)

#  define MIKTEX_ASSUME(expr) __assume(expr)

#else

#  define MIKTEX_ASSUME(expr)

#endif

/* _________________________________________________________________________

   MIKTEX_ASSERT_VALID_HEAP
   _________________________________________________________________________ */

#if defined(MIKTEX_DEBUG)

#  define MIKTEX_ASSERT_VALID_HEAP() MiKTeX::Debug::AssertValidHeap ()

#else

#  define MIKTEX_ASSERT_VALID_HEAP()

#endif

/* _________________________________________________________________________

   MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL
   _________________________________________________________________________ */

#if defined(MIKTEX_DEBUG)

#  define MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL(ptr)	\
  if (ptr != 0)						\
    {							\
      MiKTeX::Debug::AssertValidHeapPointer (ptr);	\
    }
#else

#  define MIKTEX_ASSERT_VALID_HEAP_POINTER_OR_NIL()

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

#if defined(__cplusplus)
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
#endif

/* _________________________________________________________________________

   AssertValidString
   _________________________________________________________________________ */

#if defined(__cplusplus)
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

/* _________________________________________________________________________

   AssertValidHeap
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEX_DEBUG_BEGIN_NAMESPACE;

inline
void
AssertValidHeap ()
{
#if defined(_MSC_VER)
#  if defined(_DEBUG)
  MIKTEX_ASSERT (_CrtCheckMemory());
#  else
  int heapStatus;
  MIKTEX_ASSERT ((heapStatus = _heapchk()) == _HEAPOK
		 || (heapStatus == _HEAPEMPTY));
#  endif
#endif
}

MIKTEX_DEBUG_END_NAMESPACE;
#endif

/* _________________________________________________________________________

   AssertValidHeapPointer
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEX_DEBUG_BEGIN_NAMESPACE;

inline
void
AssertValidHeapPointer (/*[in]*/ const void * ptr)
{
#if defined(_MSC_VER)
#  if defined(_DEBUG)
  MIKTEX_ASSERT (_CrtIsValidHeapPointer(ptr));
#  endif
#endif
}

MIKTEX_DEBUG_END_NAMESPACE;
#endif

#endif
