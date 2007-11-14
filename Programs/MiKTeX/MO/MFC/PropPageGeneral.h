/* PropPageGeneral.h:						-*- C++ -*-

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

class PropPageGeneral
  : public CPropertyPage,
    public ICreateFndbCallback
{
private:
  enum { IDD = IDD_PROPPAGE_GENERAL };
  
protected:
  DECLARE_MESSAGE_MAP();
  
public:
  PropPageGeneral (/*[in]*/ PackageManager * pManager);

public:
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
  OnChangeInstallOnTheFly ();

protected:
  afx_msg
  BOOL
  OnHelpInfo (/*[in]*/ HELPINFO * pHelpInfo);
  
protected:
  afx_msg
  void
  OnContextMenu (/*[in]*/ CWnd * pWnd,
		 /*[in]*/ CPoint point);

protected:
  afx_msg
  void
  OnRefreshFndb ();

protected:
  afx_msg
  void
  OnUpdateFmt ();

protected:
  afx_msg
  void
  OnChangePaperSize ();

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
  ProgressDialog * pProgressDialog;
  
private:
  int installOnTheFly;

private:
  int previousInstallOnTheFly;

private:
  int paperSizeIndex;

private:
  int previousPaperSizeIndex;

private:
  CComboBox paperSizeComboBox;
  
private:
  PackageManagerPtr pManager;
};
