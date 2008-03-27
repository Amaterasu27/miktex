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

#if 0				// TODO
#include <miktex/UI/Qt/SiteWizSheet>
#endif

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
      lblPackageName->setText (QString::fromLocal8Bit(lpszPackageName));
      lblMissingFile->setText (QString::fromLocal8Bit(lpszTrigger));
      PackageInfo packageInfo = pManager->GetPackageInfo(lpszPackageName);
      string repository;
      RepositoryType repositoryType (RepositoryType::Unknown);
      if (pManager->TryGetDefaultPackageRepository(repositoryType, repository))
	{
	  editInstallationSource->setText
	    (QString::fromLocal8Bit(repository.c_str()));
	}
      else
	{
	  editInstallationSource->setText (T_("<Random package repository>"));
	}
#if defined(MIKTEX_WINDOWS)
      // show the Windows Vista shield icon
      bool restrictedUserSetup =
	(! (SessionWrapper(true)->IsSharedMiKTeXSetup() == TriState::True
	    || SessionWrapper(true)->IsUserAnAdministrator()));
      if (IsWindowsVista() && ! restrictedUserSetup)
	{
	  HWND hwnd = btnInstall->winId();
	  if (hwnd == 0)
	    {
	      UNEXPECTED_CONDITION ("InstallPackageDialog::OnInitDialog");
	    }
	  Button_SetElevationRequiredState (hwnd, TRUE);
	}
#endif
    }
  catch (const MiKTeXException & e)
    {
#if 0				// TODO
      ErrorDialog::DoModal (this, e);
#endif
    }
  catch (const exception & e)
    {
#if 0				// TODO
      ErrorDialog::DoModal (this, e);
#endif
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
#if 0				// TODO
      if (SiteWizSheet::DoModal(this) != ID_WIZFINISH)
	{
	  return;
	}
#endif
      string repository;
      RepositoryType repositoryType (RepositoryType::Unknown);
      if (pManager->TryGetDefaultPackageRepository(repositoryType,
						   repository))
	{
	  editInstallationSource->setText
	    (QString::fromLocal8Bit(repository.c_str()));
	}
    }
  catch (const MiKTeXException & e)
    {
#if 0				// TODO
      ErrorDialog::DoModal (this, e);
#endif
    }
  catch (const exception & e)
    {
#if 0				// TODO
      ErrorDialog::DoModal (this, e);
#endif
    }
}
