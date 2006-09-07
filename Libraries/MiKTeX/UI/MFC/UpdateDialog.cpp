/* UpdateDialog.cpp:

   Copyright (C) 2000-2006 Christian Schenk

   This file is part of MiKTeX UI Library.

   MiKTeX UI Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   MiKTeX UI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX UI Library; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#include "StdAfx.h"
#include "internal.h"
#include "resource.h"

#include <miktex/UpdateDialog.h>

const SHORT PROGRESS_MAX = 1000;

/* _________________________________________________________________________

   UpdateDialogImpl
   _________________________________________________________________________ */

class UpdateDialogImpl
  : public CDialog,
    public IRunProcessCallback,
    public IPackageInstallerCallback
{
protected:
  DECLARE_MESSAGE_MAP();

public:
  UpdateDialogImpl (/*[in]*/ CWnd *		pParent,
		    /*[in]*/ PackageManager *	pManager);

public:
  virtual
  ~UpdateDialogImpl ();

public:
  void
  SetFileLists (/*[in]*/ const vector<tstring> & toBeInstalled,
		/*[in]*/ const vector<tstring> & toBeRemoved);

protected:
  virtual
  BOOL
  OnInitDialog ();

protected:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);

protected:
  virtual
  void
  OnCancel ();
 
protected:
  afx_msg
  LRESULT
  OnStartFileCopy (/*[in]*/ WPARAM wParam,
		   /*[in]*/ LPARAM lParam);

protected:
  afx_msg
  LRESULT
  OnProgress (/*[in]*/ WPARAM wParam,
	      /*[in]*/ LPARAM lParam);

protected:
  virtual
  bool
  MIKTEXCALL
  OnProcessOutput (/*[in]*/ const void *	pOutput,
		   /*[in]*/ size_t		n);
  
public:
  virtual
  void
  MPMCALL
  ReportLine (/*[in]*/ const MIKTEXCHAR * lpszLine);

public:
  virtual
  bool
  MPMCALL
  OnRetryableError (/*[in]*/ const MIKTEXCHAR * lpszMessage);

public:
  virtual
  bool
  MPMCALL
  OnProgress (/*[in]*/ Notification	nf);

private:
  static
  UINT
  WorkerThread (/*[in]*/ void * pParam);

private:
  void
  DoTheUpdate ();

private:
  void
  RunIniTeXMF ();

private:
  void
  Report (/*[in]*/ bool			immediate,
	  /*[in]*/ const MIKTEXCHAR *	lpszFmt,
	  /*[in]*/			...);

private:
  void
  ReportError (/*[in]*/ const MiKTeXException & e)
  {
    errorOccured = true;
    ErrorDialog::DoModal (this, e);
  }

private:
  void
  ReportError (/*[in]*/ const exception & e)
  {
    errorOccured = true;
    ErrorDialog::DoModal (this, e);
  }

private:
  CWnd *
  GetControl (/*[in]*/ UINT	controlId);

private:
  void
  EnableControl (/*[in]*/ UINT	controlId,
		 /*[in]*/ bool	enable);

private:
  void
  FormatControlText (/*[in]*/ UINT			ctrlId,
		     /*[in]*/ const MIKTEXCHAR *	lpszFormat,
		     /*[in]*/				...);

private:
  void
  SetCancelFlag ()
  {
    EnableControl (IDCANCEL, false);
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
  bool
  GetErrorFlag ()
    const
  {
    return (errorOccured);
  }

private:
  CCriticalSection criticalSectionMonitor;

private:
  struct SharedData
  {
    SharedData ()
      : ready (false),
	progress1Pos (0),
	progress2Pos (0),
	newPackage (false),
	reportUpdate (false),
	secondsRemaining (0),
	waitingForClickOnClose (false)
    {
    }
    PackageInstaller::ProgressInfo progressInfo;
    CString report;
    DWORD secondsRemaining;
    bool newPackage;
    bool ready;
    bool reportUpdate;
    bool waitingForClickOnClose;
    int progress1Pos;
    int progress2Pos;
    tstring packageName;
  };

private:
  SharedData sharedData;

private:
  auto_ptr<PackageInstaller> pInstaller;

private:
  HANDLE hWorkerThread;
  
private:
  CAnimateCtrl animationControl;

private:
  CEdit reportEditBox;

private:
  CProgressCtrl progressControl1;

private:
  CProgressCtrl progressControl2;

private:
  CString report;

private:
  enum { WM_STARTFILECOPY = WM_APP + 1, WM_PROGRESS };

private:
  PackageManagerPtr pManager;

private:
  bool errorOccured;

private:
  bool cancelled;
};

/* _________________________________________________________________________

   UpdateDialogImpl Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(UpdateDialogImpl, CDialog)
  ON_MESSAGE(WM_PROGRESS, OnProgress)
  ON_MESSAGE(WM_STARTFILECOPY, OnStartFileCopy)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   UpdateDialogImpl::UpdateDialogImpl
   _________________________________________________________________________ */

enum { IDD = IDD_MIKTEX_UPDATE };

UpdateDialogImpl::UpdateDialogImpl (/*[in]*/ CWnd *		pParent,
				    /*[in]*/ PackageManager *	pManager)
  : CDialog (IDD, pParent),
    cancelled (false),
    errorOccured (false),
    hWorkerThread (0),
    pInstaller (pManager->CreateInstaller()),
    pManager (pManager)
{
}

/* _________________________________________________________________________

   UpdateDialogImpl::~UpdateDialogImpl
   _________________________________________________________________________ */

UpdateDialogImpl::~UpdateDialogImpl ()
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

   UpdateDialogImpl::OnInitDialog
   _________________________________________________________________________ */

BOOL
UpdateDialogImpl::OnInitDialog ()
{
  BOOL ret = CDialog::OnInitDialog();
  try
    {
      reportEditBox.LimitText (100000);
      if (! PostMessage(WM_STARTFILECOPY))
	{
	  FATAL_WINDOWS_ERROR (T_("CWnd::PostMessage"), 0);
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
  return (ret);
}


/* _________________________________________________________________________

   UpdateDialogImpl::DoDataExchange
   _________________________________________________________________________ */

void
UpdateDialogImpl::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_ANI, animationControl);
  DDX_Control (pDX, IDC_PROGRESS1, progressControl1);
  DDX_Control (pDX, IDC_PROGRESS2, progressControl2);
  DDX_Control (pDX, IDC_REPORT, reportEditBox);
  DDX_Text (pDX, IDC_REPORT, report);
}

/* _________________________________________________________________________

   UpdateDialogImpl::OnCancel
   _________________________________________________________________________ */

void
UpdateDialogImpl::OnCancel ()
{
  if (hWorkerThread == 0 || sharedData.waitingForClickOnClose)
    {
      CDialog::OnCancel ();
      return;
    }
  try
    {
      if (AfxMessageBox(IDS_CANCEL_UPDATE,
			MB_OKCANCEL | MB_ICONEXCLAMATION)
	  == IDOK)
	{
	  SetCancelFlag ();
	  if (! PostMessage(WM_PROGRESS))
	    {
	      FATAL_WINDOWS_ERROR (T_("CWnd::PostMessage"), 0);
	    }
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
}

/* _________________________________________________________________________

   UpdateDialogImpl::OnStartFileCopy
   _________________________________________________________________________ */

LRESULT
UpdateDialogImpl::OnStartFileCopy (/*[in]*/ WPARAM	wParam,
			       /*[in]*/ LPARAM	lParam)
{
  UNUSED_ALWAYS (wParam);
  UNUSED_ALWAYS (lParam);

  try
    {
      if (! animationControl.Open(IDA_DOWNLOAD))
	{
	  FATAL_WINDOWS_ERROR (T_("CAnimateCtrl::Open"), 0);
	}

      if (! animationControl.Play(0,
				  static_cast<unsigned int>(-1),
				  static_cast<unsigned int>(-1)))
	{
	  FATAL_WINDOWS_ERROR (T_("CAnimateCtrl::Play"), 0);
	}

      // initialize progress bar controls
      progressControl1.SetRange (0, 1000);
      progressControl1.SetPos (0);
      progressControl2.SetRange (0, 1000);
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

   UpdateDialogImpl::OnProgress
   _________________________________________________________________________ */

LRESULT
UpdateDialogImpl::OnProgress (/*[in]*/ WPARAM	wParam,
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
	  || GetCancelFlag()
	  || GetErrorFlag())
	{
	  // check to see if we are already ready
	  if (sharedData.waitingForClickOnClose)
	    {
	      return (0);
	    }

	  sharedData.waitingForClickOnClose = true;

	  // stop the video
	  if (! animationControl.Stop())
	    {
	      FATAL_WINDOWS_ERROR (T_("CAnimateCtrl::Stop"), 0);
	    }
	  animationControl.Close (); // always returns FALSE
	  animationControl.ShowWindow (SW_HIDE);

	  // disable controls
	  EnableControl (IDC_PROGRESS1_TITLE, false);
	  FormatControlText (IDC_PACKAGE, T_(""));
	  EnableControl (IDC_PACKAGE, false);
	  progressControl1.SetPos (0);
	  progressControl1.EnableWindow (FALSE);
	  CHECK_WINDOWS_ERROR (T_("CWnd::EnableWindow"), 0);
	  EnableControl (IDC_PROGRESS2_TITLE, false);
	  progressControl2.SetPos (0);
	  progressControl2.EnableWindow (FALSE);
	  CHECK_WINDOWS_ERROR (T_("CWnd::EnableWindow"), 0);
	  FormatControlText (IDCANCEL, T_("%s"), T_("Close"));
	  EnableControl (IDCANCEL, true);
	}
      else
	{
	  // show the package name
	  if (sharedData.newPackage)
	    {
	      FormatControlText (IDC_PACKAGE,
				 T_("%s"),
				 sharedData.packageName.c_str());
	      sharedData.newPackage = false;
	    }

	  // update progress bars
	  progressControl1.SetPos (sharedData.progress1Pos);
	  progressControl2.SetPos (sharedData.progress2Pos);

	  // update "Removed files (packages)"
	  FormatControlText (IDC_REMOVED_FILES,
			     T_("%u (%u)"),
			     sharedData.progressInfo.cFilesRemoveCompleted,
			     sharedData.progressInfo.cPackagesRemoveCompleted);
	  
	  // update "New files (packages)"
	  FormatControlText
	    (IDC_NEW_FILES,
	     T_("%u (%u)"),
	     sharedData.progressInfo.cFilesInstallCompleted,
	     sharedData.progressInfo.cPackagesInstallCompleted);
	  
	  // update "Downloaded bytes"
	  FormatControlText (IDC_DOWNLOADED_BYTES,
			     T_("%u"),
			     sharedData.progressInfo.cbDownloadCompleted);
	  
	  // update "Package"
	  FormatControlText (IDC_PACKAGE,
			     T_("%s"),
			     sharedData.progressInfo.displayName.c_str());
	  
	  // update "KB/s"
	  FormatControlText
	    (IDC_KB_SEC,
	     T_("%.2f"),
	     (static_cast<double>(sharedData.progressInfo.bytesPerSecond)
	      / 1024.0));
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

  return (0);
}

/* _________________________________________________________________________

   UpdateDialogImpl::OnProcessOutput
   _________________________________________________________________________ */

bool
MIKTEXCALL
UpdateDialogImpl::OnProcessOutput (/*[in]*/ const void *	pOutput,
				   /*[in]*/ size_t		n)
{
  Report (true,
	  T_("%.*s"),
	  static_cast<int>(n),
	  reinterpret_cast<const char *>(pOutput));
  return (! (GetErrorFlag() || GetCancelFlag()));
}

/* _________________________________________________________________________

   UpdateDialogImpl::ReportLine
   _________________________________________________________________________ */

void
MPMCALL
UpdateDialogImpl::ReportLine (/*[in]*/ const MIKTEXCHAR * lpszLine)
{
  Report (true, T_("%s\n"), lpszLine);
}

/* _________________________________________________________________________

   UpdateDialogImpl::OnRetryableError
   _________________________________________________________________________ */

bool
MPMCALL
UpdateDialogImpl::OnRetryableError (/*[in]*/ const MIKTEXCHAR * lpszMessage)
{
  UNUSED_ALWAYS (lpszMessage);
  return (false);
}

/* _________________________________________________________________________

   UpdateDialogImpl::OnProgress
   _________________________________________________________________________ */

bool
MPMCALL
UpdateDialogImpl::OnProgress (/*[in]*/ Notification	nf)
{
  CSingleLock (&criticalSectionMonitor, TRUE);
  PackageInstaller::ProgressInfo progressInfo = pInstaller->GetProgressInfo();
  sharedData.progressInfo = progressInfo;
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
  return (! (GetErrorFlag() || GetCancelFlag()));
}

/* _________________________________________________________________________

   UpdateDialogImpl::WorkerThread
   _________________________________________________________________________ */

UINT
UpdateDialogImpl::WorkerThread (/*[in]*/ void * pParam)
{
  UpdateDialogImpl * This = reinterpret_cast<UpdateDialogImpl*>(pParam);

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

   UpdateDialogImpl::DoTheUpdate
   _________________________________________________________________________ */

void
UpdateDialogImpl::DoTheUpdate ()
{
  pInstaller->SetCallback (this);
  pInstaller->InstallRemove ();
  if (GetCancelFlag())
    {
      return;
    }
  RunIniTeXMF ();
}

/* _________________________________________________________________________

   UpdateDialogImpl::RunIniTeXMF
   _________________________________________________________________________ */

void
UpdateDialogImpl::RunIniTeXMF ()
{
  PathName exePath;

  if (! SessionWrapper(true)->FindFile(MIKTEX_INITEXMF_EXE,
				       FileType::EXE,
				       exePath))
    {
      FATAL_MIKTEX_ERROR (T_("UpdateDialogImpl::RunIniTeXMF"),
			  T_("\
The MiKTeX configuration utility could not be found."),
			  0);
    }

  CommandLineBuilder commandLine;
  commandLine.AppendOption (T_("--mkmaps"));

  SessionWrapper(true)->UnloadFilenameDatabase ();

  Process::Run (exePath, commandLine.Get(), this);
}

/* _________________________________________________________________________

   UpdateDialogImpl::Report
   _________________________________________________________________________ */

void
UpdateDialogImpl::Report (/*[in]*/ bool			immediate,
			  /*[in]*/ const MIKTEXCHAR *	lpszFmt,
			  /*[in]*/			...)
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
  if (immediate)
    {
      if (! PostMessage(WM_PROGRESS))
	{
	  FATAL_WINDOWS_ERROR (T_("CWnd::PostMessage"), 0);
	}
    }
}

/* _________________________________________________________________________

   UpdateDialogImpl::GetControl
   _________________________________________________________________________ */

CWnd *
UpdateDialogImpl::GetControl (/*[in]*/ UINT	controlId)
{
  CWnd * pWnd = GetDlgItem(controlId);
  if (pWnd == 0)
    {
      UNEXPECTED_CONDITION (T_("UpdateDialogImpl::GetControl"));
    }
  return (pWnd);
}

/* _________________________________________________________________________

   UpdateDialogImpl::EnableControl
   _________________________________________________________________________ */

void
UpdateDialogImpl::EnableControl (/*[in]*/ UINT	controlId,
				 /*[in]*/ bool	enable)
{
  GetControl(controlId)->EnableWindow (enable ? TRUE : FALSE);
  CHECK_WINDOWS_ERROR (T_("CWnd::EnableWindow"), 0);
}

/* _________________________________________________________________________

   UpdateDialogImpl::FormatControlText
   _________________________________________________________________________ */

void
UpdateDialogImpl::FormatControlText (/*[in]*/ UINT		ctrlId,
				     /*[in]*/ const MIKTEXCHAR * lpszFormat,
				     /*[in]*/			...)
{
  va_list marker;
  va_start (marker, lpszFormat);
  tstring str = Utils::FormatString(lpszFormat, marker);
  va_end (marker);
  GetControl(ctrlId)->SetWindowText (str.c_str());
}

/* _________________________________________________________________________

   UpdateDialogImpl::SetFileLists
   _________________________________________________________________________ */

void
UpdateDialogImpl::SetFileLists (/*[in]*/ const vector<tstring> & toBeInstalled,
				/*[in]*/ const vector<tstring> & toBeRemoved)
{
  pInstaller->SetFileLists (toBeInstalled, toBeRemoved);
}

/* _________________________________________________________________________

   UpdateDialog::DoModal
   _________________________________________________________________________ */

INT_PTR
MIKTEXUICALL
UpdateDialog::DoModal (/*[in]*/ CWnd *			pParent,
		       /*[in]*/ PackageManager *	pManager,
		       /*[in]*/ const vector<tstring> &	toBeInstalled,
		       /*[in]*/ const vector<tstring> &	toBeRemoved)
{
  tstring url;
  RepositoryType repositoryType (RepositoryType::Unknown);
  if (toBeInstalled.size() > 0
      && PackageManager::TryGetDefaultPackageRepository(repositoryType, url)
      && repositoryType == RepositoryType::Remote
      && ! MiKTeX::UI::ProxyAuthenticationDialog((pParent == 0
						  ? 0
						  : pParent->GetSafeHwnd())))
    {
      return (IDCANCEL);
    }
  UpdateDialogImpl dlg (pParent, pManager);
  dlg.SetFileLists (toBeInstalled, toBeRemoved);
  return (dlg.DoModal());
}
