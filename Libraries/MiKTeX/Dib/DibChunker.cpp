/* DibChunker.cpp:

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

#include "StdAfx.h"

#include "internal.h"

const COLORREF RGB_WHITE = RGB(255, 255, 255);
const COLORREF RGB_BLACK = RGB(0, 0, 0);

const RGBQUAD RGBQUAD_WHITE = { 255, 255, 255, 0 };
const RGBQUAD RGBQUAD_BLACK = { 0, 0, 0, 0 };

const RGBQUAD whiteAndBlack[2] = { RGBQUAD_WHITE, RGBQUAD_BLACK };

/* _________________________________________________________________________

   DibChunk::~DibChunk
   _________________________________________________________________________ */

DIBCHUNKERCALL
DibChunk::~DibChunk ()
{
}

/* _________________________________________________________________________

   DibChunker::Create
   _________________________________________________________________________ */

DibChunker *
DIBCHUNKERCALL
DibChunker::Create ()
{
  return (new DibChunkerImpl);
}

/* _________________________________________________________________________

   DibChunker::~DibChunker
   _________________________________________________________________________ */

DIBCHUNKERCALL
DibChunker::~DibChunker ()
{
}

/* _________________________________________________________________________

   DibChunkerImpl::DibChunkerImpl
   _________________________________________________________________________ */

DibChunkerImpl::DibChunkerImpl ()
  : pColors (0),
    pScanLine (0),
    pBits (0),
    processingFlags (Default),
    trace_dib (TraceStream::Open(MIKTEX_TRACE_DIB))
{
}

/* _________________________________________________________________________

   DibChunkerImpl::~DibChunkerImpl
   _________________________________________________________________________ */

DibChunkerImpl::~DibChunkerImpl ()
{
  try
    {
      if (pColors != 0)
	{
	  delete [] pColors;
	  pColors = 0;
	}
      if (pScanLine != 0)
	{
	  delete [] pScanLine;
	  pScanLine = 0;
	}
      if (pBits != 0)
	{
	  delete [] pBits;
	  pBits = 0;
	}
      if (trace_dib.get() != 0)
	{
	  trace_dib->Close ();
	  trace_dib.reset ();
	}
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   DibChunkerImpl::Read
   _________________________________________________________________________ */

bool
DibChunkerImpl::Read (/*[out]*/ void *	pData,
		      /*[in]*/ size_t	size,
		      /*[in]*/ bool	allowEof)
{
  while (size > 0)
    {
      size_t n = pCallback->Read(pData, size);
      size -= n;
      this->numBytesRead += n;
      pData = reinterpret_cast<unsigned char*>(pData) + n;
      if (n == 0)
	{
	  if (allowEof)
	    {
	      return (false);
	    }
	  FATAL_MIKTEX_ERROR (T_("DibChunkerImpl::Read"),
			      T_("Unexpected end of bitmap input stream."),
			      0);
	}
    }
  return (true);
}

/* _________________________________________________________________________

   DibChunkerImpl::ReadBitmapFileHeader
   _________________________________________________________________________ */

bool
DibChunkerImpl::ReadBitmapFileHeader ()
{
  if (! Read(&bitmapFileHeader.bfType,
	     sizeof(bitmapFileHeader.bfType),
	     true))
    {
      return (false);
    }
  if (bitmapFileHeader.bfType != 0x4d42)
    {
      FATAL_MIKTEX_ERROR (T_("DibChunkerImpl::ReadBitmapFileHeader"),
			  T_("Not a bitmap file header."),
			  0);
    }
  Read (&bitmapFileHeader.bfSize,
	(sizeof(BITMAPFILEHEADER)
	 - offsetof(BITMAPFILEHEADER, bfSize)));
  return (true);
}

/* _________________________________________________________________________

   CalcNumColors
   _________________________________________________________________________ */

unsigned long
CalcNumColors (/*[in]*/ unsigned long	bitCount)
{
  if (bitCount == 24)
    {
      return (0);
    }
  else
    {
      return (1 << bitCount);
    }
}

/* _________________________________________________________________________

   DibChunkerImpl::ReadBitmapInfo
   _________________________________________________________________________ */

void
DibChunkerImpl::ReadBitmapInfo ()
{
  // get known part of bitmap info header
  Read (&bitmapInfoHeader, sizeof(bitmapInfoHeader));
  if (bitmapInfoHeader.biSize < sizeof(bitmapInfoHeader))
    {
      FATAL_MIKTEX_ERROR (T_("DibChunkerImpl::ReadBitmapInfo"),
			  T_("Unsupported bitmap format."),
			  0);
    }
  if (! (bitmapInfoHeader.biBitCount == 1
	 || bitmapInfoHeader.biBitCount == 4
	 || bitmapInfoHeader.biBitCount == 8
	 || bitmapInfoHeader.biBitCount == 24))
    {
       FATAL_MIKTEX_ERROR (T_("DibChunkerImpl::ReadBitmapInfo"),
			  T_("Unsupported bitmap format."),
			  0);
    }
  if (bitmapInfoHeader.biCompression != BI_RGB)
    {
       FATAL_MIKTEX_ERROR (T_("DibChunkerImpl::ReadBitmapInfo"),
			  T_("Unsupported bitmap format."),
			  0);
    }

  // skip unknown part of bitmap info header
  if (bitmapInfoHeader.biSize > sizeof(bitmapInfoHeader))
    {
      unsigned long n = bitmapInfoHeader.biSize - sizeof(bitmapInfoHeader);
      void * p = malloc(n);
      if (p == 0)
	{
	  OUT_OF_MEMORY (T_("DibChunkerImpl::ReadBitmapInfo"));
	}
      AutoMemoryPointer autoFree (p);
      Read (p, n);
    }

  // get colors
  numColors =
    (bitmapInfoHeader.biClrUsed == 0
     ? CalcNumColors(bitmapInfoHeader.biBitCount)
     : bitmapInfoHeader.biClrUsed);
  if (numColors > 0)
    {
      pColors = new RGBQUAD[numColors];
      Read (pColors, sizeof(RGBQUAD) * numColors);
    }

  // seek for bitmap bits
  if (bitmapFileHeader.bfOffBits < numBytesRead)
    {
      FATAL_MIKTEX_ERROR (T_("DibChunkerImpl::ReadBitmapInfo"),
			  T_("Invalid bitmap data."),
			  0);
    }
  if (bitmapFileHeader.bfOffBits > numBytesRead)
    {
      unsigned long n = bitmapFileHeader.bfOffBits - numBytesRead;
      void * p = malloc(n);
      if (p == 0)
	{
	  OUT_OF_MEMORY (T_("DibChunkerImpl::ReadBitmapInfo"));
	}
      AutoMemoryPointer autoFree (p);
      Read (p, n);
    }
  MIKTEX_ASSERT (bitmapFileHeader.bfOffBits == numBytesRead);
  trace_dib->WriteFormattedLine (T_("libdib"),
				 T_("chunking bitmap %ldx%ld, %u colors"),
				 bitmapInfoHeader.biWidth,
				 bitmapInfoHeader.biHeight,
				 numColors);
}

/* _________________________________________________________________________

   DibChunkerImpl::Crop1
   _________________________________________________________________________ */

bool
DibChunkerImpl::Crop1 (/*[out]*/ unsigned long &	left,
		       /*[out]*/ unsigned long &	right)
{
  left = UINT_MAX;
  right = UINT_MAX;

  // find white in color table
  unsigned long whiteIdx = 0;
  if (GetColor(0) != RGB_WHITE)
    {
      whiteIdx = 1;
      if (GetColor(1) != RGB_WHITE)
	{
	  // white not found
	  return (false);
	}
    }

  // white byte
  unsigned char white = static_cast<unsigned char>(whiteIdx == 0 ? 0 : 255);

  // number of bytes to check
  unsigned long n = (bitmapInfoHeader.biWidth + 7) / 8;

  // crop
  for (unsigned long idx = 0; idx < n; idx += 1)
    {
      if (pScanLine[idx] != white)
	{
	  if (idx < left)
	    {
	      left = idx;
	    }
	  right = idx;
	}
    }

  return (true);
}

/* _________________________________________________________________________

   DibChunkerImpl::Crop4
   _________________________________________________________________________ */

bool
DibChunkerImpl::Crop4 (/*[out]*/ unsigned long &	left,
		       /*[out]*/ unsigned long &	right)
{
  left = UINT_MAX;
  right = UINT_MAX;

  // number of bytes to check
  unsigned long n = (bitmapInfoHeader.biWidth + 1) / 2;

  bool isBlackAndWhite = true;

  // crop
  for (unsigned long idx = 0; idx < n; idx += 1)
    {
      unsigned char byte = pScanLine[idx];
      unsigned high = (byte >> 4) & 15;
      unsigned low = byte & 15;
      if (! (GetColor(high) == RGB_WHITE && GetColor(low) == RGB_WHITE))
	{
	  if (idx < left)
	    {
	      left = idx;
	    }
	  right = idx;
	  if (! (GetColor(high) == RGB_BLACK && GetColor(low) == RGB_BLACK))
	    {
	      isBlackAndWhite = false;
	    }
	}
    }

  return (isBlackAndWhite);
}

/* _________________________________________________________________________

   DibChunkerImpl::Crop8
   _________________________________________________________________________ */

bool
DibChunkerImpl::Crop8 (/*[out]*/ unsigned long &	left,
		       /*[out]*/ unsigned long &	right)
{
  left = UINT_MAX;
  right = UINT_MAX;

  // number of bytes to check
  unsigned long n = bitmapInfoHeader.biWidth;

  bool isBlackAndWhite = true;

  // crop
  for (unsigned long idx = 0; idx < n; idx += 1)
    {
      if (GetColor(pScanLine[idx]) != RGB_WHITE)
	{
	  if (idx < left)
	    {
	      left = idx;
	    }
	  right = idx;
	  if (GetColor(pScanLine[idx]) != RGB_BLACK)
	    {
	      isBlackAndWhite = false;
	    }
	}
    }

  return (isBlackAndWhite);
}

/* _________________________________________________________________________

   DibChunkerImpl::Crop24
   _________________________________________________________________________ */

bool
DibChunkerImpl::Crop24 (/*[out]*/ unsigned long &	left,
			/*[out]*/ unsigned long &	right)
{
  left = UINT_MAX;
  right = UINT_MAX;

  // number of  bytes to check
  unsigned long n = bitmapInfoHeader.biWidth * 3;

  bool isBlackAndWhite = true;

  // crop
  for (unsigned long idx = 0; idx < n; idx += 3)
    {
      COLORREF clr = RGB(pScanLine[idx + 2],
			 pScanLine[idx + 1],
			 pScanLine[idx]);
      if (clr != RGB_WHITE)
	{
	  if (idx < left)
	    {
	      left = idx;
	    }
	  right = idx + 2;
	  if (clr != RGB_BLACK)
	    {
	      isBlackAndWhite = false;
	    }
	}
    }

  return (isBlackAndWhite);
}

/* _________________________________________________________________________

   DibChunkerImpl::ReadScanLine
   _________________________________________________________________________ */

bool
DibChunkerImpl::ReadScanLine (/*[out]*/ unsigned long &	left,
			      /*[out]*/ unsigned long &	right)
{
  unsigned long n = BytesPerLine();

  if (pScanLine == 0)
    {
      pScanLine = new unsigned char[n];
    }

  Read (pScanLine, n);

  bool isBlackAndWhite;

  if ((processingFlags & Crop) != 0)
    {
      // analyse the scan line
      switch (bitmapInfoHeader.biBitCount)
	{
	case 1:
	  isBlackAndWhite = true;
	  Crop1 (left, right);
	  break;
	case 4:
	  isBlackAndWhite = Crop4(left, right);
	  break;
	case 8:
	  isBlackAndWhite = Crop8(left, right);
	  break;
	case 24:
	  isBlackAndWhite = Crop24(left, right);
	  break;
	default:
	  UNEXPECTED_CONDITION (T_("DibChunkerImpl::ReadScanLine"));
	}
    }
  else
    {
      isBlackAndWhite = false;
      left = 0;
      // analyse the scan line
      switch (bitmapInfoHeader.biBitCount)
	{
	case 1:
	  right = (bitmapInfoHeader.biWidth + 7) / 8 - 1;
	  break;
	case 4:
	  right = (bitmapInfoHeader.biWidth + 1) / 2 - 1;
	  break;
	case 8:
	  right = bitmapInfoHeader.biWidth;
	  break;
	case 24:
	  right = bitmapInfoHeader.biWidth * 3 - 1;
	  break;
	default:
	  UNEXPECTED_CONDITION (T_("DibChunkerImpl::ReadScanLine"));
	}
    }

  return (isBlackAndWhite);
}

/* _________________________________________________________________________

   DibChunkerImpl::AddScanLine
   _________________________________________________________________________ */

void
DibChunkerImpl::AddScanLine ()
{
  unsigned long n = BytesPerLine();
  memcpy (&pBits[n * numScanLines], pScanLine, n);
  numScanLines += 1;
}

/* _________________________________________________________________________

   DibChunkerImpl::BeginChunk
   _________________________________________________________________________ */

void
DibChunkerImpl::BeginChunk ()
{
  inChunk = true;
  numScanLines = 0;
  blankLines = 0;
  leftPos = BytesPerLine();
  rightPos = 0;
  isBlackAndWhite = true;
  yPosChunk = yPos;
}

/* _________________________________________________________________________

   DibChunkerImpl::Monochromize24
   _________________________________________________________________________ */

void
DibChunkerImpl::Monochromize24 (/*[in]*/ const unsigned char *	pSrc,
				/*[out]*/ unsigned char *	pDst,
				/*[in]*/ unsigned long		width)
{
  unsigned char byte = 0;
  unsigned long idx;
  for (idx = 0; idx < width; ++ idx)
    {
      unsigned n = (idx % 8);
      COLORREF clr = RGB(pSrc[idx * 3 + 2],
			 pSrc[idx * 3 + 1],
			 pSrc[idx * 3 + 0]);
      if (clr != RGB_WHITE)
	{
	  byte |= (1 << (7 - n));
	}
      if (n == 7)
	{
	  *pDst++ = byte;
	  byte = 0;
	}
    }
  if ((idx % 8) > 0)
    {
      *pDst++ = byte;
    }
}

/* _________________________________________________________________________

   DibChunkerImpl::EndChunk
   _________________________________________________________________________ */

void
DibChunkerImpl::EndChunk ()
{
  // cut off white lines 
  numScanLines -= blankLines;

  long bytesInChunk = (rightPos - leftPos + 1);
  int x;
  long biWidth;
  long bytesPerLine;
  bool monochromize24 = false;
  switch (bitmapInfoHeader.biBitCount)
    {
    case 1:
      x = leftPos * 8;
      biWidth = bytesInChunk * 8;
      bytesPerLine = BytesPerLine(biWidth, 1);
      break;
    case 4:
      x = leftPos * 2;
      biWidth = bytesInChunk * 2;
      bytesPerLine = BytesPerLine(biWidth, 4);
      break;
    case 8:
      x = leftPos;
      biWidth = bytesInChunk;
      bytesPerLine = BytesPerLine(biWidth, 8);
      break;
    case 24:
      MIKTEX_ASSERT (leftPos % 3 == 0);
      x = leftPos / 3;
      MIKTEX_ASSERT (bytesInChunk % 3 == 0);
      biWidth = bytesInChunk / 3;
#if 1
      if (isBlackAndWhite)
	{
	  monochromize24 = true;
	  bytesPerLine = BytesPerLine(biWidth, 1);
	}
      else
#endif
	{
	  bytesPerLine = BytesPerLine(biWidth, 24);
	}
      break;
    default:
      UNEXPECTED_CONDITION (T_("DibChunkerImpl::EndChunk"));
    }

  SmartPointer<DibChunkImpl>
    pChunk (new DibChunkImpl(bytesPerLine, numScanLines));
  
  pChunk->SetX (x);

  // make chunked bitmap info header
  BITMAPINFOHEADER bitmapinfoheader;
  bitmapinfoheader = this->bitmapInfoHeader;
  bitmapinfoheader.biHeight = numScanLines;
  bitmapinfoheader.biWidth = biWidth;
  bitmapinfoheader.biSizeImage = 0;
  const RGBQUAD * pColors = 0;
  unsigned numColors = 0;
  if (monochromize24)
    {
      bitmapinfoheader.biBitCount = 1;
      numColors = 2;
      pColors = &whiteAndBlack[0];
    }
  else
    {
      numColors = this->numColors;
      pColors = this->pColors;
    }
  // <fixme>okay?
  int y = yPosChunk;
  y += bitmapinfoheader.biHeight - 1;
  y = this->bitmapInfoHeader.biHeight - y;
  pChunk->SetY (y);
  // </fixme>
  unsigned char * pBits =
    reinterpret_cast<unsigned char*>(pChunk->GetBits2());
  for (unsigned long i = 0; i < numScanLines; ++ i)
    {
      const unsigned char * pSrc = this->pBits + i * BytesPerLine() + leftPos;
      unsigned char * pDest = pBits + i * bytesPerLine;
      memset (pDest, 0, bytesPerLine);
      if (monochromize24)
	{
	  Monochromize24 (pSrc, pDest, biWidth);
	}
      else
	{
	  memcpy (pDest, pSrc, bytesInChunk);
	}
    }
  trace_dib->WriteFormattedLine
    (T_("libdib"),
     T_("shipping chunk: x=%ld, y=%ld, w=%ld, h=%ld, monochromized=%s"),
     pChunk->GetX(),
     pChunk->GetY(),
     bitmapinfoheader.biWidth,
     bitmapinfoheader.biHeight,
     (monochromize24
      ? T_("true")
      : T_("false")));
  inChunk = false;
  pChunk->SetBitmapInfo (bitmapinfoheader, numColors, pColors);
  pCallback->OnNewChunk (pChunk.Get());
}

/* _________________________________________________________________________

   DibChunkerImpl::Process
   _________________________________________________________________________ */

bool
DibChunkerImpl::Process (/*[in]*/ unsigned long		flags,
			 /*[in]*/ unsigned long		chunkSize,
			 /*[in]*/ IDibChunkerCallback *	pCallback)
{
  this->processingFlags = flags;
  this->pCallback = pCallback;
  numBytesRead = 0;
  if (! ReadBitmapFileHeader())
    {
      return (false);
    }
  ReadBitmapInfo ();
  if (BytesPerLine() > chunkSize)
    {
       INVALID_ARGUMENT (T_("DibChunkerImpl::Process"), 0);
    }
  pBits = new unsigned char[chunkSize];
  try
    {
      inChunk = false;
      blankLines = 0;
      for (yPos = 0; yPos < bitmapInfoHeader.biHeight; yPos += 1)
	{
	  unsigned long left;
	  unsigned long right;
	  if (! ReadScanLine(left, right))
	    {
	      isBlackAndWhite = false;
	    }
	  if (left == UINT_MAX)
	    {			// white line
	      if (! inChunk)
		{
		  continue;
		}
	      if (inChunk && (flags & RemoveBlankLines))
		{
		  EndChunk ();
		  continue;
		}
	      blankLines += 1;
	    }
	  else
	    {
	      blankLines = 0;
	    }
	  if (! inChunk)
	    {
	      BeginChunk ();
	    }
	  if ((numScanLines + 1) * BytesPerLine() > chunkSize)
	    {
	      if (! (flags & CreateChunks))
		{
		  FATAL_MIKTEX_ERROR (T_("DibChunkerImpl::Process"),
				      T_("Chunk size exceeded."),
				      0);
		}
	      EndChunk ();
	      BeginChunk ();
	    }
	  AddScanLine ();
	  if (left < this->leftPos)
	    {
	      this->leftPos = left;
	    }
	  if (right != UINT_MAX && right > this->rightPos)
	    {
	      this->rightPos = right;
	    }
	}
      if (inChunk)
	{
	  EndChunk ();
	}
      delete [] pBits;
      pBits = 0;
      return (true);
    }
  catch (const exception &)
    {
      delete [] pBits;
      pBits = 0;
      throw;
    }
}
