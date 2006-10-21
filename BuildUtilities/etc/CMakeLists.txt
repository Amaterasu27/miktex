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

###############################################################################
# bintoc

add_executable(bintoc
  bintoc.cpp
)

install(TARGETS bintoc DESTINATION "${bindir}")

###############################################################################
# headerguard

if(MIKTEX_WINDOWS)

  add_executable(headerguard
    headerguard.cpp
  )

  target_link_libraries(headerguard Rpcrt4)

  install(TARGETS headerguard DESTINATION "${bindir}")

endif(MIKTEX_WINDOWS)

###############################################################################
# j2000

add_executable(j2000
  j2000.cpp
)

install(TARGETS j2000 DESTINATION "${bindir}")

###############################################################################
# mkdllbases

if(MIKTEX_WINDOWS)

  add_executable(mkdllbases
    mkdllbases.cpp
  )

  target_link_libraries(mkdllbases imagehlp)

  install(TARGETS mkdllbases DESTINATION "${bindir}")

endif(MIKTEX_WINDOWS)
