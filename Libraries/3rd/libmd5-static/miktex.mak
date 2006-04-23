## miktex.mak: libmd5-static
##
## Copyright (C) 2001-2006 Christian Schenk
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
miktex_cc_warnings_as_errors = 1
no_miktex_dlls = 1

!include <miktex.inc>

srcdir = $(md5libdir)

objects = \
	$(outdir)\md5c.obj \

sources = \
	$(md5libdir)\md5c.cpp \

{$(md5libdir)\}.cpp{$(outdir)\}.obj:
	 $(cc) $(cppstandard) $(ccopt_output_file)$@ $<

all: common-all $(md5_static_lib)

install: common-install all

check: common-check

# _____________________________________________________________________________
#
# mp5-static.lib
# _____________________________________________________________________________

$(md5_static_lib): $(outdir) $(objects)
	$(libtool) $(libflags) $(libopt_output_file)$@ $(objects)

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean

distclean: common-distclean

maintainer-clean: common-maintainer-clean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: $(sources)
	$(MAKEDEPEND) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\md5c.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/internal.h
$(outdir)\md5c.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/global.h
$(outdir)\md5c.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
