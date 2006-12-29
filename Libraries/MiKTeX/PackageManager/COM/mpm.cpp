/* mpm.cpp: MiKTeX Package Manager

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
#include "Resource.h"
#include "mpmidl.h"

/* _________________________________________________________________________

   MiKTeXPackageManagerModule
   _________________________________________________________________________ */

class MiKTeXPackageManagerModule
  : public CAtlDllModuleT< MiKTeXPackageManagerModule >
{
public:
  DECLARE_LIBID(LIBID_MiKTeXPackageManagerLib);
  DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MIKTEXPACKAGEMANAGER,
				    "{43A56D8A-873D-4B5E-A067-64AF545699FB}");
};

MiKTeXPackageManagerModule _AtlModule;

/* _________________________________________________________________________

   DllMain
   _________________________________________________________________________ */

#if defined(_MANAGED)
#  pragma managed(push, off)
#endif

extern "C"
BOOL
WINAPI
DllMain (/*[in]*/ HINSTANCE	hInstance,
	 /*[in]*/ DWORD		dwReason,
	 /*[in]*/ LPVOID	lpReserved)
{
  hInstance;
  return (_AtlModule.DllMain(dwReason, lpReserved));
}

#if defined(_MANAGED)
#  pragma managed(pop)
#endif

/* _________________________________________________________________________

   DllCanUnloadNow
   _________________________________________________________________________ */

STDAPI
DllCanUnloadNow ()
{
  return (_AtlModule.DllCanUnloadNow());
}

/* _________________________________________________________________________

   DllGetClassObject
   _________________________________________________________________________ */

STDAPI
DllGetClassObject (/*[in]*/ REFCLSID	rclsid,
		   /*[in]*/ REFIID	riid,
		   /*[out]*/ LPVOID *	ppv)
{
  return (_AtlModule.DllGetClassObject(rclsid, riid, ppv));
}

/* _________________________________________________________________________

   DllRegisterServer
   _________________________________________________________________________ */

STDAPI
DllRegisterServer ()
{
  HRESULT hr = _AtlModule.DllRegisterServer();
  return (hr);
}

/* _________________________________________________________________________

   DllUnregisterServer
   _________________________________________________________________________ */

STDAPI
DllUnregisterServer ()
{
  HRESULT hr = _AtlModule.DllUnregisterServer();
  return (hr);
}
