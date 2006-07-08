/* internal.h: internal definitions				-*- C++ -*-

   Copyright (C) 2001-2006 Christian Schenk

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
   along with MiKTeX Package Manager; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER) && defined(MPM_DLL)
#  define MPMEXPORT __declspec(dllexport)
#else
#  define MPMEXPORT
#endif

#define F927BA187CB94546AB9CA9099D989E81
#include "miktex/mpm.h"

#include "mpm-version.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace std;

#define BEGIN_INTERNAL_NAMESPACE			\
namespace MiKTeX {					\
  namespace Packages {					\
    namespace D6AAD62216146D44B580E92711724B78 {

#define END_INTERNAL_NAMESPACE			\
    }						\
  }						\
}

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

#if ! defined(USE_WEB_SERVICE)
#  if defined(MIKTEX_WINDOWS)
#    define USE_WEB_SERVICE 1
#  else
#    define USE_WEB_SERVICE 0
#  endif
#endif

/* _________________________________________________________________________

   Debug-dependant Macros
   _________________________________________________________________________ */

#if ! defined(NDEBUG)

#  define MIKTEX_ASSERT(expr)					\
  static_cast<void>						\
   ((expr)							\
    ? 0								\
    : (FATAL_MIKTEX_ERROR (0, T_("Assertion failed."), #expr),	\
       0))

#  define MIKTEX_ASSERT_BUFFER(buf, n) AssertValidBuf (buf, n)

#  define MIKTEX_ASSERT_BUFFER_OR_NIL(buf, n)	\
  if (buf != 0)					\
    {						\
      AssertValidBuf (buf, n);			\
    }

#  define MIKTEX_ASSERT_CHAR_BUFFER(buf, n)	\
  AssertValidBuf (buf, sizeof(MIKTEXCHAR) * n)

#  define MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL(buf, n)	\
  if (buf != 0)						\
    {							\
      AssertValidBuf (buf, sizeof(MIKTEXCHAR) * n);	\
    }

#  define MIKTEX_ASSERT_STRING(str) AssertValidString (str)

#  define MIKTEX_ASSERT_STRING_OR_NIL(str)	\
  if (str != 0)					\
    {						\
      AssertValidString (str);			\
    }

#  define MIKTEX_ASSERT_PATH_BUFFER(buf)		\
  MIKTEX_ASSERT_CHAR_BUFFER (buf, BufferSizes::MaxPath)

#  define MIKTEX_ASSERT_PATH_BUFFER_OR_NIL(buf)			\
  MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL (buf, BufferSizes::MaxPath)

#  define MIKTEX_ASSERT_FNAME_BUFFER(buf)		\
  MIKTEX_ASSERT_CHAR_BUFFER (buf, BufferSizes::MaxPath)

#else

#  define MIKTEX_ASSERT(expr)
#  define MIKTEX_ASSERT_BUFFER(buf, n)
#  define MIKTEX_ASSERT_BUFFER_OR_NIL(buf, n)
#  define MIKTEX_ASSERT_CHAR_BUFFER(buf, n)
#  define MIKTEX_ASSERT_CHAR_BUFFER_OR_NIL(buf, n)
#  define MIKTEX_ASSERT_FNAME_BUFFER(buf)
#  define MIKTEX_ASSERT_PATH_BUFFER(buf)
#  define MIKTEX_ASSERT_PATH_BUFFER_OR_NIL(buf)
#  define MIKTEX_ASSERT_STRING(str)
#  define MIKTEX_ASSERT_STRING_OR_NIL(str)
#endif

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

#define FATAL_MPM_ERROR(miktexFunction, traceMessage, lpszInfo) \
  FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)

#if USE_WEB_SERVICE
#  define FATAL_SOAP_ERROR(pSoap)		\
  FatalSoapError (pSoap,			\
                  T_(__FILE__),			\
                  __LINE__)
#endif

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) Quoted(x).c_str()

#define MPMSTATICFUNC(type) static type
#define MPMINTERNALFUNC(type) type
#define MPMINTERNALVAR(type) type

#define ARRAY_SIZE(buf) (sizeof(buf)/sizeof(buf[0]))

#define MPM_APSIZE_DB_LIGHT 1024 * 40
#define MPM_APSIZE_DB_FULL 1024 * 245

#define POLLUTE_THE_DEBUG_STREAM 0
#define DUMP_PACKAGE_TABLE_ON_ERROR 1
#define DUMP_CURRENT_DIRECTORY_ON_ERROR 1

#define DB_ARCHIVE_FILE_TYPE ArchiveFileType::TarBzip2

#define ZZDB1_FILE_NAME							\
  PathName(0,								\
           MIKTEX_MPM_DB_LIGHT_FILE_NAME_NO_SUFFIX,			\
           ArchiveFileType::GetFileNameExtension(DB_ARCHIVE_FILE_TYPE))

#define ZZDB2_FILE_NAME							\
  PathName(0,								\
           MIKTEX_MPM_DB_FULL_FILE_NAME_NO_SUFFIX,			\
           ArchiveFileType::GetFileNameExtension(DB_ARCHIVE_FILE_TYPE))

#include "Extractor.h"
#include "WebSession.h"

BEGIN_INTERNAL_NAMESPACE;

const time_t Y2000 = 946681200;

typedef basic_ostringstream<MIKTEXCHAR> otstringstream;

const MIKTEXCHAR * const MPM_AGENT = T_("MPM/") VER_FILEVERSION_STR;

const size_t MAXURL = 1024;

/* _________________________________________________________________________

   CURRENT_DIRECTORY
   _________________________________________________________________________ */

#define CURRENT_DIRECTORY T_(".")

/* _________________________________________________________________________

   TEXMF_PREFIX_DIRECTORY
   _________________________________________________________________________ */

#define TEXMF_PREFIX_DIRECTORY \
  T_("texmf") MIKTEX_PATH_DIRECTORY_DELIMITER_STRING

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
tstring
Quoted (/*[in]*/ const MIKTEXCHAR * lpsz)
{
  bool needQuotes = (strchr(lpsz, T_(' ')) != 0);
  tstring result;
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
tstring
Quoted (/*[in]*/ const tstring & str)
{
  return (Quoted(str.c_str()));
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
tstring
Quoted (/*[in]*/ const PathName & path)
{
  return (Quoted(path.Get()));
}

/* _________________________________________________________________________

   PrefixedPath
   _________________________________________________________________________ */

inline
PathName
PrefixedPath (/*[in]*/ const MIKTEXCHAR * lpszPath)
{
  MIKTEX_ASSERT (! Utils::IsAbsolutePath(lpszPath));
  return (PathName(TEXMF_PREFIX_DIRECTORY, lpszPath, 0));
}

/* _________________________________________________________________________

   StripPrefix
   _________________________________________________________________________ */

inline
bool
StripPrefix (/*[in]*/ const tstring &		str,
	     /*[in]*/ const MIKTEXCHAR *	lpszPrefix,
	     /*[out]*/ tstring &		result)
{
  size_t n = StrLen(lpszPrefix);
  if (PathName::Compare(str.c_str(), lpszPrefix, n) != 0)
    {
      return (false);
    }
  result = str.c_str() + n;
  return (true);
}

/* _________________________________________________________________________

   InstalledFileInfo
   _________________________________________________________________________ */

struct InstalledFileInfo
{
  InstalledFileInfo ()
    : refCount (0)
  {
  }
  unsigned long	refCount;
};

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
    const
  {
    return (buffer);
  }

private:
  MIKTEXCHAR buffer[BUFSIZE];
};

#define NUMTOSTR(num) NumberToStringConverter_(num).Get()

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
    catch (const exception &)
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

   TempFile
   _________________________________________________________________________ */

class TempFile
{
public:
  TempFile ()
  {
  }

public:
  TempFile (/*[in]*/ const PathName & path)
    : path (path)
  {
  }

public:
  TempFile &
  operator= (/*[in]*/ const PathName & path)
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
	PathName tmp (path);
	path = T_("");
	File::Delete (tmp);
      }
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

   AssertValidBuf
   _________________________________________________________________________ */

inline void
AssertValidBuf (/*[in]*/ void *	lp,
		/*[in]*/ size_t	n)
{
#if defined(_MSC_VER) && ! defined(NDEBUG)
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
#if defined(_MSC_VER) && ! defined(NDEBUG)
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
tstring &
GetErrnoMessage (/*[in]*/ int		err,
		 /*[out]*/ tstring &	message)
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

   PackageManagerImpl
   _________________________________________________________________________ */

class
PackageManagerImpl
  : public PackageManager,
    public ICreateFndbCallback,
    public IProgressNotify_
{
public:
  virtual
  MPMCALL
  ~PackageManagerImpl ();

public:
  virtual
  void
  MPMCALL
  AddRef ();

public:
  virtual
  void
  MPMCALL
  Release ();

public:
  virtual
  class PackageInstaller *
  MPMCALL
  CreateInstaller ();

public:
  virtual
  class PackageIterator *
  MPMCALL
  CreateIterator ();

public:
  virtual
  void
  MPMCALL
  CreateMpmFndb ();

public:
  virtual
  unsigned long
  MPMCALL
  GetFileRefCount (/*[in]*/ const PathName &	path);

public:
  virtual
  PackageInfo
  MPMCALL
  GetPackageInfo (/*[in]*/ const tstring &	deploymentName);

public:
  virtual
  void
  MPMCALL
  LoadDatabase (/*[in]*/ const PathName & path);

public:
  virtual
  void
  MPMCALL
  UnloadDatabase ();

public:
  virtual
  void
  MPMCALL
  DownloadRepositoryList ();

public:
  virtual
  vector<RepositoryInfo>
  MPMCALL
  GetRepositories ()
  {
    return (repositories);
  }

public:
  virtual
  tstring
  MPMCALL
  PickRepositoryUrl ();

public:
  virtual
  bool
  MPMCALL
  TryGetPackageInfo (/*[in]*/ const tstring &	deploymentName,
		     /*[out]*/ PackageInfo &	packageInfo);

private:
  virtual
  bool
  MIKTEXCALL
  ReadDirectory (/*[in]*/ const MIKTEXCHAR *	lpszPath,
		 /*[out]*/ MIKTEXCHAR * *	ppSubDirNames,
		 /*[out]*/ MIKTEXCHAR * *	ppFileNames,
		 /*[out]*/ MIKTEXCHAR * *	ppFileNameInfos);

private:
  virtual
  bool
  MIKTEXCALL
  OnProgress (/*[in]*/ unsigned			level,
	      /*[in]*/ const MIKTEXCHAR *	lpszDirectory);

public:
  virtual
  void
  OnProgress ();
  
public:
  PackageManagerImpl ();

public:
  void
  ClearAll ();

public:
  void
  IncrementFileRefCounts (/*[in]*/ const tstring & deploymentName);

public:
  void
  NeedInstalledFileInfoTable ();

public:
  void
  FlushVariablePackageTable ();

public:
  void
  GetAllPackageDefinitions (/*[out]*/ vector<PackageInfo> & packages);

public:
  PackageInfo *
  TryGetPackageInfo (/*[in]*/ const tstring & deploymentName);

public:
  InstalledFileInfo *
  GetInstalledFileInfo (/*[in]*/ const MIKTEXCHAR * lpszPath);

public:
  time_t
  GetTimeInstalled (/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName);

public:
  bool
  IsPackageInstalled (/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName);

public:
  bool
  IsPackageObsolete (/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName);

public:
  void
  DeclarePackageObsolete (/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName,
			  /*[in]*/ bool			obsolete);

public:
  void
  SetTimeInstalled (/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName,
		    /*[in]*/ time_t		timeInstalled);

public:
  PackageInfo *
  DefinePackage (/*[in]*/ const tstring &	deploymentName,
		 /*[in]*/ const PackageInfo &	packageinfo);

public:
  void
  TraceError (/*[in]*/ const MIKTEXCHAR *   lpszFormat,
	      /*[in]*/			    ...);

#if USE_WEB_SERVICE
private:
  void
  DownloadRepositoryListWS ();
#endif

#if ! USE_WEB_SERVICE
private:
  void
  DownloadRepositoryListCSV ();
#endif

private:
  void
  LoadVariablePackageTable ();

private:
  void
  IncrementFileRefCounts (/*[in]*/ const vector<tstring> & files);

private:
  void
  ParseAllPackageDefinitionFilesInDirectory
  (/*[in]*/ const PathName & directory);

private:
  void
  ParseAllPackageDefinitionFiles ();

private:
  void
  Dispose ();

public:
  static
  RepositoryType
  DetermineRepositoryType (/*[in]*/ const tstring & repository);

private:
  int refCount;

private:
  auto_ptr<TraceStream> trace_error;

private:
  auto_ptr<TraceStream> trace_mpm;

private:
#if defined(USE_HASH_MAP)
  typedef hash_map<tstring,
		   PackageInfo,
		   hash_compare_icase> PackageDefinitionTable;
#else
  typedef map<tstring,
	      PackageInfo,
	      hash_compare_icase> PackageDefinitionTable;
#endif

private:
  PackageDefinitionTable packageTable;

private:
#if defined(USE_HASH_MAP)
  typedef hash_map<tstring,
		   InstalledFileInfo,
		   hash_compare_path> InstalledFileInfoTable;
#else
  typedef map<tstring,
	       InstalledFileInfo,
	       hash_compare_path> InstalledFileInfoTable;
#endif

private:
  InstalledFileInfoTable installedFileInfoTable;

private:
  bool parsedAllPackageDefinitionFiles;

private:
  SmartPointer<Cfg> variablePackageTable;

private:
  SessionWrapper pSession;

private:
  auto_ptr<WebSession> webSession;

public:
  WebSession *
  GetWebSession ()
    const
  {
    return (webSession.get());
  }

private:
  vector<RepositoryInfo> repositories;

public:
  static tstring proxyUser;

public:
  static tstring proxyPassword;
};

/* _________________________________________________________________________

   DbLight
   _________________________________________________________________________ */

class DbLight
{
private:
  SmartPointer<Cfg> pcfg;

public:
  DbLight ()
    : pcfg(Cfg::Create())
  {
  }

public:
  virtual
  ~DbLight ()
  {
  }

public:
  MD5
  GetDigest ()
  {
    return (pcfg->GetDigest());
  }

private:
  bool
  TryGetValue (/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName,
	    /*[in]*/ const MIKTEXCHAR *	lpszValueName,
	    /*[out]*/ tstring &		value)
  {
    return (pcfg->TryGetValue(lpszDeploymentName, lpszValueName, value));
  }

public:
  void
  Read (/*[in]*/ const PathName & path)
  {
    pcfg->Read (path);
  }

public:
  MIKTEXCHAR *
  FirstPackage (/*[out]*/ MIKTEXCHAR *	lpszDeploymentName)
  {
    return (pcfg->FirstKey(lpszDeploymentName, BufferSizes::MaxPath));
  }

public:
  MIKTEXCHAR *
  NextPackage (/*[out]*/ MIKTEXCHAR *	lpszDeploymentName)
  {
    return (pcfg->NextKey(lpszDeploymentName, BufferSizes::MaxPath));
  }

public:
  void
  Clear ()
  {
    pcfg.Release ();
    pcfg = Cfg::Create();
  }

public:
  int
  GetArchiveFileSize (/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName)
  {
    tstring str;
    if (! TryGetValue(lpszDeploymentName, T_("CabSize"), str))
      {
	FATAL_MPM_ERROR (T_("DbLight::GetArchiveFileSize"),
			 T_("Unknown archive file size."),
			 lpszDeploymentName);
      }
    return (AToI(str.c_str()));
  }

public:
  MD5
  GetArchiveFileDigest (/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName)
  {
    tstring str;
    if (! pcfg->TryGetValue(lpszDeploymentName, T_("CabMD5"), str))
      {
	FATAL_MPM_ERROR (T_("DbLight::GetArchiveFileDigest"),
			 T_("Unknown archive file digest."),
			 lpszDeploymentName);
      }
    return (MD5::Parse(str.c_str()));
  }

public:
  MD5
  GetPackageDigest (/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName)
  {
    tstring str;
    if (! pcfg->TryGetValue(lpszDeploymentName, T_("MD5"), str))
      {
	FATAL_MPM_ERROR (T_("DbLight::GetPackageDigest"),
			 T_("Unknown package digest."),
			 lpszDeploymentName);
      }
    return (MD5::Parse(str.c_str()));
  }

public:
  time_t
  GetTimePackaged (/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName)
  {
    tstring str;
    if (! TryGetValue(lpszDeploymentName, T_("TimePackaged"), str))
      {
	FATAL_MPM_ERROR (T_("DbLight::GetTimePackaged"),
			 T_("Unknown timestamp."),
			 lpszDeploymentName);
      }
    unsigned time = static_cast<unsigned>(AToI(str.c_str()));
    if (time < Y2000)
      {
	FATAL_MPM_ERROR (T_("DbLight::GetTimePackaged"),
			 T_("Invalid timestamp."),
			 lpszDeploymentName);
      }
    return (time);
  }

public:
  PackageLevel
  GetPackageLevel (/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName)
  {
    tstring str;
    if (! TryGetValue(lpszDeploymentName, T_("Level"), str))
      {
	FATAL_MPM_ERROR (T_("DbLight::GetPackageLevel"),
			 T_("Unknown package level."),
			 lpszDeploymentName);
      }
    return (CharToPackageLevel(str[0]));
  }

public:
  tstring
  GetPackageVersion (/*[in]*/ const MIKTEXCHAR *	lpszDeploymentName)
  {
    tstring version;
    if (! TryGetValue(lpszDeploymentName, T_("Version"), version))
      {
	version = T_("");
      }
    return (version);
  }

public:
  ArchiveFileType
  GetArchiveFileType (/*[in]*/  const MIKTEXCHAR *	lpszDeploymentName)
  {
    tstring str;
    if (! TryGetValue(lpszDeploymentName, T_("Type"), str))
      {
	return (ArchiveFileType::MSCab);
      }
    if (str == T_("MSCab"))
      {
	return (ArchiveFileType::MSCab);
      }
    else if (str == T_("TarBzip2"))
      {
	return (ArchiveFileType::TarBzip2);
      }
    else
      {
	FATAL_MPM_ERROR (T_("DbLight::GetArchiveFileType"),
			 T_("Unknown archive file type."),
			 lpszDeploymentName);
      }
  }

public:
  static
  PackageLevel
  CharToPackageLevel (/*[in]*/ MIKTEXCHARINT ch)
  {
    switch (toupper(ch))
    {
    case T_('S'): return (PackageLevel::Essential);
    case T_('M'): return (PackageLevel::Basic);
    case T_('L'): return (PackageLevel::Advanced);
    case T_('T'): return (PackageLevel::Complete);
    default:
      FATAL_MPM_ERROR (T_("DbLight::CharToPackageLevel"),
		       T_("Invalid package level."),
		       0);
    }
  }
};

/* _________________________________________________________________________

   PackageInstallerImpl
   _________________________________________________________________________ */

class PackageInstallerImpl
  : public PackageInstaller,
    public IProgressNotify_,
    public ICreateFndbCallback,
    public IExtractCallback
{
private:
  enum ErrorCode {
    ERROR_UNKNOWN_PACKAGE,
    ERROR_DOWNLOAD, ERROR_PACKAGE_NOT_INSTALLED,
    ERROR_CANNOT_DELETE,
    ERROR_MISSING_PACKAGE, ERROR_CORRUPTED_PACKAGE,
    ERROR_SOURCE_FILE_NOT_FOUND,
    ERROR_SIZE_MISMATCH,
  };

public:
  PackageInstallerImpl (/*[in]*/ PackageManagerImpl * pManager);

public:
  virtual
  MPMCALL
  ~PackageInstallerImpl ();

public:
  virtual
  void
  MPMCALL
  Dispose ();

public:
  virtual
  void
  MPMCALL
  SetRepository (/*[in]*/ const MiKTeX::Core::tstring & repository)
  {
    this->repository = repository;
    repositoryType =
      PackageManagerImpl::DetermineRepositoryType(repository);
  }

public:
  virtual
  void
  MPMCALL
  SetDestination (/*[in]*/ const PathName & destinationDirectory)
  {
    this->destinationDirectory = destinationDirectory;
  }

public:
  virtual
  void
  MPMCALL
  UpdateDb ();
  
public:
  virtual
  void
  MPMCALL
  UpdateDbAsync ();
  
public:
  virtual
  void
  MPMCALL
  FindUpdates ();

public:
  virtual
  void
  MPMCALL
  FindUpdatesAsync ();

public:
  virtual
  vector<UpdateInfo>
  MPMCALL
  GetUpdates ()
  {
    return (updates);
  }

public:
  virtual
  void
  MPMCALL
  InstallRemove ();

public:
  virtual
  void
  MPMCALL
  InstallRemoveAsync ();

public:
  virtual
  void
  MPMCALL
  WaitForCompletion ();

public:
  virtual
  void
  MPMCALL
  Download ();

public:
  virtual
  void
  MPMCALL
  DownloadAsync ();

private:
  void
  StartTaskThread (void (MIKTEXCALLBACK * pFunc) (/*[in]*/ void * pData));

public:
  virtual
  ProgressInfo
  MPMCALL
  GetProgressInfo ()
  {
    MIKTEX_LOCK(ProgressIndicator)
      {
	if (progressInfo.numErrors > 0)
	  {
	    throw threadMiKTeXException;
	  }
	return (progressInfo);
      }
    MIKTEX_UNLOCK();
  }

public:
  virtual
  void
  MPMCALL
  SetCallback (/*[in]*/ IPackageInstallerCallback *	pCallback);
  
public:
  virtual
  void
  MPMCALL
  SetFileLists (/*[in]*/ const vector<tstring> & toBeInstalled,
		/*[in]*/ const vector<tstring> & toBeRemoved)
  {
    this->toBeInstalled = toBeInstalled;
    this->toBeRemoved = toBeRemoved;
  }

public:
  virtual
  void
  MPMCALL
  SetPackageLevel (/*[in]*/ PackageLevel packageLevel)
  {
    taskPackageLevel = packageLevel;
  }
  
public:
  virtual
  void
  MPMCALL
  SetFileList (/*[in]*/ const vector<tstring> & toBeInstalled)
  {
    this->toBeInstalled = toBeInstalled;
    toBeRemoved.clear ();
  }

public:
  virtual
  void
  OnProgress ();
  
public:
  virtual
  void
  OnBeginFileExtraction (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
			 /*[in]*/ size_t		uncompressedSize);

public:
  virtual
  void
  OnEndFileExtraction (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
		       /*[in]*/ size_t			uncompressedSize);

public:
  virtual
  bool
  OnError (/*[in]*/ const MIKTEXCHAR *	lpszMessage);

private:
  void
  UpdateMpmFndb (/*[in]*/ const vector<tstring> &	installedFiles,
		 /*[in]*/ const vector<tstring> &	removedFiles,
		 /*[in]*/ const MIKTEXCHAR *		lpszPackageName);
  
private:
  void
  CalculateExpenditure (/*[in]*/ bool downloadOnly = false);
  
private:
  MIKTEXNORETURN
  void
  FatalError (/*[in]*/ ErrorCode	error,
	      /*[in]*/			...);
  
private:
  Thread * pThread;
  
public:
  virtual
  bool
  MPMCALL
  IsRunning ()
    const
  {
    if (pThread == 0)
      {
	return (false);
      }
    return (! progressInfo.ready);
  }

private:
  tstring repository;
  
private:
  PathName destinationDirectory;
  
private:
  DbLight dbLight;
  
private:
  void
  InstallDbLight ();

private:
  void
  LoadDbLight (/*[in]*/ bool download);
  
private:
  RepositoryType repositoryType;

private:
  void
  ReportLine (/*[in]*/ const MIKTEXCHAR *   lpsz,
	      /*[in]*/			    ...);

private:
  tstring
  MakeUrl (/*[in]*/ const MIKTEXCHAR * lpszBase,
	   /*[in]*/ const MIKTEXCHAR * lpszRel);

private:
  tstring
  MakeUrl (/*[in]*/ const MIKTEXCHAR * lpszRel);

private:
  bool
  AbortOrRetry (/*[in]*/ const MIKTEXCHAR * lpszMessage)
  {
    return (pCallback == 0 || ! pCallback->OnRetryableError(lpszMessage));
  }
  
private:
  void
  Notify (/*[in]*/ Notification	      nf = Notification::None)
  {
    if (pCallback != 0 && ! pCallback->OnProgress(nf))
      {
	trace_mpm->WriteLine (T_("libmpm"), T_("client wants to cancel"));
	trace_mpm->WriteLine (T_("libmpm"),
			      T_("throwing OperationCancelledException"));
	throw OperationCancelledException ();
      }
  }

private:
  bool autoFndbSync;
  
private:
  ProgressInfo progressInfo;

private:
  MiKTeXException threadMiKTeXException;
  
private:
  clock_t timeStarted;
  
private:
  vector<tstring> toBeInstalled;
  
private:
  vector<tstring> toBeRemoved;
  
private:
  MIKTEX_DEFINE_LOCK(ProgressIndicator);
  MIKTEX_DEFINE_LOCK(This);
  
private:
  PackageLevel taskPackageLevel;
  
private:
  static
  void
  MIKTEXCALLBACK
  InstallRemoveThread (/*[in]*/ void * pv);

private:
  static
  void
  MIKTEXCALLBACK
  DownloadThread (/*[in]*/ void * pv);

private:
  static
  void
  MIKTEXCALLBACK
  UpdateDbThread (/*[in]*/ void * pv);
  
private:
  static
  void
  MIKTEXCALLBACK
  FindUpdatesThread (/*[in]*/ void * pv);
  
private:
  void
  DoInstall ();

private:
  void
  SetUpPackageDefinitionFiles (/*[in]*/ const PathName & directory);

private:
  void
  HandleObsoletePackageDefinitionFiles
  (/*[in]*/ const PathName & temporaryDirectory);
  
private:
  void
  Download (/*[in]*/ const tstring &	url,
	    /*[in]*/ const PathName &	dest,
	    /*[in]*/ size_t		expectedSize = 0);
  
private:
  void
  Download (/*[in]*/ const PathName &	fileName,
	    /*[in]*/ size_t		expectedSize = 0);
  
private:
  void
  RemoveFiles (/*[in]*/ const vector<tstring> & toBeRemoved,
	       /*[in]*/ bool			silently = false);
  
private:
  void
  ExtractFiles (/*[in]*/ const PathName &	archiveFileName,
		/*[in]*/ ArchiveFileType	archiveFileType);

private:
  void
  CopyFiles (/*[in]*/ const vector<tstring> & fileList);

private:
  void
  AddToFileList (/*[in,out]*/ vector<tstring> &	fileList,
		 /*[in]*/ const PathName &	fileName)
    const;

private:
  void
  RemoveFromFileList (/*[in,out]*/ vector<tstring> &	fileList,
		      /*[in]*/ const PathName &		fileName)
    const;

private:
  void
  CopyPackage (/*[in]*/ const tstring & deploymentName);
  
private:
  virtual
  bool
  MIKTEXCALL
  ReadDirectory (/*[in]*/ const MIKTEXCHAR *	lpszPath,
		 /*[out]*/ MIKTEXCHAR * *	ppSubDirNames,
		 /*[out]*/ MIKTEXCHAR * *	ppFileNames,
		 /*[out]*/ MIKTEXCHAR * *	ppFileNameInfos);

private:
  virtual
  bool
  MIKTEXCALL
  OnProgress (/*[in]*/ unsigned			level,
	      /*[in]*/ const MIKTEXCHAR *	lpszDirectory);

private:
  void
  RemovePackage (/*[in]*/ const tstring &	deploymentName);

private:
  void
  InstallPackage (/*[in]*/ const tstring & deploymentName);

private:
  void
  MyCopyFile (/*[in]*/ const PathName &		source,
	      /*[in]*/ const PathName &		dest,
	      /*[out]*/ size_t &		size);
  
private:
  void
  DownloadPackage (/*[in]*/ const tstring & deploymentName);
  
private:
  bool
  CheckArchiveFile (/*[in]*/ const MIKTEXCHAR *	lpszPackage,
		    /*[in]*/ const PathName &	archiveFileName,
		    /*[in]*/ bool		mustBeOk);
  
private:
  auto_ptr<TraceStream> trace_error;

private:
  auto_ptr<TraceStream> trace_mpm;

private:
  SmartPointer<PackageManagerImpl> pManager;

private:
  IPackageInstallerCallback * pCallback;

private:
  vector<UpdateInfo> updates;
};

/* _________________________________________________________________________

   PackageIteratorImpl
   _________________________________________________________________________ */

class PackageIteratorImpl : public PackageIterator
{
public:
  virtual
  MPMCALL
  ~PackageIteratorImpl ();

public:
  virtual
  void
  MPMCALL
  Dispose ();
  
public:
  virtual
  void
  MPMCALL
  AddFilter (/*[in]*/ unsigned int		filter,
	     /*[in]*/ const MIKTEXCHAR *	lpsz = 0)
  {
    if ((filter & PackageFilter::RequiredBy) != 0 && lpsz != 0)
      {
	requiredBy = lpsz;
      }
    this->filter |= filter;
  }
  
public:
  virtual
  bool
  MPMCALL
  GetNext (/*[out]*/ PackageInfo & packageinfo);
  
public:
  PackageIteratorImpl (/*[in]*/ PackageManagerImpl * pManager);
  
private:
  vector<PackageInfo> snapshot;
  
private:
  vector<PackageInfo>::const_iterator iter;
  
private:
  tstring requiredBy;
  
private:
  unsigned long filter;
};

/* _________________________________________________________________________ */

END_INTERNAL_NAMESPACE;

using namespace MiKTeX::Packages::D6AAD62216146D44B580E92711724B78;
