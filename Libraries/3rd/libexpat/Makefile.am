## Makefile.am: libexpat
##
## Copyright (C) 2005 Christian Schenk
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

pkglib_LTLIBRARIES = libmiktex-expat.la

libmiktex_expat_la_LDFLAGS = -no-undefined -version-info 1:0:0

AM_CFLAGS = -DHAVE_EXPAT_CONFIG_H

libmiktex_expat_la_SOURCES =
libmiktex_expat_la_SOURCES += ascii.h
libmiktex_expat_la_SOURCES += asciitab.h
libmiktex_expat_la_SOURCES += expat-version.h
libmiktex_expat_la_SOURCES += expat.h
libmiktex_expat_la_SOURCES += expat_config.h
libmiktex_expat_la_SOURCES += expat_external.h
libmiktex_expat_la_SOURCES += iasciitab.h
libmiktex_expat_la_SOURCES += internal.h
libmiktex_expat_la_SOURCES += latin1tab.h
libmiktex_expat_la_SOURCES += nametab.h
libmiktex_expat_la_SOURCES += utf8tab.h
libmiktex_expat_la_SOURCES += xmlparse.c
libmiktex_expat_la_SOURCES += xmlrole.c
libmiktex_expat_la_SOURCES += xmlrole.h
libmiktex_expat_la_SOURCES += xmltok.c
libmiktex_expat_la_SOURCES += xmltok.h
libmiktex_expat_la_SOURCES += xmltok_impl.h

EXTRA_DIST =
EXTRA_DIST += xmltok_impl.c
EXTRA_DIST += xmltok_ns.c
