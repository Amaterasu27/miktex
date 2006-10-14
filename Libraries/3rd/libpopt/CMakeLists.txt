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

set(${popt_dll_name}_sources
  findme.c
  findme.h
  popt-miktex.h
  popt-version.h
  popt.c
  popt.h
  poptconfig.c
  popthelp.c
  poptint.h
  poptparse.c
  system.h
)

if(WIN32)
  set(${popt_dll_name}_sources ${${popt_dll_name}_sources}
    popt.rc
  )
endif(WIN32)

add_library(${popt_dll_name} SHARED ${${popt_dll_name}_sources})

install(TARGETS ${popt_dll_name} DESTINATION "${bindir}")
