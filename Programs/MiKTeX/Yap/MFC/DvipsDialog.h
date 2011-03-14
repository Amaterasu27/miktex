/* DvipsDialog.h:						-*- C++ -*-

   Copyright (C) 1996-2011 Christian Schenk

   This file is part of Yap.

   Yap is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   Yap is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with Yap; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#pragma once

class DvipsDialog : public CDialog
{
private:
  enum { IDD = IDD_DVIPS };

protected:
  DECLARE_MESSAGE_MAP();

public:
  DvipsDialog (/*[in]*/ CWnd * pParent = 0);

public:
  const char *
  GetPrinterName ()
    const
  {
    return (CT2A(m_strPrinterName));
  }

public:
  CEdit	m_editFirst;

public:
  CComboBox m_comboPrinterName;

public:
  int m_nPageRange;

public:
  int m_nEvenOdd;

public:
  int m_iFirstPage;

public:
  int m_iLastPage;

public:
  int m_nPrinterName;

protected:
  virtual
  BOOL
  OnInitDialog ();

protected:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);
  
protected:
  afx_msg
  void
  OnChangeFirstPage ();
  
protected:
  afx_msg
  void
  OnChangeLastPage ();
  
protected:
  afx_msg
  void
  OnPageRange ();

private:
  CString m_strPrinterName;
  afx_msg void OnBnClickedCancel();
};
