/* BitmapPrinter.cpp:

   Copyright (C) 2003-2006 Christian Schenk

   This file is part of MTPrint.

   MTPrint is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   MTPrint is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MTPrint; if not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

#include "BitmapPrinter.h"

/* _________________________________________________________________________

   BitmapPrinter::BitmapPrinter
   _________________________________________________________________________ */

BitmapPrinter::BitmapPrinter (/*[in]*/ const PRINTINFO &	printInfo,
			      /*[in]*/ bool			printNothing)

  : Printer (printInfo, printNothing)

{
}

/* _________________________________________________________________________

   BitmapPrinter::PrintChunk
   _________________________________________________________________________ */

void
BitmapPrinter::PrintChunk (/*[in]*/ DibChunk * pChunk)
{
  BITMAPINFOHEADER bitmapInfoHeader = pChunk->GetBitmapInfoHeader();

  unsigned nColors =
    (bitmapInfoHeader.biBitCount == 24
     ? 0
     : 1 << bitmapInfoHeader.biBitCount);

  size_t size = sizeof(BITMAPINFO) + nColors * sizeof(RGBQUAD);

  BITMAPINFO * pBMI = reinterpret_cast<BITMAPINFO*>(_alloca(size));

  pBMI->bmiHeader = bitmapInfoHeader;
  memcpy (&pBMI->bmiColors, pChunk->GetColors(), nColors * sizeof(RGBQUAD));

  if (! dryRun)
    {
      int n =
	StretchDIBits(GetDC(),
		      pChunk->GetX() - offsetX,
		      pChunk->GetY() - offsetY,
		      bitmapInfoHeader.biWidth,
		      bitmapInfoHeader.biHeight,
		      0,
		      0,
		      bitmapInfoHeader.biWidth,
		      bitmapInfoHeader.biHeight,
		      pChunk->GetBits(),
		      pBMI,
		      DIB_RGB_COLORS,
		      SRCCOPY);
      
      if (n == GDI_ERROR)
	{
	  FATAL_MIKTEX_ERROR (T_("BitmapPrinter::PrintChunk"),
			      T_("StretchDIBits() failed for some reason."),
			      0);
	}
    }
}

/* _________________________________________________________________________

   BitmapPrinter::OnNewChunk
   _________________________________________________________________________ */

void
DIBCHUNKERCALL
BitmapPrinter::OnNewChunk (/*[in]*/ DibChunk * pChunk)
{
  if (! JobStarted())
    {
      StartJob ();
      int rasterCaps = ::GetDeviceCaps(GetDC(), RASTERCAPS);
      if ((rasterCaps & RC_STRETCHDIB) == 0)
	{
	  FATAL_MIKTEX_ERROR
	    (T_("BitmapPrinter::OnNewChunk"),
	     T_("StretchDIBits() is not supported."),
	     0);
	}
      offsetX = ::GetDeviceCaps(GetDC(), PHYSICALOFFSETX);
      offsetY = ::GetDeviceCaps(GetDC(), PHYSICALOFFSETY);
      trace_mtprint->WriteFormattedLine
	(T_("mtprint"),
	 T_("PHYSICALOFFSETX: %d"),
	 offsetX);
      trace_mtprint->WriteFormattedLine
	(T_("mtprint"),
	 T_("PHYSICALOFFSETY: %d"),
	 offsetY);
    }
  if (! PageStarted())
    {
      StartPage ();
    }
  PrintChunk (pChunk);
}

/* _________________________________________________________________________

   BitmapPrinter::Read
   _________________________________________________________________________ */

size_t
DIBCHUNKERCALL
BitmapPrinter::Read (/*[out]*/ void *	pBuf,
		     /*[in]*/ size_t	size)
{
  return (stream.Read(pBuf, size));
}

/* _________________________________________________________________________

   BitmapPrinter::Print
   _________________________________________________________________________ */

void
BitmapPrinter::Print (/*[in]*/ FILE *	pfileDibStream)
{
  stream.Attach (pfileDibStream);
  try
    {
      auto_ptr<DibChunker> pChunker (DibChunker::Create());
      const size_t CHUNK_SIZE = 1024 * 64;
      try
	{
	  do
	    {
	      if (PageStarted())
		{
		  EndPage ();
		}
	    }
	  while (pChunker->Process(DibChunker::Default,
				   CHUNK_SIZE,
				   this));
	}
      catch (const exception &)
	{
	  Finalize ();
	  throw;
	}
      Finalize ();
    }
  catch (const exception &)
    {
      stream.Detach ();
      throw;
    }
  stream.Detach ();
}
