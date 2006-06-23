## miktex.mak:
## 
## Copyright (C) 1996-2005 Christian Schenk
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
	$(outdir)\conspawn.exe \
	$(outdir)\copystart.exe \
	$(outdir)\findexe.exe \
	$(outdir)\neolecho.com \
	$(outdir)\runbat.exe \
	$(outdir)\runperl2.exe \

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_directory)$(outdir)\ $<

all: $(binaries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# findexe
# _____________________________________________________________________________

$(outdir)\findexe.exe: \
			$(outdir) \
			findexe.cpp \

	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_directory)$(outdir)\ \
		$(ccopt_quiet) \
		findexe.cpp \
		$(ccopt_link) \
			$(linkopt_output_file)$@

# _____________________________________________________________________________
#
# conspawn
# _____________________________________________________________________________

$(outdir)\conspawn.exe: \
			$(outdir) \
			conspawn.cpp \

	$(cc) \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_minimize_size) \
		$(ccopt_output_directory)$(outdir)\ \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_static_lib) \
		$(ccopt_whole_program_optimization) \
		$(cflagseh) \
		conspawn.cpp \
		$(ccopt_link) \
			$(linkopt_output_file)$@

# _____________________________________________________________________________
#
# copystart
# _____________________________________________________________________________

$(outdir)\copystart.exe: \
			$(outdir) \
			$(bz2_static_lib) \
			$(md5_static_lib) \
			$(miktex_static_lib) \
			$(zlib_static_lib) \
			copystart.cpp \

	$(cc) \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_minimize_size) \
		$(ccopt_output_directory)$(outdir)\
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_static_lib) \
		$(ccopt_whole_program_optimization) \
		$(cflagseh) \
		$(cvars) \
		$(incdirs) \
		copystart.cpp \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(bz2_static_lib) \
			$(md5_static_lib) \
			$(miktex_static_lib) \
			$(zlib_static_lib) \
			advapi32.lib \
			delayimp.lib \
			user32.lib \
			winspool.lib \
			/DELAYLOAD:advapi32.dll \
			/DELAYLOAD:user32.dll \
			/DELAYLOAD:winspool.drv \

# _____________________________________________________________________________
#
# runbat
# _____________________________________________________________________________

$(outdir)\runbat.exe: \
		$(outdir) \
		$(app_lib) \
		$(miktex_lib) \
		$(mpm_lib) \
		$(texmf_lib) \
		runbat.cpp \

	$(cc) \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_minimize_size) \
		$(ccopt_output_directory)$(outdir)\
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		$(ccopt_whole_program_optimization) \
		$(cflagseh) \
		$(cvars) \
		$(incdirs) \
		runbat.cpp \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(app_lib) \
			$(miktex_lib) \
			$(mpm_lib) \
			$(texmf_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# runperl2
# _____________________________________________________________________________

$(outdir)\runperl2.exe: \
		$(outdir) \
		$(app_lib) \
		$(miktex_lib) \
		$(mpm_lib) \
		$(texmf_lib) \
		runperl.cpp \

	$(cc) \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_minimize_size) \
		$(ccopt_output_directory)$(outdir)\ \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		$(ccopt_whole_program_optimization) \
		$(cflagseh) \
		$(cvars) \
		$(incdirs) \
		runperl.cpp \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(app_lib) \
			$(miktex_lib) \
			$(mpm_lib) \
			$(texmf_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# neolecho
# _____________________________________________________________________________

$(outdir)\neolecho.com: \
		$(outdir) \
		neolecho.cpp \

	$(cc) \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_minimize_size) \
		$(ccopt_output_directory)$(outdir)\ \
		$(ccopt_quiet) \
		$(ccopt_select_crt_mt_dll) \
		$(ccopt_whole_program_optimization) \
		$(cflagseh) \
		neolecho.cpp \
		$(ccopt_link) \
			$(linkopt_output_file)$@
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# cleanup
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

depend: runbat.cpp runperl.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
