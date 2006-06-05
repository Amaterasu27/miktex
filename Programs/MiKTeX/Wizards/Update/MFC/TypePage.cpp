/* TypePage.cpp:

   Copyright (C) 2002-2006 Christian Schenk

   This file is part of the MiKTeX Update Wizard.

   The MiKTeX Update Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX Update Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX Update Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"
#include "Update.h"

#include "TypePage.h"
#include "UpdateWizard.h"

#include "ConnectionSettingsDialog.h"
#include "ProxyAuthenticationDialog.h"

IMPLEMENT_DYNCREATE(TypePage, CPropertyPage);

/* _________________________________________________________________________

   TypePage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(TypePage, CPropertyPage)
  ON_BN_CLICKED(IDC_INSTALL_FROM_CD, &TypePage::OnInstallFromCd)
  ON_BN_CLICKED(IDC_INSTALL_FROM_INTERNET, &TypePage::OnInstallFromInternet)
  ON_BN_CLICKED(IDC_INSTALL_FROM_LOCAL_REPOSITORY,
		&TypePage::OnInstallFromLocalRepository)
  ON_BN_CLICKED(IDC_CONNECTION_SETTINGS, &TypePage::OnConnectionSettings)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   TypePage::TypePage
   _________________________________________________________________________ */

TypePage::TypePage ()
  : CPropertyPage (IDD, 0, IDS_HEADER_TYPE, IDS_SUBHEADER_TYPE, 0),
    pSheet (0)
{
  m_psp.dwFlags &= ~ PSP_HASHELP;

  sourceChoice =
    SessionWrapper(true)->GetConfigValue(T_("Update"),
					 T_("lastSource"),
					 0);
  
  remoteChoice =
    SessionWrapper(true)->GetConfigValue(T_("Update"),
					 T_("lastRemote"),
					 0);
  
  localChoice =
    SessionWrapper(true)->GetConfigValue(T_("Update"),
					 T_("lastLocal"),
					 0);

  haveRemoteRepository = g_pManager->TryGetRemotePackageRepository(repository);

  if (haveRemoteRepository && remoteChoice == 2)
    {
      remoteChoice = 1;
    }

  if (! haveRemoteRepository && remoteChoice == 1)
    {
      remoteChoice = 2;
    }

  haveLocalRepository =
    g_pManager->TryGetLocalPackageRepository(localDirectory);

  if (haveLocalRepository && localChoice == 1)
    {
      localChoice = 0;
    }

  if (! haveLocalRepository && localChoice == 0)
    {
      localChoice = 1;
    }
}

/* _________________________________________________________________________

   TypePage::OnInitDialog
   _________________________________________________________________________ */

BOOL
TypePage::OnInitDialog ()
{
  pSheet = reinterpret_cast<UpdateWizard*>(GetParent());
  BOOL ret = CPropertyPage::OnInitDialog();
  try
    {
      if (haveRemoteRepository)
	{
	  tstring protocol;
	  tstring host;
	  SplitUrl (repository, protocol, host);
	  CString text;
	  text.Format (T_("%s://%s (last used)"),
		       protocol.c_str(),
		       host.c_str());
	  GetControl(IDC_LAST_USED_REPOSITORY)->SetWindowText (text);
	}

      if (haveLocalRepository)
	{
	  CString text;
	  text.Format (T_("%s (last used)"), localDirectory.Get());
	  GetControl(IDC_LAST_USED_DIRECTORY)->SetWindowText (text);
	}

      EnableButtons ();
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
    }
  return (ret);
}

/* _________________________________________________________________________

   TypePage::OnSetActive
   _________________________________________________________________________ */

BOOL
TypePage::OnSetActive ()
{
  noDdv = false;
  if (g_upgrading)
    {
      pSheet->SetWizardButtons (PSWIZB_NEXT);
    }
  else
    {
      pSheet->SetWizardButtons (PSWIZB_NEXT | PSWIZB_BACK);
    }
  return (CPropertyPage::OnSetActive());
}

/* _________________________________________________________________________

   TypePage::DoDataExchange
   _________________________________________________________________________ */

void
TypePage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Radio (pDX, IDC_INSTALL_FROM_INTERNET, sourceChoice);
  DDX_Radio (pDX, IDC_RANDOM, remoteChoice);
  DDX_Radio (pDX, IDC_LAST_USED_DIRECTORY, localChoice);
}

/* _________________________________________________________________________

   TypePage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
TypePage::OnWizardNext ()
{
  ASSERT (IDC_INSTALL_FROM_INTERNET < IDC_INSTALL_FROM_CD);
  int controlId =
    GetCheckedRadioButton(IDC_INSTALL_FROM_INTERNET, IDC_INSTALL_FROM_CD);
  int nextPage = -1;
  if (controlId == IDC_INSTALL_FROM_INTERNET)
    {
      ASSERT (IDC_RANDOM < IDC_CHOOSE_REPOSITORY);
      ProxySettings proxySettings;
      if (PackageManager::TryGetProxy(proxySettings)
	  && proxySettings.authenticationRequired
	  && proxySettings.user.empty())
	{
	  ProxyAuthenticationDialog dlg (this);
	  if (dlg.DoModal() != IDOK)
	    {
	      return (-1);
	    }
	  proxySettings.user = dlg.GetName();
	  proxySettings.password = dlg.GetPassword();
	  PackageManager::SetProxy (proxySettings);
	}
      int controlId2 =
	GetCheckedRadioButton(IDC_RANDOM, IDC_CHOOSE_REPOSITORY);
      if (controlId2 == IDC_RANDOM)
	{
	  nextPage = IDD_PACKAGE_LIST;
	}
      else if (controlId2 == IDC_LAST_USED_REPOSITORY)
	{
	  nextPage = IDD_PACKAGE_LIST;
	}
      else if (controlId2 == IDC_CHOOSE_REPOSITORY)
	{
	  nextPage = IDD_REMOTE_REPOSITORY;
	}
    }
  else if (controlId == IDC_INSTALL_FROM_LOCAL_REPOSITORY)
    {
      ASSERT (IDC_LAST_USED_DIRECTORY < IDC_SPECIFY_DIRECTORY);
      int controlId2 =
	GetCheckedRadioButton(IDC_LAST_USED_DIRECTORY, IDC_SPECIFY_DIRECTORY);
      if (controlId2 == IDC_LAST_USED_DIRECTORY)
	{
	  nextPage = IDD_PACKAGE_LIST;
	}
      else if (controlId2 == IDC_SPECIFY_DIRECTORY)
	{
	  nextPage = IDD_LOCAL_REPOSITORY;
	}
    }
  else if (controlId == IDC_INSTALL_FROM_CD)
    {
      nextPage = IDD_CD;
    }
  pSheet->SetCameFrom (IDD);
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(nextPage)));
}

/* _________________________________________________________________________

   TypePage::OnWizardBack
   _________________________________________________________________________ */

LRESULT
TypePage::OnWizardBack ()
{
  noDdv = true;
  return (CPropertyPage::OnWizardBack());
}

/* _________________________________________________________________________

   TypePage::OnKillActive
   _________________________________________________________________________ */

BOOL
TypePage::OnKillActive ()
{
  BOOL ret = CPropertyPage::OnKillActive();
  if (ret && ! noDdv)
    {
      try
	{
	  SessionWrapper(true)->SetUserConfigValue (T_("Update"),
						    T_("lastSource"),
						    sourceChoice);
	  SessionWrapper(true)->SetUserConfigValue (T_("Update"),
						    T_("lastRemote"),
						    remoteChoice);
	  SessionWrapper(true)->SetUserConfigValue (T_("Update"),
						    T_("lastLocal"),
						    localChoice);
	  pSheet->SetRandomRepositoryFlag (remoteChoice == 0);
	  pSheet->SetRepositoryType (sourceChoice == 0
				     ? RepositoryType::Remote
				     : (sourceChoice == 1
					? RepositoryType::Local
					: RepositoryType::MiKTeXDirect));
	}
      catch (const MiKTeXException & e)
	{
	  pSheet->ReportError (e);
	  ret = FALSE;
	}
      catch (const exception & e)
	{
	  pSheet->ReportError (e);
	  ret = FALSE;
	}
    }
  return (ret);
}

/* _________________________________________________________________________

   TypePage::OnInstallFromInternet
   _________________________________________________________________________ */

void
TypePage::OnInstallFromInternet ()
{
  try
    {
      sourceChoice = 0;
      EnableButtons ();
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
    }
}

/* _________________________________________________________________________

   TypePage::OnInstallFromLocalRepository
   _________________________________________________________________________ */

void
TypePage::OnInstallFromLocalRepository ()
{
  try
    {
      sourceChoice = 1;
      EnableButtons ();
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
    }
}

/* _________________________________________________________________________

   TypePage::OnInstallFromCd
   _________________________________________________________________________ */

void
TypePage::OnInstallFromCd ()
{
  try
    {
      sourceChoice = 2;
      EnableButtons ();
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
    }
}

/* _________________________________________________________________________

   TypePage::EnableButtons
   _________________________________________________________________________ */

void
TypePage::EnableButtons ()
{
  EnableControl (IDC_RANDOM, sourceChoice == 0);
  EnableControl (IDC_LAST_USED_REPOSITORY,
		 sourceChoice == 0 && haveRemoteRepository);
  EnableControl (IDC_CHOOSE_REPOSITORY, sourceChoice == 0);

  EnableControl (IDC_LAST_USED_DIRECTORY,
		 sourceChoice == 1 && haveLocalRepository);
  EnableControl (IDC_SPECIFY_DIRECTORY, sourceChoice == 1);
}

/* _________________________________________________________________________

   TypePage::EnableControl
   _________________________________________________________________________ */

void
TypePage::EnableControl (/*[in]*/ UINT	controlId,
			 /*[in]*/ bool	enable)
{
  GetControl(controlId)->EnableWindow (enable ? TRUE : FALSE);
  CHECK_WINDOWS_ERROR (T_("CWnd::EnableWindow"), 0);
}

/* _________________________________________________________________________

   TypePage::GetControl
   _________________________________________________________________________ */

CWnd *
TypePage::GetControl (/*[in]*/ UINT	controlId)
{
  CWnd * pWnd = GetDlgItem(controlId);
  if (pWnd == 0)
    {
      UNEXPECTED_CONDITION (T_("TypePage::GetControl"));
    }
  return (pWnd);
}

/* _________________________________________________________________________

   TypePage::OnConnectionSettings
   _________________________________________________________________________ */

void
TypePage::OnConnectionSettings ()
{
  ConnectionSettingsDialog dlg (this);
  dlg.DoModal ();
}
