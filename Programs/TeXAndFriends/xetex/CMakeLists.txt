## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2007 Christian Schenk
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
  --declare-c-type=realpoint
  --declare-c-type=realrect
  --declare-c-type=transform
  --declare-c-type=unicodefile
  --declare-c-type=voidpointer
)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_icu_dir}/include/layout
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_goo_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_xpdf_dir}
)

add_definitions(
  -DHAVE_C4P_PRE_H
  -DLE_USE_CMEMORY
  -DMIKTEX_XETEX
  -DXETEXAPP=g_XETEXApp
  -DXETEXCLASS=XETEX
  -DXETEXDATA=g_XETEXData
  -DXETEX_OTHER
  -DXeTeX
)

#add_custom_command(
#  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/xetex.defaults.h
#  COMMAND ${cfg_exe} --print-classes ${xetex_defaults} > xetex.defaults.h
#  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#  DEPENDS ${xetex_defaults}
#  VERBATIM
#)

set(miktex_tex_change_files
  ${mltex_miktex_ch}
  ${enctex_miktex_ch}
  ${tex_miktex_ch}
  ${tex_miktex_hyph_ch}
  ${tex_miktex_quiet_ch}
  ${tex_miktex_src_ch}
  ${tex_miktex_stat_ch}
  ${tex_miktex_write18_ch}
  ${tex_miktex_web2c_ch}
)

set(pre_xetex_change_files
  ${etex_ch}
  ${etex_miktex_pre_ch}
  ${etex_miktex_ch}
  ${miktex_tex_change_files}
)

set(pre_xetex1_web_file ${CMAKE_CURRENT_BINARY_DIR}/pre-xetex1.web)

add_custom_command(
  OUTPUT
	${pre_xetex1_web_file}
  COMMAND
	${tie_exe} -m ${pre_xetex1_web_file}
		${tex_web}
		${pre_xetex_change_files}
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${tex_web}
	${pre_xetex_change_files}
)

set(pre_xetex2_web_file ${CMAKE_CURRENT_BINARY_DIR}/pre-xetex2.web)

add_custom_command(
  OUTPUT
	${pre_xetex2_web_file}
  COMMAND
	${tie_exe} -m ${pre_xetex2_web_file}
		${pre_xetex1_web_file}
		${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-2.ch
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${pre_xetex1_web_file}
	${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-2.ch
)

set(xetex_change_files
  ${CMAKE_CURRENT_SOURCE_DIR}/xetex.ch
)

set(xetex_web_file ${CMAKE_CURRENT_BINARY_DIR}/xetex.web)
set(xetex_change_file ${CMAKE_CURRENT_SOURCE_DIR}/xetex-miktex.ch)

set(xetex_header_file ${CMAKE_CURRENT_BINARY_DIR}/xetexd.h)

add_custom_command(
  OUTPUT
	${xetex_web_file}
  COMMAND
	${tie_exe} -m ${xetex_web_file}
		${pre_xetex2_web_file}
		${xetex_change_files}
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${pre_xetex2_web_file}
	${xetex_change_files}
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/xetex_pool.cpp
  COMMAND
	perl ${CMAKE_CURRENT_SOURCE_DIR}/pool2c.pl
		${CMAKE_CURRENT_BINARY_DIR}/xetex.pool
		${CMAKE_CURRENT_BINARY_DIR}/xetex_pool.cpp
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/xetex.pool
)

file(READ ${tex_dyn_sed} tex_dyn_sed_contents)
file(READ ${etex_dyn_sed} etex_dyn_sed_contents)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed "${tex_dyn_sed_contents}")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed "${etex_dyn_sed_contents}")

create_cpp_from_c(
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeX_ext.c
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeX_pic.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bmpimage.c
  ${CMAKE_CURRENT_SOURCE_DIR}/jpegimage.c
  ${CMAKE_CURRENT_SOURCE_DIR}/mfileio.c
  ${CMAKE_CURRENT_SOURCE_DIR}/numbers.c
  ${CMAKE_CURRENT_SOURCE_DIR}/pngimage.c
  ${CMAKE_CURRENT_SOURCE_DIR}/trans.c
)

set(${xetex_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/XeTeX_ext.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/XeTeX_pic.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/bmpimage.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/jpegimage.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/mfileio.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/numbers.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/pngimage.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/trans.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/xetex_pool.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/FontTableCache.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeXFontInst.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeXFontInst_FT2.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeXFontMgr.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeXFontMgr_FC.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeXGrLayout.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeXLayoutInterface.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeXOTLayoutEngine.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeXOTMath.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cmaps.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/pdfimage.cpp
)

set_source_files_properties(
  ${${xetex_dll_name}_sources}
  PROPERTIES COMPILE_FLAGS "-DC4PEXTERN=extern"
)

set(TANGLE_FLAGS -omega)

create_web_app(XeTeX)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/xetex.cc
  PROPERTIES COMPILE_FLAGS "-DCOMPILING_XETEX_CC"
)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/xetex.cc
  PROPERTIES COMPILE_FLAGS "-DCOMPILING_XETEX_CC"
)

target_link_libraries(${xetex_dll_name}
  ${fofi_lib_name}
  ${fontconfig_dll_name}
  ${freetype2_dll_name}
  ${goo_lib_name}
  ${graphite_engine_lib_name}
  ${icudt_dll_name}
  ${icule_dll_name}
  ${icuuc_dll_name}
  ${kpsemu_dll_name}
  ${png_dll_name}
  ${teckit_dll_name}
  ${xpdf_lib_name}
)

delay_load(${xetex_dll_name}
  ${fontconfig_dll_name}
  ${freetype2_dll_name}
#  icudt36
#  icule36
#  icuuc36
  ${png_dll_name}
  ${teckit_dll_name}
)

## dev targets

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/etex-miktex.web
	${CMAKE_CURRENT_BINARY_DIR}/etex-miktex-n.web
  COMMAND ${tie_exe} -m ${CMAKE_CURRENT_BINARY_DIR}/etex-miktex.web
		${tex_web}
		${etex_ch}
		${etex_miktex_pre_ch}
		${etex_miktex_ch}
		${miktex_tex_change_files}
  COMMAND ${webn_exe}
	< ${CMAKE_CURRENT_BINARY_DIR}/etex-miktex.web
	> ${CMAKE_CURRENT_BINARY_DIR}/etex-miktex-n.web
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${tex_web}
	${etex_ch}
	${etex_miktex_pre_ch}
	${etex_miktex_ch}
	${miktex_tex_change_files}
  VERBATIM
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/pre-xetex1-n.web
  COMMAND ${webn_exe}
	< ${pre_xetex1_web_file}
	> ${CMAKE_CURRENT_BINARY_DIR}/pre-xetex1-n.web
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${pre_xetex1_web_file}
  VERBATIM
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/pre-xetex2-n.web
  COMMAND ${webn_exe}
	< ${pre_xetex2_web_file}
	> ${CMAKE_CURRENT_BINARY_DIR}/pre-xetex2-n.web
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${pre_xetex2_web_file}
  VERBATIM
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/xetex-n.web
  COMMAND ${webn_exe}
	< ${xetex_web_file}
	> ${CMAKE_CURRENT_BINARY_DIR}/xetex-n.web
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${xetex_web_file}
  VERBATIM
)

add_custom_target(dev ALL
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/etex-miktex-n.web
	${CMAKE_CURRENT_BINARY_DIR}/pre-xetex1-n.web
	${CMAKE_CURRENT_BINARY_DIR}/pre-xetex2-n.web
	${CMAKE_CURRENT_BINARY_DIR}/xetex-n.web
)
