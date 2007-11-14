/* mikui.cpp:

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

#include <afxdllx.h>

#include "internal.h"

#include "InstallPackageDialog.h"
#include "ProxyAuthenticationDialog.h"

AFX_EXTENSION_MODULE MikuiDLL = { 0, 0 };

/* _________________________________________________________________________
   
   DllMain
   _________________________________________________________________________ */

extern "C"
int
APIENTRY
DllMain (/*[in]*/ HINSTANCE	hInstance,
	 /*[in]*/ DWORD		reason,
	 /*[in]*/ LPVOID	pReserved)
{
  UNREFERENCED_PARAMETER(pReserved);
  
  if (reason == DLL_PROCESS_ATTACH)
    {
      if (! AfxInitExtensionModule(MikuiDLL, hInstance))
	{
	  return (0);
	}
      new CDynLinkLibrary (MikuiDLL);
    }
  else if (reason == DLL_PROCESS_DETACH)
    {
      AfxTermExtensionModule (MikuiDLL);
    }
  return (1);
}

/* _________________________________________________________________________
   
   MiKTeX::UI::MFC::InitializeFramework
   _________________________________________________________________________ */

MIKTEXUIEXPORT
void
MIKTEXCEECALL
MiKTeX::UI::MFC::InitializeFramework ()
{
  if (! AfxWinInit(::GetModuleHandle(0), 0, ::GetCommandLine(), 0))
    {
      Session::FatalMiKTeXError (T_("MiKTeX::UI::MFC::InitializeFramework"),
				 T_("AfxWinInit() failed for some reason."),
				 0,
				 __FILE__,
				 __LINE__);
    }
}

/* _________________________________________________________________________
   
   MiKTeX::UI::MFC::InstallPackageMessageBox
   _________________________________________________________________________ */

MIKTEXUIEXPORT
unsigned int
MIKTEXCEECALL
MiKTeX::UI::MFC::InstallPackageMessageBox
(/*[in]*/ HWND			hwndParent,
 /*[in]*/ PackageManager *	pManager,
 /*[in]*/ const char *	lpszPackageName,
 /*[in]*/ const char *	lpszTrigger)
{
  SessionWrapper pSession (true);
  TriState enableInstaller
    = pSession->GetConfigValue(MIKTEX_REGKEY_PACKAGE_MANAGER,
			       MIKTEX_REGVAL_AUTO_INSTALL,
			       TriState(TriState::Undetermined));
  unsigned int ret;
  if (enableInstaller != TriState::Undetermined)
    {
      ret = DONTASKAGAIN;
      ret |= (enableInstaller == TriState::True ? YES : NO);
    }
  else
    {
      InstallPackageDialog dlg ((hwndParent == 0
				 ? 0
				 : CWnd::FromHandle(hwndParent)),
				pManager,
				lpszPackageName,
				lpszTrigger);
      dlg.alwaysAsk = (enableInstaller == TriState::True ? false : true);
      INT_PTR dlgRet = dlg.DoModal();
      if (dlgRet != IDOK && dlgRet != IDCANCEL)
	{
	  ret = (NO | DONTASKAGAIN);
	}
      else
	{
	  ret = (dlgRet == IDOK ? YES : NO);
	  if (dlgRet == IDOK && ! dlg.alwaysAsk)
	    {
	      pSession->SetUserConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
					    MIKTEX_REGVAL_AUTO_INSTALL,
					    "1");
	    }
	}
    }
  return (ret);
}

/* _________________________________________________________________________
   
   MiKTeX::UI::MFC::ProxyAuthenticationDialog
   _________________________________________________________________________ */

MIKTEXUIEXPORT
bool
MIKTEXCEECALL
MiKTeX::UI::MFC::ProxyAuthenticationDialog (/*[in]*/ HWND hwndParent)
{
  ProxySettings proxySettings;

  bool done = true;

  if (PackageManager::TryGetProxy(proxySettings)
      && proxySettings.useProxy
      && proxySettings.authenticationRequired
      && proxySettings.user.empty())
    {
      ::ProxyAuthenticationDialog dlg ((hwndParent == 0
					? 0
					: CWnd::FromHandle(hwndParent)));
      if (dlg.DoModal() == IDOK)
	{
	  proxySettings.user = dlg.GetName();
	  proxySettings.password = dlg.GetPassword();
	  PackageManager::SetProxy (proxySettings);
	}
      else
	{
	  done = false;
	}
    }

  return (done);
}
