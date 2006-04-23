## arctrl.cpp:
##
## Copyright (C) 2006 Christian Schenk
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
## along with This file; if not, write to the Free Software
## Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
## USA. */

miktex_cc_warnings_as_errors = 1

!include <miktex.inc>

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_directory)$(outdir)\ $<

binaries = \
	$(outdir)\$(arctrl_dll_name).dll \
	$(outdir)\arctrl.exe \
	$(outdir)\pdfclose.exe \
	$(outdir)\pdfdde.exe \
	$(outdir)\pdfopen.exe \

all: common-all

install: common-install install-binaries

check: common-crt

# _____________________________________________________________________________
#
# arctrl.dll
# _____________________________________________________________________________

objects = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\arctrl.obj \
	$(outdir)\arctrl.res \
	$(popt_lib) \

$(outdir)\$(arctrl_dll_name).dll $(arctrl_lib): \
			$(outdir) \
			$(dllbases) \
			$(objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(arctrl_dll_name) \
		-export:arctrlmain \
		-implib:$(arctrl_lib) \
		-out:$(outdir)\$(arctrl_dll_name).dll \
		$(objects) \
		shell32.lib \
		user32.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\arctrl.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			arctrl-version.h \
			arctrl.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ arctrl.rc

# _____________________________________________________________________________
#
# programs
# _____________________________________________________________________________

$(outdir)\arctrl.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\arctrl.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLMAIN=arctrlmain \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\arctrl.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\pdfdde.exe: $(outdir)\arctrl.exe
	copy $(outdir)\arctrl.exe $@

$(outdir)\pdfclose.exe: $(outdir)\arctrl.exe
	copy $(outdir)\arctrl.exe $@

$(outdir)\pdfopen.exe: $(outdir)\arctrl.exe
	copy $(outdir)\arctrl.exe $@

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

depend: arctrl.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\arctrl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\arctrl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\arctrl.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\arctrl.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\arctrl.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\arctrl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\arctrl.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\arctrl.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\arctrl.obj: arctrl-version.h
