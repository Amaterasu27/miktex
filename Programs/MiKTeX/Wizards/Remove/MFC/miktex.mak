## miktex.mak: Wizards/Remove
## 
## Copyright (C) 2000-2006 Christian Schenk
##
## This file is part of the Remove MiKTeX! Wizard.
##
## The Remove MiKTeX! Wizard is free software; you can redistribute it
## and/or modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2, or
## (at your option) any later version.
## 
## The Remove MiKTeX! Wizard is distributed in the hope that it will
## be useful, but WITHOUT ANY WARRANTY; without even the implied
## warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## See the GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with the Remove MiKTeX! Wizard; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

!include <miktex.inc>

!if $(MIKTEX_RELEASE_STATE) <= 2
cfg = Release
!else
cfg = Debug
!endif

all: all-local
	vcbuild /nologo Remove2005.vcproj $(cfg)

configdir = $(prefix)\miktex\config

install: all install-local $(configdir)
	copy $(outdir)\Remove.exe $(configdir)\uninstall.dat

check:

$(configdir):
	mkdir $(configdir)

uninstall:

mostlyclean: mostlyclean-local

clean: mostlyclean clean-local
	vcbuild /nologo /clean Remove2005.vcproj $(cfg)

distclean: clean distclean-local

maintainer-clean: distclean maintainer-clean-local

all-local:

install-local:

mostlyclean-local: common-mostlyclean

clean-local: common-clean mostlyclean-local

distclean-local: common-distclean clean-local

maintainer-clean-local: common-maintainer-clean distclean-local

!include <common-dependencies.inc>

depend:
