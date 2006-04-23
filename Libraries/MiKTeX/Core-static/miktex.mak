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

miktex_cc_buffer_security_check = 1
miktex_cc_maximize_speed = 0
miktex_cc_minimize_size = 1
miktex_cc_use_crt_dll = 0
no_miktex_dlls = 1

!include <miktex.inc>

srcdir = $(libdir)

sources = \
	$(libdir)\StdAfx.cpp \
	$(libdir)\CommandLine.cpp \
	$(libdir)\FileStream.cpp \
	$(libdir)\FormatInfo.cpp \
	$(libdir)\MemoryMappedFile.cpp \
	$(libdir)\appnames.cpp \
	$(libdir)\cfg.cpp \
	$(libdir)\config.cpp \
	$(libdir)\error.cpp \
	$(libdir)\files.cpp \
	$(libdir)\filetypes.cpp \
	$(libdir)\findfile.cpp \
	$(libdir)\fnamedb.cpp \
	$(libdir)\fontinfo.cpp \
	$(libdir)\getdelim.cpp \
	$(libdir)\graphics.cpp \
	$(libdir)\gsinfo.cpp \
	$(libdir)\log.cpp \
	$(libdir)\makefndb.cpp \
	$(libdir)\mfmodes.cpp \
	$(libdir)\miktex.cpp \
	$(libdir)\obsolete.cpp \
	$(libdir)\papersize.cpp \
	$(libdir)\path.cpp \
	$(libdir)\process.cpp \
	$(libdir)\runbat.cpp \
	$(libdir)\runperl.cpp \
	$(libdir)\scratchdir.cpp \
	$(libdir)\searchpath.cpp \
	$(libdir)\tempfile.cpp \
	$(libdir)\texmfroot.cpp \
	$(libdir)\thread.cpp \
	$(libdir)\util.cpp \
	$(libdir)\win\win.cpp \
	$(libdir)\win\winDirectoryLister.cpp \
	$(libdir)\win\winFile.cpp \
	$(libdir)\win\winMemoryMappedFile.cpp \
	$(libdir)\win\winProcess.cpp \
	$(libdir)\win\winRegistry.cpp \
	$(libdir)\win\winThread.cpp \

objects = \
	$(outdir)\StdAfx.obj \
	$(outdir)\CommandLine.obj \
	$(outdir)\FileStream.obj \
	$(outdir)\FormatInfo.obj \
	$(outdir)\MemoryMappedFile.obj \
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
	$(outdir)\process.obj \
	$(outdir)\runbat.obj \
	$(outdir)\runperl.obj \
	$(outdir)\scratchdir.obj \
	$(outdir)\searchpath.obj \
	$(outdir)\tempfile.obj \
	$(outdir)\texmfroot.obj \
	$(outdir)\thread.obj \
	$(outdir)\util.obj \
	$(outdir)\win.obj \
	$(outdir)\winDirectoryLister.obj \
	$(outdir)\winFile.obj \
	$(outdir)\winMemoryMappedFile.obj \
	$(outdir)\winProcess.obj \
	$(outdir)\winRegistry.obj \
	$(outdir)\winThread.obj \

compile = \
	$(cc) \
		$(cppstandard) \
		$(ccopt_name_precompiled_header_file)$(outdir)\miktex.pch \
		$(ccopt_output_directory)$(outdir)\ \
		$(ccopt_use_precompiled_header_file)StdAfx.h \

all: common-all $(outdir) $(miktex_static_lib)

install: common-install all

check: common-check

{$(libdir)\}.cpp{$(outdir)\}.obj:
	$(compile) $<

{$(libdir)\win\}.cpp{$(outdir)\}.obj:
	$(compile) $<

# _____________________________________________________________________________
#
# core-static.lib
# _____________________________________________________________________________

$(miktex_static_lib): $(outdir) $(objects)
	$(libtool) $(libflags) $(libopt_output_file)$@ $(objects)

# _____________________________________________________________________________
#
# precompiled headers
# _____________________________________________________________________________

$(outdir)\StdAfx.obj: $(outdir) $(libdir)\StdAfx.cpp
	$(compile) \
		$(ccopt_create_precompiled_header_file)StdAfx.h \
		$(libdir)\StdAfx.cpp

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

$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\StdAfx.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\CommandLine.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\FileStream.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\FormatInfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\MemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\appnames.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\cfg.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/version.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/env.h
$(outdir)\config.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/win/winRegistry.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\error.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/fnamedb.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/fndbmem.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\filetypes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/fnamedb.h
$(outdir)\findfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/fndbmem.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/fnamedb.h
$(outdir)\fnamedb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/fndbmem.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\fontinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\getdelim.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\graphics.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\gsinfo.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\log.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\makefndb.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/fndbmem.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\mfmodes.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/env.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/version.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/fnamedb.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/fndbmem.h
$(outdir)\miktex.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/core-version.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\obsolete.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\papersize.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\path.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\process.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\process.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\process.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\process.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\process.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\process.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\process.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\process.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\process.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\runbat.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\runperl.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\scratchdir.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\searchpath.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\tempfile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/env.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/fnamedb.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/fndbmem.h
$(outdir)\texmfroot.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/version.h
$(outdir)\thread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\thread.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\thread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\thread.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\thread.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\thread.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\thread.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\thread.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\thread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\util.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/version.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/core-version.h
$(outdir)\win.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/win/winRegistry.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\winDirectoryLister.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/win/winDirectoryLister.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\winFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\winMemoryMappedFile.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/win/winMemoryMappedFile.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\winProcess.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/win/winProcess.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\winRegistry.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/win/winRegistry.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/StdAfx.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries\3rd\libmd5/include/miktex/md5.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/internal.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/core.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/char.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/paths.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/reg.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries\MiKTeX\Core/include/miktex/trace.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/MiKTeXEvents.h
$(outdir)\winThread.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/win/winThread.h
