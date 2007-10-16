/* PackageTreeCtrl.cpp:

   Copyright (C) 2000-2006 Christian Schenk

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

#include <MiKTeX/UI/MFC/PackageTreeCtrl>
#include <MiKTeX/UI/MFC/PropSheetPackage>

#include "SearchPackageDialog.h"

#define ROOT_EXTERNAL_NAME T_("_miktex")

/* _________________________________________________________________________

   PackageTreeCtrlImpl
   _________________________________________________________________________ */

class PackageTreeCtrlImpl : public PackageTreeCtrl
{
public:
  PackageTreeCtrlImpl ();

public:
  PackageTreeCtrlImpl (/*[in]*/ PackageManager * pManager);

public:
  virtual
  void
  Refresh ();

public:
  virtual
  bool
  GetPackageInfo (/*[in]*/ HTREEITEM		hItem,
		  /*[out]*/ PackageInfo &	packageInfo);

public:
  virtual
  void
  GetDelta (/*[out]*/ vector<tstring> & toBeInstalled,
	    /*[out]*/ vector<tstring> & toBeRemoved);

protected:
  afx_msg
  void
  OnLButtonDblClk (/*[in]*/ UINT	nFlags,
		   /*[in]*/ CPoint	point);

protected:
  afx_msg
  void
  OnLButtonDown (/*[in]*/ UINT		nFlags,
		 /*[in]*/ CPoint	point);

protected:
  afx_msg
  void
  OnRButtonDown (/*[in]*/ UINT		nFlags,
		 /*[in]*/ CPoint	point);

protected:
  DECLARE_MESSAGE_MAP();

private:
  enum ITEMSTATE {
    ITEMSTATE_NONE = 0,
    ITEMSTATE_UNCHECKED,
    ITEMSTATE_CHECKED,
    ITEMSTATE_UNDETERMINED,
  };

private:
  void
  AddPackage (/*[in]*/ HTREEITEM		hParent,
	      /*[in]*/ const PackageInfo &	packageInfo,
	      /*[in]*/ LPARAM			lvl);

private:
  void
  OnItemClicked (/*[in]*/ HTREEITEM hItem);

private:
  void
  PropagateState (/*[in]*/ HTREEITEM	hItem,
		  /*[in]*/ DWORD	dwState,
		  /*[in]*/ bool		fSucc = true,
		  /*[in]*/ bool		fPred = true);

private:
  DWORD
  DetermineState (/*[in]*/ HTREEITEM hItem);

private:
  bool
  DoContextMenu (/*[in]*/ CPoint	point,
		 /*[in]*/ HTREEITEM	hItem);

private:
  map<HTREEITEM, tstring> packageNames;

private:
  multimap<tstring, HTREEITEM> treeItems;

private:
  map<HTREEITEM, DWORD> treeItemStates;

private:
  CImageList imageList;

private:
  PackageManagerPtr pManager;
};

/* _________________________________________________________________________

   PackageTreeCtrlImpl::PackageTreeCtrlImpl
   _________________________________________________________________________ */

PackageTreeCtrlImpl::PackageTreeCtrlImpl ()
{
}

/* _________________________________________________________________________

   PackageTreeCtrlImpl::PackageTreeCtrlImpl
   _________________________________________________________________________ */

PackageTreeCtrlImpl::PackageTreeCtrlImpl (/*[in]*/ PackageManager * pManager)
  : pManager (pManager)
{
}

/* _________________________________________________________________________

   PackageTreeCtrlImpl::DetermineState
   _________________________________________________________________________ */

DWORD
PackageTreeCtrlImpl::DetermineState (/*[in]*/ HTREEITEM hItem)
{
  HTREEITEM hChild = GetChildItem(hItem);
  if (hChild == 0)
    {
      return (treeItemStates[hItem]);
    }
  DWORD treeItemState = ITEMSTATE_NONE;
  do
    {
      // <recursivecall>
      DWORD treeItemState2 = DetermineState(hChild);
      // </recursivecall>
      MIKTEX_ASSERT (treeItemState2 >= ITEMSTATE_UNCHECKED
		     && treeItemState2 <= ITEMSTATE_UNDETERMINED);
      if (treeItemState == ITEMSTATE_NONE)
	{
	  treeItemState = treeItemState2;
	}
      else if (treeItemState != treeItemState2)
	{
	  treeItemState = ITEMSTATE_UNDETERMINED;
	}
    }
  while ((hChild = GetNextItem(hChild, TVGN_NEXT)) != 0);
  DWORD oldTreeItemState = treeItemStates[hItem];
  if (oldTreeItemState != treeItemState)
    {
      if (! SetItemState(hItem,
			 INDEXTOSTATEIMAGEMASK(treeItemState),
			 TVIS_STATEIMAGEMASK))
	{
	  FATAL_WINDOWS_ERROR (T_("CTreeCtrl::SetItemState"), 0);
	}
      treeItemStates[hItem] = treeItemState;
    }
  return (treeItemState);
}

/* _________________________________________________________________________

   PackageTreeCtrlImpl::Refresh
   _________________________________________________________________________ */

void
PackageTreeCtrlImpl::Refresh ()
{
  // create the image list
  if (imageList.m_hImageList == 0)
    {
      if (! imageList.Create(IDB_PACKAGE_STATES,
			     13,
			     1,
			     RGB(255, 255, 255)))
	{
	  FATAL_WINDOWS_ERROR (T_("CImageList::Create"), 0);
	}
      SetImageList (&imageList, TVSIL_STATE);
    }

  // cut down the tree
  if (! DeleteAllItems())
    {
      FATAL_WINDOWS_ERROR (T_("CTreeCtrl::DeleteAllItems"), 0);
    }

  // clear maps
  packageNames.clear ();
  treeItems.clear ();

  // create the root item
  PackageInfo rootPackageInfo;
  rootPackageInfo.deploymentName = ROOT_EXTERNAL_NAME;
  rootPackageInfo.displayName = T_("MiKTeX Packages");
  TVINSERTSTRUCT ins;
  ins.hParent = TVI_ROOT;
  ins.hInsertAfter = TVI_ROOT;
  ins.item.mask = TVIF_PARAM | TVIF_TEXT;
  ins.item.pszText =
    const_cast<MIKTEXCHAR *>(rootPackageInfo.displayName.c_str());
  ins.item.lParam = 0;
  HTREEITEM hRoot = InsertItem(&ins);
  if (hRoot == 0)
    {
      FATAL_WINDOWS_ERROR (T_("CTreeCtrl::InsertItem"), 0);
    }
  packageNames[hRoot] = rootPackageInfo.deploymentName;
  treeItems.insert
    (make_pair<tstring, HTREEITEM>(rootPackageInfo.deploymentName,
				   hRoot));
  treeItemStates[hRoot] = ITEMSTATE_UNDETERMINED;

  // let the tree grow
  auto_ptr<PackageIterator> pIter (pManager->CreateIterator());
  pIter->AddFilter (PackageFilter::Top);
  PackageInfo packageInfo;
  while (pIter->GetNext(packageInfo))
    {
      AddPackage (hRoot, packageInfo, 1);
    }
  pIter->Dispose ();

  // set the states
  DetermineState (hRoot);

  Expand (hRoot, TVE_EXPAND);
}

/* _________________________________________________________________________

   PackageTreeCtrlImpl::AddPackage
   _________________________________________________________________________ */

void
PackageTreeCtrlImpl::AddPackage (/*[in]*/ HTREEITEM		hParent,
				 /*[in]*/ const PackageInfo &	packageInfo,
				 /*[in]*/ LPARAM		lvl)
{
  // insert this package
  TVINSERTSTRUCT ins;
  ins.hParent = hParent;
  ins.hInsertAfter = TVI_SORT;
  ins.item.mask = TVIF_PARAM | TVIF_STATE | TVIF_TEXT;
  ITEMSTATE state =
    (packageInfo.IsInstalled()
     ? ITEMSTATE_CHECKED
     : ITEMSTATE_UNCHECKED);
  ins.item.state = INDEXTOSTATEIMAGEMASK(state);
  ins.item.stateMask = TVIS_STATEIMAGEMASK;
  MIKTEX_ASSERT (packageInfo.displayName.length() > 0);
  ins.item.pszText = const_cast<MIKTEXCHAR *>(packageInfo.displayName.c_str());
  ins.item.lParam = lvl;
  HTREEITEM hItem = InsertItem(&ins);
  if (hItem == 0)
    {
      FATAL_WINDOWS_ERROR (T_("CTreeCtrl::InsertItem"), 0);
    }
  packageNames[hItem] = packageInfo.deploymentName;
  treeItems.insert (make_pair<tstring, HTREEITEM>(packageInfo.deploymentName,
						  hItem));
  treeItemStates[hItem] = state;
  
  // insert sub-packages
  for (vector<tstring>::const_iterator
	 it = packageInfo.requiredPackages.begin();
       it != packageInfo.requiredPackages.end();
       ++ it)
    {
      PackageInfo packageInfo2 = pManager->GetPackageInfo(it->c_str());
      AddPackage (hItem, packageInfo2, lvl + 1);
    }
}

/* _________________________________________________________________________

   PackageTreeCtrlImpl::PropagateState
   _________________________________________________________________________ */

void
PackageTreeCtrlImpl::PropagateState (/*[in]*/ HTREEITEM	hItem,
				     /*[in]*/ DWORD	treeItemState,
				     /*[in]*/ bool	fSucc,
				     /*[in]*/ bool	fPred)
{
  // determine the state of this item, if treeItemState is undefined
  if (treeItemState == ITEMSTATE_NONE)
    {
      MIKTEX_ASSERT (! fSucc);
      for (HTREEITEM hChild = GetChildItem(hItem);
	   hChild != 0 && treeItemState != ITEMSTATE_UNDETERMINED;
	   hChild = GetNextItem(hChild, TVGN_NEXT))
	{
	  DWORD treeItemState2 = treeItemStates[hChild];
	  MIKTEX_ASSERT (treeItemState2 >= ITEMSTATE_UNCHECKED
			 && treeItemState2 <= ITEMSTATE_UNDETERMINED);
	  if (treeItemState == ITEMSTATE_NONE)
	    {
	      treeItemState = treeItemState2;
	    }
	  else if (treeItemState != treeItemState2)
	    {
	      treeItemState = ITEMSTATE_UNDETERMINED;
	    }
	}
    }

  MIKTEX_ASSERT (treeItemState != ITEMSTATE_NONE);

  // change the state of this item and all twin items
  tstring displayName =  packageNames[hItem];
  multimap<tstring, HTREEITEM>::const_iterator it =
    treeItems.find(displayName);
  MIKTEX_ASSERT (it != treeItems.end());
  for (; it != treeItems.end() && it->first == displayName; ++it)
    {
      DWORD oldTreeItemState = treeItemStates[it->second];
      if (oldTreeItemState != treeItemState)
	{
	  if (! SetItemState(it->second,
			     INDEXTOSTATEIMAGEMASK(treeItemState),
			     TVIS_STATEIMAGEMASK))
	    {
	      FATAL_WINDOWS_ERROR (T_("CTreeCtrl::SetItemState"), 0);
	    }
	  treeItemStates[it->second] = treeItemState;
	}

      // change the state of all successors
      if (fSucc)
	{
	  for (HTREEITEM hChild = GetChildItem(it->second);
	       hChild != 0;
	       hChild = GetNextItem(hChild, TVGN_NEXT))
	    {
	      PropagateState (hChild, treeItemState, true, false);
	    }
	}
      
      // determine the state of all predecessors
      if (fPred)
	{
	  HTREEITEM hParent = GetParentItem(it->second);
	  if (hParent != 0 && hParent != GetRootItem())
	    {
	      PropagateState (hParent, ITEMSTATE_NONE, false, true);
	    }
	}
    }
}

/* _________________________________________________________________________

   PackageTreeCtrlImpl::OnItemClicked
   _________________________________________________________________________ */

void
PackageTreeCtrlImpl::OnItemClicked (/*[in]*/ HTREEITEM hItem)
{
  try
    {
      // get the state of the item
      DWORD treeItemState = treeItemStates[hItem];
      MIKTEX_ASSERT (treeItemState >= ITEMSTATE_UNCHECKED
		     && treeItemState <= ITEMSTATE_UNDETERMINED);

      // toggle state
      treeItemState =
	(treeItemState == ITEMSTATE_CHECKED
	 ? ITEMSTATE_UNCHECKED
	 : ITEMSTATE_CHECKED);
      
      // disallow package removal
      //   if this is the root item
      //   if running from CD
      //   if this is a container
      //   if the reference count is greater than 1
      if (treeItemState == ITEMSTATE_UNCHECKED)
	{
	  if (hItem == GetRootItem())
	    {
	      return;
	    }
	  if (SessionWrapper(true)->IsMiKTeXDirect())
	    {
	      return;
	    }
	  PackageInfo packageInfo;
	  GetPackageInfo(hItem, packageInfo);
	  if (packageInfo.IsContainer() || packageInfo.GetRefCount() > 1)
	    {
	      return;
	    }
	}
      
      // change the state
      PropagateState (hItem, treeItemState);
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

   PackageTreeCtrlImpl::OnLButtonDown
   _________________________________________________________________________ */

void
PackageTreeCtrlImpl::OnLButtonDown (/*[in]*/ UINT	nFlags,
				    /*[in]*/ CPoint	point)
{
  try
    {
      // check to see if the button was clicked
      UINT flags = 0;
      HTREEITEM hItem = HitTest(point, &flags);
      if (hItem == 0 || (flags & TVHT_ONITEMSTATEICON) == 0)
	{
	  CTreeCtrl::OnLButtonDown (nFlags, point);
	  return;
	}
      
      CWaitCursor cur;
      
      // set the state of the item
      OnItemClicked (hItem);
      
      // notify parent window
      CWnd * pParent = GetParent();
      MIKTEX_ASSERT (pParent != 0);
      if (pParent != 0)
	{
	  GetParent()->SendMessage (WM_ONTOGGLE);
	}
      
      CTreeCtrl::OnLButtonDown (nFlags, point);
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

   PackageTreeCtrlImpl::GetPackageInfo
   _________________________________________________________________________ */

bool
PackageTreeCtrlImpl::GetPackageInfo (/*[in]*/ HTREEITEM		hItem,
				     /*[out]*/ PackageInfo &	packageInfo)
{
  if (GetItemData(hItem) == 0)
    {
      return (false);
    }
  packageInfo = pManager->GetPackageInfo(packageNames[hItem].c_str());
  return (true);
}

/* _________________________________________________________________________

   PackageTreeCtrlImpl Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(PackageTreeCtrlImpl, CTreeCtrl)
  ON_WM_LBUTTONDBLCLK()
  ON_WM_LBUTTONDOWN()
  ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP();

/* _________________________________________________________________________

   PackageTreeCtrlImpl::OnRButtonDown
   _________________________________________________________________________ */

void
PackageTreeCtrlImpl::OnRButtonDown (/*[in]*/ UINT	nFlags,
				    /*[in]*/ CPoint	point)
{
  try
    {
      UINT flags;
      HTREEITEM hItem = HitTest(point, &flags);
      if (hItem == 0 || (flags & TVHT_ONITEM) == 0)
	{
	  CTreeCtrl::OnRButtonDown (nFlags, point);
	  return;
	}
      HTREEITEM hItemSelected = GetSelectedItem();
      if (! Select(hItem, TVGN_CARET))
	{
	  FATAL_WINDOWS_ERROR (T_("CTreeCtrl::Select"), 0);
	}
      if (! DoContextMenu(point, hItem) && hItemSelected != 0)
	{
	  if (! Select(hItemSelected, TVGN_CARET))
	    {
	      FATAL_WINDOWS_ERROR (T_("CTreeCtrl::Select"), 0);
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

   PackageTreeCtrlImpl::DoContextMenu
   _________________________________________________________________________ */

bool
PackageTreeCtrlImpl::DoContextMenu (/*[in]*/ CPoint	point,
				    /*[in]*/ HTREEITEM	hItem)
{
  CMenu menu;
  if (! menu.LoadMenu(ID_PACKAGE_MENU))
    {
      FATAL_WINDOWS_ERROR (T_("CMenu::LoadMenu"), 0);
    }
  CMenu * pPopup = menu.GetSubMenu(0);
  if (pPopup == 0)
    {
      UNEXPECTED_CONDITION (T_("PackageTreeCtrlImpl::DoContextMenu"));
    }
  PackageInfo packageInfo;
  if (hItem == GetRootItem())
    {
      if (pPopup->EnableMenuItem(ID_PROPERTIES, MF_GRAYED)
	  == static_cast<UINT>(-1))
	{
	  FATAL_WINDOWS_ERROR (T_("CMenu::EnableItem"), 0);
	}
    }
  else
    {
      GetPackageInfo (hItem, packageInfo);
      if (! packageInfo.IsPureContainer())
	{
	  if (pPopup->EnableMenuItem(ID_SEARCH_PACKAGE, MF_GRAYED)
	      == static_cast<UINT>(-1))
	    {
	      FATAL_WINDOWS_ERROR (T_("CMenu::EnableItem"), 0);
	    }
	}
    }
  ClientToScreen (&point);
  UINT cmd =
    TrackPopupMenu(pPopup->GetSafeHmenu(),
		   (0
		    | TPM_LEFTALIGN
		    | TPM_RIGHTBUTTON
		    | TPM_NONOTIFY
		    | TPM_RETURNCMD
		    | TPM_TOPALIGN
		    | 0),
		   point.x,
		   point.y,
		   0,
		   GetSafeHwnd(),
		   0);
  if (cmd == 0)
    {
      if (::GetLastError() != 0)
	{
	  FATAL_WINDOWS_ERROR (T_("TrackPopupMenu"), 0);
	}
      return (false);
    }
  switch (cmd)
    {
    case ID_PROPERTIES:
      {
	PropSheetPackage::DoModal (packageInfo, this->GetParent());
	break;
      }
    case ID_SEARCH_PACKAGE:
      {
	SearchPackageDialog dlg (this->GetParent(), pManager.Get());
	if (dlg.DoModal() != IDOK)
	  {
	    break;
	  }
	auto_ptr<PackageIterator> pIter (pManager->CreateIterator());
	PackageInfo packageInfo;
	while (pIter->GetNext(packageInfo))
	  {
	    if (_tcsicmp(packageInfo.displayName.c_str(),
			 dlg.GetName())
		== 0)
	      {
		multimap<tstring, HTREEITEM>::const_iterator
		  it = treeItems.find(packageInfo.deploymentName);
		MIKTEX_ASSERT (it != treeItems.end());
		if (it != treeItems.end())
		  {
		    if (! Select(it->second, TVGN_CARET))
		      {
			FATAL_WINDOWS_ERROR (T_("CTreeCtrl::Select"), 0);
		      }
		    return (true);
		  }
		break;
	      }
	  }
	pIter->Dispose ();
	break;
      }
    }
  return (false);
}

/* _________________________________________________________________________

   PackageTreeCtrlImpl::GetDelta
   _________________________________________________________________________ */

void
PackageTreeCtrlImpl::GetDelta (/*[out]*/ vector<tstring> &	toBeInstalled,
			       /*[out]*/ vector<tstring> &	toBeRemoved)
{
  for (map<HTREEITEM, tstring>::const_iterator
	 it = packageNames.begin();
       it != packageNames.end();
       ++ it)
    {
      if (it->second == ROOT_EXTERNAL_NAME)
	{
	  continue;
	}
      DWORD treeItemState = treeItemStates[it->first];
      PackageInfo packageInfo = pManager->GetPackageInfo(it->second.c_str());
      if (treeItemState == ITEMSTATE_UNCHECKED && packageInfo.IsInstalled())
	{
	  if (! (packageInfo.IsPureContainer()
		 || (find(toBeRemoved.begin(), toBeRemoved.end(),
			  packageInfo.deploymentName)
		     != toBeRemoved.end())))
	    {
	      toBeRemoved.push_back (packageInfo.deploymentName);
	    }
	}
      else if (treeItemState == ITEMSTATE_CHECKED
	       && ! packageInfo.IsInstalled())
	{
	  if (! (packageInfo.IsPureContainer()
		 || (find(toBeInstalled.begin(),
			  toBeInstalled.end(),
			  packageInfo.deploymentName)
		     != toBeInstalled.end())))
	    {
	      toBeInstalled.push_back (packageInfo.deploymentName);
	    }
	}
    }
}

/* _________________________________________________________________________

   PackageTreeCtrlImpl::OnLButtonDblClk
   _________________________________________________________________________ */

void
PackageTreeCtrlImpl::OnLButtonDblClk (/*[in]*/ UINT	nFlags,
				      /*[in]*/ CPoint	point)
{
  try
    {
      // check to see if the button was clicked
      UINT flags = 0;
      HTREEITEM hItem = HitTest(point, &flags);
      if (hItem == 0 || (flags & TVHT_ONITEM) == 0)
	{
	  return;
	}
      
      if (ItemHasChildren(hItem))
	{
	  CTreeCtrl::OnLButtonDblClk (nFlags, point);
	  return;
	}
  
      // "open" the package
      PackageInfo packageInfo;
      GetPackageInfo (hItem, packageInfo);
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

/* _________________________________________________________________________

   PackageTreeCtrl::Create
   _________________________________________________________________________ */

PackageTreeCtrl *
PackageTreeCtrl::Create (/*[in]*/ PackageManager * pManager)
{
  return new PackageTreeCtrlImpl (pManager);
}

/* _________________________________________________________________________

   PackageTreeCtrl:Destroy
   _________________________________________________________________________ */

void
PackageTreeCtrl::Destroy (/*[in]*/ PackageTreeCtrl * pCtrl)
{
  delete pCtrl;
}
