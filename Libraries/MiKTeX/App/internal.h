/* internal.h: internal definitions				-*- C++ -*-

   Copyright (C) 2005-2006 Christian Schenk

   This file is part of the MiKTeX App Library.

   The MiKTeX App Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX App Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX App Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if defined(_MSC_VER) && ! defined(MIKTEX_STATIC)
#  define MIKTEXAPPEXPORT __declspec(dllexport)
#else
#  define MIKTEXAPPEXPORT
#endif

#define BDF6E2537F116547846406B5B2B65949

#include <miktex/app.h>

using namespace MiKTeX::App;
using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace std;

#if defined(MIKTEX_WINDOWS)
using namespace MiKTeX::UI;
#endif

/* _________________________________________________________________________

   Macros
   _________________________________________________________________________ */

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

#if defined(MIKTEX_UNICODE)
#  define tcout wcout
#  define tcerr wcerr
#else
#  define tcout cout
#  define tcerr cerr
#endif

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
                    T_(__FILE__),				\
		    __LINE__)

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  Session::FatalMiKTeXError (miktexFunction,				\
			     traceMessage,				\
			     lpszInfo,					\
			     T_(__FILE__),				\
			     __LINE__)

#define CRT_ERROR(lpszCrtFunction, lpszInfo)		\
  TraceStream::TraceLastCRTError (lpszCrtFunction,	\
				  lpszInfo,		\
				  T_(__FILE__),		\
				  __LINE__)

#define FATAL_CRT_ERROR(lpszCrtFunction, lpszInfo)	\
  Session::FatalCrtError (lpszCrtFunction,		\
			  lpszInfo,			\
			  T_(__FILE__),			\
			  __LINE__)

#define FATAL_CRT_ERROR_2(lpszCrtFunction, errorCode, lpszInfo)	\
  Session::FatalCrtError (lpszCrtFunction,			\
			  errorCode,				\
			  lpszInfo,				\
			  T_(__FILE__),				\
			  __LINE__)

#if defined(MIKTEX_WINDOWS)
#  define WINDOWS_ERROR(lpszWindowsFunction, lpszInfo)		\
  TraceStream::TraceLastWin32Error (lpszWindowsFunction,	\
				    lpszInfo,			\
				    T_(__FILE__),		\
				    __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,			\
			      lpszInfo,				\
			      T_(__FILE__),			\
			      __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR_2(windowsfunction, errorCode, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,				\
			      errorCode,				\
			      lpszInfo,					\
			      T_(__FILE__),				\
			      __LINE__)
#endif

/* _________________________________________________________________________

   FPrintF
   _________________________________________________________________________ */

inline
int
FPrintF (/*[in]*/ FILE *		pfile,
	 /*[in]*/ const MIKTEXCHAR *	lpszFormat,
	 /*[in]*/ const MIKTEXCHAR *	lpsz)
{
#if defined(_MSC_VER)
  return (_ftprintf(pfile, lpszFormat, lpsz));
#else
  return (fprintf(pfile, lpszFormat, lpsz));
#endif
}

/* _________________________________________________________________________

   FPutS
   _________________________________________________________________________ */

inline
MIKTEXCHARINT
FPutS (/*[in]*/ const MIKTEXCHAR *	lpsz,
       /*[in]*/ FILE *			stream)
{
#if defined(_MSC_VER)
  MIKTEXCHARINT n = _fputts(lpsz, stream);
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: FPutS()
#else
  int n = fputs(lpsz, stream);
#endif
  if (n < 0)
    {
      FATAL_CRT_ERROR (T_("fputs"), 0);
    }
  return (n);
}

/* _________________________________________________________________________

   FPutC
   _________________________________________________________________________ */

inline
MIKTEXCHARINT
FPutC (/*[in]*/ MIKTEXCHARINT	ch,
       /*[in]*/ FILE *		stream)
{
#if defined(_MSC_VER)
  MIKTEXCHARINT chWritten = _fputtc(ch, stream);
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: FPutC()
#else
  int chWritten = fputc(ch, stream);
#endif
  if (chWritten != ch)
    {
      FATAL_CRT_ERROR (T_("fputc"), 0);
    }
  return (chWritten);
}
