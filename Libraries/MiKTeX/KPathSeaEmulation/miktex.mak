## miktex.mak:
##
## Copyright (C) 2000-2006 Christian Schenk
##
## This file is part of the MiKTeX KPSEMU Library.
##
## The MiKTeX KPSEMU Library is free software; you can redistribute it
## and/or modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2, or
## (at your option) any later version.
## 
## The MiKTeX KPSEMU Library is distributed in the hope that it will
## be useful, but WITHOUT ANY WARRANTY; without even the implied
## warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## See the GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with the MiKTeX KPSEMU Library; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

miktex_cc_warnings_as_errors = 1

!include <miktex.inc>

objects = \
	$(outdir)\StdAfx.obj \
	$(outdir)\kpsemu.obj \

cppfiles = \
	StdAfx.cpp \
	kpsemu.cpp \

sources = $(cppfiles)

defines = -DKPS_DLL

compile = $(cc) $(cppstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		$(defines) \
		$(ccopt_name_precompiled_header_file)$(outdir)\kpsemu.pch \
		$(ccopt_use_precompiled_header_file)StdAfx.h \

libraries = $(kps_lib)

kps_dll = $(kps_dll_name).dll

binaries = $(outdir)\$(kps_dll)

all: common-all

install: common-install install-binaries

check: common-check

.cpp{$(outdir)\}.obj:
	$(compile) $<

# _____________________________________________________________________________
#
# kpsemu.dll
# _____________________________________________________________________________

kps_dll_objects = \
	$(objects) \
	$(outdir)\kpsemu.res \
	$(miktex_lib) \
	$(texmf_lib) \

$(outdir)\$(kps_dll) $(kps_lib): \
			$(outdir) \
			$(dllbases) \
			$(kps_dll_objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(kps_dll_name) \
		-implib:$(kps_lib) \
		-out:$(outdir)\$(kps_dll) \
		$(kps_dll_objects) \
		delayimp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\kpsemu.res: \
			$(outdir) \
			$(libdir)\include\miktex\miktex.version \
			$(libdir)\include\miktex\version.h \
			kpsemu-version.h \
			kpsemu.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ kpsemu.rc

# _____________________________________________________________________________
#
# precompiled headers
# _____________________________________________________________________________

$(outdir)\StdAfx.obj: $(outdir) StdAfx.cpp
	$(compile) $(ccopt_create_precompiled_header_file)StdAfx.h StdAfx.cpp

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

depend:
	$(MAKEDEPEND) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\StdAfx.obj: StdAfx.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\kpsemu.obj: StdAfx.h
$(outdir)\kpsemu.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\kpsemu.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\kpsemu.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\kpsemu.obj: $(miktexsrcdir)/Libraries\3rd\libgnu/gnu-miktex.h
$(outdir)\kpsemu.obj: internal.h
