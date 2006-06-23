## miktex.mak: dvipng
##
## Copyright (C) 2004 Christian Schenk
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

objects = \
	$(outdir)\color.obj \
	$(outdir)\draw.obj \
	$(outdir)\dvi.obj \
	$(outdir)\dvipng.obj \
	$(outdir)\font.obj \
	$(outdir)\misc.obj \
	$(outdir)\papersiz.obj \
	$(outdir)\pk.obj \
	$(outdir)\ppagelist.obj \
	$(outdir)\set.obj \
	$(outdir)\special.obj \
	$(outdir)\vf.obj \
	$(outdir)\wrapper.obj \

sources = \
	color.c \
	draw.c \
	dvi.c \
	dvipng.c \
	font.c \
	misc.c \
	papersiz.c \
	pk.c \
	ppagelist.c \
	set.c \
	special.c \
	vf.c \
	$(wrapper_cpp) \

extra_cdefines = \
	-Dalloca=_alloca \
	-DUSE_MIKTEX_EXIT \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(extra_cdefines) \
		$(extra_cincludes) \
		$(ccopt_output_directory)$(outdir)\ $<

binaries = $(outdir)\dvipng.exe

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
# dvipng
# -----------------------------------------------------------------------------

libs = \
	$(app_lib) \
	$(gd_lib) \
	$(gnu_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(texmf_lib) \

$(outdir)\dvipng.exe: \
			$(outdir) \
			$(objects) \
			$(libs) \
			$(outdir)\dvipng.res \

	$(link) $(lstandard) \
		$(libs) \
		$(linkopt_output_file)$@ \
		$(objects) \
		$(outdir)\dvipng.res
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\dvipng.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		dvipng-version.h \
		dvipng.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ dvipng.rc

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
	$(MAKEDEPEND) $(extra_cincludes) $(extra_cdefines) $(sources)
	$(fixdep)


# DO NOT DELETE

$(outdir)\color.obj: dvipng.h config.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\color.obj: ./inttypes.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\color.obj: commands.h
$(outdir)\draw.obj: dvipng.h config.h
$(outdir)\draw.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\draw.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\draw.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\draw.obj: ./inttypes.h
$(outdir)\draw.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\draw.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\draw.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\draw.obj: commands.h
$(outdir)\dvi.obj: dvipng.h config.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\dvi.obj: ./inttypes.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\dvi.obj: commands.h
$(outdir)\dvi.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/gnu-miktex.h
$(outdir)\dvipng.obj: dvipng.h config.h
$(outdir)\dvipng.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\dvipng.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\dvipng.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\dvipng.obj: ./inttypes.h
$(outdir)\dvipng.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\dvipng.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\dvipng.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\dvipng.obj: commands.h
$(outdir)\font.obj: dvipng.h config.h
$(outdir)\font.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\font.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\font.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\font.obj: ./inttypes.h
$(outdir)\font.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\font.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\font.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\font.obj: commands.h
$(outdir)\misc.obj: dvipng.h config.h
$(outdir)\misc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\misc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\misc.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\misc.obj: ./inttypes.h
$(outdir)\misc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\misc.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\misc.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\misc.obj: commands.h
$(outdir)\papersiz.obj: dvipng.h config.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\papersiz.obj: ./inttypes.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\papersiz.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\papersiz.obj: commands.h
$(outdir)\pk.obj: dvipng.h config.h
$(outdir)\pk.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\pk.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\pk.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\pk.obj: ./inttypes.h
$(outdir)\pk.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\pk.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\pk.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\pk.obj: commands.h
$(outdir)\ppagelist.obj: dvipng.h config.h
$(outdir)\ppagelist.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\ppagelist.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\ppagelist.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\ppagelist.obj: ./inttypes.h
$(outdir)\ppagelist.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\ppagelist.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\ppagelist.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\ppagelist.obj: commands.h
$(outdir)\set.obj: dvipng.h config.h
$(outdir)\set.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\set.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\set.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\set.obj: ./inttypes.h
$(outdir)\set.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\set.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\set.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\set.obj: commands.h
$(outdir)\special.obj: dvipng.h config.h
$(outdir)\special.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\special.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\special.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\special.obj: ./inttypes.h
$(outdir)\special.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\special.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\special.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\special.obj: commands.h
$(outdir)\vf.obj: dvipng.h config.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\vf.obj: ./inttypes.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/kpathsea.h
$(outdir)\vf.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\vf.obj: commands.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\wrapper.obj: ./inttypes.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
