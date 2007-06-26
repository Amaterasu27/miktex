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

foreach(t conspawn findexe runbat runperl)
  add_executable(${t} ${t}.cpp)
  merge_trustinfo_manifest(${t} asInvoker)
  target_link_libraries(${t}
    ${app_dll_name}
    ${core_dll_name}
  )
  install(TARGETS ${t} DESTINATION "${bindir}")
endforeach(t)

add_executable(copystart_admin WIN32 copystart.cpp)

merge_trustinfo_manifest(copystart_admin highestAvailable)

target_link_libraries(copystart_admin
  ${core_dll_name}
)

install(TARGETS copystart_admin DESTINATION "${bindir}")
