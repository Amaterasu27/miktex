/* FileCopyPage.h:						-*- C++ -*-

   Copyright (C) 2002-2011 Christian Schenk

   This file is part of the MiKTeX Update Wizard.

   The MiKTeX Update Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX Update Wizard is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX Update Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#include "resource.h"

class FileCopyPage
  : public CPropertyPage,
    public IRunProcessCallback,
    public PackageInstallerCallback
{
private:
  DECLARE_DYNCREATE(FileCopyPage);
  
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
  OnProgress (/*[in]*/ WPARAM wParam,
	      /*[in]*/ LPARAM lParam);

private:
  virtual
  bool
  MIKTEXTHISCALL
  OnProcessOutput (/*[in]*/ const void *	pOutput,
		   /*[in]*/ size_t		n);
  
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
  OnProgress (/*[in]*/ Notification		nf);

private:
  static
  UINT
  WorkerThread (/*[in]*/ void * pParam);

private:
  void
  DoTheUpdate ();

private:
  void
  ConfigureMiKTeX ();

private:
  void
  RunMpm (/*[in]*/ const CommandLineBuilder & cmdLine1);

private:
  void
  RunIniTeXMF (/*[in]*/ const CommandLineBuilder & cmdLine1);

private:
  void
  OpenLog ();

private:
  void
  Log (/*[in]*/ const char *	lpszFormat,
       /*[in]*/				...);

private:
  void
  Report (/*[in]*/ bool			writeLog,
	  /*[in]*/ bool			immediate,
	  /*[in]*/ const char *	lpszFormat,
	  /*[in]*/			...);

private:
  void
  CollectFiles (/*[out]*/ vector<string> &	vec,
		/*[in]*/ const PathName &	dir,
		/*[in]*/ const char *	lpszExt);
    
private:
  void
  RemoveFormatFiles ();

private:
  void
  RemoveFalseConfigFiles ();

private:
  void
  RemoveOldRegistrySettings ();

private:
  CWnd *
  GetControl (/*[in]*/ UINT	controlId);

private:
  void
  EnableControl (/*[in]*/ UINT	controlId,
		 /*[in]*/ bool	enable);

private:
  void
  ReportError (/*[in]*/ const exception & e);

private:
  void
  ReportError (/*[in]*/ const MiKTeXException & e);

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
	waitingForClickOnNext (false)
    {
    }
    string currentLine;
    string report;
    DWORD secondsRemaining;
    auto_ptr<StreamWriter> pLogStream;
    bool newPackage;
    bool ready;
    bool reportUpdate;
    bool waitingForClickOnNext;
    int progress1Pos;
    int progress2Pos;
    string packageName;
  };

private:
  SharedData sharedData;
  
private:
  auto_ptr<PackageInstaller> pInstaller;
  
private:
  class UpdateWizard * pSheet;
  
private:
  HANDLE hWorkerThread;
  
private:
  DWORD timeOfLastProgressRefresh;

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
};
