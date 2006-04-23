## miktex.mak: ps2pk/ps2pk
## 
## Copyright (C) 2000-2005 Christian Schenk
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

sources = pkout.c ps2pk.c

binaries = $(outdir)\ps2pk.exe

defines = \
	-DKPATHSEA \

includes = \
	$(ccinc_zlib) \
	-I..\common \
	-I..\psres \
	-I..\type1 \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(defines) \
		$(includes) \
		$(ccopt_output_directory)$(outdir)\ \
		$<

all: common-all

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# ps2pk.exe
# _____________________________________________________________________________

ps2pk_objects = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\encoding.obj \
	$(outdir)\pkout.obj \
	$(outdir)\ps2pk.obj \
	$(outdir)\ps2pk.res \
	$(outdir)\wrapper.obj \
	$(kps_lib) \
	$(texmf_lib) \
	$(zlib_lib) \
	..\PSres\$(outdir)\psres.lib \
	..\common\$(outdir)\common.lib \
	..\type1\$(outdir)\type1.lib \

$(outdir)\ps2pk.exe: \
			$(outdir) \
			$(ps2pk_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(ps2pk_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\ps2pk.res: \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			$(outdir) \
			ps2pk-version.h \
			ps2pk.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ ps2pk.rc

$(outdir)\wrapper.obj: $(wrapper_cpp)
	$(cc) $(cppstandard_aeh) \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_select_fastcall) \
		$(ccopt_output_file)$(outdir)\ \
		$(wrapper_cpp)

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend:
	$(MAKEDEPEND) $(defines) $(includes) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\pkout.obj: ..\common/basics.h pkout.h
$(outdir)\ps2pk.obj: ..\psres/PSres.h ..\common/basics.h ..\common/filenames.h
$(outdir)\ps2pk.obj: ..\common/psearch.h pkout.h
$(outdir)\ps2pk.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\ps2pk.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\ps2pk.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\ps2pk.obj: ..\type1/ffilest.h ..\type1/types.h ..\type1/Xstuff.h
$(outdir)\ps2pk.obj: ..\type1/fontmisc.h ..\type1/fontstruct.h ..\type1/font.h
$(outdir)\ps2pk.obj: ..\type1/fsmasks.h ..\type1/fontfile.h ..\type1/fontxlfd.h
