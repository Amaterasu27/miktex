/* DviDraw.h:							-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of Yap.

   Yap is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   Yap is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with Yap; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#pragma once

class DviDoc;

struct foreback
{
  unsigned numcolors;
  COLORREF fore;
  COLORREF back;
};

/* _________________________________________________________________________

   operator< (foreback, foreback)
   _________________________________________________________________________ */

inline bool
operator< (const foreback &	fb1,
	   const foreback &	fb2)
{
  return (fb1.numcolors < fb2.numcolors
	  ? true
	  : (fb1.numcolors > fb2.numcolors
	     ? false
	     : (fb1.fore < fb2.fore
		? true
		: (fb1.fore > fb2.fore
		   ? false
		   : fb1.back < fb2.back))));
}

/* _________________________________________________________________________

   DviDraw
   _________________________________________________________________________ */

class DviDraw
{
protected:
  DviDraw ();

protected:
  virtual
  ~DviDraw ();

protected:
  void
  DrawDviBitmaps (/*[in]*/ CDC *	pDC,
		  /*[in]*/ DviDoc *	pDoc,
		  /*[in]*/ DviPage *	pPage);

protected:
  void
  DrawDibChunks (/*[in]*/ CDC *		pDC,
		 /*[in]*/ DviDoc *	pDoc,
		 /*[in]*/ DviPage *	pPage);

protected:
  void
  DrawRules (/*[in]*/ CDC *	pDC,
	     /*[in]*/ bool	blackBoards,
	     /*[in]*/ DviDoc *	pDoc,
	     /*[in]*/ DviPage *	pPage);

protected:
  void
  InitializeDviBitmapPalettes ();

private:
  HPALETTE
  CreateDviBitmapPalette (/*[in]*/ COLORREF foreColor,
			  /*[in]*/ COLORREF backColor,
			  /*[in]*/ size_t	nColors);

private:
  BITMAPINFO *
  MakeBitmapInfo (/*[in]*/ size_t	width,
		  /*[in]*/ size_t	height,
		  /*[in]*/ size_t	dpi,
		  /*[in]*/ size_t	bytesPerLine,
		  /*[in]*/ DviDoc *	pDoc);

protected:
  map<foreback, HPALETTE> foregroundPalettes;

protected:
  double gamma;

protected:
  enum {
    DVIVIEW_DISPLAY = 0,
    DVIVIEW_PRINTER = 1
  };

private:
  LPBITMAPINFO bitmapInfoTable[2];
};
