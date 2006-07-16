## miktex.mak:
##
## Copyright (C) 1996-2006 Christian Schenk
## 
## This file is part of the MiKTeX DVI Library.
## 
## The MiKTeX DVI Library is free software; you can redistribute it
## and/or modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2, or
## (at your option) any later version.
## 
## The MiKTeX DVI Library is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied warranty
## of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with the MiKTeX DVI Library; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

miktex_cc_warnings_as_errors = 1
miktex_cc_buffer_security_check = 1

miktex_enable_pgo = 1

!include <miktex.inc>

objects = \
	$(outdir)\StdAfx.obj \
	$(outdir)\Dvi.obj \
	$(outdir)\DviChar.obj \
	$(outdir)\DviFont.obj \
	$(outdir)\DviPage.obj \
	$(outdir)\PkChar.obj \
	$(outdir)\PkFont.obj \
	$(outdir)\Tfm.obj \
	$(outdir)\VFont.obj \
	$(outdir)\VfChar.obj \
	$(outdir)\color.obj \
	$(outdir)\common.obj \
	$(outdir)\graphics.obj \
	$(outdir)\hypertex.obj \
	$(outdir)\ps.obj \
	$(outdir)\special.obj \
	$(outdir)\src.obj \
	$(outdir)\tpic.obj \

sources = \
	StdAfx.cpp \
	Dvi.cpp \
	DviChar.cpp \
	DviFont.cpp \
	DviPage.cpp \
	PkChar.cpp \
	PkFont.cpp \
	Tfm.cpp \
	VFont.cpp \
	VfChar.cpp \
	color.cpp \
	common.cpp \
	graphics.cpp \
	hypertex.cpp \
	ps.cpp \
	special.cpp \
	src.cpp \
	tpic.cpp \

libraries = $(dvi_lib)

dvi_dll = $(dvi_dll_name).dll

binaries = $(outdir)\$(dvi_dll) $(outdir)\dviscan.exe

compile = \
	$(cc) $(cppstandard) \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_enable_rtti_support) \
		$(ccopt_name_precompiled_header_file)$(outdir)\dvi.pch \
		$(ccopt_output_file)$@ \
		$(ccopt_use_precompiled_header_file)StdAfx.h \
		-DDVI_DLL \
		-DIN_DVI_DLL \
		-DNO_C4P \

.cpp{$(outdir)\}.obj:
	 $(compile) $<

all: common-all $(libraries)

install: common-install install-binaries

check: common-check
	pushd test & $(MAKE) -f $(miktex_mak) check & popd

# _____________________________________________________________________________
#
# dvi.dll
# _____________________________________________________________________________

$(outdir)\$(dvi_dll) $(dvi_lib): \
			$(outdir) \
			$(dllbases) \
			$(dib_lib) \
			$(miktex_lib) \
			$(objects) \
			$(outdir)\dvi.res \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(dvi_dll_name) \
		-implib:$(dvi_lib) \
		-out:$(outdir)\$(dvi_dll) \
		$(objects) \
		$(dib_lib) \
		$(miktex_lib) \
		$(outdir)\dvi.res
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\dvi.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			dvi-version.h \
			dvi.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ dvi.rc

# _____________________________________________________________________________
#
# precompiled header
# _____________________________________________________________________________

$(outdir)\StdAfx.obj: $(outdir) StdAfx.cpp
	$(compile) $(ccopt_create_precompiled_header_file)StdAfx.h StdAfx.cpp

# _____________________________________________________________________________
#
# dviscan
# _____________________________________________________________________________

$(outdir)\dviscan.obj: \
			$(outdir) \
			dviscan.cpp \

	$(cc) $(cppstandard) \
		$(ccopt_enable_rtti_support) \
		$(ccopt_output_file)$@ \
		-DDVI_DLL \
		-DNO_C4P \
		dviscan.cpp

$(outdir)\dviscan.exe: \
			$(outdir) \
			$(app_lib) \
			$(dvi_lib) \
			$(gnu_lib) \
			$(miktex_lib) \
			$(outdir)\dviscan.obj \
			$(popt_lib) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(app_lib) \
		$(dvi_lib) \
		$(gnu_lib) \
		$(miktex_lib) \
		$(outdir)\dviscan.obj \
		$(popt_lib)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

# _____________________________________________________________________________
#
# bitcounts.h
# _____________________________________________________________________________

bitcounts.h: $(outdir)\makebitcounts.exe
	$(outdir)\makebitcounts > bitcounts.h

$(outdir)\makebitcounts.exe: $(outdir) makebitcounts.cpp
	$(cc) $(ccopt_quiet) \
		$(ccopt_output_file)$(outdir)\makebitcounts.obj \
		makebitcounts.cpp \
		$(ccopt_link) \
			$(linkopt_output_file)$@

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean
	if exist bitcounts.h del /f bitcounts.h

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend: $(sources) dviscan.cpp
	$(MAKEDEPEND) $(sources) dviscan.cpp
	$(fixdep)

$(outdir)\PkChar.obj: $(outdir) bitcounts.h

# DO NOT DELETE

$(outdir)\StdAfx.obj: StdAfx.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\Dvi.obj: StdAfx.h
$(outdir)\Dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\Dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\Dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\Dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\Dvi.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\Dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\Dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\Dvi.obj: common.h
$(outdir)\Dvi.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\Dvi.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h VfChar.h
$(outdir)\Dvi.obj: inliners.h
$(outdir)\DviChar.obj: StdAfx.h
$(outdir)\DviChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\DviChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\DviChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\DviChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\DviChar.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\DviChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\DviChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\DviChar.obj: common.h
$(outdir)\DviChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\DviChar.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h
$(outdir)\DviChar.obj: VfChar.h inliners.h
$(outdir)\DviFont.obj: StdAfx.h
$(outdir)\DviFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\DviFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\DviFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\DviFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\DviFont.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\DviFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\DviFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\DviFont.obj: common.h
$(outdir)\DviFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\DviFont.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h
$(outdir)\DviFont.obj: VfChar.h inliners.h
$(outdir)\DviPage.obj: StdAfx.h
$(outdir)\DviPage.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\DviPage.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\DviPage.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\DviPage.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\DviPage.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\DviPage.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\DviPage.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\DviPage.obj: common.h
$(outdir)\DviPage.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\DviPage.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h
$(outdir)\DviPage.obj: VfChar.h inliners.h
$(outdir)\PkChar.obj: StdAfx.h
$(outdir)\PkChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\PkChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\PkChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\PkChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\PkChar.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\PkChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\PkChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\PkChar.obj: common.h
$(outdir)\PkChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\PkChar.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h
$(outdir)\PkChar.obj: VfChar.h inliners.h bitcounts.h
$(outdir)\PkFont.obj: StdAfx.h
$(outdir)\PkFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\PkFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\PkFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\PkFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\PkFont.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\PkFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\PkFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\PkFont.obj: common.h
$(outdir)\PkFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\PkFont.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h
$(outdir)\PkFont.obj: VfChar.h inliners.h
$(outdir)\Tfm.obj: StdAfx.h
$(outdir)\Tfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\Tfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\Tfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\Tfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\Tfm.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\Tfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\Tfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\Tfm.obj: common.h
$(outdir)\Tfm.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\Tfm.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h VfChar.h
$(outdir)\Tfm.obj: inliners.h
$(outdir)\VFont.obj: StdAfx.h
$(outdir)\VFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\VFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\VFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\VFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\VFont.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\VFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\VFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\VFont.obj: common.h
$(outdir)\VFont.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\VFont.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h VfChar.h
$(outdir)\VFont.obj: inliners.h
$(outdir)\VfChar.obj: StdAfx.h
$(outdir)\VfChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\VfChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\VfChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\VfChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\VfChar.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\VfChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\VfChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\VfChar.obj: common.h
$(outdir)\VfChar.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\VfChar.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h
$(outdir)\VfChar.obj: VfChar.h inliners.h
$(outdir)\color.obj: StdAfx.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\color.obj: common.h
$(outdir)\color.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\color.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h VfChar.h
$(outdir)\color.obj: inliners.h
$(outdir)\common.obj: StdAfx.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\common.obj: common.h
$(outdir)\common.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\common.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h
$(outdir)\common.obj: VfChar.h inliners.h
$(outdir)\graphics.obj: StdAfx.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\graphics.obj: common.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\graphics.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h
$(outdir)\graphics.obj: VfChar.h inliners.h
$(outdir)\hypertex.obj: StdAfx.h
$(outdir)\hypertex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\hypertex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\hypertex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\hypertex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\hypertex.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\hypertex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\hypertex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\hypertex.obj: common.h
$(outdir)\hypertex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\hypertex.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h
$(outdir)\hypertex.obj: VfChar.h inliners.h
$(outdir)\ps.obj: StdAfx.h
$(outdir)\ps.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\ps.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\ps.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\ps.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\ps.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\ps.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\ps.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\ps.obj: common.h
$(outdir)\ps.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\ps.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h VfChar.h
$(outdir)\ps.obj: inliners.h
$(outdir)\special.obj: StdAfx.h
$(outdir)\special.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\special.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\special.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\special.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\special.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\special.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\special.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\special.obj: common.h
$(outdir)\special.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\special.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h
$(outdir)\special.obj: VfChar.h inliners.h
$(outdir)\src.obj: StdAfx.h
$(outdir)\src.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\src.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\src.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\src.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\src.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\src.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\src.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\src.obj: common.h
$(outdir)\src.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\src.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h VfChar.h
$(outdir)\src.obj: inliners.h
$(outdir)\tpic.obj: StdAfx.h
$(outdir)\tpic.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\tpic.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\tpic.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\tpic.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\tpic.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\tpic.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\tpic.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\tpic.obj: common.h
$(outdir)\tpic.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\tpic.obj: DviChar.h DviFont.h PkChar.h PkFont.h Tfm.h VFont.h VfChar.h
$(outdir)\tpic.obj: inliners.h
$(outdir)\dviscan.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\dviscan.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\dviscan.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\dviscan.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dvi/include/miktex/dvi.h
$(outdir)\dviscan.obj: $(miktexsrcdir)/Libraries/MiKTeX/Dib/include/miktex/DibChunker.h
$(outdir)\dviscan.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt-miktex.h
$(outdir)\dviscan.obj: $(miktexsrcdir)/Libraries/3rd/libpopt/popt.h
