/* session.cpp: MiKTeX session

   Copyright (C) 2006-2007 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"
#include "internal.h"
#include "Resource.h"
#include "COM/session.h"

MiKTeXSessionModule _AtlModule;

using namespace MiKTeX::Core;

/* _________________________________________________________________________

   DllMain
   _________________________________________________________________________ */

#if defined(_MANAGED)
#  pragma managed(push, off)
#endif

#if ! defined(MIKTEX_PREVENT_DYNAMIC_LOADS)
#  define MIKTEX_PREVENT_DYNAMIC_LOADS 0
#endif

HINSTANCE SessionImpl::hinstDLL = 0;
TriState SessionImpl::dynamicLoad = TriState::Undetermined;

extern "C"
BOOL
WINAPI
DllMain (/*[in]*/ HINSTANCE	hInstance,
	 /*[in]*/ DWORD		reason,
	 /*[in]*/ LPVOID	lpReserved)
{
  BOOL retCode = TRUE;

  switch (reason)
    {
      // initialize primary thread
    case DLL_PROCESS_ATTACH:
      SessionImpl::dynamicLoad =
	(lpReserved == 0 ? TriState::True : TriState::False);
#if MIKTEX_PREVENT_DYNAMIC_LOADS
      if (SessionImpl::dynamicLoad == TriState::True)
	{
	  retCode = FALSE;
	}
#endif
      SessionImpl::hinstDLL = hInstance;
      break;

      // finalize primary thread
    case DLL_PROCESS_DETACH:
      SessionImpl::dynamicLoad = TriState::Undetermined;
      SessionImpl::hinstDLL = 0;
      break;
    }

  if (retCode)
    {
      retCode = _AtlModule.DllMain(reason, lpReserved);
    }

  return (retCode);
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
#if REPORT_EVENTS
  if (! AddEventSource())
    {
      return (E_FAIL);
    }
#endif
  HRESULT hr = _AtlModule.DllRegisterServer();
  return (hr);
}

/* _________________________________________________________________________

   DllUnregisterServer
   _________________________________________________________________________ */

STDAPI
DllUnregisterServer ()
{
#if REPORT_EVENTS
  if (RemoveEventSource())
    {
      return (E_FAIL);
    }
#endif
  HRESULT hr = _AtlModule.DllUnregisterServer();
  return (hr);
}
