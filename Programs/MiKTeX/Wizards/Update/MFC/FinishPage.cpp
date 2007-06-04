/* FinishPage.cpp:

   Copyright (C) 2002-2007 Christian Schenk

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

#include "FinishPage.h"
#include "UpdateWizard.h"

IMPLEMENT_DYNCREATE(FinishPage, CPropertyPage);

/* _________________________________________________________________________

   FinishPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(FinishPage, CPropertyPage)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   FinishPage::FinishPage
   _________________________________________________________________________ */

FinishPage::FinishPage()
  : CPropertyPage (IDD),
    viewReport (BST_UNCHECKED),
    pSheet (0)
{
  m_psp.dwFlags |= PSP_HIDEHEADER;
}

/* _________________________________________________________________________

   FinishPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
FinishPage::OnInitDialog ()
{
  pSheet = reinterpret_cast<UpdateWizard *>(GetParent());
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
  try
    {
      pSheet->SetFinishText (T_("Finish"));
      if (pSheet->GetErrorFlag())
	{
	  CString str;
	  if (! str.LoadString(IDS_FINISH_ERROR))
	    {
	      UNEXPECTED_CONDITION (T_("FinishPage::OnSetActive"));
	    }
	  CWnd * pWnd;
	  pWnd = GetDlgItem(IDC_STATUS);
	  if (pWnd == 0)
	    {
	      UNEXPECTED_CONDITION (T_("FinishPage::OnSetActive"));
	    }
	  pWnd->SetWindowText (str);
	  if (! str.LoadString(IDS_REMEDY))
	    {
	      UNEXPECTED_CONDITION (T_("FinishPage::OnSetActive"));
	    }
	  pWnd = GetDlgItem(IDC_MESSAGE);
	  if (pWnd == 0)
	    {
	      UNEXPECTED_CONDITION (T_("FinishPage::OnSetActive"));
	    }
	  pWnd->SetWindowText (str);
	  viewReport = BST_CHECKED;
	  UpdateData (FALSE);
	}
      CancelToClose ();
      return (CPropertyPage::OnSetActive());
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
      return (FALSE);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
      return (FALSE);
    }
}

/* _________________________________________________________________________

   FinishPage::DoDataExchange
   _________________________________________________________________________ */

void
FinishPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Check (pDX, IDC_VIEW_REPORT, viewReport);
}

/* _________________________________________________________________________

   FinishPage::OnWizardFinish
   _________________________________________________________________________ */

BOOL
FinishPage::OnWizardFinish ()
{
  BOOL ret = CPropertyPage::OnWizardFinish();
  if (ret)
    {
      try
	{
	  if (viewReport == BST_CHECKED)
	    {
	      if (ShellExecute(0,
			       T_("open"),
			       g_logFileName.Get(),
			       0,
			       0,
			       SW_SHOWNORMAL)
		  <= reinterpret_cast<HINSTANCE>(32))
		{
		  Process::Start (T_("notepad.exe"), g_logFileName.Get());
		}
	    }
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
    }
  return (ret);
}
