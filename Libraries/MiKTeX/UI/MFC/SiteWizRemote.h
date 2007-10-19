/* SiteWizRemote.h:						-*- C++ -*-

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

#if defined(_MSC_VER)
#  pragma once
#endif

class SiteWizRemote : public CPropertyPage
{
private:
  enum { IDD = IDD_SITEWIZ_REMOTE_REPOSITORY };

protected:
  enum { FILL_LIST = WM_APP + 1 };

public:
  SiteWizRemote (/*[in]*/ PackageManager * pManager);

protected:
  virtual
  BOOL
  OnInitDialog ();

protected:
  virtual
  BOOL
  OnSetActive ();

protected:
  virtual
  BOOL
  OnWizardFinish ();

protected:
  virtual
  LRESULT
  OnWizardBack ();

protected:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);

protected:
  afx_msg
  void
  OnItemSelection (/*[in]*/ NMHDR *	pNMHDR,
		   /*[in]*/ LRESULT *	pResult);

protected:
  afx_msg
  LRESULT
  FillList (/*[in]*/ WPARAM wParam,
	    /*[in]*/ LPARAM lParam);

protected:
  DECLARE_MESSAGE_MAP();

private:
  void
  InsertColumn (/*[in]*/ int		colIdx,
		/*[in]*/ const char *	lpszLabel,
		/*[in]*/ const char *	lpszLongest);

private:
  void
  SetItemText (/*[in]*/ int		itemIdx,
	       /*[in]*/ int		colIdx,
	       /*[in]*/ const char *	lpszText);

private:
  static
  UINT
  DownloadList (/*[in]*/ void * pv);
  
private:
  vector<RepositoryInfo> repositories;

private:
  bool firstSetActive;

private:
  CListCtrl listControl;
    
private:
  PackageManagerPtr pManager;
};
