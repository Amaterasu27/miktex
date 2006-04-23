/* basename.c:						-*- C++ -*-

   Copyright (C) 1999 Free Software Foundation, Inc.
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
substring (/*[in]*/ const char *	lpszStart,
	   /*[in]*/ const char *	lpszEnd)
{
  char * lpszResult = malloc((lpszEnd - lpszStart + 1) * sizeof(*lpszStart));
  char * lpszScanResult = lpszResult;
  const char * lpsz = lpszStart;
  if (lpszResult == 0)
    {
      return (0);
    }
  while (lpsz < lpszEnd)
    {
      *lpszScanResult++ = *lpsz++;
    }
  *lpszScanResult = 0;
  return (lpszResult);
}
