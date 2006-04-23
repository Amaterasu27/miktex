/* Extractor.h:							-*- C++ -*-

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

#if ! defined(GUARD_84BF935BF7F2B24790D070733E790F54_)
#define GUARD_84BF935BF7F2B24790D070733E790F54_

BEGIN_INTERNAL_NAMESPACE;

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
    Zip = 3
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
      case Zip:
	return (MIKTEX_ZIP_FILE_SUFFIX);
      default:
	FATAL_MPM_ERROR (T_("ArchiveFileTypeEnum::GetFileNameExtension"),
			 T_("Unknown archive file type."),
			 0);
      }
  }
};

typedef EnumWrapper<ArchiveFileTypeEnum> ArchiveFileType;

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
  OnBeginFileExtraction (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
			 /*[in]*/ size_t		uncompressedSize)
    = 0;

public:
  virtual
  void
  OnEndFileExtraction (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
		       /*[in]*/ size_t			uncompressedSize)
    = 0;

public:
  virtual
  bool
  OnError (/*[in]*/ const MIKTEXCHAR *	lpszMessage)
    = 0;
};

/* _________________________________________________________________________

   Extractor
   _________________________________________________________________________ */


class Extractor
{
public:
  virtual
  ~Extractor ();

public:
  virtual
  void
  Extract (/*[in]*/ const PathName &		path,
	   /*[in]*/ const PathName &		destDir,
	   /*[in]*/ bool			makeDirectories,
	   /*[in]*/ IExtractCallback *		pCallback,
	   /*[in]*/ const MIKTEXCHAR *		lpszPrefix)
    = 0;

public:
  virtual
  void
  Dispose ()
    = 0;

public:
  static
  Extractor *
  CreateExtractor (/*[in]*/ ArchiveFileType archiveFileType);
};

END_INTERNAL_NAMESPACE;

#endif
