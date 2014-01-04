/* LocalRepositoryPag.cpp:

   Copyright (C) 1999-2011 Christian Schenk

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

#include "LocalRepositoryPage.h"
#include "SetupWizard.h"

/* _________________________________________________________________________

   LocalRepositoryPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(LocalRepositoryPage, CPropertyPage)
  ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
  ON_EN_CHANGE(IDC_FILENAME, OnChangePathName)
END_MESSAGE_MAP()

/* _________________________________________________________________________

   LocalRepositoryPage::LocalRepositoryPage
   _________________________________________________________________________ */

LocalRepositoryPage::LocalRepositoryPage ()
  : CPropertyPage (IDD,
		   0,
		   IDS_HEADER_LOCAL_REPOSITORY,
		   IDS_SUBHEADER_LOCAL_REPOSITORY),
    pSheet (0)
{
}

/* _________________________________________________________________________

   LocalRepositoryPage::OnInitDialog ()
   _________________________________________________________________________ */

BOOL
LocalRepositoryPage::OnInitDialog ()
{
  pSheet = reinterpret_cast<SetupWizard *>(GetParent());
  fileName = theApp.GetLocalPackageRepository().Get();
  return (CPropertyPage::OnInitDialog());
}

/* _________________________________________________________________________

   LocalRepositoryPage::OnSetActive
   _________________________________________________________________________ */

BOOL
LocalRepositoryPage::OnSetActive ()
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
	      UNEXPECTED_CONDITION ("LocalRepositoryPage::OnSetActive");
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
	  pWnd = GetDlgItem(IDC_MESSAGE);
	  if (pWnd == 0)
	    {
	      UNEXPECTED_CONDITION ("LocalRepositoryPage::OnSetActive");
	    }
	  if (theApp.GetSetupTask() == SetupTask::Download)
	    {
	      pWnd->SetWindowText
		(T_(_T("Download the MiKTeX distribution to:")));
	    }
	  else
	    {
	      pWnd->SetWindowText
		(T_(_T("Install MiKTeX from:")));
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

   LocalRepositoryPage::DoDataExchange
   _________________________________________________________________________ */

void
LocalRepositoryPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Text (pDX, IDC_FILENAME, fileName);
}

/* _________________________________________________________________________

   LocalRepositoryPage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
LocalRepositoryPage::OnWizardNext ()
{
  UINT next;
  switch (theApp.GetSetupTask().Get())
    {
    case SetupTask::Download:
      next = IDD_INFOLIST;
      break;
    case SetupTask::InstallFromLocalRepository:
    case SetupTask::InstallFromRemoteRepository:
      next = IDD_INSTALLDIR;
      break;
    default:
      ASSERT (false);
      __assume (false);
      break;
    }
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(next)));
}

/* _________________________________________________________________________

   LocalRepositoryPage::OnWizardBack
   _________________________________________________________________________ */

LRESULT
LocalRepositoryPage::OnWizardBack ()
{
  UINT prev;
  switch (theApp.GetSetupTask().Get())
    {
    case SetupTask::Download:
    case SetupTask::InstallFromRemoteRepository:
      prev = IDD_REMOTE_REPOSITORY;
      break;
    case SetupTask::InstallFromLocalRepository:
      if (theApp.IsPortable())
      {
	prev = IDD_PACKAGE_SET_INSTALL;
      }
      else
      {
	prev = IDD_SHARED;
      }
      break;
    default:
      ASSERT (false);
      __assume (false);
      break;
    }
  noDdv = true;
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(prev)));
}

/* _________________________________________________________________________

   LocalRepositoryPage::OnKillActive
   _________________________________________________________________________ */

BOOL
LocalRepositoryPage::OnKillActive ()
{
  BOOL ret = CPropertyPage::OnKillActive();
  if (ret && ! noDdv)
    {
      try
	{
	  if (theApp.GetSetupTask() == SetupTask::InstallFromLocalRepository)
	    {
	      PackageLevel foundPackageLevel =
		TestLocalRepository(PathName(fileName),
				    theApp.GetPackageLevel());
	      if (foundPackageLevel == PackageLevel::None)
		{
		  CString message;
		  AfxFormatString1 (message,
				    IDP_NOT_LOCAL_REPOSITORY,
				    fileName);
		  AfxMessageBox (message, MB_OK | MB_ICONSTOP);
		  ret = FALSE;
		}
	    }
	  if (ret)
	    {
	      SetupOptions options = theApp.pSetupService->GetOptions();
	      options.LocalPackageRepository = static_cast<LPCTSTR>(fileName);
	      theApp.pSetupService->SetOptions(options);
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

   LocalRepositoryPage::OnBrowse
   _________________________________________________________________________ */

void
LocalRepositoryPage::OnBrowse ()
{
  try
    {
      BROWSEINFO bi;
      ZeroMemory (&bi, sizeof(bi));
      bi.hwndOwner = GetSafeHwnd();
      bi.pszDisplayName = 0;
      CString title;
      if (! title.LoadString(IDS_BROWSE_LOCAL))
	{
	  UNEXPECTED_CONDITION ("LocalRepositoryPage::OnBrowse");
	}
      bi.lpszTitle = title;
      bi.ulFlags = (BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS);
      if (theApp.GetSetupTask() == SetupTask::InstallFromLocalRepository)
	{
	  bi.ulFlags |= BIF_NONEWFOLDERBUTTON;
	}
      LPCITEMIDLIST pidl = SHBrowseForFolder(&bi);
      if (pidl == 0)
	{
	  return;
	}
      _TCHAR szFolderPath[BufferSizes::MaxPath];
      BOOL havePath = SHGetPathFromIDList(pidl, szFolderPath);
      CoTaskMemFree (const_cast<LPITEMIDLIST>(pidl));
      if (! havePath)
	{
	  UNEXPECTED_CONDITION ("LocalRepositoryPage::OnBrowse");
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

   LocalRepositoryPage::OnChangePathName
   _________________________________________________________________________ */

void
LocalRepositoryPage::OnChangePathName ()
{
  try
    {
      CWnd * pWnd = GetDlgItem(IDC_FILENAME);
      if (pWnd == 0)
	{
	  UNEXPECTED_CONDITION ("LocalRepositoryPage::OnChangeFilename");
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
