/* PropPageLanguages.cpp:

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

#include "StdAfx.h"
#include "mo.h"

#include "LanguageDefinitionDialog.h"
#include "PropPageLanguages.h"
#include "PropSheet.h"
#include "resource.hm"

IMPLEMENT_DYNCREATE(PropPageLanguages, CPropertyPage);

/* _________________________________________________________________________

   PropPageLanguages::PropPageLanguages
   _________________________________________________________________________ */

PropPageLanguages::PropPageLanguages ()
  : CPropertyPage (PropPageLanguages::IDD),
    inserting (false)
{
  m_psp.dwFlags &= ~(PSP_HASHELP);
}

/* _________________________________________________________________________

   PropPageLanguages::~PropPageLanguages
   _________________________________________________________________________ */

PropPageLanguages::~PropPageLanguages ()
{
}

/* _________________________________________________________________________

   PropPageLanguages::DoDataExchange
   _________________________________________________________________________ */

void
PropPageLanguages::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  DDX_Control (pDX, IDC_EDIT, editButton);
  DDX_Control (pDX, IDC_LIST, listControl);
  DDX_Control (pDX, IDC_MOVEDOWN, downButton);
  DDX_Control (pDX, IDC_MOVEUP, upButton);
  DDX_Control (pDX, IDC_REMOVE, removeButton);
}

/* _________________________________________________________________________

   PropPageLanguages Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP (PropPageLanguages, CPropertyPage)
  ON_BN_CLICKED (IDC_EDIT, OnEdit)
  ON_BN_CLICKED (IDC_MOVEDOWN, OnMoveDown)
  ON_BN_CLICKED (IDC_MOVEUP, OnMoveUp)
  ON_BN_CLICKED (IDC_NEW, OnNew)
  ON_BN_CLICKED (IDC_REMOVE, OnRemove)
  ON_NOTIFY (LVN_ITEMCHANGED, IDC_LIST, OnItemchangedList)
  ON_NOTIFY (NM_DBLCLK, IDC_LIST, OnDblclkList)
  ON_WM_CONTEXTMENU ()
  ON_WM_HELPINFO ()
END_MESSAGE_MAP ()

/* _________________________________________________________________________

   PropPageLanguages::InsertColumn
   _________________________________________________________________________ */

void
PropPageLanguages::InsertColumn (/*[in]*/ int			colIdx,
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

   PropPageLanguages::OnInitDialog
   _________________________________________________________________________ */

BOOL
PropPageLanguages::OnInitDialog () 
{
  BOOL ret = CPropertyPage::OnInitDialog();

  try
    {
      
      listControl.SetExtendedStyle (listControl.GetExtendedStyle()
				    | LVS_EX_FULLROWSELECT
				    | (HasIE3() ? LVS_EX_CHECKBOXES : 0));

      int colIdx = 0;

      InsertColumn (colIdx++, T_("Language"), T_("xxxx portuguese"));

      if (! HasIE3())
	{
	  InsertColumn (colIdx++, T_("State"), T_("xxxx Active"));
	}

      InsertColumn (colIdx++,
		    T_("Hyphenation Table"),
		    T_("xxxx nehyph96.tex"));

      InsertColumn (colIdx++, T_("Synonyms"), T_("xxxx patois, francais"));

      ReadLanguageDat ();
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

   PropPageLanguages::OnApply
   _________________________________________________________________________ */

BOOL
PropPageLanguages::OnApply ()
{
  try
    {
      PathName root;
      PathName relative;
      if (SessionWrapper(true)->SplitTEXMFPath(languageDat, root, relative)
	  == INVALID_ROOT_INDEX)
	{
	  UNEXPECTED_CONDITION (T_("PropPageLanguages::OnApply"));
	}
      PathName localRoot =
	SessionWrapper(true)->GetSpecialPath(SpecialPath::ConfigRoot);
      if (PathName::Compare(root, localRoot) != 0)
	{
	  languageDat.Set (localRoot, relative);
	  PathName dir (languageDat);
	  dir.RemoveFileSpec ();
	  if (! Directory::Exists(dir))
	    {
	      Directory::Create (dir);
	    }
	}
      StreamWriter writer (languageDat);
      writer.WriteLine (T_("%%% Created by MiKTeX Options---do not edit!"));
      for (vector<LANGUAGE>::const_iterator it = languages.begin();
	   it != languages.end();
	   ++it)
	{
	  writer.WriteFormattedLine
	    (T_("%s%s %s"),
	     it->active ? T_("") : T_("%"),
	     static_cast<const MIKTEXCHAR *>(it->languageName),
	     static_cast<const MIKTEXCHAR *>(it->fileName));
	  for (vector<CString>::const_iterator it2 = it->synonyms.begin();
	       it2 != it->synonyms.end();
	       ++it2)
	    {
	      writer.WriteFormattedLine
		(T_("=%s"),
		 static_cast<const MIKTEXCHAR *>(*it2));
	    }
	}
      writer.Close ();
      if (! Fndb::FileExists(languageDat))
	{
	  Fndb::Add (languageDat);
	}
      return (TRUE);
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
      return (FALSE);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
      return (FALSE);
    }
}

/* _________________________________________________________________________

   aHelpIDs
   _________________________________________________________________________ */

#define MAKE_ID_HID_PAIR(id) id, H##id

namespace
{
  const DWORD aHelpIDs[] = {
    MAKE_ID_HID_PAIR(IDC_NEW),
    MAKE_ID_HID_PAIR(IDC_REMOVE),
    MAKE_ID_HID_PAIR(IDC_EDIT),
    MAKE_ID_HID_PAIR(IDC_LIST),
    0, 0,
  };
}

/* _________________________________________________________________________

   PropPageLanguages::OnHelpInfo
   _________________________________________________________________________ */

BOOL
PropPageLanguages::OnHelpInfo (/*[in]*/ HELPINFO * pHelpInfo) 
{
  return (::OnHelpInfo(pHelpInfo, aHelpIDs, T_("LanguagesPage.txt")));
}

/* _________________________________________________________________________

   PropPageLanguages::OnContextMenu
   _________________________________________________________________________ */

void
PropPageLanguages::OnContextMenu (/*[in]*/ CWnd *	pWnd,
				   /*[in]*/ CPoint	point) 
{
  try
    {
      DoWhatsThisMenu (pWnd, point, aHelpIDs, T_("LanguagesPage.txt"));
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

   PropPageLanguages::ReadLanguageDat
   _________________________________________________________________________ */

void
PropPageLanguages::ReadLanguageDat ()
{
  if (! SessionWrapper(true)->FindFile(T_("language.dat"),
				       FileType::TEX,
				       languageDat))
    {
      FATAL_MIKTEX_ERROR (T_("PropPageLanguages::ReadLanguageDat"),
			  T_("\
The language definition file (languages.dat) could not be found."),
			  0);
      return;
    }
  StreamReader reader (languageDat);
  tstring line;
  size_t idx = 0;
  while (reader.ReadLine(line))
    {
      if (line.empty())
	{
	  continue;
	}
      bool active = true;
      if (line[0] == T_('%'))
	{
	  line.erase (0, 1);
	  active = false;
	}
      if (line[0] == T_('='))
	{
	  if (idx > 0)
	    {
	      Tokenizer tok (line.c_str() + 1, T_(" \t\n"));
	      if (tok.GetCurrent() != 0)
		{
		  LANGUAGE & lang = languages[idx - 1];
		  lang.synonyms.push_back (tok.GetCurrent());
		}
	    }
	}
      else
	{
	  Tokenizer tok (line.c_str(), T_(" \t\n"));
	  const MIKTEXCHAR * lpsz = tok.GetCurrent();
	  if (lpsz != 0 && *lpsz != T_('%'))
	    {
	      ++ tok;
	      const MIKTEXCHAR * lpsz2 = tok.GetCurrent();
	      if (lpsz2 != 0)
		{
		  LANGUAGE lang;
		  lang.languageName = lpsz;
		  lang.fileName = lpsz2;
		  lang.active = active;
		  languages.push_back (lang);
		  ++ idx;
		}
	    }
	}
    }
  reader.Close ();
}

/* _________________________________________________________________________

   PropPageLanguages::InsertLanguage
   _________________________________________________________________________ */

void
PropPageLanguages::InsertLanguage (/*[in]*/ int idx)
{
  LV_ITEM lvitem;

  lvitem.iItem = idx;
  lvitem.mask = LVIF_PARAM;
  lvitem.lParam = idx;
  lvitem.iSubItem = 0;

  inserting = true;
  if (listControl.InsertItem(&lvitem) < 0)
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::InsertItem"), 0);
    }
  inserting = false;

  RefreshRow (idx);
}

/* _________________________________________________________________________

   PropPageLanguages::Refresh
   _________________________________________________________________________ */

void
PropPageLanguages::Refresh ()
{
  if (listControl.DeleteAllItems() < 0)
    {
       FATAL_WINDOWS_ERROR (T_("CListCtrl::DeleteAllItems"), 0);
    }
    
  int idx = 0;

  for (vector<LANGUAGE>::const_iterator it = languages.begin();
       it != languages.end();
       ++it, ++idx)
    {
      InsertLanguage (idx);
    }

  EnableButtons ();
}

/* _________________________________________________________________________

   PropPageLanguages::OnEdit
   _________________________________________________________________________ */

void
PropPageLanguages::OnEdit ()
{
  try
    {
      int idx = GetSelectedItem();
      LanguageDefinitionDialog dlg (this, languages[idx]);
      if (dlg.DoModal() == IDOK)
	{
	  languages[idx] = dlg.GetLanguage();
	  RefreshRow (idx);
	}
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

   PropPageLanguages::OnRemove
   _________________________________________________________________________ */

void
PropPageLanguages::OnRemove ()
{
  try
    {
      UINT n = listControl.GetSelectedCount();
      for (UINT i = 0; i < n; ++i)
	{
	  int idx = GetSelectedItem();
	  if (! listControl.DeleteItem(idx))
	    {
	      FATAL_WINDOWS_ERROR (T_("CListCtrl::DeleteItem"), 0);
	    }
	  languages.erase (languages.begin() + idx);
	}
      EnableButtons ();
      SetModified ();
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

   PropPageLanguages::OnNew
   _________________________________________________________________________ */

void
PropPageLanguages::OnNew ()
{
  try
    {
      LANGUAGE lang;
      LanguageDefinitionDialog dlg (this, lang);
      if (dlg.DoModal() == IDOK)
	{
	  lang = dlg.GetLanguage();
	  languages.push_back (lang);
	  InsertLanguage (static_cast<int>(languages.size() - 1));
	  SetModified ();
	}
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

   PropPageLanguages::OnDblclkList
   _________________________________________________________________________ */

void
PropPageLanguages::OnDblclkList (/*[in]*/ NMHDR *	pNMHDR,
				 /*[in]*/ LRESULT *	pResult)
{
  UNUSED_ALWAYS (pNMHDR);
  *pResult = 0;
}

/* _________________________________________________________________________

   PropPageLanguages::GetSelectedItem
   _________________________________________________________________________ */

int
PropPageLanguages::GetSelectedItem ()
{
  POSITION pos = listControl.GetFirstSelectedItemPosition();
  if (pos == 0)
    {
      UNEXPECTED_CONDITION (T_("PropPageLanguages::GetSelectedItem"));
    }
  return (listControl.GetNextSelectedItem(pos));
}

/* _________________________________________________________________________

   PropPageLanguages::OnItemchangedList
   _________________________________________________________________________ */

void
PropPageLanguages::OnItemchangedList (/*[in]*/ NMHDR *		pNMHDR,
				      /*[in]*/ LRESULT *	pResult)
{
  *pResult = 0;
  if (inserting)
    {
      return;
    }
  try
    {
      LPNMLISTVIEW pnmv = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
      if (HasIE3() && pnmv->iItem >= 0 && (pnmv->uChanged & LVIF_STATE))
	{
	  bool b = (listControl.GetCheck(pnmv->iItem) ? true : false);
	  if (languages[pnmv->iItem].active != b)
	    {
	      languages[pnmv->iItem].active = b;
	      SetModified ();
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

   PropPageLanguages::RefreshRow
   _________________________________________________________________________ */

void
PropPageLanguages::RefreshRow (/*[in]*/ int idx)
{
  const LANGUAGE lang = languages[idx];

  int colIdx = 0;

  if (! listControl.SetItemText(idx, colIdx++, lang.languageName))
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItemText"), 0);
    }
  
  if (! HasIE3())
    {
      if (! listControl.SetItemText(idx,
				    colIdx++,
				    lang.active ? T_("Active") : T_("")))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItemText"), 0);
	}
    }

  if (! listControl.SetItemText(idx, colIdx++, lang.fileName))
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItemText"), 0);
    }

  CString synonyms;

  for (vector<CString>::const_iterator it = lang.synonyms.begin();
       it != lang.synonyms.end();
       ++it)
    {
      if (synonyms.GetLength() > 0)
	{
	  synonyms += T_(",");
	}
      synonyms += *it;
    }

  if (! listControl.SetItemText(idx, colIdx++, synonyms))
    {
      FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItemText"), 0);
    }

  if (HasIE3())
    {
      listControl.SetCheck (idx, lang.active ? TRUE : FALSE);
    }
}

/* _________________________________________________________________________

   CompareItems
   _________________________________________________________________________ */

static
int
CALLBACK 
CompareItems (/*[in]*/ LPARAM	lParam1,
	      /*[in]*/ LPARAM	lParam2,
	      /*[in]*/ LPARAM	lParamSort)
{
  UNUSED_ALWAYS (lParamSort);
  return (static_cast<int>(lParam1 - lParam2));
}

/* _________________________________________________________________________

   PropPageLanguages::OnMoveDown
   _________________________________________________________________________ */

void
PropPageLanguages::OnMoveDown ()
{
  try
    {
      int idx = GetSelectedItem();

      MIKTEX_ASSERT (idx + 1 < listControl.GetItemCount());

      swap (languages[idx], languages[idx+1]);

      LVITEM lvitem;
      lvitem.mask = LVIF_PARAM;
      lvitem.iSubItem = 0;

      lvitem.iItem = idx;
      if (! listControl.GetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::GetItem"), 0);
	}
      lvitem.lParam = idx + 1;
      if (! listControl.SetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
	}

      lvitem.iItem = idx + 1;
      if (! listControl.GetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::GetItem"), 0);
	}
      lvitem.lParam = idx;
      if (! listControl.SetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
	}

      if (! listControl.SortItems(CompareItems, 0))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SortItems"), 0);
	}

      EnableButtons ();
      SetModified ();
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

   PropPageLanguages::OnMoveUp
   _________________________________________________________________________ */

void
PropPageLanguages::OnMoveUp ()
{
  try
    {
      int idx = GetSelectedItem();
      
      MIKTEX_ASSERT (idx > 0);

      swap (languages[idx - 1], languages[idx]);

      LVITEM lvitem;
      lvitem.mask = LVIF_PARAM;
      lvitem.iSubItem = 0;
      
      lvitem.iItem = idx - 1;
      if (listControl.GetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::GetItem"), 0);
	}
      lvitem.lParam = idx;
      if (listControl.SetItem(&lvitem))
  	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
	}
    
      lvitem.iItem = idx;
      if (listControl.GetItem(&lvitem))
	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::GetItem"), 0);
	}
      lvitem.lParam = idx - 1;
      if (listControl.SetItem(&lvitem))
  	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SetItem"), 0);
	}
      
      if (listControl.SortItems(CompareItems, 0))
        	{
	  FATAL_WINDOWS_ERROR (T_("CListCtrl::SortItems"), 0);
	}

      EnableButtons ();
      SetModified ();
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

   PropPageLanguages::EnableButtons
   _________________________________________________________________________ */

void
PropPageLanguages::EnableButtons ()
{
  int itemCount = listControl.GetItemCount();
  int selectedCount = listControl.GetSelectedCount();
  int firstSelected = 0;
  if (selectedCount > 0)
    {
      POSITION pos = listControl.GetFirstSelectedItemPosition();
      if (pos == 0)
	{
	  UNEXPECTED_CONDITION (T_("PropPageLanguages::EnableButtons"));
	}
      firstSelected = listControl.GetNextSelectedItem(pos);
    }
  upButton.EnableWindow (selectedCount == 1 && firstSelected > 0);
  downButton.EnableWindow (selectedCount == 1
			   && firstSelected + 1 < itemCount);
  removeButton.EnableWindow (selectedCount > 0);
  editButton.EnableWindow (selectedCount == 1);
}

/* _________________________________________________________________________

   PropPageLanguages::SetModified
   _________________________________________________________________________ */

void
PropPageLanguages::SetModified ()
{
  PropSheet * pSheet =
    reinterpret_cast<PropSheet*>(GetParent());
  pSheet->ScheduleBuildFormats ();
  CPropertyPage::SetModified (TRUE);
}
