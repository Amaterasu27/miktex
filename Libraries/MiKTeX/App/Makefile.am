## Makefile.am: libapp
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

AM_CXXFLAGS = $(PTHREAD_CFLAGS)

pkgincludedir = $(includedir)/miktex
pkglibdir = $(libdir)/miktex

pkglib_LTLIBRARIES = libmiktex-app.la

pkginclude_HEADERS =
pkginclude_HEADERS += include/miktex/app.h

libmiktex_app_la_CPPFLAGS =
libmiktex_app_la_CPPFLAGS += $(PTHREAD_CFLAGS)
libmiktex_app_la_CPPFLAGS += $(LIBCURL_CPPFLAGS)
libmiktex_app_la_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/bzip2
libmiktex_app_la_CPPFLAGS += -I$(top_srcdir)/Libraries/MiKTeX/Core/include
libmiktex_app_la_CPPFLAGS += -I$(top_srcdir)/Libraries/MiKTeX/PackageManager/include
libmiktex_app_la_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/libexpat
libmiktex_app_la_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/libmd5/include
libmiktex_app_la_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/libmspack
libmiktex_app_la_CPPFLAGS += -I$(srcdir)/include

libmiktex_app_la_LIBADD =
libmiktex_app_la_LIBADD += $(LIBCURL)
libmiktex_app_la_LIBADD += $(PTHREAD_LIBS)
libmiktex_app_la_LIBADD += $(top_builddir)/Libraries/MiKTeX/Core/libmiktex-core.la
libmiktex_app_la_LIBADD += $(top_builddir)/Libraries/MiKTeX/PackageManager/libmiktex-packagemanager.la

libmiktex_app_la_LDFLAGS = -no-undefined -version-info 1:0:0

libmiktex_app_la_SOURCES =
libmiktex_app_la_SOURCES += StdAfx.h
libmiktex_app_la_SOURCES += app-version.h
libmiktex_app_la_SOURCES += app.cpp
libmiktex_app_la_SOURCES += internal.h
