/* WebSession.h:						-*- C++ -*-

   Copyright (C) 2001-2009 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(GUARD_878D85749EB68F488FB1FC91A4502461_)
#define GUARD_878D85749EB68F488FB1FC91A4502461_

#include "WebFile.h"

BEGIN_INTERNAL_NAMESPACE;

class
MIKTEXNOVTABLE
IProgressNotify_
{
public:
  virtual
  void
  OnProgress ()
    = 0;
};

class
MIKTEXNOVTABLE
WebSession
{
public:
  virtual
  ~WebSession ()
    = 0;

public:
  virtual
  WebFile *
  OpenUrl (/*[in]*/ const char * lpszUrl)
    = 0;

public:
  virtual
  void
  Dispose ()
    = 0;

public:
  static
  WebSession *
  Create (/*[in]*/ IProgressNotify_ * pIProgressNotify);
};

END_INTERNAL_NAMESPACE;

#endif
