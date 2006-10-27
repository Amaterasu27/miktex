## miktex.mak: mpm
## 
## Copyright (C) 2002-2006 Christian Schenk
## 
## This file is part of MiKTeX Package Manager.
## 
## MiKTeX Package Manager is free software; you can redistribute it
## and/or modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2, or
## (at your option) any later version.
## 
## MiKTeX Package Manager is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied warranty
## of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with MiKTeX Package Manager; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

!include <miktex.inc>

docdir = $(prefix)\doc\miktex

!if $(MIKTEX_RELEASE_STATE) <= 2
cfg = Release
!else
cfg = Debug
!endif

binaries = $(outdir)\mpm_mfc.exe

documents = #help\409\mpm409.chm

all: common-all $(binaries) $(documents)

$(binaries) $(documents): make-all

make-all:
!if ($(MSCVER) >= 1400)
	vcbuild /nologo mpm2005.vcproj $(cfg)
!else
	devenv mpm.sln /build $(cfg)
!endif

install: common-install install-binaries #install-documents

check: common-check

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean
!if ($(MSCVER) >= 1400)
	vcbuild /nologo /clean mpm2005.vcproj $(cfg)
!else
	devenv mpm.sln /clean $(cfg)
!endif

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend:
