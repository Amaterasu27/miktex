## miktex.mak: texinfo/util
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

binaries = $(outdir)\texindex.exe $(outdir)\texi2dvi.exe

all: common-all

.c{$(outdir)\}.obj:
	$(cc) \
		-I.. \
		$(cstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		-DLOCALEDIR=0 \
		-D_POSIX_VERSION \
		-Dmain=Main \
		-Dxexit=exit \
		-Dxmalloc=malloc \
		-Dxrealloc=realloc \
		$<

install: common-install install-binaries

check: common-check

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

texindex_objects = \
	$(app_lib) \
	$(gnu_lib) \
	$(miktex_lib) \
	$(outdir)\texindex.obj \
	$(outdir)\texindex.res \
	$(outdir)\wrapper.obj \
	$(texmf_lib) \

$(outdir)\texindex.exe: \
			$(outdir) \
			$(texindex_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(texindex_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\texindex.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		texindex-version.h \
		texindex.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ texindex.rc

$(outdir)\wrapper.obj: $(wrapper_cpp)
	$(cc) $(cppstandard_aeh) \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_select_fastcall) \
		$(ccopt_output_file)$@ \
		$(wrapper_cpp)

$(outdir)\texi2dvi.exe: $(mcdlibdir)\$(outdir)\texify.exe
	copy $(mcdlibdir)\$(outdir)\texify.exe $@

!include <common-dependencies.inc>

depend: texindex.c
	$(MAKEDEPEND) -I.. $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\texindex.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\texindex.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\texindex.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h
