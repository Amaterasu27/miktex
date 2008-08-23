/* miktex/Graphics/DibChunker.h:				-*- C++ -*-

   Copyright (C) 2002-2008 Christian Schenk

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

#include <miktex/Core/Core>

#if defined(MIKTEX_WINDOWS)
#  include <windows.h>
#endif

// DLL import/export switch
#if ! defined(CA71604DC6CF21439A421D76D5C2E5D7)
#  define MIKTEXDIBEXPORT __declspec(dllimport)
#endif

// API decoration for exported member functions
#define MIKTEXDIBCEEAPI(type) MIKTEXDIBEXPORT type MIKTEXCEECALL

#define MIKTEX_GRAPHICS_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace Graphics {

#define MIKTEX_GRAPHICS_END_NAMESPACE		\
    }						\
  }

MIKTEX_GRAPHICS_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   DibChunk
   _________________________________________________________________________ */

class
MIKTEXNOVTABLE
DibChunk
{
protected:
  virtual
  MIKTEXTHISCALL
  ~DibChunk ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  AddRef ()
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
  Release ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetX ()
    = 0;

public:
  virtual
  int
  MIKTEXTHISCALL
  GetY ()
    = 0;

public:
  virtual
  BITMAPINFOHEADER
  MIKTEXTHISCALL
  GetBitmapInfoHeader ()
    = 0;

public:
  virtual
  const RGBQUAD *
  MIKTEXTHISCALL
  GetColors ()
    = 0;

public:
  virtual
  const void *
  MIKTEXTHISCALL
  GetBits ()
    = 0;

public:
  virtual
  const BITMAPINFO *
  MIKTEXTHISCALL
  GetBitmapInfo ()
    = 0;

public:
  virtual
  size_t
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
  Read (/*[out]*/ void *	pBuf,
	/*[in]*/ size_t		size)
    = 0;

public:
  virtual
  void
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
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
  MIKTEXTHISCALL
  Process (/*[in]*/ unsigned long		flags,
	   /*[in]*/ unsigned long		chunkSize,
	   /*[in]*/ IDibChunkerCallback *	pCallback)
    = 0;

public:
  static
  MIKTEXDIBCEEAPI(DibChunker *)
  Create ();
};

MIKTEX_GRAPHICS_END_NAMESPACE;

#undef MIKTEX_GRAPHICS_BEGIN_NAMESPACE
#undef MIKTEX_GRAPHICS_END_NAMESPACE

#endif
