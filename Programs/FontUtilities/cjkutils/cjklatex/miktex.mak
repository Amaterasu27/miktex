## miktex.mak: cjkutils/cjklatex
##
## Copyright (C) 2001-2004 Christian Schenk
##
## This file is part of the CJKLaTeX.
## 
## The CJKLaTeX is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License as
## published by the Free Software Foundation; either version 2, or (at
## your option) any later version.
## 
## The CJKLaTeX is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with The CJKLaTeX; if not, write to the Free Software
## Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
## USA. */

!include <miktex.inc>

cloptions = $(cstandard) $(cflagspackfunc) $(cflagseh) 

.cpp{$(outdir)\}.obj:
	$(cc) $(cloptions) $(ccopt_output_directory)$(outdir)\ $<

aliases = \
	$(outdir)\bg5+latex.exe \
	$(outdir)\bg5+pdflatex.exe \
	$(outdir)\bg5latex.exe \
	$(outdir)\bg5pdflatex.exe \
	$(outdir)\bg5platex.exe \
	$(outdir)\bg5ppdflatex.exe \
	$(outdir)\cef5latex.exe \
	$(outdir)\cef5pdflatex.exe \
	$(outdir)\ceflatex.exe \
	$(outdir)\cefpdflatex.exe \
	$(outdir)\cefslatex.exe \
	$(outdir)\cefspdflatex.exe \
	$(outdir)\gbklatex.exe \
	$(outdir)\gbkpdflatex.exe \
	$(outdir)\sjislatex.exe \
	$(outdir)\sjispdflatex.exe \

binaries = \
	$(aliases) \
	$(outdir)\$(cjklatex_dll_name).dll \
	$(outdir)\cjklatex.exe \

all: common-all

install: common-install install-binaries

check: common-crt

# _____________________________________________________________________________
#
# cjklatex.dll
# _____________________________________________________________________________

objects = \
	$(app_lib) \
	$(gnu_lib) \
	$(miktex_lib) \
	$(mpm_lib) \
	$(outdir)\cjklatex.obj \
	$(outdir)\cjklatex.res \
	$(texmf_lib) \

$(outdir)\$(cjklatex_dll_name).dll $(outdir)\cjklatex.lib: \
			$(outdir) \
			$(dllbases) \
			$(objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(cjklatex_dll_name) \
		-delayload:$(mpm_dll_name).dll \
		-export:cjklatexmain \
		-implib:$(outdir)\cjklatex.lib \
		$(linkopt_output_file)$(outdir)\$(cjklatex_dll_name).dll \
		$(objects) \
		delayimp.lib

$(outdir)\cjklatex.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			$(outdir) cjklatex-version.h \
			$(outdir) cjklatex.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ cjklatex.rc

# _____________________________________________________________________________
#
# programs
# _____________________________________________________________________________

$(outdir)\cjklatex.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\cjklatex.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLMAIN=cjklatexmain \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\cjklatex.lib \

$(outdir)\bg5+latex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe "$@"

$(outdir)\bg5+pdflatex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe "$@"

$(outdir)\bg5latex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe $@

$(outdir)\bg5pdflatex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe $@

$(outdir)\bg5platex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe $@

$(outdir)\bg5ppdflatex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe $@

$(outdir)\cef5latex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe $@

$(outdir)\cef5pdflatex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe $@

$(outdir)\ceflatex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe $@

$(outdir)\cefpdflatex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe $@

$(outdir)\cefslatex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe $@

$(outdir)\cefspdflatex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe $@

$(outdir)\gbklatex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe $@

$(outdir)\gbkpdflatex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe $@

$(outdir)\sjislatex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe $@

$(outdir)\sjispdflatex.exe: $(outdir)\cjklatex.exe
	copy $(outdir)\cjklatex.exe $@

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: cjklatex.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\cjklatex.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h
$(outdir)\cjklatex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\cjklatex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\cjklatex.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\cjklatex.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\cjklatex.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\cjklatex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\cjklatex.obj: cjklatex-version.h
