## miktex.mak: dvicopy
## 
## Copyright (C) 1996-2005 Christian Schenk
## 
## This file is part of DVIcopy.
## 
## DVIcopy is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
## 
## DVIcopy is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with DVIcopy; if not, write to the Free Software Foundation,
## 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

!include <miktex.inc>

!ifndef dvicopyexport
dvicopyexport = MiKTeX_DVIcopy
!endif

dvicopyclass = CDVIcopyApp
dvicopymain = RunDVIcopy
dvicopyapp = g_DVIcopyApp
dvicopydata = g_DVIcopyData

defines = \
	-DDVICOPYAPP=$(dvicopyapp) \
	-DDVICOPYCLASS=$(dvicopyclass) \
	-DDVICOPYDATA=$(dvicopydata) \
	-DMIKTEX_DVICOPY \

.cc{$(outdir)\}.obj:
	$(cc) $(cppstandard) \
		$(defines) \
		$(ccopt_output_directory)$(outdir)\ \
		$<

binaries = \
	$(outdir)\$(dvicopy_dll_name).dll \
	$(outdir)\dvicopy.exe \

all: common-all $(binaries)

install: common-install install-binaries

check: common-check
	$(pushd) test & $(MAKE) -f $(miktex_mak) check & $(popd)

# -----------------------------------------------------------------------------
# dvicopy
# -----------------------------------------------------------------------------

dvicopy_obj = \
	$(app_lib) \
	$(miktex_lib) \
	$(outdir)\dvicopy.obj \
	$(outdir)\dvicopy.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\$(dvicopy_dll_name).dll $(outdir)\dvicopy.lib: \
			$(outdir) \
			$(dllbases) \
			$(dvicopy_obj) \

	$(link) $(dlllstandard) \
		$(linkopt_output_file)$(outdir)\$(dvicopy_dll_name).dll \
		-base:@$(dllbases),$(dvicopy_dll_name) \
		-export:$(dvicopyexport) \
		-implib:$(outdir)\dvicopy.lib \
		$(dvicopy_obj)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\dvicopy.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			dvicopy-version.h \
			dvicopy.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@  dvicopy.rc

$(outdir)\dvicopy.exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\dvicopy.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(dvicopyexport) \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\dvicopy.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

dvicopy.cc dvicopy.h: dvicopy.p
	$(c4p) \
		$(c4prenames) \
		--def-filename=dvicopydefs.h \
		--dll \
		--entry-name=$(dvicopymain) \
		--include-filename=dvicopy-miktex.h \
		--one=dvicopy \
		--var-name-prefix=m_ \
		--var-struct=$(dvicopydata) \
		-C \
		dvicopy.p
	type << >> dvicopy.cc
MIKTEX_DEFINE_WEBAPP(
  $(dvicopyexport),
  $(dvicopyclass),
  $(dvicopyapp),
  $(dvicopymain),
  $(dvicopydata))
<<

dvicopy.p: dvicopy.web dvicopy-miktex.ch
	$(tangle) dvicopy.web dvicopy-miktex.ch dvicopy.p nul

# -----------------------------------------------------------------------------
# clean-up
# -----------------------------------------------------------------------------

mostlyclean: common-mostlyclean
	-del /f dvicopy.cc
	-del /f dvicopy.h
	-del /f dvicopydefs.h
	-del /f *.p

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# -----------------------------------------------------------------------------
# dependencies
# -----------------------------------------------------------------------------

!include <common-dependencies.inc>

depend: dvicopy.cc
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\dvicopy.obj: dvicopy.h dvicopy-miktex.h dvicopy.rc dvicopy-version.h
$(outdir)\dvicopy.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/miktex.version
$(outdir)\dvicopy.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/version.h
$(outdir)\dvicopy.obj: dvicopydefs.h
$(outdir)\dvicopy.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/help.h
