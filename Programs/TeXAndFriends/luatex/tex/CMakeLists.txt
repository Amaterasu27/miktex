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
  ${CMAKE_BINARY_DIR}/${rel_mp_dir}
  ${CMAKE_BINARY_DIR}/${rel_xpdf_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_goo_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_xpdf_dir}
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
  ../image/epdf.w
  ../image/writeimg.w
  ../image/writejbig2.w
  ../image/writejpg.w
  ../image/writepng.w
  ../lua/luagen.w
  ../lua/luainit.w
  ../lua/luanode.w
  ../lua/luastuff.w
  ../lua/luatoken.w
  ../lua/texluac.w
  ../ocp/ocp.w
  ../ocp/runocp.w
  ../ocp/readocp.w
  ../ocp/ocplist.w
  ../lang/hnjalloc.w
  ../lang/hyphen.w
  ../lang/texlang.w
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
  ../tex/dumpdata.w
  ../tex/expand.w
  ../tex/inputstack.w
  ../tex/maincontrol.w
  ../tex/mlist.w
  ../tex/postlinebreak.w
  ../tex/scanning.w
  ../tex/texdeffont.w
  ../tex/texnodes.w
  ../tex/arithmetic.w
  ../tex/conditional.w
  ../tex/equivalents.w
  ../tex/extensions.w
  ../tex/commands.w
  ../tex/linebreak.w
  ../tex/mathcodes.w
  ../tex/nesting.w
  ../tex/primitive.w
  ../tex/texfileio.w
  ../tex/textcodes.w
  ../tex/buildpage.w
  ../tex/directions.w
  ../tex/errors.w
  ../tex/filename.w
  ../tex/mainbody.w
  ../tex/memoryword.w
  ../tex/packaging.w
  ../tex/printing.w
  ../tex/stringpool.w
  ../tex/texmath.w
  ../tex/textoken.w
  ../utils/utils.w
  ../utils/managed-sa.w
  ../utils/avlstuff.w
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

set(luatexlib_sources
  ${luatexlib_cweb_sources}
  ${CMAKE_SOURCE_DIR}/${rel_mp_dir}/lmplib.c
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
  ../luatex.h
  ../ocp/ocp.h
  ../ocp/ocplist.h
  ../ocp/readocp.h
  ../ocp/runocp.h
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
  ../utils/synctex-common.h
  ../utils/synctex-luatex.h
  ../utils/synctex.c
  ../utils/synctex.h
  ../utils/utils.h
)

add_library(luatex-static STATIC ${luatexlib_sources})
