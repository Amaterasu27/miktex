## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2009-2015 Christian Schenk
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


file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/src/TWVersion.h TeXworks_VER_MAJOR REGEX "VER_MAJOR")
string(REGEX MATCH "([0-9]+)" TeXworks_VER_MAJOR ${TeXworks_VER_MAJOR})
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/src/TWVersion.h TeXworks_VER_MINOR REGEX "VER_MINOR")
string(REGEX MATCH "([0-9]+)" TeXworks_VER_MINOR ${TeXworks_VER_MINOR})
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/src/TWVersion.h TeXworks_VER_PATCH REGEX "VER_BUGFIX")
string(REGEX MATCH "([0-9]+)" TeXworks_VER_PATCH ${TeXworks_VER_PATCH})
set(TeXworks_VERSION ${TeXworks_VER_MAJOR}.${TeXworks_VER_MINOR}.${TeXworks_VER_PATCH})

configure_file(
  GitArchiveInfo.txt
  ${CMAKE_CURRENT_BINARY_DIR}/src/GitRev.h
  COPYONLY
)

file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/src/GitRev.h TeXworks_GIT_INFO)
list(GET TeXworks_GIT_INFO 1 TeXworks_GIT_COMMIT)
string(REGEX REPLACE "#define GIT_COMMIT_HASH \"([a-f0-9]+)\"" "\\1" TeXworks_GIT_HASH "${TeXworks_GIT_COMMIT}")
list(GET TeXworks_GIT_INFO 2 TeXworks_GIT_COMMIT)
string(REGEX REPLACE "#define GIT_COMMIT_DATE \"([-:0-9 Z]+)\"" "\\1" TeXworks_GIT_DATE "${TeXworks_GIT_COMMIT}")


set(CMAKE_AUTOMOC TRUE)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_BINARY_DIR}/src
  ${CMAKE_SOURCE_DIR}/${rel_hunspell_dir}/src/hunspell
)

add_definitions(
  -DHAVE_POPPLER_XPDF_HEADERS
)

add_subdirectory(plugins-src/TWLuaPlugin)
#add_subdirectory(plugins-src/TWPythonPlugin)

set(texworks_sources
  src/ClickableLabel.cpp
  src/ClickableLabel.h
  src/ClosableTabWidget.cpp
  src/ClosableTabWidget.h
  src/CommandlineParser.cpp
  src/CommandlineParser.h
  src/CompletingEdit.cpp
  src/CompletingEdit.h
  src/ConfigurableApp.cpp
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
  src/TWApp.cpp
  src/TWApp.h
  src/TWScript.cpp
  src/TWScript.h
  src/TWScriptAPI.cpp
  src/TWScriptAPI.h
  src/TWScriptable.cpp
  src/TWScriptable.h
  src/TWSynchronizer.cpp
  src/TWSynchronizer.h
  src/TWSystemCmd.cpp
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

  texworks-version.h
  ${CMAKE_CURRENT_BINARY_DIR}/src/GitRev.h
)

if(NATIVE_WINDOWS)
  set(texworks_sources ${texworks_sources}
    ${CMAKE_CURRENT_SOURCE_DIR}/texworks.rc
  )
endif()

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
  res/miktex.qrc
  res/resfiles.qrc
  res/resources.qrc
)

qt5_wrap_ui(texworks_ui_headers ${texworks_ui_files})

qt5_add_resources(texworks_rcc_sources ${texworks_rcs_files})

set(libs
  twluaplugin-static
  #twpythonplugin-static
  ${core_dll_name}
  ${hunspell_dll_name}
  ${poppler_qt_dll_name}
  ${utf8wrap_dll_name}
  ${zlib_dll_name}
  Qt5::Core
  Qt5::Gui
  Qt5::PrintSupport
  Qt5::UiTools
  Qt5::Widgets
  Qt5::Script
  Qt5::ScriptTools
  Qt5::Xml
)

add_executable(texworks WIN32
  ${texworks_sources}
  ${texworks_ui_headers}
  ${texworks_rcc_sources}
)

target_link_libraries(texworks ${libs})

merge_trustinfo_manifest(texworks asInvoker)

install(TARGETS texworks DESTINATION ${bindir})

add_executable(${miktex_prefix}texworks WIN32
  ${texworks_sources}
  ${texworks_ui_headers}
  ${texworks_rcc_sources}
)

target_link_libraries(${miktex_prefix}texworks ${libs})

merge_trustinfo_manifest(${miktex_prefix}texworks asInvoker)

install(TARGETS ${miktex_prefix}texworks DESTINATION ${bindir})
