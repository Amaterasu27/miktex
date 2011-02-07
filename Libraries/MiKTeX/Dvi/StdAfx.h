/* StdAfx.h:							-*- C++ -*-

   Copyright (C) 1996-2011 Christian Schenk

   This file is part of the MiKTeX DVI Library.

   The MiKTeX DVI Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2, or (at your option) any later version.

   The MiKTeX DVI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the MiKTeX DVI Library; if not, write to the
   Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139,
   USA.  */

#pragma once

#include <miktex/Core/First>

#if defined(HAVE_CONFIG_H)
#  include "config.h"
#endif

#include <cctype>
#include <climits>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#if ! defined(HAVE_UNORDERED_MAP)
#  include <map>
#endif
#include <memory>
#include <stack>
#include <string>
#if defined(HAVE_UNORDERED_MAP)
#  include <unordered_map>
#endif

#include <bmeps.h>

#include <miktex/Graphics/DibChunker>
#include <miktex/Core/Core>
#include <miktex/Core/Debug>
#include <miktex/Core/Paths>
#include <miktex/Core/Trace>
