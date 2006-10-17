## miktex.mak: c4p
## 
## Copyright (C) 1991-2005 Christian Schenk
## 
## This file is part of C4P.
## 
## C4P is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
## 
## C4P is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
## or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
## License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with C4P; if not, write to the Free Software Foundation, 59
## Temple Place - Suite 330, Boston, MA 02111-1307, USA.

miktex_cc_no_runtime_checks = 1

!include <miktex.inc>

objects = \
	$(outdir)\gram.obj \
	$(outdir)\main.obj \
	$(outdir)\misc.obj \
	$(outdir)\output.obj \
	$(outdir)\scan.obj \
	$(outdir)\subrange.obj \
	$(outdir)\symtab.obj \
	$(outdir)\type.obj \

sources = \
	gram.cpp \
	main.cpp \
	misc.cpp \
	output.cpp \
	scan.cpp \
	subrange.cpp \
	symtab.cpp \
	type.cpp \

binaries = $(outdir)\c4p.exe

.cpp{$(outdir)\}.obj:
	$(cc) $(cppstandard) \
		$(ccopt_all_cpp) \
		$(ccopt_output_directory)$(outdir)\ \
		-Dalloca=_alloca \
		$<

all: common-all $(binaries)

install: common-install install-binaries

check: common-check


# -----------------------------------------------------------------------------
# c4p.exe
# -----------------------------------------------------------------------------

c4p_objects = \
	$(gnu_lib) \
	$(objects) \
	$(outdir)\c4p.res \

$(outdir)\c4p.exe: \
			$(outdir) \
			$(c4p_objects) \

	$(link) $(lstandard) \
		-out:$@ \
		$(c4p_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\c4p.res: \
			$(outdir) \
			$(libdir)\include\miktex\version.h \
			$(libdir)\include\miktex\miktex.version \
			c4p-version.h \
			c4p.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ c4p.rc

scan.cpp: scan.l
	$(flex) -t $(LEXFLAGS) scan.l > $@

gram.cpp gram.h: gram.y
	$(yacc) -d gram.y
	-del /f gram.cpp
	-del /f gram.h
	move y.tab.c gram.cpp
	move y.tab.h gram.h

# -----------------------------------------------------------------------------
# clean-up
# -----------------------------------------------------------------------------

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean
	-del /f gram.cpp
	-del /f gram.h
	-del /f scan.cpp

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

# -----------------------------------------------------------------------------
# dependencies
# -----------------------------------------------------------------------------

!include <common-dependencies.inc>

depend: $(sources)
	$(MAKEDEPEND) $(sources)
	$(fixdep)

# DO NOT DELETE

$(outdir)\gram.obj: common.h
$(outdir)\main.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h common.h
$(outdir)\main.obj: c4p-version.h
$(outdir)\misc.obj: c4p.rc c4p-version.h
$(outdir)\misc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/miktex.version
$(outdir)\misc.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/version.h
$(outdir)\misc.obj: common.h
$(outdir)\output.obj: common.h
$(outdir)\scan.obj: common.h gram.h
$(outdir)\subrange.obj: common.h
$(outdir)\symtab.obj: common.h gram.h
$(outdir)\type.obj: common.h gram.h
