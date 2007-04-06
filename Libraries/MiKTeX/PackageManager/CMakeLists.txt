## CMakeLists.txt						-*- CMake -*-
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

add_definitions(-DHAVE_LIBCURL)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_gsoap_dir}
)

set(${mpm_dll_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_gsoap_dir}/stdsoap2.cpp
  ${CMAKE_SOURCE_DIR}/${rel_gsoap_dir}/stdsoap2.h
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/RepositorySoap.nsmap
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryC.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryH.h
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryRepositorySoapProxy.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryRepositorySoapProxy.h
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryStub.h
  CurlWebFile.cpp
  CurlWebFile.h
  CurlWebSession.cpp
  CurlWebSession.h
  PackageInstaller.cpp
  PackageIterator.cpp
  PackageManager.cpp
  StdAfx.h
  TpmParser.cpp
  TpmParser.h
  WebFile.h
  WebSession.cpp
  WebSession.h
  include/miktex/mpm.h
  internal.h
  mpm-version.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/mpm.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/mpm.rc
  )
  set(${mpm_dll_name}_sources ${${mpm_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/mpm.rc
    win/winWebFile.cpp
    win/winWebFile.h
    win/winWebSession.cpp
    win/winWebSession.h
  )
  if(MFC_FOUND)
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/COM/mpm.rgs.in
      ${CMAKE_CURRENT_BINARY_DIR}/COM/mpm.rgs
    )
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/miktex)
    add_definitions(-DUSE_LOCAL_SERVER -D_USRDLL -D_WINDLL -D_ATL_DLL)
    set(${mpm_dll_name}_sources ${${mpm_dll_name}_sources}
      ${CMAKE_CURRENT_BINARY_DIR}/mpm.tlb
      ${CMAKE_CURRENT_BINARY_DIR}/mpm_i.c
      COM/comPackageInstaller.cpp
      COM/comPackageInstaller.h
      COM/comPackageManager.cpp
      COM/comPackageManager.h
      COM/mpm.cpp
      COM/mpm.def
    )
    add_custom_command(
      OUTPUT
		${CMAKE_CURRENT_BINARY_DIR}/dlldata.c
		${CMAKE_CURRENT_BINARY_DIR}/mpm.tlb
		${CMAKE_CURRENT_BINARY_DIR}/mpm_i.c
		${CMAKE_CURRENT_BINARY_DIR}/mpm_p.c
		${CMAKE_CURRENT_BINARY_DIR}/include/mpmidl.h
      COMMAND ${MIDL_EXE}
	/nologo
	/I ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include
	/char signed
	/env win32
	/Oicf
	/tlb ${CMAKE_CURRENT_BINARY_DIR}/mpm.tlb
	/h ${CMAKE_CURRENT_BINARY_DIR}/include/mpmidl.h
	/iid ${CMAKE_CURRENT_BINARY_DIR}/mpm_i.c
	/proxy ${CMAKE_CURRENT_BINARY_DIR}/mpm_p.c
	/no_robust
	${CMAKE_CURRENT_SOURCE_DIR}/COM/mpm.idl
      DEPENDS
		${CMAKE_CURRENT_SOURCE_DIR}/COM/mpm.idl
      VERBATIM
    )
    add_library(
      ${mpm_ps_dll_name}
      SHARED
      ${CMAKE_CURRENT_BINARY_DIR}/dlldata.c
      ${CMAKE_CURRENT_BINARY_DIR}/mpm_i.c
      ${CMAKE_CURRENT_BINARY_DIR}/mpm_p.c
      ${CMAKE_CURRENT_BINARY_DIR}/include/mpmidl.h
      ${CMAKE_CURRENT_SOURCE_DIR}/COM/ProxyStub.def
    )
    set_source_files_properties(
      ${CMAKE_CURRENT_BINARY_DIR}/dlldata.c
      ${CMAKE_CURRENT_BINARY_DIR}/mpm_i.c
      ${CMAKE_CURRENT_BINARY_DIR}/mpm_p.c
      PROPERTIES COMPILE_FLAGS
      "-DREGISTER_PROXY_DLL"
    )
    target_link_libraries(${mpm_ps_dll_name}
      oleaut32
      rpcndr
      rpcns4
      rpcrt4
      uuid
    )
    install(TARGETS ${mpm_ps_dll_name} DESTINATION "${bindir}")
  endif(MFC_FOUND)
endif(NATIVE_WINDOWS)

add_library(${mpm_dll_name} SHARED ${${mpm_dll_name}_sources})

target_link_libraries(${mpm_dll_name}
  ${core_dll_name}
  ${expat_dll_name}
  ${extractor_dll_name}
  ${md5_dll_name}
)

if(NATIVE_WINDOWS)
  target_link_libraries(${mpm_dll_name}
    ${curl_dll_name}
    Ws2_32
    wininet
  )
else(NATIVE_WINDOWS)
  target_link_libraries(${mpm_dll_name}
    ${CURL_LIBRARIES}
  )
endif(NATIVE_WINDOWS)

install(TARGETS ${mpm_dll_name} DESTINATION "${bindir}")
