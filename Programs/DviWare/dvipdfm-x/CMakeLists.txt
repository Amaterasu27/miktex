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

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

add_definitions(-DHAVE_CONFIG_H)

add_definitions(-DUNICODE -D_UNICODE)

add_definitions(
  -DHAVE_BASENAME
)

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

set(libutil_sources
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
  agl.c
  agl.h
  bmpimage.c
  bmpimage.h
  cff_dict.h
  cff_limits.h
  cff_stdstr.h
  cff_types.h
  cid_basefont.h
  cidtype0.h
  cidtype2.h
  cmap.h
  cmap_p.h
  cmap_read.h
  cmap_write.c
  cmap_write.h
  cs_type2.c
  cs_type2.h
  dpxconf.c
  dpxconf.h
  dpxcrypt.c
  dpxcrypt.h
  dpxfile.c
  dpxfile.h
  dpxutil.c
  dpxutil.h
  dvi.h
  dvicodes.h
  dvipdfmx.h
  epdf.c
  epdf.h
  error.c
  error.h
  jpegimage.c
  jpegimage.h
  mem.c
  mem.h
  mfileio.c
  mfileio.h
  mpost.h
  numbers.c
  numbers.h
  otl_conf.c
  otl_conf.h
  otl_opt.c
  otl_opt.h
  pdfcolor.c
  pdfcolor.h
  pdfdev.h
  pdfdoc.h
  pdfdraw.c
  pdfdraw.h
  pdfencrypt.c
  pdfencrypt.h
  pdfencoding.c
  pdfencoding.h
  pdflimits.h
  pdfnames.h
  pdfobj.c
  pdfobj.h
  pdfparse.c
  pdfparse.h
  pdfresource.c
  pdfresource.h
  pdfximage.c
  pdfximage.h
  pkfont.h
  pngimage.c
  pngimage.h
  pst.c
  pst.h
  pst_obj.c
  pst_obj.h
  spc_color.c
  spc_color.h
  spc_dvips.c
  spc_dvips.h
  spc_html.c
  spc_html.h
  spc_misc.c
  spc_misc.h
  spc_pdfm.h
  spc_tpic.c
  spc_tpic.h
  spc_util.h
  spc_util.c
  spc_xtx.h
  specials.c
  specials.h
  subfont.c
  subfont.h
  system.h
  t1_char.h
  t1_load.h
  tfm.c
  tfm.h
  truetype.h
  tt_aux.h
  tt_glyf.h
  tt_gsub.h
  tt_post.h
  tt_table.h
  type0.h
  type1.h
  type1c.c
  type1c.h
  unicode.c
  unicode.h
  vf.c
  vf.h
  xbb.c
)

set(common_program_sources
  cff.c
  cff.h
  cff_dict.c
  cid.c
  cid.h
  cid_p.h
  cidtype0.c
  cidtype2.c
  cmap.c
  cmap_read.c
  dvi.c
  dvipdfmx.c
  fontmap.c
  fontmap.h
  mpost.c
  pdfdev.c
  pdfdoc.c
  pdffont.c
  pdffont.h
  pdfnames.c
  pkfont.c
  sfnt.c
  sfnt.h
  spc_pdfm.c
  spc_xtx.c
  t1_char.c
  t1_load.c
  truetype.c
  tt_aux.c
  tt_cmap.c
  tt_cmap.h
  tt_glyf.c
  tt_gsub.c
  tt_post.c
  tt_table.c
  type0.c
  type1.c
)

set(dvipdfmx_dll_sources ${libutil_sources} ${common_program_sources} dvipdfmx-version.h)

if(NATIVE_WINDOWS)
  set(dvipdfmx_dll_sources ${dvipdfmx_dll_sources}
    dvipdfmx.rc
  )
endif(NATIVE_WINDOWS)

add_library(
  ${dvipdfmx_dll_name}
  SHARED
  ${dvipdfmx_dll_sources}
)

install(TARGETS ${dvipdfmx_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}")

target_link_libraries(${dvipdfmx_dll_name}
  PRIVATE
    ${app_dll_name}
    ${core_dll_name}
    ${getopt_dll_name}
    ${kpsemu_dll_name}
    ${png_dll_name}
    ${texmf_dll_name}
    ${unxemu_dll_name}
    ${utf8wrap_dll_name}
    ${zlib_dll_name}
)

set(dvipdfmx_exes
  dvipdfm
  dvipdfmx
  ebb
  extractbb
  xbb
)

foreach(x ${dvipdfmx_exes})
  add_executable (${x} ${wrapper_cpp})
  target_link_libraries(${x}
    ${app_dll_name}
    ${core_dll_name}
    ${dvipdfmx_dll_name}
    ${utf8wrap_dll_name}
  )
  merge_trustinfo_manifest(${x} asInvoker)
  install(TARGETS ${x} DESTINATION ${bindir})
endforeach(x)

set(xdvipdfmx_sources ${libutil_sources} ${common_program_sources} dvipdfmx-version.h ${wrapper_cpp})

if(NATIVE_WINDOWS)
  set(xdvipdfmx_sources ${xdvipdfmx_sources}
    xdvipdfmx.rc
  )
endif(NATIVE_WINDOWS)

add_executable(xdvipdfmx ${xdvipdfmx_sources})

set_property(
  TARGET xdvipdfmx
  APPEND
  PROPERTY COMPILE_DEFINITIONS "XETEX")

merge_trustinfo_manifest(xdvipdfmx asInvoker)

target_link_libraries(xdvipdfmx
  PRIVATE
    ${app_dll_name}
    ${core_dll_name}
    ${fontconfig_dll_name}
    ${freetype2_dll_name}
    ${kpsemu_dll_name}
    ${png_dll_name}
    ${unxemu_dll_name}
    ${utf8wrap_dll_name}
    ${zlib_dll_name}
)

delay_load(xdvipdfmx
  ${fontconfig_dll_name}
  ${freetype2_dll_name}
  ${png_dll_name}
  ${zlib_dll_name}
)

install(TARGETS xdvipdfmx DESTINATION ${bindir})

set(dvipdft_sources
  dvipdft.cpp
  dvipdft-version.h
)

if(NATIVE_WINDOWS)
  set(dvipdft_sources ${dvipdft_sources}
    dvipdft.rc
  )
endif(NATIVE_WINDOWS)

add_executable(dvipdft ${dvipdft_sources})

merge_trustinfo_manifest(dvipdft asInvoker)

target_link_libraries(dvipdft
  ${app_dll_name}
  ${core_dll_name}
  ${kpsemu_dll_name}
  ${texmf_dll_name}
)

install(TARGETS dvipdft DESTINATION ${bindir})
