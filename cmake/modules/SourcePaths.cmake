## SourcePaths.cmake
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

# directories
set(rel_7zip_dir	"Libraries/3rd/7-Zip")
set(rel_app_dir		"Libraries/MiKTeX/App")
set(rel_arctrl_win_dir	"Programs/MiKTeX/arctrl/win")
set(rel_bibtex_dir	"Programs/Bibliography/bibtex")
set(rel_bibtexx_dir	"Programs/Bibliography/bibtex-x")
set(rel_bmeps_dir	"Libraries/3rd/libbmeps")
set(rel_botan_dir	"Libraries/3rd/botan")
set(rel_build_tools_etc_dir	"BuildUtilities/etc")
set(rel_bzip2_dir	"Libraries/3rd/bzip2")
set(rel_c4p_dir		"BuildUtilities/c4p")
set(rel_cairo_dir	"Libraries/3rd/cairo")
set(rel_cairo_include_dir	"${rel_cairo_dir}/include")
set(rel_cairo_source_dir	"${rel_cairo_dir}/src")
set(rel_cfg_dir		"BuildUtilities/cfg")
set(rel_cjkutils_dir	"Programs/FontUtilities/cjkutils")
set(rel_config_dir	"Admin/ConfigFiles")
set(rel_copystart_dir	"Programs/MiKTeX/copystart")
set(rel_core_dir	"Libraries/MiKTeX/Core")
set(rel_curl_dir	"Libraries/3rd/libcurl")
set(rel_cweb_dir	"BuildUtilities/cweb")
set(rel_defaults_dir	"Admin/Defaults")
set(rel_dib_dir		"Libraries/MiKTeX/Dib")
set(rel_doc_dir		"Documentation")
set(rel_docbook_dir	"BuildUtilities/docbook")
set(rel_docbook_xsl_dir	"BuildUtilities/docbook/docbook-xsl")
set(rel_dvi_dir		"Libraries/MiKTeX/Dvi")
set(rel_dvicopy_dir	"Programs/DviWare/dvicopy")
set(rel_dvipdfmx_dir	"Programs/DviWare/dvipdfm-x")
set(rel_dvipng_dir	"Programs/DviWare/dvipng")
set(rel_dvips_dir	"Programs/DviWare/dvips")
set(rel_epstopdf_dir	"Programs/MiKTeX/epstopdf")
set(rel_etex_dir	"Programs/TeXAndFriends/etex")
set(rel_expat_dir	"Libraries/3rd/libexpat")
set(rel_extractor_dir	"Libraries/MiKTeX/Extractor")
set(rel_findtexmf_dir	"Programs/MiKTeX/findtexmf")
set(rel_fontconfig_dir	"Libraries/3rd/fontconfig")
set(rel_freetype2_dir	"Libraries/3rd/freetype2")
set(rel_freetype_dir	"Programs/FontUtilities/freetype")
set(rel_gd_dir		"Libraries/3rd/libgd")
set(rel_getopt_dir	"Libraries/3rd/getopt")
set(rel_graphite2_dir	"Libraries/3rd/graphite2")
set(rel_gsf2pk_dir	"Programs/MiKTeX/gsf2pk")
set(rel_gsoap_dir	"Libraries/3rd/gsoap")
set(rel_harfbuzz_dir	"Libraries/3rd/harfbuzz")
set(rel_harfbuzz_source_dir	"${rel_harfbuzz_dir}/src")
set(rel_harfbuzz_include_dir	"${rel_harfbuzz_dir}/src")
set(rel_hunspell_dir	"Libraries/3rd/hunspell")
set(rel_inipool_dir	"BuildUtilities/inipool")
set(rel_initexmf_dir	"Programs/MiKTeX/initexmf")
set(rel_jpeg_dir	"Libraries/3rd/libjpeg")
set(rel_kpsemu_dir	"Libraries/MiKTeX/KPathSeaEmulation")
set(rel_kpsewhich_dir	"${rel_kpsemu_dir}/test/kpsewhich")
set(rel_lua_dir		"Libraries/3rd/lua")
set(rel_lua_include_dir "${rel_lua_dir}/include")
set(rel_luajit_dir	"Libraries/3rd/luajit")
set(rel_luajit_include_dir "${rel_luajit_dir}/include")
set(rel_luatex_dir	"Programs/TexAndFriends/luatex")
set(rel_lzmalib_dir	"${rel_7zip_dir}/CPP/7zip/Compress/LZMA")
set(rel_makeindex_dir	"Programs/Indexing/makeindex")
set(rel_makex_dir	"Programs/MiKTeX/makex")
set(rel_md5_dir		"Libraries/3rd/libmd5")
set(rel_md5walk_dir	"BuildUtilities/md5walk")
set(rel_mf_dir		"Programs/TeXAndFriends/mf")
set(rel_mfware_dir	"Programs/TeXAndFriends/mfware")
set(rel_migrate_dir	"Programs/MiKTeX/migrate")
set(rel_miktexsetup_dir	"Programs/MiKTeX/Setup/miktexsetup")
set(rel_mkfntmap_dir	"Programs/MiKTeX/mkfntmap")
set(rel_mo_mfc_dir	"Programs/MiKTeX/MO/MFC")
set(rel_mp_dir		"Programs/TeXAndFriends/mpost/mplib")
set(rel_mpc_dir		"BuildUtilities/mpc")
set(rel_mpm_dir		"Programs/MiKTeX/PackageManager/mpm")
set(rel_mpm_mfc_dir	"Programs/MiKTeX/PackageManager/MFC")
set(rel_mpm_qt_dir	"Programs/MiKTeX/PackageManager/Qt")
set(rel_mpmlib_dir	"Libraries/MiKTeX/PackageManager")
set(rel_msinttypes_dir	"Libraries/3rd/msinttypes")
set(rel_mspack_dir	"Libraries/3rd/libmspack")
set(rel_mthelp_dir	"Programs/MiKTeX/mthelp")
set(rel_mtprint_dir	"Programs/MiKTeX/mtprint")
set(rel_mtrep_dir	"Libraries/MiKTeX/WebServiceClients/Repository")
set(rel_musixflx_dir	"Programs/Converters/musixflx")
set(rel_netpbm_dir	"Libraries/3rd/netpbm")
set(rel_obsdcompat_dir	"Libraries/3rd/obsdcompat")
set(rel_omega_dir	"Programs/TeXAndFriends/omega/omega")
set(rel_omegaware_dir	"Programs/TeXAndFriends/omega/omegaware")
set(rel_otps_dir	"Programs/TeXAndFriends/omega/otps")
set(rel_padout_dir	"BuildUtilities/padout")
set(rel_pdftex_dir	"Programs/TeXAndFriends/pdftex")
set(rel_pixman_dir	"Libraries/3rd/pixman")
set(rel_pixman_include_dir	"${rel_pixman_dir}/pixman")
set(rel_pixman_source_dir	"${rel_pixman_dir}/pixman")
set(rel_png_dir		"Libraries/3rd/libpng")
set(rel_poppler_dir	"Libraries/3rd/poppler")
set(rel_popt_dir	"Libraries/3rd/libpopt")
set(rel_programs_etc_dir	"Programs/MiKTeX/etc")
set(rel_ps2pk_dir	"Programs/FontUtilities/ps2pk")
set(rel_psutils_dir	"Programs/GraphicsUtilities/psutils")
set(rel_regex_dir	"Libraries/3rd/regex")
set(rel_remove_mfc_dir	"Programs/MiKTeX/Wizards/Remove/MFC")
set(rel_setuplib_dir	"Libraries/MiKTeX/Setup")
set(rel_setup_mfc_dir	"Programs/MiKTeX/Wizards/Setup/MFC")
set(rel_synctex_dir	"Programs/TeXAndFriends/synctex")
set(rel_synctex_include_dir	"${rel_synctex_dir}")
set(rel_taskbar_icon_win_dir "Programs/MiKTeX/Desktop/Taskbar/Status/Icon/win")
set(rel_teckit_dir	"Libraries/3rd/teckit")
set(rel_tex4ht_dir	"Programs/Converters/tex4ht")
set(rel_tex_dir		"Programs/TeXAndFriends/tex")
set(rel_tex_etc_dir	"Programs/TeXAndFriends/etc")
set(rel_texify_dir	"Programs/MiKTeX/texify")
set(rel_texinfo_dir	"Programs/Converters/texinfo")
set(rel_texmf_dir	"Libraries/MiKTeX/TeXAndFriends")
set(rel_texware_dir	"Programs/TeXAndFriends/texware")
set(rel_texworks_dir	"Programs/Editors/TeXworks")
set(rel_tie_dir		"BuildUtilities/tie")
set(rel_tiff_dir	"Libraries/3rd/libtiff")
set(rel_tpm_dir		"Admin/TPM")
set(rel_touch_dir	"BuildUtilities/touch")
set(rel_ui_dir		"Libraries/MiKTeX/UI")
set(rel_ui_mfc_dir	"Libraries/MiKTeX/UI/MFC")
set(rel_ui_qt_dir	"Libraries/MiKTeX/UI/Qt")
set(rel_unxemu_dir	"Libraries/MiKTeX/UnxEmulation")
set(rel_uriparser_dir	"Libraries/3rd/uriparser")
set(rel_update_mfc_dir	"Programs/MiKTeX/Wizards/Update/MFC")
set(rel_utf8wrap_dir	"Libraries/MiKTeX/UTF8Wrap")
set(rel_w2cemu_dir	"Libraries/MiKTeX/Web2CEmulation")
set(rel_web_dir		"Programs/TeXAndFriends/web")
set(rel_xetex_dir	"Programs/TeXAndFriends/xetex")
set(rel_xpdf_dir	"Programs/TeXAndFriends/pdftex/xpdf")
set(rel_xpdf_fofi_dir	"Programs/TeXAndFriends/pdftex/xpdf/fofi")
set(rel_xpdf_goo_dir	"Programs/TeXAndFriends/pdftex/xpdf/goo")
set(rel_xpdf_xpdf_dir	"Programs/TeXAndFriends/pdftex/xpdf/xpdf")
set(rel_yap_mfc_dir	"Programs/MiKTeX/Yap/MFC")
set(rel_zlib_dir	"Libraries/3rd/zlib")
set(rel_zzip_dir	"Libraries/3rd/zzip")

set(rel_css_dir		"${rel_doc_dir}/Styles")
set(rel_doc_api_dir     "${rel_doc_dir}/API")
set(rel_xsl_dir		"${rel_doc_dir}/Styles")

# source files
set(alias_cpp "${CMAKE_SOURCE_DIR}/Libraries/MiKTeX/etc/alias.cpp")
set(entities_ent "${CMAKE_SOURCE_DIR}/${rel_doc_dir}/entities.ent")
set(help_h "${CMAKE_SOURCE_DIR}/${rel_core_dir}/include/miktex/Core/Help")
set(html_xsl "${CMAKE_SOURCE_DIR}/${rel_xsl_dir}/html.xsl")
set(www_xsl "${CMAKE_SOURCE_DIR}/${rel_xsl_dir}/www.xsl")
set(htmlchunk_xsl "${CMAKE_SOURCE_DIR}/${rel_xsl_dir}/htmlchunk.xsl")
set(wwwchunk_xsl "${CMAKE_SOURCE_DIR}/${rel_xsl_dir}/wwwchunk.xsl")
set(htmlhelp_xsl "${CMAKE_SOURCE_DIR}/${rel_xsl_dir}/htmlhelp.xsl")
set(htmltext_xsl "${CMAKE_SOURCE_DIR}/${rel_xsl_dir}/htmltext.xsl")
set(mf_defaults "${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/mf.defaults")
set(miktexhelp_css "${CMAKE_SOURCE_DIR}/${rel_css_dir}/miktexhelp.css")
set(miktexman_css "${CMAKE_SOURCE_DIR}/${rel_css_dir}/miktexman.css")
set(omega_defaults "${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/omega.defaults")
set(pdftex_defaults "${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/pdftex.defaults")
set(tex_web "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex.web")
set(tex98_web "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/legacy/1998/tex.web")
set(wrapper_cpp "${CMAKE_SOURCE_DIR}/Libraries/MiKTeX/etc/wrapper.cpp")
set(xml_catalog_file "${CMAKE_BINARY_DIR}/BuildUtilities/docbook/catalog")

# resources
set(download_avi "${CMAKE_SOURCE_DIR}/Resources/Videos/download.avi")
set(filecopy_avi "${CMAKE_SOURCE_DIR}/Resources/Videos/filecopy.avi")
set(miktex_ico "${CMAKE_SOURCE_DIR}/Resources/Graphics/miktex.ico")
set(wiz97_header_bmp "${CMAKE_SOURCE_DIR}/Resources/Graphics/wiz97-header.bmp")
set(wiz97_watermark_bmp "${CMAKE_SOURCE_DIR}/Resources/Graphics/wiz97-watermark.bmp")

# sed scripts
set(etex_dyn_sed "${CMAKE_SOURCE_DIR}/${rel_etex_dir}/dyn.sed")
set(omega_dyn_sed "${CMAKE_SOURCE_DIR}/${rel_omega_dir}/dyn.sed")
set(pdftex_dyn_sed "${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/dyn.sed")
set(tex_dyn_sed "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/dyn.sed")

# change files
set(enctex_miktex_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/enctex-miktex.ch")
set(etex_ch "${CMAKE_SOURCE_DIR}/${rel_etex_dir}/etex.ch")
set(etex_miktex_ch "${CMAKE_SOURCE_DIR}/${rel_etex_dir}/etex-miktex.ch")
set(etex_miktex_post_ch "${CMAKE_SOURCE_DIR}/${rel_etex_dir}/etex-miktex-post.ch")
set(etex_miktex_pre_ch "${CMAKE_SOURCE_DIR}/${rel_etex_dir}/etex-miktex-pre.ch")
set(mltex_miktex_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/mltex-miktex.ch")
set(tex_miktex_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-misc.ch")
set(tex_miktex_enctex_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-enctex.ch")
set(tex_miktex_hash_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-hash.ch")
set(tex_miktex_hyph_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-hyph.ch")
set(tex_miktex_pool_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-pool.ch")
set(tex_miktex_quiet_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-quiet.ch")
set(tex_miktex_src_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-src.ch")
set(tex_miktex_stat_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-stat.ch")
set(tex_miktex_synctex_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-synctex.ch")
set(tex_miktex_write18_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-write18.ch")
set(tex_miktex_web2c_ch "${CMAKE_SOURCE_DIR}/${rel_tex_dir}/tex-miktex-web2c.ch")
set(vadjust_ch "${CMAKE_SOURCE_DIR}/${rel_pdftex_dir}/vadjust.ch")

#
if(WIN32)
  if(CMAKE_CL_64)
    set(dll_locs "${CMAKE_SOURCE_DIR}/Libraries/MiKTeX/etc/dll_locs-x64.txt")
  else()
    set(dll_locs "${CMAKE_SOURCE_DIR}/Libraries/MiKTeX/etc/dll_locs.txt")
  endif()
endif()

# build utilities
if(WIN32)
  set(exe_suffix ".exe")
else()
  set(exe_suffix "")
endif()
set(bintoc_exe "${outdir}/${CMAKE_CFG_INTDIR}/bintoc${exe_suffix}")
set(c4p_exe "${outdir}/${CMAKE_CFG_INTDIR}/c4p${exe_suffix}")
set(cfg_exe "${outdir}/${CMAKE_CFG_INTDIR}/cfg${exe_suffix}")
set(ctangle_exe "${outdir}/${CMAKE_CFG_INTDIR}/ctangle${exe_suffix}")
set(dvitype_exe "${outdir}/${CMAKE_CFG_INTDIR}/dvitype${exe_suffix}")
set(findtexmf_exe "${outdir}/${CMAKE_CFG_INTDIR}/findtexmf${exe_suffix}")
set(gftype_exe "${outdir}/${CMAKE_CFG_INTDIR}/gftype${exe_suffix}")
set(inipool_exe "${outdir}/${CMAKE_CFG_INTDIR}/inipool${exe_suffix}")
set(mpost_exe "${outdir}/${CMAKE_CFG_INTDIR}/mpost${exe_suffix}")
set(pltotf_exe "${outdir}/${CMAKE_CFG_INTDIR}/pltotf${exe_suffix}")
set(rmtree_perl "${CMAKE_SOURCE_DIR}/${rel_build_tools_etc_dir}/rmtree.perl")
set(runhhc_perl "${CMAKE_BINARY_DIR}/${rel_build_tools_etc_dir}/runhhc.perl")
set(runxsltproc_perl "${CMAKE_BINARY_DIR}/${rel_build_tools_etc_dir}/runxsltproc.perl")
set(tangle_exe "${outdir}/${CMAKE_CFG_INTDIR}/${miktex_prefix}tangle${exe_suffix}")
set(tftopl_exe "${outdir}/${CMAKE_CFG_INTDIR}/${miktex_prefix}tftopl${exe_suffix}")
set(tie_exe "${outdir}/${CMAKE_CFG_INTDIR}/${miktex_prefix}tie${exe_suffix}")
set(trapmf_exe "${outdir}/${CMAKE_CFG_INTDIR}/${miktex_prefix}trapmf${exe_suffix}")
set(triptex_exe "${outdir}/${CMAKE_CFG_INTDIR}/${miktex_prefix}triptex${exe_suffix}")
set(webn_exe "${outdir}/${CMAKE_CFG_INTDIR}/web-n${exe_suffix}")
set(mkdllbases_exe "${outdir}/${CMAKE_CFG_INTDIR}/mkdllbases${exe_suffix}")
set(putj2000version_cmd "${CMAKE_BINARY_DIR}/Admin/Scripts/putj2000version.cmd")
