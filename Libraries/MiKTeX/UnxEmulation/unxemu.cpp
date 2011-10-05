/* unxemu.cpp:

   Copyright (C) 2007-2011 Christian Schenk

   This file is part of the MiKTeX UNXEMU Library.

   The MiKTeX UNXEMU Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX UNXEMU Library is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX UNXEMU Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "internal.h"

#include <errno.h>

#include <miktex/Core/Core>

using namespace MiKTeX::Core;
using namespace std;

struct DIR_
{
  DirectoryLister * pLister;
  struct dirent direntry;
  PathName path;
  DIR_ (/*[in]*/ const char * lpszPath)
    : path (lpszPath),
      pLister (DirectoryLister::Open(lpszPath))
  {
  }
  ~DIR_ ()
  {
    try
      {
	delete pLister;
      }
    catch (const exception &)
      {
      }
  }
};

struct WDIR_
{
  DirectoryLister * pLister;
  struct wdirent direntry;
  PathName path;
  WDIR_ (/*[in]*/ const wchar_t * lpszPath)
    : path (lpszPath),
      pLister (DirectoryLister::Open(lpszPath))
  {
  }
  ~WDIR_ ()
  {
    try
      {
	delete pLister;
      }
    catch (const exception &)
      {
      }
  }
};

/* _________________________________________________________________________

   closedir
   _________________________________________________________________________ */

MIKTEXUNXCEEAPI(int)
closedir (/*[in]*/ DIR * pDir)
{
  C_FUNC_BEGIN ();
  delete pDir;
  return (0);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   wclosedir
   _________________________________________________________________________ */

MIKTEXUNXCEEAPI(int)
wclosedir (/*[in]*/ WDIR * pDir)
{
  C_FUNC_BEGIN ();
  delete pDir;
  return (0);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   opendir
   _________________________________________________________________________ */

MIKTEXUNXCEEAPI(DIR *)
opendir (/*[in]*/ const char * lpszPath)
{
  C_FUNC_BEGIN ();
  if (! Directory::Exists(lpszPath))
    {
      errno = ENOENT;
      return (0);
    }
  return (new DIR_(lpszPath));
  C_FUNC_END ();
}

/* _________________________________________________________________________

   wopendir
   _________________________________________________________________________ */

MIKTEXUNXCEEAPI(WDIR *)
wopendir (/*[in]*/ const wchar_t * lpszPath)
{
  C_FUNC_BEGIN ();
  if (! Directory::Exists(lpszPath))
    {
      errno = ENOENT;
      return (0);
    }
  return (new WDIR_(lpszPath));
  C_FUNC_END ();
}

/* _________________________________________________________________________

   readdir
   _________________________________________________________________________ */

MIKTEXUNXCEEAPI(struct dirent *)
readdir (/*[in]*/ DIR * pDir)
{
  C_FUNC_BEGIN ();
  DirectoryEntry directoryEntry;
  if (! pDir->pLister->GetNext(directoryEntry))
    {
      return (0);
    }
  Utils::CopyString (pDir->direntry.d_name,
		     (sizeof(pDir->direntry.d_name)
		      / sizeof(pDir->direntry.d_name[0])),
		     directoryEntry.name.c_str());
  return (&pDir->direntry);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   wreaddir
   _________________________________________________________________________ */

MIKTEXUNXCEEAPI(struct wdirent *)
wreaddir (/*[in]*/ WDIR * pDir)
{
  C_FUNC_BEGIN ();
  DirectoryEntry directoryEntry;
  if (! pDir->pLister->GetNext(directoryEntry))
    {
      return (0);
    }
  Utils::CopyString (pDir->direntry.d_name,
		     (sizeof(pDir->direntry.d_name)
		      / sizeof(pDir->direntry.d_name[0])),
		     directoryEntry.wname.c_str());
  return (&pDir->direntry);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   rewinddir
   _________________________________________________________________________ */

MIKTEXUNXCEEAPI(void)
rewinddir (/*[in]*/ DIR * pDir)
{
  C_FUNC_BEGIN ();
  pDir->pLister->Close ();
  delete pDir->pLister;
  pDir->pLister = DirectoryLister::Open(pDir->path);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   wrewinddir
   _________________________________________________________________________ */

MIKTEXUNXCEEAPI(void)
wrewinddir (/*[in]*/ WDIR * pDir)
{
  C_FUNC_BEGIN ();
  pDir->pLister->Close ();
  delete pDir->pLister;
  pDir->pLister = DirectoryLister::Open(pDir->path);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   miktex_strncasecmp
   _________________________________________________________________________ */

MIKTEXUNXCEEAPI(int)
miktex_strncasecmp (/*[in]*/ const char * lpsz1,
		    /*[in]*/ const char * lpsz2,
		    /*[in]*/ size_t	  n)
{
  return (MiKTeX::Core::StringCompare(lpsz1, lpsz2, n, true));
}

/* _________________________________________________________________________

   miktex_gettimeofday
   _________________________________________________________________________ */

MIKTEXUNXCEEAPI(int)
miktex_gettimeofday(/*[in]*/ struct timeval * ptv, void * pNull)
{
  MIKTEX_ASSERT (pNull == 0);
  MIKTEX_ASSERT (ptv != 0);
  SYSTEMTIME systemTime;
  GetSystemTime (&systemTime);
  struct tm tm;
  memset (&tm, 0, sizeof(tm));
  tm.tm_year = systemTime.wYear;
  tm.tm_mon = systemTime.wMonth - 1;
  tm.tm_mday = systemTime.wDay;
  tm.tm_hour = systemTime.wHour;
  tm.tm_min = systemTime.wMinute;
  tm.tm_sec = systemTime.wSecond;
  ptv->tv_sec = static_cast<long>(mktime(&tm));
  ptv->tv_usec = systemTime.wMilliseconds;
  return (0);
}
