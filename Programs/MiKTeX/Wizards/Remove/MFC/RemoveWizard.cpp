/* RemoveWizard.cpp:

   Copyright (C) 2000-2007 Christian Schenk

   This file is part of the Remove MiKTeX! Wizard.

   The Remove MiKTeX! Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The Remove MiKTeX! Wizard is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the Remove MiKTeX! Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"
#include "Remove.h"

#include "FinishPage.h"
#include "RemoveFilesPage.h"
#include "RemoveWizard.h"
#include "WelcomePage.h"

/* _________________________________________________________________________

   RemoveWizard Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(RemoveWizard, CPropertySheet)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   RemoveWizard::RemoveWizard
   _________________________________________________________________________ */

CFont fntWelcome;

RemoveWizard::RemoveWizard ()
  : error (false),
    cancelled (false)
{
  NONCLIENTMETRICS ncm;
  memset (&ncm, 0, sizeof(ncm));
  ncm.cbSize = sizeof(ncm);
  if (WINVER >= 0x0600 && ! IsWindowsVista())
    {
      // work-around SDK bug
      ncm.cbSize -= sizeof(int/*NONCLIENTMETRICS::iPaddedBorderWidth*/);
    }
  if (! SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0))
    {
      FATAL_WINDOWS_ERROR (T_("SystemParametersInfo"), 0);
    }
  LOGFONT TitleLogFont = ncm.lfMessageFont;
  TitleLogFont.lfWeight = FW_BOLD;
  Utils::CopyString (TitleLogFont.lfFaceName, LF_FACESIZE, T_("Verdana Bold"));
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

  Construct (IDS_REMOVEWIZ, 0, 0, watermarkBitmap, 0, headerBitmap);
  
  AddPage (new WelcomePage());
  AddPage (new RemoveFilesPage());
  AddPage (new FinishPage());

  m_psh.dwFlags |= PSH_WIZARD97;
}

/* _________________________________________________________________________

   RemoveWizard::OnInitDialog
   _________________________________________________________________________ */

BOOL
RemoveWizard::OnInitDialog () 
{
  BOOL ret = CPropertySheet::OnInitDialog();
  try
    {
      SetIcon (AfxGetApp()->LoadIcon (IDI_REMOVE), TRUE);
      CString title;
      if (! title.LoadString(IDS_REMOVEWIZ))
	{
	  FATAL_WINDOWS_ERROR (T_("CString::LoadString"), 0);
	}
      SetTitle (title);
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

   RemoveWizard::EnableCancelButton
   _________________________________________________________________________ */

void
RemoveWizard::EnableCancelButton (/*[in]*/ bool enable)
{
  CWnd * pWnd = GetDlgItem(IDCANCEL);
  if (pWnd == 0)
    {
      UNEXPECTED_CONDITION (T_("RemoveWizard::EnableCancel"));
    }
  pWnd->EnableWindow (enable ? TRUE : FALSE);
}

/* _________________________________________________________________________

   RemoveWizard::ReportError
   _________________________________________________________________________ */

void
RemoveWizard::ReportError (/*[in]*/ const MiKTeXException & e)
{
  SetErrorFlag ();
  ::ReportError (e);
}

/* _________________________________________________________________________

   RemoveWizard::ReportError
   _________________________________________________________________________ */

void
RemoveWizard::ReportError (/*[in]*/ const exception & e)
{
  SetErrorFlag ();
  ::ReportError (e);
}

/* _________________________________________________________________________

   RemoveWizard::SetNextText
   _________________________________________________________________________ */

CString
RemoveWizard::SetNextText (/*[in]*/ const MIKTEXCHAR * lpszText)
{
  CWnd * pWnd = GetDlgItem(ID_WIZNEXT);
  if (pWnd == 0)
    {
      UNEXPECTED_CONDITION (T_("RemoveWizard::SetNextText"));
    }
  CString ret;
  pWnd->GetWindowText (ret);
  pWnd->SetWindowText (lpszText);
  return (ret);
}
