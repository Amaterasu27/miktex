/* comPackageIterator.cpp:

   Copyright (C) 2001-2011 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"

#include "internal.h"

#include "COM/comPackageIterator.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace MiKTeXPackageManagerLib;
using namespace std;

/* _________________________________________________________________________

   comPackageIterator::comPackageIterator
   _________________________________________________________________________ */

comPackageIterator::comPackageIterator ()
{
}

/* _________________________________________________________________________

   comPackageIterator::~comPackageIterator
   _________________________________________________________________________ */


comPackageIterator::~comPackageIterator ()
{
}

/* _________________________________________________________________________

   comPackageIterator::FinalRelease
   _________________________________________________________________________ */

void
comPackageIterator::FinalRelease ()
{
}

/* _________________________________________________________________________

   comPackageIterator::Initialize
   _________________________________________________________________________ */

void
comPackageIterator::Initialize ()
{
  if (pManager.Get() == 0)
    {
      pManager.Create ();
    }
  pIter =
    auto_ptr<MiKTeX::Packages::PackageIterator>(pManager->CreateIterator());
}

/* _________________________________________________________________________

   comPackageIterator::GetNextPackageInfo
   _________________________________________________________________________ */

STDMETHODIMP
comPackageIterator::GetNextPackageInfo
(/*[out]*/ MiKTeXPackageManagerLib::PackageInfo *	pPackageInfo,
  /*[out,retval]*/ VARIANT_BOOL *			pDone)
{
  *pDone = VARIANT_FALSE;
  MiKTeX::Packages::PackageInfo packageInfo;
  while (*pDone == VARIANT_FALSE && pIter->GetNext(packageInfo))
    {
      if (! packageInfo.IsPureContainer())
	{
	  CopyPackageInfo (*pPackageInfo, packageInfo);
	  *pDone = VARIANT_TRUE;
	}
    }
  return (*pDone == VARIANT_FALSE ? S_FALSE : S_OK);
}

/* _________________________________________________________________________

   comPackageIterator::GetNextPackageInfo2
   _________________________________________________________________________ */

STDMETHODIMP
comPackageIterator::GetNextPackageInfo2
(/*[out]*/ MiKTeXPackageManagerLib::PackageInfo2 *	pPackageInfo,
  /*[out,retval]*/ VARIANT_BOOL *			pDone)
{
  *pDone = VARIANT_FALSE;
  MiKTeX::Packages::PackageInfo packageInfo;
  while (*pDone == VARIANT_FALSE && pIter->GetNext(packageInfo))
    {
      if (! packageInfo.IsPureContainer())
	{
	  CopyPackageInfo (*pPackageInfo, packageInfo);
	  *pDone = VARIANT_TRUE;
	}
    }
  return (*pDone == VARIANT_FALSE ? S_FALSE : S_OK);
}

/* _________________________________________________________________________

   comPackageIterator::InterfaceSupportsErrorInfo
   _________________________________________________________________________ */

STDMETHODIMP
comPackageIterator::InterfaceSupportsErrorInfo (/*[in]*/ REFIID riid)
{
  static const IID * arr[] = 
    {
      &__uuidof(IPackageIterator),
      &__uuidof(IPackageIterator2)
    };
  
  for (int idx = 0; idx < sizeof(arr) / sizeof(arr[0]); ++ idx)
    {
      if (InlineIsEqualGUID(*arr[idx], riid))
	{
	  return (S_OK);
	}
    }
  return (S_FALSE);
}
