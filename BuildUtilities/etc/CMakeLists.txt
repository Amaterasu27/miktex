## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2007 Christian Schenk
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

add_executable(bintoc bintoc.cpp)

merge_trustinfo_manifest(bintoc asInvoker)

if(BUILDING_MIKTEX)
  install(TARGETS bintoc DESTINATION "${bindir}")
endif(BUILDING_MIKTEX)

if(NATIVE_WINDOWS)
  add_executable(headerguard headerguard.cpp)
  merge_trustinfo_manifest(headerguard asInvoker)
  target_link_libraries(headerguard Rpcrt4)
  install(TARGETS headerguard DESTINATION "${bindir}")
endif(NATIVE_WINDOWS)

add_executable(j2000 j2000.cpp)

merge_trustinfo_manifest(j2000 asInvoker)

if(BUILDING_MIKTEX)
  install(TARGETS j2000 DESTINATION "${bindir}")
endif(BUILDING_MIKTEX)

if(NATIVE_WINDOWS)
  add_executable(mkdllbases mkdllbases.cpp)
  merge_trustinfo_manifest(mkdllbases asInvoker)
  target_link_libraries(mkdllbases imagehlp)
  add_link_flags(mkdllbases "setargv.obj")
  install(TARGETS mkdllbases DESTINATION "${bindir}")
endif(NATIVE_WINDOWS)

add_executable(web-n web-n.cpp)

merge_trustinfo_manifest(web-n asInvoker)

if(BUILDING_MIKTEX)
  install(TARGETS web-n DESTINATION "${bindir}")
endif(BUILDING_MIKTEX)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/runhhc.perl.in
  ${CMAKE_CURRENT_BINARY_DIR}/runhhc.perl
  @ONLY
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/runxsltproc.perl.in
  ${CMAKE_CURRENT_BINARY_DIR}/runxsltproc.perl
  @ONLY
)
