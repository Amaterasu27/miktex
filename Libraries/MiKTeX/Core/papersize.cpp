/* papersize.cpp: paper size info

   Copyright (C) 1996-2007 Christian Schenk

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
		  /*[in]*/ string &	name)
{
#define mm2bp(mm) static_cast<int>(((mm) * (1.0 / 25.4) * 72.0) + 0.5)
#define inch2bp(inch) static_cast<int>(((inch) * 72.0) + 0.5)

  static const struct
  {
    const char *	lpszName;
    int			width;
    int			height;
  } knownPaperSizes[] = {
    "A3", mm2bp(297), mm2bp(420),
    "A4", mm2bp(210), mm2bp(297),
    "A5", mm2bp(148), mm2bp(210),
    "A6", mm2bp(257), mm2bp(364),
    "Ledger", inch2bp(17), inch2bp(11),
    "Legal", inch2bp(8.5), inch2bp(14),
    "Letter", inch2bp(8.5), inch2bp(11),
    "Tabloid", inch2bp(11), inch2bp(17),
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
    { "pt", (72.27)				/ 72.0 },
    { "pc", (72.27 / 12)			/ 72.0 },
    { "in", (1.0)				/ 72.0 },
    { "bp", (72.0)				/ 72.0 },
    { "cm", (2.54)				/ 72.0 },
    { "mm", (25.4)				/ 72.0 },
    { "dd", (72.27 / (1238.0 / 1157))	/ 72.0 },
    { "cc", (72.27 / 12 / (1238.0 / 1157))	/ 72.0 },
    { "sp", (72.27 * 65536)			/ 72.0 },
  };
  for (size_t idx = 0; idx < sizeof(unittable) / sizeof(unittable[0]); ++ idx)
    {
      if (StringCompare(unittable[idx].lpszUnit, lpszUnit) == 0)
	{
#define round(x) static_cast<int>(x + 0.5)
	  return (round(value / unittable[idx].factor));
	}
    }
  FATAL_MIKTEX_ERROR ("CalculatePostScriptPoints",
		      T_("Invalid unit."),
		      lpszUnit);
}

/* _________________________________________________________________________

   ChopFloat
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
ChopFloat (/*[in,out]*/ char * &	lpsz,
	   /*[out]*/ double &		ret)
{
  SkipSpace (lpsz);
  bool gotDot = false;
  char * lpszStart = lpsz;
  string str;
  while (*lpsz != 0 && (IsDigit(*lpsz) || (*lpsz == '.' && ! gotDot)))
    {
      str += *lpsz;
      if (*lpsz == '.')
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
  ret = atof(str.c_str());
  return (true);
}

/* _________________________________________________________________________

   ChopToken
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
ChopToken (/*[in,out]*/ char * &	lpsz,
	   /*[out]*/ string &		ret)
{
  SkipSpace (lpsz);
  char * lpszStart = lpsz;
  ret = "";
  while (*lpsz != 0 && ! IsSpace(*lpsz) && *lpsz != ',')
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
      if (! IsManagedRoot(r - 1))
	{
	  continue;
	}
      PathName configPs (GetRootDirectory(r - 1), MIKTEX_PATH_CONFIG_PS);
#undef BAD_PAPER_SIZE
#define BAD_PAPER_SIZE()			\
  FATAL_MIKTEX_ERROR				\
    ("SessionImpl::ReadDvipsPaperSizes",	\
     T_("Bad paper size definition."),		\
     configPs.Get())
      if (File::Exists(configPs))
	{
	  StreamReader reader (configPs);
	  string line;
	  bool inDefinition = false;
	  DvipsPaperSizeInfo current;
	  while (reader.ReadLine(line))
	    {
	      if (line.empty() || line[0] != '@')
		{
		  if (inDefinition)
		    {
		      AddDvipsPaperSize (current);
		      current.definition.clear ();
		      inDefinition = false;
		    }
		  continue;
		}
	      CharBuffer<char> buf (line.c_str() + 1);
	      char * lpsz = buf.GetBuffer();
	      SkipSpace (lpsz);
	      if (*lpsz == 0)
		{
		  dvipsPaperSizes.clear ();
		  inDefinition = false;
		  continue;
		}
	      if (*lpsz == '+')
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
		  string unit1;
		  if (! ChopToken(lpsz, unit1))
		    {
		      BAD_PAPER_SIZE ();
		    }
		  double texHeight;
		  if (! ChopFloat(lpsz, texHeight))
		    {
		      BAD_PAPER_SIZE ();
		    }
		  string unit2;
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
	("SessionImpl::ReadDvipsPaperSizes",
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
      INVALID_ARGUMENT ("SessionImpl::GetPaperSizeInfo", NUMTOSTR(idx));
    }

  paperSizeInfo = dvipsPaperSizes[idx];
  
  return (true);
}

/* _________________________________________________________________________

   SessionImpl::GetPaperSizeInfo
   _________________________________________________________________________ */

PaperSizeInfo
SessionImpl::GetPaperSizeInfo (/*[in]*/ const char * lpszDvipsName)
{
  PaperSizeInfo paperSizeInfo;
  for (int idx = 0;
       GetPaperSizeInfo(idx, paperSizeInfo);
       ++ idx)
    {
      if (StringCompare(paperSizeInfo.dvipsName.c_str(), lpszDvipsName, true)
	  == 0)
	{
	  return (paperSizeInfo);
	}
    }
  FATAL_MIKTEX_ERROR ("SessionImpl::GetPaperSizeInfo",
		      T_("Unknown paper size."),
		      lpszDvipsName);
}

/* _________________________________________________________________________

   PaperSizeInfo::Parse
   _________________________________________________________________________ */

PaperSizeInfo
PaperSizeInfo::Parse (/*[in]*/ const char * lpszSpec)
{
#undef BAD_PAPER_SIZE
#define BAD_PAPER_SIZE()					\
  FATAL_MIKTEX_ERROR						\
    ("PaperSizeInfo::Parse",					\
     T_("The paper size specification could not be parsed."),	\
     lpsz)

  double texWidth;

  CharBuffer<char> buf (lpszSpec);

  char * lpsz = buf.GetBuffer();

  if (! ChopFloat(lpsz, texWidth))
    {
      BAD_PAPER_SIZE ();
    }
  
  string unit1;
  
  if (! ChopToken(lpsz, unit1))
    {
      BAD_PAPER_SIZE ();
    }

  if (*lpsz != ',')
    {
      BAD_PAPER_SIZE ();
    }

  ++ lpsz;

  double texHeight;

  if (! ChopFloat(lpsz, texHeight))
    {
      BAD_PAPER_SIZE ();
    }
  
  string unit2;

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
       SessionImpl::GetSession()->GetPaperSizeInfo(idx, paperSizeInfo);
       ++ idx)
    {
      if (paperSizeInfo.width == width
	  && paperSizeInfo.height == height)
	{
	  return (paperSizeInfo);
	}
    }

  paperSizeInfo.dvipsName = "custom";
  paperSizeInfo.name = "custom";
  paperSizeInfo.width = width;
  paperSizeInfo.height = height;

  return (paperSizeInfo);
}

/* _________________________________________________________________________

   SessionImpl::SetDefaultPaperSize
   _________________________________________________________________________ */

void
SessionImpl::SetDefaultPaperSize (/*[in]*/ const char * lpszDvipsName)
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
      FATAL_MIKTEX_ERROR ("SessionImpl::GetPaperSizeInfo",
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

   StreamEditor
   _________________________________________________________________________ */

class
StreamEditor
{
public:
  StreamEditor (/*[in]*/ const PathName & path)
    : path (path)
  {
    bak = path;
    bak.Append (".bak", false);
    File::Move (path, bak);
    reader.Attach (File::Open(bak, FileMode::Open, FileAccess::Read));
    writer.Attach (File::Open(path, FileMode::Create, FileAccess::Write));
  }

public:
  bool
  ReadLine (/*[out]*/ string & line)
  {
    return (reader.ReadLine(line));
  }

public:
  void
  WriteLine (/*[out]*/ const string & line)
  {
    writer.WriteLine (line);
  }

public:
  void
  WriteLine ()
  {
    writer.WriteLine ();
  }

public:
  void
  WriteFormattedLine (/*[in]*/ const char *	lpszFormat,
		      /*[in]*/			...)
  {
    va_list marker;
    va_start (marker, lpszFormat);
    WriteLine (Utils::FormatString(lpszFormat, marker));
    va_end (marker);
  }

public:
  ~StreamEditor ()
  {
    try
      {
	reader.Close ();
	writer.Close ();
	File::Delete (bak);
	if (! Fndb::FileExists(path))
	  {
	    Fndb::Add (path);
	  }
      }
    catch (const exception &)
      {
      }
  }

private:
  PathName path;

private:
  PathName bak;

private:
  StreamReader reader;

private:
  StreamWriter writer;
};

/* _________________________________________________________________________

   SessionImpl::TryCreateFromTemplate
   _________________________________________________________________________ */

bool
SessionImpl::TryCreateFromTemplate (/*[in]*/ const PathName & path)
{
  unsigned r = TryDeriveTEXMFRoot(path);
  if (r == INVALID_ROOT_INDEX)
    {
      return (false);
    }
  const char * lpszRelPath =
    Utils::GetRelativizedPath(path.Get(), GetRootDirectory(r).Get());
  if (lpszRelPath == 0)
    {
      UNEXPECTED_CONDITION ("SessionImpl::TryCreateFromTemplate");
    }
  PathName configTemplate (GetSpecialPath(SpecialPath::InstallRoot));
  configTemplate += lpszRelPath;
  configTemplate.Append (".template", false);
  if (File::Exists(configTemplate))
    {
      Directory::Create (PathName(path).RemoveFileSpec());
      File::Copy (configTemplate, path);
      FileAttributes attr = File::GetAttributes(path);
      attr &= ~ FileAttributes(FileAttributes::ReadOnly);
      File::SetAttributes (path, attr);
      if (! Fndb::FileExists(path))
	{
	  Fndb::Add (path);
	}
      return (true);
    }
  else
    {
      return (false);
    }
}

/* _________________________________________________________________________

   SessionImpl::WriteDvipsPaperSizes
   _________________________________________________________________________ */

void
SessionImpl::WriteDvipsPaperSizes ()
{
  PathName configFile (GetSpecialPath(SpecialPath::ConfigRoot),
		       MIKTEX_PATH_CONFIG_PS);
  
  if (! File::Exists(configFile))
    {
      if (! TryCreateFromTemplate(configFile))
	{
	  Directory::Create (PathName(configFile).RemoveFileSpec());
	  StreamWriter writer (configFile);
	  writer.Close ();
	}
    }

  StreamEditor editor (configFile);

  string line;
  string lastLine;

  for (; editor.ReadLine(line); lastLine = line)
    {
      if (! line.empty() && line[0] == '@')
	{
	  line = "";
	}
      if (! (line.empty() && lastLine.empty()))
	{
	  editor.WriteLine (line);
	}
    }

  editor.WriteLine ("@");
  editor.WriteLine ();

  for (vector<DvipsPaperSizeInfo>::const_iterator it = dvipsPaperSizes.begin();
       it != dvipsPaperSizes.end();
       ++ it)
    {
      for (vector<string>::const_iterator it2 = it->definition.begin();
	   it2 != it->definition.end();
	   ++ it2)
	{
	  editor.WriteLine (it2->c_str());
	}
      editor.WriteLine ();
    }
}

/* _________________________________________________________________________

   SessionImpl::WriteDvipdfmPaperSize
   _________________________________________________________________________ */

void
SessionImpl::WriteDvipdfmPaperSize ()
{
  if (dvipsPaperSizes.size() == 0)
    {
      UNEXPECTED_CONDITION ("SessionImpl::WritePdfTeXPaperSize");
    }

  DvipsPaperSizeInfo paperSizeInfo = dvipsPaperSizes[0];

  PathName configFile (GetSpecialPath(SpecialPath::ConfigRoot),
		       MIKTEX_PATH_DVIPDFM_CONFIG);

  if (! File::Exists(configFile))
    {
      if (! TryCreateFromTemplate(configFile))
	{
	  Directory::Create (PathName(configFile).RemoveFileSpec());
	  StreamWriter writer (configFile);
	  writer.Close ();
	}
    }

  StreamEditor editor (configFile);
  string line;
  while (editor.ReadLine(line))
    {
      if (! (line.compare(0, 2, "p ") == 0))
	{
	  editor.WriteLine (line);
	}
    }

  editor.WriteFormattedLine ("p %s",
			     MakeLower(paperSizeInfo.name).c_str());
}

/* _________________________________________________________________________

   SessionImpl::WriteDvipdfmxPaperSize
   _________________________________________________________________________ */

void
SessionImpl::WriteDvipdfmxPaperSize ()
{
  if (dvipsPaperSizes.size() == 0)
    {
      UNEXPECTED_CONDITION ("SessionImpl::WritePdfTeXPaperSize");
    }

  DvipsPaperSizeInfo paperSizeInfo = dvipsPaperSizes[0];

  PathName configFile (GetSpecialPath(SpecialPath::ConfigRoot),
		       MIKTEX_PATH_DVIPDFMX_CONFIG);

  if (! File::Exists(configFile))
    {
      if (! TryCreateFromTemplate(configFile))
	{
	  Directory::Create (PathName(configFile).RemoveFileSpec());
	  StreamWriter writer (configFile);
	  writer.Close ();
	}
    }

  StreamEditor editor (configFile);
  string line;
  while (editor.ReadLine(line))
    {
      if (! (line.compare(0, 2, "p ") == 0))
	{
	  editor.WriteLine (line);
	}
    }

  editor.WriteFormattedLine ("p %dbp,%dbp",
			     paperSizeInfo.width,
			     paperSizeInfo.height);
}

/* _________________________________________________________________________

   SessionImpl::WritePdfTeXPaperSize
   _________________________________________________________________________ */

void
SessionImpl::WritePdfTeXPaperSize ()
{
  if (dvipsPaperSizes.size() == 0)
    {
      UNEXPECTED_CONDITION ("SessionImpl::WritePdfTeXPaperSize");
    }

  DvipsPaperSizeInfo paperSizeInfo = dvipsPaperSizes[0];

  PathName configFile (GetSpecialPath(SpecialPath::ConfigRoot),
		       MIKTEX_PATH_PDFTEX_CFG);

  if (! File::Exists(configFile))
    {
      if (! TryCreateFromTemplate(configFile))
	{
	  Directory::Create (PathName(configFile).RemoveFileSpec());
	  StreamWriter writer (configFile);
	  writer.Close ();
	}
    }

  StreamEditor editor (configFile);
  string line;
  while (editor.ReadLine(line))
    {
      if (! (line.compare(0, 11, "page_width ") == 0
	     || line.compare(0, 12, "page_height ") == 0))
	{
	  editor.WriteLine (line);
	}
    }

  editor.WriteFormattedLine ("page_width %d true bp",
			     paperSizeInfo.width);
  
  editor.WriteFormattedLine ("page_height %d true bp",
			     paperSizeInfo.height);
}
