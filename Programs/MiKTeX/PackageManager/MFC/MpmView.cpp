/* MpmView.cpp:

   Copyright (C) 2002-2007 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */


#include "stdafx.h"
#include "mpm.h"

#include "MainFrame.h"
#include "MpmDoc.h"
#include "MpmView.h"

IMPLEMENT_DYNCREATE(MpmView, CListView);

/* _________________________________________________________________________

   MpmView Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(MpmView, CListView)
  ON_BN_CLICKED(IDOK, OnButtonSearchClicked)
  ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
  ON_COMMAND(ID_INSTALL, OnInstall)
  ON_COMMAND(ID_PROPERTIES, OnProperties)
  ON_COMMAND(ID_RESET_VIEW, OnResetView)
  ON_COMMAND(ID_SELECT_ALL, OnSelectAll)
  ON_COMMAND(ID_SELECT_INSTALLABLE_PACKAGES, OnSelectInstallablePackages)
  ON_COMMAND(ID_SELECT_UPDATEABLE_PACKAGES, OnSelectUpdateablePackages)
  ON_COMMAND(ID_SITE_WIZARD, OnSiteWizard)
  ON_COMMAND(ID_UNINSTALL, OnUninstall)
  ON_COMMAND(ID_UPDATE_DATABASE, OnUpdateDatabase)
  ON_COMMAND(ID_UPDATE_WIZARD, OnUpdateWizard)
  ON_MESSAGE(WM_FILLLISTVIEW, OnFillListView)
  ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnLvnColumnclick)
  ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
  ON_UPDATE_COMMAND_UI(ID_INSTALL, OnUpdateInstall)
  ON_UPDATE_COMMAND_UI(ID_PROPERTIES, OnUpdateProperties)
  ON_UPDATE_COMMAND_UI(ID_RESET_VIEW, OnUpdateResetView)
  ON_UPDATE_COMMAND_UI(ID_SELECT_UPDATEABLE_PACKAGES,
		       OnUpdateSelectUpdateablePackages)
  ON_UPDATE_COMMAND_UI(ID_SITE_WIZARD, OnUpdateSiteWizard)
  ON_UPDATE_COMMAND_UI(ID_UNINSTALL, OnUpdateUninstall)
  ON_UPDATE_COMMAND_UI(ID_UPDATE_DATABASE, OnUpdateUpdateDatabase)
  ON_UPDATE_COMMAND_UI(ID_UPDATE_WIZARD, OnUpdateUpdateWizard)
  ON_WM_CONTEXTMENU()
END_MESSAGE_MAP();

/* _________________________________________________________________________

   MpmView::GetContainerPath
   _________________________________________________________________________ */

bool
MpmView::GetContainerPath (/*[in]*/ const tstring &	deploymentName,
				      /*[in,out]*/ tstring &	path)
{
  PackageInfo packageInfo = pManager->GetPackageInfo(deploymentName.c_str());
  if (packageInfo.requiredBy.size() == 0)
    {
      return (false);
    }
  PackageInfo packageInfo2 =
    pManager->GetPackageInfo(packageInfo.requiredBy[0].c_str());
  tstring parent;
  // <recursivecall>
  GetContainerPath (packageInfo.requiredBy[0], parent);
  // </recursivecall>
  path = parent;
  path += PathName::DirectoryDelimiter;
  path += packageInfo2.displayName;
  return (true);
}

/* _________________________________________________________________________

   MpmView::MpmView
   _________________________________________________________________________ */

MpmView::MpmView ()
  : pManager (PackageManager::Create()),
    pSession (true)
{
}

/* _________________________________________________________________________

   MpmView::PreCreateWindow
   _________________________________________________________________________ */

BOOL
MpmView::PreCreateWindow (/*[in,out]*/ CREATESTRUCT & cs)
{
  cs.style |= LVS_REPORT;
  return (CListView::PreCreateWindow(cs));
}

/* _________________________________________________________________________

   MpmView::InsertColumn
   _________________________________________________________________________ */

void
MpmView::InsertColumn (/*[in]*/ int			colIdx,
		       /*[in]*/ const MIKTEXCHAR *	lpszLabel,
		       /*[in]*/ const MIKTEXCHAR *	lpszLongest)
{
  CListCtrl & listControl = GetListCtrl();
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

   MpmView::OnInitialUpdate
   _________________________________________________________________________ */

void
MpmView::OnInitialUpdate ()
{
  try
    {
      CListView::OnInitialUpdate ();
      
      MIKTEX_ASSERT (GetStyle() & LVS_REPORT);
      
      CListCtrl & listctrl = GetListCtrl();
      
#if 1
      listctrl.SetExtendedStyle (listctrl.GetExtendedStyle()
				 | LVS_EX_FULLROWSELECT);
#endif

      int colIdx = 0;

      InsertColumn (colIdx,
		    T_("Name"),
		    T_("xxx mmmmmmmm"));

      ++ colIdx;
      
      InsertColumn (colIdx,
		    T_("Category"),
		    T_("xxx Formats\\LaTeX\\LaTeX contrib"));

      ++ colIdx;
      
      InsertColumn (colIdx,
		    T_("Size"),
		    T_("xxx 12341234"));

      ++ colIdx;
      
      InsertColumn (colIdx,
		    T_("Packaged on"),
		    T_("xxx 2002-02-22"));

      ++ colIdx;
      
      InsertColumn (colIdx,
		    T_("Installed on"),
		    T_("xxx 2002-02-22"));

      ++ colIdx;
      
      InsertColumn (colIdx,
		    T_("Title"),
	    T_("xxx mmmm mmmmmmmm mmmmmmmmm mmmmmmmmmmm mm mmmmmmmmm"));
      
      PostMessage (WM_FILLLISTVIEW);
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

   MpmView::OnPreparePrinting
   _________________________________________________________________________ */

BOOL
MpmView::OnPreparePrinting (/*[in]*/ CPrintInfo * pInfo)
{
  try
    {
      return (DoPreparePrinting(pInfo));
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

   MpmView::OnBeginPrinting
   _________________________________________________________________________ */

void
MpmView::OnBeginPrinting (/*[in]*/ CDC *	pDC,
			  /*[in]*/ CPrintInfo *	pInfo)
{
  UNUSED_ALWAYS (pDC);
  UNUSED_ALWAYS (pInfo);
}

/* _________________________________________________________________________

   MpmView::OnEndPrinting
   _________________________________________________________________________ */

void
MpmView::OnEndPrinting (/*[in]*/ CDC *		pDC,
			/*[in]*/ CPrintInfo *	pInfo)
{
  UNUSED_ALWAYS (pDC);
  UNUSED_ALWAYS (pInfo);
}

/* _________________________________________________________________________

   MpmView::FillListView
   _________________________________________________________________________ */

void
MpmView::FillListView ()
{
  pManager->UnloadDatabase ();
  CWaitCursor cur;
  auto_ptr<PackageIterator> pIter (pManager->CreateIterator());
  PackageInfo packageInfo;
  packages.clear ();
  if (! GetListCtrl().DeleteAllItems())
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::DeleteAllItems"), 0);
    }
  for (int idx = 0; pIter->GetNext(packageInfo); ++ idx)
    {
      if (! packageInfo.IsPureContainer())
	{
	  InsertItem (idx, packageInfo);
	}
    }
  pIter->Dispose ();
  sortOrder = 1;
  clickedColumn = 0;
  GetListCtrl().SortItems (CompareItems, reinterpret_cast<LPARAM>(this));
}

/* _________________________________________________________________________

   MpmView::OnFillListView
   _________________________________________________________________________ */

LRESULT
MpmView::OnFillListView (/*[in]*/ WPARAM	wParam,
			 /*[in]*/ LPARAM	lParam)
{
  UNUSED_ALWAYS (wParam);
  UNUSED_ALWAYS (lParam);
  try
    {
      FillListView ();
      return (0);
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
      return (0);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
      return (0);
    }
}

/* _________________________________________________________________________

   MpmView::OnLvnColumnclick
   _________________________________________________________________________ */

void
MpmView::OnLvnColumnclick (/*[in]*/ NMHDR *	pNMHDR,
			   /*[in]*/ LRESULT *	pResult)
{
  *pResult = 0;
  try
    {
      LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
      if (clickedColumn == pNMLV->iSubItem)
	{
	  sortOrder *= -1;
	}
      else
	{
	  sortOrder = 1;
	}
      clickedColumn = pNMLV->iSubItem;
      if (! GetListCtrl().SortItems(CompareItems,
				    reinterpret_cast<LPARAM>(this)))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SortItems"), 0);
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

   MpmView::CompareItems
   _________________________________________________________________________ */

int
CALLBACK
MpmView::CompareItems (/*[in]*/ LPARAM	lParam1,
		       /*[in]*/ LPARAM	lParam2,
		       /*[in]*/ LPARAM	lParamSort)
{
  MpmView * This =
    reinterpret_cast<MpmView*>(lParamSort);
  try
    {
      const PackageInfo & packageInfo1 = This->packages[lParam1];
      const PackageInfo & packageInfo2 = This->packages[lParam2];
      int ret;
      switch (This->clickedColumn)
	{
	case 0:
	  ret =
	    (StringCompare(packageInfo1.deploymentName.c_str(),
			   packageInfo2.deploymentName.c_str(),
			   true));
	  break;
	case 1:
	  {
	    tstring str1;
	    tstring str2;
	    This->GetContainerPath (packageInfo1.deploymentName, str1);
	    This->GetContainerPath (packageInfo2.deploymentName, str2);
	    ret = PathName::Compare(str1.c_str(), str2.c_str());
	    break;
	  }
	case 2:
	  ret =
	    (static_cast<int>(packageInfo1.GetSize())
	     - static_cast<int>(packageInfo2.GetSize()));
	  break;
	case 3:
	  ret =
	    static_cast<int>(difftime(packageInfo1.timePackaged,
				      packageInfo2.timePackaged));
	  break;
	case 4:
	  ret =
	    static_cast<int>(difftime(packageInfo1.timeInstalled,
				      packageInfo2.timeInstalled));
	  break;
	case 5:
	  ret =
	    (StringCompare(packageInfo1.title.c_str(),
			   packageInfo2.title.c_str(),
			   true));
	  break;
	default:
	  ret = 0;
	  break;
	}
      return (ret * This->sortOrder);
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (This, e);
      return (0);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (This, e);
      return (0);
    }
}

/* _________________________________________________________________________

   MpmView::OnButtonSearchClicked
   _________________________________________________________________________ */

afx_msg
void
MpmView::OnButtonSearchClicked ()
{
  try
    {
      CWinApp * pApp = AfxGetApp();
      MIKTEX_ASSERT
	(pApp->GetMainWnd()->IsKindOf(RUNTIME_CLASS(MainFrame)));
      MainFrame * pMain
	= DYNAMIC_DOWNCAST(MainFrame, pApp->GetMainWnd());
      CString deploymentName = pMain->GetDlgBarText(IDC_EDIT_PACKAGE_NAME);
      deploymentName.MakeLower ();
      CString searchWords =  pMain->GetDlgBarText(IDC_EDIT_WORDS);
      searchWords.MakeLower ();
      CString fileNamePattern = pMain->GetDlgBarText(IDC_EDIT_FILE_NAME);
      CListCtrl & listctrl = GetListCtrl();
      CWaitCursor cur;
      auto_ptr<PackageIterator> pIter (pManager->CreateIterator());
      PackageInfo packageInfo;
      int idx = 0;
      packages.clear ();
      if (! listctrl.DeleteAllItems())
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::DeleteAllItems"), 0);
	}
      while (pIter->GetNext(packageInfo))
	{
	  bool match = false;
	  CString str = packageInfo.deploymentName.c_str();
	  str.MakeLower ();
	  match = (deploymentName.GetLength() == 0
		    || str.Find(deploymentName) >= 0);
	  if (! match)
	    {
	      continue;
	    }
	  // <todo>
	  match =
	    (searchWords.GetLength() == 0
	     || (packageInfo.title.find(searchWords) != tstring::npos
		 || (packageInfo.description.find(searchWords)
		     != tstring::npos)));
	  // </todo>
	  if (! match)
	    {
	      continue;
	    }
	  if (fileNamePattern.GetLength() > 0)
	    {
	      vector<tstring>::const_iterator it;
	      bool found = false;
	      for (it = packageInfo.runFiles.begin();
		   ! found && it != packageInfo.runFiles.end();
		   ++ it)
		{
		  found =
		    PathName::Match(fileNamePattern,
				    PathName(*it).RemoveDirectorySpec());
		}
	      for (it = packageInfo.docFiles.begin();
		   ! found && it != packageInfo.docFiles.end();
		   ++ it)
		{
		  found =
		    PathName::Match(fileNamePattern,
				    PathName(*it).RemoveDirectorySpec());
		}
	      for (it = packageInfo.sourceFiles.begin();
		   ! found && it != packageInfo.sourceFiles.end();
		   ++ it)
		{
		  found =
		    PathName::Match(fileNamePattern,
				    PathName(*it).RemoveDirectorySpec());
		}
	      match = found;
	    }
	  if (! match || packageInfo.IsPureContainer())
	    {
	      continue;
	    }
	  InsertItem (idx, packageInfo);
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

   MpmView::InsertItem
   _________________________________________________________________________ */

void
MpmView::InsertItem (/*[in]*/ int			idx,
		     /*[in]*/ const Packages::PackageInfo & packageInfo)
{
  CListCtrl & listctrl = GetListCtrl();

  LVITEM lvitem;

  packages[idx] = packageInfo;
  
  // column 0
  lvitem.iItem = 0;
  lvitem.mask = LVIF_TEXT | LVIF_PARAM;
  lvitem.iSubItem = 0;
  lvitem.lParam = idx;
  lvitem.pszText = const_cast<MIKTEXCHAR *>(packageInfo.deploymentName.c_str());
  if (listctrl.InsertItem(&lvitem) < 0)
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::InsertItem"), 0);
    }
  
  // column 1
  lvitem.mask = LVIF_TEXT;
  lvitem.iSubItem = 1;
  tstring str;
  if (! GetContainerPath(packageInfo.deploymentName, str))
    {
      str = T_("");
    }
  lvitem.pszText = const_cast<MIKTEXCHAR *>(str.c_str());
  if (! listctrl.SetItem(&lvitem))
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
    }

  // column 2
  lvitem.iSubItem = 2;
  lvitem.pszText =
    const_cast<MIKTEXCHAR *>
    (NUMTOSTR(static_cast<unsigned>(packageInfo.GetSize())));
  if (! listctrl.SetItem(&lvitem))
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
    }

  // column 3
  CString str2;
  lvitem.iSubItem = 3;
  CTime t (packageInfo.timePackaged);
  str2 = t.FormatGmt(T_("%Y-%m-%d"));
  lvitem.pszText = const_cast<MIKTEXCHAR *>(str2.GetString());
  if (! listctrl.SetItem(&lvitem))
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
    }

  // column 4
  lvitem.iSubItem = 4;
  if (packageInfo.timeInstalled == 0)
    {
      str2 = T_("");
    }
  else
    {
      CTime t (packageInfo.timeInstalled);
      str2 = t.FormatGmt(T_("%Y-%m-%d"));
    }
  lvitem.pszText = const_cast<MIKTEXCHAR *>(str2.GetString());
  if (! listctrl.SetItem(&lvitem))
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
    }

  // column 5
  lvitem.iSubItem = 5;
  lvitem.pszText = const_cast<MIKTEXCHAR *>(packageInfo.title.c_str());
  if (! listctrl.SetItem(&lvitem))
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
    }
}

/* _________________________________________________________________________

   MpmView::OnContextMenu
   _________________________________________________________________________ */

void
MpmView::OnContextMenu (/*[in]*/ CWnd *	pWnd,
				   /*[in]*/ CPoint	point)
{
  try
    {
      CListCtrl & listctrl = GetListCtrl();

      int nInstall = 0;
      int nUninstall = 0;

      CMenu menu;

      int count = listctrl.GetSelectedCount();
      
      if (count == 0)
	{
	  if (! menu.LoadMenu(IDR_CONTEXT_MENU))
	    {
	      FATAL_WINDOWS_ERROR (T_("CMenu::LoadMenu"), 0);
	    }
	}
      else
	{
	  if (! pSession->IsMiKTeXDirect())
	    {
	      int idx = -1;
	      while ((idx = listctrl.GetNextItem(idx, LVNI_SELECTED)) >= 0)
		{
		  PackageInfo packageInfo =
		    packages[listctrl.GetItemData(idx)];
		  if (packageInfo.timeInstalled == 0)
		    {
		      ++ nInstall;
		    }
		  else
		    {
		      ++ nUninstall;
		    }
		}
	      CHECK_WINDOWS_ERROR (T_("CListCtrl::GetNextItem"), 0);
	      MIKTEX_ASSERT (nInstall + nUninstall == count);
	    }
	  if (! menu.LoadMenu(IDR_CONTEXT_MENU_SEL))
	    {
	      FATAL_WINDOWS_ERROR (T_("CMenu::LoadMenu"), 0);
	    }
	}
      CMenu * pPopup = menu.GetSubMenu(0);
      if (pPopup == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("CMenu::GetSubMenu"), 0);
	}
      if (count > 0)
	{
	  if (pPopup->EnableMenuItem(ID_INSTALL,
				     (MF_BYCOMMAND
				      | (nInstall == count
					 ? MF_ENABLED
					 : MF_GRAYED)))
	      < 0)
	    {
	      FATAL_WINDOWS_ERROR (T_("CMenu::EnableMenuItem"), 0);
	    }
	  if (pPopup->EnableMenuItem(ID_UNINSTALL,
				     (MF_BYCOMMAND
				      | (nUninstall == count
					 ? MF_ENABLED
					 : MF_GRAYED)))
	      < 0)
	    {
	      FATAL_WINDOWS_ERROR (T_("CMenu::EnableMenuItem"), 0);
	    }
	  if (pPopup->EnableMenuItem(ID_PROPERTIES,
				     (MF_BYCOMMAND
				      | (1 == count ?
					 MF_ENABLED
					 : MF_GRAYED))))
	    {
	      FATAL_WINDOWS_ERROR (T_("CMenu::EnableMenuItem"), 0);
	    }
	}
      UINT cmd =
	TrackPopupMenu(pPopup->GetSafeHmenu(),
		       TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,
		       point.x,
		       point.y,
		       0,
		       pWnd->GetSafeHwnd(),
		       0);
      if (cmd == 0)
	{
	  CHECK_WINDOWS_ERROR (T_("TrackPopupMenu"), 0);
	  return;
	}
      switch (cmd)
	{
	case ID_INSTALL:
	  OnInstall ();
	  break;
	case ID_UNINSTALL:
	  OnUninstall ();
	  break;
	case ID_PROPERTIES:
	  OnProperties ();
	  break;
	case ID_SITE_WIZARD:
	  OnSiteWizard ();
	  break;
	case ID_UPDATE_WIZARD:
	  OnUpdateWizard ();
	  break;
	case ID_UPDATE_DATABASE:
	  OnUpdateDatabase ();
	  break;
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

   MpmView::OnNMDblclk
   _________________________________________________________________________ */

void
MpmView::OnNMDblclk (/*[in]*/ NMHDR *	pNMHDR,
				/*[in]*/ LRESULT *	pResult)
{
  UNUSED_ALWAYS (pNMHDR);
  OnProperties ();
  *pResult = 0;
}

/* _________________________________________________________________________

   MpmView::OnUpdateInstall
   _________________________________________________________________________ */

void
MpmView::OnUpdateInstall (/*[in]*/ CCmdUI *	pCmdUI)
{
  try
    {
      BOOL enable;
      if (pSession->IsMiKTeXDirect())
	{
	  enable = FALSE;
	}
      else
	{
	  CListCtrl & listctrl = GetListCtrl();
	  int idx = -1;
	  enable = (listctrl.GetSelectedCount() > 0);
	  while (enable
		 && (idx = listctrl.GetNextItem(idx, LVNI_SELECTED)) >= 0)
	    {
	      PackageInfo packageInfo = packages[listctrl.GetItemData(idx)];
	      if (packageInfo.timeInstalled > 0)
		{
		  enable = FALSE;
		}
	    }
	  CHECK_WINDOWS_ERROR (T_("CListCtrl::GetNextItem"), 0);
	}
      pCmdUI->Enable (enable);
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

   MpmView::OnInstall
   _________________________________________________________________________ */

void MpmView::OnInstall ()
{
  try
    {
      vector<tstring> toBeInstalled;
      vector<tstring> toBeRemoved;
      int idx = -1;
      CListCtrl & listctrl = GetListCtrl();
      while ((idx = listctrl.GetNextItem(idx, LVNI_SELECTED)) >= 0)
	{
	  PackageInfo packageInfo = packages[listctrl.GetItemData(idx)];
	  if (packageInfo.timeInstalled == 0)
	    {
	      toBeInstalled.push_back (packageInfo.deploymentName);
	    }
	  else
	    {
	      toBeRemoved.push_back (packageInfo.deploymentName);
	    }
	}
      CHECK_WINDOWS_ERROR (T_("CListCtrl::GetNextItem"), 0);
      MIKTEX_ASSERT (toBeInstalled.size() > 0 || toBeRemoved.size() > 0);
      CString str1;
      str1.Format (T_("%u"), toBeInstalled.size());
      CString str2;
      str2.Format (T_("%u"), toBeRemoved.size());
      CString str;
      AfxFormatString2 (str, IDP_UPDATE_MESSAGE, str1, str2);
      if (IsWindowsVista())
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
		  return;
		}
	    }
	  else
	    {
	      UNEXPECTED_CONDITION (T_("MpmView::OnInstall"));
	    }
	}
      else
	{
	  if (AfxMessageBox(str, MB_OKCANCEL | MB_ICONINFORMATION) == IDCANCEL)
	    {
	      return;
	    }
	}
      UpdateDialog::DoModal (this,
			     pManager.Get(),
			     toBeInstalled,
			     toBeRemoved);
      OnResetView ();
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

   MpmView::OnProperties
   _________________________________________________________________________ */

void
MpmView::OnProperties ()
{
  try
    {
      CListCtrl & listctrl = GetListCtrl();
      int nItem = listctrl.GetNextItem(-1, LVNI_SELECTED);
      if (nItem < 0)
	{
	  CHECK_WINDOWS_ERROR (T_("CListCtrl::GetNextItem"), 0);
	  return;
	}
      PackageInfo packageInfo = packages[listctrl.GetItemData(nItem)];
      PropSheetPackage::DoModal (packageInfo, this);
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

   MpmView::OnUpdateProperties
   _________________________________________________________________________ */

void
MpmView::OnUpdateProperties (/*[in]*/ CCmdUI *	pCmdUI)
{
  CListCtrl & listctrl = GetListCtrl();
  pCmdUI->Enable (listctrl.GetSelectedCount() == 1);
}

/* _________________________________________________________________________

   MpmView::OnUninstal
   _________________________________________________________________________ */

void
MpmView::OnUninstall ()
{
  OnInstall ();
}

/* _________________________________________________________________________

   MpmView::OnUpdateUninstall
   _________________________________________________________________________ */

void
MpmView::OnUpdateUninstall (/*[in]*/ CCmdUI *	pCmdUI)
{
  try
    {
      BOOL enable;
      if (pSession->IsMiKTeXDirect())
	{
	  enable = FALSE;
	}
      else
	{
	  CListCtrl & listctrl = GetListCtrl();
	  int idx = -1;
	  enable = (listctrl.GetSelectedCount() > 0);
	  while (enable
		 && (idx = listctrl.GetNextItem(idx, LVNI_SELECTED)) >= 0)
	    {
	      PackageInfo packageInfo = packages[listctrl.GetItemData(idx)];
	      if (packageInfo.timeInstalled == 0)
		{
		  enable = FALSE;
		}
	    }
	  CHECK_WINDOWS_ERROR (T_("CListCtrl::GetNextItem"), 0);
	}
      pCmdUI->Enable (enable);
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

   MpmView::OnUpdateResetView
   _________________________________________________________________________ */

void
MpmView::OnUpdateResetView (/*[in]*/ CCmdUI *	pCmdUI)
{
  pCmdUI->Enable (TRUE);
}

/* _________________________________________________________________________

   MpmView::OnResetView
   _________________________________________________________________________ */

void
MpmView::OnResetView ()
{
  try
    {
      CWinApp * pApp = AfxGetApp();
      MIKTEX_ASSERT
	(pApp->GetMainWnd()->IsKindOf(RUNTIME_CLASS(MainFrame)));
      MainFrame * pMain =
	DYNAMIC_DOWNCAST(MainFrame, pApp->GetMainWnd());
      pMain->GetDlgBarItem(IDC_EDIT_PACKAGE_NAME)->SetWindowText(T_(""));
      CString searchWords;
      pMain->GetDlgBarItem(IDC_EDIT_WORDS)->SetWindowText(T_(""));
      CString strFileName;
      pMain->GetDlgBarItem(IDC_EDIT_FILE_NAME)->SetWindowText(T_(""));
      FillListView ();
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

   MpmView::OnSiteWizard
   _________________________________________________________________________ */

void
MpmView::OnSiteWizard ()
{
  try
    {
      if (SiteWizSheet::DoModal(this) == ID_WIZFINISH)
	{
	  OnUpdateDatabase ();
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

   MpmView::OnUpdateDatabase
   _________________________________________________________________________ */

void
MpmView::OnUpdateDatabase ()
{
  try
    {
      auto_ptr<PackageInstaller> pInstaller (pManager->CreateInstaller());
      pInstaller->UpdateDbAsync ();
      auto_ptr<ProgressDialog> pProgDlg (ProgressDialog::Create());
      pProgDlg->SetTitle (T_("Synchronize"));
      pProgDlg->SetLine (1, T_("Synchronizing the package database..."));
      pProgDlg->SetLine (2, T_(""));
      pProgDlg->StartProgressDialog (GetSafeHwnd());
      while (! pProgDlg->HasUserCancelled())
	{
	  PackageInstaller::ProgressInfo progressinfo =
	    pInstaller->GetProgressInfo();
	  if (progressinfo.ready)
	    {
	      break;
	    }
	  Sleep (1000);
	}
      pInstaller->Dispose ();
      if (pProgDlg->HasUserCancelled())
	{
	  pProgDlg->StopProgressDialog ();
	  return;
	}
      OnResetView ();
      pProgDlg->StopProgressDialog ();
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

   MpmView::OnSelectUpdateablePackages
   _________________________________________________________________________ */

void
MpmView::OnSelectUpdateablePackages ()
{
  // <todo/>
}

/* _________________________________________________________________________

   MpmView::OnSelectInstallablePackages
   _________________________________________________________________________ */

void
MpmView::OnSelectInstallablePackages ()
{
  try
    {
      CListCtrl & listctrl = GetListCtrl();
      for (map<LPARAM, PackageInfo>::const_iterator it =
	     packages.begin();
	   it != packages.end();
	   ++ it)
	{
	  const PackageInfo & packageInfo = it->second;
	  if (packageInfo.timeInstalled == 0)
	    {
	      LVFINDINFO fi;
	      fi.flags = LVFI_PARAM;
	      fi.lParam = it->first;
	      int idx = listctrl.FindItem(&fi);
	      if (idx < 0)
		{
		  FATAL_WINDOWS_ERROR (T_("CListCtrl::FindItem"), 0);
		}
	      if (! listctrl.SetItemState(idx, LVIS_SELECTED, LVIS_SELECTED))
		{
		  FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItemState"), 0);
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

   MpmView::OnUpdateSelectUpdateablePackages
   _________________________________________________________________________ */

void
MpmView::OnUpdateSelectUpdateablePackages (/*[in]*/ CCmdUI *	pCmdUI)
{
  // <todo/>
  pCmdUI->Enable (FALSE);
}

/* _________________________________________________________________________

   MpmView::OnSelectAll
   _________________________________________________________________________ */

void
MpmView::OnSelectAll ()
{
  try
    {
      CListCtrl & listctrl = GetListCtrl();
      int count = listctrl.GetItemCount();
      for (int idx = 0; idx < count; ++ idx)
	{
	  if (! listctrl.SetItemState(idx, LVIS_SELECTED, LVIS_SELECTED))
	    {
	      FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItemState"), 0);
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

   MpmView::OnUpdateWizard
   _________________________________________________________________________ */

void
MpmView::OnUpdateWizard ()
{
  try
    {
      PathName updateDat = pSession->GetSpecialPath(SpecialPath::InstallRoot);
      updateDat += MIKTEX_PATH_UPDATE_DAT;
      if (! File::Exists(updateDat))
	{
	  FATAL_MIKTEX_ERROR (T_("MpmView::OnUpdateWizard"),
			      T_("The update wizard could not be found."),
			      0);
	}
      PathName copystart;
      if (! pSession->FindFile(T_("copystart"),
			       FileType::EXE,
			       copystart))
	{
	  FATAL_MIKTEX_ERROR (T_("MpmView::OnUpdateWizard"),
			      T_("The update helper could not be found."),
			      0);
	}
      CommandLineBuilder arguments;
      arguments.AppendArgument (updateDat);
      if (! pSession->UnloadFilenameDatabase())
	{
	  FATAL_MIKTEX_ERROR
	    (T_("MpmView::OnUpdateWizard"),
	     T_("The file name database could not be closed."),
	     0);
	}
      Process::Start (copystart, arguments.Get());
      // <todo>close app</todo>
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

   MpmView::OnUpdateSiteWizard
   _________________________________________________________________________ */

void
MpmView::OnUpdateSiteWizard (/*[in]*/ CCmdUI * pCmdUI)
{
  pCmdUI->Enable (! pSession->IsMiKTeXDirect());
}

/* _________________________________________________________________________

   MpmView::OnUpdateUpdateDatabase
   _________________________________________________________________________ */

void
MpmView::OnUpdateUpdateDatabase (/*[in]*/ CCmdUI * pCmdUI)
{
  try
    {
      pCmdUI->Enable (! pSession->IsMiKTeXDirect());
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

   MpmView::OnUpdateUpdateWizard
   _________________________________________________________________________ */

void
MpmView::OnUpdateUpdateWizard (/*[in]*/ CCmdUI * pCmdUI)
{
  try
    {
      pCmdUI->Enable (! pSession->IsMiKTeXDirect());
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
