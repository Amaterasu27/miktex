## miktex.mak:
## 
## Copyright (C) 2006 Christian Schenk
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
	$(outdir)\migrate.exe \

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_directory)$(outdir)\ $<

all: $(binaries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# migrate
# _____________________________________________________________________________

$(outdir)\migrate.exe: \
			$(outdir) \
			$(md5_static_lib) \
			$(miktex_static_lib) \
			migrate.cpp \

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
		migrate.cpp \
		$(ccopt_link) \
			$(linkopt_output_file)$@ \
			$(md5_static_lib) \
			$(miktex_static_lib) \
			advapi32.lib \
			delayimp.lib \
			user32.lib \
			winspool.lib \
			/DELAYLOAD:advapi32.dll \
			/DELAYLOAD:user32.dll \
			/DELAYLOAD:winspool.drv \

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

depend: migrate.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE
