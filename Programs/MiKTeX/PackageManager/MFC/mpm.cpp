/* mpm.cpp:

   Copyright (C) 2002-2011 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "stdafx.h"
#include "mpm.h"

#include "MainFrame.h"
#include "MpmDoc.h"
#include "MpmView.h"

/* _________________________________________________________________________

   Application Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(PackageManagerApplication, CWinApp)
  ON_COMMAND(ID_APP_ABOUT, &PackageManagerApplication::OnAppAbout)
  ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
  ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
  ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   PackageManagerApplication::PackageManagerApplication
   _________________________________________________________________________ */

PackageManagerApplication::PackageManagerApplication ()
{
  SetAppID (CA2T("MiKTeXorg.MiKTeX.PackageManaer." MIKTEX_COMPONENT_VERSION_STR));
  EnableHtmlHelp ();
}

/* _________________________________________________________________________

   The one and only PackageManagerApplication object.
   _________________________________________________________________________ */

PackageManagerApplication theApp;

/* _________________________________________________________________________

   PackageManagerApplication::InitInstance
   _________________________________________________________________________ */

BOOL
PackageManagerApplication::InitInstance ()
{
  CoInitializeEx (0, COINIT_APARTMENTTHREADED);

  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(InitCtrls);
  InitCtrls.dwICC = ICC_WIN95_CLASSES;
  InitCommonControlsEx (&InitCtrls);

  if (! CWinApp::InitInstance())
    {
      CoUninitialize ();
      return (FALSE);
    }

  EnableTaskbarInteraction (FALSE);

  try
    {
      pSession.CreateSession (Session::InitInfo("mpm"));
      TraceStream::SetTraceFlags ("error,mpm,config");
      
      SetRegistryKey (CA2T(MIKTEX_COMP_COMPANY_STR
		      "\\"
		      MIKTEX_PRODUCTNAME_STR
		      "\\"
		      MIKTEX_SERIES_STR));
      
      LoadStdProfileSettings (0);
      
      CSingleDocTemplate * pDocTemplate =
	new CSingleDocTemplate(IDR_MAINFRAME,
			       RUNTIME_CLASS(MpmDoc),
			       RUNTIME_CLASS(MainFrame),
			       RUNTIME_CLASS(MpmView));
      
      AddDocTemplate (pDocTemplate);
      
      CCommandLineInfo cmdInfo;
      ParseCommandLine (cmdInfo);
      
      if (! ProcessShellCommand(cmdInfo))
	{
	  pSession.Reset ();
	  CoUninitialize ();
	  return (FALSE);
	}
      
      m_pMainWnd->ShowWindow (SW_SHOW);
      m_pMainWnd->UpdateWindow ();

      return (TRUE);
    }

  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (0, e);
      pSession.Reset ();
      CoUninitialize ();
      return (FALSE);
    }

  catch (const exception & e)
    {
      ErrorDialog::DoModal (0, e);
      pSession.Reset ();
      CoUninitialize ();
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
  OnClickRegisterMiKTeXUser ();
};

/* _________________________________________________________________________

   AboutDialog Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(AboutDialog, CDialog)
  ON_BN_CLICKED(ID_REGISTER_MIKTEX, &AboutDialog::OnClickRegisterMiKTeXUser)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   AboutDialog::AboutDialog
   _________________________________________________________________________ */

AboutDialog::AboutDialog ()
  : CDialog (IDD)
{
}

/* _________________________________________________________________________

   AboutDialog::DoDataExchange
   _________________________________________________________________________ */

void
AboutDialog::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  try
    {
      CDialog::DoDataExchange (pDX);
      if (! pDX->m_bSaveAndValidate)
	{
	  CString str;
	  str.Format (T_(_T("MiKTeX Package Manager (windowed mode) version %s")),
		      static_cast<LPCTSTR>(CA2T(MIKTEX_COMPONENT_VERSION_STR)));
	  str += _T("\r\n");
	  str += MIKTEX_COMP_COPYRIGHT_STR;
	  CWnd * pWnd = GetDlgItem(IDC_THE_NAME_OF_THE_GAME);
	  if (pWnd == 0)
	    {
	      UNEXPECTED_CONDITION ("AboutDialog::DoDataExchange");
	    }
	  pWnd->SetWindowText (str);
	}
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

   AboutDialog::OnClickRegisterMiKTeXUser
   _________________________________________________________________________ */

void
AboutDialog::OnClickRegisterMiKTeXUser ()
{
  try
  {
#if HAVE_MIKTEX_USER_INFO
    MiKTeXUserInfo info;
    SessionWrapper(true)->RegisterMiKTeXUser (info);
#else
    Utils::RegisterMiKTeXUser ();
#endif
  }
  catch (const OperationCancelledException &)
  {
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

   PackageManagerApplication::OnAppAbout
   _________________________________________________________________________ */

void
PackageManagerApplication::OnAppAbout ()
{
  AboutDialog aboutDlg;
  aboutDlg.DoModal ();
}

/* _________________________________________________________________________

   PackageManagerApplication::ExitInstance
   _________________________________________________________________________ */

int
PackageManagerApplication::ExitInstance ()
{
  try
    {
      pSession.Reset ();
    }
  catch (const exception &)
    {
    }
  int ret = CWinApp::ExitInstance();
  CoUninitialize ();
  return (ret);
}
