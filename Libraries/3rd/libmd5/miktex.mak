## miktex.mak: libmd5
##
## Copyright (C) 2001-2005 Christian Schenk
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

miktex_cc_warnings_as_errors = 1

!include <miktex.inc>

objects = \
	$(outdir)\md5c.obj \

sources = \
	md5c.cpp \

md5_dll = $(md5_dll_name).dll

binaries = $(outdir)\$(md5_dll)

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) \
		-DMD5_DLL \
		$(ccopt_output_file)$@ $<

all: common-all $(md5_lib)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# md5.dll
# _____________________________________________________________________________

$(outdir)\$(md5_dll) $(md5_lib): \
			$(outdir) \
			$(dllbases) \
			$(objects) \
			$(outdir)\md5.res \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(md5_dll_name) \
		-implib:$(md5_lib) \
		-out:$(outdir)\$(md5_dll) \
		$(objects) $(outdir)\md5.res
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\md5.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			md5-version.h \
			md5.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ md5.rc

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

$(outdir)\md5c.obj: internal.h global.h
$(outdir)\md5c.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
