/* PropSheetPackage.cpp:

   Copyright (C) 2000-2007 Christian Schenk

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

#include "PropPageFiles.h"
#include "PropPagePackage.h"

#include <MiKTeX/UI/MFC/PropSheetPackage>

/* _________________________________________________________________________

   PropSheetPackageImpl
   _________________________________________________________________________ */

class PropSheetPackageImpl : public CPropertySheet
{
protected:
  DECLARE_MESSAGE_MAP();
  
public:
  PropSheetPackageImpl (/*[in]*/ const PackageInfo &	packageInfo,
			/*[in]*/ CWnd *			pParentWnd = 0,
			/*[in]*/ UINT			selectPage = 0);

public:
  virtual
  ~PropSheetPackageImpl ();

protected:
  CPropertyPage * pPage1;

protected:
  CPropertyPage * pPage2;
};

/* _________________________________________________________________________

   PropSheetPackageImpl::PropSheetPackageImpl
   _________________________________________________________________________ */

PropSheetPackageImpl::PropSheetPackageImpl
(/*[in]*/ const PackageInfo &	packageInfo,
 /*[in]*/ CWnd *		pParentWnd,
 /*[in]*/ UINT			selectPage)
{
  Construct (packageInfo.deploymentName.c_str(), pParentWnd, selectPage);
  pPage1 = new PropPagePackage (packageInfo);
  pPage2 = new PropPageFiles (packageInfo);
  AddPage (pPage1);
  AddPage (pPage2);
}

/* _________________________________________________________________________

   PropSheetPackageImpl::~PropSheetPackageImpl
   _________________________________________________________________________ */

PropSheetPackageImpl::~PropSheetPackageImpl ()
{
  delete pPage1;
  delete pPage2;
}

/* _________________________________________________________________________

   PropSheetPackageImpl Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(PropSheetPackageImpl, CPropertySheet)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   PropSheetPackage::DoModal
   _________________________________________________________________________ */

INT_PTR
MIKTEXUICALL
PropSheetPackage::DoModal (/*[in]*/ const PackageInfo &	packageInfo,
			   /*[in]*/ CWnd *		pParentWnd)
{
  BEGIN_USE_MY_RESOURCES()
    {
      PropSheetPackageImpl dlg (packageInfo, pParentWnd);
      return (dlg.DoModal());
    }
  END_USE_MY_RESOURCES()
}
