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

miktex_cc_use_crt_dll = 0
no_miktex_dlls = 1

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
  ..\LZMA\LZMADecoder.cpp \
  ..\LZMA\LZMAEncoder.cpp \
  ..\LZMA\lzma-miktex.cpp \


{..\..\..\..\CPP\7zip\Common\}.cpp{$(outdir)\}.obj:
	$(cc) -D_7ZIP_LARGE_PAGES $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

{..\..\..\..\CPP\7zip\Compress\LZ\}.cpp{$(outdir)\}.obj:
	$(cc) -D_7ZIP_LARGE_PAGES $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

{..\..\..\..\CPP\7zip\Compress\RangeCoder\}.cpp{$(outdir)\}.obj:
	$(cc) -D_7ZIP_LARGE_PAGES $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

{..\LZMA\}.cpp{$(outdir)\}.obj:
	$(cc) -D_7ZIP_LARGE_PAGES $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

.cpp{$(outdir)\}.obj:
	$(cc) -D_7ZIP_LARGE_PAGES $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

{..\..\..\..\C\}.c{$(outdir)\}.obj:
	$(cc) -D_7ZIP_LARGE_PAGES $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

{..\..\..\..\C\Compress\Lz\}.c{$(outdir)\}.obj:
	$(cc) -D_7ZIP_LARGE_PAGES $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(lzma_static_lib)

install: common-install all

check: common-check

# _____________________________________________________________________________
#
# lzma-static.lib
# _____________________________________________________________________________

$(lzma_static_lib): \
			$(outdir) \
			$(libobjects) \

	$(libtool) $(libflags) $(libopt_output_file)$@ $(libobjects)

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

$(outdir)\LZMADecoder.obj: ../../../Common/Defs.h
$(outdir)\LZMAEncoder.obj: ../../../Common/Defs.h
$(outdir)\LZMAEncoder.obj: ../../Common/StreamUtils.h ../../IStream.h
$(outdir)\LZMAEncoder.obj: ../../../Common/MyUnknown.h
$(outdir)\LZMAEncoder.obj: ../../../Common/Types.h
$(outdir)\lzma-miktex.obj: ../../../Common/MyInitGuid.h
