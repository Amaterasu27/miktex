## miktex.mak: tex4ht
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

!include <miktex.inc>

defines = \
	-DANSI \
	-DCDECL=__cdecl \
	-DDOS \
	-DDOS_C \
	-DHAVE_IO_H \
	-DHAVE_STRING_H \
	-DKPATHSEA \
	-DPLATFORM=\"Windows\" \
	-DWIN32 \
	-Dmain=Main \

includes = \
	$(ccinc_libkps) \

sources = \
	tex4ht.c \

.c{$(outdir)\}.obj:
	$(cc) $(cppstandard) \
		$(ccopt_output_file)$@ \
		$(defines) \
		$(includes) \
		$<

binaries = \
	$(outdir)\htcmd.exe \
	$(outdir)\t4ht.exe \
	$(outdir)\tex4ht.exe \

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# -----------------------------------------------------------------------------
# tex4ht
# -----------------------------------------------------------------------------

tex4ht_objects = \
	$(app_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\tex4ht.obj \
	$(outdir)\tex4ht.res \
	$(outdir)\wrapper.obj \
	$(texmf_lib) \

$(outdir)\tex4ht.exe: \
			$(outdir) \
			$(tex4ht_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(tex4ht_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\tex4ht.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			tex4ht-version.h \
			tex4ht.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ tex4ht.rc

# -----------------------------------------------------------------------------
# t4ht
# -----------------------------------------------------------------------------

t4ht_objects = \
	$(app_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\t4ht.obj \
	$(outdir)\t4ht.res \
	$(outdir)\wrapper.obj \
	$(texmf_lib) \

$(outdir)\t4ht.exe: \
			$(outdir) \
			$(t4ht_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(t4ht_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\t4ht.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			t4ht-version.h \
			t4ht.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ t4ht.rc

# -----------------------------------------------------------------------------
# htcmd
# -----------------------------------------------------------------------------

htcmd_objects = \
	$(app_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\htcmd.obj \
	$(outdir)\htcmd.res \
	$(outdir)\wrapper.obj \
	$(texmf_lib) \

$(outdir)\htcmd.exe: \
			$(outdir) \
			$(htcmd_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(htcmd_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\htcmd.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			htcmd-version.h \
			htcmd.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ htcmd.rc

# -----------------------------------------------------------------------------
# wrapper.obj
# -----------------------------------------------------------------------------

$(outdir)\wrapper.obj: $(wrapper_cpp)
	$(cc) $(cppstandard_aeh) \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_output_file)$@ \
		$(ccopt_select_fastcall) \
		-DAPPNAME=\"TEX4HT\" \
		-DCPLUSPLUSMAIN \
		-DDISABLE_INSTALLER \
		$(wrapper_cpp)

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

$(outdir)\tex4ht.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/config.h
$(outdir)\tex4ht.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\tex4ht.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\tex4ht.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\tex4ht.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\tex4ht.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-errno.h
$(outdir)\tex4ht.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-ctype.h
$(outdir)\tex4ht.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-fopen.h
$(outdir)\tex4ht.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-pathmx.h
$(outdir)\tex4ht.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/proginit.h
$(outdir)\tex4ht.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/tex-file.h
$(outdir)\tex4ht.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/tex-make.h
$(outdir)\tex4ht.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/variable.h
