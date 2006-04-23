## miktex.mak: util
##
## Copyright (C) 2000-2005 Christian Schenk
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

!include <miktex.inc>

binaries = \
	$(outdir)\bintoc.exe \
	$(outdir)\headerguard.exe \
	$(outdir)\j2000.exe \
	$(outdir)\mkdllbases.exe \

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) $(ccopt_output_file)$@ $<

all: common-all $(binaries)

bintoc_objects = $(outdir)\bintoc.obj

$(outdir)\bintoc.exe: \
			$(outdir) \
			$(bintoc_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(bintoc_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

headerguard_objects = $(outdir)\headerguard.obj

$(outdir)\headerguard.exe: \
			$(outdir) \
			$(headerguard_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(headerguard_objects) \
		Rpcrt4.lib
	mt -nologo -manifest $@.manifest -outputresource:$@;1

j2000_objects = $(outdir)\j2000.obj

$(outdir)\j2000.exe: \
			$(outdir) \
			$(j2000_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(j2000_objects) \
		setargv.obj
	mt -nologo -manifest $@.manifest -outputresource:$@;1

mkdllbases_objects = $(outdir)\mkdllbases.obj
mkdllbases_libs = imagehlp.lib setargv.obj

$(outdir)\mkdllbases.exe: \
			$(outdir) \
			$(mkdllbases_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(mkdllbases_objects) \
		$(mkdllbases_libs)		
	mt -nologo -manifest $@.manifest -outputresource:$@;1

install: common-install install-binaries

check: common-check

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend: bintoc.cpp headerguard.cpp j2000.cpp
	$(MAKEDEPEND) $**
	$(fixdep)

# DO NOT DELETE
