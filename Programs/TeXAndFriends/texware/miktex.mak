## miktex.mak: texware
## 
## Copyright (C) 1991-2005 Christian Schenk
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

#miktex_cc_warnings_as_errors = 1

!include <miktex.inc>

binaries = \
	$(outdir)\$(dvitype_dll_name).dll \
	$(outdir)\$(pltotf_dll_name).dll \
	$(outdir)\$(pooltype_dll_name).dll \
	$(outdir)\$(tftopl_dll_name).dll \
	$(outdir)\dvitype.exe \
	$(outdir)\pltotf.exe \
	$(outdir)\pooltype.exe \
	$(outdir)\tftopl.exe \

.cc{$(outdir)\}.obj:
	$(cc) $(cppstandard) \
		$(ccopt_all_cpp) \
		$(ccopt_output_directory)$(outdir)\ $<

all: common-all

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# dvitype
# _____________________________________________________________________________

!ifndef dvitypeexport
dvitypeexport = MiKTeX_DVItype
!endif

dvitypeapp = g_DVItypeApp
dvitypeclass = CDVItypeApp
dvitypedata = g_DVItypeData
dvitypemain = RunDVItype

dvitype_objects = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\dvitype.obj \
	$(outdir)\dvitype.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(dvitype_dll_name).dll $(outdir)\dvitype.lib: \
			$(outdir) \
			$(dllbases) \
			$(dvitype_objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(dvitype_dll_name) \
		-export:$(dvitypeexport) \
		-implib:$(outdir)\dvitype.lib \
		-out:$(outdir)\$(dvitype_dll_name).dll \
		$(dvitype_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\dvitype.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\dvitype.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(dvitypeexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\dvitype.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\dvitype.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			dvitype-version.h \
			dvitype.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ dvitype.rc

$(outdir)\dvitype.obj: dvitype.cc dvitype.h
	$(cc) $(cppstandard) \
		$(ccopt_all_cpp) \
		-DDVITYPECLASS=$(dvitypeclass) \
		-DDVITYPEAPP=$(dvitypeapp) \
		-DDVITYPEDATA=$(dvitypedata) \
		$(ccopt_output_directory)$(outdir)\ \
		dvitype.cc

dvitype.cc dvitype.h: dvitype.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(dvitypemain) \
		--include-filename=dvitype-miktex.h \
		--one=dvitype \
		--var-name-prefix=m_ \
		--var-struct=$(dvitypedata) \
		-C \
		dvitype.p
	type << >> dvitype.cc
MIKTEX_DEFINE_WEBAPP(
  $(dvitypeexport),
  $(dvitypeclass),
  $(dvitypeapp),
  $(dvitypemain),
  $(dvitypedata))
<<

dvitype.p: dvitype.web dvitype-miktex.ch
	$(tangle) dvitype.web dvitype-miktex.ch dvitype.p nul

# _____________________________________________________________________________
#
# pooltype
# _____________________________________________________________________________

!ifndef pooltypeexport
pooltypeexport = MiKTeX_POOLtype
!endif

pooltypeapp = g_POOLtypeApp
pooltypeclass = WebApp
pooltypedata = g_POOLtypeData
pooltypemain = RunPOOLtype

pooltype_objects = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\pooltype.obj \
	$(outdir)\pooltype.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(pooltype_dll_name).dll $(outdir)\pooltype.lib: \
			$(outdir) \
			$(pooltype_objects) \
			$(dllbases) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(pooltype_dll_name) \
		-export:$(pooltypeexport) \
		-implib:$(outdir)\pooltype.lib \
		-out:$(outdir)\$(pooltype_dll_name).dll \
		$(pooltype_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\pooltype.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\pooltype.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(pooltypeexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\pooltype.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\pooltype.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			pooltype-version.h \
			pooltype.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ pooltype.rc

pooltype.cc pooltype.h: pooltype.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(pooltypemain) \
		--include-filename=mywebapp.h \
		--one=pooltype \
		--var-struct=$(pooltypedata) \
		-C \
		pooltype.p
	type << >> pooltype.cc
MIKTEX_DEFINE_WEBAPP(
  $(pooltypeexport),
  $(pooltypeclass),
  $(pooltypeapp),
  $(pooltypemain),
  $(pooltypedata))
<<

mywebapp.h:
	type << >> $@
#include <miktex/webapp.h>
using namespace MiKTeX::TeXAndFriends;
<<

$(outdir)\pooltype.obj: mywebapp.h

pooltype.p: pooltype.web pooltype-miktex.ch
	$(tangle) pooltype.web pooltype-miktex.ch pooltype.p nul

# _____________________________________________________________________________
#
# pltotf
# _____________________________________________________________________________

!ifndef pltotfexport
pltotfexport = MiKTeX_PLtoTF
!endif

pltotfapp = g_PLtoTFApp
pltotfclass = WebApp
pltotfdata = g_PLtoTFData
pltotfmain = RunPLtoTF

pltotf_objects = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\pltotf.obj \
	$(outdir)\pltotf.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(pltotf_dll_name).dll $(outdir)\pltotf.lib: \
			$(outdir) \
			$(dllbases) \
			$(pltotf_objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(pltotf_dll_name) \
		-export:$(pltotfexport) \
		-implib:$(outdir)\pltotf.lib \
		-out:$(outdir)\$(pltotf_dll_name).dll \
		$(pltotf_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\pltotf.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\pltotf.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(pltotfexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\pltotf.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\pltotf.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			pltotf-version.h \
			pltotf.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ pltotf.rc

pltotf.cc pltotf.h: pltotf.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(pltotfmain) \
		--include-filename=mywebapp.h \
		--one=pltotf \
		--var-struct=$(pltotfdata) \
		-C \
		pltotf.p
	type << >> pltotf.cc
MIKTEX_DEFINE_WEBAPP(
  $(pltotfexport),
  $(pltotfclass),
  $(pltotfapp),
  $(pltotfmain),
  $(pltotfdata))
<<

$(outdir)\pltotf.obj: mywebapp.h

pltotf.p: pltotf.web pltotf-miktex.ch
	$(tangle) pltotf.web pltotf-miktex.ch pltotf.p nul

# _____________________________________________________________________________
#
# tftopl
# _____________________________________________________________________________

!ifndef tftoplexport
tftoplexport = MiKTeX_TFtoPL
!endif

tftoplapp = g_TFtoPLApp
tftoplclass = WebApp
tftopldata = g_TFtoPLData
tftoplmain = RunTFtoPL

tftopl_objects = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\tftopl.obj \
	$(outdir)\tftopl.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(tftopl_dll_name).dll $(outdir)\tftopl.lib: \
			$(outdir) \
			$(dllbases) \
			$(tftopl_objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(tftopl_dll_name) \
		-export:$(tftoplexport) \
		-implib:$(outdir)\tftopl.lib \
		-out:$(outdir)\$(tftopl_dll_name).dll \
		$(tftopl_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\tftopl.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\tftopl.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(tftoplexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\tftopl.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\tftopl.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			tftopl-version.h \
			tftopl.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ tftopl.rc

tftopl.cc tftopl.h: tftopl.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(tftoplmain) \
		--include-filename=mywebapp.h \
		--one=tftopl \
		--var-struct=$(tftopldata) \
		-C \
		tftopl.p
	type << >> tftopl.cc
MIKTEX_DEFINE_WEBAPP(
  $(tftoplexport),
  $(tftoplclass),
  $(tftoplapp),
  $(tftoplmain),
  $(tftopldata))
<<

$(outdir)\tftopl.obj: mywebapp.h

tftopl.p: tftopl.web tftopl-miktex.ch
	$(tangle) tftopl.web tftopl-miktex.ch tftopl.p nul

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

mostlyclean: common-mostlyclean
	-del /f *.cc
	-del /f *.p
	-del /f dvitype.h
	-del /f mywebapp.h
	-del /f pltotf.h
	-del /f pooltype.h
	-del /f tftopl.h

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: dvitype.cc pooltype.cc tftopl.cc pltotf.cc
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\dvitype.obj: dvitype.h
$(outdir)\dvitype.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\dvitype.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\dvitype.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\dvitype.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\dvitype.obj: dvitype-miktex.h dvitype.rc dvitype-version.h
$(outdir)\dvitype.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\dvitype.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\dvitype.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\dvitype.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\dvitype.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\dvitype.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\dvitype.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\dvitype.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\dvitype.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\pooltype.obj: pooltype.h
$(outdir)\pooltype.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\pooltype.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\pooltype.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\pooltype.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\pooltype.obj: mywebapp.h
$(outdir)\pooltype.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\pooltype.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\pooltype.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\pooltype.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\pooltype.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\pooltype.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\tftopl.obj: tftopl.h
$(outdir)\tftopl.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\tftopl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\tftopl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\tftopl.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\tftopl.obj: mywebapp.h
$(outdir)\tftopl.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\tftopl.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\tftopl.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\tftopl.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\tftopl.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\tftopl.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\pltotf.obj: pltotf.h
$(outdir)\pltotf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\pltotf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\pltotf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\pltotf.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\pltotf.obj: mywebapp.h
$(outdir)\pltotf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\pltotf.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\pltotf.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\pltotf.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\pltotf.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\pltotf.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
