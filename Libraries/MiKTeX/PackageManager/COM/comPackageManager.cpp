/* PackageManager.cpp:

   Copyright (C) 2001-2006 Christian Schenk

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
  try
    {
      CComObject<comPackageInstaller> * pInstaller = 0;
      HRESULT hr
	= CComObject<comPackageInstaller>::CreateInstance(&pInstaller);
      if (FAILED(hr))
	{
	  return (hr);
	}
      
      // increment the reference count; decrement it at the end of the
      // block
      CComPtr<IUnknown> pUnk (pInstaller->GetUnknown());

      if (! initialized)
	{
	  pSession.CreateSession (Session::InitInfo(T_("mpmsvc")));
	  initialized = true;
	}

      return (pInstaller->QueryInterface(ppInstaller));
    }
  catch (const exception &)
    {
      return (E_FAIL);
    }
}
