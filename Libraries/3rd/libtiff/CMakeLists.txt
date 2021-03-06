## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2006-2015 Christian Schenk
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

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(${tiff_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/tif_fax3sm.c
  ${CMAKE_CURRENT_BINARY_DIR}/include/tiffvers.h
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

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/libtiff.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libtiff.rc
  )
  set(${tiff_dll_name}_sources ${${tiff_dll_name}_sources}
    libtiff.def
    ${CMAKE_CURRENT_BINARY_DIR}/libtiff.rc
    tif_win32.c
  )
endif()

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/tif_fax3sm.c
  ${CMAKE_CURRENT_BINARY_DIR}/include/tiffvers.h
  PROPERTIES GENERATED TRUE
)

add_executable(mkversion EXCLUDE_FROM_ALL
  mkversion.c
)

merge_trustinfo_manifest(mkversion asInvoker)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/tiffvers.h
  COMMAND mkversion
	-r ${CMAKE_CURRENT_SOURCE_DIR}/RELEASE-DATE
	-v ${CMAKE_CURRENT_SOURCE_DIR}/VERSION
	${CMAKE_CURRENT_BINARY_DIR}/include/tiffvers.h
  MAIN_DEPENDENCY
	VERSION
  DEPENDS
	mkversion
	${CMAKE_CURRENT_SOURCE_DIR}/RELEASE-DATE
	${CMAKE_CURRENT_SOURCE_DIR}/VERSION
  VERBATIM
)

add_executable(mkg3states EXCLUDE_FROM_ALL
  mkg3states.c
)

merge_trustinfo_manifest(mkg3states asInvoker)

target_link_libraries(mkg3states
  PRIVATE
    ${getopt_dll_name}
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tif_fax3sm.c
  COMMAND mkg3states -c const ${CMAKE_CURRENT_BINARY_DIR}/tif_fax3sm.c
  DEPENDS mkg3states
  VERBATIM
)

add_library(${tiff_dll_name} SHARED ${${tiff_dll_name}_sources})

set_target_properties(${tiff_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_include_directories(${tiff_dll_name}
  PUBLIC
    ${CMAKE_BINARY_DIR}/${rel_tiff_dir}/include
)

rebase(${tiff_dll_name})

install(TARGETS ${tiff_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

set(public_headers
  tiff.h
  tiffio.h
)

foreach(h ${public_headers})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${h}
    ${CMAKE_BINARY_DIR}/${rel_tiff_dir}/include/${h}
    COPYONLY
  )
endforeach()
