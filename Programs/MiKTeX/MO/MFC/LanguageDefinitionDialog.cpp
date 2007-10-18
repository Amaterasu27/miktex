/* LanguageDefinitionDialog.cpp:

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
#include "resource.hm"

/* _________________________________________________________________________

   LanguageDefinitionDialog::LanguageDefinitionDialog
   _________________________________________________________________________ */

LanguageDefinitionDialog::LanguageDefinitionDialog (/*[in]*/ CWnd * pParent,
				/*[in]*/ const LANGUAGE &	   lang)
  : CDialog (LanguageDefinitionDialog::IDD, pParent),
    active (FALSE),
    language (lang)
{
  active = (language.active ? TRUE : FALSE);
  fileName = language.fileName;
  languageName = language.languageName;
  
  for (vector<CString>::const_iterator it = language.synonyms.begin();
       it != language.synonyms.end();
       ++it)
    {
      if (it != language.synonyms.begin())
	{
	  synonyms += T_(',');
	}
      synonyms += *it;
    }
}

/* _________________________________________________________________________

   LanguageDefinitionDialog::DoDataExchange
   _________________________________________________________________________ */

void
LanguageDefinitionDialog::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);

  DDX_Check (pDX, IDC_ACTIVE, active);
  DDX_Text (pDX, IDC_HYPHENATION_FILE, fileName);
  DDX_Text (pDX, IDC_LANG_NAME, languageName);
  DDX_Text (pDX, IDC_SYNONYMS, synonyms);

  if (! pDX->m_bSaveAndValidate)
    {
      return;
    }

  language.active = (active ? true : false);
  language.languageName = languageName;
  language.fileName = fileName;

  language.synonyms.clear ();

  for (CSVList syn (synonyms, T_(',')); syn.GetCurrent() != 0; ++ syn)
    {
      language.synonyms.push_back (syn.GetCurrent());
    }
}

/* _________________________________________________________________________

   LanguageDefinitionDialog Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(LanguageDefinitionDialog, CDialog)
  ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
  ON_WM_CONTEXTMENU()
  ON_WM_HELPINFO()
END_MESSAGE_MAP();

/* _________________________________________________________________________

   aHelpIDs
   _________________________________________________________________________ */

#define MAKE_ID_HID_PAIR(id) id, H##id

namespace
{
  const DWORD aHelpIDs[] = {
    MAKE_ID_HID_PAIR(IDC_ACTIVE),
    MAKE_ID_HID_PAIR(IDC_BROWSE),
    MAKE_ID_HID_PAIR(IDC_HYPHENATION_FILE),
    MAKE_ID_HID_PAIR(IDC_LANG_NAME),
    MAKE_ID_HID_PAIR(IDC_SYNONYMS),
    0, 0,
  };
}

/* _________________________________________________________________________

   LanguageDefinitionDialog::OnHelpInfo
   _________________________________________________________________________ */

BOOL
LanguageDefinitionDialog::OnHelpInfo (/*[in]*/ HELPINFO * pHelpInfo) 
{
  return (::OnHelpInfo(pHelpInfo, aHelpIDs, T_("LangDefDialog.txt")));
}

/* _________________________________________________________________________

   LanguageDefinitionDialog::OnContextMenu
   _________________________________________________________________________ */

void
LanguageDefinitionDialog::OnContextMenu (/*[in]*/ CWnd *	pWnd,
					 /*[in]*/ CPoint	point) 
{
  try
    {
      DoWhatsThisMenu (pWnd, point, aHelpIDs, T_("LangDefDialog.txt"));
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

   LanguageDefinitionDialog::OnInitDialog
   _________________________________________________________________________ */

BOOL
LanguageDefinitionDialog::OnInitDialog () 
{
  BOOL ret = CDialog::OnInitDialog();
  return (ret);
}

/* _________________________________________________________________________

   LanguageDefinitionDialog::OnBrowse
   _________________________________________________________________________ */

void
LanguageDefinitionDialog::OnBrowse ()
{
  try
    {
      OPENFILENAME ofn;
      ZeroMemory (&ofn, sizeof(ofn));
      ofn.lStructSize = sizeof(ofn);
      ofn.hwndOwner = GetSafeHwnd();
      ofn.hInstance = 0;
      string filter;
      filter += T_("Hyphenation patterns (*.tex)");
      filter += T_('\0');
      filter += T_("*.tex");
      filter += T_('\0');
      filter += T_("All files (*.*)");
      filter += T_('\0');
      filter += T_("*.*");
      filter += T_('\0');
      filter += T_('\0');
      ofn.lpstrFilter = filter.c_str();
      ofn.lpstrCustomFilter = 0;
      ofn.nMaxCustFilter = 0;
      ofn.nFilterIndex = 1;
      char szFileName[BufferSizes::MaxPath];
      szFileName[0] = 0;
      ofn.lpstrFile = szFileName;
      ofn.nMaxFile = BufferSizes::MaxPath;
      ofn.lpstrFileTitle = 0;
      ofn.nMaxFileTitle = 0;
      PathName pathSample;
      PathName pathInitialDir;
      if (SessionWrapper(true)->FindFile(T_("ushyph.tex"),
					 FileType::TEX,
					 pathSample))
	{
	  pathInitialDir = pathSample;
	  pathInitialDir.RemoveFileSpec ();
	  ofn.lpstrInitialDir = pathInitialDir.Get();
	}
      else
	{
	  ofn.lpstrInitialDir = 0;
	}
      ofn.lpstrTitle = 0;
      ofn.Flags = 0;
      ofn.Flags |= OFN_FILEMUSTEXIST;
      ofn.Flags |= OFN_PATHMUSTEXIST;
      ofn.lpstrDefExt = T_(".tex");
      if (! GetOpenFileName(&ofn))
	{
	  // <todo>check error condition with CommDlgExtendedError</todo>
	  return;
	}
      PathName fileName (ofn.lpstrFile);
      fileName.RemoveDirectorySpec ();
      CWnd * pWnd = GetDlgItem(IDC_HYPHENATION_FILE);
      if (pWnd == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("CWnd::GetDlgItem"), 0);
	}
      pWnd->SetWindowText (fileName.Get());
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
