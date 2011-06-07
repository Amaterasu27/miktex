/* PropPagePackage.cpp:

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

#include "PropPagePackage.h"

/* _________________________________________________________________________

   PropPagePackage::PropPagePackage
   _________________________________________________________________________ */

PropPagePackage::PropPagePackage (/*[in]*/ const PackageInfo & packageInfo)
  : packageInfo (packageInfo)
{
  Construct (IDD_PROPPAGE_PACKAGE);
  m_psp.dwFlags &= ~ (PSP_HASHELP);
  date =
    CTime(packageInfo.timePackaged).Format(_T("%Y-%m-%d %H:%M:%S"));  
  size.Format (T_(_T("%u Bytes")),
	       static_cast<unsigned>(packageInfo.GetSize()));
}

/* _________________________________________________________________________

   PropPagePackage::DoDataExchange
   _________________________________________________________________________ */

void
PropPagePackage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Text(pDX, IDC_DATE, date);
  DDX_Text(pDX, IDC_PACKAGE_SIZE, size);
}

/* _________________________________________________________________________

   PropPagePackage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(PropPagePackage, CPropertyPage)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   PropPagePackage::SetUtf8Text
   _________________________________________________________________________ */

void
PropPagePackage::SetUtf8Text (/*[in]*/ UINT	      ctrlId,
			      /*[in]*/ const char *  lpszUtf8Text)
{
  ::SetWindowTextW(GetDlgItem(ctrlId)->GetSafeHwnd(),
    Utils::UTF8ToWideChar(lpszUtf8Text).c_str());
}

/* _________________________________________________________________________

   PropPagePackage::OnInitDialog
   _________________________________________________________________________ */

BOOL
PropPagePackage::OnInitDialog ()
{
  BOOL ret = CPropertyPage::OnInitDialog();
  SetUtf8Text (IDC_NAME, packageInfo.displayName.c_str());
  SetUtf8Text (IDC_DESCRIPTION, packageInfo.description.c_str());
  SetUtf8Text (IDC_TITLE, packageInfo.title.c_str());
  return (ret);
}
