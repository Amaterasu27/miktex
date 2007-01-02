/* winWebFile.h:						-*- C++ -*-

   Copyright (C) 2001-2006 Christian Schenk

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

#pragma once

#include "WebFile.h"
#include "win/winWebSession.h"

BEGIN_INTERNAL_NAMESPACE;

class
winWebFile : public WebFile
{
public:
  virtual
  ~winWebFile ();

public:
  virtual
  size_t
  Read (/*[out]*/ void *	pBuffer,
	/*[in]*/ size_t		n);

public:
  virtual
  void
  Close ();

public:
  winWebFile (/*[in]*/ winWebSession *		pSession,
	      /*[in]*/ const MIKTEXCHAR *	lpszUrl);

private:
  HINTERNET hUrl;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_error;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_mpm;

private:
  MiKTeX::Core::tstring url; 
};

END_INTERNAL_NAMESPACE;
