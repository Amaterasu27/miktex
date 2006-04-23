/* TextViewerDialog.cpp:

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

#include "StdAfx.h"
#include "internal.h"
#include "resource.h"

#include <miktex/TextViewerDialog.h>

/* _________________________________________________________________________

   TextViewerDlgImpl
   _________________________________________________________________________ */


class TextViewerDlgImpl : public CDialog
{
public:
  TextViewerDlgImpl (/*[in]*/ CWnd *			pParent,
		     /*[in]*/ const MIKTEXCHAR *	lpszTitle,
		     /*[in]*/ const MIKTEXCHAR *	lpszText);

private:
  virtual
  BOOL
  OnInitDialog ();

protected:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);
  
protected:
  DECLARE_MESSAGE_MAP();

private:
  CString title;

protected:
  CEdit editControl;

protected:
  CString text;
};

/* _________________________________________________________________________

   TextViewerDlgImpl::TextViewerDlgImpl
   _________________________________________________________________________ */

TextViewerDlgImpl::TextViewerDlgImpl (/*[in]*/ CWnd *		pParent,
				      /*[in]*/ const MIKTEXCHAR * lpszTitle,
				      /*[in]*/ const MIKTEXCHAR * lpszText)
  : CDialog (IDD_TEXT_VIEWER, pParent)
{
  if (lpszTitle != 0)
    {
      title = lpszTitle;
    }
  if (lpszText != 0)
    {
      text = lpszText;
    }
}

/* _________________________________________________________________________

   TextViewerDlgImpl::DoDataExchange
   _________________________________________________________________________ */

void
TextViewerDlgImpl::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_EDIT, editControl);
  DDX_Text (pDX, IDC_EDIT, text);
  editControl.SetSel (-1, -1, FALSE);
}

/* _________________________________________________________________________

   TextViewerDlgImpl Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(TextViewerDlgImpl, CDialog)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   TextViewerDlgImpl::OnInitDialog
   _________________________________________________________________________ */

BOOL
TextViewerDlgImpl::OnInitDialog ()
{
  BOOL ret = CDialog::OnInitDialog();
  SetWindowText (title);
  return (ret);
}

/* _________________________________________________________________________

   TextViewerDialog::DoModal
   _________________________________________________________________________ */

INT_PTR
TextViewerDialog::DoModal (/*[in]*/ CWnd *			pParent,
			   /*[in]*/ const MIKTEXCHAR *		lpszTitle,
			   /*[in]*/ const MIKTEXCHAR *		lpszText)
{
  TextViewerDlgImpl dlg (pParent, lpszTitle, lpszText);
  return (dlg.DoModal());
}
