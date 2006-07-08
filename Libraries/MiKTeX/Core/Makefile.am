## Makefile.am:
##
## Copyright (C) 2005-2006 Christian Schenk
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

SUBDIRS = test

AM_CXXFLAGS = $(PTHREAD_CFLAGS)

pkgincludedir = $(includedir)/miktex
pkglibdir = $(libdir)/miktex

pkglib_LTLIBRARIES = libmiktex-core.la

pkginclude_HEADERS =
pkginclude_HEADERS += include/miktex/char.h
pkginclude_HEADERS += include/miktex/core.h
pkginclude_HEADERS += include/miktex/version.h

libmiktex_core_la_CPPFLAGS =
libmiktex_core_la_CPPFLAGS += -I$(srcdir)/include
libmiktex_core_la_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/gl
libmiktex_core_la_CPPFLAGS += -I$(top_srcdir)/Libraries/MiKTeX/PackageManager/include
libmiktex_core_la_CPPFLAGS += -DMIKTEX_INSTALLROOT=\"$(MIKTEX_INSTALLROOT)\"
libmiktex_core_la_CPPFLAGS += -DMIKTEX_DATAROOT=\"$(MIKTEX_DATAROOT)\"
libmiktex_core_la_CPPFLAGS += -DMIKTEX_ROOTS=\"$(MIKTEX_ROOTS)\"

libmiktex_core_la_LIBADD =
libmiktex_core_la_LIBADD += $(top_builddir)/Libraries/3rd/bzip2/libmiktex-bz2.la
libmiktex_core_la_LIBADD += $(top_builddir)/Libraries/3rd/gl/libgl.la
libmiktex_core_la_LIBADD += $(top_builddir)/Libraries/3rd/libmd5/libmiktex-md5.la
libmiktex_core_la_LIBADD += $(top_builddir)/Libraries/3rd/zlib/libmiktex-zlib.la
libmiktex_core_la_LIBADD += $(PTHREAD_LIBS)

libmiktex_core_la_LDFLAGS = -no-undefined -version-info 1:0:0

nodist_libmiktex_core_la_SOURCES =
nodist_libmiktex_core_la_SOURCES += include/miktex/version.h

libmiktex_core_la_SOURCES =
libmiktex_core_la_SOURCES += CommandLine.cpp
libmiktex_core_la_SOURCES += FileStream.cpp
libmiktex_core_la_SOURCES += FormatInfo.cpp
libmiktex_core_la_SOURCES += MemoryMappedFile.cpp
libmiktex_core_la_SOURCES += Process.cpp
libmiktex_core_la_SOURCES += StdAfx.h
libmiktex_core_la_SOURCES += Thread.cpp
libmiktex_core_la_SOURCES += appnames.cpp
libmiktex_core_la_SOURCES += cfg.cpp
libmiktex_core_la_SOURCES += config.cpp
libmiktex_core_la_SOURCES += core-version.h
libmiktex_core_la_SOURCES += error.cpp
libmiktex_core_la_SOURCES += files.cpp
libmiktex_core_la_SOURCES += filetypes.cpp
libmiktex_core_la_SOURCES += findfile.cpp
libmiktex_core_la_SOURCES += fnamedb.cpp
libmiktex_core_la_SOURCES += fnamedb.h
libmiktex_core_la_SOURCES += fndbmem.h
libmiktex_core_la_SOURCES += fontinfo.cpp
libmiktex_core_la_SOURCES += getdelim.cpp
libmiktex_core_la_SOURCES += graphics.cpp
libmiktex_core_la_SOURCES += gsinfo.cpp
libmiktex_core_la_SOURCES += include/miktex/char.h
libmiktex_core_la_SOURCES += include/miktex/core.h
libmiktex_core_la_SOURCES += include/miktex/env.h
libmiktex_core_la_SOURCES += include/miktex/first.h
libmiktex_core_la_SOURCES += include/miktex/paths.h
libmiktex_core_la_SOURCES += include/miktex/reg.h
libmiktex_core_la_SOURCES += include/miktex/test.h
libmiktex_core_la_SOURCES += include/miktex/trace.h
libmiktex_core_la_SOURCES += internal.h
libmiktex_core_la_SOURCES += log.cpp
libmiktex_core_la_SOURCES += makefndb.cpp
libmiktex_core_la_SOURCES += mfmodes.cpp
libmiktex_core_la_SOURCES += miktex.cpp
libmiktex_core_la_SOURCES += obsolete.cpp
libmiktex_core_la_SOURCES += papersize.cpp
libmiktex_core_la_SOURCES += path.cpp
libmiktex_core_la_SOURCES += runbat.cpp
libmiktex_core_la_SOURCES += runperl.cpp
libmiktex_core_la_SOURCES += scratchdir.cpp
libmiktex_core_la_SOURCES += searchpath.cpp
libmiktex_core_la_SOURCES += tempfile.cpp
libmiktex_core_la_SOURCES += texmfroot.cpp
libmiktex_core_la_SOURCES += uncompress.cpp
libmiktex_core_la_SOURCES += unx/unx.cpp
libmiktex_core_la_SOURCES += unx/unxDirectoryLister.cpp
libmiktex_core_la_SOURCES += unx/unxDirectoryLister.h
libmiktex_core_la_SOURCES += unx/unxFile.cpp
libmiktex_core_la_SOURCES += unx/unxMemoryMappedFile.cpp
libmiktex_core_la_SOURCES += unx/unxMemoryMappedFile.h
libmiktex_core_la_SOURCES += unx/unxProcess.cpp
libmiktex_core_la_SOURCES += unx/unxProcess.h
libmiktex_core_la_SOURCES += unx/unxThread.cpp
libmiktex_core_la_SOURCES += unx/unxThread.h
libmiktex_core_la_SOURCES += util.cpp
