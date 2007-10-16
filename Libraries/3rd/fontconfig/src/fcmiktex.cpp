/* miktex.cpp:							-*- C++ -*-

   Copyright (C) 2007 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.
   
   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  */

#include <cstdlib>
#include <exception>
#include <MiKTeX/Core/Core>
#include <miktex/paths.h>

using namespace MiKTeX::Core;
using namespace std;

extern "C"
const char *
miktex_fontconfig_path ()
{
  try
    {
      static PathName path;
      if (path[0] == 0)
	{
	  path = SessionWrapper(true)->GetSpecialPath(SpecialPath::ConfigRoot);
	  path += MIKTEX_PATH_FONTCONFIG_CONFIG_DIR;
	}
      return (path.Get());
    }
  catch (const exception &)
    {
      exit (1);
    }
}

extern "C"
const char *
miktex_fc_cachedir()
{
  try
    {
      static PathName path;
      if (path[0] == 0)
	{
	  path = SessionWrapper(true)->GetSpecialPath(SpecialPath::DataRoot);
	  path += MIKTEX_PATH_FONTCONFIG_CACHE_DIR;
	}
      return (path.Get());
    }
  catch (const exception &)
    {
      exit (1);
    }
}

extern "C"
const char *
miktex_fc_default_fonts ()
{
  try
    {
      static PathName path;
      if (path[0] == 0)
	{
	  UINT l = 
	    GetWindowsDirectory(path.GetBuffer(),
				static_cast<UINT>(path.GetCapacity()));
	  if (l == 0 || l >= path.GetCapacity())
	    {
	      path = "C:/wInDoWs";
	    }
	  path += "Fonts";
	}
      return (path.Get());
    }
  catch (const exception &)
    {
      exit (1);
    }
}

extern "C"
const char *
miktex_fontconfig_file ()
{
  try
    {
      static PathName path;
      if (path[0] == 0)
	{
	  path = SessionWrapper(true)->GetSpecialPath(SpecialPath::ConfigRoot);
	  path += MIKTEX_PATH_FONTCONFIG_CONFIG_FILE;
	}
      return (path.Get());
    }
  catch (const exception &)
    {
      exit (1);
    }
}

