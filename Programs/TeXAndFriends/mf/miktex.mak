## miktex.mak: mf
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

!include <miktex.inc>

!ifndef metafontexport
metafontexport = MiKTeX_METAFONT
!endif

metafontapp = g_METAFONTApp
metafontclass = CMETAFONTApp
metafontdata = g_METAFONTData
metafontmain = RunMETAFONT

cloptions = \
	$(ccopt_enable_function_level_linking) \
	$(ccopt_select_fastcall) \
	$(cppstandard) \
	-DMETAFONTAPP=$(metafontapp) \
	-DMETAFONTCLASS=$(metafontclass) \
	-DMETAFONTDATA=$(metafontdata) \
	-DMIKTEX_METAFONT \

.cc{$(outdir)\}.obj:
	$(cc) $(cloptions) $(ccopt_output_directory)$(outdir)\ $<

.cpp{$(outdir)\}.obj:
	$(cc) $(cloptions) $(ccopt_output_directory)$(outdir)\ $<

basedir = $(prefix)\miktex\base

binaries = \
	$(outdir)\$(mf_dll_name).dll \
	$(outdir)\inimf.exe \
	$(outdir)\mf.exe \
	$(outdir)\virmf.exe \

all: common-all

$(outdir)\virmf.exe: $(outdir) $(outdir)\mf.exe
	copy $(outdir)\mf.exe $@

$(outdir)\inimf.exe: $(outdir) $(outdir)\mf.exe
	copy $(outdir)\mf.exe $@

install: common-install install2 traptest

install2: mf.pool "$(basedir)" install-binaries
	copy mf.pool "$(basedir)"
	$(initexmf) --add-file "$(basedir)\mf.pool"

"$(basedir)":
	$(mkpath) "$(basedir)"

check: common-check

# _____________________________________________________________________________
#
# metafont
# _____________________________________________________________________________

mf_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\mf.obj \
	$(outdir)\mf.res \
	$(outdir)\screen.obj \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(mf_dll_name).dll $(outdir)\mf.lib: \
			$(outdir) \
			$(dllbases) \
			$(mf_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(mf_dll_name) \
		-delayload:gdi32.dll \
		-delayload:user32.dll \
		-export:$(metafontexport) \
		-implib:$(outdir)\mf.lib \
		-out:$(outdir)\$(mf_dll_name).dll \
		$(mf_obj) \
		delayimp.lib \
		gdi32.lib \
		user32.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\mf.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			mf-version.h \
			mf.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ mf.rc

$(outdir)\mf.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\mf.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(metafontexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\mf.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\mf.obj: mf.cc mf.h
	$(cc) $(cloptions) $(ccopt_output_directory)$(outdir)\ mf.cc

mf.cc mf.h: mf.p dyn.sed
	$(c4p) \
		$(c4prenames) \
		--def-filename=mfdefs.h \
		--dll \
		--entry-name=$(metafontmain) \
		--include-filename=mf-miktex.h \
		--one=mf \
		--var-name-prefix=m_ \
		--var-struct=$(metafontdata) \
		-C \
		mf.p
	type << >> mf.cc
MIKTEX_DEFINE_WEBAPP(
  $(metafontexport),
  $(metafontclass),
  $(metafontapp),
  $(metafontmain),
  $(metafontdata))
<<
	sed -f dyn.sed mf.h > mf.h.new
	del mf.h
	move mf.h.new mf.h

mf.p: mf.web mf-miktex.ch
	$(tangle) mf.web mf-miktex.ch mf.p nul

mf.pool: mf-miktex.ch mf.web
	$(tangle) mf.web mf-miktex.ch nul $@

# _____________________________________________________________________________
#
# trapmf
# _____________________________________________________________________________

trapmf_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\screen-trapmf.obj \
	$(outdir)\trapmf.obj \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\trapmf.dll $(outdir)\trapmf.lib: \
			$(outdir) \
			$(trapmf_obj) \

	$(link) $(dlllstandard) \
		-export:$(metafontexport) \
		-implib:$(outdir)\trapmf.lib \
		-out:$(outdir)\trapmf.dll \
		$(trapmf_obj) \
		gdi32.lib \
		user32.lib
	mt -nologo \
		-manifest $(outdir)\trapmf.dll.manifest \
		-outputresource:$(outdir)\trapmf.dll;1

$(outdir)\trapmf.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\trapmf.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(metafontexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\trapmf.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\trapmf.obj: trapmf.cc trapmf.h
	$(cc) $(cloptions) \
		$(ccopt_output_directory)$(outdir)\ \
		-DTRAPMF \
		trapmf.cc

$(outdir)\screen-trapmf.obj: \
			$(outdir) \
			screen.cpp \
			screen.h \
			trapmf.h \

	$(cc) $(cloptions) \
		$(ccopt_output_file)$(outdir)\screen-trapmf.obj \
		-DTRAPMF \
		screen.cpp

trapmf.cc trapmf.h: trapmf.p dyn.sed
	$(c4p) \
		$(c4prenames) \
		--def-filename=trapmfdefs.h \
		--dll --one=trapmf \
		--entry-name=$(metafontmain) \
		--include-filename=mf-miktex.h \
		--var-name-prefix=m_ \
		--var-struct=$(metafontdata) \
		-C \
		trapmf.p
	type << >> trapmf.cc
MIKTEX_DEFINE_WEBAPP(
  $(metafontexport),
  $(metafontclass),
  $(metafontapp),
  $(metafontmain),
  $(metafontdata))
<<
	sed -f dyn.sed trapmf.h > trapmf.h.new
	del trapmf.h
	move trapmf.h.new trapmf.h

trapmf.p: mf.web trapmf-miktex.ch
	$(tangle) mf.web trapmf-miktex.ch trapmf.p nul

trapmf-miktex.ch: mf-miktex.ch mf-miktex-trap.ch
	$(tie) -c $@ mf.web mf-miktex.ch mf-miktex-trap.ch

# _____________________________________________________________________________
#
# traptest
# _____________________________________________________________________________

trapoptions = \
	-error-line=64 \
	-half-error-line=32 \
	-max-print-line=72 \
	-mem-max=3000 \
	-mem-top=3000 \

PATH = $(bindir);$(PATH)

traptest: $(outdir)\trapmf.exe
	copy trap\trap.mf . > nul
	-$(outdir)\trapmf -initialize $(trapoptions) < trap\traptest.s2 > nul
	-diff --strip-trailing-cr trap\trapin.log trap.log
	-$(outdir)\trapmf \
		$(trapoptions) \
		-initialize \
		< trap\traptest.s3 \
		> trap.fot
	-diff --strip-trailing-cr trap\trap.fot trap.fot
	-diff --strip-trailing-cr trap\trap.log trap.log
	$(GFTYPE) trap.72270gf < trap\traptest.s5 > trap.typ
	-diff --strip-trailing-cr trap\trap.typ trap.typ
	$(TFTOPL) trap.tfm trap.pl
	-diff --strip-trailing-cr trap\trap.pl trap.pl

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

traptest-clean:
	-del /f trap.72270gf
	-del /f trap.base
	-del /f trap.fot
	-del /f trap.log
	-del /f trap.mf
	-del /f trap.pl
	-del /f trap.tfm
	-del /f trap.typ

mostlyclean: common-mostlyclean
	-del /f *.p
	-del /f mf.cc
	-del /f mf.h
	-del /f mfdefs.h
	-del /f trapmf-miktex.ch
	-del /f trapmf.cc
	-del /f trapmf.h
	-del /f trapmfdefs.h

clean: common-clean mostlyclean traptest-clean
	-del /f mf.pool
	-del /f plain.log

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: trapmf.cc mf.cc screen.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\trapmf.obj: trapmf.h mf-miktex.h mfdefs.h mf.rc mf-version.h
$(outdir)\trapmf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/miktex.version
$(outdir)\trapmf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/version.h
$(outdir)\trapmf.obj: screen.h
$(outdir)\trapmf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/help.h
$(outdir)\mf.obj: mf.h mf-miktex.h mfdefs.h mf.rc mf-version.h
$(outdir)\mf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/miktex.version
$(outdir)\mf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/version.h
$(outdir)\mf.obj: screen.h
$(outdir)\mf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/help.h
$(outdir)\screen.obj: mf.h mf-miktex.h mfdefs.h mf.rc mf-version.h
$(outdir)\screen.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/miktex.version
$(outdir)\screen.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/version.h
$(outdir)\screen.obj: screen.h
$(outdir)\screen.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/help.h
