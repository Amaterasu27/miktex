/* PropPageGeneral.cpp:

   Copyright (C) 2000-2006 Christian Schenk

   This file is part of MiKTeX Options.

   MiKTeX Options is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   MiKTeX Options is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX Options; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"

#include "mo.h"

#include "PropPageGeneral.h"
#include "PropSheet.h"
#include "resource.hm"

/* _________________________________________________________________________

   PropPageGeneral Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(PropPageGeneral, CPropertyPage)
  ON_BN_CLICKED(IDC_REFRESH_FNDB, OnRefreshFndb)
  ON_BN_CLICKED(IDC_UPDATE_FMT, OnUpdateFmt)
  ON_CBN_SELCHANGE(IDC_COMBO_PAPER, &PropPageGeneral::OnChangePaperSize)
  ON_CBN_SELCHANGE(IDC_INSTALL_ON_THE_FLY, OnChangeInstallOnTheFly)
  ON_WM_CONTEXTMENU()
  ON_WM_HELPINFO()
END_MESSAGE_MAP();

/* _________________________________________________________________________

   PropPageGeneral::PropPageGeneral
   _________________________________________________________________________ */

PropPageGeneral::PropPageGeneral (/*[in]*/ PackageManager * pManager)
  : CPropertyPage(PropPageGeneral::IDD),
    pManager (pManager),
    pProgressDialog (0),
    installOnTheFly (-1),
    previousInstallOnTheFly (-1),
    paperSizeIndex (-1),
    previousPaperSizeIndex (-1)
{
  m_psp.dwFlags &= ~ (PSP_HASHELP);
}

/* _________________________________________________________________________

   PropPageGeneral::OnInitDialog
   _________________________________________________________________________ */

BOOL
PropPageGeneral::OnInitDialog ()
{
  BOOL ret = TRUE;
  try
    {
      switch (SessionWrapper(true)
	      ->GetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			       MIKTEX_REGVAL_AUTO_INSTALL,
			       TriState(TriState::Undetermined)).Get())
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
	}
      previousInstallOnTheFly = installOnTheFly;
      ret = CPropertyPage::OnInitDialog();
      PaperSizeInfo defaultPaperSizeInfo;
      SessionWrapper(true)->GetPaperSizeInfo (-1, defaultPaperSizeInfo);
      PaperSizeInfo paperSizeInfo;
      for (int idx = 0;
	   SessionWrapper(true)->GetPaperSizeInfo(idx, paperSizeInfo);
	   ++ idx)
	{
	  CString displayName = paperSizeInfo.name.c_str();
	  if (StringCompare(paperSizeInfo.name.c_str(),
			    paperSizeInfo.dvipsName.c_str(),
			    true)
	      != 0)
	    {
	      displayName += T_(" (");
	      displayName += paperSizeInfo.dvipsName.c_str();
	      displayName += T_(")");
	    }
	  if (paperSizeComboBox.AddString(displayName) < 0)
	    {
	      FATAL_WINDOWS_ERROR (T_("CComboBox::AddString"), 0);
	    }
	  if (StringCompare(paperSizeInfo.dvipsName.c_str(),
			    defaultPaperSizeInfo.dvipsName.c_str(),
			    true)
	      == 0)
	    {
	      paperSizeIndex = idx;
	      previousPaperSizeIndex = idx;
	      if (paperSizeComboBox.SetCurSel(idx) < 0)
		{
		  FATAL_WINDOWS_ERROR (T_("CComboBox::SetCurSel"), 0);
		}
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
  return (ret);
}

/* _________________________________________________________________________

   PropPageGeneral::DoDataExchange
   _________________________________________________________________________ */

void
PropPageGeneral::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_CBIndex (pDX, IDC_COMBO_PAPER, paperSizeIndex);
  DDX_CBIndex (pDX, IDC_INSTALL_ON_THE_FLY, installOnTheFly);
  DDX_Control (pDX, IDC_COMBO_PAPER, paperSizeComboBox);
}

/* _________________________________________________________________________

   PropPageGeneral::OnApply
   _________________________________________________________________________ */

BOOL
PropPageGeneral::OnApply ()
{
   try
    {
      if (installOnTheFly != previousInstallOnTheFly)
	{
	  TriState tri (TriState::Undetermined);
	  switch (installOnTheFly)
	    {
	    case 0:
	      tri = TriState::True;
	      break;
	    case 1:
	      tri = TriState::False;
	      break;
	    default:
	      tri = TriState::Undetermined;	  
	      break;
	    }
	  SessionWrapper(true)
	    ->SetUserConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
				  MIKTEX_REGVAL_AUTO_INSTALL,
				  tri.Get());
	  previousInstallOnTheFly = installOnTheFly;
	}
      if (paperSizeIndex != previousPaperSizeIndex)
	{
	  PaperSizeInfo paperSizeInfo;
	  if (! SessionWrapper(true)->GetPaperSizeInfo(paperSizeIndex,
						       paperSizeInfo))
	    {
	      UNEXPECTED_CONDITION (T_("PropPageGeneral::OnApply"));
	    }
	  SessionWrapper(true)
	    ->SetDefaultPaperSize (paperSizeInfo.dvipsName.c_str());
	  previousPaperSizeIndex = paperSizeIndex;
	  PropSheet * pSheet = reinterpret_cast<PropSheet*>(GetParent());
	  pSheet->ScheduleBuildFormats ();
	}
      return (TRUE);
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
      return (FALSE);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
      return (FALSE);
    }
}

/* _________________________________________________________________________

   PropPageGeneral::OnRefreshFndb
   _________________________________________________________________________ */

void
PropPageGeneral::OnRefreshFndb ()
{
  try
    {
      auto_ptr<ProgressDialog> pProgressDialog (ProgressDialog::Create());
      pProgressDialog->StartProgressDialog (GetParent()->GetSafeHwnd());
      pProgressDialog->SetTitle (T_("Refresh File Name Database"));
      pProgressDialog->SetLine (1, T_("Scanning directories..."));
      this->pProgressDialog = pProgressDialog.get();
      Fndb::Refresh (this);
      if (! pProgressDialog->HasUserCancelled())
	{
	  pProgressDialog->SetLine
	    (1,
	     T_("Scanning package definitions"));
	  pProgressDialog->SetLine
	    (2,
	     T_("create the MPM file name database..."));
	  pManager->CreateMpmFndb ();
	}
      pProgressDialog->StopProgressDialog ();
      pProgressDialog.reset ();
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  this->pProgressDialog = 0;
}

/* _________________________________________________________________________

   PropPageGeneral::OnUpdateFmt
   _________________________________________________________________________ */

void
PropPageGeneral::OnUpdateFmt ()
{
  try
    {
      PropSheet * pSheet = reinterpret_cast<PropSheet*>(GetParent());
      pSheet->BuildFormats ();
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

   PropPageGeneral::OnChangeInstallOnTheFly
   _________________________________________________________________________ */

void
PropPageGeneral::OnChangeInstallOnTheFly ()
{
  SetModified (TRUE);
}

/* _________________________________________________________________________

   PropPageGeneral::OnChangePaperSize
   _________________________________________________________________________ */

void
PropPageGeneral::OnChangePaperSize ()
{
  SetModified (TRUE);
}

/* _________________________________________________________________________

   aHelpIDs
   _________________________________________________________________________ */

#define MAKE_ID_HID_PAIR(id) id, H##id

namespace
{
  const DWORD aHelpIDs[] = {
    MAKE_ID_HID_PAIR(IDC_INSTALL_ON_THE_FLY),
    MAKE_ID_HID_PAIR(IDC_REFRESH_FNDB),
    MAKE_ID_HID_PAIR(IDC_UPDATE_FMT),
    0, 0,
  };
}

/* _________________________________________________________________________

   PropPageGeneral::OnHelpInfo
   _________________________________________________________________________ */

BOOL
PropPageGeneral::OnHelpInfo (/*[in]*/ HELPINFO * pHelpInfo) 
{
  try
    {
      return (::OnHelpInfo(pHelpInfo, aHelpIDs, T_("GeneralPage.txt")));
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
      return (FALSE);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
      return (FALSE);
    }
}

/* _________________________________________________________________________

   PropPageGeneral::OnContextMenu
   _________________________________________________________________________ */

void
PropPageGeneral::OnContextMenu (/*[in]*/ CWnd *	pWnd,
				/*[in]*/ CPoint	point) 
{
  try
    {
      DoWhatsThisMenu (pWnd, point, aHelpIDs, T_("GeneralPage.txt"));
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

   PropPageGeneral::ReadDirectory
   _________________________________________________________________________ */

bool
MIKTEXCALL
PropPageGeneral::ReadDirectory (/*[in]*/ const MIKTEXCHAR * lpszPath,
				/*[out]*/ MIKTEXCHAR * * ppSubDirNames,
				/*[out]*/ MIKTEXCHAR * * ppFileNames,
				/*[out]*/ MIKTEXCHAR * * ppFileNameInfos)
  
{
  UNUSED_ALWAYS (lpszPath);
  UNUSED_ALWAYS (ppSubDirNames);
  UNUSED_ALWAYS (ppFileNames);
  UNUSED_ALWAYS (ppFileNameInfos);
  return (false);
}

/* _________________________________________________________________________

   PropPageGeneral::OnProgress
   _________________________________________________________________________ */

bool
MIKTEXCALL
PropPageGeneral::OnProgress (/*[in]*/ unsigned			level,
			     /*[in]*/ const MIKTEXCHAR *	lpszDirectory)
{
  UNUSED_ALWAYS (level);
  pProgressDialog->SetLine (2, lpszDirectory);
  return (! pProgressDialog->HasUserCancelled());
}
