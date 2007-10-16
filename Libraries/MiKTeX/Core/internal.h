/* internal.h: internal definitions				-*- C++ -*-

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

#if defined(_MSC_VER) && ! defined(MIKTEX_STATIC)
#  define MIKTEXEXPORT __declspec(dllexport)
#else
#  define MIKTEXEXPORT
#endif

#define EAD86981C92C904D808A5E6CEC64B90E
#include "MiKTeX/Core/Core"

#include "core-version.h"

#if defined(MIKTEX_WINDOWS)
#  include "MiKTeX/Core/win/DllProc"
#endif

#include "MiKTeX/Core/Debug"
#include "miktex/paths.h"
#include "miktex/reg.h"
#include "miktex/trace.h"
#include "miktex/urls.h"

using namespace MiKTeX::Core;

#define BEGIN_INTERNAL_NAMESPACE		\
namespace MiKTeX {				\
  namespace ABF3880A6239B84E87DC7E727A8BBFD4 {

#define END_INTERNAL_NAMESPACE			\
  }						\
}

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

BEGIN_INTERNAL_NAMESPACE;

#define REPORT_EVENTS 0

#if defined(MIKTEX_WINDOWS)
#  include "MiKTeXEvents.h"
#endif

#if defined(_MSC_VER) && defined(MIKTEX_WINDOWS)
#  pragma comment(lib, "comsuppw.lib")
#endif

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
namespace MiKTeXSessionLib = MAKE_CURVER_ID(MiKTeXSession);
#endif

/* _________________________________________________________________________

   C API Helper

   Calling exit() isn't nice. But it's C.
   _________________________________________________________________________ */

#define C_FUNC_BEGIN()				\
  {						\
    try						\
      {

#define C_FUNC_END()				\
      }						\
    catch (const MiKTeXException & e)		\
      {						\
        if (stderr != 0)			\
	  {					\
	    Utils::PrintException (e);	\
	  }					\
        exit (1);				\
      }						\
    catch (const exception & e)			\
      {						\
        if (stderr != 0)			\
	  {					\
	    Utils::PrintException (e);	\
	  }					\
        exit (1);				\
      }						\
  }

/* _________________________________________________________________________

   DEBUG_BREAK
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
#  define DEBUG_BREAK() DebugBreak()
#else
#  define DEBUG_BREAK()
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

#define DEFINED(x) static_cast<void>(x)

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

#if defined(_MSC_VER)
#define UNSUPPORTED_PLATFORM()						\
  __assume(false)
#else
#define UNSUPPORTED_PLATFORM()						\
  MIKTEX_ASSERT (false)
#endif

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) Quoted(x).Get()

#if defined(MIKTEX_UNICODE)
#  define tcout wcout
#  define tcerr wcerr
#else
#  define tcout cout
#  define tcerr cerr
#endif

#define MIKTEXINTERNALFUNC(type) type
#define MIKTEXINTERNALVAR(type) type

#define ARRAY_SIZE(buf) (sizeof(buf) / sizeof(buf[0]))

#define MAKE_SEARCH_PATH(dir)			\
  TEXMF_PLACEHOLDER				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  dir

#define CFG_MACRO_NAME_BINDIR T_("bindir")
#define CFG_MACRO_NAME_PSFONTDIRS T_("psfontdirs")
#define CFG_MACRO_NAME_OTFDIRS T_("otfdirs")
#define CFG_MACRO_NAME_TTFDIRS T_("ttfdirs")
#define CFG_MACRO_NAME_WINDIR T_("windir")

/* _________________________________________________________________________

   Constants
   _________________________________________________________________________ */

const unsigned PAGESIZE = 0x1000;

#define TEXMF_PLACEHOLDER T_("%R")

#define CURRENT_DIRECTORY T_(".")

#define PARENT_DIRECTORY T_("..")

#define PARENT_PARENT_DIRECTORY			\
  PARENT_DIRECTORY				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  PARENT_DIRECTORY

#define PARENT_PARENT_PARENT_DIRECTORY		\
  PARENT_DIRECTORY				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  PARENT_PARENT_DIRECTORY

#if defined(MIKTEX_WINDOWS)
const MIKTEXCHAR PATH_DELIMITER = T_(';');
#define PATH_DELIMITER_STRING T_(";")
#else
const MIKTEXCHAR PATH_DELIMITER = T_(':');
#define PATH_DELIMITER_STRING T_(":")
#endif

const MIKTEXCHAR * const RECURSION_INDICATOR = T_("//");
const size_t RECURSION_INDICATOR_LENGTH = 2;
const MIKTEXCHAR * const SESSIONSVC = T_("sessionsvc");

/* _________________________________________________________________________

   U32
   _________________________________________________________________________ */

#if defined(_MSC_VER)
typedef unsigned __int32 U32;
#else
typedef uint32_t U32;
#endif

/* _________________________________________________________________________

   otstringstream
   _________________________________________________________________________ */

typedef basic_ostringstream<MIKTEXCHAR> tostringstream;

/* _________________________________________________________________________

   PathNameArray
   _________________________________________________________________________ */

typedef std::vector<PathName> PathNameArray;

/* _________________________________________________________________________

   Functions
   _________________________________________________________________________ */

void
AppendDirectoryDelimiter (/*[in,out]*/ tstring & path);

void
AppendDirectoryDelimiter (/*[in,out]*/ MIKTEXCHAR *	lpszPath,
			  /*[in]*/ size_t		size);

MIKTEXCHARINT
CompareFileNameChars (/*[in]*/ MIKTEXCHAR	ch1,
		      /*[in]*/ MIKTEXCHAR	ch2);
  
void
CopyString2 (/*[out]*/ MIKTEXCHAR *		lpszBuf,
	     /*[in]*/ size_t			bufSize,
	     /*[in]*/ const MIKTEXCHAR *	lpszSource,
	     /*[in]*/ size_t			count);

void
CreateDirectoryPath (/*[in]*/ const MIKTEXCHAR *	lpszPath);

bool
FileIsOnROMedia (/*[in]*/ const MIKTEXCHAR *	lpszPath);
  
bool
GetCrtErrorMessage (/*[in]*/ int		functionResult,
		    /*[out]*/ tstring &		errorMessage);

PathName
GetFullPath (/*[in]*/ const MIKTEXCHAR * lpszPath);

PathName
GetHomeDirectory ();
  
#if defined(MIKTEX_WINDOWS)
bool
GetUserProfileDirectory (/*[out]*/ PathName & path);
#endif
  
  
const MIKTEXCHAR *
GetFileNameExtension (/*[in]*/ const MIKTEXCHAR * lpszPath);
  
bool
HaveEnvironmentString (/*[in]*/ const MIKTEXCHAR * lpszName);

const MIKTEXCHAR *
GetEnvironmentString (/*[in]*/ const MIKTEXCHAR * lpszName);
  
bool
IsExplicitlyRelativePath (/*[in]*/ const MIKTEXCHAR * lpszPath);

bool
IsMpmFile (/*[in]*/ const MIKTEXCHAR * lpszPath);
  
void
RemoveDirectoryDelimiter (/*[in,out]*/ MIKTEXCHAR * lpszPath);
  
#if defined(MIKTEX_WINDOWS)
bool
ReportMiKTeXEvent (/*[in]*/ unsigned short	eventType,
		   /*[in]*/ unsigned long	eventId,
		   /*[in]*/			...);
#endif
  
void
TraceError (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	    /*[in]*/			...);

void
TraceMiKTeXError (/*[in]*/ const MIKTEXCHAR *	lpszMiktexFunction,
		  /*[in]*/ const MIKTEXCHAR *	lpszMessage,
		  /*[in]*/ const MIKTEXCHAR *	lpszInfo,
		  /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		  /*[in]*/ int			lpszSourceLine);
  
#if defined(MIKTEX_WINDOWS)
void
TraceWindowsError (/*[in]*/ const MIKTEXCHAR *	lpszWindowsFunction,
		   /*[in]*/ unsigned long	functionResult,
		   /*[in]*/ const MIKTEXCHAR *	lpszInfo,
		   /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		   /*[in]*/ int			lpszSourceLine);
#endif

/* _________________________________________________________________________

   MakeLong
   _________________________________________________________________________ */

inline
long
MakeLong (/*[in]*/ unsigned a,
	  /*[in]*/ unsigned b)
{
  unsigned long ul = (a & 0xffff);
  ul |= static_cast<unsigned long>(b & 0xffff) << 16;
  return (static_cast<long>(ul));
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

/* _________________________________________________________________________

   StringComparerIgnoringCase
   _________________________________________________________________________ */

struct StringComparerIgnoringCase
  : public std::binary_function<tstring, tstring, bool>
{
  bool
  operator() (/*[in]*/ const tstring & lhs,
	      /*[in]*/ const tstring & rhs)
    const
  {
    return (StringCompare(lhs.c_str(), rhs.c_str(), true) < 0);
  }
};

#if defined(StrDup)
#  undef StrDup
#endif

inline
MIKTEXCHAR *
StrDup (/*[in]*/ const MIKTEXCHAR * lpsz)
{
  MIKTEXCHAR * ret;
#if defined(_MSC_VER)
  ret = _tcsdup(lpsz);
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: StrDup()
#else
  ret = strdup(lpsz);
#endif
  if (ret == 0)
    {
      FATAL_CRT_ERROR (T_("strdup"), 0);
    }
  return (ret);
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

   GetC
   _________________________________________________________________________ */

inline
MIKTEXCHARINT
GetC (/*[in]*/ FILE *	stream)
{
#if defined(_MSC_VER)
  MIKTEXCHARINT ch = _gettc(stream);
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: GetC()
#else
  MIKTEXCHARINT ch = getc(stream);
#endif
  if (ferror(stream) != 0)
    {
      FATAL_CRT_ERROR (T_("getc"), 0);
    }
  return (ch);
}

/* _________________________________________________________________________

   UnGetC
   _________________________________________________________________________ */

inline
void
UnGetC (/*[in]*/ MIKTEXCHARINT	ch,
	/*[in]*/ FILE *		stream)
{
#if defined(_MSC_VER)
  MIKTEXCHARINT ch2 = _ungettc(ch, stream);
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: GetC()
#else
  MIKTEXCHARINT ch2 = ungetc(ch, stream);
#endif
  if (ch2 == MIKTEXEOF)
    {
      FATAL_CRT_ERROR (T_("ungetc"), 0);
    }
}

/* _________________________________________________________________________

   StrPBrk
   _________________________________________________________________________ */

#if defined(StrPBrk)
#  undef StrPBrk
#endif

inline
const MIKTEXCHAR *
StrPBrk (/*[in]*/ const MIKTEXCHAR *	lpsz1,
	 /*[in]*/ const MIKTEXCHAR *	lpsz2)
{
#if defined(_MSC_VER)
  return (_tcspbrk(lpsz1, lpsz2));
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: StrPBrk()
#else
  return (strpbrk(lpsz1, lpsz2));
#endif
}

/* _________________________________________________________________________

   StrStr
   _________________________________________________________________________ */

#if defined(StrStr)
#  undef StrStr
#endif

inline
const MIKTEXCHAR *
StrStr (/*[in]*/ const MIKTEXCHAR *	lpsz1,
	/*[in]*/ const MIKTEXCHAR *	lpsz2)
{
#if defined(MIKTEX_UNICODE)
  return (wcsstr(lpsz1, lpsz2));
#else
  return (strstr(lpsz1, lpsz2));
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

   StrNCmpI
   _________________________________________________________________________ */

inline
int
StrNCmpI (/*[in]*/ const MIKTEXCHAR *	lpsz1,
	  /*[in]*/ const MIKTEXCHAR *	lpsz2,
	  /*[in]*/ size_t		n)
{
#if defined(_MSC_VER)
  return (_tcsnicmp(lpsz1, lpsz2, n));
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: StrNCmpI()
#else
  return (strncasecmp(lpsz1, lpsz2, n));
#endif
}

/* _________________________________________________________________________

   FdOpen
   _________________________________________________________________________ */

inline
FILE *
FdOpen (/*[in]*/ int			fd,
	/*[in]*/ const MIKTEXCHAR *	lpszMode)
{
  FILE * stream;
#if defined(_MSC_VER)
  stream = _tfdopen(fd, lpszMode);
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: FdOpen()
#else
  stream = fdopen(fd, lpszMode);
#endif
  if (stream == 0)
    {
      FATAL_CRT_ERROR (T_("fdopen"), 0);
    }
  return (stream);
}

/* _________________________________________________________________________

   GetLastChar
   _________________________________________________________________________ */

inline
MIKTEXCHAR
GetLastChar (/*[in]*/ const MIKTEXCHAR *	lpsz)
{
  MIKTEX_ASSERT (lpsz != 0);
  size_t len = StrLen(lpsz);
  return (len < 2 ? 0 : lpsz[len - 1]);
}

/* _________________________________________________________________________

   ClearString
   _________________________________________________________________________ */

inline
void
ClearString (/*[in,out]*/ MIKTEXCHAR *	lpsz)
{
  lpsz[0] = 0;
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

template<typename CharType>
inline
CharBuffer<CharType>
Quoted (/*[in]*/ const CharType * lpsz)
{
  bool needQuotes = (StrChr(lpsz, ' ') != 0);
  CharBuffer<CharType> result;
  if (needQuotes)
    {
      result += '"';
    }
  result += lpsz;
  if (needQuotes)
    {
      result += '"';
    }
  return (result);
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

template<typename CharType>
inline
CharBuffer<CharType>
Quoted (/*[in]*/ const std::basic_string<CharType> & str)
{
  return (Quoted(str.c_str()));
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
CharBuffer<char>
Quoted (/*[in]*/ const PathName & path)
{
  return (Quoted(path.Get()));
}

/* _________________________________________________________________________

   STRDUP
   _________________________________________________________________________ */

class STRDUP : public CharBuffer<MIKTEXCHAR>
{
public:
  STRDUP (/*[in]*/ const MIKTEXCHAR * lpsz)
    : CharBuffer (lpsz)
  {
  }

public:
  STRDUP (/*[in]*/ const MIKTEXCHAR *	lpsz,
	  /*[in]*/ size_t		n)
    : CharBuffer (n + 1)
  {
    CopyString2 (GetBuffer(), GetCapacity(), lpsz, n);
  }
};

/* _________________________________________________________________________

   RootDirectory

   Representation of a TEXMF root.
   _________________________________________________________________________ */

struct RootDirectory
{
public:
  RootDirectory ()
    : noFndb (false),
      pFndb (0)
  {
  }

public:
  RootDirectory (/*[in]*/ const PathName &	path)
    : noFndb (false),
      pFndb (0),
      path (path)
  {
  }

public:
  RootDirectory (/*[in]*/ const RootDirectory & other)
    : noFndb (other.noFndb),
      pFndb (0),
      path (other.path)
  {
    SetFndb (other.GetFndb());
  }

public:
  ~RootDirectory ();

public:
  RootDirectory &
  operator= (/*[in]*/ const RootDirectory & rhs)
  {
    noFndb = rhs.noFndb;
    path = rhs.path;
    SetFndb (rhs.GetFndb());
    return (*this);
  }

public:
  const PathName &
  get_Path ()
    const
  {
    return (path);
  }

public:
  void
  set_NoFndb (/*[in]*/ bool noFndb)
  {
    this->noFndb = noFndb;
  }

public:
  bool
  get_NoFndb ()
    const
  {
    return (noFndb);
  }

public:
  void
  SetFndb (/*[in]*/ class FileNameDatabase * pFndb);

public:
  class FileNameDatabase *
  GetFndb ()
    const
  {
    return (pFndb);
  }

  // fully qualified path to root folder
private:
  PathName path;

  // associated file name database object
private:
  class FileNameDatabase * pFndb;

  // true, if an FNDB doesn't exist
private:
  bool noFndb;
};

/* _________________________________________________________________________

   FormatInfo_
   _________________________________________________________________________ */

struct FormatInfo_ : public FormatInfo
{
public:
  FormatInfo_ ()
  {
  }

public:
  FormatInfo_ (/*[in]*/ const FormatInfo & other)
    : FormatInfo (other)
  {
  }

public:
  PathName cfgFile;
};

/* _________________________________________________________________________

   InternalFileTypeInfo
   _________________________________________________________________________ */

struct InternalFileTypeInfo : public FileTypeInfo
{
public:
  PathNameArray searchVec;
};

/* _________________________________________________________________________

   DvipsPaperSizeInfo
   _________________________________________________________________________ */

class DvipsPaperSizeInfo : public PaperSizeInfo
{
public:
  vector<tstring> definition;
};

/* _________________________________________________________________________

   SessionImpl
   _________________________________________________________________________ */

class
SessionImpl : public Session
{

  // -----------------------------------------------------------------------
  // *** interface ***

public:
  SessionImpl ();

public:
  virtual
  ~SessionImpl ();

private:
  void
  Initialize (/*[in]*/ const InitInfo & initInfo);

private:
  void
  Uninitialize ();

public:
  virtual
  void
  MIKTEXCALL
  PushAppName (/*[in]*/ const MIKTEXCHAR *	lpszAppName);

public:
  virtual
  void
  MIKTEXCALL
  PushBackAppName (/*[in]*/ const MIKTEXCHAR *	lpszAppName);

public:
  virtual
  void
  MIKTEXCALL
  AddWorkingDirectory (/*[in]*/ const MIKTEXCHAR *	lpszPath,
		       /*[in]*/ bool			bAtEnd);

public:
  virtual
  PathName
  MIKTEXCALL
  GetSpecialPath (/*[in]*/ SpecialPath		specialPath);

public:
  virtual
  unsigned
  MIKTEXCALL
  GetNumberOfTEXMFRoots ();

public:
  virtual
  PathName
  MIKTEXCALL
  GetRootDirectory (/*[in]*/ unsigned	r);

public:
  virtual
  PathName
  MIKTEXCALL
  GetMpmDatabasePathName ();

public:
  unsigned
  MIKTEXCALL
  DeriveTEXMFRoot (/*[in]*/ const PathName & path);

public:
  virtual
  bool
  MIKTEXCALL
  FindFilenameDatabase (/*[in]*/ unsigned	r,
			/*[out]*/ PathName &	path);

public:
  virtual
  PathName
  MIKTEXCALL
  GetFilenameDatabasePathName (/*[in]*/ unsigned	r);

public:
  virtual
  bool
  MIKTEXCALL
  UnloadFilenameDatabase ();

public:
  virtual
  unsigned
  MIKTEXCALL
  SplitTEXMFPath (/*[in]*/ const PathName &	path,
		  /*[out]*/ PathName &		root,
		  /*[out]*/ PathName &		relative);

public:
  virtual
  void
  MIKTEXCALL
  RegisterRootDirectories (/*[in]*/ const tstring &	roots);

public:
  virtual
  void
  MIKTEXCALL
  RegisterRootDirectories (/*[in]*/ const StartupConfig &	startupConfig,
			   /*[in]*/ bool			sessionOnly);

public:
  virtual
  bool
  MIKTEXCALL
  IsMiKTeXDirect ();

public:
  virtual
  bool
  MIKTEXCALL
  GetMETAFONTMode (/*[in]*/ unsigned		idx,
		   /*[out]*/ MIKTEXMFMODE *	pMode);

public:
  virtual
  bool
  MIKTEXCALL
  DetermineMETAFONTMode (/*[in]*/ unsigned		dpi,
			 /*[out]*/ MIKTEXMFMODE *	pMode);

public:
  virtual
  bool
  MIKTEXCALL
  TryGetConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		     /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		     /*[out]*/ tstring &		value);

public:
  virtual
  tstring
  MIKTEXCALL
  GetConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		  /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		  /*[in]*/ const MIKTEXCHAR *	lpszDefaultValue);

public:
  virtual
  int
  MIKTEXCALL
  GetConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		  /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		  /*[in]*/ int			defaultValue);

public:
  virtual
  bool
  MIKTEXCALL
  GetConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		  /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		  /*[in]*/ bool			defaultValue);

public:
  virtual
  TriState
  MIKTEXCALL
  GetConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		  /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		  /*[in]*/ TriState		defaultValue);

public:
  virtual
  void
  MIKTEXCALL
  SetUserConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		  /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		  /*[in]*/ const MIKTEXCHAR *	lpszValue);

public:
  virtual
  void
  MIKTEXCALL
  SetUserConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		  /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		  /*[in]*/ bool			value);

public:
  virtual
  void
  MIKTEXCALL
  SetUserConfigValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		      /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		      /*[in]*/ int			value);

public:
  virtual
  void
  MIKTEXCALL
  SharedMiKTeXSetup (/*[in]*/ bool shared);

public:
  virtual
  void
  MIKTEXCALL
  SharedMiKTeXSetup (/*[in]*/ bool shared,
		     /*[in]*/ bool sessionOnly);
  
public:
  virtual
  FILE *
  MIKTEXCALL
  OpenFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	    /*[in]*/ FileMode		mode,
	    /*[in]*/ FileAccess		access,
	    /*[in]*/ bool		isTextFile);

public:
  virtual
  FILE *
  MIKTEXCALL
  TryOpenFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	       /*[in]*/ FileMode		mode,
	       /*[in]*/ FileAccess		access,
	       /*[in]*/ bool			isTextFile);

public:
  virtual
  FILE *
  MIKTEXCALL
  OpenFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	    /*[in]*/ FileMode		mode,
	    /*[in]*/ FileAccess		access,
	    /*[in]*/ bool		isTextFile,
	    /*[in]*/ FileShare		share);

public:
  virtual
  FILE *
  MIKTEXCALL
  TryOpenFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	       /*[in]*/ FileMode		mode,
	       /*[in]*/ FileAccess		access,
	       /*[in]*/ bool			isTextFile,
	       /*[in]*/ FileShare		share);

public:
  virtual
  void
  MIKTEXCALL
  CloseFile (/*[in]*/ FILE *	pFile);

public:
  virtual
  bool
  MIKTEXCALL
  IsOutputFile (/*[in]*/ const FILE *	pFile);

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  bool
  MIKTEXCALL
  IsFileAlreadyOpen (/*[in]*/ const MIKTEXCHAR *	lpszFileName);
#endif

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  void
  MIKTEXCALL
  ScheduleFileRemoval (/*[in]*/ const MIKTEXCHAR *	lpszFileName);
#endif

public:
  virtual
  bool
  MIKTEXCALL
  StartFileInfoRecorder (/*[in]*/ bool bRecordPackageNames);

public:
  virtual
  vector<FileInfoRecord>
  MIKTEXCALL
  GetFileInfoRecords ();

public:
  virtual
  FileType
  MIKTEXCALL
  DeriveFileType (/*[in]*/ const MIKTEXCHAR * lpszPath);

public:
  virtual
  bool
  MIKTEXCALL
  FindFile (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
	    /*[in]*/ const MIKTEXCHAR *	lpszPathList,
	    /*[out]*/ PathName &	path);

public:
  virtual
  bool
  MIKTEXCALL
  FindFile (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
	    /*[in]*/ FileType		fileType,
	    /*[out]*/ PathName &	path);

public:
  virtual
  bool
  MIKTEXCALL
  FindPkFile (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
	      /*[in]*/ const MIKTEXCHAR *	lpszMode,
	      /*[in]*/ int			dpi,
	      /*[out]*/ PathName &		path);

public:
  virtual
  bool
  MIKTEXCALL
  FindTfmFile (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
	       /*[out]*/ PathName &		path,
	       /*[in]*/ bool			create);

public:
  virtual
  void
  MIKTEXCALL
  SetFindFileCallback (/*[in]*/ IFindFileCallback *	pCallback);

public:
  virtual
  void
  MIKTEXCALL
  SplitFontPath (/*[in]*/ const MIKTEXCHAR *	lpszFontPath,
		 /*[out]*/ MIKTEXCHAR *		lpszFontType,
		 /*[out]*/ MIKTEXCHAR *		lpszSupplier,
		 /*[out]*/ MIKTEXCHAR *		lpszTypeface,
		 /*[out]*/ MIKTEXCHAR *		lpszFontName,
		 /*[out]*/ MIKTEXCHAR *		lpszPointSize);

public:
  virtual
  bool
  MIKTEXCALL
  GetFontInfo (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
	       /*[out]*/ MIKTEXCHAR *		lpszSupplier,
	       /*[out]*/ MIKTEXCHAR *		lpszTypeface,
	       /*[out]*/ double *		lpGenSize);

public:
  virtual
  void
  MIKTEXCALL
  GetGhostscript (/*[out]*/ MIKTEXCHAR *	lpszPath,
		  /*[out]*/ unsigned long *	pVersionNumber);

public:
  virtual
  tstring
  MIKTEXCALL
  GetExpandedSearchPath (/*[in]*/ FileType	fileType);

public:
  virtual
  bool
  MIKTEXCALL
  FindGraphicsRule (/*[in]*/ const MIKTEXCHAR *	lpszFrom,
		    /*[in]*/ const MIKTEXCHAR *	lpszTo,
		    /*[out]*/ MIKTEXCHAR *	lpszRule,
		    /*[in]*/ size_t	bufSize);

public:
  virtual
  bool
  MIKTEXCALL
  ConvertToBitmapFile (/*[in]*/ const MIKTEXCHAR *	lpszSourceFileName,
		       /*[out]*/ MIKTEXCHAR *		lpszDestFileName,
		       /*[in]*/ IRunProcessCallback *	pCallback);

public:
  virtual
  bool
  MIKTEXCALL
  EnableFontMaker (/*[in]*/ bool enable);

public:
  virtual
  bool
  MIKTEXCALL
  GetMakeFontsFlag ();

public:
  virtual
  void
  MIKTEXCALL
  MakeMakePkCommandLine (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
			 /*[in]*/ int			dpi,
			 /*[in]*/ int			baseDpi,
			 /*[in]*/ const MIKTEXCHAR *	lpszMfMode,
			 /*[out]*/ PathName &		fileName,
			 /*[out]*/ MIKTEXCHAR *		lpszArguments,
			 /*[in]*/ size_t		bufSize);

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  int
  MIKTEXCALL
  RunBatch (/*[in]*/ int			argc,
	    /*[in]*/ const MIKTEXCHAR **	argv);
#endif

public:
  virtual
  int
  MIKTEXCALL
  RunPerl (/*[in]*/ int			argc,
	   /*[in]*/ const MIKTEXCHAR **	argv);

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  bool
  MIKTEXCALL
  ShowManualPageAndWait (/*[in]*/ HWND		hWnd,
			 /*[in]*/ unsigned long	topic);
#endif

public:
  virtual
  bool
  MIKTEXCALL
  GetNextFileTypeInfo (/*[in]*/ unsigned	index,
		       /*[out]*/ FileTypeInfo &	fileTypeInfo);

public:
  virtual
  bool
  MIKTEXCALL
  GetFormatInfo (/*[in]*/ unsigned	index,
		 /*[out]*/ FormatInfo &	formatInfo);

public:
  virtual
  FormatInfo
  MIKTEXCALL
  GetFormatInfo (/*[in]*/ const MIKTEXCHAR *	lpszKey);

public:
  virtual
  bool
  MIKTEXCALL
  TryGetFormatInfo (/*[in]*/ const MIKTEXCHAR *	lpszKey,
		    /*[out]*/ FormatInfo &	formatInfo);

public:
  virtual
  void
  MIKTEXCALL
  DeleteFormatInfo (/*[in]*/ const MIKTEXCHAR *	lpszKey);

public:
  virtual
  void
  MIKTEXCALL
  SetFormatInfo (/*[in]*/ const FormatInfo &	formatInfo);

public:
  virtual
  PathName
  MIKTEXCALL
  GetMyLocation ();

public:
  virtual
  bool
  MIKTEXCALL
  RunningAsAdministrator ();

public:
  virtual
  TriState
  MIKTEXCALL
  IsSharedMiKTeXSetup ();

public:
  virtual
  bool
  MIKTEXCALL
  GetPaperSizeInfo (/*[in]*/ int		idx,
		    /*[out]*/ PaperSizeInfo &	paperSize);

public:
  virtual
  PaperSizeInfo
  MIKTEXCALL
  GetPaperSizeInfo (/*[in]*/ const MIKTEXCHAR *	lpszName);

public:
  virtual
  PolicyFlags
  MIKTEXCALL
  GetPolicyFlags ();

public:
  virtual
  void
  MIKTEXCALL
  SetDefaultPaperSize (/*[in]*/ const MIKTEXCHAR * lpszDvipsName);

public:
  virtual
  bool
  MIKTEXCALL
  TryCreateFromTemplate (/*[in]*/ const PathName & path);

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  bool
  MIKTEXCALL
  RunningAsPowerUser ();
#endif

public:
  virtual
  bool
  MIKTEXCALL
  IsUserAnAdministrator ();

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  bool
  MIKTEXCALL
  IsUserAPowerUser ();
#endif

public:
  virtual
  void
  MIKTEXCALL
  ConfigureFile (/*[in]*/ const PathName & pathIn,
		 /*[in]*/ const PathName & pathOut);

public:
  virtual
  void
  MIKTEXCALL
  SetTheNameOfTheGame (/*[in]*/ const MIKTEXCHAR * lpszTheNameOfTheGame);

public:
  virtual
  void
  MIKTEXCALL
  ConfigureFile (/*[in]*/ const PathName & path);

  // -----------------------------------------------------------------------
  // *** public ***

public:
  unsigned
  TryDeriveTEXMFRoot (/*[in]*/ const PathName & path);

public:
  bool
  IsTEXMFFile (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	       /*[out]*/ MIKTEXCHAR *		lpszRelPath,
	       /*[out]*/ unsigned *		pRootIndex);

public:
  void
  RecordFileInfo (/*[in]*/ const MIKTEXCHAR *	lpszPath,
		  /*[in]*/ FileAccess		access);

public:
  bool
  UnloadFilenameDatabaseInternal (/*[in]*/ unsigned	r,
				  /*[in]*/ bool		bRemove);

public:
  class FileNameDatabase *
  GetFileNameDatabase (/*[in]*/ unsigned	r,
		       /*[in]*/ TriState	triReadOnly);

public:
  class FileNameDatabase *
  GetFileNameDatabase (/*[in]*/ const MIKTEXCHAR *	lpszPath);

public:
  PathName
  GetTempDirectory ();

public:
  void
  RemoveWorkingDirectory (/*[in]*/ const MIKTEXCHAR * lpszPath);
  
public:
  PathName
  GetMyProgramFile ();

public:
  bool
  GetPsFontDirs (/*[out]*/ tstring &	psFontDirs);

public:
  bool
  GetOTFDirs (/*[out]*/ tstring &	otfDirs);

public:
  bool
  GetTTFDirs (/*[out]*/ tstring &	ttfDirs);

private:
  void
  ReadDvipsPaperSizes ();

private:
  void
  WriteDvipsPaperSizes ();

private:
  void
  WriteDvipdfmPaperSize ();

private:
  void
  WriteDvipdfmxPaperSize ();

private:
  void
  WritePdfTeXPaperSize ();

private:
  void
  AddDvipsPaperSize (/*[in]*/ const DvipsPaperSizeInfo & dvipsPaperSizeInfo);

#if defined(MIKTEX_WINDOWS)
private:
  bool
  GetAcrobatFontDir (/*[out]*/ MIKTEXCHAR *	lpszPath);
#endif

#if defined(MIKTEX_WINDOWS)
private:
  bool
  GetATMFontDir (/*[out]*/ MIKTEXCHAR *	lpszPath);
#endif

#if defined(MIKTEX_WINDOWS)
private:
  bool
  IsUserMemberOfGroup (/*[in]*/ DWORD localGroup);
#endif

#if defined(MIKTEX_WINDOWS)
private:
  bool
  RunningElevated ();
#endif

public:
  auto_ptr<TraceStream> trace_access;
  auto_ptr<TraceStream> trace_config;
  auto_ptr<TraceStream> trace_core;
  auto_ptr<TraceStream> trace_env;
  auto_ptr<TraceStream> trace_error;
  auto_ptr<TraceStream> trace_files;
  auto_ptr<TraceStream> trace_filesearch;
  auto_ptr<TraceStream> trace_fndb;
  auto_ptr<TraceStream> trace_fonts;
  auto_ptr<TraceStream> trace_packages;
  auto_ptr<TraceStream> trace_process;
  auto_ptr<TraceStream> trace_tempfile;
  auto_ptr<TraceStream> trace_time;
  auto_ptr<TraceStream> trace_values;

public:
  locale defaultLocale;

public:
  struct ScratchDirectoryInfo
  {
    // previous directory
    tstring previous;
    
    // fully qualified & resolved path to scratch directory
    tstring scratchDirectory;
  };

public:
  std::stack<ScratchDirectoryInfo> scratchDirectoryStack;

#if defined(MIKTEX_WINDOWS) && ! defined(MIKTEX_STATIC)
public:
  static HINSTANCE hinstDLL;
#endif

#if defined(MIKTEX_WINDOWS) && ! defined(MIKTEX_STATIC)
public:
  static TriState dynamicLoad;
#endif

public:
  static SessionImpl * theSession;

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
public:
  static bool runningAsLocalServer;
#endif

public:
  static
  SessionImpl *
  TryGetSession ()
  {
    return (theSession);
  }

public:
  static
  SessionImpl *
  GetSession ()
  {
    if (theSession == 0)
      {
	FATAL_MIKTEX_ERROR
	  (T_("SessionImpl::GetSession"),
	   T_("The MiKTeX Core library has not been initialized."),
	   0);
      }
    return (theSession);
  }

public:
  static
  locale &
  GetDefaultLocale ()
  {
    return (GetSession()->defaultLocale);
  }

public:
  PathName
  GetRelativeFilenameDatabasePathName (/*[in]*/  unsigned r);

  // -----------------------------------------------------------------------
  // *** private ***

private:
  class FlagsEnum {
  public:
    enum EnumType {
      CachedPsFontDirs,
      CachedTtfDirs,
      CachedOtfDirs,
#if defined(MIKTEX_WINDOWS)
      CachedAcrobatFontDir,
#endif
#if defined(MIKTEX_WINDOWS)
      CachedAtmFontDir,
#endif
    };
  };

private:
  typedef EnumWrapper<FlagsEnum> Flags;

private:

private:
  std::bitset<32> flags;

private:
  tstring psFontDirs;

private:
  tstring ttfDirs;

private:
  tstring otfDirs;

#if defined(MIKTEX_WINDOWS)
private:
  tstring acrobatFontDir;
#endif

#if defined(MIKTEX_WINDOWS)
private:
  tstring atmFontDir;
#endif

private:
  void
  SetCWDEnv ();

private:
  bool
  FindPerl (/*[out]*/ PathName & perl);

private:
  bool
  FindPerlScript (/*[in]*/ const MIKTEXCHAR *	lpszName,
		  /*[out]*/ PathName &		path);

#if defined(MIKTEX_WINDOWS)
private:
  bool
  FindBatchFile (/*[in]*/ const MIKTEXCHAR *	lpszName,
		 /*[in]*/ PathName &		path);
#endif

private:
  bool
  GetWorkingDirectory (/*[in]*/ unsigned	n,
		       /*[out]*/ PathName &	path);

private:
  PathNameArray
  ConstructSearchVector (/*[in]*/ FileType	fileType);

private:
  void
  TraceSearchVector (/*[in]*/ const MIKTEXCHAR *	lpszKey,
		     /*[in]*/ const PathNameArray &	pathvec);

private:
  void
  RegisterLibraryTraceStreams ();

private:
  void
  UnregisterLibraryTraceStreams ();

  
private:
  bool
  FindInTypefaceMap (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
		     /*[out]*/ MIKTEXCHAR *		lpszTypeface);

private:
  bool
  FindInSupplierMap (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
		     /*[out]*/ MIKTEXCHAR *		lpszSupplier,
		     /*[out]*/ MIKTEXCHAR *		lpszTypeface);

private:
  bool
  FindInSpecialMap (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
		    /*[out]*/ MIKTEXCHAR *	lpszSupplier,
		    /*[out]*/ MIKTEXCHAR *	lpszTypeface);

private:
  bool
  InternalGetFontInfo (/*[in]*/ const MIKTEXCHAR *	lpszFontName,
		       /*[out]*/ MIKTEXCHAR *		lpszSupplier,
		       /*[out]*/ MIKTEXCHAR *		lpszTypeface);


private:
  bool
  MakePkFileName (/*[out]*/ PathName &		pkFileName,
		  /*[in]*/ const MIKTEXCHAR *	lpszFontName,
		  /*[in]*/ int			dpi);


private:
  bool
  FindFileAlongVec (/*[in]*/ const MIKTEXCHAR *		lpszFileName,
		    /*[in]*/ const PathNameArray &	vec,
		    /*[out]*/ PathName &		path);

private:
  bool
  SlowFindFile (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
		/*[in]*/ const MIKTEXCHAR *	lpszPathList,
		/*[in]*/ PathName &		path);

private:
  bool
  SearchFileSystem (/*[in]*/ const MIKTEXCHAR *	lpszRelPath,
		    /*[in]*/ const MIKTEXCHAR *	lpszDirPath,
		    /*[out]*/ PathName &	result);


private:
bool
SearchFileSystem (/*[in]*/ const MIKTEXCHAR *	lpszCurDir,
		  /*[in]*/ const MIKTEXCHAR *	lpszSubDir,
		  /*[in]*/ const MIKTEXCHAR *	lpszSearchSpec,
		  /*[out]*/ PathName &		result);


private:
  bool
  CheckCandidate (/*[in,out]*/ PathName &	path,
		  /*[in]*/ const MIKTEXCHAR *	lpszFileInfo);

private:
  bool
  GetSessionValue (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
		   /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		   /*[out]*/ tstring &		value,
		   /*[in]*/const MIKTEXCHAR *	lpszDefaultValue);

private:
  void
  ReadAllConfigFiles (/*[in]*/ const MIKTEXCHAR *	lpszBaseName,
		      /*[in,out]*/ Cfg *		pCfg);

private:
  deque<PathName> workingDirectories;

public:
  void
  SetEnvironmentVariables ();

private:
  unsigned
  GetMpmRoot ();

public:
  unsigned
  GetDataRoot ();

public:
  unsigned
  GetCommonDataRoot ();

public:
  unsigned
  GetUserDataRoot ();

public:
  unsigned
  GetConfigRoot ();

public:
  unsigned
  GetCommonConfigRoot ();

public:
  unsigned
  GetUserConfigRoot ();

public:
  unsigned
  GetInstallRoot ();

private:
  bool
  IsManagedRoot (/*[in]*/ unsigned root);

private:
  unsigned
  RegisterRootDirectory (/*[in]*/ const PathName & root);

private:
  bool
  FindStartupConfigFile (/*[out]*/ PathName & path);

private:
  StartupConfig
  ReadStartupConfigFile ();

private:
  void
  WriteStartupConfigFile (/*[in]*/ const StartupConfig & startupConfig);

private:
  StartupConfig
  ReadEnvironment ();

#if defined(MIKTEX_WINDOWS)
private:
  StartupConfig
  ReadRegistry ();
#endif

#if defined(MIKTEX_WINDOWS)
private:
  void
  WriteRegistry (/*[in]*/ const StartupConfig & startupConfig);
#endif

private:
  StartupConfig
  DefaultConfig (/*[in]*/ bool sharedSetup);

private:
  void
  InitializeRootDirectories (/*[in]*/ const StartupConfig & startupConfig);

private:
  void
  InitializeRootDirectories ();

private:
  void
  SaveRootDirectories ();

private:
  bool
  IsTeXMFReadOnly (/*[in]*/ unsigned r);

private:
  class FileNameDatabase *
  GetFileNameDatabase (/*[in]*/ unsigned r);

private:
  PathName
  GetBinDirectory ();

private:
  void
  AppendToSearchPath (/*[in,out]*/ tstring &	strSearchPath,
		      /*[in]*/ const tstring &	strSearchPath2);

private:
  void
  SplitSearchPath (/*[in,out]*/ PathNameArray &	pathvec,
		   /*[in]*/ const MIKTEXCHAR *	lpszSearchPath);

private:
  PathNameArray
  SplitSearchPath (/*[in]*/ const MIKTEXCHAR *	lpszSearchPath);

private:
  void
  PushBackPath (/*[in,out]*/ PathNameArray &	pathvec,
		/*[in]*/ const PathName &	path);

private:
  void
  ReadFormatsIni ();

private:
  void
  ReadFormatsIni (/*[in]*/ const PathName & cfgFile);

private:
  void
  WriteFormatsIni ();

private:
  void
  ReadMetafontModes ();

private:
  bool
  FindMETAFONTMode (/*[in]*/ const MIKTEXCHAR *	lpszMnemonic,
		    /*[out]*/ MIKTEXMFMODE *	pMode);

private:
  FILE *
  InitiateProcessPipe (/*[in]*/ const MIKTEXCHAR *	lpszCommand,
		       /*[in]*/ FileAccess		access,
		       /*[in,out]*/ FileMode &		mode);

private:
  FILE *
  OpenGZipFile (/*[in]*/ const PathName & path);

public:
  FILE *
  OpenBZip2File (/*[in]*/ const PathName & path);

private:
  void
  CheckOpenFiles ();

private:
  void
  WritePackageHistory ();

private:
  void
  RegisterFileType (/*[in]*/ FileType		fileType,
		    /*[in]*/ const MIKTEXCHAR *	lpszFileType,
		    /*[in]*/ const MIKTEXCHAR *	lpszApplication,
		    /*[in]*/ const MIKTEXCHAR *	lpszFileNameExtensions,
		    /*[in]*/ const MIKTEXCHAR *	lpszDefaultSearchPath,
		    /*[in]*/ const MIKTEXCHAR *	lpszEnvVarNames);
    
private:
  void
  RegisterFileTypes ();

private:
  InternalFileTypeInfo *
  GetFileTypeInfo (/*[in]*/ FileType		fileType);
  
private:
  void
  ClearSearchVectors ();

private:
  PathName pathGsExe;

private:
  VersionNumber gsVersion;

private:
  IFindFileCallback * pInstallPackageCallback;

private:
  vector<InternalFileTypeInfo> fileTypes;

private:
  vector<MIKTEXMFMODE> metafontModes;

  // info about open file
private:
  struct OpenFileInfo
  {
    OpenFileInfo ()
      : pFile (0),
	mode (FileMode::Open),
	access (FileAccess::None)
    {
    }
    const FILE *	pFile;
    tstring		fileName;
    FileMode		mode;
    FileAccess		access;
  };

  // caching open files
private:
  map<const FILE *, OpenFileInfo> openFilesMap;

  // file access history
private:
  vector<FileInfoRecord> fileInfoRecords;

  // true, if we record a file history
private:
  bool recordingFileNames;

  // true, if we record a package history
private:
  bool recordingPackageNames;

  // package history file
private:
  tstring packageHistoryFile;

private:
  bool makeFonts;

private:
#if defined(USE_HASH_MAP)
  typedef
  hash_map<tstring, SmartPointer<Cfg>, hash_compare_icase>
  ConfigurationSettings;
#else
  typedef
  map<tstring, SmartPointer<Cfg>, hash_compare_icase> ConfigurationSettings;
#endif
 
private:
  ConfigurationSettings configurationSettings;

private:
  vector<FormatInfo_> formats;

private:
  InitInfo initInfo;

private:
  TriState sharedSetup;

private:
  TriState runningAsAdministrator;

private:
  TriState isUserAnAdministrator;

#if defined(MIKTEX_WINDOWS)
private:
  TriState runningAsPowerUser;
#endif

#if defined(MIKTEX_WINDOWS)
private:
  TriState isUserAPowerUser;
#endif

private:
  tstring applicationNames;

private:
  tstring theNameOfTheGame;

private:
  const tstring &
  get_ApplicationNames ()
    const
  {
    return (applicationNames);
  }

  // registered root directories
private:
  vector<RootDirectory> rootDirectories;

private:
  vector<RootDirectory> &
  get_RootDirectories ()
  {
    return (rootDirectories);
  }

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
private:
  bool
  UseLocalServer ();
#endif

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
private:
  void
  ConnectToServer ();
#endif

  // index of common data root
private:
  unsigned commonDataRootIndex;

  // index of user data root
private:
  unsigned userDataRootIndex;

  // index of install root
private:
  unsigned installRootIndex;

  // index of common config root
private:
  unsigned commonConfigRootIndex;

  // index of user config root
private:
  unsigned userConfigRootIndex;

private:
  bool haveStartupConfigFile;

  // fully qualified path to paths.ini; valid only if haveStartupConfigFile
private:
  PathName startupConfigFile;

private:
  vector<DvipsPaperSizeInfo> dvipsPaperSizes;

  // the MiKTeXDirect flag:
  // TriState::True if running from a MiKTeXDirect medium
  // TriState::False if not running from a MiKTeXDirect medium
  // TriState::Undetermined otherwise
private:
  TriState triMiKTeXDirect;

  // fully qualified path to the running application file
private:
  PathName myProgramFile;

private:
  int refCount;

private:
  bool initialized;

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
private:
  struct
  {
    CComQIPtr<MiKTeXSessionLib::ISession> pSession;
  } localServer;
#endif

private:
  friend class Session;
};

/* _________________________________________________________________________

   CTYPE_FACET
   _________________________________________________________________________ */

#define CTYPE_FACET \
  use_facet<ctype<MIKTEXCHAR> >(SessionImpl::GetDefaultLocale())

/* _________________________________________________________________________

   ToLower
   _________________________________________________________________________ */

template<typename CharType>
inline
CharType
ToLower (/*[in]*/ CharType ch)
{
  if (static_cast<unsigned>(ch) < 128)
    {
      if (ch >= 'A' && ch <= 'Z')
	{
	  ch = ch - 'A' + 'a';
	}
    }
  else
    {
      ch = CTYPE_FACET.tolower(ch);
    }
  return (ch);
}

/* _________________________________________________________________________

   ToUpper
   _________________________________________________________________________ */

template<typename CharType>
inline
CharType
ToUpper (/*[in]*/ CharType ch)
{
  if (static_cast<unsigned>(ch) < 128)
    {
      if (ch >= 'a' && ch <= 'z')
	{
	  ch = ch - 'a' + 'A';
	}
    }
  else
    {
      ch = CTYPE_FACET.toupper(ch);
    }
  return (ch);
}

/* _________________________________________________________________________

   IsDriveLetter
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
inline
bool
IsDriveLetter (/*[in]*/ MIKTEXCHAR ch)
{
  return (ch >= T_('A') && ch <= T_('Z')
	  || ch >= T_('a') && ch <= T_('z'));
}
#endif

/* _________________________________________________________________________

   IsAlpha
   _________________________________________________________________________ */

inline
bool
IsAlpha (/*[in]*/ MIKTEXCHAR ch)
{
  return (CTYPE_FACET.is(ctype<MIKTEXCHAR>::alpha, ch));
}

/* _________________________________________________________________________

   IsDigit
   _________________________________________________________________________ */

inline
bool
IsDigit (/*[in]*/ MIKTEXCHAR ch)
{
  return (CTYPE_FACET.is(ctype<MIKTEXCHAR>::digit, ch));
}

/* _________________________________________________________________________

   IsAlNum
   _________________________________________________________________________ */

inline
bool
IsAlNum (/*[in]*/ MIKTEXCHAR ch)
{
  return (CTYPE_FACET.is(ctype<MIKTEXCHAR>::alnum, ch));
}

/* _________________________________________________________________________

   IsSpace
   _________________________________________________________________________ */

inline
bool
IsSpace (/*[in]*/ MIKTEXCHAR ch)
{
  return (CTYPE_FACET.is(ctype<MIKTEXCHAR>::space, ch));
}

/* _________________________________________________________________________

   SkipSpace
   _________________________________________________________________________ */

inline
void
SkipSpace (/*[in,out]*/ const MIKTEXCHAR * &	lpsz)
{
  while (*lpsz != 0 && IsSpace(*lpsz))
    {
      ++ lpsz;
    }
}

/* _________________________________________________________________________

   SkipSpace
   _________________________________________________________________________ */

inline
void
SkipSpace (/*[in,out]*/ MIKTEXCHAR * &	lpsz)
{
  while (*lpsz != 0 && IsSpace(*lpsz))
    {
      ++ lpsz;
    }
}

/* _________________________________________________________________________

   SkipNonSpace
   _________________________________________________________________________ */

inline
void
SkipNonSpace (/*[in,out]*/ const MIKTEXCHAR * &	lpsz)
{
  while (*lpsz != 0 && ! IsSpace(*lpsz))
    {
      ++ lpsz;
    }
}

/* _________________________________________________________________________

   SkipNonSpace
   _________________________________________________________________________ */

inline
void
SkipNonSpace (/*[in,out]*/ MIKTEXCHAR * &	lpsz)
{
  while (*lpsz != 0 && ! IsSpace(*lpsz))
    {
      ++ lpsz;
    }
}

/* _________________________________________________________________________

   SkipAlpha
   _________________________________________________________________________ */

inline
void
SkipAlpha (/*[in,out]*/ const MIKTEXCHAR * &	lpsz)
{
  while (*lpsz != 0 && IsAlpha(*lpsz))
    {
      ++ lpsz;
    }
}

/* _________________________________________________________________________

   SkipNonDigit
   _________________________________________________________________________ */

inline
void
SkipNonDigit (/*[in]*/ const MIKTEXCHAR * &	lpsz)
{
  while (*lpsz != 0 && ! IsDigit(*lpsz))
    {
      ++ lpsz;
    }
}

/* _________________________________________________________________________

   STSCANF
   _________________________________________________________________________ */

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#  define STSCANF _stscanf_s
#elif defined (MIKTEX_UNICODE)
#  error Unimplemented: STSCANF
#else
#  define STSCANF sscanf
#endif

/* _________________________________________________________________________

   MakeLower
   _________________________________________________________________________ */

template<typename CharType>
inline
MIKTEXCHAR *
MakeLower (/*[out]*/ CharType *		lpszBuf,
	   /*[in]*/ size_t		bufSize,
	   /*[in]*/ const CharType *	lpszSource)
{
  MIKTEX_ASSERT_CHAR_BUFFER (lpszBuf, bufSize);
  size_t l = 0;
  for (; *lpszSource != 0; ++ lpszSource, ++ lpszBuf, ++ l)
    {
      if (l == bufSize)
	{
	  INVALID_ARGUMENT (T_("MakeLower"), lpszSource);
	}
      *lpszBuf = ToLower(*lpszSource);
    }
  if (l == bufSize)
    {
      INVALID_ARGUMENT (T_("MakeLower"), lpszSource);
    }
  *lpszBuf = 0;
  return (lpszBuf);
}

/* _________________________________________________________________________

   MakeLower
   _________________________________________________________________________ */

inline
tstring
MakeLower (/*[in]*/ const tstring &	str)
{
  tstring ret;
  for (tstring::const_iterator it = str.begin(); it != str.end(); ++ it)
    {
      ret += ToLower(*it);
    }
  return (ret);
}

/* _________________________________________________________________________ */

extern int debugOnStdException;

END_INTERNAL_NAMESPACE;

using namespace MiKTeX::ABF3880A6239B84E87DC7E727A8BBFD4;

#undef MIKTEXINTERNALFUNC
#define MIKTEXINTERNALFUNC(type) \
  type MiKTeX::ABF3880A6239B84E87DC7E727A8BBFD4::
#define MIKTEXINTERNALFUNC2(type, callconv) \
  type callconv MiKTeX::ABF3880A6239B84E87DC7E727A8BBFD4::

#undef MIKTEXINTERNALVAR
#define MIKTEXINTERNALVAR(type) \
  type MiKTeX::ABF3880A6239B84E87DC7E727A8BBFD4::

#if 0
#  define MIKTEXSTATICFUNC(type) static type
#  define MIKTEXSTATICFUNC2(type, callconv) static type callconv
#else
#  define MIKTEXSTATICFUNC(type) type
#  define MIKTEXSTATICFUNC2(type, callconv) type callconv
#endif

#define MIKTEXPERMANENTVAR(type) static type
