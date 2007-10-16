/* DibChunker.h:						-*- C++ -*-

   Copyright (C) 2002-2006 Christian Schenk

   This file is part of the MiKTeX DibChunker Library.

   The MiKTeX DibChunker Library is free software; you can
   redistribute it and/or modify it under the terms of the GNU General
   Public License as published by the Free Software Foundation; either
   version 2, or (at your option) any later version.
   
   The MiKTeX DibChunker Library is distributed in the hope that it
   will be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX DibChunker Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(EA12EA6239A13E40B589B41B84360852)
#define EA12EA6239A13E40B589B41B84360852

#if ! defined(CA71604DC6CF21439A421D76D5C2E5D7)
#  define DIBCHUNKEREXPORT __declspec(dllimport)
#endif

#if defined(_MSC_VER)
#  define DIBCHUNKERCALL __stdcall
#else
#  define DIBCHUNKERCALL
#endif

#include <miktex/core.h>

#if defined(MIKTEX_WINDOWS)
#  include <windows.h>
#endif

#define DIBCHUNKER_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace Graphics {

#define DIBCHUNKER_END_NAMESPACE		\
    }						\
  }

DIBCHUNKER_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   DibChunk
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
DibChunk
{
protected:
  virtual
  DIBCHUNKERCALL
  ~DibChunk ()
    = 0;

public:
  virtual
  void
  DIBCHUNKERCALL
  AddRef ()
    = 0;

public:
  virtual
  void
  DIBCHUNKERCALL
  Release ()
    = 0;

public:
  virtual
  int
  DIBCHUNKERCALL
  GetX ()
    = 0;

public:
  virtual
  int
  DIBCHUNKERCALL
  GetY ()
    = 0;

public:
  virtual
  BITMAPINFOHEADER
  DIBCHUNKERCALL
  GetBitmapInfoHeader ()
    = 0;

public:
  virtual
  const RGBQUAD *
  DIBCHUNKERCALL
  GetColors ()
    = 0;

public:
  virtual
  const void *
  DIBCHUNKERCALL
  GetBits ()
    = 0;

public:
  virtual
  const BITMAPINFO *
  DIBCHUNKERCALL
  GetBitmapInfo ()
    = 0;

public:
  virtual
  size_t
  DIBCHUNKERCALL
  GetSize ()
    = 0;
};

/* _________________________________________________________________________

   IDibChunkerCallback
   _________________________________________________________________________ */

/// Callback interface.
class
MIKTEXNOVTABLE
IDibChunkerCallback
{
public:
  virtual
  size_t
  DIBCHUNKERCALL
  Read (/*[out]*/ void *	pBuf,
	/*[in]*/ size_t		size)
    = 0;

public:
  virtual
  void
  DIBCHUNKERCALL
  OnNewChunk (/*[in]*/ DibChunk * pChunk)
    = 0;
};

/* _________________________________________________________________________

   DibChunker
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
DibChunker
{
public:
  virtual
  DIBCHUNKERCALL
  ~DibChunker ()
    = 0;

public:
  enum {
    RemoveBlankLines = 1,
    CreateChunks = 2,
    Crop = 4,
    Default = 7
  };

public:
  virtual
  bool
  DIBCHUNKERCALL
  Process (/*[in]*/ unsigned long		flags,
	   /*[in]*/ unsigned long		chunkSize,
	   /*[in]*/ IDibChunkerCallback *	pCallback)
    = 0;

public:
  static
  DIBCHUNKEREXPORT
  DibChunker *
  DIBCHUNKERCALL
  Create ();
};

DIBCHUNKER_END_NAMESPACE;

#undef DIBCHUNKER_BEGIN_NAMESPACE
#undef DIBCHUNKER_END_NAMESPACE

#endif
