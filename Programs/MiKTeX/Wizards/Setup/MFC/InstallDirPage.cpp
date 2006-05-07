/* InstallDirPage.cpp:

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

#include "InstallDirPage.h"
#include "SetupWizard.h"

/* _________________________________________________________________________

   InstallDirPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(InstallDirPage, CPropertyPage)
  ON_EN_CHANGE(IDC_PATHNAME, OnChangePathName)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   InstallDirPage::InstallDirPage
   _________________________________________________________________________ */

InstallDirPage::InstallDirPage ()
  : CPropertyPage (IDD,
		   0,
		   IDS_HEADER_INSTALLDIR,
		   IDS_SUBHEADER_INSTALLDIR),
    pSheet (0)
{
}

/* _________________________________________________________________________

   InstallDirPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
InstallDirPage::OnInitDialog ()
{
  m_strInstallDir = theApp.startupConfig.installRoot.Get();
  pSheet = reinterpret_cast<SetupWizard *>(GetParent());
  return (CPropertyPage::OnInitDialog());
}

/* _________________________________________________________________________

   InstallDirPage::OnSetActive
   _________________________________________________________________________ */

BOOL
InstallDirPage::OnSetActive ()
{
  noDdv = false;
  BOOL ret = CPropertyPage::OnSetActive();
  pSheet->SetWizardButtons (PSWIZB_BACK
			    | (m_strInstallDir.GetLength() > 0
			       ? PSWIZB_NEXT
			       : 0));
  return (ret);
}

/* _________________________________________________________________________

   InstallDirPage::DoDataExchange
   _________________________________________________________________________ */

void
InstallDirPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);

  try
    {
      DDX_Text (pDX, IDC_PATHNAME, m_strInstallDir);
      if (pDX->m_bSaveAndValidate)
	{
	  m_strInstallDir.Trim ();
	  m_strInstallDir.Replace (T_('/'), T_('\\'));
	  if (m_strInstallDir.GetLength() > 3)
	    {
	      m_strInstallDir.TrimRight (T_('\\'));
	    }
	}
      DDV_Path (pDX, m_strInstallDir);
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
      if (pDX->m_bSaveAndValidate)
	{
	  pDX->Fail ();
	}
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
      if (pDX->m_bSaveAndValidate)
	{
	  pDX->Fail ();
	}
    }
}

/* _________________________________________________________________________

   InstallDirPage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
InstallDirPage::OnWizardNext ()
{
#if SHOW_FOLDER_PAGE
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_FOLDER)));
#else
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_SETTINGS)));
#endif
}

/* _________________________________________________________________________

   InstallDirPage::OnWizardBack
   _________________________________________________________________________ */

LRESULT
InstallDirPage::OnWizardBack ()
{
  noDdv = true;
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_SHARED)));
}

/* _________________________________________________________________________

   InstallDirPage::OnKillActive
   _________________________________________________________________________ */

BOOL
InstallDirPage::OnKillActive ()
{
  BOOL ret = CPropertyPage::OnKillActive();
  if (ret && ! noDdv)
    {
      try
	{
	  PathName uninstLog (m_strInstallDir);
	  uninstLog += MIKTEX_PATH_UNINST_LOG;
	  if (File::Exists(uninstLog))
	    {
	      AfxMessageBox (IDS_ALREADY_INSTALLED, MB_OK | MB_ICONSTOP);
	      ret = FALSE;
	    }
	  else
	    {
	      theApp.startupConfig.installRoot = m_strInstallDir;
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

   InstallDirPage::OnChangePathName
   _________________________________________________________________________ */

void
InstallDirPage::OnChangePathName ()
{
  try
    {
      CWnd * pWnd = GetDlgItem(IDC_PATHNAME);
      if (pWnd == 0)
	{
	  UNEXPECTED_CONDITION (T_("InstallDirPage::OnChangePathName"));
	}
      CString str;
      pWnd->GetWindowText (str);
      str.TrimLeft ();
      str.TrimRight ();
      pSheet->SetWizardButtons (PSWIZB_BACK
				| (str.GetLength() > 0 ? PSWIZB_NEXT : 0));
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
