/* StdAfx.h: pre-compiled header stuff				-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of the MiKTeX TeXMF Library.

   The MiKTeX TeXMF Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX TeXMF Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX TeXMF Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if defined(_MSC_VER)
#  pragma warning (disable: 4702)
#  pragma warning (disable: 4786)
#endif

#if defined(_WIN32) || defined(_WIN64)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

#include <cstdlib>
#include <ctime>

#include <malloc.h>

#include <iomanip>
#include <iostream>

#include <direct.h>

#include <miktex/core.h>
#include <miktex/app.h>
#include <miktex/reg.h>
#include <miktex/paths.h>
#include <miktex/trace.h>

#include <miktex/mikui.h>

#include <process.h>

#if defined(_MSC_VER)
#  pragma warning (push, 1)
#  pragma warning (disable: 4267)
#  include <hash_set>
#  include <vector>
#  pragma warning (pop)
#endif

extern "C" {
#include <zlib.h>
#include <bzlib.h>
}
