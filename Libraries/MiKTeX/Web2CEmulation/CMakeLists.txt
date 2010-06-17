## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2010 Christian Schenk
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
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(${w2cemu_dll_name}_sources
  StdAfx.cpp
  StdAfx.h
  include/miktex/W2C/Emulation.h
  internal.h
  w2cemu-version.h
  w2cemu.cpp
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/w2cemu.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/w2cemu.rc
  )
  set(${w2cemu_dll_name}_sources ${${w2cemu_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/w2cemu.rc
  )
endif(NATIVE_WINDOWS)

if(NOT LINK_EVERYTHING_STATICALLY)
  add_library(${w2cemu_dll_name} SHARED ${${w2cemu_dll_name}_sources})

  set_target_properties(${w2cemu_dll_name}
    PROPERTIES
      VERSION "${MIKTEX_SERIES_STR}"
      SOVERSION "1"
  )

  target_link_libraries(${w2cemu_dll_name}
    ${core_dll_name}
    ${kpsemu_dll_name}
#    ${texmf_dll_name}
  )

  rebase(${w2cemu_dll_name})

  install(TARGETS ${w2cemu_dll_name}
    RUNTIME DESTINATION "${bindir}"
    LIBRARY DESTINATION "${libdir}"
    ARCHIVE DESTINATION "${libdir}"
  )
endif(NOT LINK_EVERYTHING_STATICALLY)

if(INSTALL_MIKTEX_HEADERS)
  install(
    FILES
      include/miktex/W2C/Emulation
      include/miktex/W2C/Emulation.h
    DESTINATION
      ${incdir}/miktex/W2C
  )
endif(INSTALL_MIKTEX_HEADERS)