/* PropPageRoots.h:						-*- C++ -*-

   Copyright (C) 2000-2007 Christian Schenk

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

class PropPageTeXMFRoots
  : public CPropertyPage,
    public ICreateFndbCallback
{
private:
  enum { IDD = IDD_PROPPAGE_ROOTS };

protected:
  DECLARE_MESSAGE_MAP();
  
public:
  PropPageTeXMFRoots ();

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
  void
  OnScan();

protected:
  afx_msg
  void
  OnAdd ();

protected:
  afx_msg
  void
  OnMovedown ();

protected:
  afx_msg
  void
  OnMoveup ();

protected:
  afx_msg
  void
  OnRemove ();

protected:
  afx_msg
  void
  OnGetInfoTip (/*[in]*/ NMHDR *	pNMHDR,
		/*[in]*/ LRESULT *	pResult);

protected:
  afx_msg
  void
  OnSelectionChange (/*[in]*/ NMHDR * pNMHDR,
		     /*[in]*/ LRESULT * pResult);

protected:
  afx_msg
  BOOL
  OnHelpInfo (/*[in]*/ HELPINFO * pHelpInfo);
  
protected:
  afx_msg
  void
  OnContextMenu (/*[in]*/ CWnd* pWnd,
		 /*[in]*/ CPoint point);

private:
  virtual
  bool
  MIKTEXTHISCALL
  ReadDirectory (/*[in]*/ const char *	lpszPath,
		 /*[out]*/ char * *	ppSubDirNames,
		 /*[out]*/ char * *	ppFileNames,
		 /*[out]*/ char * *	ppFileNameInfos);

private:
  virtual
  bool
  MIKTEXTHISCALL
  OnProgress (/*[in]*/ unsigned			level,
	      /*[in]*/ const char *	lpszDirectory);

private:
  bool isModified;

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
  InsertColumn (/*[in]*/ int			colIdx,
		/*[in]*/ const char *	lpszLabel,
		/*[in]*/ const char *	lpszLongest);

private:
  void
  CheckRoot (/*[in]*/ const PathName & root);

private:
  static
  INT
  CALLBACK
  BrowseCallbackProc (/*[in]*/ HWND hwnd, 
		      /*[in]*/ UINT uMsg,
		      /*[in]*/ LPARAM lParam, 
		      /*[in]*/ LPARAM pData);

private:
  void
  SetElevationRequired (/*[in]*/ bool f);

private:
  void
  SetChanged (/*[in]*/ bool f);

private:
  CButton scanButton;

private:
  CButton removeButton;

private:
  CButton upButton;

private:
  CButton downButton;

private:
  CButton addButton;

private:
  CListCtrl listControl;

private:
  PathName installRoot;

private:
  PathName commonDataRoot;

private:
  PathName userDataRoot;

private:
  PathName commonConfigRoot;

private:
  PathName userConfigRoot;

private:
  vector<PathName> roots;

private:
  ProgressDialog * pProgressDialog;
};
