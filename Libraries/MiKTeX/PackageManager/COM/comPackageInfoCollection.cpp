/* comPackageInfoCollection.cpp:

   Copyright (C) 2001-2008 Christian Schenk

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

#include "COM/comPackageInfoCollection.h"

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace MiKTeXPackageManagerLib;
using namespace std;

/* _________________________________________________________________________

   comPackageInfoCollection::comPackageInfoCollection
   _________________________________________________________________________ */

comPackageInfoCollection::comPackageInfoCollection ()
{
}

/* _________________________________________________________________________

   comPackageInfoCollection::~comPackageInfoCollection
   _________________________________________________________________________ */


comPackageInfoCollection::~comPackageInfoCollection ()
{
}

/* _________________________________________________________________________

   comPackageInfoCollection::FinalRelease
   _________________________________________________________________________ */

void
comPackageInfoCollection::FinalRelease ()
{
}

/* _________________________________________________________________________

   comPackageInfoCollection::Initialize
   _________________________________________________________________________ */

void
comPackageInfoCollection::Initialize ()
{
  if (pManager.Get() == 0)
    {
      pManager.Create ();
    }
  auto_ptr<PackageIterator> pIter (pManager->CreateIterator());
  MiKTeX::Packages::PackageInfo packageInfo;
  while (pIter->GetNext(packageInfo))
    {
      if (! packageInfo.IsPureContainer())
	{
	  m_coll.push_back (packageInfo);
	}
    }
  pIter->Dispose ();
}

/* _________________________________________________________________________

   comPackageInfoCollection::InterfaceSupportsErrorInfo
   _________________________________________________________________________ */

STDMETHODIMP
comPackageInfoCollection::InterfaceSupportsErrorInfo (/*[in]*/ REFIID riid)
{
  static const IID * arr[] = 
    {
      &__uuidof(IPackageInfoCollection)
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




/* _________________________________________________________________________

   comEnumPackageInfo::comEnumPackageInfo
   _________________________________________________________________________ */

comEnumPackageInfo::comEnumPackageInfo ()
{
}

/* _________________________________________________________________________

   comEnumPackageInfo::~comEnumPackageInfo
   _________________________________________________________________________ */


comEnumPackageInfo::~comEnumPackageInfo ()
{
}

/* _________________________________________________________________________

   comEnumPackageInfo::FinalRelease
   _________________________________________________________________________ */

void
comEnumPackageInfo::FinalRelease ()
{
}

/* _________________________________________________________________________

   comEnumPackageInfo::Initialize
   _________________________________________________________________________ */

void
comEnumPackageInfo::Initialize ()
{
  if (pManager.Get() == 0)
    {
      pManager.Create ();
    }
  pIter =
    auto_ptr<MiKTeX::Packages::PackageIterator>(pManager->CreateIterator());
}

/* _________________________________________________________________________

   comEnumPackageInfo::Next
   _________________________________________________________________________ */

STDMETHODIMP
comEnumPackageInfo::Next
(/*[in]*/ ULONG						count, 
 /*[out]*/ MiKTeXPackageManagerLib::PackageInfo *	pPackageInfo,
 /*[out]*/ ULONG *					pFetched)
{
  *pFetched = 0;
  MiKTeX::Packages::PackageInfo packageInfo;
  while (pIter->GetNext(packageInfo) && *pFetched < count)
    {
      if (packageInfo.IsPureContainer())
	{
	  continue;
	}
      CopyPackageInfo (pPackageInfo[*pFetched], packageInfo);
      *pFetched += 1;
    }
  return (S_OK);
}

/* _________________________________________________________________________

   comEnumPackageInfo::Skip
   _________________________________________________________________________ */

STDMETHODIMP
comEnumPackageInfo::Skip (/*[in] */ ULONG count)
{
  ULONG skipped = 0;
  MiKTeX::Packages::PackageInfo packageInfo;
  while (pIter->GetNext(packageInfo) && skipped < count)
    {
      skipped += 1;
    }
  return (S_OK);
}

/* _________________________________________________________________________

   comEnumPackageInfo::Reset
   _________________________________________________________________________ */

STDMETHODIMP
comEnumPackageInfo::Reset ()
{
  pIter->Dispose ();
  pIter =
    auto_ptr<MiKTeX::Packages::PackageIterator>(pManager->CreateIterator());
  return (S_OK);
}

/* _________________________________________________________________________

   comEnumPackageInfo::Clone
   _________________________________________________________________________ */

STDMETHODIMP
comEnumPackageInfo::Clone (/*[out]*/ IEnumPackageInfo ** pOther)
{
  return (E_NOTIMPL);
}

/* _________________________________________________________________________

   comEnumPackageInfo::InterfaceSupportsErrorInfo
   _________________________________________________________________________ */

STDMETHODIMP
comEnumPackageInfo::InterfaceSupportsErrorInfo (/*[in]*/ REFIID riid)
{
  static const IID * arr[] = 
    {
      &__uuidof(IEnumPackageInfo)
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
  while (pIter->GetNext(packageInfo) && *pDone == VARIANT_FALSE)
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
      &__uuidof(IEnumPackageInfo)
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
