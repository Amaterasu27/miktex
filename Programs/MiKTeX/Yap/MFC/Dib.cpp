/* Dib.cpp:

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

#include "StdAfx.h"

#include "yap.h"

#include "Dib.h"

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

   Dib::Dib
   _________________________________________________________________________ */

Dib::Dib (/*[in]*/ const TempFile & tempFile)
  : hFile (INVALID_HANDLE_VALUE),
    hMap (0),
    pBitmapFileHeader (0),
    tempFile (tempFile)
{
  AttachFile (tempFile.Get());
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

   Dib::Draw
   _________________________________________________________________________ */

void
Dib::Draw (/*[in]*/ CDC *	pDC,
	   /*[in]*/ int		x,
	   /*[in]*/ int		y,
	   /*[in]*/ int		cx,
	   /*[in]*/ int		cy)
{
  pDC->SetStretchBltMode (COLORONCOLOR);
  if (::StretchDIBits(pDC->GetSafeHdc(),
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
      FATAL_WINDOWS_ERROR (T_("StretchDIBits"), 0);
    }
}

/* _________________________________________________________________________

   Dib::AttachFile
   _________________________________________________________________________ */

void
Dib::AttachFile (/*[in]*/ const MIKTEXCHAR *	lpszFileName)
{
  YapLog (T_("reading bitmap file %s"), lpszFileName);
  hFile =
    CreateFile(lpszFileName,
	       GENERIC_READ,
	       FILE_SHARE_READ,
	       0,
	       OPEN_EXISTING,
	       FILE_FLAG_RANDOM_ACCESS,
	       0);
  if (hFile == INVALID_HANDLE_VALUE)
    {
      FATAL_WINDOWS_ERROR (T_("CreateFile"), lpszFileName);
    }
  hMap = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
  if (hMap == 0)
    {
      FATAL_WINDOWS_ERROR (T_("CreateFileMapping"), lpszFileName);
    }
  void * pv = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
  if (pv == 0)
    {
      FATAL_WINDOWS_ERROR (T_("MapViewOfFile"), lpszFileName);
    }
  pBitmapFileHeader = reinterpret_cast<BITMAPFILEHEADER *>(pv);
  if (pBitmapFileHeader->bfType != 0x4d42)
    {
      FATAL_MIKTEX_ERROR (T_("Dib::AttachFile"),
			  T_("Not a BMP file."),
			  lpszFileName);
    }
}
