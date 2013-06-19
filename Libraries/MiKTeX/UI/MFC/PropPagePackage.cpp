/* PropPagePackage.cpp:

   Copyright (C) 2000-2013 Christian Schenk

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

   MakeOneline
   _________________________________________________________________________ */

static
void
MakeOneLine (/*[out]*/ CString &	dest,
	     /*[in]*/ const wstring &	source)
{
  for (wstring::const_iterator it = source.begin();
       it != source.end();
       ++ it)
  {
    if (*it == L'\n')
    {
      dest += L' ';
    }
    else
    {
      dest += *it;
    }
  }
}
	     
/* _________________________________________________________________________

   PropPagePackage::PropPagePackage
   _________________________________________________________________________ */

PropPagePackage::PropPagePackage (/*[in]*/ const PackageInfo & packageInfo)
  : packageInfo (packageInfo)
{
  Construct (IDD_PROPPAGE_PACKAGE);
  m_psp.dwFlags &= ~ (PSP_HASHELP);
  date = CTime(packageInfo.timePackaged).Format(_T("%Y-%m-%d %H:%M:%S"));
  MakeOneLine (description, Utils::UTF8ToWideChar(packageInfo.description));
  name = Utils::UTF8ToWideChar(packageInfo.displayName).c_str();
  MakeOneLine (title, Utils::UTF8ToWideChar(packageInfo.title));
  size.Format (T_(_T("%u Bytes")), static_cast<unsigned>(packageInfo.GetSize()));
  maintainer = Utils::UTF8ToWideChar(packageInfo.copyrightOwner).c_str();
  version = Utils::UTF8ToWideChar(packageInfo.version).c_str();
}

/* _________________________________________________________________________

   PropPagePackage::DoDataExchange
   _________________________________________________________________________ */

void
PropPagePackage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Text(pDX, IDC_DATE, date);
  DDX_Text(pDX, IDC_DESCRIPTION, description);
  DDX_Text(pDX, IDC_MAINTAINER, maintainer);
  DDX_Text(pDX, IDC_NAME, name);
  DDX_Text(pDX, IDC_PACKAGE_SIZE, size);
  DDX_Text(pDX, IDC_TITLE, title);
  DDX_Text(pDX, IDC_VERSION, version);
}

/* _________________________________________________________________________

   PropPagePackage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(PropPagePackage, CPropertyPage)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   PropPagePackage::OnInitDialog
   _________________________________________________________________________ */

BOOL
PropPagePackage::OnInitDialog ()
{
  return (CPropertyPage::OnInitDialog());
}
