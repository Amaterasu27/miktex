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

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/res/yap.manifest.in
  ${CMAKE_CURRENT_BINARY_DIR}/res/yap.manifest
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/yap.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/yap.rc
)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_bmeps_dir}
)

set(CMAKE_MFC_FLAG 2)

add_definitions(
  -D_AFXDLL
)

set(yap_mfc_sources
  ${CMAKE_CURRENT_BINARY_DIR}/yaphelp.h
  AddChangeEditorDialog.cpp
  AdvancedOptionsPage.cpp
  ChildFrame.cpp
  Dib.cpp
  DisplayOptionsPage.cpp
  DocumentFontsPage.cpp
  DviDoc.cpp
  DviDraw.cpp
  DviMagnifyingGlass.cpp
  DviStatusBar.cpp
  DviView.cpp
  DvipsDialog.cpp
  GotoPageDialog.cpp
  InverseSearchOptionsPage.cpp
  MagnifyingGlassOptionsPage.cpp
  MainFrame.cpp
  MyScrollView.cpp
  PrintOptionsPage.cpp
  ProgressIndicatorDialog.cpp
  Ruler.cpp
  SecurityOptionsPage.cpp
  SourceSpecialsDialog.cpp
  StdAfx.cpp
  UIOptionsPage.cpp
  YapConfig.cpp
  draw.cpp
  graphics.cpp
  hypertex.cpp
  msdnutil.cpp
  print.cpp
  source.cpp
  tpic.cpp
  winplace.cpp
  yap.cpp
)

if(NATIVE_WINDOWS)
  set(yap_mfc_sources ${yap_mfc_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/yap.rc)
endif(NATIVE_WINDOWS)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/yaphelp.h
  COMMAND
	echo \#define KEYBOARD_SHORTCUTS 101
		> ${CMAKE_CURRENT_BINARY_DIR}/yaphelp.h
  COMMAND
	${MAKEHM_EXE} /h ID_,HID_,0x10000 IDM_,HIDM_,0x10000
		${CMAKE_CURRENT_SOURCE_DIR}/resource.h
		>> ${CMAKE_CURRENT_BINARY_DIR}/yaphelp.h
  COMMAND
	${MAKEHM_EXE} /h IDP_,HIDP_,0x30000
		${CMAKE_CURRENT_SOURCE_DIR}/resource.h
		>> ${CMAKE_CURRENT_BINARY_DIR}/yaphelp.h
  COMMAND
	${MAKEHM_EXE} /h IDR_,HIDR_,0x20000
		${CMAKE_CURRENT_SOURCE_DIR}/resource.h
		>> ${CMAKE_CURRENT_BINARY_DIR}/yaphelp.h
  COMMAND
	${MAKEHM_EXE} /h IDD_,HIDD_,0x20000
		${CMAKE_CURRENT_SOURCE_DIR}/resource.h
		>> ${CMAKE_CURRENT_BINARY_DIR}/yaphelp.h
  COMMAND
	${MAKEHM_EXE} /h IDW_,HIDW_,0x50000
		${CMAKE_CURRENT_SOURCE_DIR}/resource.h
		>> ${CMAKE_CURRENT_BINARY_DIR}/yaphelp.h
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/resource.h
  VERBATIM
)

add_executable(yap WIN32 ${yap_mfc_sources})

merge_trustinfo_manifest(yap asInvoker)

target_link_libraries(yap
  ${core_dll_name}
  ${dvi_dll_name}
  ${popt_dll_name}
  ${ui_dll_name}
)

get_target_property(yap_mfc_exe yap LOCATION)

if(NATIVE_WINDOWS)
  add_custom_command(
    TARGET yap
    POST_BUILD
    COMMAND ${MT_EXE} -nologo
	-manifest ${CMAKE_CURRENT_BINARY_DIR}/res/yap.manifest
	-updateresource:${yap_mfc_exe}\;1
    VERBATIM
  )
endif(NATIVE_WINDOWS)

install(TARGETS yap DESTINATION ${bindir})

set(screenshots_png
  ${CMAKE_CURRENT_SOURCE_DIR}/../Screenshots/propui.png
  ${CMAKE_CURRENT_SOURCE_DIR}/../Screenshots/propdisplay.png
  ${CMAKE_CURRENT_SOURCE_DIR}/../Screenshots/propprinter.png
  ${CMAKE_CURRENT_SOURCE_DIR}/../Screenshots/propmagglass.png
  ${CMAKE_CURRENT_SOURCE_DIR}/../Screenshots/propinvsearch.png
  ${CMAKE_CURRENT_SOURCE_DIR}/../Screenshots/propsecurity.png
  ${CMAKE_CURRENT_SOURCE_DIR}/../Screenshots/propadvanced.png
  ${CMAKE_CURRENT_SOURCE_DIR}/../Screenshots/ruler.png
  ${CMAKE_CURRENT_SOURCE_DIR}/../Screenshots/magglass.png
)

set(yap_guide_xml_files
  ${CMAKE_CURRENT_SOURCE_DIR}/../Documentation/Guide/commandline.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/../Documentation/Guide/customizing.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/../Documentation/Guide/dvisearch.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/../Documentation/Guide/keyboard.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/../Documentation/Guide/menus.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/../Documentation/Guide/printing.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/../Documentation/Guide/tools.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/../Documentation/Guide/yap.xml
)

set(xslt_path "${CMAKE_CURRENT_BINARY_DIR}")
set(xslt_path
  "${xslt_path} ${CMAKE_SOURCE_DIR}/${rel_doc_dir}/Manual")
set(xslt_path
  "${xslt_path} ${CMAKE_SOURCE_DIR}/${rel_doc_dir}")
set(xslt_path
  "${xslt_path} ${CMAKE_CURRENT_SOURCE_DIR}/../Documentation/Guide")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/yap.chm
  COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/${rel_docbook_xsl_dir}/images
		${CMAKE_CURRENT_BINARY_DIR}/scratch/images
  COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/../Screenshots
		${CMAKE_CURRENT_BINARY_DIR}/scratch/images
  COMMAND
	${CMAKE_COMMAND} -E copy
		${miktexhelp_css}
		${CMAKE_CURRENT_BINARY_DIR}/scratch
  COMMAND
	${PERL_EXE} ${runxsltproc_perl}
		--path \"${xslt_path}\"
		--stringparam base.dir scratch/
		--stringparam chunk.fast 1
		--stringparam manifest.in.base.dir 1
		\"${htmlhelp_xsl}\"
		\"${CMAKE_CURRENT_SOURCE_DIR}/../Documentation/Guide/yap.xml\"
  COMMAND
	${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_BINARY_DIR}/yaphelp.h
		${CMAKE_CURRENT_BINARY_DIR}/scratch/context.h
  COMMAND
	${MAKEHM_EXE}
		/h /a afxhh.h IDW_,HIDW_,0x50000
		${CMAKE_CURRENT_SOURCE_DIR}/resource.h
		>> ${CMAKE_CURRENT_BINARY_DIR}/scratch/context.h
  COMMAND
  	${CMAKE_COMMAND} -E
		chdir ${CMAKE_CURRENT_BINARY_DIR}/scratch
		${PERL_EXE} ${runhhc_perl} htmlhelp
  COMMAND
  	${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_BINARY_DIR}/scratch/htmlhelp.chm
		${CMAKE_CURRENT_BINARY_DIR}/yap.chm
  DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/yaphelp.h
	${CMAKE_CURRENT_SOURCE_DIR}/resource.h
	${screenshots_png}
	${yap_guide_xml_files}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
)

add_custom_target(helpfile ALL
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/yap.chm
)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/yap.chm
  DESTINATION ${docdir}
)
