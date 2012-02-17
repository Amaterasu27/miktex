/* initexmf.cpp: MiKTeX configuration utility

   Copyright (C) 1996-2012 Christian Schenk

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
    printf ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  }

public:
  void
  StartElement (/*[in]*/ const char *	lpszName)
  {
    if (freshElement)
    {
      putchar ('>');
    }
    printf ("<%s", lpszName);
    freshElement = true;
    elements.push (lpszName);
  }

public:
  void
  AddAttribute (/*[in]*/ const char *	lpszAttributeName,
		/*[in]*/ const char *	lpszAttributeValue)
  {
    printf (" %s=\"%s\"", lpszAttributeName, lpszAttributeValue);
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
      printf ("/>");
      freshElement = false;
    }
    else
    {
      printf ("</%s>", elements.top().c_str());
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
	putchar ('>');
	freshElement = false;
      }
    for (const char * lpszText = text.c_str();
	 *lpszText != 0;
	 ++ lpszText)
      {
	switch (*lpszText)
	  {
	  case '&':
	    printf ("&amp;");
	    break;
	  case '<':
	    printf ("&lt;");
	    break;
	  case '>':
	    printf ("&gt;");
	    break;
	  default:
	    putchar (*lpszText);
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
#if defined(MIKTEX_WINDOWS)
  void
  SetTeXMFRootDirectories (/*[in]*/ bool noRegistry);
#else
  void
  SetTeXMFRootDirectories ();
#endif
  
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
  MakeMaps (/*[in]*/ bool force);

private:
  void
  CreateConfigFile (/*[in]*/ const char * lpszRelPath,
		    /*[in]*/ bool	  edit);

private:
  void
  SetConfigValue (/*[in]*/ const char * lpszValueSpec);

private:
  void
  ShowConfigValue (/*[in]*/ const char * lpszValueSpec);

private:
  void
  MakeLinks (/*[in]*/ bool force);

private:
  void
  MakeLanguageDat (/*[in]*/ bool force);

private:
  void
  RegisterRoots (/*[in]*/ const vector<PathName> &  roots,
		 /*[in]*/ bool			    reg);

#if defined(MIKTEX_WINDOWS)
private:
  void
  RegisterShellFileTypes (/*[in]*/ bool reg);
#endif

private:
  void
  ModifyPath ();

private:
  void
  MakeLink (/*[in]*/ const PathName & source,
            /*[in]*/ const PathName & dest,
	    /*[in]*/ bool	      overwrite);


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
  OPT_REGISTER_ROOT,
  OPT_QUIET,
  OPT_UNREGISTER_ROOT,
  OPT_REPORT,
  OPT_UPDATE_FNDB,
  OPT_USER_ROOTS,
  OPT_VERBOSE,
  OPT_VERSION,

  OPT_ADD_FILE,			// <experimental/>
  OPT_CREATE_CONFIG_FILE,	// <experimental/>
  OPT_CSV,			// <experimental/>
  OPT_LIST_DIRECTORY,		// <experimental/>
  OPT_LIST_FORMATS,		// <experimental/>
  OPT_MODIFY_PATH,		// <experimental/>
  OPT_RECURSIVE,		// <experimental/>
  OPT_REGISTER_SHELL_FILE_TYPES,	// <experimental/>
  OPT_REMOVE_FILE,		// <experimental/>
  OPT_SET_CONFIG_VALUE,		// <experimental/>
  OPT_SHOW_CONFIG_VALUE,		// <experimental/>
  OPT_UNREGISTER_SHELL_FILE_TYPES,	// <experimental/>
  OPT_XML,			// <experimental/>

  OPT_COMMON_CONFIG,		// <internal/>
  OPT_COMMON_DATA,		// <internal/>
  OPT_COMMON_INSTALL,		// <internal/>
  OPT_COMMON_ROOTS,		// <internal/>
  OPT_MKLANGS,			// <internal/>
  OPT_LOG_FILE,			// <internal/>
  OPT_DEFAULT_PAPER_SIZE,	// <internal/>
#if defined(MIKTEX_WINDOWS)
  OPT_NO_REGISTRY,		// <internal/>
#endif
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
    "create-config-file", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_CREATE_CONFIG_FILE,
    T_("\
Create the specified configuration file."),
    T_("CONFIGFILE")
  },

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
    "mklangs", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_MKLANGS,
    T_("Create language.dat, language.dat.lua and language.def."),
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
    "modify-path", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_MODIFY_PATH,
    T_("Modify the environmaent variable PATH."),
    0,
  },

#if defined(MIKTEX_WINDOWS)
  {
    "no-registry", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_NO_REGISTRY,
    T_("Do not use the Windows Registry to store configuration settings."),
    0
  },
#endif

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
    "register-root", 0,
    POPT_ARG_STRING, 0,
    OPT_REGISTER_ROOT,
    T_("Register a TEXMF root directory."),
    T_("DIR"),
  },

#if defined(MIKTEX_WINDOWS)
  {
    "register-shell-file-types", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_REGISTER_SHELL_FILE_TYPES,
    T_("Register shell file types."),
    0,
  },
#endif

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
    "set-config-value", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_SET_CONFIG_VALUE,
    T_("\
Set the specified configuration value."),
    T_("[SECTION]VALUENAME=VALUE")
  },

  {
    "show-config-value", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_SHOW_CONFIG_VALUE,
    T_("\
Show the specified configuration value."),
    T_("[SECTION]VALUENAME")
  },

  {
    "unregister-root", 0,
    POPT_ARG_STRING, 0,
    OPT_UNREGISTER_ROOT,
    T_("Unegister a TEXMF root directory."),
    T_("DIR"),
  },

#if defined(MIKTEX_WINDOWS)
  {
    "unregister-shell-file-types", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_UNREGISTER_SHELL_FILE_TYPES,
    T_("Unregister shell file types."),
    0,
  },
#endif

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
    "create-config-file", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_CREATE_CONFIG_FILE,
    T_("\
Create the specified configuration file."),
    T_("CONFIGFILE")
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
    "mklangs", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_MKLANGS,
    T_("Create language.dat, language.dat.lua and language.def."),
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
    "modify-path", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_MODIFY_PATH,
    T_("Modify the environmaent variable PATH."),
    0,
  },

#if defined(MIKTEX_WINDOWS)
  {
    "no-registry", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_NO_REGISTRY,
    T_("Do not use the Windows Registry to store configuration settings."),
    0
  },
#endif

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
    "register-root", 0,
    POPT_ARG_STRING, 0,
    OPT_REGISTER_ROOT,
    T_("Register a TEXMF root directory."),
    T_("DIR"),
  },

#if defined(MIKTEX_WINDOWS)
  {
    "register-shell-file-types", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_REGISTER_SHELL_FILE_TYPES,
    T_("Register shell file types."),
    0,
  },
#endif

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
    "set-config-value", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_SET_CONFIG_VALUE,
    T_("\
Set the specified configuration value."),
    T_("[SECTION]VALUENAME=VALUE")
  },

  {
    "show-config-value", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_SHOW_CONFIG_VALUE,
    T_("\
Show the specified configuration value."),
    T_("[SECTION]VALUENAME")
  },

  {
    "unregister-root", 0,
    POPT_ARG_STRING, 0,
    OPT_UNREGISTER_ROOT,
    T_("Unegister a TEXMF root directory."),
    T_("DIR"),
  },

#if defined(MIKTEX_WINDOWS)
  {
    "unregister-shell-file-types", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_UNREGISTER_SHELL_FILE_TYPES,
    T_("Unregister shell file types."),
    0,
  },
#endif

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
    "create-config-file", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_CREATE_CONFIG_FILE,
    T_("\
Create the specified configuration file."),
    T_("CONFIGFILE")
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
    "mklangs", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_MKLANGS,
    T_("Create language.dat, language.dat.lua and language.def."),
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
    "modify-path", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_MODIFY_PATH,
    T_("Modify the environmaent variable PATH."),
    0,
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
    "register-root", 0,
    POPT_ARG_STRING, 0,
    OPT_REGISTER_ROOT,
    T_("Register a TEXMF root directory."),
    T_("DIR"),
  },

#if defined(MIKTEX_WINDOWS)
  {
    "register-shell-file-types", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_REGISTER_SHELL_FILE_TYPES,
    T_("Register shell file types."),
    0,
  },
#endif

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
    "set-config-value", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_SET_CONFIG_VALUE,
    T_("\
Set the specified configuration value."),
    T_("[SECTION]VALUENAME=VALUE")
  },

  {
    "show-config-value", 0,
    POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_SHOW_CONFIG_VALUE,
    T_("\
Show the specified configuration value."),
    T_("[SECTION]VALUENAME")
  },

  {
    "unregister-root", 0,
    POPT_ARG_STRING, 0,
    OPT_UNREGISTER_ROOT,
    T_("Unegister a TEXMF root directory."),
    T_("DIR"),
  },

#if defined(MIKTEX_WINDOWS)
  {
    "unregister-shell-file-types", 0,
    POPT_ARG_NONE | POPT_ARGFLAG_DOC_HIDDEN, 0,
    OPT_UNREGISTER_SHELL_FILE_TYPES,
    T_("Unregister shell file types."),
    0,
  },
#endif

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
  wchar_t szText[200];
  if (GetWindowTextW(hwnd, szText, 200) == 0)
    {
      return (TRUE);
    }
  if (wcsstr(szText, L"MiKTeX") != 0)
    {
      if (wcsstr(szText, L"Update") != 0)
	{
	  This->updateWizardRunning = true;
	}
      else if (wcsstr(szText, L"Setup") != 0
	       || wcsstr(szText, L"Installer") != 0)
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
  vprintf (lpszFormat, arglist);
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
  vprintf (lpszFormat, arglist);
  va_end (arglist);
  putchar ('\n');
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
  vprintf (lpszFormat, arglist);
  va_end (arglist);
  putchar ('\n');
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
  fprintf (stderr, "%s: warning: ", PROGNAME);
  va_list arglist;
  va_start (arglist, lpszFormat);
  vfprintf (stderr, lpszFormat, arglist);
  va_end (arglist);
  putc ('\n', stderr);
}

/* _________________________________________________________________________
     
   IniTeXMFApp::FatalError
   _________________________________________________________________________ */
  
MIKTEXNORETURN
void
IniTeXMFApp::FatalError (/*[in]*/ const char *	lpszFormat,
			 /*[in]*/		...)
{
  fprintf (stderr, "%s: ", PROGNAME);
  va_list arglist;
  va_start (arglist, lpszFormat);
  vfprintf (stderr, lpszFormat, arglist);
  va_end (arglist);
  putc ('\n', stderr);
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
  if (pSession->IsCommonRootDirectory(rootIdx))
    {
      Verbose (T_("Creating fndb for common root directory (%s)..."),
	       Q_(root));
    }
  else
    {
      Verbose (T_("Creating fndb for user root directory (%s)..."),
	       Q_(root));
    }
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
    printf ("%s (%s)\n", formatInfo.key.c_str(), formatInfo.description.c_str());
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
    printf ("%-8s  %5dx%-5d  %s\n", mode.szMnemonic, mode.iHorzRes, mode.iVertRes, mode.szDescription);
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
IniTeXMFApp::SetTeXMFRootDirectories (
#if defined(MIKTEX_WINDOWS)
				      /*[in]*/ bool noRegistry
#endif
				      )
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
    RegisterRootDirectoriesFlags flags (RegisterRootDirectoriesFlags::None);
#if defined(MIKTEX_WINDOWS)
    if (noRegistry)
    {
      flags |= RegisterRootDirectoriesFlags::NoRegistry;
    }
#endif
    pSession->RegisterRootDirectories (startupConfig, flags.Get());
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
     
   IniTeXMFApp::MakeLink
   _________________________________________________________________________ */

void
IniTeXMFApp::MakeLink (/*[in]*/ const PathName &  source,
		       /*[in]*/ const PathName &  dest,
		       /*[in]*/ bool		  overwrite)
{
  if (! File::Exists(dest) || overwrite)
  {
    PrintOnly ("cp %s %s", Q_(source), Q_(dest));
    if (! printOnly)
    {
      File::Copy (source, dest);
    }
  }
  if (logStream.IsOpen())
  {
    logStream.WriteLine (dest.Get());
  }
}

/* _________________________________________________________________________
     
   IniTeXMFApp::RegisterRoots
   _________________________________________________________________________ */

void
IniTeXMFApp::RegisterRoots (/*[in]*/ const vector<PathName> &	roots,
			    /*[in]*/ bool			reg)
{
  string newRoots;

  PathName userInstallRoot;
  PathName userConfigRoot;
  PathName userDataRoot;

  if (! pSession->IsAdminMode())
  {
    userInstallRoot = pSession->GetSpecialPath(SpecialPath::UserInstallRoot);
    userConfigRoot = pSession->GetSpecialPath(SpecialPath::UserConfigRoot);
    userDataRoot = pSession->GetSpecialPath(SpecialPath::UserDataRoot);
  }

  PathName commonInstallRoot = pSession->GetSpecialPath(SpecialPath::CommonInstallRoot);
  PathName commonConfigRoot = pSession->GetSpecialPath(SpecialPath::CommonConfigRoot);
  PathName commonDataRoot = pSession->GetSpecialPath(SpecialPath::CommonDataRoot);

  for (unsigned r = 0; r < pSession->GetNumberOfTEXMFRoots(); ++ r)
  {
    PathName root = pSession->GetRootDirectory(r);
    int rootOrdinal = pSession->DeriveTEXMFRoot(root);
    if (pSession->IsAdminMode() && ! pSession->IsCommonRootDirectory(rootOrdinal))
    {
      continue;
    }
    if (! pSession->IsAdminMode()
        && (pSession->IsCommonRootDirectory(rootOrdinal)
	    || root == userInstallRoot
	    || root == userConfigRoot
	    || root == userDataRoot))
    {
      continue;
    }
    if (root == commonInstallRoot
      || root == commonConfigRoot
      || root == commonDataRoot)
    {
      continue;
    }
    if (! reg)
    {
      bool toBeUnregistered = false;
      for (vector<PathName>::const_iterator it = roots.begin(); it != roots.end() && ! toBeUnregistered; ++ it)
      {
	if (*it == root)
	{
	  toBeUnregistered = true;
	}
      }
      if (toBeUnregistered)
      {
	continue;
      }
    }
    if (! newRoots.empty())
    {
      newRoots += PathName::PathNameDelimiter;
    }
    newRoots += root.Get();
  }

  if (reg)
  {
    for (vector<PathName>::const_iterator it = roots.begin(); it != roots.end(); ++ it)
    {
      if (! newRoots.empty())
      {
	newRoots += PathName::PathNameDelimiter;
      }
      newRoots += it->Get();
    }
  }

  pSession->RegisterRootDirectories(newRoots);

  if (reg)
  {
    for (vector<PathName>::const_iterator it = roots.begin(); it != roots.end(); ++ it)
    {
      UpdateFilenameDatabase (*it);
    }
  }
}

/* _________________________________________________________________________
     
   IniTeXMFApp::RegisterShellFileTypes
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)

struct ShellFileType {
  const char * lpszComponent;
  const char * lpszExtension;
  const char * lpszUserFriendlyName;
  const char * lpszExecutable;
  int iconIndex;
  bool takeOwnership;
  const char * lpszVerb;
  const char * lpszCommandArgs;
  const char * lpszDdeArgs;
} const shellFileTypes[] = {
  "bib", ".bib", "BibTeX Database", MIKTEX_TEXWORKS_EXE, -2, false, "open", "\"%1\"", 0,
  "cls", ".cls", "LaTeX Class", MIKTEX_TEXWORKS_EXE, -2, false, "open", "\"%1\"", 0,
  "dtx", ".dtx", "LaTeX Macros", MIKTEX_TEXWORKS_EXE, -2, false, "open", "\"%1\"", 0,
  "dvi", ".dvi", "DVI File", MIKTEX_YAP_EXE, 1, false, "open", "/dde", "[open(\"%1\")]",
  "dvi", 0, 0, MIKTEX_YAP_EXE, INT_MAX, false, "print", "/dde", "[print(\"%1\")]",
  "dvi", 0, 0, MIKTEX_YAP_EXE, INT_MAX, false, "printto", "/dde", "[printto(\"%1\",\"%2\",\"%3\",\"%4\")]",
  "ltx", ".ltx", "LaTeX Document", MIKTEX_TEXWORKS_EXE, -2, false, "open", "\"%1\"", 0,
  "pdf", ".pdf", "PDF File", MIKTEX_TEXWORKS_EXE, INT_MAX, false, "open", "\"%1\"", 0,
  "sty", ".sty", "LaTeX Style", MIKTEX_TEXWORKS_EXE, -2, false, "open", "\"%1\"", 0,
  "tex", ".tex", "TeX Document", MIKTEX_TEXWORKS_EXE, -2, false, "open", "\"%1\"", 0,
};

void
IniTeXMFApp::RegisterShellFileTypes (/*[in]*/ bool	  reg)
{
  for (int idx = 0; idx < sizeof(shellFileTypes) / sizeof(shellFileTypes[0]); ++ idx)
  {
    ShellFileType sft = shellFileTypes[idx];
    string progId = Utils::MakeProgId(sft.lpszComponent);
    if (reg)
    {
      PathName exe;
      if (sft.lpszExecutable != 0 && ! pSession->FindFile(sft.lpszExecutable, FileType::EXE, exe))
      {
	FatalError (T_("Could not find %s."), sft.lpszExecutable);
      }
      string command;
      if (sft.lpszExecutable != 0 && sft.lpszCommandArgs != 0)
      {
	command = '\"';
	command += exe.Get();
	command += "\" ";
	command += sft.lpszCommandArgs;
      }
      string iconPath;
      if (sft.lpszExecutable != 0 && sft.iconIndex != INT_MAX)
      {
	iconPath += exe.Get();
	iconPath += ",";
	iconPath += NUMTOSTR(sft.iconIndex);
      }
      if (sft.lpszUserFriendlyName != 0 || ! iconPath.empty())
      {
	Utils::RegisterShellFileType (
	  progId.c_str(),
	  sft.lpszUserFriendlyName,
	  (iconPath.empty() ? 0 : iconPath.c_str()));
      }
      if (sft.lpszVerb != 0 && (! command.empty() || sft.lpszDdeArgs != 0))
      {
	Utils::RegisterShellVerb (
	  progId.c_str(),
	  sft.lpszVerb,
	  (command.empty() ? 0 : command.c_str()),
	  sft.lpszDdeArgs);
      }
      if (sft.lpszExtension != 0)
      {
	Utils::RegisterShellFileAssoc (
	  sft.lpszExtension,
	  progId.c_str(),
	  sft.takeOwnership);
      }
    }
    else
    {
      Utils::UnregisterShellFileType (progId.c_str());
      if (sft.lpszExtension != 0)
      {
	Utils::UnregisterShellFileAssoc (sft.lpszExtension, progId.c_str());
      }
    }
  }
}
#endif

/* _________________________________________________________________________
     
   IniTeXMFApp::ModifyPath
   _________________________________________________________________________ */

void
IniTeXMFApp::ModifyPath ()
{
  Utils::CheckPath (true);
}

/* _________________________________________________________________________
     
   IniTeXMFApp::MakeLinks
   _________________________________________________________________________ */

void
IniTeXMFApp::MakeLinks (/*[in]*/ bool force)
{
  PathName pathBinDir = pSession->GetSpecialPath(SpecialPath::BinDirectory);
  PathName internalBinDir =
    pSession->GetSpecialPath(SpecialPath::InternalBinDirectory);

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
      if (formatInfo.noExecutable)
      {
	continue;
      }
      PathName compilerPath;
      if (! pSession->FindFile(formatInfo.compiler.c_str(),
			       FileType::EXE,
			       compilerPath))
	{
	  Warning (T_("The %s executable could not be found."),
		   formatInfo.compiler.c_str());
	  continue;
	}
      PathName tmp;
      if (overwrite
	  || ! pSession->FindFile(formatInfo.name.c_str(), FileType::EXE, tmp))
	{
	  PathName exePath
	    (pathBinDir, formatInfo.name, MIKTEX_EXE_FILE_SUFFIX);
	  if (! (compilerPath == exePath))
	  {
	    MakeLink (compilerPath, exePath, overwrite);
	  }
	}
    }

  Verbose (T_("Making script links..."));

  PathName scriptsIni = pSession->GetSpecialPath(SpecialPath::CommonInstallRoot);
  scriptsIni += MIKTEX_PATH_SCRIPTS_INI;

  if (! File::Exists(scriptsIni))
  {
    scriptsIni = pSession->GetSpecialPath(SpecialPath::UserInstallRoot);
    scriptsIni += MIKTEX_PATH_SCRIPTS_INI;
  }

  SmartPointer<Cfg> pConfig (Cfg::Create());

  pConfig->Read (scriptsIni, true);

  char szScriptEngine[BufferSizes::MaxCfgName];
  for (const char * lpszScriptEngine = pConfig->FirstKey(szScriptEngine, BufferSizes::MaxCfgName);
    lpszScriptEngine != 0;
    lpszScriptEngine = pConfig->NextKey(szScriptEngine, BufferSizes::MaxCfgName))
  {
    PathName wrapper = pSession->GetSpecialPath(SpecialPath::InternalBinDirectory);
    wrapper.AppendAltDirectoryDelimiter ();
    wrapper.Append ("run", false);
    wrapper.Append (lpszScriptEngine, false);
    wrapper.Append (MIKTEX_EXE_FILE_SUFFIX, false);
    if (! File::Exists(wrapper))
    {
      continue;
    }
    char szScript[BufferSizes::MaxCfgName];
    for (const char * lpszScript = pConfig->FirstValue(lpszScriptEngine, szScript, BufferSizes::MaxCfgName);
      lpszScript != 0;
      lpszScript = pConfig->NextValue(szScript, BufferSizes::MaxCfgName))
    {
      PathName pathExe (pathBinDir, szScript, MIKTEX_EXE_FILE_SUFFIX);
      MakeLink (wrapper, pathExe, overwrite);
    }
  }

#if defined(MIKTEX_WINDOWS)
  static const char * const copystarters[] = {
    MIKTEX_PATH_INTERNAL_TASKBAR_ICON_EXE,
    MIKTEX_PATH_INTERNAL_UPDATE_EXE,
  };

  PathName copystart;

  if (pSession->FindFile(MIKTEX_PATH_INTERNAL_COPYSTART_EXE, "%R", copystart))
  {
    for (size_t idx = 0;
      idx < sizeof(copystarters) / sizeof(copystarters[0]);
      ++ idx)
    {
      PathName pathExe (pathBinDir);
      char szFileName[BufferSizes::MaxPath];
      pathExe += PathName(copystarters[idx]).GetFileName(szFileName);
      Verbose ("  %s", pathExe.Get());
      MakeLink (copystart, pathExe, overwrite);
    }
  }

  static const char * const copystarters_admin[] = {
    MIKTEX_PATH_INTERNAL_UPDATE_ADMIN_EXE,
  };

  PathName copystart_admin;

  if (pSession->FindFile(MIKTEX_PATH_INTERNAL_COPYSTART_ADMIN_EXE,
			 "%R",
			 copystart_admin))
    {
      for (size_t idx = 0;
	   idx < sizeof(copystarters_admin) / sizeof(copystarters_admin[0]);
	   ++ idx)
	{
	  PathName pathExe (pathBinDir);
	  char szFileName[BufferSizes::MaxPath];
	  pathExe += PathName(copystarters_admin[idx]).GetFileName(szFileName);
	  MakeLink (copystart_admin, pathExe, overwrite);
	}
    }
#endif
}

/* _________________________________________________________________________
     
   IniTeXMFApp::MakeLanguageDat
   _________________________________________________________________________ */

void
IniTeXMFApp::MakeLanguageDat (/*[in]*/ bool force)
{
  Verbose (T_("Creating language.dat, language.dat.lua and language.def..."));

  if (printOnly)
  {
    return;
  }

  PathName dir;

  PathName languageDatPath = pSession->GetSpecialPath(SpecialPath::ConfigRoot);
  languageDatPath += MIKTEX_PATH_LANGUAGE_DAT;
  dir = languageDatPath;
  dir.RemoveFileSpec ();
  Directory::Create (dir);
  StreamWriter languageDat (languageDatPath);

  PathName languageDatLuaPath = pSession->GetSpecialPath(SpecialPath::ConfigRoot);
  languageDatLuaPath += MIKTEX_PATH_LANGUAGE_DAT_LUA;
  dir = languageDatLuaPath;
  dir.RemoveFileSpec ();
  Directory::Create (dir);
  StreamWriter languageDatLua (languageDatLuaPath);

  PathName languageDefPath = pSession->GetSpecialPath(SpecialPath::ConfigRoot);
  languageDefPath += MIKTEX_PATH_LANGUAGE_DEF;
  dir = languageDefPath;
  dir.RemoveFileSpec ();
  Directory::Create (dir);
  StreamWriter languageDef (languageDefPath);

  languageDatLua.WriteLine ("return {");
  languageDef.WriteLine ("%% e-TeX V2.2");

  LanguageInfo languageInfo;
  for (int idx = 0; pSession->GetLanguageInfo(idx, languageInfo); ++ idx)
  {
    if (languageInfo.exclude)
    {
      continue;
    }

    PathName loaderPath;
    if (! pSession->FindFile(languageInfo.loader.c_str(), "%r/tex//", loaderPath))
    {
      continue;
    }

    // language.dat
    languageDat.WriteFormattedLine ("%s %s", languageInfo.key.c_str(), languageInfo.loader.c_str());
    for (CSVList synonym (languageInfo.synonyms.c_str(), ','); synonym.GetCurrent() != 0; ++ synonym)
    {
      languageDat.WriteFormattedLine ("=%s", synonym.GetCurrent());
    }

    // language.def
    languageDef.WriteFormattedLine ("\\addlanguage{%s}{%s}{}{%d}{%d}",
      languageInfo.key.c_str(),
      languageInfo.loader.c_str(),
      languageInfo.lefthyphenmin,
      languageInfo.righthyphenmin);

    // language.dat.lua
    languageDatLua.WriteFormattedLine ("\t['%s'] = {", languageInfo.key.c_str());
    languageDatLua.WriteFormattedLine ("\t\tloader='%s',", languageInfo.loader.c_str());
    languageDatLua.WriteFormattedLine ("\t\tlefthyphenmin=%d,", languageInfo.lefthyphenmin);
    languageDatLua.WriteFormattedLine ("\t\trighthyphenmin=%d,", languageInfo.righthyphenmin);
    languageDatLua.Write ("\t\tsynonyms={ ");
    int nSyn = 0;
    for (CSVList synonym (languageInfo.synonyms.c_str(), ','); synonym.GetCurrent() != 0; ++ synonym)
    {
      languageDatLua.WriteFormatted ("%s'%s'", nSyn > 0 ? "," : "", synonym.GetCurrent());
      nSyn++;
    }
    languageDatLua.WriteLine (" },");
    languageDatLua.WriteFormattedLine ("\t\tpatterns='%s',", languageInfo.patterns.c_str());
    languageDatLua.WriteFormattedLine ("\t\thyphenation='%s',", languageInfo.hyphenation.c_str());
    if (! languageInfo.luaspecial.empty())
    {
      languageDatLua.WriteFormattedLine ("\t\tspecial='%s',", languageInfo.luaspecial.c_str());
    }
    languageDatLua.WriteLine ("\t},");
  }

  languageDatLua.WriteLine ("}");

  languageDatLua.Close ();
  Fndb::Add (languageDatLuaPath);

  languageDef.Close ();
  Fndb::Add (languageDefPath);

  languageDat.Close ();
  Fndb::Add (languageDatPath);
}

/* _________________________________________________________________________

   IniTeXMFApp::MakeMaps
   _________________________________________________________________________ */

void
IniTeXMFApp::MakeMaps (/*[in]*/ bool force)
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
  if (force)
  {
    arguments.AppendOption ("--force");
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

   IniTeXMFApp::CreateConfigFile
   _________________________________________________________________________ */

void
IniTeXMFApp::CreateConfigFile (/*[in]*/ const char *  lpszName,
			       /*[in]*/ bool	      edit)
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
  if (edit)
  {
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
}

/* _________________________________________________________________________

   IniTeXMFApp::SetConfigValue

   ValueSpec:
     [SECTION]NAME=VALUE
   _________________________________________________________________________ */

void
IniTeXMFApp::SetConfigValue (/*[in]*/ const char *  lpszValueSpec)
{
  const char * lpsz = lpszValueSpec;
  string section;
  bool haveSection = (*lpsz == '[');
  if (haveSection)
    {
      ++ lpsz;
      for (; *lpsz != 0 && *lpsz != ']'; ++ lpsz)
	{
	  section += *lpsz;
	}
      if (*lpsz == 0)
	{
	  FatalError (T_("Invalid value: %s."), Q_(lpszValueSpec));
	}
      ++ lpsz;
    }
  string valueName;
  for (; *lpsz != 0 && *lpsz != '='; ++ lpsz)
    {
      valueName += *lpsz;
    }
  if (*lpsz == 0)
    {
      FatalError (T_("Invalid value: %s."), Q_(lpszValueSpec));
    }
  ++ lpsz;
  string value = lpsz;
  pSession->SetConfigValue (
			    haveSection ? section.c_str() : 0,
			    valueName.c_str(),
			    value.c_str());
}

/* _________________________________________________________________________

   IniTeXMFApp::ShowConfigValue

   ValueSpec:
     [SECTION]NAME
   _________________________________________________________________________ */

void
IniTeXMFApp::ShowConfigValue (/*[in]*/ const char *  lpszValueSpec)
{
  const char * lpsz = lpszValueSpec;
  string section;
  bool haveSection = (*lpsz == '[');
  if (haveSection)
    {
      ++ lpsz;
      for (; *lpsz != 0 && *lpsz != ']'; ++ lpsz)
	{
	  section += *lpsz;
	}
      if (*lpsz == 0)
	{
	  FatalError (T_("Invalid value: %s."), Q_(lpszValueSpec));
	}
      ++ lpsz;
    }
  string valueName = lpsz;
  string value;
  if (pSession->TryGetConfigValue(haveSection ? section.c_str() : 0,
				  valueName.c_str(),
				  value))
    {
      printf ("%s\n", value.c_str());
    }
}

/* _________________________________________________________________________

   IniTeXMFApp::ReportMiKTeXVersion
   _________________________________________________________________________ */

void
IniTeXMFApp::ReportMiKTeXVersion ()
{
  vector<string> invokerNames = Process2::GetInvokerNames();
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
	}
      xmlWriter.EndElement ();
#endif
    }
  else
    {
      printf ("MiKTeX: %s\n", Utils::GetMiKTeXVersionString().c_str());
      printf (T_("Invokers: "));
      for (vector<string>::const_iterator it = invokerNames. begin(); it != invokerNames.end(); ++ it)
      {
	if (it != invokerNames.begin())
	{
	  printf ("/");
	}
	printf ("%s", it->c_str());
      }
      putchar ('\n');
#if defined(MIKTEX_WINDOWS)
      if (IsWindowsNT())
      {
	printf ("SystemAdmin: %s\n", pSession->IsUserAnAdministrator()
	  ? T_("yes")
	  : T_("no"));
	printf ("PowerUser: %s\n", pSession->IsUserAPowerUser()
	  ? T_("yes")
	  : T_("no"));
      }
#endif
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
      printf ("OS: %s\n", Utils::GetOSVersionString().c_str());
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
	printf (T_("Root %u: %s\n"), idx, root.Get());
      }
      printf (T_("UserInstall: %s\n"), pSession->GetSpecialPath(SpecialPath::UserInstallRoot).Get());
      printf (T_("UserData: %s\n"), pSession->GetSpecialPath(SpecialPath::UserDataRoot).Get());
      printf (T_("UserConfig: %s\n"), pSession->GetSpecialPath(SpecialPath::UserConfigRoot).Get());
      printf (T_("CommonInstall: %s\n"), pSession->GetSpecialPath(SpecialPath::CommonInstallRoot).Get());
      printf (T_("CommonData: %s\n"), pSession->GetSpecialPath(SpecialPath::CommonDataRoot).Get());
      printf (T_("CommonConfig: %s\n"), pSession->GetSpecialPath(SpecialPath::CommonConfigRoot).Get());
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
	printf (T_("fndb: %u"), idx);
	if (pSession->FindFilenameDatabase(idx, absFileName))
	{
	  printf ("%s\n", absFileName.Get());
	}
	else
	{
	  printf (T_("<does not exist>\n"));
	}
      }
      unsigned r = pSession->DeriveTEXMFRoot(pSession->GetMpmRootPath());
      PathName path;
      printf ("fndbmpm: ");
      if (pSession->FindFilenameDatabase(r, path))
      {
	printf ("%s\n", path.Get());;
      }
      else
      {
	printf (T_("<does not exist>\n"));
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
  wchar_t * lpszEnv = reinterpret_cast<wchar_t*>(GetEnvironmentStringsW());
  if (lpszEnv == 0)
    {
      return;
    }
  xmlWriter.StartElement ("environment");
  for (wchar_t * p = lpszEnv; *p != 0; p += wcslen(p) + 1)
    {
      Tokenizer tok (Utils::WideCharToUTF8(p).c_str(), "=");
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
	      printf (T_("%s: needs to be reinstalled\n"), it->c_str());
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
	  else
	    {
	      Verbose (T_("Skipping user root directory (%s)..."),
		       Q_(pSession->GetRootDirectory(r)));
	    }
	}
      else
	{
	  if (! pSession->IsCommonRootDirectory(r))
	    {
	      UpdateFilenameDatabase (r);
	    }
	  else
	    {
	      Verbose (T_("Skipping common root directory (%s)..."),
		       Q_(pSession->GetRootDirectory(r)));		       
	    }
	}
    }
  pManager->CreateMpmFndb ();
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
  pConfig->Write (startupFile, T_("MiKTeX startup configuration"));
  PathName tempDir (portableRoot);
  tempDir += MIKTEX_PATH_MIKTEX_TEMP_DIR;
  if (! Directory::Exists(tempDir))
  {
    Directory::Create (tempDir);
  }
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
	      printf ("%s\n", lpszRel);
	    }
	  else
	    {
	      if (csv)
		{
		  printf ("%;%s\n", lpszRel, lpszInfo);
		}
	      else
		{
		  printf ("%s (\"%s\")\n", lpszRel, lpszInfo);
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
	  printf ("%c %s\n", isDirectory ? 'D' : ' ', lpszName);
	}
      else
	{
	  printf ("%c %s (\"%s\")\n", isDirectory ? 'D' : ' ', lpszName, lpszInfo);
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
  vector<string> showConfigValue;
  vector<string> setConfigValues;
  vector<string> createConfigFiles;
  vector<string> editConfigFiles;
  vector<string> formats;
  vector<string> formatsByName;
  vector<string> listDirectories;
  vector<string> removeFiles;
  vector<string> updateRoots;
  vector<PathName> registerRoots;
  vector<PathName> unregisterRoots;
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
  bool optMakeLanguageDat = false;
  bool optMakeMaps = false;
  bool optListFormats = false;
  bool optListModes = false;
  bool optMakeLinks = false;
#if defined(MIKTEX_WINDOWS)
  bool optNoRegistry = false;
#endif
  bool optPortable = false;
  bool optRegisterShellFileTypes = false;
  bool optModifyPath = false;
  bool optReport = false;
  bool optUnRegisterShellFileTypes = false;
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

	case OPT_CREATE_CONFIG_FILE:

	  createConfigFiles.push_back (lpszOptArg);
	  break;

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

	case OPT_MKLANGS:

	  optMakeLanguageDat = true;
	  break;

	case OPT_MKLINKS:

	  optMakeLinks = true;
	  break;

	case OPT_MKMAPS:

	  optMakeMaps = true;
	  break;

	case OPT_MODIFY_PATH:

	  optModifyPath = true;
	  break;

#if defined(MIKTEX_WINDOWS)
	case OPT_NO_REGISTRY:

	  optNoRegistry = true;
	  break;
#endif

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

	case OPT_REGISTER_SHELL_FILE_TYPES:

	  optRegisterShellFileTypes = true;
	  break;

	case OPT_REGISTER_ROOT:

	  registerRoots.push_back(lpszOptArg);
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

	case OPT_SET_CONFIG_VALUE:

	  setConfigValues.push_back (lpszOptArg);
	  break;

	case OPT_SHOW_CONFIG_VALUE:

	  showConfigValue.push_back (lpszOptArg);
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

	case OPT_UNREGISTER_ROOT:

	  unregisterRoots.push_back(lpszOptArg);
	  break;

	case OPT_UNREGISTER_SHELL_FILE_TYPES:

	  optUnRegisterShellFileTypes = true;
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
      printf ("%s\n", Utils::MakeProgramVersionString(TheNameOfTheGame,
	MIKTEX_COMPONENT_VERSION_STR).c_str());
      printf (T_("\n\
Copyright (C) 1996-2012 Christian Schenk\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"));
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
#if defined(MIKTEX_WINDOWS)
    SetTeXMFRootDirectories (optNoRegistry);
#else
    SetTeXMFRootDirectories ();
#endif
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

#if defined(MIKTEX_WINDOWS)
  if (optRegisterShellFileTypes)
  {
    RegisterShellFileTypes (true);
  }
#endif

#if defined(MIKTEX_WINDOWS)
  if (optUnRegisterShellFileTypes)
  {
    RegisterShellFileTypes (false);
  }
#endif

  if (optModifyPath)
  {
    ModifyPath ();
  }

  if (optMakeLanguageDat)
  {
    MakeLanguageDat (optForce);
  }

  if (optMakeLinks)
    {
      MakeLinks (optForce);
    }

  if (optMakeMaps)
    {
      MakeMaps (optForce);
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

  if (! unregisterRoots.empty())
  {
    RegisterRoots (unregisterRoots, false);
  }

  if (! registerRoots.empty())
  {
    RegisterRoots (registerRoots, true);
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
		  else
		    {
		      Verbose (T_("Skipping user root directory (%s)..."),
			       Q_(pSession->GetRootDirectory(r)));
		    }
		}
	      else
		{
		  if (! pSession->IsCommonRootDirectory(r) || pSession->IsMiKTeXPortable())
		    {
		      UpdateFilenameDatabase (r);
		    }
		  else
		    {
		      Verbose (T_("Skipping common root directory (%s)..."),
			       Q_(pSession->GetRootDirectory(r)));
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

  for (vector<string>::const_iterator it = createConfigFiles.begin();
    it != createConfigFiles.end();
    ++ it)
  {
    CreateConfigFile (it->c_str(), false);
  }

  for (vector<string>::const_iterator it = setConfigValues.begin();
    it != setConfigValues.end();
    ++ it)
  {
    SetConfigValue (it->c_str());
  }

  for (vector<string>::const_iterator it = showConfigValue.begin();
    it != showConfigValue.end();
    ++ it)
  {
    ShowConfigValue (it->c_str());
  }

  for (vector<string>::const_iterator it = editConfigFiles.begin();
       it != editConfigFiles.end();
       ++ it)
    {
      CreateConfigFile (it->c_str(), true);
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
#if defined(_UNICODE)
wmain (/*[in]*/ int			argc,
       /*[in]*/ const wchar_t **	argv)
#else
main (/*[in]*/ int		argc,
      /*[in]*/ const char **	argv)
#endif
{
  try
    {
      vector<string> utf8args;
      utf8args.reserve (argc);
      vector<const char *> newargv;
      newargv.reserve (argc + 1);
      for (int idx = 0; idx < argc; ++ idx)
      {
#if defined(_UNICODE)
	utf8args.push_back (Utils::WideCharToUTF8(argv[idx]));
#else
	utf8args.push_back (Utils::AnsiToUTF8(argv[idx]));
#endif
	newargv.push_back (utf8args[idx].c_str());
      }
      newargv.push_back (0);
      IniTeXMFApp app;
      app.Init (newargv[0]);
      app.Run (argc, &newargv[0]);
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
