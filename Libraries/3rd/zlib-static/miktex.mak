## miktex.mak: zlib-static
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

srcdir = $(zlibdir)

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
	$(zlibdir)\adler32.c \
	$(zlibdir)\compress.c \
	$(zlibdir)\crc32.c \
	$(zlibdir)\deflate.c \
	$(zlibdir)\gzio.c \
	$(zlibdir)\infback.c \
	$(zlibdir)\inffast.c \
	$(zlibdir)\inflate.c \
	$(zlibdir)\inftrees.c \
	$(zlibdir)\trees.c \
	$(zlibdir)\uncompr.c \
	$(zlibdir)\zutil.c \

{$(zlibdir)\}.c{$(outdir)\}.obj:
	$(cc) $(cstandard) $(extradefs) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(zlib_static_lib)

install: common-install all

check: common-check

# _____________________________________________________________________________
#
# zlib-static.lib
# _____________________________________________________________________________

$(zlib_static_lib): $(outdir) $(objects)
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

$(outdir)\adler32.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\adler32.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\compress.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\compress.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\crc32.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zutil.h
$(outdir)\crc32.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\crc32.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\crc32.obj: $(miktexsrcdir)/Libraries/3rd/zlib/crc32.h
$(outdir)\deflate.obj: $(miktexsrcdir)/Libraries/3rd/zlib/deflate.h
$(outdir)\deflate.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zutil.h
$(outdir)\deflate.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\deflate.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\gzio.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zutil.h
$(outdir)\gzio.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\gzio.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\infback.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zutil.h
$(outdir)\infback.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\infback.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\infback.obj: $(miktexsrcdir)/Libraries/3rd/zlib/inftrees.h
$(outdir)\infback.obj: $(miktexsrcdir)/Libraries/3rd/zlib/inflate.h
$(outdir)\infback.obj: $(miktexsrcdir)/Libraries/3rd/zlib/inffast.h
$(outdir)\infback.obj: $(miktexsrcdir)/Libraries/3rd/zlib/inffixed.h
$(outdir)\inffast.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zutil.h
$(outdir)\inffast.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\inffast.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\inffast.obj: $(miktexsrcdir)/Libraries/3rd/zlib/inftrees.h
$(outdir)\inffast.obj: $(miktexsrcdir)/Libraries/3rd/zlib/inflate.h
$(outdir)\inffast.obj: $(miktexsrcdir)/Libraries/3rd/zlib/inffast.h
$(outdir)\inflate.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zutil.h
$(outdir)\inflate.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\inflate.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\inflate.obj: $(miktexsrcdir)/Libraries/3rd/zlib/inftrees.h
$(outdir)\inflate.obj: $(miktexsrcdir)/Libraries/3rd/zlib/inflate.h
$(outdir)\inflate.obj: $(miktexsrcdir)/Libraries/3rd/zlib/inffast.h
$(outdir)\inflate.obj: $(miktexsrcdir)/Libraries/3rd/zlib/inffixed.h
$(outdir)\inftrees.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zutil.h
$(outdir)\inftrees.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\inftrees.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\inftrees.obj: $(miktexsrcdir)/Libraries/3rd/zlib/inftrees.h
$(outdir)\trees.obj: $(miktexsrcdir)/Libraries/3rd/zlib/deflate.h
$(outdir)\trees.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zutil.h
$(outdir)\trees.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\trees.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\trees.obj: $(miktexsrcdir)/Libraries/3rd/zlib/trees.h
$(outdir)\uncompr.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\uncompr.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\zutil.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zutil.h
$(outdir)\zutil.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\zutil.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
