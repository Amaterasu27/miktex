/* mpm.h:							-*- C++ -*-

   Copyright (C) 2002-2007 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#pragma once

#include "resource.h"

/* _________________________________________________________________________

   Debug-dependant Macros
   _________________________________________________________________________ */

#if defined(MIKTEX_DEBUG)

#  define MIKTEX_ASSERT(expr)					\
  static_cast<void>						\
   ((expr)							\
    ? 0								\
    : (FATAL_MIKTEX_ERROR (0, T_("Assertion failed."), #expr),	\
       0))

#else

#  define MIKTEX_ASSERT(expr)

#endif

/* _________________________________________________________________________

   Error Macros
   _________________________________________________________________________ */

#define INVALID_ARGUMENT(function, param)				\
  FATAL_MIKTEX_ERROR (function, T_("Invalid argument."), param)

#define UNEXPECTED_CONDITION(function)				\
  FATAL_MIKTEX_ERROR (function, T_("Unexpected condition."), 0)

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  Session::FatalMiKTeXError (miktexFunction,				\
			     traceMessage,				\
			     lpszInfo,					\
			     T_(__FILE__),				\
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

#define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,		\
			      lpszInfo,			\
			      T_(__FILE__),		\
			      __LINE__)

#define CHECK_WINDOWS_ERROR(windowsFunction, lpszInfo)	\
  if (::GetLastError() != ERROR_SUCCESS)		\
    {							\
      FATAL_WINDOWS_ERROR (windowsFunction, lpszInfo);	\
    }

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

/* _________________________________________________________________________

   IsWindowsVista
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
inline
bool
IsWindowsVista ()
{
  return ((GetVersion() & 0xff) >= 6);
}
#endif

/* _________________________________________________________________________

   NUMTOSTR
   _________________________________________________________________________ */

class NumberToStringConverter_
{
private:
  enum { BUFSIZE = 30 };

public:
  NumberToStringConverter_ (/*[in]*/ unsigned u)
  {
    int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
    n = _stprintf_s (buffer, BUFSIZE, T_("%u"), u);
#  else
    n = _stprintf (buffer, T_("%u"), u);
#  endif
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: NumberToStringConverter_::NumberToStringConverter_()
#else
    n = sprintf (buffer, T_("%u"), u);
#endif
    if (n < 0)
      {
	FATAL_CRT_ERROR (T_("sprintf"), 0);
      }
  }

public:
  NumberToStringConverter_ (/*[in]*/ time_t t)
  {
    int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
    n = _stprintf_s (buffer, BUFSIZE, T_("%u"), static_cast<unsigned>(t));
#  else
    n = _stprintf (buffer, T_("%u"), static_cast<unsigned>(t));
#  endif
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: NumberToStringConverter_::NumberToStringConverter_()
#else
    n = sprintf (buffer, T_("%u"), static_cast<unsigned>(t));
#endif
    if (n < 0)
      {
	FATAL_CRT_ERROR (T_("sprintf"), 0);
      }
  }

public:
  NumberToStringConverter_ (/*[in]*/ unsigned long dw)
  {
    unsigned u = static_cast<unsigned>(dw);
    int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
    n = _stprintf_s (buffer, BUFSIZE, T_("%u"), u);
#  else
    n = _stprintf (buffer, T_("%u"), u);
#  endif
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: NumberToStringConverter_::NumberToStringConverter_()
#else
    n = sprintf (buffer, T_("%u"), u);
#endif
    if (n < 0)
      {
	FATAL_CRT_ERROR (T_("sprintf"), 0);
      }
  }

public:
  NumberToStringConverter_ (/*[in]*/ int i)
  {
    int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
    n = _stprintf_s (buffer, BUFSIZE, T_("%d"), i);
#  else
    n = _stprintf (buffer, T_("%d"), i);
#  endif
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: NumberToStringConverter_::NumberToStringConverter_()
#else
    n = sprintf (buffer, T_("%d"), i);
#endif
    if (n < 0)
      {
	FATAL_CRT_ERROR (T_("sprintf"), 0);
      }
  }

public:
  NumberToStringConverter_ (/*[in]*/ double f)
  {
    int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
    n = _stprintf_s (buffer, BUFSIZE, T_("%f"), f);
#  else
    n = _stprintf (buffer, T_("%f"), f);
#  endif
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: NumberToStringConverter_::NumberToStringConverter_()
#else
    n = sprintf (buffer, T_("%f"), f);
#endif
    if (n < 0)
      {
	FATAL_CRT_ERROR (T_("sprintf"), 0);
      }
  }

public:
  NumberToStringConverter_ (/*[in]*/ bool b)
  {
    unsigned u = (b ? 1 : 0);
    int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
    n = _stprintf_s (buffer, BUFSIZE, T_("%u"), u);
#  else
    n = _stprintf (buffer, T_("%u"), u);
#  endif
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: NumberToStringConverter_::NumberToStringConverter_()
#else
    n = sprintf (buffer, T_("%u"), u);
#endif
    if (n < 0)
      {
	FATAL_CRT_ERROR (T_("sprintf"), 0);
      }
  }

public:
  const MIKTEXCHAR *
  Get ()
  {
    return (buffer);
  }

private:
  MIKTEXCHAR buffer[BUFSIZE];
};

#define NUMTOSTR(num) NumberToStringConverter_(num).Get()

/* _________________________________________________________________________

   PackageManagerApplication
   _________________________________________________________________________ */

class PackageManagerApplication : public CWinApp
{
protected:
  DECLARE_MESSAGE_MAP();

public:
  PackageManagerApplication ();

public:
  virtual
  BOOL
  InitInstance ();

public:
  afx_msg
  void
  OnAppAbout ();

public:
  virtual
  int
  ExitInstance ();

private:
  SessionWrapper pSession;
};

extern PackageManagerApplication theApp;
