/* FinishPage.cpp:

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

#include "FinishPage.h"
#include "SetupWizard.h"

/* _________________________________________________________________________

   FinishPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(FinishPage, CPropertyPage)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   FinishPage::FinishPage
   _________________________________________________________________________ */

FinishPage::FinishPage ()
  : CPropertyPage (IDD),
    viewReleaseNotes (BST_UNCHECKED),
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
  pSheet = reinterpret_cast<SetupWizard *>(GetParent());
  BOOL ret = CPropertyPage::OnInitDialog();
  try
    {
      extern CFont fntWelcome;
      CWnd * pWnd = GetDlgItem(IDC_WELCOME);
      if (pWnd == 0)
	{
	  UNEXPECTED_CONDITION ("FinishPage::OnInitDialog");
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
      pSheet->SetFinishText (T_("Close"));

      if (pSheet->GetErrorFlag())
	{
	  CString str;

	  if (! str.LoadString(IDS_FINISH_ERROR))
	    {
	      UNEXPECTED_CONDITION ("FinishPage::OnSetActive");
	    }

	  status.SetWindowText (str);

	  if (! str.LoadString(IDS_REMEDY))
	    {
	      UNEXPECTED_CONDITION ("FinishPage::OnSetActive");
	    }

	  message.SetWindowText (str);

	  viewReleaseNotes = BST_CHECKED;

	  UpdateData (FALSE);

	  if (! str.LoadString(IDS_VIEW_LOG_FILE))
	    {
	      UNEXPECTED_CONDITION ("FinishPage::OnSetActive");
	    }

	  checkBox.SetWindowText (str);
	}
      else if (theApp.setupTask == SetupTask::Download)
	{
	  CString str;

	  if (! str.LoadString(IDS_DOWNLOAD_COMPLETE))
	    {
	      UNEXPECTED_CONDITION ("FinishPage::OnSetActive");
	    }

	  message.SetWindowText (str);

	  viewReleaseNotes = BST_UNCHECKED;

	  UpdateData (FALSE);

	  checkBox.ShowWindow (SW_HIDE);
	}
      else
	{
	  viewReleaseNotes = BST_UNCHECKED;
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
  DDX_Check (pDX, IDC_VIEW_RELNOTES, viewReleaseNotes);
  DDX_Control (pDX, IDC_STATUS, status);
  DDX_Control (pDX, IDC_MESSAGE, message);
  DDX_Control (pDX, IDC_VIEW_RELNOTES, checkBox);
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
	  CWnd * pWnd = GetDlgItem(IDC_VIEW_RELNOTES);
	  if (pWnd == 0)
	    {
	      UNEXPECTED_CONDITION ("FinishPage::OnWizardFinish");
	    }
	  if (viewReleaseNotes == BST_CHECKED)
	    {
	      if (pSheet->GetErrorFlag())
		{
		  theApp.showLogFileOnExit = true;
		}
	      else
		{
		  PathName pathRelNotes (theApp.startupConfig.installRoot,
					 MIKTEX_PATH_RELNOTES_HTML);
		  if (ShellExecute(0,
				   T_("open"),
				   pathRelNotes.Get(),
				   0,
				   0,
				   SW_SHOWNORMAL)
		      <= reinterpret_cast<HINSTANCE>(32))
		    {
		      FATAL_MIKTEX_ERROR ("FinishPage::OnWizardFinish",
				  T_("The file could not be opened."),
					  pathRelNotes.Get());
		    }
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
