/* internal.h: internal stuff					-*- C++ -*-

   Copyright (C) 2000-2006 Christian Schenk

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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <malloc.h>

#if defined(_MSC_VER)
#  if ! defined(MIKTEX_STATIC)
#    define GNUDLLAPI(type) __declspec(dllexport) type __cdecl
#  else
#    define GNUDLLAPI(type) type __cdecl
#  endif
#else
#  define GNUDLLAPI(type) type
#endif

#define MIKTEXGNU__DD8F4232_5316_4DA9_9EF5_E0B47647A406__
#include "gnu-miktex.h"
