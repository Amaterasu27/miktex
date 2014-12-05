## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2014 Christian Schenk
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

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/StdAfx.h
)  

add_definitions(-DHAVE_LIBCURL)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_gsoap_dir}
)

set(${mpm_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/StdAfx.h
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
  TpmParser.cpp
  TpmParser.h
  WebFile.h
  WebSession.cpp
  WebSession.h
  include/miktex/PackageManager/PackageManager.h
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
  if(HAVE_ATLBASE_H)
    set(mpmidl_h MiKTeX${MIKTEX_SERIES_INT}-packagemanageridl.h)
    set(mpm_idl MiKTeX${MIKTEX_SERIES_INT}-packagemanager.idl)
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/COM/mpm.idl.in
      ${CMAKE_CURRENT_BINARY_DIR}/COM/${mpm_idl}
    )
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/COM/mpm.rgs.in
      ${CMAKE_CURRENT_BINARY_DIR}/COM/mpm.rgs
    )
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/COM/comPackageManager.rgs.in
      ${CMAKE_CURRENT_BINARY_DIR}/COM/comPackageManager.rgs
    )
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/miktex)
    add_definitions(-DUSE_LOCAL_SERVER -D_USRDLL -D_WINDLL)
    set(${mpm_dll_name}_sources ${${mpm_dll_name}_sources}
      ${CMAKE_CURRENT_BINARY_DIR}/${mpm_tlb_name}.tlb
      ${CMAKE_CURRENT_BINARY_DIR}/mpm_i.c
      COM/comPackageInstaller.cpp
      COM/comPackageInstaller.h
      COM/comPackageManager.cpp
      COM/comPackageManager.h
      COM/comPackageIterator.cpp
      COM/comPackageIterator.h
      COM/mpm.cpp
      COM/mpm.def
    )
    if(CMAKE_CL_64)
      set(env "amd64")
    else(CMAKE_CL_64)
      set(env "win32")
    endif(CMAKE_CL_64)
    add_custom_command(
      OUTPUT
		${CMAKE_CURRENT_BINARY_DIR}/dlldata.c
		${CMAKE_CURRENT_BINARY_DIR}/${mpm_tlb_name}.tlb
		${CMAKE_CURRENT_BINARY_DIR}/mpm_i.c
		${CMAKE_CURRENT_BINARY_DIR}/mpm_p.c
		${CMAKE_CURRENT_BINARY_DIR}/include/${mpmidl_h}
      COMMAND ${MIDL_EXE}
	/nologo
	/I ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include
	/char signed
	/env ${env}
	/Oicf
	/tlb ${CMAKE_CURRENT_BINARY_DIR}/${mpm_tlb_name}.tlb
	/h ${CMAKE_CURRENT_BINARY_DIR}/include/${mpmidl_h}
	/iid ${CMAKE_CURRENT_BINARY_DIR}/mpm_i.c
	/proxy ${CMAKE_CURRENT_BINARY_DIR}/mpm_p.c
	/no_robust
	${CMAKE_CURRENT_BINARY_DIR}/COM/${mpm_idl}
      DEPENDS
		${CMAKE_CURRENT_BINARY_DIR}/COM/${mpm_idl}
      VERBATIM
    )
    add_library(
      ${mpm_ps_dll_name}
      SHARED
      ${CMAKE_CURRENT_BINARY_DIR}/dlldata.c
      ${CMAKE_CURRENT_BINARY_DIR}/mpm_i.c
      ${CMAKE_CURRENT_BINARY_DIR}/mpm_p.c
      ${CMAKE_CURRENT_BINARY_DIR}/include/${mpmidl_h}
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
      PRIVATE
        oleaut32
        rpcns4
	rpcrt4
	uuid
    )
    install(TARGETS ${mpm_ps_dll_name} DESTINATION "${bindir}")
  endif(HAVE_ATLBASE_H)
endif(NATIVE_WINDOWS)

if(NOT LINK_EVERYTHING_STATICALLY)
  add_library(${mpm_dll_name} SHARED ${${mpm_dll_name}_sources})

  set_target_properties(${mpm_dll_name}
    PROPERTIES
      VERSION "${MIKTEX_SERIES_STR}"
      SOVERSION "1"
  )

  target_link_libraries(${mpm_dll_name}
    PRIVATE  
      ${core_dll_name}
      ${expat_dll_name}
      ${extractor_dll_name}
      ${md5_dll_name}
  )

  if(NATIVE_WINDOWS)
    target_link_libraries(${mpm_dll_name}
      PRIVATE
        ${curl_dll_name}
	Ws2_32
	wininet
    )
  else(NATIVE_WINDOWS)
    target_link_libraries(${mpm_dll_name}
      PRIVATE
        ${CURL_LIBRARIES}
    )
  endif(NATIVE_WINDOWS)

  delay_load(${mpm_dll_name}
    ${curl_dll_name}
    ${extractor_dll_name}
    Ws2_32
    wininet
  )

  rebase(${mpm_dll_name})

  install(TARGETS ${mpm_dll_name}
    RUNTIME DESTINATION "${bindir}"
    LIBRARY DESTINATION "${libdir}"
    ARCHIVE DESTINATION "${libdir}"
  )
endif(NOT LINK_EVERYTHING_STATICALLY)

if(INSTALL_MIKTEX_HEADERS)
  install(
    FILES
      include/miktex/PackageManager/PackageManager
      include/miktex/PackageManager/PackageManager.h
    DESTINATION
      ${incdir}/miktex/PackageManager
  )
endif(INSTALL_MIKTEX_HEADERS)
