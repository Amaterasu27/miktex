## mcd.cpp:
##
## Copyright (C) 1998-2006 Christian Schenk
##
## This file is part of the MiKTeX Compiler Driver.
## 
## The MiKTeX Compiler Driver is free software; you can redistribute
## it and/or modify it under the terms of the GNU General Public
## License as published by the Free Software Foundation; either
## version 2, or (at your option) any later version.
## 
## The MiKTeX Compiler Driver is distributed in the hope that it will
## be useful, but WITHOUT ANY WARRANTY; without even the implied
## warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## See the GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with The MiKTeX Compiler Driver; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA. */

miktex_cc_warnings_as_errors = 1

!include <miktex.inc>

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_directory)$(outdir)\ $<

i = i
f = f
y = y

TheNameOfTheGame = tex$(i)$(f)$(y)

binaries = $(outdir)\$(mcd_dll_name).dll $(outdir)\$(TheNameOfTheGame).exe

all: common-all

install: common-install install-binaries

check: common-crt

# _____________________________________________________________________________
#
# mcd.dll
# _____________________________________________________________________________

objects = \
	$(app_lib) \
	$(gnu_lib) \
	$(miktex_lib) \
	$(outdir)\mcd.obj \
	$(outdir)\mcd.res \
	$(popt_lib) \
	$(regex_lib) \
	$(texmf_lib) \

$(outdir)\$(mcd_dll_name).dll $(mcd_lib): \
			$(outdir) \
			$(dllbases) \
			$(objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(mcd_dll_name) \
		-export:mcdmain \
		-implib:$(mcd_lib) \
		-out:$(outdir)\$(mcd_dll_name).dll \
		$(objects) \
		shell32.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\mcd.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			mcd-version.h \
			mcd.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ mcd.rc

# _____________________________________________________________________________
#
# program
# _____________________________________________________________________________

$(outdir)\$(TheNameOfTheGame).exe: \
			$(outdir) \
			$(alias_cpp) \
			$(outdir)\mcd.lib \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		-DDLLMAIN=mcdmain \
		$(alias_cpp) \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(outdir)\mcd.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: mcd.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\mcd.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\mcd.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\mcd.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\mcd.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\mcd.obj: $(miktexsrcdir)/Libraries\MiKTeX\PackageManager/include/miktex/mpm.h
$(outdir)\mcd.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\mcd.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/env.h
$(outdir)\mcd.obj: $(miktexsrcdir)/Libraries\3rd\libpopt/popt-miktex.h
$(outdir)\mcd.obj: $(miktexsrcdir)/Libraries\3rd\libpopt/popt.h
$(outdir)\mcd.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/regex.h mcd-version.h
