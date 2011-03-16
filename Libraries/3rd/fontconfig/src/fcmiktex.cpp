/* fcmiktex.cpp:						-*- C++ -*-

   Copyright (C) 2007-2011 Christian Schenk

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

#include <config.h>

#include <cstdlib>
#include <io.h>
#include <exception>
#include <set>
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
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      exit (1);
    }
  catch (const exception & e)
    {
      Utils::PrintException (e);
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
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      exit (1);
    }
  catch (const exception & e)
    {
      Utils::PrintException (e);
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
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      exit (1);
    }
  catch (const exception & e)
    {
      Utils::PrintException (e);
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
	  for (unsigned idx = 0; idx < nConfigDirs; ++ idx, ++ nPaths)
	    {
	      PathName path (SessionWrapper(true)->GetRootDirectory(idx));
	      path +=  MIKTEX_PATH_FONTCONFIG_CONFIG_DIR;
	      pPaths[nPaths] = strdup(path.Get());
	    }
	}
      return (static_cast<int>(nConfigDirs));
    }
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      exit (1);
    }
  catch (const exception & e)
    {
      Utils::PrintException (e);
      exit (1);
    }
}

extern "C"
void
miktex_close_cache_file (/*[in]*/ int		fd,
			 /*[in]*/ const char  *	lpszDir)
{
  try
    {
      time_t dirCreationTime, dirAccessTime, dirWriteTime;
      File::GetTimes (lpszDir,
		      dirCreationTime,
		      dirAccessTime,
		      dirWriteTime);
      time_t cache_mtime = time(0);
      static set<time_t> modificationTimes;
      if (dirWriteTime != static_cast<time_t>(-1)
	  && cache_mtime < dirWriteTime)
	{
	  cache_mtime = dirWriteTime;
	}
      do
	{
	  cache_mtime += 2;
	}
      while (modificationTimes.find(cache_mtime) != modificationTimes.end());
      File::SetTimes (fd, cache_mtime, cache_mtime, cache_mtime);
      modificationTimes.insert (cache_mtime);
      _close (fd);
    }
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      exit (1);
    }
  catch (const exception & e)
    {
      Utils::PrintException (e);
      exit (1);
    }
}
