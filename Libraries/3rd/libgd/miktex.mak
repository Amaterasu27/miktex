## miktex.mak: libgd
##
## Copyright (C) 2004-2005 Christian Schenk
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
	$(outdir)\gd.obj \
	$(outdir)\gd_gd.obj \
	$(outdir)\gd_gd2.obj \
	$(outdir)\gd_gif_in.obj \
	$(outdir)\gd_io.obj \
	$(outdir)\gd_io_dp.obj \
	$(outdir)\gd_io_file.obj \
	$(outdir)\gd_io_ss.obj \
	$(outdir)\gd_jpeg.obj \
	$(outdir)\gd_png.obj \
	$(outdir)\gd_ss.obj \
	$(outdir)\gd_topal.obj \
	$(outdir)\gd_wbmp.obj \
	$(outdir)\gdcache.obj \
	$(outdir)\gdfontg.obj \
	$(outdir)\gdfontl.obj \
	$(outdir)\gdfontmb.obj \
	$(outdir)\gdfonts.obj \
	$(outdir)\gdfontt.obj \
	$(outdir)\gdft.obj \
	$(outdir)\gdhelpers.obj \
	$(outdir)\gdkanji.obj \
	$(outdir)\gdtables.obj \
	$(outdir)\gdxpm.obj \
	$(outdir)\wbmp.obj \

sources = \
	gd.c \
	gd_gd.c \
	gd_gd2.c \
	gd_gif_in.c \
	gd_io.c \
	gd_io_dp.c \
	gd_io_file.c \
	gd_io_ss.c \
	gd_jpeg.c \
	gd_png.c \
	gd_ss.c \
	gd_topal.c \
	gd_wbmp.c \
	gdcache.c \
	gdfontg.c \
	gdfontl.c \
	gdfontmb.c \
	gdfonts.c \
	gdfontt.c \
	gdft.c \
	gdhelpers.c \
	gdkanji.c \
	gdtables.c \
	gdxpm.c \
	wbmp.c \

binaries = $(outdir)\$(gd_dll_name).dll

extra_defines = \
	-DBGDWIN32 \
	-DHAVE_LIBJPEG \
	-DHAVE_LIBPNG \
	-DPATHSEPARATOR=\";\" \
	-Dinline=__inline \

extra_includes = \
	-I$(jpeglibdir) \
	$(ccinc_libpng) \
	$(ccinc_zlib) \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(extra_defines) \
		$(extra_includes) \
		$(ccopt_output_directory)$(outdir)\ $<

all: common-all

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# gd.dll
# _____________________________________________________________________________

libs = \
	$(jpeg_lib) \
	$(png_lib) \
	$(zlib_lib) \

$(outdir)\$(gd_dll_name).dll $(gd_lib): \
			$(outdir) \
			$(dllbases) \
			$(libs) \
			$(objects) \
			$(outdir)\gd.res \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(gd_dll_name) \
		-implib:$(gd_lib) \
		-out:$(outdir)\$(gd_dll_name).dll \
		$(objects) \
		$(libs) \
		$(outdir)\gd.res
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\gd.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		gd-version.h \
		gd.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ gd.rc

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
	$(MAKEDEPEND) $(extra_defines) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\gd.obj: gd.h gd_io.h gdfx.h gdhelpers.h
$(outdir)\gd_gd.obj: gd.h gd_io.h gdfx.h
$(outdir)\gd_gd2.obj: gd.h gd_io.h gdfx.h gdhelpers.h
$(outdir)\gd_gif_in.obj: gd.h gd_io.h gdfx.h
$(outdir)\gd_io.obj: gd.h gd_io.h gdfx.h
$(outdir)\gd_io_dp.obj: gd.h gd_io.h gdfx.h gdhelpers.h
$(outdir)\gd_io_file.obj: gd.h gd_io.h gdfx.h gdhelpers.h
$(outdir)\gd_io_ss.obj: gd.h gd_io.h gdfx.h gdhelpers.h
$(outdir)\gd_jpeg.obj: gd.h gd_io.h gdfx.h gdhelpers.h
$(outdir)\gd_jpeg.obj: $(miktexsrcdir)/Libraries/3rd/libjpeg/jpeglib.h
$(outdir)\gd_jpeg.obj: $(miktexsrcdir)/Libraries/3rd/libjpeg/jconfig.h
$(outdir)\gd_jpeg.obj: $(miktexsrcdir)/Libraries/3rd/libjpeg/jmorecfg.h
$(outdir)\gd_jpeg.obj: $(miktexsrcdir)/Libraries/3rd/libjpeg/jerror.h
$(outdir)\gd_png.obj: gd.h gd_io.h gdfx.h gdhelpers.h
$(outdir)\gd_png.obj: $(miktexsrcdir)/Libraries/3rd/libpng/png.h
$(outdir)\gd_png.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\gd_png.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\gd_png.obj: $(miktexsrcdir)/Libraries/3rd/libpng/pngconf.h
$(outdir)\gd_ss.obj: gd.h gd_io.h gdfx.h
$(outdir)\gd_topal.obj: gd.h gd_io.h gdfx.h gdhelpers.h
$(outdir)\gd_wbmp.obj: gd.h gd_io.h gdfx.h ./gdfonts.h wbmp.h
$(outdir)\gdcache.obj: gd.h gd_io.h gdfx.h gdhelpers.h
$(outdir)\gdfontg.obj: gdfontg.h gd.h gd_io.h gdfx.h
$(outdir)\gdfontl.obj: gdfontl.h gd.h gd_io.h gdfx.h
$(outdir)\gdfontmb.obj: gdfontmb.h gd.h gd_io.h gdfx.h
$(outdir)\gdfonts.obj: ./gdfonts.h gd.h gd_io.h gdfx.h
$(outdir)\gdfontt.obj: gdfontt.h gd.h gd_io.h gdfx.h
$(outdir)\gdft.obj: gd.h gd_io.h gdfx.h gdhelpers.h entities.h
$(outdir)\gdhelpers.obj: gd.h gd_io.h gdfx.h gdhelpers.h
$(outdir)\gdkanji.obj: gd.h gd_io.h gdfx.h gdhelpers.h
$(outdir)\gdxpm.obj: gd.h gd_io.h gdfx.h gdhelpers.h
$(outdir)\wbmp.obj: wbmp.h gd.h gd_io.h gdfx.h gdhelpers.h
