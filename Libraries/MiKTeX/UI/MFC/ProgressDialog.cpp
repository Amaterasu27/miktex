/* ProgressDialog.cpp:

   Copyright (C) 2000-2007 Christian Schenk

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
#include "resource.h"

#include <MiKTeX/UI/MFC/ProgressDialog>

/* _________________________________________________________________________

   ProgressUIDialog
   _________________________________________________________________________ */

class ProgressUIDialog : public CDialog
{
private:
  enum { IDD = IDD_PROGRESS };

protected:
  DECLARE_MESSAGE_MAP ();

public:
  ProgressUIDialog ();

protected:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);

protected:
  virtual
  void
  PostNcDestroy ();

protected:
  virtual
  void
  OnCancel ();

protected:
  afx_msg
  LRESULT
  HasUserCancelled (/*[in]*/ WPARAM wParam,
		    /*[in]*/ LPARAM lParam);

protected:
  afx_msg
  LRESULT
  Destroy (/*[in]*/ WPARAM wParam,
	   /*[in]*/ LPARAM lParam);

private:
  CCriticalSection criticalSectionMonitor;

private:
  bool cancelled;
};

/* _________________________________________________________________________

   ProgressUIDialog Message Map
   _________________________________________________________________________ */

enum {
  DESTROY = WM_APP + 314,
  HASUSERCANCELLED
};

BEGIN_MESSAGE_MAP(ProgressUIDialog, CDialog)
  ON_MESSAGE(DESTROY, Destroy)
  ON_MESSAGE(HASUSERCANCELLED, HasUserCancelled)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   ProgressUIDialog::ProgressUIDialog
   _________________________________________________________________________ */

ProgressUIDialog::ProgressUIDialog ()
  : cancelled(false)
{
}

/* _________________________________________________________________________

   ProgressUIDialog::DoDataExchange
   _________________________________________________________________________ */

void
ProgressUIDialog::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CDialog::DoDataExchange (pDX);
}

/* _________________________________________________________________________

   ProgressUIDialog::OnCancel
   _________________________________________________________________________ */

void
ProgressUIDialog::OnCancel () 
{
  CSingleLock singleLock (&criticalSectionMonitor, TRUE);
  cancelled = true;
}

/* _________________________________________________________________________

   ProgressUIDialog::HasUserCancelled
   _________________________________________________________________________ */

LRESULT
ProgressUIDialog::HasUserCancelled (/*[in]*/ WPARAM	wParam,
				    /*[in]*/ LPARAM	lParam)
{
  UNUSED_ALWAYS (wParam);
  UNUSED_ALWAYS (lParam);
  CSingleLock singleLock (&criticalSectionMonitor, TRUE);
  return (static_cast<LRESULT>(cancelled));
}

/* _________________________________________________________________________

   ProgressUIDialog::Destroy
   _________________________________________________________________________ */

LRESULT
ProgressUIDialog::Destroy (/*[in]*/ WPARAM	wParam,
			   /*[in]*/ LPARAM	lParam)
{
  UNUSED_ALWAYS (wParam);
  UNUSED_ALWAYS (lParam);
  return (static_cast<LRESULT>(DestroyWindow()));
}

/* _________________________________________________________________________

   ProgressUIDialog::PostNcDestroy
   _________________________________________________________________________ */

void
ProgressUIDialog::PostNcDestroy ()
{
  CDialog::PostNcDestroy ();
  delete this;
}

/* _________________________________________________________________________

   ProgressUIThread
   _________________________________________________________________________ */

class ProgressUIThread : public CWinThread
{
public:
  ProgressUIThread ();

public:
  ProgressUIThread (/*[in]*/ HWND	hwndParent,
		    /*[in]*/ CEvent *	pReady);

public:
  HWND
  GetProgressWindow ();

public:
  virtual
  BOOL
  InitInstance ();

protected:
  HWND hParentWindow;

protected:
  CEvent * pReadyEvent;

protected:
  HWND hWindow;

protected:
  CCriticalSection criticalSectionMonitor;
};

/* _________________________________________________________________________

   ProgressUIThread::ProgressUIThread
   _________________________________________________________________________ */
					
ProgressUIThread::ProgressUIThread ()
  : hParentWindow (0),
    hWindow (0),
    pReadyEvent (0)
{
}

/* _________________________________________________________________________

   ProgressUIThread::ProgressUIThread
   _________________________________________________________________________ */
					
ProgressUIThread::ProgressUIThread (/*[in]*/ HWND	hwndParent,
				    /*[in]*/ CEvent *	pReady)
  : hParentWindow (hwndParent),
    hWindow (0),
    pReadyEvent (pReady)
{
}

/* _________________________________________________________________________

   ProgressUIThread::InitInstance
   _________________________________________________________________________ */

BOOL
ProgressUIThread::InitInstance ()
{
  try
    {
      ProgressUIDialog * pDlg = new ProgressUIDialog;
      m_pMainWnd = pDlg;
      MIKTEX_ASSERT (hParentWindow == 0 || IsWindow(hParentWindow));
      CWnd * pWnd = 0;
      if (hParentWindow != 0)
	{
	  pWnd = CWnd::FromHandle(hParentWindow);
	}
      if (! pDlg->Create(IDD_PROGRESS, pWnd))
	{
	  FATAL_WINDOWS_ERROR (T_("CDialog::Create"), 0);
	}
      hWindow = pDlg->GetSafeHwnd();
      MIKTEX_ASSERT (hWindow != 0);
      pReadyEvent->SetEvent ();
      return (TRUE);
    }
  catch (const exception &)
    {
      return (FALSE);
    }
}

/* _________________________________________________________________________

   ProgressUIThread::GetProgressWindow
   _________________________________________________________________________ */

HWND
ProgressUIThread::GetProgressWindow ()
{
  CSingleLock singleLock (&criticalSectionMonitor, TRUE);
  return (hWindow);
}

/* _________________________________________________________________________

   ProgressDialogImpl
   _________________________________________________________________________ */

class ProgressDialogImpl : public ProgressDialog
{
public:
  ProgressDialogImpl ();

public:
  virtual
  ~ProgressDialogImpl ();
  
public:
  virtual
  bool
  HasUserCancelled ();

public:
  virtual
  bool
  SetLine (/*[in]*/ unsigned		lineNum,
	   /*[in]*/ const MIKTEXCHAR *	lpszText);

public:
  virtual
  bool
  SetTitle (/*[in]*/ const MIKTEXCHAR *	lpszTitle);

public:
  virtual
  bool
  StartProgressDialog (/*[in]*/ HWND hwndParent);

public:
  virtual
  bool
  StopProgressDialog ();

private:
  ProgressUIThread * pThread;

private:
  HWND hWindow;

private:
  CString lines[2];

private:
  CString title;

private:
  CEvent readyEvent;
};

/* _________________________________________________________________________

   ProgressDialogImpl::ProgressDialogImpl
   _________________________________________________________________________ */

ProgressDialogImpl::ProgressDialogImpl ()
  : pThread (0),
    hWindow (0)
{
}

/* _________________________________________________________________________

   ProgressDialogImpl::~ProgressDialogImpl
   _________________________________________________________________________ */

ProgressDialogImpl::~ProgressDialogImpl ()
{
  try
    {
      StopProgressDialog ();
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   ProgressDialogImpl::HasUserCancelled
   _________________________________________________________________________ */

bool
ProgressDialogImpl::HasUserCancelled ()
{
  if (hWindow == 0)
    {
      return (false);
    }

  DWORD res;

  if (! SendMessageTimeout(hWindow,
			   HASUSERCANCELLED,
			   0,
			   0,
			   0,
			   1000,
			   &res))
    {
      CHECK_WINDOWS_ERROR (T_("SendMessageTimeout"), 0);
      FATAL_MIKTEX_ERROR (T_("ProgressDialogImpl::HasUserCancelled"),
			  T_("The progress window does not respond."),
			  0);
    }

  return (res ? true : false);
}

/* _________________________________________________________________________

   ProgressDialogImpl::SetLine
   _________________________________________________________________________ */

bool
ProgressDialogImpl::SetLine (/*[in]*/ unsigned			lineNum,
			     /*[in]*/ const MIKTEXCHAR *	lpszText)
{
  MIKTEX_ASSERT (lineNum > 0 && lineNum <= 2);
  if (pThread == 0)
    {
      lines[lineNum - 1] = lpszText;
      return (true);
    }
  HWND hwndLine = ::GetDlgItem(hWindow, lineNum == 1 ? IDC_LINE1 : IDC_LINE2);
  if (hwndLine == 0)
    {
      FATAL_WINDOWS_ERROR (T_("GetDlgItem"), 0);
    }
  if (! SetWindowText(hwndLine, lpszText))
    {
      FATAL_WINDOWS_ERROR (T_("SetWindowText"), 0);
    }
  return (true);
}

/* _________________________________________________________________________

   ProgressDialogImpl::SetTitle
   _________________________________________________________________________ */

bool
ProgressDialogImpl::SetTitle (/*[in]*/ const MIKTEXCHAR * lpszTitle)
{
  if (pThread == 0)
    {
      title = lpszTitle;
      return (true);
    }
  if (! SetWindowText(hWindow, lpszTitle))
    {
      FATAL_WINDOWS_ERROR (T_("SetWindowText"), 0);
    }
  return (true);
}

/* _________________________________________________________________________

   ProgressDialogImpl::StartProgressDialog
   _________________________________________________________________________ */

namespace {
  // <fixme/>
  HWND hParentWindow;
  bool haveProgressDialog = false;
}

bool
ProgressDialogImpl::StartProgressDialog (/*[in]*/ HWND hwndParent)
{
  if (haveProgressDialog)
    {
      FATAL_MIKTEX_ERROR (T_("ProgressDialogImpl::StartProgressDialog"),
			  T_("A progress dialog window is already open."),
			  0);
    }

  CWnd * pParent = 0;

  if (hwndParent != 0)
    {
      pParent = CWnd::FromHandlePermanent(hwndParent);
      if (pParent == 0)
	{
	  INVALID_ARGUMENT (T_("ProgressDialogImpl::StartProgressDialog"), 0);
	}
    }

  pParent = CWnd::GetSafeOwner(pParent, 0);

  if (pParent != 0)
    {
      hwndParent = pParent->GetSafeHwnd();
    }

  hParentWindow = hwndParent;

  // disable mouse and keyboard input in the parent window
  if (hParentWindow != 0)
    {
      EnableWindow (hParentWindow, FALSE);
    }

  // create the user interface thread
  pThread = new ProgressUIThread(0, &readyEvent);
  pThread->CreateThread ();

  // wait for the progress window to become available
  CSingleLock singleLock (&readyEvent);
  if (! singleLock.Lock(1000))
    {
      FATAL_MIKTEX_ERROR (T_("ProgressDialogImpl::StartProgressDialog"),
			  T_("The progress window is not available."),
			  0);
    }
  hWindow = pThread->GetProgressWindow();
  MIKTEX_ASSERT (IsWindow(hWindow));
  haveProgressDialog = true;

  // set the window texts
  SetTitle (title);
  SetLine (1, lines[0]);
  SetLine (2, lines[1]);

  // make the progress window visible
  ShowWindow (hWindow, SW_SHOW);

  return (true);
}

/* _________________________________________________________________________

   ProgressDialogImpl::StopProgressDialog
   _________________________________________________________________________ */

bool
ProgressDialogImpl::StopProgressDialog ()
{
  if (hWindow == 0)
    {
      return (false);
    }

  HWND hwnd = hWindow;
  hWindow = 0;
  
  // enable mouse and keyboard input in the parent window
  if (hParentWindow != 0)
    {
      EnableWindow (hParentWindow, TRUE);
    }

  // destroy the dialog window
  haveProgressDialog = false;
  DWORD res;
  if (! SendMessageTimeout(hwnd,
			   DESTROY,
			   0,
			   0,
			   0,
			   1000,
			   &res))
    {
      CHECK_WINDOWS_ERROR (T_("SendMessageTimeout"), 0);
      FATAL_MIKTEX_ERROR (T_("ProgressDialogImpl::StopProgressDialog"),
			  T_("The progress window does not respond."),
			  0);
    }
		       
  return (res ? true : false);
}

/* _________________________________________________________________________

   ProgressDialog::Create
   _________________________________________________________________________ */

ProgressDialog *
ProgressDialog::Create ()
{
  return (new ProgressDialogImpl ());
}

/* _________________________________________________________________________

   ProgressDialog::~ProgressDialog
   _________________________________________________________________________ */

ProgressDialog::~ProgressDialog ()
{
}
