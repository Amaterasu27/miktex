/* TarBzip2Extractor.h:					-*- C++ -*-

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(GUARD_E03089A1127031428EBAC6B157F22548_)
#define GUARD_E03089A1127031428EBAC6B157F22548_

#include "Extractor.h"

BEGIN_INTERNAL_NAMESPACE;

class TarBzip2Extractor : public Extractor
{
public:
  TarBzip2Extractor ();

public:
  virtual
  ~TarBzip2Extractor ();

public:
  virtual
  void
  Extract (/*[in]*/ const MiKTeX::Core::PathName &	cabinetPath,
	   /*[in]*/ const MiKTeX::Core::PathName &	destDir,
	   /*[in]*/ bool		makeDirectories,
	   /*[in]*/ IExtractCallback *	pCallback,
	   /*[in]*/ const MIKTEXCHAR *	lpszPrefix);
  
public:
  virtual
  void
  Dispose ();

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> traceStream;
};

END_INTERNAL_NAMESPACE;

#endif
