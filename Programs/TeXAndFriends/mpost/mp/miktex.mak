## miktex.mak: metapost/mp
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

!ifndef metapostexport
metapostexport = MiKTeX_METAPOST
!endif

metapostclass = CMetaPostApp
metapostmain = RunMetaPost
metapostapp = g_MetaPostApp
metapostdata = g_MetaPostData

cloptions = \
	$(cppstandard) \
	-DMIKTEX_METAPOST \
	-DMETAPOSTCLASS=$(metapostclass) \
	-DMETAPOSTAPP=$(metapostapp) \
	-DMETAPOSTDATA=$(metapostdata) \

.cc{$(outdir)\}.obj:
	$(cc) $(cloptions) $(ccopt_output_directory)$(outdir)\ $<

memdir = $(prefix)\miktex\mem

binaries = $(outdir)\$(mp_dll_name).dll \
	$(outdir)\mp.exe \
	$(outdir)\inimp.exe \
	$(outdir)\virmp.exe \
	$(outdir)\mpost.exe \
	$(outdir)\inimpost.exe \
	$(outdir)\virmpost.exe \

all: common-all

$(outdir)\virmp.exe: $(outdir) $(outdir)\mp.exe
	copy $(outdir)\mp.exe $@

$(outdir)\inimp.exe: $(outdir) $(outdir)\mp.exe
	copy $(outdir)\mp.exe $@

$(outdir)\mpost.exe: $(outdir) $(outdir)\mp.exe
	copy $(outdir)\mp.exe $@

$(outdir)\virmpost.exe: $(outdir) $(outdir)\mp.exe
	copy $(outdir)\mp.exe $@

$(outdir)\inimpost.exe: $(outdir) $(outdir)\mp.exe
	copy $(outdir)\mp.exe $@

install: common-install install2 traptest

install2: mp.pool $(memdir) install-binaries
	copy mp.pool $(memdir)
	$(initexmf) --add-file $(memdir)\mp.pool

$(memdir):
	$(mkpath) $(memdir)

check: common-check
	$(pushd) test & $(MAKE) -f $(miktex_mak) & $(popd)

# _____________________________________________________________________________
#
# metapost
# _____________________________________________________________________________

mp_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\mp.obj \
	$(outdir)\mp.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(mp_dll_name).dll $(outdir)\mp.lib: \
			$(outdir) \
			$(dllbases) \
			$(mp_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(mp_dll_name) \
		-export:$(metapostexport) \
		-implib:$(outdir)\mp.lib \
		-out:$(outdir)\$(mp_dll_name).dll \
		$(mp_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\mp.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			mp-version.h \
			mp.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ mp.rc

$(outdir)\mp.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\mp.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(metapostexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\mp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\mp.obj: mp.cc mp.h
	$(cc) $(cloptions) $(ccopt_output_directory)$(outdir)\ mp.cc

mp.cc mp.h: mp.p dyn.sed
	$(c4p) $(c4prenames) \
		-C \
		--def-filename=mpdefs.h \
		--dll \
		--entry-name=$(metapostmain) \
		--include-filename=mp-miktex.h \
		--one=mp \
		--var-name-prefix=m_ \
		--var-struct=$(metapostdata) \
		mp.p
	type << >> mp.cc
MIKTEX_DEFINE_WEBAPP(
  $(metapostexport),
  $(metapostclass),
  $(metapostapp),
  $(metapostmain),
  $(metapostdata))
<<
	sed -f dyn.sed mp.h > mp.h.new
	del mp.h
	move mp.h.new mp.h

mp.p: mp.web mp-miktex.ch
	$(tangle) mp.web mp-miktex.ch mp.p nul

mp.pool: mp-miktex.ch mp.web
	$(tangle) mp.web mp-miktex.ch nul $@

# _____________________________________________________________________________
#
# trapmp
# _____________________________________________________________________________

trapmp_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\trapmp.obj \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\trapmp.dll $(outdir)\trapmp.lib: \
			$(outdir) \
			$(trapmp_obj) \

	$(link) $(dlllstandard) \
		-export:$(metapostexport) \
		-implib:$(outdir)\trapmp.lib \
		-out:$(outdir)\trapmp.dll \
		$(trapmp_obj)
	mt -nologo \
		-manifest $(outdir)\trapmp.dll.manifest \
		-outputresource:$(outdir)\trapmp.dll;1

$(outdir)\trapmp.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\trapmp.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(metapostexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\trapmp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\trapmp.obj: trapmp.cc trapmp.h
	$(cc) $(cloptions) \
		$(ccopt_output_directory)$(outdir)\ \
		-DMETAPOSTAPP=$(metapostapp) \
		-DMETAPOSTCLASS=$(metapostclass) \
		-DMETAPOSTDATA=$(metapostdata) \
		-DTRAPMP \
		trapmp.cc

trapmp.cc trapmp.h: trapmp.p dyn.sed
	$(c4p) $(c4prenames) \
		--def-filename=trapmpdefs.h \
		--dll \
		--entry-name=$(metapostmain) \
		--include-filename=mp-miktex.h \
		--one=trapmp \
		--var-name-prefix=m_ \
		--var-struct=$(metapostdata) \
		-C \
		trapmp.p
	type << >> trapmp.cc
MIKTEX_DEFINE_WEBAPP(
  $(metapostexport),
  $(metapostclass),
  $(metapostapp),
  $(metapostmain),
  $(metapostdata))
<<
	sed -f dyn.sed trapmp.h > trapmp.h.new
	del trapmp.h
	move trapmp.h.new trapmp.h

trapmp.p: mp.web trapmp-miktex.ch
	$(tangle) mp.web trapmp-miktex.ch trapmp.p nul

trapmp-miktex.ch: mp-miktex.ch mp-miktex-trap.ch
	$(tie) -c $@ mp.web mp-miktex.ch mp-miktex-trap.ch

# _____________________________________________________________________________
#
# traptest
# _____________________________________________________________________________

trapoptions = \
	-error-line=64 \
	-half-error-line=32 \
	-max-print-line=72 \
	-mem-max=4500 \
	-mem-top=4500 \

traptest: $(outdir)\trapmp.exe
	set MIKTEX_BINDIR=$(bindir)
	$(bindir)\initexmf --add-file=$(bindir)\makempx.exe
	copy trapdir\mtrap.mp .
	$(PLTOTF) trapdir\trapf.pl trapf.tfm
	-$(outdir)\trapmp -initialize $(trapoptions) -strict mtrap
	-diff --strip-trailing-cr trapdir\mtrap.log mtrap.log
	-diff --strip-trailing-cr trapdir\mtrap.0 mtrap.0
	-diff --strip-trailing-cr trapdir\mtrap.1 mtrap.1
	-diff --strip-trailing-cr trapdir\writeo writeo
	-diff --strip-trailing-cr trapdir\writeo.2 writeo.2
	copy trapdir\trap.mp .
	type trapdir\trap.mpx > trap.mpx
	-$(outdir)\trapmp \
		$(trapoptions) \
		-initialize \
		-strict \
		<trapdir\trap1.in \
		>nul
	if exist trapin.log del trapin.log 
	move trap.log  trapin.log
	-diff --strip-trailing-cr trapdir\trapin.log trapin.log
	-$(outdir)\trapmp \
		$(trapoptions) \
		-initialize \
		-strict \
		<trapdir\trap2.in \
		>trap.fot
	-diff --strip-trailing-cr trapdir\trap.log trap.log
	-diff --strip-trailing-cr trapdir\trap.fot trap.fot
	-diff --strip-trailing-cr trapdir\trap.5 trap.5
	-diff --strip-trailing-cr trapdir\trap.6 trap.6
	-diff --strip-trailing-cr trapdir\trap.148 trap.148
	-diff --strip-trailing-cr trapdir\trap.149 trap.149
	-diff --strip-trailing-cr trapdir\trap.150 trap.150
	-diff --strip-trailing-cr trapdir\trap.151 trap.151
	-diff --strip-trailing-cr trapdir\trap.197 trap.197
	-diff --strip-trailing-cr trapdir\trap.200 trap.200
	$(TFTOPL) trap.tfm trap.pl
	-diff --strip-trailing-cr trapdir\trap.pl trap.pl

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

mostlyclean: common-mostlyclean
	-del /f *.p
	-del /f mp.cc
	-del /f mp.h
	-del /f mpdefs.h
	-del /f trap.*
	-del /f trapf.tfm
	-del /f trapin.log
	-del /f trapmp-miktex.ch
	-del /f trapmp.cc
	-del /f trapmp.h
	-del /f trapmpdefs.h

clean: common-clean mostlyclean
	-del /f *.mem
	-del /f mp.pool
	-del /f mtrap.*
	-del /f writeo
	-del /f writeo.2

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean
	-del trap\traptest.log

!include <common-dependencies.inc>

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

depend: trapmp.cc mp.cc
	$(MAKEDEPEND) -DMETAPOST $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\trapmp.obj: trapmp.h mp-miktex.h mpdefs.h mp.rc mp-version.h
$(outdir)\trapmp.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/miktex.version
$(outdir)\trapmp.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/version.h
$(outdir)\trapmp.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/help.h
$(outdir)\mp.obj: mp.h mp-miktex.h mpdefs.h mp.rc mp-version.h
$(outdir)\mp.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/miktex.version
$(outdir)\mp.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/version.h
$(outdir)\mp.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/help.h
