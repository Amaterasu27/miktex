/* FileCopyPage.cpp: the actual setup process

   Copyright (C) 1999-2012 Christian Schenk

   This file is part of MiKTeX Setup Wizard.

   MiKTeX Setup Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   MiKTeX Setup Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX Setup Wizard; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"
#include "Setup.h"

#include "FileCopyPage.h"
#include "SetupWizard.h"

const SHORT PROGRESS_MAX = 1000;

const UINT WM_STARTFILECOPY = WM_APP + 1;
const UINT WM_PROGRESS = WM_APP + 2;
const UINT WM_REPORT = WM_APP + 3;

/* _________________________________________________________________________

   FileCopyPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(FileCopyPage, CPropertyPage)
  ON_MESSAGE(WM_PROGRESS, OnProgress)
  ON_MESSAGE(WM_REPORT, OnReport)
  ON_MESSAGE(WM_STARTFILECOPY, OnStartFileCopy)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   FileCopyPage::FileCopyPage
   _________________________________________________________________________ */

FileCopyPage::FileCopyPage ()
  : CPropertyPage (IDD,
		   0,
		   IDS_HEADER_FILECOPY,
		   IDS_SUBHEADER_FILECOPY),
    pSheet (0),
    hWorkerThread (0)
{
}

/* _________________________________________________________________________

   FileCopyPage::~FileCopyPage
   _________________________________________________________________________ */

FileCopyPage::~FileCopyPage ()
{
  try
    {
      if (hWorkerThread != 0)
	{
	  CloseHandle (hWorkerThread);
	  hWorkerThread = 0;
	}
      if (pInstaller.get() != 0)
	{
	  pInstaller->Dispose ();
	  pInstaller.reset ();
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   FileCopyPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
FileCopyPage::OnInitDialog ()
{
  pSheet = reinterpret_cast<SetupWizard*>(GetParent());
  BOOL ret = CPropertyPage::OnInitDialog();
  reportControl.LimitText (100000);
  try
    {
      SessionWrapper(true)->SetAdminMode (theApp.commonUserSetup);
      pInstaller =
	auto_ptr<PackageInstaller>(theApp.pManager->CreateInstaller());
      pInstaller->SetNoPostProcessing (true);
      pInstaller->SetNoLocalServer (true);
    }
  catch (const MiKTeXException & e)
    {
      ReportError (e);
    }
  catch (const exception & e)
    {
      ReportError (e);
    }
  return (ret);
}

/* _________________________________________________________________________

   FileCopyPage::OnSetActive
   _________________________________________________________________________ */

BOOL
FileCopyPage::OnSetActive ()
{
  BOOL ret = CPropertyPage::OnSetActive();

  if (ret)
    {
      try
	{
	  // disable buttons
	  pSheet->SetWizardButtons (0);

#if 0
	  if (theApp.setupTask != SetupTask::Download)
	    {
	      GetControl(IDC_ETA_TITLE)->ShowWindow (SW_HIDE);
	      GetControl(IDC_ETA)->ShowWindow (SW_HIDE);
	    }
#endif

	  // starting shot
	  if (! PostMessage (WM_STARTFILECOPY))
	    {
	      FATAL_WINDOWS_ERROR ("CWnd::PostMessage", 0);
	    }
	}
      catch (const MiKTeXException & e)
	{
	  ReportError (e);
	  ret = FALSE;
	}
      catch (const exception & e)
	{
	  ReportError (e);
	  ret = FALSE;
	}
    }

  return (TRUE);
}

/* _________________________________________________________________________

   FileCopyPage::DoDataExchange
   _________________________________________________________________________ */

void
FileCopyPage::DoDataExchange (/*[in]*/ CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_ANI, animationControl);
  DDX_Control (pDX, IDC_PROGRESS1, progressControl1);
  DDX_Control (pDX, IDC_PROGRESS2, progressControl2);
  DDX_Control (pDX, IDC_REPORT, reportControl);
}

/* _________________________________________________________________________

   FileCopyPage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
FileCopyPage::OnWizardNext ()
{
  return (reinterpret_cast<LRESULT>(MAKEINTRESOURCE(IDD_FINISH)));
}

/* _________________________________________________________________________

   FileCopyPage::OnKillActive
   _________________________________________________________________________ */

BOOL
FileCopyPage::OnKillActive ()
{
  BOOL ret = CPropertyPage::OnKillActive();
  if (ret)
    {
      try
	{
	  ULogClose (! pSheet->GetErrorFlag());
	}
      catch (const MiKTeXException & e)
	{
	  ReportError (e);
	  ret = FALSE;
	}
      catch (const exception & e)
	{
	  ReportError (e);
	  ret = FALSE;
	}
    }
  return (ret);
}

/* _________________________________________________________________________

   FileCopyPage::OnQueryCancel
   _________________________________________________________________________ */

BOOL
FileCopyPage::OnQueryCancel ()
{
  if (hWorkerThread == 0)
    {
      return (CPropertyPage::OnQueryCancel());
    }
  try
    {
      Log (T_("\n<<<Cancel? "));
      if (AfxMessageBox((theApp.setupTask == SetupTask::Download
			 ? IDS_CANCEL_DOWNLOAD
			 : IDS_CANCEL_SETUP),
			MB_OKCANCEL | MB_ICONEXCLAMATION)
	  == IDOK)
	{
	  Log (T_("Yes!>>>\n"));
	  pSheet->SetCancelFlag ();
	  if (! PostMessage(WM_PROGRESS))
	    {
	      FATAL_WINDOWS_ERROR ("CWnd::PostMessage", 0);
	    }
	}
      else
	{
	  Log (T_("No!>>>\n"));
	}
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

   FileCopyPage::OnStartFileCopy
   _________________________________________________________________________ */

LRESULT
FileCopyPage::OnStartFileCopy (/*[in]*/ WPARAM	wParam,
			       /*[in]*/ LPARAM	lParam)
{
  UNUSED_ALWAYS (wParam);
  UNUSED_ALWAYS (lParam);

  try
    {
      if (! animationControl.Open(theApp.setupTask == SetupTask::Download
				  ? IDA_DOWNLOAD
				  : IDA_FILECOPY))
	{
	  FATAL_WINDOWS_ERROR ("CAnimateCtrl::Open", 0);
	}

      if (! animationControl.Play(0,
				  static_cast<UINT>(-1),
				  static_cast<UINT>(-1)))
	{
	  FATAL_WINDOWS_ERROR ("CAnimateCtrl::Play", 0);
	}

      // initialize progress bar controls
      progressControl1.SetRange (0, PROGRESS_MAX);
      progressControl1.SetPos (0);
      progressControl2.SetRange (0, PROGRESS_MAX);
      progressControl2.SetPos (0);

      if (theApp.setupTask == SetupTask::Download)
	{
	  GetControl(IDC_PROGRESS1_TITLE)->SetWindowText (T_(_T("Downloading:")));
	}

      // create the worker thread
      CWinThread * pThread
	= AfxBeginThread(WorkerThread,
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
	  FATAL_WINDOWS_ERROR ("DuplicateHandle", 0);
	}
      pThread->ResumeThread ();
    }
  catch (const MiKTeXException & e)
    {
      ReportError (e);
    }
  catch (const exception & e)
    {
      ReportError (e);
    }

  return (0);
}

/* _________________________________________________________________________

   FileCopyPage::OnProgress
   _________________________________________________________________________ */

LRESULT
FileCopyPage::OnProgress (/*[in]*/ WPARAM	wParam,
			  /*[in]*/ LPARAM	lParam)
{
  UNUSED_ALWAYS (wParam);
  UNUSED_ALWAYS (lParam);

  try
    {
      CSingleLock singleLock (&criticalSectionMonitor, TRUE);

      // do we have to finish?
      if (sharedData.ready
	  || pSheet->GetCancelFlag()
	  || pSheet->GetErrorFlag())
	{
	  // check to see if we are already ready
	  if (sharedData.waitingForClickOnNext)
	    {
	      return (0);
	    }

	  sharedData.waitingForClickOnNext = true;

	  // close the wizard, if it is running unattended
	  if (theApp.unattended)
	    {
	      ULogClose (! pSheet->GetErrorFlag());
	      EndDialog (IDOK);
	    }

	  // stop the video
	  if (! animationControl.Stop())
	    {
	      FATAL_WINDOWS_ERROR ("CAnimateCtrl::Stop", 0);
	    }
	  animationControl.Close (); // always returns FALSE
	  animationControl.ShowWindow (SW_HIDE);

	  // disable controls
	  EnableControl (IDC_PROGRESS1_TITLE, false);
	  GetControl(IDC_PACKAGE)->SetWindowText (_T(""));
	  EnableControl (IDC_PACKAGE, false);
	  progressControl1.SetPos (0);
	  progressControl1.EnableWindow (FALSE);
	  EnableControl (IDC_PROGRESS2_TITLE, false);
	  progressControl2.SetPos (0);
	  progressControl2.EnableWindow (FALSE);
#if 0
	  GetControl(IDC_ETA_TITLE)->ShowWindow (SW_HIDE);
	  GetControl(IDC_ETA)->SetWindowText ("");
	  GetControl(IDC_ETA)->ShowWindow (SW_HIDE);
#endif
      
	  // enable Next button
	  pSheet->SetWizardButtons (PSWIZB_NEXT);
	}
      else
	{
	  // show the package name
	  if (sharedData.newPackage)
	    {
	      GetControl(IDC_PACKAGE)->SetWindowText
		(UW_(sharedData.packageName.c_str()));
	      sharedData.newPackage = false;
	    }

	  // update progress bars
	  progressControl1.SetPos (sharedData.progress1Pos);
	  progressControl2.SetPos (sharedData.progress2Pos);

#if 0
	  // update ETA
	  if (GetTickCount() > timeOfLastProgressRefresh + 1000)
	    {
	      CTimeSpan timeRemaining (sharedData.secondsRemaining);
	      CString str;
	      if (timeRemaining.GetTotalHours() > 0)
		{
		  str.Format (T_("%u hours, %u minutes"),
			      timeRemaining.GetTotalHours(),
			      timeRemaining.GetMinutes());
		}
	      else if (timeRemaining.GetTotalMinutes() > 0)
		{
		  str.Format (T_("%u minutes"),
			      timeRemaining.GetTotalMinutes());
		}
	      else
		{
		  str.Format (T_("%u seconds"),
			      timeRemaining.GetTotalSeconds());
		}
	      timeOfLastProgressRefresh = GetTickCount();
	      GetControl(IDC_ETA)->SetWindowText (str);
	    }
#endif
	}
    }
  catch (const MiKTeXException & e)
    {
      ReportError (e);
      pSheet->SetWizardButtons (PSWIZB_NEXT);
      sharedData.waitingForClickOnNext = true;
    }
  catch (const exception & e)
    {
      ReportError (e);
      pSheet->SetWizardButtons (PSWIZB_NEXT);
      sharedData.waitingForClickOnNext = true;
    }
  return (0);
}

/* _________________________________________________________________________

   FileCopyPage::OnReport
   _________________________________________________________________________ */

LRESULT
FileCopyPage::OnReport (/*[in]*/ WPARAM	wParam,
			/*[in]*/ LPARAM	lParam)
{
  UNUSED_ALWAYS (lParam);
  long len = reportControl.GetTextLength();
  reportControl.SetSel (len, len);
  reportControl.ReplaceSel (reinterpret_cast<LPCTSTR>(wParam));
  reportControl.SendMessage (EM_SCROLLCARET);
  return (0);
}
/* _________________________________________________________________________

   FileCopyPage::OnProcessOutput
   _________________________________________________________________________ */

bool
FileCopyPage::OnProcessOutput (/*[in]*/ const void *	pOutput,
			       /*[in]*/ size_t		n)
{
  Report (true, "%.*s", n, reinterpret_cast<const char *>(pOutput));
  return (! (pSheet->GetErrorFlag() || pSheet->GetCancelFlag()));
}

/* _________________________________________________________________________

   FileCopyPage::ReportLine
   _________________________________________________________________________ */

void
FileCopyPage::ReportLine (/*[in]*/ const char * lpszLine)
{
  Report (true, "%s\n", lpszLine);
}

/* _________________________________________________________________________

   FileCopyPage::OnRetryableError
   _________________________________________________________________________ */

bool
FileCopyPage::OnRetryableError (/*[in]*/ const char * lpszMessage)
{
  UINT uType = MB_ICONSTOP;
  uType |= MB_RETRYCANCEL;
  string str = lpszMessage;
  str += T_("  Then click Retry to complete the operation.");
  UINT u = ::MessageBoxA(0, str.c_str(), 0, uType);
  return (u != IDCANCEL);
}

/* _________________________________________________________________________

   FileCopyPage::OnProgress
   _________________________________________________________________________ */

bool
FileCopyPage::OnProgress (/*[in]*/ Notification		nf)
{
  CSingleLock singlelock (&criticalSectionMonitor, TRUE);

  bool visibleProgress = false;

  PackageInstaller::ProgressInfo progressInfo = pInstaller->GetProgressInfo();

  if (nf == Notification::InstallPackageStart
      || nf == Notification::DownloadPackageStart)
    {
      visibleProgress = true;
      sharedData.newPackage = true;
      sharedData.packageName = progressInfo.displayName;
    }

  if (theApp.setupTask == SetupTask::Download)
    {
      if (progressInfo.cbPackageDownloadTotal > 0)
	{
	  int oldValue = sharedData.progress1Pos;
	  sharedData.progress1Pos
	    = static_cast<int>(
	      ((static_cast<double>(progressInfo.cbPackageDownloadCompleted)
		/ progressInfo.cbPackageDownloadTotal)
	       * PROGRESS_MAX));
	  visibleProgress =
	    (visibleProgress || (sharedData.progress1Pos != oldValue));
	}
      if (progressInfo.cbDownloadTotal > 0)
	{
	  int oldValue = sharedData.progress2Pos;
	  sharedData.progress2Pos
	    = static_cast<int>(
	      ((static_cast<double>(progressInfo.cbDownloadCompleted)
		/ progressInfo.cbDownloadTotal)
	       * PROGRESS_MAX));
	  visibleProgress =
	    (visibleProgress || (sharedData.progress2Pos != oldValue));
	}
      DWORD oldValue = sharedData.secondsRemaining;
      sharedData.secondsRemaining
	= static_cast<DWORD>(progressInfo.timeRemaining / 1000);
      visibleProgress =
	(visibleProgress || (sharedData.secondsRemaining != oldValue));
    }
  else if ((theApp.setupTask == SetupTask::InstallFromLocalRepository
	    || theApp.setupTask == SetupTask::InstallFromCD)
	   && progressInfo.cbInstallTotal > 0)
    {
      totalSize = static_cast<DWORD>(progressInfo.cbInstallTotal);

      // calculate initexmf contribution
      size_t uSizeExtra =
	totalIniTeXMFRuns * GetIniTeXMFRunSize();

      overallExpenditure = totalSize + uSizeExtra;
      if (progressInfo.cbPackageInstallTotal)
	{
	  int oldValue = sharedData.progress1Pos;
	  sharedData.progress1Pos
	    = static_cast<int>(
	      ((static_cast<double>(progressInfo.cbPackageInstallCompleted)
		/ progressInfo.cbPackageInstallTotal)
	       * PROGRESS_MAX));
	  visibleProgress =
	    (visibleProgress || (sharedData.progress1Pos != oldValue));
	}
      int oldValue = sharedData.progress2Pos;
      sharedData.progress2Pos
	= static_cast<int>(
	  ((static_cast<double>(progressInfo.cbInstallCompleted)
	    / overallExpenditure)
	   * PROGRESS_MAX));
      visibleProgress =
	(visibleProgress || (sharedData.progress2Pos != oldValue));
    }

  if (visibleProgress)
    {
      if (! PostMessage(WM_PROGRESS))
	{
	  FATAL_WINDOWS_ERROR ("CWnd::PostMessage", 0);
	}
    }

  return (! (pSheet->GetErrorFlag() || pSheet->GetCancelFlag()));
}

/* _________________________________________________________________________

   FileCopyPage::WorkerThread
   _________________________________________________________________________ */

UINT
FileCopyPage::WorkerThread (/*[in]*/ void * pParam)
{
  FileCopyPage * This = reinterpret_cast<FileCopyPage*>(pParam);

  This->timeOfLastProgressRefresh = 0;

  if (theApp.setupTask == SetupTask::PrepareMiKTeXDirect)
    {
      theApp.SetInstallRoot (theApp.MiKTeXDirectTeXMFRoot);
    }

  bool comInit = false;

  try
    {
      // initialize COM
      if (FAILED(CoInitialize(0)))
	{
	  UNEXPECTED_CONDITION ("FileCopyPage::WorkerThread");
	}
      comInit = true;

      // write the log header
      LogHeader ();
      
      switch (theApp.setupTask.Get())
	{
	case SetupTask::Download:
	  This->DoTheDownload ();
	  break;
	case SetupTask::PrepareMiKTeXDirect:
	  This->DoPrepareMiKTeXDirect ();
	  break;
	case SetupTask::InstallFromCD:
	case SetupTask::InstallFromLocalRepository:
	  This->DoTheInstallation ();
	  break;
	default:
	  ASSERT (false);
	  __assume (false);
	  break;
	}
    }
  catch (const MiKTeXException & e)
    {
      This->ReportError (e);
    }
  catch (const exception & e)
    {
      This->ReportError (e);
    }

  if (comInit)
    {
      CoUninitialize ();
    }

  This->sharedData.ready = true;

  try
    {
      if (! This->PostMessage(WM_PROGRESS))
	{
	  FATAL_WINDOWS_ERROR ("CWnd::PostMessage", 0);
	}
    }
  catch (const MiKTeXException & e)
    {
      This->ReportError (e);
    }
  catch (const exception & e)
    {
      This->ReportError (e);
    }

  return (0);
}

/* _________________________________________________________________________

   FileCopyPage::DoTheDownload
   _________________________________________________________________________ */

void
FileCopyPage::DoTheDownload ()
{
  // calculate overall expenditure
  CalculateExpenditure ();

  // initialize installer
  pInstaller->SetRepository (theApp.remotePackageRepository.c_str());
  pInstaller->SetDownloadDirectory (theApp.localPackageRepository);
  pInstaller->SetPackageLevel (theApp.packageLevel);
  pInstaller->SetCallback (this);

  // create the local repository directory
  Directory::Create (theApp.localPackageRepository);

  // remember local repository folder
  SessionWrapper(true)
    ->SetConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
		      MIKTEX_REGVAL_LOCAL_REPOSITORY,
		      theApp.localPackageRepository.Get());

  // start downloader in the background
  pInstaller->DownloadAsync();

  // wait for downloader thread
  pInstaller->WaitForCompletion ();

  if (pSheet->GetCancelFlag())
    {
      return;
    }

  // copy the license file
  PathName licenseFile;
  if (FindFile(LICENSE_FILE, licenseFile))
    {
      PathName licenseFileDest (theApp.localPackageRepository, LICENSE_FILE);
      if (ComparePaths(licenseFile.Get(), licenseFileDest.Get(), true) != 0)
	{
	  File::Copy (licenseFile, licenseFileDest);
	}
    }

  // now copy the setup program
  if (! theApp.setupPath.Empty())
    {
      char szFileName[BufferSizes::MaxPath];
      char szExt[BufferSizes::MaxPath];
      PathName::Split (theApp.setupPath.Get(),
		       0, 0,
		       0, 0,
		       szFileName, BufferSizes::MaxPath,
		       szExt, BufferSizes::MaxPath);
      PathName pathDest (theApp.localPackageRepository, szFileName, szExt);
      if (ComparePaths(theApp.setupPath.Get(), pathDest.Get(), true) != 0)
	{
	  File::Copy (theApp.setupPath, pathDest);
	}
    }

  // create info file
  CreateInfoFile ();
}

/* _________________________________________________________________________

   FileCopyPage::DoPrepareMiKTeXDirect
   _________________________________________________________________________ */

void
FileCopyPage::DoPrepareMiKTeXDirect ()
{
  // open the uninstall script
  ULogOpen ();
#if 0				// <fixme/>
  ULogAddFile (g_strLogFile);
#endif

  // run IniTeXMF
  ConfigureMiKTeX ();

  // create shell links
  if (! theApp.portable)
  {
    CreateProgramIcons ();
  }

  // register path
  if (! theApp.portable && theApp.registerPath)
    {
      Utils::CheckPath (true);
    }
}

/* _________________________________________________________________________

   FileCopyPage::DoTheInstallation
   _________________________________________________________________________ */

void
FileCopyPage::DoTheInstallation ()
{
  // register installation directory
  StartupConfig startupConfig;
  if (theApp.commonUserSetup)
  {
    startupConfig.commonInstallRoot = theApp.startupConfig.commonInstallRoot;
  }
  startupConfig.userInstallRoot = theApp.startupConfig.userInstallRoot;
  SessionWrapper(true)->RegisterRootDirectories (
    startupConfig,
    RegisterRootDirectoriesFlags::Temporary | RegisterRootDirectoriesFlags::NoRegistry);

  // parse package definition files
  PathName pathDB;
  if (theApp.isMiKTeXDirect)
    {
      pathDB.Set (theApp.MiKTeXDirectTeXMFRoot,
		  MIKTEX_PATH_PACKAGE_DEFINITION_DIR);
    }
  else
    {
      pathDB.Set (theApp.localPackageRepository, MIKTEX_MPM_DB_FULL_FILE_NAME);
    }
  Report (true, T_("Loading package database...\n"));
  theApp.pManager->LoadDatabase (pathDB);

  // calculate overall expenditure
  CalculateExpenditure ();

  if (theApp.isMiKTeXDirect)
    {
      pInstaller->SetRepository (theApp.MiKTeXDirectRoot.Get());
    }
  else
    {
      pInstaller->SetRepository  (theApp.localPackageRepository.Get());
      // remember local repository folder
      if (! theApp.prefabricated)
	{
	  theApp.pManager->SetLocalPackageRepository
	    (theApp.localPackageRepository);
	}
    }
  pInstaller->SetPackageLevel (theApp.packageLevel);
  pInstaller->SetCallback (this);

  // create the destination directory
  Directory::Create (theApp.GetInstallRoot());

  // open the uninstall script
  ULogOpen ();
#if 0				// <fixme/>
  ULogAddFile (g_strLogFile);
#endif

  // run installer
  pInstaller->InstallRemove ();

  if (pSheet->GetCancelFlag())
    {
      return;
    }

  // install package definition files
  theApp.pManager->UnloadDatabase ();
  pInstaller->UpdateDb ();

  if (pSheet->GetCancelFlag())
    {
      return;
    }

  // run IniTeXMF
  ConfigureMiKTeX ();

  if (pSheet->GetCancelFlag())
    {
      return;
    }

  // remove obsolete files
  RemoveObsoleteFiles ();

  if (pSheet->GetCancelFlag())
    {
      return;
    }

  // create shell links
  if (! theApp.portable)
  {
    CreateProgramIcons ();
  }

  if (pSheet->GetCancelFlag())
    {
      return;
    }

  // register path
  if (! theApp.portable && theApp.registerPath)
    {
      Utils::CheckPath (true);
    }

  if (theApp.portable)
  {
    PathName fileName (theApp.startupConfig.commonInstallRoot);
    fileName += "miktex-portable.cmd";
    StreamWriter starter (fileName);
    starter.WriteLine ("@echo off");
    starter.WriteLine ("miktex\\bin\\miktex-taskbar-icon.exe");
    starter.Close ();
  }
}

/* _________________________________________________________________________

   FileCopyPage::GetIniTeXMFRunSize

   Calculate the contribution of one initexmf run to the total size.
   _________________________________________________________________________ */

size_t
FileCopyPage::GetIniTeXMFRunSize ()
  const
{
  MIKTEX_ASSERT (totalSize > 0);
  return (totalSize / 200 + 1);
}

/* _________________________________________________________________________

   FileCopyPage::ConfigureMiKTeX
   _________________________________________________________________________ */

void
FileCopyPage::ConfigureMiKTeX ()
{
  PathName initexmf (theApp.GetInstallRoot());
  initexmf += MIKTEX_PATH_BIN_DIR;
  initexmf += MIKTEX_INITEXMF_EXE;

  VersionNumber initexmfVer = GetFileVersion(initexmf);

  completedIniTeXMFRuns = 0;

  // refresh progress bar
  {
    CSingleLock singlelock (&criticalSectionMonitor, TRUE);
    CString str;
    VERIFY (str.LoadString(IDS_INITEXMF));
    sharedData.packageName = TU_(str);
    sharedData.newPackage = true;
    sharedData.progress1Pos = 0;
    if (! PostMessage (WM_PROGRESS))
      {
	FATAL_WINDOWS_ERROR ("CWnd::PostMessage", 0);
      }
  }
  
  CommandLineBuilder cmdLine;

  if (theApp.setupTask != SetupTask::PrepareMiKTeXDirect)
    {
      // [1] define roots & remove old fndbs
      cmdLine.Clear ();
      if (theApp.portable)
      {
	cmdLine.AppendOption ("--portable=",
	  theApp.startupConfig.commonInstallRoot);
      }
      else
      {
	if (! theApp.startupConfig.userInstallRoot.Empty())
	{
	  cmdLine.AppendOption ("--user-install=",
	    theApp.startupConfig.userInstallRoot);
	}
	if (! theApp.startupConfig.userDataRoot.Empty())
	{
	  cmdLine.AppendOption ("--user-data=",
	    theApp.startupConfig.userDataRoot);
	}
	if (! theApp.startupConfig.userConfigRoot.Empty())
	{
	  cmdLine.AppendOption ("--user-config=",
	    theApp.startupConfig.userConfigRoot);
	}
	if (! theApp.startupConfig.commonDataRoot.Empty())
	{
	  cmdLine.AppendOption ("--common-data=",
	    theApp.startupConfig.commonDataRoot);
	}
	if (! theApp.startupConfig.commonConfigRoot.Empty())
	{
	  cmdLine.AppendOption ("--common-config=",
	    theApp.startupConfig.commonConfigRoot);
	}
	if (! theApp.startupConfig.commonInstallRoot.Empty())
	{
	  cmdLine.AppendOption ("--common-install=",
	    theApp.startupConfig.commonInstallRoot);
	}
	if (theApp.noRegistry)
	{
	  cmdLine.AppendOption ("--no-registry");
	  cmdLine.AppendOption ("--create-config-file=", MIKTEX_PATH_MIKTEX_INI);
	  cmdLine.AppendOption (
	    "--set-config-value=",
	    "[" MIKTEX_REGKEY_CORE "]" MIKTEX_REGVAL_NO_REGISTRY "=1");
	}
      }
      if (! theApp.startupConfig.commonRoots.empty())
      {
	cmdLine.AppendOption ("--common-roots=", theApp.startupConfig.commonRoots);
      }
      if (! theApp.startupConfig.userRoots.empty())
      {
	cmdLine.AppendOption ("--user-roots=", theApp.startupConfig.userRoots);
      }
      cmdLine.AppendOption ("--rmfndb");
      RunIniTeXMF (cmdLine);
      if (pSheet->GetCancelFlag())
	{
	  return;
	}
      
      // [2] register components, configure files
      RunMpm ("--register-components");

      // [3] create filename database files
      cmdLine.Clear ();
      cmdLine.AppendOption ("--update-fndb");
      RunIniTeXMF (cmdLine);
      if (pSheet->GetCancelFlag())
	{
	  return;
	}

      // [4] create latex.exe, ...
      RunIniTeXMF (CommandLineBuilder("--force", "--mklinks"));
      if (pSheet->GetCancelFlag())
	{
	  return;
	}
      
      // [5] create font map files and language.dat
      RunIniTeXMF (CommandLineBuilder("--mkmaps", "--mklangs"));
      if (pSheet->GetCancelFlag())
	{
	  return;
	}
    }
      
  // [6] set paper size
  if (! theApp.paperSize.empty())
    {
      cmdLine.Clear ();
      if (StringCompare(theApp.paperSize.c_str(), "a4", true) == 0)
	{
	  cmdLine.AppendOption ("--default-paper-size=", "A4size");
	}
      else
	{
	}
    }
  else
    {
      completedIniTeXMFRuns += 1;
    }

  // [ ] set auto-install
  string valueSpec = "[" MIKTEX_REGKEY_PACKAGE_MANAGER "]";
  valueSpec += MIKTEX_REGVAL_AUTO_INSTALL;
  valueSpec += "=";
  valueSpec += NUMTOSTR(theApp.installOnTheFly.Get());
  cmdLine.Clear ();
  cmdLine.AppendOption ("--set-config-value=", valueSpec.c_str());
  RunIniTeXMF (cmdLine);

  if (theApp.setupTask != SetupTask::PrepareMiKTeXDirect)
    {
      // [7] refresh file name database again
      RunIniTeXMF ("--update-fndb");
      if (pSheet->GetCancelFlag())
	{
	  return;
	}
    }

  if (! theApp.portable)
  {
    RunIniTeXMF ("--register-shell-file-types");
  }
      
  if (! theApp.portable && theApp.registerPath)
  {
    RunIniTeXMF ("--modify-path");
  }

  // [8] create report
  RunIniTeXMF ("--report");
  if (pSheet->GetCancelFlag())
    {
      return;
    }
}

/* _________________________________________________________________________

   FileCopyPage::RunIniTeXMF
   _________________________________________________________________________ */

void
FileCopyPage::RunIniTeXMF (/*[in]*/ const CommandLineBuilder & cmdLine1)
{
  // make absolute exe path name
  PathName exePath;
  exePath = theApp.GetInstallRoot();
  exePath += MIKTEX_PATH_BIN_DIR;
  exePath += MIKTEX_INITEXMF_EXE;

  // make command line
  CommandLineBuilder cmdLine (cmdLine1);
  if (theApp.commonUserSetup)
  {
    cmdLine.AppendOption ("--admin");
  }
  cmdLine.AppendOption ("--log-file=", GetLogFileName());
  cmdLine.AppendOption ("--verbose");

  // run initexmf.exe
  if (! theApp.dryRun)
    {
      Log ("%s %s:\n", Q_(exePath.Get()), cmdLine.Get());
      ULogClose ();
      SessionWrapper(true)->UnloadFilenameDatabase ();
      Process::Run (exePath.Get(), cmdLine.Get(), this);
      ULogOpen ();
    }

  // refresh progress bars
  if (! pSheet->GetCancelFlag())
    {
      completedIniTeXMFRuns += 1;
      CSingleLock singlelock (&criticalSectionMonitor, TRUE);
      sharedData.progress1Pos
	= static_cast<int>(
	  ((static_cast<double>(completedIniTeXMFRuns)
	    / totalIniTeXMFRuns)
	   * PROGRESS_MAX));
      sharedData.progress2Pos
	= static_cast<int>(
	  ((static_cast<double>(totalSize
				+ (GetIniTeXMFRunSize()
				   * completedIniTeXMFRuns))
	    / overallExpenditure)
	   * PROGRESS_MAX));
      PostMessage (WM_PROGRESS);
    }
}

/* _________________________________________________________________________

   FileCopyPage::RunMpm
   _________________________________________________________________________ */

void
FileCopyPage::RunMpm (/*[in]*/ const CommandLineBuilder & cmdLine1)
{
  // make absolute exe path name
  PathName exePath;
  exePath = theApp.GetInstallRoot();
  exePath += MIKTEX_PATH_BIN_DIR;
  exePath += MIKTEX_MPM_EXE;

  // make command line
  CommandLineBuilder cmdLine (cmdLine1);
  if (theApp.commonUserSetup)
  {
    cmdLine.AppendOption ("--admin");
  }
  cmdLine.AppendOption ("--verbose");

  // run mpm.exe
  if (! theApp.dryRun)
    {
      Log ("%s %s:\n", Q_(exePath.Get()), cmdLine.Get());
      ULogClose ();
      SessionWrapper(true)->UnloadFilenameDatabase ();
      Process::Run (exePath.Get(), cmdLine.Get(), this);
      ULogOpen ();
    }

  // refresh progress bars
  if (! pSheet->GetCancelFlag())
    {
      completedIniTeXMFRuns += 1;
      CSingleLock singlelock (&criticalSectionMonitor, TRUE);
      sharedData.progress1Pos
	= static_cast<int>(
	  ((static_cast<double>(completedIniTeXMFRuns)
	    / totalIniTeXMFRuns)
	   * PROGRESS_MAX));
      sharedData.progress2Pos
	= static_cast<int>(
	  ((static_cast<double>(totalSize
				+ (GetIniTeXMFRunSize()
				   * completedIniTeXMFRuns))
	    / overallExpenditure)
	   * PROGRESS_MAX));
      PostMessage (WM_PROGRESS);
    }
}

/* _________________________________________________________________________

   FileCopyPage::CalcutaleExpenditure
   _________________________________________________________________________ */

void
FileCopyPage::CalculateExpenditure ()
{
  if (theApp.setupTask == SetupTask::InstallFromLocalRepository
      || theApp.setupTask == SetupTask::InstallFromCD)
    {
      totalIniTeXMFRuns = 8;
    }
  else
    {
      totalIniTeXMFRuns = 2;
    }
}

/* _________________________________________________________________________

   FileCopyPage::Report
   _________________________________________________________________________ */

void
FileCopyPage::Report (/*[in]*/ bool		writeLog,
		      /*[in]*/ const char *	lpszFmt,
		      /*[in]*/			...)
{
  MIKTEX_ASSERT (lpszFmt != 0);
  va_list args;
  va_start (args, lpszFmt);
  string str (Utils::FormatString(lpszFmt, args));
  va_end (args);
  int len = str.length();
  CSingleLock singlelock (&criticalSectionMonitor, TRUE);
  string lines;
  for (int i = 0; i < len; ++ i)
  {
    if (str[i] == '\n' && i > 0 && str[i] != '\r')
    {
      sharedData.currentLine += '\r';
    }
    sharedData.currentLine += str[i];
    if (str[i] == '\n')
    {
      lines += sharedData.currentLine;
      sharedData.currentLine.clear ();
    }
  }
  if (writeLog)
  {
    Log ("%s", str.c_str());
  }
  if (! lines.empty())
  {
    singlelock.Unlock ();
    SendMessage (WM_REPORT, reinterpret_cast<WPARAM>(static_cast<LPCTSTR>(UT_(lines))));
  }
}

/* _________________________________________________________________________

   FileCopyPage::GetControl
   _________________________________________________________________________ */

CWnd *
FileCopyPage::GetControl (/*[in]*/ UINT	controlId)
{
  CWnd * pWnd = GetDlgItem(controlId);
  if (pWnd == 0)
    {
      UNEXPECTED_CONDITION ("FileCopyPage::GetControl");
    }
  return (pWnd);
}


/* _________________________________________________________________________

   FileCopyPage::EnableControl
   _________________________________________________________________________ */

void
FileCopyPage::EnableControl (/*[in]*/ UINT	controlId,
			     /*[in]*/ bool	enable)
{
  GetControl(controlId)->EnableWindow (enable ? TRUE : FALSE);
}

/* _________________________________________________________________________

   FileCopyPage::RemoveObsoleteFiles
   _________________________________________________________________________ */

void
FileCopyPage::RemoveObsoleteFiles ()
{
  extern const char * g_apszObsoleteFiles[];
  for (size_t i = 0; g_apszObsoleteFiles[i] != 0; ++ i)
    {
      PathName path (theApp.GetInstallRoot(), g_apszObsoleteFiles[i]);
      // <todo/>
    }
}

/* _________________________________________________________________________

   FileCopyPage::CreateInfoFile
   _________________________________________________________________________ */

void
FileCopyPage::CreateInfoFile ()
{
  StreamWriter
    stream (PathName(theApp.localPackageRepository, DOWNLOAD_INFO_FILE));
  const char * lpszPackageSet;
  switch (theApp.packageLevel.Get())
    {
    case PackageLevel::Essential:
      lpszPackageSet = ESSENTIAL_MIKTEX;
      break;
    case PackageLevel::Basic:
      lpszPackageSet = BASIC_MIKTEX;
      break;
    case PackageLevel::Complete:
      lpszPackageSet = COMPLETE_MIKTEX;
      break;
    default:
      MIKTEX_ASSERT (false);
      __assume (false);
    }
  PathName setupExe (theApp.setupPath);
  setupExe.RemoveDirectorySpec ();
  stream.WriteFormattedLine (T_("\
This folder contains the %s package set.\n\
\n\
To install MiKTeX, run %s.\n\
\n\
For more information, visit the MiKTeX project page at\n\
http://miktex.org.\n"),
			     lpszPackageSet,
			     setupExe.Get());
  stream.Close ();
  RepositoryInfo repositoryInfo;
  if (theApp.pManager->TryGetRepositoryInfo(theApp.remotePackageRepository, repositoryInfo))
  {
    StreamWriter stream (PathName(theApp.localPackageRepository, "pr.ini"));
    stream.WriteFormattedLine ("[repository]");
    stream.WriteFormattedLine ("date=%d", static_cast<int>(repositoryInfo.timeDate));
    stream.WriteFormattedLine ("version=%u", static_cast<unsigned>(repositoryInfo.version));
    stream.Close ();
  }
}

/* _________________________________________________________________________

   FileCopyPage::ReportError
   _________________________________________________________________________ */

void
FileCopyPage::ReportError (/*[in]*/ const MiKTeXException & e)
{
  Report (false, T_("\nError: %s\n"), e.what());
  pSheet->ReportError (e);
}

/* _________________________________________________________________________

   FileCopyPage::ReportError
   _________________________________________________________________________ */

void
FileCopyPage::ReportError (/*[in]*/ const exception & e)
{
  Report (false, T_("\nError: %s\n"), e.what());
  pSheet->ReportError (e);
}

