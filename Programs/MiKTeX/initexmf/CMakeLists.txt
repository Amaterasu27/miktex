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

set(initexmf_sources
  initexmf-version.h
  initexmf.cpp
)

if(NATIVE_WINDOWS)
  set(initexmf_sources ${initexmf_sources} initexmf.rc)
endif(NATIVE_WINDOWS)

add_executable(initexmf ${initexmf_sources})

install(TARGETS initexmf DESTINATION ${bindir})

target_link_libraries(initexmf
  ${core_dll_name}
  ${mpm_dll_name}
  ${popt_dll_name}
)

if(BUILDING_MIKTEX)
  target_link_libraries(initexmf
    ${texmf_dll_name}
  )
endif(BUILDING_MIKTEX)

merge_trustinfo_manifest(initexmf asInvoker)

if(NATIVE_WINDOWS)
  set(aliases mktexlsr texhash texlinks)
  create_exe_alias(mktexlsr initexmf -u -v)
  create_exe_alias(texhash initexmf -u -v)
  create_exe_alias(texlinks initexmf --mklinks -v)
  install(TARGETS ${aliases} DESTINATION ${bindir})
endif(NATIVE_WINDOWS)
