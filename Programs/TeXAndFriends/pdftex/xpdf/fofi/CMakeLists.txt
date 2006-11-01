## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006 Christian Schenk
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

include_directories(
  ..
  ../goo
)

set(${fofi_lib_name}_sources
  FoFiBase.cc
  FoFiBase.h
  FoFiEncodings.cc
  FoFiEncodings.h
  FoFiTrueType.cc
  FoFiTrueType.h
  FoFiType1.cc
  FoFiType1.h
  FoFiType1C.cc
  FoFiType1C.h
)

add_library(${fofi_lib_name} STATIC ${${fofi_lib_name}_sources})
