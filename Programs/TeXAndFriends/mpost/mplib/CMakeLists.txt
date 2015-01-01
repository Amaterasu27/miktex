## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2010-2014 Christian Schenk
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

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_cairo_dir}
  ${CMAKE_BINARY_DIR}/${rel_cairo_include_dir}
  ${CMAKE_SOURCE_DIR}/${rel_cairo_include_dir}
  ${CMAKE_SOURCE_DIR}/${rel_pixman_include_dir}
)

add_definitions(
  -DAPPTAG=mpost
  -DUSE_MIKTEX_EXIT
  -DHAVE_SNPRINTF=1
  -Dsnprintf=_snprintf
  -DHAVE_SYS_STAT_H=1
)

add_definitions(
  -D_UNICODE -DUNICODE
)

set(mplib_cweb_sources
  ${CMAKE_CURRENT_BINARY_DIR}/mp.c
  ${CMAKE_CURRENT_BINARY_DIR}/mpmath.c
  ${CMAKE_CURRENT_BINARY_DIR}/mpmathdecimal.c
  ${CMAKE_CURRENT_BINARY_DIR}/mpmathdouble.c
  ${CMAKE_CURRENT_BINARY_DIR}/mpstrings.c
  ${CMAKE_CURRENT_BINARY_DIR}/pngout.c
  ${CMAKE_CURRENT_BINARY_DIR}/psout.c
  ${CMAKE_CURRENT_BINARY_DIR}/svgout.c
  ${CMAKE_CURRENT_BINARY_DIR}/tfmin.c

  ${CMAKE_CURRENT_BINARY_DIR}/mplib.h
  ${CMAKE_CURRENT_BINARY_DIR}/mplibps.h
  ${CMAKE_CURRENT_BINARY_DIR}/mplibsvg.h
  ${CMAKE_CURRENT_BINARY_DIR}/mpmath.h
  ${CMAKE_CURRENT_BINARY_DIR}/mpmathdecimal.h
  ${CMAKE_CURRENT_BINARY_DIR}/mpmathdouble.h
  ${CMAKE_CURRENT_BINARY_DIR}/mpmp.h
  ${CMAKE_CURRENT_BINARY_DIR}/mppsout.h
  ${CMAKE_CURRENT_BINARY_DIR}/mpstrings.h
  ${CMAKE_CURRENT_BINARY_DIR}/mpsvgout.h
  ${CMAKE_CURRENT_BINARY_DIR}/mptfmin.h
  ${CMAKE_CURRENT_BINARY_DIR}/pngout.h
)

set_source_files_properties(
  ${mplib_cweb_sources}
  PROPERTIES GENERATED TRUE
)

add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/mp.c
    ${CMAKE_CURRENT_BINARY_DIR}/mplib.h
    ${CMAKE_CURRENT_BINARY_DIR}/mpmp.h
  COMMAND
    ${ctangle_exe} ${CMAKE_CURRENT_SOURCE_DIR}/mp
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}
  MAIN_DEPENDENCY
    mp.w
  DEPENDS
    ${ctangle_exe}
    ${CMAKE_CURRENT_SOURCE_DIR}/mp.w
  VERBATIM
)

add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/mpmath.c
    ${CMAKE_CURRENT_BINARY_DIR}/mpmath.h
  COMMAND
    ${ctangle_exe} ${CMAKE_CURRENT_SOURCE_DIR}/mpmath
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}
  MAIN_DEPENDENCY
    mpmath.w
  DEPENDS
    ${ctangle_exe}
    ${CMAKE_CURRENT_SOURCE_DIR}/mpmath.w
  VERBATIM
)

add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/mpmathdecimal.c
    ${CMAKE_CURRENT_BINARY_DIR}/mpmathdecimal.h
  COMMAND
    ${ctangle_exe} ${CMAKE_CURRENT_SOURCE_DIR}/mpmathdecimal
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}
  MAIN_DEPENDENCY
    mpmathdecimal.w
  DEPENDS
    ${ctangle_exe}
    ${CMAKE_CURRENT_SOURCE_DIR}/mpmathdecimal.w
  VERBATIM
)

add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/mpmathdouble.c
    ${CMAKE_CURRENT_BINARY_DIR}/mpmathdouble.h
  COMMAND
    ${ctangle_exe} ${CMAKE_CURRENT_SOURCE_DIR}/mpmathdouble
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}
  MAIN_DEPENDENCY
    mpmathdouble.w
  DEPENDS
    ${ctangle_exe}
    ${CMAKE_CURRENT_SOURCE_DIR}/mpmathdouble.w
  VERBATIM
)

add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/mpstrings.c
    ${CMAKE_CURRENT_BINARY_DIR}/mpstrings.h
  COMMAND
    ${ctangle_exe} ${CMAKE_CURRENT_SOURCE_DIR}/mpstrings
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}
  MAIN_DEPENDENCY
    mpstrings.w
  DEPENDS
    ${ctangle_exe}
    ${CMAKE_CURRENT_SOURCE_DIR}/mpstrings.w
  VERBATIM
)

add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/pngout.c
    ${CMAKE_CURRENT_BINARY_DIR}/pngout.h
  COMMAND
    ${ctangle_exe} ${CMAKE_CURRENT_SOURCE_DIR}/pngout
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}
  MAIN_DEPENDENCY
    pngout.w
  DEPENDS
    ${ctangle_exe}
    ${CMAKE_CURRENT_SOURCE_DIR}/pngout.w
  VERBATIM
)

add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/mplibps.h
    ${CMAKE_CURRENT_BINARY_DIR}/mppsout.h
    ${CMAKE_CURRENT_BINARY_DIR}/psout.c
  COMMAND
    ${ctangle_exe} ${CMAKE_CURRENT_SOURCE_DIR}/psout
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}
  MAIN_DEPENDENCY
    psout.w
  DEPENDS
    ${ctangle_exe}
    ${CMAKE_CURRENT_SOURCE_DIR}/psout.w
  VERBATIM
)

add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/mplibsvg.h
    ${CMAKE_CURRENT_BINARY_DIR}/mpsvgout.h
    ${CMAKE_CURRENT_BINARY_DIR}/svgout.c
  COMMAND
    ${ctangle_exe} ${CMAKE_CURRENT_SOURCE_DIR}/svgout
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}
  MAIN_DEPENDENCY
    svgout.w
  DEPENDS
    ${ctangle_exe}
    ${CMAKE_CURRENT_SOURCE_DIR}/svgout.w
  VERBATIM
)

add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/mptfmin.h
    ${CMAKE_CURRENT_BINARY_DIR}/tfmin.c
  COMMAND
    ${ctangle_exe} ${CMAKE_CURRENT_SOURCE_DIR}/tfmin
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}
  MAIN_DEPENDENCY
    tfmin.w
  DEPENDS
    ${ctangle_exe}
    ${CMAKE_CURRENT_SOURCE_DIR}/tfmin.w
  VERBATIM
)

set(mplib_sources
  ${mplib_cweb_sources}
  avl.c
  avl.h
  decContext.c
  decContext.h
  decNumber.c
  decNumber.h
  decNumberLocal.h
)

add_library(${mp_lib_name} STATIC ${mplib_sources})

set(mpost_cweb_sources
  ${CMAKE_CURRENT_BINARY_DIR}/mpost.c
  ${CMAKE_CURRENT_BINARY_DIR}/mpxout.c
  ${CMAKE_CURRENT_BINARY_DIR}/mpxout.h
)

set_source_files_properties(
  ${mpost_cweb_sources}
  PROPERTIES GENERATED TRUE
)

add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/mpost.c
  COMMAND
    ${ctangle_exe} ${CMAKE_CURRENT_SOURCE_DIR}/mpost
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}
  MAIN_DEPENDENCY
    mpost.w
  DEPENDS
    ${ctangle_exe}
    ${CMAKE_CURRENT_SOURCE_DIR}/mpost.w
  VERBATIM
)

add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/mpxout.c
    ${CMAKE_CURRENT_BINARY_DIR}/mpxout.h
  COMMAND
    ${ctangle_exe} ${CMAKE_CURRENT_SOURCE_DIR}/mpxout
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}
  MAIN_DEPENDENCY
    mpxout.w
  DEPENDS
    ${ctangle_exe}
    ${CMAKE_CURRENT_SOURCE_DIR}/mpxout.w
  VERBATIM
)

set(mpost_sources
  ${mpost_cweb_sources}
  ${wrapper_cpp}
  mpost-version.h
)

if(NATIVE_WINDOWS)
  set(mpost_sources ${mpost_sources} mpost.rc)
endif(NATIVE_WINDOWS)

set(executables
  ${miktex_prefix}mpost
  dvitomp
  mpost
)

foreach(x ${executables})
  add_executable(${x} ${mpost_sources})
  target_link_libraries(${x}
    ${app_dll_name}
    ${cairo_dll_name}
    ${core_dll_name}
    ${getopt_dll_name}
    ${kpsemu_dll_name}
    ${mp_lib_name}
	${png_dll_name}
    ${pixman_lib_name}
    ${utf8wrap_dll_name}
    ${w2cemu_dll_name}
    ${zlib_dll_name}
  )
  merge_trustinfo_manifest(${x} asInvoker)
  install(TARGETS ${x} DESTINATION "${bindir}")
endforeach(x)

set(trap_dir ${CMAKE_CURRENT_SOURCE_DIR}/trapdir)

configure_file(
  traptest.perl.in
  ${CMAKE_CURRENT_BINARY_DIR}/traptest.perl
)

add_custom_target(mp-traptest
  COMMAND
    ${PERL_EXE} ${CMAKE_CURRENT_BINARY_DIR}/traptest.perl
  DEPENDS
    mpost
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
)
