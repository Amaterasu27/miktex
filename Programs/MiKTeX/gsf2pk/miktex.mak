## miktex.mak:
## 
## Copyright (C) 2004-2006 Christian Schenk
##
## This file is part of Gsf2pk.
##
## Gsf2pk is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
## 
## Gsf2pk is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with Gsf2pk; if not, write to the Free Software Foundation,
## 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

miktex_cc_warnings_as_errors = 1

!include <miktex.inc>

binaries = $(outdir)\gsf2pk.exe

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# gsf2pk
# _____________________________________________________________________________

gsf2pk_objects = \
	$(app_lib) \
	$(miktex_lib) \
	$(popt_lib) \
	$(outdir)\gsf2pk.obj \
	$(outdir)\gsf2pk.res \
	$(texmf_lib) \

$(outdir)\gsf2pk.exe: $(outdir) $(gsf2pk_objects)
	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(gsf2pk_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\gsf2pk.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		gsf2pk-version.h \
		gsf2pk.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ gsf2pk.rc

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

depend: gsf2pk.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\gsf2pk.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\gsf2pk.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\gsf2pk.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\gsf2pk.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\gsf2pk.obj: $(miktexsrcdir)/Libraries\MiKTeX\PackageManager/include/miktex/mpm.h
$(outdir)\gsf2pk.obj: $(miktexsrcdir)/Libraries\3rd\libpopt/popt-miktex.h
$(outdir)\gsf2pk.obj: $(miktexsrcdir)/Libraries\3rd\libpopt/popt.h
$(outdir)\gsf2pk.obj: gsf2pk-version.h
