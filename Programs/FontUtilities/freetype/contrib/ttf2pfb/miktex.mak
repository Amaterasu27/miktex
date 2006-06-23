## miktex.mak: freetype/contrib/ttf2pk
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

sources = \
	getafm.cpp \
	t1asm.c \
	ttf2pfb.c \

getafmobjs = \
	$(outdir)\getafm.obj \

ttf2pfbobjs = \
	$(outdir)\ttf2pfb.obj \
	$(outdir)\wrapper.obj \

t1asmobjs = \
	$(outdir)\t1asm.obj

binaries = \
	$(outdir)\getafm.exe \
	$(outdir)\t1asm.exe \
	$(outdir)\ttf2pfb.exe \

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

defines = \
	-DUSE_MIKTEX_EXIT \

includes = \
	-I$(freetypelibdir) \
	$(ccinc_libgnu) \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		$(defines) \
		$(includes) \
		$<

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		$(defines) \
		$(includes) \
		$<

$(outdir)\wrapper.obj: $(wrapper_cpp)
	$(cc) $(cppstandard_aeh) \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_select_fastcall) \
		$(ccopt_output_file)$(outdir)\ \
		$(wrapper_cpp)

# _____________________________________________________________________________
# 
# ttf2pfb
# _____________________________________________________________________________

ttf2pfb_objects = \
	$(app_lib) \
	$(freetype_lib) \
	$(miktex_lib) \
	$(outdir)\ttf2pfb.res \
	$(texmf_lib) \
	$(ttf2pfbobjs) \

$(outdir)\ttf2pfb.exe: \
		$(outdir) \
		$(ttf2pfb_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(ttf2pfb_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\ttf2pfb.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			ttf2pfb-version.h \
			ttf2pfb.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ ttf2pfb.rc

# _____________________________________________________________________________
#
# t1asm
# _____________________________________________________________________________

t1asm_objects = \
	$(freetype_lib) \
	$(gnu_lib) \
	$(miktex_lib) \
	$(outdir)\t1asm.res \
	$(t1asmobjs) \

$(outdir)\t1asm.exe: \
			$(outdir) \
			$(t1asm_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(t1asm_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\t1asm.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			t1asm-version.h \
			t1asm.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ t1asm.rc

# _____________________________________________________________________________
#
# getafm
# _____________________________________________________________________________

getafm_objects = \
	$(getafmobjs) \
	$(outdir)\getafm.res \

$(outdir)\getafm.exe: \
			$(outdir) \
			$(getafm_objects) \

	$(link) $(lstandard) $(linkopt_output_file)$@ $(getafm_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\getafm.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			getafm-version.h \
			getafm.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ getafm.rc

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean
	-del /f *.dvi

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend:
	$(MAKEDEPEND) -I..\..\lib $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\t1asm.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h
$(outdir)\ttf2pfb.obj: ..\..\lib/freetype.h ..\..\lib/fterrid.h
$(outdir)\ttf2pfb.obj: ..\..\lib/ftnameid.h ..\..\lib/extend/ftxpost.h
$(outdir)\ttf2pfb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\ttf2pfb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\ttf2pfb.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
