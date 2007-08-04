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
  --declare-c-type=unicodefile
)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_definitions(
  -DXeTeX
  -DXETEXAPP=g_XETEXApp
  -DXETEXCLASS=XETEX
  -DXETEXDATA=g_XETEXData
  -DMIKTEX_XETEX
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
)

set(xetex_change_files
  ${CMAKE_CURRENT_SOURCE_DIR}/xetex-new.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/xetex-noenc.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/xetex-upwards.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/xetex-otmath.ch
)

set(change_files
  ${etex_ch}
  ${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-1.ch
  ${miktex_tex_change_files}
  ${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-2.ch
  ${xetex_change_files}
  ${CMAKE_CURRENT_SOURCE_DIR}/xetex-miktex.ch
)

set(xetex_web_file ${CMAKE_CURRENT_BINARY_DIR}/xetex.web)
set(xetex_header_file ${CMAKE_CURRENT_BINARY_DIR}/xetexd.h)

add_custom_command(
  OUTPUT ${xetex_web_file}
  COMMAND ${tie_exe} -m ${xetex_web_file} ${change_files}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${change_files}
)

file(READ ${tex_dyn_sed} tex_dyn_sed_contents)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed "${tex_dyn_sed_contents}")

create_cpp_from_c(
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeX_ext.c
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeX_pic.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bmpimage.c
  ${CMAKE_CURRENT_SOURCE_DIR}/jpegimage.c
  ${CMAKE_CURRENT_SOURCE_DIR}/mfileio.c
  ${CMAKE_CURRENT_SOURCE_DIR}/numbers.c
  ${CMAKE_CURRENT_SOURCE_DIR}/pdfimage.c
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
  ${CMAKE_CURRENT_BINARY_DIR}/pdfimage.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/pngimage.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/trans.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/FontTableCache.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeXFontInst.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeXFontInst_FT2.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeXFontMgr.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeXFontMgr_FC.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeXLayoutInterface.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeXOTLayoutEngine.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XeTeXOTMath.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cmaps.cpp
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

target_link_libraries(${xetex_dll_name}
  ${fofi_lib_name}
  ${goo_lib_name}
  ${kpsemu_dll_name}
  ${png_dll_name}
  ${regex_dll_name}
  ${xpdf_lib_name}
)

## dev targets

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/etex-miktex.web
	${CMAKE_CURRENT_BINARY_DIR}/etex-miktex-n.web
  COMMAND ${tie_exe} -m ${CMAKE_CURRENT_BINARY_DIR}/etex-miktex.web
		${tex_web}
		${etex_ch}
		${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-1.ch
		${miktex_tex_change_files}
  COMMAND ${webn_exe}
	< ${CMAKE_CURRENT_BINARY_DIR}/etex-miktex.web
	> ${CMAKE_CURRENT_BINARY_DIR}/etex-miktex-n.web
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${tex_web_file}
	${etex_ch}
	${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-1.ch
	${miktex_tex_change_files}
  VERBATIM
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/etex-miktex2.web
	${CMAKE_CURRENT_BINARY_DIR}/etex-miktex2-n.web
  COMMAND ${tie_exe} -m ${CMAKE_CURRENT_BINARY_DIR}/etex-miktex2.web
		${tex_web}
		${etex_ch}
		${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-1.ch
		${miktex_tex_change_files}
		${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-2.ch
  COMMAND ${webn_exe}
	< ${CMAKE_CURRENT_BINARY_DIR}/etex-miktex2.web
	> ${CMAKE_CURRENT_BINARY_DIR}/etex-miktex2-n.web
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${tex_web_file}
	${etex_ch}
	${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-1.ch
	${miktex_tex_change_files}
	${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-2.ch
  VERBATIM
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/xetex-pre.web
	${CMAKE_CURRENT_BINARY_DIR}/xetex-pre-n.web
  COMMAND ${tie_exe} -m ${CMAKE_CURRENT_BINARY_DIR}/xetex-pre.web
		${tex_web}
		${etex_ch}
		${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-1.ch
		${miktex_tex_change_files}
		${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-2.ch
		${xetex_change_files}
  COMMAND ${webn_exe}
	< ${CMAKE_CURRENT_BINARY_DIR}/xetex-pre.web
	> ${CMAKE_CURRENT_BINARY_DIR}/xetex-pre-n.web
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${tex_web_file}
	${etex_ch}
	${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-1.ch
	${miktex_tex_change_files}
	${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-2.ch
  VERBATIM
)

add_custom_target(dev ALL
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/etex-miktex-n.web
	${CMAKE_CURRENT_BINARY_DIR}/etex-miktex2-n.web
	${CMAKE_CURRENT_BINARY_DIR}/xetex-pre-n.web
)
