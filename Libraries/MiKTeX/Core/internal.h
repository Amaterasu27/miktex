/* internal.h: internal definitions				-*- C++ -*-

   Copyright (C) 1996-2009 Christian Schenk

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

#if ! defined(MIKTEX_STATIC) && defined(_MSC_VER)
#  define MIKTEXCOREEXPORT __declspec(dllexport)
#elif ! defined(MIKTEX_STATIC) && __GNUC__ >=4
#  define MIKTEXCOREEXPORT __attribute__((visibility("default")))
#else
#  define MIKTEXCOREEXPORT
#endif

#define EAD86981C92C904D808A5E6CEC64B90E
#include "miktex/Core/Core"

#include "core-version.h"

#if defined(MIKTEX_WINDOWS)
#  include "MiKTeX/Core/win/DllProc"
#endif

#include "miktex/Core/Debug"
#include "miktex/Core/Paths"
#include "miktex/Core/Registry"
#include "miktex/Core/Trace"
#include "miktex/Core/Urls"

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
	    Utils::PrintException (e);		\
	  }					\
        exit (1);				\
      }						\
    catch (const exception & e)			\
      {						\
        if (stderr != 0)			\
	  {					\
	    Utils::PrintException (e);		\
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

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) MiKTeX::Core::Quoter<char>(x).Get()

#define MIKTEXINTERNALFUNC(type) type
#define MIKTEXINTERNALVAR(type) type

#define ARRAY_SIZE(buf) (sizeof(buf) / sizeof(buf[0]))

#define MAKE_SEARCH_PATH(dir)			\
  TEXMF_PLACEHOLDER				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  dir

#define CFG_MACRO_NAME_BINDIR "bindir"
#define CFG_MACRO_NAME_LOCALFONTDIRS "localfontdirs"
#define CFG_MACRO_NAME_PSFONTDIRS "psfontdirs"
#define CFG_MACRO_NAME_OTFDIRS "otfdirs"
#define CFG_MACRO_NAME_TTFDIRS "ttfdirs"
#define CFG_MACRO_NAME_WINDIR "windir"

/* _________________________________________________________________________

   Constants
   _________________________________________________________________________ */

const unsigned FNDB_PAGESIZE = 0x1000;

#define TEXMF_PLACEHOLDER "%R"

#define CURRENT_DIRECTORY "."

#define PARENT_DIRECTORY ".."

#define PARENT_PARENT_DIRECTORY			\
  PARENT_DIRECTORY				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  PARENT_DIRECTORY

#define PARENT_PARENT_PARENT_DIRECTORY		\
  PARENT_DIRECTORY				\
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING	\
  PARENT_PARENT_DIRECTORY

#if defined(MIKTEX_WINDOWS)
const char PATH_DELIMITER = ';';
#define PATH_DELIMITER_STRING ";"
#else
const char PATH_DELIMITER = ':';
#define PATH_DELIMITER_STRING ":"
#endif

const char * const RECURSION_INDICATOR = "//";
const size_t RECURSION_INDICATOR_LENGTH = 2;
const char * const SESSIONSVC = "sessionsvc";

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

typedef basic_ostringstream<char> tostringstream;

/* _________________________________________________________________________

   PathNameArray
   _________________________________________________________________________ */

typedef std::vector<PathName> PathNameArray;

/* _________________________________________________________________________

   Functions
   _________________________________________________________________________ */

void
AppendDirectoryDelimiter (/*[in,out]*/ string & path);

void
AppendDirectoryDelimiter (/*[in,out]*/ char *	lpszPath,
			  /*[in]*/ size_t		size);

int
CompareFileNameChars (/*[in]*/ char	ch1,
		      /*[in]*/ char	ch2);
  
void
CopyString2 (/*[out]*/ char *		lpszBuf,
	     /*[in]*/ size_t			bufSize,
	     /*[in]*/ const char *	lpszSource,
	     /*[in]*/ size_t			count);

void
CreateDirectoryPath (/*[in]*/ const char *	lpszPath);

bool
FileIsOnROMedia (/*[in]*/ const char *	lpszPath);
  
bool
GetCrtErrorMessage (/*[in]*/ int		functionResult,
		    /*[out]*/ string &		errorMessage);

PathName
GetFullPath (/*[in]*/ const char * lpszPath);

PathName
GetHomeDirectory ();
  
#if defined(MIKTEX_WINDOWS)
bool
GetUserProfileDirectory (/*[out]*/ PathName & path);
#endif

#if defined(MIKTEX_WINDOWS)
bool
GetWindowsFontsDirectory (/*[out]*/ PathName & path);
#endif  
  
const char *
GetFileNameExtension (/*[in]*/ const char * lpszPath);
  
bool
HaveEnvironmentString (/*[in]*/ const char * lpszName);

const char *
GetEnvironmentString (/*[in]*/ const char * lpszName);
  
bool
IsExplicitlyRelativePath (/*[in]*/ const char * lpszPath);

bool
IsMpmFile (/*[in]*/ const char * lpszPath);
  
void
RemoveDirectoryDelimiter (/*[in,out]*/ char * lpszPath);
  
#if defined(MIKTEX_WINDOWS)
bool
ReportMiKTeXEvent (/*[in]*/ unsigned short	eventType,
		   /*[in]*/ unsigned long	eventId,
		   /*[in]*/			...);
#endif
  
void
TraceError (/*[in]*/ const char *	lpszFormat,
	    /*[in]*/			...);

void
TraceMiKTeXError (/*[in]*/ const char *	lpszMiktexFunction,
		  /*[in]*/ const char *	lpszMessage,
		  /*[in]*/ const char *	lpszInfo,
		  /*[in]*/ const char *	lpszSourceFile,
		  /*[in]*/ int			lpszSourceLine);
  
#if defined(MIKTEX_WINDOWS)
void
TraceWindowsError (/*[in]*/ const char *	lpszWindowsFunction,
		   /*[in]*/ unsigned long	functionResult,
		   /*[in]*/ const char *	lpszInfo,
		   /*[in]*/ const char *	lpszSourceFile,
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
  : public std::binary_function<string, string, bool>
{
  bool
  operator() (/*[in]*/ const string & lhs,
	      /*[in]*/ const string & rhs)
    const
  {
    return (StringCompare(lhs.c_str(), rhs.c_str(), true) < 0);
  }
};

/* _________________________________________________________________________

   GetC
   _________________________________________________________________________ */

inline
int
GetC (/*[in]*/ FILE *	stream)
{
  int ch = getc(stream);
  if (ferror(stream) != 0)
    {
      FATAL_CRT_ERROR ("getc", 0);
    }
  return (ch);
}

/* _________________________________________________________________________

   UnGetC
   _________________________________________________________________________ */

inline
void
UnGetC (/*[in]*/ int	ch,
	/*[in]*/ FILE *		stream)
{
  int ch2 = ungetc(ch, stream);
  if (ch2 == EOF)
    {
      FATAL_CRT_ERROR ("ungetc", 0);
    }
}

/* _________________________________________________________________________

   StrNCmpI
   _________________________________________________________________________ */

inline
int
StrNCmpI (/*[in]*/ const char *	lpsz1,
	  /*[in]*/ const char *	lpsz2,
	  /*[in]*/ size_t		n)
{
#if defined(_MSC_VER)
  return (_strnicmp(lpsz1, lpsz2, n));
#else
  return (strncasecmp(lpsz1, lpsz2, n));
#endif
}

/* _________________________________________________________________________

   FdOpen
   _________________________________________________________________________ */

inline
FILE *
FdOpen (/*[in]*/ int		fd,
	/*[in]*/ const char *	lpszMode)
{
  FILE * stream;
#if defined(_MSC_VER)
  stream = _tfdopen(fd, lpszMode);
#else
  stream = fdopen(fd, lpszMode);
#endif
  if (stream == 0)
    {
      FATAL_CRT_ERROR ("fdopen", 0);
    }
  return (stream);
}

/* _________________________________________________________________________

   GetLastChar
   _________________________________________________________________________ */

inline
char
GetLastChar (/*[in]*/ const char *	lpsz)
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
ClearString (/*[in,out]*/ char *	lpsz)
{
  lpsz[0] = 0;
}

/* _________________________________________________________________________

   STRDUP
   _________________________________________________________________________ */

class STRDUP : public CharBuffer<char>
{
public:
  STRDUP (/*[in]*/ const char * lpsz)
    : CharBuffer<char> (lpsz)
  {
  }

public:
  STRDUP (/*[in]*/ const char *	lpsz,
	  /*[in]*/ size_t		n)
    : CharBuffer<char> (n + 1)
  {
    CopyString2 (CharBuffer<char>::GetBuffer(),
		 CharBuffer<char>::GetCapacity(),
		 lpsz,
		 n);
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
  vector<string> definition;
};

/* _________________________________________________________________________

   SessionImpl
   _________________________________________________________________________ */

class
SessionImpl : public Session
{

  // -----------------------------------------------------------------------
  // *** Session interface ***

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
  MIKTEXTHISCALL
  PushAppName (/*[in]*/ const char *	lpszAppName);

public:
  virtual
  void
  MIKTEXTHISCALL
  PushBackAppName (/*[in]*/ const char *	lpszAppName);

public:
  virtual
  void
  MIKTEXTHISCALL
  AddInputDirectory (/*[in]*/ const char *	lpszPath,
		     /*[in]*/ bool		atEnd);

public:
  virtual
  PathName
  MIKTEXTHISCALL
  GetSpecialPath (/*[in]*/ SpecialPath		specialPath);

public:
  virtual
  unsigned
  MIKTEXTHISCALL
  GetNumberOfTEXMFRoots ();

public:
  virtual
  PathName
  MIKTEXTHISCALL
  GetRootDirectory (/*[in]*/ unsigned	r);

public:
  virtual
  PathName
  MIKTEXTHISCALL
  GetMpmDatabasePathName ();

public:
  unsigned
  MIKTEXTHISCALL
  DeriveTEXMFRoot (/*[in]*/ const PathName & path);

public:
  virtual
  bool
  MIKTEXTHISCALL
  FindFilenameDatabase (/*[in]*/ unsigned	r,
			/*[out]*/ PathName &	path);

public:
  virtual
  PathName
  MIKTEXTHISCALL
  GetFilenameDatabasePathName (/*[in]*/ unsigned	r);

public:
  virtual
  bool
  MIKTEXTHISCALL
  UnloadFilenameDatabase ();

public:
  virtual
  unsigned
  MIKTEXTHISCALL
  SplitTEXMFPath (/*[in]*/ const PathName &	path,
		  /*[out]*/ PathName &		root,
		  /*[out]*/ PathName &		relative);

public:
  virtual
  void
  MIKTEXTHISCALL
  RegisterRootDirectories (/*[in]*/ const string &	roots);

public:
  virtual
  void
  MIKTEXTHISCALL
  RegisterRootDirectories (/*[in]*/ const StartupConfig &	startupConfig,
			   /*[in]*/ bool			sessionOnly);

public:
  virtual
  bool
  MIKTEXTHISCALL
  IsMiKTeXDirect ();

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetMETAFONTMode (/*[in]*/ unsigned		idx,
		   /*[out]*/ MIKTEXMFMODE *	pMode);

public:
  virtual
  bool
  MIKTEXTHISCALL
  DetermineMETAFONTMode (/*[in]*/ unsigned		dpi,
			 /*[out]*/ MIKTEXMFMODE *	pMode);

public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetConfigValue (/*[in]*/ const char *	lpszSectionName,
		     /*[in]*/ const char *	lpszValueName,
		     /*[out]*/ string &		value);

public:
  virtual
  string
  MIKTEXTHISCALL
  GetConfigValue (/*[in]*/ const char *	lpszSectionName,
		  /*[in]*/ const char *	lpszValueName,
		  /*[in]*/ const char *	lpszDefaultValue);

public:
  virtual
  int
  MIKTEXTHISCALL
  GetConfigValue (/*[in]*/ const char *	lpszSectionName,
		  /*[in]*/ const char *	lpszValueName,
		  /*[in]*/ int			defaultValue);

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetConfigValue (/*[in]*/ const char *	lpszSectionName,
		  /*[in]*/ const char *	lpszValueName,
		  /*[in]*/ bool			defaultValue);

public:
  virtual
  TriState
  MIKTEXTHISCALL
  GetConfigValue (/*[in]*/ const char *	lpszSectionName,
		  /*[in]*/ const char *	lpszValueName,
		  /*[in]*/ TriState		defaultValue);

public:
  virtual
  void
  MIKTEXTHISCALL
  SetUserConfigValue (/*[in]*/ const char *	lpszSectionName,
		  /*[in]*/ const char *	lpszValueName,
		  /*[in]*/ const char *	lpszValue);

public:
  virtual
  void
  MIKTEXTHISCALL
  SetUserConfigValue (/*[in]*/ const char *	lpszSectionName,
		  /*[in]*/ const char *	lpszValueName,
		  /*[in]*/ bool			value);

public:
  virtual
  void
  MIKTEXTHISCALL
  SetUserConfigValue (/*[in]*/ const char *	lpszSectionName,
		      /*[in]*/ const char *	lpszValueName,
		      /*[in]*/ int			value);

public:
  virtual
  void
  MIKTEXTHISCALL
  SharedMiKTeXSetup (/*[in]*/ bool shared);

public:
  virtual
  void
  MIKTEXTHISCALL
  SharedMiKTeXSetup (/*[in]*/ bool shared,
		     /*[in]*/ bool sessionOnly);
  
public:
  virtual
  FILE *
  MIKTEXTHISCALL
  OpenFile (/*[in]*/ const char *	lpszPath,
	    /*[in]*/ FileMode		mode,
	    /*[in]*/ FileAccess		access,
	    /*[in]*/ bool		isTextFile);

public:
  virtual
  FILE *
  MIKTEXTHISCALL
  TryOpenFile (/*[in]*/ const char *	lpszPath,
	       /*[in]*/ FileMode		mode,
	       /*[in]*/ FileAccess		access,
	       /*[in]*/ bool			isTextFile);

public:
  virtual
  FILE *
  MIKTEXTHISCALL
  OpenFile (/*[in]*/ const char *	lpszPath,
	    /*[in]*/ FileMode		mode,
	    /*[in]*/ FileAccess		access,
	    /*[in]*/ bool		isTextFile,
	    /*[in]*/ FileShare		share);

public:
  virtual
  FILE *
  MIKTEXTHISCALL
  TryOpenFile (/*[in]*/ const char *	lpszPath,
	       /*[in]*/ FileMode		mode,
	       /*[in]*/ FileAccess		access,
	       /*[in]*/ bool			isTextFile,
	       /*[in]*/ FileShare		share);

public:
  virtual
  void
  MIKTEXTHISCALL
  CloseFile (/*[in]*/ FILE *	pFile);

public:
  virtual
  bool
  MIKTEXTHISCALL
  IsOutputFile (/*[in]*/ const FILE *	pFile);

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  bool
  MIKTEXTHISCALL
  IsFileAlreadyOpen (/*[in]*/ const char *	lpszFileName);
#endif

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  void
  MIKTEXTHISCALL
  ScheduleFileRemoval (/*[in]*/ const char *	lpszFileName);
#endif

public:
  virtual
  bool
  MIKTEXTHISCALL
  StartFileInfoRecorder (/*[in]*/ bool bRecordPackageNames);

public:
  virtual
  vector<FileInfoRecord>
  MIKTEXTHISCALL
  GetFileInfoRecords ();

public:
  virtual
  FileType
  MIKTEXTHISCALL
  DeriveFileType (/*[in]*/ const char * lpszPath);

public:
  virtual
  bool
  MIKTEXTHISCALL
  FindFile (/*[in]*/ const char *	lpszFileName,
	    /*[in]*/ const char *	lpszPathList,
	    /*[out]*/ PathName &	path);

public:
  virtual
  bool
  MIKTEXTHISCALL
  FindFile (/*[in]*/ const char *	lpszFileName,
	    /*[in]*/ FileType		fileType,
	    /*[out]*/ PathName &	path);

public:
  virtual
  bool
  MIKTEXTHISCALL
  FindPkFile (/*[in]*/ const char *	lpszFontName,
	      /*[in]*/ const char *	lpszMode,
	      /*[in]*/ int			dpi,
	      /*[out]*/ PathName &		path);

public:
  virtual
  bool
  MIKTEXTHISCALL
  FindTfmFile (/*[in]*/ const char *	lpszFontName,
	       /*[out]*/ PathName &		path,
	       /*[in]*/ bool			create);

public:
  virtual
  void
  MIKTEXTHISCALL
  SetFindFileCallback (/*[in]*/ IFindFileCallback *	pCallback);

public:
  virtual
  void
  MIKTEXTHISCALL
  SplitFontPath (/*[in]*/ const char *	lpszFontPath,
		 /*[out]*/ char *		lpszFontType,
		 /*[out]*/ char *		lpszSupplier,
		 /*[out]*/ char *		lpszTypeface,
		 /*[out]*/ char *		lpszFontName,
		 /*[out]*/ char *		lpszPointSize);

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetFontInfo (/*[in]*/ const char *	lpszFontName,
	       /*[out]*/ char *		lpszSupplier,
	       /*[out]*/ char *		lpszTypeface,
	       /*[out]*/ double *		lpGenSize);

public:
  virtual
  void
  MIKTEXTHISCALL
  GetGhostscript (/*[out]*/ char *	lpszPath,
		  /*[out]*/ unsigned long *	pVersionNumber);

public:
  virtual
  string
  MIKTEXTHISCALL
  GetExpandedSearchPath (/*[in]*/ FileType	fileType);

public:
  virtual
  bool
  MIKTEXTHISCALL
  FindGraphicsRule (/*[in]*/ const char *	lpszFrom,
		    /*[in]*/ const char *	lpszTo,
		    /*[out]*/ char *	lpszRule,
		    /*[in]*/ size_t	bufSize);

public:
  virtual
  bool
  MIKTEXTHISCALL
  ConvertToBitmapFile (/*[in]*/ const char *	lpszSourceFileName,
		       /*[out]*/ char *		lpszDestFileName,
		       /*[in]*/ IRunProcessCallback *	pCallback);

public:
  virtual
  bool
  MIKTEXTHISCALL
  EnableFontMaker (/*[in]*/ bool enable);

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetMakeFontsFlag ();

public:
  virtual
  void
  MIKTEXTHISCALL
  MakeMakePkCommandLine (/*[in]*/ const char *	lpszFontName,
			 /*[in]*/ int			dpi,
			 /*[in]*/ int			baseDpi,
			 /*[in]*/ const char *	lpszMfMode,
			 /*[out]*/ PathName &		fileName,
			 /*[out]*/ char *		lpszArguments,
			 /*[in]*/ size_t		bufSize);

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  int
  MIKTEXTHISCALL
  RunBatch (/*[in]*/ int			argc,
	    /*[in]*/ const char **	argv);
#endif

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  int
  MIKTEXTHISCALL
  RunBatch (/*[in]*/ const char *	lpszName,
	    /*[in]*/ const char *	lpszArguments);
#endif

public:
  virtual
  int
  MIKTEXTHISCALL
  RunPerl (/*[in]*/ int			argc,
	   /*[in]*/ const char **	argv);

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  bool
  MIKTEXTHISCALL
  ShowManualPageAndWait (/*[in]*/ HWND		hWnd,
			 /*[in]*/ unsigned long	topic);
#endif

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetNextFileTypeInfo (/*[in]*/ unsigned	index,
		       /*[out]*/ FileTypeInfo &	fileTypeInfo);

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetFormatInfo (/*[in]*/ unsigned	index,
		 /*[out]*/ FormatInfo &	formatInfo);

public:
  virtual
  FormatInfo
  MIKTEXTHISCALL
  GetFormatInfo (/*[in]*/ const char *	lpszKey);

public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetFormatInfo (/*[in]*/ const char *	lpszKey,
		    /*[out]*/ FormatInfo &	formatInfo);

public:
  virtual
  void
  MIKTEXTHISCALL
  DeleteFormatInfo (/*[in]*/ const char *	lpszKey);

public:
  virtual
  void
  MIKTEXTHISCALL
  SetFormatInfo (/*[in]*/ const FormatInfo &	formatInfo);

public:
  virtual
  PathName
  MIKTEXTHISCALL
  GetMyLocation ();

public:
  virtual
  bool
  MIKTEXTHISCALL
  RunningAsAdministrator ();

public:
  virtual
  TriState
  MIKTEXTHISCALL
  IsSharedMiKTeXSetup ();

public:
  virtual
  bool
  MIKTEXTHISCALL
  GetPaperSizeInfo (/*[in]*/ int		idx,
		    /*[out]*/ PaperSizeInfo &	paperSize);

public:
  virtual
  PaperSizeInfo
  MIKTEXTHISCALL
  GetPaperSizeInfo (/*[in]*/ const char *	lpszName);

public:
  virtual
  PolicyFlags
  MIKTEXTHISCALL
  GetPolicyFlags ();

public:
  virtual
  void
  MIKTEXTHISCALL
  SetDefaultPaperSize (/*[in]*/ const char * lpszDvipsName);

public:
  virtual
  bool
  MIKTEXTHISCALL
  TryCreateFromTemplate (/*[in]*/ const PathName & path);

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  bool
  MIKTEXTHISCALL
  RunningAsPowerUser ();
#endif

public:
  virtual
  bool
  MIKTEXTHISCALL
  IsUserAnAdministrator ();

#if defined(MIKTEX_WINDOWS)
public:
  virtual
  bool
  MIKTEXTHISCALL
  IsUserAPowerUser ();
#endif

public:
  virtual
  void
  MIKTEXTHISCALL
  ConfigureFile (/*[in]*/ const PathName & pathIn,
		 /*[in]*/ const PathName & pathOut);

public:
  virtual
  void
  MIKTEXTHISCALL
  SetTheNameOfTheGame (/*[in]*/ const char * lpszTheNameOfTheGame);

public:
  virtual
  string
  MIKTEXTHISCALL
  GetLocalFontDirectories ();

public:
  virtual
  void
  MIKTEXTHISCALL
  ConfigureFile (/*[in]*/ const PathName & path);

  // -----------------------------------------------------------------------
  // *** public ***

public:
  unsigned
  TryDeriveTEXMFRoot (/*[in]*/ const PathName & path);

public:
  bool
  IsTEXMFFile (/*[in]*/ const char *	lpszPath,
	       /*[out]*/ char *		lpszRelPath,
	       /*[out]*/ unsigned *		pRootIndex);

public:
  void
  RecordFileInfo (/*[in]*/ const char *	lpszPath,
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
  GetFileNameDatabase (/*[in]*/ const char *	lpszPath);

public:
  PathName
  GetTempDirectory ();

public:
  PathName
  GetMyProgramFile ();

public:
  bool
  GetPsFontDirs (/*[out]*/ string &	psFontDirs);

public:
  bool
  GetOTFDirs (/*[out]*/ string &	otfDirs);

public:
  bool
  GetTTFDirs (/*[out]*/ string &	ttfDirs);

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
  GetAcrobatFontDir (/*[out]*/ PathName &	path);
#endif

#if defined(MIKTEX_WINDOWS)
private:
  bool
  GetATMFontDir (/*[out]*/ PathName &	path);
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
  auto_ptr<TraceStream> trace_mem;
  auto_ptr<TraceStream> trace_packages;
  auto_ptr<TraceStream> trace_process;
  auto_ptr<TraceStream> trace_tempfile;
  auto_ptr<TraceStream> trace_time;
  auto_ptr<TraceStream> trace_values;

public:
  locale defaultLocale;

private:
  std::vector<PathName> scratchDirectories;

public:
  size_t
  GetNumberOfScratchDirectories ()
  {
    return (scratchDirectories.size());
  }

public:
  PathName
  GetScratchDirectory ()
  {
    MIKTEX_ASSERT (! scratchDirectories.empty());
    return (scratchDirectories.back());
  }

public:
  void
  PushScratchDirectory (/*[in]*/ const PathName & dir)
  {
    scratchDirectories.push_back (dir);
    ClearSearchVectors ();
  }

public:
  void
  PopScratchDirectory ()
  {
    MIKTEX_ASSERT (! scratchDirectories.empty());
    scratchDirectories.pop_back ();
    ClearSearchVectors ();
  }

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
	  ("SessionImpl::GetSession",
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

#if defined(HAVE_ATLBASE_H) && ! defined(MIKTEX_STATIC)
public:
  static
  BOOL
  AtlDllMain (/*[in]*/ DWORD	reason,
	      /*[in]*/ LPVOID	lpReserved);
#endif

  // -----------------------------------------------------------------------
  // *** private ***

private:
  class FlagsEnum {
  public:
    enum EnumType {
      CachedLocalFontDirs,
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
  string localFontDirs;

private:
  string psFontDirs;

private:
  string ttfDirs;

private:
  string otfDirs;

#if defined(MIKTEX_WINDOWS)
private:
  PathName acrobatFontDir;
#endif

#if defined(MIKTEX_WINDOWS)
private:
  PathName atmFontDir;
#endif

private:
  void
  SetCWDEnv ();

private:
  bool
  FindPerl (/*[out]*/ PathName & perl);

private:
  bool
  FindPerlScript (/*[in]*/ const char *	lpszName,
		  /*[out]*/ PathName &	path);

#if defined(MIKTEX_WINDOWS)
private:
  bool
  FindBatchFile (/*[in]*/ const char *	lpszName,
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
  TraceSearchVector (/*[in]*/ const char *	lpszKey,
		     /*[in]*/ const PathNameArray &	pathvec);

private:
  void
  RegisterLibraryTraceStreams ();

private:
  void
  UnregisterLibraryTraceStreams ();

  
private:
  bool
  FindInTypefaceMap (/*[in]*/ const char *	lpszFontName,
		     /*[out]*/ char *		lpszTypeface);

private:
  bool
  FindInSupplierMap (/*[in]*/ const char *	lpszFontName,
		     /*[out]*/ char *		lpszSupplier,
		     /*[out]*/ char *		lpszTypeface);

private:
  bool
  FindInSpecialMap (/*[in]*/ const char *	lpszFontName,
		    /*[out]*/ char *	lpszSupplier,
		    /*[out]*/ char *	lpszTypeface);

private:
  bool
  InternalGetFontInfo (/*[in]*/ const char *	lpszFontName,
		       /*[out]*/ char *		lpszSupplier,
		       /*[out]*/ char *		lpszTypeface);


private:
  bool
  MakePkFileName (/*[out]*/ PathName &		pkFileName,
		  /*[in]*/ const char *	lpszFontName,
		  /*[in]*/ int			dpi);


private:
  bool
  FindFileAlongVec (/*[in]*/ const char *		lpszFileName,
		    /*[in]*/ const PathNameArray &	vec,
		    /*[out]*/ PathName &		path);

private:
  bool
  SlowFindFile (/*[in]*/ const char *	lpszFileName,
		/*[in]*/ const char *	lpszPathList,
		/*[in]*/ PathName &		path);

private:
  bool
  SearchFileSystem (/*[in]*/ const char *	lpszRelPath,
		    /*[in]*/ const char *	lpszDirPath,
		    /*[out]*/ PathName &	result);


private:
bool
SearchFileSystem (/*[in]*/ const char *	lpszCurDir,
		  /*[in]*/ const char *	lpszSubDir,
		  /*[in]*/ const char *	lpszSearchSpec,
		  /*[out]*/ PathName &		result);


private:
  bool
  CheckCandidate (/*[in,out]*/ PathName &	path,
		  /*[in]*/ const char *	lpszFileInfo);

private:
  bool
  GetSessionValue (/*[in]*/ const char *	lpszSectionName,
		   /*[in]*/ const char *	lpszValueName,
		   /*[out]*/ string &		value,
		   /*[in]*/const char *	lpszDefaultValue);

private:
  void
  ReadAllConfigFiles (/*[in]*/ const char *	lpszBaseName,
		      /*[in,out]*/ Cfg *		pCfg);

private:
  std::deque<PathName> inputDirectories;

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

public:
  unsigned
  GetCommonInstallRoot ();

public:
  unsigned
  GetUserInstallRoot ();

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
  AppendToSearchPath (/*[in,out]*/ string &	strSearchPath,
		      /*[in]*/ const string &	strSearchPath2);

private:
  void
  SplitSearchPath (/*[in,out]*/ PathNameArray &	pathvec,
		   /*[in]*/ const char *	lpszSearchPath);

private:
  PathNameArray
  SplitSearchPath (/*[in]*/ const char *	lpszSearchPath);

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
  FindMETAFONTMode (/*[in]*/ const char *	lpszMnemonic,
		    /*[out]*/ MIKTEXMFMODE *	pMode);

private:
  FILE *
  InitiateProcessPipe (/*[in]*/ const char *	lpszCommand,
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
		    /*[in]*/ const char *	lpszFileType,
		    /*[in]*/ const char *	lpszApplication,
		    /*[in]*/ const char *	lpszFileNameExtensions,
		    /*[in]*/ const char *	lpszDefaultSearchPath,
		    /*[in]*/ const char *	lpszEnvVarNames);
    
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
    string		fileName;
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
  string packageHistoryFile;

private:
  bool makeFonts;

private:
#if defined(USE_HASH_MAP)
  typedef
  hash_map<string, SmartPointer<Cfg>, hash_compare_icase>
  ConfigurationSettings;
#else
  typedef
  map<string, SmartPointer<Cfg>, hash_compare_icase> ConfigurationSettings;
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
  string applicationNames;

private:
  string theNameOfTheGame;

private:
  const string &
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

#if defined(MIKTEX_WINDOWS)
private:
  void
  MyCoInitialize ();
#endif

#if defined(MIKTEX_WINDOWS)
private:
  void
  MyCoUninitialize ();
#endif

  // index of common data root
private:
  unsigned commonDataRootIndex;

  // index of user data root
private:
  unsigned userDataRootIndex;

  // index of common install root
private:
  unsigned commonInstallRootIndex;

  // index of user install root
private:
  unsigned userInstallRootIndex;

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

#if defined(MIKTEX_WINDOWS)
private:
  int numCoInitialize;
#endif

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

   AutoTraceTime
   _________________________________________________________________________ */

class AutoTraceTime
{
public:
  AutoTraceTime (/*[in]*/ const char * lpsz1,
		 /*[in]*/ const char * lpsz2)
    : start(clock()),
      str1 (lpsz1),
      str2 (lpsz2)
  {
  }
public:
  ~AutoTraceTime ()
  {
    try
      {
	if (SessionImpl::GetSession()->trace_time->IsEnabled())
	  {
	    SessionImpl::GetSession()->trace_time->WriteFormattedLine
	      ("core",
	       "%s %s %s clock ticks",
	       str1.c_str(),
	       str2.c_str(),
	       NUMTOSTR(clock() - start));
	  }
      }
    catch (const std::exception &)
      {
      }
  }
private:
  clock_t start;
private:
  std::string str1;
private:
  std::string str2;
};

/* _________________________________________________________________________

   CTYPE_FACET
   _________________________________________________________________________ */

#define CTYPE_FACET \
  use_facet<ctype<char> >(SessionImpl::GetDefaultLocale())

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
IsDriveLetter (/*[in]*/ char ch)
{
  return (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z');
}
#endif

/* _________________________________________________________________________

   IsAlpha
   _________________________________________________________________________ */

inline
bool
IsAlpha (/*[in]*/ char ch)
{
  return (CTYPE_FACET.is(ctype<char>::alpha, ch));
}

/* _________________________________________________________________________

   IsDigit
   _________________________________________________________________________ */

inline
bool
IsDigit (/*[in]*/ char ch)
{
  return (CTYPE_FACET.is(ctype<char>::digit, ch));
}

/* _________________________________________________________________________

   IsAlNum
   _________________________________________________________________________ */

inline
bool
IsAlNum (/*[in]*/ char ch)
{
  return (CTYPE_FACET.is(ctype<char>::alnum, ch));
}

/* _________________________________________________________________________

   IsSpace
   _________________________________________________________________________ */

inline
bool
IsSpace (/*[in]*/ char ch)
{
  return (CTYPE_FACET.is(ctype<char>::space, ch));
}

/* _________________________________________________________________________

   SkipSpace
   _________________________________________________________________________ */

inline
void
SkipSpace (/*[in,out]*/ const char * &	lpsz)
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
SkipSpace (/*[in,out]*/ char * &	lpsz)
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
SkipNonSpace (/*[in,out]*/ const char * &	lpsz)
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
SkipNonSpace (/*[in,out]*/ char * &	lpsz)
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
SkipAlpha (/*[in,out]*/ const char * &	lpsz)
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
SkipNonDigit (/*[in]*/ const char * &	lpsz)
{
  while (*lpsz != 0 && ! IsDigit(*lpsz))
    {
      ++ lpsz;
    }
}

/* _________________________________________________________________________

   MakeLower
   _________________________________________________________________________ */

template<typename CharType>
inline
char *
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
	  INVALID_ARGUMENT ("MakeLower", lpszSource);
	}
      *lpszBuf = ToLower(*lpszSource);
    }
  if (l == bufSize)
    {
      INVALID_ARGUMENT ("MakeLower", lpszSource);
    }
  *lpszBuf = 0;
  return (lpszBuf);
}

/* _________________________________________________________________________

   MakeLower
   _________________________________________________________________________ */

inline
string
MakeLower (/*[in]*/ const string &	str)
{
  string ret;
  for (string::const_iterator it = str.begin(); it != str.end(); ++ it)
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
