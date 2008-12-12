## CMakeLists.txt						-*- CMake -*-
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

use_static_crt()
use_static_libraries()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/res/Setup.manifest.in
  ${CMAKE_CURRENT_BINARY_DIR}/res/Setup.manifest
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Setup.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/Setup.rc
)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_MFC_FLAG 1)

set(setup_mfc_sources
  ${CMAKE_CURRENT_BINARY_DIR}/ConnectionSettingsDialog.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/ConnectionSettingsDialog.h
  ${CMAKE_CURRENT_BINARY_DIR}/ProxyAuthenticationDialog.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/ProxyAuthenticationDialog.h
  AddTEXMFPage.cpp
  EditTEXMFPage.cpp
  FileCopyPage.cpp
  FinishPage.cpp
  InfoListPage.cpp
  InstallDirPage.cpp
  LicensePage.cpp
  LocalRepositoryPage.cpp
  MDTaskPage.cpp
  ObsoleteFiles.cpp
  PackageSetPage.cpp
  ProgramFolderPage.cpp
  RemoteRepositoryPage.cpp
  SettingsPage.cpp
  Setup.cpp
  SetupWizard.cpp
  SharedInstallationPage.cpp
  ShellLinkData.cpp
  StdAfx.cpp
  TaskPage.cpp
  WelcomePage.cpp
)

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

target_link_libraries(setup
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
  ${ui_lib_name}
  ${uriparser_lib_name}
  ${zlib_lib_name}
)

if(NATIVE_WINDOWS)
  target_link_libraries(setup
    Wldap32
    fdi
    version
    wininet
    winmm
    ws2_32
  )
endif(NATIVE_WINDOWS)

get_target_property(setup_mfc_exe setup LOCATION)

if(NATIVE_WINDOWS)
  add_custom_command(
    TARGET setup
    POST_BUILD
    COMMAND ${MT_EXE} -nologo
	-manifest ${CMAKE_CURRENT_BINARY_DIR}/res/Setup.manifest
	-updateresource:${setup_mfc_exe}\;1
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
