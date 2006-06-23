## miktex.mak: metapost/mpware
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

binaries = \
	$(outdir)\$(dvitomp_dll_name).dll \
	$(outdir)\dvitomp.exe \
	$(outdir)\makempx.exe \
	$(outdir)\mpto.exe \

.cc{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_directory)$(outdir)\ $<

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_directory)$(outdir)\ $<

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# dvitomp
# _____________________________________________________________________________

!ifndef dvitompexport
dvitompexport = MiKTeX_DVItoMP
!endif

dvitompclass = WebApp
dvitompmain = RunDVItoMP
dvitompapp = g_DVItoMPApp
dvitompdata = g_DVItoMPData

dvitomp_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\dvitomp.obj \
	$(outdir)\dvitomp.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(dvitomp_dll_name).dll $(outdir)\dvitomp.lib: \
			$(outdir) \
			$(dllbases) \
			$(dvitomp_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(dvitomp_dll_name) \
		-export:$(dvitompexport) \
		-implib:$(outdir)\dvitomp.lib \
		-out:$(outdir)\$(dvitomp_dll_name).dll \
		$(dvitomp_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\dvitomp.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			dvitomp-version.h \
			dvitomp.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ dvitomp.rc

$(outdir)\dvitomp.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\dvitomp.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(dvitompexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\dvitomp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

dvitomp.cc dvitomp.h: dvitomp.p
	$(c4p) $(c4prenames) \
		--dll \
		--entry-name=$(dvitompmain) \
		--include-filename=mywebapp.h \
		--one=dvitomp \
		--var-struct=$(dvitompdata) \
		-C \
		dvitomp.p
	type << >> dvitomp.cc
MIKTEX_DEFINE_WEBAPP(
  $(dvitompexport),
  $(dvitompclass),
  $(dvitompapp),
  $(dvitompmain),
  $(dvitompdata))
<<

mywebapp.h:
	type << >> $@
#include <miktex/webapp.h>
using namespace MiKTeX::TeXAndFriends;
<<

dvitomp.p: dvitomp.web dvitomp-miktex.ch
	$(tangle) dvitomp.web dvitomp-miktex.ch dvitomp.p nul

# _____________________________________________________________________________
#
# makempx
# _____________________________________________________________________________

makempx_objects = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\makempx.obj \
	$(outdir)\makempx.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\makempx.exe: \
			$(outdir) \
			$(makempx_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(makempx_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\makempx.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			makempx-version.h \
			makempx.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ makempx.rc

$(outdir)\wrapper.obj: $(wrapper_cpp)
	$(cc) $(cppstandard_aeh) \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_select_fastcall) \
		$(ccopt_output_file)$(outdir)\ \
		$(wrapper_cpp)

# _____________________________________________________________________________
#
# mpto
# _____________________________________________________________________________

mpto_objects = \
	$(miktex_lib) \
	$(outdir)\mpto.obj \
	$(outdir)\mpto.res \
#	$(outdir)\wrapper.obj \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\mpto.exe: \
			$(outdir) \
			$(mpto_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(mpto_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\mpto.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			mptotex-version.h \
			mptotex.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ mptotex.rc

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

mostlyclean: common-mostlyclean
	-del /f dvitomp.cc
	-del /f dvitomp.h
	-del /f *.p

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: makempx.cpp dvitomp.cc mpto.c
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h makempx.rc
$(outdir)\makempx.obj: makempx-version.h
$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\makempx.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\dvitomp.obj: dvitomp.h
$(outdir)\dvitomp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\dvitomp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\dvitomp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\dvitomp.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\dvitomp.obj: mywebapp.h
$(outdir)\dvitomp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\dvitomp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\dvitomp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\dvitomp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\dvitomp.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\dvitomp.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
