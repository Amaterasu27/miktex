/* internal.h: internal definitions				-*- C++ -*-

   Copyright (C) 2001-2011 Christian Schenk

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

#if defined(_MSC_VER) && ! defined(MIKTEX_STATIC)
#  define MIKTEXMPMEXPORT __declspec(dllexport)
#else
#  define MIKTEXMPMEXPORT
#endif

#define F927BA187CB94546AB9CA9099D989E81
#include "miktex/PackageManager/PackageManager"

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

#if defined(_MSC_VER) && defined(MIKTEX_WINDOWS)
#  pragma comment(lib, "comsuppw.lib")
#endif

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
namespace MiKTeXPackageManagerLib = MAKE_CURVER_ID(MiKTeXPackageManager);
#endif

#define IGNORE_OTHER_SYSTEMS 1

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
                    __FILE__,					\
		    __LINE__)

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  MiKTeX::Core::Session::FatalMiKTeXError (miktexFunction,		\
					traceMessage,			\
					 lpszInfo,			\
					 __FILE__,			\
					 __LINE__)

#define CRT_ERROR(lpszCrtFunction, lpszInfo)				\
  MiKTeX::Core::TraceStream::TraceLastCRTError (lpszCrtFunction,	\
				  lpszInfo,				\
				  __FILE__,				\
				  __LINE__)

#define FATAL_CRT_ERROR(lpszCrtFunction, lpszInfo)		\
  MiKTeX::Core::Session::FatalCrtError (lpszCrtFunction,	\
			  lpszInfo,				\
			  __FILE__,				\
			  __LINE__)

#define FATAL_CRT_ERROR_2(lpszCrtFunction, errorCode, lpszInfo)	\
  MiKTeX::Core::Session::FatalCrtError (lpszCrtFunction,	\
			  errorCode,				\
			  lpszInfo,				\
			  __FILE__,				\
			  __LINE__)

#if defined(MIKTEX_WINDOWS)
#  define WINDOWS_ERROR(lpszWindowsFunction, lpszInfo)			\
  MiKTeX::Core::TraceStream::TraceLastWin32Error (lpszWindowsFunction,	\
				    lpszInfo,				\
				    __FILE__,				\
				    __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  MiKTeX::Core::Session::FatalWindowsError (windowsfunction,	\
			      lpszInfo,				\
			      __FILE__,				\
			      __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR_2(windowsfunction, errorCode, lpszInfo)	\
  MiKTeX::Core::Session::FatalWindowsError (windowsfunction,		\
			      errorCode,				\
			      lpszInfo,					\
			      __FILE__,					\
			      __LINE__)
#endif

#define FATAL_MPM_ERROR(miktexFunction, traceMessage, lpszInfo) \
  FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)

#  define FATAL_SOAP_ERROR(pSoap)		\
  FatalSoapError (pSoap,			\
                  __FILE__,			\
                  __LINE__)

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) MiKTeX::Core::Quoter<char>(x).Get()

#define MPMSTATICFUNC(type) static type
#define MPMINTERNALFUNC(type) type
#define MPMINTERNALVAR(type) type

#define ARRAY_SIZE(buf) (sizeof(buf)/sizeof(buf[0]))

#define MPM_APSIZE_DB_LIGHT 1024 * 40
#define MPM_APSIZE_DB_FULL 1024 * 245

#define POLLUTE_THE_DEBUG_STREAM 0
#define DUMP_PACKAGE_TABLE_ON_ERROR 1
#define DUMP_CURRENT_DIRECTORY_ON_ERROR 1

#define DB_ARCHIVE_FILE_TYPE ArchiveFileType::TarLzma

#include "WebSession.h"

BEGIN_INTERNAL_NAMESPACE;

const time_t Y2000 = 946681200;

const char * const MPM_AGENT = "MPM/" MIKTEX_COMPONENT_VERSION_STR;

const char * const MPMSVC = "mpmsvc";

const size_t MAXURL = 1024;

/* _________________________________________________________________________

   CURRENT_DIRECTORY
   _________________________________________________________________________ */

#define CURRENT_DIRECTORY "."

/* _________________________________________________________________________

   TEXMF_PREFIX_DIRECTORY

   The trailing slash should not be removed.
   _________________________________________________________________________ */

#define TEXMF_PREFIX_DIRECTORY \
  "texmf" MIKTEX_PATH_DIRECTORY_DELIMITER_STRING

/* _________________________________________________________________________

   PrefixedPath
   _________________________________________________________________________ */

inline
MiKTeX::Core::PathName
PrefixedPath (/*[in]*/ const char * lpszPath)
{
  MIKTEX_ASSERT (! MiKTeX::Core::Utils::IsAbsolutePath(lpszPath));
  return (MiKTeX::Core::PathName(TEXMF_PREFIX_DIRECTORY, lpszPath, 0));
}

/* _________________________________________________________________________

   StripPrefix
   _________________________________________________________________________ */

inline
bool
StripPrefix (/*[in]*/ const std::string &	str,
	     /*[in]*/ const char *		lpszPrefix,
	     /*[out]*/ std::string &		result)
{
  size_t n = MiKTeX::Core::StrLen(lpszPrefix);
  if (MiKTeX::Core::PathName::Compare(str.c_str(), lpszPrefix, n) != 0)
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
	UNEXPECTED_CONDITION ("TempFile::operator=");
      }
    path = "";
    return (*this);
  }

public:
  void
  Delete ()
  {
    if (path[0] != 0)
      {
	MiKTeX::Core::PathName tmp (path);
	path = "";
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

inline
void
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

inline
void
AssertValidString (/*[in]*/ const char *	lp,
		   /*[in]*/ size_t		n = 4096)
{
#if defined(_MSC_VER) && defined(MIKTEX_DEBUG)
  MIKTEX_ASSERT (lp != 0);
  MIKTEX_ASSERT (! IsBadStringPtrA(lp, n));
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
std::string &
GetErrnoMessage (/*[in]*/ int			err,
		 /*[out]*/ std::string &	message)
{
#if _MSC_VER >= 1400
  char szBuf[256];
  if (strerror_s(szBuf, 256, err) != 0)
    {
      message = "";
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
ClearString (/*[in,out]*/ char *	lpsz)
{
  lpsz[0] = 0;
  return (true);
}

/* _________________________________________________________________________

   StrCmp
   _________________________________________________________________________ */

#if defined(StrCmp)
#  undef StrCmp
#endif

inline
int
StrCmp (/*[in]*/ const char *	lpsz1,
	/*[in]*/ const char *	lpsz2)
{
  return (strcmp(lpsz1, lpsz2));
}

inline
int
StrCmp (/*[in]*/ const wchar_t *	lpsz1,
	/*[in]*/ const wchar_t *	lpsz2)
{
  return (wcscmp(lpsz1, lpsz2));
}

/* _________________________________________________________________________

   FPutS
   _________________________________________________________________________ */

inline
int
FPutS (/*[in]*/ const char *	lpsz,
       /*[in]*/ FILE *		stream)
{
  int n = fputs(lpsz, stream);
  if (n < 0)
    {
      FATAL_CRT_ERROR ("fputs", 0);
    }
  return (n);
}

/* _________________________________________________________________________

   FPutC
   _________________________________________________________________________ */

inline
int
FPutC (/*[in]*/ int	ch,
       /*[in]*/ FILE *	stream)
{
  int chWritten = fputc(ch, stream);
  if (chWritten == EOF)
    {
      FATAL_CRT_ERROR ("fputc", 0);
    }
  return (chWritten);
}

/* _________________________________________________________________________

   PathNameComparer
   _________________________________________________________________________ */

struct PathNameComparer
  : public std::binary_function<std::string,
				std::string,
				bool>
{
  bool
  operator() (/*[in]*/ const std::string & str1,
	      /*[in]*/ const std::string & str2)
    const
  {
    return (MiKTeX::Core::PathName::Compare(str1.c_str(), str2.c_str()) < 0);
  }
};

/* _________________________________________________________________________

   FileDigestTable
   _________________________________________________________________________ */

typedef std::map<std::string,
		 MiKTeX::Core::MD5,
		 PathNameComparer>
FileDigestTable;
  
/* _________________________________________________________________________

   PackageManagerImpl
   _________________________________________________________________________ */

class
PackageManagerImpl
  : public PackageManager,
    public MiKTeX::Core::ICreateFndbCallback,
    public IProgressNotify_
{
public:
  virtual
  MIKTEXTHISCALL
  ~PackageManagerImpl ();

public:
  virtual
  void
  MIKTEXTHISCALL
  AddRef ();

public:
  virtual
  void
  MIKTEXTHISCALL
  Release ();

public:
  virtual
  class PackageInstaller *
  MIKTEXTHISCALL
  CreateInstaller ();

public:
  virtual
  class PackageIterator *
  MIKTEXTHISCALL
  CreateIterator ();

public:
  virtual
  void
  MIKTEXTHISCALL
  CreateMpmFndb ();

public:
  virtual
  unsigned long
  MIKTEXTHISCALL
  GetFileRefCount (/*[in]*/ const MiKTeX::Core::PathName &	path);

public:
  virtual
  PackageInfo
  MIKTEXTHISCALL
  GetPackageInfo (/*[in]*/ const std::string & deploymentName);

public:
  virtual
  void
  MIKTEXTHISCALL
  LoadDatabase (/*[in]*/ const MiKTeX::Core::PathName & path);

public:
  virtual
  void
  MIKTEXTHISCALL
  UnloadDatabase ();

public:
  virtual
  void
  MIKTEXTHISCALL
  DownloadRepositoryList ();

public:
  virtual
  std::vector<RepositoryInfo>
  MIKTEXTHISCALL
  GetRepositories ()
  {
    return (repositories);
  }

public:
  virtual
  std::string
  MIKTEXTHISCALL
  PickRepositoryUrl ();

public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetPackageInfo (/*[in]*/ const std::string &	deploymentName,
		     /*[out]*/ PackageInfo &		packageInfo);

private:
  virtual
  bool
  MIKTEXTHISCALL
  ReadDirectory (/*[in]*/ const char *	lpszPath,
		 /*[out]*/ char * *	ppSubDirNames,
		 /*[out]*/ char * *	ppFileNames,
		 /*[out]*/ char * *	ppFileNameInfos);

private:
  virtual
  bool
  MIKTEXTHISCALL
  OnProgress (/*[in]*/ unsigned		level,
	      /*[in]*/ const char *	lpszDirectory);

public:
  virtual
  void
  OnProgress ();
  
public:
  virtual
  bool
  MIKTEXTHISCALL
  TryGetRepositoryInfo (/*[in]*/ const std::string &	url,
			/*[out]*/ RepositoryInfo &	repositoryInfo);

public:
  virtual
  RepositoryInfo
  MIKTEXTHISCALL
  VerifyPackageRepository (/*[in]*/ const std::string & url);

public:
  virtual
  bool
  MIKTEXTHISCALL
  TryVerifyInstalledPackage
  (/*[in]*/ const std::string & deploymentName);

public:
  virtual
  std::string
  MIKTEXTHISCALL
  GetContainerPath (/*[in]*/ const std::string &	deploymentName,
		    /*[in]*/ bool			useDisplayNames);

public:
  PackageManagerImpl ();

public:
  void
  ClearAll ();

public:
  void
  IncrementFileRefCounts
  (/*[in]*/ const std::string & deploymentName);

public:
  void
  NeedInstalledFileInfoTable ();

public:
  void
  FlushVariablePackageTable ();

public:
  void
  GetAllPackageDefinitions (/*[out]*/ std::vector<PackageInfo> & packages);

public:
  PackageInfo *
  TryGetPackageInfo (/*[in]*/ const std::string & deploymentName);

public:
  InstalledFileInfo *
  GetInstalledFileInfo (/*[in]*/ const char * lpszPath);

public:
  bool
  IsRemovable (/*[in]*/ const char * lpszDeploymentName);

public:
  time_t
  GetUserTimeInstalled (/*[in]*/ const char *	lpszDeploymentName);

public:
  time_t
  GetCommonTimeInstalled (/*[in]*/ const char *	lpszDeploymentName);

public:
  time_t
  GetTimeInstalled (/*[in]*/ const char *	lpszDeploymentName);

public:
  bool
  IsPackageInstalled (/*[in]*/ const char *	lpszDeploymentName);

public:
  bool
  IsPackageObsolete (/*[in]*/ const char *	lpszDeploymentName);

public:
  void
  DeclarePackageObsolete (/*[in]*/ const char *	lpszDeploymentName,
			  /*[in]*/ bool		obsolete);

public:
  void
  SetTimeInstalled (/*[in]*/ const char *	lpszDeploymentName,
		    /*[in]*/ time_t		timeInstalled);

public:
  PackageInfo *
  DefinePackage (/*[in]*/ const std::string &	deploymentName,
		 /*[in]*/ const PackageInfo &	packageinfo);

public:
  void
  TraceError (/*[in]*/ const char *	lpszFormat,
	      /*[in]*/			...);

private:
  void
  LoadVariablePackageTable ();

private:
  void
  IncrementFileRefCounts
  (/*[in]*/ const std::vector<std::string> & files);

private:
  void
  ParseAllPackageDefinitionFilesInDirectory
  (/*[in]*/ const MiKTeX::Core::PathName & directory);

private:
  void
  ParseAllPackageDefinitionFiles ();

private:
  bool
  TryVerifyInstalledPackageHelper
  (/*[in]*/ const MiKTeX::Core::PathName &  prefix,
   /*[in]*/ const std::string &		    fileName,
   /*[out]*/ bool &			    haveDigest,
   /*[out]*/ MiKTeX::Core::MD5 &	    digest);

private:
  void
  Dispose ();

public:
  static
  RepositoryType
  DetermineRepositoryType (/*[in]*/ const std::string & repository);

private:
  int refCount;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_error;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_mpm;

private:
#if defined(HAVE_UNORDERED_MAP)
  typedef std::tr1::unordered_map<std::string,
				  PackageInfo,
				  MiKTeX::Core::hash_icase,
				  MiKTeX::Core::equal_icase>
  PackageDefinitionTable;
#else
  typedef std::map<std::string,
		   PackageInfo,
		   MiKTeX::Core::less_icase>
  PackageDefinitionTable;
#endif

private:
  PackageDefinitionTable packageTable;

private:
#if defined(HAVE_UNORDERED_MAP)
  typedef std::tr1::unordered_map<std::string,
				  InstalledFileInfo,
				  MiKTeX::Core::hash_path,
				  MiKTeX::Core::equal_path>
  InstalledFileInfoTable;
#else
  typedef std::map<std::string,
		   InstalledFileInfo,
		   MiKTeX::Core::less_path>
  InstalledFileInfoTable;
#endif

private:
  InstalledFileInfoTable installedFileInfoTable;

private:
  bool parsedAllPackageDefinitionFiles;

private:
  MiKTeX::Core::SmartPointer<MiKTeX::Core::Cfg> commonVariablePackageTable;

private:
  MiKTeX::Core::SmartPointer<MiKTeX::Core::Cfg> userVariablePackageTable;

private:
  MiKTeX::Core::SessionWrapper pSession;

private:
  std::auto_ptr<WebSession> webSession;

public:
  WebSession *
  GetWebSession ()
    const
  {
    return (webSession.get());
  }

private:
  std::vector<RepositoryInfo> repositories;

public:
  static std::string proxyUser;

public:
  static std::string proxyPassword;

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
public:
  static bool localServer;
#endif
};

/* _________________________________________________________________________

   DbLight
   _________________________________________________________________________ */

class DbLight
{
private:
  MiKTeX::Core::SmartPointer<MiKTeX::Core::Cfg> pcfg;

public:
  DbLight ()
    : pcfg(MiKTeX::Core::Cfg::Create())
  {
  }

public:
  virtual
  ~DbLight ()
  {
  }

public:
  MiKTeX::Core::MD5
  GetDigest ()
  {
    return (pcfg->GetDigest());
  }

private:
  bool
  TryGetValue (/*[in]*/ const char *	lpszDeploymentName,
	    /*[in]*/ const char *	lpszValueName,
	    /*[out]*/ std::string &	value)
  {
    return (pcfg->TryGetValue(lpszDeploymentName, lpszValueName, value));
  }

public:
  void
  Read (/*[in]*/ const MiKTeX::Core::PathName & path)
  {
    pcfg->Read (path);
  }

public:
  char *
  FirstPackage (/*[out]*/ char *	lpszDeploymentName)
  {
    return (pcfg->FirstKey(lpszDeploymentName,
			   MiKTeX::Core::BufferSizes::MaxPath));
  }

public:
  char *
  NextPackage (/*[out]*/ char *	lpszDeploymentName)
  {
    return (pcfg->NextKey(lpszDeploymentName,
			  MiKTeX::Core::BufferSizes::MaxPath));
  }

public:
  void
  Clear ()
  {
    pcfg.Release ();
    pcfg = MiKTeX::Core::Cfg::Create();
  }

public:
  int
  GetArchiveFileSize (/*[in]*/ const char *	lpszDeploymentName)
  {
    std::string str;
    if (! TryGetValue(lpszDeploymentName, "CabSize", str))
      {
	FATAL_MPM_ERROR ("DbLight::GetArchiveFileSize",
			 T_("Unknown archive file size."),
			 lpszDeploymentName);
      }
    return (atoi(str.c_str()));
  }

public:
  MiKTeX::Core::MD5
  GetArchiveFileDigest (/*[in]*/ const char *	lpszDeploymentName)
  {
    std::string str;
    if (! pcfg->TryGetValue(lpszDeploymentName, "CabMD5", str))
      {
	FATAL_MPM_ERROR ("DbLight::GetArchiveFileDigest",
			 T_("Unknown archive file digest."),
			 lpszDeploymentName);
      }
    return (MiKTeX::Core::MD5::Parse(str.c_str()));
  }

public:
  MiKTeX::Core::MD5
  GetPackageDigest (/*[in]*/ const char *	lpszDeploymentName)
  {
    std::string str;
    if (! pcfg->TryGetValue(lpszDeploymentName, "MD5", str))
      {
	FATAL_MPM_ERROR ("DbLight::GetPackageDigest",
			 T_("Unknown package digest."),
			 lpszDeploymentName);
      }
    return (MiKTeX::Core::MD5::Parse(str.c_str()));
  }

public:
  time_t
  GetTimePackaged (/*[in]*/ const char *	lpszDeploymentName)
  {
    std::string str;
    if (! TryGetValue(lpszDeploymentName, "TimePackaged", str))
      {
	FATAL_MPM_ERROR ("DbLight::GetTimePackaged",
			 T_("Unknown package time-stamp."),
			 lpszDeploymentName);
      }
    unsigned time = static_cast<unsigned>(atoi(str.c_str()));
    if (time < Y2000)
      {
	FATAL_MPM_ERROR ("DbLight::GetTimePackaged",
			 T_("Invalid package time-stamp."),
			 lpszDeploymentName);
      }
    return (time);
  }

public:
  PackageLevel
  GetPackageLevel (/*[in]*/ const char *	lpszDeploymentName)
  {
    std::string str;
    if (! TryGetValue(lpszDeploymentName, "Level", str))
      {
	FATAL_MPM_ERROR ("DbLight::GetPackageLevel",
			 T_("Unknown package level."),
			 lpszDeploymentName);
      }
    return (CharToPackageLevel(str[0]));
  }

public:
  std::string
  GetPackageVersion (/*[in]*/ const char *	lpszDeploymentName)
  {
    std::string version;
    if (! TryGetValue(lpszDeploymentName, "Version", version))
      {
	version = "";
      }
    return (version);
  }

public:
  std::string
  GetPackageTargetSystem (/*[in]*/ const char *	lpszDeploymentName)
  {
    std::string targetSystem;
    if (! TryGetValue(lpszDeploymentName, "TargetSystem", targetSystem))
      {
	targetSystem = "";
      }
    return (targetSystem);
  }

public:
  MiKTeX::Extractor::ArchiveFileType
  GetArchiveFileType (/*[in]*/  const char *	lpszDeploymentName)
  {
    std::string str;
    if (! TryGetValue(lpszDeploymentName, "Type", str))
      {
	return (MiKTeX::Extractor::ArchiveFileType::MSCab);
      }
    if (str == "MSCab")
      {
	return (MiKTeX::Extractor::ArchiveFileType::MSCab);
      }
    else if (str == "TarBzip2")
      {
	return (MiKTeX::Extractor::ArchiveFileType::TarBzip2);
      }
    else if (str == "TarLzma")
      {
	return (MiKTeX::Extractor::ArchiveFileType::TarLzma);
      }
    else
      {
	FATAL_MPM_ERROR ("DbLight::GetArchiveFileType",
			 T_("Unknown archive file type."),
			 lpszDeploymentName);
      }
  }

public:
  static
  PackageLevel
  CharToPackageLevel (/*[in]*/ int ch)
  {
    switch (toupper(ch))
    {
    case 'S': return (PackageLevel::Essential);
    case 'M': return (PackageLevel::Basic);
    case 'L': return (PackageLevel::Advanced);
    case 'T': return (PackageLevel::Complete);
    default:
      FATAL_MPM_ERROR ("DbLight::CharToPackageLevel",
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
    public MiKTeX::Core::ICreateFndbCallback,
#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
    public MiKTeXPackageManagerLib::IPackageInstallerCallback,
#endif
    public MiKTeX::Extractor::IExtractCallback,
    public MiKTeX::Core::IRunProcessCallback
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
  MIKTEXTHISCALL
  ~PackageInstallerImpl ();

public:
  virtual
  void
  MIKTEXTHISCALL
  Dispose ();

public:
  virtual
  void
  MIKTEXTHISCALL
  SetRepository (/*[in]*/ const std::string & repository)
  {
    repositoryType =
      PackageManagerImpl::DetermineRepositoryType(repository);
    this->repository = repository;
  }

public:
  virtual
  void
  MIKTEXTHISCALL
  SetDownloadDirectory (/*[in]*/ const MiKTeX::Core::PathName & directory)
  {
    this->downloadDirectory = directory;
  }

public:
  virtual
  void
  MIKTEXTHISCALL
  UpdateDb ();
  
public:
  virtual
  void
  MIKTEXTHISCALL
  UpdateDbAsync ();
  
public:
  virtual
  void
  MIKTEXTHISCALL
  FindUpdates ();

public:
  virtual
  void
  MIKTEXTHISCALL
  FindUpdatesAsync ();

public:
  virtual
  std::vector<UpdateInfo>
  MIKTEXTHISCALL
  GetUpdates ()
  {
    return (updates);
  }

public:
  virtual
  void
  MIKTEXTHISCALL
  InstallRemove ();

public:
  virtual
  void
  MIKTEXTHISCALL
  InstallRemoveAsync ();

public:
  virtual
  void
  MIKTEXTHISCALL
  WaitForCompletion ();

public:
  virtual
  void
  MIKTEXTHISCALL
  Download ();

public:
  virtual
  void
  MIKTEXTHISCALL
  DownloadAsync ();

private:
  void
  StartTaskThread (void (MIKTEXCALLBACK * pFunc) (/*[in]*/ void * pData));

public:
  virtual
  ProgressInfo
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
  SetCallback (/*[in]*/ PackageInstallerCallback *	pCallback);
  
public:
  virtual
  void
  MIKTEXTHISCALL
  SetFileLists (/*[in]*/ const std::vector<std::string> & tbi,
		/*[in]*/ const std::vector<std::string> & tbr)
  {
    this->toBeInstalled = tbi;
    this->toBeRemoved = tbr;
  }

public:
  virtual
  void
  MIKTEXTHISCALL
  SetPackageLevel (/*[in]*/ PackageLevel packageLevel)
  {
    taskPackageLevel = packageLevel;
  }
  
public:
  virtual
  void
  MIKTEXTHISCALL
  SetFileList (/*[in]*/ const std::vector<std::string> & tbi)
  {
    SetFileLists (tbi, std::vector<std::string>());
  }

public:
  virtual
  void
  OnProgress ();
  
public:
  virtual
  void
  MIKTEXTHISCALL
  OnBeginFileExtraction (/*[in]*/ const char *	lpszFileName,
			 /*[in]*/ size_t	uncompressedSize);

public:
  virtual
  void
  MIKTEXTHISCALL
  OnEndFileExtraction (/*[in]*/ const char *	lpszFileName,
		       /*[in]*/ size_t		uncompressedSize);

public:
  virtual
  bool
  MIKTEXTHISCALL
  OnError (/*[in]*/ const char *	lpszMessage);

public:
  virtual
  bool
  MIKTEXTHISCALL
  OnProcessOutput (/*[in]*/ const void *	pOutput,
		   /*[in]*/ size_t		n);

private:
  void
  UpdateMpmFndb
    (/*[in]*/ const std::vector<std::string> &	installedFiles,
     /*[in]*/ const std::vector<std::string> &	removedFiles,
     /*[in]*/ const char *			lpszPackageName);
  
private:
  void
  CalculateExpenditure (/*[in]*/ bool downloadOnly = false);
  
private:
  MIKTEXNORETURN
  void
  FatalError (/*[in]*/ ErrorCode	error,
	      /*[in]*/			...);
  
private:
  MiKTeX::Core::Thread * pThread;
  
public:
  virtual
  bool
  MIKTEXTHISCALL
  IsRunning ()
    const
  {
    if (pThread == 0)
      {
	return (false);
      }
    return (! progressInfo.ready);
  }

public:
  virtual
  void
  MIKTEXTHISCALL
  RegisterComponents (/*[in]*/ bool doRegister);

public:
  virtual
  void
  MIKTEXTHISCALL
  SetNoPostProcessing (/*[in]*/ bool noPostProcessing)
  {
    this->noPostProcessing = noPostProcessing;
  }

public:
  virtual
  void
  MIKTEXTHISCALL
  SetNoLocalServer (/*[in]*/ bool noLocalServer)
  {
    this->noLocalServer = noLocalServer;
  }

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
public:
  STDMETHOD(QueryInterface) (/*[in]*/ REFIID	riid,
			     /*[out]*/ LPVOID *	ppvObj);

public:
  STDMETHOD_(ULONG, AddRef) ();

public:
  STDMETHOD_(ULONG, Release) ();

public:
  virtual
  HRESULT
  __stdcall
  ReportLine (/*[in]*/ BSTR line);

public:
  virtual
  HRESULT
  __stdcall
  OnRetryableError (/*[in]*/ BSTR			message,
		    /*[out,retval]*/ VARIANT_BOOL *	pDoContinue);

public:
  virtual
  HRESULT
  __stdcall
  OnProgress (/*[in]*/ LONG			nf,
	      /*[out,retval]*/ VARIANT_BOOL *	pDoContinue);
#endif

private:
  std::string repository;
  
private:
  MiKTeX::Core::PathName downloadDirectory;
  
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
  ReportLine (/*[in]*/ const char *	lpsz,
	      /*[in]*/			...);

private:
  std::string
  MakeUrl (/*[in]*/ const char * lpszBase,
	   /*[in]*/ const char * lpszRel);

private:
  std::string
  MakeUrl (/*[in]*/ const char * lpszRel);

private:
  bool
  AbortOrRetry (/*[in]*/ const char * lpszMessage)
  {
    return (pCallback == 0 || ! pCallback->OnRetryableError(lpszMessage));
  }
  
private:
  void
  Notify (/*[in]*/ Notification	      nf = Notification::None)
  {
    if (pCallback != 0 && ! pCallback->OnProgress(nf))
      {
	trace_mpm->WriteLine ("libmpm", T_("client wants to cancel"));
	trace_mpm->WriteLine ("libmpm",
			      T_("throwing OperationCancelledException"));
	throw MiKTeX::Core::OperationCancelledException ();
      }
  }

#if defined(MIKTEX_WINDOWS)
private:
  void
  RegisterComponent
  (/*[in]*/ bool				doRegister,
   /*[in]*/ const MiKTeX::Core::PathName &	path,
   /*[in]*/ bool				mustSucceed);
#endif

private:
  void
  RegisterComponents
  (/*[in]*/ bool doRegister,
   /*[in]*/ const std::vector<std::string> & packages);

private:
  void
  RegisterComponents
  (/*[in]*/ bool doRegister,
   /*[in]*/ const std::vector<std::string> & packages,
   /*[in]*/ const std::vector<std::string> & packages2)
  {
    RegisterComponents (doRegister, packages);
    RegisterComponents (doRegister, packages2);
  }

private:
  void
  RunIniTeXMF (/*[in]*/ const char *	lpszArguments);

private:
  bool autoFndbSync;

private:
  void
  SetAutoFndbSync (/*[in]*/ bool autoFndbSync)
  {
    if (this->autoFndbSync == autoFndbSync)
      {
	return;
      }
    trace_mpm->WriteFormattedLine
      ("libmpm",
       T_("turning %s autoFndbSync"),
       (autoFndbSync ? "on" : "off"));
    this->autoFndbSync = autoFndbSync;
  }

private:
  bool noPostProcessing;
  
private:
  bool noLocalServer;
  
private:
  ProgressInfo progressInfo;

private:
  MiKTeX::Core::SessionWrapper pSession;

private:
  MiKTeX::Core::MiKTeXException threadMiKTeXException;
  
private:
  clock_t timeStarted;
  
private:
  std::vector<std::string> toBeInstalled;
  
private:
  std::vector<std::string> toBeRemoved;
  
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
  SetUpPackageDefinitionFiles
    (/*[in]*/ const MiKTeX::Core::PathName & directory);

private:
  void
  HandleObsoletePackageDefinitionFiles
  (/*[in]*/ const MiKTeX::Core::PathName & temporaryDirectory);

private:
  void
  CleanUpUserDatabase ();
  
private:
  void
  Download (/*[in]*/ const std::string &		url,
	    /*[in]*/ const MiKTeX::Core::PathName &	dest,
	    /*[in]*/ size_t				expectedSize = 0);
  
private:
  void
  Download (/*[in]*/ const MiKTeX::Core::PathName &	fileName,
	    /*[in]*/ size_t				expectedSize = 0);
  
private:
  void
  RemoveFiles (/*[in]*/ const std::vector<std::string> & toBeRemoved,
	       /*[in]*/ bool				silently = false);
  
private:
  void
  ExtractFiles (/*[in]*/ const MiKTeX::Core::PathName &	archiveFileName,
		/*[in]*/ MiKTeX::Extractor::ArchiveFileType archiveFileType);

private:
  void
  CopyFiles (/*[in]*/ const MiKTeX::Core::PathName & pathSourceRoot,
	     /*[in]*/ const std::vector<std::string> & fileList);

private:
  void
  AddToFileList (/*[in,out]*/ std::vector<std::string> &	fileList,
		 /*[in]*/ const MiKTeX::Core::PathName &	fileName)
    const;

private:
  void
  RemoveFromFileList
    (/*[in,out]*/ std::vector<std::string> &	fileList,
     /*[in]*/ const MiKTeX::Core::PathName &	fileName)
    const;

private:
  void
  CopyPackage (/*[in]*/ const MiKTeX::Core::PathName &	pathSourceRoot,
	       /*[in]*/ const std::string &		deploymentName);
  
private:
  virtual
  bool
  MIKTEXTHISCALL
  ReadDirectory (/*[in]*/ const char *	lpszPath,
		 /*[out]*/ char * *	ppSubDirNames,
		 /*[out]*/ char * *	ppFileNames,
		 /*[out]*/ char * *	ppFileNameInfos);

private:
  virtual
  bool
  MIKTEXTHISCALL
  OnProgress (/*[in]*/ unsigned		level,
	      /*[in]*/ const char *	lpszDirectory);

private:
  void
  RemovePackage (/*[in]*/ const std::string &	deploymentName);

private:
  void
  InstallPackage (/*[in]*/ const std::string & deploymentName);

private:
  void
  MyCopyFile (/*[in]*/ const MiKTeX::Core::PathName &	source,
	      /*[in]*/ const MiKTeX::Core::PathName &	dest,
	      /*[out]*/ size_t &			size);
  
private:
  void
  DownloadPackage (/*[in]*/ const std::string & deploymentName);
  
private:
  bool
  CheckArchiveFile (/*[in]*/ const char *		lpszPackage,
		    /*[in]*/ const MiKTeX::Core::PathName & archiveFileName,
		    /*[in]*/ bool			mustBeOk);

private:
  void
  CheckDependencies (/*[in,out]*/ std::set<std::string> &	packages,
		     /*[in]*/ const std::string &	deploymentName,
		     /*[in]*/ bool			force,
		     /*[in]*/ int			level);

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

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_error;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_mpm;

private:
  MiKTeX::Core::SmartPointer<PackageManagerImpl> pManager;

private:
  PackageInstallerCallback * pCallback;

private:
  std::vector<UpdateInfo> updates;

#if defined(MIKTEX_WINDOWS)
private:
  int numCoInitialize;
#endif

#if defined(MIKTEX_WINDOWS) && USE_LOCAL_SERVER
private:
  struct
  {
    CComQIPtr<MiKTeXPackageManagerLib::IPackageManager> pManager;
    CComPtr<MiKTeXPackageManagerLib::IPackageInstaller> pInstaller;
  } localServer;
#endif
};

/* _________________________________________________________________________

   PackageIteratorImpl
   _________________________________________________________________________ */

class PackageIteratorImpl : public PackageIterator
{
public:
  virtual
  MIKTEXTHISCALL
  ~PackageIteratorImpl ();

public:
  virtual
  void
  MIKTEXTHISCALL
  Dispose ();
  
public:
  virtual
  void
  MIKTEXTHISCALL
  AddFilter (/*[in]*/ unsigned int	filter,
	     /*[in]*/ const char *	lpsz = 0)
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
  MIKTEXTHISCALL
  GetNext (/*[out]*/ PackageInfo & packageinfo);
  
public:
  PackageIteratorImpl (/*[in]*/ PackageManagerImpl * pManager);

private:
  MiKTeX::Core::SmartPointer<PackageManagerImpl> pManager;
  
private:
  std::vector<PackageInfo> snapshot;
  
private:
  std::vector<PackageInfo>::const_iterator iter;
  
private:
  std::string requiredBy;
  
private:
  unsigned long filter;
};

/* _________________________________________________________________________ */

END_INTERNAL_NAMESPACE;

using namespace MiKTeX::Packages::D6AAD62216146D44B580E92711724B78;
