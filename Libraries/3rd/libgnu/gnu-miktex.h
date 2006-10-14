/* gnu-miktex.h: GNUish utilities			-*- C++ -*-

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

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(MIKTEXGNU__EF0F3CFF_7E38_4646_9562_359EBE5F2F1A__)
#define MIKTEXGNU__EF0F3CFF_7E38_4646_9562_359EBE5F2F1A__

#if defined(__cplusplus)
extern "C" {
#endif

#if ! defined(MIKTEXGNU__DD8F4232_5316_4DA9_9EF5_E0B47647A406__)
#  if ! defined(MIKTEX_STATIC) && defined(_MSC_VER)
#    define GNUDLLAPI(type) __declspec(dllimport) type __stdcall
#  elif defined(_MSC_VER)
#    define GNUDLLAPI(type) type __stdcall
#  else
#    define GNUDLLAPI(type) type
#  endif
#endif

GNUDLLAPI(char *) basename (const char * lspszFileName);
GNUDLLAPI(char *) substring (const char * lpszStart, const char * lpszEnd);

#if defined(__cplusplus)
}
#endif

#endif
