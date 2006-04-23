## miktex.mak: libexpat-static
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

miktex_cc_use_crt_dll = 0
no_miktex_dlls = 1

!include <miktex.inc>

srcdir = $(expatlibdir)

objects = \
	$(outdir)\xmlparse.obj \
	$(outdir)\xmlrole.obj \
	$(outdir)\xmltok.obj \

sources = \
	$(expatlibdir)\xmlparse.c \
	$(expatlibdir)\xmlrole.c \
	$(expatlibdir)\xmltok.c \
	$(expatlibdir)\xmltok_impl.c \
	$(expatlibdir)\xmltok_ns.c \

extradefs = \
	-DCOMPILED_FROM_DSP \

{$(expatlibdir)\}.c{$(outdir)\}.obj:
	$(cc) $(cstandard) $(extradefs) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(expat_static_lib)

install: common-install all

check: common-check

# _____________________________________________________________________________
#
# expat-static.lib
# _____________________________________________________________________________

$(expat_static_lib): $(outdir) $(objects)
	$(libtool) $(libflags) $(libopt_output_file)$@ $(objects)

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
	$(MAKEDEPEND) $(extradefs) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\xmlparse.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/winconfig.h
$(outdir)\xmlparse.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat.h
$(outdir)\xmlparse.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\xmlparse.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/internal.h
$(outdir)\xmlparse.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/xmltok.h
$(outdir)\xmlparse.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/xmlrole.h
$(outdir)\xmlrole.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/winconfig.h
$(outdir)\xmlrole.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\xmlrole.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/internal.h
$(outdir)\xmlrole.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/xmlrole.h
$(outdir)\xmlrole.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/xmltok.h
$(outdir)\xmlrole.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/ascii.h
$(outdir)\xmltok.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/winconfig.h
$(outdir)\xmltok.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/expat_external.h
$(outdir)\xmltok.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/internal.h
$(outdir)\xmltok.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/xmltok.h
$(outdir)\xmltok.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/nametab.h
$(outdir)\xmltok.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/xmltok_impl.h
$(outdir)\xmltok.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/ascii.h
$(outdir)\xmltok.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/xmltok_impl.c
$(outdir)\xmltok.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/asciitab.h
$(outdir)\xmltok.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/utf8tab.h
$(outdir)\xmltok.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/iasciitab.h
$(outdir)\xmltok.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/latin1tab.h
$(outdir)\xmltok.obj: $(miktexsrcdir)/Libraries/3rd/libexpat/xmltok_ns.c
