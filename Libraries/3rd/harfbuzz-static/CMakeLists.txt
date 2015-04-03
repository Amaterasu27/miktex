## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2013-2015 Christian Schenk
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

set(HAVE_GRAPHITE2 1)
set(HAVE_ICU 1)
set(HAVE_OT 1)

add_definitions(-DHAVE_CONFIG_H -DHB_NO_MT)

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_dir}/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_include_dir}
  ${CMAKE_SOURCE_DIR}/${rel_icu_include_dir}
)

set(${harfbuzz_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-atomic-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-blob.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-buffer-deserialize-json.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-buffer-deserialize-text.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-buffer-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-buffer-serialize.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-buffer.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-cache-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-common.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-fallback-shape.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-font-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-font.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-mutex-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-object-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-open-file-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-open-type-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-head-table.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-hhea-table.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-hmtx-table.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-maxp-table.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-name-table.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-tag.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-set-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-set.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-shape.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-shape-plan-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-shape-plan.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-shaper-list.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-shaper-impl-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-shaper-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-shaper.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-tt-font.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-unicode-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-unicode.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-utf-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-warning.cc

  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-layout.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-layout-common-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-layout-gdef-table.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-layout-gpos-table.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-layout-gsubgpos-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-layout-gsub-table.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-layout-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-map.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-map-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-complex-arabic.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-complex-arabic-fallback.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-complex-arabic-table.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-complex-default.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-complex-indic.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-complex-indic-machine.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-complex-indic-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-complex-indic-table.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-complex-myanmar.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-complex-myanmar-machine.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-complex-sea.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-complex-sea-machine.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-complex-thai.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-complex-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-normalize-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-normalize.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-fallback-private.hh
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-fallback.cc
  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-ot-shape-private.hh

  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-icu.cc

  ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_source_dir}/hb-graphite2.cc
)

add_library(${harfbuzz_lib_name} STATIC ${${harfbuzz_lib_name}_sources})

target_include_directories(${harfbuzz_lib_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/${rel_harfbuzz_include_dir}
)

target_link_libraries(${harfbuzz_lib_name}
  PUBLIC
    ${graphite2_dll_name}
)
