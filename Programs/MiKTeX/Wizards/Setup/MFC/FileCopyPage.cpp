/* FileCopyPage.cpp: the actual setup process

   Copyright (C) 1999-2006 Christian Schenk

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
      pInstaller =
	auto_ptr<PackageInstaller>(theApp.pManager->CreateInstaller());
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
	      FATAL_WINDOWS_ERROR (T_("CWnd::PostMessage"), 0);
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
	      FATAL_WINDOWS_ERROR (T_("CWnd::PostMessage"), 0);
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
	  FATAL_WINDOWS_ERROR (T_("CAnimatCtrl::Open"), 0);
	}

      if (! animationControl.Play(0,
				  static_cast<UINT>(-1),
				  static_cast<UINT>(-1)))
	{
	  FATAL_WINDOWS_ERROR (T_("CAnimatCtrl::Play"), 0);
	}

      // initialize progress bar controls
      progressControl1.SetRange (0, PROGRESS_MAX);
      progressControl1.SetPos (0);
      progressControl2.SetRange (0, PROGRESS_MAX);
      progressControl2.SetPos (0);

      if (theApp.setupTask == SetupTask::Download)
	{
	  GetControl(IDC_PROGRESS1_TITLE)->SetWindowText (T_("Downloading:"));
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
	  FATAL_WINDOWS_ERROR (T_("DuplicateHandle"), 0);
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
	      EndDialog (IDOK);
	    }

	  // stop the video
	  if (! animationControl.Stop())
	    {
	      FATAL_WINDOWS_ERROR (T_("CAnimateCtrl::Stop"), 0);
	    }
	  animationControl.Close (); // always returns FALSE
	  animationControl.ShowWindow (SW_HIDE);

	  // disable controls
	  EnableControl (IDC_PROGRESS1_TITLE, false);
	  GetControl(IDC_PACKAGE)->SetWindowText (T_(""));
	  EnableControl (IDC_PACKAGE, false);
	  progressControl1.SetPos (0);
	  progressControl1.EnableWindow (FALSE);
	  CHECK_WINDOWS_ERROR (T_("CWnd::EnableWindow"), 0);
	  EnableControl (IDC_PROGRESS2_TITLE, false);
	  progressControl2.SetPos (0);
	  progressControl2.EnableWindow (FALSE);
	  CHECK_WINDOWS_ERROR (T_("CWnd::EnableWindow"), 0);
#if 0
	  GetControl(IDC_ETA_TITLE)->ShowWindow (SW_HIDE);
	  GetControl(IDC_ETA)->SetWindowText (T_(""));
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
		(sharedData.packageName.c_str());
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
  reportControl.ReplaceSel (reinterpret_cast<const MIKTEXCHAR *>(wParam));
  reportControl.SendMessage (EM_SCROLLCARET);
  return (0);
}
/* _________________________________________________________________________

   FileCopyPage::OnProcessOutput
   _________________________________________________________________________ */

bool
MIKTEXCALL
FileCopyPage::OnProcessOutput (/*[in]*/ const void *	pOutput,
			       /*[in]*/ size_t		n)
{
  Report (true, T_("%.*s"), n, reinterpret_cast<const char *>(pOutput));
  return (! (pSheet->GetErrorFlag() || pSheet->GetCancelFlag()));
}

/* _________________________________________________________________________

   FileCopyPage::ReportLine
   _________________________________________________________________________ */

void
MPMCALL
FileCopyPage::ReportLine (/*[in]*/ const MIKTEXCHAR * lpszLine)
{
  Report (true, T_("%s\n"), lpszLine);
}

/* _________________________________________________________________________

   FileCopyPage::OnRetryableError
   _________________________________________________________________________ */

bool
MPMCALL
FileCopyPage::OnRetryableError (/*[in]*/ const MIKTEXCHAR * lpszMessage)
{
  UINT uType = MB_ICONSTOP;
  uType |= MB_RETRYCANCEL;
  tstring str = lpszMessage;
  str += T_("  Then click Retry to complete the operation.");
  UINT u = ::MessageBox(0, str.c_str(), 0, uType);
  return (u != IDCANCEL);
}

/* _________________________________________________________________________

   FileCopyPage::OnProgress
   _________________________________________________________________________ */

bool
MPMCALL
FileCopyPage::OnProgress (/*[in]*/ Notification		nf)
{
  CSingleLock (&criticalSectionMonitor, TRUE);

  PackageInstaller::ProgressInfo progressInfo = pInstaller->GetProgressInfo();

  if (nf == Notification::InstallPackageStart
      || nf == Notification::DownloadPackageStart)
    {
      sharedData.newPackage = true;
      sharedData.packageName = progressInfo.displayName;
    }

  if (theApp.setupTask == SetupTask::Download)
    {
      if (progressInfo.cbPackageDownloadTotal > 0)
	{
	  sharedData.progress1Pos
	    = static_cast<int>(
	      ((static_cast<double>(progressInfo.cbPackageDownloadCompleted)
		/ progressInfo.cbPackageDownloadTotal)
	       * PROGRESS_MAX));
	}
      if (progressInfo.cbDownloadTotal > 0)
	{
	  sharedData.progress2Pos
	    = static_cast<int>(
	      ((static_cast<double>(progressInfo.cbDownloadCompleted)
		/ progressInfo.cbDownloadTotal)
	       * PROGRESS_MAX));
	}
      sharedData.secondsRemaining
	= static_cast<DWORD>(progressInfo.timeRemaining / 1000);
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
	  sharedData.progress1Pos
	    = static_cast<int>(
	      ((static_cast<double>(progressInfo.cbPackageInstallCompleted)
		/ progressInfo.cbPackageInstallTotal)
	       * PROGRESS_MAX));
	}
      sharedData.progress2Pos
	= static_cast<int>(
	  ((static_cast<double>(progressInfo.cbInstallCompleted)
	    / overallExpenditure)
	   * PROGRESS_MAX));
    }

  if (! PostMessage(WM_PROGRESS))
    {
      FATAL_WINDOWS_ERROR (T_("CWnd::PostMessage"), 0);
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
      theApp.startupConfig.installRoot = theApp.MiKTeXDirectTeXMFRoot;
    }

  bool comInit = false;

  try
    {
      // initialize COM
      if (FAILED(CoInitialize(0)))
	{
	  UNEXPECTED_CONDITION (T_("FileCopyPage::WorkerThread"));
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
	  FATAL_WINDOWS_ERROR (T_("CWnd::PostMessage"), 0);
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
  pInstaller->SetDestination (theApp.localPackageRepository);
  pInstaller->SetPackageLevel (theApp.packageLevel);
  pInstaller->SetCallback (this);

  // create the local repository directory
  Directory::Create (theApp.localPackageRepository);

  // remember local repository folder
  SessionWrapper(true)
    ->SetUserConfigValue (MIKTEX_REGKEY_PACKAGE_MANAGER,
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
      File::Copy (licenseFile,
		  PathName(theApp.localPackageRepository, LICENSE_FILE));
    }

  // now copy the setup program
  if (! theApp.setupPath.Empty())
    {
      MIKTEXCHAR szFileName[BufferSizes::MaxPath];
      MIKTEXCHAR szExt[BufferSizes::MaxPath];
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

  // create shell links
  CreateProgramIcons ();

  RegisterMiKTeXFileTypes ();

  // register path
  if (theApp.registerPath && ! IsPathRegistered())
    {
      if (IsWindowsNT())
	{
	  RegisterPathNT ();
	}
      else
	{
	  RegisterPath95 ();
	}
    }
}

/* _________________________________________________________________________

   FileCopyPage::DoTheInstallation
   _________________________________________________________________________ */

void
FileCopyPage::DoTheInstallation ()
{
  SessionWrapper(true)->SharedMiKTeXSetup (theApp.commonUserSetup, true);

  // register installation directory
  StartupConfig startupConfig;
  startupConfig.roots = theApp.startupConfig.installRoot.Get();
  startupConfig.installRoot = theApp.startupConfig.installRoot;
  SessionWrapper(true)->RegisterRootDirectories (startupConfig, true);

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
  pInstaller->SetDestination (theApp.startupConfig.installRoot);
  pInstaller->SetPackageLevel (theApp.packageLevel);
  pInstaller->SetCallback (this);

  // create the destination directory
  Directory::Create (theApp.startupConfig.installRoot);

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
  CreateProgramIcons ();

  RegisterMiKTeXFileTypes ();

  if (pSheet->GetCancelFlag())
    {
      return;
    }

  // register path
  if (theApp.registerPath && ! IsPathRegistered())
    {
      if (IsWindowsNT())
	{
	  RegisterPathNT ();
	}
      else
	{
	  RegisterPath95 ();
	}
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
  PathName initexmf (theApp.startupConfig.installRoot);
  initexmf += MIKTEX_PATH_BIN_DIR;
  initexmf += MIKTEX_INITEXMF_EXE;

  VersionNumber initexmfVer = GetFileVersion(initexmf);

  completedIniTeXMFRuns = 0;

  // refresh progress bar
  {
    CSingleLock (&criticalSectionMonitor, TRUE);
    CString str;
    VERIFY (str.LoadString(IDS_INITEXMF));
    sharedData.packageName = static_cast<const MIKTEXCHAR *>(str);
    sharedData.newPackage = true;
    sharedData.progress1Pos = 0;
    if (! PostMessage (WM_PROGRESS))
      {
	FATAL_WINDOWS_ERROR (T_("CWnd::PostMessage"), 0);
      }
  }

  CommandLineBuilder cmdLine;

  // define root directories
  // remove old fndb files
  // set shared setup option
  cmdLine.Clear ();
  cmdLine.AppendOption (T_("--install-root="),
			theApp.startupConfig.installRoot);
  if (! theApp.startupConfig.userDataRoot.Empty())
    {
      cmdLine.AppendOption (T_("--user-data="),
			    theApp.startupConfig.userDataRoot);
    }
  if (! theApp.startupConfig.userConfigRoot.Empty())
    {
      cmdLine.AppendOption (T_("--user-config="),
			    theApp.startupConfig.userConfigRoot);
    }
  if (! theApp.startupConfig.commonDataRoot.Empty())
    {
      cmdLine.AppendOption (T_("--common-data="),
			    theApp.startupConfig.commonDataRoot);
    }
  if (! theApp.startupConfig.commonConfigRoot.Empty())
    {
      cmdLine.AppendOption (T_("--common-config="),
			    theApp.startupConfig.commonConfigRoot);
    }
  tstring rootDirectories;
  rootDirectories += theApp.startupConfig.installRoot.Get();
  if (! theApp.noAddTEXMFDirs && ! theApp.startupConfig.roots.empty())
    {
      rootDirectories += T_(";");
      rootDirectories += theApp.startupConfig.roots.c_str();
    }
  cmdLine.AppendOption (T_("--roots="), rootDirectories);
  cmdLine.AppendOption (T_("--rmfndb"));
  cmdLine.AppendOption (T_("--shared-setup="),
			(theApp.commonUserSetup ? T_("1") : T_("0")));
  RunIniTeXMF (cmdLine);
  if (pSheet->GetCancelFlag())
    {
      return;
    }

  // create filename database files
  cmdLine.Clear ();
  cmdLine.AppendOption (T_("--update-fndb"));
  RunIniTeXMF (cmdLine);
  if (pSheet->GetCancelFlag())
    {
      return;
    }

  // create latex.exe, context.exe, ...
  RunIniTeXMF (T_("--mklinks"));
  if (pSheet->GetCancelFlag())
    {
      return;
    }

  // create font map files
  RunIniTeXMF (T_("--mkmaps"));
  if (pSheet->GetCancelFlag())
    {
      return;
    }

  // set paper size
  if (! theApp.paperSize.empty())
    {
      if (StringCompare(theApp.paperSize.c_str(), T_("a4"), true) == 0)
	{
	  RunIniTeXMF (T_("--default-paper-size=A4size"));
	}
      else
	{
	  RunIniTeXMF (T_("--default-paper-size=letterSize"));
	}
    }

  // refresh file name database again
  RunIniTeXMF (T_("--update-fndb"));
  if (pSheet->GetCancelFlag())
    {
      return;
    }

  // create report
  RunIniTeXMF (T_("--report"));
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
  exePath = theApp.startupConfig.installRoot;
  exePath += MIKTEX_PATH_BIN_DIR;
  exePath += MIKTEX_INITEXMF_EXE;

  // make command line
  CommandLineBuilder cmdLine (cmdLine1);
  cmdLine.AppendOption (T_("--log-file="), GetLogFileName());
  cmdLine.AppendOption (T_("--verbose"));

  // run initexmf.exe
  if (! theApp.dryRun)
    {
      Log (T_("%s %s:\n"), Q_(exePath.Get()), cmdLine.Get());
      ULogClose ();
      SessionWrapper(true)->UnloadFilenameDatabase ();
      Process::Run (exePath.Get(), cmdLine.Get(), this);
      ULogOpen ();
    }

  // refresh progress bars
  if (! pSheet->GetCancelFlag())
    {
      completedIniTeXMFRuns += 1;
      CSingleLock (&criticalSectionMonitor, TRUE);
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
      // calculate number of initexmf runs
      totalIniTeXMFRuns = 0;
      totalIniTeXMFRuns += 1;		// set root directories
      totalIniTeXMFRuns += 1;		// fndb install dir
      totalIniTeXMFRuns += 1;		// fndb data dir
      if (! theApp.noAddTEXMFDirs && ! theApp.startupConfig.roots.empty())
	{
	   // fndb additional dirs
	  totalIniTeXMFRuns +=
	    static_cast<int>(theApp.addTEXMFDirs.size());
	}
      totalIniTeXMFRuns += 1;		// fndb data dir (defrag)
      totalIniTeXMFRuns += 1;		// report
    }
}

/* _________________________________________________________________________

   FileCopyPage::Report
   _________________________________________________________________________ */

void
FileCopyPage::Report (/*[in]*/ bool			writeLog,
		      /*[in]*/ const MIKTEXCHAR *	lpszFmt,
		      /*[in]*/				...)
{
  MIKTEX_ASSERT (lpszFmt != 0);
  CString str;
  va_list args;
  va_start (args, lpszFmt);
  str.FormatV (lpszFmt, args);
  va_end (args);
  //int len = str.GetLength();
  CSingleLock (&criticalSectionMonitor, TRUE);
  if (writeLog)
    {
      Log (T_("%s"), static_cast<const MIKTEXCHAR *>(str));
    }
  SendMessage (WM_REPORT,
	       reinterpret_cast<WPARAM>(static_cast<const MIKTEXCHAR *>(str)));
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
      UNEXPECTED_CONDITION (T_("FileCopyPage::GetControl"));
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
  CHECK_WINDOWS_ERROR (T_("CWnd::EnableWindow"), 0);
}

/* _________________________________________________________________________

   FileCopyPage::RemoveObsoleteFiles
   _________________________________________________________________________ */

void
FileCopyPage::RemoveObsoleteFiles ()
{
  extern const MIKTEXCHAR * g_apszObsoleteFiles[];
  for (size_t i = 0; g_apszObsoleteFiles[i] != 0; ++ i)
    {
      PathName path (theApp.startupConfig.installRoot, g_apszObsoleteFiles[i]);
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
  const MIKTEXCHAR * lpszPackageSet;
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
  stream.WriteFormattedLine (T_("\
This folder contains the %s package set.\n\
\n\
For more information, visit the MiKTeX project page at\n\
http://www.miktex.org.\n\
 "),
			     lpszPackageSet);
  stream.Close ();
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

