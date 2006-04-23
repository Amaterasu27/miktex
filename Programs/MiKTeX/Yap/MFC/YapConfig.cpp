/* YapConfig.cpp: Yap configuration settings

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

auto_ptr<YapConfig> g_pYapConfig;

#define DEFAULT_DVIPAGE_MODE DviPageMode::Pk
#define DEFAULT_DVIPAGE_MODE_STRING \
  EnumToString<DviPageMode>(DviPageMode::Pk, dviPageModes)

#define DEFAULT_UNIT Units::BigPoints
#define DEFAULT_UNIT_STRING \
  EnumToString<Units>(Units::BigPoints, units)

template<class Enum>
struct EnumAndString
{
  typename Enum::EnumType num;
  const MIKTEXCHAR * lpsz;
};

namespace
{
  const EnumAndString<DviPageMode> dviPageModes[] = {
    { DviPageMode::Auto, T_("auto") },
    { DviPageMode::Pk, T_("pk") },
    { DviPageMode::Dvips, T_("dvips") },
    { DviPageMode::None, 0 },
  };

  const EnumAndString<Units> units[] = {
    { Units::BigPoints, T_("bp") },
    { Units::Centimeters, T_("cm") },
    { Units::Inches, T_("in") },
    { Units::Millimeters, T_("mm") },
    { Units::Picas, T_("pc") },
    { Units::None, 0 },
  };


  const MIKTEXCHAR * const DEFAULT_SECURE_COMMANDS = T_("\
gunzip,\
zcat,\
bunzip2,\
bzcat");

  const MIKTEXCHAR * const DEFAULT_GAMMA_VALUES = T_("\
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
100.0");
};

/* _________________________________________________________________________

   EnumToString
   _________________________________________________________________________ */

template<class Enum>
const MIKTEXCHAR *
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
  UNEXPECTED_CONDITION (T_("EnumToString"));
}

/* _________________________________________________________________________

   StringToEnum
   _________________________________________________________________________ */

template<class Enum>
typename Enum::EnumType
StringToEnum (/*[in]*/ const MIKTEXCHAR *		lpsz,
	      /*[in]*/ const EnumAndString<Enum> *	pMap)
{
  for (size_t idx = 0; pMap[idx].lpsz != 0; ++ idx)
    {
      if (StringCompare(lpsz, pMap[idx].lpsz, true) == 0)
	{
	  return (pMap[idx].num);
	}
    }
  UNEXPECTED_CONDITION (T_("StringToEnum"));
}

/* _________________________________________________________________________

   GetProfileBool
   _________________________________________________________________________ */

bool
GetProfileBool (/*[in]*/ const MIKTEXCHAR *	lpszKeyName,
		/*[in]*/ const MIKTEXCHAR *	lpszValueName,
		/*[in]*/ bool			defaultValue)
{
   CWinApp * pApp = AfxGetApp();
   ASSERT_VALID (pApp);
   return (pApp->GetProfileInt(lpszKeyName,
			       lpszValueName,
			       static_cast<int>(defaultValue))
	   != 0);
}

/* _________________________________________________________________________

   GetProfileBool
   _________________________________________________________________________ */

double
GetProfileDouble (/*[in]*/ const MIKTEXCHAR *	lpszKeyName,
		  /*[in]*/ const MIKTEXCHAR *	lpszValueName,
		  /*[in]*/ double		fDefault)
{
   CWinApp * pApp = AfxGetApp();
   ASSERT_VALID (pApp);
   CString strDefault;
   strDefault.Format ("%f", fDefault);
   CString str =
     pApp->GetProfileString(lpszKeyName, lpszValueName, strDefault);
   double dbl = _tstof(str);
   return (dbl);
}

/* _________________________________________________________________________

   RemoveDefaultValue
   _________________________________________________________________________ */

bool
RemoveDefaultValue (/*[in]*/ const MIKTEXCHAR *	lpszKeyName,
		    /*[in]*/ const MIKTEXCHAR *	lpszValueName)
{
  CString strPath;
  strPath.Format (T_("Software\\MiK\\MiKTeX\\yap\\%s"), lpszKeyName);
  HKEY hKey;
  LONG res =
    RegOpenKeyEx(HKEY_CURRENT_USER,
		 strPath,
		 0,
		 KEY_ALL_ACCESS,
		 &hKey);
  if (res == ERROR_SUCCESS)
    {
      res = RegDeleteValue(hKey, lpszValueName);
      RegCloseKey (hKey);
    }
  return (res == ERROR_SUCCESS);
}

/* _________________________________________________________________________

   UpdateRegValue
   _________________________________________________________________________ */

void
UpdateRegValue (/*[in]*/ const MIKTEXCHAR *	lpszKeyName,
		/*[in]*/ const MIKTEXCHAR *	lpszValueName,
		/*[in]*/ bool			value,
		/*[in]*/ bool			defaultValue)
{
  if (value == defaultValue && RemoveDefaultValue(lpszKeyName, lpszValueName))
    {
      return;
    }
  bool oldValue = GetProfileBool(lpszKeyName, lpszValueName, defaultValue);
  if (oldValue != value)
    {
      CWinApp * pApp = AfxGetApp();
      ASSERT_VALID (pApp);
      pApp->WriteProfileInt (lpszKeyName, lpszValueName, (value ? 1 : 0));
    }
}

/* _________________________________________________________________________

   UpdateRegValue
   _________________________________________________________________________ */

void
UpdateRegValue (/*[in]*/ const MIKTEXCHAR *	lpszKeyName,
		/*[in]*/ const MIKTEXCHAR *	lpszValueName,
		/*[in]*/ double			value,
		/*[in]*/ double			defaultValue)
{
  if (value == defaultValue && RemoveDefaultValue(lpszKeyName, lpszValueName))
    {
      return;
    }
  double oldValue = GetProfileDouble(lpszKeyName, lpszValueName, defaultValue);
  if (oldValue != value)
    {
      CWinApp * pApp = AfxGetApp();
      ASSERT_VALID (pApp);
      CString strVal;
      strVal.Format (T_("%f"), value);
      pApp->WriteProfileString (lpszKeyName, lpszValueName, strVal);
    }
}

/* _________________________________________________________________________

   UpdateRegValue
   _________________________________________________________________________ */

void
UpdateRegValue (/*[in]*/ const MIKTEXCHAR *	lpszKeyName,
		/*[in]*/ const MIKTEXCHAR *	lpszValueName,
		/*[in]*/ int			value,
		/*[in]*/ int			defaultValue)
{
  if (value == defaultValue && RemoveDefaultValue(lpszKeyName, lpszValueName))
    {
      return;
    }
  CWinApp * pApp = AfxGetApp();
  ASSERT_VALID (pApp);
  int oldValue = pApp->GetProfileInt(lpszKeyName, lpszValueName, defaultValue);
  if (oldValue != value)
    {
      pApp->WriteProfileInt (lpszKeyName, lpszValueName, value);
    }
}

/* _________________________________________________________________________

   UpdateRegValue
   _________________________________________________________________________ */

void
UpdateRegValue (/*[in]*/ const MIKTEXCHAR *		lpszKeyName,
		/*[in]*/ const MIKTEXCHAR *		lpszValueName,
		/*[in]*/ const CString &		value,
		/*[in]*/ const MIKTEXCHAR *		lpszDefaultValue)
{
  if (value == (lpszDefaultValue ? lpszDefaultValue : T_(""))
      && RemoveDefaultValue(lpszKeyName, lpszValueName))
    {
      return;
    }
  CWinApp * pApp = AfxGetApp();
  ASSERT_VALID (pApp);
  CString oldValue
    = pApp->GetProfileString(lpszKeyName, lpszValueName, lpszDefaultValue);
  if (! (oldValue == value))
    {
      pApp->WriteProfileString (lpszKeyName, lpszValueName, value);
    }
}

/* _________________________________________________________________________

   SetMetafontMode
   _________________________________________________________________________ */

bool
SetMetafontMode (/*[in]*/ const MIKTEXCHAR *	lpszMnemonic,
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
YapConfig::SetDisplayMetafontMode (/*[in]*/ const MIKTEXCHAR * lpszMnemonic)
{
  return (SetMetafontMode(lpszMnemonic, displayMetafontMode));
}

/* _________________________________________________________________________

   YapConfig::SetPrinterMetafontMode
   _________________________________________________________________________ */

bool
YapConfig::SetPrinterMetafontMode (/*[in]*/ const MIKTEXCHAR * lpszMnemonic)
{
  return (SetMetafontMode(lpszMnemonic, printerMetafontMode));
}

/* _________________________________________________________________________

   YapConfig::Load
   _________________________________________________________________________ */

void
YapConfig::Load ()
{
  CWinApp * pApp = AfxGetApp();

  ASSERT_VALID (pApp);

  //
  // HKCU/Software/MiK/MiKTeX/Yap/Settings
  //
  checkFileTypeAssociations =
    GetProfileBool(T_("Settings"),
		   T_("Check Associations"),
		   true);
  inverseSearchCommandLine =
    pApp->GetProfileString(T_("Settings"),
			   T_("Editor"),
			   T_("notepad \"%f\""));
  lastTool =
    pApp->GetProfileInt(T_("Settings"),
			T_("Last Tool"),
			0);
  maintainHorizontalPosition =
    GetProfileBool(T_("Settings"),
		   T_("Maintain Horizontal Position"),
		   false);
  maintainVerticalPosition =
    GetProfileBool(T_("Settings"),
		   T_("Maintain Vertical Position"),
		   false);

#if ! defined(NDEBUG)
  showBoundingBoxes =
    GetProfileBool(T_("Settings"),
		   T_("Show Bounding Boxes"),
		   false);
#endif
  showSourceSpecials =
    GetProfileBool(T_("Settings"),
		   T_("Show Source Specials"),
		   false);

  //
  // HKCU/Software/MiK/MiKTeX/Yap/screen
  //
  continuousView =
    GetProfileBool(T_("screen"),
		   T_("ContinuousView"),
		   true);
#if WIN95NOCONTVIEW
  if (continuousView && win95)
    {
      continuousView = false;
    }
#endif
  doublePage =
    GetProfileBool(T_("screen"),
		   T_("doublepage"),
		   false);
  gamma =
    GetProfileDouble(T_("screen"),
		     T_("gamma"),
		     1.0);
  if (gamma <= 0.0)
    {
      gamma = 1.0;
    }
  gammaValues =
    pApp->GetProfileString(T_("screen"),
			   T_("gammavalues"),
			   DEFAULT_GAMMA_VALUES);
  CString displayMode =
    pApp->GetProfileString(T_("screen"),
			   T_("mode"),
			   T_("ljfour"));
  if (! SetDisplayMetafontMode(displayMode))
    {
      FATAL_MIKTEX_ERROR (T_("YapConfig::Load"),
			  T_("Unknown METAFONT mode."),
			  static_cast<const MIKTEXCHAR *>(displayMode));
    }
  displayShrinkFactor =
    pApp->GetProfileInt(T_("screen"),
			T_("shrinkfactor"),
			6);
  if (displayShrinkFactor == 0)
    {
      displayShrinkFactor = 1;
    }
  renderGraphicsInBackground =
    GetProfileBool(T_("screen"),
		   T_("graphicsinbackground"),
		   DEFAULT_bRenderGraphicsInBackground);
  dviPageMode =
    StringToEnum<DviPageMode>
    (pApp->GetProfileString(T_("screen"),
			    T_("dvipagemode"),
			    DEFAULT_DVIPAGE_MODE_STRING),
     dviPageModes);
  unit =
    StringToEnum<Units>
    (pApp->GetProfileString(T_("screen"),
			    T_("unit"),
			    DEFAULT_UNIT_STRING),
     units);

  //
  // HKCU/Software/MiK/MiKTeX/Yap/printer
  //
  CString printerMode =
    pApp->GetProfileString(T_("printer"),
			   T_("mode"),
			   T_("ljfour"));
  if (! SetPrinterMetafontMode(printerMode))
    {
      FATAL_MIKTEX_ERROR (T_("YapConfig::Load"),
			  T_("Unknown METAFONT mode."),
			  static_cast<const MIKTEXCHAR *>(printerMode));
    }
  pageXShift =
    pApp->GetProfileInt(T_("printer"),
			T_("nPageXShift"),
			0);
  pageYShift =
    pApp->GetProfileInt(T_("printer"),
			T_("nPageYShift"),
			0);

  //
  // HKCU/Software/MiK/MiKTeX/Yap/DVIPS
  //
  dvipsExtraOptions =
    pApp->GetProfileString(T_("DVIPS"),
			   T_("Extra Options"));

  //
  // HKCU/Software/MiK/MiKTeX/Yap/Magnifying Glass
  //
  magGlassHidesCursor =
    GetProfileBool(T_("Magnifying Glass"),
		   T_("Hide Cursor"),
		   false);
  magGlassOpacity =
    pApp->GetProfileInt(T_("Magnifying Glass"),
			T_("Opacity"),
			70);

  //
  // HKCU/Software/MiK/MiKTeX/Yap/Small Magnifying Glass
  //
  magGlassSmallHeight =
    pApp->GetProfileInt(T_("Small Magnifying Glass"),
			T_("Height"),
			150);
  magGlassSmallShrinkFactor =
    pApp->GetProfileInt(T_("Small Magnifying Glass"),
			T_("Shrink Factor"),
			1);
  if (magGlassSmallShrinkFactor == 0)
    {
      magGlassSmallShrinkFactor = 1;
    }
  magGlassSmallWidth =
    pApp->GetProfileInt(T_("Small Magnifying Glass"),
			T_("Width"),
			200);

  //
  // HKCU/Software/MiK/MiKTeX/Yap/Medium Magnifying Glass
  //
  magGlassMediumHeight =
    pApp->GetProfileInt(T_("Medium Magnifying Glass"),
			T_("Height"),
			250);
  magGlassMediumShrinkFactor =
    pApp->GetProfileInt(T_("Medium Magnifying Glass"),
			T_("Shrink Factor"),
			1);
  if (magGlassMediumShrinkFactor == 0)
    {
      magGlassMediumShrinkFactor = 1;
    }
  magGlassMediumWidth =
    pApp->GetProfileInt(T_("Medium Magnifying Glass"),
			T_("Width"),
			400);

  //
  // HKCU/Software/MiK/MiKTeX/Yap/Large Magnifying Glass
  //
  magGlassLargeHeight =
    pApp->GetProfileInt(T_("Large Magnifying Glass"),
			T_("Height"),
			500);
  magGlassLargeShrinkFactor =
    pApp->GetProfileInt(T_("Large Magnifying Glass"),
			T_("Shrink Factor"),
			1);
  if (magGlassLargeShrinkFactor == 0)
    {
      magGlassLargeShrinkFactor = 1;
    }
  magGlassLargeWidth =
    pApp->GetProfileInt(T_("Large Magnifying Glass"),
			T_("Width"),
			700);

  //
  // HKCU/Software/MiK/MiKTeX/Yap/Security
  //
  enableShellCommands =
    pApp->GetProfileInt(T_("Security"),
			T_("EnableEmbeddedCommands"),
			SEC_ASK_USER);
  secureCommands =
    pApp->GetProfileString(T_("Security"),
			   T_("SecureCommands"),
			   DEFAULT_SECURE_COMMANDS);
}

/* _________________________________________________________________________

   YapConfig::Save
   _________________________________________________________________________ */

void
YapConfig::Save ()
{
  MIKTEXMFMODE mfmode;

  //
  // HKCU/Software/MiK/MiKTeX/Yap/Settings
  //
  UpdateRegValue (T_("Settings"),
		  T_("Check Associations"),
		  checkFileTypeAssociations,
		  true);
  UpdateRegValue (T_("Settings"),
		  T_("Editor"),
		  inverseSearchCommandLine,
		  T_("notepad \"%f\""));
  UpdateRegValue (T_("Settings"),
		  T_("Last Tool"),
		  lastTool,
		  0);
  UpdateRegValue (T_("Settings"),
		  T_("Maintain Horizontal Position"),
		  maintainHorizontalPosition,
		  false);
  UpdateRegValue (T_("Settings"),
		  T_("Maintain Vertical Position"),
		  maintainVerticalPosition,
		  false);
#if ! defined(NDEBUG)
  UpdateRegValue (T_("Settings"),
		  T_("Show Bounding Boxes"),
		  showBoundingBoxes,
		  false);
#endif
  UpdateRegValue (T_("Settings"),
		  T_("Show Source Specials"),
		  showSourceSpecials,
		  false);

  //
  // HKCU/Software/MiK/MiKTeX/Yap/screen
  //
  UpdateRegValue (T_("screen"),
		  T_("ContinuousView"),
		  continuousView,
		  true);
  UpdateRegValue (T_("screen"),
		  T_("DoublePage"),
		  doublePage,
		  false);
  UpdateRegValue (T_("screen"),
		  T_("gamma"),
		  gamma,
		  1.0);
  UpdateRegValue (T_("screen"),
		  T_("gammavalues"),
		  gammaValues,
		  DEFAULT_GAMMA_VALUES);
  if (SessionWrapper(true)->GetMETAFONTMode(displayMetafontMode, &mfmode))
    {
      UpdateRegValue (T_("screen"),
		      T_("mode"),
		      CString(mfmode.szMnemonic),
		      T_("ljfour"));
    }
  UpdateRegValue (T_("screen"),
		  T_("shrinkfactor"),
		  displayShrinkFactor,
		  6);
  UpdateRegValue (T_("screen"),
		  T_("graphicsinbackground"),
		  renderGraphicsInBackground,
		  DEFAULT_bRenderGraphicsInBackground);
  UpdateRegValue (T_("screen"),
		  T_("dvipagemode"),
		  CString(EnumToString<DviPageMode>(dviPageMode.Get(),
						    dviPageModes)),
		  DEFAULT_DVIPAGE_MODE_STRING);
  UpdateRegValue (T_("screen"),
		  T_("unit"),
		  CString(EnumToString<Units>(unit.Get(),
					      units)),
		  DEFAULT_UNIT_STRING);

  //
  // HKCU/Software/MiK/MiKTeX/Yap/printer
  //
  if (SessionWrapper(true)->GetMETAFONTMode(printerMetafontMode, &mfmode))
    {
      UpdateRegValue (T_("printer"),
		      T_("mode"),
		      CString(mfmode.szMnemonic),
		      T_("ljfour"));
    }
  UpdateRegValue (T_("printer"),
		  T_("nPageXShift"),
		  pageXShift,
		  0);
  UpdateRegValue (T_("printer"),
		  T_("nPageYShift"),
		  pageYShift,
		  0);

  //
  // HKCU/Software/MiK/MiKTeX/Yap/DVIPS
  //
  UpdateRegValue (T_("DVIPS"),
		  T_("Extra Options"),
		  dvipsExtraOptions,
		  static_cast<const MIKTEXCHAR *>(0));

  //
  // HKCU/Software/MiK/MiKTeX/Yap/Magnifying Glass
  //
  UpdateRegValue (T_("Magnifying Glass"),
		  T_("Hide Cursor"),
		  magGlassHidesCursor,
		  false);
  UpdateRegValue (T_("Magnifying Glass"),
		  T_("Opacity"),
		  magGlassOpacity,
		  70);

  //
  // HKCU/Software/MiK/MiKTeX/Yap/Small Magnifying Glass
  //
  UpdateRegValue (T_("Small Magnifying Glass"),
		  T_("Height"),
		  magGlassSmallHeight,
		  150);
  UpdateRegValue (T_("Small Magnifying Glass"),
		  T_("Shrink Factor"),
		  magGlassSmallShrinkFactor,
		  1);
  UpdateRegValue (T_("Small Magnifying Glass"),
		  T_("Width"),
		  magGlassSmallWidth,
		  200);

  //
  // HKCU/Software/MiK/MiKTeX/Yap/Medium Magnifying Glass
  //
  UpdateRegValue (T_("Medium Magnifying Glass"),
		  T_("Height"),
		  magGlassMediumHeight,
		  250);
  UpdateRegValue (T_("Medium Magnifying Glass"),
		  T_("Shrink Factor"),
		  magGlassMediumShrinkFactor,
		  1);
  UpdateRegValue (T_("Medium Magnifying Glass"),
		  T_("Width"),
		  magGlassMediumWidth,
		  400);

  //
  // HKCU/Software/MiK/MiKTeX/Yap/Large Magnifying Glass
  //
  UpdateRegValue (T_("Large Magnifying Glass"),
		  T_("Height"),
		  magGlassLargeHeight,
		  500);
  UpdateRegValue (T_("Large Magnifying Glass"),
		  T_("Shrink Factor"),
		  magGlassLargeShrinkFactor,
		  1);
  UpdateRegValue (T_("Large Magnifying Glass"),
		  T_("Width"),
		  magGlassLargeWidth,
		  700);

  //
  // HKCU/Software/MiK/MiKTeX/Yap/Security
  //
  UpdateRegValue (T_("Security"),
		  T_("EnableEmbeddedCommands"),
		  enableShellCommands,
		  SEC_ASK_USER);
  UpdateRegValue (T_("Security"),
		  T_("SecureCommands"),
		  secureCommands,
		  DEFAULT_SECURE_COMMANDS);
}

/* _________________________________________________________________________

   YapConfig::YapConfig
   _________________________________________________________________________ */

YapConfig::YapConfig ()
  : dviPageMode (DEFAULT_DVIPAGE_MODE),
    unit (Units::None),
    win95 ((GetVersion() & 0x80000000) != 0)
{
  Load ();
}

/* _________________________________________________________________________

   YapConfig::~YapConfig
  _________________________________________________________________________ */

YapConfig::~YapConfig ()
{
  try
    {
      Save ();
    }
  catch (const exception &)
    {
    }
}
