/* Dib.h:							-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of Yap.

   Yap is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   Yap is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Yap; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#pragma once

class Dib
{
public:
  Dib ();

public:
  Dib (/*[in]*/ const PathName & path);

public:
  Dib (/*[in]*/ const TempFile & tempFile);

public:
  ~Dib ();

public:
  void
  GetSize (/*[in]*/ long *	pcx,
	   /*[in]*/ long *	pcy)
    const;

public:
  void
  Draw (/*[in]*/ CDC *	pDC,
	/*[in]*/ int	x,
	/*[in]*/ int	y,
	/*[in]*/ int	cx,
	/*[in]*/ int	cy);

public:
  void
  AttachFile (/*[in]*/ const MIKTEXCHAR * lpszFileName);

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

private:
  TempFile tempFile;
};
