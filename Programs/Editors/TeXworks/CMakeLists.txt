## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2009 Christian Schenk
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

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/qt4/src
  ${CMAKE_SOURCE_DIR}/${rel_hunspell_dir}/src/hunspell
)

set(texworks_sources
  src/CompletingEdit.cpp
  src/ConfirmDelete.cpp
  src/FindDialog.cpp
  src/HardWrapDialog.cpp
  src/PDFDocks.cpp
  src/PDFDocument.cpp
  src/PrefsDialog.cpp
  src/TWApp.cpp
  src/TWUtils.cpp
  src/TeXDocks.cpp
  src/TeXDocument.cpp
  src/TeXHighlighter.cpp
  src/TemplateDialog.cpp
  src/main.cpp
  src/synctex_parser.c
)

if(NATIVE_WINDOWS)
  set(texworks_sources ${texworks_sources}
    ${CMAKE_CURRENT_SOURCE_DIR}/texworks.rc
  )
endif(NATIVE_WINDOWS)

set(texworks_moc_sources
  src/CompletingEdit.h
  src/ConfirmDelete.h
  src/FindDialog.h
  src/HardWrapDialog.h
  src/PDFDocks.h
  src/PDFDocument.h
  src/PrefsDialog.h
  src/TWApp.h
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
  src/SearchResults.ui
  src/TeXDocument.ui
  src/TemplateDialog.ui
  src/ToolConfig.ui
)

set(texworks_rcs_files
  res/resources.qrc
  res/resfiles.qrc
)

qt4_wrap_cpp(texworks_mocs ${texworks_moc_sources})

qt4_wrap_ui(texworks_ui_headers ${texworks_ui_files})

qt4_wrap_cpp(texworks_ui_mocs ${texworks_ui_headers})

qt4_add_resources(texworks_rcc_sources ${texworks_rcs_files})

if(LINK_EVERYTHING_STATICALLY)
  set(libs ${MIKTEX_STATIC_LINK_LIBRARIES})
else(LINK_EVERYTHING_STATICALLY)
  set(libs
    ${core_dll_name}
    ${hunspell_dll_name}
    ${poppler_qt4_dll_name}
    ${QT_LIBRARIES}
  )
endif(LINK_EVERYTHING_STATICALLY)

add_executable(texworks
  ${texworks_sources}
  ${texworks_mocs}
  ${texworks_ui_mocs}
  ${texworks_rcc_sources}
)

target_link_libraries(texworks ${libs})

merge_trustinfo_manifest(texworks asInvoker)

install(TARGETS texworks DESTINATION ${bindir})

add_executable(${miktex_prefix}texworks
  ${texworks_sources}
  ${texworks_mocs}
  ${texworks_ui_mocs}
  ${texworks_rcc_sources}
)

target_link_libraries(${miktex_prefix}texworks ${libs})

merge_trustinfo_manifest(${miktex_prefix}texworks asInvoker)

install(TARGETS ${miktex_prefix}texworks DESTINATION ${bindir})
