/* RemoteRepositoryPag.cpp:

   Copyright (C) 1999-2011 Christian Schenk

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

#include "RemoteRepositoryPage.h"
#include "SetupWizard.h"

const unsigned int WM_FILL_LIST = WM_APP + 1;

#define SHOW_DESCRIPTION 0

/* _________________________________________________________________________

   RemoteRepositoryPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(RemoteRepositoryPage, CPropertyPage)
  ON_MESSAGE(WM_FILL_LIST, OnFillList)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemChanged)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   RemoteRepositoryPage::RemoteRepositoryPage
   _________________________________________________________________________ */

RemoteRepositoryPage::RemoteRepositoryPage ()
  : CPropertyPage (IDD,
		   0,
		   IDS_HEADER_REMOTE_REPOSITORY,
		   IDS_SUBHEADER_REMOTE_REPOSITORY),
    firstSetActive (true),
    ready (false),
    pSheet (0)
{
}

/* _________________________________________________________________________

   RemoteRepositoryPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
RemoteRepositoryPage::OnInitDialog ()
{
  pSheet = reinterpret_cast<SetupWizard *>(GetParent());
  
  BOOL ret = CPropertyPage::OnInitDialog();

  try
    {
      listControl.SetExtendedStyle (listControl.GetExtendedStyle()
				    | LVS_EX_FULLROWSELECT);
      
      int colIdx = 0;

      InsertColumn (colIdx,
		    T_("Country"),
		    T_("xxxx The Czech Republic"));
      ++ colIdx;

      InsertColumn (colIdx,
		    T_("Protocol"),
		    "xxxx Protocol");
      ++ colIdx;

      InsertColumn (colIdx,
		    T_("Host"),
		    T_("xxxx scratchy.emate.ucr.ac.cr"));
      ++ colIdx;

      InsertColumn (colIdx,
		    T_("Version"),
		    T_("xxxx 30-Aug-04"));
      ++ colIdx;
      
#if SHOW_DESCRIPTION
      InsertColumn (colIdx,
		    T_("Description"),
		    T_("xxxx Primary Package Repository"));
      ++ colIdx;
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

  return (ret);
}

/* _________________________________________________________________________

   RemoteRepositoryPage::OnSetActive
   _________________________________________________________________________ */

BOOL
RemoteRepositoryPage::OnSetActive ()
{
  noDdv = false;
  BOOL ret = CPropertyPage::OnSetActive();
  if (ret)
    {
      try
	{
	  if (firstSetActive)
	    {
	      firstSetActive = false;
	      showAll = (GetKeyState(VK_CONTROL) < 0);
	      SetProgressText (T_("Connecting..."));
	      pSheet->SetWizardButtons (0);
	      AfxBeginThread (WorkerThread, this);
	    }
	  else
	    {
	      OnFillList (0, 0);
	    }
	}
      catch (const MiKTeXException & e)
	{
	  pSheet->ReportError (e);
	}
      catch (const exception & e)
	{
	  pSheet->ReportError (e);
	}
    }
  return (ret);
}

/* _________________________________________________________________________

   RemoteRepositoryPage::DoDataExchange
   _________________________________________________________________________ */

void
RemoteRepositoryPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_LIST, listControl);
}

/* _________________________________________________________________________

   RemoteRepositoryPage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
RemoteRepositoryPage::OnWizardNext ()
{
  UINT next;
  switch (theApp.setupTask.Get())
    {
    case SetupTask::Download:
    case SetupTask::InstallFromRemoteRepository:
      next = IDD_LOCAL_REPOSITORY;
      break;
    default:
      MIKTEX_ASSERT (false);
      __assume (false);
      break;
    }
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(next)));
}

/* _________________________________________________________________________

   RemoteRepositoryPage::OnWizardBack
   _________________________________________________________________________ */

LRESULT
RemoteRepositoryPage::OnWizardBack ()
{
  UINT prev;
  switch (theApp.setupTask.Get())
    {
    case SetupTask::Download:
      prev = IDD_PACKAGE_SET_DOWNLOAD;
      break;
    case SetupTask::InstallFromRemoteRepository:
      prev = IDD_SHARED;
      break;
    default:
      MIKTEX_ASSERT (false);
      __assume (false);
      break;
    }
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(prev)));
}

/* _________________________________________________________________________

   RemoteRepositoryPage::OnKillActive
   _________________________________________________________________________ */

BOOL
RemoteRepositoryPage::OnKillActive ()
{
  BOOL ret = CPropertyPage::OnKillActive();
  if (ret && ! noDdv)
    {
      try
	{
	  POSITION pos = listControl.GetFirstSelectedItemPosition();
	  if (pos != 0)
	    {
	      int i = listControl.GetNextSelectedItem(pos);
	      theApp.remotePackageRepository = repositories[i].url.c_str();
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

   RemoteRepositoryPage::OnQueryCancel
   _________________________________________________________________________ */

BOOL
RemoteRepositoryPage::OnQueryCancel ()
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

   RemoteRepositoryPage::OnFillList
   _________________________________________________________________________ */

LRESULT
RemoteRepositoryPage::OnFillList (/*[in]*/ WPARAM		wParam,
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

      if (! listControl.DeleteAllItems())
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::DeleteAllItems", 0);
	}
      
      bool selected = false;

      int idx = 0;

      for (vector<RepositoryInfo>::const_iterator it = repositories.begin();
	   it != repositories.end();
	   ++ it, ++ idx)
	{
#if MIKTEX_RELEASE_STATE < 4
	  if (it->packageLevel < theApp.packageLevel)
	    {
	      continue;
	    }
#endif

	  LV_ITEM lvitem;
	  
	  lvitem.iItem = idx;
	  lvitem.mask = LVIF_PARAM;
	  lvitem.lParam = idx;
	  lvitem.iSubItem = 0;
	  
	  if (listControl.InsertItem(&lvitem) < 0)
	    {
	      FATAL_WINDOWS_ERROR ("CListCtrl::InsertItem", 0);
	    }

	  string protocol;
	  string host;

	  SplitUrl (it->url, protocol, host);

	  SetItemText (idx, 0, it->country.c_str());
	  CStringW protUC = UW_(protocol.c_str());
	  protUC.MakeUpper ();
	  SetItemText (idx, 1, WU_(protUC));
	  SetItemText (idx, 2, host.c_str());
      	  SetItemText (idx, 3, TU_(CTime(it->timeDate).Format(_T("%d-%b-%y"))));
#if SHOW_DESCRIPTION
	  SetItemText (idx, 4, it->description.c_str());
#endif

	  if (it->url == theApp.remotePackageRepository)
	    {
	      if (! listControl.SetItemState(idx,
					     LVIS_SELECTED,
					     LVIS_SELECTED))
		{
		  FATAL_WINDOWS_ERROR ("CListCtrl::SetItemState", 0);
		}
	      selected = true;
	    }
	}

      pSheet->SetWizardButtons (PSWIZB_BACK
				| (selected ? PSWIZB_NEXT : 0));
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

   RemoteRepositoryPage::OnItemChanged
   _________________________________________________________________________ */

void
RemoteRepositoryPage::OnItemChanged (/*[in]*/ NMHDR *	pNMHDR,
				     /*[in]*/ LRESULT *	pResult)
{
  UNUSED_ALWAYS (pNMHDR);
  *pResult = 0;
  if (ready && ! pSheet->GetCancelFlag() || pSheet->GetErrorFlag())
    {
      pSheet->SetWizardButtons (PSWIZB_BACK | PSWIZB_NEXT);
    }
}

/* _________________________________________________________________________

   CountryComparer
   _________________________________________________________________________ */

class CountryComparer
{
public:
  inline
  bool
  operator() (/*[in]*/ const RepositoryInfo &	lhs,
	      /*[in]*/ const RepositoryInfo &	rhs)
  {
    return (StringCompare(lhs.country.c_str(),
			  rhs.country.c_str(),
			  true)
	    < 0);
  }
};

/* _________________________________________________________________________

   RemoteRepositoryPage::WorkerThread
   _________________________________________________________________________ */

UINT
RemoteRepositoryPage::WorkerThread (/*[in]*/ void * pv)
{
  RemoteRepositoryPage * This = reinterpret_cast<RemoteRepositoryPage *>(pv);
  try
    {
      theApp.pManager->DownloadRepositoryList ();
      This->repositories = theApp.pManager->GetRepositories();
      sort (This->repositories.begin(),
	    This->repositories.end(),
	    CountryComparer());
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

   RemoteRepositoryPage::InsertColumn
   _________________________________________________________________________ */

void
RemoteRepositoryPage::InsertColumn (/*[in]*/ int		colIdx,
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

/* _________________________________________________________________________

   RemoteRepositoryPage::SetItemText
   _________________________________________________________________________ */
 
void
RemoteRepositoryPage::SetItemText (/*[in]*/ int			itemIdx,
				   /*[in]*/ int			colIdx,
				   /*[in]*/ const char *	lpszText)
{
  if (! listControl.SetItemText(itemIdx, colIdx, UT_(lpszText)))
    {
      FATAL_WINDOWS_ERROR ("CListCtrl::SetItemText", 0);
    }
}

/* _________________________________________________________________________

   RemoteRepositoryPage::SetProgressText
   _________________________________________________________________________ */

void
RemoteRepositoryPage::SetProgressText (/*[in]*/ const char * lpszText)
{
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
  listControl.SetItemState (0, 0, LVIS_STATEIMAGEMASK);
}
