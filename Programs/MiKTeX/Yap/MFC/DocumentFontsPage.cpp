/* DocumentFontsPage.cpp:

   Copyright (C) 1996-2006 Christian Schenk

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

#include "DocumentFontsPage.h"

/* _________________________________________________________________________

   DocumentFontsPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(DocumentFontsPage, CPropertyPage)
  ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClick)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   DocumentFontsPage::DocumentFontsPage
   _________________________________________________________________________ */

DocumentFontsPage::DocumentFontsPage ()
  : CPropertyPage(IDD_DOC_PROP_FONTS)
{
}

/* _________________________________________________________________________

   DocumentFontsPage::DoDataExchange
   _________________________________________________________________________ */

void
DocumentFontsPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_LIST, listControl);
}

/* _________________________________________________________________________

   DocumentFontsPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
DocumentFontsPage::OnInitDialog ()
{
  BOOL ret = CPropertyPage::OnInitDialog();

  try
    {
      listControl.SetExtendedStyle (listControl.GetExtendedStyle()
				   | LVS_EX_FULLROWSELECT);

      if (listControl.InsertColumn(0,
				  T_("Name"),
				  LVCFMT_LEFT,
				  listControl.GetStringWidth(T_("xxx \
cmbxti12")),
				  0)
	  < 0)
	{
	  UNEXPECTED_CONDITION ("DocumentFontsPage::OnInitDialog");
	}

      if (listControl.InsertColumn(1,
				  T_("File"),
				  LVCFMT_LEFT,
				  listControl.GetStringWidth(T_("xxx \
C:\\texmf\\fonts\\pk\\ljfour\\public\\cmextra\\dpi600\\cmbxti12.pk")),
				  1)
	  < 0)
	{
	  UNEXPECTED_CONDITION ("DocumentFontsPage::OnInitDialog");
	}
	  
      int idx = 0;

      for (vector<DviFontInfo>::const_iterator it = fontTable.begin();
	   it != fontTable.end();
	   ++ it, ++ idx)
	{
	  LVITEM lvitem;
	  lvitem.iItem = idx;
	  lvitem.mask = LVIF_TEXT;
	  lvitem.iSubItem = 0;
	  lvitem.pszText = const_cast<char*>(it->name.c_str());
	  if (listControl.InsertItem(&lvitem) < 0)
	    {
	      UNEXPECTED_CONDITION ("DocumentFontsPage::OnInitDialog");
	    }
	  lvitem.iSubItem = 1;
	  lvitem.pszText =
	    (it->notLoadable
	     ? T_("   >>> Not loadable - double-click to see details")
	     : const_cast<char*>(it->fileName.c_str()));
	  if (! listControl.SetItem(&lvitem))
	    {
	      UNEXPECTED_CONDITION ("DocumentFontsPage::OnInitDialog");
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

   DocumentFontsPage::OnDoubleClick
   _________________________________________________________________________ */

void
DocumentFontsPage::OnDoubleClick (/*[in]*/ NMHDR *	pNMHDR,
				  /*[in]*/ LRESULT *	pResult)
{
  UNUSED_ALWAYS (pNMHDR);

  try
    {
      int idx = listControl.GetNextItem(-1, LVNI_SELECTED);
      if (idx < 0)
	{
	  UNEXPECTED_CONDITION ("DocumentFontsPage::OnDoubleClick");
	}
      MIKTEX_ASSERT (idx < static_cast<int>(fontTable.size()));
      const DviFontInfo & info = fontTable[idx];
      if (! info.notLoadable)
	{
	  return;
	}
      CString title;
      title.Format (T_("%s Logbook"), info.name.c_str());
      TextViewerDialog::DoModal (this, title, info.transcript.c_str());
      *pResult = 0;
    }

  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }

  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
}
