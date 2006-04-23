## miktex.mak: psutils
## 
## Copyright (C) 1998-2005 Christian Schenk
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

perlscriptdir = $(destdir)\scripts\psutils\perl

paper = a4

defines = \
	-DPAPER=\"$(paper)\" \
	-DWINNT \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		$(defines) \
		$<

binaries = \
	$(outdir)\epsffit.exe \
	$(outdir)\psbook.exe \
	$(outdir)\psnup.exe \
	$(outdir)\psresize.exe \
	$(outdir)\psselect.exe \
	$(outdir)\pstops.exe \

sources = \
	epsffit.c \
	psbook.c \
	pserror.c \
	psnup.c \
	psresize.c \
	psselect.c \
	psspec.c \
	pstops.c \
	psutil.c \

perlscripts = \
	extractres.pl \
	fixdlsrps.pl \
	fixfmps.pl \
	fixmacps.pl \
	fixpsditps.pl \
	fixpspps.pl \
	fixscribeps.pl \
	fixtpps.pl \
	fixwfwps.pl \
	fixwpps.pl \
	fixwwps.pl \
	includeres.pl \
	psmerge.pl \

INCLUDEDIR = /texmf/psutils/base

all: common-all $(binaries)

install: common-install install-binaries install-scripts

install-scripts: $(perlscriptdir) $(perlscripts)
	for %f in ($(perlscripts)) do ( \
	    $(perl) maketext \
		INCLUDE=$(INCLUDEDIR) \
		OS=XYZ \
		PERL=perl \
		%f \
		> $(perlscriptdir)\%f \
	 && $(install) \
		$(miktexsrcdir)\$(rel_etcdir)\$(outdir)\runperl2.exe \
		$(bindir)\%~nf.exe \
	)

$(perlscriptdir):
	$(mkpath) $(perlscriptdir)

check: common-check

epsffit_objects = \
	$(outdir)\epsffit.obj \
	$(outdir)\pserror.obj \

$(outdir)\epsffit.exe: \
			$(outdir) \
			$(epsffit_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(epsffit_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

psnup_objects = \
	$(outdir)\pserror.obj \
	$(outdir)\psnup.obj \
	$(outdir)\psspec.obj \
	$(outdir)\psutil.obj \

$(outdir)\psnup.exe: \
			$(outdir) \
			$(psnup_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(psnup_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

psresize_objects = \
	$(outdir)\pserror.obj \
	$(outdir)\psresize.obj \
	$(outdir)\psspec.obj \
	$(outdir)\psutil.obj \

$(outdir)\psresize.exe: \
			$(outdir) \
			$(psresize_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(psresize_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

psbook_objects = \
	$(outdir)\psbook.obj \
	$(outdir)\pserror.obj \
	$(outdir)\psutil.obj \

$(outdir)\psbook.exe: \
			$(outdir) \
			$(psbook_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(psbook_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

psselect_objects = \
	$(outdir)\pserror.obj \
	$(outdir)\psselect.obj \
	$(outdir)\psutil.obj \

$(outdir)\psselect.exe: \
		$(outdir) \
		$(psselect_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(psselect_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

pstops_objects = \
	$(outdir)\pserror.obj \
	$(outdir)\psspec.obj \
	$(outdir)\pstops.obj \
	$(outdir)\psutil.obj \

$(outdir)\pstops.exe: \
		$(outdir) \
		$(pstops_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(pstops_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

depend: $(sources)
	$(MAKEDEPEND) $(defines) $(sources)
	$(fixdep)

!include <common-dependencies.inc>

# DO NOT DELETE

$(outdir)\epsffit.obj: pserror.h patchlev.h config.h
$(outdir)\psbook.obj: psutil.h config.h pserror.h patchlev.h
$(outdir)\pserror.obj: psutil.h config.h pserror.h patchlev.h
$(outdir)\psnup.obj: psutil.h config.h psspec.h pserror.h patchlev.h
$(outdir)\psresize.obj: psutil.h config.h psspec.h pserror.h patchlev.h
$(outdir)\psselect.obj: psutil.h config.h pserror.h patchlev.h
$(outdir)\psspec.obj: psutil.h config.h psspec.h pserror.h patchlev.h
$(outdir)\pstops.obj: psutil.h config.h psspec.h pserror.h patchlev.h
$(outdir)\psutil.obj: psutil.h config.h pserror.h patchlev.h
