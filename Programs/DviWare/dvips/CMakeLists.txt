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

include_directories(BEFORE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_bmeps_dir}
)

add_definitions(
  -DHAVE_BMEPS_RUN
  -DKPATHSEA
  -DMIKTEX
  -DSHIFTLOWCHARS
  -D_UNICODE -DUNICODE
)

set(dvips_c_sources
  bbox.c
  color.c
  dopage.c
  dosection.c
  dospecial.c
  download.c
  dpicheck.c
  drawPS.c
  dviinput.c
  dvips.c
  emspecial.c
  finclude.c
  fontdef.c
  header.c
  hps.c
  loadfont.c
  makefont.c
  output.c
  papersiz.c
  pprescan.c
  prescan.c
  repack.c
  resident.c
  scalewidth.c
  scanpage.c
  search.c
  skippage.c
  t1part.c
  tfmload.c
  unpack.c
  virtualfont.c
  writet1.c
)

set(dvips_sources
  ${dvips_c_sources}
  ${wrapper_cpp}
)

set_source_files_properties(${wrapper_cpp}
  PROPERTIES
    COMPILE_FLAGS "-DCPLUSPLUSMAIN -DBEQUIET"
)

set_source_files_properties(${dvips_c_sources}
  PROPERTIES
    LANGUAGE CXX
)

if(NATIVE_WINDOWS)
  set(dvips_sources ${dvips_sources}
    dvips.rc
  )
endif(NATIVE_WINDOWS)

add_executable(dvips ${dvips_sources})

merge_trustinfo_manifest(dvips asInvoker)

target_link_libraries(dvips
  ${app_dll_name}
  ${bmeps_dll_name}
  ${core_dll_name}
  ${kpsemu_dll_name}
  ${utf8wrap_dll_name}
)

delay_load(dvips ${bmeps_dll_name})

install(TARGETS dvips DESTINATION ${bindir})

set(afm2tfm_sources
  afm2tfm.c
  ${wrapper_cpp}
)

set_source_files_properties(afm2tfm.c
  PROPERTIES
    LANGUAGE CXX
)

if(NATIVE_WINDOWS)
  set(afm2tfm_sources ${afm2tfm_sources}
    afm2tfm.rc
  )
endif(NATIVE_WINDOWS)

add_executable(afm2tfm ${afm2tfm_sources})

merge_trustinfo_manifest(afm2tfm asInvoker)

target_link_libraries(afm2tfm
  ${app_dll_name}
  ${core_dll_name}
  ${kpsemu_dll_name}
  ${texmf_dll_name}
  ${utf8wrap_dll_name}
)

install(TARGETS afm2tfm DESTINATION ${bindir})
