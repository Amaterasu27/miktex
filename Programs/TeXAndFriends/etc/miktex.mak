## miktex.mak: etc
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

!include <miktex.inc>

binaries = \
	$(outdir)\$(vftovp_dll_name).dll \
	$(outdir)\$(vptovf_dll_name).dll \
	$(outdir)\vftovp.exe \
	$(outdir)\vptovf.exe \

.cc{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_directory)$(outdir)\ -Tp $<

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_directory)$(outdir)\ $<

all: $(binaries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# vftovp
# _____________________________________________________________________________

!ifndef vftovpexport
vftovpexport = MiKTeX_VFtoVP
!endif

vftovpclass = WebApp
vftovpmain = RunVFtoVP
vftovpapp = g_VFtoVPApp
vftovpdata = g_VFtoVPData

vftovp_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\vftovp.obj \
	$(outdir)\vftovp.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(vftovp_dll_name).dll $(outdir)\vftovp.lib: \
					$(outdir) \
					$(dllbases) \
					$(vftovp_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(vftovp_dll_name) \
		-export:$(vftovpexport) \
		-implib:$(outdir)\vftovp.lib \
		-out:$(outdir)\$(vftovp_dll_name).dll \
		$(vftovp_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\vftovp.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		vftovp-version.h \
		vftovp.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ vftovp.rc

$(outdir)\vftovp.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\vftovp.lib \

	$(cc) \
		$(ccopt_enable_exception_handling) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(vftovpexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\vftovp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

vftovp.cc vftovp.h: vftovp.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(vftovpmain) \
		--include-filename=mywebapp.h \
		--one=vftovp \
		--var-struct=$(vftovpdata) \
		-C \
		vftovp.p
	type << >> vftovp.cc
MIKTEX_DEFINE_WEBAPP(
  $(vftovpexport),
  $(vftovpclass),
  $(vftovpapp),
  $(vftovpmain),
  $(vftovpdata))
<<

mywebapp.h:
	type << >> $@
#include <miktex/webapp.h>
using namespace MiKTeX::TeXAndFriends;
<<

$(outdir)\vftovp.obj: mywebapp.h

vftovp.p: vftovp.web vftovp-miktex.ch
	$(tangle) vftovp.web vftovp-miktex.ch vftovp.p nul

# _____________________________________________________________________________
#
# vptovf
# _____________________________________________________________________________

!ifndef vptovfexport
vptovfexport = MiKTeX_VPtoVF
!endif

vptovfclass = WebApp
vptovfmain = RunVPtoVF
vptovfapp = g_VPtoVFApp
vptovfdata = g_VPtoVFData

vptovf_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\vptovf.obj \
	$(outdir)\vptovf.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(vptovf_dll_name).dll $(outdir)\vptovf.lib: \
			$(outdir) \
			$(dllbases) \
			$(vptovf_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(vptovf_dll_name) \
		-export:$(vptovfexport) \
		-implib:$(outdir)\vptovf.lib \
		-out:$(outdir)\$(vptovf_dll_name).dll \
		$(vptovf_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\vptovf.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		vptovf-version.h \
		vptovf.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ vptovf.rc

$(outdir)\vptovf.exe: \
		$(outdir) \
		$(alias_cpp) \
		$(outdir)\vptovf.lib \

	$(cc) \
		$(ccopt_enable_exception_handling) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(vptovfexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\vptovf.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

vptovf.cc vptovf.h: vptovf.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(vptovfmain) \
		--include-filename=mywebapp.h \
		--one=vptovf \
		--var-struct=$(vptovfdata) \
		-C \
		vptovf.p
	type << >> vptovf.cc
MIKTEX_DEFINE_WEBAPP(
  $(vptovfexport),
  $(vptovfclass),
  $(vptovfapp),
  $(vptovfmain),
  $(vptovfdata))
<<

$(outdir)\vptovf.obj: mywebapp.h

vptovf.p: vptovf.web vptovf-miktex.ch
	$(tangle) vptovf.web vptovf-miktex.ch vptovf.p nul

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

mostlyclean: common-mostlyclean
	-del /f mywebapp.h
	-del /F vftovp.cc
	-del /F vftovp.h
	-del /F vptovf.cc
	-del /F vptovf.h
	-del /F *.p

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: vftovp.cc vptovf.cc
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\vftovp.obj: vftovp.h mywebapp.h
$(outdir)\vptovf.obj: vptovf.h mywebapp.h
