/* fcmiktex.cpp:						-*- C++ -*-

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
#include <io.h>
#include <exception>
#include <miktex/Core/Core>
#include <miktex/Core/Paths>

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
  return (MIKTEX_FONTS_CONF);
}

extern "C"
int
miktex_get_fontconfig_config_dirs (/*[in,out]*/ char **	pPaths,
				   /*[in]*/ int		nPaths)
{
  try
    {
      unsigned nConfigDirs = SessionWrapper(true)->GetNumberOfTEXMFRoots();
      if (pPaths != 0)
	{
	  MIKTEX_ASSERT_BUFFER (pPaths,
				(nPaths + nConfigDirs) * sizeof(pPaths[0]));
	  for (unsigned idx = nConfigDirs; idx > 0; -- idx, ++ nPaths)
	    {
	      PathName path (SessionWrapper(true)->GetRootDirectory(idx - 1));
	      path +=  MIKTEX_PATH_FONTCONFIG_CONFIG_DIR;
	      pPaths[nPaths] = strdup(path.Get());
	    }
	}
      return (static_cast<int>(nConfigDirs));
    }
  catch (const exception &)
    {
      exit (1);
    }
}

extern "C"
void
miktex_close_cache_file (/*[in]*/ int fd)
{
  static time_t cache_ctime = static_cast<time_t>(-1);
  static time_t cache_atime = static_cast<time_t>(-1);
  static time_t cache_mtime = static_cast<time_t>(-1);
  try
    {
      if (cache_ctime == static_cast<time_t>(-1))
	{
	  cache_ctime = cache_atime = cache_mtime = time(0);
	}
      File::SetTimes (fd, cache_ctime, cache_atime, cache_mtime);
      cache_ctime += 2;
      cache_atime += 2;
      cache_mtime += 2;
      _close (fd);
    }
  catch (const exception &)
    {
      exit (1);
    }
}
