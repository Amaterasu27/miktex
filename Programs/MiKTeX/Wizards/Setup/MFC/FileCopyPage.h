/* FileCopyPage.h:						-*- C++ -*-

   Copyright (C) 1999-2014 Christian Schenk

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
    public SetupServiceCallback
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
  OnProgress (/*[in]*/ MiKTeX::Setup::Notification		nf);

private:
  static
  UINT
  WorkerThread (/*[in]*/ void * pParam);

private:
  void
  Report (/*[in]*/ bool			writeLog,
	  /*[in]*/ const char *	lpszFmt,
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
    string packageName;
    string currentLine;
  };

private:
  SharedData sharedData;

private:
  HANDLE hWorkerThread;

private:
  DWORD timeOfLastProgressRefresh;

private:
  class SetupWizard * pSheet;
};
