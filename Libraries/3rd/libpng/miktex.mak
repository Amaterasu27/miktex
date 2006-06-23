## miktex.mak: libpng
##
## Copyright (C) 2000-2005 Christian Schenk
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
	$(outdir)\png.obj \
	$(outdir)\pngerror.obj \
	$(outdir)\pngget.obj \
	$(outdir)\pngmem.obj \
	$(outdir)\pngpread.obj \
	$(outdir)\pngread.obj \
	$(outdir)\pngrio.obj \
	$(outdir)\pngrtran.obj \
	$(outdir)\pngrutil.obj \
	$(outdir)\pngset.obj \
	$(outdir)\pngtrans.obj \
	$(outdir)\pngvcrd.obj \
	$(outdir)\pngwio.obj \
	$(outdir)\pngwrite.obj \
	$(outdir)\pngwtran.obj \
	$(outdir)\pngwutil.obj \

sources = \
	png.c \
	pngerror.c \
	pngget.c \
	pngmem.c \
	pngpread.c \
	pngread.c \
	pngrio.c \
	pngrtran.c \
	pngrutil.c \
	pngset.c \
	pngtrans.c \
	pngvcrd.c \
	pngwio.c \
	pngwrite.c \
	pngwutil.c \
	pngwtran.c \

libraries = $(png_lib)

binaries = $(outdir)\$(png_dll_name).dll

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		-DPNG_BUILD_DLL \
		-DZLIB_DLL \
		-D_WINDLL \
		$(ccopt_output_file)$@ \
		$<

all: common-all

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# png.dll
# _____________________________________________________________________________

$(outdir)\$(png_dll_name).dll $(png_lib): \
			$(outdir) \
			$(objects)\
			$(outdir)\libpng.res \
			$(zlib_lib) \
			$(dllbases) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(png_dll_name) \
		-def:libpng.def \
		-implib:$(png_lib) \
		-out:$(outdir)\$(png_dll_name).dll \
		$(objects) \
		$(outdir)\libpng.res \
		$(zlib_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\libpng.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			libpng-version.h \
			libpng.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ libpng.rc

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

$(outdir)\png.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\png.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
$(outdir)\pngerror.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngerror.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
$(outdir)\pngget.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngget.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
$(outdir)\pngmem.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngmem.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
$(outdir)\pngpread.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngpread.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
$(outdir)\pngread.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngread.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
$(outdir)\pngrio.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngrio.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
$(outdir)\pngrtran.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngrtran.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
$(outdir)\pngrutil.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngrutil.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
$(outdir)\pngset.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngset.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
$(outdir)\pngtrans.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngtrans.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
$(outdir)\pngvcrd.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngvcrd.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
$(outdir)\pngwio.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngwio.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
$(outdir)\pngwrite.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngwrite.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
$(outdir)\pngwutil.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngwutil.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
$(outdir)\pngwtran.obj: png.h $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngwtran.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h pngconf.h
