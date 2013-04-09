## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2007-2013 Christian Schenk
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

include_directories(BEFORE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(
  -DDLL_EXPORT
  -DHAVE_CONFIG_H
)

set(${fontconfig_dll_name}_sources
  ${CMAKE_BINARY_DIR}/${rel_fontconfig_dir}/fc-case/fccase.h
  ${CMAKE_BINARY_DIR}/${rel_fontconfig_dir}/fc-glyphname/fcglyphname.h
  ${CMAKE_BINARY_DIR}/${rel_fontconfig_dir}/fc-lang/fclang.h
  ${CMAKE_CURRENT_BINARY_DIR}/fcalias.h
  ${CMAKE_CURRENT_BINARY_DIR}/fcaliastail.h
  ${CMAKE_CURRENT_BINARY_DIR}/fcftalias.h
  ${CMAKE_CURRENT_BINARY_DIR}/fcftaliastail.h
  fcatomic.c
  fcblanks.c
  fccache.c
  fccfg.c
  fccharset.c
  fcdbg.c
  fcdefault.c
  fcdeprecate.h
  fcdir.c
  fcformat.c
  fcfreetype.c
  fcfs.c
  fcftint.h
  fcinit.c
  fcint.h
  fclang.c
  fclist.c
  fcmatch.c
  fcmatrix.c
  fcmiktex.cpp
  fcmiktex.h
  fcname.c
  fcpat.c
  fcserialize.c
  fcstat.c
  fcstr.c
  fcxml.c
  fontconfig-version.h
  ftglue.c
  ftglue.h
)

set_source_files_properties(
  ${CMAKE_BINARY_DIR}/${rel_fontconfig_dir}/fc-case/fccase.h
  ${CMAKE_BINARY_DIR}/${rel_fontconfig_dir}/fc-glyphname/fcglyphname.h
  ${CMAKE_BINARY_DIR}/${rel_fontconfig_dir}/fc-lang/fclang.h
  ${CMAKE_CURRENT_BINARY_DIR}/fcalias.h
  ${CMAKE_CURRENT_BINARY_DIR}/fcaliastail.h
  ${CMAKE_CURRENT_BINARY_DIR}/fcftalias.h
  ${CMAKE_CURRENT_BINARY_DIR}/fcftaliastail.h
  PROPERTIES GENERATED TRUE
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/fontconfig.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/fontconfig.rc
  )
  set(${fontconfig_dll_name}_sources ${${fontconfig_dll_name}_sources}
    ${CMAKE_CURRENT_SOURCE_DIR}/fontconfig.def
    ${CMAKE_CURRENT_BINARY_DIR}/fontconfig.rc
  )
endif(NATIVE_WINDOWS)

add_library(${fontconfig_dll_name} SHARED ${${fontconfig_dll_name}_sources})

set_target_properties(${fontconfig_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

rebase(${fontconfig_dll_name})

target_link_libraries(${fontconfig_dll_name}
  ${expat_dll_name}
  ${freetype2_dll_name}
  ${unxemu_dll_name}
  ${utf8wrap_dll_name}
)

add_dependencies(${fontconfig_dll_name}
	fccase_h
	fcglyphname_h
	fclang_h
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fcalias.h
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/fcalias.h
  VERBATIM
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fcaliastail.h
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/fcaliastail.h
  VERBATIM
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fcftalias.h
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/fcftalias.h
  VERBATIM
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fcftaliastail.h
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/fcftaliastail.h
  VERBATIM
)

add_executable(fcarch fcarch.c)

install(TARGETS ${fontconfig_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
