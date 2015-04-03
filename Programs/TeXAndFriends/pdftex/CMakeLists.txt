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

if(LINK_EVERYTHING_STATICALLY)
  use_static_crt()
  use_static_libraries()
endif()

default_char_type_is_unsigned()

hide_symbols()

set(C4P_FLAGS
  --auto-exit=10
  --chars-are-unsigned
)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_synctex_include_dir}
)

add_definitions(
  -DHAVE_C4P_PRE_H
  -DHAVE_GETPDFMAJORVERSION
  -DMIKTEX_PDFTEX
  -DPDFTEXAPP=g_PDFTEXApp
  -DPDFTEXCLASS=PDFTEX
  -DPDF_PARSER_ONLY
  -DSYNCTEX_ENGINE_H="synctex-pdftex.h"
  -DUSE_GZIP
  -D__SyncTeX__
  -DpdfTeX
)

add_definitions(
  -D_UNICODE -DUNICODE
)

set(pdftex_ch_synctex
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-def.ch0
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-mem.ch0
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-e-mem.ch0
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-e-mem.ch1
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-rec.ch0
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-rec.ch1
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-e-rec.ch0
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-pdf-rec.ch2
)

set(pdftosrc_sources
  ${wrapper_cpp}
  pdftosrc.cc
  pdftosrc-version.h
)

if(NATIVE_WINDOWS)
  set(pdftosrc_sources ${pdftosrc_sources}
    pdftosrc.rc
  )
endif()

if(BUILDING_MIKTEX)
  add_executable(pdftosrc ${pdftosrc_sources})
  merge_trustinfo_manifest(pdftosrc asInvoker)
  target_link_libraries(pdftosrc
    ${app_dll_name}
    ${core_dll_name}
    ${jpeg_dll_name}
    ${poppler_lib_name}
    ${unxemu_dll_name}
    ${utf8wrap_dll_name}
    ${zlib_dll_name}
  )
  if(NATIVE_WINDOWS)
    target_link_libraries(pdftosrc
      ${getopt_dll_name}
    )
  endif()
  install(TARGETS pdftosrc DESTINATION ${bindir})
endif()

set(ttf2afm_sources
  ${wrapper_cpp}
  ttf2afm-version.h
  ttf2afm.c
)

set_source_files_properties(
  ttf2afm.c
  PROPERTIES LANGUAGE CXX
)

set_source_files_properties(
  ${wrapper_cpp}
  PROPERTIES COMPILE_FLAGS "-DCPLUSPLUSMAIN"
)

if(NATIVE_WINDOWS)
  set(ttf2afm_sources ${ttf2afm_sources}
    ttf2afm.rc
  )
endif()

if(BUILDING_MIKTEX)
  add_executable(ttf2afm ${ttf2afm_sources})
  merge_trustinfo_manifest(ttf2afm asInvoker)
  target_link_libraries(ttf2afm
    ${app_dll_name}
    ${core_dll_name}
    ${kpsemu_dll_name}
    ${texmf_dll_name}
    ${utf8wrap_dll_name}
  )
  if(NATIVE_WINDOWS)
    target_link_libraries(ttf2afm
      ${getopt_dll_name}
    )
  endif()
  install(TARGETS ttf2afm DESTINATION ${bindir})
endif()

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pdftex.defaults.h
  COMMAND ${cfg_exe} --print-classes ${pdftex_defaults} > pdftex.defaults.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${pdftex_defaults}
  VERBATIM
)

set(miktex_synctex_changefiles ${tex_miktex_synctex_ch})

set(miktex_tex_change_files
  ${mltex_miktex_ch}
  ${enctex_miktex_ch}
  ${tex_miktex_ch}
  ${tex_miktex_enctex_ch}
  ${tex_miktex_hash_ch}
  ${tex_miktex_hyph_ch}
  ${tex_miktex_pool_ch}
  ${tex_miktex_quiet_ch}
  ${tex_miktex_src_ch}
  ${miktex_synctex_changefiles}
  ${tex_miktex_stat_ch}
  ${tex_miktex_write18_ch}
)

set(pdftex_web_file ${CMAKE_CURRENT_SOURCE_DIR}/pdftex.web)
set(pdftex_change_file ${CMAKE_CURRENT_BINARY_DIR}/pdftex-miktex-tied.ch)
set(pdftex_header_file ${CMAKE_CURRENT_BINARY_DIR}/pdftexd.h)

add_custom_command(
  OUTPUT ${pdftex_change_file}
  COMMAND ${tie_exe} -c ${pdftex_change_file}
		${pdftex_web_file}
		${pdftex_ch_synctex}
		${etex_miktex_pre_ch}
		${miktex_tex_change_files}
#		${etex_miktex_post_ch}
		${etex_miktex_ch}
		${CMAKE_CURRENT_SOURCE_DIR}/pdftex-post.ch
		${CMAKE_CURRENT_SOURCE_DIR}/pdftex-miktex.ch
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${tie_exe}
	${pdftex_web_file}
	${etex_miktex_pre_ch}
	${miktex_tex_change_files}
#	${etex_miktex_post_ch}
	${CMAKE_CURRENT_SOURCE_DIR}/pdftex-post.ch
	${CMAKE_CURRENT_SOURCE_DIR}/pdftex-miktex.ch
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/pdftex_pool.cpp
  COMMAND
	${inipool_exe} 
		${CMAKE_CURRENT_BINARY_DIR}/pdftex.pool
		pdftex-miktex.h
		> ${CMAKE_CURRENT_BINARY_DIR}/pdftex_pool.cpp
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${inipool_exe}
	${CMAKE_CURRENT_BINARY_DIR}/pdftex.pool
)

file(READ ${tex_dyn_sed} tex_dyn_sed_contents)
file(READ ${etex_dyn_sed} etex_dyn_sed_contents)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed "${tex_dyn_sed_contents}")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed "${etex_dyn_sed_contents}")

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/openbsd-compat.h "
#if defined(MIKTEX_WINDOWS)
#define MIKTEX_UTF8_WRAP_ALL 1
#include <miktex/utf8wrap.h>
#define snprintf _snprintf
#if ! defined(__GNUC__)
#  define __attribute__(x)
#endif
#endif
")

set_source_files_properties(
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex.c
  avl.c
  avlstuff.c
  epdf.c
  mapfile.c
  pkin.c
  subfont.c
  tounicode.c
  utils.c
  vfpacket.c
  writeenc.c
  writefont.c
  writeimg.c
  writejbig2.c
  writejpg.c
  writepng.c
  writet1.c
  writet3.c
  writettf.c
  writezip.c
  PROPERTIES LANGUAGE CXX
)

if(LINK_EVERYTHING_STATICALLY)
  set(pdftex_target_name ${pdftex_lib_name})
else()
  set(pdftex_target_name ${pdftex_dll_name})
endif()

set(${pdftex_target_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/pdftex_pool.cpp
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-common.h
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-pdftex.h
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex.c
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex.h
  avl.c
  avl.h
  avlstuff.c
  avlstuff.h
  c4p_pre.h
  epdf.c
  image.h
  mapfile.c
  pdftex-common.h
  pdftex-miktex.h
  pdftex-version.h
  pdftex.defaults.h
  pdftex.h
  pdftoepdf.cc
  pkin.c
  ptexlib.h
  ptexmac.h
  subfont.c
  tounicode.c
  utils.c
  vfpacket.c
  writeenc.c
  writefont.c
  writeimg.c
  writejbig2.c
  writejpg.c
  writepng.c
  writet1.c
  writet3.c
  writettf.c
  writettf.h
  writezip.c
)

set_source_files_properties(
  ${${pdftex_target_name}_sources}
  PROPERTIES COMPILE_FLAGS "-DC4PEXTERN=extern"
)

create_web_app(pdfTeX pdfTeX pdftex)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/pdftex.cc
  PROPERTIES COMPILE_FLAGS "-DCOMPILING_PDFTEX_CC"
)

if(LINK_EVERYTHING_STATICALLY)
  target_link_libraries(${miktex_prefix}pdftex
    ${jpeg_dll_name}
    ${kpsemu_lib_name}
    ${md5_lib_name}
    ${png_lib_name}
    ${poppler_lib_name}
    ${unxemu_dll_name}
    ${w2cemu_lib_name}
  )
else()
  target_link_libraries(${pdftex_dll_name}
    PRIVATE
      ${jpeg_dll_name}
      ${kpsemu_dll_name}
      ${md5_dll_name}
      ${png_dll_name}
      ${poppler_lib_name}
      ${unxemu_dll_name}
      ${utf8wrap_dll_name}
      ${w2cemu_dll_name}
  )
endif()

if(NOT LINK_EVERYTHING_STATICALLY)
  if(NATIVE_WINDOWS)
    target_link_libraries(${pdftex_target_name}
      PRIVATE
        ${regex_dll_name}
	${zlib_dll_name}
    )
  else(NATIVE_WINDOWS)
    target_link_libraries(${pdftex_target_name}
      PRIVATE
        ${zlib_lib_name}
    )
  endif()
endif()

if(NOT LINK_EVERYTHING_STATICALLY)
  delay_load(${pdftex_target_name}
    ${png_dll_name}
    ${regex_dll_name}
  )
endif()

## dev targets

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/pdftex-tied.web
	${CMAKE_CURRENT_BINARY_DIR}/pdftex-tied-n.web
  COMMAND ${tie_exe} -m ${CMAKE_CURRENT_BINARY_DIR}/pdftex-tied.web
		${pdftex_web_file}
		${pdftex_ch_synctex}
		${etex_miktex_pre_ch}
		${miktex_tex_change_files}
#		${etex_miktex_post_ch}
		${etex_miktex_ch}
		${CMAKE_CURRENT_SOURCE_DIR}/pdftex-post.ch
  COMMAND ${webn_exe}
	< ${CMAKE_CURRENT_BINARY_DIR}/pdftex-tied.web
	> ${CMAKE_CURRENT_BINARY_DIR}/pdftex-tied-n.web
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${tie_exe}
    ${webn_exe}
	${pdftex_web_file}
	${etex_miktex_pre_ch}
	${miktex_tex_change_files}
#	${etex_miktex_pre_ch}
	${CMAKE_CURRENT_SOURCE_DIR}/pdftex-post.ch
  VERBATIM
)

add_custom_target(pdftex-dev ALL
    DEPENDS
	${pdftex_change_file}
	${CMAKE_CURRENT_BINARY_DIR}/pdftex-tied-n.web
)
