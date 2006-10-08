/* FormatDefinitionDialog.cpp:

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

#include "FormatDefinitionDialog.h"
#include "resource.hm"

/* _________________________________________________________________________

   FormatDefinitionDialog::FormatDefinitionDialog
   _________________________________________________________________________ */

FormatDefinitionDialog::FormatDefinitionDialog (/*in]*/ CWnd *	pParent,
			      /*in]*/ const MIKTEXCHAR *	lpszFormatName)
  : CDialog (FormatDefinitionDialog::IDD, pParent)
{
  if (lpszFormatName == 0)
    {
      formatInfo.compiler = T_("pdfetex");
      formatInfo.description = T_("New format");
      formatInfo.exclude = true;
      formatInfo.name = T_("newformat");
      formatInfo.inputFile = T_("newformat.tex");
    }
  else
    {
      formatInfo = SessionWrapper(true)->GetFormatInfo(lpszFormatName);
      originalFormatInfo = formatInfo;
    }
  compiler = formatInfo.compiler.c_str();
  description = formatInfo.description.c_str();
  exclude = (formatInfo.exclude ? TRUE : FALSE);
  formatName = formatInfo.name.c_str();
  inputName = formatInfo.inputFile.c_str();
  outputName = formatInfo.outputFile.c_str();
  preloadedFormat = formatInfo.preloaded.c_str();
}

/* _________________________________________________________________________

   FormatDefinitionDialog::DoDataExchange
   _________________________________________________________________________ */

void
FormatDefinitionDialog::DoDataExchange (/*in]*/ CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);

  DDX_CBStringExact (pDX, IDC_COMPILER, compiler);
  DDX_CBStringExact (pDX, IDC_PRELOADED_FMT, preloadedFormat);
  DDX_Check (pDX, IDC_EXCLUDE, exclude);
  DDX_Control (pDX, IDC_COMPILER, compilerComboBox);
  DDX_Control (pDX, IDC_PRELOADED_FMT, preloadedFormatComboBox);
  DDX_Text (pDX, IDC_DESCRIPTION, description);
  DDX_Text (pDX, IDC_FMT_NAME, formatName);
  DDX_Text (pDX, IDC_INPUT_NAME, inputName);
  DDX_Text (pDX, IDC_OUTPUT_NAME, outputName);

  if (! pDX->m_bSaveAndValidate)
    {
      return;
    }

  pDX->PrepareEditCtrl (IDC_FMT_NAME);
  if (formatName.IsEmpty())
    {
      AfxMessageBox (T_("You must specify a format name."),
		     MB_ICONEXCLAMATION);
      pDX->Fail ();
    }
  if (PathName::Compare(static_cast<const MIKTEXCHAR *>(formatName),
			originalFormatInfo.name)
      != 0)
    {
      FormatInfo unused;
      if (SessionWrapper(true)->TryGetFormatInfo(formatName, unused))
	{
	  AfxMessageBox
	    (T_("A format with the given name already exists."),
	     MB_ICONEXCLAMATION);
	  pDX->Fail ();
	}
    }

  pDX->PrepareEditCtrl (IDC_INPUT_NAME);
  if (inputName.IsEmpty())
    {
      AfxMessageBox (T_("You must specify an input file name."),
		     MB_ICONEXCLAMATION);
      pDX->Fail ();
    }

  pDX->PrepareCtrl (IDC_COMPILER);
  if (compiler.IsEmpty())
    {
      AfxMessageBox (T_("You must choose a compiler."), MB_ICONEXCLAMATION);
      pDX->Fail ();
    }

  formatInfo.name = formatName;
  formatInfo.description = description;
  formatInfo.exclude = (exclude ? true : false);
  formatInfo.inputFile = inputName;
  formatInfo.outputFile = outputName;
  formatInfo.preloaded = preloadedFormat;
  formatInfo.compiler = compiler;
}

/* _________________________________________________________________________

   FormatDefinitionDialog Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(FormatDefinitionDialog, CDialog)
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
    MAKE_ID_HID_PAIR(IDC_COMPILER),
    MAKE_ID_HID_PAIR(IDC_DESCRIPTION),
    MAKE_ID_HID_PAIR(IDC_EXCLUDE),
    MAKE_ID_HID_PAIR(IDC_FMT_NAME),
    MAKE_ID_HID_PAIR(IDC_INPUT_NAME),
    MAKE_ID_HID_PAIR(IDC_OUTPUT_NAME),
    MAKE_ID_HID_PAIR(IDC_PRELOADED_FMT),
    0, 0,
  };
}

/* _________________________________________________________________________

   FormatDefinitionDialog::OnHelpInfo
   _________________________________________________________________________ */

BOOL
FormatDefinitionDialog::OnHelpInfo (/*in]*/ HELPINFO * pHelpInfo) 
{
  return (::OnHelpInfo(pHelpInfo, aHelpIDs, T_("FmtDefDialog.txt")));
}

/* _________________________________________________________________________

   FormatDefinitionDialog::OnContextMenu
   _________________________________________________________________________ */

void
FormatDefinitionDialog::OnContextMenu (/*in]*/ CWnd *	pWnd,
				       /*in]*/ CPoint	point) 
{
  try
    {
      DoWhatsThisMenu (pWnd, point, aHelpIDs, T_("FmtDefDialog.txt"));
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

   FormatDefinitionDialog::OnInitDialog
   _________________________________________________________________________ */

BOOL
FormatDefinitionDialog::OnInitDialog () 
{
  BOOL ret = CDialog::OnInitDialog();
  try
    {
      FormatInfo formatInfo;
      set<tstring> compilers;
      for (unsigned idx = 0;
	   SessionWrapper(true)->GetFormatInfo(idx, formatInfo);
	   ++ idx)
	{
	  compilers.insert (formatInfo.compiler);
	  if (PathName::Compare(formatInfo.name.c_str(), formatName) == 0)
	    {
	      continue;
	    }
	  preloadedFormatComboBox.AddString (formatInfo.name.c_str());
	}
      for (set<tstring>::const_iterator it = compilers.begin();
	   it != compilers.end();
	   ++ it)
	{
	  compilerComboBox.AddString (it->c_str());
	}
      UpdateData (FALSE);
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
