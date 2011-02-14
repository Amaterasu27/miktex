/* DviPage.cpp:

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

#define SMALLBITMAPS 0

#if SMALLBITMAPS
const int MaxVerticalWhite = 0;
const int MaxHorizontalWhite = 0;
#else
const int MaxVerticalWhite = 2;
const int MaxHorizontalWhite = 32;
#endif

size_t DviPageImpl::totalSize = 0;

#if defined(max)
#undef max
#undef min
#endif

/* _________________________________________________________________________

   DviItem Comparisons
   _________________________________________________________________________ */

inline
bool
operator< (/*[in]*/ DviItem &	item1,
	   /*[in]*/ DviItem &	item2)
{
  return (item1.GetTopShr(1) < item2.GetTopShr(1));
}

inline
bool
operator>= (/*[in]*/ DviItem &	item1,
	    /*[in]*/ DviItem &	item2)
{
  return (! (item1 < item2));
}

class ItemTopDown
{
public:
  bool
  operator()
    (/*[in]*/ DviItem & i1,
     /*[in]*/ DviItem & i2)
    const
  {
    return (i1 < i2);
  }
};

class ItemLeftRight
{
public:
  bool
  operator()
    (/*[in]*/ DviItem * i1,
     /*[in]*/ DviItem * i2)
    const
  {
    return (i1->GetLeftShr(1) < i2->GetLeftShr(1));
  }
};

/* _________________________________________________________________________

   ToRoman

   Copied from tex.web.
   _________________________________________________________________________ */

string
ToRoman (/*[in]*/ unsigned n)
{
  string result;
  const char * NUMERALS = "m2d5c2l5x2v5i";
  unsigned j = 0;
  unsigned v = 1000;
  for (;;)
    {
      while (n >= v)
	{
	  result += NUMERALS[j];
	  n -= v;
	}
      if (n == 0)
	{
	  return (result);
	}
      unsigned k = j + 2;
      unsigned u = v / (NUMERALS[k - 1] - '0');
      if (NUMERALS[k - 1] == '2')
	{
	  k += 2;
	  u /= (NUMERALS[k - 1] - '0');
	}
      if (n + u >= v)
	{
	  result += NUMERALS[k];
	  n += u;
	}
      else
	{
	  j += 2;
	  v /= (NUMERALS[j - 1] - '0');
	}
    }
}

/* _________________________________________________________________________

   DviPageImpl::DviPageImpl
   _________________________________________________________________________ */

DviPageImpl::DviPageImpl (/*[in]*/ DviImpl *	pDviImpl,
			  /*[in]*/ int		pageIdx,
			  /*[in]*/ DviPageMode	pageMode,
			  /*[in]*/ long		readPosition,
			  /*[in]*/ int		c0,
			  /*[in]*/ int		c1,
			  /*[in]*/ int		c2,
			  /*[in]*/ int		c3,
			  /*[in]*/ int		c4,
			  /*[in]*/ int		c5,
			  /*[in]*/ int		c6,
			  /*[in]*/ int		c7,
			  /*[in]*/ int		c8,
			  /*[in]*/ int		c9)
  
  : pDviImpl (pDviImpl),
    pageIdx (pageIdx),
    pageMode (pageMode),
    frozen (false),
    readPosition (readPosition),
    backgroundColor (0x80000000),
    autoClean (false),
    size (0),
    nLocks (0),
    tracePage (TraceStream::Open(MIKTEX_TRACE_DVIPAGE)),
    traceBitmap (TraceStream::Open(MIKTEX_TRACE_DVIBITMAP))

{
  if (pageMode != DviPageMode::Dvips)
    {
      dviItems.reserve (500);
    }

  counts[0] = c0;
  counts[1] = c1;
  counts[2] = c2;
  counts[3] = c3;
  counts[4] = c4;
  counts[5] = c5;
  counts[6] = c6;
  counts[7] = c7;
  counts[8] = c8;
  counts[9] = c9;

  int j;

  for (j = 9; j >= 0 && counts[j] == 0; -- j)
    {
      ;
    }

  for (int k = 0; k <= j; ++ k)
    {
      if (k > 0)
	{
	  pageName += '-';
	}
      if (counts[k] > 0)
	{
	  pageName += NUMTOSTR(counts[k]);
	}
      else
	{
	  pageName += ToRoman(-counts[k]);
	}
    }

  tracePage->WriteFormattedLine
    ("libdvi",
     T_("created page object '%s'"),
     pageName.c_str());

  lastVisited = time(0) + 60 * 60;
}

/* _________________________________________________________________________

   DviPageImpl::~DviPageImpl
   _________________________________________________________________________ */

DviPageImpl::~DviPageImpl ()
{
  try
    {
      Lock ();
      AutoUnlockPage autoUnlock (this);
      FreeContents ();
      autoClean = false;
      if (tracePage.get() != 0)
	{
	  tracePage->Close ();
	  tracePage.reset ();
	} 
      if (traceBitmap.get() != 0)
	{
	  traceBitmap->Close ();
	  traceBitmap.reset ();
	}
   }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   DviPageImpl::AddItem
   _________________________________________________________________________ */

void
DviPageImpl::AddItem (/*[in]*/ const DviItem & item)
{
  MIKTEX_ASSERT (IsLocked());
  dviItems.push_back (item);
  frozen = false;
}

/* _________________________________________________________________________

   DviPageImpl::AddSpecial
   _________________________________________________________________________ */

void
DviPageImpl::AddSpecial (/*[in]*/ DviSpecial * pSpecial)
{
  MIKTEX_ASSERT (IsLocked());
#if DVI_DONT_RENDER_POSTSCRIPT_SPECIALS
  if (pSpecial->GetType() == DviSpecialType::Psdef
      || pSpecial->GetType() == DviSpecialType::Psfile
      || pSpecial->GetType() == DviSpecialType::Ps
      || pSpecial->GetType() == DviSpecialType::IncludeGraphics
      || pSpecial->GetType() == DviSpecialType::Tpic)
    {
      if (pageMode == DviPageMode::Auto)
	{
	  tracePage->WriteFormattedLine
	    ("libdvi",
	     T_("switching to Dvips mode on page '%s'"),
	     pageName.c_str());
	  pageMode = DviPageMode::Dvips;
	  dviRules.clear ();
	}
    }
#endif
  dviSpecials.reserve (100);
  dviSpecials.push_back (pSpecial);
}

/* _________________________________________________________________________

   DviPageImpl::AddRule
   _________________________________________________________________________ */

void
DviPageImpl::AddRule (/*[in]*/ DviRuleImpl * pRule)
{
  MIKTEX_ASSERT (IsLocked());
  if (pageMode != DviPageMode::Dvips)
    {
      dviRules.reserve (100);
      dviRules.push_back (pRule);
    }
}

/* _________________________________________________________________________

   DviPageImpl::GetNumberOfDviBitmaps
   _________________________________________________________________________ */

int
DviPageImpl::GetNumberOfDviBitmaps (/*[in]*/ int shrinkFactor)
{
  MIKTEX_ASSERT (IsLocked());
  MAPNUMTOBOOL::const_iterator it = haveShrinkedRaster.find(shrinkFactor);
  if (it == haveShrinkedRaster.end() || ! it->second)
    {
      MakeShrinkedRaster (shrinkFactor);
    }
  return (static_cast<int>(shrinkedDviBitmaps[ shrinkFactor ].size()));
}

/* _________________________________________________________________________

   DviPageImpl::GetNumberOfDibChunks
   _________________________________________________________________________ */

int
DviPageImpl::GetNumberOfDibChunks (/*[in]*/ int shrinkFactor)
{
  MIKTEX_ASSERT (IsLocked());
  MAPNUMTOBOOL::const_iterator it = haveShrinkedRaster.find(shrinkFactor);
  if (it == haveShrinkedRaster.end() || ! it->second)
    {
      MakeShrinkedRaster (shrinkFactor);
    }
  return (static_cast<int>(shrinkedDibChunks[ shrinkFactor ].size()));
}

/* _________________________________________________________________________

   DviPageImpl::MakeShrinkedRaster
   _________________________________________________________________________ */

void
DviPageImpl::MakeShrinkedRaster (/*[in]*/ int shrinkFactor)
{
  if (pageMode == DviPageMode::Dvips)
    {
      // make DIB chunks
      MakeDibChunks (shrinkFactor);
    }
  else
    {
      if (! dviItems.empty())
	{
	  // sort items, color items
	  Freeze (true);
	  
	  // make DVI bitmaps
	  MakeDviBitmaps (shrinkFactor);
	}
    }
  haveShrinkedRaster[ shrinkFactor ] = true;
}

/* _________________________________________________________________________

   DviPageImpl::MakeDviBitmaps
   _________________________________________________________________________ */

void
DviPageImpl::MakeDviBitmaps (/*[in]*/ int shrinkFactor)
{
  MIKTEX_ASSERT (! dviItems.empty());
  MIKTEX_ASSERT (frozen);

  vector<DviItem>::iterator it = dviItems.begin();

  // initialize band
  vector<DviItem *> dviItemPointers;
  dviItemPointers.reserve (200);
  dviItemPointers.push_back (&*it);
  int bandBottom = it->GetBottomShr(shrinkFactor);
  
  // divide vertically
  for ( ; it != dviItems.end(); ++ it)
    {
      int itemTop = it->GetTopShr(shrinkFactor);
      int itemBottom = it->GetBottomShr(shrinkFactor);

      if (itemTop > bandBottom + MaxVerticalWhite)
	{
	  ProcessBand (shrinkFactor, dviItemPointers);
	  bandBottom = itemBottom;
	}
      else
	{
	  bandBottom = std::max(bandBottom, itemBottom);
	}

      dviItemPointers.push_back (&*it);
    }

  // process last band
  ProcessBand (shrinkFactor, dviItemPointers);
}

/* _________________________________________________________________________

   DviPageImpl::ProcessBand
   _________________________________________________________________________ */

void
DviPageImpl::ProcessBand (/*[in]*/ int			shrinkFactor,
			  /*[in,out]*/ vector<DviItem *> & dviItemPointers)
{
  MIKTEX_ASSERT (dviItemPointers.size() > 0);

  // sort band from left to right
  sort (dviItemPointers.begin(), dviItemPointers.end(), ItemLeftRight());

  vector<DviItem *>::iterator itItemPtr = dviItemPointers.begin();
  vector<DviItem *>::iterator itItemPtrMark = itItemPtr;

  int x1 = (*itItemPtr)->GetLeftShr(shrinkFactor);
  int x2 =
    ((*itItemPtr)->GetLeftShr(shrinkFactor)
     + (*itItemPtr)->GetWidthShr(shrinkFactor)
     - 1);

  // initialize bitmap
  DviBitmap currentBitmap;
  currentBitmap.x = x1;
  currentBitmap.y = (*itItemPtr)->GetTopShr(shrinkFactor);
  currentBitmap.width = x2 - x1 + 1;
  currentBitmap.height = (*itItemPtr)->GetHeightShr(shrinkFactor);
  currentBitmap.pPixels = 0;
  currentBitmap.monochrome = true;
  currentBitmap.foregroundColor = (*itItemPtr)->rgbForeground;
  currentBitmap.backgroundColor = (*itItemPtr)->rgbBackground;

  int bitmapBottom = currentBitmap.y + currentBitmap.height - 1;

  // divide horizontally
  for (; itItemPtr != dviItemPointers.end(); ++ itItemPtr)
    {
      DviItem & item = **itItemPtr;

      int itemTop = item.GetTopShr(shrinkFactor);
      int item_left = item.GetLeftShr(shrinkFactor);
      int item_width = item.GetWidthShr(shrinkFactor);
      int itemBottom = item.GetBottomShr(shrinkFactor);

      if (x2 + MaxHorizontalWhite <= item_left
	  || item.rgbForeground != currentBitmap.foregroundColor
	  || item.rgbBackground != currentBitmap.backgroundColor)
	{
	  // add the current bitmap
	  if (currentBitmap.width > 0 && currentBitmap.height > 0)
	    {
	      MakeDviBitmap (shrinkFactor,
			     currentBitmap,
			     itItemPtrMark,
			     itItemPtr);
	    }

	  itItemPtrMark = itItemPtr;

	  x1 = item_left;
	  x2 = item_left + item_width - 1;

	  currentBitmap.x = x1;
	  currentBitmap.y = itemTop;
	  currentBitmap.width = x2 - x1 + 1;
	  currentBitmap.height = item.GetHeightShr(shrinkFactor);
	  currentBitmap.pPixels = 0;
	  currentBitmap.foregroundColor = item.rgbForeground;
	  currentBitmap.backgroundColor = item.rgbBackground;

	  bitmapBottom = currentBitmap.y + currentBitmap.height - 1;
	}
      else
	{
	  x1 = std::min(x1, item_left);
	  x2 = std::max(x2, item_left + item_width - 1);
	  currentBitmap.x = std::min(currentBitmap.x, x1);
	  currentBitmap.y = std::min(currentBitmap.y, itemTop);
	  currentBitmap.width = std::max(currentBitmap.width, x2 - x1 + 1);
	  if (bitmapBottom < itemBottom)
	    {
	      bitmapBottom = itemBottom;
	    }
	  currentBitmap.height = bitmapBottom - currentBitmap.y + 1;
	}
    }

  // add the current bitmap
  MakeDviBitmap (shrinkFactor,
		 currentBitmap,
		 itItemPtrMark,
		 dviItemPointers.end());

  // clear the band, since we are ready
  dviItemPointers.clear ();
}

/* _________________________________________________________________________

   DviPageImpl::MakeDviBitmap
   _________________________________________________________________________ */

void
DviPageImpl::MakeDviBitmap
(/*[in]*/ int				shrinkFactor,
 /*[in,out]*/ DviBitmap &		bitmap,
 /*[in]*/ vector<DviItem *>::iterator	itItemPtrBegin,
 /*[in]*/ vector<DviItem *>::iterator	itItemPtrEnd)
{
  MIKTEX_ASSERT (bitmap.pPixels == 0);

  vector<DviBitmap> & bitmaps = shrinkedDviBitmaps[ shrinkFactor ];

  bitmaps.reserve (1000 / shrinkFactor);

  traceBitmap->WriteFormattedLine
    ("libdvi",
     T_("bitmap %d; bounding box: %d,%d,%d,%d"),
     bitmaps.size(),
     bitmap.x,
     bitmap.y,
     bitmap.width,
     bitmap.height);

  int bytesPerLine = pDviImpl->GetBytesPerLine(shrinkFactor, bitmap.width);
  MIKTEX_ASSERT (bytesPerLine > 0);
  bitmap.bytesPerLine = bytesPerLine;

  int rasterSize = (bytesPerLine * bitmap.height);
  bitmap.pPixels = malloc(rasterSize);
  if (bitmap.pPixels == 0)
    {
      OUT_OF_MEMORY ("DviPageImpl::MakeDviBitmap");
    }
  size += rasterSize;
  totalSize += rasterSize;
  memset (const_cast<void *>(bitmap.pPixels), 0, rasterSize);

  int bitsPerPixel = pDviImpl->GetBitsPerPixel(shrinkFactor);
  int pixelsPerByte = pDviImpl->GetPixelsPerByte(shrinkFactor);
  
  for (; itItemPtrBegin != itItemPtrEnd; ++ itItemPtrBegin)
    {
      DviItem & item = **itItemPtrBegin;

      int itemLeft = item.GetLeftShr(shrinkFactor);
      int itemTop = item.GetTopShr(shrinkFactor);
      int itemHeight = item.GetHeightShr(shrinkFactor);
      int itemBottom = item.GetBottomShr(shrinkFactor);
      UNUSED (itemBottom);
      
      int itemWidth = item.GetWidthShr(shrinkFactor);
      unsigned long itemSize =
	pDviImpl->GetBytesPerLine(shrinkFactor, itemWidth);
      
      int bitOffset = (itemLeft - bitmap.x) % pixelsPerByte;
      int bitShift = (bitOffset * bitsPerPixel) % 8;
      
      MIKTEX_ASSERT (itemTop >= bitmap.y);
      MIKTEX_ASSERT (itemLeft >= bitmap.x);
      MIKTEX_ASSERT (itemBottom <= (bitmap.y + (bitmap.height - 1)));
      MIKTEX_ASSERT (item.GetRightShr(shrinkFactor)
		     <= bitmap.x + bitmap.width - 1);
      
      BYTE * pRaster =
	const_cast<BYTE *>(reinterpret_cast<const BYTE *>(bitmap.pPixels));

      const BYTE * pRasterChar =
	reinterpret_cast<const BYTE *>(item.pPkChar->GetBitmap(shrinkFactor));
      
      int column = itemLeft - bitmap.x;
      
      for (int j = 0; j < static_cast<int>(itemSize); ++ j)
	{
	  int lineNum = (bitmap.height - (itemTop - bitmap.y) - 1);
	  int rasterIdx =
	    ((lineNum * bytesPerLine) + (column / pixelsPerByte) + j);
	  for (int i = 0;
	       i < itemHeight;
	       ++ i, -- lineNum, rasterIdx -= bytesPerLine)
	    {
	      int idxRasterChar = i * itemSize + j;
	      
	      BYTE byte = pRasterChar[idxRasterChar];
	      BYTE mask = static_cast<BYTE>(byte >> bitShift);
	      
	      if (mask != 0)
		{
		  MIKTEX_ASSERT (rasterIdx >= 0 && rasterIdx < rasterSize);
		  pRaster[rasterIdx] |= mask;
		}
	      
	      if (bitOffset != 0
		  && (pixelsPerByte * j + pixelsPerByte - bitOffset
		      < itemWidth))
		{
		  unsigned mask = (byte << (8 - bitShift)) & 0xff;
		  if (mask)
		    {
		      MIKTEX_ASSERT (rasterIdx + 1 < rasterSize);
		      pRaster[rasterIdx + 1] |= mask;
		    }
		}
	    }
	}
    }
  
  bitmaps.push_back (bitmap);
}

/* _________________________________________________________________________

   DviPageImpl::DestroyDviBitmaps
   _________________________________________________________________________ */

void
DviPageImpl::DestroyDviBitmaps ()
{
  for (MAPNUMTOBITMAPVEC::iterator it = shrinkedDviBitmaps.begin();
       it != shrinkedDviBitmaps.end();
       ++ it)
    {
      vector<DviBitmap> & bitmaps = it->second;
      for (size_t j = 0; j < bitmaps.size(); ++ j)
	{
	  if (bitmaps[j].pPixels != 0)
	    {
	      size -= (bitmaps[j].bytesPerLine * bitmaps[j].height);
	      totalSize -= (bitmaps[j].bytesPerLine * bitmaps[j].height);
	      free (const_cast<void*>(bitmaps[j].pPixels));
	      bitmaps[j].pPixels = 0;
	    }
	}
      bitmaps.clear ();
    }
  shrinkedDviBitmaps.clear ();
}

/* _________________________________________________________________________

   DviPageImpl::DestroyDibChunks
   _________________________________________________________________________ */

void
DviPageImpl::DestroyDibChunks ()
{
  for (MAPNUMTODIBCHUNKVEC::iterator it
	 = shrinkedDibChunks.begin();
       it != shrinkedDibChunks.end();
       ++ it)
    {
      vector<SmartPointer <DibChunk > > & dibChunks = it->second;
      for (size_t j = 0; j < dibChunks.size(); ++ j)
	{
	  size -= dibChunks[j]->GetSize();
	  totalSize -= dibChunks[j]->GetSize();
	}
    }
  shrinkedDibChunks.clear ();
}

/* _________________________________________________________________________

   DviPageImpl::FreeContents
   _________________________________________________________________________ */

void
DviPageImpl::FreeContents (/*[in]*/ bool keepSpecials,
			   /*[in]*/ bool keepItems)
{
  MIKTEX_ASSERT (IsLocked());
  if (! keepSpecials)
    {
      for (size_t idx = 0; idx < dviSpecials.size(); ++ idx)
	{
	  DviSpecial * pSpecial = dviSpecials[idx];
	  dviSpecials[idx] = 0;
	  delete dynamic_cast<SpecialRoot*>(pSpecial);
	}
      dviSpecials.clear ();
    }
  if (! keepItems)
    {
      dviItems.clear ();
      for (size_t idx = 0; idx < dviRules.size(); ++ idx)
	{
	  DviRuleImpl * pRule = dviRules[idx];
	  dviRules[idx] = 0;
	  delete pRule;
	}
      dviRules.clear ();
    }
  haveShrinkedRaster.clear ();
  DestroyDviBitmaps ();
  DestroyDibChunks ();
  haveGraphicsInclusions.clear ();
  graphicsInclusions.clear ();
  frozen = false;
}

/* _________________________________________________________________________

   DviPageImpl::GetName
   _________________________________________________________________________ */

const char *
DviPageImpl::GetName ()
{
  return (pageName.c_str());
}

/* _________________________________________________________________________

   DviPageImpl::CheckRules
   _________________________________________________________________________ */

void
DviPageImpl::CheckRules ()
{
  for (size_t idx = 0; idx < dviRules.size(); ++ idx)
    {
      DviRuleImpl & rule = *dviRules[idx];
      rule.ClearFlag (DviRuleImpl::flblackboard);
      int ruleTop = rule.GetTopUns();
      int ruleBottom = rule.GetBottomUns();
      int ruleLeft = rule.GetLeftUns();
      int ruleRight = rule.GetRightUns();
      size_t nItems = dviItems.size();
      for (size_t j = 0; j < nItems; ++ j)
	{
	  DviItem & item = dviItems[j];
	  int itemTop = item.GetTopUns();
	  if (itemTop > ruleBottom)
	    {
	      break;
	    }
	  if (itemTop >= ruleTop
	      && item.GetBottomUns() <= ruleBottom
	      && item.GetLeftUns() >= ruleLeft
	      && item.GetRightUns() <= ruleRight)
	    {
	      rule.SetFlag (DviRuleImpl::flblackboard);
	      item.rgbBackground = rule.GetBackgroundColor();
	    }
	}
    }
}

/* _________________________________________________________________________

   DviPageImpl::Freeze
   _________________________________________________________________________ */

void
DviPageImpl::Freeze (/*[in]*/ bool force)
{
  MIKTEX_ASSERT (IsLocked());
  if (! frozen || force)
    {
      sort (dviItems.begin(), dviItems.end(), ItemTopDown());
#if defined(MIKTEX_DEBUG)
      for (size_t idx = 1; idx < dviItems.size(); ++ idx)
	{
	  MIKTEX_ASSERT (dviItems[idx] >= dviItems[idx - 1]);
	}
#endif
      CheckRules ();
      frozen = true;
   }
}

/* _________________________________________________________________________

   DviPageImpl::GetDviBitmap
   _________________________________________________________________________ */

const DviBitmap &
DviPageImpl::GetDviBitmap (/*[in]*/ int shrinkFactor,
			   /*[in]*/ int idx)
{
  MIKTEX_ASSERT (IsLocked());
  MIKTEX_ASSERT (IsFrozen());
  MIKTEX_ASSERT
    (idx >= 0
     && (static_cast<unsigned>(idx)
	 < shrinkedDviBitmaps[ shrinkFactor ].size()));
  lastVisited = time(0);
  return (shrinkedDviBitmaps[ shrinkFactor ][idx]);
}

/* _________________________________________________________________________

   DviPageImpl::GetDibChunk
   _________________________________________________________________________ */

DibChunk *
DviPageImpl::GetDibChunk (/*[in]*/ int shrinkFactor,
			  /*[in]*/ int idx)
{
  MIKTEX_ASSERT (IsLocked());
  MIKTEX_ASSERT (IsFrozen());
  MIKTEX_ASSERT
    (idx >= 0
     && (static_cast<unsigned>(idx)
	 < shrinkedDibChunks[ shrinkFactor ].size()));
  lastVisited = time(0);
  return (shrinkedDibChunks[ shrinkFactor ][idx].Get());
}

/* _________________________________________________________________________

   DviPageImpl::GetDviObject
   _________________________________________________________________________ */

DviImpl *
DviPageImpl::GetDviObject ()
{
  return (pDviImpl);
}

/* _________________________________________________________________________

   DviPageImpl::GetReg
   _________________________________________________________________________ */

int
DviPageImpl::GetReg (/*[in]*/ int idx)
{
  MIKTEX_ASSERT (idx >= 0 && idx < 10);
  return (counts[idx]);
}

/* _________________________________________________________________________

   DviPageImpl::GetRule
   _________________________________________________________________________ */

DviRule *
DviPageImpl::GetRule (/*[in]*/ int idx)
{
  MIKTEX_ASSERT (IsLocked());
  int size = static_cast<int>(dviRules.size());
  if (idx >= size)
    {
      if (idx == size)
	{
	  return (0);
	}
      else
	{
	  UNEXPECTED_CONDITION ("DviPageImpl::GetRule");
	}
    }
  else
    {
      return (dviRules[idx]);
    }
}

/* _________________________________________________________________________

   DviPageImpl::GetSpecial
   _________________________________________________________________________ */

DviSpecial *
DviPageImpl::GetSpecial (/*[in]*/ int idx)
{
  MIKTEX_ASSERT (IsLocked());
  int size = static_cast<int>(dviSpecials.size());
  if (idx >= size)
    {
      if (idx == size)
	{
	  return (0);
	}
      else
	{
	  UNEXPECTED_CONDITION ("DviPageImpl::GetSpecial");
	}
    }
  else
    {
      return (dviSpecials[idx]);
    }
}

/* _________________________________________________________________________

   DviPageImpl::GetBackgroundColor
   _________________________________________________________________________ */

unsigned long
DviPageImpl::GetBackgroundColor ()
{
  if ((backgroundColor & 0x80000000) != 0)
    {
      return (0x00ffffff);		// <todo>
    }
  else
    {
      return (backgroundColor);
    }
}

/* _________________________________________________________________________

   DviPageImpl::Lock
   _________________________________________________________________________ */

void
DviPageImpl::Lock ()
{
  criticalSectionMonitor.Enter ();
  MIKTEX_ASSERT (nLocks >= 0);
  MIKTEX_ASSERT (nLocks < 1000);
  nLocks += 1;
}

/* _________________________________________________________________________

   DviPageImpl::Unlock
   _________________________________________________________________________ */

void
DviPageImpl::Unlock ()
{
  MIKTEX_ASSERT (IsLocked());
  MIKTEX_ASSERT (nLocks > 0);
  nLocks -= 1;
  try
    {
      if (nLocks == 0 && autoClean)
	{
	  tracePage->WriteFormattedLine
	    ("libdvi",
	     T_("auto-cleaning page '%s'"),
	     pageName.c_str());
	  FreeContents (false, false);
	}
    }
  catch (const exception &)
    {
      criticalSectionMonitor.Leave ();
      throw;
    }
  criticalSectionMonitor.Leave ();
}

/* _________________________________________________________________________

   DviPageImpl::GetNextHyperref
   _________________________________________________________________________ */

HypertexSpecial *
DviPageImpl::GetNextHyperref (/*[in,out]*/ int & idx)
{
  HypertexSpecial * pHyperSpecial;
  while ((pHyperSpecial = GetNextSpecial<HypertexSpecial>(idx)) != 0)
    {
      if (! pHyperSpecial->IsName())
	{
	  return (pHyperSpecial);
	}
    }
  return (0);
}

/* _________________________________________________________________________

   DviPageImpl::MakeDibChunks
   _________________________________________________________________________ */

void
DviPageImpl::MakeDibChunks (/*[in]*/ int shrinkFactor)
{
  auto_ptr<Process> pDvips;
  auto_ptr<Process> pGhostscript;
  try
    {
      pDvips.reset (StartDvips());
      auto_ptr<Thread>
	pDvipsTranscriptReader
	(Thread::Start(DvipsTranscriptReader, this));
      pGhostscript.reset (StartGhostscript(shrinkFactor));
      auto_ptr<Thread>
	pGhostscriptTranscriptReader
	(Thread::Start(GhostscriptTranscriptReader, this));
      auto_ptr<DibChunker> pChunker (DibChunker::Create());
      const size_t CHUNK_SIZE = 1024 * 64;
      MIKTEX_ASSERT (IsLocked());
      dibShrinkFactor = shrinkFactor;
      while (pChunker->Process(DibChunker::Default,
			       CHUNK_SIZE,
			       this))
	{
	}
      pDvipsTranscriptReader->Join ();
      pDvipsTranscriptReader.reset ();
      pGhostscriptTranscriptReader->Join ();
      pGhostscriptTranscriptReader.reset ();
    }
  catch (const exception &)
    {
      dvipsErr.Close ();
      if (dvipsOut.Get() != 0)
	{
	  dvipsOut.Close ();
	}
      gsOut.Close ();
      gsErr.Close ();
      throw;
    }
  dvipsErr.Close ();
  if (dvipsOut.Get() != 0)
    {
      dvipsOut.Close ();
    }
  gsOut.Close ();
  gsErr.Close ();
  tracePage->WriteLine ("libdvi", T_("Dvips transcript:"));
  tracePage->WriteLine ("libdvi", dvipsTranscript.c_str());
  tracePage->WriteLine ("libdvi", T_("Ghostscript transcript:"));
  tracePage->WriteLine ("libdvi", gsTranscript.c_str());
  MIKTEX_ASSERT (pDvips.get() != 0);
  if (pDvips->get_ExitCode() != 0)
    {
      FATAL_DVI_ERROR ("DviPageImpl::MakeDibChunks",
		       T_("The page could not be rendered."),
		       dvipsTranscript.c_str());
    }
  MIKTEX_ASSERT (pGhostscript.get() != 0);
  if (pGhostscript->get_ExitCode() != 0)
    {
      FATAL_DVI_ERROR ("DviPageImpl::MakeDibChunks",
		       T_("The page could not be rendered."),
		       gsTranscript.c_str());
    }
}

/* _________________________________________________________________________

   DviPageImpl::DvipsTranscriptReader
   _________________________________________________________________________ */

void
DviPageImpl::DvipsTranscriptReader (/*[in]*/ void * p)
{
  DviPageImpl * This = reinterpret_cast<DviPageImpl*>(p);
  try
    {
#define CHUNK_SIZE 64
      char buf[ CHUNK_SIZE ];
      This->dvipsTranscript = "";
      try
	{
	  size_t n;
	  while ((n = This->dvipsErr.Read(buf, CHUNK_SIZE)) > 0)
	    {
	      This->dvipsTranscript.append (buf, n);
	    }
	}
      catch (const BrokenPipeException &)
	{
	}
    }
  catch (const MiKTeXException &)
    {
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   DviPageImpl::GhostscriptTranscriptReader
   _________________________________________________________________________ */

void
DviPageImpl::GhostscriptTranscriptReader (/*[in]*/ void * p)
{
  DviPageImpl * This = reinterpret_cast<DviPageImpl*>(p);
  try
    {
#define CHUNK_SIZE 64
      char buf[ CHUNK_SIZE ];
      This->gsTranscript = "";
      try
	{
	  size_t n;
	  while ((n = This->gsErr.Read(buf, CHUNK_SIZE)) > 0)
	    {
	      This->gsTranscript.append (buf, n);
	    }
	}
      catch (const BrokenPipeException &)
	{
	}
    }
  catch (const MiKTeXException &)
    {
    }
  catch (const exception &)
    {
    }
}

/* _________________________________________________________________________

   DviPageImpl::OnNewChunk
   _________________________________________________________________________ */

void
DviPageImpl::OnNewChunk (/*[in]*/ DibChunk * pChunk)
{
  vector<SmartPointer<DibChunk> > & dibChunks =
    shrinkedDibChunks[ dibShrinkFactor ];

  const BITMAPINFO * pBitmapInfo = pChunk->GetBitmapInfo();

  traceBitmap->WriteFormattedLine
    ("libdvi",
     T_("new DIB chunk %d; bounding box: %d,%d,%d,%d"),
     dibChunks.size(),
     pChunk->GetX(),
     pChunk->GetY(),
     pBitmapInfo->bmiHeader.biWidth,
     pBitmapInfo->bmiHeader.biHeight);

  size += pChunk->GetSize();
  totalSize += pChunk->GetSize();

  dibChunks.push_back (pChunk);
}

/* _________________________________________________________________________

   DviPageImpl::Read
   _________________________________________________________________________ */

size_t
DviPageImpl::Read (/*[out]*/ void *	pBuf,
		   /*[in]*/ size_t	size)
{
  return (gsOut.Read(pBuf, size));
}

/* _________________________________________________________________________

   DviPageImpl::StartDvips
   _________________________________________________________________________ */

Process *
DviPageImpl::StartDvips ()
{
  // locate dvips.exe
  PathName dvipsPath;
  if (! SessionWrapper(true)->FindFile(MIKTEX_DVIPS_EXE,
				       FileType::EXE,
				       dvipsPath))
    {
      FATAL_DVI_ERROR ("DviPageImpl::StartDvips",
		       T_("The Dvips utility could not be found."),
		       0);
    }

  // make Dvips command line
  CommandLineBuilder arguments;
  arguments.AppendOption ("-D", NUMTOSTR(pDviImpl->GetResolution()));
  string metafontMode = pDviImpl->GetMetafontMode();
  if (! metafontMode.empty())
    {
      arguments.AppendOption ("-mode ", metafontMode.c_str());
    }
  arguments.AppendOption ("-f", "1");
  arguments.AppendOption ("-p=", NUMTOSTR(pageIdx + 1));
  arguments.AppendOption ("-l", NUMTOSTR(pageIdx + 1));
  if (! pDviImpl->HavePaperSizeSpecial())
    {
      PaperSizeInfo paperSizeInfo = pDviImpl->GetPaperSizeInfo();
      int width = paperSizeInfo.width;
      int height = paperSizeInfo.height;
      if (pDviImpl->Landscape())
	{
	  swap (width, height);
	}
      arguments.AppendOption ("-T",
				(string(NUMTOSTR(width)) + "bp"
				 + ','
				 + NUMTOSTR(height) + "bp"));
    }
  arguments.AppendOption ("-Ic");
  arguments.AppendOption ("-MiKTeX:nolandscape");
  if (SessionWrapper(true)->GetConfigValue("Dvips",
					   "Pedantic",
					   false))
    {
      arguments.AppendOption ("-MiKTeX:pedantic");
    }
  arguments.AppendOption ("-MiKTeX:allowallpaths");
  arguments.AppendArgument (pDviImpl->GetDviFileName());

  PathName dir (pDviImpl->GetDviFileName());
  dir.MakeAbsolute ();
  dir.RemoveFileSpec ();

  ProcessStartInfo processStartInfo;

  processStartInfo.Arguments = arguments.Get();
  processStartInfo.FileName = dvipsPath.Get();
  processStartInfo.RedirectStandardError = true;
  processStartInfo.RedirectStandardOutput = true;
  processStartInfo.WorkingDirectory = dir.Get();

  auto_ptr<Process> pDvips (Process::Start(processStartInfo));

  dvipsOut.Attach (pDvips->get_StandardOutput());
  dvipsErr.Attach (pDvips->get_StandardError());

  return (pDvips.release());
}

/* _________________________________________________________________________

   DviPageImpl::StartGhostscript
   _________________________________________________________________________ */

Process *
DviPageImpl::StartGhostscript (/*[in]*/ int shrinkFactor)
{
  PathName gsPath;
  SessionWrapper(true)->GetGhostscript (gsPath.GetBuffer(), 0);

  // make Ghostscript command line
  CommandLineBuilder arguments;
  string res =
    NUMTOSTR(static_cast<double>(pDviImpl->GetResolution())
	     / shrinkFactor);
  arguments.AppendOption ("-r", res + 'x' + res);
  PaperSizeInfo paperSizeInfo = pDviImpl->GetPaperSizeInfo();
  int width = paperSizeInfo.width;
  int height = paperSizeInfo.height;
  if (pDviImpl->Landscape())
  {
    swap (width, height);
  }
  width = 
    static_cast<int>(((pDviImpl->GetResolution() * width) / 72.0)
		     / shrinkFactor);
  height = 
    static_cast<int>(((pDviImpl->GetResolution() * height) / 72.0)
		     / shrinkFactor);
  arguments.AppendOption ("-g",
			    (string(NUMTOSTR(width))
			     + 'x'
			     + NUMTOSTR(height)));
  arguments.AppendOption ("-sDEVICE=", "bmp16m");
  arguments.AppendOption ("-q");
  arguments.AppendOption ("-dBATCH");
  arguments.AppendOption ("-dNOPAUSE");
  arguments.AppendOption ("-dSAFER");
  arguments.AppendOption ("-sstdout=", "%stderr");
  arguments.AppendOption ("-dTextAlphaBits=", "4");
  arguments.AppendOption ("-dGraphicsAlphaBits=", "4");
  arguments.AppendOption ("-dDOINTERPOLATE");
  arguments.AppendOption ("-sOutputFile=", "-");
  arguments.AppendArgument ("-");

  ProcessStartInfo processStartInfo;

  processStartInfo.Arguments = arguments.Get();
  processStartInfo.FileName = gsPath.Get();
  processStartInfo.StandardInput = dvipsOut.Get();
  processStartInfo.RedirectStandardError = true;
  processStartInfo.RedirectStandardOutput = true;

  auto_ptr<Process> pGhostscript (Process::Start(processStartInfo));

  // close unused file stream (this prevents a hang situation)
  dvipsOut.Close ();

  gsOut.Attach (pGhostscript->get_StandardOutput());
  gsErr.Attach (pGhostscript->get_StandardError());

  return (pGhostscript.release());
}

/* _________________________________________________________________________

   DviPageImpl::GetNumberOfGraphicsInclusions
   _________________________________________________________________________ */

int
DviPageImpl::GetNumberOfGraphicsInclusions (/*[in]*/ int shrinkFactor)
{
  MIKTEX_ASSERT (IsLocked());
  MAPNUMTOBOOL::const_iterator it = haveGraphicsInclusions.find(shrinkFactor);
  if (it == haveGraphicsInclusions.end() || ! it->second)
  {
    DoPostScriptSpecials (shrinkFactor);
    DoGraphicsSpecials (shrinkFactor);
    haveGraphicsInclusions[shrinkFactor] = true;
  }
  return (static_cast<int>(graphicsInclusions[ shrinkFactor ].size()));
}

/* _________________________________________________________________________

   DviPageImpl::DoPostScriptSpecials
   _________________________________________________________________________ */

void
DviPageImpl::DoPostScriptSpecials (/*[in]*/ int shrinkFactor)
{
  Ghostscript gs;

  for (size_t idx = 0; idx < dviSpecials.size(); ++ idx)
  {
    DviSpecial * pSpecial = dviSpecials[idx];
    if (pSpecial->GetType() == DviSpecialType::Psdef)
    {
      gs.AddDefinition (reinterpret_cast<PsdefSpecial*>(pSpecial));
    }
    else if (pSpecial->GetType() == DviSpecialType::Psfile)
    {
      if (! gs.IsOpen())
      {
	gs.Open (pDviImpl, shrinkFactor);
      }
      if (gs.IsPageEmpty())
      {
	gs.BeginPage ();
      }
      gs.DoSpecial (reinterpret_cast<PsfileSpecial*>(pSpecial));
    }
    else if (pSpecial->GetType() == DviSpecialType::Ps)
    {
      if (! gs.IsOpen())
      {
	gs.Open (pDviImpl, shrinkFactor);
      }
      if (gs.IsPageEmpty())
      {
	gs.BeginPage ();
      }
      gs.DoSpecial (reinterpret_cast<DvipsSpecial*>(pSpecial));
    }
  }

  if (! gs.IsPageEmpty())
  {
    gs.EndPage ();
  }

  if (gs.IsOpen())
  {
    gs.Close ();
  }

  GraphicsInclusion * pGrinc;
  vector<SmartPointer<GraphicsInclusion> > & vec = graphicsInclusions[shrinkFactor];  
  for (int idx = 0; (pGrinc = gs.GetGraphicsInclusion(idx)) != 0; ++ idx)
  {
    vec.push_back (pGrinc);
  }
}

/* _________________________________________________________________________

   DviPageImpl::DoGraphicsSpecials
   _________________________________________________________________________ */

void
DviPageImpl::DoGraphicsSpecials (/*[in]*/ int shrinkFactor)
{
  vector<SmartPointer<GraphicsInclusion> > & vec = graphicsInclusions[shrinkFactor];  
  for (size_t idx = 0; idx < dviSpecials.size(); ++ idx)
  {
    DviSpecial * pSpecial = dviSpecials[idx];
    if (pSpecial->GetType() == DviSpecialType::IncludeGraphics)
    {
      GraphicsSpecial * pGraphicsSpecial =
	reinterpret_cast<GraphicsSpecial*>(pSpecial);
      PathName fileName;
      if (! pDviImpl->FindGraphicsFile(pGraphicsSpecial->GetFileName(), fileName))
      {
	FATAL_MIKTEX_ERROR ("DviPageImpl::DoGraphicsSpecials",
	  T_("The graphics file could not be found."),
	  pGraphicsSpecial->GetFileName());
      }
      ImageType imageType (ImageType::None);
      if (fileName.HasExtension(".bmp"))
      {
	imageType = ImageType::DIB;
      }
      else if (fileName.HasExtension(".emf") || fileName.HasExtension(".wmf"))
      {
	imageType = ImageType::EMF;
      }
      else
      {
	PathName tempFileName;
	if (! pDviImpl->TryGetTempFile(fileName.Get(), tempFileName))
	{
	  if (! SessionWrapper(true)->ConvertToBitmapFile(
	    fileName.Get(),
	    tempFileName.GetBuffer(),
	    0))
	  {
	    FATAL_MIKTEX_ERROR ("DviPageImpl::DoGraphicsSpecials",
	      T_("Could not make bitmap file."),
	      fileName.Get());
	  }
	  pDviImpl->RememberTempFile (fileName.Get(), tempFileName);
	}
	imageType = ImageType::DIB;
	fileName = tempFileName;
      }
      int x = PixelShrink(shrinkFactor, pGraphicsSpecial->GetX());
      int y = PixelShrink(shrinkFactor, pGraphicsSpecial->GetY());
      int cx = pGraphicsSpecial->GetWidth();
      if (cx >= 0)
      {
	cx = PixelShrink(shrinkFactor, cx);
      }
      int cy = pGraphicsSpecial->GetHeight();
      if (cy >= 0)
      {
	cy = PixelShrink(shrinkFactor, cy);
      }
      vec.push_back (new GraphicsInclusionImpl(
	imageType.Get(),
	fileName,
	false,
	x,
	y,
	cx,
	cy));
    }
  }
}

/* _________________________________________________________________________

   DviPageImpl::GetGraphicsInclusion
   _________________________________________________________________________ */

GraphicsInclusion *
DviPageImpl::GetGraphicsInclusion (/*[in]*/ int shrinkFactor,
				   /*[in]*/ int idx)
{
  MIKTEX_ASSERT (IsLocked());
  MIKTEX_ASSERT (IsFrozen());
  MIKTEX_ASSERT
    (idx >= 0
     && (static_cast<unsigned>(idx)
     < graphicsInclusions[ shrinkFactor ].size()));
  lastVisited = time(0);
  return (graphicsInclusions[ shrinkFactor ][idx].Get());
}

