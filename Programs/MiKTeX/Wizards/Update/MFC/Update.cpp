/* Update.cpp:

   Copyright (C) 2002-2006 Christian Schenk

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
	     /*[in,out]*/ MIKTEXCHAR ** &	argv,
	     /*[in,out]*/ int &			argMax)
{
  if (argc == argMax)
    {
      argMax += 10;
      argv =
	reinterpret_cast<MIKTEXCHAR**>(realloc(argv,
					       argMax * sizeof(argv[0])));
    }
  argv[ argc ++ ] = _tcsdup(argument);
}

/* _________________________________________________________________________

   GetArguments
   _________________________________________________________________________ */

void
GetArguments (/*[in]*/ const MIKTEXCHAR *	lpszCommandLine,
	      /*[in]*/ const MIKTEXCHAR *	lpszExeName,
	      /*[in,out]*/ int &		argc,
	      /*[in,out]*/ MIKTEXCHAR ** &	argv)
{
  argc = 0;
  argv = 0;

  int argMax = 0;

  AddArgument (lpszExeName, argc, argv, argMax);

  CString argument;
  bool copying = false;
  bool inQuotedArg = false;

  for (const MIKTEXCHAR * lpsz = lpszCommandLine; *lpsz != 0; ++ lpsz)
    {
      if (*lpsz == T_(' ') && ! inQuotedArg)
	{
	  if (copying)
	    {
	      AddArgument (argument, argc, argv, argMax);
	      argument = T_("");
	      copying = false;
	    }
	}
      else if (*lpsz == T_('"'))
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
	       /*[in]*/ MIKTEXCHAR ** & argv)
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

#define OPT_HELP T_('h')
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
ParseUpdateCommandLine (/*[in]*/ const MIKTEXCHAR *	lpszCommandLine,
			/*[in,out]*/ UpdateWizardCommandLineInfo & cmdinfo)
{
  int argc;
  MIKTEXCHAR ** argv;

  GetArguments (lpszCommandLine, AfxGetAppName(), argc, argv);

  int iOptIdx = 0;
  int c;

  cmdinfo.m_nShellCommand = CCommandLineInfo::FileNothing;

  optind = 0;

  while ((c = getopt_long_only(argc, argv, T_(""), long_options, &iOptIdx))
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

  InitCommonControls ();

  if (FAILED(CoInitialize(0)))
    {
      AfxMessageBox (T_("The application could not be initialized (1)."),
		     MB_ICONSTOP | MB_OK);
      return (FALSE);
    }

  try
    {
      SessionWrapper pSession (Session::InitInfo(T_("update")));

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
	  Process::Run (T_("migrate"));
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
  return (PathName(GetMainConfigDir().Get(), T_("uninst"), T_(".log")));
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
    auto_ptr<StreamWriter>(File::Open(GetULogFileName(),
				      (append
				       ? FileMode::Append
				       : FileMode::Create),
				      FileAccess::Write));;
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
  tstring str;
  str = T_("The operation could not be completed for the following reason: ");
  str += T_("\n\n");
  str += e.what();
  if (! e.GetInfo().empty())
    {
      str += T_("\n\n");
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
  tstring str;
  str = T_("The operation could not be completed for the following reason: ");
  str += T_("\n\n");
  str += e.what();
  AfxMessageBox (str.c_str(), MB_OK | MB_ICONSTOP);
}

/* _________________________________________________________________________

   SplitUrl
   _________________________________________________________________________ */

void
SplitUrl (/*[in]*/ const tstring &	url,
	  /*[out]*/ tstring &		protocol,
	  /*[out]*/ tstring &		host)
{
  MIKTEXCHAR szProtocol[200];
  MIKTEXCHAR szHost[200];
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
