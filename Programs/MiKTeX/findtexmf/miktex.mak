## miktex.mak:
##
## Copyright (C) 2001-2006 Christian Schenk
##
## This file is part of the MiKTeX Finder.
## 
## The MiKTeX Finder is free software; you can redistribute
## it and/or modify it under the terms of the GNU General Public
## License as published by the Free Software Foundation; either
## version 2, or (at your option) any later version.
## 
## The MiKTeX Finder is distributed in the hope that it will
## be useful, but WITHOUT ANY WARRANTY; without even the implied
## warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## See the GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with The MiKTeX Finder; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA. */

miktex_cc_warnings_as_errors = 1

!include <miktex.inc>

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_directory)$(outdir)\ $<

binaries = \
	$(outdir)\$(findtexmf_dll_name).dll \
	$(outdir)\findtexmf.exe \
	$(outdir)\kpsewhich.exe \

all: common-all

install: common-install install-binaries

check: common-crt

# _____________________________________________________________________________
#
# findtexmf.dll
# _____________________________________________________________________________

objects = \
	$(app_lib) \
	$(miktex_lib) \
	$(mpm_lib) \
	$(outdir)\findtexmf.obj \
	$(outdir)\findtexmf.res \
	$(popt_lib) \

$(outdir)\$(findtexmf_dll_name).dll $(outdir)\findtexmf.lib: \
			$(outdir) \
			$(objects) \
			$(dllbases) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(findtexmf_dll_name) \
		-delayload:$(mpm_dll_name).dll \
		-export:findtexmfmain \
		-implib:$(outdir)\findtexmf.lib \
		-out:$(outdir)\$(findtexmf_dll_name).dll \
		$(objects) \
		delayimp.lib \
		shell32.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\findtexmf.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		findtexmf-version.h \
		findtexmf.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ findtexmf.rc

# _____________________________________________________________________________
#
# programs
# _____________________________________________________________________________

$(outdir)\findtexmf.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\findtexmf.lib \

	$(cc) \
		$(alias_cpp) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLMAIN=findtexmfmain \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\findtexmf.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\kpsewhich.exe: $(outdir)\findtexmf.exe
	copy $(outdir)\findtexmf.exe $@

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: findtexmf.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\findtexmf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\findtexmf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\findtexmf.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\findtexmf.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\findtexmf.obj: $(miktexsrcdir)/Libraries\MiKTeX\PackageManager/include/miktex/mpm.h
$(outdir)\findtexmf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\findtexmf.obj: $(miktexsrcdir)/Libraries\3rd\libpopt/popt-miktex.h
$(outdir)\findtexmf.obj: $(miktexsrcdir)/Libraries\3rd\libpopt/popt.h
$(outdir)\findtexmf.obj: findtexmf-version.h
