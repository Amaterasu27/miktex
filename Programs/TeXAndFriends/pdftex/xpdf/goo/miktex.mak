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
	$(outdir)\FixedPoint.obj \
	$(outdir)\GHash.obj \
	$(outdir)\GList.obj \
	$(outdir)\GString.obj \
	$(outdir)\gfile.obj \
	$(outdir)\gmem.obj \
	$(outdir)\gmempp.obj \
	$(outdir)\parseargs.obj \

sources = \
	FixedPoint.cc \
	GHash.cc \
	GList.cc \
	GString.cc \
	gfile.cc \
	gmem.c \
	gmempp.cc \
	parseargs.c \

defines = \
	-DWIN32 \

includes = \
	-I.. \

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

libraries = $(outdir)\goo.lib

all: common-all $(libraries)

install: common-install $(libraries)

check: common-check

$(outdir)\goo.lib: $(outdir) $(objects)
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

$(outdir)\FixedPoint.obj: ../aconf.h
$(outdir)\GHash.obj: ../aconf.h gmem.h GString.h GHash.h gtypes.h
$(outdir)\GList.obj: ../aconf.h gmem.h GList.h gtypes.h
$(outdir)\GString.obj: ../aconf.h gtypes.h GString.h
$(outdir)\gfile.obj: ../aconf.h gfile.h gtypes.h
$(outdir)\gmem.obj: ../aconf.h gmem.h
$(outdir)\gmempp.obj: ../aconf.h gmem.h
$(outdir)\parseargs.obj: parseargs.h gtypes.h
