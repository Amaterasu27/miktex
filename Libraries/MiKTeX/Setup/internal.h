/* internal.h: internal definitions				-*- C++ -*-

   Copyright (C) 2013-2014 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.
   
   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER) && ! defined(MIKTEX_STATIC)
#  define MIKTEXSETUPEXPORT __declspec(dllexport)
#else
#  define MIKTEXSETUPEXPORT
#endif

#define C09CDC45E4B649EDA745DAF436D18309
#include "miktex/Setup/SetupService"

#define BEGIN_INTERNAL_NAMESPACE			\
namespace MiKTeX {					\
  namespace Setup {					\
    namespace B3CB81AE1C634DFBB208D80FA1A264AE {

#define END_INTERNAL_NAMESPACE			\
    }						\
  }						\
}

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

BEGIN_INTERNAL_NAMESPACE;

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

#define FATAL_SETUP_ERROR(miktexFunction, traceMessage, lpszInfo) \
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

#define TU_(x) MiKTeX::Core::CharBuffer<char>(x).GetBuffer()
#define UT_(x) MiKTeX::Core::CharBuffer<wchar_t>(x).GetBuffer()

#define UW_(x) MiKTeX::Core::Utils::UTF8ToWideChar(x).c_str()
#define WU_(x) MiKTeX::Core::Utils::WideCharToUTF8(x).c_str()

#define SETUPSTATICFUNC(type) static type
#define SETUPINTERNALFUNC(type) type
#define SETUPINTERNALVAR(type) type

#define ARRAY_SIZE(buf) (sizeof(buf)/sizeof(buf[0]))

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

   ShellLinkData
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
struct ShellLinkData  
{
  bool isUrl;
  const char * lpszFolder;
  const char * lpszName;
  const char * lpszPathName;
  unsigned long flags;
  const char * lpszDescription;
  const char * lpszArgs;
  const char * lpszIconPath;
  int iconIndex;
  const char * lpszWorkingDir;
  int showCmd;
  WORD hotKey;
};
#endif

/* _________________________________________________________________________

   SetupServiceImpl
   _________________________________________________________________________ */

class
SetupServiceImpl : public SetupService, public MiKTeX::Core::IRunProcessCallback, public MiKTeX::Packages::PackageInstallerCallback
{
public:
  virtual MIKTEXTHISCALL ~SetupServiceImpl();

public:
  virtual void MIKTEXTHISCALL AddRef();

public:
  virtual void MIKTEXTHISCALL Release();

public:
  virtual SetupOptions MIKTEXTHISCALL GetOptions()
  {
    return this->options;
  }

public:
  virtual SetupOptions MIKTEXTHISCALL SetOptions(const SetupOptions & options);

public:
  virtual void MIKTEXTHISCALL OpenLog();

public:
  virtual MiKTeX::Core::PathName MIKTEXTHISCALL CloseLog(bool cancel);

public:
  virtual void MIKTEXCEECALL Log(const char * lpszFormat, ...);

public:
  virtual void MIKTEXTHISCALL LogV(const char * lpszFormat, va_list argList);

public:
  virtual void ULogOpen();

public:
  virtual void ULogClose(bool finalize);

public:
  virtual MiKTeX::Core::PathName GetULogFileName();

public:
  virtual void ULogAddFile(const MiKTeX::Core::PathName & path);

#if defined(MIKTEX_WINDOWS)
public:
  virtual void ULogAddRegValue(HKEY hkey, const char * lpszSubKey, const char * lpszValueName);
#endif

public:
  virtual ProgressInfo GetProgressInfo();

public:
  virtual void SetCallback(/*[in]*/ SetupServiceCallback * pCallback);

public:
  virtual void Run();

#if defined(MIKTEX_WINDOWS)
public:
  virtual void CreateProgramIcons();
#endif

public:
  SetupServiceImpl();

private:
  void Initialize();

private:
  void DoTheDownload();

private:
  void DoPrepareMiKTeXDirect();

private:
  void DoTheInstallation();

private:
  virtual bool OnProcessOutput(const void * pOutput, size_t n);

private:
  virtual void ReportLine(const char * lpszLine);

private:
  virtual bool OnRetryableError(const char * lpszMessage);

private:
  virtual bool OnProgress(MiKTeX::Packages::Notification nf);

private:
  int refCount;

private:
  SetupOptions options;

private:
  enum Section { None, Files, HKLM, HKCU };

private:
  Section section;

private:
  MiKTeX::Core::StreamWriter logStream;

private:
  MIKTEX_DEFINE_LOCK(logStream);

private:
  MiKTeX::Core::PathName intermediateLogFile;

private:
  MiKTeX::Core::StreamWriter uninstStream;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> traceStream;

private:
  MiKTeX::Core::PathName GetInstallRoot() const
  {
    return (options.IsCommonSetup
      ? options.Config.commonInstallRoot
      : options.Config.userInstallRoot);
  }

#if defined(MIKTEX_WINDOWS)
private:
  void RegisterUninstaller();
#endif

#if defined(MIKTEX_WINDOWS)
private:
  void AddUninstallerRegValue(HKEY hkey, const char * lpszValueName, const char * lpszValue);
#endif

#if defined(MIKTEX_WINDOWS)
private:
  void AddUninstallerRegValue(HKEY hkey, const char * lpszValueName, DWORD value);
#endif

#if defined(MIKTEX_WINDOWS)
private:
  MiKTeX::Core::PathName SetupServiceImpl::CreateProgramFolder ();
#endif

#if defined(MIKTEX_WINDOWS)
private:
  void CreateShellLink(const MiKTeX::Core::PathName & pathFolder, const ShellLinkData & ld);
#endif

#if defined(MIKTEX_WINDOWS)
private:
  void CreateInternetShortcut(const MiKTeX::Core::PathName & path, const char * lpszUrl);
#endif

private:
  void ConfigureMiKTeX();

private:
  void RunIniTeXMF(const MiKTeX::Core::CommandLineBuilder & cmdLine1);

private:
  void RunMpm(const MiKTeX::Core::CommandLineBuilder & cmdLine1);

private:
  std::wstring & Expand(const char * lpszSource, std::wstring & dest);

private:
  bool FindFile(const MiKTeX::Core::PathName & fileName, MiKTeX::Core::PathName & result);

private:
  void CreateInfoFile();

private:
  void SetupServiceImpl::LogHeader();

private:
  bool logging;

private:
  bool cancelled;

private:
  MiKTeX::Packages::PackageManagerPtr pManager;

private:
  std::auto_ptr<MiKTeX::Packages::PackageInstaller> pInstaller;

private:
  SetupServiceCallback * pCallback;
};

/* _________________________________________________________________________ */

END_INTERNAL_NAMESPACE;

using namespace MiKTeX::Setup::B3CB81AE1C634DFBB208D80FA1A264AE;
