## InstallPaths.cmake
##
## Copyright (C) 2006-2009 Christian Schenk
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
  set(miktex_bindir "${texmfdir}/miktex/bin")
else(NATIVE_WINDOWS AND NOT CMAKE_CL_64)
  set(miktex_bindir "${texmfdir}/miktex/${target_system_tag}/bin")
endif(NATIVE_WINDOWS AND NOT CMAKE_CL_64)

if(NATIVE_WINDOWS)
  set(bindir ${miktex_bindir})
else(NATIVE_WINDOWS)
  set(bindir "bin")
endif(NATIVE_WINDOWS)

set(miktex_internal_bindir "${texmfdir}/miktex/bin/internal")

set(incdir "include")
set(libdir "lib")
set(mandir "share/man")

set(basedir "${texmfdir}/miktex/base")
set(configdir "${texmfdir}/miktex/config")
set(docdir "${texmfdir}/doc/miktex")
set(formatdir "${texmfdir}/miktex/fmt")
set(memdir "${texmfdir}/miktex/mem")
