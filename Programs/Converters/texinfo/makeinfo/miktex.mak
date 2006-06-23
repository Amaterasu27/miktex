## miktex.mak: texinfo/makeinfo
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

!if ($(MSCVER) < 1400)
miktex_cc_disable_optimization = 1
!endif

!include <miktex.inc>

objects = \
	$(outdir)\cmds.obj \
	$(outdir)\defun.obj \
	$(outdir)\files.obj \
	$(outdir)\float.obj \
	$(outdir)\footnote.obj \
	$(outdir)\html.obj \
	$(outdir)\index.obj \
	$(outdir)\insertion.obj \
	$(outdir)\lang.obj \
	$(outdir)\macro.obj \
	$(outdir)\makeinfo.obj \
	$(outdir)\multi.obj \
	$(outdir)\node.obj \
	$(outdir)\sectioning.obj \
	$(outdir)\toc.obj \
	$(outdir)\wrapper.obj \
	$(outdir)\xml.obj \
	$(outdir)\xref.obj \

sources = \
	cmds.c \
	defun.c \
	files.c \
	float.c \
	footnote.c \
	html.c \
	index.c \
	insertion.c \
	lang.c \
	macro.c \
	makeinfo.c \
	multi.c \
	node.c \
	sectioning.c \
	toc.c \
	$(wrapper_cpp) \
	xml.c \
	xref.c \

.c{$(outdir)\}.obj:
	$(cc) \
		-I.. \
		$(cstandard) \
		$(ccopt_output_directory)$(outdir)\ \
		-DR_OK=0 \
		-Dalloca=_alloca \
		-Dindex=_tcschr \
		-Dmain=Main \
		-Dstrcasecmp=_tcsicmp \
		-Dstrncasecmp=_tcsnicmp \
		-Dxexit=exit \
		-Dxmalloc=malloc \
		-Dxrealloc=realloc \
		-Dxstrdup=_tcsdup \
		$<

binaries = $(outdir)\makeinfo.exe

all: common-all $(binaries)

makeinfo_objects = \
	$(app_lib) \
	$(gnu_lib) \
	$(miktex_lib) \
	$(objects) \
	$(outdir)\makeinfo.res \
	$(texmf_lib) \

$(outdir)\makeinfo.exe: \
			$(outdir) \
			$(makeinfo_objects) \

	$(link) $(lstandard) \
		$(linkopt_output_file)$@ \
		$(makeinfo_objects)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(outdir)\makeinfo.res: \
		$(outdir) \
		$(libdir)\include\miktex\version.h \
		$(libdir)\include\miktex\miktex.version \
		makeinfo-version.h \
		makeinfo.rc \

	$(rc) $(rcstandard) $(rcopt_output_file)$@ makeinfo.rc

$(outdir)\wrapper.obj: $(wrapper_cpp)
	$(cc) $(cppstandard_aeh) \
		$(ccopt_enable_function_level_linking) \
		$(ccopt_select_fastcall) \
		$(ccopt_output_file)$(outdir)\ \
		$(wrapper_cpp)

install: common-install install-binaries

check: common-check

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

maintainer-clean: common-maintainer-clean distclean

!include <common-dependencies.inc>

depend: $(sources)
	$(MAKEDEPEND) -I.. $(sources)
	$(fixdep)


# DO NOT DELETE

$(outdir)\cmds.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\cmds.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h cmds.h
$(outdir)\cmds.obj: defun.h insertion.h files.h footnote.h html.h lang.h macro.h
$(outdir)\cmds.obj: makeinfo.h node.h xref.h sectioning.h toc.h xml.h
$(outdir)\defun.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\defun.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h defun.h
$(outdir)\defun.obj: insertion.h xml.h makeinfo.h cmds.h html.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\files.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h files.h
$(outdir)\files.obj: html.h index.h makeinfo.h cmds.h macro.h node.h xref.h
$(outdir)\float.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\float.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\float.obj: makeinfo.h cmds.h files.h float.h html.h sectioning.h xml.h
$(outdir)\footnote.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\footnote.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\footnote.obj: footnote.h macro.h makeinfo.h node.h xref.h xml.h
$(outdir)\html.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\html.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h cmds.h
$(outdir)\html.obj: files.h html.h lang.h makeinfo.h node.h xref.h sectioning.h
$(outdir)\index.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\index.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h files.h
$(outdir)\index.obj: footnote.h html.h index.h makeinfo.h cmds.h lang.h macro.h
$(outdir)\index.obj: sectioning.h toc.h xml.h
$(outdir)\insertion.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\insertion.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\insertion.obj: cmds.h defun.h insertion.h float.h html.h macro.h
$(outdir)\insertion.obj: makeinfo.h multi.h xml.h
$(outdir)\lang.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\lang.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h cmds.h
$(outdir)\lang.obj: files.h lang.h makeinfo.h xml.h
$(outdir)\macro.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\macro.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h cmds.h
$(outdir)\macro.obj: files.h macro.h makeinfo.h insertion.h
$(outdir)\makeinfo.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\makeinfo.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\makeinfo.obj: $(miktexsrcdir)/Libraries/3rd/libgnu/getopt.h
$(outdir)\makeinfo.obj: makeinfo.h cmds.h files.h float.h footnote.h html.h
$(outdir)\makeinfo.obj: index.h insertion.h lang.h macro.h node.h xref.h
$(outdir)\makeinfo.obj: sectioning.h toc.h xml.h
$(outdir)\multi.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\multi.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h cmds.h
$(outdir)\multi.obj: insertion.h makeinfo.h multi.h xml.h
$(outdir)\node.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\node.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h cmds.h
$(outdir)\node.obj: files.h float.h footnote.h macro.h makeinfo.h node.h xref.h
$(outdir)\node.obj: html.h sectioning.h insertion.h xml.h
$(outdir)\sectioning.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\sectioning.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h
$(outdir)\sectioning.obj: cmds.h macro.h makeinfo.h node.h xref.h toc.h
$(outdir)\sectioning.obj: sectioning.h xml.h
$(outdir)\toc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\toc.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h makeinfo.h
$(outdir)\toc.obj: cmds.h files.h macro.h node.h xref.h html.h lang.h
$(outdir)\toc.obj: sectioning.h toc.h xml.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/core.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/MiKTeX/Core/include/miktex/char.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/3rd/libmd5/include/miktex/md5.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/MiKTeX/App/include/miktex/app.h
$(outdir)\wrapper.obj: $(miktexsrcdir)/Libraries/MiKTeX/PackageManager/include/miktex/mpm.h
$(outdir)\xml.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\xml.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h makeinfo.h
$(outdir)\xml.obj: insertion.h files.h float.h macro.h cmds.h lang.h xml.h
$(outdir)\xref.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/system.h
$(outdir)\xref.obj: $(miktexsrcdir)/Libraries/3rd/libmspack/mspack.h cmds.h
$(outdir)\xref.obj: float.h html.h index.h makeinfo.h macro.h node.h xref.h xml.h
