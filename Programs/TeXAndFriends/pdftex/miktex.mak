## miktex.mak: pdftex
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

miktex_cc_buffer_security_check = 1

!include <miktex.inc>

defines = \
	-DKPS_DLL \
	-DMIKTEX_PDFTEX \
	-DNOGDI \
	-DUSE_MIKTEX_EXIT \
	-DZLIB_DLL \
	-D__WIN32DLL__ \
	-DpdfTeX \

includes = \
	-Ixpdf \
	-Ixpdf/goo \
	-Ixpdf/xpdf \

cloptions = \
	$(cppstandard) \
	$(ccopt_default_char_type_is_unsigned) \
	$(ccopt_enable_function_level_linking) \
	$(ccopt_select_fastcall) \
	$(defines) \
	$(includes) \

formatdir = $(prefix)\miktex\fmt

binaries = \
#	$(outdir)\$(pdftex_dll_name).dll \
#	$(outdir)\pdfinitex.exe \
#	$(outdir)\pdftex.exe \
	$(outdir)\pdftosrc.exe \
#	$(outdir)\pdfvirtex.exe \
	$(outdir)\ttf2afm.exe \

pdftexdata = g_pdfTeXData

.c{$(outdir)\}.obj:
	$(cc) $(cloptions) \
		$(ccopt_output_directory)$(outdir)\ \
		-DC4PEXTERN=extern \
		-DPDFTEXDATA=$(pdftexdata) \
		$<

.cc{$(outdir)\}.obj:
	$(cc) $(cloptions) \
		$(ccopt_output_directory)$(outdir)\ \
		-DPDFTEXDATA=$(pdftexdata) \
		$<

all: common-all $(outdir)

$(outdir)\pdfvirtex.exe: $(outdir) $(outdir)\pdftex.exe
	copy $(outdir)\pdftex.exe $@

$(outdir)\pdfinitex.exe: $(outdir) $(outdir)\pdftex.exe
	copy $(outdir)\pdftex.exe $@

install: common-install pdftex.pool "$(formatdir)" install-binaries
	copy pdftex.pool "$(formatdir)"
	$(initexmf) --add-file "$(formatdir)"\pdftex.pool
	pushd xpdf & $(MAKE) -f $(miktex_mak) install & popd

"$(formatdir)":
	$(mkpath) "$(formatdir)"

check: common-check

$(outdir)\wrapper.obj: $(wrapper_cpp)
	$(cc) $(cppstandard_aeh) \
		-DCPLUSPLUSMAIN \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_select_fastcall) \
		$(ccopt_output_file)$(outdir)\ \
		$(wrapper_cpp)

# _____________________________________________________________________________
#
# pdftex0.web:
# _____________________________________________________________________________

tex_web = $(miktexsrcdir)\$(rel_texdir)\tex.web

tex_change_files = \
	pdftex.ch \
	hz.ch \
	misc.ch \
	vadjust.ch \
	pdftex2.ch \

pdftex0.web: $(tex_web) $(tex_change_files)
	$(tie) -m $@ $(tex_web) $(tex_change_files)

# _____________________________________________________________________________
#
# pdftex.web:
# _____________________________________________________________________________

tex_miktex_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-misc.ch
tex_mltex_miktex_ch = $(miktexsrcdir)\$(rel_texdir)\mltex-miktex.ch
tex_miktex_hyph_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-hyph.ch
tex_miktex_quiet_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-quiet.ch
tex_miktex_src_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-src.ch
tex_miktex_stat_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-stat.ch
tex_miktex_write18_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-write18.ch

miktex_tex_change_files = \
	$(tex_mltex_miktex_ch) \
	$(tex_miktex_ch) \
	$(tex_miktex_hyph_ch) \
	$(tex_miktex_quiet_ch) \
	$(tex_miktex_src_ch) \
	$(tex_miktex_stat_ch) \
	$(tex_miktex_write18_ch) \

pdftex0_change_files = \
	pdftex-pre.ch \
	$(miktex_tex_change_files) \
	pdftex-post.ch \
	noligatures.ch \
	pdfstrcmp.ch \
	randoms.ch \

pdftex.web: pdftex0.web $(pdftex0_change_files)
	$(tie) -m $@ pdftex0.web $(pdftex0_change_files)

# _____________________________________________________________________________
#
# pdftex
# _____________________________________________________________________________

!ifndef pdftexexport
pdftexexport = MiKTeX_pdfTeX
!endif

pdftexapp = g_pdfTeXApp
pdftexclass = CpdfTeXApp
pdftexmain = RunpdfTeX

tex_dyn_sed = $(miktexsrcdir)\$(rel_texdir)\dyn.sed

pdftex_dyn_sed = dyn.sed

fofi_lib = xpdf\fofi\$(outdir)\fofi.lib
goo_lib = xpdf\goo\$(outdir)\goo.lib
xpdf_lib = xpdf\xpdf\$(outdir)\xpdf.lib

pdftex_objects = \
	$(outdir)\avl.obj \
	$(outdir)\avlstuff.obj \
	$(outdir)\epdf.obj \
	$(outdir)\mapfile.obj \
	$(outdir)\papersiz.obj \
	$(outdir)\pdftex.obj \
	$(outdir)\pdftoepdf.obj \
	$(outdir)\pkin.obj \
	$(outdir)\utils.obj \
	$(outdir)\vfpacket.obj \
	$(outdir)\writeenc.obj \
	$(outdir)\writefont.obj \
	$(outdir)\writeimg.obj \
	$(outdir)\writejpg.obj \
	$(outdir)\writepng.obj \
	$(outdir)\writet1.obj \
	$(outdir)\writet3.obj \
	$(outdir)\writettf.obj \
	$(outdir)\writezip.obj \

pdftex_libs = \
	$(app_lib) \
	$(fofi_lib) \
	$(gnu_lib) \
	$(goo_lib) \
	$(kps_lib) \
	$(md5_lib) \
	$(miktex_lib) \
	$(png_lib) \
	$(popt_lib) \
	$(regex_lib) \
	$(texmf_lib) \
	$(xpdf_lib) \
	$(zlib_lib) \

$(outdir)\$(pdftex_dll_name).dll $(outdir)\pdftex.lib: \
			$(outdir) \
			$(dllbases) \
			$(outdir)\pdftex.res \
			$(pdftex_libs) \
			$(pdftex_objects) \
			pdftex.prf \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(pdftex_dll_name) \
		-delayload:$(md5_dll_name).dll \
		-delayload:$(png_dll_name).dll \
		-delayload:$(zlib_dll_name).dll \
		-export:$(pdftexexport) \
		-implib:$(outdir)\pdftex.lib \
		-out:$(outdir)\$(pdftex_dll_name).dll \
		$(pdftex_objects) \
		$(pdftex_libs) \
		$(outdir)\pdftex.res \
		delayimp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\pdftex.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			pdftex-version.h \
			pdftex.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ pdftex.rc

$(outdir)\pdftex.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\pdftex.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(pdftexexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\pdftex.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\pdftex.obj: pdftex.cc pdftex.h
	$(cc) $(cloptions) \
		$(ccopt_output_file)$(outdir)\ \
		-DPDFTEXAPP=$(pdftexapp) \
		-DPDFTEXCLASS=$(pdftexclass) \
		-DPDFTEXDATA=$(pdftexdata) \
		pdftex.cc

pdftex.cc pdftexd.h: \
			$(pdftex_dyn_sed) \
			$(tex_dyn_sed) \
			pdftex.p \

	$(c4p) \
		$(c4prenames) \
		--auto-exit=10 \
		--chars-are-unsigned \
		--def-filename=pdftexdefs.h \
		--dll \
		--entry-name=$(pdftexmain) \
		--header-file=pdftexd.h \
		--header-file=pdftexd.h \
		--include-filename=pdftex-miktex.h \
		--one=pdftex \
		--var-name-prefix=m_ \
		--var-struct=$(pdftexdata) \
		-C \
		pdftex.p
	type << >> pdftex.cc
MIKTEX_DEFINE_WEBAPP(
  $(pdftexexport),
  $(pdftexclass),
  $(pdftexapp),
  $(pdftexmain),
  $(pdftexdata))
<<
	sed -f $(tex_dyn_sed) pdftexd.h > pdftexd.h.tmp
	sed -f $(pdftex_dyn_sed) pdftexd.h.tmp > pdftexd.h
	echo #define pdfTeX >> pdftexd.h
	@del pdftexd.h.tmp

pdftex.p: pdftex.web pdftex-miktex.ch
	$(tangle) pdftex.web pdftex-miktex.ch $@ nul

pdftex.pool: pdftex.web pdftex-miktex.ch
	$(tangle) pdftex.web pdftex-miktex.ch nul $@

$(xpdf_lib):
	pushd xpdf\xpdf & $(MAKE) -f $(miktex_mak) & popd

$(goo_lib):
	pushd xpdf\goo & $(MAKE) -f $(miktex_mak) & popd

$(fofi_lib):
	pushd xpdf\fofi & $(MAKE) -f $(miktex_mak) & popd

# _____________________________________________________________________________
#
# ttf2afm
# _____________________________________________________________________________

ttf2afm_objects = \
	$(app_lib) \
	$(gnu_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\ttf2afm.obj \
	$(outdir)\ttf2afm.res \
	$(outdir)\wrapper.obj \
	$(texmf_lib) \

$(outdir)\ttf2afm.exe: \
			$(outdir) \
			$(ttf2afm_objects) \

	$(link) $(lstandard) -out:$@ $(ttf2afm_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\ttf2afm.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		ttf2afm-version.h \
		ttf2afm.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ ttf2afm.rc

$(outdir)\ttf2afm.obj: ttf2afm.c
	$(cc) $(cloptions) \
		-I$(kpslibdir) \
		$(ccopt_output_directory)$(outdir)\ \
		ttf2afm.c

# _____________________________________________________________________________
#
# pdftosrc
# _____________________________________________________________________________

pdftosrc_objects = \
	$(app_lib) \
	$(gnu_lib) \
	$(fofi_lib) \
	$(goo_lib) \
	$(miktex_lib) \
	$(outdir)\pdftosrc.obj \
	$(outdir)\pdftosrc.res \
	$(xpdf_lib) \

$(outdir)\pdftosrc.exe: \
			$(outdir) \
			$(pdftosrc_objects) \

	$(link) $(lstandard) \
		-out:$@ \
		$(pdftosrc_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\pdftosrc.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		pdftosrc-version.h \
		pdftosrc.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ pdftosrc.rc

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

mostlyclean: common-mostlyclean
	-del /f *.p
	-del /f pdftex.cc
	-del /f pdftex.web
	-del /f pdftex0.web
	-del /f pdftexd.h
	-del /f pdftexdefs.h
	-del /f texsys.aux
	pushd xpdf & $(MAKE) -f $(miktex_mak) mostlyclean & popd

clean: common-clean mostlyclean
	-del /f pdftex.pool
	pushd xpdf & $(MAKE) -f $(miktex_mak) clean & popd

distclean: common-distclean clean
	pushd xpdf & $(MAKE) -f $(miktex_mak) distclean & popd

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

sources = \
	avl.c \
	avlstuff.c \
	macnames.c \
	mapfile.c \
	papersiz.c \
	pdftex.cc \
	pdftosrc.cc \
	pdftoepdf.cc \
	pkin.c \
	ttf2afm.c \
	utils.c \
	vfpacket.c \
	writeenc.c \
	writefont.c \
	writeimg.c \
	writejpg.c \
	writepng.c \
	writet1.c \
	writet3.c \
	writettf.c \
	writezip.c \

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

depend: $(sources)
	$(MAKEDEPEND) $(defines) $(includes) -D__cplusplus $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\avl.obj: avl.h
$(outdir)\avlstuff.obj: ptexlib.h pdftexd.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\avlstuff.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\avlstuff.obj: pdftex.defaults.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\avlstuff.obj: pdftex.h ptexmac.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-vararg.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-proto.h
$(outdir)\avlstuff.obj: avl.h
$(outdir)\mapfile.obj: ptexlib.h pdftexd.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\mapfile.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\mapfile.obj: pdftex.defaults.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\mapfile.obj: pdftex.h ptexmac.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-memstr.h
$(outdir)\mapfile.obj: avlstuff.h avl.h epdf.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-proto.h
$(outdir)\papersiz.obj: ptexlib.h pdftexd.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\papersiz.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\papersiz.obj: pdftex.defaults.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\papersiz.obj: pdftex.h ptexmac.h
$(outdir)\pdftex.obj: pdftexd.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\pdftex.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\pdftex.obj: pdftex.defaults.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\pdftex.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\pdftex.obj: pdftex.h ptexlib.h ptexmac.h
$(outdir)\pdftosrc.obj: xpdf/aconf.h xpdf/goo/GString.h xpdf/goo/gmem.h
$(outdir)\pdftosrc.obj: xpdf/goo/gfile.h xpdf/goo/gtypes.h xpdf/xpdf/Object.h
$(outdir)\pdftosrc.obj: xpdf/xpdf/Stream.h xpdf/xpdf/Array.h xpdf/xpdf/Dict.h
$(outdir)\pdftosrc.obj: xpdf/xpdf/XRef.h xpdf/xpdf/Catalog.h xpdf/xpdf/Page.h
$(outdir)\pdftosrc.obj: xpdf/xpdf/GfxFont.h xpdf/xpdf/CharTypes.h
$(outdir)\pdftosrc.obj: xpdf/xpdf/PDFDoc.h xpdf/xpdf/GlobalParams.h
$(outdir)\pdftosrc.obj: xpdf/goo/GMutex.h xpdf/xpdf/Error.h xpdf/xpdf/config.h
$(outdir)\pdftoepdf.obj: xpdf/aconf.h xpdf/goo/GString.h xpdf/goo/gmem.h
$(outdir)\pdftoepdf.obj: xpdf/goo/gfile.h xpdf/goo/gtypes.h xpdf/xpdf/config.h
$(outdir)\pdftoepdf.obj: xpdf/xpdf/Object.h xpdf/xpdf/Stream.h xpdf/xpdf/Array.h
$(outdir)\pdftoepdf.obj: xpdf/xpdf/Dict.h xpdf/xpdf/XRef.h xpdf/xpdf/Catalog.h
$(outdir)\pdftoepdf.obj: xpdf/xpdf/Link.h xpdf/xpdf/Page.h xpdf/xpdf/GfxFont.h
$(outdir)\pdftoepdf.obj: xpdf/xpdf/CharTypes.h xpdf/xpdf/PDFDoc.h
$(outdir)\pdftoepdf.obj: xpdf/xpdf/GlobalParams.h xpdf/goo/GMutex.h
$(outdir)\pdftoepdf.obj: xpdf/xpdf/Error.h epdf.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\pdftoepdf.obj: pdftex-miktex.h pdftexdefs.h pdftexd.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\pdftoepdf.obj: pdftex.rc pdftex-version.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\pdftoepdf.obj: pdftex.defaults.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\pdftoepdf.obj: pdftex.h ptexlib.h ptexmac.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\pdftoepdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-proto.h
$(outdir)\pkin.obj: ptexlib.h pdftexd.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\pkin.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\pkin.obj: pdftex.defaults.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\pkin.obj: pdftex.h ptexmac.h
$(outdir)\ttf2afm.obj: ptexmac.h
$(outdir)\ttf2afm.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\ttf2afm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\ttf2afm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\ttf2afm.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\ttf2afm.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h
$(outdir)\ttf2afm.obj: ./writettf.h macnames.c
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/regex.h ptexlib.h
$(outdir)\utils.obj: pdftexd.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\utils.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\utils.obj: pdftex.defaults.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\utils.obj: pdftex.h ptexmac.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-vararg.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-proto.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-stat.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-fopen.h
$(outdir)\vfpacket.obj: ptexlib.h pdftexd.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\vfpacket.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\vfpacket.obj: pdftex.defaults.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\vfpacket.obj: pdftex.h ptexmac.h
$(outdir)\writeenc.obj: ptexlib.h pdftexd.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writeenc.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\writeenc.obj: pdftex.defaults.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writeenc.obj: pdftex.h ptexmac.h avlstuff.h avl.h
$(outdir)\writefont.obj: ptexlib.h pdftexd.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writefont.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\writefont.obj: pdftex.defaults.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writefont.obj: pdftex.h ptexmac.h xpdf/goo/gmem.h
$(outdir)\writeimg.obj: ptexlib.h pdftexd.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writeimg.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\writeimg.obj: pdftex.defaults.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writeimg.obj: pdftex.h ptexmac.h image.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/3rd/libpng/png.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/3rd/libpng/pngconf.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-memstr.h
$(outdir)\writejpg.obj: ptexlib.h pdftexd.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writejpg.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\writejpg.obj: pdftex.defaults.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writejpg.obj: pdftex.h ptexmac.h image.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/3rd/libpng/png.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/3rd/libpng/pngconf.h
$(outdir)\writepng.obj: ptexlib.h pdftexd.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writepng.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\writepng.obj: pdftex.defaults.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writepng.obj: pdftex.h ptexmac.h image.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/3rd/libpng/png.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/3rd/libpng/pngconf.h
$(outdir)\writet1.obj: ptexlib.h pdftexd.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writet1.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\writet1.obj: pdftex.defaults.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writet1.obj: pdftex.h ptexmac.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-vararg.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-proto.h
$(outdir)\writet3.obj: ptexlib.h pdftexd.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writet3.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\writet3.obj: pdftex.defaults.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writet3.obj: pdftex.h ptexmac.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/tex-glyph.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/magstep.h
$(outdir)\writettf.obj: ptexlib.h pdftexd.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writettf.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\writettf.obj: pdftex.defaults.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writettf.obj: pdftex.h ptexmac.h ./writettf.h macnames.c
$(outdir)\writezip.obj: ptexlib.h pdftexd.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writezip.obj: pdftex-miktex.h pdftexdefs.h pdftex.rc pdftex-version.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\writezip.obj: pdftex.defaults.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writezip.obj: pdftex.h ptexmac.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
