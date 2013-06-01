## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2013 Christian Schenk
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

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

set(xdvipdfmx_sources
  ${wrapper_cpp}
  agl.c
  bmpimage.c
  cff.c
  cff_dict.c
  cid.c
  cidtype0.c
  cidtype2.c
  cmap.c
  cmap_read.c
  cmap_write.c
  cs_type2.c
  dpxconf.c
  dpxcrypt.c
  dpxfile.c
  dpxutil.c
  dvi.c
  dvipdfmx.c
  epdf.c
  error.c
  fontmap.c
  jpegimage.c
  mem.c
  mfileio.c
  mpost.c
  numbers.c
  otl_conf.c
  otl_opt.c
  pdfcolor.c
  pdfdev.c
  pdfdoc.c
  pdfdraw.c
  pdfencoding.c
  pdfencrypt.c
  pdffont.c
  pdfnames.c
  pdfobj.c
  pdfparse.c
  pdfresource.c
  pdfximage.c
  pkfont.c
  pngimage.c
  pst.c
  pst_obj.c
  sfnt.c
  spc_color.c
  spc_dvips.c
  spc_html.c
  spc_misc.c
  spc_pdfm.c
  spc_tpic.c
  spc_util.c
  spc_xtx.c
  specials.c
  subfont.c
  t1_char.c
  t1_load.c
  tfm.c
  truetype.c
  tt_aux.c
  tt_cmap.c
  tt_glyf.c
  tt_gsub.c
  tt_post.c
  tt_table.c
  type0.c
  type1.c
  type1c.c
  unicode.c
  vf.c
  xbb.c
)

if(NATIVE_WINDOWS)
  set(xdvipdfmx_sources ${xdvipdfmx_sources}
    xdvipdfmx.rc
  )
endif(NATIVE_WINDOWS)

add_executable(xdvipdfmx ${xdvipdfmx_sources})

merge_trustinfo_manifest(xdvipdfmx asInvoker)

target_link_libraries(xdvipdfmx
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
