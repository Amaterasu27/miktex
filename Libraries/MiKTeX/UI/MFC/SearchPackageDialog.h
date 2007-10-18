/* SearchPackageDialog.h:					-*- C++ -*-

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

#pragma once

#include "afxcmn.h"

class SearchPackageDialog : public CDialog
{
protected:
  DECLARE_MESSAGE_MAP();

private:
  enum { IDD = IDD_SEARCH_PACKAGE };

public:
  SearchPackageDialog
  (/*[in]*/ CWnd *		pWnd,
   /*[in]*/ PackageManager *	pManager);

protected:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);
  
private:
  virtual
  BOOL
  OnInitDialog ();

protected:
  afx_msg
  void
  OnChangeName ();

public:
  afx_msg
  void
  OnEnChangeTitle ();
  
public:
  afx_msg
  void
  OnEnChangeDescription ();

public:
  afx_msg
  void
  OnEnChangeFile ();

public:
  afx_msg
  void
  OnBnClickedFindNow ();
  
public:
  afx_msg
  void
  OnLvnItemchangedListPackages (/*[in]*/ NMHDR *    pNMHDR,
                                /*[out]*/ LRESULT * pResult);

public:
  afx_msg
  void
  OnNMDblclkListPackages (/*[in]*/ NMHDR *    pNMHDR,
			  /*[in]*/ LRESULT *  pResult);

public:
  afx_msg
  void
  OnNMRclickListPackages (/*[in]*/ NMHDR *    pNMHDR,
			  /*[in]*/ LRESULT *  pResult);

public:
  void
  DoContextMenu (/*[in]*/ POINT			pt,
		 /*[in]*/ const char *	lpszExternalName);

public:
  CString
  GetName ()
    const
  {
    return (name);
  }

private:
  void
  EnableButtons ();

private:
  CString
  GetWindowText (/*[in]*/ UINT controlId);

private:
  CListCtrl listControl;

private:
  CString name;

protected:
  map<LPARAM, PackageInfo> packages;

private:
  PackageManagerPtr pManager;
};
