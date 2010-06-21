## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2010 Christian Schenk
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
  add_definitions(-DIN_JPEG_DLL)
endif(NATIVE_WINDOWS)

set(${jpeg_dll_name}_sources
  #COMOBJECTS
  jaricom.c
  jcomapi.c
  jutils.c
  jerror.c
  jmemmgr.c
  #SYSDEPMEM
  jmemnobs.c
  #CLIBOBJECTS
  jcapimin.c
  jcapistd.c
  jcarith.c
  jctrans.c
  jcparam.c
  jdatadst.c
  jcinit.c
  jcmaster.c
  jcmarker.c
  jcmainct.c
  jcprepct.c
  jccoefct.c
  jccolor.c
  jcsample.c
  jchuff.c
  jcdctmgr.c
  jfdctfst.c
  jfdctflt.c
  jfdctint.c
  #DLIBOBJECTS
  jdapimin.c
  jdapistd.c
  jdarith.c
  jdtrans.c
  jdatasrc.c
  jdmaster.c
  jdinput.c
  jdmarker.c
  jdhuff.c
  jdmainct.c
  jdcoefct.c
  jdpostct.c
  jddctmgr.c
  jidctfst.c
  jidctflt.c
  jidctint.c
  jdsample.c
  jdcolor.c
  jquant1.c
  jquant2.c
  jdmerge.c
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/libjpeg.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libjpeg.rc
  )
  set(${jpeg_dll_name}_sources ${${jpeg_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/libjpeg.rc
  )
endif(NATIVE_WINDOWS)

add_library(${jpeg_dll_name} SHARED ${${jpeg_dll_name}_sources})

set_target_properties(${jpeg_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

rebase(${jpeg_dll_name})

install(TARGETS ${jpeg_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
