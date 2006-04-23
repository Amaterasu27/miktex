/* MagnifyingGlassOptionsPage.cpp:

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

#include "StdAfx.h"

#include "yap.h"

#include "MagnifyingGlassOptionsPage.h"

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(MagnifyingGlassOptionsPage, CPropertyPage)
  ON_BN_CLICKED(IDC_HIDE_CURSOR,
		&MagnifyingGlassOptionsPage::OnClickHideMouseCursor)
  ON_CBN_SELCHANGE(IDC_MAGGLASS_LARGE_SHRINK_FACTOR,
		   &MagnifyingGlassOptionsPage::OnChangeLargeShrinkFactor)
  ON_CBN_SELCHANGE(IDC_MAGGLASS_MEDIUM_SHRINK_FACTOR,
		   &MagnifyingGlassOptionsPage::OnChangeMediumShrinkFactor)
  ON_CBN_SELCHANGE(IDC_MAGGLASS_SMALL_SHRINK_FACTOR,
		   &MagnifyingGlassOptionsPage::OnChangeSmallShrinkFactor)
  ON_EN_CHANGE(IDC_MAGGLASS_LARGE_HEIGHT,
	       &MagnifyingGlassOptionsPage::OnChangeLargeHeight)
  ON_EN_CHANGE(IDC_MAGGLASS_LARGE_WIDTH,
	       &MagnifyingGlassOptionsPage::OnChangeLargeWidth)
  ON_EN_CHANGE(IDC_MAGGLASS_MEDIUM_HEIGHT,
	       &MagnifyingGlassOptionsPage::OnChangeMediumHeight)
  ON_EN_CHANGE(IDC_MAGGLASS_MEDIUM_WIDTH,
	       &MagnifyingGlassOptionsPage::OnChangeMediumWidth)
  ON_EN_CHANGE(IDC_MAGGLASS_SMALL_HEIGHT,
	       &MagnifyingGlassOptionsPage::OnChangeSmallHeight)
  ON_EN_CHANGE(IDC_MAGGLASS_SMALL_WIDTH,
	       &MagnifyingGlassOptionsPage::OnChangeSmallWidth)
  ON_CBN_SELCHANGE(IDC_COMBO_OPACITY,
		   &MagnifyingGlassOptionsPage::OnChangeOpacity)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage::MagnifyingGlassOptionsPage
   _________________________________________________________________________ */

MagnifyingGlassOptionsPage::MagnifyingGlassOptionsPage ()

  : CPropertyPage (IDD),
    hideCursor (g_pYapConfig->magGlassHidesCursor),
    smallWidth (g_pYapConfig->magGlassSmallWidth),
    smallHeight (g_pYapConfig->magGlassSmallHeight),
    mediumWidth (g_pYapConfig->magGlassMediumWidth),
    mediumHeight (g_pYapConfig->magGlassMediumHeight),
    largeWidth (g_pYapConfig->magGlassLargeWidth),
    largeHeight (g_pYapConfig->magGlassLargeHeight)

{
  smallShrinkFactor.Format
    (T_("%d"),
     g_pYapConfig->magGlassSmallShrinkFactor);
  mediumShrinkFactor.Format
    (T_("%d"),
     g_pYapConfig->magGlassMediumShrinkFactor);
  largeShrinkFactor.Format
    (T_("%d"),
     g_pYapConfig->magGlassLargeShrinkFactor);
}

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
MagnifyingGlassOptionsPage::OnInitDialog ()
{
  BOOL ret = CPropertyPage::OnInitDialog();
  try
    {
      int idx;
      if (g_pYapConfig->magGlassOpacity < 10)
	{
	  idx = 0;
	}
      else if (g_pYapConfig->magGlassOpacity >= 100)
	{
	  idx = 9;
	}
      else
	{
	  idx = (g_pYapConfig->magGlassOpacity / 10 - 1);
	}
      if (comboOpacity.SetCurSel(idx) < 0)
	{
	  UNEXPECTED_CONDITION
	    (T_("MagnifyingGlassOptionsPage::OnInitDialog"));
	}
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  return (ret);
}

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage::DoDataExchange
   _________________________________________________________________________ */

void
MagnifyingGlassOptionsPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_Text(pDX, IDC_MAGGLASS_MEDIUM_HEIGHT, mediumHeight);
  DDX_Text(pDX, IDC_MAGGLASS_LARGE_HEIGHT, largeHeight);
  DDX_Text(pDX, IDC_MAGGLASS_SMALL_HEIGHT, smallHeight);
  DDX_Text(pDX, IDC_MAGGLASS_LARGE_WIDTH, largeWidth);
  DDX_Text(pDX, IDC_MAGGLASS_MEDIUM_WIDTH, mediumWidth);
  DDX_Text(pDX, IDC_MAGGLASS_SMALL_WIDTH, smallWidth);
  DDX_CBString (pDX,
    IDC_MAGGLASS_LARGE_SHRINK_FACTOR,
    largeShrinkFactor);
  DDX_CBString (pDX,
    IDC_MAGGLASS_MEDIUM_SHRINK_FACTOR,
    mediumShrinkFactor);
  DDX_CBString (pDX,
    IDC_MAGGLASS_SMALL_SHRINK_FACTOR,
    smallShrinkFactor);
  DDX_Check(pDX, IDC_HIDE_CURSOR, hideCursor);
  DDX_Control(pDX, IDC_COMBO_OPACITY, comboOpacity);
}

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage::OnApply
   _________________________________________________________________________ */

BOOL
MagnifyingGlassOptionsPage::OnApply ()
{
  try
    {
      g_pYapConfig->magGlassHidesCursor = (hideCursor ? true : false);
      
      int idx= comboOpacity.GetCurSel();

      if (idx < 0 || idx > 9)
	{
	  UNEXPECTED_CONDITION (T_("MagnifyingGlassOptionsPage::OnApply"));
	}

      g_pYapConfig->magGlassOpacity = (idx + 1) * 10;
      
      g_pYapConfig->magGlassSmallWidth = smallWidth;
      g_pYapConfig->magGlassSmallHeight = smallHeight;
      g_pYapConfig->magGlassSmallShrinkFactor = _ttoi(smallShrinkFactor);
      if (g_pYapConfig->magGlassSmallShrinkFactor == 0)
	{
	  g_pYapConfig->magGlassSmallShrinkFactor = 1;
	}
      
      g_pYapConfig->magGlassMediumWidth = mediumWidth;
      g_pYapConfig->magGlassMediumHeight = mediumHeight;
      g_pYapConfig->magGlassMediumShrinkFactor = _ttoi(mediumShrinkFactor);
      if (g_pYapConfig->magGlassMediumShrinkFactor == 0)
	{
	  g_pYapConfig->magGlassMediumShrinkFactor = 1;
	}
      
      g_pYapConfig->magGlassLargeWidth = largeWidth;
      g_pYapConfig->magGlassLargeHeight = largeHeight;
      g_pYapConfig->magGlassLargeShrinkFactor = _ttoi(largeShrinkFactor);
      if (g_pYapConfig->magGlassLargeShrinkFactor == 0)
	{
	  g_pYapConfig->magGlassLargeShrinkFactor = 1;
	}
      
      return (CPropertyPage::OnApply());
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
      return (FALSE);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
      return (FALSE);
    }
}

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage::OnChangeSmallWidth
   _________________________________________________________________________ */

void
MagnifyingGlassOptionsPage::OnChangeSmallWidth ()
{
  SetModified (TRUE);
}

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage::OnChangeMediumWidth
   _________________________________________________________________________ */

void
MagnifyingGlassOptionsPage::OnChangeMediumWidth ()
{
  SetModified (TRUE);
}

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage::OnChangeLargeWidth
   _________________________________________________________________________ */

void
MagnifyingGlassOptionsPage::OnChangeLargeWidth ()
{
  SetModified (TRUE);
}

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage::OnChangeSmallHeight
   _________________________________________________________________________ */

void
MagnifyingGlassOptionsPage::OnChangeSmallHeight ()
{
  SetModified (TRUE);
}

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage::OnChangeMediumHeight
   _________________________________________________________________________ */

void
MagnifyingGlassOptionsPage::OnChangeMediumHeight ()
{
  SetModified (TRUE);
}

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage::OnChangeLargeHeight
   _________________________________________________________________________ */

void
MagnifyingGlassOptionsPage::OnChangeLargeHeight ()
{
  SetModified (TRUE);
}

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage::OnChangeSmallShrinkFactor
   _________________________________________________________________________ */

void
MagnifyingGlassOptionsPage::OnChangeSmallShrinkFactor ()
{
  SetModified (TRUE);
}

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage::OnChangeMediumShrinkFactor
   _________________________________________________________________________ */

void
MagnifyingGlassOptionsPage::OnChangeMediumShrinkFactor ()
{
  SetModified (TRUE);
}

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage::OnChangeLargeShrinkFactor
   _________________________________________________________________________ */

void
MagnifyingGlassOptionsPage::OnChangeLargeShrinkFactor ()
{
  SetModified (TRUE);
}

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage::OnClickHideMouseCursor
   _________________________________________________________________________ */

void
MagnifyingGlassOptionsPage::OnClickHideMouseCursor ()
{
  SetModified (TRUE);
}

/* _________________________________________________________________________

   MagnifyingGlassOptionsPage::OnChangeOpacity
   _________________________________________________________________________ */

void
MagnifyingGlassOptionsPage::OnChangeOpacity ()
{
  SetModified (TRUE);
}
