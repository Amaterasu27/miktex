## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2009-2012 Christian Schenk
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

##use_static_crt()
##use_static_libraries()

include_directories(BEFORE
  ${CMAKE_BINARY_DIR}/${rel_poppler_dir}
  ${CMAKE_BINARY_DIR}/${rel_poppler_dir}/poppler
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/fofi
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler
)

set(MULTITHREAD TRUE)
set(OPI_SUPPORT TRUE)
set(TEXTOUT_WORD_LIST TRUE)

set(ENABLE_LIBJPEG TRUE)
set(ENABLE_SPLASH TRUE)
set(ENABLE_ZLIB TRUE)

set(HAVE_LIBZ TRUE)
set(HAVE_SPLASH TRUE)
set(HAVE_FREETYPE_H TRUE)

add_definitions(-DHAVE_CONFIG_H=1)

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/config.h.cmake
  ${CMAKE_BINARY_DIR}/${rel_poppler_dir}/config.h
)

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler-config.h.cmake
  ${CMAKE_BINARY_DIR}/${rel_poppler_dir}/poppler/poppler-config.h
)

set(${poppler_lib_name}_sources

  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/gfile.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/gmempp.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/GooHash.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/GooList.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/GooTimer.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/GooString.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/gmem.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/FixedPoint.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/PNGWriter.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/TiffWriter.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/JpegWriter.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/ImgWriter.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/gstrtod.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/fofi/FoFiBase.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/fofi/FoFiEncodings.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/fofi/FoFiTrueType.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/fofi/FoFiType1.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/fofi/FoFiType1C.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/fofi/FoFiIdentifier.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Annot.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Array.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/BuiltinFont.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/BuiltinFontTables.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/CachedFile.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Catalog.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/CharCodeToUnicode.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/CMap.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/DateInfo.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Decrypt.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Dict.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Error.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/FileSpec.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/FontEncodingTables.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Form.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/FontInfo.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Function.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Gfx.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/GfxFont.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/GfxState.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/GlobalParams.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Hints.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/JArithmeticDecoder.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/JBIG2Stream.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Lexer.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Link.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Linearization.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/LocalPDFDocBuilder.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/NameToCharCode.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Object.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/OptionalContent.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Outline.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/OutputDev.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Page.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PageTransition.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Parser.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PDFDoc.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PDFDocEncoding.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PDFDocFactory.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PopplerCache.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/ProfileData.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PreScanOutputDev.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PSTokenizer.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Stream.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/strtok_r.cpp
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/UnicodeMap.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/UnicodeTypeTable.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/XRef.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PSOutputDev.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/TextOutputDev.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PageLabelInfo.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/SecurityHandler.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/StdinCachedFile.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/StdinPDFDocBuilder.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Sound.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/ViewerPreferences.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/XpdfPluginAPI.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Movie.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Rendition.cc

  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/DCTStream.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/FlateStream.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/JPXStream.cc

  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/GlobalParamsWin.cc

  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/*.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/fofi/*.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/*.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/*.h
)

add_library(${poppler_lib_name} STATIC ${${poppler_lib_name}_sources})
