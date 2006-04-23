## miktex.mak: cjkutils/bg5conv
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

binaries = \
	$(outdir)\cef5conv.exe \
	$(outdir)\cefconv.exe \
	$(outdir)\cefsconv.exe \

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# cef5conv
# _____________________________________________________________________________

cef5conv_objects = \
	$(outdir)\cef5conv.obj \
	$(outdir)\cef5conv.res \

$(outdir)\cef5conv.exe: \
			$(outdir) \
			$(cef5conv_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(cef5conv_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\cef5conv.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		cef5conv-version.h \
		cef5conv.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ cef5conv.rc

cef5conv.c: cef5conv.w
	$(ctangle) cef5conv.w

# _____________________________________________________________________________
#
# cefconv
# _____________________________________________________________________________

cefconv_objects = \
	$(outdir)\cefconv.obj \
	$(outdir)\cefconv.res \

$(outdir)\cefconv.exe: \
			$(outdir) \
			$(cefconv_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(cefconv_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\cefconv.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			cefconv-version.h \
			cefconv.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ cefconv.rc

cefconv.c: cefconv.w
	$(ctangle) cefconv.w

# _____________________________________________________________________________
#
# cefsconv
# _____________________________________________________________________________

cefsconv_objects = \
	$(outdir)\cefsconv.obj \
	$(outdir)\cefsconv.res \

$(outdir)\cefsconv.exe: \
			$(outdir) \
			$(cefsconv_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(cefsconv_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\cefsconv.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			cefsconv-version.h \
			cefsconv.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ cefsconv.rc

cefsconv.c: cefsconv.w
	$(ctangle) cefsconv.w

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean
	-del /f cef5conv.c
	-del /f cefconv.c
	-del /f cefsconv.c

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: cef5conv.c cefconv.c cefsconv.c
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE
