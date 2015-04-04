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

add_definitions(-Dpoppler_qt5_EXPORTS)

set(CMAKE_AUTOMOC ON)

include_directories(BEFORE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(CMAKE_CXX_FLAGS "/Zc:wchar_t- ${CMAKE_CXX_FLAGS}")

set(${poppler_qt_dll_name}_sources
  poppler-annotation.cc
  poppler-document.cc
  poppler-embeddedfile.cc
  poppler-fontinfo.cc
  poppler-form.cc
  poppler-link.cc
  poppler-link-extractor.cc
  poppler-movie.cc
  poppler-optcontent.cc
  poppler-page.cc
  poppler-base-converter.cc
  poppler-pdf-converter.cc
  poppler-private.cc
  poppler-ps-converter.cc
  poppler-qiodeviceoutstream.cc
  poppler-sound.cc
  poppler-textbox.cc
  poppler-page-transition.cc
  poppler-media.cc
  ArthurOutputDev.cc

  ArthurOutputDev.h
  poppler-annotation-helper.h
  poppler-annotation-private.h
  poppler-annotation.h
  poppler-converter-private.h
  poppler-embeddedfile-private.h
  poppler-export.h
  poppler-form.h
  poppler-link-extractor-private.h
  poppler-link.h
  poppler-media.h
  poppler-optcontent-private.h
  poppler-optcontent.h
  poppler-page-private.h
  poppler-page-transition-private.h
  poppler-page-transition.h
  poppler-private.h
  poppler-qiodeviceoutstream-private.h
  poppler-qt5.h
)

add_library(${poppler_qt_dll_name} SHARED ${${poppler_qt_dll_name}_sources})

set_target_properties(${poppler_qt_dll_name}
  PROPERTIES
   VERSION "${MIKTEX_SERIES_STR}"
   SOVERSION 1
)

target_include_directories(${poppler_qt_dll_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/qt5/src
)

target_link_libraries(${poppler_qt_dll_name}
  PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Xml
    ${core_dll_name}
  PUBLIC
    ${poppler_lib_name}
)

rebase(${poppler_qt_dll_name})

install(TARGETS ${poppler_qt_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
