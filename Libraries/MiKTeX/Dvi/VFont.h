/* vfont.h:							-*- C++ -*-

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
class VfChar;

class VFont : public DviFont
{
public:
  VFont (/*[in]*/ DviImpl *	pDvi,
	 /*[in]*/ int		checkSum,
	 /*[in]*/ int		scaledSize,
	 /*[in]*/ int		designSize,
	 /*[in]*/ const char *	lpszAreaName,
	 /*[in]*/ const char *	lpszFontName,
	 /*[in]*/ const char *	lpszFileName,
	 /*[in]*/ double	tfmConv,
	 /*[in]*/ double	conv,
	 /*[in]*/ int		mag,
	 /*[in]*/ const char *	lpszMetafontMode,
	 /*[in]*/ int		baseDpi);

public:
  virtual
  ~VFont ();

public:
  VfChar *
  GetCharAt (/*[in]*/ int idx);

public:
  const FontMap &
  GetFontMap ()
    const
  {
    return (fontMap);
  }

private:
  void
  Read ();

private:
  void
  ReadPreamble (/*[in]*/ InputStream & inputStream);

private:
  void
  ReadFontDefsAndCharPackets (/*[in]*/ InputStream & inputStream);

private:
  void
  ReadFontDef (/*[in]*/ InputStream &	inputStream,
	       /*[in]*/ short		fntDefX);

private:
  void
  ReadCharPacket (/*[in]*/ InputStream &	inputStream,
		  /*[in]*/ short		size);

private:
  void
  ReadPostamble (/*[in]*/ InputStream & inputStream);

private:
  int mag;

private:
  string metafontMode;

private:
  int baseDpi;

private:
  int shrinkFactor;

private:
  map<int, class VfChar *> characterTable;

private:
  FontMap fontMap;

private:
  auto_ptr<TraceStream> trace_error;

private:
  auto_ptr<TraceStream> trace_vfont;
};
