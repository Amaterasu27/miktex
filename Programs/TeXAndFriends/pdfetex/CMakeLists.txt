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
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_etex_dir}
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_fofi_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_goo_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_xpdf_dir}
)

add_definitions(
  -DpdfTeX
  -DpdfeTeX
  -DETEXCLASS=eTeXApp
  -DPDFTEXCLASS=pdfTeXApp
  -DPDFETEXDATA=g_PDFETEXData
  -DMIKTEX_PDFETEX
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pdftex.defaults.h
  COMMAND ${cfg_exe} --print-classes ${pdftex_defaults} > pdftex.defaults.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${pdftex_defaults}
  VERBATIM
)

set(tex_change_files
  ${etex_ch}
  ${CMAKE_CURRENT_SOURCE_DIR}/pdfetex.ch1
  ${pdftex_ch}
  ${hz_ch}
  ${misc_ch}
  ${vadjust_ch}
  ${pdftex2_ch}
  ${CMAKE_CURRENT_SOURCE_DIR}/pdfetex.ch2
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pdfetex0.web
  COMMAND ${tie_exe}
	-m ${CMAKE_CURRENT_BINARY_DIR}/pdfetex0.web
	${tex_web}
	${tex_change_files}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${tex_web} ${tex_change_files}
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

set(miktex_etex_change_files
  ${etex_miktex_ch}
)

set(pdfetex0_change_files
  ${pdftex_pre_ch}
  ${miktex_tex_change_files}
  ${miktex_etex_change_files}
  ${pdftex_post_ch}
  ${noligatures_ch}
  ${pdfstrcmp_ch}
  ${randoms_ch}
  ${pdftex_miktex_ch}
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pdfetex.web
  COMMAND ${tie_exe}
	-m ${CMAKE_CURRENT_BINARY_DIR}/pdfetex.web
	${CMAKE_CURRENT_BINARY_DIR}/pdfetex0.web
	${pdfetex0_change_files}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
 	${CMAKE_CURRENT_BINARY_DIR}/pdfetex0.web
	${pdfetex0_change_files}
  VERBATIM
)

create_cpp_from_c(
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/avl.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/avlstuff.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/epdf.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/mapfile.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/papersiz.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/pkin.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/utils.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/vfpacket.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/writeenc.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/writefont.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/writeimg.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/writejpg.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/writepng.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/writet1.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/writet3.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/writettf.c
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/writezip.c
)

set(${pdfetex_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/avl.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/avlstuff.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/epdf.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/mapfile.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/papersiz.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/pdftex.defaults.h
  ${CMAKE_CURRENT_BINARY_DIR}/pkin.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/utils.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/vfpacket.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writeenc.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writefont.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writeimg.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writejpg.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writepng.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writet1.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writet3.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writettf.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writezip.cpp
  ${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/pdftoepdf.cc
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

set(pdfetex_web_file ${CMAKE_CURRENT_BINARY_DIR}/pdfetex.web)
set(pdfetex_header_file ${CMAKE_CURRENT_BINARY_DIR}/pdfetexd.h)

create_web_app(pdfeTeX)

target_link_libraries(${pdfetex_dll_name}
  ${fofi_lib_name}
  ${goo_lib_name}
  ${kpsemu_dll_name}
  ${png_dll_name}
  ${regex_dll_name}
  ${xpdf_lib_name}
)