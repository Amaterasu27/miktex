/* SiteWizRemote.cpp:

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

#include "SiteWizRemote.h"

/* _________________________________________________________________________

   SiteWizRemote::SiteWizRemote
   _________________________________________________________________________ */

SiteWizRemote::SiteWizRemote (/*[in]*/ PackageManager * pManager)
  : CPropertyPage (SiteWizRemote::IDD, IDS_SITEWIZ),
    firstSetActive (true),
    pManager (pManager)
{
  m_psp.dwFlags &= ~ PSP_HASHELP;
}

/* _________________________________________________________________________

   SiteWizRemote::DoDataExchange
   _________________________________________________________________________ */

void
SiteWizRemote::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_LIST, listControl);
}

/* _________________________________________________________________________

   SiteWizRemote Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(SiteWizRemote, CPropertyPage)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemSelection)
  ON_MESSAGE(FILL_LIST, FillList)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   SiteWizRemote::OnSetActive
   _________________________________________________________________________ */

BOOL
SiteWizRemote::OnSetActive ()
{
  BOOL ret = CPropertyPage::OnSetActive();
  try
    {
      CPropertySheet * pSheet = reinterpret_cast<CPropertySheet*>(GetParent());
      ASSERT_KINDOF (CPropertySheet, pSheet);
      if (firstSetActive)
	{
	  LV_ITEM lvitem;
	  lvitem.iItem = 0;
	  lvitem.mask = 0;
	  lvitem.iSubItem = 0;
	  if (listControl.InsertItem(&lvitem) < 0)
	    {
	      FATAL_WINDOWS_ERROR ("CListCtrl::InsertItem", 0);
	    }
	  if (! listControl.SetItemText(0,
					0,
					T_("Connecting...")))
	    {
	      FATAL_WINDOWS_ERROR ("CListCtrl::SetItemText", 0);
	    }
	  pSheet->SetWizardButtons (0);
	  firstSetActive = false;
	  AfxBeginThread (DownloadList, this);
	}
      else
	{
	  FillList (0, 0);
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

   SiteWizRemote::DownloadList
   _________________________________________________________________________ */

UINT
SiteWizRemote::DownloadList (/*[in]*/ void * pv)
{
  SiteWizRemote * This = reinterpret_cast<SiteWizRemote*>(pv);
  try
    {
      This->pManager->DownloadRepositoryList ();
      This->repositories = This->pManager->GetRepositories();
      sort (This->repositories.begin(),
	    This->repositories.end(),
	    CountryComparer());
      if (! This->PostMessage(FILL_LIST))
	{
	  FATAL_WINDOWS_ERROR ("CWnd::PostMessage", 0);
	}
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (This, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (This, e);
    }
  return (0);
}

/* _________________________________________________________________________

   SiteWizRemote::InsertColumn
   _________________________________________________________________________ */

void
SiteWizRemote::InsertColumn (/*[in]*/ int		colIdx,
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

   SiteWizRemote::OnInitDialog
   _________________________________________________________________________ */

BOOL
SiteWizRemote::OnInitDialog ()
{
  BOOL ret = CPropertyPage::OnInitDialog();

  try
    {
      listControl.SetExtendedStyle (listControl.GetExtendedStyle()
				    | LVS_EX_FULLROWSELECT);
      
      int colIdx = 0;
      
      InsertColumn (colIdx,
		    T_("Country"),
		    "xxxx Australia");
      
      ++ colIdx;
      
      InsertColumn (colIdx,
		    T_("Protocol"),
		    "xxxx HTTP");

      ++ colIdx;

      InsertColumn (colIdx,
		    T_("Host"),
		    "xxxx scratchy.emate.ucr.ac.cr");
      
      ++ colIdx;
      
      InsertColumn (colIdx,
		    T_("Version"),
		    "xxxx 30-Aug-04");
      
      ++ colIdx;
      
      InsertColumn (colIdx,
		    T_("Description"),
		    "xxxx Primary Package Repository");
      
      ++ colIdx;
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

   SiteWizRemote::OnWizardFinish
   _________________________________________________________________________ */

BOOL
SiteWizRemote::OnWizardFinish ()
{
  try
    {
      POSITION pos = listControl.GetFirstSelectedItemPosition();
      MIKTEX_ASSERT (pos != 0);
      if (pos == 0)
	{
	  return (FALSE);
	}
      int i = listControl.GetNextSelectedItem(pos);
      pManager->SetDefaultPackageRepository (RepositoryType::Remote,
					     repositories[i].url);
      return (CPropertyPage::OnWizardFinish());
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

   SiteWizRemote::OnWizardBack
   _________________________________________________________________________ */

LRESULT
SiteWizRemote::OnWizardBack ()
{
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_SITEWIZ_TYPE)));
}

/* _________________________________________________________________________

   SiteWizRemote::OnItemSelection
   _________________________________________________________________________ */

void
SiteWizRemote::OnItemSelection (/*[in]*/ NMHDR *	pNMHDR,
				/*[in]*/ LRESULT *	pResult)
{
  UNUSED_ALWAYS (pNMHDR);
  CPropertySheet * pSheet = reinterpret_cast<CPropertySheet*>(GetParent());
  ASSERT_KINDOF (CPropertySheet, pSheet);
  pSheet->SetWizardButtons (PSWIZB_BACK | PSWIZB_FINISH);
  *pResult = 0;
}

/* _________________________________________________________________________

   SiteWizRemote::SetItemText
   _________________________________________________________________________ */

void
SiteWizRemote::SetItemText (/*[in]*/ int		itemIdx,
			    /*[in]*/ int		colIdx,
			    /*[in]*/ const char *	lpszText)
{
  if (! listControl.SetItemText(itemIdx, colIdx, lpszText))
    {
      FATAL_WINDOWS_ERROR ("CListCtrl::SetItemText", 0);
    }
}

/* _________________________________________________________________________

   SplitUrl
   _________________________________________________________________________ */

void
SplitUrl (/*[in]*/ const string &	url,
	  /*[out]*/ string &		protocol,
	  /*[out]*/ string &		host)
{
  char szProtocol[200];
  char szHost[200];
  URL_COMPONENTS url_comp = { 0 };
  url_comp.dwStructSize = sizeof(url_comp);
  url_comp.lpszScheme = szProtocol;
  url_comp.dwSchemeLength = 200;
  url_comp.lpszHostName = szHost;
  url_comp.dwHostNameLength = 200;
  if (! InternetCrackUrl(url.c_str(), 0, 0, &url_comp))
    {
      FATAL_WINDOWS_ERROR ("InternetCrackUrl", 0);
    }
  protocol = szProtocol;
  host = szHost;
}
/* _________________________________________________________________________

   SiteWizRemote::FillList
   _________________________________________________________________________ */

LRESULT
SiteWizRemote::FillList (/*[in]*/ WPARAM	wParam,
			 /*[in]*/ LPARAM	lParam)
{
  UNUSED_ALWAYS (wParam);
  UNUSED_ALWAYS (lParam);

  CPropertySheet * pSheet = reinterpret_cast<CPropertySheet*>(GetParent());
  ASSERT_KINDOF (CPropertySheet, pSheet);

  try
    {
      if (! listControl.DeleteAllItems())
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::DeleteAllItems", 0);
	}
      
      bool selected = false;
      
      string url;

      if (! pManager->TryGetRemotePackageRepository(url))
	{
	  url = "";
	}

      int idx = 0;
      
      for (vector<RepositoryInfo>::const_iterator it = repositories.begin();
	   it != repositories.end();
	   ++ it, ++ idx)
	{
#if MIKTEX_RELEASE_STATE < 4
	  if (it->packageLevel == PackageLevel::None)
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
	  CString protUC = protocol.c_str();
	  protUC.MakeUpper ();
	  SetItemText (idx, 1, protUC);
	  SetItemText (idx, 2, host.c_str());
      	  SetItemText (idx, 3, CTime(it->timeDate).Format("%d-%b-%y"));
	  SetItemText (idx, 4, it->description.c_str());

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
				| (selected ? PSWIZB_FINISH : 0));
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }

  return (0);
}
