/* PackageListPag.cpp:

   Copyright (C) 2002-2011 Christian Schenk

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

#include "PackageListPage.h"
#include "UpdateWizard.h"
#include "resource.h"

IMPLEMENT_DYNCREATE(PackageListPage, CPropertyPage);

const unsigned int WM_FILL_LIST = WM_APP + 1;

/* _________________________________________________________________________

   PackageListPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(PackageListPage, CPropertyPage)
  ON_BN_CLICKED(IDC_DESELECT_ALL, OnDeselectAll)
  ON_BN_CLICKED(IDC_SELECT_ALL, OnSelectAll)
  ON_MESSAGE(WM_FILL_LIST, OnFillList)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemChanged)
  ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST, OnItemChanging)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   PackageListPage::PackageListPage
   _________________________________________________________________________ */

PackageListPage::PackageListPage ()
  : CPropertyPage (IDD,
		   0,
		   IDS_HEADER_PACKAGE_LIST,
		   IDS_SUBHEADER_PACKAGE_LIST),
    fillingTheListView (false),
    ready (false),
    hWorkerThread (0),
    pSheet (0)
{
}

/* _________________________________________________________________________

   PackageListPage::~PackageListPage
   _________________________________________________________________________ */

PackageListPage::~PackageListPage ()
{
  try
  {
    if (hWorkerThread != 0)
    {
      CloseHandle (hWorkerThread);
      hWorkerThread = 0;
    }
    if (pInstaller.get() != 0)
    {
      pInstaller->Dispose ();
      pInstaller.reset ();
    }
  }
  catch (const exception &)
  {
  }
}

/* _________________________________________________________________________

   PackageListPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
PackageListPage::OnInitDialog ()
{
  pSheet = reinterpret_cast<UpdateWizard*>(GetParent());
  ASSERT_KINDOF (UpdateWizard, pSheet);  

  BOOL ret = CPropertyPage::OnInitDialog();

  try
  {
    MIKTEX_ASSERT (pInstaller.get() == 0);
    pInstaller = auto_ptr<PackageInstaller>(g_pManager->CreateInstaller());
    listControl.SetExtendedStyle (listControl.GetExtendedStyle()
      | LVS_EX_CHECKBOXES);
    InsertColumn (0, T_("Name"), T_("xxxx yet another package"));
    InsertColumn (1, T_("Old"), T_("xxxx 19-December-2000"));
    InsertColumn (2, T_("New"), T_("xxxx 19-December-2000"));
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

   PackageListPage::OnSetActive
   _________________________________________________________________________ */

BOOL
PackageListPage::OnSetActive ()
{
  BOOL ret = CPropertyPage::OnSetActive();

  if (ret)
  {
    try
    {
      pSheet->ClearErrorFlag ();

      EnableSelectButtons ();

      string repository = pSheet->GetRepository();

      // start search thread, if this is the first activation or
      // if the package repository URL has changed; otherwise fill
      // the list control with the information we have
      if (! ready || repository != this->repository)
      {
	ready = false;
	this->repository = repository;

	// a kind of progress info
	SetProgressText (T_("Searching..."));

	// disable wizard buttons
	pSheet->SetWizardButtons (0);

	// start thread
	CWinThread * pThread =
	  AfxBeginThread (WorkerThread,
			  this,
			  THREAD_PRIORITY_NORMAL,
			  0,
			  CREATE_SUSPENDED);
	MIKTEX_ASSERT (pThread != 0);
	MIKTEX_ASSERT (pThread->m_hThread != 0);
	if (! DuplicateHandle(GetCurrentProcess(),
			      pThread->m_hThread,
			      GetCurrentProcess(),
			      &hWorkerThread,
			      0,
			      FALSE,
			      DUPLICATE_SAME_ACCESS))
	{
	  FATAL_WINDOWS_ERROR ("DuplicateHandle", 0);
	}
	pThread->ResumeThread ();
      }
      else
      {
	OnFillList (0, 0);
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

/* _________________________________________________________________________

   PackageListPage::DoDataExchange
   _________________________________________________________________________ */

void
PackageListPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Control(pDX, IDC_LIST, listControl);
}

/* _________________________________________________________________________

   PackageListPage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
PackageListPage::OnWizardNext ()
{
  pSheet->SetCameFrom (IDD);
  return (0);
}

/* _________________________________________________________________________

   PackageListPage::OnWizardBack
   _________________________________________________________________________ */

LRESULT
PackageListPage::OnWizardBack ()
{
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(pSheet->GetCameFrom())));
}

/* _________________________________________________________________________

   PackageListPage::OnKillActive
   _________________________________________________________________________ */

BOOL
PackageListPage::OnKillActive ()
{
  BOOL ret = CPropertyPage::OnKillActive();
  if (ret)
  {
    vector<string> toBeUpdated;
    toBeUpdated.reserve (updates.size());
    vector<string> toBeRemoved;
    toBeRemoved.reserve (updates.size());
    int n = listControl.GetItemCount();
    for (int i = 0; i < n; ++ i)
    {
      size_t idx = listControl.GetItemData(i);
      if (listControl.GetCheck(i))
      {
	switch (updates[idx].action)
	{
	case PackageInstaller::UpdateInfo::Repair:
	case PackageInstaller::UpdateInfo::Update:
	case PackageInstaller::UpdateInfo::ForceUpdate:
	  toBeUpdated.push_back (updates[idx].deploymentName);
	  break;
	case PackageInstaller::UpdateInfo::ForceRemove:
	  toBeRemoved.push_back (updates[idx].deploymentName);
	  break;
	}
      }
    }
    pSheet->SetUpdateList (toBeUpdated);
    pSheet->SetRemoveList (toBeRemoved);
  }
  return (ret);
}

/* _________________________________________________________________________

   PackageListPage::OnQueryCancel
   _________________________________________________________________________ */

BOOL
PackageListPage::OnQueryCancel ()
{
  if (ready)
  {
    return (TRUE);
  }
  try
  {
    pSheet->SetCancelFlag ();
    SetProgressText (T_("Cancelling..."));
  }
  catch (const MiKTeXException & e)
  {
    pSheet->ReportError (e);
  }
  catch (const exception & e)
  {
    pSheet->ReportError (e);
  }
  return (FALSE);
}

/* _________________________________________________________________________

   PackageListPage::OnFillList

   Fill the list control.
   _________________________________________________________________________ */

LRESULT
PackageListPage::OnFillList (/*[in]*/ WPARAM		wParam,
			     /*[in]*/ LPARAM		lParam)
{
  UNUSED_ALWAYS (wParam);
  UNUSED_ALWAYS (lParam);

  try
  {
    if (pSheet->GetCancelFlag() || pSheet->GetErrorFlag())
    {
      SetProgressText (pSheet->GetCancelFlag()
	? T_("Cancelled")
	: T_("Unsuccessful"));
      pSheet->EnableCloseButton ();
      return (0);
    }

    CWaitCursor wait;

    AUTO_TOGGLE (fillingTheListView);

    if (! listControl.DeleteAllItems())
    {
      FATAL_WINDOWS_ERROR ("CListCtrl::DeleteAllItems", 0);
    }

    int idx = 0;

    bool somethingChecked = false;

    for (vector<PackageInstaller::UpdateInfo>::const_iterator it
	  = updates.begin();
	 it != updates.end();
	 ++ it, ++ idx)
    {
      LV_ITEM lvitem;

      lvitem.iItem = idx;
      lvitem.mask = LVIF_PARAM;
      lvitem.lParam = idx;
      lvitem.iSubItem = 0;

      if (listControl.InsertItem(&lvitem) < 0)
      {
	FATAL_WINDOWS_ERROR ("CListCtrl::InsertItem", 0);
      }

      // try to get the package info
      // <fixme>we need someting like IsKnownPackage()
      PackageInfo oldPackageInfo;
      bool locallyKnown;
      try
      {
	oldPackageInfo =
	  g_pManager->GetPackageInfo(it->deploymentName.c_str());
	locallyKnown = true;
      }
      catch (const MiKTeXException &)
      {
	locallyKnown = false;
      }
      // </fixme>

      // display the deployment name
      listControl.SetItemText (idx, 0, UT_(it->deploymentName.c_str()));

      // display the 'old' package time-stamp, if the package is known
      if (locallyKnown && oldPackageInfo.timeInstalled > 0)
      {
	CTime timeOld (oldPackageInfo.timePackaged);
	CString strOld = timeOld.Format("%d-%b-%y");
	listControl.SetItemText (idx, 1, strOld);
      }

      // display the 'new' package time-stamp, if the package is
      // correctly installed
      if (it->action == PackageInstaller::UpdateInfo::Repair)
      {
	listControl.SetItemText (idx, 2, T_(_T("to be repaired")));
      }
      else if (it->action == PackageInstaller::UpdateInfo::ForceRemove
	       || it->action == PackageInstaller::UpdateInfo::KeepObsolete)
      {
	listControl.SetItemText (idx, 2, T_(_T("obsolete (to be removed)")));
      }
      else
      {
	CTime timeNew (it->timePackaged);
	CString strNew = timeNew.Format ("%d-%b-%y");
	listControl.SetItemText (idx, 2, strNew);
      }

      bool toBeChecked = false;

      if (g_upgrading)
      {
	toBeChecked = true;
      }

      if (it->action == PackageInstaller::UpdateInfo::ForceUpdate
	  || it->action == PackageInstaller::UpdateInfo::ForceRemove
	  || it->action == PackageInstaller::UpdateInfo::Update
	  || it->action == PackageInstaller::UpdateInfo::Repair)
      {
	toBeChecked = true;
      }

      if (toBeChecked)
      {
	listControl.SetCheck (idx);
	somethingChecked = true;
      }
    }

    EnableSelectButtons ();

    // disable Next, if no package is selected
    pSheet->SetWizardButtons (PSWIZB_BACK | (somethingChecked ? PSWIZB_NEXT : 0));

#if 1
    RepositoryInfo repositoryInfo;
    if (g_pManager->TryGetRepositoryInfo(repository, repositoryInfo))
    {
      struct tm * pTime1 = gmtime(&repositoryInfo.timeDate);
      if (pTime1 == 0)
      {
	UNEXPECTED_CONDITION ("PackageListPage::OnFillList");
      }
      pTime1->tm_hour = 23;
      pTime1->tm_min = 59;
      pTime1->tm_sec = 59;
      if (difftime(time(0), mktime(pTime1)) < 60 * 60 * 30)
      {
	AfxMessageBox (T_(_T("The repository contains untestet packages. It is recommended that you \
wait a few days before updating.")),
          MB_OK | MB_ICONEXCLAMATION);
      }
    }
#endif


  }
  catch (const MiKTeXException & e)
  {
    pSheet->ReportError (e);
  }
  catch (const exception & e)
  {
    pSheet->ReportError (e);
  }

  return (0);
}

/* _________________________________________________________________________

   PackageListPage::OnSelectAll
   _________________________________________________________________________ */

void
PackageListPage::OnSelectAll ()
{
  int n = listControl.GetItemCount();
  for (int i = 0; i < n; ++ i)
  {
    listControl.SetCheck (i, TRUE);
  }
}

/* _________________________________________________________________________

   PackageListPage::OnDeselectAll
   _________________________________________________________________________ */

void
PackageListPage::OnDeselectAll ()
{
  int n = listControl.GetItemCount();
  for (int i = 0; i < n; ++ i)
  {
    listControl.SetCheck (i, FALSE);
  }
}

/* _________________________________________________________________________

   PackageListPage::OnItemChanging
   _________________________________________________________________________ */

void
PackageListPage::OnItemChanging (/*[in]*/ NMHDR *	pNMHDR,
				 /*[in]*/ LRESULT *	pResult)
{
  LPNMLISTVIEW pnmlv = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

  if (fillingTheListView
      || pnmlv == 0
      || pnmlv->iItem < 0
      || pnmlv->iItem >= static_cast<int>(updates.size()))
  {
    *pResult = FALSE;
    return;
  }

  const PackageInstaller::UpdateInfo & updateInfo = updates[pnmlv->iItem];

  if (g_upgrading
      && IsMiKTeXPackage(updateInfo.deploymentName))
  {
    AfxMessageBox (T_(_T("The MiKTeX upgrade operation must be executed \
as a whole.")),
		   MB_OK | MB_ICONEXCLAMATION);
    *pResult = TRUE;
  }
  else if (updateInfo.action == PackageInstaller::UpdateInfo::KeepAdmin)
  {
    AfxMessageBox (T_(_T("This package is currently installed for all users. Only the admin variant of the wizard can update this package.")),
		   MB_OK | MB_ICONEXCLAMATION);
    *pResult = TRUE;
  }
  else if (updateInfo.action == PackageInstaller::UpdateInfo::KeepObsolete)
  {
    AfxMessageBox (T_(_T("This package is currently installed for all users. Only the admin variant of the wizard can remove this package.")),
		   MB_OK | MB_ICONEXCLAMATION);
    *pResult = TRUE;
  }
  else if (updateInfo.action == PackageInstaller::UpdateInfo::Keep)
  {
    AfxMessageBox (T_(_T("This package cannot be updated rihgt now. \
Let the wizard conclude. Then run the wizard again.")),
		   MB_OK | MB_ICONEXCLAMATION);
    *pResult = TRUE;
  }
  else if (updateInfo.action == PackageInstaller::UpdateInfo::Repair)
  {
    AfxMessageBox (T_(_T("This package needs to be repaired.")),
		   MB_OK | MB_ICONEXCLAMATION);
    *pResult = TRUE;
  }
  else if (updateInfo.action == PackageInstaller::UpdateInfo::ForceRemove)
  {
    AfxMessageBox (T_(_T("This package must be removed.")),
		   MB_OK | MB_ICONEXCLAMATION);
    *pResult = TRUE;
  }
  else if (updateInfo.action == PackageInstaller::UpdateInfo::ForceUpdate)
  {
    AfxMessageBox (T_(_T("This package has to be updated.")),
		   MB_OK | MB_ICONEXCLAMATION);
    *pResult = TRUE;
  }
  else
  {
    *pResult = FALSE;
  }
}

/* _________________________________________________________________________

   PackageListPage::OnItemChanged
   _________________________________________________________________________ */

void
PackageListPage::OnItemChanged (/*[in]*/ NMHDR *	pNMHDR,
				/*[in]*/ LRESULT *	pResult)
{
  UNUSED_ALWAYS (pNMHDR);
  *pResult = 0;
  if (fillingTheListView || pSheet->GetCancelFlag() || pSheet->GetErrorFlag())
  {
    return;
  }
  bool somethingChecked = false;
  int n = listControl.GetItemCount();
  for (int i = 0; ! somethingChecked && i < n; ++ i)
  {
    if (listControl.GetCheck(i))
    {
      somethingChecked = true;
    }
  }
  pSheet->SetWizardButtons (PSWIZB_BACK
			    | (somethingChecked ? PSWIZB_NEXT : 0));
}

/* _________________________________________________________________________

   PackageListPage::WorkerThread
   _________________________________________________________________________ */

UINT
PackageListPage::WorkerThread (/*[in]*/ void * pv)
{
  PackageListPage * This = reinterpret_cast<PackageListPage*>(pv);

  try
  {
    This->DoFindUpdates ();
  }
  catch (const OperationCancelledException &)
  {
  }
  catch (const MiKTeXException & e)
  {
    This->pSheet->ReportError (e);
  }
  catch (const exception & e)
  {
    This->pSheet->ReportError (e);
  }

  try
  {
    if (! This->PostMessage(WM_FILL_LIST))
    {
      FATAL_WINDOWS_ERROR ("CWnd::PostMessage", 0);
    }
  }
  catch (const MiKTeXException & e)
  {
    This->pSheet->ReportError (e);
  }
  catch (const exception & e)
  {
    This->pSheet->ReportError (e);
  }

  This->ready = true;

  return (0);
}

/* _________________________________________________________________________

   UpdateInfoComparer
   _________________________________________________________________________ */

class UpdateInfoComparer
{
public:
  bool
  operator() (/*[in]*/ const PackageInstaller::UpdateInfo & ui1,
	      /*[in]*/ const PackageInstaller::UpdateInfo & ui2)
    const
  {
    return (PathName::Compare(ui1.deploymentName, ui2.deploymentName) < 0);
  }
};

/* _________________________________________________________________________

   PackageListPage::DoFindUpdates
   _________________________________________________________________________ */

void
PackageListPage::DoFindUpdates ()
{
  pInstaller->SetCallback (this);

  updates.reserve (100);

  pInstaller->SetRepository (repository);
  
  // find updateable packages
  pInstaller->FindUpdates ();
  
  // get list of updateable packages
  updates = pInstaller->GetUpdates();
  
  // sort by package name
  sort (updates.begin(), updates.end(), UpdateInfoComparer());
  
  canSelectAll = true;
  canDeselectAll = true;
  for (vector<PackageInstaller::UpdateInfo>::const_iterator
	 it = updates.begin();
       it != updates.end();
       ++ it)
  {
    if (it->action == PackageInstaller::UpdateInfo::Repair
      || it->action == PackageInstaller::UpdateInfo::ForceRemove
      || it->action == PackageInstaller::UpdateInfo::ForceUpdate)
    {
      canDeselectAll = false;
    }
    if (it->action == PackageInstaller::UpdateInfo::Keep
      || it->action == PackageInstaller::UpdateInfo::KeepAdmin)
    {
      canSelectAll = false;
    }
  }

  if (updates.size() == 0)
  {
    AfxMessageBox (T_(_T("There are currently no updates available.")),
		   MB_OK | MB_ICONINFORMATION);
  }
}

/* _________________________________________________________________________

   PackageListPage::ReportLine
   _________________________________________________________________________ */

void
PackageListPage::ReportLine (/*[in]*/ const char * lpszLine)
{
  UNUSED_ALWAYS (lpszLine);
}

/* _________________________________________________________________________

   PackageListPage::OnRetryableError
   _________________________________________________________________________ */

bool
PackageListPage::OnRetryableError (/*[in]*/ const char * lpszMessage)
{
  UINT style = MB_ICONSTOP;
  style |= MB_RETRYCANCEL;
  string str = lpszMessage;
  str += T_("  Then click Retry to complete the operation.");
  return (::MessageBoxW(0, UW_(str.c_str()), 0, style) != IDCANCEL);
}

/* _________________________________________________________________________

   PackageListPage::OnProgress
   _________________________________________________________________________ */

bool
PackageListPage::OnProgress (/*[in]*/ Notification	nf)
{
  UNUSED_ALWAYS (nf);
  return (! (pSheet->GetErrorFlag() || pSheet->GetCancelFlag()));
}

/* _________________________________________________________________________

   PackageListPage::EnableSelectButtons
   _________________________________________________________________________ */

void
PackageListPage::EnableSelectButtons ()
{
  BOOL enable =
    (ready
     && ! pSheet->GetCancelFlag()
     && ! pSheet->GetErrorFlag()
     && listControl.GetItemCount() > 0);
  CWnd * pWnd;
  if ((pWnd = GetDlgItem(IDC_SELECT_ALL)) == 0)
  {
    UNEXPECTED_CONDITION ("PackageListPage::EnableSelectButtons");
  }
  pWnd->EnableWindow (enable && canSelectAll);
  if ((pWnd = GetDlgItem(IDC_DESELECT_ALL)) == 0)
  {
    UNEXPECTED_CONDITION ("PackageListPage::EnableSelectButtons");
  }
  pWnd->EnableWindow (enable && canDeselectAll);
}

/* _________________________________________________________________________

   PackageListPage::SetProgressText
   _________________________________________________________________________ */

void
PackageListPage::SetProgressText (/*[in]*/ const char * lpszText)
{
  AUTO_TOGGLE (fillingTheListView);
  if (! listControl.DeleteAllItems())
  {
    FATAL_WINDOWS_ERROR ("CListCtrl::DeleteAllItems", 0);
  }
  LV_ITEM lvitem;
  lvitem.iItem = 0;
  lvitem.mask = 0;
  lvitem.iSubItem = 0;
  if (listControl.InsertItem(&lvitem) < 0)
  {
    FATAL_WINDOWS_ERROR ("CListCtrl::InsertItem", 0);
  }
  if (! listControl.SetItemText(0, 0, UT_(lpszText)))
  {
    FATAL_WINDOWS_ERROR ("CListCtrl::SetItemText", 0);
  }
  if (! listControl.SetItemState(0, 0, LVIS_STATEIMAGEMASK))
  {
    FATAL_WINDOWS_ERROR ("CListCtrl::SetItemState", 0);
  }
}

/* _________________________________________________________________________

   PackageListPage::InsertColumn
   _________________________________________________________________________ */

void
PackageListPage::InsertColumn (/*[in]*/ int		colIdx,
			       /*[in]*/ const char *	lpszLabel,
			       /*[in]*/ const char *	lpszLongest)
{
  if (listControl.InsertColumn(colIdx,
			       UT_(lpszLabel),
			       LVCFMT_LEFT,
			       listControl.GetStringWidth(UT_(lpszLongest)),
			       colIdx)
      < 0)
  {
    FATAL_WINDOWS_ERROR ("CListCtrl::InsertColumn", 0);
  }
}
