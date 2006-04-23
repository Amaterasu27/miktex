## miktex.mak:
##
## Copyright (C) 2002-2006 Christian Schenk
##
## This file is part of the MiKTeX Font Map Creator.
## 
## The MiKTeX Font Map Creator is free software; you can redistribute
## it and/or modify it under the terms of the GNU General Public
## License as published by the Free Software Foundation; either
## version 2, or (at your option) any later version.
## 
## The MiKTeX Font Map Creator is distributed in the hope that it will
## be useful, but WITHOUT ANY WARRANTY; without even the implied
## warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## See the GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with The MiKTeX Font Map Creator; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA. */

miktex_cc_warnings_as_errors = 1

!include <miktex.inc>

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_directory)$(outdir)\ $<

binaries = \
	$(outdir)\$(mkfntmap_dll_name).dll \
	$(outdir)\mkfntmap.exe \
	$(outdir)\updmap.exe

all: common-all

install: common-install install-binaries

check: common-crt

# _____________________________________________________________________________
#
# mkfntmap.dll
# _____________________________________________________________________________

objects = \
	$(outdir)\mkfntmap.obj \
	$(outdir)\mkfntmap.res \
	$(miktex_lib) \
	$(popt_lib)

$(outdir)\$(mkfntmap_dll_name).dll $(outdir)\mkfntmap.lib: \
					$(outdir) \
					$(dllbases) \
					$(objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(mkfntmap_dll_name) \
		-export:mkfntmapmain \
		-implib:$(outdir)\mkfntmap.lib \
		-out:$(outdir)\$(mkfntmap_dll_name).dll \
		$(objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\mkfntmap.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			mkfntmap-version.h \
			mkfntmap.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ mkfntmap.rc

# _____________________________________________________________________________
#
# programs
# _____________________________________________________________________________

$(outdir)\mkfntmap.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\mkfntmap.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLMAIN=mkfntmapmain \
		-DDLLAPI=__cdecl \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\mkfntmap.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\updmap.exe: $(outdir)\mkfntmap.exe
	copy $(outdir)\mkfntmap.exe $@

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: mkfntmap.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\mkfntmap.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\mkfntmap.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\mkfntmap.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\mkfntmap.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\mkfntmap.obj: $(miktexsrcdir)/Libraries\3rd\libpopt/popt-miktex.h
$(outdir)\mkfntmap.obj: $(miktexsrcdir)/Libraries\3rd\libpopt/popt.h
$(outdir)\mkfntmap.obj: internal.h mkfntmap.rc mkfntmap-version.h
$(outdir)\mkfntmap.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/miktex.version
$(outdir)\mkfntmap.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/version.h
