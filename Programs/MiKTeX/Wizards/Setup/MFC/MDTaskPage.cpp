/* MDTaskPage.cpp:

   Copyright (C) 1999-2006 Christian Schenk

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

#include "SetupWizard.h"
#include "MDTaskPage.h"

/* _________________________________________________________________________

   MDTaskPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(MDTaskPage, CPropertyPage)
  ON_BN_CLICKED(IDC_INSTALL, OnInstall)
  ON_BN_CLICKED(IDC_PREPARE_MIKTEXDIRECT, OnPrepareMiKTeXDirect)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   MDTaskPage::MDTaskPage
   _________________________________________________________________________ */

MDTaskPage::MDTaskPage ()
  : CPropertyPage (IDD,
		   0,
		   IDS_HEADER_TASK,
		   IDS_SUBHEADER_TASK),
    task (-1),
    pSheet (0)
{
}

/* _________________________________________________________________________

   MDTaskPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
MDTaskPage::OnInitDialog ()
{
  root = theApp.MiKTeXDirectRoot.Get();
  pSheet = reinterpret_cast<SetupWizard *>(GetParent());
  switch (theApp.GetSetupTask().Get())
    {
    case SetupTask::PrepareMiKTeXDirect:
      task = 0;
      break;
    case SetupTask::InstallFromCD:
      task = 1;
      break;
    default:
      ASSERT (false);
      __assume (false);
      break;
    }
  return (CPropertyPage::OnInitDialog());
}

/* _________________________________________________________________________

   MDTaskPage::OnSetActive
   _________________________________________________________________________ */

BOOL
MDTaskPage::OnSetActive ()
{
  BOOL ret = CPropertyPage::OnSetActive();
  if (ret)
    {
      pSheet->SetWizardButtons (PSWIZB_BACK
				| (task >= 0 ? PSWIZB_NEXT : 0));
    }
  return (ret);
}

/* _________________________________________________________________________

   MDTaskPage::DoDataExchange
   _________________________________________________________________________ */

void
MDTaskPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Radio (pDX, IDC_PREPARE_MIKTEXDIRECT, task);
  DDX_Text (pDX, IDC_TEXMF_ROOT, root);
}

/* _________________________________________________________________________

   MDTaskPage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
MDTaskPage::OnWizardNext ()
{
  UINT next;
  switch (GetCheckedRadioButton(IDC_PREPARE_MIKTEXDIRECT, IDC_INSTALL))
    {
    case IDC_PREPARE_MIKTEXDIRECT:
      next = IDD_SHARED;
      break;
    case IDC_INSTALL:
      next = IDD_PACKAGE_SET_INSTALL;
      break;
    default:
      ASSERT (false);
      __assume (false);
    }
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(next)));
}

/* _________________________________________________________________________

   MDTaskPage::OnWizardBack
   _________________________________________________________________________ */

LRESULT
MDTaskPage::OnWizardBack ()
{
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_LICENSE)));
}

/* _________________________________________________________________________

   MDTaskPage::OnKillActive
   _________________________________________________________________________ */

BOOL
MDTaskPage::OnKillActive ()
{
  BOOL ret = CPropertyPage::OnKillActive();
  if (ret)
    {
      theApp.GetSetupTask() = (task == 0
		? SetupTask::PrepareMiKTeXDirect
		: SetupTask::InstallFromCD);
    }
  return (ret);
}

/* _________________________________________________________________________

   MDTaskPage::OnPrepareMiKTeXDirect
   _________________________________________________________________________ */

void
MDTaskPage::OnPrepareMiKTeXDirect ()
{
  pSheet->SetWizardButtons (PSWIZB_BACK | PSWIZB_NEXT);
}

/* _________________________________________________________________________

   MDTaskPage::OnInstall
   _________________________________________________________________________ */

void
MDTaskPage::OnInstall ()
{
  pSheet->SetWizardButtons (PSWIZB_BACK | PSWIZB_NEXT);
}
