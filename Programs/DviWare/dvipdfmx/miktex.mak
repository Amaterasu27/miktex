## miktex.mak: dvipdfmx
##
## Copyright (C) 1998-2004 Christian Schenk
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

#miktex_cc_no_runtime_checks = 1

!include <miktex.inc>

objects = \
	$(outdir)\agl.obj \
	$(outdir)\bmpimage.obj \
	$(outdir)\cff.obj \
	$(outdir)\cff_dict.obj \
	$(outdir)\cid.obj \
	$(outdir)\cidtype0.obj \
	$(outdir)\cidtype2.obj \
	$(outdir)\cmap.obj \
	$(outdir)\cmap_read.obj \
	$(outdir)\cmap_write.obj \
	$(outdir)\cs_type2.obj \
	$(outdir)\dpxconf.obj \
	$(outdir)\dpxcrypt.obj \
	$(outdir)\dpxfile.obj \
	$(outdir)\dpxutil.obj \
	$(outdir)\dvi.obj \
	$(outdir)\dvipdfmx.obj \
	$(outdir)\epdf.obj \
	$(outdir)\error.obj \
	$(outdir)\fontmap.obj \
	$(outdir)\jpegimage.obj \
	$(outdir)\mem.obj \
	$(outdir)\mfileio.obj \
	$(outdir)\mpost.obj \
	$(outdir)\numbers.obj \
	$(outdir)\otl_conf.obj \
	$(outdir)\otl_opt.obj \
	$(outdir)\pdfcolor.obj \
	$(outdir)\pdfdev.obj \
	$(outdir)\pdfdoc.obj \
	$(outdir)\pdfdraw.obj \
	$(outdir)\pdfencoding.obj \
	$(outdir)\pdfencrypt.obj \
	$(outdir)\pdffont.obj \
	$(outdir)\pdfnames.obj \
	$(outdir)\pdfobj.obj \
	$(outdir)\pdfparse.obj \
	$(outdir)\pdfresource.obj \
	$(outdir)\pdfximage.obj \
	$(outdir)\pkfont.obj \
	$(outdir)\pngimage.obj \
	$(outdir)\pst.obj \
	$(outdir)\pst_obj.obj \
	$(outdir)\sfnt.obj \
	$(outdir)\spc_color.obj \
	$(outdir)\spc_dvips.obj \
	$(outdir)\spc_html.obj \
	$(outdir)\spc_misc.obj \
	$(outdir)\spc_pdfm.obj \
	$(outdir)\spc_tpic.obj \
	$(outdir)\spc_util.obj \
	$(outdir)\specials.obj \
	$(outdir)\subfont.obj \
	$(outdir)\t1_char.obj \
	$(outdir)\t1_load.obj \
	$(outdir)\tfm.obj \
	$(outdir)\truetype.obj \
	$(outdir)\tt_aux.obj \
	$(outdir)\tt_cmap.obj \
	$(outdir)\tt_glyf.obj \
	$(outdir)\tt_gsub.obj \
	$(outdir)\tt_post.obj \
	$(outdir)\tt_table.obj \
	$(outdir)\type0.obj \
	$(outdir)\type1.obj \
	$(outdir)\type1c.obj \
	$(outdir)\unicode.obj \
	$(outdir)\vf.obj \
	$(outdir)\wrapper.obj \

sources = \
	agl.c \
	bmpimage.c \
	cff.c \
	cff_dict.c \
	cid.c \
	cidtype0.c \
	cidtype2.c \
	cmap.c \
	cmap_read.c \
	cmap_write.c \
	cs_type2.c \
	dpxconf.c \
	dpxcrypt.c \
	dpxfile.c \
	dpxutil.c \
	dvi.c \
	dvipdfmx.c \
	epdf.c \
	error.c \
	fontmap.c \
	jpegimage.c \
	mem.c \
	mfileio.c \
	mpost.c \
	numbers.c \
	otl_conf.c \
	otl_opt.c \
	pdfcolor.c \
	pdfdev.c \
	pdfdoc.c \
	pdfdraw.c \
	pdfencoding.c \
	pdfencrypt.c \
	pdffont.c \
	pdfnames.c \
	pdfobj.c \
	pdfparse.c \
	pdfresource.c \
	pdfximage.c \
	pkfont.c \
	pngimage.c \
	pst.c \
	pst_obj.c \
	sfnt.c \
	spc_color.c \
	spc_dvips.c \
	spc_html.c \
	spc_misc.c \
	spc_pdfm.c \
	spc_tpic.c \
	spc_util.c \
	specials.c \
	subfont.c \
	t1_char.c \
	t1_load.c \
	tfm.c \
	truetype.c \
	tt_aux.c \
	tt_cmap.c \
	tt_glyf.c \
	tt_gsub.c \
	tt_post.c \
	tt_table.c \
	type0.c \
	type1.c \
	type1c.c \
	unicode.c \
	vf.c \
	$(wrapper_cpp) \

defines = \
	-DCMAP_ADOBE_IDENTITY_UCS2=\"Adobe-Identity-UCS2\" \
	-DHAVE_BASENAME \
	-DHAVE_CONFIG_H \
	-DUSE_MIKTEX_EXIT \
	-Dstrncasecmp=_strnicmp \
	-DMIKTEX_2_4_COMPAT=1

includes = \
	$(ccinc_libgnu) \
	$(ccinc_libkps) \
	$(ccinc_lib) \
	$(ccinc_libpng) \
	$(ccinc_zlib) \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		$(defines) \
		$(includes) \
		$<

binaries = $(outdir)\dvipdfmx.exe

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

$(outdir)\wrapper.obj: $(wrapper_cpp)
	$(cc) $(cppstandard_aeh) \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_select_fastcall) \
		$(ccopt_output_file)$(outdir)\ \
		$(wrapper_cpp)

# -----------------------------------------------------------------------------
# dvipdfmx
# -----------------------------------------------------------------------------

dvipdfmx_link_files = \
	$(app_lib) \
	$(gnu_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(objects) \
	$(outdir)\dvipdfmx.res \
	$(png_lib) \
	$(texmf_lib) \
	$(zlib_lib) \

$(outdir)\dvipdfmx.exe: \
			$(outdir) \
			$(dvipdfmx_link_files) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(dvipdfmx_link_files)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\dvipdfmx.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			dvipdfmx-version.h \
			dvipdfmx.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ dvipdfmx.rc

# -----------------------------------------------------------------------------
# clean-up
# -----------------------------------------------------------------------------

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# -----------------------------------------------------------------------------
# dependencies
# -----------------------------------------------------------------------------

!include <common-dependencies.inc>

depend: $(sources)
	$(MAKEDEPEND) $(defines) $(includes) $(sources)
	$(fixdep)


# DO NOT DELETE

$(outdir)\agl.obj: config.h system.h
$(outdir)\agl.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\agl.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\agl.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\agl.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\agl.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\agl.obj: mem.h error.h mfileio.h numbers.h pdfparse.h pdfobj.h
$(outdir)\agl.obj: dpxutil.h unicode.h agl.h
$(outdir)\bmpimage.obj: config.h system.h
$(outdir)\bmpimage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\bmpimage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\bmpimage.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\bmpimage.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\bmpimage.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\bmpimage.obj: error.h mem.h pdfobj.h bmpimage.h mfileio.h numbers.h
$(outdir)\bmpimage.obj: pdfximage.h pdfdev.h
$(outdir)\cff.obj: mem.h error.h system.h
$(outdir)\cff.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\cff.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\cff.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\cff.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\cff.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\cff.obj: mfileio.h numbers.h cff_limits.h cff_types.h cff_stdstr.h
$(outdir)\cff.obj: cff_dict.h cff.h
$(outdir)\cff_dict.obj: error.h system.h
$(outdir)\cff_dict.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\cff_dict.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\cff_dict.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\cff_dict.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\cff_dict.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\cff_dict.obj: mem.h mfileio.h numbers.h cff_types.h cff_limits.h
$(outdir)\cff_dict.obj: cff_dict.h cff.h
$(outdir)\cid.obj: config.h system.h
$(outdir)\cid.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\cid.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\cid.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\cid.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\cid.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\cid.obj: mem.h error.h dpxutil.h pdfobj.h cidtype0.h cid.h type0.h
$(outdir)\cid.obj: fontmap.h cid_p.h cidtype2.h pdfparse.h numbers.h
$(outdir)\cid.obj: cid_basefont.h
$(outdir)\cidtype0.obj: config.h system.h
$(outdir)\cidtype0.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\cidtype0.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\cidtype0.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\cidtype0.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\cidtype0.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\cidtype0.obj: mem.h error.h dpxfile.h mfileio.h numbers.h pdfobj.h
$(outdir)\cidtype0.obj: dpxutil.h sfnt.h tt_aux.h tt_table.h cff_types.h
$(outdir)\cidtype0.obj: cff_limits.h cff.h cff_dict.h cs_type2.h cmap.h cid.h
$(outdir)\cidtype0.obj: type0.h fontmap.h cid_p.h cidtype0.h unicode.h agl.h
$(outdir)\cidtype0.obj: t1_load.h t1_char.h cmap_write.h
$(outdir)\cidtype2.obj: system.h
$(outdir)\cidtype2.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\cidtype2.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\cidtype2.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\cidtype2.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\cidtype2.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\cidtype2.obj: numbers.h mem.h error.h dpxfile.h mfileio.h dpxutil.h
$(outdir)\cidtype2.obj: pdflimits.h pdfobj.h sfnt.h tt_aux.h tt_glyf.h tt_cmap.h
$(outdir)\cidtype2.obj: tt_table.h tt_gsub.h otl_opt.h cmap.h cid.h type0.h
$(outdir)\cidtype2.obj: fontmap.h cid_p.h cidtype2.h
$(outdir)\cmap.obj: config.h system.h
$(outdir)\cmap.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\cmap.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\cmap.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\cmap.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\cmap.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\cmap.obj: mem.h error.h dpxutil.h cmap_p.h cid.h pdfobj.h type0.h
$(outdir)\cmap.obj: fontmap.h cmap.h cmap_read.h dpxfile.h mfileio.h numbers.h
$(outdir)\cmap_read.obj: system.h
$(outdir)\cmap_read.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\cmap_read.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\cmap_read.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\cmap_read.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\cmap_read.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\cmap_read.obj: mem.h error.h dpxutil.h pst.h cmap_p.h cid.h pdfobj.h
$(outdir)\cmap_read.obj: type0.h fontmap.h cmap.h cmap_read.h mfileio.h numbers.h
$(outdir)\cmap_write.obj: config.h system.h
$(outdir)\cmap_write.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\cmap_write.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\cmap_write.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\cmap_write.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\cmap_write.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\cmap_write.obj: mem.h error.h dpxutil.h pdfobj.h pdfresource.h cmap_p.h
$(outdir)\cmap_write.obj: cid.h type0.h fontmap.h cmap.h cmap_write.h
$(outdir)\cs_type2.obj: error.h system.h
$(outdir)\cs_type2.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\cs_type2.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\cs_type2.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\cs_type2.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\cs_type2.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\cs_type2.obj: cff_types.h cff_limits.h cs_type2.h
$(outdir)\dpxcrypt.obj: config.h dpxcrypt.h
$(outdir)\dpxfile.obj: config.h system.h
$(outdir)\dpxfile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\dpxfile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\dpxfile.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\dpxfile.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\dpxfile.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\dpxfile.obj: mem.h error.h dpxutil.h mfileio.h numbers.h dpxfile.h
$(outdir)\dpxutil.obj: mem.h error.h system.h
$(outdir)\dpxutil.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\dpxutil.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\dpxutil.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\dpxutil.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\dpxutil.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\dpxutil.obj: dpxutil.h
$(outdir)\dvi.obj: config.h system.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\dvi.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\dvi.obj: mem.h error.h mfileio.h numbers.h pdflimits.h pdfdev.h
$(outdir)\dvi.obj: pdfobj.h pdfdoc.h pdfparse.h fontmap.h dvicodes.h tfm.h vf.h
$(outdir)\dvi.obj: subfont.h spc_util.h pdfcolor.h specials.h dvi.h
$(outdir)\dvipdfmx.obj: config.h system.h
$(outdir)\dvipdfmx.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\dvipdfmx.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\dvipdfmx.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\dvipdfmx.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\dvipdfmx.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\dvipdfmx.obj: mem.h dpxfile.h mfileio.h numbers.h dvi.h error.h
$(outdir)\dvipdfmx.obj: pdfdev.h pdfobj.h pdfdoc.h pdfparse.h pdfencrypt.h
$(outdir)\dvipdfmx.obj: spc_tpic.h specials.h spc_util.h pdfcolor.h spc_pdfm.h
$(outdir)\dvipdfmx.obj: psimage.h pdfximage.h mpost.h fontmap.h pdffont.h
$(outdir)\dvipdfmx.obj: pdflimits.h cid.h type0.h
$(outdir)\epdf.obj: config.h system.h
$(outdir)\epdf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\epdf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\epdf.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\epdf.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\epdf.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\epdf.obj: mem.h mfileio.h numbers.h error.h
$(outdir)\epdf.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zlib.h
$(outdir)\epdf.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zconf.h pdfobj.h
$(outdir)\epdf.obj: pdfdev.h pdfximage.h epdf.h
$(outdir)\error.obj: error.h system.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\error.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\fontmap.obj: config.h mem.h error.h system.h
$(outdir)\fontmap.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\fontmap.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\fontmap.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\fontmap.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\fontmap.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\fontmap.obj: dpxfile.h mfileio.h numbers.h pdfparse.h pdfobj.h
$(outdir)\fontmap.obj: dpxutil.h fontmap.h subfont.h
$(outdir)\jpegimage.obj: config.h system.h
$(outdir)\jpegimage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\jpegimage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\jpegimage.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\jpegimage.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\jpegimage.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\jpegimage.obj: error.h mem.h mfileio.h numbers.h pdfobj.h jpegimage.h
$(outdir)\jpegimage.obj: pdfximage.h pdfdev.h pdfcolor.h
$(outdir)\mem.obj: mem.h
$(outdir)\mfileio.obj: system.h
$(outdir)\mfileio.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\mfileio.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\mfileio.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\mfileio.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\mfileio.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\mfileio.obj: mfileio.h numbers.h error.h
$(outdir)\mpost.obj: config.h system.h
$(outdir)\mpost.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\mpost.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\mpost.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\mpost.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\mpost.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\mpost.obj: mem.h error.h mfileio.h numbers.h tfm.h pdfobj.h pdfparse.h
$(outdir)\mpost.obj: pdflimits.h pdfdev.h pdfdoc.h pdfcolor.h pdfdraw.h fontmap.h
$(outdir)\mpost.obj: subfont.h pdfximage.h mpost.h
$(outdir)\numbers.obj: system.h
$(outdir)\numbers.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\numbers.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\numbers.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\numbers.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\numbers.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\numbers.obj: error.h mfileio.h numbers.h
$(outdir)\otl_conf.obj: system.h
$(outdir)\otl_conf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\otl_conf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\otl_conf.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\otl_conf.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\otl_conf.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\otl_conf.obj: error.h mem.h pdfobj.h pdfparse.h numbers.h agl.h
$(outdir)\otl_conf.obj: otl_conf.h mfileio.h
$(outdir)\otl_opt.obj: system.h
$(outdir)\otl_opt.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\otl_opt.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\otl_opt.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\otl_opt.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\otl_opt.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\otl_opt.obj: error.h mem.h mfileio.h numbers.h otl_opt.h
$(outdir)\pdfcolor.obj: config.h system.h
$(outdir)\pdfcolor.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pdfcolor.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pdfcolor.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pdfcolor.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pdfcolor.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pdfcolor.obj: mem.h error.h dpxfile.h mfileio.h numbers.h pdfdoc.h
$(outdir)\pdfcolor.obj: pdfobj.h pdfdev.h colors.h pdfcolor.h dpxcrypt.h
$(outdir)\pdfdev.obj: config.h system.h
$(outdir)\pdfdev.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pdfdev.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pdfdev.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pdfdev.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pdfdev.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pdfdev.obj: mem.h error.h mfileio.h numbers.h pdfdoc.h pdfobj.h
$(outdir)\pdfdev.obj: pdfdev.h pdffont.h fontmap.h pdflimits.h cmap.h cid.h
$(outdir)\pdfdev.obj: type0.h pdfximage.h pdfdraw.h pdfcolor.h
$(outdir)\pdfdoc.obj: config.h system.h
$(outdir)\pdfdoc.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pdfdoc.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pdfdoc.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pdfdoc.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pdfdoc.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pdfdoc.obj: mem.h error.h mfileio.h numbers.h pdfobj.h pdfparse.h
$(outdir)\pdfdoc.obj: pdfnames.h dpxutil.h pdfencrypt.h pdfdev.h pdfdraw.h
$(outdir)\pdfdoc.obj: pdfcolor.h pdfresource.h pdffont.h fontmap.h pdflimits.h
$(outdir)\pdfdoc.obj: pdfximage.h pngimage.h jpegimage.h pdfdoc.h mpost.h
$(outdir)\pdfdraw.obj: config.h system.h
$(outdir)\pdfdraw.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pdfdraw.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pdfdraw.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pdfdraw.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pdfdraw.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pdfdraw.obj: error.h mem.h mfileio.h numbers.h pdfdoc.h pdfobj.h
$(outdir)\pdfdraw.obj: pdfdev.h pdfcolor.h pdfdraw.h
$(outdir)\pdfencoding.obj: config.h mem.h error.h system.h
$(outdir)\pdfencoding.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pdfencoding.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pdfencoding.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pdfencoding.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pdfencoding.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pdfencoding.obj: dpxutil.h pdfparse.h numbers.h pdfobj.h dpxfile.h
$(outdir)\pdfencoding.obj: mfileio.h pdfencoding.h asl_charset.h cid.h type0.h
$(outdir)\pdfencoding.obj: fontmap.h cmap.h cmap_read.h cmap_write.h agl.h
$(outdir)\pdfencrypt.obj: config.h mem.h error.h system.h
$(outdir)\pdfencrypt.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pdfencrypt.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pdfencrypt.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pdfencrypt.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pdfencrypt.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pdfencrypt.obj: pdfobj.h dpxcrypt.h pdfencrypt.h
$(outdir)\pdffont.obj: config.h system.h
$(outdir)\pdffont.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pdffont.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pdffont.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pdffont.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pdffont.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pdffont.obj: mfileio.h numbers.h mem.h error.h pdfobj.h pdfencoding.h
$(outdir)\pdffont.obj: cmap.h cid.h type0.h fontmap.h unicode.h agl.h dpxutil.h
$(outdir)\pdffont.obj: type1.h pdffont.h pdflimits.h type1c.h truetype.h pkfont.h
$(outdir)\pdffont.obj: tt_cmap.h sfnt.h cidtype0.h cid_p.h otl_conf.h
$(outdir)\pdfnames.obj: config.h system.h
$(outdir)\pdfnames.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pdfnames.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pdfnames.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pdfnames.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pdfnames.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pdfnames.obj: mem.h error.h numbers.h dpxutil.h pdfobj.h pdfnames.h
$(outdir)\pdfobj.obj: config.h system.h
$(outdir)\pdfobj.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pdfobj.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pdfobj.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pdfobj.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pdfobj.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pdfobj.obj: mem.h error.h mfileio.h numbers.h pdflimits.h pdfencrypt.h
$(outdir)\pdfobj.obj: pdfparse.h pdfobj.h
$(outdir)\pdfobj.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zlib.h
$(outdir)\pdfobj.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zconf.h
$(outdir)\pdfparse.obj: config.h system.h
$(outdir)\pdfparse.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pdfparse.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pdfparse.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pdfparse.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pdfparse.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pdfparse.obj: mem.h error.h numbers.h mfileio.h pdfobj.h pdfdoc.h
$(outdir)\pdfparse.obj: pdfdev.h pdfparse.h specials.h
$(outdir)\pdfresource.obj: config.h system.h
$(outdir)\pdfresource.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pdfresource.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pdfresource.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pdfresource.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pdfresource.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pdfresource.obj: mem.h error.h dpxutil.h pdfobj.h pdfresource.h
$(outdir)\pdfximage.obj: config.h system.h
$(outdir)\pdfximage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pdfximage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pdfximage.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pdfximage.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pdfximage.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pdfximage.obj: mem.h error.h mfileio.h numbers.h pdflimits.h pdfobj.h
$(outdir)\pdfximage.obj: pdfdoc.h pdfdev.h pdfdraw.h pdfparse.h epdf.h
$(outdir)\pdfximage.obj: pdfximage.h mpost.h psimage.h pngimage.h jpegimage.h
$(outdir)\pdfximage.obj: bmpimage.h
$(outdir)\pkfont.obj: config.h system.h
$(outdir)\pkfont.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pkfont.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pkfont.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pkfont.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pkfont.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pkfont.obj: mem.h error.h dpxfile.h mfileio.h numbers.h pdfobj.h
$(outdir)\pkfont.obj: pdffont.h fontmap.h pdflimits.h pkfont.h tfm.h
$(outdir)\pngimage.obj: config.h system.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pngimage.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pngimage.obj: error.h mem.h pdfcolor.h pdfobj.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries\3rd\libpng/png.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zlib.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries\3rd\zlib/zconf.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries\3rd\libpng/pngconf.h
$(outdir)\pngimage.obj: pngimage.h mfileio.h numbers.h pdfximage.h pdfdev.h
$(outdir)\psimage.obj: config.h system.h
$(outdir)\psimage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\psimage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\psimage.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\psimage.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\psimage.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\psimage.obj: error.h mem.h mfileio.h numbers.h pdfobj.h pdfdev.h epdf.h
$(outdir)\psimage.obj: pdfximage.h psimage.h
$(outdir)\pst.obj: system.h
$(outdir)\pst.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pst.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pst.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pst.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pst.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pst.obj: mem.h error.h dpxutil.h pst_obj.h pst.h
$(outdir)\pst_obj.obj: system.h
$(outdir)\pst_obj.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pst_obj.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pst_obj.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pst_obj.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\pst_obj.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\pst_obj.obj: mem.h error.h dpxutil.h pst.h pst_obj.h
$(outdir)\sfnt.obj: system.h
$(outdir)\sfnt.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\sfnt.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\sfnt.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\sfnt.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\sfnt.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\sfnt.obj: error.h mem.h mfileio.h numbers.h sfnt.h pdfobj.h
$(outdir)\spc_color.obj: config.h system.h
$(outdir)\spc_color.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\spc_color.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\spc_color.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\spc_color.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\spc_color.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\spc_color.obj: mem.h error.h pdfdev.h numbers.h pdfobj.h pdfparse.h
$(outdir)\spc_color.obj: pdfcolor.h specials.h spc_util.h spc_color.h
$(outdir)\spc_dvips.obj: config.h system.h
$(outdir)\spc_dvips.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\spc_dvips.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\spc_dvips.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\spc_dvips.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\spc_dvips.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\spc_dvips.obj: mem.h error.h dpxfile.h mfileio.h numbers.h pdfparse.h
$(outdir)\spc_dvips.obj: pdfobj.h pdfdoc.h pdfdev.h psimage.h pdfximage.h mpost.h
$(outdir)\spc_dvips.obj: pdfdraw.h pdfcolor.h specials.h spc_util.h spc_dvips.h
$(outdir)\spc_html.obj: config.h system.h
$(outdir)\spc_html.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\spc_html.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\spc_html.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\spc_html.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\spc_html.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\spc_html.obj: mem.h error.h dvi.h numbers.h pdfdev.h pdfobj.h
$(outdir)\spc_html.obj: pdfparse.h pdfdraw.h pdfdoc.h specials.h spc_util.h
$(outdir)\spc_html.obj: pdfcolor.h spc_html.h
$(outdir)\spc_misc.obj: config.h system.h
$(outdir)\spc_misc.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\spc_misc.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\spc_misc.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\spc_misc.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\spc_misc.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\spc_misc.obj: mem.h error.h mfileio.h numbers.h pdfparse.h pdfobj.h
$(outdir)\spc_misc.obj: pdfcolor.h pdfdraw.h pdfdev.h pdfximage.h mpost.h
$(outdir)\spc_misc.obj: specials.h spc_util.h spc_misc.h
$(outdir)\spc_pdfm.obj: config.h system.h
$(outdir)\spc_pdfm.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\spc_pdfm.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\spc_pdfm.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\spc_pdfm.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\spc_pdfm.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\spc_pdfm.obj: mem.h error.h mfileio.h numbers.h dvi.h pdfdev.h pdfobj.h
$(outdir)\spc_pdfm.obj: fontmap.h dpxutil.h pdflimits.h pdfparse.h pdfdoc.h
$(outdir)\spc_pdfm.obj: pdfximage.h pdfdraw.h pdfcolor.h specials.h spc_util.h
$(outdir)\spc_pdfm.obj: spc_pdfm.h cmap.h cid.h type0.h
$(outdir)\spc_tpic.obj: config.h system.h
$(outdir)\spc_tpic.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\spc_tpic.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\spc_tpic.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\spc_tpic.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\spc_tpic.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\spc_tpic.obj: mem.h error.h pdfresource.h pdfobj.h pdfparse.h numbers.h
$(outdir)\spc_tpic.obj: pdfdoc.h pdfdev.h pdfcolor.h pdfdraw.h specials.h
$(outdir)\spc_tpic.obj: spc_util.h spc_tpic.h
$(outdir)\spc_util.obj: config.h system.h
$(outdir)\spc_util.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\spc_util.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\spc_util.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\spc_util.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\spc_util.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\spc_util.obj: mem.h error.h pdfdev.h numbers.h pdfobj.h pdfparse.h
$(outdir)\spc_util.obj: pdfcolor.h specials.h spc_util.h
$(outdir)\specials.obj: config.h system.h
$(outdir)\specials.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\specials.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\specials.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\specials.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\specials.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\specials.obj: mem.h error.h numbers.h dvi.h pdfdev.h pdfobj.h
$(outdir)\specials.obj: pdflimits.h pdfparse.h pdfdoc.h pdfnames.h dpxutil.h
$(outdir)\specials.obj: pdfdraw.h spc_pdfm.h specials.h spc_tpic.h spc_html.h
$(outdir)\specials.obj: spc_misc.h spc_color.h spc_dvips.h
$(outdir)\subfont.obj: system.h
$(outdir)\subfont.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\subfont.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\subfont.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\subfont.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\subfont.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\subfont.obj: mem.h error.h mfileio.h numbers.h pdfparse.h pdfobj.h
$(outdir)\subfont.obj: pdflimits.h dpxfile.h subfont.h
$(outdir)\t1crypt.obj: t1crypt.h
$(outdir)\tfm.obj: config.h system.h
$(outdir)\tfm.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\tfm.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\tfm.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\tfm.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\tfm.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\tfm.obj: mem.h mfileio.h numbers.h error.h dpxutil.h tfm.h
$(outdir)\tt_aux.obj: config.h system.h
$(outdir)\tt_aux.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\tt_aux.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\tt_aux.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\tt_aux.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\tt_aux.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\tt_aux.obj: mem.h error.h numbers.h pdfobj.h sfnt.h mfileio.h
$(outdir)\tt_aux.obj: tt_table.h tt_post.h tt_aux.h
$(outdir)\tt_glyf.obj: config.h system.h
$(outdir)\tt_glyf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\tt_glyf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\tt_glyf.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\tt_glyf.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\tt_glyf.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\tt_glyf.obj: mem.h error.h dpxutil.h sfnt.h mfileio.h numbers.h
$(outdir)\tt_glyf.obj: pdfobj.h tt_table.h tt_glyf.h
$(outdir)\tt_cmap.obj: config.h system.h
$(outdir)\tt_cmap.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\tt_cmap.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\tt_cmap.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\tt_cmap.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\tt_cmap.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\tt_cmap.obj: mem.h error.h sfnt.h mfileio.h numbers.h pdfobj.h cmap.h
$(outdir)\tt_cmap.obj: cid.h type0.h fontmap.h cmap_write.h tt_aux.h tt_gsub.h
$(outdir)\tt_cmap.obj: otl_opt.h unicode.h agl.h pdfparse.h pdfresource.h
$(outdir)\tt_cmap.obj: otl_conf.h dpxfile.h dpxutil.h tt_cmap.h tt_table.h
$(outdir)\tt_cmap.obj: cff_types.h cff_dict.h cff.h
$(outdir)\tt_gsub.obj: system.h
$(outdir)\tt_gsub.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\tt_gsub.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\tt_gsub.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\tt_gsub.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\tt_gsub.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\tt_gsub.obj: error.h mem.h mfileio.h numbers.h sfnt.h pdfobj.h
$(outdir)\tt_gsub.obj: otl_opt.h tt_gsub.h
$(outdir)\tt_post.obj: system.h
$(outdir)\tt_post.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\tt_post.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\tt_post.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\tt_post.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\tt_post.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\tt_post.obj: error.h mem.h dpxfile.h mfileio.h numbers.h sfnt.h
$(outdir)\tt_post.obj: pdfobj.h tt_post.h
$(outdir)\tt_table.obj: system.h
$(outdir)\tt_table.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\tt_table.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\tt_table.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\tt_table.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\tt_table.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\tt_table.obj: error.h mem.h mfileio.h numbers.h sfnt.h pdfobj.h
$(outdir)\tt_table.obj: tt_table.h
$(outdir)\truetype.obj: config.h system.h
$(outdir)\truetype.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\truetype.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\truetype.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\truetype.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\truetype.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\truetype.obj: mem.h error.h dpxfile.h mfileio.h numbers.h dpxutil.h
$(outdir)\truetype.obj: pdfobj.h pdfresource.h pdffont.h fontmap.h pdflimits.h
$(outdir)\truetype.obj: pdfencoding.h unicode.h agl.h sfnt.h tt_cmap.h tt_table.h
$(outdir)\truetype.obj: tt_glyf.h tt_post.h tt_gsub.h otl_opt.h tt_aux.h
$(outdir)\truetype.obj: truetype.h
$(outdir)\type0.obj: config.h system.h
$(outdir)\type0.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\type0.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\type0.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\type0.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\type0.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\type0.obj: mem.h error.h dpxfile.h mfileio.h numbers.h pdfobj.h
$(outdir)\type0.obj: fontmap.h cmap.h cid.h type0.h tt_cmap.h sfnt.h cmap_read.h
$(outdir)\type0.obj: cmap_write.h pdfresource.h pdfencoding.h
$(outdir)\t1_char.obj: config.h system.h
$(outdir)\t1_char.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\t1_char.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\t1_char.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\t1_char.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\t1_char.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\t1_char.obj: mfileio.h numbers.h mem.h error.h pdfobj.h pdffont.h
$(outdir)\t1_char.obj: fontmap.h pdflimits.h pdfencoding.h unicode.h agl.h
$(outdir)\t1_char.obj: dpxutil.h t1crypt.h pst_obj.h pst.h cff_limits.h
$(outdir)\t1_char.obj: cff_types.h cff_dict.h cff.h t1_char.h
$(outdir)\t1_load.obj: config.h system.h
$(outdir)\t1_load.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\t1_load.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\t1_load.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\t1_load.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\t1_load.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\t1_load.obj: mfileio.h numbers.h mem.h error.h pdfobj.h pdffont.h
$(outdir)\t1_load.obj: fontmap.h pdflimits.h pdfencoding.h unicode.h agl.h
$(outdir)\t1_load.obj: dpxutil.h t1crypt.h pst_obj.h pst.h cff_limits.h
$(outdir)\t1_load.obj: cff_types.h cff_dict.h cff.h t1_load.h
$(outdir)\type1.obj: config.h system.h
$(outdir)\type1.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\type1.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\type1.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\type1.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\type1.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\type1.obj: mem.h error.h dpxfile.h mfileio.h numbers.h pdfobj.h
$(outdir)\type1.obj: pdffont.h fontmap.h pdflimits.h pdfencoding.h unicode.h
$(outdir)\type1.obj: agl.h dpxutil.h t1crypt.h pst_obj.h pst.h cff_limits.h
$(outdir)\type1.obj: cff_types.h cff_dict.h cff.h t1_load.h t1_char.h type1.h
$(outdir)\type1c.obj: config.h system.h
$(outdir)\type1c.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\type1c.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\type1c.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\type1c.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\type1c.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\type1c.obj: mem.h error.h dpxfile.h mfileio.h numbers.h pdfobj.h
$(outdir)\type1c.obj: pdffont.h fontmap.h pdflimits.h pdfencoding.h unicode.h
$(outdir)\type1c.obj: agl.h dpxutil.h sfnt.h tt_aux.h cff_types.h cff_limits.h
$(outdir)\type1c.obj: cff.h cff_dict.h cs_type2.h type1c.h
$(outdir)\unicode.obj: config.h system.h
$(outdir)\unicode.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\unicode.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\unicode.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\unicode.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\unicode.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\unicode.obj: mem.h error.h dpxutil.h pdfobj.h pdfresource.h
$(outdir)\unicode.obj: pdfencoding.h agl.h unicode.h
$(outdir)\vf.obj: system.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\vf.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\vf.obj: mfileio.h numbers.h pdflimits.h mem.h error.h tfm.h pdfdev.h
$(outdir)\vf.obj: pdfobj.h dvi.h vf.h dvicodes.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries\MiKTeX\PackageManager/include/miktex/mpm.h
