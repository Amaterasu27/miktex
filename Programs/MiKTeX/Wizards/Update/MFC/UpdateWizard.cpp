/* UpdateWizard.cpp:

   Copyright (C) 2002-2006 Christian Schenk

   This file is part of the MiKTeX Update Wizard.

   The MiKTeX Update Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX Update Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX Update Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"
#include "Update.h"

#include "Update-version.h"
#include "UpdateWizard.h"

IMPLEMENT_DYNAMIC(UpdateWizard, CPropertySheet);

/* _________________________________________________________________________

   UpdateWizard Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(UpdateWizard, CPropertySheet)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   UpdateWizard::UpdateWizard
   _________________________________________________________________________ */

CFont fntWelcome;

UpdateWizard::UpdateWizard ()
  : error (false),
    cancelled (false),
    randomRepositoryFlag (false),
    repositoryType (RepositoryType::Unknown)
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

  Construct (IDS_UPDATEWIZ, 0, 0, watermarkBitmap, 0, headerBitmap);

  AddPage (&welcomePage);
  AddPage (&typePage);
  AddPage (&localPage);
  AddPage (&remotePage);
  AddPage (&cdPage);
  AddPage (&packageListPage);
  AddPage (&fileCopyPage);
  AddPage (&finishPage);

  m_psh.dwFlags |= PSH_WIZARD97;
}

/* _________________________________________________________________________

   UpdateWizard::OnInitDialog
   _________________________________________________________________________ */

BOOL
UpdateWizard::OnInitDialog () 
{
  BOOL ret = CPropertySheet::OnInitDialog();
  try
    {
      SetIcon (AfxGetApp()->LoadIcon (IDI_UPDATE), TRUE);
      CString title;
      if (! title.LoadString(g_upgrading ? IDS_UPGRADEWIZ : IDS_UPDATEWIZ))
	{
	  FATAL_WINDOWS_ERROR (T_("CString::LoadString"), 0);
	}
      SetTitle (title);
      if (! g_upgrading
	  && SessionWrapper(true)->GetConfigValue(T_("Update"),
						  T_("alwaysWelcome"),
						  false))
	{
	  SetActivePage (&welcomePage);
	}
      else
	{
	  SetActivePage (&typePage);
	}
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

   UpdateWizard::EnableCancelButton
   _________________________________________________________________________ */

void
UpdateWizard::EnableCancelButton (/*[in]*/ bool enable)
{
  CWnd * pWnd = GetDlgItem(IDCANCEL);
  if (pWnd == 0)
    {
      UNEXPECTED_CONDITION (T_("UpdateWizard::EnableCancel"));
    }
  pWnd->EnableWindow (enable ? TRUE : FALSE);
  CHECK_WINDOWS_ERROR (T_("CWnd::EnableWindow"), 0);
}

/* _________________________________________________________________________

   UpdateWizard::ReportError
   _________________________________________________________________________ */

void
UpdateWizard::ReportError (/*[in]*/ const MiKTeXException & e)
{
  SetErrorFlag ();
  ::ReportError (e);
}

/* _________________________________________________________________________

   UpdateWizard::ReportError
   _________________________________________________________________________ */

void
UpdateWizard::ReportError (/*[in]*/ const exception & e)
{
  SetErrorFlag ();
  ::ReportError (e);
}

/* _________________________________________________________________________

   UpdateWizard::GetRepository
   _________________________________________________________________________ */

tstring
UpdateWizard::GetRepository ()
{
  tstring repository;
  switch (GetRepositoryType().Get())
    {
    case RepositoryType::Remote:
      if (randomRepositoryFlag)
	{
	  if (randomRepository.empty())
	    {
	      randomRepository = g_pManager->PickRepositoryUrl();
	    }
	  repository = randomRepository;
	}
      else
	{
	  repository = g_pManager->GetRemotePackageRepository();
	}
      break;
    case RepositoryType::Local:
      repository = g_pManager->GetLocalPackageRepository().Get();
      break;
    case RepositoryType::MiKTeXDirect:
      repository = g_pManager->GetMiKTeXDirectRoot().Get();
      break;
    };
  return (repository);
}
