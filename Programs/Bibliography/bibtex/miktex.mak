## miktex.mak: bibtex
## 
## Copyright (C) 1996-2005 Christian Schenk
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

!ifndef bibtexexport
bibtexexport = MiKTeX_BibTeX
!endif

bibtexclass = CBibTeXApp
bibtexmain = RunBibTeX
bibtexapp = g_BibTeXApp
bibtexdata = g_BibTeXData

.cc{$(outdir)\}.obj:
	$(cc) $(cppstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		-DBIBTEXAPP=$(bibtexapp) \
		-DBIBTEXCLASS=$(bibtexclass) \
		-DBIBTEXDATA=$(bibtexdata) \
		-DMIKTEX_BIBTEX \
		$<

binaries = $(outdir)\$(bibtex_dll_name).dll $(outdir)\bibtex.exe

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# bibtex
# _____________________________________________________________________________

bibtex_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\bibtex.obj \
	$(outdir)\bibtex.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(bibtex_dll_name).dll $(outdir)\bibtex.lib: \
			$(outdir) \
			$(bibtex_obj) \
			$(dllbases) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(bibtex_dll_name) \
		-export:$(bibtexexport) \
		-implib:$(outdir)\bibtex.lib \
		-out:$(outdir)\$(bibtex_dll_name).dll \
		$(bibtex_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\bibtex.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			bibtex-version.h \
			bibtex.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ bibtex.rc

$(outdir)\bibtex.exe: \
			$(outdir) \
			$(outdir)\bibtex.lib \
			$(alias_cpp) \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(bibtexexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\bibtex.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

bibtex.cc bibtex.h: bibtex.p dyn.sed
	$(c4p) $(c4prenames) \
		--def-filename=bibtexdefs.h \
		--dll \
		--entry-name=$(bibtexmain) \
		--include-filename=bibtex-miktex.h \
		--one=bibtex \
		--var-name-prefix=m_ \
		--var-struct=$(bibtexdata) \
		-C \
		bibtex.p
	type << >> bibtex.cc
MIKTEX_DEFINE_WEBAPP(
  $(bibtexexport),
  $(bibtexclass),
  $(bibtexapp),
  $(bibtexmain),
  $(bibtexdata))
<<
	sed -f dyn.sed bibtex.h > bibtex.h.new
	del bibtex.h
	move bibtex.h.new bibtex.h

bibtex.p: bibtex.web bibtex-miktex.ch
	$(tangle) bibtex.web bibtex-miktex.ch bibtex.p nul

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

mostlyclean: common-mostlyclean
	-del /f *.cc
	-del /f bibtex.h
	-del /f bibtexdefs.h
	-del /f *.p

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: bibtex.cc
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\bibtex.obj: bibtex.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\bibtex.obj: bibtex-miktex.h bibtex.rc bibtex-version.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\bibtex.obj: bibtexdefs.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
