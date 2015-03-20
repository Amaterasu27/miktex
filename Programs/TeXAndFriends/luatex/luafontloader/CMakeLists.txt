## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2010-2015 Christian Schenk
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
  ${CMAKE_CURRENT_SOURCE_DIR}/fontforge/fontforge
  ${CMAKE_CURRENT_SOURCE_DIR}/fontforge/inc
  ${CMAKE_SOURCE_DIR}/${rel_luatex_dir}/unilib
)

add_definitions(
  -DFONTFORGE_CONFIG_NO_WINDOWING_UI=1
  -DLUA_FF_LIB=1
  -DNODYNAMIC=1
  -DUSE_OUR_MEMORY=1
  -DX_DISPLAY_MISSING=1
  -D_NO_FREETYPE=1
  -D_NO_LIBUNINAMESLIST=1
  -D_NO_PYTHON=1
  -UHAVE_LIBINTL_H
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/ff-config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/ff-config.h
)

set(luafontforge_sources
  fontforge/fontforge/PfEd.h
  fontforge/fontforge/autohint.c
  fontforge/fontforge/baseviews.h
  fontforge/fontforge/clipnoui.c
  fontforge/fontforge/configure-pfaedit.h
  fontforge/fontforge/cvundoes.c
  fontforge/fontforge/dumppfa.c
  fontforge/fontforge/edgelist.h
  fontforge/fontforge/edgelist2.h
  fontforge/fontforge/encoding.c
  fontforge/fontforge/encoding.h
  fontforge/fontforge/featurefile.c
  fontforge/fontforge/fontforgevw.h
  fontforge/fontforge/fontviewbase.c
  fontforge/fontforge/fvcomposit.c
  fontforge/fontforge/fvfonts.c
  fontforge/fontforge/lookups.c
  fontforge/fontforge/lookups.h
  fontforge/fontforge/macbinary.c
  fontforge/fontforge/macenc.c
  fontforge/fontforge/mathconstants.c
  fontforge/fontforge/memory.c
  fontforge/fontforge/mm.c
  fontforge/fontforge/mm.h
  fontforge/fontforge/namelist.c
  fontforge/fontforge/noprefs.c
  fontforge/fontforge/nouiutil.c
  fontforge/fontforge/parsepfa.c
  fontforge/fontforge/parsettf.c
  fontforge/fontforge/parsettfatt.c
  fontforge/fontforge/pfaedit.h
  fontforge/fontforge/plugins.h
  fontforge/fontforge/psfont.h
  fontforge/fontforge/psread.c
  fontforge/fontforge/pua.c
  fontforge/fontforge/python.c
  fontforge/fontforge/scripting.h
  fontforge/fontforge/sd.h
  fontforge/fontforge/sfd1.c
  fontforge/fontforge/sfd1.h
  fontforge/fontforge/splinechar.c
  fontforge/fontforge/splinefill.c
  fontforge/fontforge/splinefont.c
  fontforge/fontforge/splinefont.h
  fontforge/fontforge/splineorder2.c
  fontforge/fontforge/splineoverlap.c
  fontforge/fontforge/splinerefigure.c
  fontforge/fontforge/splinesave.c
  fontforge/fontforge/splinesaveafm.c
  fontforge/fontforge/splinestroke.c
  fontforge/fontforge/splineutil.c
  fontforge/fontforge/splineutil2.c
  fontforge/fontforge/start.c
  fontforge/fontforge/stemdb.c
  fontforge/fontforge/stemdb.h
  fontforge/fontforge/tottf.c
  fontforge/fontforge/tottfgpos.c
  fontforge/fontforge/ttf.h
  fontforge/fontforge/ttfspecial.c
  fontforge/fontforge/uiinterface.h
  fontforge/fontforge/unicoderange.h
  fontforge/gutils/fsys.c
  fontforge/inc/basics.h
  fontforge/inc/gfile.h
  fontforge/inc/gimage.h
  fontforge/inc/intl.h
  src/ffdummies.c
  src/luafflib.c
)

add_library(luatex-luafontforge-static STATIC ${luafontforge_sources})
