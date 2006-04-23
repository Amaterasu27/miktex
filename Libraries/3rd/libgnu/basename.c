/* basename.c:						-*- C++ -*-

   Copyright (C) 1996,97,98,2002 Free Software Foundation, Inc.
   Copyright (C) 2000-2004 Christian Schenk

   This file is part of the MiKTeX GNU Library (gnu.dll).

   The MiKTeX GNU Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2, or (at your option) any later version.

   The MiKTeX GNU Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with The MiKTeX GNU Library; if not, write to the
   Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139,
   USA.  */

#include "internal.h"

GNUDLLAPI(char *)
basename (/*[in]*/ const char * lpszFileName)
{
  const char * lpsz = lpszFileName + strlen(lpszFileName);
  while (lpsz != lpszFileName)
    {
      lpsz --;
      if (*lpsz == '/' || *lpsz == '\\' || *lpsz == ':')
	return ((char *) lpsz + 1);
    }
  return ((char *) lpszFileName);
}
