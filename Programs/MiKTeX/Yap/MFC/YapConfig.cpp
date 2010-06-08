/* YapConfig.cpp: Yap configuration settings

   Copyright (C) 1996-2010 Christian Schenk

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

auto_ptr<YapConfig> g_pYapConfig;

#define DEFAULT_DVIPAGE_MODE DviPageMode::Auto
#define DEFAULT_DVIPAGE_MODE_STRING \
  EnumToString<DviPageMode>(DviPageMode::Auto, dviPageModes)

#define DEFAULT_UNIT Units::BigPoints
#define DEFAULT_UNIT_STRING \
  EnumToString<Units>(Units::BigPoints, units)

template<class Enum>
struct EnumAndString
{
  typename Enum::EnumType num;
  const char * lpsz;
};

namespace
{
  const EnumAndString<DviPageMode> dviPageModes[] = {
    { DviPageMode::Auto, "auto" },
    { DviPageMode::Pk, "pk" },
    { DviPageMode::Dvips, "dvips" },
    { DviPageMode::None, 0 },
  };

  const EnumAndString<Units> units[] = {
    { Units::BigPoints, "bp" },
    { Units::Centimeters, "cm" },
    { Units::Inches, "in" },
    { Units::Millimeters, "mm" },
    { Units::Picas, "pc" },
    { Units::None, 0 },
  };


  const char * const DEFAULT_SECURE_COMMANDS = "\
gunzip,\
zcat,\
bunzip2,\
bzcat";

  const char * const DEFAULT_GAMMA_VALUES = "\
0.01 \
0.05 \
0.1 \
0.2 \
0.3 \
0.4 \
0.5 \
0.6 \
0.7 \
0.8 \
0.9 \
1.0 \
1.25 \
1.5 \
1.75 \
2.0 \
3.0 \
4.0 \
5.0 \
7.5 \
10.0 \
100.0";
};

/* _________________________________________________________________________

   EnumToString
   _________________________________________________________________________ */

template<class Enum>
const char *
EnumToString (/*[in]*/ typename Enum::EnumType		num,
	      /*[in]*/ const EnumAndString<Enum> *	pMap)
{
  for (size_t idx = 0; pMap[idx].lpsz != 0; ++ idx)
    {
      if (pMap[idx].num == num)
	{
	  return (pMap[idx].lpsz);
	}
    }
  UNEXPECTED_CONDITION ("EnumToString");
}

/* _________________________________________________________________________

   StringToEnum
   _________________________________________________________________________ */

template<class Enum>
typename Enum::EnumType
StringToEnum (/*[in]*/ const char *		lpsz,
	      /*[in]*/ const EnumAndString<Enum> *	pMap)
{
  for (size_t idx = 0; pMap[idx].lpsz != 0; ++ idx)
    {
      if (StringCompare(lpsz, pMap[idx].lpsz, true) == 0)
	{
	  return (pMap[idx].num);
	}
    }
  UNEXPECTED_CONDITION ("StringToEnum");
}

/* _________________________________________________________________________

   SetMetafontMode
   _________________________________________________________________________ */

bool
SetMetafontMode (/*[in]*/ const char *	lpszMnemonic,
		 /*[out]*/ unsigned long &	metafontModeIdx)
{
  MIKTEXMFMODE metafontMode;
  for (unsigned long idx = 0;
       SessionWrapper(true)->GetMETAFONTMode(idx, &metafontMode);
       ++ idx)
    {
      if (StringCompare(lpszMnemonic, metafontMode.szMnemonic) == 0)
	{
	  metafontModeIdx = idx;
	  return (true);
	}
    }
  return (false);
}

/* _________________________________________________________________________

   YapConfig::SetDisplayMetafontMode
   _________________________________________________________________________ */

bool
YapConfig::SetDisplayMetafontMode (/*[in]*/ const char * lpszMnemonic)
{
  return (SetMetafontMode(lpszMnemonic, displayMetafontMode));
}

/* _________________________________________________________________________

   YapConfig::SetPrinterMetafontMode
   _________________________________________________________________________ */

bool
YapConfig::SetPrinterMetafontMode (/*[in]*/ const char * lpszMnemonic)
{
  return (SetMetafontMode(lpszMnemonic, printerMetafontMode));
}

/* _________________________________________________________________________

   YapConfig::Load
   _________________________________________________________________________ */

void
YapConfig::Load ()
{
  PathName fileName =
    SessionWrapper(true)->GetSpecialPath(SpecialPath::UserConfigRoot);
  fileName += MIKTEX_PATH_MIKTEX_CONFIG_DIR;
  fileName += MIKTEX_YAP_INI_FILENAME;
  if (File::Exists(fileName))
  {
    pCfg->Read (fileName);
  }

  //
  // [Settings]
  //
  checkFileTypeAssociations =
    GetValue("Settings", "Check Associations", true);
  inverseSearchCommandLine =
    GetValue("Settings", "Editor", CString("notepad \"%f\""));
  lastTool =
    GetValue("Settings", "Last Tool", 0);
  maintainHorizontalPosition =
    GetValue("Settings", "Maintain Horizontal Position", false);
  maintainVerticalPosition =
    GetValue("Settings", "Maintain Vertical Position", false);

#if defined(MIKTEX_DEBUG)
  showBoundingBoxes =
    GetValue("Settings", "Show Bounding Boxes", false);
#endif
  showSourceSpecials =
    GetValue("Settings", "Show Source Specials", false);

  //
  // [screen]
  //
  continuousView =
    GetValue("screen", "ContinuousView", true);
#if WIN95NOCONTVIEW
  if (continuousView && win95)
  {
    continuousView = false;
  }
#endif
  doublePage =
    GetValue("screen", "doublepage", false);
  gamma =
    GetValue("screen", "gamma", 1.0);
  if (gamma <= 0.0)
  {
    gamma = 1.0;
  }
  gammaValues =
    GetValue("screen", "gammavalues", CString(DEFAULT_GAMMA_VALUES));
  CString displayMode =
    GetValue("screen", "mode", CString("ljfour"));
  if (! SetDisplayMetafontMode(displayMode))
  {
    FATAL_MIKTEX_ERROR (
      "YapConfig::Load",
      T_("Unknown METAFONT mode."),
      static_cast<const char *>(displayMode));
  }
  displayShrinkFactor =
    GetValue("screen", "shrinkfactor", 6);
  if (displayShrinkFactor == 0)
  {
    displayShrinkFactor = 1;
  }
  renderGraphicsInBackground =
    GetValue("screen", "graphicsinbackground", DEFAULT_bRenderGraphicsInBackground);
  dviPageMode = StringToEnum<DviPageMode>(
    GetValue("screen", "dvipagemode", CString(DEFAULT_DVIPAGE_MODE_STRING)),
     dviPageModes);
  unit = StringToEnum<Units>(
    GetValue("screen", "unit", CString(DEFAULT_UNIT_STRING)),
     units);

  //
  // [printer]
  //
  CString printerMode =
    GetValue("printer", "mode", CString("ljfour"));
  if (! SetPrinterMetafontMode(printerMode))
  {
    FATAL_MIKTEX_ERROR (
      "YapConfig::Load",
      T_("Unknown METAFONT mode."),
      static_cast<const char *>(printerMode));
  }
  pageXShift =
    GetValue("printer", "nPageXShift", 0);
  pageYShift =
    GetValue("printer", "nPageYShift", 0);

  //
  // [DVIPS]
  //
  dvipsExtraOptions =
    GetValue("DVIPS", "Extra Options",  CString(""));

  //
  // [Magnifying Glass]
  //
  magGlassHidesCursor =
    GetValue("Magnifying Glass", "Hide Cursor", false);
  magGlassOpacity =
    GetValue("Magnifying Glass", "Opacity", 70);

  //
  // [Small Magnifying Glass]
  //
  magGlassSmallHeight =
    GetValue("Small Magnifying Glass", "Height", 150);
  magGlassSmallShrinkFactor =
    GetValue("Small Magnifying Glass", "Shrink Factor",	1);
  if (magGlassSmallShrinkFactor == 0)
  {
    magGlassSmallShrinkFactor = 1;
  }
  magGlassSmallWidth =
    GetValue("Small Magnifying Glass", "Width", 200);

  //
  // [Medium Magnifying Glass]
  //
  magGlassMediumHeight =
    GetValue("Medium Magnifying Glass", "Height", 250);
  magGlassMediumShrinkFactor =
    GetValue("Medium Magnifying Glass", "Shrink Factor", 1);
  if (magGlassMediumShrinkFactor == 0)
  {
    magGlassMediumShrinkFactor = 1;
  }
  magGlassMediumWidth =
    GetValue("Medium Magnifying Glass", "Width", 400);

  //
  // [Large Magnifying Glass]
  //
  magGlassLargeHeight =
    GetValue("Large Magnifying Glass", "Height", 500);
  magGlassLargeShrinkFactor =
   GetValue("Large Magnifying Glass", "Shrink Factor", 1);
  if (magGlassLargeShrinkFactor == 0)
  {
    magGlassLargeShrinkFactor = 1;
  }
  magGlassLargeWidth =
    GetValue("Large Magnifying Glass", "Width", 700);

  //
  // [Security]
  //
  enableShellCommands =
    GetValue("Security", "EnableEmbeddedCommands", SEC_ASK_USER);
  secureCommands =
    GetValue("Security", "SecureCommands", CString(DEFAULT_SECURE_COMMANDS));
}

/* _________________________________________________________________________

   YapConfig::Save
   _________________________________________________________________________ */

void
YapConfig::Save ()
{
  MIKTEXMFMODE mfmode;

  //
  // [Settings]
  //
  UpdateValue (
    "Settings","Check Associations",
    checkFileTypeAssociations,
    true);
  UpdateValue (
    "Settings",
    "Editor",
    inverseSearchCommandLine,
    CString("notepad \"%f\""));
  UpdateValue (
    "Settings",
    "Last Tool",
    lastTool,
    0);
  UpdateValue (
    "Settings",
    "Maintain Horizontal Position",
    maintainHorizontalPosition,
    false);
  UpdateValue (
    "Settings",
    "Maintain Vertical Position",
    maintainVerticalPosition,
    false);
#if defined(MIKTEX_DEBUG)
  UpdateValue (
    "Settings",
    "Show Bounding Boxes",
    showBoundingBoxes,
    false);
#endif
  UpdateValue (
    "Settings",
    "Show Source Specials",
    showSourceSpecials,
    false);

  //
  // [screen]
  //
  UpdateValue (
    "screen",
    "ContinuousView",
    continuousView,
    true);
  UpdateValue (
    "screen",
    "DoublePage",
    doublePage,
    false);
  UpdateValue (
    "screen",
    "gamma",
    gamma,
    1.0);
  UpdateValue (
    "screen",
    "gammavalues",
    gammaValues,
    CString(DEFAULT_GAMMA_VALUES));
  if (SessionWrapper(true)->GetMETAFONTMode(displayMetafontMode, &mfmode))
  {
    UpdateValue (
      "screen",
      "mode",
      CString(mfmode.szMnemonic),
      CString("ljfour"));
  }
  UpdateValue ("screen",
		  "shrinkfactor",
		  displayShrinkFactor,
		  6);
  UpdateValue (
    "screen",
    "graphicsinbackground",
    renderGraphicsInBackground,
    DEFAULT_bRenderGraphicsInBackground);
  UpdateValue (
    "screen",
    "dvipagemode",
    CString(EnumToString<DviPageMode>(dviPageMode.Get(), dviPageModes)),
    CString(DEFAULT_DVIPAGE_MODE_STRING));
  UpdateValue ("screen",
    "unit",
    CString(EnumToString<Units>(unit.Get(), units)),
    CString(DEFAULT_UNIT_STRING));

  //
  // [printer]
  //
  if (SessionWrapper(true)->GetMETAFONTMode(printerMetafontMode, &mfmode))
  {
    UpdateValue (
      "printer",
      "mode",
      CString(mfmode.szMnemonic),
      CString("ljfour"));
  }
  UpdateValue (
    "printer",
    "nPageXShift",
    pageXShift,
    0);
  UpdateValue (
    "printer",
    "nPageYShift",
    pageYShift,
    0);

  //
  // [DVIPS]
  //
  UpdateValue (
    "DVIPS",
    "Extra Options",
    dvipsExtraOptions,
    CString(""));

  //
  // [Magnifying Glass]
  //
  UpdateValue (
    "Magnifying Glass",
    "Hide Cursor",
    magGlassHidesCursor,
    false);
  UpdateValue (
    "Magnifying Glass",
    "Opacity",
    magGlassOpacity,
    70);

  //
  // [Small Magnifying Glass]
  //
  UpdateValue (
    "Small Magnifying Glass",
    "Height",
    magGlassSmallHeight,
    150);
  UpdateValue (
    "Small Magnifying Glass",
    "Shrink Factor",
    magGlassSmallShrinkFactor,
    1);
  UpdateValue (
    "Small Magnifying Glass",
    "Width",
    magGlassSmallWidth,
    200);

  //
  // [Medium Magnifying Glass]
  //
  UpdateValue (
    "Medium Magnifying Glass",
    "Height",
    magGlassMediumHeight,
    250);
  UpdateValue (
    "Medium Magnifying Glass",
    "Shrink Factor",
    magGlassMediumShrinkFactor,
    1);
  UpdateValue (
    "Medium Magnifying Glass",
    "Width",
    magGlassMediumWidth,
    400);

  //
  // [Large Magnifying Glass]
  //
  UpdateValue (
    "Large Magnifying Glass",
    "Height",
    magGlassLargeHeight,
    500);
  UpdateValue (
    "Large Magnifying Glass",
    "Shrink Factor",
    magGlassLargeShrinkFactor,
    1);
  UpdateValue (
    "Large Magnifying Glass",
    "Width",
    magGlassLargeWidth,
    700);

  //
  // [Security]
  //
  UpdateValue (
    "Security",
    "EnableEmbeddedCommands",
    enableShellCommands,
    int(SEC_ASK_USER));
  UpdateValue (
    "Security",
    "SecureCommands",
    secureCommands,
    CString(DEFAULT_SECURE_COMMANDS));

  PathName fileName =
    SessionWrapper(true)->GetSpecialPath(SpecialPath::UserConfigRoot);
  fileName += MIKTEX_PATH_MIKTEX_CONFIG_DIR;
  fileName += MIKTEX_YAP_INI_FILENAME;
  bool firstTime = ! File::Exists(fileName);
  pCfg->Write (fileName);
  if (firstTime)
  {
    Fndb::Add (fileName);
  }
}

/* _________________________________________________________________________

   YapConfig::YapConfig
   _________________________________________________________________________ */

YapConfig::YapConfig ()
  : dviPageMode (DEFAULT_DVIPAGE_MODE),
    unit (Units::None),
    win95 ((GetVersion() & 0x80000000) != 0),
    pCfg (Cfg::Create())
{
  Load ();
}

/* _________________________________________________________________________

   YapConfig::~YapConfig
  _________________________________________________________________________ */

YapConfig::~YapConfig ()
{
#if 0
  try
    {
      Save ();
    }
  catch (const exception &)
    {
    }
#endif
}
