## miktex.mak: libpopt-static
## 
## Copyright (C) 2001-2006 Christian Schenk
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

no_miktex_dlls = 1
miktex_cc_use_crt_dll = 0

!include <miktex.inc>

srcdir = $(poptlibdir)

sources = \
	$(poptlibdir)\findme.c \
	$(poptlibdir)\popt.c \
	$(poptlibdir)\poptconfig.c \
	$(poptlibdir)\popthelp.c \
	$(poptlibdir)\poptparse.c \

cc_options = $(cflags) $(cdebug) $(cvarsdll)

{$(poptlibdir)\}.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		$<

all: common-all $(popt_static_lib)

install: common-install all

check: common-check

# _____________________________________________________________________________
#
# popt-static.lib
# _____________________________________________________________________________

popt_objects = \
	$(outdir)\findme.obj \
	$(outdir)\popt.obj \
	$(outdir)\poptconfig.obj \
	$(outdir)\popthelp.obj \
	$(outdir)\poptparse.obj \

$(popt_static_lib): $(outdir) $(popt_objects)
	$(libtool) $(libflags) $(libopt_output_file)$@ $(popt_objects)

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

$(outdir)\findme.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/system.h
$(outdir)\findme.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/win/config.h
$(outdir)\findme.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\findme.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/findme.h
$(outdir)\popt.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/system.h
$(outdir)\popt.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/win/config.h
$(outdir)\popt.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\popt.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/findme.h
$(outdir)\popt.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/poptint.h
$(outdir)\poptconfig.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/system.h
$(outdir)\poptconfig.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/win/config.h
$(outdir)\poptconfig.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\poptconfig.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/poptint.h
$(outdir)\popthelp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/system.h
$(outdir)\popthelp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/win/config.h
$(outdir)\popthelp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\popthelp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/poptint.h
$(outdir)\poptparse.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/system.h
$(outdir)\poptparse.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/win/config.h
$(outdir)\poptparse.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
