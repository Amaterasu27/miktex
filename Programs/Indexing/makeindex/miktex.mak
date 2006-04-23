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

$(outdir)\genind.obj: mkind.h genind.h
$(outdir)\mkind.obj: mkind.h
$(outdir)\qsort.obj: mkind.h
$(outdir)\scanid.obj: mkind.h scanid.h
$(outdir)\scanst.obj: mkind.h scanst.h
$(outdir)\sortid.obj: mkind.h
