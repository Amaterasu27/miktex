## miktex.mak:
## 
## Copyright (C) 2003-2006 Christian Schenk
##
## This file is part of "MiKTeX Print Utility".
##
## "MiKTeX Print Utility" is free software; you can redistribute it
## and/or modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2, or
## (at your option) any later version.
## 
## "MiKTeX Print Utility" is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied warranty
## of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with "MiKTeX Print Utility"; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

miktex_cc_warnings_as_errors = 1

!include <miktex.inc>

binaries = $(outdir)\mtprint.exe

ccoptions = \
	$(cppstandard) \
	$(ccopt_output_directory)$(outdir)\ \
	$(ccopt_name_precompiled_header_file)$(outdir)\mtprint.pch \
	$(ccopt_use_precompiled_header_file)StdAfx.h \

.cpp{$(outdir)\}.obj:
	$(cc) $(ccoptions) $<

all: common-all $(binaries)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# mtprint
# _____________________________________________________________________________

mtprint_objects = \
	$(outdir)\StdAfx.obj \
	\
	$(app_lib) \
	$(dib_lib) \
	$(miktex_lib) \
	$(outdir)\BitmapPrinter.obj \
	$(outdir)\Printer.obj \
	$(outdir)\mtprint.obj \
	$(outdir)\mtprint.res \
	$(popt_lib) \
	$(texmf_lib) \

$(outdir)\mtprint.exe: \
			$(outdir) \
			$(mtprint_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(mtprint_objects) \
		gdi32.lib \
		user32.lib \
		winspool.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\mtprint.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			mtprint-version.h \
			mtprint.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ mtprint.rc

$(outdir)\StdAfx.obj: $(outdir) StdAfx.cpp
	$(cc) $(ccoptions) $(ccopt_create_precompiled_header_file)StdAfx.h StdAfx.cpp

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

depend: StdAfx.cpp mtprint.cpp Printer.cpp BitmapPrinter.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE

$(outdir)\StdAfx.obj: StdAfx.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\mtprint.obj: StdAfx.h
$(outdir)\mtprint.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\mtprint.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\mtprint.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\mtprint.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\mtprint.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\mtprint.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\mtprint.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\mtprint.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\mtprint.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\mtprint.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\mtprint.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h internal.h
$(outdir)\mtprint.obj: mtprint-version.h BitmapPrinter.h Printer.h
$(outdir)\Printer.obj: StdAfx.h
$(outdir)\Printer.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\Printer.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\Printer.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\Printer.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\Printer.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\Printer.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\Printer.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\Printer.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\Printer.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\Printer.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\Printer.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h internal.h
$(outdir)\Printer.obj: Printer.h
$(outdir)\BitmapPrinter.obj: StdAfx.h
$(outdir)\BitmapPrinter.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\BitmapPrinter.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\BitmapPrinter.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\BitmapPrinter.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\BitmapPrinter.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\BitmapPrinter.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\BitmapPrinter.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/help.h
$(outdir)\BitmapPrinter.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\BitmapPrinter.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\BitmapPrinter.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\BitmapPrinter.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\BitmapPrinter.obj: internal.h BitmapPrinter.h Printer.h
