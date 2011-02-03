/* pkfont.h:							-*- C++ -*-

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

#if defined(HAVE_UNORDERED_MAP)
typedef tr1::unordered_map<int, PkChar *> MAPNUMTOPKCHAR;
#else
typedef map<int, PkChar *> MAPNUMTOPKCHAR;
#endif

class PkFont : public DviFont
{
public:
  PkFont (/*[in]*/ DviImpl *	pDvi,
	  /*[in]*/ int		checksum,
	  /*[in]*/ int		scaledSize,
	  /*[in]*/ int		designSize,
	  /*[in]*/ const char *	lpszAreaName,
	  /*[in]*/ const char *	lpszFontName,
	  /*[in]*/ const char *	lpszFileName,
	  /*[in]*/ double	tfmConf,
	  /*[in]*/ double	conv,
	  /*[in]*/ int		mag,
	  /*[in]*/ const char *	lpszMetafontMode,
	  /*[in]*/ int		baseDpi);

public:
  virtual
  ~PkFont ();

public:
  PkChar *
  operator[] (/*[in]*/ unsigned long idx);

private:
  void
  AddSize (/*[in]*/ int rhsize);

private:
  void
  AddDpi (/*[in]*/ int hsize);

private:
  int
  CheckDpi (/*[in]*/ int	dpi,
	    /*[in]*/ int	baseDpi);

private:
  bool
  Make (/*[in]*/ const string &		name,
	/*[in]*/ int			dpi,
	/*[in]*/ int			baseDpi,
	/*[in]*/ const string &	metafontMode);

private:
  bool
  MakeTFM (/*[in]*/ const string &	name);

public:
  void
  Read ();

public:
  void
  ReadTFM ();

private:
  int mag;

private:
  string metafontMode;

private:
  int baseDpi;

private:
  MAPNUMTOPKCHAR pkChars;

private:
  int existSizes[30];

private:
  bool checkDpi;

private:
  int hppp;

private:
  int vppp;

private:
  auto_ptr<TraceStream> trace_error;

private:
  auto_ptr<TraceStream> trace_pkfont;
};
