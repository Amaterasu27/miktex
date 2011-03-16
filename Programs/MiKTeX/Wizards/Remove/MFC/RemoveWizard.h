/* RemoveWizard.h:						-*- C++ -*-

   Copyright (C) 2000-2011 Christian Schenk

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

class RemoveWizard : public CPropertySheet
{
protected:
  DECLARE_MESSAGE_MAP();

public:
  RemoveWizard ();

protected:
  virtual
  BOOL
  OnInitDialog ();

public:
  void
  SetErrorFlag ()
  {
    error = true;
  }

public:
  bool
  GetErrorFlag ()
    const
  {
    return (error);
  }

public:
  void
  SetCancelFlag ()
  {
    EnableCancelButton (false);
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
  void
  SetThoroughlyFlag (/*[in]*/ bool f)
  {
    thorough = f;
  }

public:
  bool
  GetThoroughlyFlag ()
    const
  {
    return (thorough);
  }

public:
  void
  EnableCloseButton ()
  {
    SetWizardButtons (PSWIZB_FINISH);
    EnableCancelButton (false);
    SetFinishText (T_(_T("Close")));
  }

public:
  CString
  SetNextText (/*[in]*/ const char * lpszText);

public:
  void
  ReportError (/*[in]*/ const exception & e);

public:
  void
  ReportError (/*[in]*/ const MiKTeXException & e);

private:
  void
  EnableCancelButton (/*[in]*/ bool enable);

private:
  bool error;

private:
  bool cancelled;

private:
  bool thorough;

private:
  CBitmap watermarkBitmap;
  
private:
  CBitmap headerBitmap;
};
