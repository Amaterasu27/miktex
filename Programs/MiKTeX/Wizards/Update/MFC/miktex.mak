## miktex.mak: Wizards/Update
## 
## Copyright (C) 2002-2006 Christian Schenk
##
## This file is part of the MiKTeX Update Wizard.
##
## The MiKTeX Update Wizard is free software; you can redistribute it
## and/or modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2, or
## (at your option) any later version.
## 
## The MiKTeX Update Wizard is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied warranty
## of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with the MiKTeX Update Wizard; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

!include <miktex.inc>

!if $(MIKTEX_RELEASE_STATE) <= 2
cfg = Release
!else
cfg = Debug
!endif

all: all-local
	vcbuild /nologo Update2005.vcproj $(cfg)

configdir = $(prefix)\miktex\config

extrasources = \
		ConnectionSettingsDialog.cpp \
		ConnectionSettingsDialog.h \
		ProxyAuthenticationDialog.cpp \
		ProxyAuthenticationDialog.h \

install: all install-local
	copy $(outdir)\updatewiz.exe $(configdir)\update.dat

check:

uninstall:

mostlyclean: mostlyclean-local

clean: mostlyclean clean-local
	vcbuild /nologo /clean Update2005.vcproj $(cfg)

distclean: clean distclean-local

maintainer-clean: distclean maintainer-clean-local

all-local: $(extrasources)

install-local: $(extrasources)

ConnectionSettingsDialog.cpp: $(mikuilibdir)\ConnectionSettingsDialog.cpp
	copy $(mikuilibdir)\ConnectionSettingsDialog.cpp $@

ConnectionSettingsDialog.h: $(mikuilibdir)\ConnectionSettingsDialog.h
	copy $(mikuilibdir)\ConnectionSettingsDialog.h $@

ProxyAuthenticationDialog.cpp: $(mikuilibdir)\ProxyAuthenticationDialog.cpp
	copy $(mikuilibdir)\ProxyAuthenticationDialog.cpp $@

ProxyAuthenticationDialog.h: $(mikuilibdir)\ProxyAuthenticationDialog.h
	copy $(mikuilibdir)\ProxyAuthenticationDialog.h $@

mostlyclean-local: common-mostlyclean

clean-local: common-clean mostlyclean-local

distclean-local: common-distclean clean-local

maintainer-clean-local: common-maintainer-clean distclean-local

!include <common-dependencies.inc>

depend:
