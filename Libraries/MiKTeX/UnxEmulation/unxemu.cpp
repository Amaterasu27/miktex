/* unxemu.cpp:

   Copyright (C) 2007 Christian Schenk

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

#include <miktex/core.h>

using namespace MiKTeX::Core;
using namespace std;

struct DIR_
{
  DirectoryLister * pLister;
  struct dirent direntry;
  DIR_ (/*[in]*/ const char * lpszPath)
    : pLister (DirectoryLister::Open(lpszPath))
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

/* _________________________________________________________________________

   closedir
   _________________________________________________________________________ */

MIKTEXUNXAPI(int)
closedir (/*[in]*/ DIR * pDir)
{
  C_FUNC_BEGIN ();
  delete pDir;
  return (0);
  C_FUNC_END ();
}

/* _________________________________________________________________________

   opendir
   _________________________________________________________________________ */

MIKTEXUNXAPI(DIR *)
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

   readdir
   _________________________________________________________________________ */

MIKTEXUNXAPI(struct dirent *)
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
