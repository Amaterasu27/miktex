## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2007-2010 Christian Schenk
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
  if(CMAKE_CL_64)
    set(iculibdir_ ${CMAKE_CURRENT_SOURCE_DIR}/lib/vc100/x64)
  else(CMAKE_CL_64)
    set(iculibdir_ ${CMAKE_CURRENT_SOURCE_DIR}/lib/vc100/x86)
  endif(CMAKE_CL_64)
  configure_file(
    ${iculibdir_}/icudt.lib
    ${LIBRARY_OUTPUT_PATH}/icudt.lib
    COPYONLY
  )
  configure_file(
    ${iculibdir_}/icule.lib
    ${LIBRARY_OUTPUT_PATH}/icule.lib
    COPYONLY
  )
  configure_file(
    ${iculibdir_}/icuuc.lib
    ${LIBRARY_OUTPUT_PATH}/icuuc.lib
    COPYONLY
  )
endif(NATIVE_WINDOWS)
