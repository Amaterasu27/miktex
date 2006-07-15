## miktex.mak: pdfetex
## 
## Copyright (C) 2001-2006 Christian Schenk
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

miktex_enable_pgo = 1

!include <miktex.inc>

defines = \
	-DMIKTEX_PDFETEX \
	-DNOGDI \
	-DUSE_MIKTEX_EXIT \
	-DZLIB_DLL \
	-D__WIN32DLL__ \
	-DpdfTeX \
	-DpdfeTeX \

includes = \
	-I$(miktexsrcdir:\=/)/$(rel_etexdir) \
	-I$(miktexsrcdir:\=/)/$(rel_pdftexdir) \
	-I$(miktexsrcdir:\=/)/$(rel_pdftexdir)/xpdf \
	-I$(miktexsrcdir:\=/)/$(rel_pdftexdir)/xpdf/fofi \
	-I$(miktexsrcdir:\=/)/$(rel_pdftexdir)/xpdf/goo \
	-I$(miktexsrcdir:\=/)/$(rel_pdftexdir)/xpdf/xpdf \

cloptions = \
	$(cppstandard) \
	$(ccopt_default_char_type_is_unsigned) \
	$(ccopt_enable_function_level_linking) \
	$(ccopt_select_fastcall) \
	$(defines) \
	$(includes) \

formatdir = $(prefix)\miktex\fmt

binaries = \
	$(outdir)\$(pdfetex_dll_name).dll \
	$(outdir)\pdfetex.exe \

pdfetexdata = g_pdfeTeXData

{$(miktexsrcdir)\$(rel_pdftexdir)\}.c{$(outdir)\}.obj:
	$(cc) $(cloptions) \
		$(ccopt_output_directory)$(outdir)\ \
		-DC4PEXTERN=extern \
		-DETEXCLASS=$(etexclass) \
		-DPDFETEXDATA=$(pdfetexdata) \
		-DPDFTEXCLASS=$(pdftexclass) \
		$<

{$(miktexsrcdir)\$(rel_pdftexdir)\}.cc{$(outdir)\}.obj:
	$(cc) $(cloptions) \
		$(ccopt_output_directory)$(outdir)\ \
		-DC4PEXTERN=extern \
		-DETEXCLASS=$(etexclass) \
		-DPDFETEXDATA=$(pdfetexdata) \
		-DPDFTEXCLASS=$(pdftexclass) \
		$<

all: common-all $(outdir)

install: common-install pdfetex.pool $(formatdir) install-binaries
	copy pdfetex.pool $(formatdir)\pdfetex.pool

"$(formatdir)":
	$(mkpath) "$(formatdir)"

check: common-check

pdftexini = $(miktexsrcdir)\$(rel_defaultsdir)\pdftex.defaults

pdftex.defaults.h: $(pdftexini)
	cfg --print-classes $(pdftexini) > $@

# _____________________________________________________________________________
#
# pdfetex0.web:
# _____________________________________________________________________________

tex_web = $(miktexsrcdir)\$(rel_texdir)\tex.web

etex_ch = $(miktexsrcdir)\$(rel_etexdir)\etex.ch

pdftex_ch = $(miktexsrcdir)\$(rel_pdftexdir)\pdftex.ch
hz_ch = $(miktexsrcdir)\$(rel_pdftexdir)\hz.ch
misc_ch =  $(miktexsrcdir)\$(rel_pdftexdir)\misc.ch
vadjust_ch = $(miktexsrcdir)\$(rel_pdftexdir)\vadjust.ch


tex_change_files = \
	$(etex_ch) \
	pdfetex.ch1 \
	$(pdftex_ch) \
	$(hz_ch) \
	$(misc_ch) \
	$(vadjust_ch) \
	pdfetex.ch2 \

pdfetex0.web: $(tex_web) $(tex_change_files)
	$(tie) -m $@ $(tex_web) $(tex_change_files)

# _____________________________________________________________________________
#
# pdfetex.web:
# _____________________________________________________________________________

tex_miktex_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex.ch
tex_mltex_miktex_ch = $(miktexsrcdir)\$(rel_texdir)\mltex-miktex.ch
tex_miktex_hyph_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-hyph.ch
tex_miktex_quiet_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-quiet.ch
tex_miktex_src_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-src.ch
tex_miktex_stat_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-stat.ch
tex_miktex_write18_ch = $(miktexsrcdir)\$(rel_texdir)\tex-miktex-write18.ch

etex_miktex_ch = $(miktexsrcdir)\$(rel_etexdir)\etex-miktex.ch

pdftex_pre_ch = $(miktexsrcdir)\$(rel_pdftexdir)\pdftex-pre.ch
pdftex_post_ch = $(miktexsrcdir)\$(rel_pdftexdir)\pdftex-post.ch

pdftex_miktex_ch = $(miktexsrcdir)\$(rel_pdftexdir)\pdftex-miktex.ch

noligatures_ch = $(miktexsrcdir)\$(rel_pdftexdir)\noligatures.ch
pdf_mem_ch = $(miktexsrcdir)\$(rel_pdftexdir)\pdf_mem.ch
pdfstrcmp_ch = $(miktexsrcdir)\$(rel_pdftexdir)\pdfstrcmp.ch
randoms_ch = $(miktexsrcdir)\$(rel_pdftexdir)\randoms.ch

miktex_tex_change_files = \
	$(tex_mltex_miktex_ch) \
	$(tex_miktex_ch) \
	$(tex_miktex_hyph_ch) \
	$(tex_miktex_quiet_ch) \
	$(tex_miktex_src_ch) \
	$(tex_miktex_stat_ch) \
	$(tex_miktex_write18_ch) \

miktex_etex_change_files = \
	$(etex_miktex_ch) \

pdfetex0_change_files = \
	$(pdftex_pre_ch) \
	$(miktex_tex_change_files) \
	$(miktex_etex_change_files) \
	$(pdftex_post_ch) \
	$(noligatures_ch) \
	$(pdfstrcmp_ch) \
	$(randoms_ch) \
	$(pdftex_miktex_ch) \

pdfetex.web: pdfetex0.web $(pdfetex0_change_files)
	$(tie) -m $@ pdfetex0.web $(pdfetex0_change_files)

# _____________________________________________________________________________
#
# pdeftex
# _____________________________________________________________________________

!ifndef pdfetexexport
pdfetexexport = MiKTeX_pdfeTeX
!endif

pdftexclass = CpdfTeXApp

etexclass = CeTeXApp

pdfetexapp = g_pdfeTeXApp
pdfetexclass = CpdfeTeXApp
pdfetexmain = RunepdfTeX

tex_dyn_sed = $(miktexsrcdir)\$(rel_texdir)\dyn.sed

etex_dyn_sed = $(miktexsrcdir)\$(rel_etexdir)\dyn.sed

pdftex_dyn_sed = $(miktexsrcdir)\$(rel_pdftexdir)\dyn.sed

fofi_lib = $(miktexsrcdir)\$(rel_pdftexdir)\xpdf\fofi\$(outdir)\fofi.lib
goo_lib = $(miktexsrcdir)\$(rel_pdftexdir)\xpdf\goo\$(outdir)\goo.lib
xpdf_lib = $(miktexsrcdir)\$(rel_pdftexdir)\xpdf\xpdf\$(outdir)\xpdf.lib

pdfetex_objects = \
	$(outdir)\avl.obj \
	$(outdir)\avlstuff.obj \
	$(outdir)\epdf.obj \
	$(outdir)\mapfile.obj \
	$(outdir)\papersiz.obj \
	$(outdir)\pdfetex.obj \
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

pdfetex_libs = \
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

$(outdir)\$(pdfetex_dll_name).dll $(outdir)\pdfetex.lib: \
			$(outdir) \
			$(dllbases) \
			$(outdir)\pdfetex.res \
			$(pdfetex_libs) \
			$(pdfetex_objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(pdfetex_dll_name) \
		-delayload:$(md5_dll_name).dll \
		-delayload:$(png_dll_name).dll \
		-delayload:$(zlib_dll_name).dll \
		-export:$(pdfetexexport) \
		-implib:$(outdir)\pdfetex.lib \
		-out:$(outdir)\$(pdfetex_dll_name).dll \
		$(pdfetex_objects) \
		$(pdfetex_libs) \
		$(outdir)\pdfetex.res \
		$(conlibsdll) \
		delayimp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\pdfetex.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			pdfetex-version.h \
			pdfetex.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ pdfetex.rc

$(outdir)\pdfetex.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\pdfetex.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(pdfetexexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\pdfetex.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\pdfetex.obj: pdfetex.cc $(miktexsrcdir)\$(rel_pdftexdir)\pdftex.h
	$(cc) $(cloptions) \
		$(ccopt_output_file)$(outdir)\ \
		-DETEXCLASS=$(etexclass) \
		-DPDFETEXAPP=$(pdfetexapp) \
		-DPDFETEXCLASS=$(pdfetexclass) \
		-DPDFETEXDATA=$(pdfetexdata) \
		-DPDFTEXCLASS=$(pdftexclass) \
		pdfetex.cc

pdfetex.cc pdfetexd.h: \
		$(etex_dyn_sed) \
		$(pdftex_dyn_sed) \
		$(tex_dyn_sed) \
		pdfetex.p \

	$(c4p) \
		$(c4prenames) \
		--auto-exit=10 \
		--chars-are-unsigned \
		--def-filename=pdfetexdefs.h \
		--dll \
		--entry-name=$(pdfetexmain) \
		--header-file=pdfetexd.h \
		--include-filename=pdfetex-miktex.h \
		--one=pdfetex \
		--var-name-prefix=m_ \
		--var-struct=$(pdfetexdata) \
		-C \
		pdfetex.p
	type << >> pdfetex.cc
MIKTEX_DEFINE_WEBAPP(
  $(pdfetexexport),
  $(pdfetexclass),
  $(pdfetexapp),
  $(pdfetexmain),
  $(pdfetexdata))
<<
	sed -f $(tex_dyn_sed) pdfetexd.h > pdfetexd.h.tmp
	sed -f $(pdftex_dyn_sed) pdfetexd.h.tmp > pdfetexd.h
	sed -f $(etex_dyn_sed) pdfetexd.h > pdfetexd.h.tmp
	@del pdfetexd.h
	@move pdfetexd.h.tmp pdfetexd.h
	echo #define pdfeTeX >> pdfetexd.h

pdfetex.p: pdfetex.web
	$(tangle) pdfetex.web nul $@ nul

pdfetex.pool: pdfetex.web
	$(tangle) pdfetex.web nul nul $@

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

mostlyclean: common-mostlyclean
	-del /f *.p
	-del /f pdfetex.cc
	-del /f pdfetex.web
	-del /f pdfetex.web
	-del /f pdfetex0.web
	-del /f pdfetexd.h
	-del /f pdfetexdefs.h
	-del /f texsys.aux

clean: common-clean mostlyclean
	-del /f pdfetex.pool

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean
	if exists pdftex.defaults.h del pdftex.defaults.h

!include <common-dependencies.inc>

sources =	\
		$(miktexsrcdir)\$(rel_pdftexdir)\avl.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\avlstuff.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\macnames.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\mapfile.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\papersiz.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\pkin.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\utils.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\vfpacket.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\writeenc.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\writefont.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\writeimg.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\writejpg.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\writepng.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\writet1.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\writet3.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\writettf.c \
		$(miktexsrcdir)\$(rel_pdftexdir)\writezip.c \
		pdfetex.cc \

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

depend: $(sources)
	$(MAKEDEPEND) $(defines) $(includes) $**
	$(fixdep)


# DO NOT DELETE

$(outdir)\avl.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/avl.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\avlstuff.obj: pdfetexd.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\avlstuff.obj: pdfetex-miktex.h
$(outdir)\avlstuff.obj: pdfetexdefs.h
$(outdir)\avlstuff.obj: pdfetex.rc
$(outdir)\avlstuff.obj: pdfetex-version.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
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
$(outdir)\avlstuff.obj: ./pdftex.defaults.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-vararg.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-proto.h
$(outdir)\avlstuff.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/avl.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\mapfile.obj: pdfetexd.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\mapfile.obj: pdfetex-miktex.h
$(outdir)\mapfile.obj: pdfetexdefs.h
$(outdir)\mapfile.obj: pdfetex.rc
$(outdir)\mapfile.obj: pdfetex-version.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
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
$(outdir)\mapfile.obj: ./pdftex.defaults.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\mapfile.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-memstr.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/avlstuff.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/avl.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/epdf.h
$(outdir)\mapfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-proto.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\papersiz.obj: pdfetexd.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\papersiz.obj: pdfetex-miktex.h
$(outdir)\papersiz.obj: pdfetexdefs.h
$(outdir)\papersiz.obj: pdfetex.rc
$(outdir)\papersiz.obj: pdfetex-version.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
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
$(outdir)\papersiz.obj: ./pdftex.defaults.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\papersiz.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\pkin.obj: pdfetexd.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\pkin.obj: pdfetex-miktex.h
$(outdir)\pkin.obj: pdfetexdefs.h
$(outdir)\pkin.obj: pdfetex.rc
$(outdir)\pkin.obj: pdfetex-version.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
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
$(outdir)\pkin.obj: ./pdftex.defaults.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\pkin.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\pkin.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/regex.h
$(outdir)\utils.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\utils.obj: pdfetexd.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\utils.obj: pdfetex-miktex.h
$(outdir)\utils.obj: pdfetexdefs.h
$(outdir)\utils.obj: pdfetex.rc
$(outdir)\utils.obj: pdfetex-version.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\utils.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
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
$(outdir)\utils.obj: ./pdftex.defaults.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\utils.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\utils.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\utils.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\utils.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-version.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-vararg.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-proto.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-stat.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-fopen.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\vfpacket.obj: pdfetexd.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\vfpacket.obj: pdfetex-miktex.h
$(outdir)\vfpacket.obj: pdfetexdefs.h
$(outdir)\vfpacket.obj: pdfetex.rc
$(outdir)\vfpacket.obj: pdfetex-version.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
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
$(outdir)\vfpacket.obj: ./pdftex.defaults.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\vfpacket.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\writeenc.obj: pdfetexd.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writeenc.obj: pdfetex-miktex.h
$(outdir)\writeenc.obj: pdfetexdefs.h
$(outdir)\writeenc.obj: pdfetex.rc
$(outdir)\writeenc.obj: pdfetex-version.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
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
$(outdir)\writeenc.obj: ./pdftex.defaults.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writeenc.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writeenc.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/avlstuff.h
$(outdir)\writeenc.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/avl.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\writefont.obj: pdfetexd.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writefont.obj: pdfetex-miktex.h
$(outdir)\writefont.obj: pdfetexdefs.h
$(outdir)\writefont.obj: pdfetex.rc
$(outdir)\writefont.obj: pdfetex-version.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
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
$(outdir)\writefont.obj: ./pdftex.defaults.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writefont.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writefont.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\writefont.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/xpdf/goo/gmem.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\writeimg.obj: pdfetexd.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writeimg.obj: pdfetex-miktex.h
$(outdir)\writeimg.obj: pdfetexdefs.h
$(outdir)\writeimg.obj: pdfetex.rc
$(outdir)\writeimg.obj: pdfetex-version.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
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
$(outdir)\writeimg.obj: ./pdftex.defaults.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writeimg.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/image.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/3rd/libpng/png.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/3rd/libpng/pngconf.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\writeimg.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-memstr.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\writejpg.obj: pdfetexd.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writejpg.obj: pdfetex-miktex.h
$(outdir)\writejpg.obj: pdfetexdefs.h
$(outdir)\writejpg.obj: pdfetex.rc
$(outdir)\writejpg.obj: pdfetex-version.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
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
$(outdir)\writejpg.obj: ./pdftex.defaults.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writejpg.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/image.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/3rd/libpng/png.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\writejpg.obj: $(miktexsrcdir)/Libraries/3rd/libpng/pngconf.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\writepng.obj: pdfetexd.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writepng.obj: pdfetex-miktex.h
$(outdir)\writepng.obj: pdfetexdefs.h
$(outdir)\writepng.obj: pdfetex.rc
$(outdir)\writepng.obj: pdfetex-version.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
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
$(outdir)\writepng.obj: ./pdftex.defaults.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writepng.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/image.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/3rd/libpng/png.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\writepng.obj: $(miktexsrcdir)/Libraries/3rd/libpng/pngconf.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\writet1.obj: pdfetexd.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writet1.obj: pdfetex-miktex.h
$(outdir)\writet1.obj: pdfetexdefs.h
$(outdir)\writet1.obj: pdfetex.rc
$(outdir)\writet1.obj: pdfetex-version.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
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
$(outdir)\writet1.obj: ./pdftex.defaults.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writet1.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-vararg.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-proto.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\writet3.obj: pdfetexd.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writet3.obj: pdfetex-miktex.h
$(outdir)\writet3.obj: pdfetexdefs.h
$(outdir)\writet3.obj: pdfetex.rc
$(outdir)\writet3.obj: pdfetex-version.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
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
$(outdir)\writet3.obj: ./pdftex.defaults.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writet3.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/tex-glyph.h
$(outdir)\writet3.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/magstep.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\writettf.obj: pdfetexd.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writettf.obj: pdfetex-miktex.h
$(outdir)\writettf.obj: pdfetexdefs.h
$(outdir)\writettf.obj: pdfetex.rc
$(outdir)\writettf.obj: pdfetex-version.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
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
$(outdir)\writettf.obj: ./pdftex.defaults.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writettf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writettf.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/writettf.h
$(outdir)\writettf.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/macnames.c
$(outdir)\writezip.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\writezip.obj: pdfetexd.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\writezip.obj: pdfetex-miktex.h
$(outdir)\writezip.obj: pdfetexdefs.h
$(outdir)\writezip.obj: pdfetex.rc
$(outdir)\writezip.obj: pdfetex-version.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
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
$(outdir)\writezip.obj: ./pdftex.defaults.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\writezip.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\writezip.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\pdfetex.obj: pdfetexd.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\pdfetex.obj: pdfetex-miktex.h pdfetexdefs.h pdfetex.rc
$(outdir)\pdfetex.obj: pdfetex-version.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex-miktex.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.defaults.h
$(outdir)\pdfetex.obj: ./pdftex.defaults.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/pdftex.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexlib.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Programs\TeXAndFriends\pdftex/ptexmac.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Programs\TeXAndFriends\etex/etex-miktex.h
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.inl
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.inl
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.inl
$(outdir)\pdfetex.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
