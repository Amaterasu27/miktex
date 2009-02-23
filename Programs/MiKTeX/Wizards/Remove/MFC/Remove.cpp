/* Remove.cpp:

   Copyright (C) 2000-2009 Christian Schenk

   This file is part of the Remove MiKTeX! Wizard.

   The Remove MiKTeX! Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The Remove MiKTeX! Wizard is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the Remove MiKTeX! Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "Remove.h"

#include "RemoveWizard.h"

PathName logFileName;

/* _________________________________________________________________________

   RemoveWizardApp Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(RemoveWizardApp, CWinApp)
#if 0
  ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
#endif
END_MESSAGE_MAP();

/* _________________________________________________________________________

   RemoveWizardApp::RemoveWizardApp
   _________________________________________________________________________ */

RemoveWizardApp::RemoveWizardApp ()
{
}

/* _________________________________________________________________________

   RemoveWizardApp::InitInstance
   _________________________________________________________________________ */

RemoveWizardApp theApp;

BOOL
RemoveWizardApp::InitInstance ()
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

  if (FAILED(CoInitialize(0)))
    {
      AfxMessageBox (T_("The application could not be initialized (2)."),
		     MB_ICONSTOP | MB_OK);
      return (FALSE);
    }

  try
    {
      SessionWrapper pSession (Session::InitInfo(_T("remove")));
  
      logFileName =
	SessionWrapper(true)->GetSpecialPath(SpecialPath::InstallRoot);
      logFileName += MIKTEX_PATH_UNINST_LOG;

      {
	RemoveWizard dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal ();
      }

      pSession.Reset ();
    }
  catch (const MiKTeXException & e)
    {
      ReportError (e);
    }
  catch (const exception & e)
    {
      ReportError (e);
    }

  return (FALSE);
}

/* _________________________________________________________________________

   ReportError
   _________________________________________________________________________ */

void
ReportError (/*[in]*/ const MiKTeXException & e)
{
  string str;
  str = T_("The operation could not be completed for the following reason: ");
  str += "\n\n";
  str += e.what();
  if (! e.GetInfo().empty())
    {
      str += "\n\n";
      str += T_("Details: ");
      str += e.GetInfo();
    }
  AfxMessageBox (str.c_str(), MB_OK | MB_ICONSTOP);
}

/* _________________________________________________________________________

   ReportError
   _________________________________________________________________________ */

void
ReportError (/*[in]*/ const exception & e)
{
  string str;
  str = T_("The operation could not be completed for the following reason: ");
  str += "\n\n";
  str += e.what();
  AfxMessageBox (str.c_str(), MB_OK | MB_ICONSTOP);
}

/* _________________________________________________________________________

   RemoveEmptyDirectoryChain
   _________________________________________________________________________ */

void
RemoveEmptyDirectoryChain (/*[in]*/ const PathName &	directory)
{
  auto_ptr<DirectoryLister> pLister (DirectoryLister::Open(directory));
  DirectoryEntry dirEntry;
  bool empty = ! pLister->GetNext(dirEntry);
  pLister->Close ();
  if (! empty)
    {
      return;
    }
  FileAttributes attributes = File::GetAttributes(directory);
  if ((attributes & FileAttributes::ReadOnly) != 0)
    {
      attributes &= ~ FileAttributes(FileAttributes::ReadOnly);
      File::SetAttributes (directory, attributes);
    }
  Directory::Delete (directory);
  PathName parentDir (directory);
  parentDir.CutOffLastComponent ();
  if (parentDir == directory)
    {
      return;
    }
  RemoveEmptyDirectoryChain (parentDir);
}

/* _________________________________________________________________________

   Contains
   _________________________________________________________________________ */

bool
Contains (/*[in]*/ const vector<PathName> &	vec,
	  /*[in]*/ const PathName &		pathName)
{
  for (vector<PathName>::const_iterator it = vec.begin();
       it != vec.end();
       ++ it)
    {
      if (*it == pathName)
	{
	  return (true);
	}
    }
  return (false);
}

/* _________________________________________________________________________

   GetRoots
   _________________________________________________________________________ */

vector<PathName>
GetRoots ()
{
  vector<PathName> vec;
  if (! SessionWrapper(true)->IsMiKTeXDirect())
    {
      PathName installRoot =
	SessionWrapper(true)->GetSpecialPath(SpecialPath::InstallRoot);
      vec.push_back (installRoot);
    }
  PathName userDataRoot =
    SessionWrapper(true)->GetSpecialPath(SpecialPath::UserDataRoot);
  if (! Contains(vec, userDataRoot))
    {
      vec.push_back (userDataRoot);
    }
  PathName userConfigRoot =
    SessionWrapper(true)->GetSpecialPath(SpecialPath::UserConfigRoot);
  if (! Contains(vec, userConfigRoot))
    {
      vec.push_back (userConfigRoot);
    }
  if (SessionWrapper(true)->IsAdminMode())
    {
      PathName commonDataRoot =
	SessionWrapper(true)->GetSpecialPath(SpecialPath::CommonDataRoot);
      if (! Contains(vec, commonDataRoot))
	{
	  vec.push_back (commonDataRoot);
	}
      PathName commonConfigRoot =
	SessionWrapper(true)->GetSpecialPath(SpecialPath::CommonConfigRoot);
      if (! Contains(vec, commonConfigRoot))
	{
	  vec.push_back (commonConfigRoot);
	}
    }
  return (vec);
}
