/* PropPageFormats.h:						-*- C++ -*-

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

#if defined(_MSC_VER)
#  pragma once
#endif

class PropPageFormats : public CPropertyPage
{
private:
  enum { IDD = IDD_PROPPAGE_FMT };

protected:
  DECLARE_MESSAGE_MAP();
  
public:
  PropPageFormats ();

public:
  CButton editButton;

public:
  CButton removeButton;

public:
  CButton makeButton;

public:
  CButton newButton;

public:
  CListCtrl listControl;

public:
  virtual
  BOOL
  OnInitDialog ();

public:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);
  
protected:
  afx_msg
  BOOL
  OnHelpInfo (/*[in]*/ HELPINFO * pHelpInfo);

public:
  afx_msg
  void
  OnContextMenu (/*[in]*/ CWnd * pWnd,
		 /*[in]*/ CPoint point);

public:
  afx_msg
  void
  OnListDoubleClick (/*[in]*/ NMHDR *		pNMHDR,
		     /*[out]*/ LRESULT *	pResult);

public:
  afx_msg
  void
  OnEdit ();

public:
  afx_msg
  void
  OnSelectionChange (/*[in]*/ NMHDR *	pNMHDR,
		     /*[in]*/ LRESULT *	pResult);

public:
  afx_msg
  void
  OnMake ();

public:
  afx_msg
  void
  OnNew ();

public:
  afx_msg
  void
  OnRemove ();

private:
  void
  Refresh ();

private:
  int
  GetSelectedItem ();

private:
  void
  EnableButtons ();

private:
  void
  MakeAlias (/*[in]*/ const FormatInfo &	formatInfo);

private:
  void
  InsertColumn (/*[in]*/ int			colIdx,
		/*[in]*/ const MIKTEXCHAR *	lpszLabel,
		/*[in]*/ const MIKTEXCHAR *	lpszLongest);  

private:
  bool modified;
};
