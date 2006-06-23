## miktex.mak: omega/omega
##
## Copyright (C) 1998-2006 Christian Schenk
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

defines = \
	-DMIKTEX_OMEGA \
	-DOMEGADATA=$(omegadata) \
	-Dalloca=_alloca \

includes = \
	$(ccinc_libkps) \

cloptions = \
	$(cppstandard) \
	$(ccopt_enable_function_level_linking) \
	$(ccopt_select_fastcall) \
	$(defines) \
	$(includes) \

formatdir = $(prefix)\miktex\fmt

binaries = \
	$(outdir)\$(omega_dll_name).dll \
	$(outdir)\iniomega.exe \
	$(outdir)\omega.exe \
	$(outdir)\viromega.exe \

omegadata = g_OmegaData

.c{$(outdir)\}.obj:
	$(cc) $(cloptions) $(ccopt_output_directory)$(outdir)\ $<

.cc{$(outdir)\}.obj:
	$(cc) $(cloptions) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(binaries)

$(outdir)\viromega.exe: $(outdir) $(outdir)\omega.exe
	copy $(outdir)\omega.exe $@

$(outdir)\iniomega.exe: $(outdir) $(outdir)\omega.exe
	copy $(outdir)\omega.exe $@

install: common-install $(formatdir) omega.pool install-binaries
	copy omega.pool $(formatdir)\omega.pool

$(formatdir):
	$(mkpath) $(formatdir)

check: common-check

omegaini = $(miktexsrcdir)\$(rel_defaultsdir)\omega.defaults

omega.defaults.h: $(omegaini)
	cfg --print-classes $(omegaini) > $@

# _____________________________________________________________________________
#
# omega
# _____________________________________________________________________________

!ifndef omegaexport
omegaexport = MiKTeX_Omega
!endif

omegaapp = g_OmegaApp
omegaclass = COmegaApp
omegamain = RunOmega

tex_dyn_sed = $(miktexsrcdir)\$(rel_texdir)\dyn.sed
tex_fallback_ch = $(miktexsrcdir)\$(rel_texdir)\tex-fallback.ch
tex_web = $(miktexsrcdir)\$(rel_texdir)\tex.web

omega_dyn_sed = omega_dyn.sed

omega_miktex_all_ch = omega-miktex-all.ch

miktex_changefiles = \
	omega-miktex.ch \
	omega-miktex-fmt.ch \
	omega-miktex-misc.ch \

omfiles = \
	om16bit.ch \
	omstr.ch \
	omfont.ch \
	omchar.ch \
	omfi.ch \
	ompar.ch \
	omocp.ch \
	omfilter.ch \
	omtrans.ch \
	omdir.ch \
	omxml.ch \

omega_obj = \
	$(app_lib) \
	$(gnu_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\omega-miktex.obj \
	$(outdir)\omega.obj \
	$(outdir)\omega.res \
	$(outdir)\omegabis.obj \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(omega_dll_name).dll $(outdir)\omega.lib: \
			$(outdir) \
			$(dllbases) \
			$(omega_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(omega_dll_name) \
		-export:$(omegaexport) \
		-implib:$(outdir)\omega.lib \
		-out:$(outdir)\$(omega_dll_name).dll \
		$(omega_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\omega.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			omega-version.h \
			omega.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ omega.rc

$(outdir)\omega.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\omega.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(omegaexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\omega.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

omega.cc omega.h omegadefs.h: omega.p $(tex_dyn_sed) $(omega_dyn_sed)
	$(c4p) \
		$(c4prenames) \
		--def-filename=omegadefs.h \
		--dll \
		--entry-name=$(omegamain) \
		--include-filename=omega-miktex.h \
		--one=omega \
		--var-name-prefix=m_ \
		--var-struct=$(omegadata) \
		-C \
		omega.p
	type << >> omega.cc
MIKTEX_DEFINE_WEBAPP(
  $(omegaexport),
  $(omegaclass),
  $(omegaapp),
  $(omegamain),
  $(omegadata))
<<
	copy /b $(tex_dyn_sed) + $(omega_dyn_sed) dyn.sed
	sed -f dyn.sed omega.h > omega.h.new
	del omega.h
	del dyn.sed
	move omega.h.new omega.h

$(outdir)\omega.obj: $(outdir) omega.cc
	$(cc) $(cloptions) \
		-DMIKTEX_OMEGA \
		-DOMEGAAPP=$(omegaapp) \
		-DOMEGACLASS=$(omegaclass) \
		-DOMEGADATA=$(omegadata) \
		$(ccopt_output_directory)$(outdir)\ \
		omega.cc

omega.pool omega.p: $(omega_miktex_all_ch) omega.web
	$(otangle) omega.web $(omega_miktex_all_ch) omega.p omega.pool

omega.web: $(tex_web) $(tex_fallback_ch) $(omfiles)
	$(tie) -m omega.web $(tex_web) $(tex_fallback_ch) $(omfiles)

$(omega_miktex_all_ch): omega.web $(miktex_changefiles)
	$(tie) -c $(omega_miktex_all_ch) omega.web $(miktex_changefiles)

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

mostlyclean: common-mostlyclean
	-del /f $(omega_miktex_all_ch)
	-del /f *.p
	-del /f omega.cc
	-del /f omega.h
	-del /f omega.web
	-del /f omegadefs.h

clean: common-clean mostlyclean
	-del /f omega.pool
	-del /f *.fmt

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean
	if exists omega.defaults.h del omega.defaults.h

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: omega.cc omega-miktex.cc omegabis.c
	$(MAKEDEPEND) $(defines) $(includes) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\omega.obj: omega.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\omega.obj: omega-miktex.h omegadefs.h omega.rc omega-version.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\omega.obj: ./omega.defaults.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\omega.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\omega-miktex.obj: omega-miktex.h omegadefs.h omega.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\omega-miktex.obj: omega.rc omega-version.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\omega-miktex.obj: ./omega.defaults.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\omega-miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\omegabis.obj: omega-miktex.h omegadefs.h omega.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\omegabis.obj: omega.rc omega-version.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\omegabis.obj: ./omega.defaults.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\omegabis.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
