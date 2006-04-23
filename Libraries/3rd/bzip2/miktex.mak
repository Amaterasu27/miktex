## miktex.mak: bzip2
##
## Copyright (C) 2000-2006 Christian Schenk
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

libobjects = \
	$(outdir)\blocksort.obj \
	$(outdir)\bzlib.obj \
	$(outdir)\compress.obj \
	$(outdir)\crctable.obj \
	$(outdir)\decompress.obj \
	$(outdir)\huffman.obj \
	$(outdir)\randtable.obj \

sources = \
	blocksort.c \
	bzlib.c \
	compress.c \
	crctable.c \
	decompress.c \
	huffman.c \
	randtable.c \

libbz2_dll = $(bzip2_dll_name).dll

binaries = $(outdir)\$(libbz2_dll)

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(bz2_lib)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# libbz2.dll
# _____________________________________________________________________________

$(outdir)\$(libbz2_dll) $(bz2_lib): \
			$(outdir) \
			$(dllbases) \
			$(libobjects) \
			$(outdir)\libbz2.res \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(bzip2_dll_name) \
		-def:libbz2.def \
		-implib:$(bz2_lib) \
		-out:$(outdir)\$(libbz2_dll) \
		$(libobjects) $(outdir)\libbz2.res
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\libbz2.res: $(outdir) libbz2.rc $(libdir)\include\miktex\miktex.version
	$(rc) $(rcstandard) $(rcopt_output_file)$@ libbz2.rc

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

$(outdir)\blocksort.obj: bzlib_private.h bzlib.h
$(outdir)\bzlib.obj: bzlib_private.h bzlib.h
$(outdir)\compress.obj: bzlib_private.h bzlib.h
$(outdir)\crctable.obj: bzlib_private.h bzlib.h
$(outdir)\decompress.obj: bzlib_private.h bzlib.h
$(outdir)\huffman.obj: bzlib_private.h bzlib.h
$(outdir)\randtable.obj: bzlib_private.h bzlib.h
