/* InstallPackageDialog.cpp:

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

#include "InstallPackageDialog.h"

#include <MiKTeX/UI/MFC/SiteWizSheet>

/* _________________________________________________________________________

   InstallPackageDialog::InstallPackageDialog
   _________________________________________________________________________ */

InstallPackageDialog::InstallPackageDialog
(/*[in]*/ CWnd *		pParent,
 /*[in]*/ PackageManager *	pManager,
 /*[in]*/ const char *	lpszPackageName,
 /*[in]*/ const char *	lpszTrigger)
  : CDialog (InstallPackageDialog::IDD, pParent),
    alwaysAsk (TRUE),
    pManager (pManager),
    trigger (lpszTrigger == 0 ? "" : lpszTrigger)
{
  PackageInfo packageInfo = pManager->GetPackageInfo(lpszPackageName);
  packageName = packageInfo.deploymentName.c_str();
  packageTitle = packageInfo.title.c_str();
}

/* _________________________________________________________________________

   InstallPackageDialog::FormatControlText
   _________________________________________________________________________ */

void
InstallPackageDialog::FormatControlText
(/*[in]*/ UINT			ctrlId,
 /*[in]*/ const char *	lpszFormat,
 /*[in]*/			...)
{
  CWnd * pWnd = GetDlgItem(ctrlId);
  if (pWnd == 0)
    {
      UNEXPECTED_CONDITION ("InstallPackageDialog::FormatControlText");
    }
  va_list marker;
  va_start (marker, lpszFormat);
  string str = Utils::FormatString(lpszFormat, marker);
  va_end (marker);
  pWnd->SetWindowText (str.c_str());
}

/* _________________________________________________________________________

   InstallPackageDialog::OnInitDialog
   _________________________________________________________________________ */

BOOL
InstallPackageDialog::OnInitDialog ()
{
  BOOL ret = CDialog::OnInitDialog();
  try
    {
      string repository;
      RepositoryType repositoryType (RepositoryType::Unknown);
      if (pManager->TryGetDefaultPackageRepository(repositoryType, repository))
	{
	  FormatControlText (IDC_EDIT_REPOSITORY_LOCATION,
			     "%s",
			     repository.c_str());
	}
      else
	{
	  FormatControlText (IDC_EDIT_REPOSITORY_LOCATION,
			     "%s",
			     T_("<Random package repository>"));
	}
      bool restrictedUserSetup =
	(! (SessionWrapper(true)->IsSharedMiKTeXSetup() == TriState::True
	    || SessionWrapper(true)->IsUserAnAdministrator()));
      if (IsWindowsVista() && ! restrictedUserSetup)
	{
	  HWND hwnd = ::GetDlgItem(m_hWnd, IDOK);
	  if (hwnd == 0)
	    {
	      UNEXPECTED_CONDITION ("InstallPackageDialog::OnInitDialog");
	    }
	  Button_SetElevationRequiredState (hwnd, TRUE);
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
  return (ret);
}

/* _________________________________________________________________________

   InstallPackageDialog::DoDataExchange
   _________________________________________________________________________ */

void
InstallPackageDialog::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);
  DDX_Check (pDX, IDC_CHECK_ALWAYS_ASK, alwaysAsk);
  DDX_Text (pDX, IDC_PACKAGE_NAME, packageName);
  DDX_Text (pDX, IDC_PACKAGE_FILE, trigger);
  DDX_Text (pDX, IDC_PACKAGE_TITLE, packageTitle);
}

/* _________________________________________________________________________

   InstallPackageDialog Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(InstallPackageDialog, CDialog)
  ON_BN_CLICKED(IDC_BUTTON_CHANGE, OnChangeRepository)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   InstallPackageDialog::OnChangeRepository
   _________________________________________________________________________ */

void
InstallPackageDialog::OnChangeRepository ()
{
  try
    {
      if (SiteWizSheet::DoModal(this) != ID_WIZFINISH)
	{
	  return;
	}
      string repository;
      RepositoryType repositoryType (RepositoryType::Unknown);
      if (pManager->TryGetDefaultPackageRepository(repositoryType,
						   repository))
	{
	  FormatControlText (IDC_EDIT_REPOSITORY_LOCATION,
			     "%s",
			     repository.c_str());
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
