/* LanguageInfo.cpp:

   Copyright (C) 2010 Christian Schenk

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

   SessionImpl::GetLanguageInfo
   _________________________________________________________________________ */

bool
SessionImpl::GetLanguageInfo (/*[in]*/ unsigned		index,
			      /*[out]*/ LanguageInfo &	languageInfo)
{
  ReadLanguagesIni ();
  if (index > languages.size())
    {
      INVALID_ARGUMENT ("SessionImpl::GetLanguageInfo", NUMTOSTR(index));
    }
  if (index == languages.size())
    {
      return (false);
    }
  languageInfo = languages[index];
  return (true);
}

/* _________________________________________________________________________

   SessionImpl::ReadLanguagesIni
   _________________________________________________________________________ */

void
SessionImpl::ReadLanguagesIni (/*[in]*/ const PathName & cfgFile)
{
  SmartPointer<Cfg> pLanguages (Cfg::Create());
  pLanguages->Read (cfgFile);
  bool custom =
    (TryDeriveTEXMFRoot(cfgFile.Get()) != GetDistRoot());
  char szKey[BufferSizes::MaxPath];
  for (char * lpszKey
	 = pLanguages->FirstKey(szKey, BufferSizes::MaxPath);
       lpszKey != 0;
       lpszKey = pLanguages->NextKey(szKey, BufferSizes::MaxPath))
    {
      LanguageInfo_ languageInfo;
      vector<LanguageInfo_>::iterator itExisting;
      for (itExisting = languages.begin(); itExisting != languages.end(); ++ itExisting)
      {
	if (itExisting->key == lpszKey)
	{
	  languageInfo = *itExisting;
	  break;
	}
      }
      string val;
      languageInfo.cfgFile = cfgFile;
      languageInfo.key = lpszKey;
      if (pLanguages->TryGetValue(lpszKey, "synonyms", val))
	{
	  languageInfo.synonyms = val;
	}
      if (pLanguages->TryGetValue(lpszKey, "loader", val))
	{
	  languageInfo.loader = val;
	}
      if (pLanguages->TryGetValue(lpszKey, "patterns", val))
	{
	  languageInfo.patterns = val;
	}
      if (pLanguages->TryGetValue(lpszKey, "hyphenation", val))
	{
	  languageInfo.hyphenation = val;
	}
      if (pLanguages->TryGetValue(lpszKey, "luaspecial", val))
	{
	  languageInfo.luaspecial = val;
	}
      if (pLanguages->TryGetValue(lpszKey, "lefthyphenmin", val))
	{
	  languageInfo.lefthyphenmin = atoi(val.c_str());
	}
      if (pLanguages->TryGetValue(lpszKey, "righthyphenmin", val))
	{
	  languageInfo.righthyphenmin = atoi(val.c_str());
	}
      if (pLanguages->TryGetValue(lpszKey, "attributes", val))
      {
	languageInfo.exclude = (val == "exclude");
      }
      if (itExisting == languages.end())
	{
	  languageInfo.custom = custom;
	  if (StringCompare(lpszKey, "english") == 0)
	    {
	      languages.insert (languages.begin(), languageInfo);
	    }
	  else
	    {
	      languages.push_back (languageInfo);
	    }
	}
      else
	{
	  *itExisting = languageInfo;
	}
    }
  pLanguages.Release ();
}

/* _________________________________________________________________________

   SessionImpl::ReadLanguagesIni
   _________________________________________________________________________ */

void
SessionImpl::ReadLanguagesIni ()
{
  if (languages.size() > 0)
  {
    return;
  }
  bool readSomething = false;
  for (unsigned r = GetNumberOfTEXMFRoots(); r > 0; -- r)
  {
#if 0
    if (! IsManagedRoot(r - 1))
    {
      continue;
    }
#endif
    PathName languagesIni = GetRootDirectory(r - 1);
    languagesIni += MIKTEX_PATH_LANGUAGES_INI;
    if (File::Exists(languagesIni))
    {
      ReadLanguagesIni (languagesIni);
      readSomething = true;
    }
  }
  if (! readSomething)
  {
    FATAL_MIKTEX_ERROR ("SessionImpl::ReadLanguagesIni", T_("No languages.ini exists."), 0);
  }
  sort (languages.begin(), languages.end());
}

/* _________________________________________________________________________

   SessionImpl::WriteLanguagesIni
   _________________________________________________________________________ */

void
SessionImpl::WriteLanguagesIni ()
{
  SmartPointer<Cfg> pLanguages (Cfg::Create());

  for (vector<LanguageInfo_>::iterator it = languages.begin();
    it != languages.end();
    ++ it)
  {
    if (it->custom)
    {
      if (! it->synonyms.empty())
      {
	pLanguages->PutValue (it->key.c_str(), "synonyms", it->synonyms.c_str());
      }
      if (! it->loader.empty())
      {
	pLanguages->PutValue (it->key.c_str(), "loader", it->loader.c_str());
      }
      if (! it->patterns.empty())
      {
	pLanguages->PutValue (it->key.c_str(), "patterns", it->patterns.c_str());
      }
      if (! it->luaspecial.empty())
      {
	pLanguages->PutValue (it->key.c_str(), "luaspecial", it->luaspecial.c_str());
      }
      if (it->lefthyphenmin != -1)
      {
	pLanguages->PutValue (it->key.c_str(), "lefthyphenmin", NUMTOSTR(it->lefthyphenmin));
      }
      if (it->righthyphenmin != -1)
      {
	pLanguages->PutValue (it->key.c_str(), "righthyphenmin", NUMTOSTR(it->righthyphenmin));
      }
    }
    if (it->exclude)
    {
      pLanguages->PutValue (it->key.c_str(), "attributes", "exclude");
    }
    else
    {
      pLanguages->PutValue (it->key.c_str(), "attributes", "");
    }
  }

  PathName pathLocalLanguagesIni (GetSpecialPath(SpecialPath::ConfigRoot),
				  MIKTEX_PATH_LANGUAGES_INI);

  Directory::Create (PathName(pathLocalLanguagesIni).RemoveFileSpec());

  pLanguages->Write (pathLocalLanguagesIni);

  pLanguages.Release ();

  if (! Fndb::FileExists(pathLocalLanguagesIni))
  {
    Fndb::Add (pathLocalLanguagesIni);
  }
}

/* _________________________________________________________________________

   SessionImpl::SetLanguageInfo
   _________________________________________________________________________ */

void
SessionImpl::SetLanguageInfo (/*[in]*/ const LanguageInfo &	languageInfo)
{
  ReadLanguagesIni ();
  vector<LanguageInfo_>::iterator it;
  for (it = languages.begin(); it != languages.end(); ++ it)
  {
    if (it->key == languageInfo.key)
    {
      bool custom = it->custom;
      if (! custom)
      {
	bool cannotChange = false;
	if (languageInfo.custom)
	{
	  cannotChange = true;
	}
	if (languageInfo.synonyms != it->synonyms)
	{
	  cannotChange = true;
	}
	if (languageInfo.loader != it->loader)
	{
	  cannotChange = true;
	}
	if (languageInfo.patterns != it->patterns)
	{
	  cannotChange = true;
	}
	if (languageInfo.hyphenation != it->hyphenation)
	{
	  cannotChange = true;
	}
	if (languageInfo.luaspecial != it->luaspecial)
	{
	  cannotChange = true;
	}
	if (languageInfo.lefthyphenmin != it->lefthyphenmin)
	{
	  cannotChange = true;
	}
	if (languageInfo.righthyphenmin != it->righthyphenmin)
	{
	  cannotChange = true;
	}
	if (cannotChange)
	{
	  FATAL_MIKTEX_ERROR (
	    "",
	    T_("Built-in language definitions may not be changed."),
	    languageInfo.key.c_str());
	}
      }
      if (languageInfo.exclude && languageInfo.key == "english")
      {
	FATAL_MIKTEX_ERROR (
	  "",
	  T_("The English language may not be excluded."),
	  0);

      }
      *it = languageInfo;
      it->custom = custom;
      break;
    }
  }
  if (it == languages.end())
  {
    if (! languageInfo.custom)
    {
      INVALID_ARGUMENT ("SessionImpl::SetLanguageInfo", 0);
    }
    languages.push_back (languageInfo);
  }
  WriteLanguagesIni ();
}
