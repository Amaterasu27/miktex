/* LanguageDefinitionDialog.h:						-*- C++ -*-

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

class LanguageDefinitionDialog : public CDialog
{
public:
  LanguageDefinitionDialog (/*[in]*/ CWnd * pParent = 0)
  {
    UNUSED_ALWAYS (pParent);
  }
  
public:
  LanguageDefinitionDialog (/*[in]*/ CWnd *		pParent,
			    /*[in]*/ const LANGUAGE &	lang);

public:
  LANGUAGE
  GetLanguage ()
    const
  {
    return (language);
  }

private:
  enum { IDD = IDD_LANG_DEFINITION };

private:
  BOOL	active;

private:
  CString fileName;

private:
  CString languageName;

private:
  CString synonyms;

protected:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange* pDX);
  
protected:
  afx_msg
  BOOL
  OnHelpInfo (/*[in]*/ HELPINFO* pHelpInfo);
  
protected:
  afx_msg
  void
  OnContextMenu (/*[in]*/ CWnd * pWnd,
		 /*[in]*/ CPoint point);
  
protected:
  afx_msg
  void
  OnBrowse ();

protected:
  DECLARE_MESSAGE_MAP();

protected:
  virtual
  BOOL
  OnInitDialog ();

private:
  LANGUAGE language;
};
