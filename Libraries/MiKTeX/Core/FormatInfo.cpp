/* FormatInfo.cpp:

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

   SessionImpl::GetFormatInfo
   _________________________________________________________________________ */

bool
MIKTEXCALL
SessionImpl::GetFormatInfo (/*[in]*/ unsigned		index,
			    /*[out]*/ FormatInfo &	formatInfo)
{
  ReadFormatsIni ();
  if (index > formats.size())
    {
      INVALID_ARGUMENT (T_("SessionImpl::GetFormatInfo"), NUMTOSTR(index));
    }
  if (index == formats.size())
    {
      return (false);
    }
  formatInfo = formats[index];
  return (true);
}

/* _________________________________________________________________________

   SessionImpl::GetFormatInfo
   _________________________________________________________________________ */

FormatInfo
MIKTEXCALL
SessionImpl::GetFormatInfo (/*[in]*/ const MIKTEXCHAR *	lpszKey)
{
  FormatInfo formatInfo;
  if (! TryGetFormatInfo(lpszKey, formatInfo))
    {
      INVALID_ARGUMENT (T_("SessionImpl::GetFormatInfo"), lpszKey);
    }
  return (formatInfo);
}

/* _________________________________________________________________________

   SessionImpl::TryGetFormatInfo
   _________________________________________________________________________ */

bool
MIKTEXCALL
SessionImpl::TryGetFormatInfo (/*[in]*/ const MIKTEXCHAR *	lpszKey,
			       /*[out]*/ FormatInfo &		formatInfo)
{
  ReadFormatsIni ();
  for (vector<FormatInfo_>::const_iterator it = formats.begin();
       it != formats.end();
       ++ it)
    {
      if (PathName::Compare(it->key, lpszKey) == 0)
	{
	  formatInfo = *it;
	  return (true);
	}
    }
  return (false);
}

/* _________________________________________________________________________

   SessionImpl::ReadFormatsIni
   _________________________________________________________________________ */

void
SessionImpl::ReadFormatsIni (/*[in]*/ const PathName & cfgFile)
{
  SmartPointer<Cfg> pFormats (Cfg::Create());
  pFormats->Read (cfgFile);
  MIKTEXCHAR szKey[BufferSizes::MaxPath];
  bool custom =
    (TryDeriveTEXMFRoot(cfgFile.Get()) != GetInstallRoot());
  for (MIKTEXCHAR * lpszKey
	 = pFormats->FirstKey(szKey, BufferSizes::MaxPath);
       lpszKey != 0;
       lpszKey = pFormats->NextKey(szKey, BufferSizes::MaxPath))
    {
      FormatInfo_ formatInfo;
      vector<FormatInfo_>::iterator it;
      for (it = formats.begin(); it != formats.end(); ++ it)
	{
	  if (PathName::Compare(it->key, lpszKey) == 0)
	    {
	      formatInfo = *it;
	      break;
	    }
	}
      tstring val;
      formatInfo.cfgFile = cfgFile;
      formatInfo.key = lpszKey;
      if (pFormats->TryGetValue(lpszKey,
				T_("name"),
				val))
	{
	  formatInfo.name = val;
	}
      else
	{
	  formatInfo.name = lpszKey;
	}
      if (pFormats->TryGetValue(lpszKey,
				T_("description"),
				val))
	{
	  formatInfo.description = val;
	}
      if (pFormats->TryGetValue(lpszKey,
				T_("compiler"),
				val))
	{
	  formatInfo.compiler = val;
	}
      if (pFormats->TryGetValue(lpszKey,
				T_("input"),
				val))
	{
	  formatInfo.inputFile = val;
	}
      if (pFormats->TryGetValue(lpszKey,
				T_("output"),
				val))
	{
	  formatInfo.outputFile = val;
	}
      if (pFormats->TryGetValue(lpszKey,
				T_("preloaded"),
				val))
	{
	  formatInfo.preloaded = val;
	}
      if (pFormats->TryGetValue(lpszKey,
				T_("attributes"),
				val))
	{
	  formatInfo.exclude = (val == T_("exclude"));
	}
      if (pFormats->TryGetValue(lpszKey,
				T_("arguments"),
				val))
	{
	  formatInfo.arguments = val;
	}
      if (it == formats.end())
	{
	  formatInfo.custom = custom;
	  formats.push_back (formatInfo);
	}
      else
	{
	  *it = formatInfo;
	}
    }
  pFormats.Release ();
}

/* _________________________________________________________________________

   SessionImpl::ReadFormatsIni
   _________________________________________________________________________ */

void
SessionImpl::ReadFormatsIni ()
{
  if (formats.size() > 0)
    {
      return;
    }
  bool readSomething = false;
  for (unsigned r = GetNumberOfTEXMFRoots(); r > 0; -- r)
    {
      if (! IsManagedRoot(r - 1))
	{
	  continue;
	}
      PathName formatsIni = GetRootDirectory(r - 1);
      formatsIni += MIKTEX_PATH_FORMATS_INI;
      if (File::Exists(formatsIni))
	{
	  ReadFormatsIni (formatsIni);
	  readSomething = true;
	}
    }
  if (! readSomething)
    {
      FATAL_MIKTEX_ERROR (T_("SessionImpl::ReadFormatsIni"),
			  T_("No formats.ini exists."),
			  0);
    }
}

/* _________________________________________________________________________

   SessionImpl::WriteFormatsIni
   _________________________________________________________________________ */

void
SessionImpl::WriteFormatsIni ()
{
  SmartPointer<Cfg> pFormats (Cfg::Create());

  for (vector<FormatInfo_>::iterator it = formats.begin();
       it != formats.end();
       ++ it)
    {
      if (it->custom)
	{
	  if (PathName::Compare(it->key, it->name) != 0)
	    {
	      pFormats->PutValue (it->key.c_str(),
				  T_("name"),
				  it->name.c_str());
	    }
	  if (! it->description.empty())
	    {
	      pFormats->PutValue (it->key.c_str(),
				  T_("description"),
				  it->description.c_str());
	    }
	  if (it->compiler.empty())
	    {
	      UNEXPECTED_CONDITION (T_("SessionImpl::WriteFormatsIni"));
	    }
	  pFormats->PutValue (it->key.c_str(),
			      T_("compiler"),
			      it->compiler.c_str());
	  if (it->inputFile.empty())
	    {
	      UNEXPECTED_CONDITION (T_("SessionImpl::WriteFormatsIni"));
	    }
	  pFormats->PutValue (it->key.c_str(),
			      T_("input"),
			      it->inputFile.c_str());
	  if (! it->outputFile.empty())
	    {
	      pFormats->PutValue (it->key.c_str(),
			      T_("output"),
			      it->outputFile.c_str());
	    }
	  if (! it->preloaded.empty())
	    {
	      pFormats->PutValue (it->key.c_str(),
				  T_("preloaded"),
				  it->preloaded.c_str());
	    }
	  if (! it->arguments.empty())
	    {
	      pFormats->PutValue (it->key.c_str(),
				  T_("arguments"),
				  it->arguments.c_str());
	    }
	}
      if (it->exclude)
	{
	  pFormats->PutValue (it->key.c_str(),
			      T_("attributes"),
			      T_("exclude"));
	}
      else
	{
	  pFormats->PutValue (it->key.c_str(),
			      T_("attributes"),
			      T_(""));
	}
    }

  PathName pathLocalFormatsIni (GetSpecialPath(SpecialPath::ConfigRoot),
				MIKTEX_PATH_FORMATS_INI);

  Directory::Create (PathName(pathLocalFormatsIni).RemoveFileSpec());

  pFormats->Write (pathLocalFormatsIni);

  pFormats.Release ();

  if (! Fndb::FileExists(pathLocalFormatsIni))
    {
      Fndb::Add (pathLocalFormatsIni);
    }
}

/* _________________________________________________________________________

   SessionImpl::DeleteFormatInfo
   _________________________________________________________________________ */

void
MIKTEXCALL
SessionImpl::DeleteFormatInfo (/*[in]*/ const MIKTEXCHAR * lpszKey)
{
  ReadFormatsIni ();
  for (vector<FormatInfo_>::iterator it = formats.begin();
       it != formats.end();
       ++ it)
    {
      if (PathName::Compare(it->key, lpszKey) == 0)
	{
	  if (! it->custom)
	    {
	      FATAL_MIKTEX_ERROR (T_("SessionImpl::DeleteFormatInfo"),
				  T_("\
Built-in format definitions may not be deleted."),
				  0);
	    }
	  formats.erase (it);
	  WriteFormatsIni ();
	  return;
	}
    }
  FATAL_MIKTEX_ERROR (T_("SessionImpl::DeleteFormatInfo"),
		      T_("The format could not be found."),
		      lpszKey);
}

/* _________________________________________________________________________

   SessionImpl::SetFormatInfo
   _________________________________________________________________________ */

void
MIKTEXCALL
SessionImpl::SetFormatInfo (/*[in]*/ const FormatInfo &	formatInfo)
{
  ReadFormatsIni ();
  vector<FormatInfo_>::iterator it;
  for (it = formats.begin(); it != formats.end(); ++ it)
    {
      if (PathName::Compare(it->key, formatInfo.key) == 0)
	{
	  if (! it->custom)
	    {
	      bool cannotChange = false;
	      if (formatInfo.custom)
		{
		  cannotChange = true;
		}
	      if (formatInfo.name != it->name)
		{
		  cannotChange = true;
		}
	      if (formatInfo.compiler != it->compiler)
		{
		  cannotChange = true;
		}
	      if (formatInfo.inputFile != it->inputFile)
		{
		  cannotChange = true;
		}
	      if (formatInfo.outputFile != it->outputFile)
		{
		  cannotChange = true;
		}
	      if (formatInfo.preloaded != it->preloaded)
		{
		  cannotChange = true;
		}
	      if (formatInfo.arguments != it->arguments)
		{
		  cannotChange = true;
		}
	      if (cannotChange)
		{
		  FATAL_MIKTEX_ERROR
		    (T_(""),
		     T_("Built-in format definitions may not be changed."),
		     formatInfo.name.c_str());
		}
	    }
	  *it = formatInfo;
	  break;
	}
    }
  if (it == formats.end())
    {
      if (! formatInfo.custom)
	{
	  INVALID_ARGUMENT (T_("SessionImpl::SetFormatInfo"), 0);
	}
      formats.push_back (formatInfo);
    }
  WriteFormatsIni ();
}
