/* StdAfx.h: pre-compiled header stuff				-*- C++ -*-

   Copyright (C) 2005-2006 Christian Schenk

   This file is part of the MiKTeX App Library.

   The MiKTeX App Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX App Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX App Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if defined(HAVE_CONFIG_H)
#  include "config.h"
#endif

#if defined(_WIN32) || defined(_WIN64)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

#include <signal.h>

#include <cstdlib>
#include <ctime>

#include <MiKTeX/Core/Core>
#include <miktex/reg.h>
#include <miktex/paths.h>
#include <miktex/trace.h>

#if defined(MIKTEX_WINDOWS)
#  include <MiKTeX/UI/MFC/Prototypes>
#endif
