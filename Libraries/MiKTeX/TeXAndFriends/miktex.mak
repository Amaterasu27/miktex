## miktex.mak: MiKTeX TeXMF library
##
## Copyright (C) 1996-2006 Christian Schenk
##
## This file is part of the MiKTeX TeXMF Library.
##
## The MiKTeX TeXMF Library is free software; you can redistribute it
## and/or modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2, or
## (at your option) any later version.
## 
## The MiKTeX TeXMF Library is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied warranty
## of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with the MiKTeX TeXMF Library; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

miktex_cc_warnings_as_errors = 1
miktex_cc_64bit_portability = 1

## experimental
use_mikui = 1

!include <miktex.inc>

objects = \
	$(outdir)\StdAfx.obj \
	$(outdir)\c4plib.obj \
	$(outdir)\c4pstart.obj \
	$(outdir)\inputline.obj \
	$(outdir)\mfapp.obj \
	$(outdir)\mfmpi386asm.obj \
	$(outdir)\texapp.obj \
	$(outdir)\texmfapp.obj \
	$(outdir)\texmflib.obj \
	$(outdir)\webapp.obj \

cppfiles = \
	StdAfx.cpp \
	c4plib.cpp \
	c4pstart.cpp \
	inputline.cpp \
	mfapp.cpp \
	mfmpi386asm.cpp \
	texapp.cpp \
	texmfapp.cpp \
	texmflib.cpp \
	webapp.cpp \

sources = $(cppfiles)

defines = -DTEXMF_DLL

!if $(use_mikui) > 0
defines = $(defines) -DUSE_MIKUI -D_AFXDLL
!endif

compile = \
	$(cc) $(cppstandard) \
		$(ccinc_libmikui) \
		$(ccopt_output_directory)$(outdir)\ \
		$(defines) \
		$(ccopt_name_precompiled_header_file)$(outdir)\texmf.pch \
		$(ccinc_libbz2) \
		$(ccinc_zlib) \
		$(ccopt_use_precompiled_header_file)StdAfx.h \

texmf_dll = $(texmf_dll_name).dll

binaries = $(outdir)\$(texmf_dll)

all: iniheaders common-all

install: iniheaders common-install install-binaries

iniheaders: \
	include\miktex\mfapp.defaults.h \
	include\miktex\texapp.defaults.h \
	include\miktex\texmfapp.defaults.h \

check: common-check

.cpp{$(outdir)\}.obj:
	$(compile) $<

# _____________________________________________________________________________
#
# texmfapp.defaults.h
# _____________________________________________________________________________

texmfappini = $(miktexsrcdir)\$(rel_defaultsdir)\texmfapp.defaults

include\miktex\texmfapp.defaults.h: $(texmfappini)
	cfg --print-classes $(texmfappini) > $@

# _____________________________________________________________________________
#
# mfapp.defaults.h
# _____________________________________________________________________________

mfappini = $(miktexsrcdir)\$(rel_defaultsdir)\mfapp.defaults

include\miktex\mfapp.defaults.h: $(mfappini)
	cfg --print-classes $(mfappini) > $@

# _____________________________________________________________________________
#
# texapp.defaults.h
# _____________________________________________________________________________

texappini = $(miktexsrcdir)\$(rel_defaultsdir)\texapp.defaults

include\miktex\texapp.defaults.h: $(texappini)
	cfg --print-classes $(texappini) > $@

# _____________________________________________________________________________
#
# texmf.dll
# _____________________________________________________________________________

texmf_dll_objects = \
	$(app_lib) \
	$(bz2_lib) \
	$(miktex_lib) \
	$(mpm_lib) \
	$(objects) \
	$(outdir)\texmf.res \
	$(popt_lib) \
	$(zlib_lib) \

$(outdir)\$(texmf_dll) $(texmf_lib): \
			$(dllbases) \
			$(outdir) \
			$(texmf_dll_objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(texmf_dll_name) \
		-delayload:$(bzip2_dll_name).dll \
		-delayload:$(mpm_dll_name).dll \
		-delayload:$(zlib_dll_name).dll \
		-implib:$(texmf_lib) \
		-out:$(outdir)\$(texmf_dll) \
		$(texmf_dll_objects) \
		user32.lib \
		delayimp.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\texmf.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			texmf.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ texmf.rc

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
	if exist include\miktex\mfapp.defaults.h \
		del include\miktex\mfapp.defaults.h
	if exist include\miktex\texapp.defaults.h \
		del include\miktex\texapp.defaults.h
	if exist include\miktex\texmfapp.defaults.h \
		del include\miktex\texmfapp.defaults.h

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
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\c4plib.obj: StdAfx.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h internal.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/mfapp.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\c4plib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\c4pstart.obj: StdAfx.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h internal.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/mfapp.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\c4pstart.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\inputline.obj: StdAfx.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\inputline.obj: internal.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/mfapp.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\inputline.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\mfapp.obj: StdAfx.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h internal.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/mfapp.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\mfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\mfmpi386asm.obj: StdAfx.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\mfmpi386asm.obj: internal.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/mfapp.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\mfmpi386asm.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\texapp.obj: StdAfx.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h internal.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/mfapp.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\texapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\texmfapp.obj: StdAfx.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h internal.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/mfapp.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\texmfapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\texmflib.obj: StdAfx.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h internal.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/mfapp.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\texmflib.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
$(outdir)\webapp.obj: StdAfx.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h internal.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/c4plib.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/mfapp.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/inputline.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/webapp.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/miktex-texmf.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texmfapp.defaults.h
$(outdir)\webapp.obj: $(miktexsrcdir)/Libraries/MiKTeX/TeXAndFriends/include/miktex/texapp.h
