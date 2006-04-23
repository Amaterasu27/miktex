## miktex.mak:
## 
## Copyright (C) 1998-2006 Christian Schenk
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

!include <miktex.inc>

objects = \
	$(outdir)\Array.obj \
	$(outdir)\BuiltinFont.obj \
	$(outdir)\BuiltinFontTables.obj \
	$(outdir)\CMap.obj \
	$(outdir)\Catalog.obj \
	$(outdir)\CharCodeToUnicode.obj \
	$(outdir)\Decrypt.obj \
	$(outdir)\Dict.obj \
	$(outdir)\Error.obj \
	$(outdir)\FontEncodingTables.obj \
	$(outdir)\Function.obj \
	$(outdir)\GfxFont.obj \
	$(outdir)\GfxState.obj \
	$(outdir)\GlobalParams.obj \
	$(outdir)\JArithmeticDecoder.obj \
	$(outdir)\JBIG2Stream.obj \
	$(outdir)\JPXStream.obj \
	$(outdir)\Lexer.obj \
	$(outdir)\Link.obj \
	$(outdir)\NameToCharCode.obj \
	$(outdir)\Object.obj \
	$(outdir)\Outline.obj \
	$(outdir)\OutputDev.obj \
	$(outdir)\PDFDoc.obj \
	$(outdir)\PDFDocEncoding.obj \
	$(outdir)\PSTokenizer.obj \
	$(outdir)\Page.obj \
	$(outdir)\Parser.obj \
	$(outdir)\SecurityHandler.obj \
	$(outdir)\Stream.obj \
	$(outdir)\UnicodeMap.obj \
	$(outdir)\XRef.obj \

sources = \
	Array.cc \
	BuiltinFont.cc \
	BuiltinFontTables.cc \
	CMap.cc \
	Catalog.cc \
	CharCodeToUnicode.cc \
	Decrypt.cc \
	Dict.cc \
	Error.cc \
	FontEncodingTables.cc \
	Function.cc \
	GfxFont.cc \
	GfxState.cc \
	GlobalParams.cc \
	JArithmeticDecoder.cc \
	JBIG2Stream.cc \
	JPXStream.cc \
	Lexer.cc \
	Link.cc \
	NameToCharCode.cc \
	Object.cc \
	Outline.cc \
	OutputDev.cc \
	PDFDoc.cc \
	PDFDocEncoding.cc \
	PSTokenizer.cc \
	Page.cc \
	Parser.cc \
	SecurityHandler.cc \
	Stream.cc \
	UnicodeMap.cc \
	XRef.cc \

.SUFFIXES: .cc

defines = \
	-DHAVE_POPEN \
	-DPDF_PARSER_ONLY \
	-DUSE_GZIP \
	-DWIN32 \
	-DWIN32_LEAN_AND_MEAN \

includes = \
	$(ccinc_libpng) \
	$(ccinc_zlib) \
	-I.. \
	-I../.. \
	-I../fofi\
	-I../goo\

.cc{$(outdir)\}.obj:
	$(cc) $(cppstandard) \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_output_directory)$(outdir)\ \
		$(ccopt_select_fastcall) \
		$(defines) \
		$(includes) \
		$<

libraries = $(outdir)\xpdf.lib

all: common-all $(libraries)

install: common-install $(libraries)

check: common-check

$(outdir)\xpdf.lib: $(outdir) $(objects)
	$(libtool) $(libflags) -out:$@ $(objects)

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend:
	$(MAKEDEPEND) $(defines) $(includes) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\Array.obj: ../aconf.h ../goo/gmem.h Object.h ../goo/gtypes.h
$(outdir)\Array.obj: ../goo/GString.h Array.h
$(outdir)\BuiltinFont.obj: ../aconf.h ../goo/gmem.h FontEncodingTables.h
$(outdir)\BuiltinFont.obj: BuiltinFont.h ../goo/gtypes.h
$(outdir)\BuiltinFontTables.obj: ../aconf.h FontEncodingTables.h
$(outdir)\BuiltinFontTables.obj: BuiltinFontTables.h BuiltinFont.h
$(outdir)\BuiltinFontTables.obj: ../goo/gtypes.h
$(outdir)\CMap.obj: ../aconf.h ../goo/gmem.h ../goo/gfile.h ../goo/gtypes.h
$(outdir)\CMap.obj: ../goo/GString.h Error.h config.h GlobalParams.h CharTypes.h
$(outdir)\CMap.obj: ../goo/GMutex.h PSTokenizer.h CMap.h
$(outdir)\Catalog.obj: ../aconf.h ../goo/gmem.h Object.h ../goo/gtypes.h
$(outdir)\Catalog.obj: ../goo/GString.h XRef.h Array.h Dict.h Page.h Error.h
$(outdir)\Catalog.obj: config.h Link.h Catalog.h
$(outdir)\CharCodeToUnicode.obj: ../aconf.h ../goo/gmem.h ../goo/gfile.h
$(outdir)\CharCodeToUnicode.obj: ../goo/gtypes.h ../goo/GString.h Error.h
$(outdir)\CharCodeToUnicode.obj: config.h GlobalParams.h CharTypes.h
$(outdir)\CharCodeToUnicode.obj: ../goo/GMutex.h PSTokenizer.h
$(outdir)\CharCodeToUnicode.obj: CharCodeToUnicode.h
$(outdir)\Decrypt.obj: ../aconf.h ../goo/gmem.h Decrypt.h ../goo/gtypes.h
$(outdir)\Decrypt.obj: ../goo/GString.h
$(outdir)\Dict.obj: ../aconf.h ../goo/gmem.h Object.h ../goo/gtypes.h
$(outdir)\Dict.obj: ../goo/GString.h XRef.h Dict.h
$(outdir)\Error.obj: ../aconf.h GlobalParams.h ../goo/gtypes.h CharTypes.h
$(outdir)\Error.obj: ../goo/GMutex.h Error.h config.h
$(outdir)\FontEncodingTables.obj: ../aconf.h FontEncodingTables.h
$(outdir)\Function.obj: ../aconf.h ../goo/gmem.h Object.h ../goo/gtypes.h
$(outdir)\Function.obj: ../goo/GString.h Dict.h Stream.h Error.h config.h
$(outdir)\Function.obj: Function.h
$(outdir)\GfxFont.obj: ../aconf.h ../goo/gmem.h Error.h config.h Object.h
$(outdir)\GfxFont.obj: ../goo/gtypes.h ../goo/GString.h Dict.h GlobalParams.h
$(outdir)\GfxFont.obj: CharTypes.h ../goo/GMutex.h CMap.h CharCodeToUnicode.h
$(outdir)\GfxFont.obj: FontEncodingTables.h BuiltinFontTables.h BuiltinFont.h
$(outdir)\GfxFont.obj: ../fofi/FoFiType1.h ../fofi/FoFiBase.h
$(outdir)\GfxFont.obj: ../fofi/FoFiType1C.h ../fofi/FoFiTrueType.h GfxFont.h
$(outdir)\GfxState.obj: ../aconf.h ../goo/gmem.h Error.h config.h Object.h
$(outdir)\GfxState.obj: ../goo/gtypes.h ../goo/GString.h Array.h Page.h
$(outdir)\GfxState.obj: GfxState.h Function.h
$(outdir)\GlobalParams.obj: ../aconf.h ../goo/gmem.h ../goo/GString.h
$(outdir)\GlobalParams.obj: ../goo/GList.h ../goo/gtypes.h ../goo/GHash.h
$(outdir)\GlobalParams.obj: ../goo/gfile.h Error.h config.h NameToCharCode.h
$(outdir)\GlobalParams.obj: CharTypes.h CharCodeToUnicode.h ../goo/GMutex.h
$(outdir)\GlobalParams.obj: UnicodeMap.h CMap.h BuiltinFontTables.h BuiltinFont.h
$(outdir)\GlobalParams.obj: FontEncodingTables.h GlobalParams.h
$(outdir)\GlobalParams.obj: NameToUnicodeTable.h UnicodeMapTables.h UTF8.h
$(outdir)\JArithmeticDecoder.obj: ../aconf.h Object.h ../goo/gtypes.h
$(outdir)\JArithmeticDecoder.obj: ../goo/gmem.h ../goo/GString.h Stream.h
$(outdir)\JArithmeticDecoder.obj: JArithmeticDecoder.h
$(outdir)\JBIG2Stream.obj: ../aconf.h ../goo/GList.h ../goo/gtypes.h Error.h
$(outdir)\JBIG2Stream.obj: config.h JArithmeticDecoder.h JBIG2Stream.h Object.h
$(outdir)\JBIG2Stream.obj: ../goo/gmem.h ../goo/GString.h Stream.h
$(outdir)\JPXStream.obj: ../aconf.h ../goo/gmem.h Error.h config.h
$(outdir)\JPXStream.obj: JArithmeticDecoder.h ../goo/gtypes.h JPXStream.h
$(outdir)\JPXStream.obj: Object.h ../goo/GString.h Stream.h
$(outdir)\Lexer.obj: ../aconf.h Lexer.h Object.h ../goo/gtypes.h ../goo/gmem.h
$(outdir)\Lexer.obj: ../goo/GString.h Stream.h Error.h config.h
$(outdir)\Link.obj: ../aconf.h ../goo/gmem.h ../goo/GString.h Error.h config.h
$(outdir)\Link.obj: Object.h ../goo/gtypes.h Array.h Dict.h Link.h
$(outdir)\NameToCharCode.obj: ../aconf.h ../goo/gmem.h NameToCharCode.h
$(outdir)\NameToCharCode.obj: CharTypes.h
$(outdir)\Object.obj: ../aconf.h Object.h ../goo/gtypes.h ../goo/gmem.h
$(outdir)\Object.obj: ../goo/GString.h Array.h Dict.h Error.h config.h Stream.h
$(outdir)\Object.obj: XRef.h
$(outdir)\Outline.obj: ../aconf.h ../goo/gmem.h ../goo/GString.h ../goo/GList.h
$(outdir)\Outline.obj: ../goo/gtypes.h Link.h Object.h PDFDocEncoding.h
$(outdir)\Outline.obj: CharTypes.h Outline.h
$(outdir)\OutputDev.obj: ../aconf.h Object.h ../goo/gtypes.h ../goo/gmem.h
$(outdir)\OutputDev.obj: ../goo/GString.h Stream.h GfxState.h Function.h
$(outdir)\OutputDev.obj: OutputDev.h CharTypes.h
$(outdir)\PDFDoc.obj: ../aconf.h ../goo/GString.h config.h GlobalParams.h
$(outdir)\PDFDoc.obj: ../goo/gtypes.h CharTypes.h ../goo/GMutex.h Page.h Object.h
$(outdir)\PDFDoc.obj: ../goo/gmem.h Catalog.h Stream.h XRef.h Link.h OutputDev.h
$(outdir)\PDFDoc.obj: Error.h ErrorCodes.h Lexer.h Parser.h SecurityHandler.h
$(outdir)\PDFDoc.obj: Outline.h PDFDoc.h
$(outdir)\PDFDocEncoding.obj: PDFDocEncoding.h CharTypes.h
$(outdir)\PSTokenizer.obj: ../aconf.h PSTokenizer.h ../goo/gtypes.h
$(outdir)\Page.obj: ../aconf.h GlobalParams.h ../goo/gtypes.h CharTypes.h
$(outdir)\Page.obj: ../goo/GMutex.h Object.h ../goo/gmem.h ../goo/GString.h
$(outdir)\Page.obj: Array.h Dict.h XRef.h Link.h OutputDev.h Error.h config.h
$(outdir)\Page.obj: Page.h
$(outdir)\Parser.obj: ../aconf.h Object.h ../goo/gtypes.h ../goo/gmem.h
$(outdir)\Parser.obj: ../goo/GString.h Array.h Dict.h Parser.h Lexer.h Stream.h
$(outdir)\Parser.obj: XRef.h Error.h config.h Decrypt.h
$(outdir)\SecurityHandler.obj: ../aconf.h ../goo/GString.h PDFDoc.h XRef.h
$(outdir)\SecurityHandler.obj: ../goo/gtypes.h Object.h ../goo/gmem.h Catalog.h
$(outdir)\SecurityHandler.obj: Page.h Decrypt.h Error.h config.h GlobalParams.h
$(outdir)\SecurityHandler.obj: CharTypes.h ../goo/GMutex.h SecurityHandler.h
$(outdir)\Stream.obj: ../aconf.h ../goo/gmem.h ../goo/gfile.h ../goo/gtypes.h
$(outdir)\Stream.obj: config.h Error.h Object.h ../goo/GString.h Lexer.h Stream.h
$(outdir)\Stream.obj: Decrypt.h GfxState.h Function.h JBIG2Stream.h JPXStream.h
$(outdir)\Stream.obj: Stream-CCITT.h
$(outdir)\UnicodeMap.obj: ../aconf.h ../goo/gmem.h ../goo/gfile.h ../goo/gtypes.h
$(outdir)\UnicodeMap.obj: ../goo/GString.h ../goo/GList.h Error.h config.h
$(outdir)\UnicodeMap.obj: GlobalParams.h CharTypes.h ../goo/GMutex.h UnicodeMap.h
$(outdir)\XRef.obj: ../aconf.h ../goo/gmem.h Object.h ../goo/gtypes.h
$(outdir)\XRef.obj: ../goo/GString.h Stream.h Lexer.h Parser.h Dict.h Error.h
$(outdir)\XRef.obj: config.h ErrorCodes.h XRef.h
