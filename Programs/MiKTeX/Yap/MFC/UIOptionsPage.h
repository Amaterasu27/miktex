/* UIOptionsPage.h:						-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of Yap.

   Yap is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   Yap is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with Yap; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#pragma once

class UIOptionsPage : public CPropertyPage
{
private:
  enum { IDD = IDD_USER_INTERFACE };

protected:
  DECLARE_MESSAGE_MAP();

public:
  UIOptionsPage ();

protected:
  virtual
  void
  DoDataExchange (/*[in]*/ CDataExchange * pDX);

protected:
  virtual
  BOOL
  OnApply ();

protected:
  afx_msg
  void
  OnClickMaintainVerticalPosition ();

protected:
  afx_msg
  void
  OnClickMaintainHorizontalPosition ();

protected:
  afx_msg
  void
  OnClickShowSourceSpecials ();

private:
  BOOL maintainHorizontalPosition;

private:
  BOOL maintainVerticalPosition;

private:
  BOOL showSourceSpecials;
};
