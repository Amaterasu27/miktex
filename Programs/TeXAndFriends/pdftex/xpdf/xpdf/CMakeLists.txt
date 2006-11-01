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

add_definitions(
  -DHAVE_POPEN
  -DPDF_PARSER_ONLY
  -DUSE_GZIP
  -DNATIVE_WINDOWS_LEAN_AND_MEAN
)

include_directories(
  ..
  ../goo
  ../fofi
)

set(${xpdf_lib_name}_sources
  Array.cc
  Array.h
  BuiltinFont.cc
  BuiltinFont.h
  BuiltinFontTables.cc
  BuiltinFontTables.h
  CMap.cc
  CMap.h
  Catalog.cc
  Catalog.h
  CharCodeToUnicode.cc
  CharCodeToUnicode.h
  CharTypes.h
  Decrypt.cc
  Decrypt.h
  Dict.cc
  Dict.h
  Error.cc
  Error.h
  ErrorCodes.h
  FontEncodingTables.cc
  FontEncodingTables.h
  Function.cc
  Function.h
  GfxFont.cc
  GfxFont.h
  GfxState.cc
  GfxState.h
  GlobalParams.cc
  GlobalParams.h
  JArithmeticDecoder.cc
  JArithmeticDecoder.h
  JBIG2Stream.cc
  JBIG2Stream.h
  JPXStream.cc
  JPXStream.h
  Lexer.cc
  Lexer.h
  Link.cc
  Link.h
  NameToCharCode.cc
  NameToCharCode.h
  NameToUnicodeTable.h
  Object.cc
  Object.h
  Outline.cc
  Outline.h
  OutputDev.cc
  OutputDev.h
  PDFDoc.cc
  PDFDoc.h
  PDFDocEncoding.cc
  PDFDocEncoding.h
  PSTokenizer.cc
  PSTokenizer.h
  Page.cc
  Page.h
  Parser.cc
  Parser.h
  SecurityHandler.cc
  SecurityHandler.h
  Stream-CCITT.h
  Stream.cc
  Stream.h
  UTF8.h
  UnicodeMap.cc
  UnicodeMap.h
  UnicodeMapTables.h
  XRef.cc
  XRef.h
  config.h
)

add_library(${xpdf_lib_name} STATIC ${${xpdf_lib_name}_sources})
