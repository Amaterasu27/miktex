## miktex.mak: mfware
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
	$(outdir)\$(gftodvi_dll_name).dll $(outdir)\gftodvi.exe \
	$(outdir)\$(gftopk_dll_name).dll $(outdir)\gftopk.exe \
	$(outdir)\$(gftype_dll_name).dll $(outdir)\gftype.exe \
	$(outdir)\$(mft_dll_name).dll $(outdir)\mft.exe \

.cc{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# gftodvi
# _____________________________________________________________________________

!ifndef gftodviexport
gftodviexport = MiKTeX_Gftodvi
!endif

gftodviapp = g_GFtoDVIApp
gftodviclass = CGFtoDVIApp
gftodvidata = g_GFtoDVIData
gftodvimain = RunGFtoDVI

gftodvi_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\gftodvi.obj \
	$(outdir)\gftodvi.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(gftodvi_dll_name).dll $(outdir)\gftodvi.lib: \
			$(outdir) \
			$(dllbases) \
			$(gftodvi_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(gftodvi_dll_name) \
		-export:$(gftodviexport) \
		-implib:$(outdir)\gftodvi.lib \
		-out:$(outdir)\$(gftodvi_dll_name).dll \
		$(gftodvi_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\gftodvi.res: \
			$(outdir) \
			gftodvi.rc \
			gftodvi-version.h \
			$(libdir)\include\miktex\miktex.version \
			$(libdir)\include\miktex\version.h \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ gftodvi.rc

$(outdir)\gftodvi.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\gftodvi.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(gftodviexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\gftodvi.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\gftodvi.obj: gftodvi.cc gftodvi.h
	$(cc) $(cppstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		-DGFTODVIAPP=$(gftodviapp) \
		-DGFTODVICLASS=$(gftodviclass) \
		-DGFTODVIDATA=$(gftodvidata) \
		gftodvi.cc

gftodvi.cc gftodvi.h: gftodvi.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(gftodvimain) \
		--include-filename=gftodvi-miktex.h \
		--one=gftodvi \
		--var-name-prefix=m_ \
		--var-struct=$(gftodvidata) \
		-C \
		gftodvi.p
	type << >> gftodvi.cc
MIKTEX_DEFINE_WEBAPP(
  $(gftodviexport),
  $(gftodviclass),
  $(gftodviapp),
  $(gftodvimain),
  $(gftodvidata))
<<

gftodvi.p: gftodvi.web gftodvi-miktex.ch
	$(tangle) gftodvi.web gftodvi-miktex.ch gftodvi.p nul

# _____________________________________________________________________________
#
# gftopk
# _____________________________________________________________________________

!ifndef gftopkexport
gftopkexport = MiKTeX_Gftopk
!endif

gftopkapp = g_GFtoPKApp
gftopkclass = WebApp
gftopkdata = g_GFtoPKData
gftopkmain = RunGFtoPK

gftopk_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\gftopk.obj \
	$(outdir)\gftopk.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(gftopk_dll_name).dll $(outdir)\gftopk.lib: \
			$(outdir) \
			$(dllbases) \
			$(gftopk_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(gftopk_dll_name) \
		-export:$(gftopkexport) \
		-implib:$(outdir)\gftopk.lib \
		-out:$(outdir)\$(gftopk_dll_name).dll \
		$(gftopk_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\gftopk.res: \
			$(outdir) \
			gftopk.rc \
			gftopk-version.h \
			$(libdir)\include\miktex\miktex.version \
			$(libdir)\include\miktex\version.h \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ gftopk.rc

$(outdir)\gftopk.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\gftopk.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(gftopkexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\gftopk.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

gftopk.cc gftopk.h: gftopk.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(gftopkmain) \
		--include-filename=mywebapp.h \
		--one=gftopk \
		--var-struct=$(gftopkdata) \
		-C \
		gftopk.p
	type << >> gftopk.cc
MIKTEX_DEFINE_WEBAPP(
  $(gftopkexport),
  $(gftopkclass),
  $(gftopkapp),
  $(gftopkmain),
  $(gftopkdata))
<<

mywebapp.h:
	type << >> $@
#include <miktex/webapp.h>
using namespace MiKTeX::TeXAndFriends;
<<

$(outdir)\gftopk.obj: mywebapp.h

gftopk.p: gftopk.web gftopk-miktex.ch
	$(tangle) gftopk.web gftopk-miktex.ch gftopk.p nul

# _____________________________________________________________________________
#
# gftype
# _____________________________________________________________________________

!ifndef gftypeexport
gftypeexport = MiKTeX_Gftype
!endif

gftypeapp = g_GftypeApp
gftypeclass = WebApp
gftypedata = g_GftypeData
gftypemain = RunGftype

gftype_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\gftype.obj \
	$(outdir)\gftype.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(gftype_dll_name).dll $(outdir)\gftype.lib: \
			$(outdir) \
			$(gftype_obj) \
			$(dllbases) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(gftype_dll_name) \
		-export:$(gftypeexport) \
		-implib:$(outdir)\gftype.lib \
		-out:$(outdir)\$(gftype_dll_name).dll \
		$(gftype_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\gftype.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			gftype-version.h \
			gftype.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ gftype.rc

$(outdir)\gftype.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\gftype.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(gftypeexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\gftype.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

gftype.cc gftype.h: gftype.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(gftypemain) \
		--include-filename=mywebapp.h \
		--one=gftype \
		--var-struct=$(gftypedata) \
		-C \
		gftype.p
	type << >> gftype.cc
MIKTEX_DEFINE_WEBAPP(
  $(gftypeexport),
  $(gftypeclass),
  $(gftypeapp),
  $(gftypemain),
  $(gftypedata))
<<

$(outdir)\gftype.obj: mywebapp.h

gftype.p: gftype.web gftype-miktex.ch
	$(tangle) gftype.web gftype-miktex.ch gftype.p nul

# _____________________________________________________________________________
#
# mft
# _____________________________________________________________________________

!ifndef mftexport
mftexport = MiKTeX_MFT
!endif

mftapp = g_MFTApp
mftclass = WebApp
mftdata = g_MFTData
mftmain = RunMFT

mft_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\mft.obj \
	$(outdir)\mft.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(mft_dll_name).dll $(outdir)\mft.lib: \
			$(outdir) \
			$(dllbases) \
			$(mft_obj) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(mft_dll_name) \
		-export:$(mftexport) \
		-implib:$(outdir)\mft.lib \
		-out:$(outdir)\$(mft_dll_name).dll \
		$(mft_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\mft.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			mft-version.h \
			mft.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ mft.rc

$(outdir)\mft.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\mft.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(mftexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\mft.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

mft.cc mft.h: mft.p
	$(c4p) \
		$(c4prenames) \
		--dll \
		--entry-name=$(mftmain) \
		--include-filename=mywebapp.h \
		--one=mft \
		--var-struct=$(mftdata) \
		-C \
		mft.p
	type << >> mft.cc
MIKTEX_DEFINE_WEBAPP(
  $(mftexport),
  $(mftclass),
  $(mftapp),
  $(mftmain),
  $(mftdata))
<<

$(outdir)\mft.obj: mywebapp.h

mft.p: mft.web mft-miktex.ch
	$(tangle) mft.web mft-miktex.ch mft.p nul

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

mostlyclean: common-mostlyclean
	-del /f *.cc
	-del /f *.p
	-del /f gftodvi.h
	-del /f gftopk.h
	-del /f gftype.h
	-del /f mft.h
	-del /f mywebapp.h

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

depend: gftopk.cc gftype.cc gftodvi.cc mft.cc
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\gftopk.obj: gftopk.h
$(outdir)\gftopk.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\gftopk.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\gftopk.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\gftopk.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\gftype.obj: gftype.h
$(outdir)\gftype.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\gftype.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\gftype.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\gftype.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\gftodvi.obj: gftodvi.h
$(outdir)\gftodvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\gftodvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\gftodvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\gftodvi.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\gftodvi.obj: gftodvi-miktex.h gftodvi.rc gftodvi-version.h
$(outdir)\gftodvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\gftodvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\gftodvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\gftodvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\gftodvi.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\gftodvi.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\gftodvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\gftodvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\gftodvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\gftodvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.inl
$(outdir)\mft.obj: mft.h
$(outdir)\mft.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\mft.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\mft.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\mft.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
