## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2007-2015 Christian Schenk
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

include_directories(BEFORE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(fc-case)
add_subdirectory(fc-lang)
add_subdirectory(fc-glyphname)
add_subdirectory(src)
add_subdirectory(fc-cache)
add_subdirectory(fc-cat)
add_subdirectory(fc-list)
add_subdirectory(fc-match)
add_subdirectory(fc-pattern)
add_subdirectory(fc-query)
add_subdirectory(fc-scan)
