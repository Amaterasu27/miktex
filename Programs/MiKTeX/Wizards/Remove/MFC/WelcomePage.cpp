/* WelcomePage.cpp:

   Copyright (C) 2000-2007 Christian Schenk

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

#include "RemoveWizard.h"
#include "WelcomePage.h"

/* _________________________________________________________________________

   WelcomePage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(WelcomePage, CPropertyPage)
  ON_BN_CLICKED(IDC_THOROUGHLY, &WelcomePage::OnThoroughly)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   WelcomePage::WelcomePage
   _________________________________________________________________________ */

WelcomePage::WelcomePage()
  : CPropertyPage(WelcomePage::IDD),
    pSheet (0),
    thoroughly (TRUE)
{
  m_psp.dwFlags |= PSP_HIDEHEADER;
  m_psp.dwFlags &= ~ PSP_HASHELP;
}

/* _________________________________________________________________________

   WelcomePage::DoDataExchange
   _________________________________________________________________________ */

void
WelcomePage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_LIST, listBox);
  DDX_Check (pDX, IDC_THOROUGHLY, thoroughly);
  DDX_Control (pDX, IDC_THOROUGHLY, thoroughlyButton);
}

/* _________________________________________________________________________

   WelcomePage::OnInitDialog
   _________________________________________________________________________ */

BOOL
WelcomePage::OnInitDialog ()
{
  pSheet = reinterpret_cast<RemoveWizard *>(GetParent());
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
      ShowItems ();
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
  BOOL ret = CPropertyPage::OnSetActive();
  if (ret)
    {
      try
	{
	  pSheet->SetWizardButtons (PSWIZB_NEXT);
	  oldNextText = pSheet->SetNextText (T_("&Remove"));
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

/* _________________________________________________________________________

   WelcomePage::OnKillActive
   _________________________________________________________________________ */

BOOL
WelcomePage::OnKillActive ()
{
  BOOL ret = CPropertyPage::OnKillActive();
  if (ret)
    { 
      try
	{
	  pSheet->SetThoroughlyFlag (thoroughly != FALSE);
	  pSheet->SetNextText (oldNextText);
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

/* _________________________________________________________________________

   WelcomePage::OnThoroughly
   _________________________________________________________________________ */

void
WelcomePage::OnThoroughly ()
{
  thoroughly = thoroughlyButton.GetCheck ();
  ShowItems ();
}
/* _________________________________________________________________________

   WelcomePage::ShowItems
   _________________________________________________________________________ */

void
WelcomePage::ShowItems ()
{
  listBox.ResetContent ();
  if (listBox.AddString(T_("Installed packages")) < 0)
    {
      FATAL_WINDOWS_ERROR ("CListBox::AddString", 0);
    }
  if (listBox.AddString(T_("Shortcuts")) < 0)
    {
      FATAL_WINDOWS_ERROR ("CListBox::AddString", 0);
    }
  if (listBox.AddString(T_("MiKTeX registry settings")) < 0)
    {
      FATAL_WINDOWS_ERROR ("CListBox::AddString", 0);
    }
  if (thoroughly != FALSE)
    {
      vector<PathName> vec = GetRoots();
      for (vector<PathName>::const_iterator it = vec.begin();
	   it != vec.end();
	   ++ it)
	{
	  if (listBox.AddString(it->Get()) < 0)
	    {
	      FATAL_WINDOWS_ERROR ("CListBox::AddString", 0);
	    }
	}
    }
}
