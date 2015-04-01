/* internal.h: internal definitions			-*- C++ -*-

   Copyright (C) 1996-2015 Christian Schenk

   This file is part of the MiKTeX TeXMF Library.

   The MiKTeX TeXMF Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX TeXMF Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX TeXMF Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if defined(MIKTEX_TEXMF_SHARED) && defined(_MSC_VER)
#  define C4PEXPORT __declspec(dllexport)
#  define MIKTEXMFEXPORT __declspec(dllexport)
#elif defined(MIKTEX_TEXMF_SHARED) && __GNUC__ >=4
#  define C4PEXPORT __attribute__((visibility("default")))
#  define MIKTEXMFEXPORT __attribute__((visibility("default")))
#else
#  define C4PEXPORT
#  define MIKTEXMFEXPORT
#endif

#define B8C7815676699B4EA2DE96F0BD727276
#define C1F0C63F01D5114A90DDF8FC10FF410B
#include "miktex/C4P/C4P"
#include "miktex/TeXAndFriends/MetafontApp"
#include "miktex/TeXAndFriends/ETeXApp"

using namespace C4P;
using namespace MiKTeX::Packages;
using namespace MiKTeX::TeXAndFriends;
using namespace MiKTeX::Core;
using namespace std;

#define FIRST_OPTION_VAL 256

#define BEGIN_INTERNAL_NAMESPACE		\
namespace MiKTeX {				\
  namespace B3EB240141E54EF2BE3E8E2C742908B9 {

#define END_INTERNAL_NAMESPACE			\
  }						\
}

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

BEGIN_INTERNAL_NAMESPACE;


/* _________________________________________________________________________

   Macros
   _________________________________________________________________________ */

#define STATICFUNC(type) type

#define PERMANENTVAR(type) static type
#define INTERNALFUNC(type) type

#define ARRAY_SIZE(buf) (sizeof(buf)/sizeof(buf[0]))

#define MIKTEX_API_BEGIN(funcname) {
#define MIKTEX_API_END(funcname) }

#if ! defined(UNUSED)
#  if ! defined(NDEBUG)
#    define UNUSED(x)
#  else
#    define UNUSED(x) x
#  endif
#endif

#if ! defined(UNUSED_ALWAYS)
#  define UNUSED_ALWAYS(x) x
#endif

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) MiKTeX::Core::Quoter<char>(x).Get()

/* _________________________________________________________________________

   Error Macros
   _________________________________________________________________________ */

#define BUF_TOO_SMALL(function)						\
  FATAL_MIKTEX_ERROR (function,						\
                      T_("Not enough room in an internal buffer."),	\
                      0)

#define INVALID_ARGUMENT(function, param)				\
  FATAL_MIKTEX_ERROR (function, T_("Invalid argument."), param)

#define OUT_OF_MEMORY(function)					\
  FATAL_MIKTEX_ERROR(function, T_("Virtual memory exhausted."), 0)

#define UNEXPECTED_CONDITION(function)				\
  FATAL_MIKTEX_ERROR (function, T_("Unexpected condition."), 0)

#define UNIMPLEMENTED(function)						\
  FATAL_MIKTEX_ERROR (function, T_("Function not implemented."), 0)

#define MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  TraceMiKTeXError (miktexFunction,				\
                    traceMessage,				\
                    lpszInfo,					\
                    __FILE__,					\
		    __LINE__)

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  Session::FatalMiKTeXError (miktexFunction,				\
			     traceMessage,				\
			     lpszInfo,					\
			     __FILE__,					\
			     __LINE__)

#define CRT_ERROR(lpszCrtFunction, lpszInfo)		\
  TraceStream::TraceLastCRTError (lpszCrtFunction,	\
				  lpszInfo,		\
				  __FILE__,		\
				  __LINE__)

#define FATAL_CRT_ERROR(lpszCrtFunction, lpszInfo)	\
  Session::FatalCrtError (lpszCrtFunction,		\
			  lpszInfo,			\
			  __FILE__,			\
			  __LINE__)

#define FATAL_CRT_ERROR_2(lpszCrtFunction, errorCode, lpszInfo)	\
  Session::FatalCrtError (lpszCrtFunction,			\
			  errorCode,				\
			  lpszInfo,				\
			  __FILE__,				\
			  __LINE__)

#if defined(MIKTEX_WINDOWS)
#  define WINDOWS_ERROR(lpszWindowsFunction, lpszInfo)		\
  TraceStream::TraceLastWin32Error (lpszWindowsFunction,	\
				    lpszInfo,			\
				    __FILE__,			\
				    __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,			\
			      lpszInfo,				\
			      __FILE__,				\
			      __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR_2(windowsfunction, errorCode, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,				\
			      errorCode,				\
			      lpszInfo,					\
			      __FILE__,					\
			      __LINE__)
#endif

#if defined(_MSC_VER)
#define UNSUPPORTED_PLATFORM()						\
  __assume(false)
#else
#define UNSUPPORTED_PLATFORM()						\
  MIKTEX_ASSERT (false)
#endif

/* _________________________________________________________________________

   IsNameManglingEnabled
   _________________________________________________________________________ */

#if 1 // 2015-01-17
extern bool IsNameManglingEnabled;
#endif

/* _________________________________________________________________________

   ParseFileName
   _________________________________________________________________________ */

inline
void
ParseFileName (/*[in]*/ string &	filename,
	       /*[in]*/ const char * &	lpsz)
{
  while (*lpsz != 0 && ! isspace(*lpsz))
    {
      filename += *lpsz;
      ++ lpsz;
    }
}

/* _________________________________________________________________________

   SkipSpace
   _________________________________________________________________________ */

inline
void
SkipSpace (/*[in]*/ const char * &	lpsz)
{
  while (isspace(*lpsz))
    {
      ++ lpsz;
    }
}

inline
void
SkipSpace (/*[in]*/ char * &	lpsz)
{
  while (isspace(*lpsz))
    {
      ++ lpsz;
    }
}

/* _________________________________________________________________________

   STRDUP
   _________________________________________________________________________ */

typedef CharBuffer<char> STRDUP;

/* _________________________________________________________________________

   GetC
   _________________________________________________________________________ */

inline
int
GetC (/*[in]*/ FILE *		stream)
{
  int ch = getc(stream);
  if (ch == EOF && ferror(stream) != 0)
    {
      FATAL_CRT_ERROR ("getc", 0);
    }
  return (ch);
}

/* _________________________________________________________________________

   AutoRestore
   _________________________________________________________________________ */

template<class VALTYPE>
class AutoRestore
{
public:
  AutoRestore (/*[in]*/ VALTYPE & val)
    : oldVal (val),
      pVal (&val)
  {
  }

public:
  ~AutoRestore ()
  {
    *pVal = oldVal;
  }

private:
  VALTYPE oldVal;

private:
  VALTYPE * pVal;
};

END_INTERNAL_NAMESPACE

using namespace MiKTeX::B3EB240141E54EF2BE3E8E2C742908B9;
