/* internal.h: internal definitions				-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

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

#if defined(_MSC_VER) && defined(TEXMF_DLL)
#  define MIKTEXMFEXPORT __declspec(dllexport)
#  define C4PEXPORT __declspec(dllexport)
#else
#  define MIKTEXMFEXPORT
#  define C4PEXPORT
#endif

#define C1F0C63F01D5114A90DDF8FC10FF410B
#define B8C7815676699B4EA2DE96F0BD727276
#include "miktex/c4plib.h"
#include "miktex/mfapp.h"
#include "miktex/texapp.h"

using namespace C4P;
using namespace MiKTeX::Packages;
using namespace MiKTeX::TeXAndFriends;
using namespace MiKTeX::UI;
using namespace MiKTeX::Core;
using namespace std;

#define FIRST_OPTION_VAL 256

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

   AutoResource
   _________________________________________________________________________ */

template<class HandleType>
HandleType
InvalidHandleValue ()
{
  return (0);
}

template<class HandleType,
	 class Destroyer>
class AutoResource
{
public:
  AutoResource (/*[in]*/ HandleType handle = InvalidHandleValue<HandleType>())
    : handle (handle)
  {
  }

public:
  ~AutoResource ()
  {
    try
      {
	Reset ();
      }
    catch (const MiKTeXException &)
      {
      }
  }

public:
  HandleType
  Get ()
    const
  {
    return (handle);
  }

public:
  void
  Attach (/*[in]*/ HandleType handle)
  {
    Reset ();
    this->handle = handle;
  }

public:
  HandleType
  Detach ()
  {
    HandleType handle = Get();
    this->handle = InvalidHandleValue<HandleType>();
    return (handle);
  }

public:
  void
  Reset ()
  {
    if (handle != InvalidHandleValue<HandleType>())
      {
	HandleType tmp = handle;
	handle = InvalidHandleValue<HandleType>();
	Destroyer() (tmp);
      }
  }

public:
  HandleType *
  operator & ()
  {
    return (&handle);
  }

private:
  HandleType handle;
};

/* _________________________________________________________________________

   AutoFILE
   _________________________________________________________________________ */

class fclose_
{
public:
  void
  operator() (/*[in]*/ FILE * pFile)
  {
    fclose (pFile);
  }
};

typedef AutoResource<FILE *, fclose_> AutoFILE;

/* _________________________________________________________________________

   AutoMemoryPointer
   _________________________________________________________________________ */

class free_
{
public:
  void
  operator() (/*[in]*/ void * p)
  {
    free (p);
  }
};

typedef AutoResource<void *, free_> AutoMemoryPointer;

/* _________________________________________________________________________

   IsWindowsNT
   _________________________________________________________________________ */

inline
bool
IsWindowsNT ()
{
  return (GetVersion () < 0x80000000);
}

/* _________________________________________________________________________

   ParseFileName
   _________________________________________________________________________ */

inline
void
ParseFileName (/*[in]*/ tstring &		filename,
	       /*[in]*/ const MIKTEXCHAR * &	lpsz)
{
  while (*lpsz != 0 && ! _istspace(*lpsz))
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
SkipSpace (/*[in]*/ const MIKTEXCHAR * &	lpsz)
{
  while (_istspace(*lpsz))
    {
      ++ lpsz;
    }
}

inline
void
SkipSpace (/*[in]*/ LPTSTR &	lpsz)
{
  while (_istspace(*lpsz))
    {
      ++ lpsz;
    }
}

/* _________________________________________________________________________

   NUMTOSTR
   _________________________________________________________________________ */

class NUMTOSTRHELPER
{
public:
  NUMTOSTRHELPER (unsigned u)
  {
#if _MSC_VER >= 1400
    _stprintf_s (szBuf, ARRAY_SIZE(szBuf), _T("%u"), u);
#else
    _stprintf (szBuf, _T("%u"), u);
#endif
  }

public:
  NUMTOSTRHELPER (DWORD dw)
  {
    unsigned u = dw;
#if _MSC_VER >= 1400
    _stprintf_s (szBuf, ARRAY_SIZE(szBuf), _T("%u"), u);
#else
    _stprintf (szBuf, _T("%u"), u);
#endif
  }

public:
  NUMTOSTRHELPER (int i)
  {
#if _MSC_VER >= 1400
    _stprintf_s (szBuf, ARRAY_SIZE(szBuf), _T("%d"), i);
#else
    _stprintf (szBuf, _T("%d"), i);
#endif
  }

public:
  operator const MIKTEXCHAR * () const
  {
    return (szBuf);
  }

private:
  enum { BUFSIZE = 30 };

private:
  _TCHAR szBuf[BUFSIZE];
};

#define NUMTOSTR(num) static_cast<const MIKTEXCHAR *>(NUMTOSTRHELPER(num))

/* _________________________________________________________________________

   STRDUP
   _________________________________________________________________________ */

class STRDUP
{
private:
  enum { BUFSIZE = 512 };

private:
  _TCHAR szBuf[BUFSIZE];

private:
  LPTSTR lpszBuf;

public:
  STRDUP (/*[in]*/ const MIKTEXCHAR * lpsz)
  {
    size_t l = _tcslen(lpsz);
    if (l < BUFSIZE)
      {
	Utils::CopyString (szBuf, BUFSIZE, lpsz);
	lpszBuf = szBuf;
      }
    else
      {
	lpszBuf = _tcsdup(lpsz);
      }
  }

public:
  ~STRDUP ()
  {
    if (lpszBuf != 0 && lpszBuf != szBuf)
      {
	free (lpszBuf);
	lpszBuf = 0;
      }
  }

public:
  operator LPTSTR ()
  {
    return (lpszBuf);
  }
};

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

   GetC
   _________________________________________________________________________ */

inline
MIKTEXCHARINT
GetC (/*[in]*/ FILE *		stream)
{
#if defined(MIKTEX_UNICODE)
  MIKTEXCHARINT ch = getwc(stream);
#else
  MIKTEXCHARINT ch = getc(stream);
#endif
  if (ch == MIKTEXEOF && ferror(stream) != 0)
    {
      FATAL_CRT_ERROR (T_("getc"), 0);
    }
  return (ch);
}

/* _________________________________________________________________________

   IsDigit
   _________________________________________________________________________ */

inline
int
IsDigit (/*[in]*/ MIKTEXCHARINT ch)
{
#if defined(_MSC_VER)
  return (_istdigit(ch));
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: IsDigit()
#else
  return (isdigit(ch));
#endif
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
