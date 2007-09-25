/* PropPageFormats.cpp:

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

#include "StdAfx.h"
#include "mo.h"

#include "FormatDefinitionDialog.h"
#include "miktex/ProgressDialog.h"
#include "PropPageFormats.h"
#include "PropSheet.h"
#include "resource.hm"

/* _________________________________________________________________________

   PropPageFormats::PropPageFormats
   _________________________________________________________________________ */

PropPageFormats::PropPageFormats ()
  : CPropertyPage (PropPageFormats::IDD),
    modified (false)
{
  m_psp.dwFlags &= ~(PSP_HASHELP);
}

/* _________________________________________________________________________

   PropPageFormats::DoDataExchange
   _________________________________________________________________________ */

void
PropPageFormats::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT, editButton);
  DDX_Control(pDX, IDC_LIST, listControl);
  DDX_Control(pDX, IDC_MAKE, makeButton);
  DDX_Control(pDX, IDC_NEW, newButton);
  DDX_Control(pDX, IDC_REMOVE, removeButton);
}

/* _________________________________________________________________________

   PropPageFormats Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(PropPageFormats, CPropertyPage)
  ON_BN_CLICKED(IDC_EDIT, OnEdit)
  ON_BN_CLICKED(IDC_MAKE, OnMake)
  ON_BN_CLICKED(IDC_NEW, OnNew)
  ON_BN_CLICKED(IDC_REMOVE, OnRemove)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnSelectionChange)
  ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnListDoubleClick)
  ON_WM_CONTEXTMENU()
  ON_WM_HELPINFO()
END_MESSAGE_MAP();

/* _________________________________________________________________________

   PropPageFormats::OnMake
   _________________________________________________________________________ */

void
PropPageFormats::OnMake ()
{
  try
    {
      PropSheet * pSheet =
	reinterpret_cast<PropSheet*>(GetParent());
      auto_ptr<ProgressDialog> ppd (ProgressDialog::Create());
      ppd->StartProgressDialog (GetParent()->GetSafeHwnd());
      if (listControl.GetSelectedCount() > 1)
	{
	  ppd->SetTitle (T_("Making Format Files"));
	}
      else
	{
	  ppd->SetTitle (T_("Making Format File"));
	}
      ppd->SetLine (1, T_("Creating format file:"));
      POSITION pos = listControl.GetFirstSelectedItemPosition();
      while (pos != 0)
	{
	  int idx = listControl.GetNextSelectedItem(pos);
	  CString formatKey = listControl.GetItemText(idx, 0);
	  FormatInfo formatInfo =
	    SessionWrapper(true)->GetFormatInfo(formatKey);
	  CommandLineBuilder cmdLine;
	  cmdLine.AppendOption (T_("--dump="), formatInfo.key);
	  if (! pSheet->RunIniTeXMF(formatInfo.description.c_str(),
				    cmdLine,
				    ppd.get()))
	    {
	      // <todo/>
	    }
	}
      ppd->StopProgressDialog ();
      ppd.reset ();
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

   PropPageFormats::OnNew
   _________________________________________________________________________ */

void
PropPageFormats::OnNew ()
{
  try
    {
      FormatDefinitionDialog dlg (this, 0);
      if (dlg.DoModal() != IDOK)
	{
	  return;
	}
      FormatInfo formatInfo = dlg.GetFormatInfo();
      SessionWrapper(true)->SetFormatInfo (formatInfo);
      Refresh ();
      MakeAlias (formatInfo);
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

   PropPageFormats::MakeAlias
   _________________________________________________________________________ */

void
PropPageFormats::MakeAlias (/*[in]*/ const FormatInfo & formatInfo)
{
  if (SessionWrapper(true)->IsMiKTeXDirect())
    {
      FATAL_MIKTEX_ERROR (T_("PropPageFormats::MakeAlias"),
			  T_("Operation not supported."),
			  formatInfo.compiler.c_str());
    }
  PathName compilerPath;
  if (! SessionWrapper(true)->FindFile(formatInfo.compiler,
				       FileType::EXE,
				       compilerPath))
    {
      FATAL_MIKTEX_ERROR (T_("PropPageFormats::MakeAlias"),
			  T_("The compiler could not be found."),
			  formatInfo.compiler.c_str());
    }
  PathName pathBinDir =
    SessionWrapper(true)->GetSpecialPath(SpecialPath::BinDirectory);
  PathName pathAlias (pathBinDir, formatInfo.name, T_(".exe"));
  if (compilerPath != pathAlias && ! File::Exists(pathAlias))
    {
      File::Copy (compilerPath, pathAlias);
      if (! Fndb::FileExists(pathAlias))
	{
	  Fndb::Add (pathAlias);
	}
    }
}

/* _________________________________________________________________________

   PropPageFormats::OnEdit
   _________________________________________________________________________ */

void
PropPageFormats::OnEdit ()
{
  try
    {
      int idx = GetSelectedItem();
      CString formatKey = listControl.GetItemText(idx, 0);
      FormatDefinitionDialog dlg (this, formatKey);
      if (dlg.DoModal() != IDOK)
	{
	  return;
	}
      FormatInfo formatInfo = dlg.GetFormatInfo();
      if (PathName::Compare(formatInfo.key.c_str(), formatKey) != 0)
	{
	  // rename key: delete old, create new
	  SessionWrapper(true)->DeleteFormatInfo (formatKey);
	  formatKey = formatInfo.key.c_str();
	}
      SessionWrapper(true)->SetFormatInfo (formatInfo);
      Refresh ();
      MakeAlias (formatInfo);
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

   PropPageFormats::OnRemove
   _________________________________________________________________________ */

void
PropPageFormats::OnRemove ()
{
  try
    {
      int n = listControl.GetSelectedCount();
      for (int i = 0; i < n; ++i)
	{
	  int idx = GetSelectedItem();
	  CString formatKey = listControl.GetItemText(idx, 0);
	  SessionWrapper(true)->DeleteFormatInfo (formatKey);
	  if (! listControl.DeleteItem(idx))
	    {
	      FATAL_WINDOWS_ERROR (T_("CListCtrl::DeleteItem"), 0);
	    }
	}
      EnableButtons ();
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

   PropPageFormats::InsertColumn
   _________________________________________________________________________ */

void
PropPageFormats::InsertColumn (/*[in]*/ int			colIdx,
			       /*[in]*/ const MIKTEXCHAR *	lpszLabel,
			       /*[in]*/ const MIKTEXCHAR *	lpszLongest)
{
  if (listControl.InsertColumn(colIdx,
			       lpszLabel,
			       LVCFMT_LEFT,
			       listControl.GetStringWidth(lpszLongest),
			       colIdx)
      < 0)
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::InsertColumn"), 0);
    }
}

/* _________________________________________________________________________

   PropPageFormats::OnInitDialog
   _________________________________________________________________________ */

BOOL
PropPageFormats::OnInitDialog () 
{
  BOOL ret = CPropertyPage::OnInitDialog();

  try
    {
      listControl.SetExtendedStyle (listControl.GetExtendedStyle()
				    | LVS_EX_FULLROWSELECT);

      InsertColumn (0, T_("Key"), T_("pdfjadetex     "));
      InsertColumn (1, T_("Description"), T_("pdfLaTeX bla bla     "));
      InsertColumn (2, T_("Attributes"), T_("Exclude, bla bla  "));
      
      Refresh ();
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }

  return (ret);
}

/* _________________________________________________________________________

   PropPageFormats::EnableButtons
   _________________________________________________________________________ */

void
PropPageFormats::EnableButtons ()
{
  UINT n = listControl.GetSelectedCount();
  makeButton.EnableWindow (n > 0 && ! modified);
  removeButton.EnableWindow (n > 0);
  editButton.EnableWindow (n == 1);
}

/* _________________________________________________________________________

   PropPageFormats::GetSelectedItem
   _________________________________________________________________________ */

int
PropPageFormats::GetSelectedItem ()
{
  POSITION pos = listControl.GetFirstSelectedItemPosition();
  if (pos == 0)
    {
      UNEXPECTED_CONDITION (T_("PropPageFormats::GetSelectedItem"));
    }
  return (listControl.GetNextSelectedItem(pos));
}

/* _________________________________________________________________________

   PropPageFormats::Refresh
   _________________________________________________________________________ */

void
PropPageFormats::Refresh ()
{
  if (! listControl.DeleteAllItems())
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::DeleteAllItems"), 0);
    }

  FormatInfo formatInfo;

  for (unsigned idx = 0;
       SessionWrapper(true)->GetFormatInfo(idx, formatInfo);
       ++ idx)
    {
      LV_ITEM lvitem;
      lvitem.iItem = static_cast<int>(idx);
      lvitem.mask = LVIF_TEXT | LVIF_PARAM;
      lvitem.iSubItem = 0;
      lvitem.pszText = const_cast<MIKTEXCHAR*>(formatInfo.key.c_str());
      lvitem.lParam = idx;
      int whereIndex = listControl.InsertItem(&lvitem);
      if (whereIndex < 0)
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::InsertItem"), 0);
	}
      lvitem.iItem = whereIndex;
      lvitem.mask = LVIF_TEXT;
      lvitem.iSubItem = 1;
      lvitem.pszText = const_cast<MIKTEXCHAR*>(formatInfo.description.c_str());
      if (! listControl.SetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
	}
      lvitem.mask = LVIF_TEXT;
      lvitem.iSubItem = 2;
      lvitem.pszText = (formatInfo.exclude ? T_("exclude") : T_(""));
      if (! listControl.SetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
	}
    }
  
  EnableButtons ();
}

/* _________________________________________________________________________

   aHelpIDs
   _________________________________________________________________________ */

#define MAKE_ID_HID_PAIR(id) id, H##id

namespace
{
  const DWORD aHelpIDs[] = {
    MAKE_ID_HID_PAIR(IDC_EDIT),
    MAKE_ID_HID_PAIR(IDC_LIST),
    MAKE_ID_HID_PAIR(IDC_MAKE),
    MAKE_ID_HID_PAIR(IDC_NEW),
    MAKE_ID_HID_PAIR(IDC_REMOVE),
    0, 0,
  };
}

/* _________________________________________________________________________

   PropPageFormats::OnHelpInfo
   _________________________________________________________________________ */

BOOL
PropPageFormats::OnHelpInfo (/*[in]*/ HELPINFO * pHelpInfo) 
{
  return (::OnHelpInfo(pHelpInfo, aHelpIDs, T_("FormatsPage.txt")));
}

/* _________________________________________________________________________

   PropPageFormats::OnContextMenu
   _________________________________________________________________________ */

void
PropPageFormats::OnContextMenu (/*[in]*/ CWnd *	pWnd,
				/*[in]*/ CPoint	point) 
{
  try
    {
      DoWhatsThisMenu (pWnd, point, aHelpIDs, T_("FormatsPage.txt"));
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

   PropPageFormats::OnSelectionChange
   _________________________________________________________________________ */

void
PropPageFormats::OnSelectionChange (/*[in]*/ NMHDR *	pNMHDR,
				    /*[in]*/ LRESULT *	pResult) 
{
  UNUSED_ALWAYS (pNMHDR);
  try
    {
      EnableButtons ();
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  *pResult = 0;
}

/* _________________________________________________________________________

   PropPageFormats::OnListDoubleClick
   _________________________________________________________________________ */

void
PropPageFormats::OnListDoubleClick (/*[in]*/ NMHDR *	pNMHDR,
				    /*[in]*/ LRESULT *	pResult)
{
  UNUSED_ALWAYS (pNMHDR);
  try
    {
      OnEdit ();
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  *pResult = 0;
}
