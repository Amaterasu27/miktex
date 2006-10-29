## miktex.mak: etex
## 
## Copyright (C) 1998-2005 Christian Schenk
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

cloptions = \
	$(cppstandard) \
	$(ccopt_compile_all_files_as_cpp) \
	$(ccopt_default_char_type_is_unsigned) \
	$(ccopt_enable_function_level_linking) \
	$(ccopt_select_fastcall) \
	-DMIKTEX_ETEX \

formatdir = $(prefix)\miktex\fmt

binaries = \
	$(outdir)\$(etex_dll_name).dll \
	$(outdir)\einitex.exe \
	$(outdir)\etex.exe \
	$(outdir)\evirtex.exe \

.cc{$(outdir)\}.obj:
	$(cc) $(cloptions) $(ccopt_output_file)$@ $<

all: common-all

$(outdir)\einitex.exe: $(outdir) $(outdir)\etex.exe
	copy $(outdir)\etex.exe $@

$(outdir)\evirtex.exe: $(outdir) $(outdir)\etex.exe
	copy $(outdir)\etex.exe $@

install: common-install install2 etriptest

install2: etex.pool $(formatdir) install-binaries
	copy etex.pool $(formatdir)\etex.pool

$(formatdir):
	$(mkpath) $(formatdir)

check: common-check

# _____________________________________________________________________________
#
# etex.web
#
# Build the system-independent web file.
# _____________________________________________________________________________

tex_web = $(miktexsrcdir)\$(rel_texdir)\tex.web

etex.web: $(tex_web) etex.ch
	$(tie) -m $@ $(tex_web) etex.ch

# _____________________________________________________________________________
#
# etex-miktex.web
#
# Build the MiKTeX-specific web file.
# _____________________________________________________________________________

tex_miktex_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-misc.ch
tex_miktex_hyph_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-hyph.ch
tex_miktex_quiet_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-quiet.ch
tex_miktex_src_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-src.ch
tex_miktex_stat_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-stat.ch
tex_miktex_trip_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-trip.ch
tex_miktex_write18_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-write18.ch
tex_mltex_miktex_ch = $(miktexsrcdir)\$(rel_texdir)\mltex-miktex.ch

etex_miktex_pre_ch = etex-miktex-pre.ch
etex_miktex_post_ch = etex-miktex-post.ch

etex_change_files = \
	$(etex_miktex_pre_ch) \
	$(tex_mltex_miktex_ch) \
	$(tex_miktex_ch) \
	$(tex_miktex_hyph_ch) \
	$(tex_miktex_quiet_ch) \
	$(tex_miktex_src_ch) \
	$(tex_miktex_stat_ch) \
	$(tex_miktex_write18_ch) \
	$(etex_miktex_post_ch) \

etriptex_change_files = \
	$(etex_change_files) \
	$(tex_miktex_trip_ch) \

etex-miktex.web: etex.web $(etex_change_files)
	$(tie) -m $@ etex.web $(etex_change_files)

etriptex-miktex.web: etex.web $(etriptex_change_files)
	$(tie) -m $@ etex.web $(etriptex_change_files)

# _____________________________________________________________________________
#
# etex-miktex-all.ch
#
# Build the change file which will be applied to the MiKTeX-specific
# web file.
# _____________________________________________________________________________

etex-miktex-all.ch: etex-miktex.web etex-miktex.ch etex-miktex-fmt.ch
	$(tie) -c \
		etex-miktex-all.ch \
		etex-miktex.web \
		etex-miktex.ch \
		etex-miktex-fmt.ch \

# _____________________________________________________________________________
#
# etex
# _____________________________________________________________________________

!ifndef etexexport
etexexport = MiKTeX_eTeX
!endif

etexapp = g_eTeXApp
etexclass = CeTeXApp
etexdata = g_eTeXData
etexmain = RuneTeX

tex_dyn_sed = $(miktexsrcdir)\$(rel_texdir)\dyn.sed

etex_dyn_sed = dyn.sed

etex_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\etex.obj \
	$(outdir)\etex.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(etex_dll_name).dll $(outdir)\etex.lib: \
			$(outdir) \
			$(dllbases) \
			$(etex_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(etex_dll_name) \
		-export:$(etexexport) \
		-implib:$(outdir)\etex.lib \
		-out:$(outdir)\$(etex_dll_name).dll \
		$(etex_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\etex.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			etex-version.h \
			etex.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ etex.rc

$(outdir)\etex.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\etex.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(etexexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\etex.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\etex.obj: etex.cc etex.h
	$(cc) $(cloptions) \
		$(ccopt_output_file)$@ \
		-DETEXAPP=$(etexapp) \
		-DETEXCLASS=$(etexclass) \
		-DETEXDATA=$(etexdata) \
		etex.cc

etex.cc etex.h: etex.p $(tex_dyn_sed) $(etex_dyn_sed)
	$(c4p) \
		$(c4prenames) \
		--chars-are-unsigned \
		--def-filename=etexdefs.h \
		--dll \
		--entry-name=$(etexmain) \
		--include-filename=etex-miktex.h \
		--one=etex \
		--var-name-prefix=m_ \
		--var-struct=$(etexdata) \
		-C \
		etex.p
	type << >> etex.cc
MIKTEX_DEFINE_WEBAPP(
  $(etexexport),
  $(etexclass),
  $(etexapp),
  $(etexmain),
  $(etexdata))
<<
	sed -f $(tex_dyn_sed) etex.h > etex.h.tmp
	sed -f $(etex_dyn_sed) etex.h.tmp > etex.h
	del etex.h.tmp

etex.p: etex-miktex.web etex-miktex-all.ch
	$(tangle) etex-miktex.web etex-miktex-all.ch etex.p nul

etex.pool: etex-miktex.web etex-miktex-all.ch
	$(tangle) etex-miktex.web etex-miktex-all.ch nul $@

etex-miktex.web: $(tex_web) $(change_files)
	$(tie) -m $@ $(tex_web) $(change_files)

# _____________________________________________________________________________
#
# etriptex
# _____________________________________________________________________________

etriptex_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\etriptex.obj \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\etriptex.dll $(outdir)\etriptex.lib: \
			$(outdir) \
			$(etriptex_obj) \

	$(link) $(dlllstandard) \
		-export:$(etexexport) \
		-implib:$(outdir)\etriptex.lib \
		-out:$(outdir)\etriptex.dll \
		$(etriptex_obj)
	mt -nologo \
		-manifest $(outdir)\etriptex.dll.manifest \
		-outputresource:$(outdir)\etriptex.dll;1

$(outdir)\etriptex.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\etriptex.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(etexexport) \
		$(alias_cpp) \
		$(ccopt_link) $(linkopt_output_file)$@ $(outdir)\etriptex.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\etriptex.obj: etriptex.cc etriptex.h
	$(cc) $(cloptions) \
		$(ccopt_output_file)$@ \
		-DETEXAPP=$(etexapp) \
		-DETEXCLASS=$(etexclass) \
		-DETEXDATA=$(etexdata) \
		-DETRIPTEX \
		etriptex.cc

etriptex.cc etriptex.h: etriptex.p $(tex_dyn_sed) $(etex_dyn_sed)
	$(c4p) \
		$(c4prenames) \
		--chars-are-unsigned \
		--def-filename=etriptexdefs.h \
		--dll \
		--entry-name=$(etexmain) \
		--include-filename=etex-miktex.h \
		--one=etriptex \
		--var-name-prefix=m_ \
		--var-struct=$(etexdata) \
		-C \
		etriptex.p
	type << >> etriptex.cc
MIKTEX_DEFINE_WEBAPP(
  $(etexexport),
  $(etexclass),
  $(etexapp),
  $(etexmain),
  $(etexdata))
<<
	sed -f $(tex_dyn_sed) etriptex.h > etriptex.h.tmp
	sed -f $(etex_dyn_sed) etriptex.h.tmp > etriptex.h
	del etriptex.h.tmp

etriptex.p: etriptex-miktex.web etex-miktex.ch
	$(tangle) etriptex-miktex.web etex-miktex-all.ch etriptex.p nul

# _____________________________________________________________________________
#
# etriptest
# _____________________________________________________________________________

etripoptions =	\
	-buf-size=500 \
	-error-line=64 \
	-font-max=75 \
	-font-mem-size=20000 \
	-half-error-line=32 \
	-max-in-open=6 \
	-max-print-line=72 \
	-max-strings=3200 \
	-mem-bot=1 \
	-mem-max=3000 \
	-mem-min=1 \
	-mem-top=3000 \
	-nest-size=40 \
	-param-size=60 \
	-pool-size=50000 \
	-save-size=600 \
	-stack-size=200 \
	-strict \
	-string-vacancies=8000 \
	-trie-op-size=500 \
	-trie-size=8000 \

etriptest: etex.pool triptest $(outdir)\etriptex.exe
	$(pltotf) etrip\etrip.pl etrip.tfm
	copy etrip\etrip.tex .>nul
	-$(outdir)\etriptex \
		-initialize \
		$(etripoptions) \
		< etrip\etriptest.s7 \
		> nul
	-diff --strip-trailing-cr etrip\etripin.log etrip.log
	-$(outdir)\etriptex \
		-initialize \
		$(etripoptions) \
		< etrip\etriptest.s8 \
		> etrip.fot
	-diff --strip-trailing-cr etrip\etrip.fot etrip.fot
	-diff --strip-trailing-cr etrip\etrip.out etrip.out
	-diff --strip-trailing-cr etrip\etrip.log etrip.log
	$(dvitype) \
		-out-mode=2 \
		-start-there=*.*.*.*.*.*.*.*.*.* \
		-resolution=7227/100 etrip.dvi \
		> etrip.typ
	-diff --strip-trailing-cr etrip\etrip.typ etrip.typ

# _____________________________________________________________________________
#
# triptest
# _____________________________________________________________________________

tripdir = $(miktexsrcdir)\$(rel_texdir)\trip
etripdir = etrip

triptest: $(outdir)\etriptex.exe
	$(pltotf) $(tripdir)\trip.pl trip.tfm
	copy $(tripdir)\trip.tex . > nul
	-$(outdir)\etriptex \
		-initialize $(etripoptions) \
		< $(tripdir)\triptest.s3 \
		> nul
	-diff --strip-trailing-cr $(tripdir)\tripin.log trip.log
	-$(outdir)\etriptex \
		-initialize \
		$(etripoptions) \
		< $(tripdir)\triptest.s4 \
		> trip.fot
	-diff --strip-trailing-cr $(tripdir)\trip.fot trip.fot
	-diff --strip-trailing-cr $(tripdir)\trip.log trip.log
	$(dvitype) \
		-out-mode=2 \
		-start-there=*.*.*.*.*.*.*.*.*.* \
		-resolution=7227/100 \
		trip.dvi \
		> trip.typ
	-diff --strip-trailing-cr $(tripdir)\trip.typ trip.typ
	-$(outdir)\etriptex \
		-initialize \
		$(etripoptions) \
		< $(tripdir)\triptest.s3 \
		> nul
	-diff --strip-trailing-cr $(tripdir)\tripin.log trip.log
	-$(outdir)\etriptex \
		-initialize \
		$(etripoptions) \
		< $(tripdir)\triptest.s4 \
		> trip.fot
	-diff --strip-trailing-cr $(tripdir)\trip.fot trip.fot
	-diff --strip-trailing-cr $(tripdir)\trip.log trip.log
	$(dvitype) \
		-out-mode=2 \
		-start-there=*.*.*.*.*.*.*.*.*.* \
		-resolution=7227/100 \
		trip.dvi \
		> trip.typ
	-diff --strip-trailing-cr $(tripdir)\trip.typ trip.typ
	-$(outdir)\etriptex \
		-initialize \
		$(etripoptions) \
		< $(etripdir)\triptest.s3 \
		> nul
	-diff --strip-trailing-cr $(tripdir)\tripin.log trip.log
	-$(outdir)\etriptex \
		-initialize \
		$(etripoptions) \
		< $(tripdir)\triptest.s4 \
		> trip.fot
	-diff --strip-trailing-cr $(tripdir)\trip.fot trip.fot
	-diff --strip-trailing-cr $(tripdir)\trip.log trip.log
	$(dvitype) \
		-out-mode=2 \
		-start-there=*.*.*.*.*.*.*.*.*.* \
		-resolution=7227/100 trip.dvi \
		> trip.typ
	-diff --strip-trailing-cr $(tripdir)\trip.typ trip.typ

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

triptest-clean:
	-del /f 8terminal.tex
	-del /f trip.dvi
	-del /f trip.efmt
	-del /f trip.fot
	-del /f trip.log
	-del /f trip.tex
	-del /f trip.tfm
	-del /f trip.typ
	-del /f tripos.tex

etriptest-clean:
	-del /f 8terminal.tex
	-del /f etrip.dvi
	-del /f etrip.efmt
	-del /f etrip.fot
	-del /f etrip.log
	-del /f etrip.out
	-del /f etrip.tex
	-del /f etrip.tfm
	-del /f etrip.typ
	-del /f etripos.tex
	-del /f etriptexdefs.h
	-del /f tmp.pl

mostlyclean: common-mostlyclean triptest-clean etriptest-clean
	-del /f *.p
	-del /f etex-miktex-all.ch
	-del /f etex-miktex.web
	-del /f etex.cc
	-del /f etex.h
	-del /f etexdefs.h
	-del /f etriptex-miktex.web
	-del /f etriptex.cc
	-del /f etriptex.h
	-del /f texsys.aux
	-del /f triptex-miktex.ch

clean: common-clean mostlyclean
	-del /f *.efmt
	-del /f etex.pool

distclean: common-distclean clean
	-del /f etex-n.web

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: etriptex.cc etex.cc
	$(MAKEDEPEND) -DMIKTEX_ETEX $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\etriptex.obj: etriptex.h etex-miktex.h etexdefs.h etex.rc
$(outdir)\etriptex.obj: etex-version.h
$(outdir)\etriptex.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/miktex.version
$(outdir)\etriptex.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/version.h
$(outdir)\etriptex.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/help.h
$(outdir)\etex.obj: etex.h etex-miktex.h etexdefs.h etex.rc etex-version.h
$(outdir)\etex.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/miktex.version
$(outdir)\etex.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/version.h
$(outdir)\etex.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/help.h
