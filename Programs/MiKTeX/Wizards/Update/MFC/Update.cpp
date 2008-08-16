/* Update.cpp:

   Copyright (C) 2002-2008 Christian Schenk

   This file is part of MiKTeX Update Wizard.

   MiKTeX Update Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Update Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX Update Wizard; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"
#include "Update.h"

#include "UpdateWizard.h"

UpdateWizardApplication theApp;

PathName g_logFileName;
PackageManagerPtr g_pManager;
bool g_upgrading;

/* _________________________________________________________________________

   UpdateWizardCommandLineInfo
   _________________________________________________________________________ */

class UpdateWizardCommandLineInfo : public CCommandLineInfo
{
public:
  UpdateWizardCommandLineInfo ()
    : dryRun (false)
  {
  }
  
public:
  bool dryRun;
};

/* _________________________________________________________________________

   AddArgument
   _________________________________________________________________________ */

void
AddArgument (/*[in]*/ const CString &		argument,
	     /*[in,out]*/ int &			argc,
	     /*[in,out]*/ char ** &	argv,
	     /*[in,out]*/ int &			argMax)
{
  if (argc == argMax)
    {
      argMax += 10;
      argv =
	reinterpret_cast<char**>(realloc(argv,
					       argMax * sizeof(argv[0])));
    }
  argv[ argc ++ ] = _tcsdup(argument);
}

/* _________________________________________________________________________

   GetArguments
   _________________________________________________________________________ */

void
GetArguments (/*[in]*/ const char *	lpszCommandLine,
	      /*[in]*/ const char *	lpszExeName,
	      /*[in,out]*/ int &		argc,
	      /*[in,out]*/ char ** &	argv)
{
  argc = 0;
  argv = 0;

  int argMax = 0;

  AddArgument (lpszExeName, argc, argv, argMax);

  CString argument;
  bool copying = false;
  bool inQuotedArg = false;

  for (const char * lpsz = lpszCommandLine; *lpsz != 0; ++ lpsz)
    {
      if (*lpsz == ' ' && ! inQuotedArg)
	{
	  if (copying)
	    {
	      AddArgument (argument, argc, argv, argMax);
	      argument = "";
	      copying = false;
	    }
	}
      else if (*lpsz == '"')
	{
	  inQuotedArg = ! inQuotedArg;
	  copying = true;
	}
      else
	{
	  argument += *lpsz;
	  copying = true;
	}
    }

  if (copying)
    {
      AddArgument (argument, argc, argv, argMax);
    }
}

/* _________________________________________________________________________

   FreeArguments
   _________________________________________________________________________ */

void
FreeArguments (/*[in]*/ int		argc,
	       /*[in]*/ char ** & argv)
{
  for (int i = 0; i < argc; ++ i)
    {
      free (argv[i]);
      argv[i] = 0;
    }
  free (argv);
  argv = 0;
}

/* _________________________________________________________________________

   long_options / short_options
   _________________________________________________________________________ */

#define OPT_HELP 'h'
#define OPT_DRYRUN 1

const struct option long_options[] =
{
  T_("dry-run"),		no_argument,		0,	OPT_DRYRUN,
  T_("help"),                   no_argument,		0,	OPT_HELP,
  0,				no_argument,		0,	0,
};

/* _________________________________________________________________________

   ShowHelpAndExit
   _________________________________________________________________________ */

void
ShowHelpAndExit (/*[in]*/ int retCode = 0)
{
  AfxMessageBox (T_("Usage: updatewiz [OPTIONS]\r\n\r\n\
Options:\r\n\r\n\
  --dry-run\r\n\
  --help\r\n"));
  exit (retCode);
}

/* _________________________________________________________________________

   ParseUpdateCommandLine
   _________________________________________________________________________ */

void
ParseUpdateCommandLine (/*[in]*/ const char *	lpszCommandLine,
			/*[in,out]*/ UpdateWizardCommandLineInfo & cmdinfo)
{
  int argc;
  char ** argv;

  GetArguments (lpszCommandLine, AfxGetAppName(), argc, argv);

  int iOptIdx = 0;
  int c;

  cmdinfo.m_nShellCommand = CCommandLineInfo::FileNothing;

  optind = 0;

  while ((c = getopt_long_only(argc, argv, "", long_options, &iOptIdx))
	 != EOF)
    {
      switch (c)
	{

	case OPT_HELP:
	  FreeArguments (argc, argv);
	  ShowHelpAndExit ();
	  break;

	case OPT_DRYRUN:
	  cmdinfo.dryRun = true;
	  break;

	default:
	  FreeArguments (argc, argv);
	  ShowHelpAndExit (1);
	  break;

	}
    }

  FreeArguments (argc, argv);
}

/* _________________________________________________________________________

   UpdateWizardApplication Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(UpdateWizardApplication, CWinApp)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   UpdateWizardApplication::UpdateWizardApplication
   _________________________________________________________________________ */

UpdateWizardApplication::UpdateWizardApplication ()
{
}

/* _________________________________________________________________________

   UpdateWizardApplication::InitInstance
   _________________________________________________________________________ */

BOOL
UpdateWizardApplication::InitInstance ()
{
  if (! Upgrade(g_upgrading))
    {
      return (FALSE);
    }

  INITCOMMONCONTROLSEX initCtrls;

  initCtrls.dwSize = sizeof(initCtrls);
  initCtrls.dwICC = ICC_WIN95_CLASSES;

  if (! InitCommonControlsEx(&initCtrls))
    {
      AfxMessageBox (T_("The application could not be initialized (1)."),
		     MB_ICONSTOP | MB_OK);
      return (FALSE);
    }

  if (FAILED(CoInitialize(0)))
    {
      AfxMessageBox (T_("The application could not be initialized (2)."),
		     MB_ICONSTOP | MB_OK);
      return (FALSE);
    }

  try
    {
      SessionWrapper pSession (Session::InitInfo(T_("update")));

      if (! Utils::CheckPath(false))
	{
	  if (AfxMessageBox(T_("\
MiKTeX is not correctly configured: the location of the MiKTeX executables \
is not known to the operating system.\r\n\r\n\
Click OK to repair the MiKTeX configuration."),
			    MB_OKCANCEL)
	      == IDOK)
	    {
	      Utils::CheckPath (true);
	    }
	  else
	    {
	      if (SessionWrapper(true)
		  ->GetConfigValue(MIKTEX_REGKEY_CORE,
				   MIKTEX_REGVAL_INSIST_ON_REPAIR,
				   true))
		{
		  FATAL_MIKTEX_ERROR ("UpdateWizardApplication::InitInstance",
				      T_("Broken MiKTeX configuration."),
				      0);
		}
	    }
	}
      
      g_pManager = PackageManager::Create();

      // get command-line arguments
      UpdateWizardCommandLineInfo cmdinfo;
      ParseUpdateCommandLine (m_lpCmdLine, cmdinfo);
      
      {
	UpdateWizard dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal ();
      }

      g_pManager->UnloadDatabase ();

      g_pManager.Release ();

      pSession.Reset ();
    }
  catch (const MiKTeXException & e)
    {
      ReportError (e);
    }
  catch (const exception & e)
    {
      ReportError (e);
    }

  CoUninitialize ();
  
  return (FALSE);
}

/* _________________________________________________________________________

   UpdateWizardApplication::Upgrade
   _________________________________________________________________________ */

bool
UpdateWizardApplication::Upgrade (/*[out]*/ bool & upgrading)
{
  bool done = false;
  upgrading = false;
  try
    {
      Session::InitInfo initInfo (T_("upgrade"),
				  Session::InitFlags::NoConfigFiles);
      SessionWrapper pSession (initInfo);
      PathName path;
      if (GetModuleFileName(0, path.GetBuffer(), BufferSizes::MaxPath) == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("GetModuleFileName"), 0);
	}
      if (path.GetFileNameWithoutExtension() == T_("upgrade"))
	{
	  PathName migrate;
	  if (! pSession->FindFile(T_("migrate"), FileType::EXE, migrate))
	    {
	      FATAL_MIKTEX_ERROR ("UpdateWizardApplication::Upgrade",
				  T_("migrate.exe could not be found."),
				  0);
	    }
	  char szBuf[1024];
	  size_t sizeBuf = 1024;
	  if (! Process::Run(migrate, 0, szBuf, &sizeBuf, 0))
	    {
	      FATAL_MIKTEX_ERROR ("UpdateWizardApplication::Upgrade",
				  T_("Migration failed for some reason."),
				  0);
	    }
	  upgrading = true;
	}
      done = true;
    }
  catch (const MiKTeXException & e)
    {
      ReportError (e);
    }
  catch (const exception & e)
    {
      ReportError (e);
    }
  return (done);
}

/* _________________________________________________________________________

   GetULogFileName
   _________________________________________________________________________ */

PathName
GetULogFileName ()
{
  return (PathName(GetMainConfigDir().Get(), "uninst", ".log"));
}

/* _________________________________________________________________________

   ULogOpen
   _________________________________________________________________________ */

enum Section { None, Files, HKLM, HKCU };

namespace {
  Section section = None;
  auto_ptr<StreamWriter> pLogStream;
}

void
ULogOpen (/*[in]*/ bool append)
{
  MIKTEX_ASSERT (append);
  pLogStream =
    auto_ptr<StreamWriter>(new StreamWriter(File::Open(GetULogFileName(),
						       (append
							? FileMode::Append
							: FileMode::Create),
						       FileAccess::Write)));
  section = None;
}

/* _________________________________________________________________________

   ULogClose
   _________________________________________________________________________ */

void
ULogClose (/*[in]*/ bool doRegister)
{
  UNUSED_ALWAYS (doRegister);
  if (pLogStream.get() != 0)
    {
      pLogStream->Close ();
      pLogStream.reset ();
    }
}

/* _________________________________________________________________________

   ULogAddFile
   _________________________________________________________________________ */

void
ULogAddFile (/*[in]*/ const PathName & path)
{
  if (pLogStream.get() == 0)
    {
      return;
    }
  if (section != Files)
    {
      pLogStream->WriteLine (T_("[files]"));
      section = Files;
    }
  pLogStream->WriteLine (PathName(path).ToDos().ToString());
}

/* _________________________________________________________________________

   ReportError
   _________________________________________________________________________ */

void
ReportError (/*[in]*/ const MiKTeXException & e)
{
  string str;
  str = T_("The operation could not be completed for the following reason: ");
  str += "\n\n";
  str += e.what();
  if (! e.GetInfo().empty())
    {
      str += "\n\n";
      str += T_("Details: ");
      str += e.GetInfo();
    }
  AfxMessageBox (str.c_str(), MB_OK | MB_ICONSTOP);
}

/* _________________________________________________________________________

   ReportError
   _________________________________________________________________________ */

void
ReportError (/*[in]*/ const exception & e)
{
  string str;
  str = T_("The operation could not be completed for the following reason: ");
  str += "\n\n";
  str += e.what();
  AfxMessageBox (str.c_str(), MB_OK | MB_ICONSTOP);
}

/* _________________________________________________________________________

   SplitUrl
   _________________________________________________________________________ */

void
SplitUrl (/*[in]*/ const string &	url,
	  /*[out]*/ string &		protocol,
	  /*[out]*/ string &		host)
{
  char szProtocol[200];
  char szHost[200];
  URL_COMPONENTS url_comp = { 0 };
  url_comp.dwStructSize = sizeof(url_comp);
  url_comp.lpszScheme = szProtocol;
  url_comp.dwSchemeLength = 200;
  url_comp.lpszHostName = szHost;
  url_comp.dwHostNameLength = 200;
  if (! InternetCrackUrl(url.c_str(), 0, 0, &url_comp))
    {
      FATAL_WINDOWS_ERROR (T_("InternetCrackUrl"), 0);
    }
  protocol = szProtocol;
  host = szHost;
}
