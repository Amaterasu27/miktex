## miktex.mak: top level Makefile (Windows, Visual C++)
##
## Copyright (C) 1991-2006 Christian Schenk
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

## See file win\README.win for build instructions.

!include <miktex.inc>

# _____________________________________________________________________________
#
# Projects
# _____________________________________________________________________________

# -----------------------------------------------------------------------------

prj_build_utilities1 = \
	$(rel_buildtoolsetcdir) \

projects = $(projects) $(prj_build_utilities1)

# -----------------------------------------------------------------------------

prj_3rdparty_libs1 = \
	$(rel_lzmalibdir)		\
	$(rel_lzmastaticlibdir)		\
	$(rel_bzip2dir)			\
	$(rel_bzip2staticdir)		\
	$(rel_curllibdir)		\
	$(rel_curlstaticlibdir)		\
	$(rel_expatlibdir)		\
	$(rel_expatstaticlibdir)	\
	$(rel_gnulibdir)		\
	$(rel_gnustaticlibdir)		\
	$(rel_jpeglibdir)		\
	$(rel_mspacklibdir)		\
	$(rel_mspackstaticlibdir)	\
	$(rel_poptlibdir)		\
	$(rel_poptstaticlibdir)		\
	$(rel_tifflibdir)		\
	$(rel_netpbmdir)		\
	$(rel_zlibdir)			\
	$(rel_zlibstaticlibdir)		\

projects = $(projects) $(prj_3rdparty_libs1)

# -----------------------------------------------------------------------------

prj_3rdparty_libs2 = \
	$(rel_pnglibdir)		\

projects = $(projects) $(prj_3rdparty_libs2)

# -----------------------------------------------------------------------------

prj_3rdparty_libs3 = \
	$(rel_bmepslibdir)		\
	$(rel_gdlibdir)			\

projects = $(projects) $(prj_3rdparty_libs3)

# -----------------------------------------------------------------------------

prj_3rdparty_libs4 = \
	$(rel_goostaticlibdir)		\
	$(rel_fofistaticlibdir)		\
	$(rel_xpdfstaticlibdir)		\

projects = $(projects) $(prj_3rdparty_libs4)

# -----------------------------------------------------------------------------

prj_build_utilities2 = \
	$(rel_c4pdir)		\
	$(rel_cwebdir)		\
	$(rel_tiedir)		\

projects = $(projects) $(prj_build_utilities2)

# -----------------------------------------------------------------------------

prj_miktex_libs1 = \
	$(rel_md5libdir)		\
	$(rel_md5staticlibdir)		\

projects = $(projects) $(prj_miktex_libs1)

# -----------------------------------------------------------------------------

prj_miktex_libs2 = \
	$(rel_libdir)		\
	$(rel_staticlibdir)	\

projects = $(projects) $(prj_miktex_libs2)

# -----------------------------------------------------------------------------

prj_miktex_libs3 = \
	$(rel_mpmlibdir)		\
	$(rel_mpmstaticlibdir)		\

projects = $(projects) $(prj_miktex_libs3)

# -----------------------------------------------------------------------------

prj_miktex_libs4 = \
	$(rel_mikuilibdir)		\

projects = $(projects) $(prj_miktex_libs4)

# -----------------------------------------------------------------------------

prj_miktex_libs5 = \
	$(rel_applibdir)		\
	$(rel_texmflibdir)		\

projects = $(projects) $(prj_miktex_libs5)

# -----------------------------------------------------------------------------

prj_miktex_libs6 = \
	$(rel_diblibdir)		\
	$(rel_dvilibdir)		\

projects = $(projects) $(prj_miktex_libs6)

# -----------------------------------------------------------------------------

prj_miktex_libs7 = \
	$(rel_kpslibdir)		\

projects = $(projects) $(prj_miktex_libs7)

# -----------------------------------------------------------------------------

prj_build_utilities3 = \
	$(rel_webdir)			 \

projects = $(projects) $(prj_build_utilities3)

# -----------------------------------------------------------------------------

prj_build_utilities4 = \
	$(rel_cfgdir)		\

projects = $(projects) $(prj_build_utilities4)

# -----------------------------------------------------------------------------

prj_package_manager = \
	$(rel_mpmdir)		\
	$(rel_mpmcondir)	\

projects = $(projects) $(prj_package_manager)

# -----------------------------------------------------------------------------

prj_config_utilities1 = \
	$(rel_makexlibdir)	\

projects = $(projects) $(prj_config_utilities1)

# -----------------------------------------------------------------------------

prj_config_utilities2 = \
	$(rel_initexmfdir)	\
	$(rel_configfilesdir)	\
	$(rel_findtexmfdir)	\
	$(rel_mkfntmapdir)	\
	$(rel_modir)		\

projects = $(projects) $(prj_config_utilities2)

# -----------------------------------------------------------------------------

prj_etc = \
	$(rel_etcdir)		\

projects = $(projects) $(prj_etc)

# -----------------------------------------------------------------------------

prj_tex = \
	$(rel_texetcdir)	\
	$(rel_texwaredir)	\
	$(rel_texdir)		\

projects = $(projects) $(prj_tex)

# -----------------------------------------------------------------------------

prj_pdfetex = \
	$(rel_pdftexdir)	\
	$(rel_pdfetexdir)	\

projects = $(projects) $(prj_pdfetex)

# -----------------------------------------------------------------------------

prj_omega = \
	$(rel_omegawaredir)	\
	$(rel_otpsdir)		\
	$(rel_omegadir)		\

projects = $(projects) $(prj_omega)

# -----------------------------------------------------------------------------

prj_metafont = \
	$(rel_mfwaredir)	\
	$(rel_mfdir)		\

projects = $(projects) $(prj_metafont)

# -----------------------------------------------------------------------------

prj_metapost = \
	$(rel_mpwaredir)	\
	$(rel_mpdir)		\

projects = $(projects) $(prj_metapost)

# -----------------------------------------------------------------------------

prj_makeindex = \
	$(rel_makeindexdir)	\

projects = $(projects) $(prj_makeindex)

# -----------------------------------------------------------------------------

prj_bibtex = \
	$(rel_bibtexdir)	\

projects = $(projects) $(prj_bibtex)

# -----------------------------------------------------------------------------

prj_bibtex8 = \
	$(rel_bibtex8dir)	\

projects = $(projects) $(prj_bibtex8)

# -----------------------------------------------------------------------------

prj_compiler_driver = \
	$(rel_mcdlibdir)	 \

projects = $(projects) $(prj_compiler_driver)

# -----------------------------------------------------------------------------

prj_texinfo = \
	$(rel_texinfodir)	\

projects = $(projects) $(prj_texinfo)

# -----------------------------------------------------------------------------

prj_tex4ht = \
	$(rel_tex4htdir)	\

projects = $(projects) $(prj_tex4ht)

# -----------------------------------------------------------------------------

prj_print_utility = \
	$(rel_dvispooldir)	\

projects = $(projects) $(prj_print_utility)

# -----------------------------------------------------------------------------

prj_graphics_utilities = \
	$(rel_epstopdfdir)	\
	$(rel_pnmtopngdir)	\
	$(rel_psutilsdir)	\

projects = $(projects) $(prj_graphics_utilities)

# -----------------------------------------------------------------------------

prj_font_utilities = \
	$(rel_cjkutilsdir)	\
	$(rel_freetypedir)	\
	$(rel_gsf2pkdir)	\
	$(rel_ps2pkdir)		\

projects = $(projects) $(prj_font_utilities)

# -----------------------------------------------------------------------------

prj_dviware = \
	$(rel_dvicopydir)	\
	$(rel_dvipdfmdir)	\
	$(rel_dvipdfmxdir)	\
	$(rel_dvipngdir)	\
	$(rel_dvipskdir)	\

projects = $(projects) $(prj_dviware)

# -----------------------------------------------------------------------------

prj_pdfware = \
	$(rel_arctrldir)	\

projects = $(projects) $(prj_pdfware)

# -----------------------------------------------------------------------------

prj_yap = \
	$(rel_yapdir)		\

projects = $(projects) $(prj_yap)

# -----------------------------------------------------------------------------

prj_md5walk = \
	$(rel_md5walkdir)	\

projects = $(projects) $(prj_md5walk)

# -----------------------------------------------------------------------------

prj_doc_utilities = \
	$(rel_texdocdir)	\

projects = $(projects) $(prj_doc_utilities)

# -----------------------------------------------------------------------------

prj_package_composer = \
	$(rel_mpcdir)		\

projects = $(projects) $(prj_package_composer)

# -----------------------------------------------------------------------------

prj_migrate = \
	$(rel_migrate)	\

projects = $(projects) $(prj_migrate)

# -----------------------------------------------------------------------------

prj_setup_wizard = \
	$(rel_setupwizdir)	\

projects = $(projects) $(prj_setup_wizard)

# -----------------------------------------------------------------------------

prj_update_wizard = \
	$(rel_updatewizdir)	\

projects = $(projects) $(prj_update_wizard)

# -----------------------------------------------------------------------------

prj_uninstaller = \
	$(rel_removewizdir)	 \

projects = $(projects) $(prj_uninstaller)

# -----------------------------------------------------------------------------

prj_documentation = \
	$(rel_docdir) \

projects = $(projects) $(prj_documentation)

# _____________________________________________________________________________
#
# Make Rules
# _____________________________________________________________________________

deftarget: all

all install mostlyclean clean maintainer-clean check depend: $@-local
	for %p in ($(projects)) do			\
	  (						\
	      $(pushd) "%p"				\
	    & $(MAKE) -f $(miktex_mak) $@		\
	    & $(popd)					\
	    & if errorlevel 1 exit /B 1			\
	  )

all-local:

install-local:

mostlyclean-local:

clean-local: mostlyclean-local

distclean-local: clean-local

maintainer-clean-local: distclean-local
	del setenv.cmd
	del $(rel_docdir)\version.ent
	del $(rel_libdir)\include\miktex\version.h
	del Doxyfile

cvsclean: maintainer-clean
	del msvc\config.inc

check-local:

depend-local:

make-setenv-cmd:
	type << > setenv.cmd
set INCLUDE=$(miktexsrcdir)\msvc;%INCLUDE%
set MIKTEX_BINDIR=$(prefix)\miktex\bin
set MIKTEX_STARTUPFILE=$(prefix)\miktex\config\miktexstartup.ini
set PATH=%MIKTEX_BINDIR%;%PATH%
set XML_CATALOG_FILES=$(dbcatalog)
set MIKTEX_SERIES=$(miktexseries)
set MIKTEX_J2000=$(MIKTEX_J2000_VERSION)
<<

miktexstartupini = $(prefix)\miktex\config\miktexstartup.ini

make-miktexstartup-ini:
	if not exist "$(prefix)\miktex\config"	\
	  (					\
	    $(mkpath) "$(prefix)\miktex\config"	\
	  )
	type << > $(miktexstartupini)
[paths]
Roots=$(tmdir)
Install=$(prefix)
UserData=$(prefix)
UserConfig=$(prefix)
<<

init-texmf:
	if not exist "$(prefix)\dvips\config" \
	  $(mkpath) "$(prefix)\dvips\config"
	copy /Y "$(tmdir)\dvips\config\config.ps" \
	  "$(prefix)\dvips\config"