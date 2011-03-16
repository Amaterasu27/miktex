/* SetupWizard.h:						-*- C++ -*-

   Copyright (C) 1999-2011 Christian Schenk

   This file is part of the MiKTeX Setup Wizard.

   The MiKTeX Setup Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX Setup Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX Setup Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#pragma once

#if ENABLE_ADDTEXMF
#include "AddTEXMFPage.h"
#include "EditTEXMFPage.h"
#endif

#if SHOW_FOLDER_PAGE
#  include "ProgramFolderPage.h"
#endif

#include "FileCopyPage.h"
#include "FinishPage.h"
#include "InfoListPage.h"
#include "InstallDirPage.h"
#include "LicensePage.h"
#include "LocalRepositoryPage.h"
#include "MDTaskPage.h"
#include "PackageSetPage.h"
#include "RemoteRepositoryPage.h"
#include "SharedInstallationPage.h"
#include "SettingsPage.h"
#include "TaskPage.h"
#include "WelcomePage.h"

class SetupWizard : public CPropertySheet
{
protected:
  DECLARE_MESSAGE_MAP();

public:
  //  SetupWizard ();

public:
  SetupWizard (/*[in]*/ PackageManager * pManager);

public:
  virtual
  BOOL
  OnInitDialog ();
  
public:
  void
  ReportError (/*[in]*/ const exception & e);

public:
  void
  ReportError (/*[in]*/ const MiKTeXException & e);

public:
  void
  SetErrorFlag ()
  {
    error = true;
  }

public:
  bool
  GetErrorFlag ()
    const
  {
    return (error);
  }

public:
  void
  SetCancelFlag ()
  {
    EnableCancelButton (false);
    cancelled = true;
  }

public:
  bool
  GetCancelFlag ()
    const
  {
    return (cancelled);
  }

public:
  void
  EnableCloseButton ()
  {
    SetWizardButtons (PSWIZB_FINISH);
    EnableCancelButton (false);
    SetFinishText (T_(_T("Close")));
  }

public:
  CString
  SetNextText (/*[in]*/ LPCTSTR lpszText);

private:
  void
  EnableCancelButton (/*[in]*/ bool enable);

private:
  CBitmap watermarkBitmap;

private:
  CBitmap headerBitmap;

private:
  WelcomePage m_Welcome;

private:
  MDTaskPage m_MDTask;

private:
  TaskPage m_Task;

private:
  RemoteRepositoryPage m_RemoteRepository;

private:
  LocalRepositoryPage m_LocalRepository;
 
private:
  PackageSetPage m_PackageSetDownload;

private:
  PackageSetPage m_PackageSetInstall;

private:
  SharedInstallationPage m_Shared;

private:
  InstallDirPage m_InstallDir;

#if SHOW_FOLDER_PAGE
private:
  ProgramFolderPage m_Folder;
#endif

private:
  SettingsPage m_Settings;

private:
  LicensePage m_License;

#if ENABLE_ADDTEXMF
private:
  AddTEXMFPage m_AddTEXMF;
#endif

#if ENABLE_ADDTEXMF
private:
  EditTEXMFPage m_EditTEXMF;
#endif

private:
  InfoListPage m_Info;

private:
  FileCopyPage m_FileCopy;

private:
  FinishPage m_Finish;

private:
  bool error;

private:
  bool cancelled;

private:
  PackageManagerPtr pManager;
};
