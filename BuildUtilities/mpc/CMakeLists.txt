## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2012 Christian Schenk
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

use_static_crt()
use_static_libraries()

set(mpc_sources
  StdAfx.cpp
  mpc.cpp
)

if(NATIVE_WINDOWS)
  set(mpc_sources ${mpc_sources} mpc.rc)
endif(NATIVE_WINDOWS)

add_executable(mpc ${mpc_sources})

merge_trustinfo_manifest(mpc asInvoker)

if(NATIVE_WINDOWS)
  target_link_libraries(mpc
    ${bzip2_lib_name}
    ${botan_lib_name}
    ${core_lib_name}
    ${curl_lib_name}
    ${expat_lib_name}
    ${extractor_lib_name}
    ${getopt_lib_name}
    ${lzma_lib_name}
    ${md5_lib_name}
    ${mpm_lib_name}
    ${mspack_lib_name}
    ${popt_lib_name}
    ${uriparser_lib_name}
    ${utf8wrap_lib_name}
    ${zlib_lib_name}
  )
else(NATIVE_WINDOWS)
  target_link_libraries(mpc
    ${core_dll_name}
    ${mpm_dll_name}
    ${popt_dll_name}
  )
endif(NATIVE_WINDOWS)

if(NATIVE_WINDOWS)
  target_link_libraries(mpc
    Wldap32
    shlwapi
    version
    wininet
    winmm
    ws2_32
  )
endif(NATIVE_WINDOWS)

install(TARGETS mpc DESTINATION ${bindir})
