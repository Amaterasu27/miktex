/* internal.h: internal definitions				-*- C++ -*-

   Copyright (C) 2001-2005 Christian Schenk

   This file is part of MPC.

   MPC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   MPC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MPC; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace MiKTeX;
using namespace std;

/* _________________________________________________________________________

   Debug-dependant Macros
   _________________________________________________________________________ */

#if ! defined(UNUSED)
#  if ! defined(NDEBUG)
#    define UNUSED(x)
#  else
#    define UNUSED(x) static_cast<void>(x)
#  endif
#endif

#if ! defined(UNUSED_ALWAYS)
#  define UNUSED_ALWAYS(x) static_cast<void>(x)
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

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

/* _________________________________________________________________________

   TempFile
   _________________________________________________________________________ */

class TempFile
{
public:
  TempFile ()
  {
    path.SetToTempFile ();
  }

public:
  TempFile (/*[in]*/ const PathName & path)
    : path (path)
  {
  }

public:
  TempFile &
  operator= (/*[in]*/ const PathName & rhs)
  {
    Delete ();
    path = rhs;
    return (*this);
  }

public:
  void
  Delete ()
  {
    if (path[0] != 0)
    {
      PathName tmp (path);
      path = T_("");
      File::Delete (tmp);
    }
  }

public:
  const PathName &
  GetPathName ()
    const
  {
    return (path);
  }

public:
  ~TempFile ()
  {
    try
    {
      Delete ();
    }
    catch (const exception &)
    {
    }
  }

private:
  PathName path;
};

/* _________________________________________________________________________

   TempDirectory
   _________________________________________________________________________ */

class TempDirectory
{
public:
  TempDirectory ()
  {
    path.SetToTempDirectory ();
  }

public:
  TempDirectory (/*[in]*/ const PathName & path)
    : path (path)
  {
  }

public:
  TempDirectory &
  operator= (/*[in]*/ const PathName & rhs)
  {
    Delete ();
    path = rhs;
    return (*this);
  }

public:
  void
  Delete ()
  {
    if (path[0] != 0)
    {
      PathName tmp (path);
      path = T_("");
      Directory::Delete (tmp, true);
    }
  }

public:
  const PathName &
  GetPathName ()
    const
  {
    return (path);
  }

public:
  ~TempDirectory ()
  {
    try
    {
      Delete ();
    }
    catch (const exception &)
    {
    }
  }

private:
  PathName path;
};

/* _________________________________________________________________________

   NUMTOSTR
   _________________________________________________________________________ */

template<class IntType>
class NumberToStringConverter_
{
private:
  enum { BUFSIZE = 30 };

public:
  NumberToStringConverter_ (/*[in]*/ IntType number)
  {
    int n;
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
    n = _stprintf_s(buffer, BUFSIZE, T_("%d"), static_cast<int>(number));
#  else
    n = _stprintf(buffer, T_("%d"), static_cast<int>(number));
#  endif
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: NumberToStringConverter_::NumberToStringConverter_()
#else
    n = sprintf(buffer, T_("%"), static_cast<int>(number));
#endif
    if (n < 0)
      {
	FATAL_CRT_ERROR (T_("sprintf"), 0);
      }
  }

public:
  const MIKTEXCHAR *
  GetBuffer ()
    const
  {
    return (buffer);
  }

private:
  MIKTEXCHAR buffer[BUFSIZE];
};

#define NUMTOSTR(type, num) NumberToStringConverter_<type>(num).GetBuffer()

