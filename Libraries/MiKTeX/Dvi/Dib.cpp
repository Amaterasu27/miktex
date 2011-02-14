/* Dib.cpp:

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

#include "StdAfx.h"

#include "internal.h"

/* _________________________________________________________________________

   Dib::Dib
   _________________________________________________________________________ */

Dib::Dib ()
  : hFile (INVALID_HANDLE_VALUE),
    hMap (0),
    pBitmapFileHeader (0)
{
}

/* _________________________________________________________________________

   Dib::Dib
   _________________________________________________________________________ */

Dib::Dib (/*[in]*/ const PathName & path)
  : hFile (INVALID_HANDLE_VALUE),
    hMap (0),
    pBitmapFileHeader (0)
{
  AttachFile (path.Get());
}

/* _________________________________________________________________________

   Dib::~Dib
   _________________________________________________________________________ */

Dib::~Dib ()
{
  try
  {
    Clear ();
  }
  catch (const exception &)
  {
  }
}

/* _________________________________________________________________________

   Dib::Clear
   _________________________________________________________________________ */

void
Dib::Clear ()
{
  if (hMap != 0)
  {
    CloseHandle (hMap);
    hMap = 0;
  }
  if (hFile != INVALID_HANDLE_VALUE)
  {
    CloseHandle (hFile);
    hFile = INVALID_HANDLE_VALUE;
  }
  if (pBitmapFileHeader != 0)
  {
    UnmapViewOfFile (pBitmapFileHeader);
    pBitmapFileHeader = 0;
  }
}

/* _________________________________________________________________________

   Dib::GetSize
   _________________________________________________________________________ */

void
Dib::GetSize (/*[in]*/ long *	pcx,
	      /*[in]*/ long *	pcy)
  const
{
  *pcx = static_cast<LONG>(GetBitmapInfo()->bmiHeader.biWidth);
  *pcy = static_cast<LONG>(GetBitmapInfo()->bmiHeader.biHeight);
}

/* _________________________________________________________________________

   Dib::Render
   _________________________________________________________________________ */

void
Dib::Render (/*[in]*/ HDC	hdc,
	     /*[in]*/ int	x,
	     /*[in]*/ int	y,
	     /*[in]*/ int	cx,
	     /*[in]*/ int	cy)
{
  if (cx < 0)
  {
    cx = GetWidth();
  }
  if (cy < 0)
  {
    cy = GetHeight();
  }
  SetStretchBltMode (hdc, COLORONCOLOR);
  if (StretchDIBits(hdc,
    x,
    y,
    cx,
    cy,
    0,
    0,
    GetWidth(),
    GetHeight(),
    GetBits(),
    GetBitmapInfo(),
    DIB_RGB_COLORS,
    SRCCOPY)
      == GDI_ERROR)
  {
    FATAL_WINDOWS_ERROR ("StretchDIBits", 0);
  }
}

/* _________________________________________________________________________

   Dib::AttachFile
   _________________________________________________________________________ */

void
Dib::AttachFile (/*[in]*/ const char *	lpszFileName)
{
  hFile = CreateFile(lpszFileName,
    GENERIC_READ,
    FILE_SHARE_READ,
    0,
    OPEN_EXISTING,
    FILE_FLAG_RANDOM_ACCESS,
    0);
  if (hFile == INVALID_HANDLE_VALUE)
  {
    FATAL_WINDOWS_ERROR ("CreateFile", lpszFileName);
  }
  hMap = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
  if (hMap == 0)
  {
    FATAL_WINDOWS_ERROR ("CreateFileMapping", lpszFileName);
  }
  void * pv = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
  if (pv == 0)
  {
    FATAL_WINDOWS_ERROR ("MapViewOfFile", lpszFileName);
  }
  pBitmapFileHeader = reinterpret_cast<BITMAPFILEHEADER *>(pv);
  if (pBitmapFileHeader->bfType != 0x4d42)
  {
    FATAL_MIKTEX_ERROR ("Dib::AttachFile",
      T_("Not a BMP file."),
      lpszFileName);
  }
}
