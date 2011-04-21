/* comPackageManager.cpp:

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

#include "COM/comPackageManager.h"
#include "COM/comPackageInstaller.h"
#include "COM/comPackageIterator.h"
#include "COM/mpm.h"

using namespace MiKTeX::Core;
using namespace MiKTeXPackageManagerLib;
using namespace std;

/* _________________________________________________________________________

   comPackageManager::comPackageManager
   _________________________________________________________________________ */

comPackageManager::comPackageManager ()
  : pSession (0)
{
}

/* _________________________________________________________________________

   comPackageManager::~comPackageManager
   _________________________________________________________________________ */

comPackageManager::~comPackageManager ()
{
  try
    {
      if (pSession != 0)
	{
	  Session::Release (pSession);
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   comPackageManager::FinalRelease
   _________________________________________________________________________ */

void
comPackageManager::FinalRelease ()
{
  try
    {
      if (pSession != 0)
	{
	  Session::Release (pSession);
	}
    }
  catch (const exception &)
    {
    }
  pSession = 0;
}

/* _________________________________________________________________________

   comPackageManager::InterfaceSupportsErrorInfo
   _________________________________________________________________________ */

STDMETHODIMP
comPackageManager::InterfaceSupportsErrorInfo (/*[in]*/ REFIID riid)
{
  static const IID* arr[] = 
    {
      &__uuidof(IPackageManager),
      &__uuidof(IPackageManager2),
      &__uuidof(IPackageManager3)
    };
  
  for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++ i)
    {
      if (InlineIsEqualGUID(*arr[i], riid))
	{
	  return (S_OK);
	}
    }
  return (S_FALSE);
}

/* _________________________________________________________________________

   comPackageManager::CreateInstaller
   _________________________________________________________________________ */

STDMETHODIMP
comPackageManager::CreateInstaller (/*[out,retval]*/
				    IPackageInstaller ** ppInstaller)
{
  PackageManagerImpl::localServer = true;
  try
    {
      // create the IPackageInstaller object
      CComObject<comPackageInstaller> * pInstaller = 0;
      HRESULT hr
	= CComObject<comPackageInstaller>::CreateInstance(&pInstaller);
      if (FAILED(hr))
	{
	  *ppInstaller = 0;
	  return (hr);
	}
      
      // increment the reference count of the new object; decrement it
      // at the end of the block
      CComPtr<IUnknown> pUnk (pInstaller->GetUnknown());

      CreateSession ();

      pInstaller->Initialize ();

      // return the IPackageInstaller interface
      return (pUnk->QueryInterface(ppInstaller));
    }
  catch (const exception &)
    {
      *ppInstaller = 0;
      return (E_FAIL);
    }
}

/* _________________________________________________________________________

   comPackageManager::GetPackageInfo
   _________________________________________________________________________ */

STDMETHODIMP
comPackageManager::GetPackageInfo (/*[in]*/ BSTR		deploymentName,
				   /*[out,retval]*/ PackageInfo * pPackageInfo)
{
  PackageManagerImpl::localServer = true;
  try
    {
      CreateSession ();

      if (pManager.Get() == 0)
	{
	  pManager.Create ();
	}
      
      MiKTeX::Packages::PackageInfo packageInfo =
	pManager->GetPackageInfo(static_cast<const char *>
				 (CW2A(deploymentName)));

      CopyPackageInfo (*pPackageInfo, packageInfo);

      return (S_OK);
    }
  catch (const _com_error & e)
    {
      return (e.Error());
    }
  catch (const exception &)
    {
      return (E_FAIL);
    }
}

/* _________________________________________________________________________

   comPackageManager::CreatePackageIterator
   _________________________________________________________________________ */

STDMETHODIMP
comPackageManager::CreatePackageIterator
(/*[out,retval]*/ IPackageIterator ** ppIter)
{
  PackageManagerImpl::localServer = true;
  try
    {
      // create the IPackageIterator object
      CComObject<comPackageIterator> * pIter = 0;
      HRESULT hr
	= CComObject<comPackageIterator>::CreateInstance(&pIter);
      if (FAILED(hr))
	{
	  *ppIter = 0;
	  return (hr);
	}
      
      // increment the reference count of the new object; decrement it
      // at the end of the block
      CComPtr<IUnknown> pUnk (pIter->GetUnknown());

      CreateSession ();

      pIter->Initialize ();

      // return the IPackageIterator interface
      return (pUnk->QueryInterface(ppIter));
    }
  catch (const exception &)
    {
      *ppIter = 0;
      return (E_FAIL);
    }
}

/* _________________________________________________________________________

   comPackageManager::GetPackageInfo2
   _________________________________________________________________________ */

STDMETHODIMP
comPackageManager::GetPackageInfo2 (/*[in]*/ BSTR		deploymentName,
				   /*[out,retval]*/ PackageInfo2 * pPackageInfo)
{
  PackageManagerImpl::localServer = true;
  try
    {
      CreateSession ();

      if (pManager.Get() == 0)
	{
	  pManager.Create ();
	}
      
      MiKTeX::Packages::PackageInfo packageInfo =
	pManager->GetPackageInfo(static_cast<const char *>
				 (CW2A(deploymentName)));

      CopyPackageInfo (*pPackageInfo, packageInfo);

      return (S_OK);
    }
  catch (const _com_error & e)
    {
      return (e.Error());
    }
  catch (const exception &)
    {
      return (E_FAIL);
    }
}

/* _________________________________________________________________________

   comPackageManager::CreateSession
   _________________________________________________________________________ */

void
comPackageManager::CreateSession ()
{
  if (pSession == 0)
  {
    pSession = Session::TryGet();
    if (pSession == 0)
    {
      // we are running standalone; create a new admin session
      Session::InitInfo initInfo (MPMSVC);
      initInfo.SetFlags (Session::InitFlags::AdminMode);
      pSession = Session::Get(initInfo);
    }
  }
}

/* _________________________________________________________________________

   GetAccessPermissionsForLUAServer

   See MSDN "The COM Elevation Moniker"
   _________________________________________________________________________ */

static
ULONG
GetAccessPermissionsForLUAServer (/*[out]*/ SECURITY_DESCRIPTOR **	ppSD)
{
  DllProc4<BOOL, LPCWSTR, DWORD, PSECURITY_DESCRIPTOR, PULONG>
    pConvert ("Advapi32.dll",
	      "ConvertStringSecurityDescriptorToSecurityDescriptorW");
  LPWSTR lpszSDDL = L"O:BAG:BAD:(A;;0x3;;;IU)(A;;0x3;;;SY)";
  SECURITY_DESCRIPTOR * pSD = 0;
  ULONG size = 0;
  if (! pConvert(lpszSDDL,
		 SDDL_REVISION_1,
		 reinterpret_cast<PSECURITY_DESCRIPTOR *>(&pSD),
		 &size))
    {
      FATAL_WINDOWS_ERROR
	("ConvertStringSecurityDescriptorToSecurityDescriptorW",
	 0);
    }
  *ppSD = pSD;
  return (size);
}

/* _________________________________________________________________________

   comPackageManager::UpdateRegistry
   _________________________________________________________________________ */

HRESULT
WINAPI
comPackageManager::UpdateRegistry (/*[in]*/ BOOL doRegister)
{
  HRESULT hr;
  try
    {
      vector<_ATL_REGMAP_ENTRY> regMapEntries;
      _ATL_REGMAP_ENTRY rme;
      string str;
      if (IsWindowsNT())
	{
	  SECURITY_DESCRIPTOR * pSd;
	  ULONG sizeSd = GetAccessPermissionsForLUAServer(&pSd);
	  AutoLocalMem toBeFreed (pSd);
	  str = Utils::Hexify(pSd, sizeSd, true);
	}
      else
	{
	  str = "00";
	}
      rme.szKey = L"ACCESS_SD";
      CA2W wstr (str.c_str());
      rme.szData = wstr;
      regMapEntries.push_back (rme);
      rme.szKey = 0;
      rme.szData = 0;
      regMapEntries.push_back (rme);
#if defined(_ATL_DLL)
      hr =
	_AtlModule.UpdateRegistryFromResourceD(IDR_PACKAGEMANAGER,
					       doRegister,
					       &regMapEntries[0]);
#else
      hr =
	_AtlModule.UpdateRegistryFromResourceS(IDR_PACKAGEMANAGER,
					       doRegister,
					       &regMapEntries[0]);
#endif
      if (FAILED(hr))
	{
	  //
	}
    }
  catch (const exception &)
    {
      hr = E_FAIL;
    }
  return (hr);
}
