/* initexmf.cpp: MiKTeX configuration utility

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of IniTeXMF.

   IniTeXMF is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   IniTeXMF is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with IniTeXMF; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#if defined(HAVE_CONFIG_H)
#  include <config.h>
#endif

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <miktex/core.h>
#include <miktex/paths.h>
#include <miktex/reg.h>
#include <popt-miktex.h>
#include <miktex/mpm.h>

#include "initexmf-version.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace std;

#define UNUSED_ALWAYS(x) (x)

#define T_(x) MIKTEXTEXT(x)

#if defined(MIKTEX_UNICODE)
#  define tcout wcout
#  define tcerr wcerr
#else
#  define tcout cout
#  define tcerr cerr
#endif

#define Q_(x) Quoted(x).c_str()

const MIKTEXCHAR * const TheNameOfTheGame = T_("MiKTeX Configuration Utility");
#define PROGNAME T_("initexmf")

/* _________________________________________________________________________

   IsWindowsNT
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
inline
bool
IsWindowsNT ()
{
  return (GetVersion () < 0x80000000);
}
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

tstring
Quoted (/*[in]*/ const MIKTEXCHAR * lpsz)
{
  bool needQuotes = (*lpsz == 0 || StrChr(lpsz, T_(' ')) != 0);
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

tstring
Quoted (/*[in]*/ const tstring & str)
{
  return (Quoted(str.c_str()));
}

/* _________________________________________________________________________

   Quoted
   _________________________________________________________________________ */

tstring
Quoted (/*[in]*/ const PathName & path)
{
  return (Quoted(path.Get()));
}

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
  GetBuffer ()
    const
  {
    return (buffer);
  }

private:
  MIKTEXCHAR buffer[BUFSIZE];
};

#define NUMTOSTR(num) static_cast<const MIKTEXCHAR *>(NUMTOSTRHELPER(num))

/* _________________________________________________________________________

   IniTeXMFApp
   _________________________________________________________________________ */

class IniTeXMFApp
  : public ICreateFndbCallback,
    public IEnumerateFndbCallback
{
public:
  IniTeXMFApp ();

public:
  ~IniTeXMFApp ();

public:
  void
  Init (/*[in]*/ const MIKTEXCHAR * argv0);

public:
  void
  Finalize ();

private:
  void
  Message (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	   /*[in]*/			...);

private:
  void
  Verbose (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	   /*[in]*/			...);

private:
  void
  PrintOnly (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	     /*[in]*/				...);

private:
  void
  Warning (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	   /*[in]*/			...);

private:
  MIKTEXNORETURN
  void
  FatalError (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
	      /*[in]*/				...);

private:
  void
  UpdateFilenameDatabase (/*[in]*/ const PathName & root);

private:
  void
  UpdateFilenameDatabase (/*[in]*/ unsigned root);

private:
  void
  ListFormats ();

private:
  void
  ListMetafontModes ();

private:
  void
  RemoveFndb ();
  
private:
  void
  SetTeXMFRootDirectories ();
  
private:
  void
  RunMakeTeX (/*[in]*/ const MIKTEXCHAR *		lpszMakeProg,
	      /*[in]*/ const CommandLineBuilder &	arguments);
  
private:
  void
  MakeFormatFile (/*[in]*/ const MIKTEXCHAR *	lpszFormatName);

private:
  void
  MakeFormatFiles (/*[in]*/ const vector<tstring> & formats);
  
private:
  void
  MakeMaps ();

private:
  void
  EditConfigFile (/*[in]*/ const MIKTEXCHAR * lpszRelPath);

private:
  void
  MakeLinks ();

private:
  void
  ReportMiKTeXVersion ();

private:
  void
  ReportOSVersion ();

private:
  void
  ReportRoots ();

private:
  void
  ReportFndbFiles ();

private:
  void
  WriteReport ();

public:
  void
  Run (/*[in]*/ int			argc,
       /*[in]*/ const MIKTEXCHAR **	argv);

#if defined(MIKTEX_WINDOWS)
private:
  static
  BOOL
  CALLBACK
  EnumWindowsProc (/*[in]*/ HWND	hwnd,
		   /*[in]*/ LPARAM	lparam);
#endif

private:
  void
  FindWizards ();

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
  virtual
  bool
  MIKTEXCALL
  OnFndbItem (/*[in]*/ const MIKTEXCHAR *	lpszPath,
	      /*[in]*/ const MIKTEXCHAR *	lpszName,
	      /*[in]*/ const MIKTEXCHAR *	lpszInfo,
	      /*[in]*/  bool			isDirectory);

private:
  PathName enumDir;

private:
  bool csv;
  
private:
  bool recursive;
  
private:
  bool verbose;
  
private:
  bool quiet;
  
private:
  bool printOnly;
  
private:
  bool removeFndb;

private:
  StartupConfig startupConfig;
  
private:
  vector<tstring> formatsMade;
  
private:
  StreamWriter logStream;
  
private:
  bool updateWizardRunning;
  
private:
  bool setupWizardRunning;
  
private:
  PackageManagerPtr pManager;

private:
  SessionWrapper pSession;

private:
  static const struct poptOption aoption[];
};

/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 256,

  OPT_DUMP,
  OPT_EDIT_CONFIG_FILE,
  OPT_LIST_MODES,
  OPT_MKLINKS,
  OPT_MKMAPS,
  OPT_PRINT_ONLY,
  OPT_QUIET,
  OPT_REPORT,
  OPT_UPDATE_FNDB,
  OPT_VERBOSE,
  OPT_VERSION,

  OPT_ADD_FILE,			// <experimental/>
  OPT_CSV,			// <experimental/>
  OPT_LIST_DIRECTORY,		// <experimental/>
  OPT_LIST_FORMATS,		// <experimental/>
  OPT_RECURSIVE,		// <experimental/>
  OPT_REMOVE_FILE,		// <experimental/>

  OPT_COMMON_CONFIG,		// <internal/>
  OPT_COMMON_DATA,		// <internal/>
  OPT_INSTALL_ROOT,		// <internal/>
  OPT_LOG_FILE,			// <internal/>
  OPT_RMFNDB,			// <internal/>
  OPT_ROOTS,			// <internal/>
  OPT_SHARED_SETUP,		// <internal/>
  OPT_USER_CONFIG,		// <internal/>
  OPT_USER_DATA,		// <internal/>

};

/* _________________________________________________________________________

   IniTeXMFApp::aoption
   _________________________________________________________________________ */

const struct poptOption IniTeXMFApp::aoption[] = {

  {
    T_("add-file"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_ADD_FILE,
    T_("Add a file to the file name database."),
    T_("FILE")
  },

  {
    T_("common-config"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_COMMON_CONFIG,
    T_("Register the common configuration directory."),
    T_("DIR")
  },

  {
    T_("common-data"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_COMMON_DATA,
    T_("Register the common data directory."),
    T_("DIR")
  },

  {
    T_("csv"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_CSV,
    T_("Print comma-separated values."),
    0,
  },

  {
    T_("dump"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0,
    OPT_DUMP,
    T_("Create format files."),
    T_("FORMAT")
  },

  {
    T_("edit-config-file"), 0,
    POPT_ARG_STRING, 0,
    OPT_EDIT_CONFIG_FILE,
    T_("Open the specified config file in an editor. FILE must be one of: \
dvipdfm, dvipdfmx, dvips, pdftex, updmap."),
    T_("FILE")
  },

  {
    T_("install-root"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_INSTALL_ROOT,
    T_("Register the installation TEXMF directory."),
    T_("DIR")
  },

  {
    T_("list-directory"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_LIST_DIRECTORY,
    T_("List the FNDB contents of a directory."),
    T_("DIR")
  },

  {
    T_("list-formats"), 0,
    POPT_ARG_NONE, 0,
    OPT_LIST_FORMATS,
    T_("List formats."),
    0
  },

  {
    T_("list-modes"), 0,
    POPT_ARG_NONE, 0,
    OPT_LIST_MODES,
    T_("List METAFONT modes"),
    0
  },
  
  {
    T_("log-file"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_LOG_FILE,
    T_("Append to log file."),
    T_("FILE")
  },
  
  {
    T_("mklinks"), 0,
    POPT_ARG_NONE, 0,
    OPT_MKLINKS,
    T_("Create executables."),
    0
  },

  {
    T_("mkmaps"), 0,
    POPT_ARG_NONE, 0,
    OPT_MKMAPS,
    T_("Create font map files."),
    0
  },

  {
    T_("print-only"), T_('n'),
    POPT_ARG_NONE, 0,
    OPT_PRINT_ONLY,
    T_("Print what would be done."),
    0
  },
  
  {
    T_("quiet"), T_('q'),
    POPT_ARG_NONE, 0,
    OPT_QUIET,
    T_("Suppress screen output."),
    0
  },

  {
    T_("recursive"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_RECURSIVE,
    T_("Operate recursively."),
    0
  },

  {
    T_("remove-file"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_REMOVE_FILE,
    T_("Remove a file from the file name database."),
    T_("FILE")
  },

  {
    T_("report"), 0,
    POPT_ARG_NONE, 0,
    OPT_REPORT,
    T_("Create a configuration report."),
    0
  },

  {
    T_("rmfndb"), 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_RMFNDB,
    T_("Remove file name database files."),
    0
  },

  {
    T_("roots"), T_('r'),
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_ROOTS,
    T_("Register root directories."),
    T_("DIRS")
  },
    
  {
    T_("shared-setup"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_SHARED_SETUP,
    T_("Set up a shared MiKTeX system."),
    0
  },
  
  {
    T_("update-fndb"), T_('u'),
    POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0,
    OPT_UPDATE_FNDB,
    T_("Update the file name database."),
    T_("ROOT")
  },

  {
    T_("user-config"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_USER_CONFIG,
    T_("Register the user configuration directory."),
    T_("DIR")
  },

  {
    T_("user-data"), 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_USER_DATA,
    T_("Register the user data directory."),
    T_("DIR")
  },

  {
    T_("verbose"), T_('v'),
    POPT_ARG_NONE, 0,
    OPT_VERBOSE,
    T_("Print information on what is being done."),
    0,
  },

  {
    T_("version"), T_('V'),
    POPT_ARG_NONE, 0,
    OPT_VERSION,
    T_("Print version information and exit."),
    0
  },

  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   IniTeXMFApp::IniTeXMFApp
   _________________________________________________________________________ */

IniTeXMFApp::IniTeXMFApp ()
  : printOnly (false),
    quiet (false),
    csv (false),
    recursive (false),
    removeFndb (false),
    verbose (false)
{
}

/* _________________________________________________________________________

   IniTeXMFApp::~IniTeXMFApp
   _________________________________________________________________________ */

IniTeXMFApp::~IniTeXMFApp ()
{
  try
    {
      Finalize ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   IniTeXMFApp::Init
   _________________________________________________________________________ */

void
IniTeXMFApp::Init (/*[in]*/ const MIKTEXCHAR * argv0)
{
  pSession.CreateSession (Session::InitInfo(argv0));
  pManager.Create ();
  FindWizards ();
}

/* _________________________________________________________________________

   IniTeXMFApp::Finalize
   _________________________________________________________________________ */

void
IniTeXMFApp::Finalize ()
{
  if (logStream.IsOpen())
    {
      logStream.Close ();
    }
  pManager.Release ();
  pSession.Reset ();
}

/* _________________________________________________________________________

   IniTeXMFApp::EnumWindowsProc
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)

#define UPDATEWIZ_PREFIX T_("Update MiKTeX")
#define SETUPWIZ_PREFIX T_("Setup MiKTeX")

BOOL
CALLBACK
IniTeXMFApp::EnumWindowsProc (/*[in]*/ HWND	hwnd,
			      /*[in]*/ LPARAM	lparam)
{
  IniTeXMFApp * This = reinterpret_cast<IniTeXMFApp*>(lparam);
  MIKTEXCHAR szText[200];
  if (GetWindowText(hwnd, szText, 200) == 0)
    {
      return (TRUE);
    }
  if (_tcsncmp(szText, UPDATEWIZ_PREFIX, _tcslen(UPDATEWIZ_PREFIX)) == 0)
    {
      This->updateWizardRunning = true;
    }
  else if (_tcsncmp(szText, SETUPWIZ_PREFIX, _tcslen(SETUPWIZ_PREFIX))
	   == 0)
    {
      This->setupWizardRunning = true;
    }
  return (TRUE);
}
#endif

/* _________________________________________________________________________

   IniTeXMFApp::FindWizards
   _________________________________________________________________________ */

void
IniTeXMFApp::FindWizards ()
{
  setupWizardRunning = false;
  updateWizardRunning = false;
#if defined(MIKTEX_WINDOWS)
  EnumWindows (EnumWindowsProc, reinterpret_cast<LPARAM>(this));
#else
#  warning Unimplemented: IniTeXMFApp::FindWizards
#endif
}

/* _________________________________________________________________________
     
   IniTeXMFApp::Message
   _________________________________________________________________________ */

void
IniTeXMFApp::Message (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
		      /*[in]*/				...)
{
  if (quiet || printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcout << Utils::FormatString(lpszFormat, arglist);
  va_end (arglist);
}

/* _________________________________________________________________________
     
   IniTeXMFApp::Verbose
   _________________________________________________________________________ */

void
IniTeXMFApp::Verbose (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
		      /*[in]*/				...)
{
  if (! verbose || printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcout << Utils::FormatString(lpszFormat, arglist) << endl;
  va_end (arglist);
}

/* _________________________________________________________________________
     
   IniTeXMFApp::PrintOnly
   _________________________________________________________________________ */

void
IniTeXMFApp::PrintOnly (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
			/*[in]*/			...)
{
  if (! printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcout << Utils::FormatString(lpszFormat, arglist) << endl;
  va_end (arglist);
}

/* _________________________________________________________________________
     
   IniTeXMFApp::Warning
   _________________________________________________________________________ */

void
IniTeXMFApp::Warning (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
		      /*[in]*/				...)
{
  if (quiet)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcerr << PROGNAME << T_(": warning: ")
	<< Utils::FormatString(lpszFormat, arglist)
	<< endl;
  va_end (arglist);
}

/* _________________________________________________________________________
     
   IniTeXMFApp::FatalError
   _________________________________________________________________________ */
  
MIKTEXNORETURN
void
IniTeXMFApp::FatalError (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
			 /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  tcerr << PROGNAME << T_(": ")
	<< Utils::FormatString(lpszFormat, arglist)
	<< endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________

   IniTeXMFApp::ReadDirectory
   _________________________________________________________________________ */

bool
MIKTEXCALL
IniTeXMFApp::ReadDirectory (/*[in]*/ const MIKTEXCHAR *	lpszPath,
			    /*[out]*/ MIKTEXCHAR * *	ppSubDirNames,
			    /*[out]*/ MIKTEXCHAR * *	ppFileNames,
			    /*[out]*/ MIKTEXCHAR * *	ppFileNameInfos)
  
{
  UNUSED_ALWAYS (lpszPath);
  UNUSED_ALWAYS (ppSubDirNames);
  UNUSED_ALWAYS (ppFileNames);
  UNUSED_ALWAYS (ppFileNameInfos);
  return (false);
}

/* _________________________________________________________________________

   IniTeXMFApp::OnProgress
   _________________________________________________________________________ */

bool
MIKTEXCALL
IniTeXMFApp::OnProgress (/*[in]*/ unsigned		level,
			 /*[in]*/ const MIKTEXCHAR *	lpszDirectory)
{
  if (verbose && level == 1)
    {
      Verbose (T_("Scanning %s"), Q_(lpszDirectory));
    }
  else if (level == 1)
    {
      Message (T_("."));
    }
  return (true);
}

/* _________________________________________________________________________

   IniTeXMFApp::UpdateFilenameDatabase
   _________________________________________________________________________ */

void
IniTeXMFApp::UpdateFilenameDatabase (/*[in]*/ const PathName & root)
{
  // unload the file name database
  if (! printOnly && ! pSession->UnloadFilenameDatabase())
    {
      FatalError (T_("The file name database could not be unloaded."));
    }

  unsigned rootIdx = pSession->DeriveTEXMFRoot(root);

  // remove the old FNDB file(s)
  PathName path = pSession->GetFilenameDatabasePathName(rootIdx);
  if (File::Exists(path))
    {
      Verbose (T_("Deleting %s..."), Q_(path));
      PrintOnly (T_("rm %s"), Q_(path));
      if (! printOnly)
	{
	  File::Delete (path, true);
	}
    }

  // create the FNDB file
  PathName fndbPath = pSession->GetFilenameDatabasePathName(rootIdx);
  Verbose (T_("Creating %s..."), Q_(fndbPath));
  PrintOnly (T_("fndbcreate %s %s"), Q_(fndbPath), Q_(root));
  if (! printOnly)
    {
      Fndb::Create (fndbPath, root, this);
    }

  if (! verbose)
    {
      Message (T_("\n"));
    }
}

/* _________________________________________________________________________

   IniTeXMFApp::UpdateFilenameDatabase
   _________________________________________________________________________ */

void
IniTeXMFApp::UpdateFilenameDatabase (/*[in]*/ unsigned root)
{
  UpdateFilenameDatabase (pSession->GetRootDirectory(root));
}

/* _________________________________________________________________________

   IniTeXMFApp::ListFormats
   _________________________________________________________________________ */

void
IniTeXMFApp::ListFormats ()
{
  FormatInfo formatInfo;
  for (unsigned i = 0; pSession->GetFormatInfo(i, formatInfo); ++ i)
    {
      tcout << formatInfo.name
	    << T_(" ") << formatInfo.compiler
	    << T_(" ") << formatInfo.inputFile
	    << endl;
    }
}

/* _________________________________________________________________________

   IniTeXMFApp::ListMetafontModes
     
   List all known MF modes.
   _________________________________________________________________________ */

void
IniTeXMFApp::ListMetafontModes ()
{
  MIKTEXMFMODE mode;
  for (unsigned i = 0; pSession->GetMETAFONTMode(i, &mode); ++ i)
    {
      tcout << setw(8) << left << mode.szMnemonic
	    << T_("  ") << setw(5) << right << mode.iHorzRes
	    << T_("x") << setw(5) << left << mode.iVertRes
	    << T_("  ") << mode.szDescription
	    << endl;
    }
}

/* _________________________________________________________________________
     
   RemoveFndb
   _________________________________________________________________________ */
  
void
IniTeXMFApp::RemoveFndb ()
{
  size_t nRoots = pSession->GetNumberOfTEXMFRoots();
  for (unsigned r = 0; r < nRoots; ++ r)
    {
      PathName path = pSession->GetFilenameDatabasePathName(r);
      PrintOnly (T_("rm %s"), Q_(path));
      if (! printOnly && File::Exists(path))
	{
	  Verbose (T_("Deleting %s..."), Q_(path));
	  File::Delete (path, true);
	}
    }
}

/* _________________________________________________________________________
     
   IniTeXMFApp::SetTeXMFRootDirectories
   _________________________________________________________________________ */

void
IniTeXMFApp::SetTeXMFRootDirectories ()
{
  Verbose (T_("Registering root directories..."));
  PrintOnly (T_("regroots %s %s %s %s %s %s"),
	     Q_(startupConfig.roots),
	     Q_(startupConfig.userDataRoot),
	     Q_(startupConfig.userConfigRoot),
	     Q_(startupConfig.commonDataRoot),
	     Q_(startupConfig.commonConfigRoot),
	     Q_(startupConfig.installRoot));
  if (! printOnly)
    {
      pSession
	->RegisterRootDirectories (startupConfig, false);
    }
}

/* _________________________________________________________________________

   IniTeXMFApp::RunMakeTeX
   _________________________________________________________________________ */

void
IniTeXMFApp::RunMakeTeX (/*[in]*/ const MIKTEXCHAR *		lpszMakeProg,
			 /*[in]*/ const CommandLineBuilder &	arguments)
{
  PathName exe;

  if (! pSession->FindFile(lpszMakeProg, FileType::EXE, exe))
    {
      FatalError (T_("The %s executable could not be found."),
		  Q_(lpszMakeProg));
    }

  CommandLineBuilder xArguments (arguments);

  if (printOnly)
    {
      xArguments.AppendOption (T_("--print-only"));
    }

  if (verbose)
    {
      xArguments.AppendOption (T_("--verbose"));
    }

  if (quiet)
    {
      xArguments.AppendOption (T_("--quiet"));
    }

  Process::Run (exe, xArguments.Get());
}

/* _________________________________________________________________________
     
   IniTeXMFApp::MakeFormatFile
   _________________________________________________________________________ */

void
IniTeXMFApp::MakeFormatFile (/*[in]*/ const MIKTEXCHAR * lpszFormatName)
{
  if (find(formatsMade.begin(), formatsMade.end(), lpszFormatName)
      != formatsMade.end())
    {
      return;
    }

  FormatInfo formatInfo;
  if (! pSession->TryGetFormatInfo(lpszFormatName, formatInfo))
    {
      FatalError (T_("Unknown format: %s"), Q_(lpszFormatName));
    }

  tstring maker;

  CommandLineBuilder arguments;

  if (formatInfo.compiler == T_("mf"))
    {
      maker = T_("makebase");
    }
  else if (formatInfo.compiler == T_("mpost"))
    {
      maker = T_("makemem");
    }
  else
    {
      maker = T_("makefmt");
      arguments.AppendOption (T_("--engine="), formatInfo.compiler);
    }
    
  arguments.AppendOption (T_("--dest-name="), formatInfo.name);

  if (! formatInfo.preloaded.empty())
    {
      if (PathName::Compare(formatInfo.preloaded.c_str(), lpszFormatName)
	  == 0)
	{
	  FatalError (T_("Rule recursion detected for:"), lpszFormatName);
	}
      // <recursivecall>
      MakeFormatFile (formatInfo.preloaded.c_str());
      // <//recursivecall>
      arguments.AppendOption (T_("--preload="), formatInfo.preloaded);
    }

  if (PathName(formatInfo.inputFile).HasExtension(T_(".ini")))
    {
      arguments.AppendOption (T_("--no-dump"));
    }

  arguments.AppendArgument (formatInfo.inputFile);
    
  RunMakeTeX (maker.c_str(), arguments);

  formatsMade.push_back (lpszFormatName);
}

/* _________________________________________________________________________
     
   IniTeXMFApp::MakeFormatFiles
   _________________________________________________________________________ */

void
IniTeXMFApp::MakeFormatFiles (/*[in]*/ const vector<tstring> & formats)
{
  if (formats.size() == 0)
    {
      FormatInfo formatInfo;
      for (unsigned i = 0; pSession->GetFormatInfo(i, formatInfo); ++ i)
	{
	  if (! formatInfo.exclude)
	    {
	      MakeFormatFile (formatInfo.name.c_str());
	    }
	}
    }
  else
    {
      for (vector<tstring>::const_iterator it = formats.begin();
	   it != formats.end();
	   ++ it)
	{
	  MakeFormatFile (it->c_str());
	}
    }
}

/* _________________________________________________________________________
     
   IniTeXMFApp::MakeLinks
   _________________________________________________________________________ */

void
IniTeXMFApp::MakeLinks ()
{
  PathName pathBinDir = pSession->GetSpecialPath(SpecialPath::BinDirectory);

  if (! Directory::Exists(pathBinDir))
    {
      Directory::Create (pathBinDir);
    }

  bool overwrite = true;

  if (logStream.IsOpen())
    {
      logStream.WriteLine (T_("[files]"));
    }

  Verbose (T_("Making format links..."));

  FormatInfo formatInfo;

  for (unsigned i = 0; pSession->GetFormatInfo(i, formatInfo); ++ i)
    {
      PathName compilerPath;
      if (! pSession->FindFile(formatInfo.compiler,
			       FileType::EXE,
			       compilerPath))
	{
	  Warning (T_("The %s executable could not be found."),
		   formatInfo.compiler.c_str());
	  continue;
	}
      PathName tmp;
      if (overwrite
	  || ! pSession->FindFile(formatInfo.name, FileType::EXE, tmp))
	{
	  PathName exePath
	    (pathBinDir, formatInfo.name, MIKTEX_EXE_FILE_SUFFIX);
	  if (! (compilerPath == exePath))
	    {
	      if (! File::Exists(exePath) || overwrite)
		{
		  PrintOnly (T_("cp %s %s"), Q_(compilerPath), Q_(exePath));
		  if (! printOnly)
		    {
		      File::Copy (compilerPath, exePath);
		    }
		}
	      if (logStream.IsOpen())
		{
		  logStream.WriteLine (exePath.Get());
		}
	    }
	}
    }

  Verbose (T_("Making Perl links..."));

  PathName runperl;

  if (pSession->FindFile(T_("runperl2"), FileType::EXE, runperl))
    {
      PackageInfo pi;
      auto_ptr<PackageIterator> pIter (pManager->CreateIterator());
      while (pIter->GetNext(pi))
	{
	  for (vector<tstring>::const_iterator it
		 = pi.runFiles.begin();
	       it != pi.runFiles.end();
	       ++ it)
	    {
	      MIKTEXCHAR szFileName[BufferSizes::MaxPath];
	      MIKTEXCHAR szExt[BufferSizes::MaxPath];
	      PathName::Split (it->c_str(),
			       0, 0,
			       szFileName, BufferSizes::MaxPath,
			       szExt, BufferSizes::MaxPath);
	      if (PathName::Compare(szExt, T_(".pl")) != 0)
		{
		  continue;
		}
	      PathName pathExe
		(pathBinDir, szFileName, MIKTEX_EXE_FILE_SUFFIX);
	      Verbose (T_("  %s"), pathExe.Get());
	      if (! File::Exists(pathExe) || overwrite)
		{
		  PrintOnly (T_("cp %s %s"), Q_(runperl), Q_(pathExe));
		  if (! printOnly)
		    {
		      File::Copy (runperl, pathExe);
		    }
		}
	      if (logStream.IsOpen())
		{
		  logStream.WriteLine (pathExe.Get());
		}
	    }
	}
      pIter->Dispose ();
    }

#if defined(MIKTEX_WINDOWS)
  Verbose(T_("Making batch file links...\n"));

  PathName runbat;

  if (pSession->FindFile(T_("runbat"), FileType::EXE, runbat))
    {
      PackageInfo pi;
      auto_ptr<PackageIterator> pIter (pManager->CreateIterator());
      while (pIter->GetNext(pi))
	{
	  for (vector<tstring>::const_iterator it
		 = pi.runFiles.begin();
	       it != pi.runFiles.end();
	       ++ it)
	    {
	      MIKTEXCHAR szFileName[BufferSizes::MaxPath];
	      MIKTEXCHAR szExt[BufferSizes::MaxPath];
	      PathName::Split (it->c_str(),
			       0, 0,
			       szFileName, BufferSizes::MaxPath,
			       szExt, BufferSizes::MaxPath);
	      if (PathName::Compare(szExt, T_(".bat")) != 0)
		{
		  continue;
		}
	      PathName pathExe (pathBinDir, szFileName, T_(".exe"));
	      Verbose (T_("  %s"), pathExe.Get());
	      if (! File::Exists(pathExe) || overwrite)
		{
		  PrintOnly (T_("cp %s %s"), Q_(runbat), Q_(pathExe));
		  if (! printOnly)
		    {
		      File::Copy (runbat, pathExe);
		    }
		}
	      if (logStream.IsOpen())
		{
		  logStream.WriteLine (pathExe.Get());
		}
	    }
	}
      pIter->Dispose ();
    }
#endif
}

/* _________________________________________________________________________

   IniTeXMFApp::MakeMaps
   _________________________________________________________________________ */

void
IniTeXMFApp::MakeMaps ()
{
  // locate the mkfntmap executable
  PathName pathMkfontmap;
  if (! pSession->FindFile (T_("mkfntmap"), FileType::EXE, pathMkfontmap))
    {
      FatalError (T_("The mkfntmap executable could not be found."));
    }
  CommandLineBuilder arguments;
  if (verbose)
    {
      arguments.AppendOption (T_("--verbose"));
    }
  if (printOnly)
    {
      PrintOnly (T_("mkfntmap %s"), arguments.Get());
    }
  else
    {
      Process::Run (pathMkfontmap, arguments.Get());
    }
}

/* _________________________________________________________________________

   IniTeXMFApp::EditConfigFile
   _________________________________________________________________________ */

void
IniTeXMFApp::EditConfigFile (/*[in]*/ const MIKTEXCHAR * lpszRelPath)
{
  PathName configFile
    (pSession->GetSpecialPath(SpecialPath::ConfigRoot), lpszRelPath);
  if (! File::Exists(configFile))
    {
      if (! pSession->TryCreateFromTemplate(configFile))
	{
	  Directory::Create (PathName(configFile).RemoveFileSpec());
	  StreamWriter writer (configFile);
	  writer.Close ();
	}
    }
  CommandLineBuilder commandLine;
  commandLine.AppendArgument (configFile);
  Process::Start (T_("notepad.exe"), commandLine.Get());
}

/* _________________________________________________________________________

   IniTeXMFApp::ReportMiKTeXVersion
   _________________________________________________________________________ */

void
IniTeXMFApp::ReportMiKTeXVersion ()
{
  TriState sharedSetup = pSession->IsSharedMiKTeXSetup();
  tcout << T_("MiKTeX: ") << Utils::GetMiKTeXVersionString() << endl;
#if defined(MIKTEX_WINDOWS)
  if (IsWindowsNT())
    {
      tcout << T_("SystemAdmin: ") << (pSession->RunningAsAdministrator()
				       ? T_("yes")
				       : T_("no"))
	    << endl;
      tcout << T_("PowerUser: ") << (pSession->RunningAsPowerUser()
				     ? T_("yes")
				     : T_("no"))
	    << endl;
    }
#endif
  tcout << T_("SharedSetup: ") << (sharedSetup == TriState::True
				   ? T_("yes")
				   : (sharedSetup == TriState::False
				      ? T_("no")
				      : T_("unknown")))
	<< endl
	<< T_("BinDir: ")
	<< pSession->GetSpecialPath(SpecialPath::BinDirectory).Get()
	<< endl;
}

/* _________________________________________________________________________

   IniTeXMFApp::ReportOSVersion
   _________________________________________________________________________ */

void
IniTeXMFApp::ReportOSVersion ()
{
  tcout << T_("OS: ") << Utils::GetOSVersionString() << endl;
}

/* _________________________________________________________________________

   IniTeXMFApp::ReportRoots
   _________________________________________________________________________ */

void
IniTeXMFApp::ReportRoots ()
{
  for (unsigned idx = 0; idx < pSession->GetNumberOfTEXMFRoots(); ++ idx)
    {
      PathName absFileName;
      PathName root = pSession->GetRootDirectory(idx);
      tcout << T_("Root") << idx << T_(": ") << root.Get() << endl;
    }
  tcout << T_("Install: ")
	<< pSession->GetSpecialPath(SpecialPath::InstallRoot).Get()
	<< endl;
  tcout << T_("UserData: ")
	<< pSession->GetSpecialPath(SpecialPath::UserDataRoot).Get()
	<< endl;
  tcout << T_("UserConfig: ")
	<< pSession->GetSpecialPath(SpecialPath::UserConfigRoot).Get()
	<< endl;
  if (pSession->IsSharedMiKTeXSetup() == TriState::True)
    {
      tcout << T_("CommonData: ")
	    << pSession->GetSpecialPath(SpecialPath::CommonDataRoot).Get()
	    << endl;
      tcout << T_("CommonConfig: ")
	    << pSession->GetSpecialPath(SpecialPath::CommonConfigRoot).Get()
	    << endl;
    }
}

/* _________________________________________________________________________

   IniTeXMFApp::ReportFndbFiles
   _________________________________________________________________________ */

void
IniTeXMFApp::ReportFndbFiles ()
{
  for (unsigned idx = 0; idx < pSession->GetNumberOfTEXMFRoots(); ++ idx)
    {
      PathName absFileName;
      tcout << T_("fndb") << idx << T_(": ");
      if (pSession->FindFilenameDatabase(idx, absFileName))
	{
	  tcout << absFileName.Get() << endl;
	}
      else
	{
	  tcout << T_("<does not exist>") << endl;
	}
    }
  unsigned r = pSession->DeriveTEXMFRoot(MPM_ROOT_PATH);
  PathName path;
  tcout << T_("fndbmpm: ");
  if (pSession->FindFilenameDatabase(r, path))
    {
      tcout << path.Get() << endl;
    }
  else
    {
      tcout << T_("<does not exist>") << endl;
    }
}

/* _________________________________________________________________________

   IniTeXMFApp::WriteReport
     
   Show configuration settings.
   _________________________________________________________________________ */

void
IniTeXMFApp::WriteReport ()
{
  ReportMiKTeXVersion ();
  ReportOSVersion ();
  ReportRoots ();
  ReportFndbFiles ();
}

/* _________________________________________________________________________
   
   IniTeXMFApp::OnFndbItem
   _________________________________________________________________________ */

bool
MIKTEXCALL
IniTeXMFApp::OnFndbItem (/*[in]*/ const MIKTEXCHAR *	lpszPath,
			 /*[in]*/ const MIKTEXCHAR *	lpszName,
			 /*[in]*/ const MIKTEXCHAR *	lpszInfo,
			 /*[in]*/  bool			isDirectory)
{
  if (recursive)
    {
      PathName path (lpszPath, lpszName);
      const MIKTEXCHAR * lpszRel =
	Utils::GetRelativizedPath(path.Get(), enumDir.Get());
      if (! isDirectory)
	{
	  if (lpszInfo == 0)
	    {
	      tcout << lpszRel
		    << endl;
	    }
	  else
	    {
	      if (csv)
		{
		  tcout << lpszRel
			<< T_(';')
			<< lpszInfo
			<< endl;
		}
	      else
		{
		  tcout << lpszRel
			<< T_(" (\"") << lpszInfo << T_("\")")
			<< endl;
		}
	    }
	}
      if (isDirectory)
	{
	  Fndb::Enumerate (path, this);
	}
    }
  else
    {
      if (lpszInfo == 0)
	{
	  tcout << (isDirectory ? T_('D') : T_(' ')) << lpszName << endl;
	}
      else
	{
	  tcout << (isDirectory ? T_('D') : T_(' ')) << lpszName
		<< T_(" (\"") << lpszInfo << T_("\")")
		<< endl;
	}
    }
  return (true);
}

/* _________________________________________________________________________
     
   IniTeXMFApp::Run
   _________________________________________________________________________ */

void
IniTeXMFApp::Run (/*[in]*/ int			argc,
		  /*[in]*/ const MIKTEXCHAR **	argv)
{
  vector<tstring> addFiles;
  vector<tstring> editConfigFiles;
  vector<tstring> formats;
  vector<tstring> listDirectories;
  vector<tstring> removeFiles;
  vector<tstring> updateRoots;
 
  tstring logFile;

  TriState triSharedSetup (TriState::Undetermined);

  bool optDump = false;
  bool optMakeMaps = false;
  bool optListFormats = false;
  bool optListModes = false;
  bool optMakeLinks = false;
  bool optReport = false;
  bool optUpdateFilenameDatabase = false;
  bool optVersion = false;

  Cpopt popt (argc, argv, aoption);

  int option;

  while ((option = popt.GetNextOpt()) >= 0)
    {
      const MIKTEXCHAR * lpszOptArg = popt.GetOptArg();
      switch (option)
	{

	case OPT_ADD_FILE:

	  addFiles.push_back(lpszOptArg);
	  break;

	case OPT_CSV:
	  csv = true;
	  break;

	case OPT_DUMP:

	  if (lpszOptArg != 0)
	    {
	      formats.push_back (lpszOptArg);
	    }
	  optDump = true;
	  break;

	case OPT_EDIT_CONFIG_FILE:

	  editConfigFiles.push_back (lpszOptArg);
	  break;	  

	case OPT_INSTALL_ROOT:

	  startupConfig.installRoot = lpszOptArg;
	  break;

	case OPT_LIST_DIRECTORY:

	  listDirectories.push_back (lpszOptArg);
	  break;

	case OPT_LIST_FORMATS:

	  optListFormats = true;
	  break;

	case OPT_LIST_MODES:

	  optListModes = true;
	  break;

	case OPT_COMMON_DATA:

	  startupConfig.commonDataRoot = lpszOptArg;
	  break;

	case OPT_COMMON_CONFIG:

	  startupConfig.commonConfigRoot = lpszOptArg;
	  break;

	case OPT_USER_DATA:

	  startupConfig.userDataRoot = lpszOptArg;
	  break;

	case OPT_USER_CONFIG:

	  startupConfig.userConfigRoot = lpszOptArg;
	  break;

	case OPT_LOG_FILE:

	  logFile = lpszOptArg;
	  break;

	case OPT_MKLINKS:

	  optMakeLinks = true;
	  break;

	case OPT_MKMAPS:

	  optMakeMaps = true;
	  break;

	case OPT_PRINT_ONLY:

	  printOnly = true;
	  break;

	case OPT_QUIET:

	  quiet = true;
	  break;

	case OPT_RECURSIVE:

	  recursive = true;
	  break;

	case OPT_REMOVE_FILE:

	  removeFiles.push_back (lpszOptArg);
	  break;

	case OPT_REPORT:

	  optReport = true;
	  break;

	case OPT_RMFNDB:

	  removeFndb = true;
	  break;

	case OPT_ROOTS:

	  startupConfig.roots = lpszOptArg;
	  break;

	case OPT_SHARED_SETUP:

	  triSharedSetup =
	    (StringCompare(lpszOptArg, T_("0")) == 0
	     ? TriState::False
	     : TriState::True);
	  break;

	case OPT_UPDATE_FNDB:

	  optUpdateFilenameDatabase = true;
	  if (lpszOptArg != 0)
	    {
	      updateRoots.push_back (lpszOptArg);
	    }
	  break;

	case OPT_VERBOSE:

	  verbose = true;
	  break;

	case OPT_VERSION:

	  optVersion = true;
	  break;

	}
    }

  if (option != -1)
    {
      tstring msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += T_(": ");
      msg += popt.Strerror(option);
      FatalError (T_("%s"), msg.c_str());
    }
      
  if (popt.GetArgs() != 0)
    {
      FatalError (T_("This utility does not accept non-option arguments."));
    }

  if (optVersion)
    {
      tcout << Utils::MakeProgramVersionString(TheNameOfTheGame,
					       VER_FILEVERSION_STR)
	    << T_("\n\
Copyright (C) 1996-2006 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
	    << endl;
      return;
    }

  if (! logFile.empty())
    {
      if (File::Exists(logFile))
	{
	  logStream.Attach
	    (File::Open(logFile, FileMode::Append, FileAccess::Write));
	}
      else
	{
	  logStream.Attach
	    (File::Open(logFile, FileMode::Create, FileAccess::Write));
	}
    }

  if (updateWizardRunning)
    {
      Verbose (T_("MiKTeX Update Wizard is running"));
    }

  if (setupWizardRunning)
    {
      Verbose (T_("MiKTeX Setup Wizard is running"));
    }

  if (triSharedSetup != TriState::Undetermined)
    {
      pSession->SharedMiKTeXSetup (triSharedSetup.Get() == TriState::True);
    }

  if (! startupConfig.roots.empty()
      || ! startupConfig.userDataRoot.Empty()
      || ! startupConfig.userConfigRoot.Empty()
      || ! startupConfig.commonDataRoot.Empty()
      || ! startupConfig.commonConfigRoot.Empty()
      || ! startupConfig.installRoot.Empty())
    {
      SetTeXMFRootDirectories ();
    }

  if (optDump)
    {
      MakeFormatFiles (formats);
    }

  if (optMakeLinks)
    {
      MakeLinks ();
    }

  if (optMakeMaps)
    {
      MakeMaps ();
    }

  for (vector<tstring>::const_iterator it = addFiles.begin();
       it != addFiles.end();
       ++ it)
    {
      Verbose (T_("Adding %s to the file name database..."), Q_(*it));
      PrintOnly (T_("fndbadd %s"), Q_(*it));
      if (! printOnly)
	{
	  if (! Fndb::FileExists(*it))
	    {
	      Fndb::Add (*it);
	    }
	  else
	    {
	      Warning (T_("%s is already recorded in the file name database"),
		       Q_(*it));
	    }
	}
    }

  for (vector<tstring>::const_iterator it = removeFiles.begin();
       it != removeFiles.end();
       ++ it)
    {
      Verbose (T_("Removing %s from the file name database..."), Q_(*it));
      PrintOnly (T_("fndbremove %s"), Q_(*it));
      if (! printOnly)
	{
	  if (Fndb::FileExists(*it))
	    {
	      Fndb::Remove (*it);
	    }
	  else
	    {
	      Warning (T_("%s is not recorded in the file name database"),
		       Q_(*it));
	    }
	}
    }

  if (removeFndb)
    {
      RemoveFndb ();
    }

  if (optUpdateFilenameDatabase)
    {
      if (updateRoots.size() == 0)
	{
	  unsigned nRoots = pSession->GetNumberOfTEXMFRoots();
	  for (unsigned r = 0; r < nRoots; ++ r)
	    {
	      UpdateFilenameDatabase (r);
	    }
	}
      else
	{
	  for (vector<tstring>::const_iterator it = updateRoots.begin();
	       it != updateRoots.end();
	       ++ it)
	    {
	      UpdateFilenameDatabase (it->c_str());
	    }
	}
    }

  for (vector<tstring>::const_iterator it = listDirectories.begin();
       it != listDirectories.end();
       ++ it)
    {
      enumDir = *it;
      Fndb::Enumerate (*it, this);
    }

  for (vector<tstring>::const_iterator it = editConfigFiles.begin();
       it != editConfigFiles.end();
       ++ it)
    {
      if (*it == T_("pdftex"))
	{
	  EditConfigFile (MIKTEX_PATH_PDFTEX_CFG);
	}
      else if (*it == T_("dvips"))
	{
	  EditConfigFile (MIKTEX_PATH_CONFIG_PS);
	}
      else if (*it == T_("dvipdfm"))
	{
	  EditConfigFile (MIKTEX_PATH_DVIPDFM_CONFIG);
	}
      else if (*it == T_("dvipdfmx"))
	{
	  EditConfigFile (MIKTEX_PATH_DVIPDFMX_CONFIG);
	}
      else if (*it == T_("updmap"))
	{
	  EditConfigFile (MIKTEX_PATH_UPDMAP_CFG);
	}
      else
	{
	  FatalError (T_("Unknown config file."));
	}
    }

  if (optListFormats)
    {
      ListFormats ();
    }

  if (optListModes)
    {
      ListMetafontModes ();
    }

  if (optReport)
    {
      WriteReport ();
    }
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int			argc,
      /*[in]*/ const MIKTEXCHAR **	argv)
{
  try
    {
      IniTeXMFApp app;
      app.Init (argv[0]);
      app.Run (argc, argv);
      app.Finalize ();
      return (0);
    }
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      return (1);
    }
  catch (const exception & e)
    {
      Utils::PrintException (e);
      return (1);
    }
  catch (int exitCode)
    {
      return (exitCode);
    }
}
