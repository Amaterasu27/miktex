/* TarLzmaExtractor.h:					-*- C++ -*-

   Copyright (C) 2001-2008 Christian Schenk

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

#if ! defined(GUARD_D040E724_D4A6_4B65_A182_955ED1238B7A_)
#define GUARD_D040E724_D4A6_4B65_A182_955ED1238B7A_

#include "Extractor.h"

BEGIN_INTERNAL_NAMESPACE;

class TarLzmaExtractor : public Extractor
{
public:
  TarLzmaExtractor ();

public:
  virtual
  ~TarLzmaExtractor ();

public:
  virtual
  void
  Extract (/*[in]*/ const PathName &	cabinetPath,
	   /*[in]*/ const PathName &	destDir,
	   /*[in]*/ bool		makeDirectories,
	   /*[in]*/ IExtractCallback *	pCallback,
	   /*[in]*/ const MIKTEXCHAR *	lpszPrefix);
  
public:
  virtual
  void
  Dispose ();

protected:
  bool haveLongName;

protected:
  PathName longName;

private:
  auto_ptr<TraceStream> traceStream;
};

END_INTERNAL_NAMESPACE;

#endif
