/* config.h (created from config.h.cmake)			-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

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

#cmakedefine CMAKE_USE_PTHREADS_INIT 1
#cmakedefine HAVE_DIRENT_H 1
#cmakedefine HAVE_FORK 1
#cmakedefine HAVE_INTTYPES_H 1
#cmakedefine HAVE_STRUCT_DIRENT_D_TYPE 1
#cmakedefine HAVE_SYS_MMAN_H 1
#cmakedefine HAVE_SYS_STATVFS_H 1
#cmakedefine HAVE_SYS_STAT_H 1
#cmakedefine HAVE_SYS_UTIME_H 1
#cmakedefine HAVE_SYS_UTSNAME_H 1
#cmakedefine HAVE_SYS_WAIT_H 1
#cmakedefine HAVE_UNISTD_H 1
#cmakedefine HAVE_UTIME_H 1
#cmakedefine HAVE_VFORK 1

#if defined(CMAKE_USE_PTHREADS_INIT)
#  define HAVE_PTHREAD
#endif