## miktex.mak: libgnu-static
##
## Copyright (C) 2001-2006 Christian Schenk
##
## This file is part of the MiKTeX GNU Library (gnu.dll).
## The MiKTeX GNU Library is free software; you can redistribute it
## and/or modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2, or
## (at your option) any later version.
## 
## The MiKTeX GNU Library is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied warranty
## of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with the MiKTeX GNU Library; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

no_miktex_dlls = 1
miktex_cc_use_crt_dll = 0

!include <miktex.inc>

srcdir = $(gnulibdir)

sources = \
	$(gnulibdir)\basename.c \
	$(gnulibdir)\getopt.c \
	$(gnulibdir)\getopt1.c \
	$(gnulibdir)\regex.c \
	$(gnulibdir)\substring.c \

defines = \
	-DSTDC_HEADERS \
	-DWIN32 \

{$(gnulibdir)\}.c{$(outdir)\}.obj:
	$(cc) $(cstandard) $(defines) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(gnu_static_lib)

install: common-install all

check: common-check

# _____________________________________________________________________________
#
# gnu-static.lib
# _____________________________________________________________________________

gnu_objects = \
	$(outdir)\basename.obj \
	$(outdir)\getopt.obj \
	$(outdir)\getopt1.obj \
	$(outdir)\regex.obj \
	$(outdir)\substring.obj \

$(gnu_static_lib): $(outdir) $(gnu_objects)
	$(libtool) $(libflags) $(libopt_output_file)$@ $(gnu_objects)

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
	$(MAKEDEPEND) $(defines) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\basename.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/internal.h
$(outdir)\basename.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\getopt.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h
$(outdir)\getopt.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/getopt_int.h
$(outdir)\getopt1.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h
$(outdir)\getopt1.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/getopt_int.h
$(outdir)\regex.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/regex.h
$(outdir)\regex.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/regex_internal.h
$(outdir)\regex.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/regex_internal.c
$(outdir)\regex.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/regcomp.c
$(outdir)\regex.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/regexec.c
$(outdir)\substring.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/internal.h
$(outdir)\substring.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
