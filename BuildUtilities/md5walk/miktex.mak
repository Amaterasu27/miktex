## miktex.mak: md5walk
## 
## Copyright (C) 2001-2008 Christian Schenk
## 
## This file is part of MD5Walk.
## 
## MD5Walk is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
## 
## MD5Walk is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with MD5Walk; if not, write to the Free Software Foundation,
## 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

miktex_cc_use_crt_dll = 0
miktex_cc_warnings_as_errors = 1
no_miktex_dlls = 1

!include <miktex.inc>

binaries = $(outdir)\md5walk.exe

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# md5walk
# _____________________________________________________________________________

md5walk_objects = \
	$(bz2_static_lib) \
	$(md5_static_lib) \
	$(miktex_static_lib) \
	$(outdir)\md5walk.obj \
	$(outdir)\md5walk.res \
	$(popt_static_lib) \
	$(zlib_static_lib) \

$(outdir)\md5walk.exe: \
			$(outdir) \
			$(md5walk_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(md5walk_objects) \
		advapi32.lib \
		shlwapi.lib \
		user32.lib \
		winspool.lib \

$(outdir)\md5walk.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			md5walk-version.h \
			md5walk.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ md5walk.rc

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

depend: md5walk.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\md5walk.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\md5walk.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\md5walk.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\md5walk.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\md5walk.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\md5walk.obj: md5walk-version.h
