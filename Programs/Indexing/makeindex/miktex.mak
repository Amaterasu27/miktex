## miktex.mak: makeindex/src
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

defines = \
	-DHAVE_CTYPE_H \
	-DHAVE_LOCALE_H \
	-DHAVE_SETLOCALE \
	-DHAVE_STRCHR \
	-DHAVE_STRINGS_H \
	-DHAVE_STRRCHR \
	-DIBM_PC_MICROSOFT \
	-DOS_WIN32 \
	-UDEBUG \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) $(defines) $(ccopt_output_directory)$(outdir)\ $<

objects = \
	$(outdir)\genind.obj \
	$(outdir)\mkind.obj \
	$(outdir)\qsort.obj \
	$(outdir)\scanid.obj \
	$(outdir)\scanst.obj \
	$(outdir)\sortid.obj \
	$(outdir)\wrapper.obj \

binaries = $(outdir)\makeindex.exe

all: common-all

install: common-install install-binaries

check: common-check

$(outdir)\wrapper.obj: $(wrapper_cpp)
	$(cc) $(cppstandard_aeh) \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_select_fastcall) \
		$(ccopt_output_file)$(outdir)\ \
		$(wrapper_cpp)

# _____________________________________________________________________________
#
# makeindex
# _____________________________________________________________________________

makeindex_objects = \
	$(app_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(objects) \
	$(outdir)\makeindex.res \
	$(texmf_lib) \

$(outdir)\makeindex.exe: $(outdir) $(makeindex_objects)
	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(makeindex_objects) \
		$(conlibsdll)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\makeindex.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		makeindex-version.h \
		makeindex.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ makeindex.rc

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: genind.c mkind.c qsort.c scanid.c scanst.c sortid.c
	$(MAKEDEPEND) $(defines) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\genind.obj: mkind.h
$(outdir)\genind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/config.h
$(outdir)\genind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\genind.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\genind.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\genind.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\genind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-limits.h
$(outdir)\genind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-memstr.h
$(outdir)\genind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/magstep.h
$(outdir)\genind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/proginit.h
$(outdir)\genind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/progname.h
$(outdir)\genind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/tex-glyph.h
$(outdir)\genind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/expand.h
$(outdir)\genind.obj: genind.h
$(outdir)\mkind.obj: mkind.h
$(outdir)\mkind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/config.h
$(outdir)\mkind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\mkind.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\mkind.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\mkind.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\mkind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-limits.h
$(outdir)\mkind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-memstr.h
$(outdir)\mkind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/magstep.h
$(outdir)\mkind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/proginit.h
$(outdir)\mkind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/progname.h
$(outdir)\mkind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/tex-glyph.h
$(outdir)\mkind.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/expand.h
$(outdir)\qsort.obj: mkind.h
$(outdir)\qsort.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/config.h
$(outdir)\qsort.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\qsort.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\qsort.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\qsort.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\qsort.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-limits.h
$(outdir)\qsort.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-memstr.h
$(outdir)\qsort.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/magstep.h
$(outdir)\qsort.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/proginit.h
$(outdir)\qsort.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/progname.h
$(outdir)\qsort.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/tex-glyph.h
$(outdir)\qsort.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/expand.h
$(outdir)\scanid.obj: mkind.h
$(outdir)\scanid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/config.h
$(outdir)\scanid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\scanid.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\scanid.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\scanid.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\scanid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-limits.h
$(outdir)\scanid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-memstr.h
$(outdir)\scanid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/magstep.h
$(outdir)\scanid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/proginit.h
$(outdir)\scanid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/progname.h
$(outdir)\scanid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/tex-glyph.h
$(outdir)\scanid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/expand.h
$(outdir)\scanid.obj: scanid.h
$(outdir)\scanst.obj: mkind.h
$(outdir)\scanst.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/config.h
$(outdir)\scanst.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\scanst.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\scanst.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\scanst.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\scanst.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-limits.h
$(outdir)\scanst.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-memstr.h
$(outdir)\scanst.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/magstep.h
$(outdir)\scanst.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/proginit.h
$(outdir)\scanst.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/progname.h
$(outdir)\scanst.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/tex-glyph.h
$(outdir)\scanst.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/expand.h
$(outdir)\scanst.obj: scanst.h
$(outdir)\sortid.obj: mkind.h
$(outdir)\sortid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/config.h
$(outdir)\sortid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\sortid.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\sortid.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\sortid.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\sortid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-limits.h
$(outdir)\sortid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-memstr.h
$(outdir)\sortid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/magstep.h
$(outdir)\sortid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/proginit.h
$(outdir)\sortid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/progname.h
$(outdir)\sortid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/tex-glyph.h
$(outdir)\sortid.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/expand.h
