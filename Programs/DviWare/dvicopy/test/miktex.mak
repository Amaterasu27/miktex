## miktex.mak: DVIcopy test suite
## 
## Copyright (C) 1996-2001 Christian Schenk
## 
## This file is part of DVIcopy.
## 
## DVIcopy is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
## 
## DVIcopy is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with DVIcopy; if not, write to the Free Software Foundation,
## 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

check: test001.dvi do-dvicopy clean

do-dvicopy:
	dvicopy test001.dvi test001x.dvi

test001.dvi: test001.tex
	latex --c-style test001.tex

clean:
	-del /f *.log
	-del /f *.aux
	-del /f *.dvi
