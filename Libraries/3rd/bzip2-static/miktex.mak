## miktex.mak: bzip2-static
##
## Copyright (C) 2005-2006 Christian Schenk
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

srcdir = $(bzip2dir)

objects = \
	$(outdir)\blocksort.obj \
	$(outdir)\bzlib.obj \
	$(outdir)\compress.obj \
	$(outdir)\crctable.obj \
	$(outdir)\decompress.obj \
	$(outdir)\huffman.obj \
	$(outdir)\randtable.obj \

sources = \
	$(bzip2dir)\blocksort.c \
	$(bzip2dir)\bzlib.c \
	$(bzip2dir)\compress.c \
	$(bzip2dir)\crctable.c \
	$(bzip2dir)\decompress.c \
	$(bzip2dir)\huffman.c \
	$(bzip2dir)\randtable.c \

{$(bzip2dir)\}.c{$(outdir)\}.obj:
	$(cc) $(cstandard) $(extradefs) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(bz2_static_lib)

install: common-install all

check: common-check

# _____________________________________________________________________________
#
# bz2-static.lib
# _____________________________________________________________________________

$(bz2_static_lib): $(outdir) $(objects)
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
	$(MAKEDEPEND) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\blocksort.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib_private.h
$(outdir)\blocksort.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\bzlib.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib_private.h
$(outdir)\bzlib.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\compress.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib_private.h
$(outdir)\compress.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\crctable.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib_private.h
$(outdir)\crctable.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\decompress.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib_private.h
$(outdir)\decompress.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\huffman.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib_private.h
$(outdir)\huffman.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\randtable.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib_private.h
$(outdir)\randtable.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
