## miktex.mak: omega/omegaware
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
	-DMIKTEX_OMEGA \
	-Dalloca=_alloca \

includes = \
	$(ccinc_libkps) \

cloptions = \
	$(cppstandard) \
	$(ccopt_enable_function_level_linking) \
	$(ccopt_select_fastcall) \
	$(defines) \
	$(includes)

binaries = \
	$(outdir)\$(odvicopy_dll_name).dll \
	$(outdir)\$(ofm2opl_dll_name).dll \
	$(outdir)\$(opl2ofm_dll_name).dll \
	$(outdir)\$(ovf2ovp_dll_name).dll \
	$(outdir)\$(ovp2ovf_dll_name).dll \
	$(outdir)\odvicopy.exe \
	$(outdir)\ofm2opl.exe \
	$(outdir)\opl2ofm.exe \
	$(outdir)\ovf2ovp.exe \
	$(outdir)\ovp2ovf.exe \

.cc{$(outdir)\}.obj:
	$(cc) $(cloptions) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(binaries)

install: common-install install-binaries

check:

# _____________________________________________________________________________
#
# odvicopy
# _____________________________________________________________________________

!ifndef odvicopyexport
odvicopyexport = MiKTeX_ODVIcopy
!endif

odvicopyapp = g_ODVIcopyApp
odvicopyclass = WebApp
odvicopydata = g_ODVIcopyData
odvicopymain = RunODVIcopy

odvicopy_obj = \
	$(app_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\odvicopy.obj \
	$(outdir)\odvicopy.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(odvicopy_dll_name).dll $(outdir)\odvicopy.lib: \
			$(outdir) \
			$(dllbases) \
			$(odvicopy_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(odvicopy_dll_name) \
		-export:$(odvicopyexport) \
		-implib:$(outdir)\odvicopy.lib \
		-out:$(outdir)\$(odvicopy_dll_name).dll \
		$(odvicopy_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\odvicopy.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			odvicopy-version.h \
			odvicopy.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ odvicopy.rc

$(outdir)\odvicopy.exe: \
			$(outdir) \
			$(outdir)\odvicopy.lib \
			$(alias_cpp) \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(odvicopyexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\odvicopy.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

odvicopy.cc odvicopy.h: odvicopy.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(odvicopymain) \
		--include-filename=mywebapp.h \
		--one=odvicopy \
		--var-struct=$(odvicopydata) \
		-C \
		odvicopy.p
	type << >> odvicopy.cc
MIKTEX_DEFINE_WEBAPP(
  $(odvicopyexport),
  $(odvicopyclass),
  $(odvicopyapp),
  $(odvicopymain),
  $(odvicopydata))
<<

mywebapp.h:
	type << >> $@
#include <miktex/webapp.h>
using namespace MiKTeX::TeXAndFriends;
<<

$(outdir)\odvicopy.obj: mywebapp.h

odvicopy.p: odvicopy.web odvicopy-miktex.ch
	$(tangle) odvicopy.web odvicopy-miktex.ch odvicopy.p nul

# _____________________________________________________________________________
#
# ofm2opl
# _____________________________________________________________________________

!ifndef ofm2oplexport
ofm2oplexport = MiKTeX_OFM2OPL
!endif

ofm2oplapp = g_OFM2OPLApp
ofm2oplclass = WebApp
ofm2opldata = g_OFM2OPLData
ofm2oplmain = RunOFM2OPL

ofm2opl_obj = \
	$(app_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\ofm2opl.obj \
	$(outdir)\ofm2opl.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(ofm2opl_dll_name).dll $(outdir)\ofm2opl.lib: \
			$(outdir) \
			$(dllbases) \
			$(ofm2opl_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(ofm2opl_dll_name) \
		-export:$(ofm2oplexport) \
		-implib:$(outdir)\ofm2opl.lib \
		-out:$(outdir)\$(ofm2opl_dll_name).dll \
		$(ofm2opl_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\ofm2opl.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			ofm2opl-version.h \
			ofm2opl.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ ofm2opl.rc

$(outdir)\ofm2opl.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\ofm2opl.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(ofm2oplexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\ofm2opl.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

ofm2opl.cc ofm2opl.h: ofm2opl.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(ofm2oplmain) \
		--include-filename=mywebapp.h \
		--one=ofm2opl \
		--var-struct=$(ofm2opldata) \
		-C \
		ofm2opl.p
	type << >> ofm2opl.cc
MIKTEX_DEFINE_WEBAPP(
  $(ofm2oplexport),
  $(ofm2oplclass),
  $(ofm2oplapp),
  $(ofm2oplmain),
  $(ofm2opldata))
<<

ofm2opl.p: ofm2opl.web ofm2opl-miktex.ch
	$(tangle) ofm2opl.web ofm2opl-miktex.ch ofm2opl.p nul

# _____________________________________________________________________________
#
# opl2ofm
# _____________________________________________________________________________

!ifndef opl2ofmexport
opl2ofmexport = MiKTeX_OPL2OFM
!endif

opl2ofmapp = g_OPL2OFMApp
opl2ofmclass = WebApp
opl2ofmdata = g_OPL2OFMData
opl2ofmmain = RunOPL2OFM

opl2ofm_obj = \
	$(app_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\opl2ofm.obj \
	$(outdir)\opl2ofm.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(opl2ofm_dll_name).dll $(outdir)\opl2ofm.lib: \
			$(outdir) \
			$(dllbases) \
			$(opl2ofm_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(opl2ofm_dll_name) \
		-export:$(opl2ofmexport) \
		-implib:$(outdir)\opl2ofm.lib \
		-out:$(outdir)\$(opl2ofm_dll_name).dll \
		$(opl2ofm_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\opl2ofm.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			opl2ofm-version.h \
			opl2ofm.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ opl2ofm.rc

$(outdir)\opl2ofm.exe: \
			$(outdir) \
			$(alias_cpp)\
			$(outdir)\opl2ofm.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(opl2ofmexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\opl2ofm.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

opl2ofm.cc opl2ofm.h: opl2ofm.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(opl2ofmmain) \
		--include-filename=mywebapp.h \
		--one=opl2ofm \
		--var-struct=$(opl2ofmdata) \
		-C \
		opl2ofm.p
	type << >> opl2ofm.cc
MIKTEX_DEFINE_WEBAPP(
  $(opl2ofmexport),
  $(opl2ofmclass),
  $(opl2ofmapp),
  $(opl2ofmmain),
  $(opl2ofmdata))
<<

opl2ofm.p: opl2ofm.web opl2ofm-miktex.ch
	$(tangle) opl2ofm.web opl2ofm-miktex.ch opl2ofm.p nul

# _____________________________________________________________________________
#
# ovf2ovp
# _____________________________________________________________________________

!ifndef ovf2ovpexport
ovf2ovpexport = MiKTeX_OVF2OVP
!endif

ovf2ovpapp = g_OVF2OVPApp
ovf2ovpclass = WebApp
ovf2ovpdata = g_OVF2OVPData
ovf2ovpmain = RunOVF2OVP

ovf2ovp_obj = \
	$(app_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\ovf2ovp.obj \
	$(outdir)\ovf2ovp.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(ovf2ovp_dll_name).dll $(outdir)\ovf2ovp.lib: \
			$(outdir) \
			$(dllbases) \
			$(ovf2ovp_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(ovf2ovp_dll_name) \
		-export:$(ovf2ovpexport) \
		-implib:$(outdir)\ovf2ovp.lib \
		-out:$(outdir)\$(ovf2ovp_dll_name).dll \
		$(ovf2ovp_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\ovf2ovp.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			ovf2ovp-version.h \
			ovf2ovp.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ ovf2ovp.rc

$(outdir)\ovf2ovp.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\ovf2ovp.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(ovf2ovpexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\ovf2ovp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

ovf2ovp.cc ovf2ovp.h: ovf2ovp.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(ovf2ovpmain) \
		--include-filename=mywebapp.h \
		--one=ovf2ovp \
		--var-struct=$(ovf2ovpdata) \
		-C \
		ovf2ovp.p
	type << >> ovf2ovp.cc
MIKTEX_DEFINE_WEBAPP(
  $(ovf2ovpexport),
  $(ovf2ovpclass),
  $(ovf2ovpapp),
  $(ovf2ovpmain),
  $(ovf2ovpdata))
<<

ovf2ovp.p: ovf2ovp.web ovf2ovp-miktex.ch
	$(tangle) ovf2ovp.web ovf2ovp-miktex.ch ovf2ovp.p nul

# _____________________________________________________________________________
#
# ovp2ovf
# _____________________________________________________________________________

!ifndef ovp2ovfexport
ovp2ovfexport = MiKTeX_OVP2OVF
!endif

ovp2ovfapp = g_OVP2OVFApp
ovp2ovfclass = WebApp
ovp2ovfdata = g_OVP2OVFData
ovp2ovfmain = RunOVP2OVF

ovp2ovf_obj = \
	$(app_lib) \
	$(kps_lib) \
	$(miktex_lib) \
	$(outdir)\ovp2ovf.obj \
	$(outdir)\ovp2ovf.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(ovp2ovf_dll_name).dll $(outdir)\ovp2ovf.lib: \
			$(outdir) \
			$(dllbases) \
			$(ovp2ovf_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(ovp2ovf_dll_name) \
		-export:$(ovp2ovfexport) \
		-implib:$(outdir)\ovp2ovf.lib \
		-out:$(outdir)\$(ovp2ovf_dll_name).dll \
		$(ovp2ovf_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\ovp2ovf.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			ovp2ovf-version.h \
			ovp2ovf.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ ovp2ovf.rc

$(outdir)\ovp2ovf.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\ovp2ovf.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(ovp2ovfexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\ovp2ovf.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

ovp2ovf.cc ovp2ovf.h: ovp2ovf.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(ovp2ovfmain) \
		--include-filename=mywebapp.h \
		--one=ovp2ovf \
		--var-struct=$(ovp2ovfdata) \
		-C \
		ovp2ovf.p
	type << >> ovp2ovf.cc
MIKTEX_DEFINE_WEBAPP(
  $(ovp2ovfexport),
  $(ovp2ovfclass),
  $(ovp2ovfapp),
  $(ovp2ovfmain),
  $(ovp2ovfdata))
<<

ovp2ovf.p: ovp2ovf.web ovp2ovf-miktex.ch
	$(tangle) ovp2ovf.web ovp2ovf-miktex.ch ovp2ovf.p nul

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

mostlyclean: common-mostlyclean
	-del /f *.p
	-del /f mywebapp.h
	-del /f odvicopy.cc
	-del /f odvicopy.h
	-del /f ofm2opl.cc
	-del /f ofm2opl.h
	-del /f opl2ofm.cc
	-del /f opl2ofm.h
	-del /f ovf2ovp.cc
	-del /f ovf2ovp.h
	-del /f ovp2ovf.cc
	-del /f ovp2ovf.h

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: odvicopy.cc ofm2opl.cc opl2ofm.cc ovf2ovp.cc ovp2ovf.cc
	$(MAKEDEPEND) $(defines) $(includes) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\odvicopy.obj: odvicopy.h mywebapp.h
$(outdir)\ofm2opl.obj: ofm2opl.h mywebapp.h
$(outdir)\opl2ofm.obj: opl2ofm.h mywebapp.h
$(outdir)\ovf2ovp.obj: ovf2ovp.h mywebapp.h
$(outdir)\ovp2ovf.obj: ovp2ovf.h mywebapp.h
