## miktex.mak: libmspack-static
## 
## Copyright (C) 2001-2006 Christian Schenk
## 
## This file is part of MiKTeX Package Manager (miktex.dll).
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

srcdir = $(mspacklibdir)

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
	$(mspacklibdir)\cabc.c \
	$(mspacklibdir)\cabd.c \
	$(mspacklibdir)\chmc.c \
	$(mspacklibdir)\chmd.c \
#	$(mspacklibdir)\debug.c \
	$(mspacklibdir)\hlpc.c \
	$(mspacklibdir)\hlpd.c \
	$(mspacklibdir)\kwajc.c \
	$(mspacklibdir)\kwajd.c \
	$(mspacklibdir)\litc.c \
	$(mspacklibdir)\litd.c \
	$(mspacklibdir)\lzxc.c \
	$(mspacklibdir)\lzxd.c \
	$(mspacklibdir)\mszipc.c \
	$(mspacklibdir)\mszipd.c \
	$(mspacklibdir)\qtmc.c \
	$(mspacklibdir)\qtmd.c \
	$(mspacklibdir)\system.c \
	$(mspacklibdir)\szddc.c \
	$(mspacklibdir)\szddd.c \

{$(mspacklibdir)\}.c{$(outdir)\}.obj:
	 $(cc) $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(mspack_static_lib)

install: common-install all

check: common-check

# _____________________________________________________________________________
#
# mspack.lib
# _____________________________________________________________________________

$(mspack_static_lib): \
			$(outdir) \
			$(objects) \

	$(libtool) $(libflags) $(libopt_output_file)$@ $(objects)

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean

distclean: common-distclean

maintainer-clean: common-maintainer-clean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: $(sources)
	$(MAKEDEPEND) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\cabc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\cabc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\cabc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/cab.h
$(outdir)\cabc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mszip.h
$(outdir)\cabc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/qtm.h
$(outdir)\cabc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lzx.h
$(outdir)\cabd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\cabd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\cabd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/cab.h
$(outdir)\cabd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mszip.h
$(outdir)\cabd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/qtm.h
$(outdir)\cabd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lzx.h
$(outdir)\chmc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\chmc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\chmc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/chm.h
$(outdir)\chmc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lzx.h
$(outdir)\chmd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\chmd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\chmd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/chm.h
$(outdir)\chmd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lzx.h
$(outdir)\hlpc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\hlpc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\hlpc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/hlp.h
$(outdir)\hlpc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lzss.h
$(outdir)\hlpd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\hlpd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\hlpd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/hlp.h
$(outdir)\hlpd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lzss.h
$(outdir)\kwajc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\kwajc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\kwajc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/kwaj.h
$(outdir)\kwajc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lzss.h
$(outdir)\kwajc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mszip.h
$(outdir)\kwajd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\kwajd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\kwajd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/kwaj.h
$(outdir)\kwajd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lzss.h
$(outdir)\kwajd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mszip.h
$(outdir)\litc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\litc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\litc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lit.h
$(outdir)\litc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lzx.h
$(outdir)\litc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/des.h
$(outdir)\litc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/sha.h
$(outdir)\litd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\litd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\litd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lit.h
$(outdir)\litd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lzx.h
$(outdir)\litd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/des.h
$(outdir)\litd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/sha.h
$(outdir)\lzxc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\lzxc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\lzxc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lzx.h
$(outdir)\lzxd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\lzxd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\lzxd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lzx.h
$(outdir)\mszipc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\mszipc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\mszipc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mszip.h
$(outdir)\mszipd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\mszipd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\mszipd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mszip.h
$(outdir)\qtmc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\qtmc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\qtmc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/qtm.h
$(outdir)\qtmd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\qtmd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\qtmd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/qtm.h
$(outdir)\system.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\szddc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\szddc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\szddc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/szdd.h
$(outdir)\szddc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lzss.h
$(outdir)\szddd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\szddd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\szddd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/szdd.h
$(outdir)\szddd.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/lzss.h
