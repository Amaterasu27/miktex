/* InstallPackageDialog.cpp:

   Copyright (C) 2008-2015 Christian Schenk

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

#include "InstallPackageDialog.h"

#include <miktex/UI/Qt/ErrorDialog>
#include <miktex/UI/Qt/SiteWizSheet>

/* _________________________________________________________________________

   InstallPackageDialog::InstallPackageDialog
   _________________________________________________________________________ */

InstallPackageDialog::InstallPackageDialog
(/*[in]*/ QWidget *		pParent,
 /*[in]*/ PackageManager *	pManager,
 /*[in]*/ const char *		lpszPackageName,
 /*[in]*/ const char *		lpszTrigger)
  : QDialog (pParent),
    pManager (pManager)
{
  setupUi (this);
  try
    {
      QPushButton * pOKButton = buttonBox->button(QDialogButtonBox::Ok);
      if (pOKButton == 0)
	{
	  UNEXPECTED_CONDITION ("InstallPackageDialog::InstallPackageDialog");
	}
      pOKButton->setText (T_("Install"));
      lblPackageName->setText (QString::fromLocal8Bit(lpszPackageName));
      lblMissingFile->setText (QString::fromLocal8Bit(lpszTrigger));
      PackageInfo packageInfo = pManager->GetPackageInfo(lpszPackageName);
      string repository;
      RepositoryType repositoryType (RepositoryType::Unknown);
      if (pManager->TryGetDefaultPackageRepository(repositoryType, repository))
	{
	  leInstallationSource->setText
	    (QString::fromLocal8Bit(repository.c_str()));
	}
      else
	{
	  leInstallationSource->setText (T_("<Random package repository>"));
	}
      PathName commonInstallRoot = SessionWrapper(true)->GetSpecialPath(SpecialPath::CommonInstallRoot);
      PathName userInstallRoot = SessionWrapper(true)->GetSpecialPath(SpecialPath::UserInstallRoot);
      bool enableCommonInstall = (commonInstallRoot != userInstallRoot);
      enableCommonInstall = (enableCommonInstall && Directory::Exists(commonInstallRoot));
#if defined(MIKTEX_WINDOWS)
      enableCommonInstall = (enableCommonInstall
	&& (SessionWrapper(true)->IsUserAnAdministrator() || SessionWrapper(true)->IsUserAPowerUser()));
#else
      enableCommonInstall = (enableCommonInstall && SessionWrapper(true)->IsUserAnAdministrator());
#endif
      if (enableCommonInstall)
      {
	cbInstallationDirectory->addItem (T_("Anyone who uses this computer (all users)"), true);
      }
      if (! enableCommonInstall || commonInstallRoot != userInstallRoot)
      {
	QString currentUser;
#if defined(MIKTEX_WINDOWS)
	char szLogonName[30];
	DWORD sizeLogonName = sizeof(szLogonName) / sizeof(szLogonName[0]);
	if (GetUserNameA(szLogonName, &sizeLogonName))
	{
	  currentUser = szLogonName;
	}
	else if (GetLastError() == ERROR_NOT_LOGGED_ON)
	{
	  currentUser = T_("Unknown user");
	}
	else
	{
	  FATAL_WINDOWS_ERROR ("GetUserNameA", 0);
	}
	char szDisplayName[30];
	ULONG sizeDisplayName =
	  sizeof(szDisplayName) / sizeof(szDisplayName[0]);
	if (GetUserNameExA(NameDisplay, szDisplayName, &sizeDisplayName))
	{
	  currentUser += " (";
	  currentUser += szDisplayName;
	  currentUser += ")";
	}
#else
	currentUser = T_("The current user");
#endif
	cbInstallationDirectory->addItem (currentUser, false);
      }
      cbInstallationDirectory->setCurrentIndex (0);
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

   InstallPackageDialog::on_btnChange_clicked()
   _________________________________________________________________________ */

void
InstallPackageDialog::on_btnChange_clicked ()
{
  try
    {
      if (SiteWizSheet::DoModal(this) != QDialog::Accepted)
	{
	  return;
	}
      string repository;
      RepositoryType repositoryType (RepositoryType::Unknown);
      if (pManager->TryGetDefaultPackageRepository(repositoryType,
						   repository))
	{
	  leInstallationSource->setText
	    (QString::fromLocal8Bit(repository.c_str()));
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

   InstallPackageDialog::on_cbInstallationDirectory_currentIndexChanged()
   _________________________________________________________________________ */

void
InstallPackageDialog::on_cbInstallationDirectory_currentIndexChanged (int idx)
{
  if (idx < 0)
  {
    return;
  }
  bool elevationRequired = cbInstallationDirectory->itemData(idx).toBool();
  QPushButton * pOKButton = buttonBox->button(QDialogButtonBox::Ok);
  if (elevationRequired)
  {
    bool iconSet = false;
#if defined(MIKTEX_WINDOWS)
    if (IsWindowsVista())
    {
      DllProc3<HRESULT, SHSTOCKICONID, UINT, SHSTOCKICONINFO *>
	shGetStockIconInfo ("Shell32.dll", "SHGetStockIconInfo");
      SHSTOCKICONINFO sii;
      sii.cbSize = sizeof(sii);
      if (SUCCEEDED(shGetStockIconInfo(SIID_SHIELD, SHGSI_ICON | SHGSI_SMALLICON, &sii)))
      {
	HICON hiconShield  = sii.hIcon;
	ICONINFO iconInfo;
	if (GetIconInfo(hiconShield, &iconInfo))
	{
          QPixmap pixmapShield = QtWin::fromHBITMAP(iconInfo.hbmColor, QtWin::HBitmapAlpha);
	  DeleteObject (iconInfo.hbmColor);
	  DeleteObject (iconInfo.hbmMask);
	  pOKButton->setIcon (QIcon(pixmapShield));
	  iconSet = true;
	}
	DestroyIcon (hiconShield);
      }
    }
#endif
    if (! iconSet)
    {
      pOKButton->setIcon (QIcon(":/Icons/elevationrequired16x16.png"));
    }
  }
  else
  {
    pOKButton->setIcon (QIcon());
  }
}
