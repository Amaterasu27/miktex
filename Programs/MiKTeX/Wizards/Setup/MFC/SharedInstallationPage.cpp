/* SharedInstallationPage.cpp:

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

#include "SetupWizard.h"
#include "SharedInstallationPage.h"

/* _________________________________________________________________________

   SharedInstallationPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(SharedInstallationPage, CPropertyPage)
  ON_BN_CLICKED(IDC_SHARED, &SharedInstallationPage::OnShared)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   SharedInstallationPage::SharedInstallationPage
   _________________________________________________________________________ */

SharedInstallationPage::SharedInstallationPage ()
  : CPropertyPage (IDD,
		   0,
		   IDS_HEADER_SHARED,
		   IDS_SUBHEADER_SHARED),
    commonUserSetup (-1),
    pSheet (0)
{
}

/* _________________________________________________________________________

   SharedInstallationPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
SharedInstallationPage::OnInitDialog ()
{
  pSheet = reinterpret_cast<SetupWizard*>(GetParent());

  commonUserSetup = (theApp.IsCommonSetup() ? 0 : 1);

  BOOL ret = CPropertyPage::OnInitDialog();

  try
    {
      if (IsWindowsNT()
	  && ! (SessionWrapper(true)->RunningAsAdministrator()
		|| SessionWrapper(true)->RunningAsPowerUser()))
	{
	  CWnd * pWnd = GetDlgItem(IDC_SHARED);
	  if (pWnd == 0)
	    {
	      UNEXPECTED_CONDITION
		("SharedInstallationPage::OnInitDialog");
	    }
	  pWnd->EnableWindow (FALSE);
	}
      CWnd * pWnd = GetDlgItem(IDC_JUST_FOR_ME);
      if (pWnd == 0)
	{
	  UNEXPECTED_CONDITION ("SharedInstallationPage::OnInitDialog");
	}
      wchar_t szLogonName[30];
      DWORD sizeLogonName = sizeof(szLogonName) / sizeof(szLogonName[0]);
      if (! GetUserNameW(szLogonName, &sizeLogonName))
	{
	  if (GetLastError() == ERROR_NOT_LOGGED_ON)
	    {
	      Utils::CopyString (szLogonName, 30, L"unknown user");
	    }
	  else
	    {
	      FATAL_WINDOWS_ERROR ("GetUserNameW", 0);
	    }
	}
      CString str;
      pWnd->GetWindowText (str);
      str += L" ";
      str += szLogonName;
      if (IsWindowsNT())
	{
	  wchar_t szDisplayName[30];
	  DllProc3<BOOLEAN, EXTENDED_NAME_FORMAT, LPWSTR, PULONG>
	    getUserNameExW ("Secur32.dll", "GetUserNameExW");
	  ULONG sizeDisplayName =
	    sizeof(szDisplayName) / sizeof(szDisplayName[0]);
	  if (getUserNameExW(NameDisplay, szDisplayName, &sizeDisplayName))
	    {
	      str += L" (";
	      str += szDisplayName;
	      str += L')';
	    }
	}
      pWnd->SetWindowText(str);
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

   SharedInstallationPage::OnSetActive
   _________________________________________________________________________ */

BOOL
SharedInstallationPage::OnSetActive ()
{
  pSheet->SetWizardButtons (PSWIZB_BACK | PSWIZB_NEXT);
  return (CPropertyPage::OnSetActive());
}

/* _________________________________________________________________________

   SharedInstallationPage::DoDataExchange
   _________________________________________________________________________ */

void
SharedInstallationPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Radio (pDX, IDC_SHARED, commonUserSetup);
}

/* _________________________________________________________________________

   SharedInstallationPage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
SharedInstallationPage::OnWizardNext ()
{
  UINT next;
  if (theApp.pSetupService->GetOptions().IsPrefabricated)
    {
      next = IDD_INSTALLDIR;
    }
  else
    {
      switch (theApp.GetSetupTask().Get())
	{
	case SetupTask::InstallFromCD:
	  next = IDD_INSTALLDIR;
	  break;
	case SetupTask::InstallFromLocalRepository:
	  next = IDD_LOCAL_REPOSITORY;
	  break;
	case SetupTask::InstallFromRemoteRepository:
	  next = IDD_REMOTE_REPOSITORY;
	  break;
	case SetupTask::PrepareMiKTeXDirect:
#if SHOW_FOLDER_PAGE
	  next = IDD_FOLDER;
#else
	  next = IDD_SETTINGS;
#endif
	  break;
	default:
	  ASSERT (false);
	  __assume (false);
	  break;
	}
    }
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(next)));
}

/* _________________________________________________________________________

   SharedInstallationPage::OnWizardBack
   _________________________________________________________________________ */

LRESULT
SharedInstallationPage::OnWizardBack ()
{
  UINT prev;
  if (theApp.pSetupService->GetOptions().IsPrefabricated)
    {
      prev = 
	(theApp.prefabricatedPackageLevel == PackageLevel::Complete
	 ? IDD_PACKAGE_SET_INSTALL
	 : IDD_LICENSE);
    }
  else
    {
      switch (theApp.GetSetupTask().Get())
	{
	case SetupTask::InstallFromCD:
	case SetupTask::InstallFromLocalRepository:
	case SetupTask::InstallFromRemoteRepository:
	  prev = IDD_PACKAGE_SET_INSTALL;
	  break;
	case SetupTask::PrepareMiKTeXDirect:
	  prev = IDD_MD_TASK;
	  break;
	default:
	  ASSERT (false);
	  __assume (false);
	  break;
	}
    }
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(prev)));
}

/* _________________________________________________________________________

   SharedInstallationPage::OnKillActive
   _________________________________________________________________________ */

BOOL
SharedInstallationPage::OnKillActive ()
{
  BOOL ret = CPropertyPage::OnKillActive();
  if (ret)
    {
      SetupOptions options = theApp.pSetupService->GetOptions();
      options.IsCommonSetup = (commonUserSetup == 0);
      theApp.pSetupService->SetOptions(options);
    }
  return (ret);
}

/* _________________________________________________________________________

   SharedInstallationPage::OnShared
   _________________________________________________________________________ */

void
SharedInstallationPage::OnShared ()
{
  try
    {
      if (IsWindowsNT()
	  && ! (SessionWrapper(true)->RunningAsAdministrator()
		|| SessionWrapper(true)->RunningAsPowerUser()))
	{
	  AfxMessageBox (T_(_T("You must have administrator privileges to set up \
a shared MiKTeX system.")),
			 MB_OK | MB_ICONSTOP);
	  commonUserSetup = 1;
	  UpdateData (FALSE);
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
}
