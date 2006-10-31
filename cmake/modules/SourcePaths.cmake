## SourcePaths.cmake
##
## Copyright (C) 2006 Christian Schenk
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

# directories
set(rel_app_dir		"Libraries/MiKTeX/App")
set(rel_bmeps_dir	"Libraries/3rd/libbmeps")
set(rel_build_tools_etc_dir	"BuildUtilities/etc")
set(rel_bzip2_dir	"Libraries/3rd/bzip2")
set(rel_c4p_dir		"BuildUtilities/c4p")
set(rel_cfg_dir		"BuildUtilities/cfg")
set(rel_config_dir	"Admin/ConfigFiles")
set(rel_core_dir	"Libraries/MiKTeX/Core")
set(rel_core_inc_dir	"${rel_core_dir}/include/miktex")
set(rel_curl_dir	"Libraries/3rd/libcurl")
set(rel_cweb_dir	"BuildUtilities/cweb")
set(rel_defaults_dir	"Admin/Defaults")
set(rel_dib_dir		"Libraries/MiKTeX/Dib")
set(rel_dvi_dir		"Libraries/MiKTeX/Dvi")
set(rel_expat_dir	"Libraries/3rd/libexpat")
set(rel_findtexmf_dir	"Programs/MiKTeX/findtexmf")
set(rel_gd_dir		"Libraries/3rd/libgd")
set(rel_gnu_dir		"Libraries/3rd/libgnu")
set(rel_gsoap_dir	"Libraries/3rd/gsoap")
set(rel_initexmf_dir	"Programs/MiKTeX/initexmf")
set(rel_jpeg_dir	"Libraries/3rd/libjpeg")
set(rel_kpsemu_dir	"Libraries/MiKTeX/KPathSeaEmulation")
set(rel_makex_dir	"Programs/MiKTeX/makex")
set(rel_md5_dir		"Libraries/3rd/libmd5")
set(rel_mkfntmap_dir	"Programs/MiKTeX/mkfntmap")
set(rel_mo_mfc_dir	"Programs/MiKTeX/MO/MFC")
set(rel_mpm_dir		"Programs/MiKTeX/PackageManager/mpm")
set(rel_mpm_mfc_dir	"Programs/MiKTeX/PackageManager/MFC")
set(rel_mpmlib_dir	"Libraries/MiKTeX/PackageManager")
set(rel_mspack_dir	"Libraries/3rd/libmspack")
set(rel_mtrep_dir	"Libraries/MiKTeX/WebServiceClients/Repository")
set(rel_netpbm_dir	"Libraries/3rd/netpbm")
set(rel_etex_dir	"Programs/TeXAndFriends/etex")
set(rel_pdfetex_dir	"Programs/TeXAndFriends/pdfetex")
set(rel_pdftex_dir	"Programs/TeXAndFriends/pdftex")
set(rel_png_dir		"Libraries/3rd/libpng")
set(rel_popt_dir	"Libraries/3rd/libpopt")
set(rel_programs_etc_dir	"Programs/MiKTeX/etc")
set(rel_tex_dir		"Programs/TeXAndFriends/tex")
set(rel_tex_etc_dir	"Programs/TeXAndFriends/etc")
set(rel_texmf_dir	"Libraries/MiKTeX/TeXAndFriends")
set(rel_texware_dir	"Programs/TeXAndFriends/texware")
set(rel_tie_dir		"BuildUtilities/tie")
set(rel_tiff_dir	"Libraries/3rd/libtiff")
set(rel_tpm_dir		"Admin/TPM")
set(rel_ui_mfc_dir	"Libraries/MiKTeX/UI/MFC")
set(rel_web_dir		"Programs/TeXAndFriends/web")
set(rel_xpdf_dir	"Programs/TeXAndFriends/pdftex/xpdf")
set(rel_xpdf_fofi_dir	"Programs/TeXAndFriends/pdftex/xpdf/fofi")
set(rel_xpdf_goo_dir		"Programs/TeXAndFriends/pdftex/xpdf/goo")
set(rel_xpdf_xpdf_dir	"Programs/TeXAndFriends/pdftex/xpdf/xpdf")
set(rel_zlib_dir	"Libraries/3rd/zlib")

set(alias_cpp "${CMAKE_SOURCE_DIR}/Libraries/MiKTeX/etc/alias.cpp")
set(pdftex_defaults "${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/pdftex.defaults")
set(tex_web "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex.web")
set(wrapper_cpp "${CMAKE_SOURCE_DIR}/Libraries/MiKTeX/etc/wrapper.cpp")

# sed scripts
set(tex_dyn_sed "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/dyn.sed")
set(etex_dyn_sed "${CMAKE_SOURCE_DIR}/${rel_etex_dir}/dyn.sed")
set(pdftex_dyn_sed "${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/dyn.sed")

# change files
set(etex_ch "${CMAKE_SOURCE_DIR}/${rel_etex_dir}/etex.ch")
set(etex_miktex_ch "${CMAKE_SOURCE_DIR}/${rel_etex_dir}/etex-miktex.ch")
set(hz_ch "${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/hz.ch")
set(misc_ch "${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/misc.ch")
set(noligatures_ch "${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/noligatures.ch")
set(pdf_mem_ch "${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/pdf_mem.ch")
set(pdfstrcmp_ch "${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/pdfstrcmp.ch")
set(pdftex_ch "${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/pdftex.ch")
set(pdftex_miktex_ch "${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/pdftex-miktex.ch")
set(pdftex_post_ch "${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/pdftex-post.ch")
set(pdftex_pre_ch "${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/pdftex-pre.ch")
set(randoms_ch "${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/randoms.ch")
set(tex_miktex_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-misc.ch")
set(tex_miktex_hyph_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-hyph.ch")
set(tex_miktex_quiet_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-quiet.ch")
set(tex_miktex_src_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-src.ch")
set(tex_miktex_stat_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-stat.ch")
set(tex_miktex_write18_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-write18.ch")
set(tex_mltex_miktex_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/mltex-miktex.ch")
set(vadjust_ch "${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/vadjust.ch")

# build utilities
if(WIN32)
  set(exe_suffix ".exe")
else(WIN32)
  set(exe_suffix "")
endif(WIN32)
set(c4p_exe "${outdir}/${CMAKE_CFG_INTDIR}/c4p${exe_suffix}")
set(cfg_exe "${outdir}/${CMAKE_CFG_INTDIR}/cfg${exe_suffix}")
set(ctangle_exe "${outdir}/${CMAKE_CFG_INTDIR}/ctangle${exe_suffix}")
set(dvitype_exe "${outdir}/${CMAKE_CFG_INTDIR}/dvitype${exe_suffix}")
set(pltotf_exe "${outdir}/${CMAKE_CFG_INTDIR}/pltotf${exe_suffix}")
set(tangle_exe "${outdir}/${CMAKE_CFG_INTDIR}/tangle${exe_suffix}")
set(tftopl_exe "${outdir}/${CMAKE_CFG_INTDIR}/tftopl${exe_suffix}")
set(tie_exe "${outdir}/${CMAKE_CFG_INTDIR}/tie${exe_suffix}")
set(triptex_exe "${outdir}/${CMAKE_CFG_INTDIR}/triptex${exe_suffix}")
