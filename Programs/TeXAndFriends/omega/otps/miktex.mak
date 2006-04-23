## miktex.mak: omega/otps
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

defines = \
	-Dalloca=_alloca \
	-DYY_NEVER_INTERACTIVE \
	-DMIKTEX_OMEGA \

includes = \
	$(ccinc_libkps) \

common_options = \
	$(ccopt_enable_function_level_linking) \
	$(defines) \
	$(includes) \

binaries = \
	$(outdir)\mkocp.exe \
	$(outdir)\otp2ocp.exe \
	$(outdir)\outocp.exe \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(common_options) \
		$(ccopt_output_directory)$(outdir)\ \
		$<

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) \
		$(common_options) \
		$(ccopt_output_directory)$(outdir)\ \
		$<

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
# mkocp
# _____________________________________________________________________________

mkocp_objects = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\mkocp.obj \
	$(outdir)\mkocp.res \

$(outdir)\mkocp.exe: \
			$(outdir) \
			$(mkocp_objects) \

	$(link) $(lstandard) -out:$@ $(mkocp_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\mkocp.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			mkocp-version.h \
			mkocp.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ mkocp.rc

# _____________________________________________________________________________
#
# outocp
# _____________________________________________________________________________

outocp_objects = \
	$(app_lib) \
	$(gnu_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\outocp.obj \
	$(outdir)\outocp.res \
	$(outdir)\wrapper.obj \
	$(texmf_lib) \

$(outdir)\outocp.exe: \
			$(outdir) \
			$(outocp_objects) \

	$(link) $(lstandard) -out:$@ $(outocp_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\outocp.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			outocp-version.h \
			outocp.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ outocp.rc

# _____________________________________________________________________________
#
# otp2ocp
# _____________________________________________________________________________

otp2ocp_objects = \
	$(app_lib) \
	$(gnu_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\otp2ocp.obj \
	$(outdir)\otp2ocp.res \
	$(outdir)\routines.obj \
	$(outdir)\wrapper.obj \
	$(outdir)\y_tab.obj \
	$(outdir)\yy.lex.obj \
	$(texmf_lib) \

$(outdir)\otp2ocp.exe: \
			$(outdir) \
			$(otp2ocp_objects) \

	$(link) $(lstandard) -out:$@ $(otp2ocp_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\otp2ocp.res: \
			$(outdir) \
			otp2ocp.rc \
			otp2ocp-version.h \
			$(libdir)\include\miktex\miktex.version \
			$(libdir)\include\miktex\version.h \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ otp2ocp.rc

y_tab.c y_tab.h: otp.y
	$(yacc) -d -o y_tab.c otp.y

yy.lex.c: otp.l
	$(lex) -t otp.l > $@

.bootstrap: init-y_tab.c init-y_tab.h init-yy.lex.c
	@del /f y_tab.c
	@del /f y_tab.h
	@del /f yy.lex.c
	type init-y_tab.c >  y_tab.c
	type init-y_tab.h >  y_tab.h
	type init-yy.lex.c >  yy.lex.c
	echo x > .bootstrap

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean
	-del /f .bootstrap

maintainer-clean: common-maintainer-clean distclean
	-del y_tab.c
	-del y_tab.h
	-del yy.lex.c

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: outocp.c otp2ocp.c y_tab.c yy.lex.c routines.c
	$(MAKEDEPEND) $(defines) $(includes) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\outocp.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\outocp.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\outocp.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\outocp.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\outocp.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\outocp.obj: otp.h
$(outdir)\otp2ocp.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\otp2ocp.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\otp2ocp.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\otp2ocp.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\otp2ocp.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\otp2ocp.obj: routines.h
$(outdir)\y_tab.obj: otp.h routines.h
$(outdir)\y_tab.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\y_tab.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\y_tab.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\y_tab.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\y_tab.obj: yystype.h
__xx__yy.lex.obj: routines.h
__xx__yy.lex.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
__xx__yy.lex.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
__xx__yy.lex.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
__xx__yy.lex.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
__xx__yy.lex.obj: yystype.h y_tab.h
$(outdir)\routines.obj: C:\work\miktex-2.5\Libraries\MiKTeX\KPathSeaEmulation\include/miktex/kpsemu.h
$(outdir)\routines.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\routines.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\routines.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\routines.obj: routines.h otp.h
