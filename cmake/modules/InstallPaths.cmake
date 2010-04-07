## InstallPaths.cmake
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

if(NATIVE_WINDOWS)
  set(texmfdir "texmf")
else(NATIVE_WINDOWS)
  set(texmfdir "share/${miktex_prefix}texmf")
endif(NATIVE_WINDOWS)

if(NATIVE_WINDOWS AND NOT CMAKE_CL_64)
  set(rel_miktex_bin_dir "miktex/bin")
elseif(NATIVE_WINDOWS AND CMAKE_CL_64)
  set(rel_miktex_bin_dir "miktex/bin/x64")
else(NATIVE_WINDOWS AND NOT CMAKE_CL_64)
  set(rel_miktex_bin_dir "miktex/bin/${target_system_tag}")
endif(NATIVE_WINDOWS AND NOT CMAKE_CL_64)

set(miktex_bindir "${texmfdir}/${rel_miktex_bin_dir}")

set(rel_miktex_internal_bin_dir "${rel_miktex_bin_dir}/internal")

set(miktex_internal_bindir "${texmfdir}/${rel_miktex_internal_bin_dir}")

if(NATIVE_WINDOWS)
  set(bindir ${miktex_bindir})
else(NATIVE_WINDOWS)
  set(bindir "bin")
endif(NATIVE_WINDOWS)

set(incdir "include")
set(libdir "lib")
set(mandir "share/man")

if(MIKTEX_WORDS_BIGENDIAN)
  set(rel_miktex_base_dir "miktex/data/be")
  set(rel_miktex_fmt_dir "miktex/data/be")
  set(rel_miktex_fndb_dir "miktex/data/be")
  set(rel_miktex_mem_dir "miktex/data/be")
else(MIKTEX_WORDS_BIGENDIAN)
  set(rel_miktex_base_dir "miktex/data/le")
  set(rel_miktex_fmt_dir "miktex/data/le")
  set(rel_miktex_fndb_dir "miktex/data/le")
  set(rel_miktex_mem_dir "miktex/data/le")
endif(MIKTEX_WORDS_BIGENDIAN)

set(basedir "${texmfdir}/${rel_miktex_base_dir}")
set(formatdir "${texmfdir}/${rel_miktex_fmt_dir}")
set(memdir "${texmfdir}/${rel_miktex_mem_dir}")

set(configdir "${texmfdir}/miktex/config")
set(docdir "${texmfdir}/doc/miktex")
