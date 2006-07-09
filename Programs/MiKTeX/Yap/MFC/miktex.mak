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

docdir = $(prefix)\doc\miktex

documents = yap.chm

all: common-all $(binaries) $(documents)

$(binaries): make-binaries

make-binaries: yaphelp.h
	vcbuild /nologo yap2005.vcproj $(cfg)

install: common-install install-binaries install-documents

check:

mostlyclean: common-mostlyclean
	if exist *.chm del /f /q *.chm

clean: common-clean mostlyclean
	vcbuild /nologo /clean yap2005.vcproj $(cfg)
	if exist www $(rmdir) www

distclean: common-distclean clean
	-del /f yaphelp.h

maintainer-clean: common-maintainer-clean distclean

dbxsldir = C:\sgml\docbook\docbook-xsl-1.69.0

html_docbook_images = \
	$(dbxsldir)\images\blank.png \
	$(dbxsldir)\images\caution.png \
	$(dbxsldir)\images\draft.png \
	$(dbxsldir)\images\home.png \
	$(dbxsldir)\images\important.png \
	$(dbxsldir)\images\next.png \
	$(dbxsldir)\images\note.png \
	$(dbxsldir)\images\prev.png \
	$(dbxsldir)\images\tip.png \
	$(dbxsldir)\images\toc-blank.png \
	$(dbxsldir)\images\toc-minus.png \
	$(dbxsldir)\images\toc-plus.png \
	$(dbxsldir)\images\up.png \
	$(dbxsldir)\images\warning.png \

screenshots_png = \
	..\Screenshots\propui.png \
	..\Screenshots\propdisplay.png \
	..\Screenshots\propprinter.png \
	..\Screenshots\propmagglass.png \
	..\Screenshots\propinvsearch.png \
	..\Screenshots\propsecurity.png \
	..\Screenshots\propadvanced.png \
	..\Screenshots\ruler.png \
	..\Screenshots\magglass.png \

html_yap_guide_images = \
	$(html_docbook_images) \
	$(screenshots_png) \

yap_guide_xml_files = \
	..\Documentation\Guide\commandline.xml \
	..\Documentation\Guide\customizing.xml \
	..\Documentation\Guide\dvisearch.xml \
	..\Documentation\Guide\keyboard.xml \
	..\Documentation\Guide\menus.xml \
	..\Documentation\Guide\printing.xml \
	..\Documentation\Guide\tools.xml \
	..\Documentation\Guide\yap.xml \

xslt_path = file://$(xsldir:\=/)
xslt_path = $(xslt_path) file://$(miktexsrcdir:\=/)\$(rel_docdir:\=/)/Manual

yap.chm: \
		$(cssdir)\miktexhelp.css \
		$(miktexsrcdir)\$(rel_docdir)\entities.ent \
		$(yap_guide_xml_files) \
		$(xsldir)\htmlhelp.xsl \
		yaphelp.h \

	if exist scratch $(rmdir) scratch
	mkdir scratch
	copy $(cssdir)\miktexhelp.css scratch
	mkdir scratch\images
	for %f in ($(html_yap_guide_images)) \
	  do copy %f scratch\images
	$(xsltproc) \
		--path "$(xslt_path)" \
		--stringparam base.dir scratch/ \
		--stringparam chunk.fast 1 \
		--stringparam manifest.in.base.dir 1 \
		htmlhelp.xsl \
		..\Documentation\Guide\yap.xml
	copy yaphelp.h scratch\context.h
	cd scratch
	makehm /h /a afxhh.h IDW_,HIDW_,0x50000 ..\resource.h>> context.h
	-$(hhc) htmlhelp
	copy htmlhelp.chm ..\yap.chm
	cd ..
	$(rmdir) scratch

www-yap-guide: \
	www\yap \
	www\yap\index.html \
	www\yap\miktexman.css \
	www-yap-guide-images \

www\yap:
	$(mkpath) $@

www-yap-guide-images: \
			www\yap \
			www\yap\images \
			$(html_yap_guide_images) \

	for %f in ($(html_yap_guide_images)) do copy %f www\yap\images

www\yap\images:
	$(mkpath) $@

www\yap\index.html: \
			www\yap \
			$(yap_guide_xml_files) \
			$(xsldir)\htmlchunk.xsl \

	$(xsltproc) \
		--path "$(xslt_path)" \
		--stringparam base.dir www/yap/ \
		--stringparam chunk.fast 1 \
		$(xsldir)\htmlchunk.xsl \
		..\Documentation\Guide\yap.xml

www\yap\miktexman.css: $(cssdir)\miktexman.css
	copy $(cssdir)\miktexman.css www\yap\miktexman.css

upload-yap-guide: www-yap-guide
	$(sitecopy) --update yap-guide

yaphelp.h: resource.h
	@echo #define KEYBOARD_SHORTCUTS 101> $@
	makehm /h ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h>> $@
	makehm /h IDP_,HIDP_,0x30000 resource.h>> $@
	makehm /h IDR_,HIDR_,0x20000 resource.h>> $@
	makehm /h IDD_,HIDD_,0x20000 resource.h>> $@
	makehm /h IDW_,HIDW_,0x50000 resource.h>> $@

!include <common-dependencies.inc>

depend:
