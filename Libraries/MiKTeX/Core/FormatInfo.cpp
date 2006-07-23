/* FormatInfo.cpp:

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
SessionImpl::GetFormatInfo (/*[in]*/ const MIKTEXCHAR *	lpszFormatName)
{
  FormatInfo formatInfo;
  if (! TryGetFormatInfo(lpszFormatName, formatInfo))
    {
      INVALID_ARGUMENT (T_("SessionImpl::GetFormatInfo"), lpszFormatName);
    }
  return (formatInfo);
}

/* _________________________________________________________________________

   SessionImpl::TryGetFormatInfo
   _________________________________________________________________________ */

bool
MIKTEXCALL
SessionImpl::TryGetFormatInfo (/*[in]*/ const MIKTEXCHAR *	lpszFormatName,
			       /*[out]*/ FormatInfo &		formatInfo)
{
  ReadFormatsIni ();
  for (vector<FormatInfo_>::const_iterator it = formats.begin();
       it != formats.end();
       ++ it)
    {
      if (PathName::Compare(it->name, lpszFormatName) == 0)
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
  MIKTEXCHAR szFormatName[BufferSizes::MaxPath];
  bool custom =
    (TryDeriveTEXMFRoot(cfgFile.Get()) != GetInstallRoot());
  for (MIKTEXCHAR * lpszFormatName
	 = pFormats->FirstKey(szFormatName, BufferSizes::MaxPath);
       lpszFormatName != 0;
       lpszFormatName = pFormats->NextKey(szFormatName, BufferSizes::MaxPath))
    {
      FormatInfo_ formatInfo;
      vector<FormatInfo_>::iterator it;
      for (it = formats.begin(); it != formats.end(); ++ it)
	{
	  if (PathName::Compare(it->name, lpszFormatName) == 0)
	    {
	      formatInfo = *it;
	      break;
	    }
	}
      tstring val;
      formatInfo.cfgFile = cfgFile;
      formatInfo.name = lpszFormatName;
      if (pFormats->TryGetValue(lpszFormatName,
				T_("description"),
				val))
	{
	  formatInfo.description = val;
	}
      if (pFormats->TryGetValue(lpszFormatName,
				T_("compiler"),
				val))
	{
	  formatInfo.compiler = val;
	}
      if (pFormats->TryGetValue(lpszFormatName,
				T_("input"),
				val))
	{
	  formatInfo.inputFile = val;
	}
      if (pFormats->TryGetValue(lpszFormatName,
				T_("output"),
				val))
	{
	  formatInfo.outputFile = val;
	}
      if (pFormats->TryGetValue(lpszFormatName,
				T_("preloaded"),
				val))
	{
	  formatInfo.preloaded = val;
	}
      if (pFormats->TryGetValue(lpszFormatName,
				T_("attributes"),
				val))
	{
	  formatInfo.exclude = (val == T_("exclude"));
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
	  if (! it->description.empty())
	    {
	      pFormats->PutValue (it->name.c_str(),
				  T_("description"),
				  it->description.c_str());
	    }
	  if (it->compiler.empty())
	    {
	      UNEXPECTED_CONDITION (T_("SessionImpl::WriteFormatsIni"));
	    }
	  pFormats->PutValue (it->name.c_str(),
			      T_("compiler"),
			      it->compiler.c_str());
	  if (it->inputFile.empty())
	    {
	      UNEXPECTED_CONDITION (T_("SessionImpl::WriteFormatsIni"));
	    }
	  pFormats->PutValue (it->name.c_str(),
			      T_("input"),
			      it->inputFile.c_str());
	  if (! it->outputFile.empty())
	    {
	      pFormats->PutValue (it->name.c_str(),
			      T_("output"),
			      it->outputFile.c_str());
	    }
	  if (! it->preloaded.empty())
	    {
	      pFormats->PutValue (it->name.c_str(),
				  T_("preloaded"),
				  it->preloaded.c_str());
	    }
	}
      if (it->exclude)
	{
	  pFormats->PutValue (it->name.c_str(),
			      T_("attributes"),
			      T_("exclude"));
	}
      else
	{
	  pFormats->PutValue (it->name.c_str(),
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
SessionImpl::DeleteFormatInfo (/*[in]*/ const MIKTEXCHAR * lpszFormatName)
{
  ReadFormatsIni ();
  for (vector<FormatInfo_>::iterator it = formats.begin();
       it != formats.end();
       ++ it)
    {
      if (PathName::Compare(it->name, lpszFormatName) == 0)
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
		      lpszFormatName);
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
      if (PathName::Compare(it->name, formatInfo.name) == 0)
	{
	  if (! it->custom)
	    {
	      bool cannotChange = false;
	      if (formatInfo.custom)
		{
		  cannotChange = true;
		}
	      if (formatInfo.description != it->description)
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
