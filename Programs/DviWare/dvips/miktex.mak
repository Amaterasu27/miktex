## miktex.mak: dvipsk
## 
## Copyright (C) 1996-2005 Christian Schenk
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

srcdir = .

dvipsvars = \
	-DCONFIGFILE=\"config.ps\" \
	-DCREATIONDATE \
	-DDEFRES=600 \
	-DEMTEX \
	-DHAVE_BMEPS_RUN \
	-DHPS \
	-DKPATHSEA \
	-DMAKE_TEX_PK_BY_DEFAULT \
	-DMIKTEX \
#	-DNO_DEBUG \
	-DNeedFunctionPrototypes \
	-DSHIFTLOWCHARS \
	-DTPIC \
	-DWIN32 \
	-DWRITEBIN=FOPEN_WBIN_MODE \
	-Dpclose=_pclose \
	-Dpopen=_popen \
	-UDOWNLOAD_USING_PDFTEX \

sources = \
	$(srcdir)\afm2tfm.c \
	$(srcdir)\bbox.c \
	$(srcdir)\color.c \
	$(srcdir)\dopage.c \
	$(srcdir)\dosection.c \
	$(srcdir)\dospecial.c \
	$(srcdir)\download.c \
	$(srcdir)\dpicheck.c \
	$(srcdir)\drawPS.c \
	$(srcdir)\dviinput.c \
	$(srcdir)\dvips.c \
	$(srcdir)\emspecial.c \
	$(srcdir)\finclude.c \
	$(srcdir)\fontdef.c \
	$(srcdir)\header.c \
	$(srcdir)\hps.c \
	$(srcdir)\loadfont.c \
	$(srcdir)\makefont.c \
	$(srcdir)\output.c \
	$(srcdir)\papersiz.c \
	$(srcdir)\pprescan.c \
	$(srcdir)\prescan.c \
	$(srcdir)\repack.c \
	$(srcdir)\resident.c \
	$(srcdir)\scalewidth.c \
	$(srcdir)\scanpage.c \
	$(srcdir)\search.c \
	$(srcdir)\skippage.c \
	$(srcdir)\t1part.c \
	$(srcdir)\tfmload.c \
	$(srcdir)\unpack.c \
	$(srcdir)\virtualfont.c \
	$(srcdir)\writet1.c \
	$(wrapper_cpp) \

includes = \
	-I$(srcdir) \
	-I$(bmepslibdir) \

{$(srcdir)}.c{$(outdir)\}.obj:
	$(cc) $(cppstandard) \
		$(includes) \
		$(dvipsvars) \
		$(ccopt_output_directory)$(outdir)\ \
		$<

binaries = $(outdir)\afm2tfm.exe $(outdir)\dvips.exe

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# -----------------------------------------------------------------------------
# dvips.exe
# -----------------------------------------------------------------------------

dvips_objects = \
	$(app_lib) \
	$(bmeps_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\bbox.obj \
	$(outdir)\color.obj \
	$(outdir)\dopage.obj \
	$(outdir)\dosection.obj \
	$(outdir)\dospecial.obj \
	$(outdir)\download.obj \
	$(outdir)\dpicheck.obj \
	$(outdir)\drawPS.obj \
	$(outdir)\dviinput.obj \
	$(outdir)\dvips.obj \
	$(outdir)\dvips.res \
	$(outdir)\emspecial.obj \
	$(outdir)\finclude.obj \
	$(outdir)\fontdef.obj \
	$(outdir)\header.obj \
	$(outdir)\hps.obj \
	$(outdir)\loadfont.obj \
	$(outdir)\makefont.obj \
	$(outdir)\output.obj \
	$(outdir)\papersiz.obj \
	$(outdir)\pprescan.obj \
	$(outdir)\prescan.obj \
	$(outdir)\repack.obj \
	$(outdir)\resident.obj \
	$(outdir)\scalewidth.obj \
	$(outdir)\scanpage.obj \
	$(outdir)\search.obj \
	$(outdir)\skippage.obj \
	$(outdir)\t1part.obj \
	$(outdir)\tfmload.obj \
	$(outdir)\unpack.obj \
	$(outdir)\virtualfont.obj \
	$(outdir)\wrapper.obj \
	$(outdir)\writet1.obj \
	$(texmf_lib) \

$(outdir)\dvips.exe: \
			$(outdir) \
			$(dvips_objects) \

	$(link) $(lstandard) \
		-delayload:$(bmeps_dll_name).dll \
		$(linkopt_output_file)$@ \
		$(dvips_objects) \
		delayimp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\dvips.res: \
			$(outdir) \
			$(srcdir)\dvips.rc \
			$(srcdir)\dvips-version.h \
			$(libdir)\include\miktex\miktex.version \
			$(libdir)\include\miktex\version.h \

	$(rc) $(rcstandard) \
		$(dvipsvars) \
		$(rcopt_output_file)$@ \
		$(srcdir)\dvips.rc

$(outdir)\wrapper.obj: $(wrapper_cpp)
	$(cc) $(cppstandard_aeh) \
		-DCPLUSPLUSMAIN \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_select_fastcall) \
		$(ccopt_output_file)$(outdir)\ \
		$(wrapper_cpp)

# -----------------------------------------------------------------------------
# aft2tfm.exe
# -----------------------------------------------------------------------------

afm2tfm_objects = \
	$(app_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\afm2tfm.obj \
	$(outdir)\afm2tfm.res \
	$(outdir)\wrapper.obj \
	$(texmf_lib) \

$(outdir)\afm2tfm.exe: \
			$(outdir) \
			$(afm2tfm_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(afm2tfm_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\afm2tfm.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			afm2tfm-version.h \
			afm2tfm.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ afm2tfm.rc

# -----------------------------------------------------------------------------
# squeeze.exe
# -----------------------------------------------------------------------------

$(outdir)\squeeze.exe: \
			$(outdir) \
			$(outdir)\squeeze.obj \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(outdir)\squeeze.obj
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# -----------------------------------------------------------------------------
# texc.lpro
# -----------------------------------------------------------------------------

.SUFFIXES: .pro .lpro
.lpro.pro:
	$(outdir)\squeeze <$< >$@

texc.lpro: $(srcdir)\$(miktex_mak) tex.lpro
	sed \
		-e "/% begin code for uncompressed fonts only/,/% end code for uncompressed fonts only/d" \
		-e "/% % here's the alternate code for unpacking compressed fonts/,/% % end of code for unpacking compressed fonts/ s/% / /" \
		tex.lpro \
		> texc.lpro

# -----------------------------------------------------------------------------
# clean-up
# -----------------------------------------------------------------------------

mostlyclean: common-mostlyclean
	-del /f $(outdir)\squeeze.exe

clean: common-clean mostlyclean
	-rmdir /s /q $(outdir)
	-del /f texc.lpro
	-del /f *.pro

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# -----------------------------------------------------------------------------
# dependencies
# -----------------------------------------------------------------------------

!include <common-dependencies.inc>

depend: $(sources)
	$(MAKEDEPEND) $(includes) $(dvipsvars) $(sources)
	$(fixdep)


# DO NOT DELETE

$(outdir)\afm2tfm.obj: config.h ./c-auto.h dvips.h
$(outdir)\afm2tfm.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\afm2tfm.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\afm2tfm.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\afm2tfm.obj: debug.h paths.h
$(outdir)\bbox.obj: dvips.h
$(outdir)\bbox.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\bbox.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\bbox.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\bbox.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\color.obj: dvips.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\color.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\dopage.obj: dvips.h
$(outdir)\dopage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\dopage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\dopage.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\dopage.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\dosection.obj: dvips.h
$(outdir)\dosection.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\dosection.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\dosection.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\dosection.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\dospecial.obj: dvips.h
$(outdir)\dospecial.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\dospecial.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\dospecial.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\dospecial.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\download.obj: dvips.h
$(outdir)\download.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\download.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\download.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\download.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\dpicheck.obj: dvips.h
$(outdir)\dpicheck.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\dpicheck.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\dpicheck.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\dpicheck.obj: config.h ./c-auto.h debug.h paths.h
$(outdir)\drawPS.obj: dvips.h
$(outdir)\drawPS.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\drawPS.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\drawPS.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\drawPS.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\dviinput.obj: dvips.h
$(outdir)\dviinput.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\dviinput.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\dviinput.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\dviinput.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\dvips.obj: dvips.h
$(outdir)\dvips.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\dvips.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\dvips.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\dvips.obj: config.h ./c-auto.h debug.h paths.h
$(outdir)\dvips.obj: C:\work\miktex-2.5\Libraries\3rd\libbmeps/bmeps.h protos.h
$(outdir)\emspecial.obj: dvips.h
$(outdir)\emspecial.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\emspecial.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\emspecial.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\emspecial.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\finclude.obj: dvips.h
$(outdir)\finclude.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\finclude.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\finclude.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\finclude.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\fontdef.obj: dvips.h
$(outdir)\fontdef.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\fontdef.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\fontdef.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\fontdef.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\header.obj: dvips.h
$(outdir)\header.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\header.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\header.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\header.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\hps.obj: dvips.h
$(outdir)\hps.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\hps.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\hps.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\hps.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\loadfont.obj: dvips.h
$(outdir)\loadfont.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\loadfont.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\loadfont.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\loadfont.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\makefont.obj: dvips.h
$(outdir)\makefont.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\makefont.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\makefont.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\makefont.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\output.obj: dvips.h
$(outdir)\output.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\output.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\output.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\output.obj: config.h ./c-auto.h debug.h paths.h
$(outdir)\output.obj: C:\work\miktex-2.5\Libraries\3rd\libbmeps/bmeps.h
$(outdir)\output.obj: protos.h
$(outdir)\papersiz.obj: dvips.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\papersiz.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\pprescan.obj: dvips.h
$(outdir)\pprescan.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\pprescan.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\pprescan.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\pprescan.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\prescan.obj: dvips.h
$(outdir)\prescan.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\prescan.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\prescan.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\prescan.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\repack.obj: dvips.h
$(outdir)\repack.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\repack.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\repack.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\repack.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\resident.obj: dvips.h
$(outdir)\resident.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\resident.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\resident.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\resident.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\scalewidth.obj: dvips.h
$(outdir)\scalewidth.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\scalewidth.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\scalewidth.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\scalewidth.obj: config.h ./c-auto.h debug.h paths.h
$(outdir)\scanpage.obj: dvips.h
$(outdir)\scanpage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\scanpage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\scanpage.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\scanpage.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\search.obj: dvips.h
$(outdir)\search.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\search.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\search.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\search.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\skippage.obj: dvips.h
$(outdir)\skippage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\skippage.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\skippage.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\skippage.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\t1part.obj: dvips.h
$(outdir)\t1part.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\t1part.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\t1part.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\t1part.obj: config.h ./c-auto.h debug.h paths.h t1part.h protos.h
$(outdir)\tfmload.obj: dvips.h
$(outdir)\tfmload.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\tfmload.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\tfmload.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\tfmload.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\unpack.obj: dvips.h
$(outdir)\unpack.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\unpack.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\unpack.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\unpack.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\virtualfont.obj: dvips.h
$(outdir)\virtualfont.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\virtualfont.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\virtualfont.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\virtualfont.obj: config.h ./c-auto.h debug.h paths.h protos.h
$(outdir)\writet1.obj: dvips.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\writet1.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\writet1.obj: config.h ./c-auto.h debug.h paths.h protos.h ptexmac.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries\MiKTeX\PackageManager/include/miktex/mpm.h
