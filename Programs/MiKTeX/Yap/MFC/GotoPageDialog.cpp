/* GotoPageDialog.cpp:

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

#include "StdAfx.h"

#include "yap.h"

#include "DviDoc.h"
#include "GotoPageDialog.h"

/* _________________________________________________________________________

   GotoPageDialog Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(GotoPageDialog, CDialog)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   GotoPageDialog::GotoPageDialog
   _________________________________________________________________________ */

GotoPageDialog::GotoPageDialog (/*[in]*/ CWnd *		pParent,
				/*[in]*/ DviDoc *	pDoc,
				/*[in]*/ int		pageIdx)
  : CDialog (IDD, pParent),
    pDviDoc (pDoc),
    pageIdx (pageIdx)
{
}

/* _________________________________________________________________________

   GotoPageDialog::OnInitDialog
   _________________________________________________________________________ */

BOOL
GotoPageDialog::OnInitDialog ()
{
  BOOL ret = CDialog::OnInitDialog();
  try
    {
      for (int pageIdx = 0; pageIdx < pDviDoc->GetPageCount(); ++ pageIdx)
	{
	  pageComboBox.AddString (UT_(pDviDoc->GetPageName(pageIdx)));
	}
      if (this->pageIdx >= 0)
	{
	  MIKTEX_ASSERT (this->pageIdx < pageComboBox.GetCount());
	  if (pageComboBox.SetCurSel(this->pageIdx) < 0)
	    {
	      UNEXPECTED_CONDITION ("GotoPageDialog::OnInitDialog");
	    }
	}
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (0, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (0, e);
    }
  return (ret);
}

/* _________________________________________________________________________

   GotoPageDialog::DoDataExchange
   _________________________________________________________________________ */

void GotoPageDialog::DoDataExchange (CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_COMBO_PAGENO, pageComboBox);
  DDX_CBIndex (pDX, IDC_COMBO_PAGENO, pageIdx);
}

