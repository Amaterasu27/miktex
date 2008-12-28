## InstallPaths.cmake
##
## Copyright (C) 2006-2008 Christian Schenk
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
  set(texmfdir "${CMAKE_INSTALL_PREFIX}/texmf/")
  set(bindir "${texmfdir}/miktex/bin")
  if(CMAKE_CL_64)
    set(bindir "${bindir}/x64")
  else(CMAKE_CL_64)
#   set(bindir "${bindir}/x86")
  endif(CMAKE_CL_64)
else(NATIVE_WINDOWS)
  set(texmfdir "${CMAKE_INSTALL_PREFIX}/share/${miktex_prefix}texmf")
  set(bindir "${CMAKE_INSTALL_PREFIX}/bin")
endif(NATIVE_WINDOWS)

set(incdir "${CMAKE_INSTALL_PREFIX}/include")
set(libdir "${CMAKE_INSTALL_PREFIX}/lib")
set(mandir "${CMAKE_INSTALL_PREFIX}/share/man")

set(basedir "${texmfdir}/miktex/base")
set(configdir "${texmfdir}/miktex/config")
set(docdir "${texmfdir}/doc/miktex")
set(formatdir "${texmfdir}/miktex/fmt")
set(memdir "${texmfdir}/miktex/mem")
