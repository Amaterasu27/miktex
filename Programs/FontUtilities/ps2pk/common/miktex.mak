## miktex.mak: ps2pk/common
## 
## Copyright (C) 2000-2005 Christian Schenk
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
	$(outdir)\basics.obj \
	$(outdir)\filenames.obj \
#	$(outdir)\psearch.obj \
	$(outdir)\strexpr.obj \
	$(outdir)\texfiles.obj \

sources = \
	basics.c \
	filenames.c \
#	psearch.c \
	strexpr.c \
	texfiles.c \

libraries = $(outdir)\common.lib

all: common-all $(libraries)

install: common-install $(libraries)

check: common-check

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		-DUSE_MIKTEX_EXIT \
		$(ccopt_output_directory)$(outdir)\ \
		$<

$(outdir)\common.lib: $(outdir) $(objects)
	$(libtool) $(libflags) $(libopt_output_file)$@ $(objects)

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend:
	$(MAKEDEPEND) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\filenames.obj: basics.h
$(outdir)\strexpr.obj: basics.h
$(outdir)\texfiles.obj: texfiles.h
