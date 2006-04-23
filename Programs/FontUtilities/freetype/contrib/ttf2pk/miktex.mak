## miktex.mak: freetype/contrib/ttf2pk
## 
## Copyright (C) 1998-2005 Christian Schenk
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
	case.c \
	errormsg.c \
	filesrch.c \
	ligkern.c \
	newobj.c \
	parse.c \
	pklib.c \
	subfont.c\
	texenc.c \
	tfmaux.c \
	ttf2pk.c \
	ttf2tfm.c \
	ttfaux.c \
	ttfenc.c \
	ttflib.c \
	plaux.c \

ttf2pkobjs = \
	$(outdir)\errormsg.obj \
	$(outdir)\filesrch.obj \
	$(outdir)\ligkern.obj \
	$(outdir)\newobj.obj \
	$(outdir)\parse.obj \
	$(outdir)\pklib.obj \
	$(outdir)\subfont.obj \
	$(outdir)\texenc.obj \
	$(outdir)\ttf2pk.obj \
	$(outdir)\ttfenc.obj \
	$(outdir)\ttflib.obj \
	$(outdir)\wrapper.obj \

ttf2tfmobjs = \
	$(outdir)\case.obj \
	$(outdir)\errormsg.obj \
	$(outdir)\filesrch.obj \
	$(outdir)\ligkern.obj \
	$(outdir)\newobj.obj \
	$(outdir)\parse.obj \
	$(outdir)\subfont.obj \
	$(outdir)\texenc.obj \
	$(outdir)\tfmaux.obj \
	$(outdir)\ttf2tfm.obj \
	$(outdir)\ttfaux.obj \
	$(outdir)\ttfenc.obj \
	$(outdir)\vplaux.obj \
	$(outdir)\wrapper.obj \

binaries = $(outdir)\ttf2pk.exe $(outdir)\ttf2tfm.exe

all: common-all $(binaries) $(ttf2pkdocs) $(ttf2tfmdocs)

install: common-install install-binaries

check: common-check

includes = \
	-I$(freetypelibdir) \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
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
# ttf2pk
# _____________________________________________________________________________

ttf2pk_objects = \
	$(app_lib) \
	$(freetype_lib) \
	$(miktex_lib) \
	$(outdir)\ttf2pk.res \
	$(texmf_lib) \
	$(ttf2pkobjs) \

$(outdir)\ttf2pk.exe: \
			$(outdir) \
			$(ttf2pk_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(ttf2pk_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\ttf2pk.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			ttf2pk-version.h \
			ttf2pk.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ ttf2pk.rc

# _____________________________________________________________________________
#
# ttf2tfm
# _____________________________________________________________________________

ttf2tfm_objects = \
	$(app_lib) \
	$(freetype_lib) \
	$(miktex_lib) \
	$(outdir)\ttf2tfm.res \
	$(texmf_lib) \
	$(ttf2tfmobjs) \

$(outdir)\ttf2tfm.exe: \
			$(outdir) \
			$(ttf2tfm_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(ttf2tfm_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\ttf2tfm.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			ttf2tfm-version.h \
			ttf2tfm.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ ttf2tfm.rc

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

depend:
	$(MAKEDEPEND) -I..\..\lib $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\case.obj: case.h
$(outdir)\errormsg.obj: errormsg.h
$(outdir)\errormsg.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\errormsg.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\errormsg.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\filesrch.obj: filesrch.h ttf2tfm.h newobj.h
$(outdir)\filesrch.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\filesrch.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\filesrch.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\ligkern.obj: ttf2tfm.h ligkern.h ttfenc.h texenc.h newobj.h errormsg.h
$(outdir)\newobj.obj: newobj.h ttf2tfm.h errormsg.h texenc.h
$(outdir)\parse.obj: ttf2tfm.h errormsg.h newobj.h ligkern.h texenc.h parse.h
$(outdir)\parse.obj: filesrch.h
$(outdir)\pklib.obj: newobj.h ttf2tfm.h pklib.h errormsg.h filesrch.h
$(outdir)\subfont.obj: filesrch.h ttf2tfm.h subfont.h newobj.h errormsg.h
$(outdir)\texenc.obj: ttf2tfm.h texenc.h
$(outdir)\tfmaux.obj: ttf2tfm.h newobj.h tfmaux.h errormsg.h
$(outdir)\ttf2pk.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\ttf2pk.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\ttf2pk.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\ttf2pk.obj: ttf2tfm.h newobj.h pklib.h ttfenc.h ttflib.h errormsg.h
$(outdir)\ttf2pk.obj: filesrch.h parse.h subfont.h
$(outdir)\ttf2tfm.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\ttf2tfm.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\ttf2tfm.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\ttf2tfm.obj: ttf2tfm.h newobj.h ttfenc.h ligkern.h texenc.h ttfaux.h
$(outdir)\ttf2tfm.obj: tfmaux.h vplaux.h errormsg.h filesrch.h parse.h subfont.h
$(outdir)\ttfaux.obj: ..\..\lib/freetype.h ..\..\lib/fterrid.h
$(outdir)\ttfaux.obj: ..\..\lib/ftnameid.h ..\..\lib/extend/ftxkern.h
$(outdir)\ttfaux.obj: ..\..\lib/extend/ftxpost.h ..\..\lib/extend/ftxopen.h
$(outdir)\ttfaux.obj: ..\..\lib/extend/ftxgdef.h ..\..\lib/extend/ftxgsub.h
$(outdir)\ttfaux.obj: ..\..\lib/extend/ftxgpos.h ttf2tfm.h newobj.h ligkern.h
$(outdir)\ttfaux.obj: ttfenc.h tfmaux.h errormsg.h ttfaux.h filesrch.h
$(outdir)\ttfenc.obj: newobj.h ttf2tfm.h ttfenc.h errormsg.h
$(outdir)\ttflib.obj: pklib.h ..\..\lib/freetype.h ..\..\lib/fterrid.h
$(outdir)\ttflib.obj: ..\..\lib/ftnameid.h ttfenc.h ttf2tfm.h ttflib.h errormsg.h
$(outdir)\ttflib.obj: newobj.h ..\..\lib/extend/ftxpost.h
$(outdir)\ttflib.obj: ..\..\lib/extend/ftxopen.h ..\..\lib/extend/ftxgdef.h
$(outdir)\ttflib.obj: ..\..\lib/extend/ftxgsub.h ..\..\lib/extend/ftxgpos.h
