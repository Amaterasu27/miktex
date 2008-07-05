/* TarExtractor.h:						-*- C++ -*-

   Copyright (C) 2001-2008 Christian Schenk

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

#if ! defined(GUARD_BF702FE4_09EC_4B95_9264_0F4FD967F75B_)
#define GUARD_BF702FE4_09EC_4B95_9264_0F4FD967F75B_

#include "miktex/Extractor/Extractor"

BEGIN_INTERNAL_NAMESPACE;

class TarExtractor : public MiKTeX::Extractor::Extractor
{
public:
  TarExtractor ();

public:
  virtual
  MIKTEXTHISCALL
  ~TarExtractor ();

public:
  virtual
  void
  MIKTEXTHISCALL
  Extract (/*[in]*/ const MiKTeX::Core::PathName &	tarPath,
	   /*[in]*/ const MiKTeX::Core::PathName &	destDir,
	   /*[in]*/ bool				makeDirectories,
	   /*[in]*/ IExtractCallback *			pCallback,
	   /*[in]*/ const char *			lpszPrefix);
  
public:
  virtual
  void
  MIKTEXTHISCALL
  Extract (/*[in]*/ MiKTeX::Core::Stream *		pStream,
	   /*[in]*/ const MiKTeX::Core::PathName &	destDir,
	   /*[in]*/ bool				makeDirectories,
	   /*[in]*/ IExtractCallback *			pCallback,
	   /*[in]*/ const char *			lpszPrefix);
  
public:
  virtual
  void
  MIKTEXTHISCALL
  Dispose ();

protected:
  size_t
  Read (/*[out]*/ void *	pBuffer,
	/*[in]*/ size_t		bytes)
  {
    size_t n = pStreamIn->Read(pBuffer, bytes);
    totalBytesRead += n;
    return (n);
  }

protected:
  void
  ReadBlock (/*[out]*/ void *	pBuffer);

protected:
  std::auto_ptr<MiKTeX::Core::TraceStream> traceStream;

protected:
  bool haveLongName;

protected:
  MiKTeX::Core::PathName longName;

protected:
  size_t totalBytesRead;

protected:
  MiKTeX::Core::Stream * pStreamIn;

protected:
  void
  Skip (/*[in]*/ size_t		bytes);

};

END_INTERNAL_NAMESPACE;

#endif
