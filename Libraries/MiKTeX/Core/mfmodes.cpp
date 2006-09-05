/* mfmodes.cpp: METAFONT modes

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

/* _________________________________________________________________________

   ModeComparer
   _________________________________________________________________________ */

struct ModeComparer
{
  bool
  operator() (/*[in]*/ const MIKTEXMFMODE & lhs,
	      /*[in]*/ const MIKTEXMFMODE & rhs)
    const
  {
    return (StringCompare(lhs.szDescription, rhs.szDescription) < 0);
  }
};

/* _________________________________________________________________________

   SessionImpl::ReadMetafontModes
   _________________________________________________________________________ */

void
SessionImpl::ReadMetafontModes ()
{
  PathName path;

  if (! FindFile(T_("modes.mf"), FileType::MF, path))
    {
      FATAL_MIKTEX_ERROR (T_("SessionImpl::ReadMetafontModes"),
			  T_("cannot find modes.mf"),
			  0);
    }

  StreamReader reader (path);

  bool readingModeDef = false;

  tstring line;

  metafontModes.reserve (200);

  MIKTEXMFMODE mfmode;
  
  memset (&mfmode, 0, sizeof(mfmode));

  while (reader.ReadLine(line))
    {
      if (readingModeDef)
	{
	  const MIKTEXCHAR * lpsz;
	  if (StrNCmp(line.c_str(), T_("enddef;"), 7) == 0)
	    {
	      metafontModes.push_back (mfmode);
	      readingModeDef = false;
	    }
	  else if (mfmode.iHorzRes == 0
		   && ((lpsz = StrStr(line.c_str(), T_("pixels_per_inch")))
		       != 0))
	    {
	      SkipNonDigit (lpsz);
	      mfmode.iHorzRes =
		mfmode.iVertRes = AToI(lpsz);
	    }
	  else if ((lpsz = StrStr(line.c_str(), T_("aspect_ratio"))) != 0)
	    {
	      SkipNonDigit (lpsz);
	      mfmode.iVertRes = AToI(lpsz);
	    }
	}
      else if (StrNCmp(line.c_str(), T_("mode_def"), 8) == 0)
	{
	  const MIKTEXCHAR * lpsz = line.c_str() + 8;
	  while (! IsAlpha(*lpsz) && *lpsz != T_('\n'))
	    {
	      ++lpsz;
	    }
	  if (! IsAlpha(*lpsz))
	    {
	      continue;
	    }
	  const MIKTEXCHAR * lpszModeName = lpsz;
	  SkipAlpha (lpsz);
	  if (*lpsz == 0)
	    {
	      continue;
	    }
	  *const_cast<MIKTEXCHAR *>(lpsz++) = 0;
	  if (StringCompare(lpszModeName, T_("help")) == 0)
	    {
	      continue;
	    }
	  lpsz = StrStr(lpsz, T_("%\\["));
	  if (lpsz == 0)
	    {
	      continue;
	    }
	  lpsz += 3;
	  SkipSpace (lpsz);
	  if (lpsz == 0)
	    {
	      continue;
	    }
	  const MIKTEXCHAR * printer_name = lpsz;
	  readingModeDef = true;
	  Utils::CopyString (mfmode.szMnemonic,
			     ARRAY_SIZE(mfmode.szMnemonic),
			     lpszModeName);
	  Utils::CopyString (mfmode.szDescription,
			     ARRAY_SIZE(mfmode.szDescription),
			     printer_name);
	  mfmode.iHorzRes = 0;
	}
    }

  sort (metafontModes.begin(), metafontModes.end(), ModeComparer());
}

/* _________________________________________________________________________

   SessionImpl::GetMETAFONTMode
   _________________________________________________________________________ */

bool
SessionImpl::GetMETAFONTMode (/*[in]*/ unsigned		idx,
			      /*[out]*/ MIKTEXMFMODE *	pMode)
{
  MIKTEX_ASSERT_BUFFER (pMode, sizeof(*pMode));

  if (metafontModes.size() == 0)
    {
      ReadMetafontModes ();
    }

  if (idx > metafontModes.size())
    {
      INVALID_ARGUMENT (T_("SessionImpl::GetMETAFONTMode"), NUMTOSTR(idx));
    }
  else if (idx == metafontModes.size())
    {
      return (false);
    }

  *pMode = metafontModes[idx];

  return (true);
}

/* _________________________________________________________________________

   SessionImpl::FindMETAFONTMode
   _________________________________________________________________________ */

bool
SessionImpl::FindMETAFONTMode (/*[in]*/ const MIKTEXCHAR *	lpszMnemonic,
			       /*[out]*/ MIKTEXMFMODE *		pMode)
{
  MIKTEXMFMODE candidate;

  for (unsigned long idx = 0; GetMETAFONTMode(idx, &candidate); ++ idx)
    {
      if (StringCompare(lpszMnemonic, candidate.szMnemonic) == 0)
	{
	  *pMode = candidate;
	  return (true);
	}
    }

  return (false);
}

/* _________________________________________________________________________

   SessionImpl::DetermineMETAFONTMode
   _________________________________________________________________________ */

bool
SessionImpl::DetermineMETAFONTMode (/*[in]*/ unsigned		dpi,
				    /*[out]*/ MIKTEXMFMODE *	pMode)
{
  MIKTEX_ASSERT_BUFFER (pMode, sizeof(*pMode));

  const MIKTEXCHAR * lpszMode = 0;

  // favour well known modes
  switch (dpi)
    {
    case 85:
      lpszMode = T_("sun");
      break;
    case 100:
      lpszMode = T_("nextscrn");
      break;
    case 180:
      lpszMode = T_("toshiba");
      break;
    case 300:
      lpszMode = T_("cx");
      break;
    case 400:
      lpszMode = T_("nexthi");
      break;
    case 600:
      lpszMode = T_("ljfour");
      break;
    case 1270:
      lpszMode = T_("linoone");
      break;
    }

  MIKTEXMFMODE candidate;

  if (lpszMode != 0
      && FindMETAFONTMode(lpszMode, &candidate)
      && candidate.iHorzRes == static_cast<int>(dpi))
    {
      *pMode = candidate;
      return (true);
    }

  for (unsigned long i = 0; GetMETAFONTMode(i, &candidate); ++ i)
    {
      if (candidate.iHorzRes == static_cast<int>(dpi))
	{
	  *pMode = candidate;
	  return (true);
	}
    }

  return (false);
}

/* _________________________________________________________________________

   miktex_determine_metafont_mode
   _________________________________________________________________________ */

MIKTEXAPI(int)
miktex_determine_metafont_mode (/*[in]*/ unsigned	dpi,
				/*[out]*/ MIKTEXCHAR *	lpszMnemonic,
				/*[in]*/ size_t		bufSize)
{
  MIKTEX_ASSERT_CHAR_BUFFER (lpszMnemonic, bufSize);
  MIKTEXMFMODE mode;
  if (! SessionImpl::GetSession()->DetermineMETAFONTMode(dpi, &mode))
    {
      return (0);
    }
  Utils::CopyString (lpszMnemonic, bufSize, mode.szMnemonic);
  return (1);
}
