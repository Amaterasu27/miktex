## miktex.mak: libpopt
## 
## Copyright (C) 2001-2005 Christian Schenk
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

popt_dll = $(popt_dll_name).dll

binaries = \
	$(outdir)\$(popt_dll) \

sources = \
	findme.c \
	popt.c \
	poptconfig.c \
	popthelp.c \
	poptparse.c \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) -Iwin $(ccopt_output_file)$@ $<

all: common-all $(popt_lib)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# popt.dll
# _____________________________________________________________________________

popt_objects = \
	$(outdir)\findme.obj \
	$(outdir)\popt.obj \
	$(outdir)\poptconfig.obj \
	$(outdir)\popthelp.obj \
	$(outdir)\poptparse.obj \

$(outdir)\$(popt_dll) $(popt_lib): \
			$(outdir) \
			$(popt_objects) \
			$(outdir)\popt.res \
			$(dllbases) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(popt_dll_name) \
		-implib:$(popt_lib) \
		-out:$(outdir)\$(popt_dll) \
		$(popt_objects) \
		$(outdir)\popt.res
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\popt.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			popt-version.h \
			popt.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ popt.rc

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
	$(MAKEDEPEND) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\findme.obj: system.h win/config.h popt.h findme.h
$(outdir)\popt.obj: system.h win/config.h popt.h findme.h poptint.h
$(outdir)\poptconfig.obj: system.h win/config.h popt.h poptint.h
$(outdir)\popthelp.obj: system.h win/config.h popt.h poptint.h
$(outdir)\poptparse.obj: system.h win/config.h popt.h
