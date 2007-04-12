/* FileCopyPage.h:						-*- C++ -*-

   Copyright (C) 1999-2007 Christian Schenk

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

class FileCopyPage
  : public CPropertyPage,
    public IRunProcessCallback,
    public PackageInstallerCallback
{
private:
  enum { IDD = IDD_FILECOPY };

protected:
  DECLARE_MESSAGE_MAP();

public:
  FileCopyPage ();

public:
  virtual
  ~FileCopyPage ();

protected:
  virtual
  BOOL
  OnInitDialog ();

protected:
  virtual
  BOOL
  OnSetActive ();

protected:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);

protected:
  virtual
  LRESULT
  OnWizardNext ();

protected:
  virtual
  BOOL
  OnKillActive ();

protected:
  virtual
  BOOL
  OnQueryCancel ();

protected:
  afx_msg
  LRESULT
  OnStartFileCopy (/*[in]*/ WPARAM wParam,
		   /*[in]*/ LPARAM lParam);

protected:
  afx_msg
  LRESULT
  OnReport (/*[in]*/ WPARAM wParam,
	    /*[in]*/ LPARAM lParam);

protected:
  afx_msg
  LRESULT
  OnProgress (/*[in]*/ WPARAM wParam,
	      /*[in]*/ LPARAM lParam);

private:
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
  OnProgress (/*[in]*/ Notification		nf);

private:
  static
  UINT
  WorkerThread (/*[in]*/ void * pParam);

private:
  void
  DoTheDownload ();

private:
  void
  DoPrepareMiKTeXDirect ();

private:
  void
  DoTheInstallation ();

private:
  void
  ConfigureMiKTeX ();

private:
  void
  RunIniTeXMF (/*[in]*/ const CommandLineBuilder & cmdLine1);

private:
  void
  RunMpm (/*[in]*/ const CommandLineBuilder & cmdLine1);

private:
  void
  Report (/*[in]*/ bool			writeLog,
	  /*[in]*/ const MIKTEXCHAR *	lpszFmt,
	  /*[in]*/			...);

private:
  CWnd *
  GetControl (/*[in]*/ UINT	controlId);

private:
  void
  EnableControl (/*[in]*/ UINT	controlId,
		 /*[in]*/ bool	enable);

private:
  void
  RemoveObsoleteFiles ();

private:
  void
  CalculateExpenditure ();
  
private:
  size_t
  GetIniTeXMFRunSize ()
    const;

private:
  void
  CreateInfoFile ();

private:
  void
  ReportError (/*[in]*/ const exception & e);

private:
  void
  ReportError (/*[in]*/ const MiKTeXException & e);

private:
  CAnimateCtrl animationControl;

private:
  CProgressCtrl	progressControl1;

private:
  CProgressCtrl	progressControl2;

private:
  CRichEditCtrl	reportControl;

private:
  CCriticalSection criticalSectionMonitor;
  
private:
  int totalIniTeXMFRuns;

private:
  DWORD overallExpenditure;

private:
  DWORD totalSize;

private:
  size_t completedIniTeXMFRuns;

private:
  struct SharedData
  {
    SharedData ()
      :	ready (false),
	progress1Pos (0),
	progress2Pos (0),
	newPackage (false),
	secondsRemaining (0),
	waitingForClickOnNext (false)
    {
    }
    DWORD secondsRemaining;
    bool newPackage;
    bool ready;
    bool waitingForClickOnNext;
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
  DWORD timeOfLastProgressRefresh;

private:
  class SetupWizard * pSheet;
};
