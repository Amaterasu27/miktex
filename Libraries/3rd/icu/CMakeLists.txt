## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2007 Christian Schenk
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
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/msvc90/icudt.lib
    ${LIBRARY_OUTPUT_PATH}/icudt.lib
    COPYONLY
  )
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/msvc90/icule.lib
    ${LIBRARY_OUTPUT_PATH}/icule.lib
    COPYONLY
  )
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/msvc90/icuuc.lib
    ${LIBRARY_OUTPUT_PATH}/icuuc.lib
    COPYONLY
  )
endif(NATIVE_WINDOWS)
