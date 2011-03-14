/* AddChangeEditorDialog.cpp:

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

#include "AddChangeEditorDialog.h"

/* _________________________________________________________________________

   AddChangeEditorDialog Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(AddChangeEditorDialog, CDialog)
  ON_BN_CLICKED(IDC_BROWSE, &AddChangeEditorDialog::OnBrowse)
  ON_EN_CHANGE(IDC_ARGUMENTS, &AddChangeEditorDialog::OnChangeArguments)
  ON_EN_CHANGE(IDC_PATH, &AddChangeEditorDialog::OnChangePath)
  ON_EN_CHANGE(IDC_NAME, &AddChangeEditorDialog::OnChangeName)
END_MESSAGE_MAP()

/* _________________________________________________________________________

   AddChangeEditorDialog::AddChangeEditorDialog
   _________________________________________________________________________ */

AddChangeEditorDialog::AddChangeEditorDialog
(/*[in]*/ CWnd *			pParent,
 /*[in]*/ const vector<EditorInfo> &	editors,
 /*[in]*/ int				firstCustomIdx)
  : CDialog(AddChangeEditorDialog::IDD, pParent),
    editors (editors),
    name(T_("New Editor")),
    adding (true),
    firstCustomIdx (firstCustomIdx),
    currentIdx (-1)
{
}

/* _________________________________________________________________________

   AddChangeEditorDialog::AddChangeEditorDialog
   _________________________________________________________________________ */

AddChangeEditorDialog::AddChangeEditorDialog
(/*[in]*/ CWnd *			pParent,
 /*[in]*/ const vector<EditorInfo> &	editors,
 /*[in]*/ int				firstCustomIdx,
 /*[in]*/ int				currentIdx)
  : CDialog(AddChangeEditorDialog::IDD, pParent),
    editors (editors),
    name (editors[currentIdx].name.c_str()),
    program (editors[currentIdx].program.c_str()),
    arguments (editors[currentIdx].arguments.c_str()),
    adding (false),
    firstCustomIdx (firstCustomIdx),
    currentIdx (currentIdx)
{
}

/* _________________________________________________________________________

   AddChangeEditorDialog::OnInitDialog
   _________________________________________________________________________ */

BOOL
AddChangeEditorDialog::OnInitDialog ()
{
  BOOL ret = CDialog::OnInitDialog();
  
  try
    {
      HResult hr = SHAutoComplete(programEdit.m_hWnd, SHACF_FILESYSTEM);
      EnableButtons ();
      if (hr.Failed())
	{
#if 1
	  TraceError (T_("Auto completion error: %s"), hr.GetText());
#else
	  FATAL_MIKTEX_ERROR ("AddChangeEditorDialog::OnInitDialog",
			      _T("Auto completion error."),
			      hr.GetText());
#endif
	  
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

   AddChangeEditorDialog::DoDataExchange
   _________________________________________________________________________ */

void
AddChangeEditorDialog::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_NAME, nameEdit);
  DDX_Text (pDX, IDC_NAME, name);
  DDX_Control (pDX, IDC_PATH, programEdit);
  DDX_Text (pDX, IDC_PATH, program);
  DDX_Control (pDX, IDC_ARGUMENTS, argumentsEdit);
  DDX_Text (pDX, IDC_ARGUMENTS, arguments);
  if (pDX->m_bSaveAndValidate)
    {
      pDX->PrepareEditCtrl (IDC_NAME);
      for (int idx = firstCustomIdx;
	   idx < static_cast<int>(editors.size());
	   ++ idx)
	{
	  if (idx != currentIdx
	      && StringCompare(editors[idx].name.c_str(), CT2A(name), true) == 0)
	    {
	      AfxMessageBox (T_(_T("The specified editor name already exists.")));
	      pDX->Fail ();
	    }
	}
      pDX->PrepareEditCtrl (IDC_PATH);
      if (! File::Exists(PathName(CT2A(program))))
	{
	  AfxMessageBox (T_(_T("The specified program file does not exist.")));
	  pDX->Fail ();
	}
      pDX->PrepareEditCtrl (IDC_ARGUMENTS);
      if (! (arguments.Find(_T("%f")) >= 0))
	{
	  AfxMessageBox (T_(_T("Missing file name (%f) in argument string.")));
	  pDX->Fail ();
	}
      if (! (arguments.Find(_T("%l")) >= 0))
	{
	  AfxMessageBox (T_(_T("Missing line number (%l) in argument string.")));
	  pDX->Fail ();
	}
    }
}

/* _________________________________________________________________________

   AddChangeEditorDialog::OnBrowse
   _________________________________________________________________________ */

void
AddChangeEditorDialog::OnBrowse ()
{
  try
    {
      OPENFILENAMEW ofn;
      ZeroMemory (&ofn, sizeof(ofn));
      ofn.lStructSize = sizeof(ofn);
      ofn.hwndOwner = GetSafeHwnd();
      ofn.hInstance = 0;
      wstring filter;
      filter += T_(L"Executables (*.exe)");
      filter += L'\0';
      filter += T_(L"*.exe");
      filter += L'\0';
      filter += T_(L"All files (*.*)");
      filter += L'\0';
      filter += L"*.*";
      filter += L'\0';
      ofn.lpstrFilter = filter.c_str();
      ofn.lpstrCustomFilter = 0;
      ofn.nMaxCustFilter = 0;
      ofn.nFilterIndex = 1;
      wchar_t szFileName[BufferSizes::MaxPath];
      szFileName[0] = 0;
      ofn.lpstrFile = szFileName;
      ofn.nMaxFile = BufferSizes::MaxPath;
      ofn.lpstrFileTitle = 0;
      ofn.nMaxFileTitle = 0;
      PathName pathSample;
      PathName pathInitialDir;
      ofn.lpstrInitialDir = 0;
      ofn.lpstrTitle = 0;
      ofn.Flags = 0;
      ofn.Flags |= OFN_FILEMUSTEXIST;
      ofn.Flags |= OFN_PATHMUSTEXIST;
      ofn.lpstrDefExt = L".exe";
      if (! GetOpenFileNameW(&ofn))
	{
	  // <todo>check error condition with CommDlgExtendedError</todo>
	  return;
	}
      programEdit.SetWindowText (CW2T(ofn.lpstrFile));
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

   AddChangeEditorDialog::OnChangeName
   _________________________________________________________________________ */

void
AddChangeEditorDialog::OnChangeName ()
{
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
}

/* _________________________________________________________________________

   AddChangeEditorDialog::OnChangePath
   _________________________________________________________________________ */

void
AddChangeEditorDialog::OnChangePath ()
{
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
}

/* _________________________________________________________________________

   AddChangeEditorDialog::OnChangeArguments
   _________________________________________________________________________ */

void
AddChangeEditorDialog::OnChangeArguments ()
{
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
}

/* _________________________________________________________________________

   AddChangeEditorDialog::EnableButtons
   _________________________________________________________________________ */

void
AddChangeEditorDialog::EnableButtons ()
{
  CWnd * pOk = GetDlgItem(IDOK);
  if (pOk == 0)
    {
      UNEXPECTED_CONDITION ("AddChangeEditorDialog::EnableButtons");
    }
  CString name;
  nameEdit.GetWindowText (name);
  CString program;
  programEdit.GetWindowText (program);
  CString arguments;
  argumentsEdit.GetWindowText (arguments);
  pOk->EnableWindow
    (! (name.IsEmpty() || program.IsEmpty() || arguments.IsEmpty())
     && arguments.Find(_T("%f")) >= 0
     && arguments.Find(_T("%l")) >= 0);
}

/* _________________________________________________________________________

   AddChangeEditorDialog::GetEditorInfo
   _________________________________________________________________________ */

EditorInfo
AddChangeEditorDialog::GetEditorInfo ()
{
  return (EditorInfo(CT2A(name), CT2A(program), CT2A(arguments)));
}
