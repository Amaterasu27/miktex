## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2010-2011 Christian Schenk
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
  ${CMAKE_BINARY_DIR}/${rel_mp_dir}
  ${CMAKE_BINARY_DIR}/${rel_poppler_dir}/poppler
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler
  ${CMAKE_BINARY_DIR}/${rel_poppler_dir}
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}
)

set(luatexlib_cwebs
  ../dvi/dvigen.w
  ../font/dofont.w
  ../font/luafont.w
  ../font/mapfile.w
  ../font/pkin.w
  ../font/sfnt.w
  ../font/subfont.w
  ../font/texfont.w
  ../font/tfmofm.w
  ../font/tounicode.w
  ../font/tt_glyf.w
  ../font/tt_table.w
  ../font/vfovf.w
  ../font/vfpacket.w
  ../font/writecff.w
  ../font/writeenc.w
  ../font/writefont.w
  ../font/writet1.w
  ../font/writet3.w
  ../font/writettf.w
  ../font/writetype0.w
  ../font/writetype2.w
  ../image/writeimg.w
  ../image/writejbig2.w
  ../image/writejp2.w
  ../image/writejpg.w
  ../image/writepng.w
  ../lang/hnjalloc.w
  ../lang/hyphen.w
  ../lang/texlang.w
  ../lua/luagen.w
  ../lua/luainit.w
  ../lua/luanode.w
  ../lua/luastuff.w
  ../lua/luatoken.w
  ../lua/texluac.w
  ../luascripts/txt2zlib.w
  ../pdf/pagetree.w
  ../pdf/pdfaction.w
  ../pdf/pdfannot.w
  ../pdf/pdfcolorstack.w
  ../pdf/pdfdest.w
  ../pdf/pdffont.w
  ../pdf/pdfgen.w
  ../pdf/pdfglyph.w
  ../pdf/pdfimage.w
  ../pdf/pdflink.w
  ../pdf/pdflistout.w
  ../pdf/pdfliteral.w
  ../pdf/pdfluaapi.w
  ../pdf/pdfobj.w
  ../pdf/pdfoutline.w
  ../pdf/pdfpage.w
  ../pdf/pdfrule.w
  ../pdf/pdfsaverestore.w
  ../pdf/pdfsetmatrix.w
  ../pdf/pdfshipout.w
  ../pdf/pdftables.w
  ../pdf/pdfthread.w
  ../pdf/pdfxform.w
  ../tex/align.w
  ../tex/arithmetic.w
  ../tex/buildpage.w
  ../tex/commands.w
  ../tex/conditional.w
  ../tex/directions.w
  ../tex/dumpdata.w
  ../tex/equivalents.w
  ../tex/errors.w
  ../tex/expand.w
  ../tex/extensions.w
  ../tex/filename.w
  ../tex/inputstack.w
  ../tex/linebreak.w
  ../tex/mainbody.w
  ../tex/maincontrol.w
  ../tex/mathcodes.w
  ../tex/memoryword.w
  ../tex/mlist.w
  ../tex/nesting.w
  ../tex/packaging.w
  ../tex/postlinebreak.w
  ../tex/primitive.w
  ../tex/printing.w
  ../tex/scanning.w
  ../tex/stringpool.w
  ../tex/texdeffont.w
  ../tex/texfileio.w
  ../tex/texmath.w
  ../tex/texnodes.w
  ../tex/textcodes.w
  ../tex/textoken.w
  ../utils/avlstuff.w
  ../utils/managed-sa.w
  ../utils/utils.w
)

foreach(cweb ${luatexlib_cwebs})
  get_filename_component(fn ${cweb} NAME_WE)
  add_custom_command(
    OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/${fn}.c
    COMMAND
      ${ctangle_exe} ${CMAKE_CURRENT_SOURCE_DIR}/${cweb}
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_BINARY_DIR}
    MAIN_DEPENDENCY
      ${cweb}
    DEPENDS
      ${ctangle_exe}
      ${CMAKE_CURRENT_SOURCE_DIR}/${cweb}
    VERBATIM
  )
  set(luatexlib_cweb_sources ${luatexlib_cweb_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/${fn}.c
  )
endforeach(cweb)

set_source_files_properties(
  ${luatexlib_cweb_sources}
  PROPERTIES
    GENERATED TRUE
)

add_executable(txt2zlib txt2zlib.c)

target_link_libraries(txt2zlib
  ${zlib_dll_name}
)

merge_trustinfo_manifest(txt2zlib asInvoker)

get_target_property(txt2zlib_exe txt2zlib LOCATION)

make_directory(${CMAKE_BINARY_DIR}/${rel_luatex_dir}/luascripts)

add_custom_command(
  OUTPUT
    ${CMAKE_BINARY_DIR}/${rel_luatex_dir}/luascripts/pdflua.c
  COMMAND
    ${txt2zlib_exe}
      ${CMAKE_SOURCE_DIR}/${rel_luatex_dir}/luascripts/pdflua.lua
      pdflua.c
  WORKING_DIRECTORY
    ${CMAKE_BINARY_DIR}/${rel_luatex_dir}/luascripts
  MAIN_DEPENDENCY
    ${CMAKE_SOURCE_DIR}/${rel_luatex_dir}/luascripts/pdflua.lua
  DEPENDS
    ${txt2zlib_exe}
    ${CMAKE_SOURCE_DIR}/${rel_luatex_dir}/luascripts/pdflua.lua
  VERBATIM
)

set_source_files_properties(
  ${CMAKE_BINARY_DIR}/${rel_luatex_dir}/luascripts/pdflua.c
  PROPERTIES
    GENERATED TRUE
)
set(luatexlib_sources
  ${luatexlib_cweb_sources}
  ${CMAKE_SOURCE_DIR}/${rel_mp_dir}/lmplib.c
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-common.h
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex-luatex.h
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex.c
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}/synctex.h
  ${CMAKE_BINARY_DIR}/${rel_luatex_dir}/luascripts/pdflua.c
  ../dvi/dvigen.h
  ../font/luatexfont.h
  ../font/mapfile.h
  ../font/sfnt.h
  ../font/texfont.h
  ../font/tt_glyf.h
  ../font/tt_table.h
  ../font/writecff.h
  ../font/writettf.h
  ../image/epdf.h
  ../image/image.h
  ../image/pdftoepdf.cc
  ../image/pdftoepdf.h
  ../image/writeimg.h
  ../image/writejbig2.h
  ../image/writejp2.h
  ../image/writejpg.h
  ../image/writepng.h
  ../lang/hnjalloc.h
  ../lang/hyphen.h
  ../lang/texlang.h
  ../lua/lcallbacklib.c
  ../lua/lepdflib.cc
  ../lua/lfontlib.c
  ../lua/limglib.c
  ../lua/liolib.c
  ../lua/lkpselib.c
  ../lua/llanglib.c
  ../lua/llfslibext.c
  ../lua/llualib.c
  ../lua/lnodelib.c
  ../lua/loslibext.c
  ../lua/lpdflib.c
  ../lua/lstatslib.c
  ../lua/ltexiolib.c
  ../lua/ltexlib.c
  ../lua/ltokenlib.c
  ../lua/luagen.h
  ../lua/luatex-api.h
  ../luascripts/pdflua.h
  ../luatex.h
  ../pdf/pagetree.h
  ../pdf/pdfaction.h
  ../pdf/pdfannot.h
  ../pdf/pdfcolorstack.h
  ../pdf/pdfdest.h
  ../pdf/pdffont.h
  ../pdf/pdfgen.h
  ../pdf/pdfglyph.h
  ../pdf/pdfimage.h
  ../pdf/pdflink.h
  ../pdf/pdflistout.h
  ../pdf/pdfliteral.h
  ../pdf/pdfluaapi.h
  ../pdf/pdfobj.h
  ../pdf/pdfoutline.h
  ../pdf/pdfpage.h
  ../pdf/pdfrule.h
  ../pdf/pdfsaverestore.h
  ../pdf/pdfsetmatrix.h
  ../pdf/pdfshipout.h
  ../pdf/pdftables.h
  ../pdf/pdfthread.h
  ../pdf/pdftypes.h
  ../pdf/pdfxform.h
  ../tex/align.h
  ../tex/arithmetic.h
  ../tex/buildpage.h
  ../tex/commands.h
  ../tex/conditional.h
  ../tex/directions.h
  ../tex/dumpdata.h
  ../tex/equivalents.h
  ../tex/errors.h
  ../tex/expand.h
  ../tex/extensions.h
  ../tex/filename.h
  ../tex/inputstack.h
  ../tex/linebreak.h
  ../tex/mainbody.h
  ../tex/maincontrol.h
  ../tex/mathcodes.h
  ../tex/memoryword.h
  ../tex/mlist.h
  ../tex/nesting.h
  ../tex/packaging.h
  ../tex/postlinebreak.h
  ../tex/primitive.h
  ../tex/printing.h
  ../tex/scanning.h
  ../tex/stringpool.h
  ../tex/texdeffont.h
  ../tex/texfileio.h
  ../tex/texmath.h
  ../tex/texnodes.h
  ../tex/textcodes.h
  ../tex/textoken.h
  ../utils/avl.c
  ../utils/avl.h
  ../utils/avlstuff.h
  ../utils/managed-sa.h
  ../utils/utils.h
)

add_library(luatex-static STATIC ${luatexlib_sources})
