## miktex.mak: netpbm/ppm
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

!include <miktex.inc>

pbm_lib = ..\pbm\$(outdir)\libpbm.lib
pgm_lib = ..\pgm\$(outdir)\libpgm.lib

sources = \
	bmptoppm.c \
	gouldtoppm.c \
	hpcdtoppm.c \
	ilbmtoppm.c \
	imgtoppm.c \
	libppm1.c \
	libppm2.c \
	libppm3.c \
	libppm4.c \
	libppm5.c \
	mtvtoppm.c \
	pcxtoppm.c \
	pgmtoppm.c \
	pi1toppm.c \
	picttoppm.c \
	pjtoppm.c \
	ppm3d.c \
	ppmbrighten.c \
	ppmchange.c \
	ppmdim.c \
	ppmdist.c \
	ppmdither.c \
	ppmflash.c \
	ppmforge.c \
	ppmhist.c \
	ppmmake.c \
	ppmmix.c \
	ppmnorm.c \
	ppmntsc.c \
	ppmpat.c \
	ppmquant.c \
	ppmqvga.c \
	ppmrelief.c \
	ppmshift.c \
	ppmspread.c \
	ppmtoacad.c \
	ppmtobmp.c \
	ppmtogif.c \
	ppmtoicr.c \
	ppmtoilbm.c \
	ppmtomap.c \
	ppmtomitsu.c \
	ppmtopcx.c \
	ppmtopgm.c \
	ppmtopi1.c \
	ppmtopict.c \
	ppmtopj.c \
	ppmtopjxl.c \
	ppmtopuzz.c \
	ppmtorgb3.c \
	ppmtosixel.c \
	ppmtotga.c \
	ppmtouil.c \
	ppmtoxpm.c \
	ppmtoyuv.c \
	ppmtoyuvsplit.c \
	qrttoppm.c \
	rawtoppm.c \
	rgb3toppm.c \
	sldtoppm.c \
	spctoppm.c \
	sputoppm.c \
	tgatoppm.c \
	ximtoppm.c \
	xpmtoppm.c \
	xvminitoppm.c \
	yuvsplittoppm.c \
	yuvtoppm.c \

exes = \
	$(outdir)\bmptoppm.exe \
	$(outdir)\gouldtoppm.exe \
	$(outdir)\hpcdtoppm.exe \
	$(outdir)\ilbmtoppm.exe \
	$(outdir)\imgtoppm.exe \
	$(outdir)\mtvtoppm.exe \
	$(outdir)\pcxtoppm.exe \
	$(outdir)\pgmtoppm.exe \
	$(outdir)\pi1toppm.exe \
	$(outdir)\picttoppm.exe \
	$(outdir)\pjtoppm.exe \
	$(outdir)\ppm3d.exe \
	$(outdir)\ppmbrighten.exe \
	$(outdir)\ppmchange.exe \
	$(outdir)\ppmdim.exe \
	$(outdir)\ppmdist.exe \
	$(outdir)\ppmdither.exe \
	$(outdir)\ppmflash.exe \
	$(outdir)\ppmforge.exe \
	$(outdir)\ppmhist.exe \
	$(outdir)\ppmmake.exe \
	$(outdir)\ppmmix.exe \
	$(outdir)\ppmnorm.exe \
	$(outdir)\ppmntsc.exe \
	$(outdir)\ppmpat.exe \
	$(outdir)\ppmquant.exe \
	$(outdir)\ppmqvga.exe \
	$(outdir)\ppmrelief.exe \
	$(outdir)\ppmshift.exe \
	$(outdir)\ppmspread.exe \
	$(outdir)\ppmtoacad.exe \
	$(outdir)\ppmtobmp.exe \
	$(outdir)\ppmtogif.exe \
	$(outdir)\ppmtoicr.exe \
	$(outdir)\ppmtoilbm.exe \
	$(outdir)\ppmtomap.exe \
	$(outdir)\ppmtomitsu.exe \
	$(outdir)\ppmtopcx.exe \
	$(outdir)\ppmtopgm.exe \
	$(outdir)\ppmtopi1.exe \
	$(outdir)\ppmtopict.exe \
	$(outdir)\ppmtopj.exe \
	$(outdir)\ppmtopjxl.exe \
	$(outdir)\ppmtopuzz.exe \
	$(outdir)\ppmtorgb3.exe \
	$(outdir)\ppmtosixel.exe \
	$(outdir)\ppmtotga.exe \
	$(outdir)\ppmtouil.exe \
	$(outdir)\ppmtoxpm.exe \
	$(outdir)\ppmtoyuv.exe \
	$(outdir)\ppmtoyuvsplit.exe \
	$(outdir)\qrttoppm.exe \
	$(outdir)\rawtoppm.exe \
	$(outdir)\rgb3toppm.exe \
	$(outdir)\sldtoppm.exe \
	$(outdir)\spctoppm.exe \
	$(outdir)\sputoppm.exe \
	$(outdir)\tgatoppm.exe \
	$(outdir)\ximtoppm.exe \
	$(outdir)\xpmtoppm.exe \
	$(outdir)\xvminitoppm.exe \
	$(outdir)\yuvsplittoppm.exe \
	$(outdir)\yuvtoppm.exe \

binaries = $(outdir)\$(ppm_dll_name).dll $(exes)

libraries = $(outdir)\libppm.lib

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		-I. \
		-I.. \
		-I..\pbm \
		-I..\pgm \
		$<

{$(outdir)\}.obj{$(outdir)\}.exe:
	$(link) $(lstandard) \
		-out:$*.exe \
		$(outdir)\libppm.lib \
		$(pbm_lib) \
		$(pgm_lib) \
		$<
	mt -nologo -manifest $*.exe.manifest -outputresource:$*.exe;1

all: common-all $(binaries) $(libraries)

install: common-install install-binaries

check: common-check

$(exes): $(outdir)\libppm.lib $(pbm_lib) $(pgm_lib)

objects = \
	$(outdir)\bitio.obj \
	$(outdir)\libppm1.obj \
	$(outdir)\libppm2.obj \
	$(outdir)\libppm3.obj \
	$(outdir)\libppm4.obj \
	$(outdir)\libppm5.obj \

$(outdir)\$(ppm_dll_name).dll: \
			$(outdir) \
			$(dllbases) \
			$(lib_pgm) \
			$(objects) \
			$(outdir)\libppm.exp \
			$(pbm_lib) \

	$(link) $(dlllstandard) \
		-out:$@ -base:@$(dllbases),$(ppm_dll_name) \
		$(objects) \
		$(outdir)\libppm.exp \
		$(pbm_lib) \
		$(pgm_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\libppm.lib $(outdir)\libppm.exp: \
			$(outdir) \
			libppm.def \

	$(libtool) $(libflags) \
		-def:libppm.def \
		-name:$(ppm_dll_name) \
		-out:$(outdir)\libppm.lib \

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend:
	$(MAKEDEPEND) -I.. $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\bmptoppm.obj: bmp.h ../pbmplus.h ppm.h
$(outdir)\bmptoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\bmptoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h bitio.h
$(outdir)\gouldtoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\gouldtoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\gouldtoppm.obj: ../pbmplus.h
$(outdir)\hpcdtoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\hpcdtoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\hpcdtoppm.obj: ../pbmplus.h
$(outdir)\ilbmtoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ilbmtoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ilbmtoppm.obj: ../pbmplus.h ilbm.h
$(outdir)\imgtoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\imgtoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\imgtoppm.obj: ../pbmplus.h
$(outdir)\libppm1.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\libppm1.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\libppm1.obj: ../pbmplus.h libppm.h
$(outdir)\libppm1.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/libpgm.h
$(outdir)\libppm1.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/libpbm.h
$(outdir)\libppm2.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\libppm2.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\libppm2.obj: ../pbmplus.h libppm.h
$(outdir)\libppm3.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\libppm3.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\libppm3.obj: ../pbmplus.h ppmcmap.h libppm.h
$(outdir)\libppm4.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\libppm4.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\libppm4.obj: ../pbmplus.h
$(outdir)\libppm5.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\libppm5.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\libppm5.obj: ../pbmplus.h ppmdraw.h
$(outdir)\mtvtoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\mtvtoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\mtvtoppm.obj: ../pbmplus.h
$(outdir)\pcxtoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\pcxtoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pcxtoppm.obj: ../pbmplus.h
$(outdir)\pgmtoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\pgmtoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pgmtoppm.obj: ../pbmplus.h
$(outdir)\pi1toppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\pi1toppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pi1toppm.obj: ../pbmplus.h
$(outdir)\picttoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\picttoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\picttoppm.obj: ../pbmplus.h
$(outdir)\picttoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbmfont.h
$(outdir)\pjtoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\pjtoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pjtoppm.obj: ../pbmplus.h
$(outdir)\ppm3d.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppm3d.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppm3d.obj: ../pbmplus.h
$(outdir)\ppmbrighten.obj: ppm.h
$(outdir)\ppmbrighten.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmbrighten.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmbrighten.obj: ../pbmplus.h
$(outdir)\ppmchange.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmchange.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmchange.obj: ../pbmplus.h
$(outdir)\ppmdim.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmdim.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmdim.obj: ../pbmplus.h
$(outdir)\ppmdist.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmdist.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmdist.obj: ../pbmplus.h ppmcmap.h
$(outdir)\ppmdither.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmdither.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmdither.obj: ../pbmplus.h
$(outdir)\ppmflash.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmflash.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmflash.obj: ../pbmplus.h
$(outdir)\ppmforge.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmforge.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmforge.obj: ../pbmplus.h
$(outdir)\ppmhist.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmhist.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmhist.obj: ../pbmplus.h ppmcmap.h
$(outdir)\ppmmake.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmmake.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmmake.obj: ../pbmplus.h
$(outdir)\ppmmix.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmmix.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmmix.obj: ../pbmplus.h
$(outdir)\ppmnorm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmnorm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmnorm.obj: ../pbmplus.h lum.h
$(outdir)\ppmntsc.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmntsc.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmntsc.obj: ../pbmplus.h
$(outdir)\ppmpat.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmpat.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmpat.obj: ../pbmplus.h ppmdraw.h
$(outdir)\ppmquant.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmquant.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmquant.obj: ../pbmplus.h ppmcmap.h
$(outdir)\ppmqvga.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmqvga.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmqvga.obj: ../pbmplus.h
$(outdir)\ppmrelief.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmrelief.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmrelief.obj: ../pbmplus.h
$(outdir)\ppmshift.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmshift.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmshift.obj: ../pbmplus.h
$(outdir)\ppmspread.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmspread.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmspread.obj: ../pbmplus.h
$(outdir)\ppmtoacad.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtoacad.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtoacad.obj: ../pbmplus.h ppmcmap.h autocad.h
$(outdir)\ppmtobmp.obj: bmp.h ../pbmplus.h ppm.h
$(outdir)\ppmtobmp.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtobmp.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtobmp.obj: ppmcmap.h bitio.h
$(outdir)\ppmtogif.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtogif.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtogif.obj: ../pbmplus.h ppmcmap.h
$(outdir)\ppmtoicr.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtoicr.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtoicr.obj: ../pbmplus.h ppmcmap.h
$(outdir)\ppmtoilbm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtoilbm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtoilbm.obj: ../pbmplus.h ppmcmap.h ilbm.h
$(outdir)\ppmtomap.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtomap.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtomap.obj: ../pbmplus.h ppmcmap.h
$(outdir)\ppmtomitsu.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtomitsu.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtomitsu.obj: ../pbmplus.h ppmcmap.h mitsu.h
$(outdir)\ppmtopcx.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtopcx.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtopcx.obj: ../pbmplus.h ppmcmap.h
$(outdir)\ppmtopgm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtopgm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtopgm.obj: ../pbmplus.h
$(outdir)\ppmtopi1.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtopi1.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtopi1.obj: ../pbmplus.h ppmcmap.h
$(outdir)\ppmtopict.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtopict.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtopict.obj: ../pbmplus.h ppmcmap.h
$(outdir)\ppmtopj.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtopj.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtopj.obj: ../pbmplus.h
$(outdir)\ppmtopjxl.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtopjxl.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtopjxl.obj: ../pbmplus.h ppmcmap.h
$(outdir)\ppmtopuzz.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtopuzz.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtopuzz.obj: ../pbmplus.h ppmcmap.h
$(outdir)\ppmtorgb3.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtorgb3.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtorgb3.obj: ../pbmplus.h
$(outdir)\ppmtosixel.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtosixel.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtosixel.obj: ../pbmplus.h ppmcmap.h
$(outdir)\ppmtotga.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtotga.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtotga.obj: ../pbmplus.h ppmcmap.h tga.h
$(outdir)\ppmtouil.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtouil.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtouil.obj: ../pbmplus.h ppmcmap.h
$(outdir)\ppmtoxpm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtoxpm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtoxpm.obj: ../pbmplus.h ppmcmap.h
$(outdir)\ppmtoyuv.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtoyuv.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtoyuv.obj: ../pbmplus.h
$(outdir)\ppmtoyuvsplit.obj: ppm.h
$(outdir)\ppmtoyuvsplit.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ppmtoyuvsplit.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ppmtoyuvsplit.obj: ../pbmplus.h
$(outdir)\qrttoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\qrttoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\qrttoppm.obj: ../pbmplus.h
$(outdir)\rawtoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\rawtoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\rawtoppm.obj: ../pbmplus.h
$(outdir)\rgb3toppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\rgb3toppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\rgb3toppm.obj: ../pbmplus.h
$(outdir)\sldtoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\sldtoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\sldtoppm.obj: ../pbmplus.h ppmdraw.h autocad.h
$(outdir)\spctoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\spctoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\spctoppm.obj: ../pbmplus.h
$(outdir)\sputoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\sputoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\sputoppm.obj: ../pbmplus.h
$(outdir)\tgatoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\tgatoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\tgatoppm.obj: ../pbmplus.h tga.h
$(outdir)\ximtoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\ximtoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\ximtoppm.obj: ../pbmplus.h xim.h
$(outdir)\xpmtoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\xpmtoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\xpmtoppm.obj: ../pbmplus.h
$(outdir)\xvminitoppm.obj: ppm.h
$(outdir)\xvminitoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\xvminitoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\xvminitoppm.obj: ../pbmplus.h
$(outdir)\yuvsplittoppm.obj: ppm.h
$(outdir)\yuvsplittoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\yuvsplittoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\yuvsplittoppm.obj: ../pbmplus.h
$(outdir)\yuvtoppm.obj: ppm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pgm/pgm.h
$(outdir)\yuvtoppm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\yuvtoppm.obj: ../pbmplus.h
