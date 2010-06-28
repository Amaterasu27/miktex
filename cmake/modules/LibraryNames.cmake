## LibraryNames.cmake
##
## Copyright (C) 2006-2010 Christian Schenk
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

## shared library names; naming convention:
##    MiKTeXSERIES-NAME[-REV]
## where:
##    NAME is the component name
##    SERIES is the MiKTeX series (as an integer)
##    REV is the DLL revision (relative to the current series)

set(miktex_dll_prefix MiKTeX${MIKTEX_SERIES_INT})

if(MIKTEX_RELEASE_STATE EQUAL 0)
set(miktex_dll_prefix MiKTeX${MIKTEX_SERIES_INT})
elseif(MIKTEX_RELEASE_STATE EQUAL 1)
set(miktex_dll_prefix MiKTeX${MIKTEX_SERIES_INT}-rc-${MIKTEX_RELEASE_NUM})
elseif(MIKTEX_RELEASE_STATE EQUAL 2)
set(miktex_dll_prefix MiKTeX${MIKTEX_SERIES_INT}-beta-${MIKTEX_RELEASE_NUM})
elseif(MIKTEX_RELEASE_STATE EQUAL 3)
set(miktex_dll_prefix MiKTeX${MIKTEX_SERIES_INT}-snapshot)
elseif(MIKTEX_RELEASE_STATE EQUAL 4)
set(miktex_dll_prefix MiKTeX${MIKTEX_SERIES_INT}-debug)
endif(MIKTEX_RELEASE_STATE EQUAL 0)

set(app_dll_name	"${miktex_dll_prefix}-app")
set(arctrl_dll_name	"${miktex_dll_prefix}-arctrl")
set(bibtex_dll_name	"${miktex_dll_prefix}-bibtex")
set(bmeps_dll_name	"${miktex_dll_prefix}-bmeps")
if(NOT USE_SYSTEM_BZIP2)
  set(bzip2_dll_name	"${miktex_dll_prefix}-bzip2")
endif(NOT USE_SYSTEM_BZIP2)
set(cjklatex_dll_name	"${miktex_dll_prefix}-cjklatex")
set(core_dll_name	"${miktex_dll_prefix}-core")
set(core_ps_dll_name	"${core_dll_name}-PS")
if(NOT USE_SYSTEM_CURL)
  set(curl_dll_name	"${miktex_dll_prefix}-curl")
endif(NOT USE_SYSTEM_CURL)
set(dib_dll_name	"${miktex_dll_prefix}-dib")
set(dvi_dll_name	"${miktex_dll_prefix}-dvi")
set(dvipdfmx_dll_name	"${miktex_dll_prefix}-dvipdfmx")
set(dvicopy_dll_name	"${miktex_dll_prefix}-dvicopy")
set(dvitype_dll_name	"${miktex_dll_prefix}-dvitype")
if(NOT USE_SYSTEM_EXPAT)
  set(expat_dll_name	"${miktex_dll_prefix}-expat")
endif(NOT USE_SYSTEM_EXPAT)
set(extractor_dll_name	"${miktex_dll_prefix}-extractor")
set(findtexmf_dll_name	"${miktex_dll_prefix}-findtexmf")
set(fontconfig_dll_name	"${miktex_dll_prefix}-fontconfig")
set(freetype2_dll_name	"${miktex_dll_prefix}-freetype2")
set(freetype_dll_name	"${miktex_dll_prefix}-freetype")
set(gd_dll_name		"${miktex_dll_prefix}-gd")
set(getopt_dll_name	"${miktex_dll_prefix}-getopt")
set(gftodvi_dll_name	"${miktex_dll_prefix}-gftodvi")
set(gftopk_dll_name	"${miktex_dll_prefix}-gftopk")
set(gftype_dll_name	"${miktex_dll_prefix}-gftype")
set(graphite_engine_dll_name	"${miktex_dll_prefix}-graphite-engine")
set(hunspell_dll_name	"${miktex_dll_prefix}-hunspell")
set(jpeg_dll_name	"${miktex_dll_prefix}-jpeg")
set(kpsemu_dll_name	"${miktex_dll_prefix}-kpathsea")
set(lzma_dll_name	"${miktex_dll_prefix}-lzma")
set(lua_dll_name	"${miktex_dll_prefix}-lua51")
set(makex_dll_name	"${miktex_dll_prefix}-make")
set(mcd_dll_name	"${miktex_dll_prefix}-compilerdriver")
set(md5_dll_name	"${miktex_dll_prefix}-md5")
set(mf_dll_name		"${miktex_dll_prefix}-mf")
set(mft_dll_name	"${miktex_dll_prefix}-mft")
set(mkfntmap_dll_name	"${miktex_dll_prefix}-mkfntmap")
set(mpm_dll_name	"${miktex_dll_prefix}-packagemanager")
set(mpm_ps_dll_name     "${mpm_dll_name}-PS")
set(mspack_dll_name	"${miktex_dll_prefix}-mspack")
set(mthelp_dll_name	"${miktex_dll_prefix}-mthelp")
set(odvicopy_dll_name	"${miktex_dll_prefix}-odvicopy")
set(ofm2opl_dll_name	"${miktex_dll_prefix}-ofm2opl")
set(omega_dll_name	"${miktex_dll_prefix}-omega")
set(opl2ofm_dll_name	"${miktex_dll_prefix}-opl2ofm")
set(ovf2ovp_dll_name	"${miktex_dll_prefix}-ovf2ovp")
set(ovp2ovf_dll_name	"${miktex_dll_prefix}-ovp2ovf")
set(pbm_dll_name	"${miktex_dll_prefix}-pbm")
set(pdftex_dll_name	"${miktex_dll_prefix}-pdftex")
set(pgm_dll_name	"${miktex_dll_prefix}-pgm")
set(pltotf_dll_name	"${miktex_dll_prefix}-pltotf")
if(NOT USE_SYSTEM_PNG)
  set(png_dll_name	"${miktex_dll_prefix}-png")
endif(NOT USE_SYSTEM_PNG)
set(pnm_dll_name	"${miktex_dll_prefix}-pnm")
set(pooltype_dll_name	"${miktex_dll_prefix}-pooltype")
set(poppler_qt4_dll_name "${miktex_dll_prefix}-poppler-qt4")
set(popt_dll_name	"${miktex_dll_prefix}-popt")
set(ppm_dll_name	"${miktex_dll_prefix}-ppm")
set(psres_dll_name	"${miktex_dll_prefix}-psres")
set(regex_dll_name	"${miktex_dll_prefix}-regex")
set(tangle_dll_name	"${miktex_dll_prefix}-tangle")
set(teckit_dll_name	"${miktex_dll_prefix}-teckit")
set(tex_dll_name	"${miktex_dll_prefix}-tex")
set(texmf_dll_name	"${miktex_dll_prefix}-texmf")
set(tftopl_dll_name	"${miktex_dll_prefix}-tftopl")
set(tiff_dll_name	"${miktex_dll_prefix}-tiff")
set(twluaplugin_dll_name "${miktex_dll_prefix}-twluaplugin")
set(ui_mfc_dll_name	"${miktex_dll_prefix}-ui-MFC")
set(ui_qt_dll_name	"${miktex_dll_prefix}-ui-Qt")
set(unxemu_dll_name	"${miktex_dll_prefix}-unxemu")
if(NOT USE_SYSTEM_URIPARSER)
  set(uriparser_dll_name "${miktex_dll_prefix}-uriparser")
endif(NOT USE_SYSTEM_URIPARSER)
set(vftovp_dll_name	"${miktex_dll_prefix}-vftovp")
set(vptovf_dll_name	"${miktex_dll_prefix}-vptovf")
set(w2cemu_dll_name	"${miktex_dll_prefix}-web2c")
set(weave_dll_name	"${miktex_dll_prefix}-weave")
set(xetex_dll_name	"${miktex_dll_prefix}-xetex")
if(NOT USE_SYSTEM_ZLIB)
  set(zlib_dll_name	"${miktex_dll_prefix}-zlib")
endif(NOT USE_SYSTEM_ZLIB)

set(session_tlb_name    "${miktex_dll_prefix}-session")
set(mpm_tlb_name        "${miktex_dll_prefix}-packageManager")

## prebuilt libraries
set(icudt_dll_name	"icudt")
set(icule_dll_name	"icule")
set(icuuc_dll_name	"icuuc")

## static library names
set(app_lib_name	"app-static")
if(NOT USE_SYSTEM_BZIP2)
  set(bzip2_lib_name	"bzip2-static")
endif(NOT USE_SYSTEM_BZIP2)
set(core_lib_name	"core-static")
if(NOT USE_SYSTEM_CURL)
  set(curl_lib_name	"curl-static")
endif(NOT USE_SYSTEM_CURL)
if(NOT USE_SYSTEM_EXPAT)
  set(expat_lib_name	"expat-static")
endif(NOT USE_SYSTEM_EXPAT)
set(extractor_lib_name	"extractor-static")
set(fofi_lib_name	"fofi-static")
set(getopt_lib_name	"getopt-static")
set(goo_lib_name	"goo-static")
set(graphite_engine_lib_name	"graphite-engine-static")
set(kpsemu_lib_name	"kpathsea-static")
set(lua_lib_name	"lua51-static")
set(lzma_lib_name	"lzma-static")
set(makex_lib_name	"make-static")
set(md5_lib_name	"md5-static")
set(mp_lib_name         "mp-static")
set(mpm_lib_name	"mpm-static")
set(mspack_lib_name	"mspack-static")
set(mthelp_lib_name	"mthelp-static")
set(pdftex_lib_name	"pdftex-static")
if(NOT USE_SYSTEM_PNG)
  set(png_lib_name	"png-static")
endif(NOT USE_SYSTEM_PNG)
set(poppler_lib_name	"poppler-static")
set(popt_lib_name	"popt-static")
set(regex_lib_name	"regex-static")
set(tangle_lib_name	"tangle-static")
set(texmf_lib_name	"texmf-static")
set(ui_qt_lib_name	"ui-Qt-static")
if(NOT USE_SYSTEM_URIPARSER)
  set(uriparser_lib_name	"uriparser-static")
endif(NOT USE_SYSTEM_URIPARSER)
set(weave_lib_name	"weave-static")
set(xpdf_lib_name	"xpdf-static")
if(NOT USE_SYSTEM_ZLIB)
  set(zlib_lib_name	"zlib-static")
endif(NOT USE_SYSTEM_ZLIB)
