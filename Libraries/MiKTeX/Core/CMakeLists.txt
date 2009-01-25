## CMakeLists.txt						-*- CMake -*-
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

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include/miktex/Core/Version.h.in
  ${CMAKE_BINARY_DIR}/${rel_core_dir}/include/miktex/Core/Version.h
)

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include/miktex/Core/Paths.h.in
  ${CMAKE_BINARY_DIR}/${rel_core_dir}/include/miktex/Core/Paths.h
)

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include/miktex/Core/IntegerTypes.h.cmake
  ${CMAKE_BINARY_DIR}/${rel_core_dir}/include/miktex/Core/IntegerTypes.h
)

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include/miktex/Core/config.h.cmake
  ${CMAKE_BINARY_DIR}/${rel_core_dir}/include/miktex/Core/config.h
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/StdAfx.h
)  

hide_symbols()

add_definitions(-DHAVE_CONFIG_H)

add_definitions(-D_ATL_DLL)

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

set(${core_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/StdAfx.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/miktex/Core/IntegerTypes.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/miktex/Core/Paths.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/miktex/Core/Version.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/miktex/Core/config.h
  CommandLine.cpp
  FileStream.cpp
  FormatInfo.cpp
  LzmaStream.cpp
  MemoryMappedFile.cpp
  Process.cpp
  Thread.cpp
  Uri.cpp
  appnames.cpp
  cfg.cpp
  config.cpp
  core-version.h
  error.cpp
  files.cpp
  filetypes.cpp
  findfile.cpp
  fnamedb.cpp
  fnamedb.h
  fndbmem.h
  fontinfo.cpp
  getdelim.cpp
  graphics.cpp
  gsinfo.cpp
  include/miktex/Core/Core.h
  include/miktex/Core/Debug.h
  include/miktex/Core/Definitions.h
  include/miktex/Core/Environment.h
  include/miktex/Core/First.h
  include/miktex/Core/Help.h
  include/miktex/Core/Registry.h
  include/miktex/Core/Test.h
  include/miktex/Core/Text.h
  include/miktex/Core/Trace.h
  include/miktex/Core/Urls.h
  internal.h
  log.cpp
  makefndb.cpp
  mfmodes.cpp
  miktex.cpp
  obsolete.cpp
  papersize.cpp
  path.cpp
  runbat.cpp
  runperl.cpp
  scratchdir.cpp
  searchpath.cpp
  tempfile.cpp
  texmfroot.cpp
  uncompress.cpp
  util.cpp
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/core.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/core.rc
  )
  set(${core_dll_name}_sources ${${core_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/MiKTeXEvents.h
    ${CMAKE_CURRENT_BINARY_DIR}/core.rc
    include/miktex/Core/win/DllProc.h
    win/DllProc.cpp
    win/win.cpp
    win/winDirectoryLister.cpp
    win/winDirectoryLister.h
    win/winFile.cpp
    win/winMemoryMappedFile.cpp
    win/winMemoryMappedFile.h
    win/winProcess.cpp
    win/winProcess.h
    win/winRegistry.cpp
    win/winRegistry.h
    win/winThread.cpp
    win/winThread.h
  )
  if(HAVE_ATLBASE_H)
    set(sessionidl_h MiKTeX${MIKTEX_SERIES_INT}-sessionidl.h)
    set(session_idl MiKTeX${MIKTEX_SERIES_INT}-session.idl)
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/COM/session.idl.in
      ${CMAKE_CURRENT_BINARY_DIR}/COM/${session_idl}
    )
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/COM/session.rgs.in
      ${CMAKE_CURRENT_BINARY_DIR}/COM/session.rgs
    )
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/COM/comSession.rgs.in
      ${CMAKE_CURRENT_BINARY_DIR}/COM/comSession.rgs
    )
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/miktex)
    add_definitions(-DUSE_LOCAL_SERVER -D_USRDLL -D_WINDLL)
    set(${core_dll_name}_sources ${${core_dll_name}_sources}
      ${CMAKE_CURRENT_BINARY_DIR}/include/miktexidl.h
      ${CMAKE_CURRENT_BINARY_DIR}/${session_tlb_name}.tlb
      ${CMAKE_CURRENT_BINARY_DIR}/session_i.c
      COM/comSession.cpp
      COM/comSession.h
      COM/session.cpp
      COM/session.def
    )
    add_custom_command(
      OUTPUT
		${CMAKE_CURRENT_BINARY_DIR}/include/miktexidl.h
      COMMAND ${MIDL_EXE}
	/nologo
	/I ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include
	/char signed
	/env win32
	/Oicf
	/h ${CMAKE_CURRENT_BINARY_DIR}/include/miktexidl.h
	/no_robust
	${CMAKE_CURRENT_SOURCE_DIR}/include/miktexidl.idl
      DEPENDS
		${CMAKE_CURRENT_SOURCE_DIR}/include/miktexidl.idl
      VERBATIM
    )
    add_custom_command(
      OUTPUT
		${CMAKE_CURRENT_BINARY_DIR}/dlldata.c
		${CMAKE_CURRENT_BINARY_DIR}/${session_tlb_name}.tlb
		${CMAKE_CURRENT_BINARY_DIR}/session_i.c
		${CMAKE_CURRENT_BINARY_DIR}/session_p.c
		${CMAKE_CURRENT_BINARY_DIR}/include/${sessionidl_h}
      COMMAND ${MIDL_EXE}
	/nologo
	/I ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include
	/char signed
	/env win32
	/Oicf
	/tlb ${CMAKE_CURRENT_BINARY_DIR}/${session_tlb_name}.tlb
	/h ${CMAKE_CURRENT_BINARY_DIR}/include/${sessionidl_h}
	/iid ${CMAKE_CURRENT_BINARY_DIR}/session_i.c
	/proxy ${CMAKE_CURRENT_BINARY_DIR}/session_p.c
	/no_robust
	${CMAKE_CURRENT_BINARY_DIR}/COM/${session_idl}
      DEPENDS
		${CMAKE_CURRENT_BINARY_DIR}/COM/${session_idl}
		${CMAKE_CURRENT_SOURCE_DIR}/include/miktexidl.idl
      VERBATIM
    )
    add_library(
      ${core_ps_dll_name}
      SHARED
      ${CMAKE_CURRENT_BINARY_DIR}/dlldata.c
      ${CMAKE_CURRENT_BINARY_DIR}/session_i.c
      ${CMAKE_CURRENT_BINARY_DIR}/session_p.c
      ${CMAKE_CURRENT_BINARY_DIR}/include/${sessionidl_h}
      ${CMAKE_CURRENT_SOURCE_DIR}/COM/ProxyStub.def
    )
    set_source_files_properties(
      ${CMAKE_CURRENT_BINARY_DIR}/dlldata.c
      ${CMAKE_CURRENT_BINARY_DIR}/session_i.c
      ${CMAKE_CURRENT_BINARY_DIR}/session_p.c
      PROPERTIES COMPILE_FLAGS
      "-DREGISTER_PROXY_DLL"
    )
    target_link_libraries(${core_ps_dll_name}
      oleaut32
#      rpcndr
      rpcns4
      rpcrt4
      uuid
    )
    install(TARGETS ${core_ps_dll_name}
      RUNTIME DESTINATION "${bindir}"
      LIBRARY DESTINATION "${libdir}"
      ARCHIVE DESTINATION "${libdir}")
  endif(HAVE_ATLBASE_H)
  set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/MiKTeXEvents.h
    PROPERTIES GENERATED TRUE
  )
  add_custom_command(
    OUTPUT
		${CMAKE_CURRENT_BINARY_DIR}/MiKTeXEvents.h
		${CMAKE_CURRENT_BINARY_DIR}/MiKTeXEvents.rc
		${CMAKE_CURRENT_BINARY_DIR}/MSG00001.bin
    COMMAND mc ${CMAKE_CURRENT_SOURCE_DIR}/MiKTeXEvents.mc
    MAIN_DEPENDENCY MiKTeXEvents.mc
    DEPENDS MiKTeXEvents.mc
    VERBATIM
  )
else(NATIVE_WINDOWS)
  set(${core_dll_name}_sources ${${core_dll_name}_sources}
    unx/unx.cpp
    unx/unxDirectoryLister.cpp
    unx/unxDirectoryLister.h
    unx/unxFile.cpp
    unx/unxMemoryMappedFile.cpp
    unx/unxMemoryMappedFile.h
    unx/unxProcess.cpp
    unx/unxProcess.h
    unx/unxThread.cpp
    unx/unxThread.h
  )
endif(NATIVE_WINDOWS)

add_library(${core_dll_name} SHARED ${${core_dll_name}_sources})

set_target_properties(${core_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_link_libraries(${core_dll_name}
  ${CMAKE_THREAD_LIBS_INIT}
  ${bzip2_dll_name}
  ${lzma_dll_name}
  ${md5_dll_name}
  ${uriparser_dll_name}
  ${zlib_dll_name}
)

if(NATIVE_WINDOWS)
  target_link_libraries(${core_dll_name}
    SHFolder.lib
    shlwapi.lib
    htmlhelp.lib
  )
endif(NATIVE_WINDOWS)

delay_load(${core_dll_name}
  ${bzip2_dll_name}
  ${lzma_dll_name}
  ${uriparser_dll_name}
  ${zlib_dll_name}
)
  
rebase(${core_dll_name})

install(TARGETS ${core_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/include/miktex/Core/IntegerTypes.h
    ${CMAKE_CURRENT_BINARY_DIR}/include/miktex/Core/Paths.h
    ${CMAKE_CURRENT_BINARY_DIR}/include/miktex/Core/Version.h
    ${CMAKE_CURRENT_BINARY_DIR}/include/miktex/Core/config.h
    include/miktex/Core/Core
    include/miktex/Core/Core.h
    include/miktex/Core/Debug
    include/miktex/Core/Debug.h
    include/miktex/Core/Definitions
    include/miktex/Core/Definitions.h
    include/miktex/Core/Environment
    include/miktex/Core/Environment.h
    include/miktex/Core/First
    include/miktex/Core/First.h
    include/miktex/Core/Help
    include/miktex/Core/Help.h
    include/miktex/Core/Paths
    include/miktex/Core/Registry
    include/miktex/Core/Registry.h
    include/miktex/Core/Test
    include/miktex/Core/Test.h
    include/miktex/Core/Text
    include/miktex/Core/Text.h
    include/miktex/Core/Trace
    include/miktex/Core/Trace.h
    include/miktex/Core/Urls
    include/miktex/Core/Urls.h
    include/miktex/Core/Version
  DESTINATION
    ${incdir}/miktex/Core
)

if(NATIVE_WINDOWS)
  install(
    FILES
      include/miktex/Core/win/DllProc
      include/miktex/Core/win/DllProc.h
      include/miktex/Core/win/version.rc
    DESTINATION
      ${incdir}/miktex/Core/win
  )
endif(NATIVE_WINDOWS)

add_subdirectory(test)
