/* char.cpp:

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

#include "StdAfx.h"

#include "common.h"

/* _________________________________________________________________________

   DviChar::DviChar
   _________________________________________________________________________ */

DviChar::DviChar (/*[in]*/ DviFont * pDviFont)
  : charCode (0),
    cx (0),
    cy (0),
    pDviFont (pDviFont),
    tfm (0)
{
}

/* _________________________________________________________________________

   DviChar::~DviChar
   _________________________________________________________________________ */

DviChar::~DviChar ()
{
}
