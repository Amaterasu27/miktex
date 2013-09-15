## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2008-2013 Christian Schenk
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
  ${CMAKE_SOURCE_DIR}/${rel_graphite2_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_include_dir}
  ${CMAKE_SOURCE_DIR}/${rel_icu_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}
  ${CMAKE_BINARY_DIR}/${rel_poppler_dir}/poppler
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler
  ${CMAKE_BINARY_DIR}/${rel_poppler_dir}
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}
)

add_definitions(
  -DHAVE_C4P_PRE_H
  -DLE_USE_CMEMORY
  -DMIKTEX_XETEX
  -D__SyncTeX__
  -DSYNCTEX_ENGINE_H="synctex-xetex.h"
  -DXETEXAPP=g_XETEXApp
  -DXETEXCLASS=XETEX
  -DXETEX_OTHER
  -DXeTeX
  -DPOPPLER_VERSION="${POPPLER_VERSION}"
)

set(xetex_ch_synctex
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-xe-def.ch0
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-mem.ch0
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-e-mem.ch0
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-e-mem.ch1
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-rec.ch0
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-e-rec.ch0
)

set(xetex_post_ch_synctex
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-xe-rec.ch3
)

set(miktex_tex_change_files
  ${mltex_miktex_ch}
  ${tex_miktex_ch}
  ${tex_miktex_hash_ch}
  ${tex_miktex_hyph_ch}
  ${tex_miktex_quiet_ch}
  ${tex_miktex_src_ch}
  ${tex_miktex_synctex_ch}
  ${tex_miktex_stat_ch}
  ${tex_miktex_write18_ch}
  ${tex_miktex_web2c_ch}
  ${tex_miktex_pool_ch}
)

set(pre_xetex_change_files
  ${xetex_ch_synctex}
  ${etex_miktex_pre_ch}
  ${etex_miktex_ch}
  ${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-0.ch
  ${miktex_tex_change_files}
)

set(pre_xetex1_web_file ${CMAKE_CURRENT_BINARY_DIR}/pre-xetex1.web)

add_custom_command(
  OUTPUT
	${pre_xetex1_web_file}
  COMMAND
	${tie_exe} -m ${pre_xetex1_web_file}
		${CMAKE_CURRENT_SOURCE_DIR}/xetex.web
		${pre_xetex_change_files}
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${tie_exe}
	${CMAKE_CURRENT_SOURCE_DIR}/xetex.web
	${pre_xetex_change_files}
  VERBATIM
)

set(pre_xetex2_web_file ${CMAKE_CURRENT_BINARY_DIR}/pre-xetex2.web)

add_custom_command(
  OUTPUT
	${pre_xetex2_web_file}
  COMMAND
	${tie_exe} -m ${pre_xetex2_web_file}
		${pre_xetex1_web_file}
  		${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-1.ch
		${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-2.ch
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${tie_exe}
	${pre_xetex1_web_file}
	${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-1.ch
	${CMAKE_CURRENT_SOURCE_DIR}/miktex-adapter-2.ch
  VERBATIM
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
		${CMAKE_CURRENT_SOURCE_DIR}/xetex.ch
		${xetex_post_ch_synctex}
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${tie_exe}
	${pre_xetex2_web_file}
	${CMAKE_CURRENT_SOURCE_DIR}/xetex.ch
	${xetex_post_ch_synctex}
  VERBATIM
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/xetex_pool.cpp
  COMMAND
	${inipool_exe} 
		${CMAKE_CURRENT_BINARY_DIR}/xetex.pool
		xetex-miktex.h
		> ${CMAKE_CURRENT_BINARY_DIR}/xetex_pool.cpp
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
        ${inipool_exe}
	${CMAKE_CURRENT_BINARY_DIR}/xetex.pool
)

file(READ ${tex_dyn_sed} tex_dyn_sed_contents)
file(READ ${etex_dyn_sed} etex_dyn_sed_contents)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed "${tex_dyn_sed_contents}")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed "${etex_dyn_sed_contents}")

set_source_files_properties(
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex.c
  XeTeX_ext.c
  XeTeX_pic.c
  image/bmpimage.c
  image/jpegimage.c
  image/mfileio.c
  image/numbers.c
  image/pngimage.c
  trans.c
  PROPERTIES LANGUAGE CXX
)

set(${xetex_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/xetex_pool.cpp
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-common.h
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-xetex.h
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex.c
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex.h
  MathTable.h
  XeTeXFontInst.cpp
  XeTeXFontInst.h
  XeTeXFontMgr.cpp
  XeTeXFontMgr.h
  XeTeXFontMgr_FC.cpp
  XeTeXFontMgr_FC.h
  XeTeXLayoutInterface.cpp
  XeTeXLayoutInterface.h
  XeTeXOTMath.cpp
  XeTeXOTMath.h
  XeTeX_ext.c
  XeTeX_ext.h
  XeTeX_pic.c
  XeTeXswap.h
  c4p_pre.h
  etex_version.h
  hz.cpp
  image/bmpimage.c
  image/jpegimage.c
  image/mfileio.c
  image/numbers.c
  image/pngimage.c
  pdfimage.cpp
  pdfimage.h
  trans.c
  trans.h
  xetex-miktex.h
  xetex-version.h
  xetex.h
  xetex_version.h
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
  ${fontconfig_dll_name}
  ${freetype2_dll_name}
  ${graphite2_dll_name}
  ${harfbuzz_lib_name}
  ${icudt_dll_name}
  ${icule_dll_name}
  ${icuuc_dll_name}
  ${jpeg_dll_name}
  ${kpsemu_dll_name}
  ${png_dll_name}
  ${poppler_lib_name}
  ${teckit_dll_name}
  ${unxemu_dll_name}
  ${utf8wrap_dll_name}
  ${w2cemu_dll_name}
  ${zlib_dll_name}
)

delay_load(${xetex_dll_name}
  ${fontconfig_dll_name}
  ${freetype2_dll_name}
  ${graphite2_dll_name}
#  icudt
#  icule
#  icuuc
  ${jpeg_dll_name}
#  ${kpsemu_dll_name}
  ${png_dll_name}
  ${teckit_dll_name}
  ${unxemu_dll_name}
  ${w2cemu_dll_name}
)

## dev targets

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/xetex-n.web
  COMMAND ${webn_exe}
	< ${CMAKE_CURRENT_SOURCE_DIR}/xetex.web
	> ${CMAKE_CURRENT_BINARY_DIR}/xetex-n.web
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${webn_exe}
        ${CMAKE_CURRENT_SOURCE_DIR}/xetex.web
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
    ${webn_exe}
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
    ${webn_exe}
	${pre_xetex2_web_file}
  VERBATIM
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/xetex-final-n.web
  COMMAND ${webn_exe}
	< ${xetex_web_file}
	> ${CMAKE_CURRENT_BINARY_DIR}/xetex-final-n.web
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${webn_exe}
	${xetex_web_file}
  VERBATIM
)

add_custom_target(xetex-dev ALL
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/xetex-n.web
	${CMAKE_CURRENT_BINARY_DIR}/pre-xetex1-n.web
	${CMAKE_CURRENT_BINARY_DIR}/pre-xetex2-n.web
	${CMAKE_CURRENT_BINARY_DIR}/xetex-final-n.web
)
