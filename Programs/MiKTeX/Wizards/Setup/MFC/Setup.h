/* Setup.h:							-*- C++ -*-

   Copyright (C) 1999-2014 Christian Schenk

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


#include "config.h"

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) MiKTeX::Core::Quoter<char>(x).Get()

#if ! defined(UNICODE)
#  error UNICODE required
#endif

#define TU_(x) MiKTeX::Core::CharBuffer<char>(x).GetBuffer()
#define UT_(x) MiKTeX::Core::CharBuffer<wchar_t>(x).GetBuffer()

#define UW_(x) MiKTeX::Core::Utils::UTF8ToWideChar(x).c_str()
#define WU_(x) MiKTeX::Core::Utils::WideCharToUTF8(x).c_str()

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
	if (! CreateDirectory(UT_(path.Get()), 0))
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
  auto_ptr<TraceStream> traceStream;

#if ENABLE_ADDTEXMF
public:
  vector<PathName> addTEXMFDirs;
#endif

public:
  PackageManagerPtr pManager;

public:
  SetupServicePtr pSetupService;

public:
  PathName GetInstallRoot () const
  {
    SetupOptions options = pSetupService->GetOptions();
    return (options.IsCommonSetup ? options.Config.commonInstallRoot : options.Config.userInstallRoot);
  }

public:
  void SetInstallRoot (/*[in]*/ const PathName & path)
  {
    SetupOptions options = pSetupService->GetOptions();
    if (options.IsCommonSetup)
    {
      options.Config.commonInstallRoot = path;
    }
    else
    {
      options.Config.userInstallRoot = path;
    }
    pSetupService->SetOptions(options);
  }

public:
  SetupTask GetSetupTask()
  {
    return pSetupService->GetOptions().Task;
  }

public:
  bool IsCommonSetup()
  {
    return pSetupService->GetOptions().IsCommonSetup;
  }

public:
  bool IsPortable()
  {
    return pSetupService->GetOptions().IsPortable;
  }

public:
  bool IsDryRun()
  {
    return pSetupService->GetOptions().IsDryRun;
  }

public:
  PathName GetLocalPackageRepository()
  {
    return pSetupService->GetOptions().LocalPackageRepository;
  }

public:
  string GetRemotePackageRepository()
  {
    return pSetupService->GetOptions().RemotePackageRepository;
  }

public:
  PackageLevel GetPackageLevel()
  {
     return pSetupService->GetOptions().PackageLevel;
  }

public:
  PathName GetFolderName()
  {
    return pSetupService->GetOptions().FolderName;
  }

public:
  StartupConfig GetStartupConfig()
  {
    return pSetupService->GetOptions().Config;
  }

#if ENABLE_ADDTEXMF
public:
  bool noAddTEXMFDirs;
#endif

public:
  bool allowUnattendedReboot;

public:
  bool isMiKTeXDirect;

public:
  PackageLevel prefabricatedPackageLevel;

public:
  bool mustReboot;

public:
  bool showLogFileOnExit;

public:
  bool unattended;
};

extern SetupWizardApplication theApp;

/* _________________________________________________________________________

   Protos
   _________________________________________________________________________ */

#if ENABLE_ADDTEXMF
void
CheckAddTEXMFDirs (/*[out,out]*/ string &	directories,
		   /*[out]*/ vector<PathName> &	vec);
#endif

int
ComparePaths (/*[in]*/ const PathName & path1,
	      /*[in]*/ const PathName & path2,
	      /*[in]*/ bool shortify = false);

void
DDV_Path (/*[in]*/ CDataExchange *	pDX,
	  /*[in]*/ const CString &	str);

bool
FindFile (/*[in]*/ const PathName &	fileName,
	  /*[out]*/ PathName &		result);

bool
FindDataDir (/*[out]*/ CString & strDir);

void
TraceWindowsError (/*[in]*/ const char *	lpszWindowsFunction,
		   /*[in]*/ unsigned long	functionResult,
		   /*[in]*/ const char *	lpszInfo,
		   /*[in]*/ const char *	lpszSourceFile,
		   /*[in]*/ int			lpszSourceLine);

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

#define STR_BYT_SIZ(lpsz) ((StrLen(lpsz) + 1) * sizeof(*lpsz))
