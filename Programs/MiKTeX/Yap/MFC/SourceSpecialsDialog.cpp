/* SourceSpecialsDialog.cpp:

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
#include "Dviview.h"
#include "ProgressIndicatorDialog.h"
#include "SourceSpecialsDialog.h"

/* _________________________________________________________________________

   SourceSpecialsDialog Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(SourceSpecialsDialog, CDialog)
  ON_BN_CLICKED(IDC_CLOSE, OnClose)
  ON_BN_CLICKED(IDC_EDIT_SOURCE, OnEditSource)
  ON_BN_CLICKED(IDC_GOTO, OnGoto)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_SOURCE_SPECIALS, OnItemchangedSourceSpecials)
  ON_NOTIFY(NM_DBLCLK, IDC_SOURCE_SPECIALS, OnDblclkSourceSpecials)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   SourceSpecialsDialog::SourceSpecialsDialog
   _________________________________________________________________________ */

SourceSpecialsDialog::SourceSpecialsDialog (/*[in]*/ CWnd *	pParent,
					    /*[in]*/ DviDoc *	pDoc)
  : CDialog (IDD, pParent),
    pDviDoc (pDoc),
    pView (reinterpret_cast<DviView*>(pParent))
{
  ProgressIndicatorDialog pi;
  pi.Create (IDD_PROGRESS_INDICATOR);
  pi.progressBar.SetRange (0, static_cast<short>(pDviDoc->GetPageCount()));
  
  for (int p = 0; p < pDviDoc->GetPageCount(); ++ p)
    {
      pi.progressBar.SetPos (p);
      
      DviPage * pDviPage = pDviDoc->GetLoadedPage(p);
      
      if (pDviPage == 0)
	{
	  UNEXPECTED_CONDITION
	    ("SourceSpecialsDialog::SourceSpecialsDialog");
	}
      
      AutoUnlockPage autoUnlockPage (pDviPage);
      
      DviSpecial * pDviSpecial;
      int j = 0;
      while ((pDviSpecial = pDviPage->GetSpecial(j++)) != 0)
	{
	  SourceSpecial * pSrcSpecial;
	  pSrcSpecial = dynamic_cast<SourceSpecial *>(pDviSpecial);
	  if (pSrcSpecial != 0)
	    {
	      SrcSpecial s;
	      s.pageName = pDviPage->GetName();
	      s.fileName = pSrcSpecial->GetFileName();
	      s.line = pSrcSpecial->GetLineNum();
	      sourceSpecials.push_back (s);
	    }
	}
    }
  
  if (sourceSpecials.size() == 0)
    {
      FATAL_MIKTEX_ERROR ("SourceSpecialsDialog::SourceSpecialsDialog",
			  T_("The document contains no source links."),
			  0);
    }
}

/* _________________________________________________________________________

   SourceSpecialsDialog::DoDataExchange
   _________________________________________________________________________ */

void
SourceSpecialsDialog::DoDataExchange /*[in]*/ (CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);

  DDX_Control (pDX, IDC_EDIT_SOURCE, editButton);
  DDX_Control (pDX, IDC_GOTO, goToButton);
  DDX_Control (pDX, IDC_SOURCE_SPECIALS, listControl);
}

/* _________________________________________________________________________

   SourceSpecialsDialog::OnInitDialog
   _________________________________________________________________________ */

BOOL
SourceSpecialsDialog::OnInitDialog () 
{
  BOOL ret = CDialog::OnInitDialog();

  try
    {
      CenterWindow ();
      
      listControl.SetExtendedStyle (listControl.GetExtendedStyle()
				    | LVS_EX_FULLROWSELECT);
      
      if (listControl.InsertColumn(0,
				   T_("Page"),
				   LVCFMT_LEFT,
				   listControl.GetStringWidth(T_("\
  Page  ")),
				   0)
	  < 0)
	{
	  UNEXPECTED_CONDITION ("SourceSpecialsDialog::OnInitDialog");
	}

      if (listControl.InsertColumn(1,
				   T_("Source File"),
				   LVCFMT_LEFT,
				   listControl.GetStringWidth(T_("\
  abrakadabra.tex  ")),
				   1)
	  < 0)
	{
	  UNEXPECTED_CONDITION ("SourceSpecialsDialog::OnInitDialog");
	}

      if (listControl.InsertColumn(2,
				   T_("Line"),
				   LVCFMT_LEFT,
				   listControl.GetStringWidth(T_("\
  99999  ")),
				   2)
	  < 0)
	{
	  UNEXPECTED_CONDITION ("SourceSpecialsDialog::OnInitDialog");
	}

      int idx = 0;

      for (vector<SrcSpecial>::const_iterator it = sourceSpecials.begin();
	   it != sourceSpecials.end();
	   ++ it, ++ idx)
	{
	  LV_ITEM lvitem;
	  lvitem.iItem = idx;
	  lvitem.mask = LVIF_TEXT;
	  lvitem.iSubItem = 0;
	  lvitem.pszText = const_cast<char *>(static_cast<const char *>(it->pageName.c_str()));
	  if (listControl.InsertItem(&lvitem) < 0)
	    {
	      UNEXPECTED_CONDITION ("SourceSpecialsDialog::OnInitDialog");
	    }
	  lvitem.iSubItem = 1;
	  lvitem.pszText = const_cast<char *>(it->fileName.Get());
	  if (! listControl.SetItem(&lvitem))
	    {
	      UNEXPECTED_CONDITION ("SourceSpecialsDialog::OnInitDialog");
	    }
	  lvitem.iSubItem = 2;
	  lvitem.pszText = const_cast<char *>(NUMTOSTR(it->line));
	  if (! listControl.SetItem(&lvitem))
	    {
	      UNEXPECTED_CONDITION ("SourceSpecialsDialog::OnInitDialog");
	    }
	}

      goToButton.EnableWindow (FALSE);
      editButton.EnableWindow (FALSE);
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

   SourceSpecialsDialog::OnGoto
   _________________________________________________________________________ */

void
SourceSpecialsDialog::OnGoto () 

{
  try
    {
      int idx = listControl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
      MIKTEX_ASSERT (idx >= 0);
      LV_ITEM item;
      char szPageNum[BufferSizes::MaxPath];
      char szLineNum[BufferSizes::MaxPath];
      char szFileName[BufferSizes::MaxPath];
      item.mask = LVIF_TEXT;
      item.cchTextMax = BufferSizes::MaxPath;
      item.iItem = idx;
      item.iSubItem = 0;
      item.pszText = szPageNum;
      if (! listControl.GetItem(&item))
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::GetItem", 0);
	}
      item.iSubItem = 1;
      item.pszText = szFileName;
      if (! listControl.GetItem(&item))
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::GetItem", 0);
	}
      item.iSubItem = 2;
      item.pszText = szLineNum;
      if (! listControl.GetItem(&item))
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::GetItem", 0);
	}
      pView->GotoSrcSpecial (atoi(szLineNum), szFileName);
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

/* _________________________________________________________________________

   SourceSpecialsDialog::OnItemchangedSourceSpecials
   _________________________________________________________________________ */

void
SourceSpecialsDialog::OnItemchangedSourceSpecials (/*[in]*/ NMHDR * pNMHDR,
						   /*[in]*/ LRESULT * pResult) 

{
  UNUSED_ALWAYS (pNMHDR);
  bool en = (listControl.GetSelectedCount() > 0);
  goToButton.EnableWindow (en);
  editButton.EnableWindow (en);
  *pResult = 0;
}

/* _________________________________________________________________________

   SourceSpecialsDialog::OnEditSource
   _________________________________________________________________________ */

void
SourceSpecialsDialog::OnEditSource ()
{
  try
    {
      int idx = listControl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
      MIKTEX_ASSERT (idx >= 0);
      LV_ITEM item;
      char szFileName[BufferSizes::MaxPath];
      char szLineNum[BufferSizes::MaxPath];
      item.mask = LVIF_TEXT;
      item.cchTextMax = BufferSizes::MaxPath;
      item.iItem = idx;
      item.iSubItem = 1;
      item.pszText = szFileName;
      if (! listControl.GetItem(&item))
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::GetItem", 0);
	}
      item.iSubItem = 2;
      item.pszText = szLineNum;
      if (! listControl.GetItem(&item))
	{
	  FATAL_WINDOWS_ERROR ("CListCtrl::GetItem", 0);
	}
      StartEditor (szFileName, pDviDoc->GetDocDir().Get(), atoi(szLineNum));
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

/* _________________________________________________________________________

   SourceSpecialsDialog::OnClose
   _________________________________________________________________________ */

void
SourceSpecialsDialog::OnClose () 
{
  try
    {
      pView->CloseSourceSpecialsDialog ();
      DestroyWindow ();
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

/* _________________________________________________________________________

   SourceSpecialsDialog::PostNcDestroy
   _________________________________________________________________________ */

void
SourceSpecialsDialog::PostNcDestroy () 
{
  CDialog::PostNcDestroy ();
  delete this;
}

/* _________________________________________________________________________

   SourceSpecialsDialog::OnCancel
   _________________________________________________________________________ */

void
SourceSpecialsDialog::OnCancel ()
{
  OnClose ();
}

/* _________________________________________________________________________

   SourceSpecialsDialog::OnDblclkSourceSpecials
   _________________________________________________________________________ */

void
SourceSpecialsDialog::OnDblclkSourceSpecials (/*[in]*/ NMHDR *	pNMHDR,
					      /*[out]*/ LRESULT * pResult)
{
  UNUSED_ALWAYS (pNMHDR);
  try
    {
      OnGoto ();
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
