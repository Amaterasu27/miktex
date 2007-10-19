/* WelcomePage.cpp:

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

#include "UpdateWizard.h"
#include "WelcomePage.h"

IMPLEMENT_DYNCREATE(WelcomePage, CPropertyPage);

/* _________________________________________________________________________

   WelcomePage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(WelcomePage, CPropertyPage)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   WelcomePage::WelcomePage
   _________________________________________________________________________ */

WelcomePage::WelcomePage ()
  : CPropertyPage (IDD)
{
  m_psp.dwFlags |= PSP_HIDEHEADER;
  alwaysShow =
    (SessionWrapper(true)->GetConfigValue(T_("Update"),
					  T_("alwaysWelcome"),
					  false)
     ? TRUE
     : FALSE);
}

/* _________________________________________________________________________

   WelcomePage::OnInitDialog
   _________________________________________________________________________ */

BOOL
WelcomePage::OnInitDialog ()
{
  pSheet = reinterpret_cast<UpdateWizard *>(GetParent());
  BOOL ret = CPropertyPage::OnInitDialog();
  try
    {
      CWnd * pWnd = GetDlgItem(IDC_WELCOME);
      if (pWnd == 0)
	{
	  UNEXPECTED_CONDITION ("WelcomePage::OnInitDialog");
	}
      extern CFont fntWelcome;
      pWnd->SetFont (&fntWelcome);
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

   WelcomePage::OnSetActive
   _________________________________________________________________________ */

BOOL
WelcomePage::OnSetActive ()
{
  pSheet->SetWizardButtons (PSWIZB_NEXT);
  return (CPropertyPage::OnSetActive());
}

/* _________________________________________________________________________

   WelcomePage::DoDataExchange
   _________________________________________________________________________ */

void
WelcomePage::DoDataExchange (/*[in]*/ CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Check (pDX, IDC_ALWAYS_SHOW, alwaysShow);
}

/* _________________________________________________________________________

   WelcomePage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
WelcomePage::OnWizardNext ()
{
  pSheet->SetCameFrom (IDD);
  return (CPropertyPage::OnWizardNext());
}

/* _________________________________________________________________________

   WelcomePage::OnKillActive
   _________________________________________________________________________ */

BOOL
WelcomePage::OnKillActive ()
{
  BOOL ret = CPropertyPage::OnKillActive();
  try
    {
      SessionWrapper(true)->SetUserConfigValue (T_("Update"),
						T_("alwaysWelcome"),
						(alwaysShow == 0
						 ? false
						 : true));
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
  return (ret);
}
