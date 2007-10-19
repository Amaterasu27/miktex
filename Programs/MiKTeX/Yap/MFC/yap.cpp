/* yap.cpp: Yet Another Previewer

   Copyright (C) 1996-2007 Christian Schenk

   This file is part of Yap.

   Yap is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   Yap is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with Yap; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include "StdAfx.h"

#include "yap.h"

#include "ChildFrame.h"
#include "DviDoc.h"
#include "DviView.h"
#include "InverseSearchOptionsPage.h"
#include "MainFrame.h"
#include "winplace.h"

// the one and only YapApplication object
YapApplication theApp;

/* _________________________________________________________________________

   aoption
   _________________________________________________________________________ */

enum {
  OPT_DDE = 1,
  OPT_FIND_SRC_SPECIAL,
  OPT_GOTO_HYPER_LABEL,
  OPT_PRINT,
  OPT_REGISTER,
  OPT_SINGLE_INSTANCE,
  OPT_TRACE,
  OPT_UNREGISTER,
};

namespace {
  struct poptOption const aoption[] = {
    {
      T_("dde"), 0,
      POPT_ARG_NONE, 0,
      OPT_DDE,
      T_("Starts up for DDE."), 0
    },
    {
      T_("find-src-special"), 's',
      POPT_ARG_STRING, 0,
      OPT_FIND_SRC_SPECIAL,
      T_("Initiates forward search."), T_("SrcSpecial")
    },
    {
      T_("goto-hyper-label"), 'g',
      POPT_ARG_STRING, 0,
      OPT_GOTO_HYPER_LABEL,
      T_("Jumps to a HyperTeX label."), T_("HYPERLABEL")
    },
    {
      T_("print"), 0,
      POPT_ARG_NONE, 0,
      OPT_PRINT,
      T_("Sends the document to the default printer."), 0
    },
    {
      T_("register"), 0,
      POPT_ARG_NONE, 0,
      OPT_REGISTER,
      T_("Stores Yap related keys in the Windows Registry."), 0
    },
    {
      T_("single-instance"), '1',
      POPT_ARG_NONE, 0,
      OPT_SINGLE_INSTANCE,
      T_("Reuses an application window, if one exists."), 0
    },
    {
      T_("trace"), 0, POPT_ARG_STRING, 0,
      OPT_TRACE,
      T_("Turns tracing on."), T_("TRACEFLAGS")
    },
    {
      T_("unregister"), 0,
      POPT_ARG_NONE, 0,
      OPT_UNREGISTER,
      T_("Removes Yap related keys from the Windows Registry."), 0
    },
    POPT_TABLEEND
  };
}

/* _________________________________________________________________________

   ParseYapCommandLine
   _________________________________________________________________________ */

namespace {
  bool registering = false;
  bool unregistering = false;
}

void
ParseYapCommandLine (/*[in]*/ const char *	lpszCommandLine,
		     /*[in]*/ YapCommandLineInfo &	cmdInfo)
{
  cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

  if (*lpszCommandLine == 0)
    {
      return;
    }

  int option;

  Argv argv;

  argv.Build (T_("yap"), lpszCommandLine);

  Cpopt popt (argv.GetArgc(),
	      const_cast<const char **>(argv.GetArgv()),
	      aoption);

  popt.SetOtherOptionHelp (T_("[OPTION...] [DVIFILE]"));

  while ((option = popt.GetNextOpt()) >= 0)
    {
      switch (option)
	{

	case OPT_DDE:
	  AfxOleSetUserCtrl (FALSE);
	  cmdInfo.m_nShellCommand = CCommandLineInfo::FileDDE;
	  break;

	case OPT_FIND_SRC_SPECIAL:
	  {
	    char * lpszFileName = 0;
	    cmdInfo.sourceLineNum =
	      strtol(popt.GetOptArg(), &lpszFileName, 10);
	    if (lpszFileName != 0)
	      {
		while (*lpszFileName == ' ')
		  {
		    ++ lpszFileName;
		  }
		if (*lpszFileName != 0)
		  {
		    cmdInfo.sourceFile = lpszFileName;
		  }
	      }
	    break;
	  }

	case OPT_GOTO_HYPER_LABEL:
	  cmdInfo.hyperLabel = popt.GetOptArg();
	  break;

	case OPT_PRINT:
	  cmdInfo.m_nShellCommand = CCommandLineInfo::FilePrint;
	  break;

	case OPT_SINGLE_INSTANCE:
	  cmdInfo.singleInstance = true;
	  break;

	case OPT_REGISTER:
	  cmdInfo.m_nShellCommand = CCommandLineInfo::AppRegister;
	  registering = true;
	  break;

	case OPT_TRACE:
	  cmdInfo.traceFlags = popt.GetOptArg();
	  break;

	case OPT_UNREGISTER:
	  cmdInfo.m_nShellCommand = CCommandLineInfo::AppUnregister;
	  unregistering = true;
	  break;

	}
    }

  if (option < -1)
    {
      FATAL_MIKTEX_ERROR (T_("ParseYapCommandLine"),
			  T_("Invalid Yap command-line."),
			  0);
    }

  const char ** leftovers = popt.GetArgs();

  // parse the rest
  for (; leftovers != 0 && *leftovers != 0; ++ leftovers)
    {
      if (_tcsicmp(*leftovers, T_("/dde")) == 0)
	{
	  AfxOleSetUserCtrl (FALSE);
	  cmdInfo.m_nShellCommand = CCommandLineInfo::FileDDE;
	}
      else if (cmdInfo.m_strFileName.IsEmpty())
	{
	  // we have a file name argument; append .dvi to the file
	  // name, if necessary
	  PathName pathFileName = *leftovers;
	  if (! pathFileName.HasExtension(".dvi"))
	    {
	      pathFileName.Append (".dvi", false);
	    }
	  
	  // locate the .dvi file
	  PathName pathDvi;
	  if (! SessionWrapper(true)->FindFile(pathFileName.Get(),
					       FileType::DVI,
					       pathDvi))
	    {
	      pathDvi = pathFileName;
	    }
	  
	  cmdInfo.m_strFileName = pathDvi.Get();

	  if (cmdInfo.m_nShellCommand != CCommandLineInfo::FilePrint)
	    {
	      cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
	    }
	}
    }
}

/* _________________________________________________________________________

   YapApplication Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(YapApplication, CWinApp)
  ON_COMMAND(ID_APP_ABOUT, &YapApplication::OnAppAbout)
  ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
  ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
  ON_COMMAND(ID_VIEW_TRACE, &YapApplication::OnViewTrace)
  ON_UPDATE_COMMAND_UI(ID_VIEW_TRACE, &YapApplication::OnUpdateViewTrace)
  ON_COMMAND(ID_REGISTER_MIKTEX, &YapApplication::OnRegisterMiKTeX)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   YapApplication::YapApplication
   _________________________________________________________________________ */

YapApplication::YapApplication ()
  : tracing (false)
{
  EnableHtmlHelp ();
}

/* _________________________________________________________________________

   YapApplication::RegisterWindowClass
   _________________________________________________________________________ */

namespace {
  bool classRegistered = false;
}

void
YapApplication::RegisterWindowClass ()
{
  if (! classRegistered)
    {
      WNDCLASS wndcls;
      memset (&wndcls, 0, sizeof(WNDCLASS));
      wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndcls.lpfnWndProc = ::DefWindowProc;
      wndcls.hInstance = AfxGetInstanceHandle();
      wndcls.hIcon = LoadIcon(IDR_MAINFRAME);
      wndcls.hCursor = LoadCursor(IDC_ARROW);
      wndcls.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
      wndcls.lpszMenuName = 0;
      wndcls.lpszClassName = YAP_WND_CLASS;
      if (! AfxRegisterClass(&wndcls))
	{
	  FATAL_MIKTEX_ERROR ("YapApplication::RegisterWindowClass",
			      T_("The windows class could not be registered."),
			      0);
	}
      else
	{
	  classRegistered = true;
	}
    }
}

/* _________________________________________________________________________

   YapApplication::InitInstance
   _________________________________________________________________________ */

namespace {
  bool initialized = false;
}

BOOL
YapApplication::InitInstance ()
{
  INITCOMMONCONTROLSEX initCtrls;
  initCtrls.dwSize = sizeof(initCtrls);
  initCtrls.dwICC = ICC_WIN95_CLASSES;
  if (! InitCommonControlsEx(&initCtrls))
    {
      AfxMessageBox (T_("The application could not be initialized (1)."),
		     MB_ICONSTOP | MB_OK);
      return (FALSE);
    }

  if (! CWinApp::InitInstance())
    {
      AfxMessageBox (T_("The application could not be initialized (2)."),
		     MB_ICONSTOP | MB_OK);
      return (FALSE);
    }

  if (FAILED(CoInitializeEx(0, COINIT_MULTITHREADED)))
    {
      AfxMessageBox (T_("The application could not be initialized (3)."),
		     MB_ICONSTOP | MB_OK);
      return (FALSE);
    }

  try
    {
      // initialize MiKTeX Library
      pSession.CreateSession (Session::InitInfo(T_("yap")));
      
      trace_yap = auto_ptr<TraceStream>(TraceStream::Open(T_("yap")));
      trace_error = auto_ptr<TraceStream>(TraceStream::Open(T_("error")));
      
      // get command-line arguments
      YapCommandLineInfo cmdInfo;
      ParseYapCommandLine (m_lpCmdLine, cmdInfo);
      
      // set trace flags
      if (cmdInfo.traceFlags.GetLength() > 0)
	{
	  tracing = true;
	  traceFlags = cmdInfo.traceFlags;
	  TraceStream::SetTraceFlags (cmdInfo.traceFlags);
	}
      
      YapLog (T_("Yap arguments: %s"), m_lpCmdLine);
      
      // return, if another application instance was found
      if (cmdInfo.singleInstance && ActivateFirstInstance(cmdInfo))
	{
	  trace_yap->Close ();
	  trace_error->Close ();
	  pSession.Reset ();
	  return (FALSE);
	}
      
      // register the app window class
      RegisterWindowClass ();
      
      // search the help file
      PathName helpFileName;
      if (pSession->FindFile(MIKTEX_YAP_HELP_FILE,
			     (MIKTEX_PATH_TEXMF_PLACEHOLDER
			      MIKTEX_PATH_DIRECTORY_DELIMITER_STRING
			      MIKTEX_PATH_MIKTEX_DOC_DIR
			      MIKTEX_PATH_RECURSION_INDICATOR),
			     helpFileName))
	{
	  MIKTEX_ASSERT (_CrtIsValidHeapPointer(m_pszHelpFilePath));
	  free (reinterpret_cast<void*>
		(const_cast<char *>(m_pszHelpFilePath)));
	  m_pszHelpFilePath = _tcsdup(helpFileName.Get());
	}
      
      // change the registry key under which our settings are stored
      SetRegistryKey (MIKTEX_COMPANYNAME_STR
		      "\\"
		      MIKTEX_PRODUCTNAME_STR
		      "\\"
		      MIKTEX_SERIES_STR);
      
      // load standard ini file options (including MRU)
      LoadStdProfileSettings ();
      
      // read Yap configuration settings
      g_pYapConfig = auto_ptr<YapConfig>(new YapConfig ());
      
      // now we are initialized
      initialized = true;
      
      // register the application's document templates
      CMultiDocTemplate * pDocTemplate;
      pDocTemplate =
	new CMultiDocTemplate(IDR_DVITYPE,
			      RUNTIME_CLASS(DviDoc),
			      RUNTIME_CLASS(ChildFrame),
			      RUNTIME_CLASS(DviView));
      AddDocTemplate (pDocTemplate);
      
      // create main MDI Frame window
      MainFrame * pMainFrame = new MainFrame;
      if (! pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
	  FATAL_MIKTEX_ERROR
	    ("YapApplication::InitInstance",
	     T_("The main frame window could not be initialized."),
	     0);
	}
      m_pMainWnd = pMainFrame;
      
      // enable drag/drop open
      m_pMainWnd->DragAcceptFiles ();
      YapApplication::EnableShellOpen ();
      
      // check to see if Yap is the default viewer
      if ((registering || g_pYapConfig->checkFileTypeAssociations)
	  && ! unregistering)
	{
	  char szClass[BufferSizes::MaxPath];
	  long size = BufferSizes::MaxPath;
	  if ((::RegQueryValue(HKEY_CLASSES_ROOT,
			       ".dvi",
			       szClass,
			       &size)
	       == ERROR_SUCCESS)
	      && ((StringCompare(szClass,
				 MIKTEX_DVI_FILE_TYPE_IDENTIFIER,
				 true)
		   != 0))
	      && (registering
		  || (AfxMessageBox(IDP_FILE_ASSOCIATION,
				    MB_YESNO,
				    IDP_FILE_ASSOCIATION)
		      == IDYES)))
	    {
	      // remove .dvi file association; will be restored later
	      SHDeleteKey (HKEY_CLASSES_ROOT, ".dvi");
	    }
	}
      
      // register .dvi file association
      RegisterShellFileTypes (TRUE);
      
      // dispatch commands specified on the command line
      if (! ProcessShellCommand(cmdInfo))
	{
	  return (FALSE);
	}
      
      // the main window has been initialized, so show and update it
      CWindowPlacement wp;
      if (! wp.Restore(T_("Settings"), pMainFrame))
	{
	  pMainFrame->ShowWindow (m_nCmdShow);
	}
      pMainFrame->UpdateWindow ();
      
      // perform a forward search, if requested
      if (cmdInfo.sourceLineNum >= 0)
	{
	  FindSrcSpecial (cmdInfo.sourceLineNum, cmdInfo.sourceFile);
	}
      
      // perform a hyper-goto, if requested
      if (cmdInfo.hyperLabel.GetLength() > 0)
	{
	  GotoHyperLabel (cmdInfo.hyperLabel);
	}

      return (TRUE);
    }

  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (0, e);
      return (FALSE);
    }

  catch (const exception & e)
    {
      ErrorDialog::DoModal (0, e);
      return (FALSE);
    }
}

/* _________________________________________________________________________

   AboutDialog
   _________________________________________________________________________ */

class AboutDialog : public CDialog
{
private:
  enum { IDD = IDD_ABOUTBOX };
  
protected:
  DECLARE_MESSAGE_MAP();

public:
  AboutDialog ();
  
protected:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);
  
protected:
  afx_msg
  void
  OnClickRegisterMiKTeX ();
};

/* _________________________________________________________________________

   AboutDialog::AboutDialog
   _________________________________________________________________________ */

AboutDialog::AboutDialog ()
  : CDialog(AboutDialog::IDD)
{
}

/* _________________________________________________________________________

   AboutDialog::DoDataExchange
   _________________________________________________________________________ */

void
AboutDialog::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);
  if (! pDX->m_bSaveAndValidate)
    {
      CString str;
      str.Format (T_(T_("Yet Another Previewer %s")), VER_FILEVERSION_STR);
      str += "\r\n";
      str += MIKTEX_LEGALCOPYRIGHT_STR;
      GetDlgItem(IDC_THE_NAME_OF_THE_GAME)->SetWindowText (str);
    }
}

/* _________________________________________________________________________

   AboutDialog Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(AboutDialog, CDialog)
  ON_BN_CLICKED(ID_REGISTER_MIKTEX,
		&AboutDialog::OnClickRegisterMiKTeX)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   AboutDialog::OnClickRegisterMiKTeX
   _________________________________________________________________________ */

void
AboutDialog::OnClickRegisterMiKTeX ()
{
  try
    {
      Utils::RegisterMiKTeXUser ();
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
}

/* _________________________________________________________________________

   YapApplication::OnAppAbout
   _________________________________________________________________________ */

void
YapApplication::OnAppAbout ()
{
  AboutDialog dlg;
  dlg.DoModal ();
}

/* _________________________________________________________________________

   YapApplication::ExitInstance
   _________________________________________________________________________ */

int
YapApplication::ExitInstance () 
{
  try
    {
      // save YAP configuration settings
      if (! (registering || unregistering) && initialized)
	{
	  try
	    {
	      MIKTEX_ASSERT (g_pYapConfig.get() != 0);
	      g_pYapConfig->Save ();
	      g_pYapConfig.reset ();
	    }
	  catch (const exception &)
	    {
	    }
	}
      
      // unregister app window class
      if (classRegistered)
	{
	  UnregisterClass (YAP_WND_CLASS, AfxGetInstanceHandle());
	  classRegistered = false;
	}

      if (trace_yap.get() != 0)
	{
	  trace_yap->Close ();
	  trace_yap.reset ();
	}

      if (trace_error.get() != 0)
	{
	  trace_error->Close ();
	  trace_error.reset ();
	}
      
      // uninitialize MiKTeX Library
      pSession.Reset ();
    }

  catch (const exception &)
    {
    }

  CoUninitialize ();

  return (CWinApp::ExitInstance ());
}

/* _________________________________________________________________________

   YapApplication::ActivateFirstInstance
   _________________________________________________________________________ */

void
DdeExecute (/*[in]*/ const char * server,
	    /*[in]*/ const char * topic,
	    /*[in]*/ const char * command);

bool
YapApplication::ActivateFirstInstance
(/*[in]*/ const YapCommandLineInfo & cmdInfo)
{
  // check to see whether there is another Yap instance running
  CWnd * pwndPrev = CWnd::FindWindow(YAP_WND_CLASS, 0);
  if (pwndPrev == 0)
    {
      return (false);		// we are alone
    }

  CWnd * wndChild = pwndPrev->GetLastActivePopup();
  MIKTEX_ASSERT (wndChild != 0);
  
  // restore the other app window
  if (pwndPrev->IsIconic())
    {
      pwndPrev->ShowWindow (SW_RESTORE);
    }
  wndChild->SetForegroundWindow ();
  
  if (strlen(cmdInfo.m_strFileName) == 0)
    {
      return (true);
    }
  
  // open the DVI document via DDE
  PathName path (cmdInfo.m_strFileName);
  path.MakeAbsolute ();

  CString ddeCommand;
  ddeCommand.Format (T_("[open(\"%s\")]"), path.Get());
  DdeExecute (T_("yap"), T_("system"), ddeCommand);
  
  // delegate DVI search
  if (cmdInfo.sourceLineNum >= 0)
    {
      ddeCommand.Format (T_("[findsrc(\"%d %s\")]"),
			 static_cast<int>(cmdInfo.sourceLineNum),
			 static_cast<const char *>(cmdInfo.sourceFile));
      DdeExecute (T_("yap"), T_("system"), ddeCommand);
    }
  
  // delegate hyper-goto
  if (cmdInfo.hyperLabel.GetLength() > 0)
    {
      ddeCommand.Format (T_("[gotohyperlabel(\"%s\")]"),
			 static_cast<const char *>(cmdInfo.hyperLabel));
      DdeExecute (T_("yap"), T_("system"), ddeCommand);
    }
  
  return (true);
}

/* _________________________________________________________________________

   YapClientDDECallback
   _________________________________________________________________________ */

HDDEDATA CALLBACK
YapClientDDECallback (/*[in]*/ UINT,
		      /*[in]*/ UINT,
		      /*[in]*/ HCONV,
		      /*[in]*/ HSZ,
		      /*[in]*/ HSZ,
		      /*[in]*/ HDDEDATA,
		      /*[in]*/ unsigned long,
		      /*[in]*/ unsigned long)
{
  return (0);
}

/* _________________________________________________________________________

   DdeExecute
   _________________________________________________________________________ */

void
DdeExecute (/*[in]*/ const char * lpszServer,
	    /*[in]*/ const char * lpszTopic,
	    /*[in]*/ const char * lpszCommand)
{
  YapLog (T_("DdeExecute(\"%s\", \"%s\", \"%s\")"),
	  lpszServer,
	  lpszTopic,
	  lpszCommand);
  unsigned long inst = 0;
  UINT result =
    DdeInitialize(&inst,
		  YapClientDDECallback,
		  APPCMD_CLIENTONLY,
		  0);
  if (result != DMLERR_NO_ERROR)
    {
      FATAL_MIKTEX_ERROR (T_("DdeExecute"),
			  T_("DDE communication could not be initiated."),
			  NUMTOSTR(result));
    }
  AutoDdeUninitialize autoDdeUninitialize (inst);
  HSZ hszServer = DdeCreateStringHandle(inst, lpszServer, CP_WINANSI);
  if (hszServer == 0)
    {
      FATAL_MIKTEX_ERROR (T_("DdeExecute"),
			  T_("DDE communication could not be initiated."),
			  NUMTOSTR(DdeGetLastError(inst)));
    }
  AutoDdeFreeStringHandle autoDdeFreeStringHandle1 (inst, hszServer);
  HSZ hszTopic = DdeCreateStringHandle(inst, lpszTopic, CP_WINANSI);
  if (hszTopic == 0)
    {
      FATAL_MIKTEX_ERROR (T_("DdeExecute"),
			  T_("DDE communication could not be initiated."),
			  NUMTOSTR(DdeGetLastError(inst)));
    }
  AutoDdeFreeStringHandle autoDdeFreeStringHandle2 (inst, hszTopic);
  HCONV hconv = DdeConnect(inst, hszServer, hszTopic, 0);
  if (hconv == 0)
    {
      FATAL_MIKTEX_ERROR (T_("DdeExecute"),
			  T_("DDE connection could not be established."),
			  NUMTOSTR(DdeGetLastError(inst)));
    }
  AutoDdeDisconnect autoDdeDisconnect (hconv);
  HDDEDATA hddedata;
  hddedata =
    DdeCreateDataHandle(inst,
		reinterpret_cast<BYTE*>(const_cast<char *>(lpszCommand)),
			static_cast<unsigned long>(strlen(lpszCommand) + 1),
			0, 0, CF_TEXT, 0);
  if (hddedata == 0)
    {
      FATAL_MIKTEX_ERROR (T_("DdeExecute"),
			  T_("DDE connection could not be established."),
			  NUMTOSTR(DdeGetLastError(inst)));
    }
  AutoDdeFreeDataHandle autoDdeFreeDataHandle (hddedata);
  if (DdeClientTransaction(reinterpret_cast<BYTE*>(hddedata),
			   static_cast<unsigned long>(-1),
			   hconv,
			   0,
			   0,
			   XTYP_EXECUTE,
			   10000,
			   0)
      == 0)
    {
      FATAL_MIKTEX_ERROR (T_("DdeExecute"),
			  T_("DDE transaction failed."),
			  NUMTOSTR(DdeGetLastError(inst)));
    }
}

/* _________________________________________________________________________

   YapApplication::OnDDECommand
   _________________________________________________________________________ */

namespace {
  bool ddeServing = false;
}

BOOL
YapApplication::OnDDECommand (/*[in]*/ char * lpszCommand)
{
  ddeServing = true;

  BOOL done = FALSE;

  try
    {
      YapLog (T_("OnDDECommand(\"%s\")"), lpszCommand);

      done = CWinApp::OnDDECommand(lpszCommand);

      if (! done)
	{
	  CString ddeCommand = lpszCommand;
	  if (ddeCommand.Left(10) == T_("[findsrc(\""))
	    {
	      CString src = ddeCommand.Right(ddeCommand.GetLength() - 10);
	      int i = src.Find('"');
	      if (i != -1)
		{
		  src = src.Left(i);
		  char * lpszFileName = 0;
		  long line = _tcstol(src, &lpszFileName, 10);
		  while (*lpszFileName == ' ')
		    {
		      ++ lpszFileName;
		    }
		  FindSrcSpecial (line, lpszFileName);
		  done = TRUE;
		}
	    }
	  else if (ddeCommand.Left(17) == T_("[gotohyperlabel(\""))
	    {
	      CString label = ddeCommand.Right(ddeCommand.GetLength() - 17);
	      int i = label.Find('"');
	      if (i != -1)
		{
		  label = label.Left(i);
		  GotoHyperLabel (label);
		  done = TRUE;
		}
	    }
	}
    }

  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (0, e);
    }

  catch (const exception & e)
    {
      ErrorDialog::DoModal (0, e);
    }

  ddeServing = false;

  return (done);
}

/* _________________________________________________________________________

   YapApplication::FindSrcSpecial
   _________________________________________________________________________ */

bool
YapApplication::FindSrcSpecial (/*[in]*/ int		line,
				/*[in]*/ const char *	lpszFileName)
{
  POSITION posTemplate = GetFirstDocTemplatePosition();
  while (posTemplate != 0)
    {
      CDocTemplate * pTemplate = GetNextDocTemplate(posTemplate);
      POSITION posDoc = pTemplate->GetFirstDocPosition();
      while (posDoc != 0)
	{
	  CDocument * pDoc = pTemplate->GetNextDoc(posDoc);
	  if (! pDoc->IsKindOf(RUNTIME_CLASS(DviDoc)))
	    {
	      continue;
	    }
	  DviDoc * pDviDoc = reinterpret_cast<DviDoc*>(pDoc);
	  POSITION posView = pDviDoc->GetFirstViewPosition();
	  while (posView != 0)
	    {
	      CView * pView = pDviDoc->GetNextView(posView);
	      if (! pView->IsKindOf(RUNTIME_CLASS(DviView)))
		{
		  continue;
		}
	      DviView * pDviview = reinterpret_cast<DviView*>(pView);
	      if (pDviview->GotoSrcSpecial(line, lpszFileName))
		{
		  return (true);
		}
	    }
	}
    }
  return (false);
}

/* _________________________________________________________________________

   YapApplication::GotoHyperLabel
   _________________________________________________________________________ */

bool
YapApplication::GotoHyperLabel (/*[in]*/ const char * lpszLabel)
{
  CString hashLabel;
  hashLabel = '#';
  hashLabel += lpszLabel;
  POSITION posTemplate = GetFirstDocTemplatePosition();
  while (posTemplate != 0)
    {
      CDocTemplate * pTemplate = GetNextDocTemplate(posTemplate);
      POSITION posDoc = pTemplate->GetFirstDocPosition();
      while (posDoc != 0)
	{
	  CDocument * pDoc = pTemplate->GetNextDoc(posDoc);
	  if (! pDoc->IsKindOf(RUNTIME_CLASS(DviDoc)))
	    {
	      continue;
	    }
	  DviDoc * pDviDoc = reinterpret_cast<DviDoc*>(pDoc);
	  POSITION posView = pDviDoc->GetFirstViewPosition();
	  while (posView != 0)
	    {
	      CView * pView = pDviDoc->GetNextView(posView);
	      if (! pView->IsKindOf(RUNTIME_CLASS(DviView)))
		{
		  continue;
		}
	      DviView * pDviView = reinterpret_cast<DviView *>(pView);
	      if (pDviView->Navigate(hashLabel, false))
		{
		  return (true);
		}
	    }
	}
    }
  return (false);
}

/* _________________________________________________________________________

   StartEditor
   _________________________________________________________________________ */

void
StartEditor (/*[in]*/ const char *	lpszFileName,
	     /*[in]*/ const char *	lpszDocDir,
	     /*[in]*/ int			line)
{
  // find the source file
  PathName path;
  if (Utils::IsAbsolutePath(lpszFileName) || lpszDocDir == 0)
    {
      path = lpszFileName;
    }
  else
    {
      path.Set (lpszDocDir, lpszFileName);
    }
  PathName path2;
  if (! SessionWrapper(true)->FindFile(path.Get(),
				       FileType::TEX,
				       path2))
    {
      FATAL_MIKTEX_ERROR ("DviView::OnPageEditor",
			  T_("The source file could not be found."),
			  lpszFileName);
    }

  // make command line
  string commandLine;
  const char * lpsz = g_pYapConfig->inverseSearchCommandLine;
  bool haveName = false;
  bool haveLine = false;
  while (*lpsz != 0)
    {
      if (*lpsz == '%')
	{
	  switch (lpsz[1])
	    {
	    case '%':
	      commandLine += '%';
	      break;
	    case 'f':
	      commandLine += path2.Get();
	      haveName = true;
	      break;
	    case 'l':
	      commandLine += NUMTOSTR(line);
	      haveLine = true;
	      break;
	    default:
	      FATAL_MIKTEX_ERROR
		(T_("StartEditor"),
		 T_("The editor command is not valid."),
		 (static_cast<const char *>
		  (g_pYapConfig->inverseSearchCommandLine)));
	    }
	  lpsz += 2;
	}
      else
	{
	  commandLine += *lpsz++;
	}
    }

  if (! haveName)
    {
      if (AfxMessageBox(IDP_CHOOSE_EDITOR, MB_YESNO, IDP_CHOOSE_EDITOR)
	  != IDYES)
	{
	  return;
	}
      CPropertySheet dlg (T_("Options"), 0, 0);
      InverseSearchOptionsPage pageIsearch;
      dlg.AddPage (&pageIsearch);
      if (dlg.DoModal () == IDOK)
	{
	  // <recursivecall>
	  StartEditor (lpszFileName, lpszDocDir, line);
	  // </recursivecall>
	}
      return;
    }

  // start process
  STARTUPINFO startupInfo;
  ZeroMemory (&startupInfo, sizeof(startupInfo));
  startupInfo.cb = sizeof(startupInfo);
  startupInfo.dwFlags |= STARTF_USESHOWWINDOW;
  startupInfo.wShowWindow = SW_SHOWNORMAL;
  PROCESS_INFORMATION processInfo;
  if (! ::CreateProcess(0,
			const_cast<char *>(commandLine.c_str()),
			0,
			0,
			FALSE,
			0,
			0,
			0,
			&startupInfo,
			&processInfo))
    {
      FATAL_WINDOWS_ERROR (T_("CreateProcess"), commandLine.c_str());
    }
  CloseHandle (processInfo.hThread);
  CloseHandle (processInfo.hProcess);
}

/* _________________________________________________________________________

   VYapLog
   _________________________________________________________________________ */

void
VYapLog (/*[in]*/ const char *	lpszFormat,
	 /*[in]*/ va_list		argptr)
{
  if (theApp.trace_yap.get() != 0)
    {
      theApp.trace_yap->VTrace (T_("yap"), lpszFormat, argptr);
    }
}

/* _________________________________________________________________________

   YapLog
   _________________________________________________________________________ */

void
YapLog (/*[in]*/ const char *	lpszFormat,
	/*[in]*/			...)
{
  if (theApp.trace_yap.get() != 0 && theApp.trace_yap->IsEnabled())
    {
      va_list argptr;
      va_start (argptr, lpszFormat);
      VYapLog (lpszFormat, argptr);
      va_end (argptr);
    }
}

/* _________________________________________________________________________

   TraceError
   _________________________________________________________________________ */

void
TraceError (/*[in]*/ const char *	lpszFormat,
	    /*[in]*/			...)
{
  if (theApp.trace_error.get() != 0)
    {
      va_list argptr;
      va_start (argptr, lpszFormat);
      theApp.trace_error->VTrace (T_("yap"), lpszFormat, argptr);
      va_end (argptr);
    }
}

/* _________________________________________________________________________

   YapApplication::OpenDocumentFile
   _________________________________________________________________________ */

CDocument *
YapApplication::OpenDocumentFile (/*[in]*/ const char * lpszFileName)
{
  try
    {
      PathName pathShort = lpszFileName;
#if defined(REMOVE_BLANKS_FROM_DOCUMENT_FILENAMES)
      Utils::RemoveBlanksFromPathName (pathShort);
#endif
      return (CWinApp::OpenDocumentFile(pathShort.Get()));
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (0, e);
      return (0);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (0, e);
      return (0);
    }
}

/* _________________________________________________________________________

   UpdateAllDviViews
   _________________________________________________________________________ */

void
UpdateAllDviViews (/*[in]*/ bool reread)
{
  YapApplication * pYapApp = reinterpret_cast<YapApplication*>(AfxGetApp());
  MIKTEX_ASSERT (pYapApp != 0);
  POSITION posTemplate = pYapApp->GetFirstDocTemplatePosition();
  while (posTemplate != 0)
    {
      CDocTemplate * pTemplate = pYapApp->GetNextDocTemplate(posTemplate);
      POSITION posDoc = pTemplate->GetFirstDocPosition();
      while (posDoc != 0)
	{
	  CDocument * pDoc = pTemplate->GetNextDoc(posDoc);
	  if (pDoc->IsKindOf(RUNTIME_CLASS(DviDoc)))
	    {
	      DviDoc * pDviDoc = reinterpret_cast<DviDoc*>(pDoc);
	      if (reread)
		{
		  pDviDoc->Reread ();
		}
	      pDviDoc->UpdateAllViews (0);
	    }
	}
    }
}

/* _________________________________________________________________________

   GetCommandPrefix
   _________________________________________________________________________ */

CString
GetCommandPrefix (/*[in]*/ bool bClear)
{
  ASSERT_VALID (AfxGetApp());
  ASSERT_VALID (AfxGetApp()->m_pMainWnd);
  MainFrame * pMain = reinterpret_cast<MainFrame*>(AfxGetApp()->m_pMainWnd);
  return (pMain->GetCommandPrefix(bClear));
}

/* _________________________________________________________________________

   AddCommandPrefixChar
   _________________________________________________________________________ */

bool
AddCommandPrefixChar (/*[in]*/ char ch)
{
  ASSERT_VALID (AfxGetApp());
  ASSERT_VALID (AfxGetApp()->m_pMainWnd);
  MainFrame * pMain = reinterpret_cast<MainFrame*>(AfxGetApp()->m_pMainWnd);
  return (pMain->AddCommandPrefixChar(ch));
}

/* _________________________________________________________________________

   YapApplication::OnViewTrace
   _________________________________________________________________________ */

void
YapApplication::OnViewTrace ()
{
  try
    {
      tracing = ! tracing;
      if (tracing)
	{
	  if (traceFlags.GetLength() == 0)
	    {
	      traceFlags = YAP_TRACE_FLAGS_LVL_3;
	    }
	}
      TraceStream::SetTraceFlags (tracing
				  ? traceFlags
				  : reinterpret_cast<const char *>(0));
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (0, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (0, e);
    }
}

/* _________________________________________________________________________

   YapApplication::OnUpdateViewTrace
   _________________________________________________________________________ */

void
YapApplication::OnUpdateViewTrace (/*[in]*/ CCmdUI * pCmdUI)
{
  pCmdUI->SetCheck (tracing ? 1 : 0);
}


/* _________________________________________________________________________

   YapApplication::OnRegisterMiKTeX
   _________________________________________________________________________ */

void
YapApplication::OnRegisterMiKTeX ()
{
  try
    {
      Utils::RegisterMiKTeXUser ();
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (0, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (0, e);
    }
}

/* _________________________________________________________________________

   AllowShellCommand
   _________________________________________________________________________ */

bool
AllowShellCommand (/*[in]*/ const CString & command)
{
  switch (g_pYapConfig->enableShellCommands)
    {
    default:
    case YapConfig::SEC_DISABLE_COMMANDS:
      return (false);
    case YapConfig::SEC_ASK_USER:
      {
	CString message;
	// <fixme>hard-coded string</fixme>
	message.Format ((T_("The following script is embedded in the ")
			 T_("document:\n\n%s\n\n")
			 T_("Do you allow to execute this script?")),
			static_cast<const char *>(command));
	return (AfxMessageBox(message, MB_YESNO | MB_ICONQUESTION) == IDYES);
      }
    case YapConfig::SEC_SECURE_COMMANDS:
      {
	CString name = command.SpanExcluding(" ");
	for (Tokenizer tok (g_pYapConfig->secureCommands, " ,;");
	     tok.GetCurrent() != 0;
	     ++ tok)
	  {
	    if (PathName::Compare(tok.GetCurrent(), name) == 0)
	      {
		return (true);
	      }
	  }
	return (false);
      }
    case YapConfig::SEC_ENABLE_ALL_COMMANDS:
      return (true);
    }
}
