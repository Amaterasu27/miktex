## common-dependencies.inc: common MiKTeX dependenices -*- Makefile -*-
## 
## Copyright (C) 1991-2005 Christian Schenk
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

common-all: $(outdir) $(binaries)

common-install: $(bindir)

install-binaries: $(bindir) $(binaries)
	for %f in ($(binaries)) do ( \
	  $(install) "%f" "$(bindir)" \
	)

install-documents: $(docdir) $(documents)
	for %f in ($(documents)) do $(install) "%f" "$(docdir)"

common-check:

common-mostlyclean:
	if exist "$(outdir)\*.res" del /f /q "$(outdir)\*.res"
	if exist "$(outdir)\*.exp" del /f /q "$(outdir)\*.exp"
	if exist "$(outdir)\*.obj" del /f /q "$(outdir)\*.obj"
!if ($(MSCVER) >= 1400)
	if exist vc80.pdb del /f /q vc80.pdb
!else
	if exist vc70.pdb del /f /q vc70.pdb
!endif
	if exist "$(miktex_mak).bak" del /f /q "$(miktex_mak).bak"

common-clean: common-mostlyclean
	if exist "$(outdir)" rmdir /s /q "$(outdir)"

common-distclean: common-clean
	if exist *.opt del /f /q *.opt

common-maintainer-clean: common-distclean

$(destdir):
	$(mkpath) $(destdir)

$(miktexdir): $(destdir)
	$(mkpath) $(miktexdir)

$(bindir):
	$(mkpath) $(bindir)

$(docdir):
	$(mkpath) $(docdir)

$(outdir):
	if not exist "$(outdir)" ( \
	  $(mkpath) $(outdir) \
	)
