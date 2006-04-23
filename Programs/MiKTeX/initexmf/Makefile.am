## Makefile.am: initexmf
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

bin_PROGRAMS = initexmf

AM_CXXFLAGS = $(PTHREAD_CFLAGS)

initexmf_SOURCES = 
initexmf_SOURCES += initexmf.cpp
initexmf_SOURCES += initexmf-version.h

initexmf_CPPFLAGS = 
initexmf_CPPFLAGS += -DPOPT_DLL
initexmf_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/libmd5/include
initexmf_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/libpopt
initexmf_CPPFLAGS += -I$(top_srcdir)/Libraries/MiKTeX/Core/include
initexmf_CPPFLAGS += -I$(top_srcdir)/Libraries/MiKTeX/PackageManager/include

initexmf_LDADD = 
initexmf_LDADD += $(top_builddir)/Libraries/3rd/libmd5/libmiktex-md5.la
initexmf_LDADD += $(top_builddir)/Libraries/3rd/libpopt/libmiktex-popt.la
initexmf_LDADD += $(top_builddir)/Libraries/MiKTeX/Core/libmiktex-core.la
initexmf_LDADD += $(top_builddir)/Libraries/MiKTeX/PackageManager/libmiktex-packagemanager.la

initexmf_LDFLAGS = --enable-runtime-pseudo-relocs
