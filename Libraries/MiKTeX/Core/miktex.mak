## miktex.mak:
##
## Copyright (C) 1996-2006 Christian Schenk
##
## This file is part of the MiKTeX Core Library.
##
## The MiKTeX Core Library is free software; you can redistribute it
## and/or modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2, or
## (at your option) any later version.
## 
## The MiKTeX Core Library is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied warranty
## of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with the MiKTeX Core Library; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

miktex_cc_warnings_as_errors = 1
miktex_cc_buffer_security_check = 1

miktex_enable_pgo = 1

!include <miktex.inc>

sources = \
	CommandLine.cpp \
	FileStream.cpp \
	FormatInfo.cpp \
	MemoryMappedFile.cpp \
	Process.cpp \
	StdAfx.cpp \
	Thread.cpp \
	appnames.cpp \
	cfg.cpp \
	config.cpp \
	error.cpp \
	files.cpp \
	filetypes.cpp \
	findfile.cpp \
	fnamedb.cpp \
	fontinfo.cpp \
	getdelim.cpp \
	graphics.cpp \
	gsinfo.cpp \
	log.cpp \
	makefndb.cpp \
	mfmodes.cpp \
	miktex.cpp \
	obsolete.cpp \
	papersize.cpp \
	path.cpp \
	runbat.cpp \
	runperl.cpp \
	scratchdir.cpp \
	searchpath.cpp \
	tempfile.cpp \
	texmfroot.cpp \
	uncompress.cpp \
	util.cpp \
	win\win.cpp \
	win\winDirectoryLister.cpp \
	win\winFile.cpp \
	win\winMemoryMappedFile.cpp \
	win\winProcess.cpp \
	win\winRegistry.cpp \
	win\winThread.cpp \

objects = \
	$(outdir)\StdAfx.obj \
	$(outdir)\CommandLine.obj \
	$(outdir)\FileStream.obj \
	$(outdir)\FormatInfo.obj \
	$(outdir)\MemoryMappedFile.obj \
	$(outdir)\Process.obj \
	$(outdir)\appnames.obj \
	$(outdir)\cfg.obj \
	$(outdir)\config.obj \
	$(outdir)\error.obj \
	$(outdir)\files.obj \
	$(outdir)\filetypes.obj \
	$(outdir)\findfile.obj \
	$(outdir)\fnamedb.obj \
	$(outdir)\fontinfo.obj \
	$(outdir)\getdelim.obj \
	$(outdir)\graphics.obj \
	$(outdir)\gsinfo.obj \
	$(outdir)\log.obj \
	$(outdir)\makefndb.obj \
	$(outdir)\mfmodes.obj \
	$(outdir)\miktex.obj \
	$(outdir)\obsolete.obj \
	$(outdir)\papersize.obj \
	$(outdir)\path.obj \
	$(outdir)\runbat.obj \
	$(outdir)\runperl.obj \
	$(outdir)\scratchdir.obj \
	$(outdir)\searchpath.obj \
	$(outdir)\tempfile.obj \
	$(outdir)\texmfroot.obj \
	$(outdir)\thread.obj \
	$(outdir)\uncompress.obj \
	$(outdir)\util.obj \
	$(outdir)\win.obj \
	$(outdir)\winDirectoryLister.obj \
	$(outdir)\winFile.obj \
	$(outdir)\winMemoryMappedFile.obj \
	$(outdir)\winProcess.obj \
	$(outdir)\winRegistry.obj \
	$(outdir)\winThread.obj \

compile = \
	$(cc) $(cppstandard) \
		$(ccinc_libbz2) \
		$(ccinc_libmd5) \
		$(ccinc_zlib) \
		$(ccopt_name_precompiled_header_file)$(outdir)\miktex.pch \
		$(ccopt_output_file)$@ \
		$(ccopt_use_precompiled_header_file)StdAfx.h \

binaries = $(outdir)\$(miktex_dll_name).dll

all: common-all

install: common-install install-binaries

check: common-check
	pushd test & $(MAKE) -f $(miktex_mak) check & popd

.cpp{$(outdir)\}.obj:
	$(compile) $<

{win\}.cpp{$(outdir)\}.obj:
	$(compile) $<

MiKTeXEvents.h MiKTeXEvents.rc MSG00001.bin: MiKTeXEvents.mc
	mc MiKTeXEvents.mc

$(objects): MiKTeXEvents.h

# _____________________________________________________________________________
#
# miktex.dll
# _____________________________________________________________________________

miktex_dll_objects = \
	$(objects) \
	$(outdir)\miktex.res \
	$(bz2_lib) \
	$(gnu_lib) \
	$(md5_lib) \
	$(zlib_lib) \

$(outdir)\$(miktex_dll_name).dll $(miktex_lib): \
			$(outdir) \
			$(dllbases) \
			$(miktex_dll_objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(miktex_dll_name) \
		-delayload:$(bzip2_dll_name).dll \
		-delayload:$(md5_dll_name).dll \
		-delayload:$(zlib_dll_name).dll \
		-delayload:advapi32.dll \
		-delayload:winspool.drv \
		-implib:$(miktex_lib) \
		-out:$(outdir)\$(miktex_dll_name).dll \
		$(miktex_dll_objects) \
		advapi32.lib \
		delayimp.lib \
		htmlhelp.lib \
		SHFolder.lib \
		shell32.lib \
		user32.lib \
		winspool.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\miktex.res: \
			$(outdir) \
			MSG00001.bin \
			MiKTeXEvents.rc \
			core-version.h \
			.\include\miktex\version.h \
			core.rc \
			include\miktex\miktex.version \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ core.rc

# _____________________________________________________________________________
#
# precompiled headers
# _____________________________________________________________________________

$(outdir)\StdAfx.obj: $(outdir) StdAfx.cpp
	$(compile) $(ccopt_create_precompiled_header_file)StdAfx.h StdAfx.cpp

# _____________________________________________________________________________
#
# dlls.txt
# _____________________________________________________________________________

make-dlls.txt:
	$(mkdllbases) $(bindir)\*.dll > dlls.txt

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean
	-del /f MSG00001.bin
	-del /f MiKTeXEvents.h
	-del /f MiKTeXEvents.rc

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend:
	pushd test & $(MAKE) -f $(miktex_mak) depend & popd
	$(MAKEDEPEND) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\CommandLine.obj: StdAfx.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\CommandLine.obj: internal.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\CommandLine.obj: MiKTeXEvents.h
$(outdir)\FileStream.obj: StdAfx.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\FileStream.obj: internal.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\FileStream.obj: MiKTeXEvents.h
$(outdir)\FormatInfo.obj: StdAfx.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\FormatInfo.obj: internal.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\FormatInfo.obj: MiKTeXEvents.h
$(outdir)\MemoryMappedFile.obj: StdAfx.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\MemoryMappedFile.obj: internal.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\MemoryMappedFile.obj: MiKTeXEvents.h
$(outdir)\Process.obj: StdAfx.h
$(outdir)\Process.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\Process.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\Process.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\Process.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\Process.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\Process.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\Process.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\Process.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\Process.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\Process.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\Process.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\Process.obj: MiKTeXEvents.h
$(outdir)\StdAfx.obj: StdAfx.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\Thread.obj: StdAfx.h
$(outdir)\Thread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\Thread.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\Thread.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\Thread.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\Thread.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\Thread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\Thread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\Thread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\Thread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\Thread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\Thread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\Thread.obj: MiKTeXEvents.h
$(outdir)\appnames.obj: StdAfx.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\appnames.obj: MiKTeXEvents.h
$(outdir)\cfg.obj: StdAfx.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\cfg.obj: MiKTeXEvents.h
$(outdir)\config.obj: StdAfx.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\config.obj: MiKTeXEvents.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\config.obj: win/winRegistry.h
$(outdir)\error.obj: StdAfx.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\error.obj: MiKTeXEvents.h
$(outdir)\files.obj: StdAfx.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\files.obj: MiKTeXEvents.h fnamedb.h fndbmem.h
$(outdir)\filetypes.obj: StdAfx.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\filetypes.obj: MiKTeXEvents.h
$(outdir)\findfile.obj: StdAfx.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\findfile.obj: MiKTeXEvents.h fnamedb.h fndbmem.h
$(outdir)\fnamedb.obj: StdAfx.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\fnamedb.obj: MiKTeXEvents.h fnamedb.h fndbmem.h
$(outdir)\fontinfo.obj: StdAfx.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\fontinfo.obj: MiKTeXEvents.h
$(outdir)\getdelim.obj: StdAfx.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\getdelim.obj: MiKTeXEvents.h
$(outdir)\graphics.obj: StdAfx.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\graphics.obj: MiKTeXEvents.h
$(outdir)\gsinfo.obj: StdAfx.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\gsinfo.obj: MiKTeXEvents.h
$(outdir)\log.obj: StdAfx.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\log.obj: MiKTeXEvents.h
$(outdir)\makefndb.obj: StdAfx.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\makefndb.obj: MiKTeXEvents.h fndbmem.h
$(outdir)\mfmodes.obj: StdAfx.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\mfmodes.obj: MiKTeXEvents.h
$(outdir)\miktex.obj: StdAfx.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\miktex.obj: MiKTeXEvents.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\miktex.obj: win/winRegistry.h core-version.h fnamedb.h fndbmem.h
$(outdir)\obsolete.obj: StdAfx.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\obsolete.obj: MiKTeXEvents.h
$(outdir)\papersize.obj: StdAfx.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\papersize.obj: MiKTeXEvents.h
$(outdir)\path.obj: StdAfx.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\path.obj: MiKTeXEvents.h
$(outdir)\runbat.obj: StdAfx.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\runbat.obj: MiKTeXEvents.h
$(outdir)\runperl.obj: StdAfx.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\runperl.obj: MiKTeXEvents.h
$(outdir)\scratchdir.obj: StdAfx.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\scratchdir.obj: internal.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\scratchdir.obj: MiKTeXEvents.h
$(outdir)\searchpath.obj: StdAfx.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\searchpath.obj: internal.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\searchpath.obj: MiKTeXEvents.h
$(outdir)\tempfile.obj: StdAfx.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\tempfile.obj: MiKTeXEvents.h
$(outdir)\texmfroot.obj: StdAfx.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\texmfroot.obj: MiKTeXEvents.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/env.h
$(outdir)\texmfroot.obj: fnamedb.h fndbmem.h
$(outdir)\uncompress.obj: StdAfx.h
$(outdir)\uncompress.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\uncompress.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\uncompress.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\uncompress.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\uncompress.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\uncompress.obj: internal.h
$(outdir)\uncompress.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\uncompress.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\uncompress.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\uncompress.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\uncompress.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\uncompress.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\uncompress.obj: MiKTeXEvents.h
$(outdir)\util.obj: StdAfx.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h internal.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\util.obj: MiKTeXEvents.h
$(outdir)\win.obj: StdAfx.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\win.obj: internal.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\win.obj: MiKTeXEvents.h core-version.h win/winRegistry.h
$(outdir)\winDirectoryLister.obj: StdAfx.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\winDirectoryLister.obj: internal.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\winDirectoryLister.obj: MiKTeXEvents.h win/winDirectoryLister.h
$(outdir)\winFile.obj: StdAfx.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\winFile.obj: internal.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\winFile.obj: MiKTeXEvents.h
$(outdir)\winMemoryMappedFile.obj: StdAfx.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\winMemoryMappedFile.obj: internal.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\winMemoryMappedFile.obj: MiKTeXEvents.h win/winMemoryMappedFile.h
$(outdir)\winProcess.obj: StdAfx.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\winProcess.obj: internal.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\winProcess.obj: MiKTeXEvents.h win/winProcess.h
$(outdir)\winRegistry.obj: StdAfx.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\winRegistry.obj: internal.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\winRegistry.obj: MiKTeXEvents.h win/winRegistry.h
$(outdir)\winThread.obj: StdAfx.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/first.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries/3rd/bzip2/bzlib.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zlib.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries/3rd/zlib/zconf.h
$(outdir)\winThread.obj: internal.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/paths.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/reg.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/trace.h
$(outdir)\winThread.obj: MiKTeXEvents.h win/winThread.h
