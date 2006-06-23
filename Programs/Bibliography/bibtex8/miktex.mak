## miktex.mak: bibtex8
## 
## Copyright (C) 1996-2005 Christian Schenk
## 
## This file is part of BibTeX8.
## 
## BibTeX8 is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
## 
## BibTeX8 is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with BibTeX8; if not, write to the Free Software Foundation,
## 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

!include <miktex.inc>

extra_defines = \
	-DKPATHSEA \
	-DSUPPORT_8BIT \

.c{$(outdir)\}.obj:
	$(cc) $(cppstandard) \
		$(ccopt_default_char_type_is_unsigned) \
		$(ccopt_output_directory)$(outdir)\ \
		$(extra_defines) \
		-Dmain=Main \
		$<

objects = \
	$(outdir)\bibtex.obj \
	$(outdir)\bibtex-1.obj \
	$(outdir)\bibtex-2.obj \
	$(outdir)\bibtex-3.obj \
	$(outdir)\bibtex-4.obj \
	$(outdir)\utils.obj \
	$(outdir)\wrapper.obj \

binaries = $(outdir)\bibtex8.exe

all: common-all $(binaries)

bibtex_objects = \
	$(app_lib) \
	$(gnu_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(objects) \
	$(outdir)\bibtex8.res \

$(outdir)\wrapper.obj: $(wrapper_cpp)
	$(cc) $(cppstandard_aeh) \
		-DCPLUSPLUSMAIN \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_select_fastcall) \
		$(ccopt_output_file)$(outdir)\ \
		$(wrapper_cpp)

$(outdir)\bibtex8.exe: \
			$(outdir) \
			$(bibtex_objects)
	$(link) $(lstandard)\
		$(linkopt_output_file)$@ \
		$(bibtex_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\bibtex8.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			bibtex8-version.h \
			bibtex8.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ bibtex8.rc

install: common-install install-binaries

check: common-check

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend: bibtex.c bibtex-1.c bibtex-2.c bibtex-3.c bibtex-4.c utils.c
	$(MAKEDEPEND) $(extra_defines) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\bibtex.obj: sysdep.h bibtex.h datatype.h gblprocs.h gblvars.h utils.h
$(outdir)\bibtex.obj: version.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/config.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\bibtex.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/progname.h
$(outdir)\bibtex-1.obj: sysdep.h bibtex.h datatype.h gblprocs.h gblvars.h utils.h
$(outdir)\bibtex-1.obj: version.h
$(outdir)\bibtex-2.obj: sysdep.h bibtex.h datatype.h gblprocs.h gblvars.h utils.h
$(outdir)\bibtex-2.obj: version.h
$(outdir)\bibtex-3.obj: sysdep.h bibtex.h datatype.h gblprocs.h gblvars.h utils.h
$(outdir)\bibtex-3.obj: version.h
$(outdir)\bibtex-4.obj: sysdep.h bibtex.h datatype.h gblprocs.h gblvars.h utils.h
$(outdir)\bibtex-4.obj: version.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/config.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/miktex/kpsemu.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/c-fopen.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/tex-file.h
$(outdir)\utils.obj: $(miktexsrcdir)/Libraries/MiKTeX/KPathSeaEmulation/include/kpathsea/lib.h
$(outdir)\utils.obj: sysdep.h bibtex.h datatype.h gblprocs.h gblvars.h utils.h
$(outdir)\utils.obj: version.h
