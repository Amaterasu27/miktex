## miktex.mak:
## 
## Copyright (C) 2004-2006 Christian Schenk
##
## This file is part of MTHelp.
##
## MTHelp is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
## 
## MTHelp is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with MTHelp; if not, write to the Free Software Foundation,
## 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

miktex_cc_warnings_as_errors = 1

!include <miktex.inc>

binaries = \
	$(outdir)\$(mthelp_dll_name).dll \
	$(outdir)\mthelp.exe \
	$(outdir)\texdoc.exe \

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# template.html.h
# _____________________________________________________________________________

template.html.h: template.html
	$(bintoc) templateHtml < template.html > template.html.h

# _____________________________________________________________________________
#
# mthelp.dll
# _____________________________________________________________________________

objects = \
	$(app_lib) \
	$(miktex_lib) \
	$(mpm_lib) \
	$(outdir)\mthelp.obj \
	$(outdir)\mthelp.res \
	$(popt_lib) \

$(outdir)\$(mthelp_dll_name).dll $(mthelp_lib): \
			$(outdir) \
			$(dllbases) \
			$(objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(mthelp_dll_name) \
		-export:mthelpmain \
		-implib:$(mthelp_lib) \
		-out:$(outdir)\$(mthelp_dll_name).dll \
		$(objects) \
		shell32.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\mthelp.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			mthelp-version.h \
			mthelp.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ mthelp.rc

# _____________________________________________________________________________
#
# mthelp.exe
# _____________________________________________________________________________

$(outdir)\mthelp.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\mthelp.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLMAIN=mthelpmain \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\mthelp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# texdoc.exe
# _____________________________________________________________________________

$(outdir)\texdoc.exe: $(outdir)\mthelp.exe
	copy $(outdir)\mthelp.exe $(outdir)\texdoc.exe

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean
	-del /f template.html.h

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: mthelp.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\mthelp.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\mthelp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\mthelp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\mthelp.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\mthelp.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\mthelp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\mthelp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\mthelp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\mthelp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\mthelp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\mthelp.obj: mthelp-version.h template.html.h
