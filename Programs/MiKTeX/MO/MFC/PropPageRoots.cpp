/* PropPageRoots.cpp:

   Copyright (C) 2000-2008 Christian Schenk

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
  ON_BN_CLICKED(IDC_SHOW_HIDDEN_ROOTS, OnShowHiddenRoots)
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
    isModified (false),
    pProgressDialog (0),
    userInstallRoot (SessionWrapper(true)
		      ->GetSpecialPath(SpecialPath::UserInstallRoot)),
    userDataRoot (SessionWrapper(true)
		  ->GetSpecialPath(SpecialPath::UserDataRoot)),
    userConfigRoot (SessionWrapper(true)
		    ->GetSpecialPath(SpecialPath::UserConfigRoot)),
    showHiddenRoots (FALSE)
{
  if (SessionWrapper(true)->IsSharedMiKTeXSetup() == TriState::True)
    {
      commonDataRoot =
	SessionWrapper(true)->GetSpecialPath(SpecialPath::CommonDataRoot);
      commonConfigRoot =
	SessionWrapper(true)->GetSpecialPath(SpecialPath::CommonConfigRoot);
      commonInstallRoot =
	SessionWrapper(true)->GetSpecialPath(SpecialPath::CommonInstallRoot);
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
	(0,
	 T_("Path"),
	 "C:\\Documents and Settings\\All Users\\...\\MiKTeX xxx");
      InsertColumn (1,
		    T_("Description"),
		    "common config xxx");
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
  DDX_Check (pDX, IDC_SHOW_HIDDEN_ROOTS, showHiddenRoots);
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
      pProgressDialog->SetLine (1, T_("Scanning directories..."));
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

   PropPageTeXMFRoots::BrowseCallbackProc
   _________________________________________________________________________ */

INT
CALLBACK
PropPageTeXMFRoots::BrowseCallbackProc (/*[in]*/ HWND	hwnd, 
					/*[in]*/ UINT	uMsg,
					/*[in]*/ LPARAM	lParam, 
					/*[in]*/ LPARAM	pData)
{
  UNUSED_ALWAYS (lParam);
  UNUSED_ALWAYS (pData);

  try
    {
#if 0
      PropPageTeXMFRoots * This = reinterpret_cast<PropPageTeXMFRoots*>(pData);
#endif
      switch (uMsg) 
	{
	case BFFM_INITIALIZED:
	  char szDrive[BufferSizes::MaxPath];
	  PathName::Split (PathName().SetToCurrentDirectory().Get(),
			   szDrive, BufferSizes::MaxPath,
			   0, 0,
			   0, 0,
			   0, 0);
	  if (szDrive[0] != 0)
	    {
	      PathName root (szDrive, "\\", 0, 0);
	      ::SendMessage (hwnd,
			     BFFM_SETSELECTION,
			     TRUE,
			     reinterpret_cast<LPARAM>(root.Get()));
	    }
	  return (0);
	default:
	  return (0);
	}
    }
  catch (const exception &)
    {
      return (0);
    }
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::OnAdd
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::OnAdd ()
{
  try
    {
      BROWSEINFOA browseInfo;
      ZeroMemory (&browseInfo, sizeof(browseInfo));
      browseInfo.hwndOwner = GetSafeHwnd();
      browseInfo.ulFlags = BIF_RETURNONLYFSDIRS;
      // We cannot use BIF_USENEWUI because we are running in a
      // multithreaded apartment.  See MSDN "INFO: Calling Shell
      // Functions and Interfaces from a Multithreaded Apartment".
#if 0
      browseInfo.ulFlags |= BIF_USENEWUI;
#endif
      browseInfo.lpfn = BrowseCallbackProc;
      browseInfo.lParam = reinterpret_cast<LPARAM>(this);
      browseInfo.lpszTitle = T_("Select the root directory to be added:");
      LPITEMIDLIST pidlRoot = 0;
      if (SHGetSpecialFolderLocation(0, CSIDL_DRIVES, &pidlRoot) != S_OK)
	{
	  FATAL_WINDOWS_ERROR ("SHGetSpecialFolderLocation", 0);
	}
      AutoCoTaskMem autoFree (pidlRoot);
      browseInfo.pidlRoot = pidlRoot;
      LPITEMIDLIST pidl = SHBrowseForFolderA(&browseInfo);
      if (pidl == 0)
	{
	  return;
	}
      char szDir[BufferSizes::MaxPath];
      BOOL done = SHGetPathFromIDListA(pidl, szDir);
      CoTaskMemFree (pidl);
      if (! done)
	{
	  FATAL_WINDOWS_ERROR ("SHGetPathFromIDList", 0);
	}
      CheckRoot (szDir);
      LVITEMA lvitem;
      lvitem.iItem = listControl.GetItemCount();
      lvitem.mask = LVIF_TEXT | LVIF_PARAM;
      lvitem.iSubItem = 0;
      lvitem.pszText = szDir;
      lvitem.lParam = lvitem.iItem;
      if (listControl.InsertItem(&lvitem) < 0)
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::InsertItem", 0);
	}
      roots.push_back (szDir);
      SetChanged (true);
      EnableButtons ();
      isModified = true;
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

   PropPageTeXMFRoots::CheckRoot
   _________________________________________________________________________ */

const char * tdsDirs[] = {
  "bibtex",
  "dvipdfm",
  "dvips",
  "fontname",
  "fonts",
  "makeindex",
  "metafont",
  "metapost",
  "mft",
  "miktex",
  "pdftex",
  "psutils",
  "scripts",
  "tex",
  "tpm",
  "ttf2pfb",
  "ttf2tfm",
};

void
PropPageTeXMFRoots::CheckRoot (/*[in]*/ const PathName & root)
{
  auto_ptr<DirectoryLister> pLister (DirectoryLister::Open(root));
  DirectoryEntry entry;
  bool isEmpty = true;
  while (pLister->GetNext(entry))
    {
      isEmpty = false;
      if (entry.isDirectory)
	{
	  PathName name (entry.name);
	  for (size_t idx = 0;
	       idx < sizeof(tdsDirs) / sizeof(tdsDirs[0]);
	       ++ idx)
	    {
	      if (name == tdsDirs[idx])
		{
		  return;
		}
	    }
	}
    }
  if (! isEmpty)
    {
      FATAL_MIKTEX_ERROR ("PropPageTeXMFRoots::CheckRoot",
			  T_("Not a TDS-compliant root directory."),
			  root.Get());
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

      LVITEMA lvitem;
      lvitem.mask = LVIF_PARAM;
      lvitem.iSubItem = 0;

      lvitem.iItem = idx;
      if (! listControl.GetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::GetItem", 0);
	}
      lvitem.lParam = idx + 1;
      if (! listControl.SetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::SetItem", 0);
	}

      lvitem.iItem = idx + 1;
      if (! listControl.GetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::GetItem", 0);
	}
      lvitem.lParam = idx;
      if (! listControl.SetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::SetItem", 0);
	}

      if (! listControl.SortItems(CompareItems, 0))
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::SortItem", 0);
	}

      EnableButtons ();

      SetChanged (true);

      isModified = true;
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

      LVITEMA lvitem;
      lvitem.mask = LVIF_PARAM;
      lvitem.iSubItem = 0;
      
      lvitem.iItem = idx - 1;
      if (! listControl.GetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::GetItem", 0);
	}
      lvitem.lParam = idx;
      if (! listControl.SetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::SetItem", 0);
	}

      lvitem.iItem = idx;
      if (! listControl.GetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::GetItem", 0);
	}
      lvitem.lParam = idx - 1;
      if (! listControl.SetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::SetItem", 0);
	}
      
      if (! listControl.SortItems(CompareItems, 0))
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::SortItem", 0);
	}
      
      EnableButtons ();

      SetChanged (true);

      isModified = true;
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
	      FATAL_WINDOWS_ERROR ("CListCtrl::DeleteItem", 0);
	    }
	  roots.erase (roots.begin() + idx);
	}
      EnableButtons ();
      SetChanged (true);
      isModified = true;
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

   PropPageTeXMFRoots::OnShowHiddenRoots
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::OnShowHiddenRoots ()
{
  try
    {
      UpdateData (TRUE);
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
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::InsertColumn
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::InsertColumn (/*[in]*/ int		colIdx,
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

   PropPageTeXMFRoots::IsHiddenRoot
   _________________________________________________________________________ */

bool
PropPageTeXMFRoots::IsHiddenRoot (/*[in]*/ const PathName & root)
{
  PolicyFlags policy = SessionWrapper(true)->GetPolicyFlags();
  return (((policy & PolicyFlags::DataRootHighestPriority) != 0)
	  && (root == userDataRoot
	      || root == commonDataRoot
	      || root == userConfigRoot
	      || root == commonConfigRoot));
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::EnableButtons
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::EnableButtons ()
{
  int nItems = listControl.GetItemCount();

  UINT nSelected = listControl.GetSelectedCount();

  bool isAdmin =
    (! IsWindowsNT()
     || IsWindowsVista()
     || SessionWrapper(true)->IsUserAnAdministrator()
     || SessionWrapper(true)->IsUserAPowerUser());

  bool shared =
    (SessionWrapper(true)->IsSharedMiKTeXSetup() == TriState::True);

  bool canAdd = (! shared || isAdmin);
  bool canRemove = (! shared || isAdmin);
  bool canMoveUp = (! shared || isAdmin);
  bool canMoveDown = (! shared || isAdmin);

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
      if (root == commonInstallRoot || root == userInstallRoot)
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

  upButton.EnableWindow (! SessionWrapper(true)->IsMiKTeXDirect()
			 && canMoveUp
			 && nSelected == 1
			 && idx > 0);

  downButton.EnableWindow (! SessionWrapper(true)->IsMiKTeXDirect()
			   && canMoveDown
			   && nSelected == 1
			   && idx + 1 < nItems);

  addButton.EnableWindow (! SessionWrapper(true)->IsMiKTeXDirect()
			  && canAdd);

  removeButton.EnableWindow (! SessionWrapper(true)->IsMiKTeXDirect()
			     && canRemove
			     && nSelected > 0);
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
      UNEXPECTED_CONDITION ("PropPageTeXMFRoots::GetSelectedItem");
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
      FATAL_WINDOWS_ERROR ("CListCtrl::DeleteAllItems", 0);
    }
  roots.clear ();
  unsigned nRoots = SessionWrapper(true)->GetNumberOfTEXMFRoots();
  for (unsigned rootIdx = 0; rootIdx < nRoots; ++ rootIdx)
    {
      PathName root = SessionWrapper(true)->GetRootDirectory(rootIdx);
      if (! showHiddenRoots && IsHiddenRoot(root))
	{
	  continue;
	}
      LVITEMA lvitem;
      lvitem.iItem = roots.size();
      lvitem.mask = LVIF_TEXT | LVIF_PARAM;
      lvitem.iSubItem = 0;
      PathName compacted;
      if (! PathCompactPathEx(compacted.GetBuffer(), root.Get(), 45, 0))
	{
	  compacted = root;
	}
      lvitem.pszText = const_cast<char*>(compacted.Get());
      lvitem.lParam = roots.size();
      if (listControl.InsertItem(&lvitem) < 0)
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::InsertItem", 0);
	}
      string description;
      if (root == userInstallRoot)
	{
	  if (! description.empty())
	    {
	      description += ", ";
	    }
	  description += "UserInstall";
	}
      if (root == commonInstallRoot)
	{
	  if (! description.empty())
	    {
	      description += ", ";
	    }
	  description += "CommonInstall";
	}
      if (root == userDataRoot)
	{
	  if (! description.empty())
	    {
	      description += ", ";
	    }
	  description += "UserData";
	}
      if (root == commonDataRoot)
	{
	  if (! description.empty())
	    {
	      description += ", ";
	    }
	  description += "CommonData";
	}
      if (root == userConfigRoot)
	{
	  if (! description.empty())
	    {
	      description += ", ";
	    }
	  description += "UserConfig";
	}
      if (root == commonConfigRoot)
	{
	  if (! description.empty())
	    {
	      description += ", ";
	    }
	  description += "CommonConfig";
	}
      if (! description.empty())
	{
	  lvitem.mask = LVIF_TEXT;
	  lvitem.iSubItem = 1;
	  lvitem.pszText = const_cast<char*>(description.c_str());
	  if (! listControl.SetItem(&lvitem))
	    {
	      FATAL_WINDOWS_ERROR ("CListCtrl::SetItem", 0);
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
  if (isModified)
    {
      try
	{
	  if (SessionWrapper(true)->IsMiKTeXDirect())
	    {
	      UNEXPECTED_CONDITION ("PropPageTeXMFRoots::OnApply");
	    }
	  
	  PolicyFlags policy = SessionWrapper(true)->GetPolicyFlags();
	  
	  string str;

	  for (vector<PathName>::const_iterator it = roots.begin();
	       it != roots.end();
	       ++ it)
	    {
	      if (((policy & PolicyFlags::DataRootHighestPriority) != 0)
		  && (*it == userDataRoot || *it == commonDataRoot
		      || *it == userConfigRoot || *it == commonConfigRoot))
		{
		  continue;
		}
	      if (! str.empty())
		{
		  str += ';';
		}
	      str += it->Get();
	    }

	  SessionWrapper(true)->RegisterRootDirectories (str);

	  auto_ptr<ProgressDialog>
	    pProgressDialog (ProgressDialog::Create());

	  pProgressDialog->StartProgressDialog (GetParent()->GetSafeHwnd());
	  pProgressDialog->SetTitle (T_("MiKTeX FNDB Maintenance"));
	  pProgressDialog->SetLine (1, T_("Scanning directories..."));
	  this->pProgressDialog = pProgressDialog.get();

	  isModified = ! Fndb::Refresh(this);

	  pProgressDialog->StopProgressDialog ();
	  pProgressDialog.reset ();

	  SetElevationRequired (isModified);
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
  return (! isModified);
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::OnHelpInfo
   _________________________________________________________________________ */

BOOL
PropPageTeXMFRoots::OnHelpInfo (/*[in]*/ HELPINFO * pHelpInfo) 
{
  return (::OnHelpInfo(pHelpInfo, aHelpIDs, "RootsPage.txt"));
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
      DoWhatsThisMenu (pWnd, point, aHelpIDs, "RootsPage.txt");
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
PropPageTeXMFRoots::ReadDirectory (/*[in]*/ const char * lpszPath,
				   /*[out]*/ char * * ppSubDirNames,
				   /*[out]*/ char * * ppFileNames,
				   /*[out]*/ char * * ppFileNameInfos)
  
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
PropPageTeXMFRoots::OnProgress (/*[in]*/ unsigned	level,
				/*[in]*/ const char *	lpszDirectory)
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
      string info = path.Get();
      bool maintainedByMiKTeX =
	(path == userInstallRoot
	 || path == commonInstallRoot
	 || path == userDataRoot
	 || path == commonDataRoot
	 || path == userConfigRoot
	 || path == commonConfigRoot);
      if (path == userInstallRoot)
	{
	  info += T_("\r\n\r\n\
This is the per-user installation directory.");
	}
      if (path == commonInstallRoot)
	{
	  info += T_("\r\n\r\n\
This is the per-machine installation directory.");
	}
      if (path == userDataRoot)
	{
	  info += T_("\r\n\r\n\
This is the per-user data directory.");
	}
      if (path == commonDataRoot)
	{
	  info += T_("\r\n\r\n\
This is the per-machine data directory.");
	}
      if (path == userConfigRoot)
	{
	  info += T_("\r\n\r\n\
This is the per-user configuration directory.");
	}
      if (path == commonConfigRoot)
	{
	  info +=
	    T_("\r\n\r\n\
This is the per-machine configuration directory.");
	}
      if (maintainedByMiKTeX)
	{
	  info += T_("\r\n\r\n\
This directory is maintained by MiKTeX. \
You should not install your own files here, because they might get lost \
when MiKTeX is updated.");
	}
      else
	{
	  info += T_("\r\n\r\n\
This directory can be used for local additions.");
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

/* _________________________________________________________________________

   PropPageTeXMFRoots::SetElevationRequired
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::SetElevationRequired (/*[in]*/ bool f)
{
  bool restrictedUserSetup =
    (! (SessionWrapper(true)->IsSharedMiKTeXSetup() == TriState::True
	|| ! IsWindowsNT()
	|| SessionWrapper(true)->IsUserAnAdministrator()));
  if (IsWindowsVista() && ! restrictedUserSetup)
    {
      HWND hwnd = ::GetDlgItem(::GetParent(m_hWnd), IDOK);
      if (hwnd == 0)
	{
	  UNEXPECTED_CONDITION ("PropPageTeXMFRoots::SetElevationRequired");
	}
      Button_SetElevationRequiredState (hwnd, f ? TRUE : FALSE);
      hwnd = ::GetDlgItem(::GetParent(m_hWnd), ID_APPLY_NOW);
      if (hwnd == 0)
	{
	  UNEXPECTED_CONDITION ("PropPageTeXMFRoots::SetElevationRequired");
	}
      Button_SetElevationRequiredState (hwnd, f ? TRUE : FALSE);
    }
}

/* _________________________________________________________________________

   PropPageTeXMFRoots::SetChanged
   _________________________________________________________________________ */

void
PropPageTeXMFRoots::SetChanged (/*[in]*/ bool f)
{
  SetElevationRequired (f);
  SetModified (f ? TRUE : FALSE);
}
