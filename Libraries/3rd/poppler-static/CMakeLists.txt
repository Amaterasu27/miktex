## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2009-2013 Christian Schenk
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

set(ENABLE_LIBCURL TRUE)
set(ENABLE_LIBJPEG TRUE)
set(ENABLE_SPLASH TRUE)
set(ENABLE_ZLIB TRUE)

set(WITH_FONTCONFIGURATION_WIN32 TRUE)
set(WITH_FONTCONFIGURATION_FONTCONFIG FALSE)

set(MULTITHREAD TRUE)
set(OPI_SUPPORT TRUE)
set(TEXTOUT_WORD_LIST TRUE)

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

set(CMAKE_CXX_FLAGS "/Zc:wchar_t- ${CMAKE_CXX_FLAGS}")

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
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/grandom.cc
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
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/UTF.cc

  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/SplashOutputDev.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/Splash.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashBitmap.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashClip.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashFTFont.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashFTFontEngine.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashFTFontFile.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashFont.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashFontEngine.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashFontFile.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashFontFileID.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashPath.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashPattern.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashScreen.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashState.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashT1Font.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashT1FontEngine.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashT1FontFile.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashXPath.cc
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashXPathScanner.cc

  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/DCTStream.cc

  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/FlateStream.cc

  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/JPXStream.cc

  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/fofi/FoFiBase.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/fofi/FoFiEncodings.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/fofi/FoFiIdentifier.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/fofi/FoFiTrueType.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/fofi/FoFiType1.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/fofi/FoFiType1C.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/FixedPoint.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/GooHash.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/GooLikely.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/GooList.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/GooMutex.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/GooString.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/GooTimer.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/ImgWriter.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/JpegWriter.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/PNGWriter.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/TiffWriter.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/gfile.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/gmem.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/grandom.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/gstrtod.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/gtypes.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/goo/gtypes_p.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Annot.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Array.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/ArthurOutputDev.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/BuiltinFont.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/BuiltinFontTables.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/CMap.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/CachedFile.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/CairoFontEngine.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/CairoOutputDev.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/CairoRescaleBox.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Catalog.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/CharCodeToUnicode.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/CharTypes.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/CompactFontTables.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/CurlCachedFile.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/CurlPDFDocBuilder.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/DCTStream.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/DateInfo.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Decrypt.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Dict.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Error.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/ErrorCodes.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/FileSpec.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/FlateStream.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/FontEncodingTables.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/FontInfo.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Form.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Function.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Gfx.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/GfxFont.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/GfxState.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/GfxState_helpers.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/GlobalParams.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Hints.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/JArithmeticDecoder.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/JBIG2Stream.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/JPEG2000Stream.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/JPXStream.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Lexer.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Linearization.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Link.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/LocalPDFDocBuilder.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Movie.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/NameToCharCode.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/NameToUnicodeTable.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Object.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/OptionalContent.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Outline.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/OutputDev.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PDFDoc.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PDFDocBuilder.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PDFDocEncoding.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PDFDocFactory.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PSOutputDev.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PSTokenizer.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Page.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PageLabelInfo.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PageTransition.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Parser.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PopplerCache.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/PreScanOutputDev.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/ProfileData.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Rendition.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/SecurityHandler.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Sound.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/SplashOutputDev.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/StdinCachedFile.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/StdinPDFDocBuilder.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Stream-CCITT.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/Stream.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/TextOutputDev.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/UTF.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/UTF8.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/UnicodeCClassTables.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/UnicodeCompTables.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/UnicodeDecompTables.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/UnicodeMap.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/UnicodeMapTables.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/UnicodeTypeTable.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/ViewerPreferences.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/XRef.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/poppler/XpdfPluginAPI.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/Splash.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashBitmap.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashClip.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashErrorCodes.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashFTFont.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashFTFontEngine.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashFTFontFile.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashFont.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashFontEngine.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashFontFile.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashFontFileID.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashGlyphBitmap.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashMath.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashPath.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashPattern.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashScreen.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashState.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashT1Font.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashT1FontEngine.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashT1FontFile.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashTypes.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashXPath.h
  ${CMAKE_SOURCE_DIR}/${rel_poppler_dir}/splash/SplashXPathScanner.h
)

add_library(${poppler_lib_name} STATIC ${${poppler_lib_name}_sources})
