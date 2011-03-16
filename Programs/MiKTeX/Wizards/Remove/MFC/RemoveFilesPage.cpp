/* RemoveFilesPage.cpp:

   Copyright (C) 2000-2011 Christian Schenk

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

#include "LogFile.h"
#include "RemoveFilesPage.h"
#include "RemoveWizard.h"

const UINT nIDTimer = 314;

#define REGSTR_PATH_CURRENTCONTROLSET_A "System\\CurrentControlSet"

/* _________________________________________________________________________

   RemoveFilesPage Message Map
   _________________________________________________________________________ */

#define WM_REMOVEFILES (WM_USER + 100)

BEGIN_MESSAGE_MAP(RemoveFilesPage, CPropertyPage)

  ON_WM_TIMER()
  ON_MESSAGE(WM_REMOVEFILES, OnStartRemovingFiles)
  
END_MESSAGE_MAP();

/* _________________________________________________________________________

   RemoveFilesPage::RemoveFilesPage
   _________________________________________________________________________ */

RemoveFilesPage::RemoveFilesPage ()
  : CPropertyPage(IDD,
		  0,
		  IDS_HEADER_REMOVE_FILES,
		  IDS_SUBHEADER_REMOVE_FILES),
    pSheet (0),
    hWorkerThread (0)
{
  m_psp.dwFlags &= ~ PSP_HASHELP;
}

/* _________________________________________________________________________

   RemoveFilesPage::~RemoveFilesPage
   _________________________________________________________________________ */

RemoveFilesPage::~RemoveFilesPage ()
{
  try
    {
      if (hWorkerThread != 0)
	{
	  CloseHandle (hWorkerThread);
	  hWorkerThread = 0;
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   RemoveFilesPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
RemoveFilesPage::OnInitDialog ()
{
  pSheet = reinterpret_cast<RemoveWizard *>(GetParent());
  BOOL ret = CPropertyPage::OnInitDialog();
  return (ret);
}

/* _________________________________________________________________________

   RemoveFilesPage::OnSetActive
   _________________________________________________________________________ */

BOOL
RemoveFilesPage::OnSetActive ()
{
  BOOL ret = CPropertyPage::OnSetActive();

  if (ret)
    {
      try
	{
	  // disable buttons
	  pSheet->SetWizardButtons (0);
	  
	  // starting shot
	  if (! PostMessage(WM_REMOVEFILES))
	    {
	      FATAL_WINDOWS_ERROR ("CWnd::PostMessage", 0);
	    }
	}
      catch (const MiKTeXException & e)
	{
	  pSheet->ReportError (e);
	  ret = FALSE;
	}
      catch (const exception & e)
	{
	  pSheet->ReportError (e);
	  ret = FALSE;
	}
    }

  return (ret);
}

/* _________________________________________________________________________

   RemoveFilesPage::DoDataExchange
   _________________________________________________________________________ */

void
RemoveFilesPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  
  DDX_Control (pDX, IDC_PROGRESS, progressControl);
}

/* _________________________________________________________________________

   RemoveFilesPage::OnStartRemovingFiles
   _________________________________________________________________________ */

LRESULT
RemoveFilesPage::OnStartRemovingFiles (/*[in]*/ WPARAM	wParam,
				       /*[in]*/ LPARAM	lParam)
{
  UNUSED_ALWAYS (wParam);
  UNUSED_ALWAYS (lParam);

  try
    {
      progress = 0;
      total = 0;
      ready = false;
      
      // create the worker thread
      CWinThread * pThread =
	AfxBeginThread(WorkerThread,
		       this,
		       THREAD_PRIORITY_NORMAL,
		       0,
		       CREATE_SUSPENDED);
      MIKTEX_ASSERT (pThread != 0);
      MIKTEX_ASSERT (pThread->m_hThread != 0);
      if (! DuplicateHandle(GetCurrentProcess(),
			    pThread->m_hThread,
			    GetCurrentProcess(),
			    &hWorkerThread,
			    0,
			    FALSE,
			    DUPLICATE_SAME_ACCESS))
	{
	  FATAL_WINDOWS_ERROR (T_("DuplicateHandle"), 0);
	}
      pThread->ResumeThread ();
      
      if (SetTimer(nIDTimer, 100, 0) == 0)
	{
	  FATAL_WINDOWS_ERROR ("CWnd::SetTime", 0);
	}
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
    }

  return (0);
}

/* _________________________________________________________________________

   RemoveFilesPage::WorkerThread
   _________________________________________________________________________ */

UINT
RemoveFilesPage::WorkerThread (/*[in]*/ void * pParam)
{
  RemoveFilesPage * This = reinterpret_cast<RemoveFilesPage *>(pParam);
  This->RemoveMiKTeX ();
  This->SetReadyFlag ();
  return (0);
};

/* _________________________________________________________________________

   RemoveFilesPage::RemoveMiKTeX
   _________________________________________________________________________ */

void
RemoveFilesPage::RemoveMiKTeX ()
{
  try
    {
      UnregisterShellFileTypes ();
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
    }

  try
    {
      if (IsWindowsNT())
	{
	  UnregisterPathNT ();
	}
      else
	{
#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
	  UnregisterPath95 ();
#else
	  UNSUPPORTED_PLATFORM ();
#endif
	}
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
    }

  try
    {
      UnregisterComponents ();
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
    }

  if (! SessionWrapper(true)->IsMiKTeXDirect())
    {
      try
	{
	  LogFile logfile (this);
	  logfile.Process ();
	}
      catch (const MiKTeXException & e)
	{
	  pSheet->ReportError (e);
	}
      catch (const exception & e)
	{
	  pSheet->ReportError (e);
	}
    }

  try
    {
      PathName parent;
      vector<PathName> roots = GetRoots();
      SessionWrapper pSession (true);
      pSession->UnloadFilenameDatabase ();
      if (pSheet->GetThoroughlyFlag())
	{
	  for (vector<PathName>::const_iterator it = roots.begin();
	       it != roots.end();
	       ++ it)
	    {
	      if (Directory::Exists(*it))
		{
		  Directory::Delete (*it, true);
		}
	    }
	}
      else
	{
	  PathName dir;
	  if (! SessionWrapper(true)->IsMiKTeXDirect())
	    {
	      dir = pSession->GetSpecialPath(SpecialPath::InstallRoot);
	      dir += MIKTEX_PATH_MIKTEX_DIR;
	      if (Directory::Exists(dir))
		{
		  Directory::Delete (dir, true);
		}
	    }
	  dir = pSession->GetSpecialPath(SpecialPath::UserDataRoot);
	  dir += MIKTEX_PATH_MIKTEX_DIR;
	  if (Directory::Exists(dir))
	    {
	      Directory::Delete (dir, true);
	    }
	  dir = pSession->GetSpecialPath(SpecialPath::UserConfigRoot);
	  dir += MIKTEX_PATH_MIKTEX_DIR;
	  if (Directory::Exists(dir))
	    {
	      Directory::Delete (dir, true);
	    }
	  if (pSession->IsAdminMode())
	    {
	      dir = pSession->GetSpecialPath(SpecialPath::CommonDataRoot);
	      dir += MIKTEX_PATH_MIKTEX_DIR;
	      if (Directory::Exists(dir))
		{
		  Directory::Delete (dir, true);
		}
	      dir = pSession->GetSpecialPath(SpecialPath::CommonConfigRoot);
	      dir += MIKTEX_PATH_MIKTEX_DIR;
	      if (Directory::Exists(dir))
		{
		  Directory::Delete (dir, true);
		}
	    }
	}
      if (! SessionWrapper(true)->IsMiKTeXDirect())
	{
	  parent = pSession->GetSpecialPath(SpecialPath::InstallRoot);
	  parent.CutOffLastComponent ();
	  if (Directory::Exists(parent))
	    {
	      RemoveEmptyDirectoryChain (parent);
	    }
	}
      parent = pSession->GetSpecialPath(SpecialPath::UserDataRoot);
      parent.CutOffLastComponent ();
      if (Directory::Exists(parent))
	{
	  RemoveEmptyDirectoryChain (parent);
	}
      parent = pSession->GetSpecialPath(SpecialPath::UserConfigRoot);
      parent.CutOffLastComponent ();
      if (Directory::Exists(parent))
	{
	  RemoveEmptyDirectoryChain (parent);
	}
      if (pSession->IsAdminMode())
	{
	  parent = pSession->GetSpecialPath(SpecialPath::CommonDataRoot);
	  parent.CutOffLastComponent ();
	  if (Directory::Exists(parent))
	    {
	      RemoveEmptyDirectoryChain (parent);
	    }
	  parent = pSession->GetSpecialPath(SpecialPath::CommonConfigRoot);
	  parent.CutOffLastComponent ();
	  if (Directory::Exists(parent))
	    {
	      RemoveEmptyDirectoryChain (parent);
	    }
	}
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
    }

  try
    {
      RemoveRegistryKeys ();
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
    }
};

/* _________________________________________________________________________

   RemoveFilesPage::OnQueryCancel
   _________________________________________________________________________ */

BOOL
RemoveFilesPage::OnQueryCancel ()
{
  if (hWorkerThread == 0)
    {
      return (CPropertyPage::OnQueryCancel());
    }
  try
    {
      if (AfxMessageBox(IDS_CANCEL, MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK)
	{
	  pSheet->SetCancelFlag ();
	}
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
    }
  return (FALSE);
}

/* _________________________________________________________________________

   RemoveFilesPage::OnTimer
   _________________________________________________________________________ */

void
RemoveFilesPage::OnTimer (/*[in]*/ UINT_PTR nIDEvent) 
{
  try
    {
      CSingleLock singleLock (&criticalSectionMonitor, TRUE);
      CWnd * pWnd = GetDlgItem(IDC_CURRENT_FILE);
      if (pWnd == 0)
	{
	  UNEXPECTED_CONDITION ("RemoveFilesPage::OnTimer");
	}
      pWnd->SetWindowText (currentFileName);
      if (total != 0)
	{
	  progressControl.SetPos
	    (static_cast<int>((static_cast<double>(progress) / total)
			      * 100));
	}
      if (ready || pSheet->GetCancelFlag())
	{
	  // notify user
	  CString prompt;
	  if (! prompt.LoadString(IDS_READY))
	    {
	      UNEXPECTED_CONDITION ("RemoveFilesPage::OnTimer");
	    }
	  CWnd * pWnd = GetDlgItem(IDC_PROMPT);
	  if (pWnd == 0)
	    {
	      UNEXPECTED_CONDITION ("RemoveFilesPage::OnTimer");
	    }
	  pWnd->SetWindowText (prompt);
      
	  // disable progress bars
	  pWnd = GetDlgItem(IDC_CURRENT_FILE);
	  if (pWnd == 0)
	    {
	      UNEXPECTED_CONDITION ("RemoveFilesPage::OnTimer");
	    }
	  pWnd ->SetWindowText (_T(""));
	  pWnd->EnableWindow (FALSE);
	  progressControl.SetPos (0);
	  progressControl.EnableWindow (FALSE);
      
	  // enable Next button
	  pSheet->SetWizardButtons (PSWIZB_NEXT);

	  KillTimer (nIDTimer);
	}
      else
	{
	  CPropertyPage::OnTimer (nIDEvent);
	}
    }
  catch (const MiKTeXException & e)
    {
      pSheet->ReportError (e);
    }
  catch (const exception & e)
    {
      pSheet->ReportError (e);
    }
}

/* _________________________________________________________________________

   RemoveFilesPage::Progress
   _________________________________________________________________________ */

void
RemoveFilesPage::Progress (/*[in]*/ const PathName &	fileName,
			   /*[in]*/ unsigned long	progress,
			   /*[in]*/ unsigned long	total)
{
  CSingleLock singleLock (&criticalSectionMonitor, TRUE);
  currentFileName = fileName.Get();
  this->progress = progress;
  this->total = total;
}

/* _________________________________________________________________________

   RemoveFilesPage::SetReadyFlag
   _________________________________________________________________________ */

void
RemoveFilesPage::SetReadyFlag ()
{
  CSingleLock singleLock (&criticalSectionMonitor, TRUE);
  ready = true;
}

/* _________________________________________________________________________

   RemoveFilesPage::GetCancelFlag
   _________________________________________________________________________ */

bool
RemoveFilesPage::GetCancelFlag ()
{
  return (pSheet->GetCancelFlag());
}

/* _________________________________________________________________________

   RemoveFilesPage::RemoveBinDirFromPath
   _________________________________________________________________________ */

bool
RemoveFilesPage::RemoveBinDirFromPath (/*[in,out]*/ string &	path)
{
  bool removed = false;
  string newPath;
  PathName userBinDir =
    SessionWrapper(true)->GetSpecialPath(SpecialPath::UserInstallRoot);
  userBinDir += MIKTEX_PATH_BIN_DIR;
  userBinDir.AppendDirectoryDelimiter ();
  PathName commonBinDir =
    SessionWrapper(true)->GetSpecialPath(SpecialPath::CommonInstallRoot);
  commonBinDir += MIKTEX_PATH_BIN_DIR;
  commonBinDir.AppendDirectoryDelimiter ();
  for (CSVList entry (path.c_str(), PathName::PathNameDelimiter);
    entry.GetCurrent() != 0;
    ++ entry)
  {
    PathName dir (entry.GetCurrent());
    dir.AppendDirectoryDelimiter ();
    if (userBinDir == dir || commonBinDir == dir)
    {
      removed = true;
    }
    else
    {
      if (! newPath.empty())
      {
	newPath += PathName::PathNameDelimiter;
      }
      newPath += entry.GetCurrent();
    }
  }
  if (removed)
  {
    path = newPath;
  }
  return (removed);
}

/* _________________________________________________________________________

   RemoveFilesPage::UnregisterShellFileTypes
   _________________________________________________________________________ */

void
RemoveFilesPage::UnregisterShellFileTypes ()
{
  PathName initexmfExe;
  if (! SessionWrapper(true)->FindFile(MIKTEX_INITEXMF_EXE, FileType::EXE, initexmfExe))
  {
    FATAL_MIKTEX_ERROR ("RemoveFilesPage::UnregisterShellFileTypes",
      T_("The IniTeXMF executable could not be found."), 0);
  }
 if (SessionWrapper(true)->RunningAsAdministrator()
     || SessionWrapper(true)->RunningAsPowerUser())
 {
   Process::Run (initexmfExe.Get(), "--admin --unregister-shell-file-types");
 }
 Process::Run (initexmfExe.Get(), "--unregister-shell-file-types");
}

/* _________________________________________________________________________

   RemoveFilesPage::UnregisterComponents
   _________________________________________________________________________ */

void
RemoveFilesPage::UnregisterComponents ()
{
 if (! IsWindowsNT()
     || SessionWrapper(true)->RunningAsAdministrator()
     || SessionWrapper(true)->RunningAsPowerUser())
   {
     PackageManagerPtr pManager (PackageManager::Create());
     auto_ptr<PackageInstaller> pInstaller (pManager->CreateInstaller());
     pInstaller->RegisterComponents (false);
     pInstaller->Dispose ();
   }
}

/* _________________________________________________________________________

   RemoveFilesPage::UnregisterPathNT
   _________________________________________________________________________ */

void
RemoveFilesPage::UnregisterPathNT ()
{
  if (SessionWrapper(true)->RunningAsAdministrator()
      || SessionWrapper(true)->RunningAsPowerUser())
    {
      UnregisterPathNT (true);
    }
  UnregisterPathNT (false);
}

/* _________________________________________________________________________

   RemoveFilesPage::UnregisterPathNT
   _________________________________________________________________________ */

void
RemoveFilesPage::UnregisterPathNT (/*[in]*/ bool shared)
{
#define REGSTR_KEY_ENVIRONMENT_COMMON \
   REGSTR_PATH_CURRENTCONTROLSET_A T_("\\Control\\Session Manager\\Environment")
#define REGSTR_KEY_ENVIRONMENT_USER T_("Environment")
  
  HKEY hkey;

  CA2W subkey (shared
    ? REGSTR_KEY_ENVIRONMENT_COMMON
    : REGSTR_KEY_ENVIRONMENT_USER);

  LONG result =
    RegOpenKeyExW((shared
		   ? HKEY_LOCAL_MACHINE
		   : HKEY_CURRENT_USER),
		  subkey,
		  0,
		  KEY_QUERY_VALUE | KEY_SET_VALUE,
		  &hkey);

  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 (T_("RegOpenKeyExW"), result, 0);
    }

  AutoHKEY autoHKEY (hkey);

  DWORD type;
  CharBuffer<wchar_t> value (32 * 1024);
  DWORD valueSize = static_cast<DWORD>(value.GetCapacity() * sizeof(wchar_t));

  result =
    RegQueryValueExW(hkey,
		     L"Path",
		     0,
		     &type,
		     reinterpret_cast<LPBYTE>(value.GetBuffer()),
		     &valueSize);
 
  bool havePath = (result == ERROR_SUCCESS);

  if (! havePath)
    {
      if (result != ERROR_FILE_NOT_FOUND)
	{
	  FATAL_WINDOWS_ERROR_2 (T_("RegQueryValueExW"), result, 0);
	}
    }
  else
    {
      string path = CW2A(value.Get());
      if (RemoveBinDirFromPath(path))
	{
	  CharBuffer<wchar_t> wpath (CA2W(path.c_str()));
	  result =
	    RegSetValueExW(hkey,
			   L"Path",
			   0,
			   type,
			   reinterpret_cast<const BYTE *>(wpath.Get()),
			   static_cast<DWORD>((StrLen(wpath.Get()) + 1) * sizeof(wpath.Get()[0])));

	  if (result != ERROR_SUCCESS)
	    {
	      FATAL_WINDOWS_ERROR_2 (T_("RegSetValueExW"), result, 0);
	    }
	  
	  DWORD_PTR sendMessageResult;
	  
	  if (SendMessageTimeoutW(HWND_BROADCAST,
				  WM_SETTINGCHANGE,
				  0,
				  reinterpret_cast<LPARAM>(L"Environment"),
				  SMTO_ABORTIFHUNG,
				  5000,
				  &sendMessageResult)
	      == 0)
	    {
	      CHECK_WINDOWS_ERROR (T_("SendMessageTimeout"), 0);
	    }
	}
    }
}

/* _________________________________________________________________________

   RemoveFilesPage::UnregisterPath95
   _________________________________________________________________________ */

#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
void
RemoveFilesPage::UnregisterPath95 ()
{
#define AUTOEXEC_BAT T_("c:\\autoexec.bat")
  if (File::Exists(AUTOEXEC_BAT))
    {
      StreamReader reader (AUTOEXEC_BAT);
      vector<string> lines;
      string line;
      bool removed = false;
      while (reader.ReadLine(line))
	{
	  if (line.substr(0, 9) == T_("set PATH="))
	    {
	      string path = line.substr(9);
	      if (RemoveBinDirFromPath(path))
		{
		  removed = true;
		  line = T_("set PATH=");
		  line += path;
		}
	    }
	  lines.push_back (line);
	}
      reader.Close ();
      if (removed)
	{
	  StreamWriter writer (AUTOEXEC_BAT);
	  for (vector<string>::const_iterator it = lines.begin();
	       it != lines.end();
	       ++ it)
	    {
	      writer.WriteLine (line);
	    }
	  writer.Close ();
	}
    }
}
#endif

/* _________________________________________________________________________

   RemoveFilesPage::RemoveRegistryKeys
   _________________________________________________________________________ */

void
RemoveFilesPage::RemoveRegistryKeys ()
{
  bool shared =
    (! IsWindowsNT()
     || SessionWrapper(true)->RunningAsAdministrator()
     || SessionWrapper(true)->RunningAsPowerUser());

  if (shared && Exists(HKEY_LOCAL_MACHINE, MIKTEX_REGPATH_SERIES))
    {
      RemoveRegistryKey (HKEY_LOCAL_MACHINE, MIKTEX_REGPATH_SERIES);
    }

  if (Exists(HKEY_CURRENT_USER, MIKTEX_REGPATH_SERIES))
    {
      RemoveRegistryKey (HKEY_CURRENT_USER, MIKTEX_REGPATH_SERIES);
    }

  if (shared
      && Exists(HKEY_LOCAL_MACHINE, MIKTEX_REGPATH_PRODUCT)
      && IsEmpty(HKEY_LOCAL_MACHINE, MIKTEX_REGPATH_PRODUCT))
    {
      RemoveRegistryKey (HKEY_LOCAL_MACHINE, MIKTEX_REGPATH_PRODUCT);
    }

  if (shared
      && Exists(HKEY_LOCAL_MACHINE, MIKTEX_REGPATH_COMPANY)
      && IsEmpty(HKEY_LOCAL_MACHINE, MIKTEX_REGPATH_COMPANY))
    {
      RemoveRegistryKey (HKEY_LOCAL_MACHINE, MIKTEX_REGPATH_COMPANY);
    }

  if (Exists(HKEY_CURRENT_USER, MIKTEX_REGPATH_PRODUCT)
      && IsEmpty(HKEY_CURRENT_USER, MIKTEX_REGPATH_PRODUCT))
    {
      RemoveRegistryKey (HKEY_CURRENT_USER, MIKTEX_REGPATH_PRODUCT);
    }

  if (Exists(HKEY_CURRENT_USER, MIKTEX_REGPATH_COMPANY)
      && IsEmpty(HKEY_CURRENT_USER, MIKTEX_REGPATH_COMPANY))
    {
      RemoveRegistryKey (HKEY_CURRENT_USER, MIKTEX_REGPATH_COMPANY);
    }

  if (Exists(HKEY_CURRENT_USER, MIKTEX_GPL_GHOSTSCRIPT))
    {
      RemoveRegistryKey (HKEY_CURRENT_USER, MIKTEX_GPL_GHOSTSCRIPT);
    }
}
  
/* _________________________________________________________________________

   RemoveFilesPage::RemoveRegistryKey
   _________________________________________________________________________ */

void
RemoveFilesPage::RemoveRegistryKey (/*[in]*/ HKEY		hkeyRoot,
				    /*[in]*/ const PathName &	subKey)
{
  AutoHKEY hkeySub;

  LONG result =
    RegOpenKeyExW(hkeyRoot,
                  subKey.ToWideCharString().c_str(),
		  0,
		  KEY_READ,
		  &hkeySub);

  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 (T_("RegOpenKeyExW"), result, 0);
    }
  
  wchar_t szName[BufferSizes::MaxPath];
  DWORD size = BufferSizes::MaxPath;

  FILETIME fileTime;
  
  while ((result = RegEnumKeyExW(hkeySub.Get(),
				 0,
				 szName,
				 &size,
				 0,
				 0,
				 0,
				 &fileTime))
	 == ERROR_SUCCESS)
    {
      RemoveRegistryKey (hkeyRoot, PathName(subKey, szName));
      size = BufferSizes::MaxPath;
    }

  if (result != ERROR_NO_MORE_ITEMS)
    {
      FATAL_WINDOWS_ERROR_2 (T_("RegEnumKeyExW"), result, 0);
    }

  hkeySub.Reset ();

  result = RegDeleteKey(hkeyRoot, subKey.ToWideCharString().c_str());

  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 (T_("RegDeleteKeyW"), result, 0);
    }
}

/* _________________________________________________________________________

   RemoveFilesPage::Exists
   _________________________________________________________________________ */

bool
RemoveFilesPage::Exists (/*[in]*/ HKEY			hkeyRoot,
			 /*[in]*/ const PathName &	subKey)
{
  AutoHKEY hkeySub;

  LONG result =
    RegOpenKeyExW(hkeyRoot,
                  subKey.ToWideCharString().c_str(),
		  0,
		  KEY_READ,
		  &hkeySub);

  if (result != ERROR_SUCCESS)
    {
      if (result == ERROR_FILE_NOT_FOUND)
	{
	  return (false);
	}
      FATAL_WINDOWS_ERROR_2 (T_("RegOpenKeyExW"), result, 0);
    }

  return (true);
}

/* _________________________________________________________________________

   RemoveFilesPage::IsEmpty
   _________________________________________________________________________ */

bool
RemoveFilesPage::IsEmpty (/*[in]*/ HKEY			hkeyRoot,
			  /*[in]*/ const PathName &	subKey)
{
  AutoHKEY hkeySub;

  LONG result =
    RegOpenKeyEx(hkeyRoot,
		 subKey.ToWideCharString().c_str(),
		 0,
		 KEY_READ,
		 &hkeySub);

  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 (T_("RegOpenKeyEx"), result, 0);
    }

  DWORD nSubKeys;
  DWORD nValues;

  result =
    RegQueryInfoKey(hkeySub.Get(),
		    0,
		    0,
		    0,
		    &nSubKeys,
		    0,
		    0,
		    &nValues,
		    0,
		    0,
		    0,
		    0);

  if (result != ERROR_SUCCESS)
    {
      FATAL_WINDOWS_ERROR_2 (T_("RegQueryInfoKey"), result, 0);
    }

  return (nSubKeys + nValues == 0);
}
