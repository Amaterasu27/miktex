## miktex.mak: yap
##
## Copyright (C) 1996-2006 Christian Schenk
## 
## This file is part of Yap.
## 
## Yap is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
## 
## Yap is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
## or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
## License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with Yap; if not, write to the Free Software Foundation, 59
## Temple Place - Suite 330, Boston, MA 02111-1307, USA.

!include <miktex.inc>

!if $(MIKTEX_RELEASE_STATE) <= 2
cfg = Release
!else
cfg = Debug
!endif

binaries = $(outdir)\yap.exe

documents = yap.chm

all: common-all $(binaries) $(documents)

$(binaries): make-binaries

make-binaries: yaphelp.h
	vcbuild /nologo yap2005.vcproj $(cfg)

install: common-install install-binaries install-documents

check:

mostlyclean: common-mostlyclean
	if exist *.hhk del /f /q *.hhk
	if exist *.hhc del /f /q *.hhc
	if exist *.hhp del /f /q *.hhp
	if exist *.html del /f /q *.html
	if exist *.chm del /f /q *.chm

clean: common-clean mostlyclean
	vcbuild /nologo /clean yap2005.vcproj $(cfg)

distclean: common-distclean clean
	-$(rmdir) /s /q scratchdir
	-del /f yaphelp.h

maintainer-clean: common-maintainer-clean distclean

xml_files = \
	..\Documentation\Guide\commandline.xml \
	..\Documentation\Guide\customizing.xml \
	..\Documentation\Guide\dvisearch.xml \
	..\Documentation\Guide\graphics.xml \
	..\Documentation\Guide\keyboard.xml \
	..\Documentation\Guide\menus.xml \
	..\Documentation\Guide\printing.xml \
	..\Documentation\Guide\specials.xml \
	..\Documentation\Guide\tools.xml \
	..\Documentation\Guide\tracing.xml \
	..\Documentation\Guide\troubleshooting.xml \
	..\Documentation\Guide\yap.xml \

yap.chm: \
		$(xml_files) \
		$(xsldir)\htmlhelp.xsl \
		$(cssdir)\miktexhelp.css \
		scratchdir \
		yaphelp.h \

	copy $(cssdir)\miktexhelp.css scratchdir
	$(xsltproc) \
		--path "file://$(xsldir:\=/) file://$(miktexsrcdir:\=/)\$(rel_docdir:\=/)/Manual" \
		--stringparam base.dir scratchdir/ \
		--stringparam chunk.fast 1 \
		--stringparam manifest.in.base.dir 1 \
		htmlhelp.xsl ..\Documentation\Guide\yap.xml
	copy yaphelp.h scratchdir\context.h
	cd scratchdir
	makehm /h /a afxhh.h IDW_,HIDW_,0x50000 ..\resource.h>> context.h
	-$(hhc) htmlhelp
	copy htmlhelp.chm ..\yap.chm
	cd ..

yaphelp.h: resource.h
	@echo #define KEYBOARD_SHORTCUTS 101> $@
	makehm /h ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h>> $@
	makehm /h IDP_,HIDP_,0x30000 resource.h>> $@
	makehm /h IDR_,HIDR_,0x20000 resource.h>> $@
	makehm /h IDD_,HIDD_,0x20000 resource.h>> $@
	makehm /h IDW_,HIDW_,0x50000 resource.h>> $@

scratchdir:
	if not exist scratchdir $(mkpath) scratchdir

!include <common-dependencies.inc>

depend:
