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

bin_PROGRAMS = mthelp

AM_CXXFLAGS = $(PTHREAD_CFLAGS)

mthelp_SOURCES = 
mthelp_SOURCES += mthelp.cpp
mthelp_SOURCES += mthelp-version.h

nodist_mthelp_SOURCES = template.html.h
BUILT_SOURCES = template.html.h
CLEANFILES = template.html.h
EXTRA_DIST = template.html

mthelp_CPPFLAGS = 
mthelp_CPPFLAGS += -DPOPT_DLL
mthelp_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/libmd5/include
mthelp_CPPFLAGS += -I$(top_srcdir)/Libraries/3rd/libpopt
mthelp_CPPFLAGS += -I$(top_srcdir)/Libraries/MiKTeX/App/include
mthelp_CPPFLAGS += -I$(top_srcdir)/Libraries/MiKTeX/Core/include
mthelp_CPPFLAGS += -I$(top_srcdir)/Libraries/MiKTeX/PackageManager/include

mthelp_LDADD = 
mthelp_LDADD += $(top_builddir)/Libraries/3rd/libmd5/libmiktex-md5.la
mthelp_LDADD += $(top_builddir)/Libraries/3rd/libpopt/libmiktex-popt.la
mthelp_LDADD += $(top_builddir)/Libraries/MiKTeX/App//libmiktex-app.la
mthelp_LDADD += $(top_builddir)/Libraries/MiKTeX/Core/libmiktex-core.la
mthelp_LDADD += $(top_builddir)/Libraries/MiKTeX/PackageManager/libmiktex-packagemanager.la

template.html.h: template.html
	$(top_builddir)/BuildUtilities/etc/bintoc templateHtml < $(srcdir)/template.html > $@

mthelp_LDFLAGS = --enable-runtime-pseudo-relocs
