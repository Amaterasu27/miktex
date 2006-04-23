## miktex.mak: ps2pk/type1
## 
## Copyright (C) 2000-2004 Christian Schenk
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
	$(outdir)\arith.obj \
	$(outdir)\curves.obj \
	$(outdir)\fontfcn.obj \
	$(outdir)\hints.obj \
	$(outdir)\lines.obj \
	$(outdir)\objects.obj \
	$(outdir)\paths.obj \
	$(outdir)\regions.obj \
	$(outdir)\scanfont.obj \
	$(outdir)\spaces.obj \
	$(outdir)\t1funcs.obj \
	$(outdir)\t1info.obj \
	$(outdir)\t1io.obj \
	$(outdir)\t1snap.obj \
	$(outdir)\t1stub.obj \
	$(outdir)\token.obj \
	$(outdir)\type1.obj \
	$(outdir)\util.obj \

sources = \
	arith.c \
	curves.c \
	fontfcn.c \
	hints.c \
	lines.c \
	objects.c \
	paths.c \
	regions.c \
	scanfont.c \
	spaces.c \
	t1funcs.c \
	t1info.c \
	t1io.c \
	t1snap.c \
	t1stub.c \
	token.c \
	type1.c \
	util.c \

libraries = $(outdir)\type1.lib

all: common-all $(libraries)

install: common-install $(libraries)

check: common-check

defines = \
	-DHAVE_PROTOTYPES \
	-DKPATHSEA \
	-DUSE_MIKTEX_EXIT \

includes = \
	$(ccinc_libkps) \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		$(defines) \
		$(includes) \
		$<

$(outdir)\type1.lib: $(outdir) $(objects)
	$(libtool) $(libflags) $(libopt_output_file)$@ $(objects)

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend:
	$(MAKEDEPEND) $(defines) $(includes) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\arith.obj: types.h objects.h spaces.h arith.h
$(outdir)\curves.obj: types.h objects.h spaces.h paths.h regions.h curves.h
$(outdir)\curves.obj: lines.h arith.h
$(outdir)\fontfcn.obj: t1imager.h types.h fontmisc.h util.h fontfcn.h
$(outdir)\hints.obj: types.h objects.h spaces.h paths.h regions.h hints.h
$(outdir)\lines.obj: types.h objects.h spaces.h regions.h lines.h
$(outdir)\objects.obj: types.h objects.h spaces.h paths.h regions.h fonts.h
$(outdir)\objects.obj: pictures.h strokes.h cluts.h
$(outdir)\paths.obj: types.h objects.h spaces.h paths.h regions.h fonts.h
$(outdir)\paths.obj: pictures.h strokes.h trig.h
$(outdir)\regions.obj: types.h objects.h spaces.h regions.h paths.h curves.h
$(outdir)\regions.obj: lines.h pictures.h fonts.h hints.h strokes.h
$(outdir)\scanfont.obj: t1stdio.h types.h util.h token.h fontfcn.h blues.h
$(outdir)\spaces.obj: types.h objects.h spaces.h paths.h pictures.h fonts.h
$(outdir)\spaces.obj: arith.h trig.h
$(outdir)\t1funcs.obj: types.h ffilest.h Xstuff.h fontmisc.h fontstruct.h font.h
$(outdir)\t1funcs.obj: fsmasks.h fontfile.h fontxlfd.h t1intf.h objects.h
$(outdir)\t1funcs.obj: spaces.h regions.h t1stdio.h util.h fontfcn.h
$(outdir)\t1info.obj: types.h ffilest.h Xstuff.h fontmisc.h fontstruct.h font.h
$(outdir)\t1info.obj: fsmasks.h fontfile.h fontxlfd.h t1intf.h
$(outdir)\t1io.obj: t1stdio.h types.h t1hdigit.h
$(outdir)\t1snap.obj: objects.h types.h spaces.h paths.h
$(outdir)\t1stub.obj: objects.h types.h
$(outdir)\token.obj: types.h t1stdio.h util.h digit.h token.h tokst.h hdigit.h
$(outdir)\type1.obj: types.h objects.h spaces.h paths.h fonts.h pictures.h util.h
$(outdir)\type1.obj: blues.h
$(outdir)\util.obj: types.h util.h fontmisc.h
