/* PropPageLanguages.h:						-*- C++ -*-

   Copyright (C) 2000-2010 Christian Schenk

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

class PropPageLanguages : public CPropertyPage
{
public:
  PropPageLanguages (/*[in]*/ PackageManager * pManager);

public:
  ~PropPageLanguages ();

private:
  enum { IDD = IDD_PROPPAGE_LANG };

private:
  CListCtrl listControl;

protected:
  virtual
  BOOL
  OnInitDialog ();

protected:
  virtual
  BOOL
  OnApply ();

protected:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);
  
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
  OnDblclkList (/*[in]*/ NMHDR *	pNMHDR,
		/*[in]*/ LRESULT *	pResult);

protected:
  afx_msg
  void
  OnItemChanged (/*[in]*/ NMHDR *	pNMHDR,
		 /*[in]*/ LRESULT *	pResult);

protected:
  afx_msg
  void
  OnItemChanging (/*[in]*/ NMHDR *	pNMHDR,
		  /*[in]*/ LRESULT *	pResult);

protected:
  DECLARE_MESSAGE_MAP ();

private:
  struct MyLanguageInfo : public LanguageInfo
  {
    MyLanguageInfo (const LanguageInfo & languageInfo)
      : active(false),
        newActive(false),
	LanguageInfo (languageInfo)
    {
    }
    bool active;
    PathName loaderPath;
    bool newActive;
    vector<string> packageNames;
  };

private:
  vector<MyLanguageInfo> languages;

private:
  void
  ReadLanguageDat ();

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
  RefreshRow (int);

private:
  void
  InsertLanguage (/*[in]*/ int idx);

private:
  void
  InsertColumn (/*[in]*/ int		colIdx,
		/*[in]*/ const char *	lpszLabel,
		/*[in]*/ const char *	lpszLongest);

private:
  void
  SetElevationRequired (/*[in]*/ bool f);

private:
  void
  SetChanged (/*[in]*/ bool f);

private:
  vector<string>
  WhichPackage (/*[in]*/ const char * lpszTeXInputFile);

private:
  bool inserting;

private:
  PathName languageDat;

private:
  PackageManagerPtr pManager;
};
