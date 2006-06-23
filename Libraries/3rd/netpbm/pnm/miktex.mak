## miktex.mak: netpbm/pnm
## 
## Copyright (C) 1998-2005 Christian Schenk
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

pbm_lib = ..\pbm\$(outdir)\libpbm.lib
pgm_lib = ..\pgm\$(outdir)\libpgm.lib
ppm_lib = ..\ppm\$(outdir)\libppm.lib

sources = \
	bmptopnm.c \
	fitstopnm.c \
	giftopnm.c \
	libpnm1.c \
	libpnm2.c \
	libpnm3.c \
	libpnm4.c \
	pnmalias.c \
	pnmarith.c \
	pnmcat.c \
	pnmcomp.c \
	pnmconvol.c \
	pnmcrop.c \
	pnmcut.c \
	pnmdepth.c \
	pnmenlarge.c \
	pnmfile.c \
	pnmflip.c \
	pnmgamma.c \
	pnmhistmap.c \
	pnminvert.c \
	pnmmargin.c \
	pnmnlfilt.c \
	pnmnoraw.c \
	pnmpad.c \
	pnmpaste.c \
	pnmrotate.c \
	pnmscale.c \
	pnmshear.c \
	pnmtile.c \
	pnmtoddif.c \
	pnmtofits.c \
	pnmtops.c \
	pnmtorast.c \
	pnmtosgi.c \
	pnmtosir.c \
	pnmtotiff.c \
	pnmtoxwd.c \
	rasttopnm.c \
	sgitopnm.c \
	sirtopnm.c \
	tifftopnm.c \
	xwdtopnm.c \
	zeisstopnm.c \

exes = \
	$(outdir)\bmptopnm.exe \
	$(outdir)\fitstopnm.exe \
	$(outdir)\giftopnm.exe \
	$(outdir)\pnmalias.exe \
	$(outdir)\pnmarith.exe \
	$(outdir)\pnmcat.exe \
	$(outdir)\pnmcomp.exe \
	$(outdir)\pnmconvol.exe \
	$(outdir)\pnmcrop.exe \
	$(outdir)\pnmcut.exe \
	$(outdir)\pnmdepth.exe \
	$(outdir)\pnmenlarge.exe \
	$(outdir)\pnmfile.exe \
	$(outdir)\pnmflip.exe \
	$(outdir)\pnmgamma.exe \
	$(outdir)\pnmhistmap.exe \
	$(outdir)\pnminvert.exe \
	$(outdir)\pnmmargin.exe \
	$(outdir)\pnmnlfilt.exe \
	$(outdir)\pnmnoraw.exe \
	$(outdir)\pnmpad.exe \
	$(outdir)\pnmpaste.exe \
	$(outdir)\pnmrotate.exe \
	$(outdir)\pnmscale.exe \
	$(outdir)\pnmshear.exe \
	$(outdir)\pnmtile.exe \
	$(outdir)\pnmtoddif.exe \
	$(outdir)\pnmtofits.exe \
	$(outdir)\pnmtops.exe \
	$(outdir)\pnmtorast.exe \
	$(outdir)\pnmtosgi.exe \
	$(outdir)\pnmtosir.exe \
	$(outdir)\pnmtotiff.exe \
	$(outdir)\pnmtoxwd.exe \
	$(outdir)\rasttopnm.exe \
	$(outdir)\sgitopnm.exe \
	$(outdir)\sirtopnm.exe \
	$(outdir)\tifftopnm.exe \
	$(outdir)\xwdtopnm.exe \
	$(outdir)\zeisstopnm.exe \

binaries = $(outdir)\$(pnm_dll_name).dll $(exes)

libraries = $(outdir)\libpnm.lib

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		-DLIBTIFF \
		-I. \
		-I.. \
		-I$(tifflibdir) \
		-I..\pbm \
		-I..\pgm \
		-I..\ppm \
		$<

{$(outdir)\}.obj{$(outdir)\}.exe:
	$(link) $(lstandard) \
		-out:$*.exe \
		$(outdir)\libpnm.lib \
		$(pbm_lib) \
		$(pgm_lib) \
		$(ppm_lib) \
		$(tiff_lib) \
		$<
	mt -nologo -manifest $*.exe.manifest -outputresource:$*.exe;1

all: common-all $(binaries) $(libraries)

install: common-install install-binaries

check: common-check

$(exes): $(outdir)\libpnm.lib $(pbm_lib) $(pgm_lib) $(ppm_lib)

$(outdir)\tifftopnm.exe: $(tiff_lib)
$(outdir)\pnmtotiff.exe: $(tiff_lib)

objects = \
	$(outdir)\libpnm1.obj \
	$(outdir)\libpnm2.obj \
	$(outdir)\libpnm3.obj \
	$(outdir)\libpnm4.obj \

$(outdir)\$(pnm_dll_name).dll: \
			$(outdir) \
			$(dllbases) \
			$(objects) \
			$(outdir)\libpnm.exp \
			$(pbm_lib) \
			$(pgm_lib) \
			$(ppm_lib) \

	$(link) $(dlllstandard) \
		-out:$@ \
		-base:@$(dllbases),$(pnm_dll_name) \
		$(objects) \
		$(outdir)\libpnm.exp \
		$(pbm_lib) \
		$(pgm_lib) \
		$(ppm_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\libpnm.lib $(outdir)\libpnm.exp: \
			$(outdir) \
			libpnm.def \

	$(libtool) $(libflags) \
		-def:libpnm.def \
		-name:$(pnm_dll_name) \
		-out:$(outdir)\libpnm.lib \

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend:
	$(MAKEDEPEND) -I.. $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\bmptopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/bmp.h
$(outdir)\bmptopnm.obj: ../pbmplus.h pnm.h
$(outdir)\bmptopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\bmptopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\bmptopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\bmptopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/bitio.h
$(outdir)\fitstopnm.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\fitstopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\fitstopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\fitstopnm.obj: ../pbmplus.h
$(outdir)\giftopnm.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\giftopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\giftopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\giftopnm.obj: ../pbmplus.h
$(outdir)\libpnm1.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\libpnm1.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\libpnm1.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\libpnm1.obj: ../pbmplus.h
$(outdir)\libpnm1.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/libppm.h
$(outdir)\libpnm1.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/libpgm.h
$(outdir)\libpnm1.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/libpbm.h
$(outdir)\libpnm2.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\libpnm2.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\libpnm2.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\libpnm2.obj: ../pbmplus.h
$(outdir)\libpnm2.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/libppm.h
$(outdir)\libpnm2.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/libpgm.h
$(outdir)\libpnm2.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/libpbm.h
$(outdir)\libpnm3.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\libpnm3.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\libpnm3.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\libpnm3.obj: ../pbmplus.h
$(outdir)\libpnm3.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/libppm.h
$(outdir)\libpnm3.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/libpgm.h
$(outdir)\libpnm3.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/libpbm.h
$(outdir)\libpnm4.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\libpnm4.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\libpnm4.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\libpnm4.obj: ../pbmplus.h rast.h
$(outdir)\pnmalias.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmalias.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmalias.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmalias.obj: ../pbmplus.h
$(outdir)\pnmarith.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmarith.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmarith.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmarith.obj: ../pbmplus.h
$(outdir)\pnmcat.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmcat.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmcat.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmcat.obj: ../pbmplus.h
$(outdir)\pnmcomp.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmcomp.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmcomp.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmcomp.obj: ../pbmplus.h
$(outdir)\pnmconvol.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmconvol.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmconvol.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmconvol.obj: ../pbmplus.h
$(outdir)\pnmcrop.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmcrop.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmcrop.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmcrop.obj: ../pbmplus.h
$(outdir)\pnmcut.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmcut.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmcut.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmcut.obj: ../pbmplus.h
$(outdir)\pnmdepth.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmdepth.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmdepth.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmdepth.obj: ../pbmplus.h
$(outdir)\pnmenlarge.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmenlarge.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmenlarge.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmenlarge.obj: ../pbmplus.h
$(outdir)\pnmfile.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmfile.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmfile.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmfile.obj: ../pbmplus.h
$(outdir)\pnmflip.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmflip.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmflip.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmflip.obj: ../pbmplus.h
$(outdir)\pnmgamma.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmgamma.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmgamma.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmgamma.obj: ../pbmplus.h
$(outdir)\pnmhistmap.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmhistmap.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmhistmap.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmhistmap.obj: ../pbmplus.h
$(outdir)\pnminvert.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnminvert.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnminvert.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnminvert.obj: ../pbmplus.h
$(outdir)\pnmmargin.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmmargin.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmmargin.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmmargin.obj: ../pbmplus.h
$(outdir)\pnmnlfilt.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmnlfilt.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmnlfilt.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmnlfilt.obj: ../pbmplus.h
$(outdir)\pnmnoraw.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmnoraw.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmnoraw.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmnoraw.obj: ../pbmplus.h
$(outdir)\pnmpad.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmpad.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmpad.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmpad.obj: ../pbmplus.h
$(outdir)\pnmpaste.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmpaste.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmpaste.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmpaste.obj: ../pbmplus.h
$(outdir)\pnmrotate.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmrotate.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmrotate.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmrotate.obj: ../pbmplus.h
$(outdir)\pnmscale.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmscale.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmscale.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmscale.obj: ../pbmplus.h
$(outdir)\pnmshear.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmshear.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmshear.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmshear.obj: ../pbmplus.h
$(outdir)\pnmtile.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmtile.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmtile.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmtile.obj: ../pbmplus.h
$(outdir)\pnmtoddif.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmtoddif.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmtoddif.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmtoddif.obj: ../pbmplus.h
$(outdir)\pnmtofits.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmtofits.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmtofits.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmtofits.obj: ../pbmplus.h
$(outdir)\pnmtops.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmtops.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmtops.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmtops.obj: ../pbmplus.h
$(outdir)\pnmtorast.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmtorast.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmtorast.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmtorast.obj: ../pbmplus.h rast.h
$(outdir)\pnmtorast.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppmcmap.h
$(outdir)\pnmtosgi.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmtosgi.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmtosgi.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmtosgi.obj: ../pbmplus.h sgi.h
$(outdir)\pnmtosir.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmtosir.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmtosir.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmtosir.obj: ../pbmplus.h
$(outdir)\pnmtosir.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppmcmap.h
$(outdir)\pnmtotiff.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmtotiff.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmtotiff.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmtotiff.obj: ../pbmplus.h
$(outdir)\pnmtotiff.obj: $(miktexsrcdir)/Libraries\3rd\libtiff/tiffio.h
$(outdir)\pnmtotiff.obj: $(miktexsrcdir)/Libraries\3rd\libtiff/tiff.h
$(outdir)\pnmtotiff.obj: $(miktexsrcdir)/Libraries\3rd\libtiff/tiffvers.h
$(outdir)\pnmtotiff.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppmcmap.h
$(outdir)\pnmtoxwd.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\pnmtoxwd.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\pnmtoxwd.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\pnmtoxwd.obj: ../pbmplus.h x11wd.h
$(outdir)\pnmtoxwd.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppmcmap.h
$(outdir)\rasttopnm.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\rasttopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\rasttopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\rasttopnm.obj: ../pbmplus.h rast.h
$(outdir)\sgitopnm.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\sgitopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\sgitopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\sgitopnm.obj: ../pbmplus.h sgi.h
$(outdir)\sirtopnm.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\sirtopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\sirtopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\sirtopnm.obj: ../pbmplus.h
$(outdir)\tifftopnm.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\tifftopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\tifftopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\tifftopnm.obj: ../pbmplus.h
$(outdir)\tifftopnm.obj: $(miktexsrcdir)/Libraries\3rd\libtiff/tiffio.h
$(outdir)\tifftopnm.obj: $(miktexsrcdir)/Libraries\3rd\libtiff/tiff.h
$(outdir)\tifftopnm.obj: $(miktexsrcdir)/Libraries\3rd\libtiff/tiffvers.h
$(outdir)\xwdtopnm.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\xwdtopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\xwdtopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\xwdtopnm.obj: ../pbmplus.h x10wd.h x11wd.h
$(outdir)\zeisstopnm.obj: pnm.h $(miktexsrcdir)/Libraries/3rd/netpbm/ppm/ppm.h
$(outdir)\zeisstopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pgm/pgm.h
$(outdir)\zeisstopnm.obj: $(miktexsrcdir)/Libraries/3rd/netpbm/pbm/pbm.h
$(outdir)\zeisstopnm.obj: ../pbmplus.h
