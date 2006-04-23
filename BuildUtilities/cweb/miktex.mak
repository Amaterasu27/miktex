## miktex.mak: cweb
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

binaries = $(outdir)\ctangle.exe $(outdir)\cweave.exe

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# -----------------------------------------------------------------------------
# ctangle.exe
# -----------------------------------------------------------------------------

$(outdir)\ctangle.exe: \
			.bootstrap \
			$(outdir) \
			$(outdir)\common.obj \
			$(outdir)\ctangle.obj \
			$(outdir)\ctangle.res \

	$(link) $(lstandard) \
		-out:$@ \
		$(outdir)\common.obj \
		$(outdir)\ctangle.obj \
		$(outdir)\ctangle.res
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\ctangle.res: \
			$(outdir) \
			ctangle.rc \
			ctangle-version.h \
			$(libdir)\include\miktex\miktex.version \
			$(libdir)\include\miktex\version.h \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ ctangle.rc

ctangle.c: ctangle.w common.w
	$(ctangle) ctangle.w

common.c: common.w common-miktex.ch
	$(outdir)\ctangle.exe common.w common-miktex.ch

# -----------------------------------------------------------------------------
# cweave.exe
# -----------------------------------------------------------------------------

cweave_objects = \
	$(outdir)\common.obj \
	$(outdir)\cweave.obj \
	$(outdir)\cweave.res \

$(outdir)\cweave.exe: \
			.bootstrap \
			$(outdir) \
			$(cweave_objects) \

	$(link) $(lstandard) \
		-out:$@ \
		$(cweave_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\cweave.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			cweave-version.h \
			cweave.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ cweave.rc

cweave.c: cweave.w common.w prod.w
	$(outdir)\ctangle.exe cweave.w

# -----------------------------------------------------------------------------
# bootstrapping
# -----------------------------------------------------------------------------

.bootstrap: initctangle.c initcommon.c
	@-del /f ctangle.c
	@-del /f common.c
	type initctangle.c > ctangle.c
	type initcommon.c > common.c
	echo x > .bootstrap

# -----------------------------------------------------------------------------
# clean-up
# -----------------------------------------------------------------------------

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean
	-del /f /q .bootstrap
	-del /f /q cweave.c
	-del /f /q common.c

distclean: common-distclean clean
	-del /f /q ctangle.c

maintainer-clean: common-maintainer-clean distclean

# -----------------------------------------------------------------------------
# dependencies
# -----------------------------------------------------------------------------

!include <common-dependencies.inc>

depend: ctangle.c common.c cweave.c
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE
