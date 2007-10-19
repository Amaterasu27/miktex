/* SiteWizDrive.cpp:

   Copyright (C) 2000-2006 Christian Schenk

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
#include "Resource.h"

#include "SiteWizDrive.h"

/* _________________________________________________________________________

   SiteWizDrive::SiteWizDrive
   _________________________________________________________________________ */

SiteWizDrive::SiteWizDrive (/*[in]*/ PackageManager * pManager)
  : CPropertyPage (IDD_SITEWIZ_DRIVE, IDS_SITEWIZ),
    pManager (pManager),
    drive (-1)
{
  m_psp.dwFlags &= ~ PSP_HASHELP;
}

/* _________________________________________________________________________

   SiteWizDrive Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(SiteWizDrive, CPropertyPage)
  ON_CBN_SELCHANGE(IDC_COMBO_DRIVE, OnSelchangeComboDrive)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   SiteWizDrive::DoDataExchange
   _________________________________________________________________________ */

void
SiteWizDrive::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_COMBO_DRIVE, driveComboBox);
  DDX_CBIndex (pDX, IDC_COMBO_DRIVE, drive);
}

/* _________________________________________________________________________

   SiteWizDrive::OnSetActive
   _________________________________________________________________________ */

BOOL
SiteWizDrive::OnSetActive ()
{
  BOOL ret = CPropertyPage::OnSetActive();
  try
    {
      ShowDrives ();
      EnableButtons ();
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

   SiteWizDrive::Showdrives 
   _________________________________________________________________________ */

void
SiteWizDrive::ShowDrives ()
{
#if 0
  // this requires that we are called by an MFC application
  CWaitCursor curWait;
#endif

  driveComboBox.ResetContent ();
  drives.clear ();
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

  driveComboBox.SetCurSel (0);
}

/* _________________________________________________________________________

   SiteWizDrive::OnSelchangeComboDrive
   _________________________________________________________________________ */

void
SiteWizDrive::OnSelchangeComboDrive ()
{
  try
    {
      EnableButtons ();
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

   SiteWizDrive::EnableButtons
   _________________________________________________________________________ */

void
SiteWizDrive::EnableButtons ()
{
  DWORD flags = PSWIZB_BACK;
  if (driveComboBox.GetCurSel() >= 0 && ! noDriveFound)
    {
      flags |= PSWIZB_FINISH;
    }
  CPropertySheet * pSheet = reinterpret_cast<CPropertySheet*>(GetParent());
  ASSERT_KINDOF (CPropertySheet, pSheet);
  pSheet->SetWizardButtons (flags);
}

/* _________________________________________________________________________

   SiteWizDrive::OnWizardFinish
   _________________________________________________________________________ */

BOOL
SiteWizDrive::OnWizardFinish ()
{
  try
    {
      UpdateData ();
      PathName path (drives[drive].c_str(), "\\", 0, 0);
      pManager->SetDefaultPackageRepository (RepositoryType::MiKTeXDirect,
					     path.Get());
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

   SiteWizDrive::OnWizardBack
   _________________________________________________________________________ */

LRESULT
SiteWizDrive::OnWizardBack ()
{
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_SITEWIZ_TYPE)));
}
