/* PackageListPag.h:						-*- C++ -*-

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

#pragma once

#include "resource.h"

class PackageListPage
  : public CPropertyPage,
    public PackageInstallerCallback
{
private:
  DECLARE_DYNCREATE(PackageListPage);

protected:
  DECLARE_MESSAGE_MAP();
  
private:
  enum { IDD = IDD_PACKAGE_LIST };
  
public:
  PackageListPage ();
  
public:
  virtual
  ~PackageListPage ();
  
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
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);
  
protected:
  virtual
  LRESULT
  OnWizardNext ();
  
protected:
  virtual
  LRESULT
  OnWizardBack ();

protected:
  virtual
  BOOL
  OnKillActive ();

protected:
  virtual
  BOOL
  OnQueryCancel ();
  
protected:
  afx_msg
  LRESULT
  OnFillList (/*[in]*/ WPARAM wParam,
	      /*[in]*/ LPARAM lParam);
  
protected:
  afx_msg
  void
  OnSelectAll ();
  
protected:
  afx_msg
  void
  OnDeselectAll ();

protected:
  afx_msg
  void
  OnItemActivate (/*[in]*/ NMHDR *	pNMHDR,
		  /*[in]*/ LRESULT *	pResult);

protected:
  afx_msg
  void
  OnItemChanging (/*[in]*/ NMHDR *	pNMHDR,
		  /*[in]*/ LRESULT *	pResult);

protected:
  afx_msg
  void
  OnItemChanged (/*[in]*/ NMHDR *	pNMHDR,
		 /*[in]*/ LRESULT *	pResult);

private:
  static
  UINT
  WorkerThread (/*[in]*/ void * pv);

private:  
  void
  DoFindUpdates ();

public:
  virtual
  void
  MIKTEXTHISCALL
  ReportLine (/*[in]*/ const char * lpszLine);

public:
  virtual
  bool
  MIKTEXTHISCALL
  OnRetryableError (/*[in]*/ const char * lpszMessage);

public:
  virtual
  bool
  MIKTEXTHISCALL
  OnProgress (/*[in]*/ Notification		nf);

private:
  void
  EnableSelectButtons ();

private:
  void
  InsertColumn (/*[in]*/ int			colIdx,
		/*[in]*/ const char *	lpszLabel,
		/*[in]*/ const char *	lpszLongest);
 
private:
  void
  SetProgressText (/*[in]*/ const char * lpszText);

private:
  HANDLE hWorkerThread;
  
private:
  vector<PackageInstaller::UpdateInfo> updates;
  
private:
  auto_ptr<PackageInstaller> pInstaller;
  
private:
  bool canSelectAll;

private:
  bool canDeselectAll;

private:
  string repository;
  
private:
  bool fillingTheListView;

private:
  bool ready;

private:
  CListCtrl listControl;
  

private:
  class UpdateWizard * pSheet;
};
