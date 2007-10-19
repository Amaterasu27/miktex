/* PropPagePackages.cpp:

   Copyright (C) 2000-2007 Christian Schenk

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

#include "PropPagePackages.h"
#include "PropSheet.h"
#include "resource.hm"

/* _________________________________________________________________________

   PropPagePackages::PropPagePackages
   _________________________________________________________________________ */

PropPagePackages::PropPagePackages (/*[in]*/ PackageManager * pManager)
  : CPropertyPage (PropPagePackages::IDD),
    pManager (pManager),
    pPackageTreeCtrl (PackageTreeCtrl::Create(pManager))
{
  m_psp.dwFlags &= ~(PSP_HASHELP);
  string repository;
  RepositoryType repositoryType (RepositoryType::Unknown);
  if (pManager->TryGetDefaultPackageRepository(repositoryType, repository))
    {
      this->url = repository.c_str();
    }
  else
    {
      this->url = (T_("<Random package repository>"));
    }
}

/* _________________________________________________________________________

   PropPagePackages::~PropPagePackages
   _________________________________________________________________________ */

PropPagePackages::~PropPagePackages ()
{
  try
    {
      PackageTreeCtrl::Destroy (pPackageTreeCtrl);
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   PropPagePackages::DoDataExchange
   _________________________________________________________________________ */

void
PropPagePackages::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_TREE, *pPackageTreeCtrl);
  DDX_Text (pDX, IDC_URL, url);
}

/* _________________________________________________________________________

   PropPagePackages Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(PropPagePackages, CPropertyPage)
  ON_BN_CLICKED(IDC_CHANGE_URL, OnChangeUrl)
  ON_BN_CLICKED(IDC_PACKAGE_MANAGER, OnBnClickedPackageManager)
  ON_MESSAGE(PackageTreeCtrl::WM_ONTOGGLE, OnToggle)
  ON_NOTIFY(TVN_GETINFOTIP, IDC_TREE, OnGetInfoTip)
  ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
  ON_WM_CONTEXTMENU()
  ON_WM_HELPINFO()
END_MESSAGE_MAP();

/* _________________________________________________________________________

   PropPagePackages::OnInitDialog
   _________________________________________________________________________ */

BOOL
PropPagePackages::OnInitDialog () 
{
  BOOL ret = CPropertyPage::OnInitDialog();
  try
    {
      CWaitCursor cur;
      pPackageTreeCtrl->Refresh ();
      ShowPackageInfo (0);
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

/* _________________________________________________________________________

   PropPagePackages::OnApply
   _________________________________________________________________________ */

BOOL
PropPagePackages::OnApply ()
{
  try
    {
      vector<string> toBeInstalled;
      vector<string> toBeRemoved;
      pPackageTreeCtrl->GetDelta (toBeInstalled, toBeRemoved);
      if (toBeInstalled.size() == 0 && toBeRemoved.size() == 0)
	{
	  return (CPropertyPage::OnApply());
	}
      CString str1;
      str1.Format (_T("%u"), toBeInstalled.size());
      CString str2;
      str2.Format (_T("%u"), toBeRemoved.size());
      CString str;
      AfxFormatString2 (str, IDP_UPDATE_MESSAGE, str1, str2);
      bool restrictedUserSetup =
	(! (SessionWrapper(true)->IsSharedMiKTeXSetup() == TriState::True
	    || SessionWrapper(true)->IsUserAnAdministrator()));
      if (IsWindowsVista() && ! restrictedUserSetup)
	{
	  DllProc4<HRESULT, const TASKDIALOGCONFIG *, int *, int *, BOOL *>
	    taskDialogIndirect (T_("comctl32.dll"), T_("TaskDialogIndirect"));
	  TASKDIALOGCONFIG taskDialogConfig;
	  memset (&taskDialogConfig, 0, sizeof(taskDialogConfig));
	  taskDialogConfig.cbSize = sizeof(TASKDIALOGCONFIG);
	  taskDialogConfig.hwndParent = 0;
	  taskDialogConfig.hInstance = 0;
	  taskDialogConfig.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION;
	  taskDialogConfig.pszMainIcon = MAKEINTRESOURCEW(TD_SHIELD_ICON);
	  taskDialogConfig.pszWindowTitle =
	    MAKEINTRESOURCEW(AFX_IDS_APP_TITLE);
	  taskDialogConfig.pszMainInstruction = L"Do you want to proceed?";
	  CStringW strContent (str);
	  taskDialogConfig.pszContent = strContent;
	  taskDialogConfig.cButtons = 2;
	  TASKDIALOG_BUTTON const buttons[] = {
	    {IDOK, L"Proceed"},
	    {IDCANCEL, L"Cancel"}
	  };
	  taskDialogConfig.pButtons = buttons;
	  taskDialogConfig.nDefaultButton = IDOK;
	  int result = 0;
	  if(SUCCEEDED(taskDialogIndirect(&taskDialogConfig, &result, 0, 0)))
	    {
	      if (IDOK != result)
		{
		  return (FALSE);
		}
	    }
	  else
	    {
	      UNEXPECTED_CONDITION ("PropPagePackages::OnApply");
	    }
	}
      else
	{
	  if (AfxMessageBox(str, MB_OKCANCEL | MB_ICONINFORMATION) == IDCANCEL)
	    {
	      return (FALSE);
	    }
	}
      if (UpdateDialog::DoModal(this,
				pManager.Get(),
				toBeInstalled,
				toBeRemoved)
	  == IDOK)
	{
	  SetElevationRequired (false);
	  return (TRUE);
	}
      else
	{
	  SetElevationRequired (true);
	  return (FALSE);
	}
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

   aHelpIDs
   _________________________________________________________________________ */

#define MAKE_ID_HID_PAIR(id) id, H##id

namespace
{
  const DWORD aHelpIDs[] = {
    MAKE_ID_HID_PAIR(IDC_CHANGE_URL),
    MAKE_ID_HID_PAIR(IDC_INFO),
    MAKE_ID_HID_PAIR(IDC_INSTALLED),
    MAKE_ID_HID_PAIR(IDC_PACKAGE_MANAGER),
    MAKE_ID_HID_PAIR(IDC_PACKAGE_SIZE),
    MAKE_ID_HID_PAIR(IDC_PACKED),
    MAKE_ID_HID_PAIR(IDC_TREE),
    MAKE_ID_HID_PAIR(IDC_URL),
    MAKE_ID_HID_PAIR(IDC_VERSION),
    0, 0,
  };
}

/* _________________________________________________________________________

   PropPagePackages::OnHelpInfo
   _________________________________________________________________________ */

BOOL
PropPagePackages::OnHelpInfo (/*[in]*/ HELPINFO * pHelpInfo) 
{
  return (::OnHelpInfo(pHelpInfo, aHelpIDs, _T(_T("PackagesPage.txt"))));
}

/* _________________________________________________________________________

   PropPagePackages::OnContextMenu
   _________________________________________________________________________ */

void
PropPagePackages::OnContextMenu (/*[in]*/ CWnd *	pWnd,
				 /*[in]*/ CPoint	point) 
{
  try
    {
      DoWhatsThisMenu (pWnd, point, aHelpIDs, _T(_T("PackagesPage.txt")));
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

   PropPagePackages::OnSelchangedTree
   _________________________________________________________________________ */

void
PropPagePackages::OnSelchangedTree (/*[in]*/ NMHDR *	pNMHDR,
				    /*[in]*/ LRESULT *	pResult) 
{
  try
    {
      NM_TREEVIEW * pNMTreeView = reinterpret_cast<NM_TREEVIEW*>(pNMHDR);
      MIKTEX_ASSERT (pNMTreeView != 0);
      PackageInfo packageInfo;
      *pResult = 0;
      if (pNMTreeView->itemNew.hItem == 0)
	{
	  return;
	}
      if (pPackageTreeCtrl->GetPackageInfo(pNMTreeView->itemNew.hItem,
					   packageInfo))
	{
	  ShowPackageInfo (&packageInfo);
	}
      else
	{
	  ShowPackageInfo (0);
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

   PropPagePackages::SetWindowText
   _________________________________________________________________________ */

void
PropPagePackages::SetWindowText (/*[in]*/ UINT			controlId,
				 /*[in]*/ const char *	lpszText)
{
  CWnd * pWnd = GetDlgItem(controlId);
  if (pWnd == 0)
    {
      UNEXPECTED_CONDITION ("PropPagePackages::SetWindowText");
    }
  pWnd->SetWindowText (lpszText);
}

/* _________________________________________________________________________

   PropPagePackages::ShowPackageInfo
   _________________________________________________________________________ */

void
PropPagePackages::ShowPackageInfo (/*[in]*/ const PackageInfo * pPackageInfo)
{
  if (pPackageInfo == 0)
    {
      SetWindowText (IDC_INFO, "");
      SetWindowText (IDC_INSTALLED, "");
      SetWindowText (IDC_PACKAGE_SIZE, "");
      SetWindowText (IDC_VERSION, "");
    }
  else
    {
      SetWindowText (IDC_INFO, pPackageInfo->title.c_str());
      SetWindowText (IDC_VERSION, pPackageInfo->version.c_str());
      if (pPackageInfo->timeInstalled > 0)
	{
	  CTime timeInstall (pPackageInfo->timeInstalled);
	  SetWindowText (IDC_INSTALLED, timeInstall.Format(_T("%x %X")));
	}
      else
	{
	  SetWindowText (IDC_INSTALLED, _T("<not installed>"));
	}
      if (! pPackageInfo->IsPureContainer() && pPackageInfo->timePackaged > 0)
	{
    	  CTime timePackaged (pPackageInfo->timePackaged);
	  SetWindowText (IDC_PACKED, timePackaged.Format(_T("%x %X")));
	}
      else
	{
	  SetWindowText (IDC_PACKED, _T(""));
	}
      CString text;
      text.Format (_T("%u Bytes"),
		   (pPackageInfo->sizeRunFiles
		    + pPackageInfo->sizeDocFiles
		    + pPackageInfo->sizeSourceFiles));
      SetWindowText (IDC_PACKAGE_SIZE, text);
    }
}

/* _________________________________________________________________________

   PropPagePackages::OnToggle
   _________________________________________________________________________ */

LRESULT
PropPagePackages::OnToggle (/*[in]*/ WPARAM	wParam,
			    /*[in]*/ LPARAM	lParam)
{
  UNUSED_ALWAYS (wParam);
  UNUSED_ALWAYS (lParam);
  SetChanged (true);
  return (0);
}

/* _________________________________________________________________________

   PropPagePackages::OnChangeUrl
   _________________________________________________________________________ */

void
PropPagePackages::OnChangeUrl ()
{
  try
    {
      if (SiteWizSheet::DoModal(this) == ID_WIZFINISH)
	{
	  string repository;
	  RepositoryType repositoryType =
	    pManager->GetDefaultPackageRepository(repository);
	  this->url = repository.c_str();
	  SetWindowText (IDC_URL, this->url);
	  Scan ();
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

   PropPagePackages::Scan
   _________________________________________________________________________ */

void
PropPagePackages::Scan ()
{
  pManager->UnloadDatabase ();
  auto_ptr<PackageInstaller> pInstaller (pManager->CreateInstaller());
  pInstaller->UpdateDbAsync ();
  auto_ptr<ProgressDialog> pProgDlg (ProgressDialog::Create());
  pProgDlg->SetTitle (_T("Package Database Maintenance"));
  pProgDlg->SetLine (1, _T("Downloading package database..."));
  pProgDlg->SetLine (2, _T(""));
  pProgDlg->StartProgressDialog (GetSafeHwnd());
  while (! pProgDlg->HasUserCancelled())
    {
      PackageInstaller::ProgressInfo progressinfo =
	pInstaller->GetProgressInfo();
      if (progressinfo.ready)
	{
	  break;
	}
      Sleep (1000);
    }
  if (! pProgDlg->HasUserCancelled())
    {
      pPackageTreeCtrl->Refresh ();
      ShowPackageInfo (0);
    }
  pProgDlg->StopProgressDialog ();
  pInstaller->Dispose ();
}

/* _________________________________________________________________________

   PropPagePackages::OnBnClickedPackageManager
   _________________________________________________________________________ */

void
PropPagePackages::OnBnClickedPackageManager ()
{
  try
    {
      PathName path;
      if (! SessionWrapper(true)->FindFile(MIKTEX_MPM_MFC_EXE,
					   FileType::EXE,
					   path))
	{
	  FATAL_MIKTEX_ERROR
	    ("PropPagePackages::OnBnClickedPackageManager",
	     T_("The package manager application could not be found."),
	     0);
	}
      Process::Start (path);
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

   PropPagePackages::OnGetInfoTip
   _________________________________________________________________________ */

void
PropPagePackages::OnGetInfoTip (/*[in]*/ NMHDR *	pNMHDR,
				/*[in]*/ LRESULT *	pResult) 
{
  try
    {
      NMTVGETINFOTIP * pInfoTip = reinterpret_cast<NMTVGETINFOTIP*>(pNMHDR);
      MIKTEX_ASSERT (pInfoTip != 0);
      *pResult = 0;
      PackageInfo pi;
      if (! pPackageTreeCtrl->GetPackageInfo(pInfoTip->hItem, pi))
	{
	  return;
	}
#if 0
      set<string> directories;
      for (vector<string>::const_iterator it = pi.runFiles.begin();
	   it != pi.runFiles.end();
	   ++ it)
	{
	  PathName path (it->c_str());
	  if (! path.HasExtension(_T(".tpm")))
	    {
	      directories.insert (path.RemoveFileSpec().Get());
	    }
	}
      for (vector<string>::const_iterator it = pi.docFiles.begin();
	   it != pi.docFiles.end();
	   ++ it)
	{
	  PathName path (it->c_str());
	  directories.insert (path.RemoveFileSpec().Get());
	}
#endif
      string info;
      if (! pi.title.empty())
	{
	  info += pi.title;
	}
      if (! pi.description.empty())
	{
	  if (! info.empty())
	    {
	      info += _T("\r\n\r\n");
	      info += pi.description;
	    }
	}
#if 0
      if (directories.size() > 0)
	{
	  if (! info.empty())
	    {
	      info += _T("\r\n\r\n");
	    }
	  for (set<string>::const_iterator it = directories.begin();
	       it != directories.end();
	       ++ it)
	    {
	      if (it != directories.begin())
		{
		  info += _T("\r\n");
		}
	      info += *it;
	    }
	}
#endif
      _tcsncpy_s (pInfoTip->pszText,
		  pInfoTip->cchTextMax,
		  info.c_str(),
		  _TRUNCATE);
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

   PropPagePackages::SetElevationRequired
   _________________________________________________________________________ */

void
PropPagePackages::SetElevationRequired (/*[in]*/ bool f)
{
  bool restrictedUserSetup =
    (! (SessionWrapper(true)->IsSharedMiKTeXSetup() == TriState::True
	|| SessionWrapper(true)->IsUserAnAdministrator()));
  if (IsWindowsVista() && ! restrictedUserSetup)
    {
      HWND hwnd = ::GetDlgItem(::GetParent(m_hWnd), IDOK);
      if (hwnd == 0)
	{
	  UNEXPECTED_CONDITION ("PropPagePackages::SetElevationRequired");
	}
      Button_SetElevationRequiredState (hwnd, f ? TRUE : FALSE);
      hwnd = ::GetDlgItem(::GetParent(m_hWnd), ID_APPLY_NOW);
      if (hwnd == 0)
	{
	  UNEXPECTED_CONDITION ("PropPagePackages::SetElevationRequired");
	}
      Button_SetElevationRequiredState (hwnd, f ? TRUE : FALSE);
    }
}

/* _________________________________________________________________________

   PropPagePackages::SetChanged
   _________________________________________________________________________ */

void
PropPagePackages::SetChanged (/*[in]*/ bool f)
{
  SetElevationRequired (f);
  SetModified (f ? TRUE : FALSE);
}
