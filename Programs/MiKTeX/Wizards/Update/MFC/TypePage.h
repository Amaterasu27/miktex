/* TypePage.h:							-*- C++ -*-

   Copyright (C) 2002-2006 Christian Schenk

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

#pragma once

#include "resource.h"

class TypePage : public CPropertyPage
{
private:
  DECLARE_DYNCREATE(TypePage);

protected:
  DECLARE_MESSAGE_MAP();

private:
  enum { IDD = IDD_TYPE };
  
public:
  TypePage ();
  
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
  LRESULT
  OnWizardBack ();
  
protected:
  virtual
  BOOL
  OnKillActive ();
  
protected:
  afx_msg
  void
  OnInstallFromInternet ();
  
protected:
  afx_msg
  void
  OnInstallFromLocalRepository ();

protected:
  afx_msg
  void
  OnInstallFromCd ();

protected:
  afx_msg
  void
  OnConnectionSettings ();
  
protected:
  void
  EnableButtons ();
  
private:
  CWnd *
  GetControl (/*[in]*/ UINT	controlId);

private:
  void
  EnableControl (/*[in]*/ UINT	controlId,
		 /*[in]*/ bool	enable);
  
private:
  class UpdateWizard * pSheet;

private:
  int sourceChoice;

private:
  int remoteChoice;
  
private:
  int localChoice;

private:
  tstring repository;

private:
  bool haveRemoteRepository;

private:
  PathName localDirectory;

private:
  bool haveLocalRepository;

private:
  bool noDdv;
};
