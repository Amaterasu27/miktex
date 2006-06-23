## miktex.mak: ps2pk/PSres
## 
## Copyright (C) 2000-2004 Christian Schenk
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

common_lib = ..\common\$(outdir)\common.lib

libraries = $(outdir)\psres.lib

binaries = $(outdir)\$(psres_dll_name).dll

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		-DUSE_MIKTEX_EXIT \
		$(ccopt_output_directory)$(outdir)\ \
		$<

all: common-all $(libraries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# psres.dll
# _____________________________________________________________________________

psres_objects = \
	$(common_lib) \
	$(miktex_lib) \
	$(outdir)\PSres.obj \
	$(outdir)\PSres.res \
	$(outdir)\psres.exp \

$(outdir)\$(psres_dll_name).dll: \
			$(outdir) \
			$(psres_objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(psres_dll_name) \
		-out:$@ \
		$(psres_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\PSres.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			PSres-version.h \
			PSres.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ PSres.rc

$(outdir)\psres.lib $(outdir)\psres.exp: \
			$(outdir) \
			psres.def \

	$(libtool) $(libflags) \
		-def:psres.def \
		-name:$(psres_dll_name) \
		$(libopt_output_file)$(outdir)\psres.lib \

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend:
	$(MAKEDEPEND) PSres.c
	$(fixdep)

# DO NOT DELETE

$(outdir)\PSres.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\PSres.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\PSres.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\PSres.obj: PSres.h defs.h
