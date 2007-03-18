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
set(arctrl_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-arctrl")
set(app_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-app")
set(bmeps_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-bmeps")
set(bzip2_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-bzip2")
set(core_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-core")
set(core_ps_dll_name	"${core_dll_name}-PS")
set(curl_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-curl")
set(dib_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-dib")
set(dvi_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-dvi")
set(dvitype_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-dvitype")
set(expat_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-expat")
set(extractor_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-extractor")
set(findtexmf_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-findtexmf")
set(gd_dll_name		"MiKTeX${MIKTEX_SERIES_INT}-gd")
set(gnu_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-gnu")
set(jpeg_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-jpeg")
set(kpsemu_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-kpseweb2cemu")
set(makex_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-make")
set(dvicopy_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-dvicopy")
set(md5_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-md5")
set(mkfntmap_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-mkfntmap")
set(mpm_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-packagemanager")
set(mpm_ps_dll_name     "${mpm_dll_name}-PS")
set(mspack_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-mspack")
set(mthelp_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-mthelp")
set(odvicopy_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-odvicopy")
set(ofm2opl_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-ofm2opl")
set(omega_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-omega")
set(opl2ofm_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-opl2ofm")
set(ovf2ovp_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-ovf2ovp")
set(ovp2ovf_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-ovp2ovf")
set(pbm_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-pbm")
set(pdftex_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-pdftex")
set(pgm_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-pgm")
set(pltotf_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-pltotf")
set(png_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-png")
set(pnm_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-pnm")
set(pooltype_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-pooltype")
set(popt_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-popt")
set(ppm_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-ppm")
set(regex_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-regex")
set(tangle_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-tangle")
set(tex_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-tex")
set(texmf_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-texmf")
set(tftopl_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-tftopl")
set(tiff_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-tiff")
set(ui_dll_name		"MiKTeX${MIKTEX_SERIES_INT}-ui")
set(vftovp_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-vftovp")
set(vptovf_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-vptovf")
set(weave_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-weave")
set(zlib_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-zlib")
set(gftopk_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-gftopk")
set(mft_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-mft")
set(gftype_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-gftype")
set(gftodvi_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-gftodvi")
set(mf_dll_name		"MiKTeX${MIKTEX_SERIES_INT}-mf")
set(mp_dll_name		"MiKTeX${MIKTEX_SERIES_INT}-mp")
set(dvitomp_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-dvitomp")
set(bibtex_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-bibtex")
set(mcd_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-compilerdriver")
set(cjklatex_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-cjklatex")
set(freetype_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-freetype")
set(psres_dll_name	"MiKTeX${MIKTEX_SERIES_INT}-psres")

## static library names
set(bzip2_lib_name	"bzip2-static")
set(core_lib_name	"core-static")
set(curl_lib_name	"curl-static")
set(expat_lib_name	"expat-static")
set(extractor_lib_name	"extractor-static")
set(fofi_lib_name	"fofi-static")
set(gnu_lib_name	"gnu-static")
set(goo_lib_name	"goo-static")
set(md5_lib_name	"md5-static")
set(mpm_lib_name	"mpm-static")
set(mspack_lib_name	"mspack-static")
set(popt_lib_name	"popt-static")
set(regex_lib_name	"regex-static")
set(xpdf_lib_name	"xpdf-static")
set(zlib_lib_name	"zlib-static")
