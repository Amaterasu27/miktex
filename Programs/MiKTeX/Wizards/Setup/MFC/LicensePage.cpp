/* LicensePage.cpp:

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

#include "SetupWizard.h"
#include "LicensePage.h"

/* _________________________________________________________________________

   LicensePage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(LicensePage, CPropertyPage)
  ON_BN_CLICKED(IDC_ACCEPT_LICENSE, OnAcceptLicense)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   LicensePage::LicensePage
   _________________________________________________________________________ */

LicensePage::LicensePage ()
  : CPropertyPage (IDD,
		   0,
		   IDS_HEADER_LICENSE,
		   IDS_SUBHEADER_LICENSE),
    pSheet (0)
{
}

/* _________________________________________________________________________

   StreamInCallback
   _________________________________________________________________________ */

static void * pLicense;
static long offset;
static long sizeLicense;

static
DWORD
CALLBACK 
StreamInCallback (/*[in]*/ DWORD		cookie,
		  /*[out]*/ unsigned char *	pBuf,
		  /*[in]*/ long			sizeBuf,
		  /*[out]*/ long *		pRead)
{
  UNUSED_ALWAYS (cookie);
  if (sizeBuf < sizeLicense - offset)
    {
      *pRead = sizeBuf;
    }
  else
    {
      *pRead = sizeLicense - offset;
    }
  memcpy (pBuf, static_cast<char *>(pLicense) + offset, *pRead);
  offset += *pRead;
  return (0);
}

/* _________________________________________________________________________

   LicensePage::OnInitDialog
   _________________________________________________________________________ */

BOOL
LicensePage::OnInitDialog ()
{
  pSheet = reinterpret_cast<SetupWizard *>(GetParent());
  BOOL ret = CPropertyPage::OnInitDialog();
  try
    {
      CHARFORMAT cf; 
      cf.cbSize = sizeof(cf); 
      cf.dwMask = CFM_FACE | CFM_SIZE; 
      cf.dwEffects = 0; 
      cf.bPitchAndFamily = FIXED_PITCH | FF_MODERN; 
      Utils::CopyString (cf.szFaceName,
			 LF_FACESIZE,
			 T_("Courier New"));
      cf.yHeight = 160;
      licenseControl.SetDefaultCharFormat (cf); 
      HRSRC hrsrc = FindResource(0,
				 MAKEINTRESOURCE(IDR_LICENSE),
				 T_("LICENSE"));
      if (hrsrc == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("FindResource"), 0);
	}
      HGLOBAL hglobal = LoadResource(0, hrsrc);
      if (hglobal == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("LoadResource"), 0);
	}
      pLicense = LockResource(hglobal);
      if (pLicense == 0)
	{
	  UNEXPECTED_CONDITION ("LicensePage::OnInitDialog");
	}
      sizeLicense = SizeofResource(0, hrsrc);
      if (sizeLicense == 0)
	{
	  FATAL_WINDOWS_ERROR (T_("SizeofResource"), 0);
	}
      offset = 0;
      EDITSTREAM editStream;
      editStream.dwCookie = 0;
      editStream.pfnCallback = StreamInCallback;
      licenseControl.StreamIn (SF_TEXT, editStream);
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
    }
  return (ret);
}

/* _________________________________________________________________________

   LicensePage::OnSetActive
   _________________________________________________________________________ */

BOOL
LicensePage::OnSetActive ()
{
  BOOL ret = CPropertyPage::OnSetActive();
  if (ret)
    {
      pSheet->SetWizardButtons
	(PSWIZB_BACK
	 | (acceptLicenseButton.GetCheck() == BST_CHECKED
	    ? PSWIZB_NEXT
	    : 0));
    }
  return (ret);
}

/* _________________________________________________________________________

   LicensePage::DoDataExchange
   _________________________________________________________________________ */

void
LicensePage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_LICENSE, licenseControl);
  DDX_Control (pDX, IDC_ACCEPT_LICENSE, acceptLicenseButton);
}

/* _________________________________________________________________________

   LicensePage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
LicensePage::OnWizardNext ()
{
  UINT next =
    (theApp.isMiKTeXDirect
     ? IDD_MD_TASK
     : (theApp.prefabricated
	? (theApp.packageLevel == PackageLevel::Complete
	   ? IDD_PACKAGE_SET_INSTALL
	   : IDD_SHARED)
	: IDD_TASK));
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(next)));
}

/* _________________________________________________________________________

   LicensePage::OnWizardBack
   _________________________________________________________________________ */

LRESULT
LicensePage::OnWizardBack ()
{
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_WELCOME)));
}

/* _________________________________________________________________________

   LicensePage::OnKillActive
   _________________________________________________________________________ */

BOOL
LicensePage::OnKillActive ()
{
  BOOL ret = CPropertyPage::OnKillActive();
  return (ret);
}

/* _________________________________________________________________________

   LicensePage::OnAcceptLicense
   _________________________________________________________________________ */

void
LicensePage::OnAcceptLicense ()
{
  pSheet->SetWizardButtons
    (PSWIZB_BACK
     | (acceptLicenseButton.GetCheck() == BST_CHECKED
	? PSWIZB_NEXT
	: 0));
}
