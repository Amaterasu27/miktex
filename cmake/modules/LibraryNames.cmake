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

## shared library names; follow this naming convention:
##    MiKTeX-NAME-REV
## where:
##    NAME is the component name
##    REV  is the API revision
set(app_dll_name	"MiKTeX-app-1")
set(bmeps_dll_name	"MiKTeX-bmeps-3")
set(bzip2_dll_name	"MiKTeX-bzip2-3")
set(core_dll_name	"MiKTeX-core-3")
set(curl_dll_name	"MiKTeX-curl-1")
set(dib_dll_name	"MiKTeX-dib-3")
set(dvi_dll_name	"MiKTeX-dvi-3")
set(expat_dll_name	"MiKTeX-expat-3")
set(gd_dll_name		"MiKTeX-gd-2")
set(gnu_dll_name	"MiKTeX-gnu-3")
set(jpeg_dll_name	"MiKTeX-jpeg-3")
set(kpsemu_dll_name	"MiKTeX-kpseweb2cemu-3")
set(md5_dll_name	"MiKTeX-md5-3")
set(mpm_dll_name	"MiKTeX-packagemanager-3")
set(mspack_dll_name	"MiKTeX-mspack-1")
set(pbm_dll_name	"MiKTeX-pbm-3")
set(pgm_dll_name	"MiKTeX-pgm-3")
set(png_dll_name	"MiKTeX-png-3")
set(pnm_dll_name	"MiKTeX-pnm-3")
set(popt_dll_name	"MiKTeX-popt-3")
set(ppm_dll_name	"MiKTeX-ppm-3")
set(regex_dll_name	"MiKTeX-regex-3")
set(tangle_dll_name	"MiKTeX-tangle-3")
set(texmf_dll_name	"MiKTeX-texmf-3")
set(tiff_dll_name	"MiKTeX-tiff-3")
set(ui_dll_name		"MiKTeX-ui-3")
set(weave_dll_name	"MiKTeX-weave-3")
set(zlib_dll_name	"MiKTeX-zlib-3")
set(makex_dll_name	"MiKTeX-make-3")

## static library names
set(bzip2_lib_name	"bzip2-static")
set(core_lib_name	"core-static")
set(curl_lib_name	"curl-static")
set(expat_lib_name	"expat-static")
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
