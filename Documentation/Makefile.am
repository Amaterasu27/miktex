## Makefile.am: doc
##
## Copyright (C) 2005-2006 Christian Schenk
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

manual_xml_files = \
	Manual/configuring.xml \
	Manual/envvars.xml \
	Manual/features.xml \
	Manual/installation-instructions.xml \
	Manual/installing.xml \
	Manual/intro.xml \
	Manual/localguide.xml \
	Manual/printing.xml \
	Manual/tcxtables.xml \
	Manual/texfeatures.xml \
	Manual/texifying.xml \
	Manual/tracestreams.xml \
	EnvVars/BIBINPUTS.xml \
	EnvVars/BSTINPUTS.xml \
	EnvVars/MFINPUTS.xml \
	EnvVars/MIKTEX_REPOSITORY.xml \
	EnvVars/MIKTEX_TRACE.xml \
	EnvVars/MPINPUTS.xml \
	EnvVars/TEXINPUTS.xml \
	EnvVars/TFMFONTS.xml \
	Options/alias.xml \
	Options/auxdirectory.xml \
	Options/bistacksize.xml \
	Options/bufsize.xml \
	Options/cstyleerrors.xml \
	Options/disableinstaller.xml \
	Options/disablepipes.xml \
	Options/disablewriteeighteen.xml \
	Options/dontparsefirstline.xml \
	Options/enableinstaller.xml \
	Options/enablepipes.xml \
	Options/enablewriteeighteen.xml \
	Options/errorline.xml \
	Options/fontmax.xml \
	Options/fontmemsize.xml \
	Options/halferrorline.xml \
	Options/haltonerror.xml \
	Options/help.xml \
	Options/hhelp.xml \
	Options/includedirectory.xml \
	Options/initialize.xml \
	Options/interaction.xml \
	Options/jobname.xml \
	Options/jobtime.xml \
	Options/ligtablesize.xml \
	Options/maxinopen.xml \
	Options/maxprintline.xml \
	Options/maxstrings.xml \
	Options/maxwiggle.xml \
	Options/membot.xml \
	Options/memmax.xml \
	Options/memmin.xml \
	Options/memtop.xml \
	Options/mltex.xml \
	Options/movesize.xml \
	Options/nestsize.xml \
	Options/nocstyleerrors.xml \
	Options/outputdirectory.xml \
	Options/paramsize.xml \
	Options/parsefirstline.xml \
	Options/pathsize.xml \
	Options/poolsize.xml \
	Options/quiet.xml \
	Options/recorder.xml \
	Options/recordpackageusages.xml \
	Options/savesize.xml \
	Options/srcspecials.xml \
	Options/stacksize.xml \
	Options/stringvacancies.xml \
	Options/tcx.xml \
	Options/terminal.xml \
	Options/timestatistics.xml \
	Options/trace.xml \
	Options/trieopsize.xml \
	Options/triesize.xml \
	Options/undump.xml \
	Options/version.xml \
	Ref/bibtex.xml \
	Ref/dvicopy.xml \
	Ref/dvips.xml \
	Ref/findtexmf.xml \
	Ref/gftodvi.xml \
	Ref/initexmf.xml \
	Ref/mf.xml \
	Ref/mpm.xml \
	Ref/mpost.xml \
	Ref/mthelp.xml \
	Ref/mtprint.xml \
	Ref/omega.xml \
	Ref/pdfetex.xml \
	Ref/pdftexcfg.xml \
	Ref/setupwiz.xml \
	Ref/tex.xml \
	Ref/texify.xml \
	Ref/updmapcfg.xml \
	miktex.xml

manual_listings = \
	$(top_srcdir)/Admin/Defaults/mf.defaults \
	$(top_srcdir)/Admin/Defaults/mfapp.defaults \
	$(top_srcdir)/Admin/Defaults/mpost.defaults \
	$(top_srcdir)/Admin/Defaults/omega.defaults \
	$(top_srcdir)/Admin/Defaults/pdftex.defaults \
	$(top_srcdir)/Admin/Defaults/texapp.defaults \
	$(top_srcdir)/Admin/Defaults/texmfapp.defaults \
	filetypes

man_MANS = 
man_MANS += initexmf.1
man_MANS += mpm.1
man_MANS += mthelp.1

ver = $(MIKTEX_MAJOR_VERSION).$(MIKTEX_MINOR_VERSION)

EXTRA_DIST =
EXTRA_DIST += $(man_MANS)
EXTRA_DIST += $(manual_xml_files)
EXTRA_DIST += $(manual_listings)
EXTRA_DIST += Styles/htmltext.xsl
EXTRA_DIST += Styles/manpages.xsl
EXTRA_DIST += entities.ent
EXTRA_DIST += news.xml
EXTRA_DIST += news/news-$(ver).xml

MAINTAINERCLEANFILES = *.1

initexmf.1: miktex.xml Ref/initexmf.xml Styles/manpages.xsl entities.ent
	XML_CATALOG_FILES=$(top_srcdir)/BuildUtilities/docbook/catalog \
		xsltproc --path $(srcdir) Styles/manpages.xsl miktex.xml

mpm.1: miktex.xml Ref/mpm.xml Styles/manpages.xsl entities.ent
	XML_CATALOG_FILES=$(top_srcdir)/BuildUtilities/docbook/catalog \
		xsltproc --path $(srcdir) Styles/manpages.xsl miktex.xml

mthelp.1: miktex.xml Ref/mthelp.xml Styles/manpages.xsl entities.ent
	XML_CATALOG_FILES=$(top_srcdir)/BuildUtilities/docbook/catalog \
		xsltproc --path $(srcdir) Styles/manpages.xsl miktex.xml

dist-hook:
	XML_CATALOG_FILES=$(top_srcdir)/BuildUtilities/docbook/catalog \
		xsltproc --path $(srcdir) \
			--output tmp.html Styles/htmltext.xsl news.xml
	lynx -dump -hiddenlinks=ignore -nolist tmp.html > $(top_distdir)/NEWS
	rm tmp.html
