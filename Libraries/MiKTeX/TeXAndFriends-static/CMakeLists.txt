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

include_directories(BEFORE ${CMAKE_BINARY_DIR}/${rel_texmf_dir})
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/${rel_texmf_dir})

use_static_crt()
use_static_libraries()

set(${texmf_lib_name}_sources
  ${CMAKE_BINARY_DIR}/${rel_texmf_dir}/include/miktex/mfapp.defaults.h
  ${CMAKE_BINARY_DIR}/${rel_texmf_dir}/include/miktex/texapp.defaults.h
  ${CMAKE_BINARY_DIR}/${rel_texmf_dir}/miktex/texmfapp.defaults.h
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/StdAfx.cpp
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/StdAfx.h
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/c4plib.cpp
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/c4pstart.cpp
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/etexapp.cpp
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/C4P/C4P.h
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/TeXAndFriends/ETeXApp.h
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/TeXAndFriends/ETeXApp.inl
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/TeXAndFriends/MetafontApp.h
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/TeXAndFriends/MetafontApp.inl
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/TeXAndFriends/Prototypes.h
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/TeXAndFriends/TeXApp.h
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/TeXAndFriends/TeXApp.inl
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/TeXAndFriends/TeXMFApp.h
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/TeXAndFriends/TeXMFApp.inl
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/TeXAndFriends/WebApp.h
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/TeXAndFriends/WebApp.inl
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/TeXAndFriends/WebAppInputLine.h
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/TeXAndFriends/WebAppInputLine.inl
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/inputline.cpp
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/internal.h
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/mfapp.cpp
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/texapp.cpp
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/texmf-version.h
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/texmfapp.cpp
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/texmflib.cpp
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/webapp.cpp
)

add_library(${texmf_lib_name} STATIC ${${texmf_lib_name}_sources})

if(INSTALL_STATIC_LIBRARIES)
  install(TARGETS ${texmf_lib_name}
    RUNTIME DESTINATION "${bindir}"
    LIBRARY DESTINATION "${libdir}"
    ARCHIVE DESTINATION "${libdir}"
  )
endif(INSTALL_STATIC_LIBRARIES)
