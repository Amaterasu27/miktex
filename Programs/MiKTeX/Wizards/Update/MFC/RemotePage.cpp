/* RemotePage.cpp:

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

#include "RemotePage.h"
#include "UpdateWizard.h"

IMPLEMENT_DYNCREATE(RemotePage, CPropertyPage);

const unsigned int WM_FILL_LIST = WM_APP + 1;

#define SHOW_DESCRIPTION 0

/* _________________________________________________________________________

   RemotePage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(RemotePage, CPropertyPage)
  ON_MESSAGE(WM_FILL_LIST, OnFillList)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemChanged)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   RemotePage::RemotePage
   _________________________________________________________________________ */

RemotePage::RemotePage ()
  : CPropertyPage (IDD, 0, IDS_HEADER_REMOTE, IDS_SUBHEADER_REMOTE),
    firstSetActive (true),
    ready (false),
    showAll (false),
    pSheet (0)
{
  m_psp.dwFlags &= ~ PSP_HASHELP;
}

/* _________________________________________________________________________

   RemotePage::OnInitDialog
   _________________________________________________________________________ */

BOOL
RemotePage::OnInitDialog ()
{
  pSheet = reinterpret_cast<UpdateWizard*>(GetParent());
  ASSERT_KINDOF (UpdateWizard, pSheet);  

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

   RemotePage::OnSetActive
   _________________________________________________________________________ */

BOOL
RemotePage::OnSetActive ()
{
  noDdv = false;
  BOOL ret = CPropertyPage::OnSetActive();
  if (ret)
    {
      try
	{
	  pSheet->ClearErrorFlag ();
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

   RemotePage::DoDataExchange
   _________________________________________________________________________ */

void
RemotePage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_LIST, listControl);
}

/* _________________________________________________________________________

   RemotePage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
RemotePage::OnWizardNext ()
{
  pSheet->SetCameFrom (IDD);
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_PACKAGE_LIST)));
}

/* _________________________________________________________________________

   RemotePage::OnWizardBack
   _________________________________________________________________________ */

LRESULT
RemotePage::OnWizardBack ()
{
  noDdv = true;
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_TYPE)));
}

/* _________________________________________________________________________

   RemotePage::OnKillActive
   _________________________________________________________________________ */

BOOL
RemotePage::OnKillActive ()
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
	      g_pManager->SetRemotePackageRepository (repositories[i].url);
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

   RemotePage::OnQueryCancel
   _________________________________________________________________________ */

BOOL
RemotePage::OnQueryCancel ()
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

   RemotePage::OnFillList
   _________________________________________________________________________ */

LRESULT
RemotePage::OnFillList (/*[in]*/ WPARAM		wParam,
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

      string url;

      if (! g_pManager->TryGetRemotePackageRepository(url))
	{
	  url = "";
	}
  
      int idx = 0;

      for (vector<RepositoryInfo>::const_iterator it = repositories.begin();
	   it != repositories.end();
	   ++ it, ++ idx)
	{
	  if (it->packageLevel == PackageLevel::None && ! showAll)
	    {
	      continue;
	    }
	  
	  LVITEM lvitem;
	  
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
	  CString protUC (protocol.c_str());
	  protUC.MakeUpper ();
	  SetItemText (idx, 1, CT2A(protUC));
	  SetItemText (idx, 2, host.c_str());
      	  SetItemText (idx, 3, CT2A(CTime(it->timeDate).Format(_T("%d-%b-%y"))));
#if SHOW_DESCRIPTION
	  SetItemText (idx, 4, it->description.c_str());
#endif

	  if (it->url == url)
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

   RemotePage::OnItemChanged
   _________________________________________________________________________ */

void
RemotePage::OnItemChanged (/*[in]*/ NMHDR *	pNMHDR,
			   /*[out]*/ LRESULT *	pResult)
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

   RemotePage::WorkerThread
   _________________________________________________________________________ */

UINT
RemotePage::WorkerThread (/*[in]*/ void * pv)
{
  RemotePage * This = reinterpret_cast<RemotePage*>(pv);

  try
    {
      g_pManager->DownloadRepositoryList ();
      This->repositories = g_pManager->GetRepositories();
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

   RemotePage::InsertColumn
   _________________________________________________________________________ */

void
RemotePage::InsertColumn (/*[in]*/ int		colIdx,
			  /*[in]*/ const char *	lpszLabel,
			  /*[in]*/ const char *	lpszLongest)
{
  if (listControl.InsertColumn(colIdx,
			       CA2T(lpszLabel),
			       LVCFMT_LEFT,
			       listControl.GetStringWidth(CA2T(lpszLongest)),
			       colIdx)
      < 0)
    {
      FATAL_WINDOWS_ERROR ("CListCtrl::InsertColumn", 0);
    }
}

/* _________________________________________________________________________

   RemotePage::SetItemText
   _________________________________________________________________________ */
 
void
RemotePage::SetItemText (/*[in]*/ int		itemIdx,
			 /*[in]*/ int		colIdx,
			 /*[in]*/ const char *	lpszText)
{
  if (! listControl.SetItemText(itemIdx, colIdx, CA2T(lpszText)))
    {
      FATAL_WINDOWS_ERROR ("CListCtrl::SetItemText", 0);
    }
}

/* _________________________________________________________________________

   RemotePage::SetProgressText
   _________________________________________________________________________ */

void
RemotePage::SetProgressText (/*[in]*/ const char * lpszText)
{
  if (! listControl.DeleteAllItems())
    {
      FATAL_WINDOWS_ERROR ("CListCtrl::DeleteAllItems", 0);
    }
  LVITEM lvitem;
  lvitem.iItem = 0;
  lvitem.mask = 0;
  lvitem.iSubItem = 0;
  if (listControl.InsertItem(&lvitem) < 0)
    {
      FATAL_WINDOWS_ERROR ("CListCtrl::InsertItem", 0);
    }
  if (! listControl.SetItemText(0, 0, CA2T(lpszText)))
    {
      FATAL_WINDOWS_ERROR ("CListCtrl::SetItemText", 0);
    }
  listControl.SetItemState (0, 0, LVIS_STATEIMAGEMASK);
}
