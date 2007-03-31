## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2007 Christian Schenk
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

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.ent.in
               ${CMAKE_CURRENT_BINARY_DIR}/version.ent)

if(BUILDING_MIKTEX)
  install(
    FILES local.txt
    DESTINATION ${CMAKE_INSTALL_PREFIX}/doc/latex/00LocalGuide
  )
endif(BUILDING_MIKTEX)

set(screenshots_png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/autoinstall.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/browselocaltexmf.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/localtexmf.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/mo-general.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/mo-languages.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/mo-mark-package.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/mo-packages.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/mo-roots.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/mo-roots2.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/mo-roots3.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/mo-search-package.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/mpm-context.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/mpm.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/updatewiz-download-type.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/updatewiz-packagelist.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/updatewiz-progress.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/updatewiz-remote-repository.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/updatewiz-source.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/yap-inversesearchoptions.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/yap-rotating.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/yap-ruler.png
  ${CMAKE_CURRENT_SOURCE_DIR}/Images/yap-texeditor.png
)

set(screenshots75_png)
foreach(f ${screenshots_png})
  get_filename_component(name "${f}" NAME)
  set(screenshots75_png ${screenshots75_png}
    ${CMAKE_CURRENT_BINARY_DIR}/png75/${name}
  )
endforeach(f)

if(BUILDING_MIKTEX)
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/png75")
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/www/faq")
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/www/manual")
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/www/relnotes")
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/www/roadmap")
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  foreach(f ${screenshots_png})
    get_filename_component(name "${f}" NAME)
    add_custom_command(
      OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/png75/${name}
      COMMAND
	${IMAGEMAGICK_CONVERT_EXECUTABLE}
		 -scale 75%
		 ${f}
		 ${CMAKE_CURRENT_BINARY_DIR}/png75/${name}
      DEPENDS
	${f}
      VERBATIM
    )
  endforeach(f)
endif(BUILDING_MIKTEX)

set(manual_xml_files
  ${CMAKE_CURRENT_SOURCE_DIR}/EnvVars/BIBINPUTS.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/EnvVars/BSTINPUTS.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/EnvVars/MFINPUTS.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/EnvVars/MIKTEX_REPOSITORY.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/EnvVars/MIKTEX_TRACE.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/EnvVars/MPINPUTS.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/EnvVars/TEXINPUTS.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/EnvVars/TFMFONTS.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Manual/configuring.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Manual/envvars.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Manual/features.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Manual/installation-instructions.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Manual/installing.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Manual/intro.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Manual/localguide.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Manual/printing.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Manual/tcxtables.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Manual/texfeatures.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Manual/texifying.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Manual/tracestreams.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/alias.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/auxdirectory.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/bistacksize.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/bufsize.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/cstyleerrors.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/disableinstaller.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/disablepipes.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/disablewriteeighteen.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/dontparsefirstline.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/enableinstaller.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/enablepipes.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/enablewriteeighteen.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/errorline.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/fontmax.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/fontmemsize.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/halferrorline.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/haltonerror.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/help.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/hhelp.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/includedirectory.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/initialize.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/interaction.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/jobname.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/jobtime.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/ligtablesize.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/maxinopen.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/maxprintline.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/maxstrings.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/maxwiggle.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/membot.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/memmax.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/memmin.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/memtop.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/mltex.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/movesize.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/nestsize.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/nocstyleerrors.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/outputdirectory.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/paramsize.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/parsefirstline.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/pathsize.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/poolsize.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/quiet.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/recorder.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/recordpackageusages.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/savesize.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/srcspecials.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/stacksize.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/stringvacancies.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/tcx.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/terminal.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/timestatistics.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/trace.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/trieopsize.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/triesize.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/undump.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Options/version.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/bibtex.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/dvicopy.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/dvips.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/findtexmf.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/gftodvi.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/initexmf.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/mf.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/mpm.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/mpost.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/mthelp.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/mtprint.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/omega.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/pdftex.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/pdftexcfg.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/setupwiz.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/tex.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/texify.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/Ref/updmapcfg.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/miktex.xml
)

set(manual_listings
  ${CMAKE_CURRENT_BINARY_DIR}/filetypes
  ${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/mf.defaults
  ${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/mfapp.defaults
  ${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/mpost.defaults
  ${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/omega.defaults
  ${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/pdftex.defaults
  ${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/texapp.defaults
  ${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/texmfapp.defaults
)

if(BUILDING_MIKTEX)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/filetypes
    COMMAND ${findtexmf_exe}
	--list-file-types
	>${CMAKE_CURRENT_BINARY_DIR}/filetypes
    VERBATIM
  )
else(BUILDING_MIKTEX)
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/filetypes "")
endif(BUILDING_MIKTEX)

set(xslt_path "${CMAKE_CURRENT_BINARY_DIR}")
set(xslt_path "${xslt_path} ${CMAKE_CURRENT_BINARY_DIR}/Styles")
set(xslt_path "${xslt_path} ${CMAKE_CURRENT_SOURCE_DIR}")
set(xslt_path "${xslt_path} ${CMAKE_CURRENT_SOURCE_DIR}/Styles")

if(BUILDING_MIKTEX)
  install(
    FILES	Styles/miktexman.css
    DESTINATION	${docdir}
  )
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  install(
    DIRECTORY	Images/ ${CMAKE_SOURCE_DIR}/${rel_docbook_xsl_dir}/images
    DESTINATION	${docdir}/Images
    PATTERN	".svn" EXCLUDE
    PATTERN	"callouts" EXCLUDE
  )
endif(BUILDING_MIKTEX)

###############################################################################
## manual

if(BUILDING_MIKTEX)
  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/miktex.chm
    COMMAND
	${PERL_EXE} ${rmtree_perl} ${CMAKE_CURRENT_BINARY_DIR}/scratch
    COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/${rel_docbook_xsl_dir}/images
		${CMAKE_CURRENT_BINARY_DIR}/scratch/images
    COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/Images
		${CMAKE_CURRENT_BINARY_DIR}/scratch/images
    COMMAND
	${CMAKE_COMMAND} -E copy
		${miktexhelp_css}
		${CMAKE_CURRENT_BINARY_DIR}/scratch
    COMMAND
	${PERL_EXE} ${runxsltproc_perl}
		--path \"${xslt_path}\"
		--stringparam base.dir scratch/
		--stringparam chunk.fast 1
		--stringparam manifest.in.base.dir 1
		htmlhelp.xsl
		miktex.xml
    COMMAND
	${CMAKE_COMMAND} -E copy
		${help_h}
		${CMAKE_CURRENT_BINARY_DIR}/scratch/context.h
    COMMAND
  	${CMAKE_COMMAND} -E
		chdir ${CMAKE_CURRENT_BINARY_DIR}/scratch
		${PERL_EXE} ${runhhc_perl} htmlhelp
    COMMAND
  	${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_BINARY_DIR}/scratch/htmlhelp.chm
		${CMAKE_CURRENT_BINARY_DIR}/miktex.chm
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/version.ent
	${entities_ent}
	${help_h}
	${htmlhelp_xsl}
	${manual_listings}
	${manual_xml_files}
	${miktexhelp_css}
	${screenshots_png}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
  )

  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/miktex.tex
    COMMAND
	${PERL_EXE} ${runxsltproc_perl}
		--path \"${xslt_path}\"
		--output ${CMAKE_CURRENT_BINARY_DIR}/miktex.tex
		db2latex-manual.xsl
		miktex.xml
    DEPENDS
	${manual_listings}
	${manual_xml_files}
	${entities_ent}
	${CMAKE_SOURCE_DIR}/${rel_xsl_dir}/db2latex-manual.xsl
	${CMAKE_BINARY_DIR}/${rel_xsl_dir}/db2latex-common.xsl
    VERBATIM
  )

  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/miktex.pdf
    COMMAND
	${PERL_EXE} ${rmtree_perl} ${CMAKE_CURRENT_BINARY_DIR}/scratch
    COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_BINARY_DIR}/png75
		${CMAKE_CURRENT_BINARY_DIR}/scratch/images
    COMMAND
  	${CMAKE_COMMAND} -E
		chdir ${CMAKE_CURRENT_BINARY_DIR}/scratch
		${TEXIFY_EXE} --pdf ${CMAKE_CURRENT_BINARY_DIR}/miktex.tex
    COMMAND
  	${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_BINARY_DIR}/scratch/miktex.pdf
		${CMAKE_CURRENT_BINARY_DIR}/miktex.pdf
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/miktex.tex
	${screenshots75_png}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
  )

  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/www/manual/index.html
    COMMAND
  	${PERL_EXE} ${rmtree_perl} ${CMAKE_CURRENT_BINARY_DIR}/www/manual
    COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/${rel_docbook_xsl_dir}/images
		${CMAKE_CURRENT_BINARY_DIR}/www/manual/images
    COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/Images
		${CMAKE_CURRENT_BINARY_DIR}/www/manual/images
    COMMAND
	${CMAKE_COMMAND} -E copy
		${miktexman_css}
		${CMAKE_CURRENT_BINARY_DIR}/www/manual
    COMMAND
	${PERL_EXE} ${runxsltproc_perl}
		--path \"${xslt_path}\"
		--stringparam base.dir www/manual/
		--stringparam chunk.fast 1
		htmlchunk.xsl
		miktex.xml
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/version.ent
	${entities_ent}
	${htmlchunk_xsl}
	${manual_listings}
	${manual_xml_files}
	${miktexhelp_css}
	${screenshots_png}
    WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
  ) 

  install(
    FILES
	${CMAKE_CURRENT_BINARY_DIR}/miktex.chm
	${CMAKE_CURRENT_BINARY_DIR}/miktex.pdf
    DESTINATION
	${docdir}
  )

  install(
    DIRECTORY	${CMAKE_CURRENT_BINARY_DIR}/www/manual/
    DESTINATION	${docdir}/manual
    PATTERN	".svn" EXCLUDE
  )
endif(BUILDING_MIKTEX)

###############################################################################
## installation instructions

if(BUILDING_MIKTEX)
  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/install.html
    COMMAND
	${PERL_EXE} ${runxsltproc_perl}
		--path \"${xslt_path}\"
		--output ${CMAKE_CURRENT_BINARY_DIR}/install.html
		html.xsl
		install.xml
    DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/Manual/installation-instructions.xml
	${CMAKE_CURRENT_SOURCE_DIR}/install.xml
	${entities_ent}
	${html_xsl}
    VERBATIM
  )

  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/install.tex
    COMMAND
	${PERL_EXE} ${runxsltproc_perl}
		--path \"${xslt_path}\"
		--output ${CMAKE_CURRENT_BINARY_DIR}/install.tex
		db2latex-install.xsl
		install.xml
    DEPENDS
	${CMAKE_BINARY_DIR}/${rel_xsl_dir}/db2latex-common.xsl
	${CMAKE_CURRENT_SOURCE_DIR}/Manual/installation-instructions.xml
	${CMAKE_CURRENT_SOURCE_DIR}/install.xml
	${CMAKE_SOURCE_DIR}/${rel_xsl_dir}/db2latex-install.xsl
	${entities_ent}
    VERBATIM
  )

  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/install.pdf
    COMMAND
	${PERL_EXE} ${rmtree_perl} ${CMAKE_CURRENT_BINARY_DIR}/scratch
    COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_BINARY_DIR}/png75
		${CMAKE_CURRENT_BINARY_DIR}/scratch/images
    COMMAND
  	${CMAKE_COMMAND} -E
		chdir ${CMAKE_CURRENT_BINARY_DIR}/scratch
		${TEXIFY_EXE} --pdf ${CMAKE_CURRENT_BINARY_DIR}/install.tex
    COMMAND
  	${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_BINARY_DIR}/scratch/install.pdf
		${CMAKE_CURRENT_BINARY_DIR}/install.pdf
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/install.tex
	${screenshots75_png}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
  )

  install(
    FILES
	${CMAKE_CURRENT_BINARY_DIR}/install.html
	${CMAKE_CURRENT_BINARY_DIR}/install.pdf
    DESTINATION
	${docdir}
  )
endif(BUILDING_MIKTEX)

###############################################################################
## preview

if(BUILDING_MIKTEX)
  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/preview.tex
    COMMAND
	${PERL_EXE} ${runxsltproc_perl}
		--path \"${xslt_path}\"
		--output ${CMAKE_CURRENT_BINARY_DIR}/preview.tex
		db2latex-preview.xsl
		preview.xml
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/preview.xml
        ${CMAKE_CURRENT_SOURCE_DIR}/changes.xml
	${entities_ent}
	${CMAKE_SOURCE_DIR}/${rel_xsl_dir}/db2latex-preview.xsl
	${CMAKE_BINARY_DIR}/${rel_xsl_dir}/db2latex-common.xsl
    VERBATIM
  )

  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/preview.pdf
    COMMAND
	${PERL_EXE} ${rmtree_perl} ${CMAKE_CURRENT_BINARY_DIR}/scratch
    COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_BINARY_DIR}/png75
		${CMAKE_CURRENT_BINARY_DIR}/scratch/images
    COMMAND
  	${CMAKE_COMMAND} -E
		chdir ${CMAKE_CURRENT_BINARY_DIR}/scratch
		${TEXIFY_EXE} --pdf ${CMAKE_CURRENT_BINARY_DIR}/preview.tex
    COMMAND
  	${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_BINARY_DIR}/scratch/preview.pdf
		${CMAKE_CURRENT_BINARY_DIR}/preview.pdf
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/preview.tex
	${screenshots75_png}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
  )

  install(
    FILES
	${CMAKE_CURRENT_BINARY_DIR}/preview.pdf
    DESTINATION
	${docdir}
  )
endif(BUILDING_MIKTEX)

###############################################################################
## release notes

if(BUILDING_MIKTEX)
  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/www/relnotes/index.html
    COMMAND
  	${PERL_EXE} ${rmtree_perl} ${CMAKE_CURRENT_BINARY_DIR}/www/relnotes
    COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/${rel_docbook_xsl_dir}/images
		${CMAKE_CURRENT_BINARY_DIR}/www/relnotes/images
    COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/Images
		${CMAKE_CURRENT_BINARY_DIR}/www/relnotes/images
    COMMAND
	${CMAKE_COMMAND} -E copy
		${miktexman_css}
		${CMAKE_CURRENT_BINARY_DIR}/www/relnotes
    COMMAND
	${PERL_EXE} ${runxsltproc_perl}
		--path \"${xslt_path}\"
		--stringparam base.dir www/relnotes/
		--output ${CMAKE_CURRENT_BINARY_DIR}/www/relnotes/index.html
		html.xsl
		relnotes.xml
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/version.ent
	${CMAKE_CURRENT_SOURCE_DIR}/relnotes.xml
	${entities_ent}
	${html_xsl}
	${miktexhelp_css}
	${screenshots_png}
    WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
  )

  install(
    DIRECTORY	${CMAKE_CURRENT_BINARY_DIR}/www/relnotes/
    DESTINATION	${docdir}/relnotes/
    PATTERN	".svn" EXCLUDE
  )
endif(BUILDING_MIKTEX)

###############################################################################
## license

if(BUILDING_MIKTEX)
  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/license.html
    COMMAND
	${PERL_EXE} ${runxsltproc_perl}
		--path \"${xslt_path}\"
		--output ${CMAKE_CURRENT_BINARY_DIR}/license.html
		html.xsl
		license.xml
    DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/license.xml
	${entities_ent}
	${html_xsl}
    VERBATIM
  )

  install(
    FILES
	${CMAKE_CURRENT_BINARY_DIR}/license.html
    DESTINATION ${docdir}
  )
endif(BUILDING_MIKTEX)

###############################################################################
## faq

if(BUILDING_MIKTEX)
  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/faq.chm
    COMMAND
	${PERL_EXE} ${rmtree_perl} ${CMAKE_CURRENT_BINARY_DIR}/scratch
    COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/${rel_docbook_xsl_dir}/images
		${CMAKE_CURRENT_BINARY_DIR}/scratch/images
    COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/Images
		${CMAKE_CURRENT_BINARY_DIR}/scratch/images
    COMMAND
	${CMAKE_COMMAND} -E copy
		${miktexhelp_css}
		${CMAKE_CURRENT_BINARY_DIR}/scratch
    COMMAND
	${PERL_EXE} ${runxsltproc_perl}
		--path \"${xslt_path}\"
		--stringparam base.dir scratch/
		--stringparam chunk.fast 1
		--stringparam manifest.in.base.dir 1
		htmlhelp.xsl
		faq.xml
    COMMAND
	${CMAKE_COMMAND} -E copy
		${help_h}
		${CMAKE_CURRENT_BINARY_DIR}/scratch/context.h
    COMMAND
  	${CMAKE_COMMAND} -E
		chdir ${CMAKE_CURRENT_BINARY_DIR}/scratch
		${PERL_EXE} ${runhhc_perl} htmlhelp
    COMMAND
  	${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_BINARY_DIR}/scratch/htmlhelp.chm
		${CMAKE_CURRENT_BINARY_DIR}/faq.chm
    DEPENDS
	${entities_ent}
	${htmlhelp_xsl}
	${manual_xml_files}
	${miktexhelp_css}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
  )

  install(
    FILES
	${CMAKE_CURRENT_BINARY_DIR}/faq.chm
    DESTINATION
	${docdir}
  )

  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/www/faq/index.html
    COMMAND
  	${PERL_EXE} ${rmtree_perl} ${CMAKE_CURRENT_BINARY_DIR}/www/faq
    COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/${rel_docbook_xsl_dir}/images
		${CMAKE_CURRENT_BINARY_DIR}/www/faq/images
    COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/Images
		${CMAKE_CURRENT_BINARY_DIR}/www/faq/images
    COMMAND
	${CMAKE_COMMAND} -E copy
		${miktexman_css}
		${CMAKE_CURRENT_BINARY_DIR}/www/faq
    COMMAND
	${PERL_EXE} ${runxsltproc_perl}
		--path \"${xslt_path}\"
		--stringparam base.dir www/faq/
		--stringparam chunk.fast 1
		htmlchunk.xsl
		faq.xml
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/version.ent
	${entities_ent}
	${htmlchunk_xsl}
	faq.xml
	${miktexhelp_css}
	${screenshots_png}
    WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
  ) 

  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/www/faq/faq.html
    COMMAND
	${PERL_EXE} ${runxsltproc_perl}
		--output ${CMAKE_CURRENT_BINARY_DIR}/www/faq/faq.html
		--path \"${xslt_path}\"
		--stringparam base.dir www/faq/
		html.xsl
		faq.xml
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/www/faq/index.html
	${CMAKE_CURRENT_BINARY_DIR}/version.ent
	${entities_ent}
	${htmlchunk_xsl}
	faq.xml
	${miktexhelp_css}
	${screenshots_png}
    WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
  ) 

  install(
    FILES	${CMAKE_CURRENT_BINARY_DIR}/www/faq/faq.html
    DESTINATION	${docdir}/faq
  )
endif(BUILDING_MIKTEX)

###############################################################################
## roadmap

if(BUILDING_MIKTEX)
  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/www/roadmap/index.html
    COMMAND
  	${PERL_EXE} ${rmtree_perl} ${CMAKE_CURRENT_BINARY_DIR}/www/roadmap
    COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/${rel_docbook_xsl_dir}/images
		${CMAKE_CURRENT_BINARY_DIR}/www/roadmap/images
    COMMAND
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/Images
		${CMAKE_CURRENT_BINARY_DIR}/www/roadmap/images
    COMMAND
	${CMAKE_COMMAND} -E copy
		${miktexman_css}
		${CMAKE_CURRENT_BINARY_DIR}/www/roadmap
    COMMAND
	${PERL_EXE} ${runxsltproc_perl}
		--path \"${xslt_path}\"
		--output ${CMAKE_CURRENT_BINARY_DIR}/www/roadmap/index.html
		html.xsl
		roadmap.xml
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/version.ent
	${CMAKE_CURRENT_SOURCE_DIR}/roadmap.xml
	${entities_ent}
	${html_xsl}
	${miktexman_css}
    VERBATIM
  ) 

  install(
    DIRECTORY	${CMAKE_CURRENT_BINARY_DIR}/www/roadmap/
    DESTINATION	${docdir}/roadmap
    PATTERN	".svn" EXCLUDE
  )
endif(BUILDING_MIKTEX)

###############################################################################
## man pages

if(NOT NATIVE_WINDOWS)
  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/initexmf.1
	${CMAKE_CURRENT_BINARY_DIR}/mpm.1
	${CMAKE_CURRENT_BINARY_DIR}/mthelp.1
    COMMAND
	${PERL_EXE} ${runxsltproc_perl}
		--path \"${xslt_path}\"
		manpages.xsl
		miktex.xml
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/filetypes
	${CMAKE_CURRENT_SOURCE_DIR}/Ref/initexmf.xml
	${CMAKE_CURRENT_SOURCE_DIR}/Ref/mpm.xml
	${CMAKE_CURRENT_SOURCE_DIR}/Ref/mthelp.xml
	${CMAKE_CURRENT_SOURCE_DIR}/Styles/manpages.xsl
	${CMAKE_CURRENT_SOURCE_DIR}/miktex.xml
	${entities_ent}
    WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
  )
  install(
    FILES
	${CMAKE_CURRENT_BINARY_DIR}/initexmf.1
	${CMAKE_CURRENT_BINARY_DIR}/mpm.1
	${CMAKE_CURRENT_BINARY_DIR}/mthelp.1
    DESTINATION
	${mandir}
  )
endif(NOT NATIVE_WINDOWS)

###############################################################################
## convenience targets

if(BUILDING_MIKTEX)
  add_custom_target(all-doc ALL
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/faq.chm
	${CMAKE_CURRENT_BINARY_DIR}/install.html
	${CMAKE_CURRENT_BINARY_DIR}/install.pdf
	${CMAKE_CURRENT_BINARY_DIR}/license.html
	${CMAKE_CURRENT_BINARY_DIR}/miktex.chm
	${CMAKE_CURRENT_BINARY_DIR}/miktex.pdf
	${CMAKE_CURRENT_BINARY_DIR}/preview.pdf
	${CMAKE_CURRENT_BINARY_DIR}/www/faq/index.html
	${CMAKE_CURRENT_BINARY_DIR}/www/faq/faq.html
	${CMAKE_CURRENT_BINARY_DIR}/www/manual/index.html
	${CMAKE_CURRENT_BINARY_DIR}/www/relnotes/index.html
	${CMAKE_CURRENT_BINARY_DIR}/www/roadmap/index.html
  )
endif(BUILDING_MIKTEX)

if(NOT NATIVE_WINDOWS)
  add_custom_target(all-man ALL
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/initexmf.1
	${CMAKE_CURRENT_BINARY_DIR}/mpm.1
	${CMAKE_CURRENT_BINARY_DIR}/mthelp.1
  )
endif(NOT NATIVE_WINDOWS)
