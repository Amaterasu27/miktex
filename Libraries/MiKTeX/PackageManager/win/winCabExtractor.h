/* winCabExtractor.h:						-*- C++ -*-

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

#include "Extractor.h"

BEGIN_INTERNAL_NAMESPACE;

class winCabExtractor : public Extractor
{
public:
  winCabExtractor ();

public:
  virtual
  ~winCabExtractor ();

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
  void
  MIKTEXNORETURN
  FatalFdiError (/*[in]*/ const MIKTEXCHAR *	lpszCabinetPath);

private:
  IExtractCallback *	pCallback;

private:
  bool error;

private:
  bool cancelled;

private:
  bool makeDirectories;

private:
  unsigned long uncompressedSize;

private:
  static FNALLOC(Alloc);
  static FNFREE(Free);
  static FNOPEN(Open);
  static FNREAD(Read);
  static FNWRITE(Write);
  static FNCLOSE(Close);
  static FNSEEK(Seek);
  static FNFDINOTIFY(Notify);

private:
  HFDI hfdi;

private:
  ERF erf;

private:
  MiKTeX::Core::PathName destinationDirectory;

private:
  MiKTeX::Core::tstring prefixToBeStripped;

private:
  size_t fileCount;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_error;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_mpm;

private:
  static std::map<int, MiKTeX::Core::PathName> openFiles;
};

END_INTERNAL_NAMESPACE;
