/* SearchPackageDialog.cpp:

   Copyright (C) 2000-2011 Christian Schenk

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
#include "resource.h"

#include "SearchPackageDialog.h"

#include <miktex/UI/MFC/PropSheetPackage>

/* _________________________________________________________________________

   SearchPackageDialog::SearchPackageDialog
   _________________________________________________________________________ */

SearchPackageDialog::SearchPackageDialog (/*[in]*/ CWnd *	pParent,
					  /*[in]*/ PackageManager * pManager)
  : CDialog (IDD_SEARCH_PACKAGE, pParent),
    pManager (pManager)
{
}

/* _________________________________________________________________________

   SearchPackageDialog::DoDataExchange
   _________________________________________________________________________ */

void
SearchPackageDialog::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_LIST_PACKAGES, listControl);
}

/* _________________________________________________________________________

   SearchPackageDialog Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(SearchPackageDialog, CDialog)
  ON_BN_CLICKED(IDC_FIND_NOW, OnBnClickedFindNow)
  ON_EN_CHANGE(IDC_DESCRIPTION, OnEnChangeDescription)
  ON_EN_CHANGE(IDC_FILE, OnEnChangeFile)
  ON_EN_CHANGE(IDC_NAME, OnChangeName)
  ON_EN_CHANGE(IDC_TITLE, OnEnChangeTitle)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PACKAGES, OnLvnItemchangedListPackages)
  ON_NOTIFY(NM_DBLCLK, IDC_LIST_PACKAGES, OnNMDblclkListPackages)
  ON_NOTIFY(NM_RCLICK, IDC_LIST_PACKAGES, OnNMRclickListPackages)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   SearchPackageDialog::OnInitDialog
   _________________________________________________________________________ */

BOOL
SearchPackageDialog::OnInitDialog ()
{
  BOOL ret = CDialog::OnInitDialog();

  try
    {
      if (listControl.InsertColumn(0,
				   T_(_T("Name")),
				   LVCFMT_LEFT,
				   listControl.GetStringWidth(_T("\
xxx mmmmmmmm")),
				   0)
	  < 0)
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::InsertColumn", 0);
	}

      if (listControl.InsertColumn(1,
				   T_(_T("Title")),
				   LVCFMT_LEFT,
				   listControl.GetStringWidth(_T("\
xxx mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm")),
				   1)
	  < 0)
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::InsertColumn", 0);
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

  return (ret);
}

/* _________________________________________________________________________

   SearchPackageDialog::OnChangeName
   _________________________________________________________________________ */

void
SearchPackageDialog::OnChangeName ()
{
  try
    {
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

   SearchPackageDialog::OnEnChangeTitle
   _________________________________________________________________________ */

void
SearchPackageDialog::OnEnChangeTitle ()
{
  try
    {
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

   SearchPackageDialog::OnEnChangeDescription
   _________________________________________________________________________ */

void
SearchPackageDialog::OnEnChangeDescription ()
{
  try
    {
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

   SearchPackageDialog::OnEnChangeFile
   _________________________________________________________________________ */

void
SearchPackageDialog::OnEnChangeFile ()
{
  try
    {
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

   SearchPackageDialog::GetWindowText
   _________________________________________________________________________ */

CString
SearchPackageDialog::GetWindowText (/*[in]*/ UINT controlId)
{
  CWnd * pWnd = GetDlgItem(controlId);
  if (pWnd == 0)
    {
      FATAL_WINDOWS_ERROR ("CWnd::GetDlgItem", 0);
    }
  CString str;
  pWnd->GetWindowText (str);
  return (str);
}

/* _________________________________________________________________________

   SearchPackageDialog::EnableButtons
   _________________________________________________________________________ */

void
SearchPackageDialog::EnableButtons ()
{
  size_t l = 0;
  l += GetWindowText(IDC_NAME).GetLength();
  l += GetWindowText(IDC_DESCRIPTION).GetLength();
  l += GetWindowText(IDC_TITLE).GetLength();
  l += GetWindowText(IDC_FILE).GetLength();
  l += GetWindowText(IDC_FIND_NOW).GetLength();
  CWnd * pOk = GetDlgItem(IDOK);
  if (pOk == 0)
    {
      FATAL_WINDOWS_ERROR ("CWnd::GetDlgItem", 0);
    }
  CWnd * pFindNow = GetDlgItem(IDC_FIND_NOW);
  if (pFindNow == 0)
    {
      FATAL_WINDOWS_ERROR ("CWnd::GetDlgItem", 0);
    }
  pOk->EnableWindow (l > 0);
  CButton * pbn;
  if (l > 0)
    {
      pbn = reinterpret_cast<CButton*>(pOk);
      SetDefID (IDOK);
    }
  else
    {
      pbn = reinterpret_cast<CButton*>(pFindNow);
      SetDefID (IDC_FIND_NOW);
    }
  pbn->SetButtonStyle (BS_DEFPUSHBUTTON);
  if (l > 0)
    {
      POSITION pos = listControl.GetFirstSelectedItemPosition();
      if (pos != 0)
	{
	  int nItem = listControl.GetNextSelectedItem(pos);
	  name = listControl.GetItemText(nItem, 0);
	}
    }
}

/* _________________________________________________________________________

   SearchPackageDialog::OnBnClickedFindNow
   _________________________________________________________________________ */

void
SearchPackageDialog::OnBnClickedFindNow ()
{
  try
    {
      CWaitCursor cur;
      if (! listControl.DeleteAllItems())
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::DeleteAllItems", 0);
	}
      packages.clear ();
      auto_ptr<PackageIterator> pIter (pManager->CreateIterator());
      PackageInfo packageInfo;
      int idx = 0;
      while (pIter->GetNext(packageInfo))
	{
	  bool haveMatch = false;
	  CString givenName = GetWindowText(IDC_NAME);
	  givenName.MakeLower ();
	  CString packageName (packageInfo.displayName.c_str());
	  packageName.MakeLower ();
	  haveMatch =
	    (givenName.GetLength() == 0
	     || packageName.Find(givenName) >= 0);
	  if (! haveMatch)
	    {
	      continue;
	    }
	  CString givenTitle = GetWindowText(IDC_TITLE);
	  haveMatch =
	    (givenTitle.GetLength() == 0
	     || packageInfo.title.find(CT2A(givenTitle)) != string::npos);
	  if (! haveMatch)
	    {
	      continue;
	    }
	  CString givenDescription = GetWindowText(IDC_DESCRIPTION);
	  haveMatch =
	    (givenDescription.GetLength() == 0
	     || (packageInfo.description.find(CT2A(givenDescription))
		 != string::npos));
	  if (! haveMatch)
	    {
	      continue;
	    }
	  CString givenFile =  GetWindowText(IDC_FILE);
	  if (givenFile.GetLength() > 0)
	    {
	      bool found = false;
	      vector<string>::const_iterator it;
	      for (it = packageInfo.runFiles.begin();
		   ! found && it != packageInfo.runFiles.end();
		   ++ it)
		{
		  PathName path (it->c_str());
		  path.RemoveDirectorySpec ();
		  found = PathName::Match(CT2A(givenFile), path.Get());
		}
	      for (it = packageInfo.docFiles.begin();
		   ! found && it != packageInfo.docFiles.end();
		   ++ it)
		{
		  PathName path (it->c_str());
		  path.RemoveDirectorySpec ();
		  found = PathName::Match(CT2A(givenFile), path.Get());
		}
	      for (it = packageInfo.sourceFiles.begin();
		   ! found && it != packageInfo.sourceFiles.end();
		   ++ it)
		{
		  PathName path (it->c_str());
		  path.RemoveDirectorySpec ();
		  found = PathName::Match(CT2A(givenFile), path.Get());
		}
	      haveMatch = found;
	    }
	  if (! haveMatch)
	    {
	      continue;
	    }
	  LVITEM lvitem;
	  lvitem.iItem = idx;
	  lvitem.mask = LVIF_TEXT | LVIF_PARAM;
	  lvitem.iSubItem = 0;
	  lvitem.lParam = idx;
	  CString displayName (packageInfo.displayName.c_str());
	  lvitem.pszText = displayName.GetBuffer();
	  if (listControl.InsertItem(&lvitem) < 0)
	    {
	      FATAL_WINDOWS_ERROR ("CListCtrl::InsertItem", 0);
	    }
	  lvitem.mask = LVIF_TEXT;
	  lvitem.iItem = idx;
	  lvitem.iSubItem = 1;
	  CString title (packageInfo.title.c_str());
	  lvitem.pszText = title.GetBuffer();
	  if (! listControl.SetItem(&lvitem))
	    {
	      FATAL_WINDOWS_ERROR ("CListCtrl::SetItem", 0);
	    }
	  packages[idx] = packageInfo;
	  ++ idx;
	}
      pIter->Dispose ();
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

   SearchPackageDialog::OnLvnItemchangedListPackages
   _________________________________________________________________________ */

void
SearchPackageDialog::OnLvnItemchangedListPackages (/*[in]*/ NMHDR *   pNMHDR,
						   /*[in]*/ LRESULT * pResult)
{
  UNUSED_ALWAYS (pNMHDR);
  *pResult = 0;
  try
    {
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

   SearchPackageDialog::OnNMRclickListPackages
   _________________________________________________________________________ */

void
SearchPackageDialog::OnNMRclickListPackages (/*[in]*/ NMHDR *	pNMHDR,
					     /*[in]*/ LRESULT * pResult)
{
  LPNMITEMACTIVATE pNMIA =
    reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR); // <fixme>4.71</fixme>
  *pResult = 0;
  try
    {
      UINT n = listControl.GetSelectedCount();
      if (n != 1)
	{
	  return;
	}
      int nItem = listControl.GetNextItem(-1, LVNI_SELECTED);
      if (nItem < 0)
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::GetNextItem", 0);
	}
      DWORD_PTR itemData = listControl.GetItemData(nItem);
      DoContextMenu (pNMIA->ptAction,
		     packages[itemData].deploymentName.c_str());
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

   SearchPackageDialog::DoContextMenu
   _________________________________________________________________________ */

void
SearchPackageDialog::DoContextMenu (/*[in]*/ POINT		pt,
				    /*[in]*/ const char * lpszExternalName)
{
  CMenu menu;
  if (! menu.LoadMenu(ID_PACKAGE_MENU2))
    {
      FATAL_WINDOWS_ERROR ("CMenu::LoadMenu", 0);
    }
  CMenu * pPopup = menu.GetSubMenu(0);
  if (pPopup == 0)
    {
      FATAL_WINDOWS_ERROR ("CMenu::GetSubMenu", 0);
    }
  PackageInfo packageInfo = pManager->GetPackageInfo(lpszExternalName);
  if (GetCursorPos(&pt))
    {
      FATAL_WINDOWS_ERROR ("GetCursorPos", 0);
    }
  UINT cmd =
    TrackPopupMenu(pPopup->GetSafeHmenu(),
		   TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,
		   pt.x,
		   pt.y,
		   0,
		   GetSafeHwnd(),
		   0);
  if (cmd == 0)
    {
      CHECK_WINDOWS_ERROR ("TrackPopupMenu", 0);
      return;
    }
  switch (cmd)
    {
    case ID_PROPERTIES:
      {
	PropSheetPackage::DoModal (packageInfo, this->GetParent());
	break;
      }
    }
}

/* _________________________________________________________________________

   SearchPackageDialog::OnNMDblclkListPackages
   _________________________________________________________________________ */

void
SearchPackageDialog::OnNMDblclkListPackages (/*[in]*/ NMHDR *	pNMHDR,
					     /*[in]*/ LRESULT * pResult)
{
  UNUSED_ALWAYS (pNMHDR);
  *pResult = 0;
  try
    {
      UINT n = listControl.GetSelectedCount();
      if (n != 1)
	{
	  return;
	}
      int nItem = listControl.GetNextItem(-1, LVNI_SELECTED);
      if (nItem < 0)
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::GetNextItem", 0);
	}
      DWORD_PTR itemData = listControl.GetItemData(nItem);
      PackageInfo packageInfo =
	pManager->GetPackageInfo(packages[itemData].deploymentName.c_str());
      PropSheetPackage::DoModal (packageInfo, this->GetParent());
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
