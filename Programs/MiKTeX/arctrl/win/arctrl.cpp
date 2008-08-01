/* arctrl.cpp: control Acrobat Reader

   Copyright (C) 2006-2008 Christian Schenk
*/

/* This program (arctrl) is based on public domain work by Fabrice
   Popineau. Based on Adobe documentation. */
   
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <iomanip>
#include <iostream>
#include <cstdarg>
#include <cstdio>

#include "arctrl-version.h"

#include <miktex/Core/Core>
#include <miktex/App/Application>
#include <miktex/Core/Paths>
#include <popt-miktex.h>

using namespace MiKTeX::App;
using namespace MiKTeX::Core;
using namespace MiKTeX;
using namespace std;

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) MiKTeX::Core::Quoter<char>(x).Get()

#if ! defined(UNUSED_ALWAYS)
#  define UNUSED_ALWAYS(x) static_cast<void>(x)
#endif

const char * const TheNameOfTheGame = T_("MiKTeX AR Controller");
const char * const PROGNAME = T_("arctrl");

/* _________________________________________________________________________

   ArCtrl
   _________________________________________________________________________ */

class ArCtrl : public Application
{
public:
  ArCtrl ();

public:
  virtual
  ~ArCtrl ();
  
private:
  void
  ShowVersion ();
  
private:
  MIKTEXNORETURN
  void
  FatalError (/*[in]*/ const char *	lpszFormat,
	      /*[in]*/				...);

public:
  void
  Run (/*[in]*/ int			argc,
       /*[in]*/ const char **	argv);

private:
  enum {
    None,
    Open,
    Close,
    CommandLoop
  } mode;

private:
  HSZ
  CreateDdeString (/*[in]*/ const char * lpsz);

private:
  void
  EstablishConversation ();

private:
  void
  TerminateConversation ();

private:
  void
  ExecuteDdeCommand (/*[in]*/ const char *	lpszCommand,
		     /*[in]*/				...);

private:
  void
  DocOpen (/*[in]*/ const PathName & path);

private:
  void
  DocClose (/*[in]*/ const PathName & path);

private:
  void
  CloseAllDocs ();

private:
  void
  AppShow ();

private:
  void
  AppHide ();

private:
  void
  AppExit ();

private:
  void
  DocGoTo (/*[in]*/ const PathName &	path,
	   /*[in]*/ int			pageNum);

private:
  void
  DocGoToNameDest (/*[in]*/ const PathName &	path,
		   /*[in]*/ const string &	nameDest);

private:
  void
  FileOpen (/*[in]*/ const PathName & path);

private:
  bool
  Execute (/*[in]*/ const string & command);

private:
  void
  ReadAndExecute ();

private:
  void
  ArCtrl::StartAR ();

private:
  static
  HDDEDATA
  CALLBACK
  DdeCallback (/*[in]*/ UINT		uType,
	       /*[in]*/ UINT		uFmt,
	       /*[in]*/ HCONV		hconv,
	       /*[in]*/ HSZ		hsz1,
	       /*[in]*/ HSZ		hsz2,
	       /*[in]*/ HDDEDATA	hdata,
	       /*[in]*/ DWORD		dwData1,
	       /*[in]*/ DWORD		dwData2);

private:
  HCONV hConv;

private:
  static DWORD idInst;

private:
  static const struct poptOption aoption[];
  
private:
  static const struct poptOption aoptionOpen[];
  
private:
  static const struct poptOption aoptionClose[];
  
private:
  static const struct poptOption aoptionCommandLoop[];
};

/* _________________________________________________________________________

   Option
   _________________________________________________________________________ */

enum Option
{
  OPT_AAA = 256,
  OPT_ALL,
  OPT_FILE,
  OPT_GOTO,
  OPT_PAGE,
  OPT_VERSION,
};

/* _________________________________________________________________________

   ArCtrl::aoption
   _________________________________________________________________________ */

const struct poptOption ArCtrl::aoption[] =
{
  {
    T_("version"), 'V',
    POPT_ARG_NONE, 0,
    OPT_VERSION,
    T_("Print version information and exit."),
    0
  },

  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   ArCtrl::aoptionOpen
   _________________________________________________________________________ */

const struct poptOption ArCtrl::aoptionOpen[] =
{
  {
    T_("file"), 0,
    POPT_ARG_STRING, 0,
    OPT_FILE,
    T_("Open FILE."),
    T_("FILE")
  },

  {
    T_("page"), 0,
    POPT_ARG_STRING, 0,
    OPT_PAGE,
    T_("Go to the N-th page."),
    "N",
  },

  {
    T_("goto"), 0,
    POPT_ARG_STRING, 0,
    OPT_GOTO,
    T_("Go to the specified named destination within the document."),
    T_("NAMEDEST")
  },

  {
    T_("version"), 'V',
    POPT_ARG_NONE, 0,
    OPT_VERSION,
    T_("Print version information and exit."),
    0
  },

  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   ArCtrl::aoptionClose
   _________________________________________________________________________ */

const struct poptOption ArCtrl::aoptionClose[] =
{
  {
    T_("all"), 0,
    POPT_ARG_NONE, 0,
    OPT_ALL,
    T_("Close all files."),
    0
  },

  {
    T_("file"), 0,
    POPT_ARG_STRING, 0,
    OPT_FILE,
    T_("Close FILE."),
    T_("FILE")
  },

  {
    T_("version"), 'V',
    POPT_ARG_NONE, 0,
    OPT_VERSION,
    T_("Print version information and exit."),
    0
  },

  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   ArCtrl::aoptionCommandLoop
   _________________________________________________________________________ */

const struct poptOption ArCtrl::aoptionCommandLoop[] =
{
  {
    T_("version"), 'V',
    POPT_ARG_NONE, 0,
    OPT_VERSION,
    T_("Print version information and exit."),
    0
  },

  POPT_AUTOHELP
  POPT_TABLEEND
};

/* _________________________________________________________________________

   ArCtrl::idInst
   _________________________________________________________________________ */

DWORD ArCtrl::idInst = 0;

/* _________________________________________________________________________

   ArCtrl::ArCtrl
   _________________________________________________________________________ */

ArCtrl::ArCtrl ()
  : mode (None),
    hConv (0)
{
}

/* _________________________________________________________________________

   ArCtrl::~ArCtrl
   _________________________________________________________________________ */

ArCtrl::~ArCtrl ()
{
  try
    {
      TerminateConversation ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   ArCtrl::ShowVersion
   _________________________________________________________________________ */

void
ArCtrl::ShowVersion ()
{
  cout << Utils::MakeProgramVersionString(TheNameOfTheGame,
					   VER_FILEVERSION_STR)
       << "\n"
       << T_("Written by Christian Schenk in 2006.\n")
       << T_("Based on public domain work by Fabrice Popineau.\n")
       << T_("Based on Adobe documentation.") << endl;
}

/* _________________________________________________________________________
     
   ArCtrl::FatalError
   _________________________________________________________________________ */
  
MIKTEXNORETURN
void
ArCtrl::FatalError (/*[in]*/ const char *	lpszFormat,
		    /*[in]*/			...)
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
     
   ArCtrl::CreateDdeString
   _________________________________________________________________________ */
  
HSZ
ArCtrl::CreateDdeString (/*[in]*/ const char * lpsz)
{
  HSZ hsz = DdeCreateStringHandle(idInst, lpsz, CP_WINANSI);
  if (hsz == 0)
    {
      FatalError (T_("DdeCreateStringHandle() failed for some reason."));
    }
  return (hsz);
}

/* _________________________________________________________________________

   ArCtrl::StartAR
   _________________________________________________________________________ */

void
ArCtrl::StartAR ()
{
  PathName pdfFile;
  if (! pSession->FindFile(T_("miktex.pdf"), FileType::TEXSYSDOC, pdfFile))
    {
      FatalError (T_("MiKTeX is not installed."));
    }
  PathName dir = pdfFile;
  dir.RemoveFileSpec ();
  char szExecutable[BufferSizes::MaxPath];
  if (FindExecutable(T_("miktex.pdf"), dir.Get(), szExecutable)
      <= reinterpret_cast<HINSTANCE>(32))
    {
      FatalError (T_("The PDF viewer could not be located."));
    }
  auto_ptr<Process> pProcess (Process::Start(ProcessStartInfo(szExecutable)));
#if 0
  // <todo>
  WaitForInputIdle (hProcess, INFINITE);
  // </todo>
#endif
}

/* _________________________________________________________________________

   ArCtrl::DdeCallback
   _________________________________________________________________________ */

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  Session::FatalMiKTeXError (miktexFunction,				\
			     traceMessage,				\
			     lpszInfo,					\
			     __FILE__,				\
			     __LINE__)

HDDEDATA
CALLBACK
ArCtrl::DdeCallback (/*[in]*/ UINT	uType,
		     /*[in]*/ UINT	uFmt,
		     /*[in]*/ HCONV	hconv,
		     /*[in]*/ HSZ	hsz1,
		     /*[in]*/ HSZ	hsz2,
		     /*[in]*/ HDDEDATA	hdata,
		     /*[in]*/ DWORD	dwData1,
		     /*[in]*/ DWORD	dwData2)
{
  UNUSED_ALWAYS (dwData1);
  UNUSED_ALWAYS (dwData2);
  UNUSED_ALWAYS (hconv);
  UNUSED_ALWAYS (hsz1);
  UNUSED_ALWAYS (hsz2);
  UNUSED_ALWAYS (uFmt);
  DWORD_PTR ret = DDE_FNOTPROCESSED;
  try
    {
      switch (uType)
	{
	case XTYP_ADVDATA:
	  {
	    DWORD len = DdeGetData(hdata, 0, 0, 0);
	    if (DdeGetLastError(idInst) != DMLERR_NO_ERROR)
	      {
		FATAL_MIKTEX_ERROR ("ArCtrl::DdeCallback",
				    T_("DdeGetData() failed for some reason."),
				    0);
	      }
	    CharBuffer<char> buf (len + 1);
	    DdeGetData (hdata,
			reinterpret_cast<LPBYTE>(buf.GetBuffer()),
			len + 1,
			0);
	    if (DdeGetLastError(idInst) != DMLERR_NO_ERROR)
	      {
		FATAL_MIKTEX_ERROR ("ArCtrl::DdeCallback",
				    T_("DdeGetData() failed for some reason."),
				    0);
	      }
	    ret = DDE_FACK;
	  }
	}
    }
  catch (const exception &)
    {
      ret = DDE_FNOTPROCESSED;
    }
  return (reinterpret_cast<HDDEDATA>(ret));
}

/* _________________________________________________________________________

   ArCtrl::EstablishConversation
   _________________________________________________________________________ */

void
ArCtrl::EstablishConversation ()
{
  DWORD idInst = 0;

  UINT ret =
    DdeInitialize(&idInst,
		  DdeCallback,
		  APPCMD_CLIENTONLY,
		  0);

  if (ret != DMLERR_NO_ERROR)
    {
      FatalError (T_("The DDE library could not be initialized."));
    }

  this->idInst = idInst;

  HSZ hszServer = CreateDdeString(T_("acroview"));

  AutoDdeFreeStringHandle autoFree1 (idInst, hszServer);

  HSZ hszTopic = CreateDdeString(T_("control"));

  AutoDdeFreeStringHandle autoFree2 (idInst, hszTopic);

  hConv = DdeConnect(idInst, hszServer, hszTopic, 0);

  if (hConv == 0)
    {
      StartAR ();
      for (int i = 0; i < 5; ++ i)
	{
	  Sleep (500);
	  hConv = DdeConnect(idInst, hszServer, hszTopic, 0);
	  if (hConv != 0)
	    {
	      break;
	    }
	}
      if (hConv == 0)
	{
	  FatalError (T_("The DDE conversation could not be established."));
	}
    }
}

/* _________________________________________________________________________

   ArCtrl::TerminateConversation
   _________________________________________________________________________ */

void
ArCtrl::TerminateConversation ()
{
  if (hConv != 0)
    {
      if (! DdeDisconnect(hConv))
	{
	  FatalError (T_("DDE conversation could not be terminated."));
	}
      hConv = 0;
    }
  if (idInst != 0)
    {
      if (! DdeUninitialize(idInst))
	{
	  FatalError (T_("DDE resources could not be freed."));
	}
      idInst = 0;
    }
}
  
/* _________________________________________________________________________

   ArCtrl::ExecuteDdeCommand
   _________________________________________________________________________ */

void
ArCtrl::ExecuteDdeCommand (/*[in]*/ const char *	lpszCommand,
			   /*[in]*/			...)
{
  va_list arglist;
  va_start (arglist, lpszCommand);
  string data = Utils::FormatString(lpszCommand, arglist);
  HDDEDATA h =
    DdeClientTransaction(const_cast<BYTE *>
			 (reinterpret_cast<const BYTE *>(data.c_str())),
			 static_cast<DWORD>
			 ((data.length() + 1) * sizeof(char)),
			 hConv,
			 0,
			 0,
			 XTYP_EXECUTE,
			 5000,
			 0);
  if (h == 0)
    {
      FatalError (T_("The command could not be executed."));
    }
  va_end (arglist);
}

/* _________________________________________________________________________

   ArCtrl::DocOpen
   _________________________________________________________________________ */

void
ArCtrl::DocOpen (/*[in]*/ const PathName & path)
{
  if (! File::Exists(path))
    {
      FatalError (T_("The specified file could not be found."));
    }
  PathName fullPath (path);
  fullPath.MakeAbsolute ();
  ExecuteDdeCommand (T_("[DocOpen(\"%s\")]"), fullPath.Get());
}

/* _________________________________________________________________________

   ArCtrl::DocClose
   _________________________________________________________________________ */

void
ArCtrl::DocClose (/*[in]*/ const PathName & path)
{
  if (! File::Exists(path))
    {
      FatalError (T_("The specified file could not be found."));
    }
  PathName fullPath (path);
  fullPath.MakeAbsolute ();
  ExecuteDdeCommand (T_("[DocClose(\"%s\")]"), fullPath.Get());
}

/* _________________________________________________________________________

   ArCtrl::CloseAllDocs
   _________________________________________________________________________ */

void
ArCtrl::CloseAllDocs ()
{
  ExecuteDdeCommand (T_("[CloseAllDocs()]"));
}

/* _________________________________________________________________________

   ArCtrl::AppShow
   _________________________________________________________________________ */

void
ArCtrl::AppShow ()
{
  ExecuteDdeCommand (T_("[AppShow()]"));
}

/* _________________________________________________________________________

   ArCtrl::AppHide
   _________________________________________________________________________ */

void
ArCtrl::AppHide ()
{
  ExecuteDdeCommand (T_("[AppHide()]"));
}

/* _________________________________________________________________________

   ArCtrl::AppExit
   _________________________________________________________________________ */

void
ArCtrl::AppExit ()
{
  ExecuteDdeCommand (T_("[AppExit()]"));
}

/* _________________________________________________________________________

   ArCtrl::DocGoTo
   _________________________________________________________________________ */

void
ArCtrl::DocGoTo (/*[in]*/ const PathName &	path,
		 /*[in]*/ int			pageNum)
{
  if (! File::Exists(path))
    {
      FatalError (T_("The specified file could not be found."));
    }
  PathName fullPath (path);
  fullPath.MakeAbsolute ();
  ExecuteDdeCommand (T_("[DocGoTo(\"%s\",%d)]"),
		     fullPath.Get(),
		     pageNum);
}

/* _________________________________________________________________________

   ArCtrl::DocGoToNameDest
   _________________________________________________________________________ */

void
ArCtrl::DocGoToNameDest (/*[in]*/ const PathName &	path,
			 /*[in]*/ const string &	nameDest)
{
    if (! File::Exists(path))
    {
      FatalError (T_("The specified file could not be found."));
    }
  PathName fullPath (path);
  fullPath.MakeAbsolute ();
  ExecuteDdeCommand (T_("[DocGoToNameDest(\"%s\",\"%s\")]"),
		     fullPath.Get(),
		     nameDest.c_str());
}

/* _________________________________________________________________________

   ArCtrl::FileOpen
   _________________________________________________________________________ */

void
ArCtrl::FileOpen (/*[in]*/ const PathName & path)
{
  if (! File::Exists(path))
    {
      FatalError (T_("The specified file could not be found."));
    }
  PathName fullPath (path);
  fullPath.MakeAbsolute ();
  ExecuteDdeCommand (T_("[FileOpen(\"%s\")]"), fullPath.Get());
}

/* _________________________________________________________________________

   ArCtrl::Execute
   _________________________________________________________________________ */

bool
ArCtrl::Execute (/*[in]*/ const string & command)
{
  StreamWriter stdErr (stderr);
  Tokenizer tok (command.c_str(), " \t\r\n");
  const char * lpszCommand = tok.GetCurrent();
  if (lpszCommand == 0)
    {
      return (true);
    }
  ++ tok;
  const char * lpszArgument = tok.GetCurrent();
  if (StringCompare(lpszCommand, T_("open"), true) == 0)
    {
      if (lpszArgument == 0)
	{
	  stdErr.WriteLine (T_("Error: missing file name argument."));
	  return (true);
	}
      DocOpen (lpszArgument);
    }
  else if (StringCompare(lpszCommand, T_("close"), true) == 0)
    {
      if (lpszArgument == 0)
	{
	  stdErr.WriteLine (T_("Error: missing file name argument."));
	  return (true);
	}
      DocClose (lpszArgument);
    }
  else if (StringCompare(lpszCommand, T_("closeall"), true) == 0)
    {
      if (lpszArgument != 0)
	{
	  stdErr.WriteLine (T_("Error: too many arguments."));
	  return (true);
	}
      CloseAllDocs ();
    }
  else if (StringCompare(lpszCommand, T_("goto"), true) == 0)
    {
      if (lpszArgument == 0)
	{
	  stdErr.WriteLine (T_("Error: missing file name argument."));
	  return (true);
	}
      ++ tok;
      if (tok.GetCurrent() == 0)
	{
	  stdErr.WriteLine (T_("Error: missing page number argument."));
	  return (true);
	}
      DocGoTo (lpszArgument, atoi(tok.GetCurrent()) - 1);
    }
  else if (StringCompare(lpszCommand, T_("gotoname"), true) == 0)
    {
      if (lpszArgument == 0)
	{
	  stdErr.WriteLine (T_("Error: missing file name argument."));
	  return (true);
	}
      ++ tok;
      if (tok.GetCurrent() == 0)
	{
	  stdErr.WriteLine (T_("Error: missing name dest argument."));
	  return (true);
	}
      DocGoToNameDest (lpszArgument, tok.GetCurrent());
    }
  else if (StringCompare(lpszCommand, T_("show"), true) == 0)
    {
      AppShow ();
    }
  else if (StringCompare(lpszCommand, T_("hide"), true) == 0)
    {
      AppHide ();
    }
  else if (StringCompare(lpszCommand, T_("exit"), true) == 0)
    {
      AppExit ();
      return (false);
    }
  else if (StringCompare(lpszCommand, T_("help"), true) == 0)
    {
      cout << T_("")
	   << T_("close FILE\n")
	   << T_("closeall\n")
	   << T_("exit\n")
	   << T_("goto FILE PAGE\n")
	   << T_("gotoname FILE NAMEDEST\n")
	   << T_("help\n")
	   << T_("hide\n")
	   << T_("open FILE\n")
	   << T_("show\n")
	   << T_("");
    }
  return (true);
}

/* _________________________________________________________________________

   ArCtrl::ReadAndExecute
   _________________________________________________________________________ */

void
ArCtrl::ReadAndExecute ()
{
  StreamReader reader (true);
  string command;
  while (reader.ReadLine(command))
    {
      if (! Execute(command))
	{
	  break;
	}
    }
}

/* _________________________________________________________________________

   ArCtrl::Run
   _________________________________________________________________________ */

void
ArCtrl::Run (/*[in]*/ int				argc,
	     /*[in]*/ const char **		argv)
{
  string program = Utils::GetExeName();

  const poptOption * pOption = &aoption[0];

  PathName file;
  int pageNum = -1;
  string nameDest;
  bool all = false;
  
  if (PathName::Compare(program, T_("pdfopen")) == 0)
    {
      mode = Open;
      pOption = &aoptionOpen[0];
    }
  else if (PathName::Compare(program, T_("pdfclose")) == 0)
    {
      mode = Close;
      pOption = &aoptionClose[0];
    }
  else if (PathName::Compare(program, T_("pdfdde")) == 0)
    {
      mode = CommandLoop;
      pOption = &aoptionCommandLoop[0];
    }

  Cpopt popt (argc, argv, pOption);

  int option;
  while ((option = popt.GetNextOpt()) >= 0)
    {
      const char * lpszOptArg = popt.GetOptArg();
      switch (option)
	{
	case OPT_ALL:

	  if (! file.Empty())
	    {
	      FatalError (T_("Options --all and --file cannot be used both."));
	    }
	  all = true;  
	  break;

	case OPT_FILE:

	  if (all)
	    {
	      FatalError (T_("Options --all and --file cannot be used both."));
	    }
	  if (! file.Empty())
	    {
	      FatalError (T_("Multiple --file arguments are not supported."));
	    }
	  file = lpszOptArg;
	  break;

	case OPT_GOTO:

	  if (! nameDest.empty())
	    {
	      FatalError (T_("Multiple --goto arguments are not supported."));
	    }
	  nameDest = lpszOptArg;
	  break;


	case OPT_PAGE:

	  if (pageNum >= 0)
	    {
	      FatalError (T_("Multiple --page arguments are not supported."));
	    }
	  pageNum = atoi(lpszOptArg) - 1;
	  break;
	  
	case OPT_VERSION:
	  
	  ShowVersion ();
	  throw (0);
	}
    }

  if (option != -1)
    {
      string msg = popt.BadOption(POPT_BADOPTION_NOALIAS);
      msg += ": ";
      msg += popt.Strerror(option);
      FatalError ("%s", msg.c_str());
    }
      
  const char ** leftovers = popt.GetArgs();

  if (leftovers != 0)
    {
      FatalError (T_("This program does not process non-option arguments."));
    }

  EstablishConversation ();

  switch (mode)
    {
    case Open:
      if (file.Empty())
	{
	  FatalError (T_("No file name was specified."));
	}
      DocOpen (file);
      FileOpen (file);
      if (pageNum >= 0)
	{
	  DocGoTo (file, pageNum);
	}
      if (! nameDest.empty())
	{
	  DocGoToNameDest (file, nameDest);
	}
      break;
    case Close:
      if (all)
	{
	  CloseAllDocs ();
	}
      else
	{
	  if (file.Empty())
	    {
	      FatalError (T_("No file name was specified."));
	    }
	  DocClose (file);
	}
      break;
    case CommandLoop:
      ReadAndExecute ();
      break;
    }

  TerminateConversation ();
}

/* _________________________________________________________________________

   arctrlmain
   _________________________________________________________________________ */

extern "C"
__declspec(dllexport)
int
__cdecl
arctrlmain (/*[in]*/ int			argc,
	    /*[in]*/ const char **	argv)
{
  try
    {
      ArCtrl app;
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
