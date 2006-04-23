## miktex.mak: cjkutils/hbf2gf
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
	-DHAVE_MIKTEX \
	-DUSE_MIKTEX_EXIT \

includes = \
	$(ccinc_libgnu) \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		$(defines) \
		$(includes) \
		$<

binaries = $(outdir)\hbf2gf.exe

all: common-all $(binaries)

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
# hbf2gf
# _____________________________________________________________________________

objects = \
	$(app_lib) \
	$(gnu_lib) \
	$(miktex_lib) \
	$(outdir)\hbf.obj \
	$(outdir)\hbf2gf.obj \
	$(outdir)\hbf2gf.res \
	$(outdir)\wrapper.obj \
	$(texmf_lib) \

$(outdir)\hbf2gf.exe: \
			$(outdir) \
			$(objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\hbf2gf.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			hbf2gf-version.h \
			hbf2gf.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ hbf2gf.rc

hbf2gf.c: hbf2gf.w hbf2gf-miktex.ch
	$(ctangle) hbf2gf hbf2gf-miktex.ch

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean
	-del /f hbf2gf.c

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: hbf2gf.c hbf.c
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\hbf2gf.obj: hbf.h
$(outdir)\hbf.obj: hbf.h
$(outdir)\hbf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\hbf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\hbf.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\hbf.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
