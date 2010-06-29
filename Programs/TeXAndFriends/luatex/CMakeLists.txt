## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2010 Christian Schenk
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
  ${CMAKE_CURRENT_SOURCE_DIR}/lua51
  ${CMAKE_CURRENT_SOURCE_DIR}/luafontloader/fontforge/inc
  ${CMAKE_CURRENT_SOURCE_DIR}/luafontloader/fontforge/fontforge
  ${CMAKE_CURRENT_BINARY_DIR}/luafontloader
)

add_definitions(
  -DUSE_MIKTEX_EXIT

  -DFONTFORGE_CONFIG_NO_WINDOWING_UI=1
  -DLUA_FF_LIB=1
  -DNODYNAMIC=1
  -DX_DISPLAY_MISSING=1
  -D_NO_FREETYPE=1
  -D_NO_LIBUNINAMESLIST=1
  -D_NO_PYTHON=1
  -UHAVE_LIBINTL_H
)

###############################################################################
## LUA51
###############################################################################

set(lua51_sources
  lua51/lapi.c
  lua51/lapi.h
  lua51/lauxlib.c
  lua51/lauxlib.h
  lua51/lbaselib.c
  lua51/lcoco.c
  lua51/lcoco.h
  lua51/lcode.c
  lua51/lcode.h
  lua51/ldblib.c
  lua51/ldebug.c
  lua51/ldebug.h
  lua51/ldo.c
  lua51/ldo.h
  lua51/ldump.c
  lua51/lfunc.c
  lua51/lfunc.h
  lua51/lgc.c
  lua51/lgc.h
  lua51/linit.c
  lua51/llex.c
  lua51/llex.h
  lua51/llimits.h
  lua51/lmathlib.c
  lua51/lmem.c
  lua51/lmem.h
  lua51/loadlib.c
  lua51/lobject.c
  lua51/lobject.h
  lua51/lopcodes.c
  lua51/lopcodes.h
  lua51/loslib.c
  lua51/lparser.c
  lua51/lparser.h
  lua51/lstate.c
  lua51/lstate.h
  lua51/lstring.c
  lua51/lstring.h
  lua51/lstrlib.c
  lua51/ltable.c
  lua51/ltable.h
  lua51/ltablib.c
  lua51/ltm.c
  lua51/ltm.h
  lua51/lua.h
  lua51/luaconf.h
  lua51/lualib.h
  lua51/lundump.c
  lua51/lundump.h
  lua51/lvm.c
  lua51/lvm.h
  lua51/lzio.c
  lua51/lzio.h
)

add_library(luatex-lua51-static STATIC ${lua51_sources})

###############################################################################
## LUASOCKET
###############################################################################

set(luasocket_sources
  luasocket/src/auxiliar.c
  luasocket/src/auxiliar.h
  luasocket/src/buffer.c
  luasocket/src/buffer.h
  luasocket/src/except.c
  luasocket/src/except.h
  luasocket/src/inet.c
  luasocket/src/inet.h
  luasocket/src/io.c
  luasocket/src/io.h
  luasocket/src/lua_preload.c
  luasocket/src/luasocket.c
  luasocket/src/luasocket.h
  luasocket/src/mime.c
  luasocket/src/mime.h
  luasocket/src/options.c
  luasocket/src/options.h
  luasocket/src/select.c
  luasocket/src/select.h
  luasocket/src/socket.c
  luasocket/src/socket.h
  luasocket/src/tcp.c
  luasocket/src/tcp.h
  luasocket/src/timeout.c
  luasocket/src/timeout.h
  luasocket/src/udp.c
  luasocket/src/udp.h
  luasocket/src/usocket.h
)

add_library(luatex-luasocket-static STATIC ${luasocket_sources})

###############################################################################
## LUAMISC
###############################################################################

set(luamisc_sources
  luafilesystem/src/lfs.c
  luafilesystem/src/lfs.h
  luamd5/luamd5.h
  luamd5/md5.c
  luamd5/md5_lua.c
  luamd5/md5lib.c
  luapeg/lpeg.c
  luaprofiler/clocks.c
  luaprofiler/clocks.h
  luaprofiler/core_profiler.c
  luaprofiler/core_profiler.h
  luaprofiler/function_meter.c
  luaprofiler/function_meter.h
  luaprofiler/lua50_profiler.c
  luaprofiler/luaprofiler.h
  luaprofiler/stack.c
  luaprofiler/stack.h
  luazip/src/luazip.c
  luazip/src/luazip.h
  luazlib/lgzip.c
  luazlib/lzlib.c
  slnunicode/slnunico.c
)

add_library(luatex-luamisc-static STATIC ${luamisc_sources})

###############################################################################
## LUAFONTFORGE
###############################################################################

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/luafontloader/ff-config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/luafontloader/ff-config.h
)

set(luafontforge_sources
  luafontloader/fontforge/Unicode/ArabicForms.c
  luafontloader/fontforge/Unicode/alphabet.c
  luafontloader/fontforge/Unicode/char.c
  luafontloader/fontforge/Unicode/cjk.c
  luafontloader/fontforge/Unicode/combiners.h
  luafontloader/fontforge/Unicode/gwwiconv.c
  luafontloader/fontforge/Unicode/memory.c
  luafontloader/fontforge/Unicode/ucharmap.c
  luafontloader/fontforge/Unicode/ustring.c
  luafontloader/fontforge/Unicode/utype.c
  luafontloader/fontforge/fontforge/PfEd.h
  luafontloader/fontforge/fontforge/autohint.c
  luafontloader/fontforge/fontforge/baseviews.h
  luafontloader/fontforge/fontforge/clipnoui.c
  luafontloader/fontforge/fontforge/configure-pfaedit.h
  luafontloader/fontforge/fontforge/cvundoes.c
  luafontloader/fontforge/fontforge/dumppfa.c
  luafontloader/fontforge/fontforge/edgelist.h
  luafontloader/fontforge/fontforge/edgelist2.h
  luafontloader/fontforge/fontforge/encoding.c
  luafontloader/fontforge/fontforge/encoding.h
  luafontloader/fontforge/fontforge/featurefile.c
  luafontloader/fontforge/fontforge/fontforgevw.h
  luafontloader/fontforge/fontforge/fontviewbase.c
  luafontloader/fontforge/fontforge/fvcomposit.c
  luafontloader/fontforge/fontforge/fvfonts.c
  luafontloader/fontforge/fontforge/libffstamp.h
  luafontloader/fontforge/fontforge/libstamp.pre
  luafontloader/fontforge/fontforge/lookups.c
  luafontloader/fontforge/fontforge/lookups.h
  luafontloader/fontforge/fontforge/macbinary.c
  luafontloader/fontforge/fontforge/macenc.c
  luafontloader/fontforge/fontforge/mathconstants.c
  luafontloader/fontforge/fontforge/mm.c
  luafontloader/fontforge/fontforge/mm.h
  luafontloader/fontforge/fontforge/namelist.c
  luafontloader/fontforge/fontforge/noprefs.c
  luafontloader/fontforge/fontforge/nouiutil.c
  luafontloader/fontforge/fontforge/parsepfa.c
  luafontloader/fontforge/fontforge/parsettf.c
  luafontloader/fontforge/fontforge/parsettfatt.c
  luafontloader/fontforge/fontforge/parsettfvar.c
  luafontloader/fontforge/fontforge/pfaedit.h
  luafontloader/fontforge/fontforge/plugins.h
  luafontloader/fontforge/fontforge/psfont.h
  luafontloader/fontforge/fontforge/psread.c
  luafontloader/fontforge/fontforge/pua.c
  luafontloader/fontforge/fontforge/python.c
  luafontloader/fontforge/fontforge/scripting.h
  luafontloader/fontforge/fontforge/sd.h
  luafontloader/fontforge/fontforge/sfd1.c
  luafontloader/fontforge/fontforge/sfd1.h
  luafontloader/fontforge/fontforge/splinechar.c
  luafontloader/fontforge/fontforge/splinefill.c
  luafontloader/fontforge/fontforge/splinefont.c
  luafontloader/fontforge/fontforge/splinefont.h
  luafontloader/fontforge/fontforge/splineorder2.c
  luafontloader/fontforge/fontforge/splineoverlap.c
  luafontloader/fontforge/fontforge/splinerefigure.c
  luafontloader/fontforge/fontforge/splinesave.c
  luafontloader/fontforge/fontforge/splinesaveafm.c
  luafontloader/fontforge/fontforge/splinestroke.c
  luafontloader/fontforge/fontforge/splineutil.c
  luafontloader/fontforge/fontforge/splineutil2.c
  luafontloader/fontforge/fontforge/start.c
  luafontloader/fontforge/fontforge/stemdb.c
  luafontloader/fontforge/fontforge/stemdb.h
  luafontloader/fontforge/fontforge/tottf.c
  luafontloader/fontforge/fontforge/tottfaat.c
  luafontloader/fontforge/fontforge/tottfgpos.c
  luafontloader/fontforge/fontforge/ttf.h
  luafontloader/fontforge/fontforge/ttfspecial.c
  luafontloader/fontforge/fontforge/uiinterface.h
  luafontloader/fontforge/fontforge/unicoderange.h
  luafontloader/fontforge/fontforge/views.h
  luafontloader/fontforge/gutils/fsys.c
  luafontloader/fontforge/gutils/gwwintl.c
  luafontloader/fontforge/inc/basics.h
  luafontloader/fontforge/inc/chardata.h
  luafontloader/fontforge/inc/charset.h
  luafontloader/fontforge/inc/dynamic.h
  luafontloader/fontforge/inc/fileutil.h
  luafontloader/fontforge/inc/gdraw.h
  luafontloader/fontforge/inc/gfile.h
  luafontloader/fontforge/inc/ggadget.h
  luafontloader/fontforge/inc/gicons.h
  luafontloader/fontforge/inc/gimage.h
  luafontloader/fontforge/inc/gio.h
  luafontloader/fontforge/inc/gkeysym.h
  luafontloader/fontforge/inc/gprogress.h
  luafontloader/fontforge/inc/gresource.h
  luafontloader/fontforge/inc/gwidget.h
  luafontloader/fontforge/inc/gwwiconv.h
  luafontloader/fontforge/inc/intl.h
  luafontloader/fontforge/inc/ustring.h
  luafontloader/fontforge/inc/utype.h
  luafontloader/src/ffdummies.c
  luafontloader/src/luafflib.c
)

add_library(luatex-luafontforge-static STATIC ${luafontforge_sources})

###############################################################################
## LUATEXLIB
###############################################################################

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dvi)

add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/dvi/dvigen.c
  COMMAND
    ${ctangle_exe} ${CMAKE_CURRENT_SOURCE_DIR}/dvi/dvigen.w
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/dvi
  MAIN_DEPENDENCY
    dvi/dvigen.w
  DEPENDS
    ${ctangle_exe}
    ${CMAKE_CURRENT_SOURCE_DIR}/dvi/dvigen.w
  VERBATIM
)

set(luatexlib_cweb_sources ${luatexlib_cweb_sources}
  ${CMAKE_CURRENT_BINARY_DIR}/dvi/dvigen.c
)



set_source_files_properties(
  ${luatexlib_cweb_sources}
  PROPERTIES GENERATED TRUE
)


set(luatexlib_sources
  ${luatexlib_cweb_sources}
)

set (todo_luatexlib_sources
  dvi/dvigen.h
  font/luatexfont.h
  font/mapfile.h
  font/sfnt.h
  font/texfont.h
  font/tt_glyf.h
  font/tt_table.h
  font/writecff.h
  font/writettf.h
  image/epdf.h
  image/image.h
  image/pdftoepdf.cc
  image/pdftoepdf.h
  image/writeimg.h
  image/writejbig2.h
  image/writejpg.h
  image/writepng.h
  lang/hnjalloc.h
  lang/hyphen.h
  lang/texlang.h
  lua/lcallbacklib.c
  lua/lepdflib.cc
  lua/lfontlib.c
  lua/limglib.c
  lua/liolib.c
  lua/lkpselib.c
  lua/llanglib.c
  lua/llualib.c
  lua/lnodelib.c
  lua/loslibext.c
  lua/lpdflib.c
  lua/lstatslib.c
  lua/ltexiolib.c
  lua/ltexlib.c
  lua/ltokenlib.c
  lua/luagen.h
  lua/luatex-api.h
  luatex.h
  ocp/ocp.h
  ocp/ocplist.h
  ocp/readocp.h
  ocp/runocp.h
  pdf/pagetree.h
  pdf/pdfaction.h
  pdf/pdfannot.h
  pdf/pdfcolorstack.h
  pdf/pdfdest.h
  pdf/pdffont.h
  pdf/pdfgen.h
  pdf/pdfglyph.h
  pdf/pdfimage.h
  pdf/pdflink.h
  pdf/pdflistout.h
  pdf/pdfliteral.h
  pdf/pdfobj.h
  pdf/pdfoutline.h
  pdf/pdfpage.h
  pdf/pdfrule.h
  pdf/pdfsaverestore.h
  pdf/pdfsetmatrix.h
  pdf/pdfshipout.h
  pdf/pdftables.h
  pdf/pdfthread.h
  pdf/pdftypes.h
  pdf/pdfxform.h
  tex/align.h
  tex/arithmetic.h
  tex/buildpage.h
  tex/commands.h
  tex/conditional.h
  tex/directions.h
  tex/dumpdata.h
  tex/equivalents.h
  tex/errors.h
  tex/expand.h
  tex/extensions.h
  tex/filename.h
  tex/inputstack.h
  tex/linebreak.h
  tex/mainbody.h
  tex/maincontrol.h
  tex/mathcodes.h
  tex/memoryword.h
  tex/mlist.h
  tex/nesting.h
  tex/packaging.h
  tex/postlinebreak.h
  tex/primitive.h
  tex/printing.h
  tex/scanning.h
  tex/stringpool.h
  tex/texdeffont.h
  tex/texfileio.h
  tex/texmath.h
  tex/texnodes.h
  tex/textcodes.h
  tex/textoken.h
  utils/avl.c
  utils/avl.h
  utils/avlstuff.h
  utils/managed-sa.h
  utils/synctex-common.h
  utils/synctex-luatex.h
  utils/synctex.c
  utils/synctex.h
  utils/utils.h
)

add_library(luatex-static STATIC ${luatexlib_sources})
