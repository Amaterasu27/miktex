/* SiteWizLocal.cpp:

   Copyright (C) 2000-2011 Christian Schenk

   This file is part of the MiKTeX UI Library.

   The MiKTeX UI Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX UI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX UI Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"
#include "internal.h"
#include "resource.h"

#include "SiteWizLocal.h"

/* _________________________________________________________________________

   SiteWizLocal::SiteWizLocal
   _________________________________________________________________________ */

SiteWizLocal::SiteWizLocal (/*[in]*/ PackageManager * pManager)
  : CPropertyPage (SiteWizLocal::IDD, IDS_SITEWIZ),
    pManager (pManager)
{
  m_psp.dwFlags &= ~ PSP_HASHELP;

  PathName path;

  if (pManager->TryGetLocalPackageRepository(path))
    {
      directory = path.Get();
    }
}

/* _________________________________________________________________________

   SiteWizLocal::DoDataExchange
   _________________________________________________________________________ */

void
SiteWizLocal::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Text (pDX, IDC_FILENAME, directory);
}

/* _________________________________________________________________________

   SiteWizLocal Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(SiteWizLocal, CPropertyPage)
  ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
  ON_EN_CHANGE(IDC_FILENAME, OnChangeDirectory)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   SiteWizLocal::OnSetActive
   _________________________________________________________________________ */

BOOL
SiteWizLocal::OnSetActive ()
{
  CPropertySheet * pSheet = reinterpret_cast<CPropertySheet*>(GetParent());
  ASSERT_KINDOF (CPropertySheet, pSheet);
  GetDlgItem(IDC_FILENAME)->SetWindowText (directory);
  if (directory.GetLength() > 0)
    {
      pSheet->SetWizardButtons (PSWIZB_BACK | PSWIZB_FINISH);
    }
  else
    {
      pSheet->SetWizardButtons (PSWIZB_BACK);
    }
  return (CPropertyPage::OnSetActive());
}

/* _________________________________________________________________________

   SiteWizLocal::OnBrowse
   _________________________________________________________________________ */

void
SiteWizLocal::OnBrowse ()
{
  try
    {
      BROWSEINFO browseInfo;
      ZeroMemory(&browseInfo, sizeof(browseInfo));
      browseInfo.hwndOwner = GetSafeHwnd();
      browseInfo.pszDisplayName = 0;
      CString title;
      VERIFY (title.LoadString(IDS_BROWSE_LOCAL));
      browseInfo.lpszTitle = title;
      browseInfo.ulFlags = BIF_RETURNONLYFSDIRS;
      LPCITEMIDLIST pidl = SHBrowseForFolder(&browseInfo);
      if (pidl == 0)
	{
	  return;
	}
      char path[BufferSizes::MaxPath];
      BOOL havePath = SHGetPathFromIDListA(pidl, path);
      CoTaskMemFree (const_cast<LPITEMIDLIST>(pidl));
      if (! havePath)
	{
	  FATAL_WINDOWS_ERROR ("SHGetPathFromIDListA", 0);
	}
      CPropertySheet * pSheet = reinterpret_cast<CPropertySheet*>(GetParent());
      ASSERT_KINDOF (CPropertySheet, pSheet);
      directory = path;
      UpdateData (FALSE);
      pSheet->SetWizardButtons (PSWIZB_BACK | PSWIZB_FINISH);
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

   SiteWizLocal::OnChangeDirectory
   _________________________________________________________________________ */

void
SiteWizLocal::OnChangeDirectory ()
{
  try
    {
      CWnd * pWnd = reinterpret_cast<CEdit*>(GetDlgItem(IDC_FILENAME));
      if (pWnd == 0)
	{
	  FATAL_WINDOWS_ERROR ("CWnd::GetDlgItem", 0);
	}
      CPropertySheet * pSheet = reinterpret_cast<CPropertySheet*>(GetParent());
      ASSERT_KINDOF (CPropertySheet, pSheet);
      if (pWnd->GetWindowTextLength() > 0)
	{
	  pSheet->SetWizardButtons (PSWIZB_BACK | PSWIZB_FINISH);
	}
      else
	{
	  pSheet->SetWizardButtons (PSWIZB_BACK);
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

   SiteWizLocal::OnWizardFinish
   _________________________________________________________________________ */

BOOL
SiteWizLocal::OnWizardFinish ()
{
  try
    {
      if (! Directory::Exists(PathName(directory)))
	{
	  CString prompt;
	  AfxFormatString1 (prompt, IDP_NOT_A_FOLDER, directory);
	  FATAL_MIKTEX_ERROR ("SiteWizLocal::OnWizardFinish",
			      TU_(prompt),
			      TU_(directory));
	}
      else
	{
	  if (! (pManager->IsLocalPackageRepository(PathName(directory))))
	    {
	      PathName mpmIni (directory);
	      mpmIni += "texmf";
	      mpmIni += MIKTEX_PATH_MPM_INI;
	      if (! File::Exists(mpmIni))
		{
		  CString prompt;
		  AfxFormatString1 (prompt,
				    IDP_NOT_LOCAL_REPOSITORY,
				    directory);
		  FATAL_MIKTEX_ERROR ("SiteWizLocal::OnWizardFinish",
				      TU_(prompt),
				      TU_(directory));
		}
	    }
	}
      pManager->SetDefaultPackageRepository
	(RepositoryType::Local,
	 string(TU_(directory)));
      return (CPropertyPage::OnWizardFinish());
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

   SiteWizLocal::OnWizardBack
   _________________________________________________________________________ */

LRESULT
SiteWizLocal::OnWizardBack ()
{
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_SITEWIZ_TYPE)));
}
