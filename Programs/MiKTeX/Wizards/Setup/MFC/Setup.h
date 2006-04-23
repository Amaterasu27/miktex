/* Setup.h:							-*- C++ -*-

   Copyright (C) 1999-2006 Christian Schenk

   This file is part of the MiKTeX Setup Wizard.

   The MiKTeX Setup Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX Setup Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX Setup Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#pragma once

#include "resource.h"

#define LICENSE_FILE T_("LICENSE.TXT")
#define DOWNLOAD_INFO_FILE T_("README.TXT")

#define BASIC_MIKTEX T_("\"Basic MiKTeX\"")
#define BASIC_MIKTEX_LEGACY T_("\"Small MiKTeX\"")
#define COMPLETE_MIKTEX T_("\"Complete MiKTeX\"")
#define COMPLETE_MIKTEX_LEGACY T_("\"Total MiKTeX\"")
#define ESSENTIAL_MIKTEX T_("\"Essential MiKTeX\"")

#define ENABLE_ADDTEXMF 0
#define SHOW_FOLDER_PAGE 0

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) Quoted(x).c_str()

#if defined(MIKTEX_UNICODE)
#  define tcout wcout
#  define tcerr wcerr
#else
#  define tcout cout
#  define tcerr cerr
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

#define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,		\
			      lpszInfo,			\
			      T_(__FILE__),		\
			      __LINE__)

#define FATAL_WINDOWS_ERROR_2(windowsfunction, errorCode, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,				\
			      errorCode,				\
			      lpszInfo,					\
			      T_(__FILE__),				\
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

   StrChr
   _________________________________________________________________________ */

inline
MIKTEXCHAR *
StrChr (/*[in]*/ MIKTEXCHAR *		lpsz,
	/*[in]*/ MIKTEXCHARINT		ch)
{
#if defined(MIKTEX_UNICODE)
  return (wcschr(lpsz, ch));
#else
  return (strchr(lpsz, ch));
#endif
}

/* _________________________________________________________________________

   StrChr
   _________________________________________________________________________ */

inline
const MIKTEXCHAR *
StrChr (/*[in]*/ const MIKTEXCHAR *	lpsz,
	/*[in]*/ MIKTEXCHARINT		ch)
{
#if defined(MIKTEX_UNICODE)
  return (wcschr(lpsz, ch));
#else
  return (strchr(lpsz, ch));
#endif
}

#if defined(StrStr)
#  undef StrStr
#endif

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

inline
tstring
Quoted (/*[in]*/ const MIKTEXCHAR * lpsz)
{
  bool needQuotes = (StrChr(lpsz, T_(' ')) != 0);
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

   AutoBuffer
   _________________________________________________________________________ */

class AutoBuffer
{
protected:
  enum { BUFSIZE = 512 };

protected:
  MIKTEXCHAR smallBuffer[BUFSIZE];

protected:
  MIKTEXCHAR * buffer;

protected:
  size_t n;

public:
  AutoBuffer ()
    : buffer (smallBuffer),
      n (BUFSIZE)
  {
  }

public:
  AutoBuffer (/*[in]*/ size_t n)
    : n (n)
  {
    if (n <= BUFSIZE)
      {
	buffer = smallBuffer;
      }
    else
      {
	buffer = new MIKTEXCHAR[n];
      }
  }

public:
  ~AutoBuffer ()
  {
    try
      {
	if (buffer != smallBuffer)
	  {
	    delete [] buffer;
	  }
	buffer = 0;
	n = 0;
      }
    catch (const MiKTeXException &)
      {
      }
  }

public:
  void
  Resize (/*[in]*/ size_t newSize)
  {
    if (newSize > BUFSIZE && newSize > n)
      {
	char * newBuffer = new MIKTEXCHAR[newSize];
	memcpy (newBuffer, buffer, n);
	if (buffer != smallBuffer)
	  {
	    delete [] buffer;
	  }
	buffer = newBuffer;
	n = newSize;
      }
  }

public:
  MIKTEXCHAR *
  GetBuffer ()
    const
  {
    return (buffer);
  }

public:
  const MIKTEXCHAR *
  Get ()
    const
  {
    return (buffer);
  }

public:
  size_t
  GetSize ()
    const
  {
    return (n);
  }
};

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

   AutoHANDLE
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
template<>
inline
HANDLE
InvalidHandleValue<HANDLE> ()
{
  return (INVALID_HANDLE_VALUE);
}

class CloseHandle_
{
public:
  void
  operator() (/*[in]*/ HANDLE h)
  {
    CloseHandle (h);
  }
};

typedef AutoResource<HANDLE, CloseHandle_> AutoHANDLE;
#endif

/* _________________________________________________________________________

   AutoHKEY
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class RegCloseKey_
{
public:
  void
  operator() (/*[in]*/ HKEY hkey)
  {
    long result = RegCloseKey(hkey);
    if (result != ERROR_SUCCESS)
      {
#if 0
	TraceWindowsError (T_("RegCloseKey"),
			   result,
			   0,
			   T_(__FILE__),
			   __LINE__);
#endif
      }
  }
};

typedef AutoResource<HKEY, RegCloseKey_> AutoHKEY;
#endif

/* _________________________________________________________________________

   SetupTask
   _________________________________________________________________________ */

class SetupTaskEnum
{
public:
  enum EnumType {
    None,
    Download,
    InstallFromCD,
    InstallFromLocalRepository,
    InstallFromRemoteRepository, // <todo/>
    PrepareMiKTeXDirect,
  };
};

typedef EnumWrapper<SetupTaskEnum> SetupTask;

/* _________________________________________________________________________

   SetupWizardApplication
   _________________________________________________________________________ */

class SetupWizardApplication : public CWinApp
{
public:
  SetupWizardApplication ();
  
public:
  virtual
  BOOL
  InitInstance ();
  
public:
  DECLARE_MESSAGE_MAP();

public:
  CCriticalSection criticalSectionMonitorLogStream;

public:
  StreamWriter logStream;

public:
  auto_ptr<TraceStream> traceStream;

public:
  PathName intermediateLogFile;

public:
  vector<PathName> addTEXMFDirs;

public:
  PackageLevel packageLevel;

public:
  PackageManagerPtr pManager;

public:
  tstring folderName;

public:
  tstring paperSize;

public:
  StartupConfig startupConfig;

public:
  PathName localPackageRepository;

public:
  PathName MiKTeXDirectTeXMFRoot;

public:
  PathName MiKTeXDirectRoot;

public:
  tstring remotePackageRepository;

public:
  PathName setupPath;

public:
  SetupTask setupTask;

public:
  bool noAddTEXMFDirs;

public:
  bool allowUnattendedReboot;

public:
  bool commonUserSetup;

public:
  bool dryRun;

public:
  bool isMiKTeXDirect;

public:
  bool prefabricated;

public:
  bool mustReboot;

public:
  bool registerPath;

public:
  bool showLogFileOnExit;

public:
  bool unattended;

public:
  StreamWriter uninstStream;
};

extern SetupWizardApplication theApp;

/* _________________________________________________________________________

   Protos
   _________________________________________________________________________ */

void
CheckAddTEXMFDirs (/*[out,out]*/ tstring &	directories,
		   /*[out]*/ vector<PathName> &	vec);

int
ComparePaths (/*[in]*/ const PathName & path1,
	      /*[in]*/ const PathName & path2,
	      /*[in]*/ bool shortify = false);

void
CreateProgramIcons ();

void
DDV_Path (/*[in]*/ CDataExchange *	pDX,
	  /*[in]*/ const CString &	str);

VersionNumber
GetFileVersion (/*[in]*/ const PathName &	path);

PathName
GetLogFileName ();

bool
FindFile (/*[in]*/ const PathName &	fileName,
	  /*[out]*/ PathName &		result);

bool
FindDataDir (/*[out]*/ CString & strDir);

bool
IsPathRegistered ();

void
Log (/*[in]*/ const MIKTEXCHAR *	lpsz,
     /*[in]*/				...);

void
TraceWindowsError (/*[in]*/ const MIKTEXCHAR *	lpszWindowsFunction,
		   /*[in]*/ unsigned long	functionResult,
		   /*[in]*/ const MIKTEXCHAR *	lpszInfo,
		   /*[in]*/ const MIKTEXCHAR *	lpszSourceFile,
		   /*[in]*/ int			lpszSourceLine);

void
LogHeader ();

void LogV
(/*[in]*/ const MIKTEXCHAR *	lpsz,
 /*[in]*/ va_list		args);

void
RegisterMiKTeXFileTypes ();

void
RegisterPath95 ();

void
RegisterPathNT ();

bool
TestLocalRepository (/*[in]*/ const PathName &	pathRepository,
		     /*[out]*/ PackageLevel &	packageLevel);

void
ULogAddFile (/*[in]*/ const PathName & path);

void
ULogAddRegValue (/*[in]*/ HKEY hkey,
		 /*[in]*/ LPCSTR pszSubKey,
		 /*[in]*/ LPCSTR pszValueName);

void
ULogClose (/*[in]*/ bool bRegister = false);

void
ULogOpen ();

void
ReportError (/*[in]*/ const MiKTeXException & e);

void
ReportError (/*[in]*/ const exception & e);

void
SplitUrl (/*[in]*/ const tstring &	url,
	  /*[out]*/ tstring &		protocol,
	  /*[out]*/ tstring &		host);

/* _________________________________________________________________________

   IsWindowsNT
   _________________________________________________________________________ */

inline
bool
IsWindowsNT ()
{
  return (GetVersion() < 0x80000000 ? true : false);
}

/* _________________________________________________________________________

   STR_BYT_SIZ
   _________________________________________________________________________ */

#define STR_BYT_SIZ(lpsz) ((StrLen(lpsz) + 1) * sizeof(MIKTEXCHAR))
