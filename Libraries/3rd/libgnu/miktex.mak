## miktex.mak: libgnu
##
## Copyright (C) 2000-2005 Christian Schenk
##
## This file is part of the MiKTeX GNU Library (gnu.dll).
## The MiKTeX GNU Library is free software; you can redistribute it
## and/or modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2, or
## (at your option) any later version.
## 
## The MiKTeX GNU Library is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied warranty
## of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with the MiKTeX GNU Library; if not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

!include <miktex.inc>

gnu_dll = $(gnu_dll_name).dll
regex_dll = $(regex_dll_name).dll

binaries = \
	$(outdir)\$(gnu_dll) \
	$(outdir)\$(regex_dll) \

sources = \
	basename.c \
	getopt.c \
	getopt1.c \
	regex.c \
	substring.c \

.c{$(outdir)\}.obj:
	$(cc) $(cstandard) $(defines) $(ccopt_output_file)$@ $<

all: common-all $(gnu_lib) $(regex_lib)

install: common-install install-binaries

check: common-check

# _____________________________________________________________________________
#
# gnu.dll
# _____________________________________________________________________________

gnu_objects = \
	$(outdir)\basename.obj \
	$(outdir)\getopt.obj \
	$(outdir)\getopt1.obj \
	$(outdir)\substring.obj \

$(outdir)\$(gnu_dll) $(gnu_lib): \
			$(outdir) \
			$(gnu_objects) \
			$(outdir)\gnu.res \
			$(dllbases) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(gnu_dll_name) \
		-implib:$(gnu_lib) \
		-out:$(outdir)\$(gnu_dll) \
		$(gnu_objects) \
		$(outdir)\gnu.res
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\gnu.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			gnu-version.h \
			gnu.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ gnu.rc

# _____________________________________________________________________________
#
# regex.dll
# _____________________________________________________________________________

regex_objects = \
	$(outdir)\regex.obj \

$(outdir)\$(regex_dll) $(regex_lib): \
			$(outdir) \
			$(dllbases) \
			$(outdir)\regex.res \
			$(regex_objects) \

	$(link) $(dlllstandard) \
		-base:@$(dllbases),$(regex_dll_name) \
		-implib:$(regex_lib) \
		-out:$(outdir)\$(regex_dll) \
		$(regex_objects) \
		$(outdir)\regex.res
	mt -nologo -manifest $@.manifest -outputresource:$@;2

$(outdir)\regex.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			regex-version.h \
			regex.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ regex.rc

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include <common-dependencies.inc>

depend:
	$(MAKEDEPEND) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\basename.obj: internal.h gnu-miktex.h
$(outdir)\getopt.obj: getopt.h getopt_int.h
$(outdir)\getopt1.obj: getopt.h getopt_int.h
$(outdir)\regex.obj: ./regex.h regex_internal.h regex_internal.c regcomp.c
$(outdir)\regex.obj: regexec.c
$(outdir)\substring.obj: internal.h gnu-miktex.h
