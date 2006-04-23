## miktex.mak:
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

objects = \
	$(outdir)\FoFiBase.obj \
	$(outdir)\FoFiEncodings.obj \
	$(outdir)\FoFiTrueType.obj \
	$(outdir)\FoFiType1.obj \
	$(outdir)\FoFiType1C.obj \

sources = \
	FoFiBase.cc \
	FoFiEncodings.cc \
	FoFiTrueType.cc \
	FoFiType1.cc \
	FoFiType1C.cc \

defines = \
	-DWIN32 \

includes = \
	-I.. \
	-I../goo \

common_options = \
	$(ccopt_enable_function_level_linking) \
	$(ccopt_select_fastcall) \
	$(defines) \
	$(includes) \

.SUFFIXES: .cc

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(common_options) \
		$(ccopt_output_directory)$(outdir)\ \
		$<

.cc{$(outdir)\}.obj:
	$(cc) $(cppstandard) \
		$(common_options) \
		$(ccopt_output_directory)$(outdir)\ \
		$<

libraries = $(outdir)\fofi.lib

all: common-all $(libraries)

install: common-install $(libraries)

check: common-check

$(outdir)\fofi.lib: $(outdir) $(objects)
	$(libtool) $(libflags) -out:$@ $(objects)

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-clean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend: $(sources)
	$(MAKEDEPEND) $(defines) $(includes) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\FoFiBase.obj: ../aconf.h ../goo/gmem.h FoFiBase.h ../goo/gtypes.h
$(outdir)\FoFiEncodings.obj: ../aconf.h FoFiEncodings.h ../goo/gtypes.h
$(outdir)\FoFiTrueType.obj: ../aconf.h ../goo/gtypes.h ../goo/gmem.h
$(outdir)\FoFiTrueType.obj: ../goo/GString.h ../goo/GHash.h FoFiTrueType.h
$(outdir)\FoFiTrueType.obj: FoFiBase.h
$(outdir)\FoFiType1.obj: ../aconf.h ../goo/gmem.h FoFiEncodings.h ../goo/gtypes.h
$(outdir)\FoFiType1.obj: FoFiType1.h FoFiBase.h
$(outdir)\FoFiType1C.obj: ../aconf.h ../goo/gmem.h ../goo/GString.h
$(outdir)\FoFiType1C.obj: FoFiEncodings.h ../goo/gtypes.h FoFiType1C.h FoFiBase.h
