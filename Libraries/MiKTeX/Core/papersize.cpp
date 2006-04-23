/* papersize.cpp: paper size info

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

   IsKnownPaperSize
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
IsKnownPaperSize (/*[in]*/ int		width,
		  /*[in]*/ int		height,
		  /*[in]*/ tstring &	name)
{
#define mm2bp(mm) static_cast<int>(((mm) * (1.0 / 25.4) * 72.0) + 0.5)
#define inch2bp(inch) static_cast<int>(((inch) * 72.0) + 0.5)

  static const struct
  {
    const MIKTEXCHAR *	lpszName;
    int			width;
    int			height;
  } knownPaperSizes[] = {
    T_("A3"), mm2bp(297), mm2bp(420),
    T_("A4"), mm2bp(210), mm2bp(297),
    T_("A5"), mm2bp(148), mm2bp(210),
    T_("A6"), mm2bp(257), mm2bp(364),
    T_("Ledger"), inch2bp(17), inch2bp(11),
    T_("Legal"), inch2bp(8.5), inch2bp(14),
    T_("Letter"), inch2bp(8.5), inch2bp(11),
    T_("Tabloid"), inch2bp(11), inch2bp(17),
  };
  for (size_t idx = 0;
       idx < sizeof(knownPaperSizes) / sizeof(knownPaperSizes[0]);
       ++ idx)
    {
      if (knownPaperSizes[idx].width == width
	  && knownPaperSizes[idx].height == height)
	{
	  name = knownPaperSizes[idx].lpszName;
	  return (true);
	}
    }
  return (false);
}

/* _________________________________________________________________________

   CalculatePostScriptPoints
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(int)
CalculatePostScriptPoints (/*[in]*/ double		value,
			   /*[in]*/ const char *	lpszUnit)
{
  static const struct
  {
    const char *	lpszUnit;
    double		factor;
  } unittable[] = {
    { T_("pt"), (72.27)				/ 72.0 },
    { T_("pc"), (72.27 / 12)			/ 72.0 },
    { T_("in"), (1.0)				/ 72.0 },
    { T_("bp"), (72.0)				/ 72.0 },
    { T_("cm"), (2.54)				/ 72.0 },
    { T_("mm"), (25.4)				/ 72.0 },
    { T_("dd"), (72.27 / (1238.0 / 1157))	/ 72.0 },
    { T_("cc"), (72.27 / 12 / (1238.0 / 1157))	/ 72.0 },
    { T_("sp"), (72.27 * 65536)			/ 72.0 },
  };
  for (size_t idx = 0; idx < sizeof(unittable) / sizeof(unittable[0]); ++ idx)
    {
      if (StringCompare(unittable[idx].lpszUnit, lpszUnit) == 0)
	{
#define round(x) static_cast<int>(x + 0.5)
	  return (round(value / unittable[idx].factor));
	}
    }
  FATAL_MIKTEX_ERROR (T_("CalculatePostScriptPoints"),
		      T_("Invalid unit."),
		      lpszUnit);
}

/* _________________________________________________________________________

   ChopFloat
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
ChopFloat (/*[in,out]*/ MIKTEXCHAR * &	lpsz,
	   /*[out]*/ double &		ret)
{
  SkipSpace (lpsz);
  bool gotDot = false;
  MIKTEXCHAR * lpszStart = lpsz;
  tstring str;
  while (*lpsz != 0 && (IsDigit(*lpsz) || (*lpsz == T_('.') && ! gotDot)))
    {
      str += *lpsz;
      if (*lpsz == T_('.'))
	{
	  gotDot = true;
	}
      *lpsz = 0;
      ++ lpsz;
    }
  if (lpsz == lpszStart)
    {
      return (false);
    }
#if defined(_MSC_VER)
  ret = _tstof(str.c_str());
#elif defined(MIKTEX_UNICODE)
#  error Unimplemented: ChopFloat
#else
  ret = atof(str.c_str());
#endif
  return (true);
}

/* _________________________________________________________________________

   ChopToken
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
ChopToken (/*[in,out]*/ MIKTEXCHAR * &	lpsz,
	   /*[out]*/ tstring &		ret)
{
  SkipSpace (lpsz);
  MIKTEXCHAR * lpszStart = lpsz;
  ret = T_("");
  while (*lpsz != 0 && ! IsSpace(*lpsz) && *lpsz != T_(','))
    {
      ret += *lpsz;
      *lpsz = 0;
      ++ lpsz;
    }
  return (lpsz != lpszStart);
}

/* _________________________________________________________________________

   SessionImpl::AddDvipsPaperSize
   _________________________________________________________________________ */

void
SessionImpl::AddDvipsPaperSize
(/*[in]*/ const DvipsPaperSizeInfo & dvipsPaperSizeInfo)
{
  for (vector<DvipsPaperSizeInfo>::iterator it = dvipsPaperSizes.begin();
       it != dvipsPaperSizes.end();
       ++ it)
    {
      if (StringCompare(it->dvipsName.c_str(),
			dvipsPaperSizeInfo.dvipsName.c_str(),
			true)
	  == 0)
	{
	  *it = dvipsPaperSizeInfo;
	  return;
	}
    }
  dvipsPaperSizes.push_back (dvipsPaperSizeInfo);
}

/* _________________________________________________________________________

   SessionImpl::ReadDvipsPaperSizes
   _________________________________________________________________________ */

void
SessionImpl::ReadDvipsPaperSizes ()
{
  for (unsigned r = GetNumberOfTEXMFRoots(); r > 0; -- r)
    {
      PathName configPs;
#undef BAD_PAPER_SIZE
#define BAD_PAPER_SIZE()			\
  FATAL_MIKTEX_ERROR				\
    (T_("SessionImpl::ReadDvipsPaperSizes"),	\
     T_("Bad paper size definition."),		\
     configPs.Get())
      if (FindFile(T_(MIKTEX_CONFIG_PS_FILENAME),
		   FileType::DVIPSCONFIG,
		   configPs))
	{
	  StreamReader reader (configPs);
	  tstring line;
	  bool inDefinition = false;
	  DvipsPaperSizeInfo current;
	  while (reader.ReadLine(line))
	    {
	      if (line.empty() || line[0] != T_('@'))
		{
		  if (inDefinition)
		    {
		      AddDvipsPaperSize (current);
		      current.definition.clear ();
		      inDefinition = false;
		    }
		  continue;
		}
	      MIKTEXCHAR * lpsz = STRDUP(line.c_str() + 1).GetBuffer();
	      SkipSpace (lpsz);
	      if (*lpsz == 0)
		{
		  dvipsPaperSizes.clear ();
		  inDefinition = false;
		  continue;
		}
	      if (*lpsz == T_('+'))
		{
		  if (! inDefinition)
		    {
		      BAD_PAPER_SIZE ();
		    }
		}
	      else if (! inDefinition)
		{
		  inDefinition = true;
		  if (! ChopToken(lpsz, current.dvipsName))
		    {
		      BAD_PAPER_SIZE ();
		    }
		  double texWidth;
		  if (! ChopFloat(lpsz, texWidth))
		    {
		      BAD_PAPER_SIZE ();
		    }
		  tstring unit1;
		  if (! ChopToken(lpsz, unit1))
		    {
		      BAD_PAPER_SIZE ();
		    }
		  double texHeight;
		  if (! ChopFloat(lpsz, texHeight))
		    {
		      BAD_PAPER_SIZE ();
		    }
		  tstring unit2;
		  if (! ChopToken(lpsz, unit2))
		    {
		      BAD_PAPER_SIZE ();
		    }
		  current.width
		    = CalculatePostScriptPoints(texWidth, unit1.c_str());
		  current.height
		    = CalculatePostScriptPoints(texHeight, unit2.c_str());

		  if (! IsKnownPaperSize(current.width,
					 current.height,
					 current.name))
		    {
		      current.name = current.dvipsName;
		    }

		}
	      current.definition.push_back (line);
	    }
	  if (inDefinition)
	    {
	      AddDvipsPaperSize (current);
	    }
	}
    }

  if (dvipsPaperSizes.size() == 0)
    {
      FATAL_MIKTEX_ERROR
	(T_("SessionImpl::ReadDvipsPaperSizes"),
	 T_("Dvips paper size definitions could not be read."),
	 0);
    }
}

/* _________________________________________________________________________

   SessionImpl::GetPaperSizeInfo
   _________________________________________________________________________ */

bool
SessionImpl::GetPaperSizeInfo (/*[in]*/ int			idx,
			       /*[out]*/ PaperSizeInfo &	paperSizeInfo)
{
  if (dvipsPaperSizes.size() == 0)
    {
      ReadDvipsPaperSizes ();
    }

  MIKTEX_ASSERT (dvipsPaperSizes.size() > 0);

  if (idx < 0)
    {
      // get default paper size
      idx = 0;
    }
  else if (static_cast<size_t>(idx) == dvipsPaperSizes.size())
    {
      return (false);
    }
  else if (static_cast<size_t>(idx) > dvipsPaperSizes.size())
    {
      INVALID_ARGUMENT (T_("SessionImpl::GetPaperSizeInfo"), NUMTOSTR(idx));
    }

  paperSizeInfo = dvipsPaperSizes[idx];
  
  return (true);
}

/* _________________________________________________________________________

   SessionImpl::GetPaperSizeInfo
   _________________________________________________________________________ */

PaperSizeInfo
SessionImpl::GetPaperSizeInfo (/*[in]*/ const MIKTEXCHAR * lpszDvipsName)
{
  PaperSizeInfo paperSizeInfo;
  for (int idx = 0;
       SessionImpl::theSession->GetPaperSizeInfo(idx, paperSizeInfo);
       ++ idx)
    {
      if (StringCompare(paperSizeInfo.dvipsName.c_str(), lpszDvipsName, true)
	  == 0)
	{
	  return (paperSizeInfo);
	}
    }
  FATAL_MIKTEX_ERROR (T_("SessionImpl::GetPaperSizeInfo"),
		      T_("Unknown paper size."),
		      lpszDvipsName);
}

/* _________________________________________________________________________

   PaperSizeInfo::Parse
   _________________________________________________________________________ */

PaperSizeInfo
PaperSizeInfo::Parse (/*[in]*/ const MIKTEXCHAR * lpszSpec)
{
#undef BAD_PAPER_SIZE
#define BAD_PAPER_SIZE()					\
  FATAL_MIKTEX_ERROR						\
    (T_("PaperSizeInfo::Parse"),				\
     T_("The paper size specification could not be parsed."),	\
     lpsz)

  double texWidth;

  MIKTEXCHAR * lpsz = STRDUP(lpszSpec).GetBuffer();

  if (! ChopFloat(lpsz, texWidth))
    {
      BAD_PAPER_SIZE ();
    }
  
  tstring unit1;
  
  if (! ChopToken(lpsz, unit1))
    {
      BAD_PAPER_SIZE ();
    }

  if (*lpsz != T_(','))
    {
      BAD_PAPER_SIZE ();
    }

  ++ lpsz;

  double texHeight;

  if (! ChopFloat(lpsz, texHeight))
    {
      BAD_PAPER_SIZE ();
    }
  
  tstring unit2;

  if (! ChopToken(lpsz, unit2))
    {
      BAD_PAPER_SIZE ();
    }

  int width =
    CalculatePostScriptPoints(texWidth, unit1.c_str());

  int height =
    CalculatePostScriptPoints(texHeight, unit2.c_str());

  PaperSizeInfo paperSizeInfo;

  for (int idx = 0;
       SessionImpl::theSession->GetPaperSizeInfo(idx, paperSizeInfo);
       ++ idx)
    {
      if (paperSizeInfo.width == width
	  && paperSizeInfo.height == height)
	{
	  return (paperSizeInfo);
	}
    }

  paperSizeInfo.dvipsName = T_("custom");
  paperSizeInfo.name = T_("custom");
  paperSizeInfo.width = width;
  paperSizeInfo.height = height;

  return (paperSizeInfo);
}

/* _________________________________________________________________________

   SessionImpl::SetDefaultPaperSize
   _________________________________________________________________________ */

void
SessionImpl::SetDefaultPaperSize (/*[in]*/ const MIKTEXCHAR * lpszDvipsName)
{
  UNUSED_ALWAYS (lpszDvipsName);

  if (dvipsPaperSizes.size() == 0)
    {
      ReadDvipsPaperSizes ();
    }

  MIKTEX_ASSERT (dvipsPaperSizes.size() > 0);

  vector<DvipsPaperSizeInfo>::iterator it;

  for (it = dvipsPaperSizes.begin(); it != dvipsPaperSizes.end(); ++ it)
    {
      if (StringCompare(it->dvipsName.c_str(), lpszDvipsName, true) == 0)
	{
	  break;
	}
    }

  if (it == dvipsPaperSizes.end())
    {
      FATAL_MIKTEX_ERROR (T_("SessionImpl::GetPaperSizeInfo"),
			  T_("Unknown paper size."),
			  lpszDvipsName);
    }

  if (it == dvipsPaperSizes.begin())
    {
      return;
    }

  swap (*dvipsPaperSizes.begin(), *it);

  WriteDvipsPaperSizes ();
  WriteDvipdfmPaperSize ();
  WriteDvipdfmxPaperSize ();
  WritePdfTeXPaperSize ();
}

/* _________________________________________________________________________

   SessionImpl::WriteDvipsPaperSizes
   _________________________________________________________________________ */

void
SessionImpl::WriteDvipsPaperSizes ()
{
  PathName configPs (GetSpecialPath(SpecialPath::ConfigRoot),
		     MIKTEX_PATH_CONFIG_PS);
  
  Directory::Create (PathName(configPs).RemoveFileSpec());

  StreamWriter writer (configPs);

  writer.WriteLine (T_("% DO NOT MODIFY THIS FILE!"));
  writer.WriteLine ();

  writer.WriteLine (T_("@"));
  writer.WriteLine ();

  for (vector<DvipsPaperSizeInfo>::const_iterator it = dvipsPaperSizes.begin();
       it != dvipsPaperSizes.end();
       ++ it)
    {
      for (vector<tstring>::const_iterator it2 = it->definition.begin();
	   it2 != it->definition.end();
	   ++ it2)
	{
	  writer.WriteLine (it2->c_str());
	}
      writer.WriteLine ();
    }

  writer.Close ();

  if (! Fndb::FileExists(configPs))
    {
      Fndb::Add (configPs);
    }
}

/* _________________________________________________________________________

   SessionImpl::WriteDvipdfmPaperSize
   _________________________________________________________________________ */

void
SessionImpl::WriteDvipdfmPaperSize ()
{
  PathName configFile (GetSpecialPath(SpecialPath::ConfigRoot),
		       MIKTEX_PATH_DVIPDFM_CONFIG);
  
  Directory::Create (PathName(configFile).RemoveFileSpec());

  StreamWriter writer (configFile);

  writer.WriteLine (T_("% DO NOT MODIFY THIS FILE!"));
  writer.WriteLine ();

  if (dvipsPaperSizes.size() == 0)
    {
      UNEXPECTED_CONDITION (T_("SessionImpl::WriteDvipdfmPaperSize"));
    }

  DvipsPaperSizeInfo paperSizeInfo = dvipsPaperSizes[0];

  writer.WriteFormattedLine (T_("p %s"), paperSizeInfo.name.c_str());

  writer.Close ();
			     
  if (! Fndb::FileExists(configFile))
    {
      Fndb::Add (configFile);
    }
}

/* _________________________________________________________________________

   SessionImpl::WriteDvipdfmxPaperSize
   _________________________________________________________________________ */

void
SessionImpl::WriteDvipdfmxPaperSize ()
{
  PathName configFile (GetSpecialPath(SpecialPath::ConfigRoot),
		       MIKTEX_PATH_DVIPDFMX_CONFIG);
  
  Directory::Create (PathName(configFile).RemoveFileSpec());

  StreamWriter writer (configFile);

  writer.WriteLine (T_("% DO NOT MODIFY THIS FILE!"));
  writer.WriteLine ();

  if (dvipsPaperSizes.size() == 0)
    {
      UNEXPECTED_CONDITION (T_("SessionImpl::WriteDvipdfmxPaperSize"));
    }

  DvipsPaperSizeInfo paperSizeInfo = dvipsPaperSizes[0];

  writer.WriteFormattedLine (T_("p %dbp,%dbp"),
			     paperSizeInfo.width,
			     paperSizeInfo.height);

  writer.Close ();
			     
  if (! Fndb::FileExists(configFile))
    {
      Fndb::Add (configFile);
    }
}

/* _________________________________________________________________________

   SessionImpl::WritePdfTeXPaperSize
   _________________________________________________________________________ */

void
SessionImpl::WritePdfTeXPaperSize ()
{
  PathName configFile (GetSpecialPath(SpecialPath::ConfigRoot),
		       MIKTEX_PATH_PDFTEX_CFG);
  
  Directory::Create (PathName(configFile).RemoveFileSpec());

  StreamWriter writer (configFile);

  writer.WriteLine (T_("% DO NOT MODIFY THIS FILE!"));
  writer.WriteLine ();

  if (dvipsPaperSizes.size() == 0)
    {
      UNEXPECTED_CONDITION (T_("SessionImpl::WritePdfTeXPaperSize"));
    }

  DvipsPaperSizeInfo paperSizeInfo = dvipsPaperSizes[0];

  writer.WriteFormattedLine (T_("page_width %d true bp"),
			     paperSizeInfo.width);

  writer.WriteFormattedLine (T_("page_height %d true bp"),
			     paperSizeInfo.height);

  writer.Close ();
			     
  if (! Fndb::FileExists(configFile))
    {
      Fndb::Add (configFile);
    }
}
