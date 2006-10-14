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

disable_runtime_checks()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(${tiff_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/tif_fax3sm.c
  ${CMAKE_CURRENT_BINARY_DIR}/tiffvers.h
  libtiff-version.h
  t4.h
  tif_aux.c
  tif_close.c
  tif_codec.c
  tif_color.c
  tif_compress.c
  tif_dir.c
  tif_dir.h
  tif_dirinfo.c
  tif_dirread.c
  tif_dirwrite.c
  tif_dumpmode.c
  tif_error.c
  tif_extension.c
  tif_fax3.c
  tif_fax3.h
  tif_flush.c
  tif_getimage.c
  tif_jpeg.c
  tif_luv.c
  tif_lzw.c
  tif_next.c
  tif_ojpeg.c
  tif_open.c
  tif_packbits.c
  tif_pixarlog.c
  tif_predict.c
  tif_predict.h
  tif_print.c
  tif_read.c
  tif_strip.c
  tif_swab.c
  tif_thunder.c
  tif_tile.c
  tif_version.c
  tif_warning.c
  tif_write.c
  tif_zip.c
  tiff.h
  tiffcomp.h
  tiffconf.h
  tiffio.h
  tiffiop.h
  uvcode.h
)

if(WIN32)
  set(${tiff_dll_name}_sources ${${tiff_dll_name}_sources}
    libtiff.def
    libtiff.rc
    tif_win32.c
  )
endif(WIN32)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/tif_fax3sm.c
  ${CMAKE_CURRENT_BINARY_DIR}/tiffvers.h
  PROPERTIES GENERATED TRUE
)

add_executable(mkversion EXCLUDE_FROM_ALL
  mkversion.c
)

get_target_property(mkversion_exe mkversion LOCATION)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tiffvers.h
  COMMAND ${mkversion_exe}
	-r ${CMAKE_CURRENT_SOURCE_DIR}/RELEASE-DATE
	-v ${CMAKE_CURRENT_SOURCE_DIR}/version
	${CMAKE_CURRENT_BINARY_DIR}/tiffvers.h
  MAIN_DEPENDENCY version
  DEPENDS mkversion RELEASE-DATE
  VERBATIM
)

add_executable(mkg3states EXCLUDE_FROM_ALL
  mkg3states.c
)

target_link_libraries(mkg3states ${gnu_dll_name})

get_target_property(mkg3states_exe mkg3states LOCATION)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tif_fax3sm.c
  COMMAND ${mkg3states_exe} -c const ${CMAKE_CURRENT_BINARY_DIR}/tif_fax3sm.c
  DEPENDS mkg3states
  VERBATIM
)

add_library(${tiff_dll_name} SHARED ${${tiff_dll_name}_sources})

install(TARGETS ${tiff_dll_name} DESTINATION "${bindir}")
