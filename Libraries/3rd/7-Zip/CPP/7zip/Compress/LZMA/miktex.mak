## miktex.mak: lzma
##
## Copyright (C) 2007 Christian Schenk
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

libobjects = \
	$(outdir)\7zCrc.obj \
	$(outdir)\Alloc.obj \
	$(outdir)\InBuffer.obj \
	$(outdir)\LZMADecoder.obj \
	$(outdir)\LZMAEncoder.obj \
	$(outdir)\LZOutWindow.obj \
	$(outdir)\MatchFinder.obj \
	$(outdir)\MatchFinderMT.obj \
	$(outdir)\OutBuffer.obj \
	$(outdir)\RangeCoderBit.obj \
	$(outdir)\StreamUtils.obj \
	$(outdir)\Threads.obj \
	$(outdir)\lzma-miktex.obj \

sources = \
  ..\..\..\..\C\7zCrc.c \
  ..\..\..\..\C\Alloc.c \
  ..\..\..\..\C\Threads.c \
  ..\..\..\..\C\Compress\Lz\MatchFinder.c \
  ..\..\..\..\C\Compress\Lz\MatchFinderMT.c \
  ..\..\..\..\CPP\7zip\Common\InBuffer.cpp \
  ..\..\..\..\CPP\7zip\Common\OutBuffer.cpp \
  ..\..\..\..\CPP\7zip\Common\StreamUtils.cpp \
  ..\..\..\..\CPP\7zip\Compress\LZ\LZOutWindow.cpp \
  ..\..\..\..\CPP\7zip\Compress\RangeCoder\RangeCoderBit.cpp \
  LZMADecoder.cpp \
  LZMAEncoder.cpp \
  lzma-miktex.cpp \


liblzma_dll = $(lzma_dll_name).dll

binaries = $(outdir)\$(liblzma_dll)

{..\..\..\..\CPP\7zip\Common\}.cpp{$(outdir)\}.obj:
	$(cc) -D_7ZIP_LARGE_PAGES $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

{..\..\..\..\CPP\7zip\Compress\LZ\}.cpp{$(outdir)\}.obj:
	$(cc) -D_7ZIP_LARGE_PAGES $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

{..\..\..\..\CPP\7zip\Compress\RangeCoder\}.cpp{$(outdir)\}.obj:
	$(cc) -D_7ZIP_LARGE_PAGES $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

.cpp{$(outdir)\}.obj:
	$(cc) -D_7ZIP_LARGE_PAGES $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

{..\..\..\..\C\}.c{$(outdir)\}.obj:
	$(cc) -D_7ZIP_LARGE_PAGES $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

{..\..\..\..\C\Compress\Lz\}.c{$(outdir)\}.obj:
	$(cc) -D_7ZIP_LARGE_PAGES $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(lzma_lib)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# liblzma.dll
# _____________________________________________________________________________

$(outdir)\$(liblzma_dll) $(lzma_lib): \
			$(outdir) \
			$(dllbases) \
			$(libobjects) \
			$(outdir)\liblzma.res \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(lzma_dll_name) \
		-implib:$(lzma_lib) \
		-out:$(outdir)\$(liblzma_dll) \
		$(libobjects) $(outdir)\liblzma.res
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\liblzma.res: $(outdir) liblzma.rc $(libdir)\include\miktex\miktex.version
	$(rc) $(rcstandard) $(rcopt_output_file)$@ liblzma.rc

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend:
	$(MAKEDEPEND) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\InBuffer.obj: StdAfx.h
$(outdir)\InBuffer.obj: ../../../Common/MyWindows.h
$(outdir)\OutBuffer.obj: StdAfx.h
$(outdir)\OutBuffer.obj: ../../../Common/MyWindows.h
$(outdir)\StreamUtils.obj: StdAfx.h
$(outdir)\StreamUtils.obj: ../../../Common/MyWindows.h
$(outdir)\LZOutWindow.obj: StdAfx.h
$(outdir)\LZOutWindow.obj: ../../../Common/MyWindows.h
$(outdir)\RangeCoderBit.obj: StdAfx.h
$(outdir)\RangeCoderBit.obj: ../../../Common/MyWindows.h
$(outdir)\LZMADecoder.obj: StdAfx.h ../../../Common/MyWindows.h LZMADecoder.h
$(outdir)\LZMADecoder.obj: ../../../Common/MyCom.h ../../../Common/MyWindows.h
$(outdir)\LZMADecoder.obj: ../../ICoder.h ../../IStream.h
$(outdir)\LZMADecoder.obj: ../../../Common/MyUnknown.h ../../../Common/Types.h
$(outdir)\LZMADecoder.obj: ../LZ/LZOutWindow.h ../../IStream.h
$(outdir)\LZMADecoder.obj: ../../Common/OutBuffer.h ../../IStream.h
$(outdir)\LZMADecoder.obj: ../../../Common/MyCom.h
$(outdir)\LZMADecoder.obj: ../RangeCoder/RangeCoderBitTree.h
$(outdir)\LZMADecoder.obj: ../RangeCoder/RangeCoderBit.h
$(outdir)\LZMADecoder.obj: ../RangeCoder/RangeCoder.h ../../Common/InBuffer.h
$(outdir)\LZMADecoder.obj: ../RangeCoder/RangeCoderOpt.h LZMA.h
$(outdir)\LZMADecoder.obj: ../../../Common/Defs.h
$(outdir)\LZMAEncoder.obj: StdAfx.h ../../../Common/MyWindows.h
$(outdir)\LZMAEncoder.obj: ../../../Common/Defs.h ../../Common/StreamUtils.h
$(outdir)\LZMAEncoder.obj: ../../IStream.h LZMAEncoder.h ../../../Common/MyCom.h
$(outdir)\LZMAEncoder.obj: ../../../Common/MyWindows.h ../../ICoder.h
$(outdir)\LZMAEncoder.obj: ../../IStream.h ../../../Common/MyUnknown.h
$(outdir)\LZMAEncoder.obj: ../../../Common/Types.h
$(outdir)\LZMAEncoder.obj: ../RangeCoder/RangeCoderBitTree.h
$(outdir)\LZMAEncoder.obj: ../RangeCoder/RangeCoderBit.h
$(outdir)\LZMAEncoder.obj: ../RangeCoder/RangeCoder.h ../../Common/InBuffer.h
$(outdir)\LZMAEncoder.obj: ../../../Common/MyCom.h ../../Common/OutBuffer.h
$(outdir)\LZMAEncoder.obj: ../RangeCoder/RangeCoderOpt.h LZMA.h
$(outdir)\lzma-miktex.obj: StdAfx.h ../../../Common/MyWindows.h
$(outdir)\lzma-miktex.obj: ../../../Common/MyInitGuid.h LZMADecoder.h
$(outdir)\lzma-miktex.obj: ../../../Common/MyCom.h ../../../Common/MyWindows.h
$(outdir)\lzma-miktex.obj: ../../ICoder.h ../../IStream.h
$(outdir)\lzma-miktex.obj: ../../../Common/MyUnknown.h ../../../Common/Types.h
$(outdir)\lzma-miktex.obj: ../LZ/LZOutWindow.h ../../IStream.h
$(outdir)\lzma-miktex.obj: ../../Common/OutBuffer.h ../../IStream.h
$(outdir)\lzma-miktex.obj: ../../../Common/MyCom.h
$(outdir)\lzma-miktex.obj: ../RangeCoder/RangeCoderBitTree.h
$(outdir)\lzma-miktex.obj: ../RangeCoder/RangeCoderBit.h
$(outdir)\lzma-miktex.obj: ../RangeCoder/RangeCoder.h ../../Common/InBuffer.h
$(outdir)\lzma-miktex.obj: ../RangeCoder/RangeCoderOpt.h LZMA.h
$(outdir)\lzma-miktex.obj: include/miktex/lzma.h
