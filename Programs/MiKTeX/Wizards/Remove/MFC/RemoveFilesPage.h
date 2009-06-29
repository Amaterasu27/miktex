/* RemoveFilesPage.h:					-*- C++ -*-

   Copyright (C) 2000-2009 Christian Schenk

   This file is part of the Remove MiKTeX! Wizard.

   The Remove MiKTeX! Wizard is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The Remove MiKTeX! Wizard is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the Remove MiKTeX! Wizard; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#pragma once

#include "Remove.h"

class RemoveFilesPage : public CPropertyPage
{
private:
  enum { IDD = IDD_REMOVE_FILES };

protected:
  DECLARE_MESSAGE_MAP();

public:
  RemoveFilesPage ();

public:
  virtual
  ~RemoveFilesPage ();

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
  BOOL
  OnQueryCancel ();

protected:
  afx_msg
  LRESULT
  OnStartRemovingFiles (/*[in]*/ WPARAM wParam,
			/*[in]*/ LPARAM lParam);

protected:
  afx_msg
  void
  OnTimer (/*[in]*/ UINT nIDEvent);
  
public:
  void
  Progress (/*[in]*/ const PathName &	fileName,
	    /*[in]*/ unsigned long	progress,
	    /*[in]*/ unsigned long	total);

public:
  bool
  GetCancelFlag ();

public:
  void
  SetReadyFlag ();

private:
  static
  UINT
  WorkerThread (/*[in]*/ void * pParam);

private:
  void
  RemoveMiKTeX ();

private:
  bool
  Exists (/*[in]*/ HKEY			hkeyRoot,
	  /*[in]*/ const PathName &	subKey);

private:
  bool
  IsEmpty (/*[in]*/ HKEY		hkeyRoot,
	   /*[in]*/ const PathName &	subKey);

private:
  void
  RemoveRegistryKeys ();
  
private:
  void
  RemoveRegistryKey (/*[in]*/ HKEY		hkeyRoot,
		     /*[in]*/ const PathName &	subKey);

private:
  bool
  RemoveBinDirFromPath (/*[in,out]*/ string &	path);

private:
  void
  UnregisterShellFileTypes ();

private:
  void
  UnregisterComponents ();

private:
  void
  UnregisterPathNT ();

private:
  void
  UnregisterPathNT (/*[in]*/ bool shared);

#if defined(MIKTEX_SUPPORT_LEGACY_WINDOWS)
private:
  void
  UnregisterPath95 ();
#endif

private:
  CProgressCtrl	progressControl;
  
private:
  CCriticalSection criticalSectionMonitor;

private:
  CString currentFileName;

private:
  unsigned long progress;

private:
  unsigned long total;

private:
  bool ready;

private:
  HANDLE hWorkerThread;

private:
  class RemoveWizard * pSheet;
};
