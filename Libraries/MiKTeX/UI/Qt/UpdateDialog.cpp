/* UpdateDialog.cpp:

   Copyright (C) 2008 Christian Schenk

   This file is part of the MiKTeX UI Library.

   The MiKTeX UI Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX UI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX UI Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

#include "UpdateDialog.h"

#include <miktex/UI/Qt/UpdateDialog>

const int PROGRESS_MAX = 1000;

/* _________________________________________________________________________

   UpdateDialog::DoModal
   _________________________________________________________________________ */

int
UpdateDialog::DoModal (/*[in]*/ QWidget *		pParent,
		       /*[in]*/ PackageManager *	pManager,
		       /*[in]*/ const vector<string> &	toBeInstalled,
		       /*[in]*/ const vector<string> &	toBeRemoved)
{
  string url;
  RepositoryType repositoryType (RepositoryType::Unknown);
  if (toBeInstalled.size() > 0
    && PackageManager::TryGetDefaultPackageRepository(repositoryType, url)
    && repositoryType == RepositoryType::Remote
    && ! ProxyAuthenticationDialog(pParent))
  {
    return (QDialog::Rejected);
  }
  UpdateDialogImpl dlg (pParent, pManager, toBeInstalled, toBeRemoved);
  return (dlg.exec());
}

/* _________________________________________________________________________

   UpdateDialogImpl::WorkerThread::run
   _________________________________________________________________________ */

void
UpdateDialogImpl::WorkerThread::run ()
{
  UpdateDialogImpl * This = reinterpret_cast<UpdateDialogImpl *>(parent());
#if defined(MIKTEX_WINDOWS)
  HRESULT hr = E_FAIL;
#endif
  try
    {
#if defined(MIKTEX_WINDOWS)
      hr = CoInitializeEx(0, COINIT_MULTITHREADED);
      if (FAILED(hr))
	{
	  FATAL_MIKTEX_ERROR ("UpdateDialogImpl::WorkerThread",
			      T_("Cannot start worker thread."),
			      0);
	}
#endif
      This->pInstaller->SetCallback (This);
      This->pInstaller->InstallRemove ();
    }
  catch (const MiKTeXException & e)
    {
      threadMiKTeXException = e;
      error = true;
    }
  catch (const exception & e)
    {
    }
  This->sharedData.ready = true;
#if defined(MIKTEX_WINDOWS)
  if (SUCCEEDED(hr))
    {
      CoUninitialize ();
    }
#endif
  emit This->ProgressChanged ();
}

/* _________________________________________________________________________

   UpdateDialogImpl::UpdateDialogImpl
   _________________________________________________________________________ */

UpdateDialogImpl::UpdateDialogImpl
(/*[in]*/ QWidget *		pParent,
 /*[in]*/ PackageManager *	pManager,
 /*[in]*/ const vector<string> &	toBeInstalled,
 /*[in]*/ const vector<string> &	toBeRemoved)
  : QDialog (pParent),
    pManager (pManager),
    pInstaller (pManager->CreateInstaller()),
    cancelled (false),
    errorOccured (false)
{
  setupUi (this);
  connect (this, SIGNAL(ProgressChanged()), this, SLOT(ShowProgress()));
  connect (pushButton, SIGNAL(clicked()), this, SLOT(Cancel()));
  progressBar1->setMinimum (0);
  progressBar1->setMaximum (PROGRESS_MAX);
  progressBar1->setValue (0);
  progressBar2->setMinimum (0);
  progressBar2->setMaximum (PROGRESS_MAX);
  progressBar2->setValue (0);
  pInstaller->SetFileLists (toBeInstalled, toBeRemoved);
  pWorkerThread = new WorkerThread (this);
  pWorkerThread->start ();
}

/* _________________________________________________________________________

   UpdateDialogImpl::~UpdateDialogImpl
   _________________________________________________________________________ */

UpdateDialogImpl::~UpdateDialogImpl ()
{
  try
    {
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

   UpdateDialogImpl::Report
   _________________________________________________________________________ */

void
UpdateDialogImpl::Report (/*[in]*/ bool		immediate,
			  /*[in]*/ const char *	lpszFmt,
			  /*[in]*/		...)
{
  MIKTEX_ASSERT (lpszFmt != 0);
  QString str;
  va_list args;
  va_start (args, lpszFmt);
  str.vsprintf (lpszFmt, args);
  va_end (args);
  MIKTEX_LOCK(sharedData)
  {
    sharedData.report += str;
    sharedData.reportUpdate = true;
  }
  MIKTEX_UNLOCK();
  if (immediate)
  {
    emit ProgressChanged ();
  }
}

/* _________________________________________________________________________

   UpdateDialogImpl::ReportLine
   _________________________________________________________________________ */

void
UpdateDialogImpl::ReportLine (/*[in]*/ const char * lpszLine)
{
  Report (true, "%s\n", lpszLine);
}

/* _________________________________________________________________________

   UpdateDialogImpl::OnRetryableError
   _________________________________________________________________________ */

bool
UpdateDialogImpl::OnRetryableError (/*[in]*/ const char * lpszMessage)
{
  return (false);
}

/* _________________________________________________________________________

   UpdateDialogImpl::OnProgress
   _________________________________________________________________________ */

bool
UpdateDialogImpl::OnProgress (/*[in]*/ Notification	nf)
{
  MIKTEX_LOCK(sharedData)
  {
    bool visibleProgress =
      (nf == Notification::DownloadPackageEnd
      || nf == Notification::InstallFileEnd
      || nf == Notification::InstallPackageEnd
      || nf == Notification::RemoveFileEnd
      || nf == Notification::RemovePackageEnd);
    PackageInstaller::ProgressInfo progressInfo = pInstaller->GetProgressInfo();
    sharedData.progressInfo = progressInfo;
    if (nf == Notification::InstallPackageStart
      || nf == Notification::DownloadPackageStart)
    {
      visibleProgress = true;
      sharedData.newPackage = true;
      sharedData.packageName = progressInfo.displayName;
    }
    if (progressInfo.cbPackageDownloadTotal > 0)
    {
      int oldValue = sharedData.progress1Pos;
      sharedData.progress1Pos
	= static_cast<int>
	(((static_cast<double>(progressInfo.cbPackageDownloadCompleted)
	/ progressInfo.cbPackageDownloadTotal)
	* PROGRESS_MAX));
      visibleProgress =
	(visibleProgress || (sharedData.progress1Pos != oldValue));
    }
    if (progressInfo.cbDownloadTotal > 0)
    {
      int oldValue = sharedData.progress2Pos;
      sharedData.progress2Pos
	= static_cast<int>
	(((static_cast<double>(progressInfo.cbDownloadCompleted)
	/ progressInfo.cbDownloadTotal)
	* PROGRESS_MAX));
      visibleProgress =
	(visibleProgress || (sharedData.progress2Pos != oldValue));
    }
    unsigned oldValue = sharedData.secondsRemaining;
    sharedData.secondsRemaining
      = static_cast<unsigned>(progressInfo.timeRemaining / 1000);
    visibleProgress =
      (visibleProgress || (sharedData.secondsRemaining != oldValue));
    if (visibleProgress)
    {
      emit ProgressChanged();
    }
    return (! (GetErrorFlag() || GetCancelFlag()));
  }
  MIKTEX_UNLOCK();
}

/* _________________________________________________________________________

   UpdateDialogImpl::ShowProgress
   _________________________________________________________________________ */

void
UpdateDialogImpl::ShowProgress ()
{
  try
  {
    if (pWorkerThread->error)
    {
      pWorkerThread->error = false;
      ReportError (pWorkerThread->threadMiKTeXException);
    }
    MIKTEX_LOCK(sharedData)
    {

      // update the report
      if (sharedData.reportUpdate)
      {
	textBrowser->setPlainText (sharedData.report);
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
	  return;
	}

	sharedData.waitingForClickOnClose = true;

	// disable controls
	labelProgress1->setEnabled (false);
	labelPackageName->setText ("");
	labelPackageName->setEnabled (false);
	progressBar1->setValue (0);
	progressBar1->setEnabled (false);
	labelProgress2->setEnabled (false);
	progressBar2->setValue (0);
	progressBar2->setEnabled (false);

	pushButton->setText (tr("Close"));
	pushButton->setEnabled (true);

	disconnect (pushButton, SIGNAL(clicked()), this, SLOT(Cancel()));
	if (GetCancelFlag() || GetErrorFlag())
	{
	  connect (pushButton, SIGNAL(clicked()), this, SLOT(reject()));
	}
	else
	{
	  connect (pushButton, SIGNAL(clicked()), this, SLOT(accept()));
	}
      }
      else
      {
	// show the package name
	if (sharedData.newPackage)
	{
	  labelPackageName->setText (sharedData.packageName.c_str());
	  sharedData.newPackage = false;
	}

	// update progress bars
	progressBar1->setValue (sharedData.progress1Pos);
	progressBar2->setValue (sharedData.progress2Pos);

	// update "Removed files (packages)"
	QString format;
	format = "%1 (%2)";
	labelRemovedFiles->setText (format
	  .arg(sharedData.progressInfo.cFilesRemoveCompleted)
	  .arg(sharedData.progressInfo.cPackagesRemoveCompleted));

	// update "New files (packages)"
	format = "%1 (%2)";
	labelInstalledFiles->setText (format
	  .arg(sharedData.progressInfo.cFilesInstallCompleted)
	  .arg(sharedData.progressInfo.cPackagesInstallCompleted));

	// update "Downloaded bytes"
	format = "%1";
	labelDownloadedBytes->setText (format
	  .arg(sharedData.progressInfo.cbDownloadCompleted));

	// update "Package"
	labelPackageName->setText (
	  sharedData.progressInfo.displayName.c_str());

	// update "KB/s"
	format = "%1";
	labelKbytesSec->setText (format
	  .arg((static_cast<double>(sharedData.progressInfo.bytesPerSecond)
	  / 1024.0)));
      }
    }
    MIKTEX_UNLOCK();
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

   UpdateDialogImpl::Cancel
   _________________________________________________________________________ */

void
UpdateDialogImpl::Cancel ()
{
  try
    {
      if (QMessageBox::Ok == QMessageBox::information(this,
	"MiKTeX Package Manager",
	"The update operation will now be cancelled.",
	QMessageBox::Ok | QMessageBox::Cancel))
      {
	cancelled = true;
	disconnect (pushButton, SIGNAL(clicked()), this, SLOT(Cancel()));
	pushButton->setEnabled (false);
	emit ProgressChanged ();
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
