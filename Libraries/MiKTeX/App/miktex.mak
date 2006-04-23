## miktex.mak:
##
## Copyright (C) 2005-2006 Christian Schenk
##
## This file is part of the MiKTeX App Library.
##
## The MiKTeX App Library is free software; you can redistribute it
## and/or modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2, or
## (at your option) any later version.
## 
## The MiKTeX App Library is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied warranty
## of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with the MiKTeX App Library; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

miktex_cc_warnings_as_errors = 1
miktex_cc_64bit_portability = 1

## experimental
use_mikui = 1

!include <miktex.inc>

objects = \
	$(outdir)\StdAfx.obj \
	$(outdir)\app.obj \

cppfiles = \
	StdAfx.cpp \
	app.cpp \

sources = $(cppfiles)

defines = -DAPP_DLL

!if $(use_mikui) > 0
defines = $(defines) -DUSE_MIKUI -D_AFXDLL
!endif

compile = \
	$(cc) $(cppstandard) \
		$(ccinc_libmikui) \
		$(ccopt_output_directory)$(outdir)\ \
		$(defines) \
		$(ccopt_name_precompiled_header_file)$(outdir)\app.pch \
		$(ccinc_libbz2) \
		$(ccinc_zlib) \
		$(ccopt_use_precompiled_header_file)StdAfx.h \

app_dll = $(app_dll_name).dll

binaries = $(outdir)\$(app_dll)

all: common-all

install: common-install install-binaries

check: common-check

.cpp{$(outdir)\}.obj:
	$(compile) $<

# _____________________________________________________________________________
#
# app.dll
# _____________________________________________________________________________

app_dll_objects = \
	$(miktex_lib) \
!if $(use_mikui) > 0
	$(mikui_lib) \
!endif
	$(mpm_lib) \
	$(objects) \
	$(outdir)\app.res \

$(outdir)\$(app_dll) $(app_lib): \
			$(dllbases) \
			$(outdir) \
			$(app_dll_objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(app_dll_name) \
!if $(use_mikui) > 0
		-delayload:$(mikui_dll_name).dll \
!endif
		-delayload:$(mpm_dll_name).dll \
		-implib:$(app_lib) \
		-out:$(outdir)\$(app_dll) \
		$(app_dll_objects) \
		user32.lib \
		delayimp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\app.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			app.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ app.rc

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
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\app.obj: StdAfx.h
$(outdir)\app.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\app.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\app.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\app.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\app.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\app.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\app.obj: internal.h
$(outdir)\app.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\app.obj: $(miktexsrcdir)/Libraries\MiKTeX\PackageManager/include/miktex/mpm.h
