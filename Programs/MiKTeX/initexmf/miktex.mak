## miktex.mak:
## 
## Copyright (C) 1996-2006 Christian Schenk
##
## This file is part of IniTeXMF.
##
## IniTeXMF is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
## 
## IniTeXMF is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with IniTeXMF; if not, write to the Free Software Foundation,
## 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

miktex_cc_warnings_as_errors = 1

!include <miktex.inc>

binaries = \
	$(outdir)\initexmf.exe \
	$(outdir)\mktexlsr.exe \
	$(outdir)\texhash.exe \
	$(outdir)\texlinks.exe \

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		$<

all: common-all $(binaries)

install: \
	common-install \
	install-binaries \
	$(inifiles) \
	$(configdir) \
	install-fndb \

check: common-check

# _____________________________________________________________________________
#
# initexmf
# _____________________________________________________________________________

initexmf_objects = \
	$(miktex_lib) \
	$(mpm_lib) \
	$(outdir)\initexmf.obj \
	$(outdir)\initexmf.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\initexmf.exe: $(outdir) $(initexmf_objects)
	$(link) $(lstandard) \
		$(linkopt_delayload)$(mktex_dll_name).dll \
		$(linkopt_delayload)$(mpm_dll_name).dll \
		$(linkopt_output_file)$@ \
		$(initexmf_objects) \
		$(conlibsdll) \
		user32.lib \
		shell32.lib \
		delayimp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\initexmf.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		initexmf-version.h \
		initexmf.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ initexmf.rc

# _____________________________________________________________________________
#
# mktexlsr
# _____________________________________________________________________________

$(outdir)\mktexlsr.exe: $(alias_cpp) $(miktex_lib)
	$(cc) \
		$(ccopt_enable_synchronous_exception_handling) \
		$(ccopt_minimize_size) \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll_retail) \
		-DPRE_ARGV=\"-u\",\"-v\" \
		-DREAL_NAME=\"initexmf\" \
		$(ccinc_lib) \
		$(ccinc_libmd5) \
		$(alias_cpp) \
		$(ccopt_output_file)$(outdir)\alias.obj \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(miktex_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# texhash
# _____________________________________________________________________________

$(outdir)\texhash.exe: $(outdir)\mktexlsr.exe
		copy $(outdir)\mktexlsr.exe $@

# _____________________________________________________________________________
#
# texlinks
# _____________________________________________________________________________

$(outdir)\texlinks.exe: $(alias_cpp) $(miktex_lib)
	$(cc) \
		$(ccopt_enable_synchronous_exception_handling) \
		$(ccopt_minimize_size) \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll_retail) \
		-DPRE_ARGV=\"--mklinks\",\"-v\" \
		-DREAL_NAME=\"initexmf\" \
		$(ccinc_lib) \
		$(ccinc_libmd5) \
		$(alias_cpp) \
		$(ccopt_output_file)$(outdir)\alias.obj \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(miktex_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# install fndb & ini files
# _____________________________________________________________________________

install-fndb: .fndb

.fndb:
	$(initexmf) --update-fndb

install-ini:
	for %f in ($(inifiles)) do ( \
		copy %f "$(configdir)" \
		 && $(initexmf) --add-file "$(configdir)\%f" \
	)

"$(configdir)":
	$(mkpath) "$(configdir)"

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

depend: initexmf.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\initexmf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\initexmf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\initexmf.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\initexmf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\initexmf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\initexmf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\initexmf.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\initexmf.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\initexmf.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\initexmf.obj: initexmf-version.h
