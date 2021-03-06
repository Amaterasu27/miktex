/* miktex/UI/MFC/ProgressDialog.h:				-*- C++ -*-

   Copyright (C) 2000-2008 Christian Schenk

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

#include <miktex/UI/MFC/Prototypes>

MIKUI_MFC_BEGIN_NAMESPACE;

class
MIKTEXNOVTABLE
ProgressDialog
{
public:
  virtual
  MIKTEXTHISCALL
  ~ProgressDialog ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  HasUserCancelled ()
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  SetLine (/*[in]*/ unsigned		lineNum,
	   /*[in]*/ const char *	lpszText)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  SetTitle (/*[in]*/ const char *	lpszTitle)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  StartProgressDialog (/*[in]*/ HWND hwndParent)
    = 0;

public:
  virtual
  bool
  MIKTEXTHISCALL
  StopProgressDialog ()
    = 0;

public:
  AFX_EXT_CLASS
  static
  ProgressDialog *
  MIKTEXCEECALL
  Create ();
};

MIKUI_MFC_END_NAMESPACE;
