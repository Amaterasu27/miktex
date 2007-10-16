/* StdAfx.h:							-*- C++ -*-

   Copyright (C) 2001-2006 Christian Schenk

   This file is part of MPC.

   MPC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   MPC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MPC; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#if defined(_MSC_VER)
// C4786: identifier was truncated to 'number' characters in the debug
// information
#  pragma warning (disable: 4786)
#endif

#include <cstdarg>
#include <cstdio>

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <sys/stat.h>

#include <getopt.h>

#include <MiKTeX/Core/Core>
#include <miktex/md5.h>
#include <miktex/mpm.h>
#include <MiKTeX/Core/Paths>
#include <miktex/version.h>
