/* Tfm.h:							-*- C++ -*-

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

class DviChar;
class DviImpl;

class Tfm : public DviFont
{
public:
  Tfm (/*[in]*/ DviImpl *		pDvi,
       /*[in]*/ int			checksum,
       /*[in]*/ int			scaledSize,
       /*[in]*/ int			designSize,
       /*[in]*/ const char *		lpszAreaName,
       /*[in]*/ const char *		lpszFontName,
       /*[in]*/ const char *	lpszFileName,
       /*[in]*/ double			tfmConf,
       /*[in]*/ double			conv);

public:
  virtual
  ~Tfm ();

public:
  DviChar *
  operator[] (/*[in]*/ unsigned long idx);

private:
  bool
  Make (/*[in]*/ const string &		name);

public:
  void
  Read ();

private:
  map<int, DviChar *> dviChars;

private:
  auto_ptr<TraceStream> log_error;

private:
  auto_ptr<TraceStream> log_tfm;
};
