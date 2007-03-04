/* CabExtractor.h:						-*- C++ -*-

   Copyright (C) 2001-2007 Christian Schenk

   This file is part of MiKTeX Extractor.

   MiKTeX Extractor is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.
   
   MiKTeX Extractor is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Extractor; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(GUARD_73FFE7F857E7814191871F96B92C5228_)
#define GUARD_73FFE7F857E7814191871F96B92C5228_

#include "miktex/Extractor.h"

BEGIN_INTERNAL_NAMESPACE;

class CabExtractor : public MiKTeX::Extractor::Extractor
{
public:
  CabExtractor ();

public:
  virtual
  EXTRACTORCALL
  ~CabExtractor ();

public:
  virtual
  void
  EXTRACTORCALL
  Extract (/*[in]*/ const MiKTeX::Core::PathName &	cabinetPath,
	   /*[in]*/ const MiKTeX::Core::PathName &	destDir,
	   /*[in]*/ bool				makeDirectories,
	   /*[in]*/ IExtractCallback *			pCallback,
	   /*[in]*/ const MIKTEXCHAR *			lpszPrefix);
  
public:
  virtual
  void
  EXTRACTORCALL
  Extract (/*[in]*/ /*[in]*/ MiKTeX::Core::Stream *	pStream,
	   /*[in]*/ const MiKTeX::Core::PathName &	destDir,
	   /*[in]*/ bool				makeDirectories,
	   /*[in]*/ IExtractCallback *			pCallback,
	   /*[in]*/ const MIKTEXCHAR *			lpszPrefix);
  
public:
  virtual
  void
  EXTRACTORCALL
  Dispose ();

private:
  mscab_decompressor * pDecompressor;

private:
  static
  struct mspack_file *
  Open (/*[in]*/ struct mspack_system *	self,
	/*[in]*/ MIKTEXCHAR *		lpszFileName,
	/*[in]*/ int			mode);

private:
  static
  void
  Close (/*[in]*/ struct mspack_file *	pFile);

private:
  static
  int
  Read (/*[in]*/ struct mspack_file *	pFile,
	/*[out]*/ void *		pBuffer,
	/*[in]*/ int			numBytes);

private:
  static
  int
  Write (/*[in]*/ struct mspack_file *	pFile,
	 /*[in]*/ void *		pBuffer,
	 /*[in]*/ int			numBytes);

private:
  static
  int
  Seek (/*[in]*/ struct mspack_file *	pFile,
	/*[in]*/ off_t			offset,
	/*[in]*/ int			mode);

private:
  static
  off_t
  Tell (/*[in]*/ struct mspack_file * pFile);

private:
  static
  void
  Message (/*[in]*/ struct mspack_file *	pFile,
	   /*[in]*/ MIKTEXCHAR *		lpszFormat,
	   /*[in]*/				...);

private:
  static
  void *
  Alloc (/*[in]*/ struct mspack_system *	self,
	 /*[in]*/ size_t			numBytes);

private:
  static
  void
  Free (/*[in]*/ void *	pv);

private:
  static
  void
  Copy (/*[in]*/ void *	pSource,
	/*[in]*/ void *	pDest,
	/*[in]*/ size_t	numBytes);

private:
  struct MySystem : public mspack_system
  {
    CabExtractor *	pCabExtractor;
  };

private:
  struct MyFile
  {
    MyFile ()
      : pFile (0)
    {
    }
    MiKTeX::Core::tstring	fileName;
    FILE *			pFile;
  };

private:
  mspack_system mspackSystem;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> traceStream;
};

END_INTERNAL_NAMESPACE;

#endif
