/* Extractor.h:							-*- C++ -*-

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

#if ! defined(CE2192A940434CEB82EFF92FA64AC9D4_)
#define CE2192A940434CEB82EFF92FA64AC9D4_

#if ! defined(DAA6476494C144C8BED9A9E8810BAABA)
#  if ! defined(MIKTEX_STATIC) && defined(_MSC_VER)
#    define EXTRACTOREXPORT __declspec(dllimport)
#  else
#    define EXTRACTOREXPORT
#  endif
#endif

#if defined(_MSC_VER)
#  define EXTRACTORCALL __stdcall
#else
#  define EXTRACTORCALL
#endif

#define EXTRACTORAPI(type) EXTRACTOREXPORT type EXTRACTORCALL

#include <miktex/core.h>
#include <miktex/paths.h>

#define EXTRACTOR_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace Extractor {

#define EXTRACTOR_END_NAMESPACE			\
    }						\
  }

EXTRACTOR_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   ArchiveFileType
   _________________________________________________________________________ */

class ArchiveFileTypeEnum
{
public:
  enum EnumType {
    None = 0,
    MSCab = 1,
    TarBzip2 = 2,
    Zip = 3,
    Tar = 4,
    TarLzma = 5,
  };

public:
  static
  const MIKTEXCHAR *
  GetFileNameExtension (/*[in]*/ EnumType archiveFileType)
  {
    switch (archiveFileType)
      {
      case MSCab:
	return (MIKTEX_CABINET_FILE_SUFFIX);
      case TarBzip2:
	return (MIKTEX_TARBZIP2_FILE_SUFFIX);
      case TarLzma:
	return (MIKTEX_TARLZMA_FILE_SUFFIX);
      case Zip:
	return (MIKTEX_ZIP_FILE_SUFFIX);
      case Tar:
	return (MIKTEX_TAR_FILE_SUFFIX);
      default:
	MiKTeX::Core::Session::FatalMiKTeXError
	  (MIKTEXTEXT("ArchiveFileTypeEnum::GetFileNameExtension"),
	   MIKTEXTEXT("Unknown archive file type."),
	   0,
	   MIKTEXTEXT(__FILE__),
	   __LINE__);
      }
  }
};

typedef MiKTeX::Core::EnumWrapper<ArchiveFileTypeEnum> ArchiveFileType;

/* _________________________________________________________________________

   IExtractCallback
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
IExtractCallback
{
public:
  virtual
  void
  EXTRACTORCALL
  OnBeginFileExtraction (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
			 /*[in]*/ size_t		uncompressedSize)
    = 0;

public:
  virtual
  void
  EXTRACTORCALL
  OnEndFileExtraction (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
		       /*[in]*/ size_t			uncompressedSize)
    = 0;

public:
  virtual
  bool
  EXTRACTORCALL
  OnError (/*[in]*/ const MIKTEXCHAR *	lpszMessage)
    = 0;
};

/* _________________________________________________________________________

   Extractor
   _________________________________________________________________________ */


class
MIKTEXNOVTABLE
Extractor
{
public:
  virtual
  EXTRACTORCALL
  ~Extractor ()
    = 0;

public:
  virtual
  void
  EXTRACTORCALL
  Extract (/*[in]*/ const MiKTeX::Core::PathName &	path,
	   /*[in]*/ const MiKTeX::Core::PathName &	destDir,
	   /*[in]*/ bool				makeDirectories,
	   /*[in]*/ IExtractCallback *			pCallback,
	   /*[in]*/ const MIKTEXCHAR *			lpszPrefix)
    = 0;

public:
  virtual
  void
  EXTRACTORCALL
  Extract (/*[in]*/ MiKTeX::Core::Stream *		pStream,
	   /*[in]*/ const MiKTeX::Core::PathName &	destDir,
	   /*[in]*/ bool				makeDirectories,
	   /*[in]*/ IExtractCallback *			pCallback,
	   /*[in]*/ const MIKTEXCHAR *			lpszPrefix)
    = 0;

public:
  virtual
  void
  EXTRACTORCALL
  Dispose ()
    = 0;

public:
  static
  EXTRACTOREXPORT
  Extractor *
  EXTRACTORCALL
  CreateExtractor (/*[in]*/ ArchiveFileType archiveFileType);
};

EXTRACTOR_END_NAMESPACE;

#undef EXTRACTOR_BEGIN_NAMESPACE
#undef EXTRACTOR_END_NAMESPACE

#endif
