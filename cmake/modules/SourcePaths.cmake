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
set(rel_bmeps_dir	"Libraries/3rd/libbmeps")
set(rel_build_tools_etc_dir	"BuildUtilities/etc")
set(rel_bzip2_dir	"Libraries/3rd/bzip2")
set(rel_c4p_dir		"BuildUtilities/c4p")
set(rel_core_dir	"Libraries/MiKTeX/Core")
set(rel_core_inc_dir	"${rel_core_dir}/include/miktex")
set(rel_curl_dir	"Libraries/3rd/libcurl")
set(rel_cweb_dir	"BuildUtilities/cweb")
set(rel_expat_dir	"Libraries/3rd/libexpat")
set(rel_fofi_dir	"Programs/TeXAndFriends/pdftex/xpdf/fofi")
set(rel_gd_dir		"Libraries/3rd/libgd")
set(rel_gnu_dir		"Libraries/3rd/libgnu")
set(rel_goo_dir		"Programs/TeXAndFriends/pdftex/xpdf/goo")
set(rel_gsoap_dir	"Libraries/3rd/gsoap")
set(rel_jpeg_dir	"Libraries/3rd/libjpeg")
set(rel_md5_dir		"Libraries/3rd/libmd5")
set(rel_mpmlib_dir	"Libraries/MiKTeX/PackageManager")
set(rel_mspack_dir	"Libraries/3rd/libmspack")
set(rel_mtrep_dir	"Libraries/MiKTeX/WebServiceClients/Repository")
set(rel_netpbm_dir	"Libraries/3rd/netpbm")
set(rel_png_dir		"Libraries/3rd/libpng")
set(rel_popt_dir	"Libraries/3rd/libpopt")
set(rel_tie_dir		"BuildUtilities/tie")
set(rel_tiff_dir	"Libraries/3rd/libtiff")
set(rel_tpm_dir		"Admin/TPM")
set(rel_ui_mfc_dir	"Libraries/MiKTeX/UI/MFC")
set(rel_xpdf_dir	"Programs/TeXAndFriends/pdftex/xpdf/xpdf")
set(rel_zlib_dir	"Libraries/3rd/zlib")

# built executables
set(ctangle_exe		"${CMAKE_BINARY_DIR}/${rel_cweb_dir}/ctangle")
