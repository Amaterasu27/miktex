/* internal.h:							-*- C++ -*-

   Copyright (C) 2002-2007 Christian Schenk

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

#pragma once

#define MIKTEXDIBEXPORT __declspec(dllexport)

#define CA71604DC6CF21439A421D76D5C2E5D7
#include "MiKTeX/Graphics/DibChunker"

/* _________________________________________________________________________

   Debug-dependant Macros
   _________________________________________________________________________ */

#if ! defined(UNUSED)
#  if ! defined(NDEBUG)
#    define UNUSED(x)
#  else
#    define UNUSED(x) static_cast<void>(x)
#  endif
#endif

#if ! defined(UNUSED_ALWAYS)
#  define UNUSED_ALWAYS(x) static_cast<void>(x)
#endif

/* _________________________________________________________________________

   Error Macros
   _________________________________________________________________________ */

#define BUF_TOO_SMALL(function)						\
  FATAL_MIKTEX_ERROR (function,						\
                      T_("Not enough room in an internal buffer."),	\
                      0)

#define INVALID_ARGUMENT(function, param)				\
  FATAL_MIKTEX_ERROR (function, T_("Invalid argument."), param)

#define OUT_OF_MEMORY(function)						\
  FATAL_MIKTEX_ERROR(function, T_("Virtual memory exhausted."), 0)

#define UNEXPECTED_CONDITION(function)				\
  FATAL_MIKTEX_ERROR (function, T_("Unexpected condition."), 0)

#define UNIMPLEMENTED(function)						\
  FATAL_MIKTEX_ERROR (function, T_("Function not implemented."), 0)

#define MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  MiKTeX::Core::TraceMiKTeXError (miktexFunction,		\
				  traceMessage,			\
				  lpszInfo,			\
				  __FILE__,			\
				  __LINE__)

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  MiKTeX::Core::Session::FatalMiKTeXError (miktexFunction,		\
					   traceMessage,		\
					   lpszInfo,			\
					   __FILE__,			\
					   __LINE__)

#define CRT_ERROR(lpszCrtFunction, lpszInfo)				\
  MiKTeX::Core::TraceStream::TraceLastCRTError (lpszCrtFunction,	\
						lpszInfo,		\
						__FILE__,		\
						__LINE__)

#define FATAL_CRT_ERROR(lpszCrtFunction, lpszInfo)		\
  MiKTeX::Core::Session::FatalCrtError (lpszCrtFunction,	\
					lpszInfo,		\
					__FILE__,		\
					__LINE__)

#define FATAL_CRT_ERROR_2(lpszCrtFunction, errorCode, lpszInfo)	\
  MiKTeX::Core::Session::FatalCrtError (lpszCrtFunction,	\
					errorCode,		\
					lpszInfo,		\
					__FILE__,		\
					__LINE__)

#if defined(MIKTEX_WINDOWS)
#  define WINDOWS_ERROR(lpszWindowsFunction, lpszInfo)			\
  MiKTeX::Core::TraceStream::TraceLastWin32Error (lpszWindowsFunction,	\
						  lpszInfo,		\
						  __FILE__,		\
						  __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  MiKTeX::Core::Session::FatalWindowsError (windowsfunction,	\
					    lpszInfo,		\
					    __FILE__,		\
					    __LINE__)
#endif

#if defined(MIKTEX_WINDOWS)
#  define FATAL_WINDOWS_ERROR_2(windowsfunction, errorCode, lpszInfo)	\
  MiKTeX::Core::Session::FatalWindowsError (windowsfunction,		\
					    errorCode,			\
					    lpszInfo,			\
					    __FILE__,			\
					    __LINE__)
#endif

#define FATAL_MPM_ERROR(miktexFunction, traceMessage, lpszInfo) \
  FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

#define INTERNALFUNC(type) type

/* _________________________________________________________________________

   DibChunkImpl
   _________________________________________________________________________ */

class DibChunkImpl : public MiKTeX::Graphics::DibChunk
{
public:
  DibChunkImpl (/*[in]*/ unsigned	bytesPerLine,
		/*[in]*/ unsigned	numScanLines)
    : pBitmapInfo (0),
      refCount (0),
      size (bytesPerLine * numScanLines)
  {
    pBits = malloc(size);
    if (pBits == 0)
      {
	OUT_OF_MEMORY ("DibChunkerImpl::EndChunk");
      }
  }
      
protected:
  virtual
  MIKTEXTHISCALL
  ~DibChunkImpl ()
  {
    if (pBitmapInfo != 0)
      {
	free (pBitmapInfo);
	pBitmapInfo = 0;
      }
    if (pBits != 0)
      {
	free (pBits);
	pBits = 0;
      }
  }

public:
  virtual
  void
  MIKTEXTHISCALL
  AddRef ()
  {
    ++ refCount;
  }

public:
  virtual
  void
  MIKTEXTHISCALL
  Release ()
  {
    -- refCount;
    if (refCount == 0)
      {
	delete this;
      }
  }

public:
  virtual
  int
  MIKTEXTHISCALL
  GetX ()
  {
    return (x);
  }

public:
  virtual
  int
  MIKTEXTHISCALL
  GetY ()
  {
    return (y);
  }

public:
  virtual
  BITMAPINFOHEADER
  MIKTEXTHISCALL
  GetBitmapInfoHeader ()
  {
    return (pBitmapInfo->bmiHeader);
  }

public:
  virtual
  const RGBQUAD *
  MIKTEXTHISCALL
  GetColors ()
  {
    return (&pBitmapInfo->bmiColors[0]);
  }

public:
  virtual
  const void *
  MIKTEXTHISCALL
  GetBits ()
  {
    return (pBits);
  }

public:
  virtual
  const BITMAPINFO *
  MIKTEXTHISCALL
  GetBitmapInfo ()
  {
    return (pBitmapInfo);
  }

public:
  virtual
  size_t
  MIKTEXTHISCALL
  GetSize ()
  {
    return (size);
  }

public:
  void
  SetX (/*[in]*/ int x)
  {
    this->x = x;
  }

public:
  void
  SetY (/*[in]*/ int y)
  {
    this->y = y;
  }

public:
  void
  SetBitmapInfo (/*[in]*/ const BITMAPINFOHEADER &	bitmapInfoHeader,
		 /*[in]*/ unsigned			numColors,
		 /*[in]*/ const RGBQUAD *		pColors)
  {
    if (pBitmapInfo != 0)
      {
	free (pBitmapInfo);
	pBitmapInfo = 0;
      }
    pBitmapInfo =
      reinterpret_cast<BITMAPINFO*>(malloc(bitmapInfoHeader.biSize
					   + numColors * sizeof(RGBQUAD)));
    if (pBitmapInfo == 0)
      {
	OUT_OF_MEMORY ("DibChunkImpl::SetBitmapInfo");
      }
    memcpy (&pBitmapInfo->bmiHeader,
	    &bitmapInfoHeader,
	    bitmapInfoHeader.biSize);
    if (numColors > 0)
      {
	memcpy (pBitmapInfo->bmiColors, pColors, numColors * sizeof(RGBQUAD));
      }
  }

public:
  void *
  GetBits2 ()
  {
    return (pBits);
  }

private:
  int x;

private:
  int y;

private:
  BITMAPINFO * pBitmapInfo;

private:
  void * pBits;

private:
  size_t size;

private:
  int refCount;
};

/* _________________________________________________________________________

   DibChunkerImpl
   _________________________________________________________________________ */

class DibChunkerImpl : public MiKTeX::Graphics::DibChunker
{
public:
  DibChunkerImpl ();

public:
  virtual
  MIKTEXTHISCALL
  ~DibChunkerImpl ();

public:
  virtual
  bool
  MIKTEXTHISCALL
  Process (/*[in]*/ unsigned long				flags,
	   /*[in]*/ unsigned long				chunkSize,
	   /*[in]*/ MiKTeX::Graphics::IDibChunkerCallback *	pCallback);

private:
  bool
  Read (/*[out]*/ void *	pData,
	/*[in]*/ size_t		n,
	/*[in]*/ bool		allowEof = false);

private:
  bool
  ReadBitmapFileHeader ();

private:
  void
  ReadBitmapInfo ();

private:
  bool
  ReadScanLine (/*[in]*/ unsigned long &	left,
		/*[in]*/ unsigned long &	right);

private:
  void
  BeginChunk ();

private:
  void
  AddScanLine ();

private:
  void
  EndChunk ();

private:
  unsigned long
  BytesPerLine (/*[in]*/ long width,
		/*[in]*/ long bitCount)
    const
  {
    return ((((width * bitCount) + 31) & ~31) >> 3);
  }

private:
  unsigned long
  BytesPerLine (/*[in]*/ long width)
    const
  {
    return (BytesPerLine(width, bitmapInfoHeader.biBitCount));
  }

private:
  unsigned long
  BytesPerLine ()
    const
  {
    return (BytesPerLine(bitmapInfoHeader.biWidth));
  }

private:
  COLORREF
  GetColor (/*[in]*/ unsigned long idx)
    const
  {
    MIKTEX_ASSERT (pColors != 0);
    MIKTEX_ASSERT (idx < numColors);
    RGBQUAD q = pColors[idx];
    return (RGB(q.rgbRed, q.rgbGreen, q.rgbBlue));
  }

private:
  bool
  Crop1 (/*[in]*/ unsigned long &	left,
	 /*[in]*/ unsigned long &	right);

private:
  bool
  Crop4 (/*[in]*/ unsigned long &	left,
	 /*[in]*/ unsigned long &	right);

private:
  bool
  Crop8 (/*[in]*/ unsigned long &	left,
	 /*[in]*/ unsigned long &	right);

private:
  bool
  Crop24 (/*[in]*/ unsigned long &	left,
	  /*[in]*/ unsigned long &	right);

private:
  void
  Monochromize24 (/*[in]*/ const unsigned char *	pSrc,
		  /*[out]*/ unsigned char *		pDst,
		  /*[in]*/ unsigned long		width);

private:
  MiKTeX::Graphics::IDibChunkerCallback * pCallback;

private:
  RGBQUAD * pColors;

private:
  size_t numBytesRead;

private:
  unsigned char * pScanLine;

private:
  unsigned char * pBits;

private:
  unsigned numScanLines;

private:
  int yPos;

private:
  bool isBlackAndWhite;

private:
  unsigned long leftPos;

private:
  unsigned long rightPos;

private:
  bool inChunk;

private:
  int yPosChunk;

private:
  unsigned long numColors;

private:
  unsigned long blankLines;

private:
  BITMAPFILEHEADER bitmapFileHeader;

private:
  BITMAPINFOHEADER bitmapInfoHeader;

private:
  std::auto_ptr<MiKTeX::Core::TraceStream> trace_dib;

private:
  int processingFlags;
};
