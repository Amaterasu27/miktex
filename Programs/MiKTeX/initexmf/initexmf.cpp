/* initexmf.cpp: MiKTeX configuration utility

   Copyright (C) 1996-2009 Christian Schenk

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
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "initexmf-version.h"

#include <miktex/Core/Core>
#include <miktex/Core/Paths>
#include <miktex/Core/Registry>
#include <popt-miktex.h>
#include <miktex/PackageManager/PackageManager>

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace std;

#define UNUSED_ALWAYS(x) (x)

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) MiKTeX::Core::Quoter<char>(x).Get()

const char * const TheNameOfTheGame = T_("MiKTeX Configuration Utility");
#define PROGNAME "initexmf"

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

/* _________________________________________________________________________

   ProcessOutput
   _________________________________________________________________________ */

class ProcessOutput
  : public IRunProcessCallback
{
public:
  ProcessOutput ()
  {
  }
public:
  void
  RemoveTrailingNewline ()
  {
    if (output.length() > 0 && output[output.length() - 1] == '\n')
      {
	output.erase (output.length() - 1);
      }
  }
public:
  const char *
  Get ()
  {
    return (output.c_str());
  }
public:
  size_t
  GetLength ()
  {
    return (output.length());
  }
public:
  void
  Clear ()
  {
    output = "";
  }
public:
  virtual
  bool
  MIKTEXTHISCALL
  OnProcessOutput (/*[in]*/ const void *	pOutput,
		   /*[in]*/ size_t		n)
  {
    output.append (reinterpret_cast<const char *>(pOutput), n);
    return (true);
  }
private:
  string output;
};

/* _________________________________________________________________________

   XmlWriter
   _________________________________________________________________________ */

class XmlWriter
{
public:
  XmlWriter ()
    : freshElement (false)
  {
  }

public:
  void
  StartDocument ()
  {
    cout << "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n";
  }

public:
  void
  StartElement (/*[in]*/ const char *	lpszName)
  {
    if (freshElement)
      {
	cout << '>';
      }
    cout << '<';
    cout << lpszName;
    freshElement = true;
    elements.push (lpszName);
  }

public:
  void
  AddAttribute (/*[in]*/ const char *	lpszAttributeName,
		/*[in]*/ const char *	lpszAttributeValue)
  {
    cout << ' ';
    cout << lpszAttributeName;
    cout << "=\"";
    cout << lpszAttributeValue;
    cout << '"';
  }

public:
  void
  EndElement ()
  {
    if (elements.empty())
      {
	FATAL_MIKTEX_ERROR ("XmlWriter::EndElement",
			    T_("No elements on the stack."),
			    0);
      }
    if (freshElement)
      {
	cout << "/>";
	freshElement = false;
      }
    else
      {
	cout << "</";
	cout << elements.top();
	cout << '>';
      }
    elements.pop ();
  }

public:
  void
  EndAllElements ()
  {
    while (! elements.empty())
      {
	EndElement ();
      }
  }

public:
  void
  Text (/*[in]*/ const string & text )
  {
    if (freshElement)
      {
	cout << '>';
	freshElement = false;
      }
    for (const char * lpszText = text.c_str();
	 *lpszText != 0;
	 ++ lpszText)
      {
	switch (*lpszText)
	  {
	  case '&':
	    cout << "&amp;";
	    break;
	  case '<':
	    cout << "&lt;";
	    break;
	  case '>':
	    cout << "&gt;";
	    break;
	  default:
	    cout << *lpszText;
	    break;
	  }
      }
  }

private:
  stack<string> elements;

private:
  bool freshElement;
};

/* _________________________________________________________________________

   configShortcuts
   _________________________________________________________________________ */

static
struct
{
  const char *	lpszShortcut;
  const char *	lpszFile;
}
configShortcuts[] = {
  "pdftex", MIKTEX_PATH_PDFTEX_CFG,
  "dvips", MIKTEX_PATH_CONFIG_PS,
  "dvipdfm", MIKTEX_PATH_DVIPDFM_CONFIG,
  "dvipdfmx", MIKTEX_PATH_DVIPDFMX_CONFIG,
  "updmap", MIKTEX_PATH_UPDMAP_CFG,
};

/* _________________________________________________________________________

   IniTeXMFApp
   _________________________________________________________________________ */

class IniTeXMFApp
  : public ICreateFndbCallback,
    public IEnumerateFndbCallback,
    public PackageInstallerCallback
{
public:
  IniTeXMFApp ();

public:
  ~IniTeXMFApp ();

public:
  void
  Init (/*[in]*/ const char * argv0);

public:
  void
  Finalize ();

private:
  void
  Message (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);

private:
  void
  Verbose (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);

private:
  void
  PrintOnly (/*[in]*/ const char *	lpszFormat,
	     /*[in]*/			...);

private:
  void
  Warning (/*[in]*/ const char *	lpszFormat,
	   /*[in]*/			...);

private:
  MIKTEXNORETURN
  void
  FatalError (/*[in]*/ const char *	lpszFormat,
	      /*[in]*/			...);

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
  RunMakeTeX (/*[in]*/ const char *			lpszMakeProg,
	      /*[in]*/ const CommandLineBuilder &	arguments);
  
private:
  void
  MakeFormatFile (/*[in]*/ const char *	lpszFormatKey);

private:
  void
  MakeFormatFiles (/*[in]*/ const vector<string> & formats);
  
private:
  void
  MakeFormatFilesByName (/*[in]*/ const vector<string> &	formatsByName,
			 /*[in]*/ const string &		engine);
  
private:
  void
  MakeMaps ();

private:
  void
  EditConfigFile (/*[in]*/ const char * lpszRelPath);

private:
  void
  MakeLinks (/*[in]*/ bool force);

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

#if defined(MIKTEX_WINDOWS)
private:
  void
  ReportEnvironmentVariables ();
#endif

private:
  void
  ReportBrokenPackages ();

private:
  void
  WriteReport ();

#if ! MIKTEX_STANDALONE
private:
  void
  Configure ();
#endif

private:
  void
  CreatePortableSetup (/*[in]*/ const PathName & portableRoot);

public:
  void
  Run (/*[in]*/ int		argc,
       /*[in]*/ const char **	argv);

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
  virtual
  bool
  MIKTEXTHISCALL
  OnFndbItem (/*[in]*/ const char *	lpszPath,
	      /*[in]*/ const char *	lpszName,
	      /*[in]*/ const char *	lpszInfo,
	      /*[in]*/  bool		isDirectory);

public:
  virtual
  void
  MIKTEXTHISCALL
  ReportLine (/*[in]*/ const char * lpszLine);

public:
  virtual
  bool
  MIKTEXTHISCALL
  OnRetryableError (/*[in]*/ const char * lpszMessage);

public:
  virtual
  bool
  MIKTEXTHISCALL
  OnProgress (/*[in]*/ Notification	nf);

private:
  PathName enumDir;

private:
  bool csv;

private:
  bool xml;
  
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
  bool adminMode;

private:
  StartupConfig startupConfig;
  
private:
  vector<string> formatsMade;
  
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
  XmlWriter xmlWriter;

private:
  static const struct poptOption aoption_user[];

private:
  static const struct poptOption aoption_setup[];

private:
  static const struct poptOption aoption_update[];
};

/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 256,

  OPT_CONFIGURE,		// ! MIKTEX_STANDALONE

  OPT_ADMIN,
  OPT_DUMP,
  OPT_DUMP_BY_NAME,
  OPT_EDIT_CONFIG_FILE,
  OPT_ENGINE,
  OPT_FORCE,
  OPT_LIST_MODES,
  OPT_MKLINKS,
  OPT_MKMAPS,
  OPT_PRINT_ONLY,
  OPT_QUIET,
  OPT_REPORT,
  OPT_UPDATE_FNDB,
  OPT_USER_ROOTS,
  OPT_VERBOSE,
  OPT_VERSION,

  OPT_ADD_FILE,			// <experimental/>
  OPT_CSV,			// <experimental/>
  OPT_LIST_DIRECTORY,		// <experimental/>
  OPT_LIST_FORMATS,		// <experimental/>
  OPT_RECURSIVE,		// <experimental/>
  OPT_REMOVE_FILE,		// <experimental/>
  OPT_XML,			// <experimental/>

  OPT_COMMON_CONFIG,		// <internal/>
  OPT_COMMON_DATA,		// <internal/>
  OPT_COMMON_INSTALL,		// <internal/>
  OPT_COMMON_ROOTS,		// <internal/>
  OPT_LOG_FILE,			// <internal/>
  OPT_DEFAULT_PAPER_SIZE,	// <internal/>
  OPT_PORTABLE,	    		// <internal/>
  OPT_RMFNDB,			// <internal/>
  OPT_USER_CONFIG,		// <internal/>
  OPT_USER_DATA,		// <internal/>
  OPT_USER_INSTALL,		// <internal/>
};

/* _________________________________________________________________________

   IniTeXMFApp::aoption_user
   _________________________________________________________________________ */

const struct poptOption IniTeXMFApp::aoption_user[] = {

  {
    "add-file", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_ADD_FILE,
    T_("Add a file to the file name database."),
    T_("FILE")
  },

  {
    "admin", 0,
    POPT_ARG_NONE, 0,
    OPT_ADMIN,
    T_("Run in administrative mode."),
    0
  },
  
#if ! MIKTEX_STANDALONE
  {
    "configure", 0,
    POPT_ARG_NONE, 0,
    OPT_CONFIGURE,
    T_("Configure MiKTeX."),
    0,
  },
#endif

  {
    "csv", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_CSV,
    T_("Print comma-separated values."),
    0,
  },

  {
    "dump", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0,
    OPT_DUMP,
    T_("Create memory dump files."),
    T_("KEY")
  },

  {
    "dump-by-name", 0,
    POPT_ARG_STRING, 0,
    OPT_DUMP_BY_NAME,
    T_("Create a memory dump file by name."),
    T_("NAME")
  },

  {
    "edit-config-file", 0,
    POPT_ARG_STRING, 0,
    OPT_EDIT_CONFIG_FILE,
    T_("\
Open the specified configuration file in an editor.\
 See the manual, for more information."),
    T_("CONFIGFILE")
  },

  {
    "engine", 0,
    POPT_ARG_STRING, 0,
    OPT_ENGINE,
    T_("Engine to be used."),
    T_("ENGINE")
  },

  {
    "force", 0,
    POPT_ARG_NONE, 0,
    OPT_FORCE,
    T_("Force --mklinks to overwrite existing files."),
    0,
  },

  {
    "list-directory", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_LIST_DIRECTORY,
    T_("List the FNDB contents of a directory."),
    T_("DIR")
  },

  {
    "list-formats", 0,
    POPT_ARG_NONE, 0,
    OPT_LIST_FORMATS,
    T_("List formats."),
    0
  },

  {
    "list-modes", 0,
    POPT_ARG_NONE, 0,
    OPT_LIST_MODES,
    T_("List METAFONT modes"),
    0
  },
  
  {
    "mklinks", 0,
    POPT_ARG_NONE, 0,
    OPT_MKLINKS,
    T_("Create executables."),
    0
  },

  {
    "mkmaps", 0,
    POPT_ARG_NONE, 0,
    OPT_MKMAPS,
    T_("Create font map files."),
    0
  },

  {
    "portable", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_PORTABLE,
    T_("Create a portable setup."),
    0
  },

  {
    "print-only", 'n',
    POPT_ARG_NONE, 0,
    OPT_PRINT_ONLY,
    T_("Print what would be done."),
    0
  },
  
  {
    "quiet", 'q',
    POPT_ARG_NONE, 0,
    OPT_QUIET,
    T_("Suppress screen output."),
    0
  },

  {
    "recursive", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_RECURSIVE,
    T_("Operate recursively."),
    0
  },

  {
    "remove-file", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_REMOVE_FILE,
    T_("Remove a file from the file name database."),
    T_("FILE")
  },

  {
    "report", 0,
    POPT_ARG_NONE, 0,
    OPT_REPORT,
    T_("Create a configuration report."),
    0
  },

  {
    "rmfndb", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_RMFNDB,
    T_("Remove file name database files."),
    0
  },

  {
    "update-fndb", 'u',
    POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0,
    OPT_UPDATE_FNDB,
    T_("Update the file name database."),
    T_("ROOT")
  },

#if ! MIKTEX_STANDALONE
  {
    "user-install", 0,
    POPT_ARG_STRING, 0,
    OPT_USER_INSTALL,
    T_("Register the user installation directory."),
    T_("DIR")
  },
#endif

  {
    "user-roots", 'r',
    POPT_ARG_STRING, 0,
    OPT_USER_ROOTS,
    T_("Register user root directories."),
    T_("DIRS")
  },

  {
    "verbose", 'v',
    POPT_ARG_NONE, 0,
    OPT_VERBOSE,
    T_("Print information on what is being done."),
    0,
  },

  {
    "version", 'V',
    POPT_ARG_NONE, 0,
    OPT_VERSION,
    T_("Print version information and exit."),
    0
  },

  {
    "xml", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_XML,
    T_("Print XML."),
    0
  },

  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   IniTeXMFApp::aoption_setup
   _________________________________________________________________________ */

const struct poptOption IniTeXMFApp::aoption_setup[] = {

  {
    "add-file", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_ADD_FILE,
    T_("Add a file to the file name database."),
    T_("FILE")
  },

  {
    "admin", 0,
    POPT_ARG_NONE, 0,
    OPT_ADMIN,
    T_("Run in administration mode."),
    0
  },
  
  {
    "common-config", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_COMMON_CONFIG,
    T_("Register the common configuration directory."),
    T_("DIR")
  },

  {
    "common-data", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_COMMON_DATA,
    T_("Register the common data directory."),
    T_("DIR")
  },

  {
    "common-install", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_COMMON_INSTALL,
    T_("Register the common installation directory."),
    T_("DIR")
  },

  {
    "common-roots", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_COMMON_ROOTS,
    T_("Register common root directories."),
    T_("DIRS")
  },

  {
    "csv", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_CSV,
    T_("Print comma-separated values."),
    0,
  },

  {
    "default-paper-size", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_DEFAULT_PAPER_SIZE,
    T_("Set the default paper size."),
    T_("SIZE"),
  },

  {
    "dump", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0,
    OPT_DUMP,
    T_("Create format files."),
    T_("FORMAT")
  },

  {
    "dump-by-name", 0,
    POPT_ARG_STRING, 0,
    OPT_DUMP_BY_NAME,
    T_("Create a memory dump file by name."),
    T_("NAME")
  },

  {
    "edit-config-file", 0,
    POPT_ARG_STRING, 0,
    OPT_EDIT_CONFIG_FILE,
    T_("\
Open the specified configuration file in an editor.\
 See the manual, for more information."),
    T_("CONFIGFILE")
  },

  {
    "engine", 0,
    POPT_ARG_STRING, 0,
    OPT_ENGINE,
    T_("Engine to be used."),
    T_("ENGINE")
  },

  {
    "force", 0,
    POPT_ARG_NONE, 0,
    OPT_FORCE,
    T_("Force --mklinks to overwrite existing files."),
    0,
  },

  {
    "list-directory", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_LIST_DIRECTORY,
    T_("List the FNDB contents of a directory."),
    T_("DIR")
  },

  {
    "list-formats", 0,
    POPT_ARG_NONE, 0,
    OPT_LIST_FORMATS,
    T_("List formats."),
    0
  },

  {
    "list-modes", 0,
    POPT_ARG_NONE, 0,
    OPT_LIST_MODES,
    T_("List METAFONT modes"),
    0
  },
  
  {
    "log-file", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_LOG_FILE,
    T_("Append to log file."),
    T_("FILE")
  },
  
  {
    "mklinks", 0,
    POPT_ARG_NONE, 0,
    OPT_MKLINKS,
    T_("Create executables."),
    0
  },

  {
    "mkmaps", 0,
    POPT_ARG_NONE, 0,
    OPT_MKMAPS,
    T_("Create font map files."),
    0
  },

  {
    "portable", 0,
    POPT_ARG_STRING, 0,
    OPT_PORTABLE,
    T_("Create a portable setup."),
    0
  },

  {
    "print-only", 'n',
    POPT_ARG_NONE, 0,
    OPT_PRINT_ONLY,
    T_("Print what would be done."),
    0
  },
  
  {
    "quiet", 'q',
    POPT_ARG_NONE, 0,
    OPT_QUIET,
    T_("Suppress screen output."),
    0
  },

  {
    "recursive", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_RECURSIVE,
    T_("Operate recursively."),
    0
  },

  {
    "remove-file", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_REMOVE_FILE,
    T_("Remove a file from the file name database."),
    T_("FILE")
  },

  {
    "report", 0,
    POPT_ARG_NONE, 0,
    OPT_REPORT,
    T_("Create a configuration report."),
    0
  },

  {
    "rmfndb", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_RMFNDB,
    T_("Remove file name database files."),
    0
  },

  {
    "update-fndb", 'u',
    POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0,
    OPT_UPDATE_FNDB,
    T_("Update the file name database."),
    T_("ROOT")
  },

  {
    "user-config", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_USER_CONFIG,
    T_("Register the user configuration directory."),
    T_("DIR")
  },

  {
    "user-data", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_USER_DATA,
    T_("Register the user data directory."),
    T_("DIR")
  },

  {
    "user-install", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_USER_INSTALL,
    T_("Register the user installation directory."),
    T_("DIR")
  },

  {
    "user-roots", 'r',
    POPT_ARG_STRING, 0,
    OPT_USER_ROOTS,
    T_("Register user root directories."),
    T_("DIRS")
  },
    
  {
    "verbose", 'v',
    POPT_ARG_NONE, 0,
    OPT_VERBOSE,
    T_("Print information on what is being done."),
    0,
  },

  {
    "version", 'V',
    POPT_ARG_NONE, 0,
    OPT_VERSION,
    T_("Print version information and exit."),
    0
  },

  {
    "xml", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_XML,
    T_("Print XML."),
    0
  },

  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   IniTeXMFApp::aoption_update
   _________________________________________________________________________ */

const struct poptOption IniTeXMFApp::aoption_update[] = {

  {
    "add-file", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_ADD_FILE,
    T_("Add a file to the file name database."),
    T_("FILE")
  },

  {
    "admin", 0,
    POPT_ARG_NONE, 0,
    OPT_ADMIN,
    T_("Run in administration mode."),
    0
  },
  
  {
    "common-config", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_COMMON_CONFIG,
    T_("Register the common configuration directory."),
    T_("DIR")
  },

  {
    "common-data", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_COMMON_DATA,
    T_("Register the common data directory."),
    T_("DIR")
  },

  {
    "common-install", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_COMMON_INSTALL,
    T_("Register the common installation directory."),
    T_("DIR")
  },

  {
    "common-roots", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_COMMON_ROOTS,
    T_("Register common root directories."),
    T_("DIRS")
  },

  {
    "csv", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_CSV,
    T_("Print comma-separated values."),
    0,
  },

  {
    "default-paper-size", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_DEFAULT_PAPER_SIZE,
    T_("Set the default paper size."),
    T_("SIZE"),
  },

  {
    "dump", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0,
    OPT_DUMP,
    T_("Create format files."),
    T_("FORMAT")
  },

  {
    "dump-by-name", 0,
    POPT_ARG_STRING, 0,
    OPT_DUMP_BY_NAME,
    T_("Create a memory dump file by name."),
    T_("NAME")
  },

  {
    "edit-config-file", 0,
    POPT_ARG_STRING, 0,
    OPT_EDIT_CONFIG_FILE,
    T_("\
Open the specified configuration file in an editor.\
 See the manual, for more information."),
    T_("CONFIGFILE")
  },

  {
    "engine", 0,
    POPT_ARG_STRING, 0,
    OPT_ENGINE,
    T_("Engine to be used."),
    T_("ENGINE")
  },

  {
    "force", 0,
    POPT_ARG_NONE, 0,
    OPT_FORCE,
    T_("Force --mklinks to overwrite existing files."),
    0,
  },

  {
    "list-directory", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_LIST_DIRECTORY,
    T_("List the FNDB contents of a directory."),
    T_("DIR")
  },

  {
    "list-formats", 0,
    POPT_ARG_NONE, 0,
    OPT_LIST_FORMATS,
    T_("List formats."),
    0
  },

  {
    "list-modes", 0,
    POPT_ARG_NONE, 0,
    OPT_LIST_MODES,
    T_("List METAFONT modes"),
    0
  },
  
  {
    "log-file", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_LOG_FILE,
    T_("Append to log file."),
    T_("FILE")
  },
  
  {
    "mklinks", 0,
    POPT_ARG_NONE, 0,
    OPT_MKLINKS,
    T_("Create executables."),
    0
  },

  {
    "mkmaps", 0,
    POPT_ARG_NONE, 0,
    OPT_MKMAPS,
    T_("Create font map files."),
    0
  },

  {
    "print-only", 'n',
    POPT_ARG_NONE, 0,
    OPT_PRINT_ONLY,
    T_("Print what would be done."),
    0
  },
  
  {
    "quiet", 'q',
    POPT_ARG_NONE, 0,
    OPT_QUIET,
    T_("Suppress screen output."),
    0
  },

  {
    "recursive", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_RECURSIVE,
    T_("Operate recursively."),
    0
  },

  {
    "remove-file", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_REMOVE_FILE,
    T_("Remove a file from the file name database."),
    T_("FILE")
  },

  {
    "report", 0,
    POPT_ARG_NONE, 0,
    OPT_REPORT,
    T_("Create a configuration report."),
    0
  },

  {
    "rmfndb", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_RMFNDB,
    T_("Remove file name database files."),
    0
  },

  {
    "update-fndb", 'u',
    POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, 0,
    OPT_UPDATE_FNDB,
    T_("Update the file name database."),
    T_("ROOT")
  },

  {
    "user-config", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_USER_CONFIG,
    T_("Register the user configuration directory."),
    T_("DIR")
  },

  {
    "user-data", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_USER_DATA,
    T_("Register the user data directory."),
    T_("DIR")
  },

  {
    "user-install", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_USER_INSTALL,
    T_("Register the user installation directory."),
    T_("DIR")
  },

  {
    "user-roots", 'r',
    POPT_ARG_STRING, 0,
    OPT_USER_ROOTS,
    T_("Register user root directories."),
    T_("DIRS")
  },
    
  {
    "verbose", 'v',
    POPT_ARG_NONE, 0,
    OPT_VERBOSE,
    T_("Print information on what is being done."),
    0,
  },

  {
    "version", 'V',
    POPT_ARG_NONE, 0,
    OPT_VERSION,
    T_("Print version information and exit."),
    0
  },

  {
    "xml", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_XML,
    T_("Print XML."),
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
    xml (false),
    recursive (false),
    removeFndb (false),
    adminMode (false),
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
IniTeXMFApp::Init (/*[in]*/ const char * argv0)
{
  Session::InitInfo initInfo (argv0);
#if defined(MIKTEX_WINDOWS)
  initInfo.SetFlags (Session::InitFlags::InitializeCOM);
#endif
  pSession.CreateSession (initInfo);
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

BOOL
CALLBACK
IniTeXMFApp::EnumWindowsProc (/*[in]*/ HWND	hwnd,
			      /*[in]*/ LPARAM	lparam)
{
  IniTeXMFApp * This = reinterpret_cast<IniTeXMFApp*>(lparam);
  char szText[200];
  if (GetWindowText(hwnd, szText, 200) == 0)
    {
      return (TRUE);
    }
  if (strstr(szText, "MiKTeX") != 0)
    {
      if (strstr(szText, "Update") != 0)
	{
	  This->updateWizardRunning = true;
	}
      else if (strstr(szText, "Setup") != 0
	       || strstr(szText, "Installer") != 0)
	{
	  This->setupWizardRunning = true;
	}
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
IniTeXMFApp::Message (/*[in]*/ const char *	lpszFormat,
		      /*[in]*/			...)
{
  if (quiet || printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  cout << Utils::FormatString(lpszFormat, arglist);
  va_end (arglist);
}

/* _________________________________________________________________________
     
   IniTeXMFApp::Verbose
   _________________________________________________________________________ */

void
IniTeXMFApp::Verbose (/*[in]*/ const char *	lpszFormat,
		      /*[in]*/			...)
{
  if (! verbose || printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  cout << Utils::FormatString(lpszFormat, arglist) << endl;
  va_end (arglist);
}

/* _________________________________________________________________________
     
   IniTeXMFApp::PrintOnly
   _________________________________________________________________________ */

void
IniTeXMFApp::PrintOnly (/*[in]*/ const char *	lpszFormat,
			/*[in]*/		...)
{
  if (! printOnly)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  cout << Utils::FormatString(lpszFormat, arglist) << endl;
  va_end (arglist);
}

/* _________________________________________________________________________
     
   IniTeXMFApp::Warning
   _________________________________________________________________________ */

void
IniTeXMFApp::Warning (/*[in]*/ const char *	lpszFormat,
		      /*[in]*/			...)
{
  if (quiet)
    {
      return;
    }
  va_list arglist;
  va_start (arglist, lpszFormat);
  cerr << PROGNAME << T_(": warning: ")
       << Utils::FormatString(lpszFormat, arglist)
       << endl;
  va_end (arglist);
}

/* _________________________________________________________________________
     
   IniTeXMFApp::FatalError
   _________________________________________________________________________ */
  
MIKTEXNORETURN
void
IniTeXMFApp::FatalError (/*[in]*/ const char *	lpszFormat,
			 /*[in]*/		...)
{
  va_list arglist;
  va_start (arglist, lpszFormat);
  cerr << PROGNAME << ": "
       << Utils::FormatString(lpszFormat, arglist)
       << endl;
  va_end (arglist);
  throw (1);
}

/* _________________________________________________________________________

   IniTeXMFApp::ReadDirectory
   _________________________________________________________________________ */

bool
IniTeXMFApp::ReadDirectory (/*[in]*/ const char *	lpszPath,
			    /*[out]*/ char * *		ppSubDirNames,
			    /*[out]*/ char * *		ppFileNames,
			    /*[out]*/ char * *		ppFileNameInfos)
  
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
IniTeXMFApp::OnProgress (/*[in]*/ unsigned	level,
			 /*[in]*/ const char *	lpszDirectory)
{
#if 0
  if (verbose && level == 1)
    {
      Verbose (T_("Scanning %s"), Q_(lpszDirectory));
    }
  else if (level == 1)
    {
      Message (".");
    }
#endif
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

  // remove the old FNDB file
  PathName path = pSession->GetFilenameDatabasePathName(rootIdx);
  if (File::Exists(path))
    {
      PrintOnly ("rm %s", Q_(path));
      if (! printOnly)
	{
	  File::Delete (path, true);
	}
    }

  // create the FNDB file
  PathName fndbPath = pSession->GetFilenameDatabasePathName(rootIdx);
  Verbose (T_("Creating fndb (%s)..."), Q_(root));
  PrintOnly ("fndbcreate %s %s", Q_(fndbPath), Q_(root));
  if (! printOnly)
    {
      Fndb::Create (fndbPath, root, this);
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
  for (unsigned idx = 0; pSession->GetFormatInfo(idx, formatInfo); ++ idx)
    {
      cout << formatInfo.key << " ("
	   << formatInfo.description
	   << ")"
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
      cout << setw(8) << left << mode.szMnemonic
	   << "  " << setw(5) << right << mode.iHorzRes
	   << "x" << setw(5) << left << mode.iVertRes
	   << "  " << mode.szDescription
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
      PrintOnly ("rm %s", Q_(path));
      if (! printOnly && File::Exists(path))
	{
	  Verbose (T_("Removing fndb (%s)..."),
		   Q_(pSession->GetRootDirectory(r)));
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
  PrintOnly ("regroots ur=%s ud=%s uc=%s ui=%s cr=%s cd=%s cc=%s ci=%s",
    Q_(startupConfig.userRoots),
    Q_(startupConfig.userDataRoot),
    Q_(startupConfig.userConfigRoot),
    Q_(startupConfig.userInstallRoot),
    Q_(startupConfig.commonRoots),
    Q_(startupConfig.commonDataRoot),
    Q_(startupConfig.commonConfigRoot),
    Q_(startupConfig.commonInstallRoot));
  if (! printOnly)
  {
    pSession->RegisterRootDirectories (startupConfig, false);
  }
}

/* _________________________________________________________________________

   IniTeXMFApp::RunMakeTeX
   _________________________________________________________________________ */

void
IniTeXMFApp::RunMakeTeX (/*[in]*/ const char *			lpszMakeProg,
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
      xArguments.AppendOption ("--print-only");
    }

  if (verbose)
    {
      xArguments.AppendOption ("--verbose");
    }

  if (quiet)
    {
      xArguments.AppendOption ("--quiet");
    }

  if (adminMode)
  {
    xArguments.AppendOption ("--admin");
  }

  Process::Run (exe, xArguments.Get());
}

/* _________________________________________________________________________
     
   IniTeXMFApp::MakeFormatFile
   _________________________________________________________________________ */

void
IniTeXMFApp::MakeFormatFile (/*[in]*/ const char * lpszFormatKey)
{
  if (find(formatsMade.begin(), formatsMade.end(), lpszFormatKey)
      != formatsMade.end())
    {
      return;
    }

  FormatInfo formatInfo;
  if (! pSession->TryGetFormatInfo(lpszFormatKey, formatInfo))
    {
      FatalError (T_("Unknown format: %s"), Q_(lpszFormatKey));
    }

  string maker;

  CommandLineBuilder arguments;

  if (formatInfo.compiler == "mf")
    {
      maker = MIKTEX_MAKEBASE_EXE;
    }
  else if (formatInfo.compiler == "mpost")
    {
      maker = MIKTEX_MAKEMEM_EXE;
    }
  else
    {
      maker = MIKTEX_MAKEFMT_EXE;
      arguments.AppendOption ("--engine=", formatInfo.compiler);
    }
    
  arguments.AppendOption ("--dest-name=", formatInfo.name);

  if (! formatInfo.preloaded.empty())
    {
      if (PathName::Compare(formatInfo.preloaded.c_str(), lpszFormatKey)
	  == 0)
	{
	  FatalError (T_("Rule recursion detected for:"), lpszFormatKey);
	}
      // <recursivecall>
      MakeFormatFile (formatInfo.preloaded.c_str());
      // <//recursivecall>
      arguments.AppendOption ("--preload=", formatInfo.preloaded);
    }

  if (PathName(formatInfo.inputFile).HasExtension(".ini"))
    {
      arguments.AppendOption ("--no-dump");
    }

  arguments.AppendArgument (formatInfo.inputFile);

  if (! formatInfo.arguments.empty())
    {
      arguments.AppendOption ("--engine-option=", formatInfo.arguments);
    }
    
  RunMakeTeX (maker.c_str(), arguments);

  formatsMade.push_back (lpszFormatKey);
}

/* _________________________________________________________________________
     
   IniTeXMFApp::MakeFormatFiles
   _________________________________________________________________________ */

void
IniTeXMFApp::MakeFormatFiles (/*[in]*/ const vector<string> & formats)
{
  if (formats.size() == 0)
    {
      FormatInfo formatInfo;
      for (unsigned idx = 0; pSession->GetFormatInfo(idx, formatInfo); ++ idx)
	{
	  if (! formatInfo.exclude)
	    {
	      MakeFormatFile (formatInfo.key.c_str());
	    }
	}
    }
  else
    {
      for (vector<string>::const_iterator it = formats.begin();
	   it != formats.end();
	   ++ it)
	{
	  MakeFormatFile (it->c_str());
	}
    }
}

/* _________________________________________________________________________
     
   IniTeXMFApp::MakeFormatFilesByName
   _________________________________________________________________________ */

void
IniTeXMFApp::MakeFormatFilesByName
(/*[in]*/ const vector<string> &	formatsByName,
 /*[in]*/ const string &		engine)
{
  for (vector<string>::const_iterator it = formatsByName.begin();
       it != formatsByName.end();
       ++ it)
    {
      bool done = false;
      FormatInfo formatInfo;
      for (unsigned idx = 0; pSession->GetFormatInfo(idx, formatInfo); ++ idx)
	{
	  if (PathName::Compare(formatInfo.name, *it) == 0
	      && (engine.empty()
		  || (StringCompare(formatInfo.compiler.c_str(),
				   engine.c_str(),
				   true)
		      == 0)))
	    {
	      MakeFormatFile (formatInfo.key.c_str());
	      done = true;
	    }
	}
      if (! done)
	{
	  if (engine.empty())
	    {
	      FatalError (T_("Unknown format name: %s"), Q_(it->c_str()));
	    }
	  else
	    {
	      FatalError (T_("Unknown format name/engine: %s/%s"),
			  Q_(it->c_str()),
			  engine.c_str());
	    }

	}
    }
}

/* _________________________________________________________________________
     
   IniTeXMFApp::MakeLinks
   _________________________________________________________________________ */

void
IniTeXMFApp::MakeLinks (/*[in]*/ bool force)
{
  PathName pathBinDir = pSession->GetSpecialPath(SpecialPath::BinDirectory);

  if (! Directory::Exists(pathBinDir))
    {
      Directory::Create (pathBinDir);
    }

  bool overwrite = force;

  if (logStream.IsOpen())
    {
      logStream.WriteLine ("[files]");
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
		  PrintOnly ("cp %s %s", Q_(compilerPath), Q_(exePath));
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

  if (pSession->FindFile("runperl", FileType::EXE, runperl))
    {
      PackageInfo pi;
      auto_ptr<PackageIterator> pIter (pManager->CreateIterator());
      while (pIter->GetNext(pi))
	{
	  for (vector<string>::const_iterator it
		 = pi.runFiles.begin();
	       it != pi.runFiles.end();
	       ++ it)
	    {
	      char szFileName[BufferSizes::MaxPath];
	      char szExt[BufferSizes::MaxPath];
	      PathName::Split (it->c_str(),
			       0, 0,
			       szFileName, BufferSizes::MaxPath,
			       szExt, BufferSizes::MaxPath);
	      if (PathName::Compare(szExt, ".pl") != 0)
		{
		  continue;
		}
	      PathName pathExe
		(pathBinDir, szFileName, MIKTEX_EXE_FILE_SUFFIX);
	      Verbose ("  %s", pathExe.Get());
	      if (! File::Exists(pathExe) || overwrite)
		{
		  PrintOnly ("cp %s %s", Q_(runperl), Q_(pathExe));
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

  if (pSession->FindFile("runbat", FileType::EXE, runbat))
    {
      PackageInfo pi;
      auto_ptr<PackageIterator> pIter (pManager->CreateIterator());
      while (pIter->GetNext(pi))
	{
	  for (vector<string>::const_iterator it
		 = pi.runFiles.begin();
	       it != pi.runFiles.end();
	       ++ it)
	    {
	      char szFileName[BufferSizes::MaxPath];
	      char szExt[BufferSizes::MaxPath];
	      PathName::Split (it->c_str(),
			       0, 0,
			       szFileName, BufferSizes::MaxPath,
			       szExt, BufferSizes::MaxPath);
	      if (PathName::Compare(szExt, ".bat") != 0
		  && PathName::Compare(szExt, ".cmd") != 0)
		{
		  continue;
		}
	      PathName pathExe (pathBinDir, szFileName, ".exe");
	      Verbose ("  %s", pathExe.Get());
	      if (! File::Exists(pathExe) || overwrite)
		{
		  PrintOnly ("cp %s %s", Q_(runbat), Q_(pathExe));
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
  if (! pSession->FindFile ("mkfntmap", FileType::EXE, pathMkfontmap))
    {
      FatalError (T_("The mkfntmap executable could not be found."));
    }
  CommandLineBuilder arguments;
  if (verbose)
    {
      arguments.AppendOption ("--verbose");
    }
  if (adminMode)
  {
    arguments.AppendOption ("--admin");
  }
  if (printOnly)
    {
      PrintOnly ("mkfntmap %s", arguments.Get());
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
IniTeXMFApp::EditConfigFile (/*[in]*/ const char * lpszName)
{
  PathName configFile (pSession->GetSpecialPath(SpecialPath::ConfigRoot));
  bool haveConfigFile = false;
  for (size_t idx = 0;
       (! haveConfigFile
	&& idx < sizeof(configShortcuts) / sizeof(configShortcuts[0]));
       ++ idx)
    {
      if (PathName::Compare(lpszName, configShortcuts[idx].lpszShortcut) == 0)
	{
	  configFile += configShortcuts[idx].lpszFile;
	  haveConfigFile = true;
	}
    }
  if (! haveConfigFile)
    {
      PathName fileName (lpszName);
      fileName.RemoveDirectorySpec ();
      if (fileName == lpszName)
	{
	  configFile += MIKTEX_PATH_MIKTEX_CONFIG_DIR;
	}
      configFile += lpszName;
      configFile.SetExtension (".ini", false);
      haveConfigFile = true;
    }
  if (! File::Exists(configFile))
    {
      if (! pSession->TryCreateFromTemplate(configFile))
	{
	  Directory::Create (PathName(configFile).RemoveFileSpec());
	  StreamWriter writer (configFile);
	  writer.Close ();
	  Fndb::Add (configFile);
	}
    }
  CommandLineBuilder commandLine;
  commandLine.AppendArgument (configFile);
  string editor;
  const char * lpszEditor = getenv("EDITOR");
  if (lpszEditor != 0)
  {
    editor = lpszEditor;
  }
  else
  {
#if defined(MIKTEX_WINDOWS)
    editor = "notepad.exe";
#else
    FatalError (T_("Environment variable EDITOR is not defined."));
#endif
  }
  Process::Start (editor.c_str(), commandLine.Get());
}

/* _________________________________________________________________________

   IniTeXMFApp::ReportMiKTeXVersion
   _________________________________________________________________________ */

void
IniTeXMFApp::ReportMiKTeXVersion ()
{
  if (xml)
    {
      xmlWriter.StartElement ("setup");
      xmlWriter.StartElement ("version");
      xmlWriter.Text (Utils::GetMiKTeXVersionString());
      xmlWriter.EndElement ();
#if defined(MIKTEX_WINDOWS)
      if (IsWindowsNT())
	{
	  xmlWriter.StartElement ("systemadmin");
	  xmlWriter.AddAttribute ("value",
				  (pSession->IsUserAnAdministrator()
				   ? "true"
				   : "false"));
	  xmlWriter.EndElement ();
	  xmlWriter.StartElement ("poweruser");
	  xmlWriter.AddAttribute ("value",
				  (pSession->IsUserAPowerUser()
				   ? "true"
				   : "false"));
	  xmlWriter.EndElement ();
	  xmlWriter.StartElement ("bindir");
	  xmlWriter.Text (pSession->GetSpecialPath(SpecialPath::BinDirectory)
			  .Get());
	  xmlWriter.EndElement ();
	}
      xmlWriter.EndElement ();
#endif
    }
  else
    {
      cout << "MiKTeX: " << Utils::GetMiKTeXVersionString() << endl;
#if defined(MIKTEX_WINDOWS)
      if (IsWindowsNT())
	{
	  cout << "SystemAdmin: " << (pSession->IsUserAnAdministrator()
					  ? T_("yes")
					  : T_("no"))
	       << endl;
	  cout << "PowerUser: " << (pSession->IsUserAPowerUser()
					? T_("yes")
					: T_("no"))
	       << endl;
	}
#endif
      cout << "BinDir: "
	   << pSession->GetSpecialPath(SpecialPath::BinDirectory).Get()
	   << endl;
    }
}

/* _________________________________________________________________________

   IniTeXMFApp::ReportOSVersion
   _________________________________________________________________________ */

void
IniTeXMFApp::ReportOSVersion ()
{
  if (xml)
    {
      xmlWriter.StartElement ("os");
      xmlWriter.StartElement ("version");
      xmlWriter.Text (Utils::GetOSVersionString());
      xmlWriter.EndElement ();
      xmlWriter.EndElement ();
    }
  else
    {
      cout << "OS: " << Utils::GetOSVersionString() << endl;
    }
}

/* _________________________________________________________________________

   IniTeXMFApp::ReportRoots
   _________________________________________________________________________ */

void
IniTeXMFApp::ReportRoots ()
{
  if (xml)
    {
      xmlWriter.StartElement ("roots");
      for (unsigned idx = 0; idx < pSession->GetNumberOfTEXMFRoots(); ++ idx)
	{
	  xmlWriter.StartElement ("path");
	  PathName root = pSession->GetRootDirectory(idx);
	  xmlWriter.AddAttribute ("index", NUMTOSTR(idx));
	  if (root == pSession->GetSpecialPath(SpecialPath::UserInstallRoot))
	    {
	      xmlWriter.AddAttribute ("userinstall", "true");
	    }
	  if (root == pSession->GetSpecialPath(SpecialPath::UserDataRoot))
	    {
	      xmlWriter.AddAttribute ("userdata", "true");
	    }
	  if (root == pSession->GetSpecialPath(SpecialPath::UserConfigRoot))
	    {
	      xmlWriter.AddAttribute ("userconfig", "true");
	    }
	  if (root
	    == pSession->GetSpecialPath(SpecialPath::CommonInstallRoot))
	  {
	    xmlWriter.AddAttribute ("commoninstall", "true");
	  }
	  if (root
	    == pSession->GetSpecialPath(SpecialPath::CommonDataRoot))
	  {
	    xmlWriter.AddAttribute ("commondata", "true");
	  }
	  if (root
	    == pSession->GetSpecialPath(SpecialPath::CommonConfigRoot))
	  {
	    xmlWriter.AddAttribute ("commonconfig", "true");
	  }
	  xmlWriter.Text (root.Get());
	  xmlWriter.EndElement ();
	}
      xmlWriter.EndElement ();
    }
  else
    {
      for (unsigned idx = 0; idx < pSession->GetNumberOfTEXMFRoots(); ++ idx)
	{
	  PathName root = pSession->GetRootDirectory(idx);
	  cout << "Root" << idx << ": " << root.Get() << endl;
	}
      cout << "UserInstall: "
	   << pSession->GetSpecialPath(SpecialPath::UserInstallRoot).Get()
	   << endl;
      cout << "UserData: "
	   << pSession->GetSpecialPath(SpecialPath::UserDataRoot).Get()
	   << endl;
      cout << "UserConfig: "
	   << pSession->GetSpecialPath(SpecialPath::UserConfigRoot).Get()
	   << endl;
      cout << "CommonInstall: "
	<< pSession->GetSpecialPath(SpecialPath::CommonInstallRoot).Get()
	<< endl;
      cout << "CommonData: "
	<< pSession->GetSpecialPath(SpecialPath::CommonDataRoot).Get()
	<< endl;
      cout << "CommonConfig: "
	<< (pSession->GetSpecialPath(SpecialPath::CommonConfigRoot).Get())
	<< endl;
    }
}

/* _________________________________________________________________________

   IniTeXMFApp::ReportFndbFiles
   _________________________________________________________________________ */

void
IniTeXMFApp::ReportFndbFiles ()
{
  if (xml)
    {
      xmlWriter.StartElement ("fndb");
      for (unsigned idx = 0; idx < pSession->GetNumberOfTEXMFRoots(); ++ idx)
	{
	  PathName absFileName;
	  if (pSession->FindFilenameDatabase(idx, absFileName))
	    {
	      xmlWriter.StartElement ("path");
	      xmlWriter.AddAttribute ("index", NUMTOSTR(idx));
	      xmlWriter.Text (absFileName.Get());
	      xmlWriter.EndElement ();
	    }
	}
      unsigned r = pSession->DeriveTEXMFRoot(pSession->GetMpmRootPath());
      PathName path;
      if (pSession->FindFilenameDatabase(r, path))
	{
	  xmlWriter.StartElement ("mpmpath");
	  xmlWriter.Text (path.Get());
	  xmlWriter.EndElement ();
	}
      xmlWriter.EndElement ();
    }
  else
    {
      for (unsigned idx = 0; idx < pSession->GetNumberOfTEXMFRoots(); ++ idx)
	{
	  PathName absFileName;
	  cout << "fndb" << idx << ": ";
	  if (pSession->FindFilenameDatabase(idx, absFileName))
	    {
	      cout << absFileName.Get() << endl;
	    }
	  else
	    {
	      cout << T_("<does not exist>") << endl;
	    }
	}
      unsigned r = pSession->DeriveTEXMFRoot(pSession->GetMpmRootPath());
      PathName path;
      cout << "fndbmpm: ";
      if (pSession->FindFilenameDatabase(r, path))
	{
	  cout << path.Get() << endl;
	}
      else
	{
	  cout << T_("<does not exist>") << endl;
	}
    }
}

/* _________________________________________________________________________

   IniTeXMFApp::ReportEnvironmentVariables
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
void
IniTeXMFApp::ReportEnvironmentVariables ()
{
  LPTSTR lpszEnv = reinterpret_cast<LPTSTR>(GetEnvironmentStrings ());
  if (lpszEnv == 0)
    {
      return;
    }
  xmlWriter.StartElement ("environment");
  for (LPTSTR p = lpszEnv; *p != 0; p += strlen(p) + 1)
    {
      Tokenizer tok (p, "=");
      if (tok.GetCurrent() == 0)
	{
	  continue;
	}
      xmlWriter.StartElement ("env");
      xmlWriter.AddAttribute ("name", tok.GetCurrent());
      ++ tok;
      if (tok.GetCurrent() != 0)
	{
	  xmlWriter.Text (tok.GetCurrent());
	}
      xmlWriter.EndElement ();
    }
  xmlWriter.EndElement ();
  FreeEnvironmentStrings (lpszEnv);
}
#endif

/* _________________________________________________________________________

   IniTeXMFApp::ReportBrokenPackages
   _________________________________________________________________________ */

void
IniTeXMFApp::ReportBrokenPackages ()
{
  vector<string> broken;
  auto_ptr<PackageIterator> pIter (pManager->CreateIterator());
  PackageInfo packageInfo;
  for (int idx = 0; pIter->GetNext(packageInfo); ++ idx)
    {
      if (! packageInfo.IsPureContainer()
	  && packageInfo.IsInstalled()
	  && packageInfo.deploymentName.compare (0, 7, "miktex-") == 0)
	{
	  if (! (pManager
		 ->TryVerifyInstalledPackage(packageInfo.deploymentName)))
	    {
	      broken.push_back (packageInfo.deploymentName);
	    }
	}
    }
  pIter->Dispose ();
  if (broken.size() > 0)
    {
      if (xml)
	{
	  xmlWriter.StartElement ("packages");
	  for (vector<string>::const_iterator it = broken.begin();
	       it != broken.end();
	       ++ it)
	    {
	      xmlWriter.StartElement ("package");
	      xmlWriter.AddAttribute ("name", it->c_str());
	      xmlWriter.AddAttribute ("integrity", "damaged");
	      xmlWriter.EndElement ();
	    }
	  xmlWriter.EndElement ();
	}
      else
	{
	  for (vector<string>::const_iterator it = broken.begin();
	       it != broken.end();
	       ++ it)
	    {
	      cout << *it << T_(": needs to be reinstalled") << endl;
	    }
	}
    }
}

/* _________________________________________________________________________

   IniTeXMFApp::ReportLine
   _________________________________________________________________________ */

void
IniTeXMFApp::ReportLine (/*[in]*/ const char * lpszLine)
{
  Verbose ("%s", lpszLine);
}

/* _________________________________________________________________________

   IniTeXMFApp::OnRetryableError
   _________________________________________________________________________ */

bool
IniTeXMFApp::OnRetryableError (/*[in]*/ const char * lpszMessage)
{
  lpszMessage;
  return (false);
}

/* _________________________________________________________________________

   IniTeXMFApp::OnProgress
   _________________________________________________________________________ */

bool
IniTeXMFApp::OnProgress (/*[in]*/ Notification		nf)
{
  nf;
  return (true);
}

/* _________________________________________________________________________

   IniTeXMFApp::Configure
   _________________________________________________________________________ */

#if ! MIKTEX_STANDALONE
void
IniTeXMFApp::Configure ()
{
  ProcessOutput output;
  int exitCode;
  output.Clear ();
  Process::ExecuteSystemCommand ("kpsewhich --expand-path \\$TEXMF",
				 &exitCode,
				 &output,
				 0);
  output.RemoveTrailingNewline ();
  if (exitCode == 0 && output.GetLength() > 0)
    {
      if (pSession->IsAdminMode())
	{
	  if (! startupConfig.commonRoots.empty())
	    {
	      startupConfig.commonRoots += PathName::PathNameDelimiter;
	    }
	  startupConfig.commonRoots += output.Get();
	}
      else
	{
	  if (! startupConfig.userRoots.empty())
	    {
	      startupConfig.userRoots += PathName::PathNameDelimiter;
	    }
	  startupConfig.userRoots += output.Get();
	}
      SetTeXMFRootDirectories ();
    }
  unsigned nRoots = pSession->GetNumberOfTEXMFRoots();
  for (unsigned r = 0; r < nRoots; ++ r)
    {
      if (pSession->IsAdminMode())
	{
	  if (pSession->IsCommonRootDirectory(r))
	    {
	      UpdateFilenameDatabase (r);
	    }
	}
      else
	{
	  if (! pSession->IsCommonRootDirectory(r))
	    {
	      UpdateFilenameDatabase (r);
	    }
	}
    }
}
#endif

/* _________________________________________________________________________

   IniTeXMFApp::CreatePortableSetup
   _________________________________________________________________________ */

void
IniTeXMFApp::CreatePortableSetup (/*[in]*/ const PathName & portableRoot)
{
  SmartPointer<Cfg> pConfig (Cfg::Create());
  pConfig->PutValue ("Auto", "Config", "Portable");
  PathName configDir (portableRoot);
  configDir += MIKTEX_PATH_MIKTEX_CONFIG_DIR;
  Directory::Create (configDir);
  PathName startupFile (portableRoot);
  startupFile += MIKTEX_PATH_STARTUP_CONFIG_FILE;
  pConfig->Write (startupFile);
}

/* _________________________________________________________________________

   IniTeXMFApp::WriteReport
     
   Show configuration settings.
   _________________________________________________________________________ */

void
IniTeXMFApp::WriteReport ()
{
  if (xml)
    {
      xmlWriter.StartDocument ();
      xmlWriter.StartElement ("miktexreport");
    }
  ReportMiKTeXVersion ();
  ReportOSVersion ();
  ReportRoots ();
  if (xml)
    {
      ReportFndbFiles ();
    }
#if defined(MIKTEX_WINDOWS)
  if (xml)
    {
      ReportEnvironmentVariables ();
    }
#endif
  ReportBrokenPackages ();
  if (xml)
    {
      xmlWriter.EndElement ();
    }  
}

/* _________________________________________________________________________
   
   IniTeXMFApp::OnFndbItem
   _________________________________________________________________________ */

bool
IniTeXMFApp::OnFndbItem (/*[in]*/ const char *	lpszPath,
			 /*[in]*/ const char *	lpszName,
			 /*[in]*/ const char *	lpszInfo,
			 /*[in]*/ bool		isDirectory)
{
  if (recursive)
    {
      PathName path (lpszPath, lpszName);
      const char * lpszRel =
	Utils::GetRelativizedPath(path.Get(), enumDir.Get());
      if (! isDirectory)
	{
	  if (lpszInfo == 0)
	    {
	      cout << lpszRel
		   << endl;
	    }
	  else
	    {
	      if (csv)
		{
		  cout << lpszRel
		       << ';'
		       << lpszInfo
		       << endl;
		}
	      else
		{
		  cout << lpszRel
		       << " (\"" << lpszInfo << "\")"
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
	  cout << (isDirectory ? 'D' : ' ') << lpszName << endl;
	}
      else
	{
	  cout << (isDirectory ? 'D' : ' ') << lpszName
	       << " (\"" << lpszInfo << "\")"
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
		  /*[in]*/ const char **	argv)
{
  vector<string> addFiles;
  vector<string> editConfigFiles;
  vector<string> formats;
  vector<string> formatsByName;
  vector<string> listDirectories;
  vector<string> removeFiles;
  vector<string> updateRoots;
  string defaultPaperSize;
  string engine;
  string logFile;
  string portableRoot;

#if ! MIKTEX_STANDALONE
  bool optConfigure = false;
#endif

  bool optDump = false;
  bool optDumpByName = false;
  bool optForce = false;
  bool optMakeMaps = false;
  bool optListFormats = false;
  bool optListModes = false;
  bool optMakeLinks = false;
  bool optPortable = false;
  bool optReport = false;
  bool optUpdateFilenameDatabase = false;
  bool optVersion = false;

  Cpopt popt (argc,
	      argv,
	      (setupWizardRunning
	       ? aoption_setup
	       : (updateWizardRunning
		  ? aoption_update
		  : aoption_user)));

  int option;

  while ((option = popt.GetNextOpt()) >= 0)
    {
      const char * lpszOptArg = popt.GetOptArg();
      switch (option)
	{

	case OPT_ADD_FILE:

	  addFiles.push_back (lpszOptArg);
	  break;

#if ! MIKTEX_STANDALONE
	case OPT_CONFIGURE:
	  optConfigure = true;
	  break;
#endif

	case OPT_CSV:
	  csv = true;
	  break;

	case OPT_DEFAULT_PAPER_SIZE:

	  defaultPaperSize = lpszOptArg;
	  break;

	case OPT_DUMP:

	  if (lpszOptArg != 0)
	    {
	      formats.push_back (lpszOptArg);
	    }
	  optDump = true;
	  break;

	case OPT_DUMP_BY_NAME:

	  formatsByName.push_back (lpszOptArg);
	  optDumpByName = true;
	  break;

	case OPT_EDIT_CONFIG_FILE:

	  editConfigFiles.push_back (lpszOptArg);
	  break;

	case OPT_ENGINE:
	  engine = lpszOptArg;
	  break;

	case OPT_FORCE:

	  optForce = true;
	  break;

	case OPT_COMMON_INSTALL:

	  startupConfig.commonInstallRoot = lpszOptArg;
	  break;

	case OPT_USER_INSTALL:

	  startupConfig.userInstallRoot = lpszOptArg;
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

	case OPT_PORTABLE:

	  portableRoot = lpszOptArg;
	  optPortable = true;
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

	case OPT_COMMON_ROOTS:

	  startupConfig.commonRoots = lpszOptArg;
	  break;

	case OPT_USER_ROOTS:

	  startupConfig.userRoots = lpszOptArg;
	  break;

	case OPT_ADMIN:

	  adminMode = true;
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

	case OPT_XML:
	  xml = true;
	  break;

	}
    }

  if (option != -1)
    {
      string msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += ": ";
      msg += popt.Strerror(option);
      FatalError ("%s", msg.c_str());
    }
      
  if (popt.GetArgs() != 0)
    {
      FatalError (T_("This utility does not accept non-option arguments."));
    }

  if (optVersion)
    {
      cout << Utils::MakeProgramVersionString(TheNameOfTheGame,
					      MIKTEX_COMPONENT_VERSION_STR)
	   << T_("\n\
Copyright (C) 1996-2009 Christian Schenk\n				\
This is free software; see the source for copying conditions.  There is NO\n \
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

  if (adminMode)
  {
    if (! pSession->RunningAsAdministrator())
    {
#if defined(MIKTEX_WINDOWS)
      FatalError (T_("Option --admin requires administrator privileges."));
#else
      FatalError (T_("Option --admin requires root privileges."));
#endif
    }
    pSession->SetAdminMode (true);
  }

  if (optPortable)
  {
    CreatePortableSetup (portableRoot);
  }

  if (! startupConfig.userRoots.empty()
    || ! startupConfig.userDataRoot.Empty()
    || ! startupConfig.userConfigRoot.Empty()
    || ! startupConfig.userInstallRoot.Empty()
    || ! startupConfig.commonRoots.empty()
    || ! startupConfig.commonDataRoot.Empty()
    || ! startupConfig.commonConfigRoot.Empty()
    || ! startupConfig.commonInstallRoot.Empty())
  {
#if ! MIKTEX_STANDALONE
    optConfigure = true;
#else
    SetTeXMFRootDirectories ();
#endif
  }

  if (! defaultPaperSize.empty())
    {
      pSession->SetDefaultPaperSize (defaultPaperSize.c_str());
    }

  if (optDump)
    {
      MakeFormatFiles (formats);
    }

  if (optDumpByName)
    {
      MakeFormatFilesByName (formatsByName, engine);
    }

  if (optMakeLinks)
    {
      MakeLinks (optForce);
    }

  if (optMakeMaps)
    {
      MakeMaps ();
    }

  for (vector<string>::const_iterator it = addFiles.begin();
       it != addFiles.end();
       ++ it)
    {
      Verbose (T_("Adding %s to the file name database..."), Q_(*it));
      PrintOnly ("fndbadd %s", Q_(*it));
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

  for (vector<string>::const_iterator it = removeFiles.begin();
       it != removeFiles.end();
       ++ it)
    {
      Verbose (T_("Removing %s from the file name database..."), Q_(*it));
      PrintOnly ("fndbremove %s", Q_(*it));
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
	      if (pSession->IsAdminMode())
		{
		  if (pSession->IsCommonRootDirectory(r))
		    {
		      UpdateFilenameDatabase (r);
		    }
		}
	      else
		{
		  if (! pSession->IsCommonRootDirectory(r))
		    {
		      UpdateFilenameDatabase (r);
		    }
		}
	    }
	  pManager->CreateMpmFndb ();
	}
      else
	{
	  for (vector<string>::const_iterator it = updateRoots.begin();
	       it != updateRoots.end();
	       ++ it)
	    {
	      UpdateFilenameDatabase (it->c_str());
	    }
	}
    }

  for (vector<string>::const_iterator it = listDirectories.begin();
       it != listDirectories.end();
       ++ it)
    {
      enumDir = *it;
      Fndb::Enumerate (*it, this);
    }

  for (vector<string>::const_iterator it = editConfigFiles.begin();
       it != editConfigFiles.end();
       ++ it)
    {
      EditConfigFile (it->c_str());
    }

  if (optListFormats)
    {
      ListFormats ();
    }

  if (optListModes)
    {
      ListMetafontModes ();
    }

#if ! MIKTEX_STANDALONE
  if (optConfigure)
    {
      Configure ();
    }
#endif

  if (optReport)
    {
      WriteReport ();
    }
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)
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
