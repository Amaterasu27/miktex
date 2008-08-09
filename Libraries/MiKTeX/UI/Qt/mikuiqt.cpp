/* mikuiqt.cpp:

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
#include "ProxyAuthenticationDialog.h"

static QApplication * pApplication = 0;

/* _________________________________________________________________________
   
   MiKTeX::UI::Qt::InitializeFramework
   _________________________________________________________________________ */

MIKTEXUIQTEXPORT
void
MIKTEXCEECALL
MiKTeX::UI::Qt::InitializeFramework ()
{
#ifdef Q_WS_X11
  bool useGUI = (getenv("DISPLAY") != 0);
#else
  bool useGUI = true;
#endif
  static int argc = 0;
  static char ** argv = 0;
  pApplication = new QApplication (argc, argv, useGUI);
}

/* _________________________________________________________________________
   
   MiKTeX::UI::Qt::FinalizeFramework
   _________________________________________________________________________ */

MIKTEXUIQTEXPORT
void
MIKTEXCEECALL
MiKTeX::UI::Qt::FinalizeFramework ()
{
  delete pApplication;
  pApplication = 0;
}

/* _________________________________________________________________________
   
   MiKTeX::UI::Qt::InstallPackageMessageBox
   _________________________________________________________________________ */

MIKTEXUIQTEXPORT
unsigned int
MIKTEXCEECALL
MiKTeX::UI::Qt::InstallPackageMessageBox
(/*[in]*/ QWidget *		pParent,
 /*[in]*/ PackageManager *	pManager,
 /*[in]*/ const char *		lpszPackageName,
 /*[in]*/ const char *		lpszTrigger)
{
  SessionWrapper pSession (true);
  TriState enableInstaller
    = pSession->GetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			       MIKTEX_REGVAL_AUTO_INSTALL,
			       TriState(TriState::Undetermined));
  unsigned int ret;
  if (enableInstaller != TriState::Undetermined)
    {
      ret = DONTASKAGAIN;
      ret |= (enableInstaller == TriState::True ? YES : NO);
    }
  else
    {
      InstallPackageDialog dlg (pParent,
				pManager,
				lpszPackageName,
				lpszTrigger);
      dlg.SetAlwaysAsk (enableInstaller == TriState::True ? false : true);
      int dialogCode = dlg.exec();
      if (dialogCode != QDialog::Accepted && dialogCode != QDialog::Rejected)
	{
	  ret = (NO | DONTASKAGAIN);
	}
      else
	{
	  ret = (dialogCode == QDialog::Accepted ? YES : NO);
	  if (dialogCode == QDialog::Accepted && ! dlg.GetAlwaysAsk())
	    {
	      pSession->SetUserConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
					    MIKTEX_REGVAL_AUTO_INSTALL,
					    "1");
	    }
	}
    }
  return (ret);
}

/* _________________________________________________________________________
   
   MiKTeX::UI::Qt::ProxyAuthenticationDialog
   _________________________________________________________________________ */

MIKTEXUIQTEXPORT
bool
MIKTEXCEECALL
MiKTeX::UI::Qt::ProxyAuthenticationDialog (/*[in]*/ QWidget * pParent)
{
  ProxySettings proxySettings;

  bool done = true;

  if (PackageManager::TryGetProxy(proxySettings)
      && proxySettings.useProxy
      && proxySettings.authenticationRequired
      && proxySettings.user.empty())
    {
      ::ProxyAuthenticationDialog dlg (pParent);
      if (dlg.exec() == QDialog::Accepted)
	{
	  proxySettings.user = dlg.GetName().toLocal8Bit().constData();
	  proxySettings.password = dlg.GetPassword().toLocal8Bit().constData();
	  PackageManager::SetProxy (proxySettings);
	}
      else
	{
	  done = false;
	}
    }

  return (done);
}
