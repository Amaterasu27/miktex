## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2007-2008 Christian Schenk
## 
## This file is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published
## by the Free Software Foundation; either version 2, or (at your
## option) any later version.
## 
## This file is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this file; if not, write to the Free Software
## Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
## USA.

if(NATIVE_WINDOWS)
  set(FREETYPE2 TRUE)
  set(HAVE_FREETYPE_FREETYPE_H TRUE)
  set(MULTITHREADED TRUE)
endif(NATIVE_WINDOWS TRUE)

set(USE_EXCEPTIONS TRUE)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/aconf.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/aconf.h
)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(goo)
add_subdirectory(fofi)
add_subdirectory(xpdf)
