## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2013 Christian Schenk
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
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(${setup_dll_name}_sources
  SetupService.cpp
  include/miktex/Setup/SetupService.h
  internal.h
  setup-version.h
  win/winSetupService.cpp
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/win/setup.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/win/setup.rc
  )
  set(${setup_dll_name}_sources ${${setup_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/win/setup.rc
  )
endif(NATIVE_WINDOWS)

if(NOT LINK_EVERYTHING_STATICALLY)
  add_library(${setup_dll_name} SHARED ${${setup_dll_name}_sources})

  set_target_properties(${setup_dll_name}
    PROPERTIES
      VERSION "${MIKTEX_SERIES_STR}"
      SOVERSION "1"
  )

  target_link_libraries(${setup_dll_name}
    ${core_dll_name}
    ${mpm_dll_name}
  )

  rebase(${setup_dll_name})

  install(TARGETS ${setup_dll_name}
    RUNTIME DESTINATION "${bindir}"
    LIBRARY DESTINATION "${libdir}"
    ARCHIVE DESTINATION "${libdir}"
  )
endif(NOT LINK_EVERYTHING_STATICALLY)
