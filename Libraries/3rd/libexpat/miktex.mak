## miktex.mak: libexpat
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

!include <miktex.inc>

expat_dll = $(expat_dll_name).dll

binaries = $(outdir)\$(expat_dll)

objects = \
	$(outdir)\xmlparse.obj \
	$(outdir)\xmlrole.obj \
	$(outdir)\xmltok.obj \

sources = \
	xmlparse.c \
	xmlrole.c \
	xmltok.c \
	xmltok_impl.c \
	xmltok_ns.c \

extradefs = \
	-DCOMPILED_FROM_DSP \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) $(extradefs) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(expat_lib)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# expat.dll
# _____________________________________________________________________________

$(outdir)\$(expat_dll): \
			$(outdir) \
			$(dllbases) \
			$(objects) \
			$(outdir)\expat.exp \
			$(outdir)\expat.res \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(expat_dll_name) \
		-implib:$(expat_lib) \
		-out:$(outdir)\$(expat_dll) \
		$(objects) \
		$(outdir)\expat.exp \
		$(outdir)\expat.res
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(expat_lib) $(outdir)\expat.exp: $(outdir) expat.def
	$(libtool) $(libflags) \
		-def:expat.def \
		-name:$(expat_dll_name) \
		-out:$(expat_lib) \

$(outdir)\expat.res: \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			expat-version.h \
			expat.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ expat.rc

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

$(outdir)\xmlparse.obj: expat.h expat_external.h internal.h xmltok.h xmlrole.h
$(outdir)\xmlrole.obj: expat_external.h internal.h xmlrole.h xmltok.h ascii.h
$(outdir)\xmltok.obj: expat_external.h internal.h xmltok.h nametab.h
$(outdir)\xmltok.obj: xmltok_impl.h ascii.h xmltok_impl.c asciitab.h utf8tab.h
$(outdir)\xmltok.obj: iasciitab.h latin1tab.h xmltok_ns.c
