/* fontinfo.cpp: font information

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

/* Algorithms are borrowed from the web2c mktex* shell scripts. */

#include "StdAfx.h"

#include "internal.h"

const char * WHITESPACE = " \t\r\n";

const char * const MAP_SEARCH_PATH = MAKE_SEARCH_PATH("fontname");

/* _________________________________________________________________________

   IsPrefixOf
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(int)
IsPrefixOf (/*[in]*/ const char *	lpsz1,
	    /*[in]*/ const char *	lpsz2)
{
  size_t l1 = StrLen(lpsz1);
  return (l1 <= StrLen(lpsz2) && StrNCmp(lpsz1, lpsz2, l1) == 0);
}

/* _________________________________________________________________________

   SessionImpl::FindInTypefaceMap
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(bool)
SessionImpl::FindInTypefaceMap (/*[in]*/ const char *	lpszFontName,
				/*[out]*/ char *		lpszTypeface)
{
  const size_t FONT_ABBREV_LENGTH = 2;

  if (StrLen(lpszFontName) <= FONT_ABBREV_LENGTH)
    {
      return (false);
    }

  // "ptmr8r" => "tm"
  char szFontAbbrev[FONT_ABBREV_LENGTH + 1];
  CopyString2 (szFontAbbrev,
	       FONT_ABBREV_LENGTH + 1,
	       lpszFontName + 1,
	       FONT_ABBREV_LENGTH);

  PathName pathFileName;
  if (! FindFile("typeface.map",
		 MAP_SEARCH_PATH,
		 pathFileName))
    {
      FATAL_MIKTEX_ERROR ("FindInTypefaceMap",
			  T_("typeface.map not found"),
			  0);
    }

  StreamReader reader (pathFileName);

  string line;
  while (reader.ReadLine(line))
    {
      Tokenizer tok (line.c_str(), WHITESPACE);
      if (tok.GetCurrent() == 0
	  || StringCompare(tok.GetCurrent(), szFontAbbrev) != 0)
	{
	  continue;
	}
      ++ tok;
      if (tok.GetCurrent() == 0
	  || StrLen(tok.GetCurrent()) >= BufferSizes::MaxPath)
	{
	  continue;
	}
      Utils::CopyString (lpszTypeface, BufferSizes::MaxPath, tok.GetCurrent());
      trace_fonts->WriteFormattedLine ("core", T_("found \"%s\" in typeface.map"), lpszTypeface);
      return (true);
    }

  return (false);
}

/* _________________________________________________________________________

   SessionImpl::FindInSupplierMap
   _________________________________________________________________________ */

bool
SessionImpl::FindInSupplierMap (/*[in]*/ const char *	lpszFontName,
				/*[out]*/ char *	lpszSupplier,
				/*[out]*/ char *	lpszTypeface)
{
  const size_t SUPPLIER_ABBREV_LENGTH = 1;

  if (StrLen(lpszFontName) < SUPPLIER_ABBREV_LENGTH)
    {
      return (false);
    }

  // "ptmr8r" => "p"
  char szSupplierAbbrev[SUPPLIER_ABBREV_LENGTH + 1];
  CopyString2 (szSupplierAbbrev,
	       SUPPLIER_ABBREV_LENGTH + 1,
	       lpszFontName,
	       SUPPLIER_ABBREV_LENGTH);

  PathName pathFileName;
  if (! FindFile("supplier.map",
		 MAP_SEARCH_PATH,
		 pathFileName))
    {
      FATAL_MIKTEX_ERROR ("FindInSupplierMap",
			  T_("supplier.map not found"),
			  0);
    }

  StreamReader reader (pathFileName);

  string line;
  bool bFound = false;
  while (! bFound && reader.ReadLine(line))
    {
      Tokenizer tok (line.c_str(), WHITESPACE);
      if (tok.GetCurrent() == 0 || StringCompare(tok.GetCurrent(), szSupplierAbbrev) != 0)
	{
	  continue;
	}
      ++ tok;
      if (tok.GetCurrent() == 0 || StrLen(tok.GetCurrent()) >= BufferSizes::MaxPath)
	{
	  continue;
	}
      Utils::CopyString (lpszSupplier, BufferSizes::MaxPath, tok.GetCurrent());
      trace_fonts->WriteFormattedLine ("core", T_("found \"%s\" in supplier.map"), lpszSupplier);
      bFound = true;
    }

  return (bFound && FindInTypefaceMap(lpszFontName, lpszTypeface));
}

/* _________________________________________________________________________

   SessionImpl::FindInSpecialMap
   _________________________________________________________________________ */

bool
SessionImpl::FindInSpecialMap (/*[in]*/ const char *	lpszFontName,
			       /*[out]*/ char *		lpszSupplier,
			       /*[out]*/ char *		lpszTypeface)
{
  PathName pathFileName;
  if (! FindFile("special.map",
		 MAP_SEARCH_PATH,
		 pathFileName))
    {
      FATAL_MIKTEX_ERROR ("FindInSpecialMap",
			  T_("special.map not found"),
			  0);
    }

  StreamReader reader (pathFileName);

  string line;
  bool bFound = false;
  while (! bFound && reader.ReadLine(line))
    {
      Tokenizer tok (line.c_str(), WHITESPACE);
      if (tok.GetCurrent() == 0
	  || ! (StringCompare(tok.GetCurrent(), lpszFontName) == 0
		|| (IsPrefixOf(tok.GetCurrent(), lpszFontName)
		    && (IsDigit(GetLastChar(lpszFontName)))
		    && (! IsDigit(GetLastChar(tok.GetCurrent()))))))
	{
	  continue;
	}
      ++ tok;
      if (tok.GetCurrent() == 0 || StrLen(tok.GetCurrent()) >= BufferSizes::MaxPath)
	{
	  continue;
	}
      Utils::CopyString (lpszSupplier, BufferSizes::MaxPath, tok.GetCurrent());
      ++ tok;
      if (tok.GetCurrent() == 0 || StrLen(tok.GetCurrent()) >= BufferSizes::MaxPath)
	{
	  continue;
	}
      Utils::CopyString (lpszTypeface, BufferSizes::MaxPath, tok.GetCurrent());
      trace_fonts->WriteFormattedLine ("core",
	     T_("found \"%s\"/\"%s\" in special.map"),
	     lpszSupplier,
	     lpszTypeface);
      return (true);
    }

  return (false);
}

/* _________________________________________________________________________

   SessionImpl::InternalGetFontInfo
   _________________________________________________________________________ */

bool
SessionImpl::InternalGetFontInfo (/*[in]*/ const char *	lpszFontName,
				  /*[out]*/ char *	lpszSupplier,
				  /*[out]*/ char *	lpszTypeface)
{
  return (FindInSpecialMap(lpszFontName, lpszSupplier, lpszTypeface)
	  || FindInSupplierMap(lpszFontName, lpszSupplier, lpszTypeface));
}

/* _________________________________________________________________________

   SessionImpl::SplitFontPath
   _________________________________________________________________________ */

void
SessionImpl::SplitFontPath (/*[in]*/ const char *	lpszFontPath,
			    /*[out]*/ char *	lpszFontType,
			    /*[out]*/ char *	lpszSupplier,
			    /*[out]*/ char *	lpszTypeface,
			    /*[out]*/ char *	lpszFontName,
			    /*[out]*/ char *	lpszPointSize)
{
  MIKTEX_ASSERT_STRING (lpszFontPath);
  MIKTEX_ASSERT_PATH_BUFFER_OR_NIL (lpszFontType);
  MIKTEX_ASSERT_PATH_BUFFER_OR_NIL (lpszSupplier);
  MIKTEX_ASSERT_PATH_BUFFER_OR_NIL (lpszTypeface);
  MIKTEX_ASSERT_PATH_BUFFER_OR_NIL (lpszFontName);
  MIKTEX_ASSERT_PATH_BUFFER_OR_NIL (lpszPointSize);

  if (lpszSupplier != 0 || lpszTypeface != 0)
    {
      if (lpszFontType != 0)
	{
	  *lpszFontType = 0;
	}
      if (lpszSupplier != 0)
	{
	  *lpszSupplier = 0;
	}
      if (lpszTypeface != 0)
	{
	  *lpszTypeface = 0;
	}
      if (Utils::IsAbsolutePath(lpszFontPath))
	{
	  PathName root;
	  PathName pathRel;
	  if (SplitTEXMFPath(lpszFontPath, root, pathRel)
	      != INVALID_ROOT_INDEX)
	    {
	      const char * d1, * d2;
	      PathNameParser tok (pathRel.Get());
	      if (((d1 = tok.GetCurrent()) != 0)
		  && PathName::Compare(d1, "fonts") == 0
		  && ((d2 = ++ tok) != 0))
		{
		  if (lpszFontType != 0)
		    {
		      Utils::CopyString (lpszFontType,
					 BufferSizes::MaxPath,
					 d2);
		    }
		  const char * d3, * d4;
		  if (((d3 = ++ tok) != 0) && ((d4 = ++ tok) != 0))
		    {
		      if (lpszSupplier != 0)
			{
			  Utils::CopyString (lpszSupplier,
					     BufferSizes::MaxPath,
					     d3);
			}
		      if (lpszTypeface != 0)
			{
			  Utils::CopyString (lpszTypeface,
					     BufferSizes::MaxPath,
					     d4);
			}
		    }
		}
	    }
	}
    }

  if (lpszFontName != 0 || lpszPointSize != 0)
    {
      char szFileName[BufferSizes::MaxPath];
      PathName::Split (lpszFontPath,
		       0, 0,
		       szFileName, BufferSizes::MaxPath,
		       0, 0);
      char * lpsz = szFileName + StrLen(szFileName) - 1;
      while (IsDigit(*lpsz))
	{
	  -- lpsz;
	}
      ++ lpsz;
      if (lpsz != szFileName)
	{
	  if (lpszPointSize != 0)
	    {
	      Utils::CopyString (lpszPointSize, BufferSizes::MaxPath, lpsz);
	    }
	  *lpsz = 0;
	}
      else if (lpszPointSize != 0)
	{
	  *lpszPointSize = 0;
	}
      if (lpszFontName != 0)
	{
	  Utils::CopyString (lpszFontName, BufferSizes::MaxPath, szFileName);
	}
    }
}

/* _________________________________________________________________________

   Comp2
   _________________________________________________________________________ */

inline
int
Comp2 (/*[in]*/ const char *	lpsz1,
       /*[in]*/ const char *	lpsz2)
{
  MIKTEX_ASSERT (StrLen(lpsz2) == 2);
  return (CompareFileNameChars(lpsz1[0], lpsz2[0]) == 0
	  && CompareFileNameChars(lpsz1[1], lpsz2[1]) == 0);
}

/* _________________________________________________________________________

   SessionImpl::GetFontInfo
   _________________________________________________________________________ */

bool
SessionImpl::GetFontInfo (/*[in]*/ const char *	lpszFontName,
			  /*[out]*/ char *	lpszSupplier,
			  /*[out]*/ char *	lpszTypeface,
			  /*[out]*/ double *		lpGenSize)
{
  MIKTEX_ASSERT_STRING (lpszFontName);
  MIKTEX_ASSERT_PATH_BUFFER_OR_NIL (lpszSupplier);
  MIKTEX_ASSERT_PATH_BUFFER_OR_NIL (lpszTypeface);
  MIKTEX_ASSERT_PATH_BUFFER_OR_NIL (lpGenSize);

  PathName pathFileName;

  // test TFM first
  bool bFound = FindFile(lpszFontName,
			 FileType::OFM,
			 pathFileName);

  // then MF
  if (! bFound)
    {
      bFound = FindFile(lpszFontName,
			FileType::MF,
			pathFileName);
    }

  // then possible sauterized MF
  if (! bFound)
    {
      string strFontNameSauter ("b-");
      strFontNameSauter += lpszFontName;
      bFound = FindFile(strFontNameSauter.c_str(),
			FileType::MF,
			pathFileName);
    }

  // LH fonts get special treatment
  if (! bFound
      && ((Comp2(lpszFontName, "wn") == 0)
	  || (Comp2(lpszFontName, "lh") == 0)
	  || (Comp2(lpszFontName, "ll") == 0)
	  || (Comp2(lpszFontName, "rx") == 0)
	  || (Comp2(lpszFontName, "la") == 0)
	  || (Comp2(lpszFontName, "lb") == 0)
	  || (Comp2(lpszFontName, "lc") == 0)))
    {
      string strFontNameLH;
      strFontNameLH = lpszFontName[0];
      strFontNameLH += lpszFontName[1];
      strFontNameLH += "codes";
      bFound = FindFile(strFontNameLH.c_str(),
			FileType::MF,
			pathFileName);
    }

  // parse the path, if the font was found
  if (bFound && IsTEXMFFile(pathFileName.Get(), 0, 0))
    {
      SplitFontPath (pathFileName.Get(),
		     0,
		     lpszSupplier,
		     lpszTypeface,
		     0,
		     0);
    }
  else
    {
      // consult the font maps
      if (! InternalGetFontInfo(lpszFontName, lpszSupplier, lpszTypeface))
	{
	  return (false);
	}
    }

  // determine the point size
  if (lpGenSize != 0)
    {
      char ptsize[BufferSizes::MaxPath];
      SplitFontPath (lpszFontName, 0, 0, 0, 0, ptsize);
      size_t l = StrLen(ptsize);
      if (l == 0)
	{
	  return (false);
	}
      else if (StringCompare(ptsize, "11") == 0)
	{
	  *lpGenSize = 10.95;	// \magstephalf
	}
      else if (StringCompare(ptsize, "14") == 0)
	{
	  *lpGenSize = 14.4;	// \magstep2
	}
      else if (StringCompare(ptsize, "17") == 0)
	{
	  *lpGenSize = 17.28;	// \magstep3
	}
      else if (StringCompare(ptsize, "20") == 0)
	{
	  *lpGenSize = 20.74;	// \magstep4
	}
      else if (StringCompare(ptsize, "25") == 0)
	{
	  *lpGenSize = 24.88;	// \magstep5;
	}
      else if (StringCompare(ptsize, "30") == 0)
	{
	  *lpGenSize = 29.86;	// \magstep6
	}
      else if (StringCompare(ptsize, "36") == 0)
	{
	  *lpGenSize = 35.83;	// \magstep7
	}
      else if (l == 4 || l == 5)
	{
	  /* The new convention is to have three or four letters for
	     the font name and four digits for the pointsize. The
	     number is pointsize * 100. We effectively divide by 100
	     by ignoring the last two digits. */
	  *lpGenSize = atoi(ptsize) / 100.0;
	}
      else
	{
	  *lpGenSize = atoi(ptsize);
	}
    }

  return (true);
}

/* _________________________________________________________________________

   Utils::ParseDvipsMapLine
   _________________________________________________________________________ */

bool
Utils::ParseDvipsMapLine (/*[in]*/ const string &	line,
			  /*[out]*/ FontMapEntry &	mapEntry)
{
  mapEntry.texName = "";
  mapEntry.psName = "";
  mapEntry.specialInstructions = "";
  mapEntry.encFile = "";
  mapEntry.fontFile = "";
  mapEntry.headerList = "";

  if (line.empty()
      || line[0] <= ' '
      || line[0] == '*'
      || line[0] == '#'
      || line[0] == ';'
      || line[0] == '%'
      )
    {
      return (false);
    }

  for (string::const_iterator it = line.begin();
       it != line.end();
       ++ it)
    {
      for (; it != line.end() && *it <= ' '; ++ it)
	{
	}
      if (it == line.end())
	{
	  break;
	}
      if (*it == '"')
	{
	  string temp;
	  ++ it;
	  for (; it != line.end() && *it != '"'; ++ it)
	    {
	      temp += *it;
	    }
	  for (Tokenizer tok (temp.c_str(), " \t");
	       tok.GetCurrent() != 0;
	       ++ tok)
	    {
	      if (mapEntry.specialInstructions.length() > 0)
		{
		  mapEntry.specialInstructions += ' ';
		}
	      mapEntry.specialInstructions += tok.GetCurrent();
	    }
	  if (it == line.end())
	    {
	      break;
	    }
	}
      else if (*it == '<')
	{
	  ++ it;
	  if (it == line.end())
	    {
	      break;
	    }
	  bool haveEncoding = false;
	  bool noPartial = false;
	  if (*it == '[')
	    {
	      haveEncoding = true;
	      ++ it;
	    }
	  else if (*it == '<')
	    {
	      noPartial = true;
	      ++ it;
	    }
	  if (it == line.end())
	    {
	      break;
	    }
	  for (; it != line.end() && *it <= ' '; ++ it)
	    {
	    }
	  if (it == line.end())
	    {
	      break;
	    }
	  string temp;
	  for (; it != line.end() && *it > ' '; ++ it)
	    {
	      temp += *it;
	    }
	  if (it == line.end())
	    {
	      -- it;
	    }
	  PathName fileName (temp);
	  if (mapEntry.headerList.length() > 0)
	    {
	      mapEntry.headerList += ';';
	    }
	  mapEntry.headerList += '<';
	  if (haveEncoding || fileName.HasExtension(".enc"))
	    {
	      mapEntry.encFile = fileName.Get();
	      if (haveEncoding)
		{
		  mapEntry.headerList += '[';
		}
	    }
	  else if (fileName.HasExtension(".pfa")
		   || fileName.HasExtension(".pfb"))
	    {
	      mapEntry.fontFile = fileName.Get();
	    }
	  if (noPartial)
	    {
	      mapEntry.headerList += '<';
	    }
	  mapEntry.headerList += fileName.Get();
	}
      else
	{
	  string name;
	  for (; it != line.end() && *it > ' '; ++ it)
	    {
	      name += *it;
	    }
	  if (mapEntry.texName.length() == 0)
	    {
	      mapEntry.texName = name;
	    }
	  else
	    {
	      mapEntry.psName = name;
	    }
	  if (it == line.end())
	    {
	      break;
	    }
	}
    }

  return (true);
}

/* _________________________________________________________________________

   SessionImpl::GetLocalFontDirectories
   _________________________________________________________________________ */

string
MIKTEXCALL
SessionImpl::GetLocalFontDirectories ()
{
  if (! flags.test(Flags::CachedLocalFontDirs))
    {
      flags.set (Flags::CachedLocalFontDirs);
#if defined(MIKTEX_WINDOWS)
      PathName winFontDir;
      if (GetWindowsFontsDirectory(winFontDir))
	{
	  if (! localFontDirs.empty())
	    {
	      localFontDirs += PathName::PathNameDelimiter;
	    }
	  localFontDirs += winFontDir.Get();
	}
      PathName atmFontDir;
      if (GetATMFontDir(atmFontDir))
	{
	  if (! localFontDirs.empty())
	    {
	      localFontDirs += PathName::PathNameDelimiter;
	    }
	  localFontDirs += atmFontDir.Get();
	}
      PathName acrobatFontDir;
      if (GetAcrobatFontDir(acrobatFontDir))
	{
	  if (! localFontDirs.empty())
	    {
	      localFontDirs += PathName::PathNameDelimiter;
	    }
	  localFontDirs += acrobatFontDir.Get();
	}
#endif
    }

  return (localFontDirs);
}
