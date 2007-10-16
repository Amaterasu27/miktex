/* StdAfx.h: pre-compiled header stuff				-*- C++ -*-

   Copyright (C) 2001-2007 Christian Schenk

   This file is part of MiKTeX Extractor.

   MiKTeX Extractor is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.
   
   MiKTeX Extractor is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Extractor; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if defined(HAVE_CONFIG_H)
#  include <config.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#  include <windows.h>
#  include <Fdi.h>
#endif

#include <algorithm>
#include <locale>
#include <map>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#if defined(_MSC_VER)
#  include <hash_map>
using stdext::hash_map;
#  define USE_HASH_MAP
#  include <hash_set>
using stdext::hash_set;
#  define USE_HASH_SET
#endif

#include <mspack.h>

#if defined(_MSC_VER)
#  include <malloc.h>
#  include <direct.h>
#  include <io.h>
#  include <share.h>
#endif

#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>

#include <cstdarg>

#include <MiKTeX/Core/Core>
#include <MiKTeX/Core/Debug>
#include <MiKTeX/Core/Environment>
#include <miktex/md5.h>
#include <miktex/paths.h>
#include <miktex/reg.h>
#include <miktex/trace.h>
