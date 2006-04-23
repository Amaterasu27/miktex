/* char.h:							-*- C++ -*-

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

class DviFont;

class DviChar
{

public:
  DviChar (/*[in]*/ DviFont * pDviFont = 0);

public:
  virtual
  ~DviChar ();

public:
  int
  GetWidth ()
  {
    return (cx);
  }

public:
  void
  SetWidth (/*[in]*/ int cx)
  {
    this->cx = cx;
  }

public:
  int
  GetDviWidth ()
  {
    return (tfm);
  }

public:
  void
  SetDviWidth (/*[in]*/ int tfm)
  {
    this->tfm = tfm;
  }

public:
  int
  GetCharacterCode ()
  {
    return (charCode);
  }

public:
  void
  SetCharacterCode (/*[in]*/ int charCode)
  {
    this->charCode = charCode;
  }
  
public:
  inline
  int
  GetScaledAt ();
  
  // character code
protected:
  int charCode;

  // glyph width (in DVI units)
protected:
  int tfm;

  // horizontal escapement (in pixels)
protected:
  int cx;

  // vertical escapement (in pixels)
protected:
  int cy;

protected:
  DviFont * pDviFont;
};
