/* Dib.h:							-*- C++ -*-

   Copyright (C) 1996-2011 Christian Schenk

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

class Dib
{
public:
  Dib ();

public:
  Dib (/*[in]*/ const PathName & path);

public:
  ~Dib ();

public:
  void
  GetSize (/*[in]*/ long *	pcx,
	   /*[in]*/ long *	pcy)
    const;

public:
  void
  Render (/*[in]*/ HDC	    hdc,
	  /*[in]*/ int	    x,
	  /*[in]*/ int	    y,
	  /*[in]*/ int	    cx,
	  /*[in]*/ int	    cy);

private:
  void
  AttachFile (/*[in]*/ const char * lpszFileName);

private:
  const BITMAPINFO *
  GetBitmapInfo ()
    const
  {
    MIKTEX_ASSERT (pBitmapFileHeader != 0);
    return (reinterpret_cast<const BITMAPINFO *>
	    (reinterpret_cast<const BYTE *>(pBitmapFileHeader)
	     + sizeof(*pBitmapFileHeader)));
  }

private:
  unsigned long
  GetWidth ()
    const
  {
    return (GetBitmapInfo()->bmiHeader.biWidth);
  }

private:
  unsigned long
  GetHeight ()
    const
  {
    return (GetBitmapInfo()->bmiHeader.biHeight);
  }

private:
  const void *
  GetBits ()
    const
  {
    MIKTEX_ASSERT (pBitmapFileHeader != 0);
    return (reinterpret_cast<const BYTE *>(pBitmapFileHeader)
	    + pBitmapFileHeader->bfOffBits);
  }
 
private:
  void
  Clear ();

private:
  HANDLE hFile;

private:
  HANDLE hMap;

private:
  BITMAPFILEHEADER * pBitmapFileHeader;
};
