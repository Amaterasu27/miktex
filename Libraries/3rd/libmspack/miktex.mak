## miktex.mak: libmspack
##
## Copyright (C) 2005 Christian Schenk
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
	$(outdir)\cabc.obj \
	$(outdir)\cabd.obj \
	$(outdir)\chmc.obj \
	$(outdir)\chmd.obj \
#	$(outdir)\debug.obj \
	$(outdir)\hlpc.obj \
	$(outdir)\hlpd.obj \
	$(outdir)\kwajc.obj \
	$(outdir)\kwajd.obj \
	$(outdir)\litc.obj \
	$(outdir)\litd.obj \
	$(outdir)\lzxc.obj \
	$(outdir)\lzxd.obj \
	$(outdir)\mszipc.obj \
	$(outdir)\mszipd.obj \
	$(outdir)\qtmc.obj \
	$(outdir)\qtmd.obj \
	$(outdir)\system.obj \
	$(outdir)\szddc.obj \
	$(outdir)\szddd.obj \

sources = \
	cabc.c \
	cabd.c \
	chmc.c \
	chmd.c \
#	debug.c \
	hlpc.c \
	hlpd.c \
	kwajc.c \
	kwajd.c \
	litc.c \
	litd.c \
	lzxc.c \
	lzxd.c \
	mszipc.c \
	mszipd.c \
	qtmc.c \
	qtmd.c \
	system.c \
	szddc.c \
	szddd.c \

binaries = $(outdir)\$(mspack_dll_name).dll

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(ccopt_output_directory)$(outdir)\ $<

all: common-all

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# mspack.dll
# _____________________________________________________________________________

$(outdir)\$(mspack_dll_name).dll: \
			$(outdir) \
			$(dllbases) \
			$(objects) \
			$(outdir)\mspack.exp \
			$(outdir)\mspack.res \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(mspack_dll_name) \
		-out:$(outdir)\$(mspack_dll_name).dll \
		$(objects) \
		$(outdir)\mspack.exp \
		$(outdir)\mspack.res
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(mspack_lib) $(outdir)\mspack.exp: \
			$(outdir) \
			mspack.def \

	$(libtool) $(libflags) \
		-def:mspack.def \
		-name:$(mspack_dll_name) \
		-out:$(mspack_lib) \

$(outdir)\mspack.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		mspack-version.h \
		mspack.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ mspack.rc

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
	$(MAKEDEPEND) $(extra_defines) $(extra_includes) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\cabc.obj: ./system.h ./mspack.h ./cab.h ./mszip.h ./qtm.h ./lzx.h
$(outdir)\cabd.obj: ./system.h ./mspack.h ./cab.h ./mszip.h ./qtm.h ./lzx.h
$(outdir)\chmc.obj: ./system.h ./mspack.h ./chm.h ./lzx.h
$(outdir)\chmd.obj: ./system.h ./mspack.h ./chm.h ./lzx.h
$(outdir)\hlpc.obj: ./system.h ./mspack.h ./hlp.h ./lzss.h
$(outdir)\hlpd.obj: ./system.h ./mspack.h ./hlp.h ./lzss.h
$(outdir)\kwajc.obj: ./system.h ./mspack.h ./kwaj.h ./lzss.h ./mszip.h
$(outdir)\kwajd.obj: ./system.h ./mspack.h ./kwaj.h ./lzss.h ./mszip.h
$(outdir)\litc.obj: ./system.h ./mspack.h ./lit.h ./lzx.h ./des.h ./sha.h
$(outdir)\litd.obj: ./system.h ./mspack.h ./lit.h ./lzx.h ./des.h ./sha.h
$(outdir)\lzxc.obj: ./system.h ./mspack.h ./lzx.h
$(outdir)\lzxd.obj: ./system.h ./mspack.h ./lzx.h
$(outdir)\mszipc.obj: ./system.h ./mspack.h ./mszip.h
$(outdir)\mszipd.obj: ./system.h ./mspack.h ./mszip.h
$(outdir)\qtmc.obj: ./system.h ./mspack.h ./qtm.h
$(outdir)\qtmd.obj: ./system.h ./mspack.h ./qtm.h
$(outdir)\system.obj: ./mspack.h
$(outdir)\szddc.obj: ./system.h ./mspack.h ./szdd.h ./lzss.h
$(outdir)\szddd.obj: ./system.h ./mspack.h ./szdd.h ./lzss.h
