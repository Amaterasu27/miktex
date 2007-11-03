/* pkchar.h:							-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of the MiKTeX DVI Library.

   The MiKTeX DVI Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2, or (at your option) any later version.

   The MiKTeX DVI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the MiKTeX DVI Library; if not, write to the
   Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139,
   USA.  */

#pragma once

class InputStream;
class DviFont;

class PkChar : public DviChar
{
public:
  PkChar (/*[in]*/ DviFont * pfont = 0);

public:
  virtual
  ~PkChar ();

public:
  void
  Read (/*[in]*/ InputStream &	inputstream,
	/*[in]*/ int		flag);

public:
  int
  GetWidthShr (/*[in]*/ int shrinkFactor)
  {
    return (shrinkFactor == 1
	    ? rasterWidth
	    : WidthShrink(shrinkFactor, rasterWidth) + 1);
  }

public:
  int
  GetHeightShr (/*[in]*/ int shrinkFactor)
  {
    return (shrinkFactor == 1
	    ? rasterHeight
	    : WidthShrink(shrinkFactor, rasterHeight) + 1);
  }

public:
  int
  GetWidthUns ()
  {
    return (rasterWidth);
  }

public:
  int
  GetHeightUns ()
  {
    return (rasterHeight);
  }

public:
  int
  GetLeftShr (/*[in]*/ int shrinkFactor,
	      /*[in]*/ int x)
  {
    return (PixelShrink(shrinkFactor, x) - cxOffset / shrinkFactor);
  }

public:
  int
  GetTopShr (/*[in]*/ int shrinkFactor,
	     /*[in]*/ int y)
  {
    return (PixelShrink(shrinkFactor, y) - cyOffset / shrinkFactor);
  }

public:
  int
  GetLeftUns (/*[in]*/ int x)
  {
    return (x - cxOffset);
  }

public:
  int
  GetTopUns (/*[in]*/ int y)
  {
    return (y - cyOffset);
  }

public:
  const void *
  GetBitmap (/*[in]*/ int shrinkFactor);

public:
  void
  Print ();

  // 16-bit raster word, big-endian
private:
  typedef short int RASTERWORD;

private:
  int
  GetLower3 ();

private:
  bool
  IsShort ();

private:
  bool
  IsExtendedShort ();

private:
  bool
  IsLong ();

private:
  unsigned long
  CountBits (/*[in]*/ const RASTERWORD *	pRasterWord,
	     /*[in]*/ int			xStart,
	     /*[in]*/ int			rasterWordsPerLine,
	     /*[in]*/ int			w,
	     /*[in]*/ int			h);

private:
  void
  Unpack ();

private:
  void *
  Shrink (/*[in]*/ int shrinkFactor);

private:
  inline
  int
  PixelShrink (/*[in]*/ int shrinkFactor,
	       /*[in]*/ int pxl);

private:
  inline
  int
  WidthShrink (/*[in]*/ int shrinkFactor,
	       /*[in]*/ int pxl);

private:
  typedef map<int, void *> MAPINTTORASTER;

private:
  MAPINTTORASTER bitmaps;

  // flag byte  
private:
  int flag;

  // length (in bytes) of packed raster data
private:
  int packetSize;

  // width (in pixels) of minimum bounding box
private:
  int rasterWidth;

  // height (in pixels) of minimum bounding box
private:
  int rasterHeight;

  // horizontal offset
private:
  int cxOffset;

  // vertical offset
private:
  int cyOffset;
  
  // the packed raster data
private:
  unsigned char * pPackedRaster;

  // the unpacked raster data
private:
  RASTERWORD * pUnpackedRaster;

  // rwords per line
private:
  int numberOfRasterWords;
  
  // contains the powers of two
private:
  static const int powerOfTwo[32];

  // contains various rows of black
private:
  static const int gpower[33];	// <fixme/>

private:
  class Unpacker
  {
  public:
    Unpacker (/*[in]*/ const unsigned char *	p,
	      /*[in]*/ int			dynf)
      : pRaster (p),
	bitWeight (0),
	dynf (dynf)
    {
    }

  public:
    void
    ResetBitWeight ()
    {
      bitWeight = 0;
    }

  public:
    int
    GetPackedNumber ();

    // dynamic packing variable
  private:
    int dynf;
  
  private:
    int
    GetByte ()
    {
      return (*pRaster++);
    }

    // the byte we are currently decimating
  private:
    int currentByte;

    // weight of the current bit
  private:
    int bitWeight;
  
    // weight of the current position
  public:			// <fixme/>
    int rasterWordHeight;

  public:			// <fixme/>
    int repeatCount;

  private:
    const unsigned char * pRaster;

  private:
    int
    GetNybble ();

  public:
    bool
    GetBit ();
  };

private:
  auto_ptr<TraceStream> trace_error;

private:
  auto_ptr<TraceStream> trace_pkchar;
};
