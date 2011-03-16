/* LocalPage.cpp:

   Copyright (C) 2002-2011 Christian Schenk

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

#include "LocalPage.h"
#include "UpdateWizard.h"

IMPLEMENT_DYNCREATE(LocalPage, CPropertyPage);

/* _________________________________________________________________________

   LocalPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(LocalPage, CPropertyPage)
  ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
  ON_EN_CHANGE(IDC_FILENAME, OnChangeFilename)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   LocalPage::LocalPage
   _________________________________________________________________________ */

LocalPage::LocalPage ()
  : CPropertyPage (IDD, 0, IDS_HEADER_LOCAL, IDS_SUBHEADER_LOCAL),
    pSheet (0)
{
  m_psp.dwFlags &= ~ PSP_HASHELP;
  PathName fileName;
  if (g_pManager->TryGetLocalPackageRepository(fileName))
    {
      this->fileName = fileName.Get();
    }
}

/* _________________________________________________________________________

   LocalPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
LocalPage::OnInitDialog ()
{
  pSheet = reinterpret_cast<UpdateWizard *>(GetParent());
  return (CPropertyPage::OnInitDialog());
}

/* _________________________________________________________________________

   LocalPage::OnSetActive
   _________________________________________________________________________ */

BOOL
LocalPage::OnSetActive ()
{
  noDdv = false;
  BOOL ret = CPropertyPage::OnSetActive();
  if (ret)
    {
      try
	{
	  CWnd * pWnd = GetDlgItem(IDC_FILENAME);
	  if (pWnd == 0)
	    {
	      UNEXPECTED_CONDITION ("LocalPage::OnSetActive");
	    }
	  pWnd->SetWindowText (fileName);
	  if (fileName.GetLength() > 0)
	    {
	      pSheet->SetWizardButtons (PSWIZB_BACK | PSWIZB_NEXT);
	    }
	  else
	    {
	      pSheet->SetWizardButtons (PSWIZB_BACK);
	    }
	}
      catch (const MiKTeXException & e)
	{
	  pSheet->ReportError (e);
	  ret = FALSE;
	}
      catch (const exception & e)
	{
	  pSheet->ReportError (e);
	  ret = FALSE;
	}
    }
  return (ret);
}

/* _________________________________________________________________________

   LocalPage::DoDataExchange
   _________________________________________________________________________ */

void
LocalPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Text (pDX, IDC_FILENAME, fileName);
}

/* _________________________________________________________________________

   LocalPage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
LocalPage::OnWizardNext ()
{
  pSheet->SetCameFrom (IDD);
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_PACKAGE_LIST)));
}

/* _________________________________________________________________________

   LocalPage::OnWizardBack
   _________________________________________________________________________ */

LRESULT
LocalPage::OnWizardBack ()
{
  noDdv = true;
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_TYPE)));
}

/* _________________________________________________________________________

   LocalPage::OnKillActive
   _________________________________________________________________________ */

BOOL
LocalPage::OnKillActive ()
{
  BOOL ret = CPropertyPage::OnKillActive();
  if (ret && ! noDdv)
    {
      try
	{
	  if (! (g_pManager->IsLocalPackageRepository(PathName(fileName))))
	    {
	      CString message;
	      AfxFormatString1 (message, IDP_NOT_LOCAL_REPOSITORY, fileName);
	      AfxMessageBox (message, MB_OK | MB_ICONSTOP);
	      ret = FALSE;
	    }
	  else
	    {
	      g_pManager->SetLocalPackageRepository (PathName(fileName));
	    }
	}
      catch (const MiKTeXException & e)
	{
	  pSheet->ReportError (e);
	  ret = FALSE;
	}
      catch (const exception & e)
	{
	  pSheet->ReportError (e);
	  ret = FALSE;
	}
    }
  return (ret);
}

/* _________________________________________________________________________

   LocalPage::OnBrowse
   _________________________________________________________________________ */

void
LocalPage::OnBrowse ()
{
  try
    {
      BROWSEINFOW bi;
      ZeroMemory (&bi, sizeof(bi));
      bi.hwndOwner = GetSafeHwnd();
      bi.pszDisplayName = 0;
      CString title;
      if (! title.LoadString(IDS_BROWSE_LOCAL))
	{
	  UNEXPECTED_CONDITION ("LocalPage::OnBrowse");
	}
      bi.lpszTitle = title;
      bi.ulFlags = BIF_RETURNONLYFSDIRS;
      LPCITEMIDLIST pidl = SHBrowseForFolderW(&bi);
      if (pidl == 0)
	{
	  return;
	}
      wchar_t szFolderPath[BufferSizes::MaxPath];
      BOOL havePath = SHGetPathFromIDListW(pidl, szFolderPath);
      CoTaskMemFree (const_cast<LPITEMIDLIST>(pidl));
      if (! havePath)
	{
	  UNEXPECTED_CONDITION ("LocalPage::OnBrowse");
	}
      fileName = szFolderPath;
      UpdateData (FALSE);
      pSheet->SetWizardButtons (PSWIZB_BACK | PSWIZB_NEXT);
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
    }
}

/* _________________________________________________________________________

   LocalPage::OnChangeFilename
   _________________________________________________________________________ */

void
LocalPage::OnChangeFilename ()
{
  try
    {
      CWnd * pWnd = reinterpret_cast<CEdit*>(GetDlgItem(IDC_FILENAME));
      if (pWnd == 0)
	{
	  UNEXPECTED_CONDITION ("LocalPage::OnChangeFilename");
	}
      if (pWnd->GetWindowTextLength() > 0)
	{
	  pSheet->SetWizardButtons (PSWIZB_BACK | PSWIZB_NEXT);
	}
      else
	{
	  pSheet->SetWizardButtons (PSWIZB_BACK);
	}
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
    }
}
