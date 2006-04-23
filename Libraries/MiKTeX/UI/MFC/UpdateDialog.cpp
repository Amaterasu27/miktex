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

/* _________________________________________________________________________

   UpdateDialogImpl
   _________________________________________________________________________ */

class UpdateDialogImpl
  : public CDialog,
    public IRunProcessCallback,
    public IPackageInstallerCallback
{
public:
  UpdateDialogImpl (/*[in]*/ CWnd *		pParent,
		    /*[in]*/ PackageManager *	pManager);

private:
  virtual
  BOOL
  OnInitDialog ();

private:
  virtual
  void
  OnCancel ();

private:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);

private:
  afx_msg
  LRESULT
  OnProgress (/*[in]*/ WPARAM wParam,
	      /*[in]*/ LPARAM lParam);

public:
  void
  SetFileLists (/*[in]*/ const vector<tstring> & toBeInstalled,
		/*[in]*/ const vector<tstring> & toBeRemoved);

private:
  void
  RunIniTeXMF ();


private:
  virtual
  bool
  MIKTEXCALL
  OnProcessOutput (/*[in]*/ const void *	pOutput,
		   /*[in]*/ size_t		n);
    
public:
  virtual
  void
  MIKTEXUICALL
  ReportLine (/*[in]*/ const MIKTEXCHAR * lpszLine);

public:
  virtual
  bool
  MIKTEXUICALL
  OnRetryableError (/*[in]*/ const MIKTEXCHAR * lpszMessage);

public:
  virtual
  bool
  MIKTEXUICALL
  OnProgress (/*[in]*/ Notification	nf);

private:
  void
  FormatControlText (/*[in]*/ UINT			ctrlId,
		     /*[in]*/ const MIKTEXCHAR *	lpszFormat,
		     /*[in]*/				...);

private:
  CAnimateCtrl downloadAnimationControl;

private:
  CEdit reportBox;

private:
  CProgressCtrl packageProgressControl;

private:
  CProgressCtrl totalProgressControl;

private:
  CString report;

private:
  enum { WM_ONPROGRESS = WM_APP + 1 };

private:
  DECLARE_MESSAGE_MAP();

private:
  bool doContinue;

private:
  PackageManagerPtr pManager;

private:
  auto_ptr<PackageInstaller> pInstaller;

private:
  MIKTEX_DEFINE_LOCK(this);

private:
  bool cancelButtonIsCloseButton;

private:
  bool errorOccured;

private:
  bool reportUpdated;
};

/* _________________________________________________________________________

   UpdateDialogImpl::UpdateDialogImpl
   _________________________________________________________________________ */

enum { IDD = IDD_MIKTEX_UPDATE };

UpdateDialogImpl::UpdateDialogImpl (/*[in]*/ CWnd *		pParent,
				    /*[in]*/ PackageManager *	pManager)
  : CDialog (IDD, pParent),
    cancelButtonIsCloseButton (false),
    doContinue (true),
    errorOccured (false),
    pInstaller (pManager->CreateInstaller()),
    pManager (pManager),
    reportUpdated (false)
{
  pInstaller->SetCallback (this);
}

/* _________________________________________________________________________

   UpdateDialogImpl::DoDataExchange
   _________________________________________________________________________ */

void
UpdateDialogImpl::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);
  DDX_Control (pDX, IDC_ANI_DOWNLOAD, downloadAnimationControl);
  DDX_Control (pDX, IDC_PROGRESS_PACKAGE, packageProgressControl);
  DDX_Control (pDX, IDC_PROGRESS_TOTAL, totalProgressControl);
  DDX_Control (pDX, IDC_REPORT, reportBox);
  DDX_Text (pDX, IDC_REPORT, report);
}

/* _________________________________________________________________________

   UpdateDialogImpl Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(UpdateDialogImpl, CDialog)
  ON_MESSAGE(WM_ONPROGRESS, OnProgress)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   UpdateDialogImpl::OnInitDialog
   _________________________________________________________________________ */

BOOL
UpdateDialogImpl::OnInitDialog ()
{
  BOOL ret = CDialog::OnInitDialog();
  try
    {
      if (! downloadAnimationControl.Open(IDA_DOWNLOAD))
	{
	  FATAL_WINDOWS_ERROR (T_("CAnimateCtrl::Open"), 0);
	}
      if (! downloadAnimationControl.Play(0,
					  static_cast<unsigned int>(-1),
					  static_cast<unsigned int>(-1)))
	{
	  FATAL_WINDOWS_ERROR (T_("CAnimateCtrl::Play"), 0);
	}
      packageProgressControl.SetRange (0, 1000);
      packageProgressControl.SetPos (0);
      totalProgressControl.SetRange (0, 1000);
      totalProgressControl.SetPos (0);
      pInstaller->InstallRemoveAsync ();
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  return (ret);
}

/* _________________________________________________________________________

   UpdateDialogImpl::ReportLine
   _________________________________________________________________________ */

void
MIKTEXUICALL
UpdateDialogImpl::ReportLine (/*[in]*/ const MIKTEXCHAR * lpszLine)
{
  MIKTEX_LOCK(this)
    {
      report += lpszLine;
      report += T_("\r\n");
      reportUpdated = true;
      if (doContinue)
	{
	  if (! PostMessage(WM_ONPROGRESS))
	    {
	      FATAL_WINDOWS_ERROR (T_("CWnd::PostMessage"), 0);
	    }
	}
    }
  MIKTEX_UNLOCK();
}

/* _________________________________________________________________________

   UpdateDialogImpl::OnRetryableError
   _________________________________________________________________________ */

bool
MIKTEXUICALL
UpdateDialogImpl::OnRetryableError (/*[in]*/ const MIKTEXCHAR * lpszMessage)
{
  UNUSED_ALWAYS (lpszMessage);
  return (false);
}

/* _________________________________________________________________________

   UpdateDialogImpl::FormatControlText
   _________________________________________________________________________ */

void
UpdateDialogImpl::FormatControlText (/*[in]*/ UINT		ctrlId,
				     /*[in]*/ const MIKTEXCHAR * lpszFormat,
				     /*[in]*/			...)
{
  CWnd * pWnd = GetDlgItem(ctrlId);
  if (pWnd == 0)
    {
      UNEXPECTED_CONDITION (T_("UpdateDialogImpl::FormatControlText"));
    }
  va_list marker;
  va_start (marker, lpszFormat);
  tstring str = Utils::FormatString(lpszFormat, marker);
  va_end (marker);
  pWnd->SetWindowText (str.c_str());
}

/* _________________________________________________________________________

   UpdateDialogImpl::OnProgress
   _________________________________________________________________________ */

bool
MIKTEXUICALL
UpdateDialogImpl::OnProgress (/*[in]*/ Notification	nf)
{
  UNUSED_ALWAYS (nf);
  bool doContinue;
  MIKTEX_LOCK(this)
    {
      if (this->doContinue)
	{
	  if (! PostMessage(WM_ONPROGRESS))
	    {
	      FATAL_WINDOWS_ERROR (T_("CWnd::PostMessage"), 0);
	    }
	}
      doContinue = this->doContinue;
    }
  MIKTEX_UNLOCK();
  return (doContinue);
}

/* _________________________________________________________________________

   UpdateDialogImpl::OnCancel
   _________________________________________________________________________ */

void
UpdateDialogImpl::OnCancel ()
{
  if (cancelButtonIsCloseButton)
    {
      if (errorOccured)
	{
	  CDialog::OnCancel ();
	}
      else
	{
	  CDialog::OnOK ();
	}
      return;
    }

  CWaitCursor wait;

  // set cancel flag
  doContinue = false;

  try
    {
      // wait for installer to finish
      pInstaller->WaitForCompletion ();      
      pInstaller->Dispose ();
      pInstaller.reset ();
      pManager.Release ();
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }

  CDialog::OnCancel ();
}

/* _________________________________________________________________________

   UpdateDialogImpl::OnProcessOutput
   _________________________________________________________________________ */

bool
MIKTEXCALL
UpdateDialogImpl::OnProcessOutput (/*[in]*/ const void *	pOutput,
				   /*[in]*/ size_t		n)
{
  UNUSED_ALWAYS (pOutput);
  UNUSED_ALWAYS (n);
  return (true);
}

/* _________________________________________________________________________

   UpdateDialogImpl::RunIniTeXMF
   _________________________________________________________________________ */

void
UpdateDialogImpl::RunIniTeXMF ()
{
  PathName exe;

  if (! SessionWrapper(true)->FindFile(T_("initexmf"), FileType::EXE, exe))
    {
      FATAL_MIKTEX_ERROR (T_("UpdateDialogImpl::RunIniTeXMF"),
			  (T_("The MiKTeX configuration utility ")
			   T_("could not be found.")),
			  0);
    }

  CommandLineBuilder commandLine;
  commandLine.AppendOption (T_("--mkmaps"));

  Process::Run (exe, commandLine.Get(), this);
}

/* _________________________________________________________________________

   UpdateDialogImpl::OnProgress
   _________________________________________________________________________ */

LRESULT
UpdateDialogImpl::OnProgress (/*[in]*/ WPARAM	,
			      /*[in]*/ LPARAM	)
{
  try
    {
      // get progress information
      PackageInstaller::ProgressInfo progressInfo =
	pInstaller->GetProgressInfo();

      // update report
      MIKTEX_LOCK(this)
	{
	  if (reportUpdated)
	    {
	      reportBox.SetWindowText (report);
	      reportBox.SetSel (100000, 100000);
	      reportUpdated = false;
	    }
	}
      MIKTEX_UNLOCK();

      // update "Removed files (packages)"
      FormatControlText (IDC_REMOVED_FILES,
			 T_("%u (%u)"),
			 progressInfo.cFilesRemoveCompleted,
			 progressInfo.cPackagesRemoveCompleted);
      
      // upate "Errors"
      FormatControlText (IDC_NUM_ERRORS,
			 T_("%u"),
			 progressInfo.numErrors);
      
      // update "New files (packages)"
      FormatControlText (IDC_NEW_FILES,
			 T_("%u (%u)"),
			 progressInfo.cFilesInstallCompleted,
			 progressInfo.cPackagesInstallCompleted);

      // update "Downloaded bytes"
      FormatControlText (IDC_DOWNLOADED_BYTES,
			 T_("%u"),
			 progressInfo.cbDownloadCompleted);
      
      // update "Package"
      FormatControlText (IDC_PACKAGE,
			 T_("%s"),
			 progressInfo.displayName.c_str());
      
      // update package progress bar
      if (progressInfo.cbPackageDownloadTotal > 0)
	{
	  packageProgressControl.SetPos
	    (static_cast<int>
	     ((static_cast<double>(progressInfo.cbPackageDownloadCompleted)
	       * 1000)
	      / progressInfo.cbPackageDownloadTotal));
	}

      // update total progress bar
      if (progressInfo.cbDownloadTotal > 0)
	{
	  totalProgressControl.SetPos
	    (static_cast<int>
	     ((static_cast<double>(progressInfo.cbDownloadCompleted)
	       * 1000)
	      / progressInfo.cbDownloadTotal));
	}
      
      // update "KB/s"
      FormatControlText (IDC_KB_SEC,
			 T_("%.2f"),
			 (static_cast<double>(progressInfo.bytesPerSecond)
			  / 1024.0));
      
      if (progressInfo.ready)
	{
	  RunIniTeXMF ();
	  if (! downloadAnimationControl.Stop())
	    {
	      FATAL_WINDOWS_ERROR (T_("CAnimateCtrl::Stop"), 0);
	    }
	  downloadAnimationControl.Close (); // always returns FALSE
	  downloadAnimationControl.ShowWindow (SW_HIDE);
	  FormatControlText (IDCANCEL, T_("%s"), T_("Close"));
	  if (progressInfo.numErrors > 0)
	    {
	      errorOccured = true;
	    }
	  pInstaller->Dispose ();
	  pInstaller.reset ();
	  pManager.Release ();
	  cancelButtonIsCloseButton = true;
	}
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }

  return (0);
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
  UpdateDialogImpl dlg (pParent, pManager);
  dlg.SetFileLists (toBeInstalled, toBeRemoved);
  return (dlg.DoModal());
}
