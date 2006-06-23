## miktex.mak: pnmtopng
##
## Copyright (C) 1998-2004 Christian Schenk
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

#miktex_cc_no_runtime_checks = 1

!include <miktex.inc>

sources = \
	pnmtopng.c \
	pngtopnm.c \

libs = \
	$(pbm_lib) \
	$(pgm_lib) \
	$(png_lib) \
	$(pnm_lib) \
	$(ppm_lib) \
	$(zlib_lib) \

defines = \
	-D__WIN32DLL__ \

cc_options = \
	$(cstandard) \
	$(defines) \
	$(includes) \

.c{$(outdir)\}.obj:
	$(cc) $(cc_options) $(ccopt_output_directory)$(outdir)\ $<

binaries = $(outdir)\pnmtopng.exe $(outdir)\pngtopnm.exe

all: common-all $(binaries)

$(outdir)\pnmtopng.exe: \
			$(outdir) \
			$(libs) \
			$(outdir)\pnmtopng.obj \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(outdir)\pnmtopng.obj \
		$(libs) 
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\pngtopnm.exe: \
			$(outdir) \
			$(libs) \
			$(outdir)\pngtopnm.obj \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(outdir)\pngtopnm.obj \
		$(libs)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

install: common-install install-binaries

check: common-check

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend:
	$(MAKEDEPEND) $(includes) $(defines) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\pnmtopng.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pnm/pnm.h
$(outdir)\pnmtopng.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmtopng.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmtopng.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmtopng.obj: $(miktexsrcdir)/Libraries/3rd/libpng/png.h
$(outdir)\pnmtopng.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pnmtopng.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\pnmtopng.obj: $(miktexsrcdir)/Libraries/3rd/libpng/pngconf.h
$(outdir)\pnmtopng.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppmcmap.h
$(outdir)\pngtopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pnm/pnm.h
$(outdir)\pngtopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pngtopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pngtopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pngtopnm.obj: $(miktexsrcdir)/Libraries/3rd/libpng/png.h
$(outdir)\pngtopnm.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngtopnm.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\pngtopnm.obj: $(miktexsrcdir)/Libraries/3rd/libpng/pngconf.h
