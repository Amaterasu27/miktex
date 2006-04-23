## miktex.mak: netpbm/pgm
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

pbm_lib = ..\pbm\$(outdir)\libpbm.lib

sources = \
	asciitopgm.c \
	bioradtopgm.c \
	fstopgm.c \
	hipstopgm.c \
	libpgm1.c \
	libpgm2.c \
	lispmtopgm.c \
	pbmtopgm.c \
	pgmbentley.c \
	pgmcrater.c \
	pgmedge.c \
	pgmenhance.c \
	pgmhist.c \
	pgmkernel.c \
	pgmnoise.c \
	pgmnorm.c \
	pgmoil.c \
	pgmramp.c \
	pgmtexture.c \
	pgmtofs.c \
	pgmtolispm.c \
	pgmtopbm.c \
	psidtopgm.c \
	rawtopgm.c \
	spottopgm.c \

exes = \
	$(outdir)\asciitopgm.exe \
	$(outdir)\bioradtopgm.exe \
	$(outdir)\fstopgm.exe \
	$(outdir)\hipstopgm.exe \
	$(outdir)\lispmtopgm.exe \
	$(outdir)\pbmtopgm.exe \
	$(outdir)\pgmbentley.exe \
	$(outdir)\pgmcrater.exe \
	$(outdir)\pgmedge.exe \
	$(outdir)\pgmenhance.exe \
	$(outdir)\pgmhist.exe \
	$(outdir)\pgmkernel.exe \
	$(outdir)\pgmnoise.exe \
	$(outdir)\pgmnorm.exe \
	$(outdir)\pgmoil.exe \
	$(outdir)\pgmramp.exe \
	$(outdir)\pgmtexture.exe \
	$(outdir)\pgmtofs.exe \
	$(outdir)\pgmtolispm.exe \
	$(outdir)\pgmtopbm.exe \
	$(outdir)\psidtopgm.exe \
	$(outdir)\rawtopgm.exe \
	$(outdir)\spottopgm.exe \

binaries = $(outdir)\$(pgm_dll_name).dll $(exes)

libraries =  $(outdir)\libpgm.lib

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		-I.. \
		-I..\pbm \
		$<

{$(outdir)\}.obj{$(outdir)\}.exe:
	$(link) $(lstandard) \
		-out:$*.exe \
		$(outdir)\libpgm.lib \
		$(pbm_lib) \
		$<
	mt -nologo -manifest $*.exe.manifest -outputresource:$*.exe;1

all: common-all $(binaries) $(libraries)

install: common-install install-binaries

check: common-check

$(exes): $(outdir)\libpgm.lib $(pbm_lib)

objects = \
	$(outdir)\libpgm1.obj \
	$(outdir)\libpgm2.obj \

$(outdir)\$(pgm_dll_name).dll: \
			$(outdir) \
			$(dllbases) \
			$(objects) \
			$(outdir)\libpgm.exp \
			$(pbm_lib) \

	$(link) $(dlllstandard) \
		-out:$@ -base:@$(dllbases),$(pgm_dll_name) \
		$(objects) \
		$(outdir)\libpgm.exp \
		$(pbm_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\libpgm.lib $(outdir)\libpgm.exp: \
			$(outdir) \
			libpgm.def \

	$(libtool) $(libflags) \
		-def:libpgm.def \
		-name:$(pgm_dll_name) \
		-out:$(outdir)\libpgm.lib \

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend:
	$(MAKEDEPEND) -I.. $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\asciitopgm.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\asciitopgm.obj: ../pbmplus.h
$(outdir)\bioradtopgm.obj: pgm.h
$(outdir)\bioradtopgm.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\bioradtopgm.obj: ../pbmplus.h
$(outdir)\fstopgm.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\fstopgm.obj: ../pbmplus.h
$(outdir)\hipstopgm.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\hipstopgm.obj: ../pbmplus.h
$(outdir)\libpgm1.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\libpgm1.obj: ../pbmplus.h libpgm.h
$(outdir)\libpgm1.obj: $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/libpbm.h
$(outdir)\libpgm2.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\libpgm2.obj: ../pbmplus.h libpgm.h
$(outdir)\lispmtopgm.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\lispmtopgm.obj: ../pbmplus.h
$(outdir)\pbmtopgm.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pbmtopgm.obj: ../pbmplus.h
$(outdir)\pgmbentley.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pgmbentley.obj: ../pbmplus.h
$(outdir)\pgmcrater.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pgmcrater.obj: ../pbmplus.h
$(outdir)\pgmedge.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pgmedge.obj: ../pbmplus.h
$(outdir)\pgmenhance.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pgmenhance.obj: ../pbmplus.h
$(outdir)\pgmhist.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pgmhist.obj: ../pbmplus.h
$(outdir)\pgmkernel.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pgmkernel.obj: ../pbmplus.h
$(outdir)\pgmnoise.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pgmnoise.obj: ../pbmplus.h
$(outdir)\pgmnorm.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pgmnorm.obj: ../pbmplus.h
$(outdir)\pgmoil.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pgmoil.obj: ../pbmplus.h
$(outdir)\pgmramp.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pgmramp.obj: ../pbmplus.h
$(outdir)\pgmtexture.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pgmtexture.obj: ../pbmplus.h
$(outdir)\pgmtofs.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pgmtofs.obj: ../pbmplus.h
$(outdir)\pgmtolispm.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pgmtolispm.obj: ../pbmplus.h
$(outdir)\pgmtopbm.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\pgmtopbm.obj: ../pbmplus.h dithers.h
$(outdir)\psidtopgm.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\psidtopgm.obj: ../pbmplus.h
$(outdir)\rawtopgm.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\rawtopgm.obj: ../pbmplus.h
$(outdir)\spottopgm.obj: pgm.h $(miktexsrcdir)/Libraries\3rd\netpbm/pbm/pbm.h
$(outdir)\spottopgm.obj: ../pbmplus.h
