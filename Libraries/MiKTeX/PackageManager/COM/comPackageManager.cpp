/* comPackageManager.cpp:

   Copyright (C) 2001-2007 Christian Schenk

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
#include "COM/mpm.h"

using namespace MiKTeX::Core;
using namespace std;

/* _________________________________________________________________________

   comPackageManager::comPackageManager
   _________________________________________________________________________ */

comPackageManager::comPackageManager ()
  : initialized (false)
{
}

/* _________________________________________________________________________

   comPackageManager::~comPackageManager
   _________________________________________________________________________ */

comPackageManager::~comPackageManager ()
{
  try
    {
      if (initialized)
	{
	  pSession.Reset ();
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
      if (initialized)
	{
	  pSession.Reset ();
	}
    }
  catch (const exception &)
    {
    }
  initialized = false;
}

/* _________________________________________________________________________

   comPackageManager::InterfaceSupportsErrorInfo
   _________________________________________________________________________ */

STDMETHODIMP
comPackageManager::InterfaceSupportsErrorInfo (/*[in]*/ REFIID riid)
{
  static const IID* arr[] = 
    {
      &IID_IPackageManager
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

      // create a MiKTeX session
      if (! initialized)
	{
	  pSession.CreateSession (Session::InitInfo(MPMSVC));
	  initialized = true;
	}

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

   GetAccessPermissionsForLUAServer

   See MSDN "The COM Elevation Moniker"
   _________________________________________________________________________ */

static
ULONG
GetAccessPermissionsForLUAServer (/*[out]*/ SECURITY_DESCRIPTOR **	ppSD)
{
  DllProc4<BOOL, LPCWSTR, DWORD, PSECURITY_DESCRIPTOR, PULONG>
    pConvert (T_("Advapi32.dll"),
	      T_("ConvertStringSecurityDescriptorToSecurityDescriptorW"));
  LPWSTR lpszSDDL = L"O:BAG:BAD:(A;;0x3;;;IU)(A;;0x3;;;SY)";
  SECURITY_DESCRIPTOR * pSD = 0;
  ULONG size = 0;
  if (! pConvert(lpszSDDL,
		 SDDL_REVISION_1,
		 reinterpret_cast<PSECURITY_DESCRIPTOR *>(&pSD),
		 &size))
    {
      FATAL_WINDOWS_ERROR
	(T_("ConvertStringSecurityDescriptorToSecurityDescriptorW"),
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
      tstring str;
      if (IsWindowsNT())
	{
	  SECURITY_DESCRIPTOR * pSd;
	  ULONG sizeSd = GetAccessPermissionsForLUAServer(&pSd);
	  AutoLocalMem toBeFreed (pSd);
	  str = Utils::Hexify(pSd, sizeSd, true).c_str();
	}
      else
	{
	  str = T_("00");
	}
      rme.szKey = L"ACCESS_SD";
      rme.szData = CT2W(str.c_str());
      regMapEntries.push_back (rme);
      rme.szKey = 0;
      rme.szData = 0;
      regMapEntries.push_back (rme);
      hr =
	_AtlModule.UpdateRegistryFromResourceD(IDR_PACKAGEMANAGER,
					       doRegister,
					       &regMapEntries[0]);
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
