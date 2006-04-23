## miktex.mak: zlib
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

miktex_cc_no_runtime_checks = 1

!include <miktex.inc>

objects = \
	$(outdir)\adler32.obj \
	$(outdir)\compress.obj \
	$(outdir)\crc32.obj \
	$(outdir)\deflate.obj \
	$(outdir)\gzio.obj \
	$(outdir)\infback.obj \
	$(outdir)\inffast.obj \
	$(outdir)\inflate.obj \
	$(outdir)\inftrees.obj \
	$(outdir)\trees.obj \
	$(outdir)\uncompr.obj \
	$(outdir)\zutil.obj \

sources = \
	adler32.c \
	compress.c \
	crc32.c \
	deflate.c \
	gzio.c \
	infback.c \
	inffast.c \
	inflate.c \
	inftrees.c \
	trees.c \
	uncompr.c \
	zutil.c \

binaries = $(outdir)\$(zlib_dll_name).dll

all: common-all $(zlib_lib)

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		-DZLIB_DLL \
		$(ccopt_output_directory)$(outdir)\ $<

$(outdir)\$(zlib_dll_name).dll: \
			$(outdir) \
			$(dllbases) \
			$(objects) \
			$(outdir)\zlib.exp \
			$(outdir)\zlib.res \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(zlib_dll_name) \
		-out:$@ \
		$(objects) $(outdir)\zlib.res $(outdir)\zlib.exp
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(zlib_lib) $(outdir)\zlib.exp: $(outdir) zlib.def
	$(libtool) $(libflags) \
		-def:zlib.def \
		-name:$(zlib_dll_name) \
		-out:$(zlib_lib)

$(outdir)\zlib.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			zlib-version.h \
			zlib.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ zlib.rc

install: common-install install-binaries

check: common-check

!include <common-dependencies.inc>

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

depend:
	$(MAKEDEPEND) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\adler32.obj: zlib.h zconf.h
$(outdir)\compress.obj: zlib.h zconf.h
$(outdir)\crc32.obj: zutil.h zlib.h zconf.h crc32.h
$(outdir)\deflate.obj: deflate.h zutil.h zlib.h zconf.h
$(outdir)\gzio.obj: zutil.h zlib.h zconf.h
$(outdir)\infback.obj: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h
$(outdir)\infback.obj: inffixed.h
$(outdir)\inffast.obj: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h
$(outdir)\inflate.obj: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h
$(outdir)\inflate.obj: inffixed.h
$(outdir)\inftrees.obj: zutil.h zlib.h zconf.h inftrees.h
$(outdir)\trees.obj: deflate.h zutil.h zlib.h zconf.h trees.h
$(outdir)\uncompr.obj: zlib.h zconf.h
$(outdir)\zutil.obj: zutil.h zlib.h zconf.h
