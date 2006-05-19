/* FileCopyPage.cpp: the actual update process

   Copyright (C) 2002-2006 Christian Schenk

   This file is part of MiKTeX Update Wizard.

   MiKTeX Update Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   MiKTeX Update Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX Update Wizard; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"
#include "Update.h"

#include "FileCopyPage.h"
#include "Update-version.h"
#include "UpdateWizard.h"

IMPLEMENT_DYNCREATE(FileCopyPage, CPropertyPage);

const SHORT PROGRESS_MAX = 1000;

const UINT WM_STARTFILECOPY = WM_APP + 1;
const UINT WM_PROGRESS = WM_APP + 2;

/* _________________________________________________________________________

   FileCopyPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(FileCopyPage, CPropertyPage)
  ON_MESSAGE(WM_PROGRESS, OnProgress)
  ON_MESSAGE(WM_STARTFILECOPY, OnStartFileCopy)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   FileCopyPage::FileCopyPage
   _________________________________________________________________________ */

FileCopyPage::FileCopyPage ()
  : CPropertyPage (IDD, 0, IDS_HEADER_UPDATE, IDS_SUBHEADER_UPDATE),
    pSheet (0),
#if CHECK_CONFIG_FILES
    pMiKTeXIniOld (Cfg::Create()),
    pMiKTeXIniNew (Cfg::Create()),
    pFormatsIniOld (Cfg::Create()),
    pFormatsIniNew (Cfg::Create()),
#endif
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
      if (sharedData.pLogStream.get() != 0)
	{
	  sharedData.pLogStream->Close ();
	  sharedData.pLogStream.reset ();
	}
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
  pSheet = reinterpret_cast<UpdateWizard*>(GetParent());
  BOOL ret = CPropertyPage::OnInitDialog();
  try
    {
      pInstaller = auto_ptr<PackageInstaller>(g_pManager->CreateInstaller());
      reportEditBox.LimitText (100000);
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
	  pInstaller->SetRepository (pSheet->GetRepository());
	  
	  // disable buttons
	  pSheet->SetWizardButtons (0);
	  
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

  return (ret);
}

/* _________________________________________________________________________

   FileCopyPage::DoDataExchange
   _________________________________________________________________________ */

void
FileCopyPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_ANI, animationControl);
  DDX_Control (pDX, IDC_PROGRESS1, progressControl1);
  DDX_Control (pDX, IDC_PROGRESS2, progressControl2);
  DDX_Control (pDX, IDC_REPORT, reportEditBox);
  DDX_Text (pDX, IDC_REPORT, report);
}

/* _________________________________________________________________________

   FileCopyPage::OnWizardNext
   _________________________________________________________________________ */

LRESULT
FileCopyPage::OnWizardNext ()
{
  pSheet->SetCameFrom (0);
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
	  CSingleLock (&criticalSectionMonitor, TRUE);
	  if (sharedData.pLogStream.get() != 0)
	    {
	      sharedData.pLogStream->Close ();
	      sharedData.pLogStream.reset ();
	    }
	  ULogClose (false);
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
      if (AfxMessageBox(IDS_CANCEL_UPDATE,
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
      if (! animationControl.Open(IDA_UPDATE))
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

      // update the report
      if (sharedData.reportUpdate)
	{
	  reportEditBox.SetWindowText (sharedData.report);
	  reportEditBox.SetSel (100000, 100000);
	  sharedData.reportUpdate = false;
	}

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

   FileCopyPage::OnProcessOutput
   _________________________________________________________________________ */

bool
MIKTEXCALL
FileCopyPage::OnProcessOutput (/*[in]*/ const void *	pOutput,
			       /*[in]*/ size_t		n)
{
  Report (true,
	  true,
	  T_("%.*s"),
	  static_cast<int>(n),
	  reinterpret_cast<const char *>(pOutput));
  return (! (pSheet->GetErrorFlag() || pSheet->GetCancelFlag()));
}

/* _________________________________________________________________________

   FileCopyPage::ReportLine
   _________________________________________________________________________ */

void
MPMCALL
FileCopyPage::ReportLine (/*[in]*/ const MIKTEXCHAR * lpszLine)
{
  Report (true, true, T_("%s\n"), lpszLine);
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
  if (progressInfo.cbPackageDownloadTotal > 0)
    {
      sharedData.progress1Pos
	= static_cast<int>
	(((static_cast<double>(progressInfo.cbPackageDownloadCompleted)
	   / progressInfo.cbPackageDownloadTotal)
	  * PROGRESS_MAX));
    }
  if (progressInfo.cbDownloadTotal > 0)
    {
      sharedData.progress2Pos
	= static_cast<int>
	(((static_cast<double>(progressInfo.cbDownloadCompleted)
	   / progressInfo.cbDownloadTotal)
	  * PROGRESS_MAX));
    }
  sharedData.secondsRemaining
    = static_cast<DWORD>(progressInfo.timeRemaining / 1000);
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

  try
    {
      This->DoTheUpdate ();
    }
  catch (const MiKTeXException & e)
    {
      This->ReportError (e);
    }
  catch (const exception & e)
    {
      This->ReportError (e);
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

   FileCopyPage::DoTheUpdate
   _________________________________________________________________________ */

void
FileCopyPage::DoTheUpdate ()
{
  pInstaller->SetCallback (this);

  pInstaller->SetFileLists (pSheet->GetUpdateList(), vector<tstring>());

#if CHECK_CONFIG_FILES
  pMiKTeXIniOld->Read (PathName(GetMainConfigDir(), MIKTEX_INI_FILENAME));
  pFormatsIniOld->Read (PathName(GetMainConfigDir(),
				 MIKTEX_FORMATS_INI_FILENAME));
#endif

  // open the log file
  OpenLog ();

  // open the uninstall script
  ULogOpen (true);
  ULogAddFile (g_logFileName);

  // update package database
  pInstaller->UpdateDb ();

  if (pSheet->GetCancelFlag())
    {
      return;
    }

  // run installer
  pInstaller->InstallRemove ();

  if (pSheet->GetCancelFlag())
    {
      return;
    }

  // run IniTeXMF
  ConfigureMiKTeX ();
}

/* _________________________________________________________________________

   FileCopyPage::ConfigureMiKTeX
   _________________________________________________________________________ */

void
FileCopyPage::ConfigureMiKTeX ()
{
#if CHECK_CONFIG_FILES
  pMiKTeXIniNew->Read (PathName(GetMainConfigDir(), MIKTEX_INI_FILENAME));
  pFormatsIniNew->Read (PathName(GetMainConfigDir(),
				 MIKTEX_FORMATS_INI_FILENAME));

  BYTE digest1[16];
  BYTE digest2[16];

  PathName pathMiKTeXIniLocal (GetLocalConfigDir(), MIKTEX_INI_FILENAME);
  if (File::Exists(pathMiKTeXIniLocal)
      && (memcmp(pMiKTeXIniNew->GetMD5(digest1),
		 pMiKTeXIniOld->GetMD5(digest2),
		 16)
	  != 0))
    {
      CString strMessage;
      AfxFormatString1 (strMessage,
			IDP_RENAME_LOCAL_INI,
			MIKTEX_INI_FILENAME);
      if (AfxMessageBox(strMessage, MB_OKCANCEL | MB_ICONINFORMATION) == IDOK)
	{
	  File::PushAside (pathMiKTeXIniLocal, T_(".BAK"), false);
	}
      else
	{
	  Log (T_("user wants to keep the local %s\n"), MIKTEX_INI_FILENAME);
	}
    }

  PathName pathFormatsIniLocal (GetLocalConfigDir(),
				MIKTEX_FORMATS_INI_FILENAME);
  if (File::Exists(pathFormatsIniLocal)
      && (memcmp(pFormatsIniNew->GetMD5(digest1),
		 pFormatsIniOld->GetMD5(digest2),
		 16)
	  != 0))
    {
      CString strMessage;
      AfxFormatString1 (strMessage,
			IDP_RENAME_LOCAL_INI,
			MIKTEX_FORMATS_INI_FILENAME);
      if (AfxMessageBox(strMessage, MB_OKCANCEL | MB_ICONINFORMATION) == IDOK)
	{
	  File::PushAside (pathFormatsIniLocal, T_(".BAK"), false);
	}
      else
	{
	  Log (T_("user wants to keep the local %s\n"),
	       MIKTEX_FORMATS_INI_FILENAME);
	}
    }
#endif

#if REMOVE_FORMAT_FILES
  RemoveFormatFiles ();
#endif

  CommandLineBuilder cmdLine;

  cmdLine.AppendOption (T_("--update-fndb"));
  cmdLine.AppendOption (T_("--mklinks"));
  cmdLine.AppendOption (T_("--mkmaps"));

  RunIniTeXMF (cmdLine);
}

/* _________________________________________________________________________

   FileCopyPage::RunIniTeXMF
   _________________________________________________________________________ */

void
FileCopyPage::RunIniTeXMF (/*[in]*/ const CommandLineBuilder & cmdLine1)
{
  PathName exePath;

  if (! SessionWrapper(true)->FindFile(MIKTEX_INITEXMF_EXE,
				       FileType::EXE,
				       exePath))
    {
      FATAL_MIKTEX_ERROR (T_("FileCopyPage::RunIniTeXMF"),
			  T_("\
The MiKTeX configuration utility could not be found."),
			  0);
    }

  // make command line
  CommandLineBuilder cmdLine (cmdLine1);
#if 0				// <todo/>
  cmdLine.AppendOption (T_(" --log-file="),
			GetLogFileName(szLogPath));
#endif
  cmdLine.AppendOption (T_(" --verbose"));

  // run initexmf.exe
  Log (T_("initexmf %s:\n"), cmdLine.Get());
#if 0				// <todo/>
  ULogClose ();
#endif

  SessionWrapper(true)->UnloadFilenameDatabase ();

  Process::Run (exePath, cmdLine.Get(), this);
#if 0				// <todo/>
  ULogOpen ();
#endif
}

/* _________________________________________________________________________

   FileCopyPage::OpenLog
   _________________________________________________________________________ */

void
FileCopyPage::OpenLog ()
{
  CSingleLock singleLock (&criticalSectionMonitor, TRUE);

  // return if the log is already open (<fixme>can this happen?</fixme>)
  if (sharedData.pLogStream.get() != 0)
    {
      return;
    }

  // make the log path name, e.g.:
  // C:\texmf\miktex\config\update-2005-11-04-08-50.log
  CTime t = CTime::GetCurrentTime();
  PathName pathLog (GetMainConfigDir(),
		    static_cast<const MIKTEXCHAR *>
		    (t.Format(T_("update-%Y-%m-%d-%H-%M"))),
		    T_(".log"));

  // open the log file
  sharedData.pLogStream = auto_ptr<StreamWriter>(new StreamWriter(pathLog));

  // remember the log file name
  g_logFileName = pathLog;

  // log general info
  Log (T_("MiKTeX Update Wizard Report\n\n"));
  Log (T_("Version: %s\n"), VER_FILEVERSION_STR);
  Log (T_("Date: %s\n"), t.Format(T_("%A, %B %d, %Y")));
  Log (T_("Time: %s\n"), t.Format(T_("%H:%M:%S")));
}

/* _________________________________________________________________________

   FileCopyPage::Log
   _________________________________________________________________________ */

void
FileCopyPage::Log (/*[in]*/ const MIKTEXCHAR *	lpszFormat,
		   /*[in]*/			...)
{
  CSingleLock singleLock (&criticalSectionMonitor, TRUE);
  if (sharedData.pLogStream.get() != 0)
    {
      va_list args;
      va_start (args, lpszFormat);
      sharedData.pLogStream->Write (Utils::FormatString(lpszFormat, args));
      va_end (args);
    }
}

/* _________________________________________________________________________

   FileCopyPage::Report
   _________________________________________________________________________ */

void
FileCopyPage::Report (/*[in]*/ bool			writeLog,
		      /*[in]*/ bool			immediate,
		      /*[in]*/ const MIKTEXCHAR *	lpszFmt,
		      /*[in]*/				...)
{
  MIKTEX_ASSERT (lpszFmt != 0);
  CString str;
  va_list args;
  va_start (args, lpszFmt);
  str.FormatV (lpszFmt, args);
  va_end (args);
  int len = str.GetLength();
  CSingleLock (&criticalSectionMonitor, TRUE);
  for (int i = 0; i < len; ++ i)
    {
      if (str[i] == T_('\n') && i > 0 && sharedData.report[i - 1] != T_('\r'))
	{
	  sharedData.report += T_('\r');
	}
      sharedData.report += str[i];
    }
  sharedData.reportUpdate = true;
  if (writeLog)
    {
      Log (T_("%s"), static_cast<const MIKTEXCHAR *>(str));
    }
  if (immediate)
    {
      if (! PostMessage(WM_PROGRESS))
	{
	  FATAL_WINDOWS_ERROR (T_("CWnd::PostMessage"), 0);
	}
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

   FileCopyPage::CollectFiles
   _________________________________________________________________________ */

void
FileCopyPage::CollectFiles (/*[in,out]*/ vector<tstring> &	vec,
			    /*[in]*/ const PathName &		dir,
			    /*[in]*/ const MIKTEXCHAR *		lpszExt)
{
  auto_ptr<DirectoryLister> pLister (DirectoryLister::Open(dir, lpszExt));
  DirectoryEntry entry;
  while (pLister->GetNext(entry))
    {
      vec.push_back (entry.name);
    }
  pLister->Close ();
  pLister.reset ();
}

/* _________________________________________________________________________

   FileCopyPage::RemoveFormatFiles
   _________________________________________________________________________ */

#if REMOVE_FORMAT_FILES
void
FileCopyPage::RemoveFormatFiles ()
{
  PathName pathFmt
    (SessionWrapper(true)->GetSpecialPath(SpecialPath::DataRoot));
  pathFmt += MIKTEX_PATH_FMT_DIR;
  if (! Directory::Exists(pathFmt))
    {
      return;
    }
  vector<tstring> toBeDeleted;
  CollectFiles (toBeDeleted, pathFmt, MIKTEX_FORMAT_FILE_SUFFIX);
  for (vector<tstring>::const_iterator it = toBeDeleted.begin();
       it != toBeDeleted.end();
       ++ it)
    {
      File::Delete (PathName(pathFmt, *it));
    }
}
#endif

/* _________________________________________________________________________

   FileCopyPage::ReportError
   _________________________________________________________________________ */

void
FileCopyPage::ReportError (/*[in]*/ const MiKTeXException & e)
{
  Report (true, true, T_("\nError: %s\n"), e.what());
  pSheet->ReportError (e);
}

/* _________________________________________________________________________

   FileCopyPage::ReportError
   _________________________________________________________________________ */

void
FileCopyPage::ReportError (/*[in]*/ const exception & e)
{
  Report (true, true, T_("\nError: %s\n"), e.what());
  pSheet->ReportError (e);
}

