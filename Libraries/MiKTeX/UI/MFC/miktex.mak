## miktex.mak: mikui
## 
## Copyright (C) 2000-2006 Christian Schenk
## 
## This file is part of the MiKTeX UI Library.
## 
## The MiKTeX UI Library is free software; you can redistribute it
## and/or modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2, or
## (at your option) any later version.
## 
## The MiKTeX UI Library is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied warranty
## of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with the MiKTeX UI Library; if not, write to the Free Software
## Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
## USA.

!include <miktex.inc>

!if $(MIKTEX_RELEASE_STATE) <= 2
cfg = Release
!else
cfg = Debug
!endif

binaries = $(outdir)\$(mikui_dll_name).dll

all: common-all make-binaries

$(binaries): make-binaries

make-binaries:
	vcbuild /nologo mikui2005.vcproj $(cfg)

install: common-install install-binaries

check:

uninstall:

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean
	vcbuild /nologo /clean mikui2005.vcproj $(cfg)

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend:
