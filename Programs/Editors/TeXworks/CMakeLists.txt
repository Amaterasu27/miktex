## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2009-2013 Christian Schenk
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

add_subdirectory(plugins-src/TWLuaPlugin)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_BINARY_DIR}/src
  ${CMAKE_BINARY_DIR}/${rel_poppler_dir}/poppler
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler
  ${CMAKE_BINARY_DIR}/${rel_poppler_dir}
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/qt4/src
  ${CMAKE_SOURCE_DIR}/${rel_hunspell_dir}/src/hunspell
)

add_definitions(
  -DHAVE_POPPLER_XPDF_HEADERS
)

set(texworks_sources
  src/ClickableLabel.h
  src/ClosableTabWidget.cpp
  src/ClosableTabWidget.h
  src/CommandlineParser.cpp
  src/CommandlineParser.h
  src/CompletingEdit.cpp
  src/CompletingEdit.h
  src/ConfigurableApp.h
  src/ConfirmDelete.cpp
  src/ConfirmDelete.h
  src/DefaultBinaryPathsWin.h
  src/DefaultPrefs.h
  src/FindDialog.cpp
  src/FindDialog.h
  src/HardWrapDialog.cpp
  src/HardWrapDialog.h
  src/PDFDocks.cpp
  src/PDFDocks.h
  src/PDFDocument.cpp
  src/PDFDocument.h
  src/PrefsDialog.cpp
  src/PrefsDialog.h
  src/ResourcesDialog.cpp
  src/ResourcesDialog.h
  src/ScriptManager.cpp
  src/ScriptManager.h
  src/SvnRev.h
  src/TWApp.cpp
  src/TWApp.h
  src/TWScript.cpp
  src/TWScript.h
  src/TWScriptAPI.cpp
  src/TWScriptAPI.h
  src/TWScriptable.cpp
  src/TWScriptable.h
  src/TWSystemCmd.h
  src/TWTextCodecs.cpp
  src/TWTextCodecs.h
  src/TWUtils.cpp
  src/TWUtils.h
  src/TWVersion.h
  src/TeXDocks.cpp
  src/TeXDocks.h
  src/TeXDocument.cpp
  src/TeXDocument.h
  src/TeXHighlighter.cpp
  src/TeXHighlighter.h
  src/TemplateDialog.cpp
  src/TemplateDialog.h
  src/main.cpp
  src/synctex_parser.c
  src/synctex_parser.h
  src/synctex_parser_utils.c
  src/synctex_parser_utils.h
)

if(NATIVE_WINDOWS)
  set(texworks_sources ${texworks_sources}
    ${CMAKE_CURRENT_SOURCE_DIR}/texworks.rc
  )
#  add_custom_command(
#    OUTPUT
#      ${CMAKE_CURRENT_BINARY_DIR}/appicon.ico
#    COMMAND
#      ${IMAGEMAGICK_CONVERT_EXECUTABLE}
#        -verbose
#        ${CMAKE_CURRENT_SOURCE_DIR}/res/images/appicon.png
#	${CMAKE_CURRENT_BINARY_DIR}/appicon.ico
#    DEPENDS
#      ${CMAKE_CURRENT_SOURCE_DIR}/res/images/appicon.png
#    VERBATIM
#  )
#  add_custom_command(
#    OUTPUT
#      ${CMAKE_CURRENT_BINARY_DIR}/texdoc.ico
#    COMMAND
#      ${IMAGEMAGICK_CONVERT_EXECUTABLE}
#        -verbose
#        ${CMAKE_CURRENT_SOURCE_DIR}/res/images/texdoc.png
#	${CMAKE_CURRENT_BINARY_DIR}/texdoc.ico
#    DEPENDS
#      ${CMAKE_CURRENT_SOURCE_DIR}/res/images/texdoc.png
#    VERBATIM
#  )
#  add_custom_target(
#    texworks_icons
#    ALL
#    DEPENDS
#      ${CMAKE_CURRENT_BINARY_DIR}/appicon.ico
#      ${CMAKE_CURRENT_BINARY_DIR}/texdoc.ico
#  )
endif(NATIVE_WINDOWS)

set(texworks_moc_sources
  src/ClickableLabel.h
  src/ClosableTabWidget.h
  src/CompletingEdit.h
  src/ConfigurableApp.h
  src/ConfirmDelete.h
  src/FindDialog.h
  src/HardWrapDialog.h
  src/PDFDocks.h
  src/PDFDocument.h
  src/PrefsDialog.h
  src/ResourcesDialog.h
  src/ScriptManager.h
  src/TWApp.h
  src/TWScript.h
  src/TWScriptAPI.h
  src/TWScriptable.h
  src/TWSystemCmd.h
  src/TWUtils.h
  src/TeXDocks.h
  src/TeXDocument.h
  src/TeXHighlighter.h
  src/TemplateDialog.h
)

set(texworks_ui_files
  src/ConfirmDelete.ui
  src/Find.ui
  src/HardWrapDialog.ui
  src/PDFDocument.ui
  src/PDFFind.ui
  src/PrefsDialog.ui
  src/Replace.ui
  src/ResourcesDialog.ui
  src/ScriptManager.ui
  src/SearchResults.ui
  src/TeXDocument.ui
  src/TemplateDialog.ui
  src/ToolConfig.ui
)

set(texworks_rcs_files
  res/resources.qrc
  res/resfiles.qrc
  res/miktex.qrc
)

qt4_wrap_cpp(texworks_mocs ${texworks_moc_sources})

qt4_wrap_ui(texworks_ui_headers ${texworks_ui_files})

qt4_wrap_cpp(texworks_ui_mocs ${texworks_ui_headers})

qt4_add_resources(texworks_rcc_sources ${texworks_rcs_files})

  set(libs
    ${core_dll_name}
    ${hunspell_dll_name}
    ${poppler_qt4_dll_name}
    ${utf8wrap_dll_name}
    ${zlib_dll_name}
    ${QT_QTMAIN_LIBRARY}
    ${QT_LIBRARIES}
  )

add_executable(texworks WIN32
  ${texworks_sources}
  ${texworks_mocs}
  ${texworks_ui_mocs}
  ${texworks_rcc_sources}
)

target_link_libraries(texworks ${libs})

merge_trustinfo_manifest(texworks asInvoker)

install(TARGETS texworks DESTINATION ${bindir})

add_executable(${miktex_prefix}texworks WIN32
  ${texworks_sources}
  ${texworks_mocs}
  ${texworks_ui_mocs}
  ${texworks_rcc_sources}
)

target_link_libraries(${miktex_prefix}texworks ${libs})

merge_trustinfo_manifest(${miktex_prefix}texworks asInvoker)

install(TARGETS ${miktex_prefix}texworks DESTINATION ${bindir})

#if(NATIVE_WINDOWS)
#  add_dependencies(texworks texworks_icons)
#  add_dependencies(${miktex_prefix}texworks texworks_icons)
#endif(NATIVE_WINDOWS)
