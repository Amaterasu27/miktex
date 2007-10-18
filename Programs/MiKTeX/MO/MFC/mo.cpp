/* mo.cpp: MiKTeX Options

   Copyright (C) 2000-2007 Christian Schenk

   This file is part of MiKTeX Options.

   MiKTeX Options is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.
   
   MiKTeX Options is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Options; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"
#include "mo.h"

#include "PropSheet.h"

MiKTeXOptionsApplication theApp;

/* _________________________________________________________________________

   MiKTeXOptionsApplication Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(MiKTeXOptionsApplication, CWinApp)
  ON_COMMAND (ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP ();

/* _________________________________________________________________________

   MiKTeXOptionsApplication::MiKTeXOptionsApplication
   _________________________________________________________________________ */

MiKTeXOptionsApplication::MiKTeXOptionsApplication ()
{
  // EnableHtmlHelp ();
}

/* _________________________________________________________________________

   MiKTeXOptionsApplication::InitInstance
   _________________________________________________________________________ */

BOOL
MiKTeXOptionsApplication::InitInstance ()
{
  INITCOMMONCONTROLSEX initCtrls;

  initCtrls.dwSize = sizeof(initCtrls);
  initCtrls.dwICC = ICC_WIN95_CLASSES;

  if (! InitCommonControlsEx(&initCtrls))
    {
      AfxMessageBox (T_("The application could not be initialized (1)."),
		     MB_ICONSTOP | MB_OK);
      return (FALSE);
    }

  if (! CWinApp::InitInstance())
    {
      AfxMessageBox (T_("The application could not be initialized (2)."),
		     MB_ICONSTOP | MB_OK);
      return (FALSE);
    }

  if (FAILED(CoInitializeEx(0, COINIT_MULTITHREADED)))
    {
      AfxMessageBox (T_("The application could not be initialized (3)."),
		     MB_ICONSTOP | MB_OK);
      return (FALSE);
    }

  try
    {
      SessionWrapper pSession (Session::InitInfo(T_("mo")));
      
      PackageManagerPtr pManager (PackageManager::Create());
      
      {			 // destroy dlg before MiKTeX is uninitialized
	PropSheet dlg (pManager.Get());
	INT_PTR ret = dlg.DoModal();
	if (ret == IDOK
	    && dlg.MustBuildFormats()
	    && (AfxMessageBox("\
To apply the new settings, it is necessary to rebuild the format files.",
			      MB_OKCANCEL)
		== IDOK))
	  {
	    dlg.BuildFormats ();
	  }
      }
      
      pManager.Release ();
      pSession.Reset ();
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (0, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (0, e);
    }

  CoUninitialize ();

  return (FALSE);
}

/* _________________________________________________________________________

   MiKTeXOptionsApplication::WinHelp
   _________________________________________________________________________ */

void
MiKTeXOptionsApplication::WinHelp (/*[in]*/ DWORD	data,
				   /*[in]*/ UINT	cmd)
{
  UNUSED_ALWAYS (data);
  UNUSED_ALWAYS (cmd);
}

/* _________________________________________________________________________

   FindHelpFile
   _________________________________________________________________________ */

PathName
FindHelpFile ()
{
  PathName path;
  if (! SessionWrapper(true)->FindFile("mo409.chm",
				       "%R\\doc\\miktex//",
				       path))
    {
      FATAL_WINDOWS_ERROR (T_("The help file could not be found."), 0);
    }
  return (path);
}

/* _________________________________________________________________________

   OnHelpInfo
   _________________________________________________________________________ */

BOOL
OnHelpInfo (/*[in]*/ HELPINFO *		pHelpInfo,
	    /*[in]*/ const DWORD *	pHelpIds,
	    /*[in]*/ const char *	lpszTopicFile)
{
  try
    {
      PathName helpFile = FindHelpFile();
      bool found = false;
      for (size_t i = 0; ! found && pHelpIds[i] != 0; i += 2)
	{
	  if (pHelpIds[i] == static_cast<DWORD>(pHelpInfo->iCtrlId))
	    {
	      found = true;
	    }
	}
      if (! found)
	{
	  return (FALSE);
	}
      CString htmlHelpUrl = helpFile.Get();
      htmlHelpUrl += T_("::/");
      htmlHelpUrl += lpszTopicFile;
      HtmlHelp (reinterpret_cast<HWND>(pHelpInfo->hItemHandle),
		htmlHelpUrl,
		HH_TP_HELP_WM_HELP,
		reinterpret_cast<DWORD_PTR>(const_cast<DWORD*>(pHelpIds)));
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (0, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (0, e);
    }
  return (TRUE);
}

/* _________________________________________________________________________

   DoWhatsThisMenu
   _________________________________________________________________________ */

void
DoWhatsThisMenu (/*[in]*/ CWnd *		pWnd,
		 /*[in]*/ CPoint		point,
		 /*[in]*/ const DWORD *		pHelpIds,
		 /*[in]*/ const char *	lpszTopicFile)
{
  // HtmlHelp() (unlike WinHelp()) doesn't display the "What's This?"
  // menu
  CMenu menu;
  if (! menu.LoadMenu(IDR_MENU_WHATS_THIS))
    {
      FATAL_WINDOWS_ERROR (T_("CMenu::LoadMenu"), 0);
    }
  CMenu * pPopup = menu.GetSubMenu(0);
  if (pPopup == 0)
    {
      FATAL_WINDOWS_ERROR (T_("CMenu::GetSubMenu"), 0);
    }
  if (pPopup->TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON,
			     point.x,
			     point.y,
			     pWnd)
      != ID_WHATS_THIS)
    {
      CHECK_WINDOWS_ERROR (T_("CMenu::TrackPopupMenu"), 0);
      return;
    }
  string helpFileUrl = FindHelpFile().ToString();
  helpFileUrl += T_("::/");
  helpFileUrl += lpszTopicFile;
  HtmlHelp (pWnd->GetSafeHwnd(),
	    helpFileUrl.c_str(),
	    HH_TP_HELP_CONTEXTMENU,
	    reinterpret_cast<DWORD_PTR>(const_cast<DWORD*>(pHelpIds)));
}

#define PACKVERSION(major,minor) MAKELONG(minor,major)

/* _________________________________________________________________________

   GetDllVersion

   See MSDN: Shell and Common Controls Versions
   _________________________________________________________________________ */

#define PACKVERSION(major,minor) MAKELONG(minor,major)

DWORD
GetDllVersion (/*[in]*/ const char * lpszDllName)
{
  DWORD version = 0;

  HINSTANCE hinstDll = LoadLibrary(lpszDllName);
  
  if (hinstDll != 0)
    {
      DLLGETVERSIONPROC pDllGetVersion
	= reinterpret_cast<DLLGETVERSIONPROC>(GetProcAddress(hinstDll,
							     "DllGetVersion"));
      if (pDllGetVersion != 0)
        {
	  DLLVERSIONINFO dvi;
	  ZeroMemory (&dvi, sizeof(dvi));
	  dvi.cbSize = sizeof(dvi);
	  HRESULT hr = (*pDllGetVersion)(&dvi);
	  if (SUCCEEDED(hr))
            {
	      version = PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
            }
        }
      FreeLibrary(hinstDll);
    }

  return (version);
}

/* _________________________________________________________________________

   HasIE3
   _________________________________________________________________________ */

bool
HasIE3 ()
{
  static bool wasHereBefore = false;
  static bool ret;
  if (! wasHereBefore)
    {
      ret = (GetDllVersion(T_("comctl32.dll")) >= PACKVERSION(4, 70));
      wasHereBefore = true;
    }
  return (ret);
}
