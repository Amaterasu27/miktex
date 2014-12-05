## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2008-2014 Christian Schenk
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

add_executable(core_thread_test1 1.cpp)

merge_trustinfo_manifest(core_thread_test1 asInvoker)

target_link_libraries(core_thread_test1
  ${CMAKE_THREAD_LIBS_INIT}
  ${core_dll_name}
  ${popt_dll_name}
)

add_test(NAME core_thread_test1 COMMAND $<TARGET_FILE:core_thread_test1>)
