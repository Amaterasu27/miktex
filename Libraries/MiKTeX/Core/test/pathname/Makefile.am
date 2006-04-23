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

AM_CPPFLAGS =
AM_CPPFLAGS += -I../../include
AM_CPPFLAGS += -I../../../../3rd/libmd5/include
AM_CPPFLAGS += -I../../../../3rd/libpopt

LDADD =
LDADD += ../../libmiktex-core.la
LDADD += ../../../../3rd/libpopt/libmiktex-popt.la
LDADD += $(PTHREAD_LIBS)

check_PROGRAMS = 1

1_SOURCES = 1.cpp

TESTS = 1
