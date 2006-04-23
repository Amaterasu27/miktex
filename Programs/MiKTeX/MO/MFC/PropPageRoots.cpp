/* PropPageRoots.cpp:

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

#include "PropPageRoots.h"
#include "resource.hm"

/* _________________________________________________________________________

   aHelpIDs
   _________________________________________________________________________ */

#define MAKE_ID_HID_PAIR(id) id, H##id

namespace
{
  const DWORD aHelpIDs[] = {
    MAKE_ID_HID_PAIR(IDC_ADD),
    MAKE_ID_HID_PAIR(IDC_LIST),
    MAKE_ID_HID_PAIR(IDC_MOVEDOWN),
    MAKE_ID_HID_PAIR(IDC_MOVEUP),
    MAKE_ID_HID_PAIR(IDC_REMOVE),
    MAKE_ID_HID_PAIR(IDC_SCAN),
    0, 0,
  };
}

/* _________________________________________________________________________

   PropPageTeXMFRoots Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(PropPageTeXMFRoots, CPropertyPage)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_MOVEDOWN, OnMovedown)
  ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
  ON_BN_CLICKED(IDC_REMOVE, OnRemove)
  ON_BN_CLICKED(IDC_SCAN, OnScan)
  ON_NOTIFY(LVN_GETINFOTIP, IDC_LIST, OnGetInfoTip)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnSelectionChange)
  ON_WM_CONTEXTMENU()
  ON_WM_HELPINFO()
END_MESSAGE_MAP();

/* _________________________________________________________________________

   PropPageTeXMFRoots::PropPageTeXMFRoots
   _________________________________________________________________________ */

PropPageTeXMFRoots::PropPageTeXMFRoots ()
  : CPropertyPage (PropPageTeXMFRoots::IDD),
    mustRefreshFndb (false),
    pProgressDialog (0),
    installRoot (SessionWrapper(true)
		 ->GetSpecialPath(SpecialPath::InstallRoot)),
    userDataRoot (SessionWrapper(true)
		  ->GetSpecialPath(SpecialPath::UserDataRoot)),
    userConfigRoot (SessionWrapper(true)
		    ->GetSpecialPath(SpecialPath::UserConfigRoot))
{
  if (SessionWrapper(true)->IsSharedMiKTeXSetup() == TriState::True)
    {
      commonDataRoot =
	SessionWrapper(true)->GetSpecialPath(SpecialPath::CommonDataRoot);
      commonConfigRoot =
	SessionWrapper(true)->GetSpecialPath(SpecialPath::CommonConfigRoot);
    }
  m_psp.dwFlags &= ~(PSP_HASHELP);
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::OnInitDialog
   _________________________________________________________________________ */

BOOL
PropPageTeXMFRoots::OnInitDialog () 
{
  BOOL ret = CPropertyPage::OnInitDialog();

  try
    {
      listControl.SetExtendedStyle (listControl.GetExtendedStyle()
				    | LVS_EX_FULLROWSELECT
				    | LVS_EX_INFOTIP);
      InsertColumn
	(0, T_("Path"),
	 (T_("C:\\Documents and Settings\\All Users\\...\\MiKTeX xxx")));
      InsertColumn (1, T_("Description"), T_("common config xxx"));
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

   PropPageTeXMFRoots::DoDataExchange
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_ADD, addButton);
  DDX_Control (pDX, IDC_LIST, listControl);
  DDX_Control (pDX, IDC_MOVEDOWN, downButton);
  DDX_Control (pDX, IDC_MOVEUP, upButton);
  DDX_Control (pDX, IDC_REMOVE, removeButton);
  DDX_Control (pDX, IDC_SCAN, scanButton);
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::OnScan
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::OnScan ()
{
  try
    {
      auto_ptr<ProgressDialog> pProgressDialog (ProgressDialog::Create());
      pProgressDialog->StartProgressDialog (GetParent()->GetSafeHwnd());
      pProgressDialog->SetTitle (T_("MiKTeX FNDB Maintenance"));
      pProgressDialog->SetLine (1, "Scanning directories...");
      POSITION pos = listControl.GetFirstSelectedItemPosition();
      this->pProgressDialog = pProgressDialog.get();
      MIKTEX_ASSERT (pos != 0);
      while (pos != 0)
	{
	  int idx = listControl.GetNextSelectedItem(pos);
	  Fndb::Refresh (roots[idx], this);
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
  pProgressDialog = 0;
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::OnAdd
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::OnAdd ()
{
  try
    {
      BROWSEINFO browseInfo;
      ZeroMemory (&browseInfo, sizeof(browseInfo));
      browseInfo.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
      browseInfo.lpszTitle = T_("Select the new TEXMF root directory:");
      LPITEMIDLIST pidl = SHBrowseForFolder(&browseInfo);
      if (pidl == 0)
	{
	  return;
	}
      MIKTEXCHAR szDir[BufferSizes::MaxPath];
      BOOL done = SHGetPathFromIDList(pidl, szDir);
      CoTaskMemFree (pidl);
      if (! done)
	{
	  FATAL_WINDOWS_ERROR (T_("SHGetPathFromIDList"), 0);
	}
      LV_ITEM lvitem;
      lvitem.iItem = listControl.GetItemCount();
      lvitem.mask = LVIF_TEXT | LVIF_PARAM;
      lvitem.iSubItem = 0;
      lvitem.pszText = szDir;
      lvitem.lParam = lvitem.iItem;
      if (listControl.InsertItem(&lvitem) < 0)
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::InsertItem"), 0);
	}
      roots.push_back (szDir);
      SetModified (TRUE);
      EnableButtons ();
      mustRefreshFndb = true;
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

   CompareItems
   _________________________________________________________________________ */

static
int
CALLBACK 
CompareItems (/*[in]*/ LPARAM	lParam1,
	      /*[in]*/ LPARAM	lParam2,
	      /*[in]*/ LPARAM	lParamSort)
{
  UNUSED_ALWAYS (lParamSort);
  return (static_cast<int>(lParam1 - lParam2));
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::OnMovedown
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::OnMovedown ()
{
  try
    {
      int idx = GetSelectedItem();

      PathName tmp = roots[idx];
      roots[idx] = roots[idx + 1];
      roots[idx + 1] = tmp;

      LVITEM lvitem;
      lvitem.mask = LVIF_PARAM;
      lvitem.iSubItem = 0;

      lvitem.iItem = idx;
      if (! listControl.GetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::GetItem"), 0);
	}
      lvitem.lParam = idx + 1;
      if (! listControl.SetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
	}

      lvitem.iItem = idx + 1;
      if (! listControl.GetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::GetItem"), 0);
	}
      lvitem.lParam = idx;
      if (! listControl.SetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
	}

      if (! listControl.SortItems(CompareItems, 0))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SortItem"), 0);
	}

      EnableButtons ();

      SetModified (TRUE);

      mustRefreshFndb = true;
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

   PropPageTeXMFRoots::OnMoveup
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::OnMoveup ()
{
  try
    {
      int idx = GetSelectedItem();
      
      PathName tmp = roots[idx];
      roots[idx] = roots[idx - 1];
      roots[idx - 1] = tmp;

      LVITEM lvitem;
      lvitem.mask = LVIF_PARAM;
      lvitem.iSubItem = 0;
      
      lvitem.iItem = idx - 1;
      if (! listControl.GetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::GetItem"), 0);
	}
      lvitem.lParam = idx;
      if (! listControl.SetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
	}

      lvitem.iItem = idx;
      if (! listControl.GetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::GetItem"), 0);
	}
      lvitem.lParam = idx - 1;
      if (! listControl.SetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
	}
      
      if (! listControl.SortItems(CompareItems, 0))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SortItem"), 0);
	}
      
      EnableButtons ();

      SetModified (TRUE);

      mustRefreshFndb = true;
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

   PropPageTeXMFRoots::OnRemove
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::OnRemove ()
{
  try
    {
      UINT n = listControl.GetSelectedCount();
      for (UINT i = 0; i < n; ++i)
	{
	  int idx = GetSelectedItem();
	  if (! listControl.DeleteItem(idx))
	    {
	      FATAL_WINDOWS_ERROR (T_("CListCtrl::DeleteItem"), 0);
	    }
	  roots.erase (roots.begin() + idx);
	}
      EnableButtons ();
      SetModified (TRUE);
      mustRefreshFndb = true;
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

   PropPageTeXMFRoots::InsertColumn
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::InsertColumn (/*[in]*/ int			colIdx,
				  /*[in]*/ const MIKTEXCHAR *	lpszLabel,
				  /*[in]*/ const MIKTEXCHAR *	lpszLongest)
{
  if (listControl.InsertColumn(colIdx,
			       lpszLabel,
			       LVCFMT_LEFT,
			       listControl.GetStringWidth(lpszLongest),
			       colIdx)
      < 0)
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::InsertColumn"), 0);
    }
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::OnSelectionChange
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::OnSelectionChange (/*[in]*/ NMHDR *		pNMHDR,
				       /*[in]*/ LRESULT *	pResult)
{
  UNUSED_ALWAYS (pNMHDR);
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
  *pResult = 0;
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::EnableButtons
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::EnableButtons ()
{
  int nItems = listControl.GetItemCount();

  UINT nSelected = listControl.GetSelectedCount();

  bool canRemove = true;
  bool canMoveUp = true;
  bool canMoveDown = true;

  int idx = -1;

  PolicyFlags policy = SessionWrapper(true)->GetPolicyFlags();

  for (POSITION pos = listControl.GetFirstSelectedItemPosition();
       pos != 0;
       )
    {
      idx = listControl.GetNextSelectedItem(pos);
      PathName root = roots[idx];
      if (root == userDataRoot || root == commonDataRoot
	  || root == userConfigRoot || root == commonConfigRoot)
	{
	  canRemove = false;
	  if ((policy & PolicyFlags::DataRootHighestPriority) != 0)
	    {
	      canMoveUp = false;
	      canMoveDown = false;
	    }
	}
      if (root == installRoot)
	{
	  canRemove = false;
	}
      if (idx > 0 && ((policy & PolicyFlags::DataRootHighestPriority) != 0))
	{
	  PathName prevRoot = roots[idx - 1];
	  if (prevRoot == userDataRoot || prevRoot == commonDataRoot
	      || prevRoot == userConfigRoot || prevRoot == commonConfigRoot)
	    {
	      canMoveUp = false;
	    }
	}
    }

  scanButton.EnableWindow (nSelected > 0);
  CHECK_WINDOWS_ERROR (T_("CWnd::EnableWindow"), 0);

  upButton.EnableWindow (! SessionWrapper(true)->IsMiKTeXDirect()
			 && canMoveUp
			 && nSelected == 1
			 && idx > 0);
  CHECK_WINDOWS_ERROR (T_("CWnd::EnableWindow"), 0);

  downButton.EnableWindow (! SessionWrapper(true)->IsMiKTeXDirect()
			   && canMoveDown
			   && nSelected == 1
			   && idx + 1 < nItems);
  CHECK_WINDOWS_ERROR (T_("CWnd::EnableWindow"), 0);

  removeButton.EnableWindow (! SessionWrapper(true)->IsMiKTeXDirect()
			     && canRemove
			     && nSelected > 0);
  CHECK_WINDOWS_ERROR (T_("CWnd::EnableWindow"), 0);
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::GetSelectedItem
   _________________________________________________________________________ */

int
PropPageTeXMFRoots::GetSelectedItem ()
{
  POSITION pos = listControl.GetFirstSelectedItemPosition();
  if (pos == 0)
    {
      UNEXPECTED_CONDITION (T_("PropPageTeXMFRoots::GetSelectedItem"));
    }
  return (listControl.GetNextSelectedItem(pos));
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::Refresh
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::Refresh ()
{
  if (! listControl.DeleteAllItems())
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::DeleteAllItems"), 0);
    }
  roots.clear ();
  unsigned nRoots = SessionWrapper(true)->GetNumberOfTEXMFRoots();
  for (unsigned r = 0; r < nRoots; ++ r)
    {
      PathName root = SessionWrapper(true)->GetRootDirectory(r);
      LV_ITEM lvitem;
      lvitem.iItem = r;
      lvitem.mask = LVIF_TEXT | LVIF_PARAM;
      lvitem.iSubItem = 0;
      PathName compacted;
      if (! PathCompactPathEx(compacted.GetBuffer(), root.Get(), 45, 0))
	{
	  compacted = root;
	}
      lvitem.pszText = const_cast<MIKTEXCHAR*>(compacted.Get());
      lvitem.lParam = r;
      if (listControl.InsertItem(&lvitem) < 0)
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::InsertItem"), 0);
	}
      tstring description;
      if (root == installRoot)
	{
	  if (! description.empty())
	    {
	      description += T_(", ");
	    }
	  description += T_("Install");
	}
      if (root == userDataRoot)
	{
	  if (! description.empty())
	    {
	      description += T_(", ");
	    }
	  description += T_("UserData");
	}
      if (root == commonDataRoot)
	{
	  if (! description.empty())
	    {
	      description += T_(", ");
	    }
	  description += T_("CommonData");
	}
      if (root == userConfigRoot)
	{
	  if (! description.empty())
	    {
	      description += T_(", ");
	    }
	  description += T_("UserConfig");
	}
      if (root == commonConfigRoot)
	{
	  if (! description.empty())
	    {
	      description += T_(", ");
	    }
	  description += T_("CommonConfig");
	}
      if (! description.empty())
	{
	  lvitem.mask = LVIF_TEXT;
	  lvitem.iSubItem = 1;
	  lvitem.pszText = const_cast<MIKTEXCHAR*>(description.c_str());
	  if (! listControl.SetItem(&lvitem))
	    {
	      FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
	    }
	}
      roots.push_back (root);
    }
  EnableButtons ();
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::OnApply
   _________________________________________________________________________ */

BOOL
PropPageTeXMFRoots::OnApply ()
{
  try
    {
      if (SessionWrapper(true)->IsMiKTeXDirect())
	{
	  UNEXPECTED_CONDITION (T_("PropPageTeXMFRoots::OnApply"));
	}
      
      tstring str;
      for (vector<PathName>::const_iterator it = roots.begin();
	   it != roots.end();
	   ++ it)
	{
	  if (it != roots.begin())
	    {
	      str += T_(';');
	    }
	  str += it->Get();
	}
      SessionWrapper(true)->RegisterRootDirectories (str);
      if (mustRefreshFndb)
	{
	  auto_ptr<ProgressDialog>
	    pProgressDialog (ProgressDialog::Create());
	  pProgressDialog->StartProgressDialog (GetParent()->GetSafeHwnd());
	  pProgressDialog->SetTitle ("MiKTeX FNDB Maintenance");
	  pProgressDialog->SetLine (1, "Scanning directories...");
	  this->pProgressDialog = pProgressDialog.get();
	  mustRefreshFndb = ! Fndb::Refresh(this);
	  pProgressDialog->StopProgressDialog ();
	  pProgressDialog.reset ();
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
  pProgressDialog = 0;
  return (! mustRefreshFndb);
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::OnHelpInfo
   _________________________________________________________________________ */

BOOL
PropPageTeXMFRoots::OnHelpInfo (/*[in]*/ HELPINFO * pHelpInfo) 
{
  return (::OnHelpInfo(pHelpInfo, aHelpIDs, T_("RootsPage.txt")));
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::OnContextMenu
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::OnContextMenu (/*[in]*/ CWnd *	pWnd,
				   /*[in]*/ CPoint	point) 
{
  try
    {
      DoWhatsThisMenu (pWnd, point, aHelpIDs, T_("RootsPage.txt"));
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

   PropPageTeXMFRoots::ReadDirectory
   _________________________________________________________________________ */

bool
MIKTEXCALL
PropPageTeXMFRoots::ReadDirectory (/*[in]*/ const MIKTEXCHAR * lpszPath,
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

   PropPageTeXMFRoots::OnProgress
   _________________________________________________________________________ */

bool
MIKTEXCALL
PropPageTeXMFRoots::OnProgress (/*[in]*/ unsigned		level,
				/*[in]*/ const MIKTEXCHAR *	lpszDirectory)
{
  UNUSED_ALWAYS (level);
  pProgressDialog->SetLine (2, lpszDirectory);
  return (! pProgressDialog->HasUserCancelled());
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::OnGetInfoTip
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::OnGetInfoTip (/*[in]*/ NMHDR *	pNMHDR,
				  /*[in]*/ LRESULT *	pResult) 
{
  *pResult = 0;
  try
    {
      NMLVGETINFOTIP * pInfoTip = reinterpret_cast<NMLVGETINFOTIP*>(pNMHDR);
      MIKTEX_ASSERT (pInfoTip != 0);
      PathName path = roots[pInfoTip->iItem];
      tstring info = path.Get();
      if (path == installRoot)
	{
	  info += T_("\r\n\r\nThis is the installation directory. Packages");
	  info += T_(" will be installed here.");
	}
      if (path == userDataRoot)
	{
	  info += T_("\r\n\r\nThis is the per-user data directory.");
	  info += T_(" User-specific data will be installed here.");
	}
      if (path == commonDataRoot)
	{
	  info += T_("\r\n\r\nThis is the per-machine data directory.");
	  info += T_(" Common data will be installed here.");
	}
      if (path == userConfigRoot)
	{
	  info += T_("\r\n\r\nThis is the per-user configuration directory.");
	  info +=
	    T_(" User-specific configuration files will be installed here.");
	}
      if (path == commonConfigRoot)
	{
	  info +=
	    T_("\r\n\r\nThis is the per-machine configuration directory.");
	  info += T_(" Common configuration files will be installed here.");
	}
      Utils::CopyString (pInfoTip->pszText,
			 pInfoTip->cchTextMax,
			 info.c_str());
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
