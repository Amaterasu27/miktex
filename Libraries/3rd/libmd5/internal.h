/* internal.h:						-*- C++ -*-

   Copyright (C) 2001-2006 Christian Schenk

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
   USA. */

#include <cstdio>
#include <cstring>

#include "global.h"

#if ! defined(MIKTEX_STATIC) && defined(_MSC_VER)
#  define MIKTEXMD5API(type) __declspec(dllexport) type __stdcall
#elif defined(_MSC_VER)
#  define MIKTEXMD5API(type) type __stdcall
#else
#  define MIKTEXMD5API(type) type
#endif

#include "MiKTeX/Core/MD5"

using namespace MiKTeX::Core;
using namespace MiKTeX;
