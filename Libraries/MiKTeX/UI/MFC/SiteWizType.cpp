/* SiteWizType.cpp:

   Copyright (C) 2000-2007 Christian Schenk

   This file is part of the MiKTeX UI Library.

   The MiKTeX UI Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX UI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX UI Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"
#include "internal.h"
#include "resource.h"

#include "SiteWizType.h"

#include "ConnectionSettingsDialog.h"

/* _________________________________________________________________________

   SiteWizType::SiteWizType
   _________________________________________________________________________ */

SiteWizType::SiteWizType ()
  : CPropertyPage (IDD_SITEWIZ_TYPE, IDS_SITEWIZ),
    repositoryType (-1)
{
  m_psp.dwFlags &= ~ PSP_HASHELP;
  string urlOrPath;
  RepositoryType repositoryType (RepositoryType::Unknown);
  if (PackageManager::TryGetDefaultPackageRepository(repositoryType,
						     urlOrPath))
    {
      switch (repositoryType.Get())
	{
	case RepositoryType::Remote:
	  this->repositoryType = 0;
	  break;
	case RepositoryType::Local:
	  this->repositoryType = 1;
	  break;
	case RepositoryType::MiKTeXDirect:
	  this->repositoryType = 2;
	  break;
	}
    }
}

/* _________________________________________________________________________

   SiteWizType Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(SiteWizType, CPropertyPage)
  ON_BN_CLICKED (IDC_INSTALL_FROM_CD, OnInstallFromCD)
  ON_BN_CLICKED (IDC_INSTALL_FROM_INTERNET, OnInstallFromInternet)
  ON_BN_CLICKED (IDC_INSTALL_FROM_LOCAL_REPOSITORY,
		 OnInstallFromLocalRepository)
  ON_BN_CLICKED (IDC_CONNECTION_SETTINGS,
		 &SiteWizType::OnConnectionSettings)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   SiteWizType::DoDataExchange
   _________________________________________________________________________ */

void
SiteWizType::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Radio (pDX, IDC_INSTALL_FROM_INTERNET, repositoryType);
}

/* _________________________________________________________________________

   SiteWizType::OnSetActive
   _________________________________________________________________________ */

BOOL
SiteWizType::OnSetActive ()
{
  BOOL ret = CPropertyPage::OnSetActive();
  try
    {
      CPropertySheet * pSheet = reinterpret_cast<CPropertySheet*>(GetParent());
      ASSERT_KINDOF (CPropertySheet, pSheet);
      pSheet->SetWizardButtons (repositoryType >= 0 ? PSWIZB_NEXT : 0);
      EnableButtons ();
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  return (ret);
}

/* _________________________________________________________________________

   SiteWizType::OnInstallFromLocalRepository
   _________________________________________________________________________ */

void
SiteWizType::OnInstallFromLocalRepository ()
{
  try
    {
      CPropertySheet * pSheet = reinterpret_cast<CPropertySheet*>(GetParent());
      ASSERT_KINDOF (CPropertySheet, pSheet);
      pSheet->SetWizardButtons (PSWIZB_NEXT);
      EnableButtons ();
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

   SiteWizType::OnInstallFromInternet
   _________________________________________________________________________ */

void
SiteWizType::OnInstallFromInternet ()
{
  try
    {
      CPropertySheet * pSheet = reinterpret_cast<CPropertySheet*>(GetParent());
      ASSERT_KINDOF (CPropertySheet, pSheet);
      pSheet->SetWizardButtons (PSWIZB_NEXT);
      EnableButtons ();
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

   SiteWizType::OnInstallFromCD
   _________________________________________________________________________ */

void
SiteWizType::OnInstallFromCD ()
{
  try
    {
      CPropertySheet * pSheet = reinterpret_cast<CPropertySheet*>(GetParent());
      ASSERT_KINDOF (CPropertySheet, pSheet);
      pSheet->SetWizardButtons (PSWIZB_NEXT);
      EnableButtons ();
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

   SiteWizType::OnWizardNext
   _________________________________________________________________________ */

LRESULT
SiteWizType::OnWizardNext ()
{
  try
    {
      ASSERT (IDC_INSTALL_FROM_INTERNET < IDC_INSTALL_FROM_CD);
      int checked =
	GetCheckedRadioButton(IDC_INSTALL_FROM_INTERNET, IDC_INSTALL_FROM_CD);
      DWORD nextPage;
      if (checked == IDC_INSTALL_FROM_INTERNET)
	{
	  if (! ProxyAuthenticationDialog(GetSafeHwnd()))
	    {
	      return (-1);
	    }
	  nextPage = IDD_SITEWIZ_REMOTE_REPOSITORY;
	}
      else if (checked == IDC_INSTALL_FROM_LOCAL_REPOSITORY)
	{
      nextPage = IDD_SITEWIZ_LOCAL_REPOSITORY;
	}
      else
	{
	  nextPage = IDD_SITEWIZ_DRIVE;
	}
      return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(nextPage)));
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
      return (-1);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
      return (-1);
    }
}

/* _________________________________________________________________________

   SiteWizType::OnConnectionSettings
   _________________________________________________________________________ */

void
SiteWizType::OnConnectionSettings ()
{
  ConnectionSettingsDialog dlg (this);
  dlg.DoModal ();
}

/* _________________________________________________________________________

   SiteWizType::EnableButtons
   _________________________________________________________________________ */

void
SiteWizType::EnableButtons ()
{
  int checked =
    GetCheckedRadioButton(IDC_INSTALL_FROM_INTERNET, IDC_INSTALL_FROM_CD);
  CWnd * pWnd = GetDlgItem(IDC_CONNECTION_SETTINGS);
  if (pWnd == 0)
    {
      UNEXPECTED_CONDITION (T_("TaskPage::EnableButtons"));
    }
  pWnd->EnableWindow (checked == IDC_INSTALL_FROM_INTERNET);
}
