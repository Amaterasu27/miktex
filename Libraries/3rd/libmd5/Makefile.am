## Makefile.am: libmd5
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

pkglib_LTLIBRARIES = libmiktex-md5.la

pkginclude_HEADERS =
pkginclude_HEADERS += include/miktex/md5.h

libmiktex_md5_la_LDFLAGS = -no-undefined -version-info 1:0:0

libmiktex_md5_la_CPPFLAGS =
libmiktex_md5_la_CPPFLAGS += -I$(srcdir)/include

libmiktex_md5_la_SOURCES =
libmiktex_md5_la_SOURCES += global.h
libmiktex_md5_la_SOURCES += internal.h
libmiktex_md5_la_SOURCES += md5-version.h
libmiktex_md5_la_SOURCES += md5c.cpp
libmiktex_md5_la_SOURCES += include/miktex/md5.h
