/* SetupWizard.cpp:

   Copyright (C) 1999-2006 Christian Schenk

   This file is part of the MiKTeX Setup Wizard.

   The MiKTeX Setup Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX Setup Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX Setup Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "Setup.h"

#include "Setup-version.h"
#include "SetupWizard.h"

/* _________________________________________________________________________

   SetupWizard Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(SetupWizard, CPropertySheet)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   SetupWizard::SetupWizard
   _________________________________________________________________________ */

CFont fntWelcome;

SetupWizard::SetupWizard (/*[in]*/ PackageManager * pManager)
  : pManager (pManager),
    error (false),
    cancelled (false),
    m_PackageSetDownload (true),
    m_PackageSetInstall (false)
{
  NONCLIENTMETRICS ncm;
  memset (&ncm, 0, sizeof(ncm));
  ncm.cbSize = sizeof(ncm);
  if (! SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0))
    {
      FATAL_WINDOWS_ERROR (T_("SystemParametersInfo"), 0);
    }
  LOGFONT TitleLogFont = ncm.lfMessageFont;
  TitleLogFont.lfWeight = FW_BOLD;
  Utils::CopyString (TitleLogFont.lfFaceName, LF_FACESIZE, _T("Verdana Bold"));
  HDC hdc = ::GetDC(0);
  TitleLogFont.lfHeight = 0 - GetDeviceCaps(hdc, LOGPIXELSY) * 12 / 72;
  ::ReleaseDC (0, hdc);
  if (! fntWelcome.CreateFontIndirect(&TitleLogFont))
    {
      FATAL_WINDOWS_ERROR (T_("CFont::CreateFontIndirect"), 0);
    }

  if (! watermarkBitmap.LoadBitmap(IDB_WATERMARK))
    {
      FATAL_WINDOWS_ERROR (T_("CBitmap::LoadBitmap"), 0);
    }

  if (! headerBitmap.LoadBitmap(IDB_HEADER))
    {
      FATAL_WINDOWS_ERROR (T_("CBitmap::LoadBitmap"), 0);
    }

  Construct (IDS_SETUPWIZ, 0, 0, watermarkBitmap, 0, headerBitmap);

  if (! theApp.unattended)
    {
      AddPage (&m_Welcome);
      AddPage (&m_License);
      AddPage (&m_MDTask);
      AddPage (&m_Task);
      AddPage (&m_RemoteRepository);
      AddPage (&m_LocalRepository);
      AddPage (&m_PackageSetDownload);
      AddPage (&m_PackageSetInstall);
      AddPage (&m_Shared);
      AddPage (&m_InstallDir);
#if SHOW_FOLDER_PAGE
      AddPage (&m_Folder);
#endif
      AddPage (&m_Settings);
#if ENABLE_ADDTEXMF
      AddPage (&m_AddTEXMF);
      AddPage (&m_EditTEXMF);
#endif
      AddPage (&m_Info);
    }

  AddPage (&m_FileCopy);

  if (! theApp.unattended)
    {
      AddPage (&m_Finish);
    }

  m_psh.dwFlags |= PSH_WIZARD97;
}

/* _________________________________________________________________________

   SetupWizard::OnInitDialog
   _________________________________________________________________________ */

BOOL
SetupWizard::OnInitDialog () 
{
  BOOL ret = CPropertySheet::OnInitDialog();
  try
    {
      SetIcon (AfxGetApp()->LoadIcon(IDI_SETUP), TRUE);
      CString title;
      if (! title.LoadString(IDS_SETUPWIZ))
	{
	  FATAL_WINDOWS_ERROR (T_("CString::LoadString"), 0);
	}
      title += T_(' ');
      title += MIKTEXTEXT(MIKTEX_VERSION_STR);
      SetTitle (title);
#if 0
      SetActivePage (&m_Task);
#endif
    }
  catch (const MiKTeXException & e)
    {
      ReportError (e);
    }
  catch (const exception & e)
    {
      ReportError (e);
    }
  return (ret);
}

/* _________________________________________________________________________

   SetupWizard::EnableCancelButton
   _________________________________________________________________________ */

void
SetupWizard::EnableCancelButton (/*[in]*/ bool enable)
{
  CWnd * pWnd = GetDlgItem(IDCANCEL);
  if (pWnd == 0)
    {
      UNEXPECTED_CONDITION (T_("SetupWizard::EnableCancel"));
    }
  pWnd->EnableWindow (enable ? TRUE : FALSE);
  CHECK_WINDOWS_ERROR (T_("CWnd::EnableWindow"), 0);
}

/* _________________________________________________________________________

   SetupWizard::SetNextText
   _________________________________________________________________________ */

CString
SetupWizard::SetNextText (/*[in]*/ const MIKTEXCHAR * lpszText)
{
  CWnd * pWnd = GetDlgItem(ID_WIZNEXT);
  if (pWnd == 0)
    {
      UNEXPECTED_CONDITION (T_("SetupWizard::SetNextText"));
    }
  CString ret;
  pWnd->GetWindowText (ret);
  pWnd->SetWindowText (lpszText);
  return (ret);
}

/* _________________________________________________________________________

   SetupWizard::ReportError
   _________________________________________________________________________ */

void
SetupWizard::ReportError (/*[in]*/ const MiKTeXException & e)
{
  SetErrorFlag ();
  ::ReportError (e);
}

/* _________________________________________________________________________

   SetupWizard::ReportError
   _________________________________________________________________________ */

void
SetupWizard::ReportError (/*[in]*/ const exception & e)
{
  SetErrorFlag ();
  ::ReportError (e);
}

