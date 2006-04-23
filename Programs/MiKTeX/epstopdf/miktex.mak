## miktex.mak:
## 
## Copyright (C) 2000-2006 Christian Schenk
## 
## This file is part of epstopdf.
##
## epstopdf is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
## 
## epstopdf is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with epstopdf; if not, write to the Free Software Foundation,
## 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

miktex_cc_warnings_as_errors = 1

!include <miktex.inc>

binaries = $(outdir)\epstopdf.exe

cc_options = $(cflags) $(cdebug) $(cvarsdll) $(cflagseh) \
		$(incdirs) -I$(mktexlibdir) $(miktexvars)

.cpp{$(outdir)\}.obj:
	$(cc) $(cc_options) $(cflagseh) $(ccopt_output_directory)$(outdir)\ $<

all: common-all $(binaries)

install: common-install install-binaries 

check: common-check

# _____________________________________________________________________________
#
# epstopdf
# _____________________________________________________________________________

epstopdf_objects = \
	$(app_lib) \
	$(miktex_lib) \
	$(mktex_lib) \
	$(outdir)\epstopdf.obj \
	$(outdir)\epstopdf.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\epstopdf.exe: $(outdir) $(epstopdf_objects)
	$(link) $(conlflags) $(ldebug) \
		-out:$@ \
		$(epstopdf_objects) $(conlibsdll) user32.lib delayimp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\epstopdf.res: $(outdir) epstopdf.rc $(libdir)\include\miktex\miktex.version \
				$(libdir)\include\miktex\version.h
	$(rc) $(rcstandard) $(rcopt_output_file)$@ epstopdf.rc

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

depend: epstopdf.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\epstopdf.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\epstopdf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\epstopdf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\epstopdf.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\epstopdf.obj: $(miktexsrcdir)/Libraries\MiKTeX\PackageManager/include/miktex/mpm.h
$(outdir)\epstopdf.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\epstopdf.obj: $(miktexsrcdir)/Libraries\3rd\libpopt/popt-miktex.h
$(outdir)\epstopdf.obj: $(miktexsrcdir)/Libraries\3rd\libpopt/popt.h
$(outdir)\epstopdf.obj: epstopdf-version.h
