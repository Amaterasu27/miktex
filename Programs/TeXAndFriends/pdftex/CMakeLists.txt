## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2009 Christian Schenk
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
endif(LINK_EVERYTHING_STATICALLY)

default_char_type_is_unsigned()

hide_symbols()

set(C4P_FLAGS
  --auto-exit=10
  --chars-are-unsigned
)

include_directories(BEFORE
  ${CMAKE_BINARY_DIR}/${rel_xpdf_dir}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_goo_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_xpdf_dir}
)

add_definitions(
  -DMIKTEX_PDFTEX
  -DPDFTEXAPP=g_PDFTEXApp
  -DPDFTEXCLASS=PDFTEX
  -DpdfTeX
)

if(WITH_SYNCTEX)
  add_definitions(-D__SyncTeX__)
endif(WITH_SYNCTEX)

set(synctex_pdftex_ch_srcs_mem_only
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-mem.ch0
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-mem.ch1
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-e-mem.ch0
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-e-mem.ch1
)

if(WITH_SYNCTEX)
  set(synctex_pdftex_ch_srcs_on
    ${synctex_pdftex_ch_srcs_mem_only}
    ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-rec.ch0
    ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-rec.ch1
    ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-e-rec.ch0
    ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-e-rec.ch1
    ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-pdf-rec.ch2
  )
endif(WITH_SYNCTEX)

set(pdftosrc_sources
  pdftosrc.cc
  pdftosrc-version.h
)

if(NATIVE_WINDOWS)
  set(pdftosrc_sources ${pdftosrc_sources}
    pdftosrc.rc
  )
endif(NATIVE_WINDOWS)

if(BUILDING_MIKTEX)
  add_executable(pdftosrc ${pdftosrc_sources})
  merge_trustinfo_manifest(pdftosrc asInvoker)
  target_link_libraries(pdftosrc
    ${xpdf_lib_name}
    ${fofi_lib_name}
    ${goo_lib_name}
    ${app_dll_name}
    ${core_dll_name}
  )
  if(NATIVE_WINDOWS)
    target_link_libraries(pdftosrc
      ${getopt_dll_name}
    )
  endif(NATIVE_WINDOWS)
  install(TARGETS pdftosrc DESTINATION ${bindir})
endif(BUILDING_MIKTEX)

create_cpp_from_c(
  ${CMAKE_CURRENT_SOURCE_DIR}/ttf2afm.c
)

set(ttf2afm_sources
  ${wrapper_cpp}
  ttf2afm-version.h
  ${CMAKE_CURRENT_BINARY_DIR}/ttf2afm.cpp
)

set_source_files_properties(
  ${wrapper_cpp}
  PROPERTIES COMPILE_FLAGS "-DCPLUSPLUSMAIN"
)

if(NATIVE_WINDOWS)
  set(ttf2afm_sources ${ttf2afm_sources}
    ttf2afm.rc
  )
endif(NATIVE_WINDOWS)

if(BUILDING_MIKTEX)
  add_executable(ttf2afm ${ttf2afm_sources})
  merge_trustinfo_manifest(ttf2afm asInvoker)
  target_link_libraries(ttf2afm
    ${app_dll_name}
    ${core_dll_name}
    ${kpsemu_dll_name}
    ${texmf_dll_name}
  )
  if(NATIVE_WINDOWS)
    target_link_libraries(ttf2afm
      ${getopt_dll_name}
    )
  endif(NATIVE_WINDOWS)
  install(TARGETS ttf2afm DESTINATION ${bindir})
endif(BUILDING_MIKTEX)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pdftex.defaults.h
  COMMAND ${cfg_exe} --print-classes ${pdftex_defaults} > pdftex.defaults.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${pdftex_defaults}
  VERBATIM
)

if(WITH_SYNCTEX)
  set(miktex_synctex_changefiles ${tex_miktex_synctex_ch})
endif(WITH_SYNCTEX)

set(miktex_tex_change_files
  ${mltex_miktex_ch}
  ${enctex_miktex_ch}
  ${tex_miktex_ch}
  ${tex_miktex_hash_ch}
  ${tex_miktex_hyph_ch}
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
		${synctex_pdftex_ch_srcs_on}
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
#include <direct.h>
#include <sys/stat.h>
#define stat _stat
#define getcwd _getcwd
#define snprintf _snprintf
#if ! defined(__GNUC__)
#  define __attribute__(x)
#endif
#endif
")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/sysexits.h "")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/md5.h "#include <miktex/Core/MD5>
typedef unsigned char md5_byte_t;
typedef MiKTeX::Core::MD5_CTX md5_state_t;
#define md5_init(pstate) MiKTeX::Core::MD5Init(pstate)
#define md5_append(pstate, pv, size) MiKTeX::Core::MD5Update(pstate, pv, size)
#define md5_finish(pstate, pdigest) MiKTeX::Core::MD5Final(pdigest, pstate)
")

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/synctex-pdftex.cpp
  COMMAND ${SED_EXE}
		s/TEX-OR-MF-OR-MP/pdftex/
		${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex.c
		> ${CMAKE_CURRENT_BINARY_DIR}/synctex-pdftex.cpp
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex.c
  VERBATIM
)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/synctex-pdftex.cpp
  PROPERTIES GENERATED TRUE
)

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex.h
  ${CMAKE_CURRENT_BINARY_DIR}/synctex-xe.h
)

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-pdftex.h
  ${CMAKE_CURRENT_BINARY_DIR}/synctex-pdftex.h
)

create_cpp_from_c(
  ${CMAKE_CURRENT_SOURCE_DIR}/avl.c
  ${CMAKE_CURRENT_SOURCE_DIR}/avlstuff.c
  ${CMAKE_CURRENT_SOURCE_DIR}/epdf.c
  ${CMAKE_CURRENT_SOURCE_DIR}/mapfile.c
  ${CMAKE_CURRENT_SOURCE_DIR}/pkin.c
  ${CMAKE_CURRENT_SOURCE_DIR}/subfont.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tounicode.c
  ${CMAKE_CURRENT_SOURCE_DIR}/utils.c
  ${CMAKE_CURRENT_SOURCE_DIR}/vfpacket.c
  ${CMAKE_CURRENT_SOURCE_DIR}/writeenc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/writefont.c
  ${CMAKE_CURRENT_SOURCE_DIR}/writeimg.c
  ${CMAKE_CURRENT_SOURCE_DIR}/writejbig2.c
  ${CMAKE_CURRENT_SOURCE_DIR}/writejpg.c
  ${CMAKE_CURRENT_SOURCE_DIR}/writepng.c
  ${CMAKE_CURRENT_SOURCE_DIR}/writet1.c
  ${CMAKE_CURRENT_SOURCE_DIR}/writet3.c
  ${CMAKE_CURRENT_SOURCE_DIR}/writettf.c
  ${CMAKE_CURRENT_SOURCE_DIR}/writezip.c
)

if(LINK_EVERYTHING_STATICALLY)
  set(pdftex_target_name ${pdftex_lib_name})
else(LINK_EVERYTHING_STATICALLY)
  set(pdftex_target_name ${pdftex_dll_name})
endif(LINK_EVERYTHING_STATICALLY)

set(${pdftex_target_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/avl.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/avlstuff.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/epdf.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/mapfile.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/pdftex.defaults.h
  ${CMAKE_CURRENT_BINARY_DIR}/pdftex_pool.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/pkin.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/subfont.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/tounicode.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/utils.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/vfpacket.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writeenc.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writefont.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writeimg.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writejbig2.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writejpg.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writepng.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writet1.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writet3.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writettf.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writezip.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/pdftoepdf.cc
)

if(WITH_SYNCTEX)
  set(${pdftex_target_name}_sources
    ${${pdftex_target_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/synctex-pdftex.cpp
  )
endif(WITH_SYNCTEX)

set_source_files_properties(
  ${${pdftex_target_name}_sources}
  PROPERTIES COMPILE_FLAGS "-DC4PEXTERN=extern"
)

create_web_app(pdfTeX pdfTeX pdftex FALSE)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/pdftex.cc
  PROPERTIES COMPILE_FLAGS "-DCOMPILING_PDFTEX_CC"
)

if(LINK_EVERYTHING_STATICALLY)
  target_link_libraries(${miktex_prefix}pdftex
    ${xpdf_lib_name}
      ${fofi_lib_name}
      ${goo_lib_name}
    ${kpsemu_lib_name}
    ${png_lib_name}
  )
else(LINK_EVERYTHING_STATICALLY)
  target_link_libraries(${pdftex_dll_name}
    ${fofi_lib_name}
    ${goo_lib_name}
    ${kpsemu_dll_name}
    ${png_dll_name}
    ${xpdf_lib_name}
  )
endif(LINK_EVERYTHING_STATICALLY)

if(NOT LINK_EVERYTHING_STATICALLY)
  if(NATIVE_WINDOWS)
    target_link_libraries(${pdftex_target_name}
      ${regex_dll_name}
      ${zlib_dll_name}
      )
  else(NATIVE_WINDOWS)
    target_link_libraries(${pdftex_target_name}
      ${zlib_lib_name}
      )
  endif(NATIVE_WINDOWS)
endif(NOT LINK_EVERYTHING_STATICALLY)

if(NOT LINK_EVERYTHING_STATICALLY)
  delay_load(${pdftex_target_name}
    ${png_dll_name}
    ${regex_dll_name}
  )
endif(NOT LINK_EVERYTHING_STATICALLY)

## dev targets

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/pdftex-tied.web
	${CMAKE_CURRENT_BINARY_DIR}/pdftex-tied-n.web
  COMMAND ${tie_exe} -m ${CMAKE_CURRENT_BINARY_DIR}/pdftex-tied.web
		${pdftex_web_file}
		${synctex_pdftex_ch_srcs_on}
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
