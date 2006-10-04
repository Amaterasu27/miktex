## miktex.mak: tex
## 
## Copyright (C) 1991-2006 Christian Schenk
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
	$(ccopt_default_char_type_is_unsigned) \
	$(ccopt_enable_function_level_linking) \
	$(ccopt_select_fastcall) \
	-DMIKTEX_TEX \

formatdir = $(prefix)\miktex\fmt

binaries = \
	$(outdir)\$(tex_dll_name).dll \
	$(outdir)\initex.exe \
	$(outdir)\tex.exe \
	$(outdir)\virtex.exe \

.cc{$(outdir)\}.obj:
	$(cc) $(cloptions) $(ccopt_output_directory)$(outdir)\ $<

all: common-all

$(outdir)\initex.exe: $(outdir) $(outdir)\tex.exe
	copy $(outdir)\tex.exe $@

$(outdir)\virtex.exe: $(outdir) $(outdir)\tex.exe
	copy $(outdir)\tex.exe $@

install: common-install install2 triptest

install2: tex.pool "$(formatdir)" install-binaries
	copy tex.pool "$(formatdir)"
	$(initexmf) --add-file "$(formatdir)\tex.pool"

"$(formatdir)":
	$(mkpath) "$(formatdir)"

check: common-check
	cd test & $(MAKE) -f $(miktex_mak) & cd ..

# _____________________________________________________________________________
#
# tex
# _____________________________________________________________________________

!ifndef texexport
texexport = MiKTeX_TeX
!endif

texapp = g_TeXApp
texclass = T_e_X
texdata = g_TeXData
texmain = RunTeX

change_file = tex-miktex-all.ch

miktex_change_files = \
	mltex-miktex.ch \
	tex-miktex.ch \
	tex-miktex-hyph.ch \
	tex-miktex-quiet.ch \
	tex-miktex-src.ch \
	tex-miktex-stat.ch \
	tex-miktex-write18.ch \

tex_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\tex.obj \
	$(outdir)\tex.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(tex_dll_name).dll $(outdir)\tex.lib: \
			$(outdir) \
			$(dllbases) \
			$(tex_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(tex_dll_name) \
		-export:$(texexport) \
		-implib:$(outdir)\tex.lib \
		-out:$(outdir)\$(tex_dll_name).dll \
		$(tex_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\tex.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			tex-version.h \
			tex.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ tex.rc

$(outdir)\tex.exe: \
		$(outdir) \
		$(alias_cpp) \
		$(outdir)\tex.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(texexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\tex.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\tex.obj: tex.cc tex.h
	$(cc) $(cloptions) \
		$(ccopt_output_file)$(outdir)\ \
		-DTEXAPP=$(texapp) \
		-DTEXCLASS=$(texclass) \
		-DTEXDATA=$(texdata) \
		tex.cc

tex.cc tex.h: tex.p dyn.sed
	$(c4p) \
		$(c4prenames) \
		--chars-are-unsigned \
		--def-filename=texdefs.h \
		--dll \
		--entry-name=$(texmain) \
		--include-filename=tex-miktex.h \
		--one=tex \
		--var-name-prefix=m_ \
		--var-struct=$(texdata) \
		-C \
		tex.p
	type << >> tex.cc
MIKTEX_DEFINE_WEBAPP(
  $(texexport),
  $(texclass),
  $(texapp),
  $(texmain),
  $(texdata))
<<
	sed -f dyn.sed tex.h > tex.h.new
	del tex.h
	move tex.h.new tex.h

tex.p: tex.web $(change_file)
	$(tangle) tex.web $(change_file) tex.p nul

tex.pool: $(change_file) tex.web
	$(tangle) tex.web $(change_file) nul $@

$(change_file): tex.web $(miktex_change_files)
	$(tie) -c $(change_file) tex.web $(miktex_change_files)

# _____________________________________________________________________________
#
# triptex
# _____________________________________________________________________________


triptex_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\triptex.obj \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\triptex.dll $(outdir)\triptex.lib: \
		$(outdir) \
		$(triptex_obj) \

	$(link) $(dlllstandard) \
		-export:$(texexport) \
		-implib:$(outdir)\triptex.lib \
		-out:$(outdir)\triptex.dll \
		$(triptex_obj)
	mt -nologo \
		-manifest $(outdir)\triptex.dll.manifest \
		-outputresource:$(outdir)\triptex.dll;1

$(outdir)\triptex.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\triptex.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(texexport) \
		$(alias_cpp) \
		$(ccopt_link) $(linkopt_output_file)$@ $(outdir)\triptex.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\triptex.obj: triptex.cc triptex.h
	$(cc) $(cloptions) \
		$(ccopt_output_file)$(outdir)\ \
		-DTEXAPP=$(texapp) \
		-DTEXCLASS=$(texclass) \
		-DTEXDATA=$(texdata) \
		-DTRIPTEX \
		triptex.cc

triptex.cc triptex.h: triptex.p dyn.sed
	$(c4p) \
		$(c4prenames) \
		--chars-are-unsigned \
		--def-filename=triptexdefs.h \
		--dll \
		--entry-name=$(texmain) \
		--include-filename=tex-miktex.h \
		--one=triptex \
		--var-name-prefix=m_ \
		--var-struct=$(texdata) \
		-C \
		triptex.p
		type << >> triptex.cc
MIKTEX_DEFINE_WEBAPP(
  $(texexport),
  $(texclass),
  $(texapp),
  $(texmain),
  $(texdata))
<<
	sed -f dyn.sed triptex.h > triptex.h.new
	del triptex.h
	move triptex.h.new triptex.h

triptex.p: tex.web triptex-miktex.ch
	$(tangle) tex.web triptex-miktex.ch triptex.p nul

triptex-miktex.ch: $(change_file) tex-miktex-trip.ch
	$(tie) -c $@ tex.web $(change_file) tex-miktex-trip.ch

# _____________________________________________________________________________
#
# triptest
# _____________________________________________________________________________

tripoptions = \
	-buf-size=500 \
	-error-line=64 \
	-font-max=75 \
	-font-mem-size=20000 \
	-half-error-line=32 \
	-max-in-open=6 \
	-max-print-line=72 \
	-max-strings=3000 \
	-mem-bot=1 \
	-mem-max=3000 \
	-mem-min=1 \
	-mem-top=3000 \
	-nest-size=40 \
	-param-size=60 \
	-pool-size=32000 \
	-save-size=600 \
	-stack-size=200 \
	-strict \
	-string-vacancies=8000 \
	-trie-op-size=500 \
	-trie-size=8000 \

PATH = $(bindir);$(PATH)

triptest: $(outdir)\triptex.exe
	$(pltotf) trip\trip.pl trip.tfm
	$(tftopl) trip.tfm tmp.pl
	-diff trip\trip.pl tmp.pl
	copy trip\trip.tex . > nul
	-$(outdir)\triptex -initialize $(tripoptions) < trip\triptest.s3 > nul
	-diff trip\tripin.log trip.log
	-$(outdir)\triptex \
		-initialize \
		$(tripoptions) \
		<trip\triptest.s4 \
		> trip.fot
	-diff trip\trip.fot trip.fot
	-diff trip\trip.log trip.log
	$(dvitype) \
		-out-mode=2 \
		-start-there=*.*.*.*.*.*.*.*.*.* \
		-resolution=7227/100 \
		trip.dvi \
		> trip.typ
	-diff trip\trip.typ trip.typ

# _____________________________________________________________________________
#
# tex.dvi
# _____________________________________________________________________________

tex.dvi: tex.tex
	$(tex) tex

tex.tex: $(change_file)
	$(weave) tex.web $(change_file) tex.tex

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

triptest-clean:
	-del /f 8terminal.tex
	-del /f tmp.pl
	-del /f trip.dvi
	-del /f trip.fmt
	-del /f trip.fot
	-del /f trip.log
	-del /f trip.tex
	-del /f trip.tfm
	-del /f trip.typ
	-del /f tripos.tex

mostlyclean: common-mostlyclean triptest-clean
	-del /f tex-miktex-all.ch
	-del /f triptex-miktex.ch
	-del /f triptex.cc
	-del /f triptex.h
	-del /f triptexdefs.h
	-del /f tex.cc
	-del /f tex.h
	-del /f texdefs.h
	-del /f *.p
	-del /f texsys.aux

clean: common-clean mostlyclean
	-del /f tex.pool
	-del /f *.fmt

distclean: common-distclean clean
	-del /f tex-n.web
	-del /f tex.tex
	-del /f tex.dvi

maintainer-clean: common-maintainer-clean distclean
	-del /f trip\triptest.log

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________


!include <common-dependencies.inc>

depend: triptex.cc tex.cc
	$(MAKEDEPEND) -DMIKTEX_TEX $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\triptex.obj: triptex.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\triptex.obj: tex-miktex.h texdefs.h tex.rc tex-version.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\triptex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\tex.obj: tex.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\tex.obj: tex-miktex.h texdefs.h tex.rc tex-version.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\tex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
