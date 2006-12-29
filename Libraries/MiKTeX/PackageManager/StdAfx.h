/* StdAfx.h: pre-compiled header stuff				-*- C++ -*-

   Copyright (C) 2001-2006 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if defined(HAVE_CONFIG_H)
#  include <config.h>
#endif

#include "resource.h"

// must be included before Fdi.h (because of HUGE)
#include <cmath>

#if defined(_WIN32) || defined(_WIN64)
#  include <windows.h>
#  include <wininet.h>
#  include <Fdi.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#  define _ATL_APARTMENT_THREADED
#  define _ATL_NO_AUTOMATIC_NAMESPACE
#  define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#  include <atlbase.h>
#  include <atlcom.h>
using namespace ATL;
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

#if defined(HAVE_LIBCURL)
#  include <curl/curl.h>
#endif

#include <mspack.h>

#include <expat.h>

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

#include <bzlib.h>
#include <miktex/core.h>
#include <miktex/env.h>
#include <miktex/md5.h>
#include <miktex/paths.h>
#include <miktex/reg.h>
#include <miktex/trace.h>
