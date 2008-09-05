/* miktex/Core/config.h: core library configuration		-*- C++ -*-

   Copyright (C) 2008 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

/* This file was generated from miktex/Core/config.h.cmake. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(EAFC4257E4C00445B61E2528E8344FE6)
#define EAFC4257E4C00445B61E2528E8344FE6

#if ! defined(HAVE_PTHREAD)
#  if ! defined(CMAKE_USE_PTHREADS_INIT)
#cmakedefine CMAKE_USE_PTHREADS_INIT ${CMAKE_USE_PTHREADS_INIT}
#  endif
#  if defined(CMAKE_USE_PTHREADS_INIT) && CMAKE_USE_PTHREADS_INIT
#    define HAVE_PTHREAD 1
#  endif
#endif

#endif
