## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006 Christian Schenk
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
  -DCONFIGFILE=\\\"config.ps\\\"
  -DCREATIONDATE
  -DDEFRES=600
  -DEMTEX
  -DHAVE_BMEPS_RUN
  -DHPS
  -DKPATHSEA
  -DMAKE_TEX_PK_BY_DEFAULT
  -DMIKTEX
  -DNeedFunctionPrototypes
  -DSHIFTLOWCHARS
  -DTPIC
  -DWRITEBIN=FOPEN_WBIN_MODE
  -Dpclose=_pclose
  -Dpopen=_popen
  -UDOWNLOAD_USING_PDFTEX
)

create_cpp_from_c(
  ${CMAKE_CURRENT_SOURCE_DIR}/bbox.c
  ${CMAKE_CURRENT_SOURCE_DIR}/color.c
  ${CMAKE_CURRENT_SOURCE_DIR}/dopage.c
  ${CMAKE_CURRENT_SOURCE_DIR}/dosection.c
  ${CMAKE_CURRENT_SOURCE_DIR}/dospecial.c
  ${CMAKE_CURRENT_SOURCE_DIR}/download.c
  ${CMAKE_CURRENT_SOURCE_DIR}/dpicheck.c
  ${CMAKE_CURRENT_SOURCE_DIR}/drawPS.c
  ${CMAKE_CURRENT_SOURCE_DIR}/dviinput.c
  ${CMAKE_CURRENT_SOURCE_DIR}/dvips.c
  ${CMAKE_CURRENT_SOURCE_DIR}/emspecial.c
  ${CMAKE_CURRENT_SOURCE_DIR}/finclude.c
  ${CMAKE_CURRENT_SOURCE_DIR}/fontdef.c
  ${CMAKE_CURRENT_SOURCE_DIR}/header.c
  ${CMAKE_CURRENT_SOURCE_DIR}/hps.c
  ${CMAKE_CURRENT_SOURCE_DIR}/loadfont.c
  ${CMAKE_CURRENT_SOURCE_DIR}/makefont.c
  ${CMAKE_CURRENT_SOURCE_DIR}/output.c
  ${CMAKE_CURRENT_SOURCE_DIR}/papersiz.c
  ${CMAKE_CURRENT_SOURCE_DIR}/pprescan.c
  ${CMAKE_CURRENT_SOURCE_DIR}/prescan.c
  ${CMAKE_CURRENT_SOURCE_DIR}/repack.c
  ${CMAKE_CURRENT_SOURCE_DIR}/resident.c
  ${CMAKE_CURRENT_SOURCE_DIR}/scalewidth.c
  ${CMAKE_CURRENT_SOURCE_DIR}/scanpage.c
  ${CMAKE_CURRENT_SOURCE_DIR}/search.c
  ${CMAKE_CURRENT_SOURCE_DIR}/skippage.c
  ${CMAKE_CURRENT_SOURCE_DIR}/t1part.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tfmload.c
  ${CMAKE_CURRENT_SOURCE_DIR}/unpack.c
  ${CMAKE_CURRENT_SOURCE_DIR}/virtualfont.c
  ${CMAKE_CURRENT_SOURCE_DIR}/writet1.c
)

set(dvips_sources
  ${CMAKE_CURRENT_BINARY_DIR}/bbox.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/color.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/dopage.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/dosection.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/dospecial.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/download.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/dpicheck.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/drawPS.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/dviinput.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/dvips.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/emspecial.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/finclude.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/fontdef.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/header.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/hps.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/loadfont.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/makefont.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/output.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/papersiz.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/pprescan.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/prescan.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/repack.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/resident.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/scalewidth.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/scanpage.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/search.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/skippage.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/t1part.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/tfmload.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/unpack.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/virtualfont.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writet1.cpp
  ${wrapper_cpp}
)

set_source_files_properties(${wrapper_cpp}
  PROPERTIES COMPILE_FLAGS "-DCPLUSPLUSMAIN -DBEQUIET"
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
)

delay_load(dvips ${bmeps_dll_name})

install(TARGETS dvips DESTINATION ${bindir})

create_cpp_from_c(
  ${CMAKE_CURRENT_SOURCE_DIR}/afm2tfm.c
)

set(afm2tfm_sources
  ${CMAKE_CURRENT_BINARY_DIR}/afm2tfm.cpp
  ${wrapper_cpp}
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
)

install(TARGETS afm2tfm DESTINATION ${bindir})
