## miktex.mak:
## 
## Copyright (C) 2000-2006 Christian Schenk
## 
## This file is part of MiKTeX Options.
## 
## MiKTeX Options is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License as
## published by the Free Software Foundation; either version 2, or (at
## your option) any later version.
## 
## MiKTeX Options is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with MiKTeX Options; if not, write to the Free Software
## Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
## USA.

!include <miktex.inc>

docdir = $(prefix)\doc\miktex

!if $(MIKTEX_RELEASE_STATE) <= 2
cfg = Release
!else
cfg = Debug
!endif

binaries = $(outdir)\mo.exe

documents = help\409\mo409.chm

all: common-all $(binaries) $(documents)

$(binaries) $(documents): make-all

make-all:
!if ($(MSCVER) >= 1400)
	vcbuild /nologo mo2005.vcproj $(cfg)
!else
	devenv mo.sln /build $(cfg)
!endif

install: common-install install-binaries install-documents

check: common-check

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean
!if ($(MSCVER) >= 1400)
	vcbuild /nologo /clean mo2005.vcproj $(cfg)
!else
	devenv mo.sln /clean $(cfg)
!endif

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend:
