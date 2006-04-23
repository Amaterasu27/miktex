/* PropPagePackages.h:						-*- C++ -*-

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

#pragma once

class PropPagePackages : public CPropertyPage
{
private:
  enum { IDD = IDD_PROPPAGE_PACKAGES };

protected:
  DECLARE_MESSAGE_MAP ();


public:
  PropPagePackages (/*[in]*/ PackageManager * pManager);
  
public:
  virtual
  ~PropPagePackages ();

protected:
  virtual
  BOOL
  OnInitDialog ();

protected:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);

protected:
  virtual
  BOOL
  OnApply ();
  
protected:
  afx_msg
  BOOL
  OnHelpInfo /*[in]*/ (HELPINFO * pHelpInfo);

  
protected:
  afx_msg
  LRESULT
  OnToggle (/*[in]*/ WPARAM wParam,
	    /*[in]*/ LPARAM lParam);
  
protected:
  afx_msg
  void
  OnChangeUrl ();
  
protected:
  afx_msg
  void
  OnContextMenu (/*[in]*/ CWnd * pWnd,
		 /*[in]*/ CPoint point);
  
protected:
  afx_msg
  void
  OnGetInfoTip (/*[in]*/ NMHDR *	pNMHDR,
		/*[in]*/ LRESULT *	pResult);
  
protected:
  afx_msg
  void
  OnSelchangedTree (/*[in]*/ NMHDR *	pNMHDR,
		    /*[in]*/ LRESULT *	pResult);
  
protected:
  afx_msg
  void
  OnBnClickedPackageManager ();
  
private:
  void
  ShowPackageInfo (/*[in]*/ const PackageInfo * pPackageInfo);
  
private:
  void
  Scan ();

private:
  void
  SetWindowText (/*[in]*/ UINT			controlId,
		 /*[in]*/ const MIKTEXCHAR *	lpszText);

private:  
  CString url;

private:
  PackageTreeCtrl * pPackageTreeCtrl;
  
private:
  PackageManagerPtr pManager;
};
