/* DisplayOptionsPage.cpp:

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

#include "DisplayOptionsPage.h"
#include "DviDoc.h"

/* _________________________________________________________________________

   DisplayOptionsPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(DisplayOptionsPage, CPropertyPage)
  ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY_MODE, OnChangeMode)
  ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY_SHRINK_FACTOR,
		   &DisplayOptionsPage::OnChangeShrinkFactor)
  ON_CBN_SELCHANGE(IDC_COMBO_UNIT, &DisplayOptionsPage::OnChangeUnit)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   DisplayOptionsPage::DisplayOptionsPage
   _________________________________________________________________________ */

DisplayOptionsPage::DisplayOptionsPage ()
  : CPropertyPage (IDD),
    metafontModeIdx (static_cast<int>(g_pYapConfig->displayMetafontMode))
{
  shrinkFactor.Format (T_("%d"), g_pYapConfig->displayShrinkFactor);
}

/* _________________________________________________________________________

   DisplayOptionsPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
DisplayOptionsPage::OnInitDialog ()
{
  BOOL ret = CPropertyPage::OnInitDialog();

  try
    {
      MIKTEXMFMODE mode;
      for (int idx = 0;
	   SessionWrapper(true)->GetMETAFONTMode(idx, &mode);
	   ++ idx)
	{
	  CString modeString = mode.szMnemonic;
	  modeString += T_(" (");
	  modeString += mode.szDescription;
	  modeString += T_(")");
	  if (modeComboBox.AddString(modeString) < 0)
	    {
	      FATAL_WINDOWS_ERROR (T_("CComboBox::AddString"), 0);
	    }
	  if (idx == metafontModeIdx)
	    {
	      if (modeComboBox.SetCurSel(idx) < 0)
		{
		  FATAL_WINDOWS_ERROR (T_("CComboBox::SetCurSel"), 0);
		}
	      resolution = mode.iHorzRes;
	      resolutionControl.SetWindowText (NUMTOSTR(resolution));
	    }
	}
      int idx = -1;
      switch (g_pYapConfig->dviPageMode.Get())
	{
	case DviPageMode::Dvips:
	  idx = 0;
	  break;
	case DviPageMode::Pk:
	  idx = 1;
	  break;
	}
      if (comboPageMode.SetCurSel(idx) < 0)
	{
	  FATAL_WINDOWS_ERROR (T_("CComboBox::SetCurSel"), 0);
	}
      idx = -1;
      switch (g_pYapConfig->unit.Get())
	{
	case Units::Centimeters:
	  idx = 0;
	  break;
	case Units::Inches:
	  idx = 1;
	  break;
	case Units::Millimeters:
	  idx = 2;
	  break;
	case Units::Picas:
	  idx = 3;
	  break;
	case Units::BigPoints:
	  idx = 4;
	  break;
	}
      if (unitComboBox.SetCurSel(idx) < 0)
	{
	  FATAL_WINDOWS_ERROR (T_("CComboBox::SetCurSel"), 0);
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

   DisplayOptionsPage::DoDataExchange
   _________________________________________________________________________ */

void
DisplayOptionsPage::DoDataExchange (/*[in]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);

  DDX_CBString (pDX, IDC_COMBO_DISPLAY_SHRINK_FACTOR, shrinkFactor);
  DDX_Control (pDX, IDC_COMBO_DISPLAY_MODE, modeComboBox);
  DDX_Control (pDX, IDC_COMBO_DISPLAY_SHRINK_FACTOR, shrinkFactorComboBox);
  DDX_Control (pDX, IDC_STATIC_RESOLUTION, resolutionControl);
  DDX_Control(pDX, IDC_COMBO_PAGE_MODE, comboPageMode);
  DDX_Control(pDX, IDC_COMBO_UNIT, unitComboBox);
}

/* _________________________________________________________________________

   DisplayOptionsPage::OnApply
   _________________________________________________________________________ */

BOOL
DisplayOptionsPage::OnApply ()
{
  try
    {
      g_pYapConfig->displayMetafontMode = metafontModeIdx;
      g_pYapConfig->displayShrinkFactor = _ttoi(shrinkFactor);
      if (g_pYapConfig->displayShrinkFactor == 0)
	{
	  g_pYapConfig->displayShrinkFactor = 1;
	}
      int idx = comboPageMode.GetCurSel();
      if (idx < 0)
	{
	  FATAL_WINDOWS_ERROR (T_("CComboBox::GetCurSel"), 0);
	}
      switch (idx)
	{
	case 0:
	  g_pYapConfig->dviPageMode = DviPageMode::Dvips;
	  break;
	case 1:
	  g_pYapConfig->dviPageMode = DviPageMode::Pk;
	  break;
	}
      idx = unitComboBox.GetCurSel();
      if (idx < 0)
	{
	  FATAL_WINDOWS_ERROR (T_("CComboBox::GetCurSel"), 0);
	}
      switch (idx)
	{
	case 0:
	  g_pYapConfig->unit = Units::Centimeters;
	  break;
	case 1:
	  g_pYapConfig->unit = Units::Inches;
	  break;
	case 2:
	  g_pYapConfig->unit = Units::Millimeters;
	  break;
	case 3:
	  g_pYapConfig->unit = Units::Picas;
	  break;
	case 4:
	  g_pYapConfig->unit = Units::BigPoints;
	  break;
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

   DisplayOptionsPage::OnChangeMode
   _________________________________________________________________________ */

void
DisplayOptionsPage::OnChangeMode ()
{
  try
    {
      int idx = modeComboBox.GetCurSel();
      if (idx < 0)
	{
	  FATAL_WINDOWS_ERROR (T_("CComboBox::GetCurSel"), 0);
	}
      MIKTEXMFMODE mode;
      if (SessionWrapper(true)->GetMETAFONTMode(idx, &mode))
	{
	  resolution = mode.iHorzRes;
	  resolutionControl.SetWindowText (NUMTOSTR(resolution));
	  metafontModeIdx = idx;
	}
      SetModified (TRUE);
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
}

/* _________________________________________________________________________

   DisplayOptionsPage::OnChangeShrinkFactor
   _________________________________________________________________________ */

void
DisplayOptionsPage::OnChangeShrinkFactor ()
{
  SetModified (TRUE);
}

/* _________________________________________________________________________

   DisplayOptionsPage::OnChangePageMode
   _________________________________________________________________________ */

void
DisplayOptionsPage::OnChangePageMode ()
{
  SetModified (TRUE);
}

/* _________________________________________________________________________

   DisplayOptionsPage::OnChangeUnit
   _________________________________________________________________________ */

void
DisplayOptionsPage::OnChangeUnit ()
{
  SetModified (TRUE);
}
