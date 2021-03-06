/* font.cpp:

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

#include "internal.h"

/* _________________________________________________________________________

   DviFont::DviFont
   _________________________________________________________________________ */

DviFont::DviFont (/*[in]*/ DviImpl *	pDviImpl,
		  /*[in]*/ int		checkSum,
		  /*[in]*/ int		scaledAt,
		  /*[in]*/ int		designSize,
		  /*[in]*/ const char *	lpszArea,
		  /*[in]*/ const char *	lpszName,
		  /*[in]*/ const char *	lpszFileName,
		  /*[in]*/ double	tfmConf,
		  /*[in]*/ double	conv)
  : pDviImpl (pDviImpl),
    checkSum (checkSum),
    scaledAt (scaledAt),
    designSize (designSize),
    tfmConv (tfmConf),
    conv (conv)
{
  UNUSED_ALWAYS (lpszArea);
  dviInfo.notLoadable = false;
  dviInfo.name = lpszName;
  dviInfo.fileName = lpszFileName;
}

/* _________________________________________________________________________

   DviFont::~DviFont
   _________________________________________________________________________ */

DviFont::~DviFont ()
{
}

/* _________________________________________________________________________

   DviFont::GetInterWordSpacing
   _________________________________________________________________________ */

int
DviFont::GetInterWordSpacing ()
{
  Read ();
  return (static_cast<int>(0.2 * scaledAt));
}

/* _________________________________________________________________________

   DviFont::GetBackSpacing
   _________________________________________________________________________ */

int
DviFont::GetBackSpacing ()
{
  Read ();
  return (static_cast<int>(0.9 * scaledAt));
}

/* _________________________________________________________________________

   DviFont::GetLineSpacing
   _________________________________________________________________________ */

int
DviFont::GetLineSpacing ()
{
  Read ();
  return (static_cast<int>(0.8 * scaledAt));
}
