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

ACLOCAL_AMFLAGS = -I m4 -I Libraries/3rd/gl/m4

bin_SCRIPTS = miktex-config

SUBDIRS =

SUBDIRS += BuildUtilities/etc

SUBDIRS += Libraries/3rd/gl

SUBDIRS += Libraries/3rd/bzip2
SUBDIRS += Libraries/3rd/libexpat
SUBDIRS += Libraries/3rd/libmspack
SUBDIRS += Libraries/3rd/zlib
SUBDIRS += Libraries/3rd/libpopt

SUBDIRS += Libraries/3rd/libmd5

SUBDIRS += Libraries/MiKTeX/Core

SUBDIRS += Libraries/MiKTeX/PackageManager

SUBDIRS += Libraries/MiKTeX/App

SUBDIRS += Programs/MiKTeX/PackageManager/mpm

SUBDIRS += Programs/MiKTeX/initexmf
SUBDIRS += Programs/MiKTeX/mthelp

SUBDIRS += Documentation

EXTRA_DIST =
EXTRA_DIST += Doxyfile
EXTRA_DIST += Libraries/3rd/gl/pathmax.h
EXTRA_DIST += unx/INSTALL.unx
EXTRA_DIST += unx/README.unx

dist-hook:
	cp $(distdir)/unx/INSTALL.unx $(distdir)/INSTALL
	sed -e 's/\@PACKAGE_STRING\@/$(PACKAGE_STRING)/' \
	 -e 's/\@PACKAGE_NAME\@/$(PACKAGE_NAME)/' \
	 $(distdir)/unx/README.unx > $(distdir)/README
	cd $(distdir); doxygen
	find $(distdir) -type f | xargs --max-args=20 dos2unix-mingw --keepdate
