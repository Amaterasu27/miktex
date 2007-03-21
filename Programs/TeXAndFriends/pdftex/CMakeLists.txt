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

default_char_type_is_unsigned()

set(C4P_FLAGS
  --auto-exit=10
  --chars-are-unsigned
)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_goo_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_xpdf_dir}
)

add_definitions(
  -DpdfTeX
  -DPDFTEXCLASS=pdfTeXApp
  -DPDFTEXDATA=g_PDFTEXData
  -DMIKTEX_PDFTEX
)

set(pdftosrc_sources
  pdftosrc.cc
  pdftosrc-version.h
)

if(NATIVE_WINDOWS)
  set(pdftosrc_sources ${pdftosrc_sources}
    pdftosrc.rc
  )
endif(NATIVE_WINDOWS)

add_executable(pdftosrc ${pdftosrc_sources})

merge_trustinfo_manifest(pdftosrc asInvoker)

target_link_libraries(pdftosrc
  ${app_dll_name}
  ${core_dll_name}
  ${fofi_lib_name}
  ${gnu_dll_name}
  ${goo_lib_name}
  ${xpdf_lib_name}
)

install(TARGETS pdftosrc DESTINATION ${bindir})

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

add_executable(ttf2afm ${ttf2afm_sources})

merge_trustinfo_manifest(ttf2afm asInvoker)

target_link_libraries(ttf2afm
  ${app_dll_name}
  ${core_dll_name}
  ${gnu_dll_name}
  ${kpsemu_dll_name}
  ${texmf_dll_name}
)

install(TARGETS ttf2afm DESTINATION ${bindir})

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pdftex.defaults.h
  COMMAND ${cfg_exe} --print-classes ${pdftex_defaults} > pdftex.defaults.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${pdftex_defaults}
  VERBATIM
)

set(miktex_tex_change_files
  ${tex_mltex_miktex_ch}
  ${tex_miktex_ch}
  ${tex_miktex_hyph_ch}
  ${tex_miktex_quiet_ch}
  ${tex_miktex_src_ch}
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
		${CMAKE_CURRENT_SOURCE_DIR}/pdftex-pre.ch
		${miktex_tex_change_files}
		${CMAKE_CURRENT_SOURCE_DIR}/pdftex-post.ch
		${CMAKE_CURRENT_SOURCE_DIR}/pdftex-miktex.ch
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${pdftex_web_file}
	${CMAKE_CURRENT_SOURCE_DIR}/pdftex-pre.ch
	${miktex_tex_change_files}
	${CMAKE_CURRENT_SOURCE_DIR}/pdftex-post.ch
	${CMAKE_CURRENT_SOURCE_DIR}/pdftex-miktex.ch
)

set(${pdftex_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/pdftex.defaults.h
)

set_source_files_properties(
  ${${pdfetex_dll_name}_sources}
  PROPERTIES COMPILE_FLAGS "-DC4PEXTERN=extern"
)

file(READ ${tex_dyn_sed} tex_dyn_sed_contents)
file(READ ${pdftex_dyn_sed} pdftex_dyn_sed_contents)
file(READ ${etex_dyn_sed} etex_dyn_sed_contents)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed "${tex_dyn_sed_contents}")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed "${pdftex_dyn_sed_contents}")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed "${etex_dyn_sed_contents}")

create_web_app(pdftex)


## test

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pdftex-tied-n.web
  COMMAND ${tie_exe} -m ${CMAKE_CURRENT_BINARY_DIR}/pdftex-tied.web
		${pdftex_web_file}
		${CMAKE_CURRENT_SOURCE_DIR}/pdftex-pre.ch
		${miktex_tex_change_files}
		${CMAKE_CURRENT_SOURCE_DIR}/pdftex-post.ch
  COMMAND ${webn_exe}
	< ${CMAKE_CURRENT_BINARY_DIR}/pdftex-tied.web
	> ${CMAKE_CURRENT_BINARY_DIR}/pdftex-tied-n.web
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${pdftex_web_file}
	${CMAKE_CURRENT_SOURCE_DIR}/pdftex-pre.ch
	${miktex_tex_change_files}
	${CMAKE_CURRENT_SOURCE_DIR}/pdftex-post.ch
  VERBATIM
)

add_custom_target(allx ALL
    DEPENDS
	${pdftex_change_file}
	${CMAKE_CURRENT_BINARY_DIR}/pdftex-tied-n.web
)
