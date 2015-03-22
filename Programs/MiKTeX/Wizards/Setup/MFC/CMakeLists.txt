## CMakeLists.txt						-*- CMake -*-
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

use_static_crt()
use_static_libraries()

set(ENABLE_ADDTEXMF FALSE)
set(SHOW_FOLDER_PAGE FALSE)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/res/Setup.manifest.in
  ${CMAKE_CURRENT_BINARY_DIR}/res/Setup.manifest
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Setup.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/Setup.rc
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

include_directories(
  ${CMAKE_SOURCE_DIR}/${rel_setuplib_dir}/include
  ${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_MFC_FLAG 1)

add_definitions(
  -D_UNICODE -DUNICODE
)

set(setup_mfc_sources
  ${CMAKE_CURRENT_BINARY_DIR}/ConnectionSettingsDialog.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/ConnectionSettingsDialog.h
  ${CMAKE_CURRENT_BINARY_DIR}/ProxyAuthenticationDialog.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/ProxyAuthenticationDialog.h
  FileCopyPage.cpp
  FileCopyPage.h
  FinishPage.cpp
  FinishPage.h
  InfoListPage.cpp
  InfoListPage.h
  InstallDirPage.cpp
  InstallDirPage.h
  LicensePage.cpp
  LicensePage.h
  LocalRepositoryPage.cpp
  LocalRepositoryPage.h
  MDTaskPage.cpp
  MDTaskPage.h
  PackageSetPage.cpp
  PackageSetPage.h
  ProgramFolderPage.cpp
  ProgramFolderPage.h
  RemoteRepositoryPage.cpp
  RemoteRepositoryPage.h
  SettingsPage.cpp
  SettingsPage.h
  Setup-version.h
  Setup.cpp
  Setup.h
  SetupWizard.cpp
  SetupWizard.h
  SharedInstallationPage.cpp
  SharedInstallationPage.h
  StdAfx.cpp
  StdAfx.h
  TaskPage.cpp
  TaskPage.h
  WelcomePage.cpp
  WelcomePage.h
  resource.h
  targetver.h
)

if(ENABLE_ADDTEXMF)
  set(setup_mfc_sources ${setup_mfc_sources}
    AddTEXMFPage.cpp
    AddTEXMFPage.h
    EditTEXMFPage.cpp
    EditTEXMFPage.h
  )
endif(ENABLE_ADDTEXMF)

foreach(f
    ConnectionSettingsDialog.cpp
    ConnectionSettingsDialog.h
    ProxyAuthenticationDialog.cpp
    ProxyAuthenticationDialog.h)
  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/${f}
    COMMAND
	${CMAKE_COMMAND} -E copy
		${CMAKE_SOURCE_DIR}/${rel_ui_mfc_dir}/${f}
		${CMAKE_CURRENT_BINARY_DIR}/${f}
    DEPENDS
	${CMAKE_SOURCE_DIR}/${rel_ui_mfc_dir}/${f}
    VERBATIM
  )
endforeach(f)

if(NATIVE_WINDOWS)
  set(setup_mfc_sources ${setup_mfc_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/Setup.rc)
endif(NATIVE_WINDOWS)

add_executable(setup WIN32 ${setup_mfc_sources})

merge_trustinfo_manifest(setup highestAvailable)

if(NATIVE_WINDOWS)
  add_link_flags(setup "/ENTRY:\"wWinMainCRTStartup\"")
endif(NATIVE_WINDOWS)

target_link_libraries(setup
  ${botan_lib_name}
  ${bzip2_lib_name}
  ${core_lib_name}
  ${curl_lib_name}
  ${expat_lib_name}
  ${extractor_lib_name}
  ${getopt_lib_name}
  ${lzma_lib_name}
  ${md5_lib_name}
  ${mpm_lib_name}
  ${mspack_lib_name}
  ${setup_lib_name}
  ${uriparser_lib_name}
  ${utf8wrap_lib_name}
  ${zlib_lib_name}
)

if(NATIVE_WINDOWS)
  target_link_libraries(setup
    version
    Wldap32
    wininet
    winmm
    ws2_32
  )
endif(NATIVE_WINDOWS)

if(NATIVE_WINDOWS)
  add_custom_command(
    TARGET setup
    POST_BUILD
    COMMAND ${MT_EXE} -nologo
	-manifest ${CMAKE_CURRENT_BINARY_DIR}/res/Setup.manifest
	-updateresource:$<TARGET_FILE:setup>\;1
    VERBATIM
  )
endif(NATIVE_WINDOWS)

install(TARGETS setup DESTINATION ${bindir})

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/LICENSE.TXT
  COMMAND
	${PERL_EXE} ${runxsltproc_perl}
		--output tmp.html
		${htmltext_xsl}
		${CMAKE_SOURCE_DIR}/${rel_doc_dir}/license.xml
  COMMAND
	${LYNX_EXE} -dump -hiddenlinks=ignore -nolist
		tmp.html
		> ${CMAKE_CURRENT_BINARY_DIR}/LICENSE.TXT
  DEPENDS
	${htmltext_xsl}
	${CMAKE_SOURCE_DIR}/${rel_doc_dir}/license.xml
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
)

add_custom_target(makelicense ALL
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/LICENSE.TXT
)
