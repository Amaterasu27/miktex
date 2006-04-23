## miktex.mak: cjkutils/sjisconv
## 
## Copyright (C) 2004 Christian Schenk
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

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

binaries = $(outdir)\sjisconv.exe

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# sjisconv
# _____________________________________________________________________________

sjisconv_objects = \
	$(outdir)\sjisconv.obj \
	$(outdir)\sjisconv.res \

$(outdir)\sjisconv.exe: \
			$(outdir) \
			$(sjisconv_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(sjisconv_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\sjisconv.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			sjisconv-version.h \
			sjisconv.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ sjisconv.rc

sjisconv.c: sjisconv.w
	$(ctangle) sjisconv.w

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean
	-del /f sjisconv.c

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: sjisconv.c
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE
