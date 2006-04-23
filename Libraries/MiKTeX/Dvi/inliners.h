/* inliners.h: inlined functions				-*- C++ -*-

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

/* _________________________________________________________________________

   PkChar::PixelShrink
   _________________________________________________________________________ */

inline
int
PkChar::PixelShrink (/*[in]*/ int shrinkFactor,
		     /*[in]*/ int pxl)
{
  return (pDviFont->PixelShrink(shrinkFactor, pxl));
}  

/* _________________________________________________________________________

   PkChar::WidthShrink
   _________________________________________________________________________ */

inline
int
PkChar::WidthShrink (/*[in]*/ int shrinkFactor,
		     /*[in]*/ int pxl)
{
  return (pDviFont->WidthShrink(shrinkFactor, pxl));
}  

/* _________________________________________________________________________

   DviPageImpl::PixelShrink
   _________________________________________________________________________ */

inline
int
DviPageImpl::PixelShrink (/*[in]*/ int shrinkFactor,
			  /*[in]*/ int pxl)
{
  return (pDviImpl->PixelShrink(shrinkFactor, pxl));
}

/* _________________________________________________________________________

   DviPageImpl::WidthShrink
   _________________________________________________________________________ */

inline
int
DviPageImpl::WidthShrink (/*[in]*/ int shrinkFactor,
			  /*[in]*/ int pxl)
{
  return (pDviImpl->WidthShrink(shrinkFactor, pxl));
}

/* _________________________________________________________________________

   DviItem::GetWidthShr
   _________________________________________________________________________ */

inline
int
DviItem::GetWidthShr (/*[in]*/ int shrinkFactor)
{
  return (pPkChar->GetWidthShr(shrinkFactor));
}

/* _________________________________________________________________________

   DviItem::GetHeightShr
   _________________________________________________________________________ */

inline
int
DviItem::GetHeightShr (/*[in]*/ int shrinkFactor)
{
  return (pPkChar->GetHeightShr(shrinkFactor));
}

/* _________________________________________________________________________

   DviItem::GetLeftShr
   _________________________________________________________________________ */

inline
int
DviItem::GetLeftShr (/*[in]*/ int shrinkFactor)
{
  return (pPkChar->GetLeftShr(shrinkFactor, x));
}

/* _________________________________________________________________________

   DviItem::GetTopShr
   _________________________________________________________________________ */

inline
int
DviItem::GetTopShr (/*[in]*/ int shrinkFactor)
{
  return (pPkChar->GetTopShr(shrinkFactor, y));
}

/* _________________________________________________________________________

   DviItem::GetWidthUns
   _________________________________________________________________________ */

inline
int
DviItem::GetWidthUns ()
{
  return (pPkChar->GetWidthUns());
}

/* _________________________________________________________________________

   DviItem::GetHeightUns
   _________________________________________________________________________ */

inline
int
DviItem::GetHeightUns ()
{
  return (pPkChar->GetHeightUns());
}

/* _________________________________________________________________________

   DviItem::GetLeftUns
   _________________________________________________________________________ */

inline
int
DviItem::GetLeftUns ()
{
  return (pPkChar->GetLeftUns(x));
}

/* _________________________________________________________________________

   DviItem::GetTopUns
   _________________________________________________________________________ */

inline
int
DviItem::GetTopUns ()
{
  return (pPkChar->GetTopUns(y));
}

/* _________________________________________________________________________

   DviChar::GetScaledAt
   _________________________________________________________________________ */

inline
int
DviChar::GetScaledAt ()
{
  return (pDviFont->GetScaledAt());
}

/* _________________________________________________________________________

   DviFont::PixelShrink
   _________________________________________________________________________ */

inline
int
DviFont::PixelShrink (/*[in]*/ int shrinkFactor,
		      /*[in]*/ int pxl)
{
  return (pDviImpl->PixelShrink(shrinkFactor, pxl));
}

/* _________________________________________________________________________

   DviFont::WidthShrink
   _________________________________________________________________________ */

inline
int
DviFont::WidthShrink (/*[in]*/ int shrinkFactor,
		      /*[in]*/ int pxl)
{
  return (pDviImpl->WidthShrink (shrinkFactor, pxl));
}

/* _________________________________________________________________________

   Round
   _________________________________________________________________________ */

inline
unsigned long
Round (/*[in]*/ double r)
{
  return (static_cast<unsigned long>(r + 0.5));
}
