/* FinishPage.cpp:

   Copyright (C) 2000-2006 Christian Schenk

   This file is part of the Remove MiKTeX! Wizard.

   The Remove MiKTeX! Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The Remove MiKTeX! Wizard is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the Remove MiKTeX! Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"
#include "Remove.h"

#include "FinishPage.h"
#include "RemoveWizard.h"

/* _________________________________________________________________________

   FinishPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(FinishPage, CPropertyPage)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   FinishPage::FinishPage
   _________________________________________________________________________ */

FinishPage::FinishPage()
  : CPropertyPage (IDD)
{
  m_psp.dwFlags |= PSP_HIDEHEADER;
  m_psp.dwFlags &= ~ PSP_HASHELP;
}

/* _________________________________________________________________________

   FinishPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
FinishPage::OnInitDialog ()
{
  pSheet = reinterpret_cast<RemoveWizard *>(GetParent());
  BOOL ret = CPropertyPage::OnInitDialog();
  try
    {
      extern CFont fntWelcome;
      CWnd * pWnd = GetDlgItem(IDC_WELCOME);
      if (pWnd == 0)
	{
	  UNEXPECTED_CONDITION (T_("FinishPage::OnInitDialog"));
	}
      pWnd->SetFont (&fntWelcome);
    }
  catch (const MiKTeXException & e)
    {
      ReportError (e);
    }
  catch (const exception & e)
    {
      ReportError (e);
    }
  return (ret);
}

/* _________________________________________________________________________

   FinishPage::OnSetActive
   _________________________________________________________________________ */

BOOL
FinishPage::OnSetActive ()
{
  pSheet->SetFinishText (T_("Finish"));
  CancelToClose ();
  return (CPropertyPage::OnSetActive());
}

/* _________________________________________________________________________

   FinishPage::DoDataExchange
   _________________________________________________________________________ */

void
FinishPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
}

