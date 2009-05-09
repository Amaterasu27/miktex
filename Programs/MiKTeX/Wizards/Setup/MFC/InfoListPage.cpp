/* InfoListPage.cpp:

   Copyright (C) 1999-2009 Christian Schenk

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

#include "InfoListPage.h"
#include "SetupWizard.h"

/* _________________________________________________________________________
   
   InfoListPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(InfoListPage, CPropertyPage)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   InfoListPage::InfoListPage
   _________________________________________________________________________ */

InfoListPage::InfoListPage ()
  : CPropertyPage(IDD,
		  0,
		  IDS_HEADER_INFOLIST,
		  IDS_SUBHEADER_INFOLIST),
    pSheet (0)
{
}

/* _________________________________________________________________________

   InfoListPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
InfoListPage::OnInitDialog ()
{
  pSheet = reinterpret_cast<SetupWizard *>(GetParent());
  return (CPropertyPage::OnInitDialog());
}

/* _________________________________________________________________________

   InfoListPage::OnSetActive
   _________________________________________________________________________ */

BOOL
InfoListPage::OnSetActive ()
{
  BOOL ret = CPropertyPage::OnSetActive();

  if (ret)
    {
      try
	{
	  CreateReport ();
	  oldNextText = pSheet->SetNextText (T_("&Start"));
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

   InfoListPage::DoDataExchange
   _________________________________________________________________________ */

void
InfoListPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);

  DDX_Text (pDX, IDC_INFO, info);
}

/* _________________________________________________________________________

   InfoListPage::OnKillActive
   _________________________________________________________________________ */

BOOL
InfoListPage::OnKillActive ()
{
  BOOL ret = CPropertyPage::OnKillActive();

  if (ret)
    {
      try
	{
	  pSheet->SetNextText (oldNextText);
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

   InfoListPage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
InfoListPage::OnWizardNext ()
{
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_FILECOPY)));
}

/* _________________________________________________________________________

   InfoListPage::OnWizardBack
   _________________________________________________________________________ */

LRESULT
InfoListPage::OnWizardBack ()
{
  UINT prev;
  switch (theApp.setupTask.Get())
    {
    case SetupTask::Download:
      prev = IDD_LOCAL_REPOSITORY;
      break;
    case SetupTask::PrepareMiKTeXDirect:
      prev = IDD_SETTINGS;
      break;
    case SetupTask::InstallFromCD:
    case SetupTask::InstallFromLocalRepository:
    case SetupTask::InstallFromRemoteRepository:
#if ENABLE_ADDTEXMF
      prev = (theApp.noAddTEXMFDirs
	      ? IDD_ADD_TEXMFROOTS
	      : IDD_EDIT_TEXMFROOTS);
#else
      prev = IDD_SETTINGS;
#endif
      break;
    default:
      ASSERT (false);
      __assume (false);
      break;
    }
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(prev)));
}
/* _________________________________________________________________________

   InfoListPage::CreateReport
   _________________________________________________________________________ */

void
InfoListPage::CreateReport ()
{
  CString CRLF ("\r\n");
  CString TAB ("\t");

  CString packageSet;

  switch (theApp.packageLevel.Get())
    {
    case PackageLevel::Essential:
      packageSet = T_("essential packages");
      break;
    case PackageLevel::Basic:
      packageSet = T_("basic packages");
      break;
    case PackageLevel::Complete:
      packageSet = T_("all packages");
      break;
    default:
      MIKTEX_ASSERT (false);
      __assume (false);
      break;
    }

  info = "";

  switch (theApp.setupTask.Get())
    {
    case SetupTask::Download:
      info += (T_("Download ") + packageSet + T_(" from ") + CRLF
	       + TAB + theApp.remotePackageRepository.c_str() + CRLF
	       + T_(" to ") + CRLF
	       + TAB + theApp.localPackageRepository.Get());
      break;
    case SetupTask::InstallFromCD:
      info += (T_("Install ") + packageSet + T_(" from CD/DVD"));
      break;
    case SetupTask::InstallFromLocalRepository:
      if (theApp.prefabricated)
	{
	  info += (T_("Install ") + packageSet + T_(" to ") + CRLF
		   + TAB + theApp.GetInstallRoot().Get());
	}
      else
	{
	  info += (T_("Install ") + packageSet + T_(" from ") + CRLF
		   + TAB + theApp.localPackageRepository.Get() + CRLF
		   + T_(" to ") + CRLF
		   + TAB + theApp.GetInstallRoot().Get());
	}
      break;
    case SetupTask::InstallFromRemoteRepository:
      info += (T_("Install ") + packageSet + T_(" from ") + CRLF
	       + TAB + theApp.remotePackageRepository.c_str() + CRLF
	       + T_(" to ") + CRLF
	       + TAB + theApp.GetInstallRoot().Get());
      break;
    case SetupTask::PrepareMiKTeXDirect:
      info += T_("Prepare to run MiKTeX from ");
      info += theApp.MiKTeXDirectTeXMFRoot.Get();
      break;
    default:
      MIKTEX_ASSERT (false);
      __assume (false);
      break;
    }

  info += CRLF + CRLF;
  
  if (theApp.setupTask != SetupTask::Download)
    {
      if (theApp.setupTask != SetupTask::PrepareMiKTeXDirect)
	{
	  if (theApp.portable)
	  {
	    info += T_("Install MiKTeX Portable");
	  }
	  else if (theApp.commonUserSetup)
	  {
	    info += T_("Install MiKTeX for all users");
	  }
	  else
	  {
	    info += T_("Install MiKTeX only for the current user");
	  }
	  info += CRLF;
	  info += CRLF;
	}
#if SHOW_FOLDER_PAGE
      info += T_("Add shortcuts to ");
      info += theApp.folderName.c_str();
      info += CRLF;
      info += CRLF;
#endif
      info += T_("Preferred paper size is ");
      info += theApp.paperSize.c_str();
      info += CRLF;
      info += CRLF;
      if (theApp.setupTask != SetupTask::PrepareMiKTeXDirect
	  && theApp.setupTask != SetupTask::Download)
	{
	  switch (theApp.installOnTheFly.Get())
	    {
	    case TriState::True:
	      info += T_("Packages will be installed on-the-fly");
	      break;
	    case TriState::False:
	      info += T_("Packages will not be installed on-the-fly");
	      break;
	    case TriState::Undetermined:
	      info +=
		T_("Packages will be installed after confirmation by user");
	      break;
	    }
	  info += CRLF;
	  info += CRLF;
	}
    }

  CWnd * pWnd = GetDlgItem(IDC_INFO);
  if (pWnd == 0)
    {
      UNEXPECTED_CONDITION ("InfoListPage::CreateReport");
    }
  pWnd->SetWindowText (info);
}
