## miktex.mak:
##
## Copyright (C) 2002-2007 Christian Schenk
## 
## This file is part of the MiKTeX DibChunker Library.
## 
## The MiKTeX DibChunker Library is free software; you can
## redistribute it and/or modify it under the terms of the GNU General
## Public License as published by the Free Software Foundation; either
## version 2, or (at your option) any later version.
## 
## The MiKTeX DibChunker Library is distributed in the hope that it
## will be useful, but WITHOUT ANY WARRANTY; without even the implied
## warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## See the GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with the MiKTeX DibChunker Library; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

miktex_cc_warnings_as_errors = 1

miktex_enable_pgo = 1

!include <miktex.inc>

objects = \
	$(outdir)\StdAfx.obj \
	$(outdir)\DibChunker.obj \

sources = \
	StdAfx.cpp \
	DibChunker.cpp \

dib_dll = $(dib_dll_name).dll

binaries = \
	$(outdir)\$(dib_dll) \
!if $(miktex_pgoptimize) == 0
	$(outdir)\chunkdib.exe \
!endif

compile = \
	$(cc) $(cppstandard) \
		$(ccopt_enable_rtti_support) \
		$(ccopt_output_directory)$(outdir)\ \
		-DDIB_DLL \
		-DIN_DIB_DLL \
		-DNO_C4P \
		$(ccopt_name_precompiled_header_file)$(outdir)\dib.pch \
		$(ccopt_use_precompiled_header_file)StdAfx.h \

.cpp{$(outdir)\}.obj:
	 $(compile) $<

all: common-all $(libraries)

install: common-install install-binaries

check: common-check sample2e.dvi
	dvips -f1 -p2 -l2 sample2e \
		| C:\gs\gs8.50\bin\gswin32c \
			-sDEVICE=bmp16m \
			-q \
			-dBATCH \
			-dNOPAUSE \
			-dSAFER \
			-sstdout=%%stderr \
			-dTextAlphaBits=4 \
			-dGraphicsAlphaBits=4 \
			-dDOINTERPOLATE \
			-sOutputFile=- \
			- \
		| $(outdir)\chunkdib.exe

sample2e.dvi:
	latex sample2e

# _____________________________________________________________________________
#
# dib.dll
# _____________________________________________________________________________

$(outdir)\$(dib_dll) $(dib_lib): \
			$(outdir) \
			$(dllbases) \
			$(miktex_lib) \
			$(objects) \
			$(outdir)\dib.res \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(dib_dll_name) \
		-implib:$(dib_lib) \
		-out:$(outdir)\$(dib_dll) \
		$(objects) \
		$(outdir)\dib.res \
		$(miktex_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\dib.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			dib-version.h \
			dib.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ dib.rc

# _____________________________________________________________________________
#
# precompiled header
# _____________________________________________________________________________

$(outdir)\StdAfx.obj: $(outdir) StdAfx.cpp
	$(compile) $(ccopt_create_precompiled_header_file)StdAfx.h StdAfx.cpp

# _____________________________________________________________________________
#
# chunkdib
# _____________________________________________________________________________

$(outdir)\chunkdib.obj: $(outdir) chunkdib.cpp
	$(cc) $(cppstandard) \
		$(ccopt_enable_rtti_support) \
		$(ccopt_output_directory)$@ \
		-DDIB_DLL \
		-DNO_C4P \
		chunkdib.cpp

!if $(miktex_pgoptimize) == 0
$(outdir)\chunkdib.exe: \
			$(outdir) \
			$(app_lib) \
			$(dib_lib) \
			$(gnu_lib) \
			$(miktex_lib) \
			$(outdir)\chunkdib.obj \
			$(popt_lib) \

	$(link) $(lstandard) \
		-out:$@ \
		$(outdir)\chunkdib.obj \
		$(app_lib) \
		$(dib_lib) \
		$(gnu_lib) \
		$(miktex_lib) \
		$(popt_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;1
!endif

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean

distclean: common-distclean
	-del /f bitcounts.h

maintainer-clean: common-maintainer-clean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: $(sources)
	$(MAKEDEPEND) $(sources) chunkdib.cpp
	$(fixdep)

# DO NOT DELETE

$(outdir)\StdAfx.obj: StdAfx.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\DibChunker.obj: StdAfx.h
$(outdir)\DibChunker.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\DibChunker.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\DibChunker.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\DibChunker.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\DibChunker.obj: internal.h
$(outdir)\DibChunker.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\chunkdib.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\chunkdib.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\chunkdib.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\chunkdib.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\chunkdib.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\chunkdib.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
