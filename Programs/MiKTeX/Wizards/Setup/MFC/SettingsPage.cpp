/* SettingsPage.cpp:

   Copyright (C) 1999-2011 Christian Schenk

   This file is part of the MiKTeX Setup Wizard.

   The MiKTeX Setup Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX Setup Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX Setup Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "Setup.h"

#include "SettingsPage.h"
#include "SetupWizard.h"

/* _________________________________________________________________________
   
   SettingsPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(SettingsPage, CPropertyPage)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   SettingsPage::SettingsPage
   _________________________________________________________________________ */

SettingsPage::SettingsPage ()
  : CPropertyPage (IDD,
		   0,
		   IDS_HEADER_SETTINGS,
		   IDS_SUBHEADER_SETTINGS),
    pSheet (0)
{
}

/* _________________________________________________________________________

   SettingsPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
SettingsPage::OnInitDialog ()
{
  pSheet = reinterpret_cast<SetupWizard *>(GetParent());
  paperSize = theApp.paperSize.c_str();
  switch (theApp.installOnTheFly.Get())
    {
    case TriState::True:
      installOnTheFly = 0;
      break;
    case TriState::False:
      installOnTheFly = 1;
      break;
    case TriState::Undetermined:
      installOnTheFly = 2;
      break;
    default:
      ASSERT (false);
      __assume (false);
      break;
    }
  BOOL ret = CPropertyPage::OnInitDialog();
  return (ret);
}

/* _________________________________________________________________________

   SettingsPage::DoDataExchange
   _________________________________________________________________________ */

void
SettingsPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_CBString (pDX, IDC_COMBO1, paperSize);
  DDX_CBIndex (pDX, IDC_INSTALL_ON_THE_FLY, installOnTheFly);
}

/* _________________________________________________________________________

   SettingsPage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
SettingsPage::OnWizardNext ()
{
  UINT next;
  switch (theApp.setupTask.Get())
    {
    case SetupTask::InstallFromCD:
    case SetupTask::InstallFromLocalRepository:
    case SetupTask::InstallFromRemoteRepository:
#if ENABLE_ADDTEXMF
      next = IDD_ADD_TEXMFROOTS;
#else
      next = IDD_INFOLIST;
#endif
      break;
    case SetupTask::PrepareMiKTeXDirect:
      next = IDD_INFOLIST;
      break;
    default:
      ASSERT (false);
      __assume (false);
      break;
    }
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(next)));
}

/* _________________________________________________________________________

   SettingsPage::OnWizardBack
   _________________________________________________________________________ */

LRESULT
SettingsPage::OnWizardBack ()
{
#if SHOW_FOLDER_PAGE
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_FOLDER)));
#else
  UINT uPrev;
  switch (theApp.setupTask.Get())
    {
    case SetupTask::InstallFromCD:
    case SetupTask::InstallFromLocalRepository:
    case SetupTask::InstallFromRemoteRepository:
      uPrev = IDD_INSTALLDIR;
      break;
    case SetupTask::PrepareMiKTeXDirect:
      uPrev = IDD_SHARED;
      break;
    default:
      ASSERT (false);
      __assume (false);
      break;
    }
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(uPrev)));
#endif
}

/* _________________________________________________________________________

   SettingsPage::OnKillActive
   _________________________________________________________________________ */

BOOL
SettingsPage::OnKillActive ()
{
  BOOL ret = CPropertyPage::OnKillActive();
  theApp.paperSize = TU_(paperSize);
  switch (installOnTheFly)
    {
    case 0:
      theApp.installOnTheFly = TriState::True;
      break;
    case 1:
      theApp.installOnTheFly = TriState::False;
      break;
    case 2:
      theApp.installOnTheFly = TriState::Undetermined;
      break;
    default:
      ASSERT (false);
      __assume (false);
      break;
    }
  return (ret);
}
