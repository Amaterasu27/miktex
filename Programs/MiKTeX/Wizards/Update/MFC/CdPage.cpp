/* CdPage.cpp

   Copyright (C) 2002-2006 Christian Schenk

   This file is part of MiKTeX Update Wizard.

   MiKTeX Update Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   MiKTeX Update Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX Update Wizard; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"
#include "Update.h"

#include "CdPage.h"
#include "UpdateWizard.h"

IMPLEMENT_DYNCREATE(CdPage, CPropertyPage);

/* _________________________________________________________________________

   CdPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(CdPage, CPropertyPage)
  ON_CBN_SELCHANGE(IDC_CD, OnSelchangeComboDrive)
END_MESSAGE_MAP()

/* _________________________________________________________________________

   CdPage::CdPage
   _________________________________________________________________________ */

CdPage::CdPage ()
  : CPropertyPage (IDD, 0, IDS_HEADER_CD, IDS_SUBHEADER_CD),
    drive (-1),
    pSheet (0),
    noDriveFound (true)
{
  m_psp.dwFlags &= ~ PSP_HASHELP;
}

/* _________________________________________________________________________

   CdPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
CdPage::OnInitDialog ()
{
  pSheet = reinterpret_cast<UpdateWizard *>(GetParent());
  return (CPropertyPage::OnInitDialog());
}

/* _________________________________________________________________________

   CdPage::OnSetActive
   _________________________________________________________________________ */

BOOL
CdPage::OnSetActive ()
{
  noDdv = false;
  BOOL ret = CPropertyPage::OnSetActive();
  if (ret)
    {
      try
	{
	  ShowDrives ();
	  EnableButtons ();
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
  return (ret);
}

/* _________________________________________________________________________

   CdPage::DoDataExchange
   _________________________________________________________________________ */

void
CdPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_CBIndex (pDX, IDC_CD, drive);
  DDX_Control (pDX, IDC_CD, driveComboBox);
}

/* _________________________________________________________________________

   CdPage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
CdPage::OnWizardNext ()
{
  pSheet->SetCameFrom (IDD);
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_PACKAGE_LIST)));
}

/* _________________________________________________________________________

   CdPage::OnWizardBack
   _________________________________________________________________________ */

LRESULT
CdPage::OnWizardBack ()
{
  noDdv = true;
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_TYPE)));
}
/* _________________________________________________________________________

   CdPage::OnKillActive
   _________________________________________________________________________ */

BOOL
CdPage::OnKillActive ()
{
  BOOL ret = CPropertyPage::OnKillActive();
  if (ret && ! noDdv && ! noDriveFound)
    {
      try
	{
	  g_pManager->SetMiKTeXDirectRoot (PathName(drives[drive].c_str(),
						    "\\",
						    0,
						    0));
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

   CdPage::OnSelchangeComboDrive
   _________________________________________________________________________ */

void
CdPage::OnSelchangeComboDrive ()
{
  try
    {
      EnableButtons ();
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

   CdPage::Showdrives 
   _________________________________________________________________________ */

void
CdPage::ShowDrives ()
{
  CWaitCursor curWait;

  driveComboBox.ResetContent ();
  drives.clear ();
  drive = -1;
  noDriveFound = true;

  AutoErrorMode autoMode (SetErrorMode(SEM_FAILCRITICALERRORS));

  DWORD logicalDrives = GetLogicalDrives();
  for (int drv = 0;
       logicalDrives != 0;
       logicalDrives >>= 1, ++ drv)
    {
      if ((logicalDrives & 1) == 0)
	{
	  continue;
	}

      char szDrive[4];
      szDrive[0] = char('A') + static_cast<char>(drv);
      szDrive[1] = ':';
      szDrive[2] = '\\';
      szDrive[3] = 0;

      DWORD fileSystemFlags;
      DWORD maximumComponentLength;
      char fileSystemName[BufferSizes::MaxPath];
      char volumeName[BufferSizes::MaxPath];

      if (! GetVolumeInformation(szDrive,
				 volumeName,
				 BufferSizes::MaxPath,
				 0,
				 &maximumComponentLength, 
				 &fileSystemFlags,
				 fileSystemName,
				 BufferSizes::MaxPath))
	{
	  continue;
	}

      if (! Utils::IsMiKTeXDirectRoot(szDrive))
	{
	  continue;
	}

      szDrive[2] = 0;

      CString comboBoxItem (szDrive);
      comboBoxItem += " (";
      comboBoxItem += volumeName;
      comboBoxItem += ')';
      if (driveComboBox.AddString(comboBoxItem) < 0)
	{
	  FATAL_WINDOWS_ERROR ("CComboBox::AddString", 0);
	}

      drives.push_back (szDrive);

      noDriveFound = false;
    }

  if (noDriveFound)
    {
      CString str;
      if (! str.LoadString(IDS_NO_DISK))
	{
	  FATAL_WINDOWS_ERROR ("CString::LoadString", 0);
	}
      driveComboBox.AddString (str);
    }

  drive = 0;
  driveComboBox.SetCurSel (0);
}

/* _________________________________________________________________________

   CdPage::EnableButtons
   _________________________________________________________________________ */

void
CdPage::EnableButtons ()
{
  DWORD flags = PSWIZB_BACK;
  if (driveComboBox.GetCurSel() >= 0 && ! noDriveFound)
    {
      flags |= PSWIZB_NEXT;
    }
  pSheet->SetWizardButtons (flags);
}

