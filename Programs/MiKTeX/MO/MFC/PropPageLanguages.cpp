/* PropPageLanguages.cpp:

   Copyright (C) 2000-2010 Christian Schenk

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

#include "PropPageLanguages.h"
#include "PropSheet.h"
#include "resource.hm"

/* _________________________________________________________________________

   PropPageLanguages::PropPageLanguages
   _________________________________________________________________________ */

PropPageLanguages::PropPageLanguages (/*[in]*/ PackageManager * pManager)
  : CPropertyPage (PropPageLanguages::IDD),
    inserting (false),
    pManager (pManager)
{
  m_psp.dwFlags &= ~(PSP_HASHELP);
}

/* _________________________________________________________________________

   PropPageLanguages::~PropPageLanguages
   _________________________________________________________________________ */

PropPageLanguages::~PropPageLanguages ()
{
}

/* _________________________________________________________________________

   PropPageLanguages::DoDataExchange
   _________________________________________________________________________ */

void
PropPageLanguages::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  DDX_Control (pDX, IDC_LIST, listControl);
}

/* _________________________________________________________________________

   PropPageLanguages Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP (PropPageLanguages, CPropertyPage)
  ON_NOTIFY (LVN_ITEMCHANGED, IDC_LIST, OnItemChanged)
  ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST, OnItemChanging)
  ON_NOTIFY (NM_DBLCLK, IDC_LIST, OnDblclkList)
  ON_WM_CONTEXTMENU ()
  ON_WM_HELPINFO ()
END_MESSAGE_MAP ()

/* _________________________________________________________________________

   PropPageLanguages::InsertColumn
   _________________________________________________________________________ */

void
PropPageLanguages::InsertColumn (/*[in]*/ int		colIdx,
				 /*[in]*/ const char *	lpszLabel,
				 /*[in]*/ const char *	lpszLongest)
{
  if (listControl.InsertColumn(colIdx,
			       lpszLabel,
			       LVCFMT_LEFT,
			       listControl.GetStringWidth(lpszLongest),
			       colIdx)
      < 0)
    {
      FATAL_WINDOWS_ERROR ("CListCtrl::InsertColumn", 0);
    }
}

/* _________________________________________________________________________

   PropPageLanguages::OnInitDialog
   _________________________________________________________________________ */

BOOL
PropPageLanguages::OnInitDialog () 
{
  BOOL ret = CPropertyPage::OnInitDialog();

  try
    {
      
      listControl.SetExtendedStyle (listControl.GetExtendedStyle()
				    | LVS_EX_CHECKBOXES);

      int colIdx = 0;

      InsertColumn (colIdx++, T_("Language"), T_("xxxx portuguese"));

      InsertColumn (colIdx++, T_("Synonyms"), T_("xxxx patois, francais"));

      InsertColumn (colIdx++,
		    T_("Package"),
		    T_("xxxx nehyph96.tex"));

      ReadLanguageDat ();
      Refresh ();
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

   PropPageLanguages::WhichPackage
   _________________________________________________________________________ */

vector<string>
PropPageLanguages::WhichPackage (/*[in]*/ const char * lpszTeXInputFile)
{
  PathName directoryPattern (SessionWrapper(true)->GetMpmRootPath());
  directoryPattern += "tex//";
  PathNameArray paths;
  vector<string> result;
  if (! Fndb::Search(lpszTeXInputFile, directoryPattern.Get(), false, paths, result))
  {
    result.clear ();
  }
  return (result);
}

/* _________________________________________________________________________

   PropPageLanguages::OnApply
   _________________________________________________________________________ */

BOOL
PropPageLanguages::OnApply ()
{
  try
  {
    vector<string> toBeRemoved;
    vector<string> toBeInstalled;
    for (vector<MyLanguageInfo>::iterator it = languages.begin(); it != languages.end(); ++ it)
    {
      if (it->active == it->newActive)
      {
	continue;
      }
      if (it->packageNames.size() > 0)
      {
	PackageInfo packageInfo = pManager->GetPackageInfo(it->packageNames[0].c_str());
	if (it->newActive)
	{
	  if (! packageInfo.IsInstalled()
	      && find(toBeInstalled.begin(), toBeInstalled.end(), it->packageNames[0]) == toBeInstalled.end())
	  {
	    toBeInstalled.push_back (it->packageNames[0]);
	  }
	}
      }
      for (vector<string>::const_iterator it2 = it->packageNames.begin(); it2 != it->packageNames.end(); ++ it2)
      {
	PackageInfo packageInfo = pManager->GetPackageInfo(it2->c_str());
	if (! it->newActive)
	{
	  if (packageInfo.IsInstalled()
	      && find(toBeInstalled.begin(), toBeInstalled.end(), *it2) == toBeInstalled.end()
	      && find(toBeRemoved.begin(), toBeRemoved.end(), *it2) == toBeRemoved.end())
	  {
	    toBeRemoved.push_back (*it2);
	  }
	}
      }
    }
    if (toBeInstalled.size() == 0 && toBeRemoved.size() == 0)
    {
      ReadLanguageDat ();
      Refresh ();
      return (CPropertyPage::OnApply());
    }
    CString str1;
    str1.Format (_T("%u"), toBeInstalled.size());
    CString str2;
    str2.Format (_T("%u"), toBeRemoved.size());
    CString str;
    AfxFormatString2 (str, IDP_UPDATE_MESSAGE, str1, str2);
    if (IsWindowsVista() && SessionWrapper(true)->IsAdminMode())
    {
      DllProc4<HRESULT, const TASKDIALOGCONFIG *, int *, int *, BOOL *>
	taskDialogIndirect (T_("comctl32.dll"), T_("TaskDialogIndirect"));
      TASKDIALOGCONFIG taskDialogConfig;
      memset (&taskDialogConfig, 0, sizeof(taskDialogConfig));
      taskDialogConfig.cbSize = sizeof(TASKDIALOGCONFIG);
      taskDialogConfig.hwndParent = 0;
      taskDialogConfig.hInstance = 0;
      taskDialogConfig.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION;
      taskDialogConfig.pszMainIcon = MAKEINTRESOURCEW(TD_SHIELD_ICON);
      taskDialogConfig.pszWindowTitle =
	MAKEINTRESOURCEW(AFX_IDS_APP_TITLE);
      taskDialogConfig.pszMainInstruction = L"Do you want to proceed?";
      CStringW strContent (str);
      taskDialogConfig.pszContent = strContent;
      taskDialogConfig.cButtons = 2;
      TASKDIALOG_BUTTON const buttons[] = {
	{IDOK, L"Proceed"},
	{IDCANCEL, L"Cancel"}
      };
      taskDialogConfig.pButtons = buttons;
      taskDialogConfig.nDefaultButton = IDOK;
      int result = 0;
      if(SUCCEEDED(taskDialogIndirect(&taskDialogConfig, &result, 0, 0)))
      {
	if (IDOK != result)
	{
	  return (FALSE);
	}
      }
      else
      {
	UNEXPECTED_CONDITION ("PropPagePackages::OnApply");
      }
    }
    else
    {
      if (AfxMessageBox(str, MB_OKCANCEL | MB_ICONINFORMATION) == IDCANCEL)
      {
	return (FALSE);
      }
    }
    if (UpdateDialog::DoModal(this,
			      pManager.Get(),
			      toBeInstalled,
			      toBeRemoved)
        == IDOK)
    {
      ReadLanguageDat ();
      Refresh ();
      SetElevationRequired (false);
      
      return (TRUE);
    }
    else
    {
      ReadLanguageDat ();
      Refresh ();
      SetElevationRequired (true);
      return (FALSE);
    }
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

   aHelpIDs
   _________________________________________________________________________ */

#define MAKE_ID_HID_PAIR(id) id, H##id

namespace
{
  const DWORD aHelpIDs[] = {
    MAKE_ID_HID_PAIR(IDC_LIST),
    0, 0,
  };
}

/* _________________________________________________________________________

   PropPageLanguages::OnHelpInfo
   _________________________________________________________________________ */

BOOL
PropPageLanguages::OnHelpInfo (/*[in]*/ HELPINFO * pHelpInfo) 
{
  return (::OnHelpInfo(pHelpInfo, aHelpIDs, T_("LanguagesPage.txt")));
}

/* _________________________________________________________________________

   PropPageLanguages::OnContextMenu
   _________________________________________________________________________ */

void
PropPageLanguages::OnContextMenu (/*[in]*/ CWnd *	pWnd,
				  /*[in]*/ CPoint	point) 
{
  try
    {
      DoWhatsThisMenu (pWnd, point, aHelpIDs, T_("LanguagesPage.txt"));
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

   PropPageLanguages::ReadLanguageDat
   _________________________________________________________________________ */

void
PropPageLanguages::ReadLanguageDat ()
{
  languages.clear ();
  LanguageInfo languageInfo;
  for (int idx = 0; SessionWrapper(true)->GetLanguageInfo(idx, languageInfo); ++ idx)
  {
    MyLanguageInfo myLanguageInfo (languageInfo);
    myLanguageInfo.active =
      SessionWrapper(true)->FindFile(myLanguageInfo.loader, "%r/tex//", myLanguageInfo.loaderPath);
    myLanguageInfo.newActive = myLanguageInfo.active;
    myLanguageInfo.packageNames = WhichPackage(myLanguageInfo.loader.c_str());
    languages.push_back (myLanguageInfo);
  }
}

/* _________________________________________________________________________

   PropPageLanguages::InsertLanguage
   _________________________________________________________________________ */

void
PropPageLanguages::InsertLanguage (/*[in]*/ int idx)
{
  LV_ITEM lvitem;

  lvitem.iItem = idx;
  lvitem.mask = LVIF_PARAM;
  lvitem.lParam = idx;
  lvitem.iSubItem = 0;

  inserting = true;
  if (listControl.InsertItem(&lvitem) < 0)
    {
      FATAL_WINDOWS_ERROR ("CListCtrl::InsertItem", 0);
    }
  RefreshRow (idx);
  inserting = false;
}

/* _________________________________________________________________________

   PropPageLanguages::Refresh
   _________________________________________________________________________ */

void
PropPageLanguages::Refresh ()
{
  if (listControl.DeleteAllItems() < 0)
    {
       FATAL_WINDOWS_ERROR ("CListCtrl::DeleteAllItems", 0);
    }
    
  int idx = 0;

  for (vector<MyLanguageInfo>::const_iterator it = languages.begin();
       it != languages.end();
       ++it, ++idx)
    {
      InsertLanguage (idx);
    }

  EnableButtons ();
}

/* _________________________________________________________________________

   PropPageLanguages::OnDblclkList
   _________________________________________________________________________ */

void
PropPageLanguages::OnDblclkList (/*[in]*/ NMHDR *	pNMHDR,
				 /*[in]*/ LRESULT *	pResult)
{
  UNUSED_ALWAYS (pNMHDR);
  *pResult = 0;
}

/* _________________________________________________________________________

   PropPageLanguages::GetSelectedItem
   _________________________________________________________________________ */

int
PropPageLanguages::GetSelectedItem ()
{
  POSITION pos = listControl.GetFirstSelectedItemPosition();
  if (pos == 0)
    {
      UNEXPECTED_CONDITION ("PropPageLanguages::GetSelectedItem");
    }
  return (listControl.GetNextSelectedItem(pos));
}

/* _________________________________________________________________________

   PropPageLanguages::OnItemChanging
   _________________________________________________________________________ */

inline bool IsChecked (/*[in]*/ UINT state)
{
  return (((state & LVIS_STATEIMAGEMASK) >> 12) == 2 ? true : false);
}

void
PropPageLanguages::OnItemChanging (/*[in]*/ NMHDR *	pNMHDR,
				   /*[in]*/ LRESULT *	pResult)
{
  *pResult = 0;
  if (inserting)
  {
    return;
  }
  try
  {
    LPNMLISTVIEW pnmv = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    if (pnmv->iItem >= 0 && (pnmv->uChanged & LVIF_STATE))
    {
      if (! IsChecked(pnmv->uOldState) && IsChecked(pnmv->uNewState))
      {
	if (languages[pnmv->iItem].packageNames.size() == 0)
	{
	  AfxMessageBox (T_("This language package is not yet available."),
			 MB_OK | MB_ICONEXCLAMATION);
	  *pResult = TRUE;
	}
      }
      else if (IsChecked(pnmv->uOldState) && ! IsChecked(pnmv->uNewState))
      {
	MIKTEX_ASSERT (languages[pnmv->iItem].packageNames.size() > 0);
	// is it possible to remove the language package?
	for (vector<string>::const_iterator it = languages[pnmv->iItem].packageNames.begin(); it != languages[pnmv->iItem].packageNames.end(); ++ it)
	{
	  PackageInfo packageInfo = pManager->GetPackageInfo(*it);
	  if (! packageInfo.isRemovable)
	  {
	    AfxMessageBox (T_("This language package is installed for all users. Only the admin variant can remove this language package."),
			   MB_OK | MB_ICONEXCLAMATION);
	    *pResult = TRUE;
	  }
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
}

/* _________________________________________________________________________

   PropPageLanguages::OnItemChanged
   _________________________________________________________________________ */

void
PropPageLanguages::OnItemChanged (/*[in]*/ NMHDR *	pNMHDR,
				  /*[in]*/ LRESULT *	pResult)
{
  *pResult = 0;
  if (inserting)
  {
    return;
  }
  try
  {
    LPNMLISTVIEW pnmv = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    if (pnmv->iItem >= 0 && (pnmv->uChanged & LVIF_STATE))
    {
      bool b = (listControl.GetCheck(pnmv->iItem) ? true : false);
      languages[pnmv->iItem].newActive = b;
      if (languages[pnmv->iItem].active != b)
      {	      
	SetChanged (true);
      }
    }
    EnableButtons ();
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

   PropPageLanguages::RefreshRow
   _________________________________________________________________________ */

void
PropPageLanguages::RefreshRow (/*[in]*/ int idx)
{
  const MyLanguageInfo & lang = languages[idx];

  int colIdx = 0;

  if (! listControl.SetItemText(idx, colIdx++, lang.key.c_str()))
  {
    FATAL_WINDOWS_ERROR ("CListCtrl::SetItemText", 0);
  }

  if (! listControl.SetItemText(idx, colIdx++, lang.synonyms.c_str()))
  {
    FATAL_WINDOWS_ERROR ("CListCtrl::SetItemText", 0);
  }

  CString packageNames;

  if (lang.packageNames.size() == 0)
  {
    packageNames = T_("not available");
  }
  else
  {
    for (int idx = 0; idx < lang.packageNames.size(); ++ idx)
    {
      if (idx > 0)
      {
	packageNames += ", ";
      }
      packageNames += lang.packageNames[idx].c_str();
    }
  }

  if (! listControl.SetItemText(idx, colIdx++, packageNames))
  {
    FATAL_WINDOWS_ERROR ("CListCtrl::SetItemText", 0);
  }

    listControl.SetCheck (idx, lang.active ? TRUE : FALSE);
}

/* _________________________________________________________________________

   PropPageLanguages::EnableButtons
   _________________________________________________________________________ */

void
PropPageLanguages::EnableButtons ()
{
  int itemCount = listControl.GetItemCount();
  int selectedCount = listControl.GetSelectedCount();
  int firstSelected = 0;
  if (selectedCount > 0)
    {
      POSITION pos = listControl.GetFirstSelectedItemPosition();
      if (pos == 0)
	{
	  UNEXPECTED_CONDITION ("PropPageLanguages::EnableButtons");
	}
      firstSelected = listControl.GetNextSelectedItem(pos);
    }
}

/* _________________________________________________________________________

   PropPageLanguages::SetElevationRequired
   _________________________________________________________________________ */

void
PropPageLanguages::SetElevationRequired (/*[in]*/ bool f)
{
  if (IsWindowsVista() && SessionWrapper(true)->IsAdminMode())
  {
    HWND hwnd = ::GetDlgItem(::GetParent(m_hWnd), IDOK);
    if (hwnd == 0)
    {
      UNEXPECTED_CONDITION ("PropPageLanguages::SetElevationRequired");
    }
    Button_SetElevationRequiredState (hwnd, f ? TRUE : FALSE);
    hwnd = ::GetDlgItem(::GetParent(m_hWnd), ID_APPLY_NOW);
    if (hwnd == 0)
    {
      UNEXPECTED_CONDITION ("PropPageLanguages::SetElevationRequired");
    }
    Button_SetElevationRequiredState (hwnd, f ? TRUE : FALSE);
  }
}

/* _________________________________________________________________________

   PropPageLanguages::SetChanged
   _________________________________________________________________________ */

void
PropPageLanguages::SetChanged (/*[in]*/ bool f)
{
  SetElevationRequired (f);
  PropSheet * pSheet =
    reinterpret_cast<PropSheet*>(GetParent());
  pSheet->ScheduleBuildFormats ();
  SetModified (f ? TRUE : FALSE);
}
