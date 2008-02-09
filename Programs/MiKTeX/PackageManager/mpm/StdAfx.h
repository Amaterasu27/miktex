/* StdAfx.h:							-*- C++ -*-

   Copyright (C) 2003-2007 Christian Schenk

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

#include <cstdio>

#if defined(_MSC_VER)
#  pragma warning (push, 1)
#  pragma warning (disable: 4702)
#endif

#if defined(HAVE_CONFIG_H)
#  include <config.h>
#endif

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include <signal.h>

#if defined(_MSC_VER)
#  pragma warning (pop)
#endif

#include "mpm-version.h"

#include <miktex/Core/Core>
#include <miktex/PackageManager/PackageManager>
#include <miktex/Core/Paths>
#include <popt-miktex.h>
#include <miktex/Core/Trace>

#if defined(MIKTEX_WINDOWS)
#  include <MiKTeX/Core/Help>
#endif

using namespace MiKTeX::Packages;
using namespace MiKTeX::Core;
using namespace std;
