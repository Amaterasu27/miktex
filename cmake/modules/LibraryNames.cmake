## LibraryNames.cmake
##
## Copyright (C) 2006-2007 Christian Schenk
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
set(bzip2_dll_name	"${miktex_dll_prefix}-bzip2")
set(cjklatex_dll_name	"${miktex_dll_prefix}-cjklatex")
set(core_dll_name	"${miktex_dll_prefix}-core")
set(core_ps_dll_name	"${core_dll_name}-PS")
set(curl_dll_name	"${miktex_dll_prefix}-curl")
set(dib_dll_name	"${miktex_dll_prefix}-dib")
set(dvi_dll_name	"${miktex_dll_prefix}-dvi")
set(dvicopy_dll_name	"${miktex_dll_prefix}-dvicopy")
set(dvitomp_dll_name	"${miktex_dll_prefix}-dvitomp")
set(dvitype_dll_name	"${miktex_dll_prefix}-dvitype")
set(expat_dll_name	"${miktex_dll_prefix}-expat")
set(extractor_dll_name	"${miktex_dll_prefix}-extractor")
set(findtexmf_dll_name	"${miktex_dll_prefix}-findtexmf")
set(fontconfig_dll_name	"${miktex_dll_prefix}-fontconfig")
set(freetype2_dll_name	"${miktex_dll_prefix}-freetype2")
set(freetype_dll_name	"${miktex_dll_prefix}-freetype")
set(gd_dll_name		"${miktex_dll_prefix}-gd")
set(gftodvi_dll_name	"${miktex_dll_prefix}-gftodvi")
set(gftopk_dll_name	"${miktex_dll_prefix}-gftopk")
set(gftype_dll_name	"${miktex_dll_prefix}-gftype")
set(gnu_dll_name	"${miktex_dll_prefix}-gnu")
set(graphite_engine_dll_name	"${miktex_dll_prefix}-graphite-engine")
set(jpeg_dll_name	"${miktex_dll_prefix}-jpeg")
set(kpsemu_dll_name	"${miktex_dll_prefix}-kpseweb2cemu")
set(lzma_dll_name	"${miktex_dll_prefix}-lzma")
set(makex_dll_name	"${miktex_dll_prefix}-make")
set(mcd_dll_name	"${miktex_dll_prefix}-compilerdriver")
set(md5_dll_name	"${miktex_dll_prefix}-md5")
set(mf_dll_name		"${miktex_dll_prefix}-mf")
set(mft_dll_name	"${miktex_dll_prefix}-mft")
set(mkfntmap_dll_name	"${miktex_dll_prefix}-mkfntmap")
set(mp_dll_name		"${miktex_dll_prefix}-mp")
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
set(png_dll_name	"${miktex_dll_prefix}-png")
set(pnm_dll_name	"${miktex_dll_prefix}-pnm")
set(pooltype_dll_name	"${miktex_dll_prefix}-pooltype")
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
set(ui_mfc_dll_name	"${miktex_dll_prefix}-uimfc")
set(unxemu_dll_name	"${miktex_dll_prefix}-unxemu")
set(vftovp_dll_name	"${miktex_dll_prefix}-vftovp")
set(vptovf_dll_name	"${miktex_dll_prefix}-vptovf")
set(weave_dll_name	"${miktex_dll_prefix}-weave")
set(xetex_dll_name	"${miktex_dll_prefix}-xetex")
set(zlib_dll_name	"${miktex_dll_prefix}-zlib")

set(session_tlb_name    "${miktex_dll_prefix}-session")
set(mpm_tlb_name        "${miktex_dll_prefix}-packageManager")

## prebuilt libraries
set(icudt_dll_name	"icudt")
set(icule_dll_name	"icule")
set(icuuc_dll_name	"icuuc")

## static library names
set(bzip2_lib_name	"bzip2-static")
set(core_lib_name	"core-static")
set(curl_lib_name	"curl-static")
set(expat_lib_name	"expat-static")
set(extractor_lib_name	"extractor-static")
set(fofi_lib_name	"fofi-static")
set(gnu_lib_name	"gnu-static")
set(goo_lib_name	"goo-static")
set(graphite_engine_lib_name	"graphite-engine-static")
set(lzma_lib_name	"lzma-static")
set(md5_lib_name	"md5-static")
set(mpm_lib_name	"mpm-static")
set(mspack_lib_name	"mspack-static")
set(popt_lib_name	"popt-static")
set(regex_lib_name	"regex-static")
set(xpdf_lib_name	"xpdf-static")
set(zlib_lib_name	"zlib-static")
