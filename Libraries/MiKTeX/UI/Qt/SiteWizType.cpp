/* SiteWizType.cpp:

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

#include "ConnectionSettingsDialog.h"
#include "SiteWizSheet.h"
#include "SiteWizType.h"

#include <miktex/UI/Qt/ErrorDialog>

/* _________________________________________________________________________

   SiteWizType::SiteWizType
   _________________________________________________________________________ */

SiteWizType::SiteWizType ()
  : QWizardPage (0)
{
  setupUi (this);
  try
    {
      string urlOrPath;
      RepositoryType repositoryType (RepositoryType::Unknown);
      if (PackageManager::TryGetDefaultPackageRepository(repositoryType,
							 urlOrPath))
	{
	  switch (repositoryType.Get())
	    {
	    case RepositoryType::Remote:
	      rbRemote->setChecked (true);
	      break;
	    case RepositoryType::Local:
	      rbLocal->setChecked (true);
	      break;
	    case RepositoryType::MiKTeXDirect:
	      rbCD->setChecked (true);
	      break;
	    }
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

   SiteWizType::nextId
   _________________________________________________________________________ */

int
SiteWizType::nextId ()
  const
{
  if (rbRemote->isChecked())
    {
      return (SiteWizSheetImpl::Page_Remote);
    }
  else if (rbLocal->isChecked())
    {
      return (SiteWizSheetImpl::Page_Local);
    }
  else if (rbCD->isChecked())
    {
      return (SiteWizSheetImpl::Page_CD);
    }
  else
    {
      return (-1);
    }
}

/* _________________________________________________________________________

   SiteWizType::isComplete
   _________________________________________________________________________ */

bool
SiteWizType::isComplete ()
  const
{
  return (nextId() >= 0);
}

/* _________________________________________________________________________

   SiteWizType::initializePage
   _________________________________________________________________________ */

void
SiteWizType::initializePage ()
{
  emit completeChanged();
}

/* _________________________________________________________________________

   SiteWizType::validatePage
   _________________________________________________________________________ */

bool
SiteWizType::validatePage ()
{
  try
    {
      if (rbRemote->isChecked())
	{
	  if (! ProxyAuthenticationDialog(this))
	    {
	      return (false);
	    }
	}
      return (true);
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
      return (false);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
      return (false);
    }
}

/* _________________________________________________________________________

   SiteWizType::on_btnConnectionSettings_clicked
   _________________________________________________________________________ */

void
SiteWizType::on_btnConnectionSettings_clicked ()
{
  ConnectionSettingsDialog dlg (this);
  dlg.exec ();
}
