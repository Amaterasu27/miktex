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

bin_PROGRAMS = mpm

AM_CXXFLAGS = $(PTHREAD_CFLAGS)

mpm_SOURCES = 
mpm_SOURCES += StdAfx.h
mpm_SOURCES += internal.h
mpm_SOURCES += mpm-version.h
mpm_SOURCES += mpm.cpp

mpm_CPPFLAGS = 
mpm_CPPFLAGS += -DPOPT_DLL
mpm_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/libmd5/include
mpm_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/libpopt
mpm_CPPFLAGS += -I$(top_srcdir)/Libraries/MiKTeX/Core/include
mpm_CPPFLAGS += -I$(top_srcdir)/Libraries/MiKTeX/PackageManager/include

mpm_LDADD = 
mpm_LDADD += $(top_builddir)/Libraries/3rd/libmd5/libmiktex-md5.la
mpm_LDADD += $(top_builddir)/Libraries/3rd/libpopt/libmiktex-popt.la
mpm_LDADD += $(top_builddir)/Libraries/MiKTeX/Core/libmiktex-core.la
mpm_LDADD += $(top_builddir)/Libraries/MiKTeX/PackageManager/libmiktex-packagemanager.la

mpm_LDFLAGS = --enable-runtime-pseudo-relocs
