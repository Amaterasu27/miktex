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

if(MIKTEX_WINDOWS)
  add_definitions(-DIN_JPEG_DLL)
endif(MIKTEX_WINDOWS)

set(${jpeg_dll_name}_sources
  jcapimin.c
  jcapistd.c
  jccoefct.c
  jccolor.c
  jcdctmgr.c
  jchuff.c
  jchuff.h
  jcinit.c
  jcmainct.c
  jcmarker.c
  jcmaster.c
  jcomapi.c
  jconfig.h
  jcparam.c
  jcphuff.c
  jcprepct.c
  jcsample.c
  jctrans.c
  jdapimin.c
  jdapistd.c
  jdatadst.c
  jdatasrc.c
  jdcoefct.c
  jdcolor.c
  jdct.h
  jddctmgr.c
  jdhuff.c
  jdhuff.h
  jdinput.c
  jdmainct.c
  jdmarker.c
  jdmaster.c
  jdmerge.c
  jdphuff.c
  jdpostct.c
  jdsample.c
  jdtrans.c
  jerror.c
  jerror.h
  jfdctflt.c
  jfdctfst.c
  jfdctint.c
  jidctflt.c
  jidctfst.c
  jidctint.c
  jidctred.c
  jinclude.h
  jmemmgr.c
  jmemnobs.c
  jmemsys.h
  jmorecfg.h
  jpegint.h
  jpeglib.h
  jquant1.c
  jquant2.c
  jutils.c
  jversion.h
  libjpeg-version.h
)

if(MIKTEX_WINDOWS)
  set(${jpeg_dll_name}_sources ${${jpeg_dll_name}_sources}
    libjpeg.rc
  )
endif(MIKTEX_WINDOWS)

add_library(${jpeg_dll_name} SHARED ${${jpeg_dll_name}_sources})

install(TARGETS ${jpeg_dll_name} DESTINATION "${bindir}")
