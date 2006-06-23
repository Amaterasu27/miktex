## miktex.mak:
## 
## Copyright (C) 1998-2006 Christian Schenk
## 
## This file is part of the MiKTeX Maker Library.
## 
## The MiKTeX Maker Library is free software; you can redistribute it
## and/or modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2, or
## (at your option) any later version.
## 
## The MiKTeX Maker Library is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied warranty
## of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with the MiKTeX Maker Library; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA. */

miktex_cc_warnings_as_errors = 1

!include <miktex.inc>

apps =	\
	$(outdir)\makebase.exe \
	$(outdir)\makefmt.exe \
	$(outdir)\makemem.exe \
	$(outdir)\makemf.exe \
	$(outdir)\makepk.exe \
	$(outdir)\maketfm.exe \

binaries = $(outdir)\$(makex_dll_name).dll $(apps)

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_directory)$(outdir)\ $<

all: common-all

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# makex.dll
# _____________________________________________________________________________

objects = \
	$(app_lib) \
	$(gnu_lib) \
	$(miktex_lib) \
	$(texmf_lib) \
	$(outdir)\common.obj \
	$(outdir)\makebase.obj \
	$(outdir)\makefmt.obj \
	$(outdir)\makemem.obj \
	$(outdir)\makemf.obj \
	$(outdir)\makepk.obj \
	$(outdir)\maketfm.obj \
	$(outdir)\makex.res \

$(outdir)\$(makex_dll_name).dll $(makex_lib): \
			$(outdir) \
			$(dllbases) \
			$(objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(makex_dll_name) \
		-implib:$(makex_lib) \
		-out:$(outdir)\$(makex_dll_name).dll \
		$(objects) delayimp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\makex.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		makex.rc \
		makex-version.h \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ makex.rc

# _____________________________________________________________________________
#
# makebase.exe
# _____________________________________________________________________________

$(outdir)\makebase.exe: \
		$(outdir) \
		$(alias_cpp) \
		$(makex_lib) \

	$(cc) \
		$(alias_cpp) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLMAIN=makebase \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(makex_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# makefmt.exe
# _____________________________________________________________________________

$(outdir)\makefmt.exe: \
		$(outdir) \
		$(alias_cpp) \
		$(makex_lib) \

	$(cc) \
		$(alias_cpp) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLMAIN=makefmt \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(makex_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# makemem.exe
# _____________________________________________________________________________

$(outdir)\makemem.exe: \
		$(outdir) \
		$(alias_cpp) \
		$(makex_lib) \

	$(cc) \
		$(alias_cpp) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLMAIN=makemem \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(makex_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# makemf.exe
# _____________________________________________________________________________

$(outdir)\makemf.exe: \
		$(outdir) \
		$(alias_cpp) \
		$(makex_lib) \

	$(cc) \
		$(alias_cpp) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLMAIN=makemf \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(makex_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# makepk.exe
# _____________________________________________________________________________

$(outdir)\makepk.exe: \
		$(outdir) \
		$(alias_cpp) \
		$(makex_lib) \

	$(cc) \
		$(alias_cpp) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLMAIN=makepk \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(makex_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# maketfm.exe
# _____________________________________________________________________________

$(outdir)\maketfm.exe: \
		$(outdir) \
		$(alias_cpp) \
		$(makex_lib) \

	$(cc) \
		$(alias_cpp) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLMAIN=maketfm \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(makex_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

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

depend: \
		common.cpp \
		makebase.cpp \
		makefmt.cpp \
		makemem.cpp \
		makemf.cpp \
		makepk.cpp \
		maketfm.cpp \

	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\common.obj: internal.h $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\common.obj: makex.h makex-version.h
$(outdir)\makebase.obj: internal.h
$(outdir)\makebase.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h
$(outdir)\makebase.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\makebase.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\makebase.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\makebase.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\makebase.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\makebase.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\makebase.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\makebase.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\makebase.obj: makex.h
$(outdir)\makefmt.obj: internal.h
$(outdir)\makefmt.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h
$(outdir)\makefmt.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\makefmt.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\makefmt.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\makefmt.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\makefmt.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\makefmt.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\makefmt.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\makefmt.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\makefmt.obj: makex.h
$(outdir)\makemem.obj: internal.h
$(outdir)\makemem.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h
$(outdir)\makemem.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\makemem.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\makemem.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\makemem.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\makemem.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\makemem.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\makemem.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\makemem.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\makemem.obj: makex.h
$(outdir)\makemf.obj: internal.h $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h
$(outdir)\makemf.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\makemf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\makemf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\makemf.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\makemf.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\makemf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\makemf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\makemf.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\makemf.obj: makex.h
$(outdir)\makepk.obj: internal.h $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h
$(outdir)\makepk.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\makepk.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\makepk.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\makepk.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\makepk.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\makepk.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\makepk.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\makepk.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\makepk.obj: makex.h
$(outdir)\maketfm.obj: internal.h
$(outdir)\maketfm.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h
$(outdir)\maketfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\maketfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\maketfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\maketfm.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\maketfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\maketfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\maketfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\maketfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\maketfm.obj: makex.h
