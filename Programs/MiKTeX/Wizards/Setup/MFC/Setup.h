/* Setup.h:							-*- C++ -*-

   Copyright (C) 1999-2009 Christian Schenk

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

#define LICENSE_FILE "LICENSE.TXT"
#define DOWNLOAD_INFO_FILE "README.TXT"

#define BASIC_MIKTEX "\"Basic MiKTeX\""
#define BASIC_MIKTEX_LEGACY "\"Small MiKTeX\""
#define COMPLETE_MIKTEX "\"Complete MiKTeX\""
#define COMPLETE_MIKTEX_LEGACY "\"Total MiKTeX\""
#define ESSENTIAL_MIKTEX "\"Essential MiKTeX\""

#define ENABLE_ADDTEXMF 0
#define SHOW_FOLDER_PAGE 0
#define FEATURE_1874934 1

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) MiKTeX::Core::Quoter<char>(x).Get()

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

#define INVALID_ARGUMENT(function, param)				\
  FATAL_MIKTEX_ERROR (function, T_("Invalid argument."), param)

#define UNEXPECTED_CONDITION(function)				\
  FATAL_MIKTEX_ERROR (function, T_("Unexpected condition."), 0)

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  Session::FatalMiKTeXError (miktexFunction,				\
			     traceMessage,				\
			     lpszInfo,					\
			     __FILE__,				\
			     __LINE__)

#define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,		\
			      lpszInfo,			\
			      __FILE__,		\
			      __LINE__)

#define FATAL_WINDOWS_ERROR_2(windowsfunction, errorCode, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,				\
			      errorCode,				\
			      lpszInfo,					\
			      __FILE__,				\
			      __LINE__)

#define CHECK_WINDOWS_ERROR(windowsFunction, lpszInfo)	\
  if (::GetLastError() != ERROR_SUCCESS)		\
    {							\
      FATAL_WINDOWS_ERROR (windowsFunction, lpszInfo);	\
    }

#define UNSUPPORTED_PLATFORM()						\
  __assume(false)

/* _________________________________________________________________________

   TempDirectory
   _________________________________________________________________________ */

class TempDirectory
{
public:
  TempDirectory ()
  {
    path.SetToTempDirectory ();
    path += T_("MiKTeX Setup");
    if (! Directory::Exists(path))
      {
	if (! CreateDirectory(path.Get(), 0))
	  {
	    FATAL_WINDOWS_ERROR ("CreateDirectory", path.Get());
	  }
      }
  }

public:
  ~TempDirectory ()
  {
    try
      {
	if (Directory::Exists(path))
	  {
	    Delete ();
	  }
      }
    catch (const exception &)
      {
      }
  }

public:
  const PathName &
  Get ()
    const
  {
    return (path);
  }

public:
  void
  Delete ()
  {
    Directory::Delete (path, true);
  }

private:
  PathName path;
};

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
  string folderName;

public:
  string paperSize;

public:
  TriState installOnTheFly;

public:
  StartupConfig startupConfig;

public:
  PathName GetInstallRoot () const
  {
    return (commonUserSetup
      ? startupConfig.commonInstallRoot
      : startupConfig.userInstallRoot);
  }

public:
  void SetInstallRoot (/*[in]*/ const PathName & path)
  {
    if (commonUserSetup)
    {
      startupConfig.commonInstallRoot = path;
    }
    else
    {
      startupConfig.userInstallRoot = path;
    }
  }

public:
  PathName localPackageRepository;

public:
  PathName MiKTeXDirectTeXMFRoot;

public:
  PathName MiKTeXDirectRoot;

public:
  string remotePackageRepository;

public:
  PathName setupPath;

public:
  SetupTask setupTask;

public:
  bool noAddTEXMFDirs;

public:
  bool noRegistry;

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
  PackageLevel prefabricatedPackageLevel;

public:
  bool mustReboot;

public:
  bool registerPath;

public:
  bool showLogFileOnExit;

public:
  bool unattended;

public:
  bool portable;

public:
  StreamWriter uninstStream;
};

extern SetupWizardApplication theApp;

/* _________________________________________________________________________

   Protos
   _________________________________________________________________________ */

void
CheckAddTEXMFDirs (/*[out,out]*/ string &	directories,
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
Log (/*[in]*/ const char *	lpsz,
     /*[in]*/				...);

void
TraceWindowsError (/*[in]*/ const char *	lpszWindowsFunction,
		   /*[in]*/ unsigned long	functionResult,
		   /*[in]*/ const char *	lpszInfo,
		   /*[in]*/ const char *	lpszSourceFile,
		   /*[in]*/ int			lpszSourceLine);

void
LogHeader ();

void LogV
(/*[in]*/ const char *	lpsz,
 /*[in]*/ va_list		args);

#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
void
RegisterPath95 ();
#endif

void
RegisterPathNT ();

PackageLevel
TestLocalRepository (/*[in]*/ const PathName &		pathRepository,
		     /*[in]*/ PackageLevel		requestedPackageLevel);

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
SplitUrl (/*[in]*/ const string &	url,
	  /*[out]*/ string &		protocol,
	  /*[out]*/ string &		host);

/* _________________________________________________________________________

   STR_BYT_SIZ
   _________________________________________________________________________ */

#define STR_BYT_SIZ(lpsz) ((StrLen(lpsz) + 1) * sizeof(char))
