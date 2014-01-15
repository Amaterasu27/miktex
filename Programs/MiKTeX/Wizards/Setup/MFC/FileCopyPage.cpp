/* FileCopyPage.cpp: the actual setup process

   Copyright (C) 1999-2014 Christian Schenk

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
      SessionWrapper(true)->SetAdminMode (theApp.IsCommonSetup());
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
	  if (theApp.GetSetupTask() != SetupTask::Download)
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
	theApp.pSetupService->ULogClose(! pSheet->GetErrorFlag());
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
      theApp.pSetupService->Log(T_("\n<<<Cancel? "));
      if (AfxMessageBox((theApp.GetSetupTask() == SetupTask::Download
			 ? IDS_CANCEL_DOWNLOAD
			 : IDS_CANCEL_SETUP),
			MB_OKCANCEL | MB_ICONEXCLAMATION)
	  == IDOK)
	{
	  theApp.pSetupService->Log(T_("Yes!>>>\n"));
	  pSheet->SetCancelFlag ();
	  if (! PostMessage(WM_PROGRESS))
	    {
	      FATAL_WINDOWS_ERROR ("CWnd::PostMessage", 0);
	    }
	}
      else
	{
	  theApp.pSetupService->Log(T_("No!>>>\n"));
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
      if (! animationControl.Open(theApp.GetSetupTask() == SetupTask::Download
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

      if (theApp.GetSetupTask() == SetupTask::Download)
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
	      theApp.pSetupService->ULogClose(! pSheet->GetErrorFlag());
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
FileCopyPage::OnProgress (/*[in]*/ MiKTeX::Setup::Notification		nf)
{
  CSingleLock singlelock (&criticalSectionMonitor, TRUE);

  bool visibleProgress = false;

  SetupService::ProgressInfo progressInfo = theApp.pSetupService->GetProgressInfo();

  if (nf == MiKTeX::Setup::Notification::InstallPackageStart
      || nf == MiKTeX::Setup::Notification::DownloadPackageStart)
    {
      visibleProgress = true;
      sharedData.newPackage = true;
      sharedData.packageName = progressInfo.displayName;
    }

  if (theApp.GetSetupTask() == SetupTask::Download)
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
  else if ((theApp.GetSetupTask() == SetupTask::InstallFromLocalRepository
	    || theApp.GetSetupTask() == SetupTask::InstallFromCD)
	   && progressInfo.cbInstallTotal > 0)
    {
      totalSize = static_cast<DWORD>(progressInfo.cbInstallTotal);

      // calculate initexmf contribution
      size_t uSizeExtra = 0;

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

  bool comInit = false;

  try
    {
      // initialize COM
      if (FAILED(CoInitialize(0)))
	{
	  UNEXPECTED_CONDITION ("FileCopyPage::WorkerThread");
	}
      comInit = true;

      theApp.pSetupService->SetCallback(This);
      theApp.pSetupService->Run();
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
    theApp.pSetupService->Log("%s", str.c_str());
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

