## miktex.mak: doc
## 
## Copyright (C) 1996-2006 Christian Schenk
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

localguidedocdir = $(docdir)\latex\00LocalGuide

www_user = csc
www_host = miktex.sourceforge.net
www_dir = /home/groups/m/mi/miktex/htdocs

sunsite_ftp_user = miktex
sunsite_ftp_server = upload.sunsite.dk
sunsite_ftp_dir = /pub

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
	Images\autoinstall.png \
	Images\browselocaltexmf.png \
	Images\localtexmf.png \
	Images\mo-general.png \
	Images\mo-languages.png \
	Images\mo-mark-package.png \
	Images\mo-packages.png \
	Images\mo-roots.png \
	Images\mo-roots2.png \
	Images\mo-roots3.png \
	Images\mo-search-package.png \
	Images\updatewiz-download-type.png \
	Images\updatewiz-packagelist.png \
	Images\updatewiz-progress.png \
	Images\updatewiz-remote-repository.png \
	Images\updatewiz-source.png \
	Images\mpm.png \
	Images\yap-rotating.png \
	Images\yap-ruler.png \
	Images\yap-texeditor.png \
	Images\yap-inversesearchoptions.png \

screenshots_eps = $(screenshots_png:.png=.eps)
screenshots_bb = $(screenshots_png:.png=.bb)

html_manual_images = \
	$(html_docbook_images) \
	$(screenshots_png) \

html_roadmap_images = \
	$(html_docbook_images) \

image_files_to_be_installed = \
	$(html_manual_images) \

chm_files = \
	faq.chm \
	miktex.chm \

html_files = \
	install.html \
	license.html \
	relnotes.html \

pdf_files = \
	install.pdf \
	miktex.pdf \

manual_xml_files = \
	Manual\configuring.xml \
	Manual\features.xml \
	Manual\installation-instructions.xml \
	Manual\installing.xml \
	Manual\intro.xml \
	Manual\localguide.xml \
	Manual\printing.xml \
	Manual\tcxtables.xml \
	Manual\texifying.xml \
	Options\alias.xml \
	Options\auxdirectory.xml \
	Options\bufsize.xml \
	Options\cstyleerrors.xml \
	Options\enablewriteeighteen.xml \
	Options\errorline.xml \
	Options\fontmax.xml \
	Options\halferrorline.xml \
	Options\haltonerror.xml \
	Options\help.xml \
	Options\includedirectory.xml \
	Options\initialize.xml \
	Options\interaction.xml \
	Options\jobname.xml \
	Options\jobtime.xml \
	Options\maxinopen.xml \
	Options\maxprintline.xml \
	Options\maxstrings.xml \
	Options\membot.xml \
	Options\memmax.xml \
	Options\memmin.xml \
	Options\memtop.xml \
	Options\nestsize.xml \
	Options\outputdirectory.xml \
	Options\paramsize.xml \
	Options\poolsize.xml \
	Options\quiet.xml \
	Options\savesize.xml \
	Options\srcspecials.xml \
	Options\stacksize.xml \
	Options\stringvacancies.xml \
	Options\tcx.xml \
	Options\terminal.xml \
	Options\timestatistics.xml \
	Options\trace.xml \
	Options\trieopsize.xml \
	Options\triesize.xml \
	Options\trygz.xml \
	Options\undump.xml \
	Options\version.xml \
	Ref\bibtex.xml \
	Ref\dvicopy.xml \
	Ref\dvips.xml \
	Ref\etex.xml \
	Ref\findtexmf.xml \
	Ref\gftodvi.xml \
	Ref\initexmf.xml \
	Ref\mf.xml \
	Ref\mpost.xml \
	Ref\mthelp.xml \
	Ref\mtprint.xml \
	Ref\omega.xml \
	Ref\pdftex.xml \
	Ref\pdftexcfg.xml \
	Ref\tex.xml \
	Ref\texify.xml \
	Ref\updmapcfg.xml \
	miktex.xml \

documents = $(html_files) $(chm_files) $(pdf_files)

.SUFFIXES: .png .eps .xml .bb

.png.bb:
	$(bmeps) -b $< $@

.eps.png:
	$(convert) $< $@

.eps.bb:
	$(convert) $< xxx.png
	$(bmeps) -b xxx.png $@
	del xxx.png

check: common-check

all: common-all $(image_files_to_be_installed) $(documents)

install: \
	common-install \
	install-images \
	install-miktex-documents \
	install-local-guide \

install-miktex-documents: $(documents)
	if not exist $(miktexdocdir) mkdir $(miktexdocdir)
	for %f in ($(documents)) do \
	  copy %f $(miktexdocdir)

install-images: $(image_files_to_be_installed)
	if not exist $(miktexdocdir)\images mkdir $(miktexdocdir)\images
	for %f in ($(image_files_to_be_installed)) do \
	  copy %f $(miktexdocdir)\images

# -----------------------------------------------------------------------------
# local guide
# -----------------------------------------------------------------------------

install-local-guide: $(localguidedocdir) local.txt
	copy local.txt $(localguidedocdir)

$(localguidedocdir):
	$(mkpath) $@

# -----------------------------------------------------------------------------
# FAQ
# -----------------------------------------------------------------------------

faq.chm: \
			entities.ent \
			faq.xml \
			$(xsldir)\htmlhelp.xsl \
			$(cssdir)\miktexhelp.css \

	if exist scratch $(rmdir) scratch
	mkdir scratch
	copy $(cssdir)\miktexhelp.css scratch
	$(xsltproc) \
		--stringparam base.dir scratch/ \
		--stringparam chunk.fast 1 \
		--stringparam manifest.in.base.dir 1 \
		$(xsldir)\htmlhelp.xsl \
		faq.xml
	cd scratch
	-$(hhc) htmlhelp
	copy htmlhelp.chm ..\faq.chm
	cd ..
	$(rmdir) scratch

faq.tex: \
			$(xsldir)\db2latex-common.xsl \
			$(xsldir)\db2latex-install.xsl \
			entities.ent \
			faq.xml \

	$(xsltproc) --output $@ $(xsldir)\db2latex-install.xsl faq.xml

faq.pdf: \
			faq.tex \
			png75 \

	if exist scratch $(rmdir) scratch
	mkdir scratch
	mkdir scratch\images
	copy png75\*.* scratch\images
	cd scratch
	$(texify) --pdf ..\faq.tex
	copy faq.pdf ..\faq.pdf
	cd ..
	$(rmdir) scratch

www-faq: \
			www\faq \
			www\faq\index.html \
			www\faq\faq.html \
			www\faq\miktexman.css \
			www-faq-images \

www\faq:
	$(mkpath) $@

html_faq_images = \
	$(html_docbook_images) \

www-faq-images: \
			www\faq\images \
			$(html_faq_images) \

	for %f in ($(html_faq_images)) do copy %f www\faq\images

www\faq\images:
	$(mkpath) $@

www\faq\index.html: \
			www\faq \
			faq.xml \
			$(xsldir)\htmlchunk.xsl \

	$(xsltproc) \
		--stringparam base.dir www/faq/ \
		--stringparam chunk.fast 1 \
		$(xsldir)\htmlchunk.xsl faq.xml

www\faq\faq.html: \
			www\faq \
			faq.xml \

	$(xsltproc) \
		--output $@ \
		--stringparam base.dir www/faq/ \
		$(xsldir)\html.xsl faq.xml

www\faq\miktexman.css: $(cssdir)\miktexman.css
	copy $(cssdir)\miktexman.css www\faq\miktexman.css

upload-faq: www-faq
	$(sitecopy) --update faq

# -----------------------------------------------------------------------------
# Installation Instructions
# -----------------------------------------------------------------------------

install.html: \
			$(xsldir)\html.xsl \
			entities.ent \
			install.xml \
			Manual\installation-instructions.xml \
			$(cssdir)\miktexman.css \

	$(xsltproc) --output $@ $(xsldir)\html.xsl install.xml

install.tex: \
			$(xsldir)\db2latex-common.xsl \
			$(xsldir)\db2latex-install.xsl \
			entities.ent \
			install.xml \
			Manual\installation-instructions.xml \

	$(xsltproc) --output $@ $(xsldir)\db2latex-install.xsl install.xml

install.pdf: \
			install.tex \
			png75 \

	if exist scratch $(rmdir) scratch
	mkdir scratch
	mkdir scratch\images
	copy png75\*.* scratch\images
	cd scratch
	$(texify) --pdf ..\install.tex
	copy install.pdf ..\install.pdf
	cd ..
	$(rmdir) scratch

# -----------------------------------------------------------------------------
# Release Notes
# -----------------------------------------------------------------------------

relnotes.html: \
			entities.ent \
			$(xsldir)\html.xsl \
			$(cssdir)\miktexman.css \
			relnotes.xml \

	$(xsltproc) --output $@ $(xsldir)\html.xsl relnotes.xml

# -----------------------------------------------------------------------------
# license.html
# -----------------------------------------------------------------------------

license.html: \
			entities.ent \
			$(xsldir)\html.xsl \
			$(cssdir)\miktexman.css \
			license.xml \

	$(xsltproc) --output $@ $(xsldir)\html.xsl license.xml

# -----------------------------------------------------------------------------
# Preview
# -----------------------------------------------------------------------------

preview.tex: \
			preview.xml \
			$(xsldir)\db2latex-common.xsl \
			$(xsldir)\db2latex-preview.xsl \
			entities.ent \

	$(xsltproc) --output $@ $(xsldir)\db2latex-preview.xsl preview.xml

preview.pdf: \
			preview.tex \
			png75 \

	if exist scratch $(rmdir) scratch
	mkdir scratch
	mkdir scratch\images
	copy png75\*.* scratch\images
	cd scratch
	$(texify) --pdf ..\preview.tex
	copy preview.pdf ..\preview.pdf
	cd ..
	$(rmdir) scratch

# -----------------------------------------------------------------------------
# NEWS
# -----------------------------------------------------------------------------

$(miktexsrcdir)\NEWS: \
			entities.ent \
			$(xsldir)\htmltext.xsl \
			news.xml \
			news\news-2.5.xml \

	$(xsltproc) --output tmp.html $(xsldir)\htmltext.xsl news.xml
	lynx -dump -hiddenlinks=ignore -nolist tmp.html > $@
	del tmp.html

# -----------------------------------------------------------------------------
# Manual
# -----------------------------------------------------------------------------

miktex.chm: \
			$(cssdir)\miktexhelp.css \
			$(html_manual_images) \
			$(libdir)\include\miktex\help.h \
			$(manual_xml_files)\
			$(xsldir)\htmlhelp.xsl \
			entities.ent \
			version.ent \

	if exist scratch $(rmdir) scratch
	mkdir scratch
	copy $(cssdir)\miktexhelp.css scratch
	mkdir scratch\images
	for %f in ($(html_manual_images)) \
	  do copy %f scratch\images
	$(xsltproc) \
		--stringparam base.dir scratch/ \
		--stringparam chunk.fast 1 \
		--stringparam manifest.in.base.dir 1 \
		$(xsldir)\htmlhelp.xsl \
		miktex.xml
	cd scratch
	copy $(libdir)\include\miktex\help.h context.h
	-$(hhc) htmlhelp
	copy htmlhelp.chm ..\miktex.chm
	cd ..
	$(rmdir) scratch

miktex.tex: \
			$(manual_xml_files) \
			$(xsldir)\db2latex-common.xsl \
			$(xsldir)\db2latex-manual.xsl \
			entities.ent \

	$(xsltproc) --output $@ $(xsldir)\db2latex-manual.xsl miktex.xml

miktex.pdf: \
			miktex.tex \
			png75 \

	if exist scratch $(rmdir) scratch
	mkdir scratch
	mkdir scratch\images
	copy png75\*.* scratch\images
	cd scratch
	$(texify) --pdf ..\miktex.tex
	copy miktex.pdf ..\miktex.pdf
	cd ..
	$(rmdir) scratch

miktex.dvi: \
			miktex.tex \
			png75 \

	if exist scratch $(rmdir) scratch
	mkdir scratch
	mkdir scratch\images
	copy png75\*.* scratch\images
	cd scratch
	$(texify) ..\miktex.tex
	copy miktex.dvi ..\miktex.dvi
	cd ..
	$(rmdir) scratch

www-manual: \
			www\manual \
			www\manual\index.html \
			www\manual\miktexman.css \
			www-manual-images \

www\manual:
	$(mkpath) $@

www-manual-images: \
			www\manual \
			www\manual\images \
			$(html_manual_images) \

	for %f in ($(html_manual_images)) do copy %f www\manual\images

www\manual\images:
	$(mkpath) $@

www\manual\index.html: \
			www\manual \
			$(manual_xml_files) \
			$(xsldir)\htmlchunk.xsl \

	$(xsltproc) \
		--stringparam base.dir www/manual/ \
		--stringparam chunk.fast 1 \
		$(xsldir)\htmlchunk.xsl \
		miktex.xml

www\manual\miktexman.css: $(cssdir)\miktexman.css
	copy $(cssdir)\miktexman.css www\manual\miktexman.css

upload-manual: www-manual
	$(sitecopy) --update manual

# -----------------------------------------------------------------------------
# Reference
# -----------------------------------------------------------------------------

refman.chm: \
			$(cssdir)\miktexhelp.css \
			$(html_manual_images) \
			$(libdir)\include\miktex\help.h \
			$(manual_xml_files)\
			$(xsldir)\htmlhelp.xsl \
			entities.ent \
			version.ent \

	if exist scratch $(rmdir) scratch
	mkdir scratch
	copy $(cssdir)\miktexhelp.css scratch
	mkdir scratch\images
	for %f in ($(html_manual_images)) \
	  do copy %f scratch\images
	$(xsltproc) \
		--stringparam base.dir scratch/ \
		--stringparam chunk.fast 1 \
		--stringparam manifest.in.base.dir 1 \
		$(xsldir)\htmlhelp.xsl \
		refman.xml
	cd scratch
	copy $(libdir)\include\miktex\help.h context.h
	-$(hhc) htmlhelp
	copy htmlhelp.chm ..\refman.chm
	cd ..
	$(rmdir) scratch

refman.tex: \
			$(manual_xml_files) \
			$(xsldir)\db2latex-common.xsl \
			$(xsldir)\db2latex-manual.xsl \
			entities.ent \

	$(xsltproc) --output $@ $(xsldir)\db2latex-manual.xsl refman.xml

refman.pdf: \
			refman.tex \
			png75 \

	if exist scratch $(rmdir) scratch
	mkdir scratch
	mkdir scratch\images
	copy png75\*.* scratch\images
	cd scratch
	$(texify) --pdf ..\refman.tex
	copy refman.pdf ..\refman.pdf
	cd ..
	$(rmdir) scratch

refman.dvi: \
			refman.tex \
			png75 \

	if exist scratch $(rmdir) scratch
	mkdir scratch
	mkdir scratch\images
	copy png75\*.* scratch\images
	cd scratch
	$(texify) ..\refman.tex
	copy refman.dvi ..\refman.dvi
	cd ..
	$(rmdir) scratch

# -----------------------------------------------------------------------------
# Roadmap
# -----------------------------------------------------------------------------

www-roadmap: \
			www\roadmap \
			www\roadmap\index.html \
			www\roadmap\miktexman.css \
			www-roadmap-images \

www\roadmap:
	$(mkpath) $@

www-roadmap-images: \
			www\roadmap\images \
			$(html_roadmap_images) \

	for %f in ($(html_roadmap_images)) do copy %f www\roadmap\images

www\roadmap\images:
	$(mkpath) $@

www\roadmap\index.html: \
			www\roadmap \
			$(xsldir)\html.xsl \
			roadmap.xml \

	$(xsltproc) --output $@ $(xsldir)\html.xsl roadmap.xml

www\roadmap\miktexman.css: $(cssdir)\miktexman.css
	copy $(cssdir)\miktexman.css www\roadmap\miktexman.css

upload-roadmap: www-roadmap
	$(sitecopy) --update roadmap

# -----------------------------------------------------------------------------
# Website
# -----------------------------------------------------------------------------

www: \
			upload-manual \
			upload-faq \
			upload-roadmap \

# -----------------------------------------------------------------------------
# png75
# -----------------------------------------------------------------------------

png75: \
			$(screenshots_png) \
			$(screenshots_bb) \

	if not exist png75 mkdir png75
	for %f in ($(screenshots_png)) do \
	  $(convert) -scale 75% %f png75\%~nf.png
	copy images\*.bb png75

# -----------------------------------------------------------------------------
# clean-up
# -----------------------------------------------------------------------------

mostlyclean:

clean: mostlyclean
	if exist www $(rmdir) www
	if exist png75 $(rmdir) png75
	if exist scratch $(rmdir) scratch
	for %f in ( \
				$(chm_files) \
				$(html_files) \
				$(pdf_files) \
				$(screenshots_bb) \
				$(screenshots_eps) \
				install.tex \
				miktex.tex \
			) do \
		@if exist %f del /f %f

distclean: clean

maintainer-clean: distclean

# -----------------------------------------------------------------------------
# dependencies
# -----------------------------------------------------------------------------

!include <common-dependencies.inc>

depend:
