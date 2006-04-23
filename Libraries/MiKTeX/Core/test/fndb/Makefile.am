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

AM_CXXFLAGS = $(PTHREAD_CFLAGS)
AM_CXXFLAGS += -DDATAROOT=\"`pwd`/sandbox-unx/localtexmf\"
AM_CXXFLAGS += -DINSTALLROOT=\"`pwd`/sandbox-unx/texmf\"

AM_CPPFLAGS =
AM_CPPFLAGS += -I../../include
AM_CPPFLAGS += -I../../../../3rd/libmd5/include
AM_CPPFLAGS += -I../../../../3rd/libpopt

LDADD =
LDADD += ../../libmiktex-core.la
LDADD += ../../../../3rd/libpopt/libmiktex-popt.la
LDADD += $(PTHREAD_LIBS)

check_PROGRAMS = 1 2

1_SOURCES = 1.cpp
2_SOURCES = 2.cpp

sandbox = ./sandbox-unx

installroot = $(sandbox)/texmf
dataroot = $(sandbox)/localtexmf

all-local: $(sandbox)

clean-local:
	-rm -fr $(sandbox)

$(sandbox):
	mkdir -p $(dataroot)
	mkdir -p $(installroot)/tex/latex/base
	mkdir -p $(installroot)/tex/test/base
	touch $(installroot)/tex/latex/base/test.cls
	touch $(installroot)/tex/test/base/test.tex

TESTS = 1 2
