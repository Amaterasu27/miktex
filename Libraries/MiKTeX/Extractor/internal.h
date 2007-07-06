/* internal.h: internal definitions				-*- C++ -*-

   Copyright (C) 2001-2007 Christian Schenk

   This file is part of MiKTeX Extractor.

   MiKTeX Extractor is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.
   
   MiKTeX Extractor is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Extractor; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER) && ! defined(MIKTEX_STATIC)
#  define EXTRACTOREXPORT __declspec(dllexport)
#else
#  define EXTRACTOREXPORT
#endif

#define DAA6476494C144C8BED9A9E8810BAABA
#include "miktex/Extractor.h"

#include "extractor-version.h"

#define BEGIN_INTERNAL_NAMESPACE			\
namespace MiKTeX {					\
  namespace Extractor {					\
    namespace AF1A1A64A53D45708F96161A1541D424 {

#define END_INTERNAL_NAMESPACE			\
    }						\
  }						\
}

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

#if defined(MIKTEX_WINDOWS)
inline
bool
IsWindowsVista ()
{
  return ((GetVersion() & 0xff) >= 6);
}
#endif

#if defined(MIKTEX_WINDOWS)
inline
bool
IsWindowsNT ()
{
  return (GetVersion() < 0x80000000);
}
#endif

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

#define OUT_OF_MEMORY(function)						\
  FATAL_MIKTEX_ERROR(function, T_("Virtual memory exhausted."), 0)

#define UNEXPECTED_CONDITION(function)				\
  FATAL_MIKTEX_ERROR (function, T_("Unexpected condition."), 0)

#define UNIMPLEMENTED(function)						\
  FATAL_MIKTEX_ERROR (function, T_("Function not implemented."), 0)

#define MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  MiKTeX::Core::TraceMiKTeXError (miktexFunction,		\
                    traceMessage,				\
                    lpszInfo,					\
                    T_(__FILE__),				\
		    __LINE__)

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  MiKTeX::Core::Session::FatalMiKTeXError (miktexFunction,		\
					traceMessage,			\
					 lpszInfo,			\
					 T_(__FILE__),			\
					 __LINE__)

#define CRT_ERROR(lpszCrtFunction, lpszInfo)				\
  MiKTeX::Core::TraceStream::TraceLastCRTError (lpszCrtFunction,	\
				  lpszInfo,				\
				  T_(__FILE__),				\
				  __LINE__)

#define FATAL_CRT_ERROR(lpszCrtFunction, lpszInfo)		\
  MiKTeX::Core::Session::FatalCrtError (lpszCrtFunction,	\
			  lpszInfo,				\
			  T_(__FILE__),				\
			  __LINE__)

#define FATAL_CRT_ERROR_2(lpszCrtFunction, errorCode, lpszInfo)	\
  MiKTeX::Core::Session::FatalCrtError (lpszCrtFunction,	\
			  errorCode,				\
			  lpszInfo,				\
			  T_(__FILE__),				\
			  __LINE__)

#if defined(MIKTEX_WINDOWS)
#  define WINDOWS_ERROR(lpszWindowsFunction, lpszInfo)			\
  MiKTeX::Core::TraceStream::TraceLastWin32Error (lpszWindowsFunction,	\
				    lpszInfo,				\
				    T_(__FILE__),			\
				    __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  MiKTeX::Core::Session::FatalWindowsError (windowsfunction,	\
			      lpszInfo,				\
			      T_(__FILE__),			\
			      __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR_2(windowsfunction, errorCode, lpszInfo)	\
  MiKTeX::Core::Session::FatalWindowsError (windowsfunction,		\
			      errorCode,				\
			      lpszInfo,					\
			      T_(__FILE__),				\
			      __LINE__)
#endif

#define FATAL_EXTRACTOR_ERROR(miktexFunction, traceMessage, lpszInfo) \
  FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)

#  define FATAL_SOAP_ERROR(pSoap)		\
  FatalSoapError (pSoap,			\
                  T_(__FILE__),			\
                  __LINE__)

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) Quoted(x).c_str()

#define EXTRACTORSTATICFUNC(type) static type
#define EXTRACTORINTERNALFUNC(type) type
#define EXTRACTORINTERNALVAR(type) type

#define ARRAY_SIZE(buf) (sizeof(buf)/sizeof(buf[0]))

#include "miktex/Extractor.h"

BEGIN_INTERNAL_NAMESPACE;

typedef std::basic_ostringstream<MIKTEXCHAR> otstringstream;

/* _________________________________________________________________________

   CURRENT_DIRECTORY
   _________________________________________________________________________ */

#define CURRENT_DIRECTORY T_(".")

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
MiKTeX::Core::tstring
Quoted (/*[in]*/ const MIKTEXCHAR * lpsz)
{
  bool needQuotes = (strchr(lpsz, T_(' ')) != 0);
  MiKTeX::Core::tstring result;
  if (needQuotes)
    {
      result += T_('"');
    }
  result += lpsz;
  if (needQuotes)
    {
      result += T_('"');
    }
  return (result);
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
MiKTeX::Core::tstring
Quoted (/*[in]*/ const MiKTeX::Core::tstring & str)
{
  return (Quoted(str.c_str()));
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
MiKTeX::Core::tstring
Quoted (/*[in]*/ const MiKTeX::Core::PathName & path)
{
  return (Quoted(path.Get()));
}

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
    catch (const std::exception &)
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
  Detach ()
  {
    handle = InvalidHandleValue<HandleType>();
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

   AutoCoTaskMem
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class CoTaskMemFree_
{
public:
  void
  operator() (/*[in]*/ void * p)
  {
    CoTaskMemFree (p);
  }
};

typedef AutoResource<void *, CoTaskMemFree_> AutoCoTaskMem;
#endif

/* _________________________________________________________________________

   AutoLocalMem
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class LocalFree_
{
public:
  void
  operator() (/*[in]*/ void * p)
  {
    LocalFree (p);
  }
};

typedef AutoResource<void *, LocalFree_> AutoLocalMem;
#endif

/* _________________________________________________________________________

   AutoSysString
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class SysFreeString_
{
public:
  void
  operator() (/*[in]*/ BSTR bstr)
  {
    SysFreeString (bstr);
  }
};

typedef AutoResource<BSTR, SysFreeString_> AutoSysString;
#endif

/* _________________________________________________________________________

   TempFile
   _________________________________________________________________________ */

class TempFile
{
public:
  TempFile ()
  {
  }

public:
  TempFile (/*[in]*/ const MiKTeX::Core::PathName & path)
    : path (path)
  {
  }

public:
  TempFile &
  operator= (/*[in]*/ const MiKTeX::Core::PathName & path)
  {
    this->path = path;
    return (*this);
  }

public:
  TempFile &
  operator= (/*[in]*/ int zero)
  {
    if (zero != 0)
      {
	UNEXPECTED_CONDITION (T_("TempFile::operator="));
      }
    path = T_("");
    return (*this);
  }

public:
  void
  Delete ()
  {
    if (path[0] != 0)
      {
	MiKTeX::Core::PathName tmp (path);
	path = T_("");
	MiKTeX::Core::File::Delete (tmp);
      }
  }

public:
  ~TempFile ()
  {
    try
      {
	Delete ();
      }
    catch (const std::exception &)
      {
      }
  }

private:
  MiKTeX::Core::PathName path;
};

/* _________________________________________________________________________

   AssertValidBuf
   _________________________________________________________________________ */

inline void
AssertValidBuf (/*[in]*/ void *	lp,
		/*[in]*/ size_t	n)
{
#if defined(_MSC_VER) && defined(MIKTEX_DEBUG)
  MIKTEX_ASSERT (lp != 0);
  MIKTEX_ASSERT (! IsBadWritePtr(lp, n));
#else
  UNUSED (lp);
  UNUSED (n);
#endif
}

/* _________________________________________________________________________

   AssertValidString
   _________________________________________________________________________ */

inline void
AssertValidString (/*[in]*/ const MIKTEXCHAR *	lp,
		   /*[in]*/ size_t	n = 4096)
{
#if defined(_MSC_VER) && defined(MIKTEX_DEBUG)
  MIKTEX_ASSERT (lp != 0);
  MIKTEX_ASSERT (! IsBadStringPtr(lp, n));
#else
  UNUSED (lp);
  UNUSED (n);
#endif
}

/* _________________________________________________________________________

   GetErrnoMessage
   _________________________________________________________________________ */

static
inline
MiKTeX::Core::tstring &
GetErrnoMessage (/*[in]*/ int		err,
		 /*[out]*/ MiKTeX::Core::tstring &	message)
{
#if _MSC_VER >= 1400
  _TCHAR szBuf[256];
  if (strerror_s(szBuf, 256, err) != 0)
    {
      message = T_("");
    }
  else
    {
      message = szBuf;
    }
#else
  message = strerror(err);
#endif
  return (message);
}

/* _________________________________________________________________________

   ClearString
   _________________________________________________________________________ */

inline
bool
ClearString (/*[in,out]*/ MIKTEXCHAR *	lpsz)
{
  lpsz[0] = 0;
  return (true);
}

/* _________________________________________________________________________

   AToI
   _________________________________________________________________________ */

inline
int
AToI (/*[in]*/ const MIKTEXCHAR * lpsz)
{
#if defined(_MSC_VER)
  return (_ttoi(lpsz));
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: AToI()
#else
  return (atoi(lpsz));
#endif
}

/* _________________________________________________________________________

   StrCmp
   _________________________________________________________________________ */

#if defined(StrCmp)
#  undef StrCmp
#endif

inline
int
StrCmp (/*[in]*/ const MIKTEXCHAR *	lpsz1,
	/*[in]*/ const MIKTEXCHAR *	lpsz2)
{
#if defined(MIKTEX_UNICODE)
  return (wcscmp(lpsz1, lpsz2));
#else
  return (strcmp(lpsz1, lpsz2));
#endif
}

/* _________________________________________________________________________

   StrNCmp
   _________________________________________________________________________ */

#if defined(StrNCmp)
#  undef StrNCmp
#endif

inline
int
StrNCmp (/*[in]*/ const MIKTEXCHAR *	lpsz1,
	 /*[in]*/ const MIKTEXCHAR *	lpsz2,
	 /*[in]*/ size_t		n)
{
#if defined(MIKTEX_UNICODE)
  return (wcsncmp(lpsz1, lpsz2, n));
#else
  return (strncmp(lpsz1, lpsz2, n));
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
  if (chWritten == MIKTEXEOF)
    {
      FATAL_CRT_ERROR (T_("fputc"), 0);
    }
  return (chWritten);
}

/* _________________________________________________________________________

   PathNameComparer
   _________________________________________________________________________ */

struct PathNameComparer
  : public std::binary_function<MiKTeX::Core::tstring,
				MiKTeX::Core::tstring,
				bool>
{
  bool
  operator() (/*[in]*/ const MiKTeX::Core::tstring & str1,
	      /*[in]*/ const MiKTeX::Core::tstring & str2)
    const
  {
    return (MiKTeX::Core::PathName::Compare(str1.c_str(), str2.c_str()) < 0);
  }
};

/* _________________________________________________________________________ */

END_INTERNAL_NAMESPACE;

using namespace MiKTeX::Extractor::AF1A1A64A53D45708F96161A1541D424;
