/* UpdateDialog.h:					-*- C++ -*-

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(A3C9B533501744AF97F67F3F0E3161DA)
#define A3C9B533501744AF97F67F3F0E3161DA

#include <memory>

#include <QThread>

#include <miktex/PackageManager/PackageManager>
#include <miktex/UI/Qt/ErrorDialog>

#include "ui_UpdateDialog.h"

class UpdateDialogImpl
  : public QDialog,
    private Ui::UpdateDialog,
    public MiKTeX::Packages::PackageInstallerCallback
{
private:
  Q_OBJECT;

public:
  UpdateDialogImpl
  (/*[in]*/ QWidget *				pParent,
   /*[in]*/ MiKTeX::Packages::PackageManager *	pManager,
   /*[in]*/ const std::vector<std::string> &	toBeInstalled,
   /*[in]*/ const std::vector<std::string> &	toBeRemoved);

public:
  ~UpdateDialogImpl ();

public:
  virtual
  void
  MIKTEXTHISCALL
  ReportLine (/*[in]*/ const char * lpszLine);

public:
  virtual
  bool
  MIKTEXTHISCALL
  OnRetryableError (/*[in]*/ const char * lpszMessage);

public:
  virtual
  bool
  MIKTEXTHISCALL
  OnProgress (/*[in]*/ MiKTeX::Packages::Notification nf);

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

private slots:
  void ShowProgress ();
  void Cancel ();

signals:
  void
  ProgressChanged ();

private:
  void
  Report (/*[in]*/ bool			immediate,
	  /*[in]*/ const char *		lpszFmt,
	  /*[in]*/			...);

private:
  void
  ReportError (/*[in]*/ const MiKTeX::Core::MiKTeXException & e)
  {
    errorOccured = true;
    MiKTeX::UI::Qt::ErrorDialog::DoModal (this, e);
  }

private:
  void
  ReportError (/*[in]*/ const std::exception & e)
  {
    errorOccured = true;
    MiKTeX::UI::Qt::ErrorDialog::DoModal (this, e);
  }

private:
  MIKTEX_DEFINE_LOCK(sharedData);

private:
  bool errorOccured;

private:
  bool cancelled;

private:
  MiKTeX::Packages::PackageManagerPtr pManager;

private:
  class WorkerThread
    : public QThread
  {
  public:
    WorkerThread (/*[in]*/ UpdateDialogImpl * pParent)
      : QThread (pParent),
        error (false)
    {
    }
  public:
    virtual
    void
    run ();
  public:
    MiKTeX::Core::MiKTeXException threadMiKTeXException;
  public:
    bool error;
  };

private:
  WorkerThread * pWorkerThread;

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
    MiKTeX::Packages::PackageInstaller::ProgressInfo progressInfo;
    QString report;
    unsigned secondsRemaining;
    bool newPackage;
    bool ready;
    bool reportUpdate;
    bool waitingForClickOnClose;
    int progress1Pos;
    int progress2Pos;
    std::string packageName;
  };

private:
  SharedData sharedData;

private:
  std::auto_ptr<MiKTeX::Packages::PackageInstaller> pInstaller;
};

#endif
