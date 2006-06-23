## miktex.mak: dvipdfm
##
## Copyright (C) 1998-2005 Christian Schenk
## 
## This file is part of dvipdfm.
## 
## dvipdfm is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
## 
## dvipdfm is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with dvipdfm; if not, write to the Free Software Foundation,
## 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#miktex_cc_no_runtime_checks = 1

!include <miktex.inc>

objects =	\
	$(outdir)\colorsp.obj \
	$(outdir)\dvi.obj \
	$(outdir)\dvipdfm.obj \
	$(outdir)\encodings.obj \
	$(outdir)\epdf.obj \
	$(outdir)\htex.obj \
	$(outdir)\jpeg.obj \
	$(outdir)\mem.obj \
	$(outdir)\mfileio.obj \
	$(outdir)\mpost.obj \
	$(outdir)\numbers.obj \
	$(outdir)\pdfdev.obj \
	$(outdir)\pdfdoc.obj \
	$(outdir)\pdfobj.obj \
	$(outdir)\pdfparse.obj \
	$(outdir)\pdfspecial.obj \
	$(outdir)\pkfont.obj \
	$(outdir)\pngimage.obj \
	$(outdir)\psimage.obj \
	$(outdir)\psspecial.obj \
	$(outdir)\t1crypt.obj \
	$(outdir)\tfm.obj \
	$(outdir)\ttf.obj \
	$(outdir)\thumbnail.obj \
	$(outdir)\tpic.obj \
	$(outdir)\type1.obj \
	$(outdir)\vf.obj \
	$(outdir)\wrapper.obj \

ebb_objects = 	\
	$(outdir)\ebb.obj \
	$(outdir)\jpeg.obj \
	$(outdir)\mem.obj \
	$(outdir)\mfileio.obj \
	$(outdir)\numbers.obj \
	$(outdir)\pdfobj.obj \
	$(outdir)\pdfparse.obj \
	$(outdir)\pngimage.obj \
	$(outdir)\wrapper.obj \

sources =	\
	colorsp.c \
	dvi.c \
	dvipdfm.c \
	dvipdft.cpp \
	ebb.c \
	encodings.c \
	epdf.c \
	htex.c \
	jpeg.c \
	mem.c \
	mfileio.c \
	mpost.c \
	numbers.c \
	pdfdev.c \
	pdfdoc.c \
	pdfobj.c \
	pdfparse.c \
	pdfspecial.c \
	pkfont.c \
	pngimage.c \
	psimage.c \
	psspecial.c \
	t1crypt.c \
	tfm.c \
	ttf.c \
	thumbnail.c \
	tpic.c \
	type1.c \
	vf.c \
	$(wrapper_cpp) \

defines = \
	-DHAVE_BASENAME \

includes = \
	$(ccinc_libgnu) \
	$(ccinc_libkps) \
	$(ccinc_lib) \
	$(ccinc_libpng) \
	$(ccinc_zlib) \

.c{$(outdir)\}.obj:
!ifdef xxx
	$(sed) -e "s/^static \(.*\)(/\1(/" $< > _xyz.c
	$(cc) $(cstandard) \
		$(defines) \
		$(includes) \
		$(ccopt_output_file)$@ \
		_xyz.c
	del _xyz.c
!else
	$(cc) $(cstandard) \
		$(defines) \
		$(includes) \
		$(ccopt_output_directory)$(outdir)\ \
		$<
!endif

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) \
		$(defines) \
		$(includes) \
		$(ccopt_output_directory)$(outdir)\ \
		$<

binaries = $(outdir)\dvipdfm.exe $(outdir)\ebb.exe $(outdir)\dvipdft.exe

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
# dvipdfm
# -----------------------------------------------------------------------------

dvipdfm_link_files = \
	$(app_lib) \
	$(gnu_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(objects) \
	$(outdir)\dvipdfm.res \
	$(png_lib) \
	$(texmf_lib) \
	$(zlib_lib) \

$(outdir)\dvipdfm.exe: \
			$(outdir) \
			$(dvipdfm_link_files) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(dvipdfm_link_files)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\dvipdfm.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			dvipdfm-version.h \
			dvipdfm.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ dvipdfm.rc

# -----------------------------------------------------------------------------
# ebb
# -----------------------------------------------------------------------------

ebb_link_files = \
	$(app_lib) \
	$(ebb_objects) \
	$(gnu_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\ebb.res \
	$(png_lib) \
	$(texmf_lib) \
	$(zlib_lib) \

$(outdir)\ebb.exe: \
			$(outdir) \
			$(ebb_link_files) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(ebb_link_files)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\ebb.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			ebb-version.h \
			ebb.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ ebb.rc

# -----------------------------------------------------------------------------
# dvipdft
# -----------------------------------------------------------------------------

dvipdft_link_files = \
	$(app_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\dvipdft.obj \
	$(outdir)\dvipdft.res \
	$(texmf_lib) \

$(outdir)\dvipdft.exe: \
			$(outdir) \
			$(dvipdft_link_files) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(dvipdft_link_files)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\dvipdft.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			dvipdft-version.h \
			dvipdft.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ dvipdft.rc

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
	$(MAKEDEPEND) $(defines) $(includes) $**
	$(fixdep)


# DO NOT DELETE

$(outdir)\colorsp.obj: system.h
$(outdir)\colorsp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\colorsp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\colorsp.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\colorsp.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\colorsp.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\colorsp.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\colorsp.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\colorsp.obj: mem.h pdfdev.h numbers.h pdfobj.h pdfparse.h pdfspecial.h
$(outdir)\colorsp.obj: dvipdfm.h
$(outdir)\dvi.obj: system.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\dvi.obj: mem.h error.h mfileio.h numbers.h dvi.h pdfdev.h pdfobj.h
$(outdir)\dvi.obj: dvicodes.h pdflimits.h pdfdoc.h tfm.h vf.h
$(outdir)\dvipdfm.obj: config.h system.h
$(outdir)\dvipdfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\dvipdfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\dvipdfm.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\dvipdfm.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\dvipdfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\dvipdfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\dvipdfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\dvipdfm.obj: mem.h mfileio.h numbers.h dvi.h error.h pdfdev.h pdfobj.h
$(outdir)\dvipdfm.obj: pdfdoc.h encodings.h type1.h ttf.h colorsp.h pdfparse.h
$(outdir)\dvipdfm.obj: pdfspecial.h vf.h pkfont.h thumbnail.h psimage.h tfm.h
$(outdir)\dvipdft.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\dvipdft.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\dvipdft.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\dvipdft.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\dvipdft.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\ebb.obj: system.h
$(outdir)\ebb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\ebb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\ebb.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\ebb.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\ebb.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\ebb.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\ebb.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\ebb.obj: mem.h mfileio.h numbers.h pdfobj.h jpeg.h pdfspecial.h
$(outdir)\ebb.obj: pdfparse.h config.h pngimage.h
$(outdir)\ebb.obj: $(miktexsrcdir)/Libraries/3rd/libpng/png.h
$(outdir)\ebb.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\ebb.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\ebb.obj: $(miktexsrcdir)/Libraries/3rd/libpng/pngconf.h
$(outdir)\encodings.obj: pdflimits.h pdfobj.h mem.h error.h system.h
$(outdir)\encodings.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\encodings.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\encodings.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\encodings.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\encodings.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\encodings.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\encodings.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\encodings.obj: mfileio.h numbers.h pdfparse.h encodings.h winansi.h
$(outdir)\epdf.obj: system.h
$(outdir)\epdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\epdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\epdf.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\epdf.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\epdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\epdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\epdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\epdf.obj: mem.h mfileio.h numbers.h pdfobj.h pdfdoc.h pdfspecial.h
$(outdir)\epdf.obj: epdf.h
$(outdir)\htex.obj: system.h
$(outdir)\htex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\htex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\htex.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\htex.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\htex.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\htex.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\htex.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\htex.obj: mem.h mfileio.h numbers.h dvi.h error.h pdfdev.h pdfobj.h
$(outdir)\htex.obj: htex.h pdfparse.h pdfdoc.h
$(outdir)\jpeg.obj: system.h
$(outdir)\jpeg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\jpeg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\jpeg.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\jpeg.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\jpeg.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\jpeg.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\jpeg.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\jpeg.obj: mem.h mfileio.h numbers.h dvi.h error.h pdfdev.h pdfobj.h
$(outdir)\jpeg.obj: jpeg.h pdfspecial.h
$(outdir)\mem.obj: mem.h
$(outdir)\mfileio.obj: system.h
$(outdir)\mfileio.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\mfileio.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\mfileio.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\mfileio.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\mfileio.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\mfileio.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\mfileio.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\mfileio.obj: mfileio.h numbers.h error.h
$(outdir)\mpost.obj: system.h
$(outdir)\mpost.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\mpost.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\mpost.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\mpost.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\mpost.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\mpost.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\mpost.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\mpost.obj: mem.h error.h mfileio.h numbers.h dvi.h pdfdev.h pdfobj.h
$(outdir)\mpost.obj: pdfspecial.h pdfparse.h mpost.h pdflimits.h pdfdoc.h
$(outdir)\numbers.obj: system.h
$(outdir)\numbers.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\numbers.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\numbers.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\numbers.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\numbers.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\numbers.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\numbers.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\numbers.obj: error.h mfileio.h numbers.h
$(outdir)\pdfdev.obj: config.h system.h
$(outdir)\pdfdev.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\pdfdev.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\pdfdev.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\pdfdev.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\pdfdev.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\pdfdev.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\pdfdev.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\pdfdev.obj: mem.h error.h mfileio.h numbers.h dvi.h pdfdev.h pdfobj.h
$(outdir)\pdfdev.obj: tfm.h pdfdoc.h type1.h ttf.h pkfont.h pdfspecial.h
$(outdir)\pdfdev.obj: pdfparse.h tpic.h htex.h mpost.h psspecial.h colorsp.h
$(outdir)\pdfdev.obj: pdflimits.h twiddle.h encodings.h colors.h
$(outdir)\pdfdoc.obj: system.h
$(outdir)\pdfdoc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\pdfdoc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\pdfdoc.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\pdfdoc.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\pdfdoc.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\pdfdoc.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\pdfdoc.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\pdfdoc.obj: config.h mem.h error.h mfileio.h numbers.h dvi.h pdfdev.h
$(outdir)\pdfdoc.obj: pdfobj.h pdflimits.h pdfdoc.h pdfspecial.h thumbnail.h
$(outdir)\pdfobj.obj: system.h
$(outdir)\pdfobj.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\pdfobj.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\pdfobj.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\pdfobj.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\pdfobj.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\pdfobj.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\pdfobj.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\pdfobj.obj: config.h mem.h error.h mfileio.h numbers.h pdflimits.h
$(outdir)\pdfobj.obj: pdfobj.h pdfspecial.h pdfparse.h twiddle.h
$(outdir)\pdfobj.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pdfobj.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\pdfparse.obj: system.h
$(outdir)\pdfparse.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\pdfparse.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\pdfparse.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\pdfparse.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\pdfparse.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\pdfparse.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\pdfparse.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\pdfparse.obj: mem.h mfileio.h numbers.h dvi.h error.h pdfdev.h pdfobj.h
$(outdir)\pdfparse.obj: pdfparse.h pdfspecial.h pdfdoc.h
$(outdir)\pdfspecial.obj: system.h
$(outdir)\pdfspecial.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\pdfspecial.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\pdfspecial.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\pdfspecial.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\pdfspecial.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\pdfspecial.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\pdfspecial.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\pdfspecial.obj: mem.h mfileio.h numbers.h dvi.h error.h pdfdev.h
$(outdir)\pdfspecial.obj: pdfobj.h pdflimits.h pdfspecial.h pdfdoc.h pdfparse.h
$(outdir)\pdfspecial.obj: jpeg.h epdf.h mpost.h psimage.h config.h
$(outdir)\pdfspecial.obj: $(miktexsrcdir)/Libraries/3rd/libpng/png.h
$(outdir)\pdfspecial.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pdfspecial.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\pdfspecial.obj: $(miktexsrcdir)/Libraries/3rd/libpng/pngconf.h
$(outdir)\pdfspecial.obj: pngimage.h
$(outdir)\pkfont.obj: system.h
$(outdir)\pkfont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\pkfont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\pkfont.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\pkfont.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\pkfont.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\pkfont.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\pkfont.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\pkfont.obj: mem.h error.h mfileio.h numbers.h pkfont.h pdfobj.h
$(outdir)\pkfont.obj: pdflimits.h tfm.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-ctype.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\pngimage.obj: system.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\pngimage.obj: config.h mem.h pdfobj.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries/3rd/libpng/png.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\pngimage.obj: $(miktexsrcdir)/Libraries/3rd/libpng/pngconf.h
$(outdir)\psimage.obj: system.h
$(outdir)\psimage.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\psimage.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\psimage.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\psimage.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\psimage.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\psimage.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\psimage.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\psimage.obj: config.h mem.h mfileio.h numbers.h pdfobj.h psimage.h
$(outdir)\psimage.obj: pdfspecial.h epdf.h
$(outdir)\psspecial.obj: system.h
$(outdir)\psspecial.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\psspecial.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\psspecial.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\psspecial.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\psspecial.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\psspecial.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\psspecial.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\psspecial.obj: mem.h mfileio.h numbers.h psspecial.h pdfparse.h
$(outdir)\psspecial.obj: pdfobj.h pdfspecial.h psimage.h mpost.h pdfdoc.h
$(outdir)\t1crypt.obj: t1crypt.h
$(outdir)\tfm.obj: system.h
$(outdir)\tfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\tfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\tfm.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\tfm.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\tfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\tfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\tfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\tfm.obj: mem.h error.h mfileio.h numbers.h pdflimits.h tfm.h config.h
$(outdir)\ttf.obj: config.h ttf.h pdfobj.h system.h
$(outdir)\ttf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\ttf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\ttf.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\ttf.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\ttf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\ttf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\ttf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\ttf.obj: mem.h error.h mfileio.h numbers.h tfm.h pdflimits.h twiddle.h
$(outdir)\ttf.obj: encodings.h macglyphs.h
$(outdir)\thumbnail.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-ctype.h
$(outdir)\thumbnail.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\thumbnail.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\thumbnail.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\thumbnail.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\thumbnail.obj: system.h
$(outdir)\thumbnail.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\thumbnail.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\thumbnail.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\thumbnail.obj: config.h mfileio.h numbers.h mem.h pdfobj.h thumbnail.h
$(outdir)\thumbnail.obj: $(miktexsrcdir)/Libraries/3rd/libpng/png.h
$(outdir)\thumbnail.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\thumbnail.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\thumbnail.obj: $(miktexsrcdir)/Libraries/3rd/libpng/pngconf.h
$(outdir)\thumbnail.obj: pngimage.h
$(outdir)\tpic.obj: system.h
$(outdir)\tpic.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\tpic.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\tpic.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\tpic.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\tpic.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\tpic.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\tpic.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\tpic.obj: mem.h mfileio.h numbers.h tpic.h pdfparse.h pdfobj.h pdfdoc.h
$(outdir)\tpic.obj: pdfdev.h dvi.h error.h
$(outdir)\type1.obj: system.h
$(outdir)\type1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\type1.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\type1.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\type1.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\type1.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\type1.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\type1.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\type1.obj: mem.h error.h mfileio.h numbers.h pdfobj.h type1.h tfm.h
$(outdir)\type1.obj: pdfparse.h pdflimits.h t1crypt.h twiddle.h encodings.h
$(outdir)\type1.obj: standardenc.h
$(outdir)\vf.obj: system.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-auto.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\vf.obj: mfileio.h numbers.h pdflimits.h mem.h error.h tfm.h pdfdev.h
$(outdir)\vf.obj: pdfobj.h dvi.h vf.h config.h dvicodes.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
