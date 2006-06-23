## miktex.mak: web
## 
## Copyright (C) 1991-2005 Christian Schenk
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

.cc{$(outdir)\}.obj:
	$(cc) \
		$(cppstandard) \
		$(ccopt_all_cpp) \
		$(ccopt_output_directory)$(outdir)\ \
		$<

binaries = \
	$(outdir)\$(tangle_dll_name).dll \
	$(outdir)\$(weave_dll_name).dll \
	$(outdir)\tangle.exe \
	$(outdir)\weave.exe \
	$(outdir)\web-n.exe \

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# web-n
# _____________________________________________________________________________

$(outdir)\web-n.exe: \
			$(outdir) \
			$(outdir)\numberit.obj \

	$(link) $(lstandard) \
		-out:$@ \
		$(outdir)\numberit.obj
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# tangle
# _____________________________________________________________________________

!ifndef tangleexport
tangleexport = MiKTeX_TANGLE
!endif

tangleclass = TANGLE
tanglemain = RunTANGLE
tangleapp = g_TANGLEApp
tangledata = g_TANGLEData

tangle_objects = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\tangle.obj \
	$(outdir)\tangle.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(tangle_dll_name).dll $(outdir)\tangle.lib: \
			$(outdir) \
			$(tangle_objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(tangle_dll_name) \
		-export:$(tangleexport) \
		-implib:$(outdir)\tangle.lib \
		-out:$(outdir)\$(tangle_dll_name).dll \
		$(tangle_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\tangle.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\tangle.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(tangleexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\tangle.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\tangle.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			tangle-version.h \
			tangle.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ tangle.rc

$(outdir)\tangle.obj: \
			tangle.cc \
			tangle.h \

	$(cc) $(cppstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		-DTANGLEAPP=$(tangleapp) \
		-DTANGLECLASS=$(tangleclass) \
		-DTANGLEDATA=$(tangledata) \
		tangle.cc

tangle.cc tangle.h: \
			.bootstrap \
			tangle.p \

	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(tanglemain) \
		--include-filename=tangle-miktex.h \
		--one=tangle \
		--var-name-prefix=m_ \
		--var-struct=$(tangledata) \
		-C \
		tangle.p
	type << >> tangle.cc
MIKTEX_DEFINE_WEBAPP(
  $(tangleexport),
  $(tangleclass),
  $(tangleapp),
  $(tanglemain),
  $(tangledata))
<<

tangle.p: tangle.web tangle-miktex.ch
	$(tangle) tangle.web tangle-miktex.ch tangle.p nul

.bootstrap: inittangle.p
	@-del /f tangle.p
	type inittangle.p > tangle.p
	echo x > .bootstrap

# _____________________________________________________________________________
#
# weave
# _____________________________________________________________________________

!ifndef weaveexport
weaveexport = MiKTeX_WEAVE
!endif

weaveclass = WebApp
weavemain = RunWEAVE
weaveapp = g_WEAVEApp
weavedata = g_WEAVEData

weave_objects = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\weave.obj \
	$(outdir)\weave.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(weave_dll_name).dll $(outdir)\weave.lib: \
			$(outdir) \
			$(weave_objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(weave_dll_name) \
		-export:$(weaveexport) \
		-implib:$(outdir)\weave.lib \
		-out:$(outdir)\$(weave_dll_name).dll \
		$(weave_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\weave.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\weave.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(weaveexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\weave.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\weave.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			weave-version.h \
			weave.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ weave.rc


weave.cc weave.h: weave.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(weavemain) \
		--include-filename=mywebapp.h \
		--one=weave \
		--var-struct=$(weavedata) \
		-C \
		weave.p
	type << >> weave.cc
MIKTEX_DEFINE_WEBAPP(
  $(weaveexport),
  $(weaveclass),
  $(weaveapp),
  $(weavemain),
  $(weavedata))
<<

mywebapp.h:
	type << >> $@
#include <miktex/webapp.h>
using namespace MiKTeX::TeXAndFriends;
<<

$(outdir)\weave.obj: mywebapp.h

PATH = $(bindir);$(PATH)

weave.p: $(outdir)\tangle.exe weave.web weave-miktex.ch
	$(outdir)\tangle.exe weave.web weave-miktex.ch weave.p nul

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean
	-del /f mywebapp.h
	-del /f tangle.cc
	-del /f tangle.h
	-del /f weave.cc
	-del /f weave.h
	-del /f weave.p

distclean: common-distclean clean
	-del /f .bootstrap
	-del /f tangle.p

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: tangle.cc weave.cc
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\tangle.obj: tangle.h
$(outdir)\tangle.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\tangle.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\tangle.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\tangle.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\tangle.obj: tangle-miktex.h tangle.rc tangle-version.h
$(outdir)\tangle.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\tangle.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\tangle.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\tangle.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\tangle.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\tangle.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\tangle.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\tangle.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\tangle.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\weave.obj: weave.h
$(outdir)\weave.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\weave.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\weave.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\weave.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\weave.obj: mywebapp.h
$(outdir)\weave.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\weave.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\weave.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\weave.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\weave.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\weave.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
