## miktex.mak: cjkutils/bg5conv
## 
## Copyright (C) 2004-2005 Christian Schenk
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

binaries = $(outdir)\bg5conv.exe

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# bg5conv
# _____________________________________________________________________________

bg5conv_objects = \
	$(outdir)\bg5conv.obj \
	$(outdir)\bg5conv.res \

$(outdir)\bg5conv.exe: \
			$(outdir) \
			$(bg5conv_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(bg5conv_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\bg5conv.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		bg5conv-version.h \
		bg5conv.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ bg5conv.rc

bg5conv.c: bg5conv.w
	$(ctangle) bg5conv.w

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean
	-del /f bg5conv.c

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: bg5conv.c
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE
