## miktex.mak: tie
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

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

binaries = $(outdir)\tie.exe

all: common-all $(binaries)

$(outdir)\tie.exe: \
			$(outdir) \
			$(outdir)\tie.obj \

	$(link) $(lstandard) \
		-out:$@ \
		$(outdir)\tie.obj
	mt -nologo -manifest $@.manifest -outputresource:$@;1

tie.c: tie.w tie-miktex.ch
	$(ctangle) tie tie-miktex.ch

install: common-install install-binaries

check: common-check

mostlyclean: common-mostlyclean
	-del /f *.h
	-del /f *.p

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean
	-del /f tie.c

!include <common-dependencies.inc>

depend: tie.c
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE
