## miktex.mak: libbmeps
## 
## Copyright (C) 2001-2005 Christian Schenk
## 
## This file is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
## 
## This file is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with This file; if not, write to the Free Software Foundation,
## 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

!include <miktex.inc>

sources = \
	bmeps.c \
	bmepsm.c \
	bmepsoe.c \
	jpgeps.c \
	pngeps.c \
	pnmeps.c \
#	tifeps.c \

defines = \
#	-D_TIFFfree=free \

includes = \
	-I$(jpeglibdir) \
	-I$(netpbmdir) \
	-I$(pbmlibdir) \
	-I$(pgmlibdir) \
	$(ccinc_libpng) \
	-I$(pnmlibdir) \
	-I$(ppmlibdir) \
#	-I$(tifflibdir) \
	$(ccinc_zlib) \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		$(defines) \
		$(includes) \
		$<

bmeps_dll = $(bmeps_dll_name).dll

binaries = $(outdir)\$(bmeps_dll) $(outdir)\bmeps.exe

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# -----------------------------------------------------------------------------
# bmeps.dll
# -----------------------------------------------------------------------------

dll_objects =	\
	$(outdir)\bmeps.obj \
	$(outdir)\bmepsoe.obj \
	$(outdir)\jpgeps.obj \
	$(outdir)\libbmeps.exp \
	$(outdir)\libbmeps.res \
	$(outdir)\pngeps.obj \
	$(outdir)\pnmeps.obj \
#	$(outdir)\tifeps.obj \

libs = \
	$(jpeg_lib) \
	$(pbm_lib) \
	$(png_lib) \
	$(pnm_lib) \
#	$(tiff_lib) \
	$(zlib_lib) \

$(outdir)\$(bmeps_dll): \
			$(outdir) \
			$(dll_objects) \
			$(dllbases) \
			$(libs) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(bmeps_dll_name) \
		-delayload:$(jpeg_dll_name).dll \
		-delayload:$(png_dll_name).dll \
		-delayload:$(pnm_dll_name).dll \
#		-delayload:$(tiff_dll_name).dll \
		-delayload:$(zlib_dll_name).dll \
		-out:$@ \
		$(dll_objects) \
		$(libs) \
		delayimp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\libbmeps.lib $(outdir)\libbmeps.exp: \
			$(outdir) \
			libbmeps.def \

	$(libtool) $(libflags) \
		-def:libbmeps.def \
		-name:$(bmeps_dll_name) \
		-out:$(outdir)\libbmeps.lib \

$(outdir)\libbmeps.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			libbmeps.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ libbmeps.rc

# -----------------------------------------------------------------------------
# bmeps.exe
# -----------------------------------------------------------------------------

bmeps_objects = $(outdir)\bmepsm.obj $(outdir)\libbmeps.lib $(outdir)\bmeps.res

$(outdir)\bmeps.exe: $(outdir) $(bmeps_objects)
	$(link) $(conlflags) $(ldebug) \
#		-order:@bmeps.prf \
		-out:$@ \
		$(bmeps_objects) $(conlibsdll)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\bmeps.res: $(outdir) bmeps.rc
	$(rc) $(rcstandard) $(rcopt_output_file)$@ bmeps.rc

# -----------------------------------------------------------------------------
# clean-up
# -----------------------------------------------------------------------------

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

# -----------------------------------------------------------------------------
# dependencies
# -----------------------------------------------------------------------------

depend: $(sources)
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\bmeps.obj: bmepsco.h pngeps.h jpgeps.h pnmeps.h bmepsoe.h
$(outdir)\bmeps.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zlib.h
$(outdir)\bmeps.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zconf.h bmeps.h
$(outdir)\bmepsm.obj: bmepsco.h bmeps.h bmepsoe.h
$(outdir)\bmepsm.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zlib.h
$(outdir)\bmepsm.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zconf.h pngeps.h
$(outdir)\bmepsoe.obj: bmepsco.h bmeps.h pngeps.h bmepsoe.h
$(outdir)\bmepsoe.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zlib.h
$(outdir)\bmepsoe.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zconf.h
$(outdir)\jpgeps.obj: bmeps.h bmepsoe.h
$(outdir)\jpgeps.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zlib.h
$(outdir)\jpgeps.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zconf.h bmepsco.h
$(outdir)\jpgeps.obj: $(miktexsrcdir)/Libraries\3rd\libjpeg/jpeglib.h
$(outdir)\jpgeps.obj: $(miktexsrcdir)/Libraries\3rd\libjpeg/jconfig.h
$(outdir)\jpgeps.obj: $(miktexsrcdir)/Libraries\3rd\libjpeg/jmorecfg.h
$(outdir)\pngeps.obj: bmepsco.h pngeps.h bmeps.h bmepsoe.h
$(outdir)\pngeps.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zlib.h
$(outdir)\pngeps.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zconf.h
$(outdir)\pngeps.obj: $(miktexsrcdir)/Libraries\3rd\libpng/png.h
$(outdir)\pngeps.obj: $(miktexsrcdir)/Libraries\3rd\libpng/pngconf.h
$(outdir)\pnmeps.obj: bmeps.h bmepsoe.h
$(outdir)\pnmeps.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zlib.h
$(outdir)\pnmeps.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zconf.h bmepsco.h
$(outdir)\pnmeps.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pnm/pnm.h
$(outdir)\pnmeps.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/ppm/ppm.h
$(outdir)\pnmeps.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\pnmeps.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
