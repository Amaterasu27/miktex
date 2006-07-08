## Makefile.am: libmpm
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

pkgincludedir = $(includedir)/miktex
pkglibdir = $(libdir)/miktex

pkglib_LTLIBRARIES = libmiktex-packagemanager.la

pkginclude_HEADERS =
pkginclude_HEADERS += include/miktex/mpm.h

libmiktex_packagemanager_la_CPPFLAGS =
libmiktex_packagemanager_la_CPPFLAGS += $(PTHREAD_CFLAGS)
libmiktex_packagemanager_la_CPPFLAGS += $(LIBCURL_CPPFLAGS)
libmiktex_packagemanager_la_CPPFLAGS += -I$(srcdir)/include
libmiktex_packagemanager_la_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/bzip2
libmiktex_packagemanager_la_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/gsoap
libmiktex_packagemanager_la_CPPFLAGS += -I$(top_srcdir)/Libraries/MiKTeX/Core/include
libmiktex_packagemanager_la_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/libexpat
libmiktex_packagemanager_la_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/libmd5/include
libmiktex_packagemanager_la_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/libmspack

libmiktex_packagemanager_la_LIBADD =
libmiktex_packagemanager_la_LIBADD += $(LIBCURL)
libmiktex_packagemanager_la_LIBADD += $(PTHREAD_LIBS)
libmiktex_packagemanager_la_LIBADD += $(top_builddir)/Libraries/MiKTeX/Core/libmiktex-core.la
libmiktex_packagemanager_la_LIBADD += $(top_builddir)/Libraries/3rd/bzip2/libmiktex-bz2.la
libmiktex_packagemanager_la_LIBADD += $(top_builddir)/Libraries/3rd/libexpat/libmiktex-expat.la
libmiktex_packagemanager_la_LIBADD += $(top_builddir)/Libraries/3rd/libmd5/libmiktex-md5.la
libmiktex_packagemanager_la_LIBADD += $(top_builddir)/Libraries/3rd/libmspack/libmiktex-mspack.la

libmiktex_packagemanager_la_LDFLAGS = -no-undefined -version-info 1:0:0

libmiktex_packagemanager_la_SOURCES =
libmiktex_packagemanager_la_SOURCES += $(top_srcdir)/Libraries/3rd/gsoap/stdsoap2.cpp
libmiktex_packagemanager_la_SOURCES += $(top_srcdir)/Libraries/3rd/gsoap/stdsoap2.h
libmiktex_packagemanager_la_SOURCES += $(top_srcdir)/Libraries/MiKTeX/WebServiceClients/Repository/RepositorySoap.nsmap
libmiktex_packagemanager_la_SOURCES += $(top_srcdir)/Libraries/MiKTeX/WebServiceClients/Repository/repositoryC.cpp
libmiktex_packagemanager_la_SOURCES += $(top_srcdir)/Libraries/MiKTeX/WebServiceClients/Repository/repositoryH.h
libmiktex_packagemanager_la_SOURCES += $(top_srcdir)/Libraries/MiKTeX/WebServiceClients/Repository/repositoryRepositorySoapProxy.cpp
libmiktex_packagemanager_la_SOURCES += $(top_srcdir)/Libraries/MiKTeX/WebServiceClients/Repository/repositoryRepositorySoapProxy.h
libmiktex_packagemanager_la_SOURCES += $(top_srcdir)/Libraries/MiKTeX/WebServiceClients/Repository/repositoryStub.h
libmiktex_packagemanager_la_SOURCES += CabExtractor.cpp
libmiktex_packagemanager_la_SOURCES += CabExtractor.h
libmiktex_packagemanager_la_SOURCES += CurlWebFile.cpp
libmiktex_packagemanager_la_SOURCES += CurlWebFile.h
libmiktex_packagemanager_la_SOURCES += CurlWebSession.cpp
libmiktex_packagemanager_la_SOURCES += CurlWebSession.h
libmiktex_packagemanager_la_SOURCES += Extractor.cpp
libmiktex_packagemanager_la_SOURCES += Extractor.h
libmiktex_packagemanager_la_SOURCES += PackageInstaller.cpp
libmiktex_packagemanager_la_SOURCES += PackageIterator.cpp
libmiktex_packagemanager_la_SOURCES += StdAfx.h
libmiktex_packagemanager_la_SOURCES += TarBzip2Extractor.cpp
libmiktex_packagemanager_la_SOURCES += TarBzip2Extractor.h
libmiktex_packagemanager_la_SOURCES += TpmParser.cpp
libmiktex_packagemanager_la_SOURCES += TpmParser.h
libmiktex_packagemanager_la_SOURCES += WebFile.h
libmiktex_packagemanager_la_SOURCES += WebSession.cpp
libmiktex_packagemanager_la_SOURCES += WebSession.h
libmiktex_packagemanager_la_SOURCES += include/miktex/mpm.h
libmiktex_packagemanager_la_SOURCES += internal.h
libmiktex_packagemanager_la_SOURCES += mpm-version.h
libmiktex_packagemanager_la_SOURCES += mpm.cpp
