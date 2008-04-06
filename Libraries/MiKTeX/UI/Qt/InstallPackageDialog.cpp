/* InstallPackageDialog.cpp:

   Copyright (C) 2008 Christian Schenk

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
#if defined(MIKTEX_WINDOWS)
      // show the Windows Vista shield icon
      bool restrictedUserSetup =
	(! (SessionWrapper(true)->IsSharedMiKTeXSetup() == TriState::True
	    || SessionWrapper(true)->IsUserAnAdministrator()));
      if (IsWindowsVista() && ! restrictedUserSetup)
	{
	  HWND hwnd = pOKButton->winId();
	  if (hwnd == 0)
	    {
	      UNEXPECTED_CONDITION
		("InstallPackageDialog::InstallPackageDialog");
	    }
	  Button_SetElevationRequiredState (hwnd, TRUE);
	}
#endif
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
