## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2008 Christian Schenk
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

if(NATIVE_WINDOWS)
  add_definitions(
    -DWINNT
  )
endif(NATIVE_WINDOWS)

set(exes
  epsffit
  psbook
  psnup
  psresize
  psselect
  pstops
)

add_executable(epsffit
  epsffit.c
  pserror.c
)

merge_trustinfo_manifest(epsffit asInvoker)

add_executable(psbook
  psbook.c
  pserror.c
  psutil.c
)

merge_trustinfo_manifest(psbook asInvoker)

add_executable(psnup
  pserror.c
  psnup.c
  psspec.c
  psutil.c
)

merge_trustinfo_manifest(psnup asInvoker)

add_executable(psresize
  pserror.c
  psresize.c
  psspec.c
  psutil.c
)

merge_trustinfo_manifest(psresize asInvoker)

add_executable(psselect
  pserror.c
  psselect.c
  psutil.c
)

merge_trustinfo_manifest(psselect asInvoker)

add_executable(pstops
  pserror.c
  psspec.c
  pstops.c
  psutil.c
)

merge_trustinfo_manifest(pstops asInvoker)

install(TARGETS ${exes} DESTINATION ${bindir})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cooked)

set(scripts
  extractres.pl
  fixdlsrps.pl
  fixfmps.pl
  fixmacps.pl
  fixpsditps.pl
  fixpspps.pl
  fixscribeps.pl
  fixtpps.pl
  fixwfwps.pl
  fixwpps.pl
  fixwwps.pl
  includeres.pl
  psmerge.pl
)

set(cooked_scripts
  ${CMAKE_CURRENT_BINARY_DIR}/cooked/extractres.pl
  ${CMAKE_CURRENT_BINARY_DIR}/cooked/fixdlsrps.pl
  ${CMAKE_CURRENT_BINARY_DIR}/cooked/fixfmps.pl
  ${CMAKE_CURRENT_BINARY_DIR}/cooked/fixmacps.pl
  ${CMAKE_CURRENT_BINARY_DIR}/cooked/fixpsditps.pl
  ${CMAKE_CURRENT_BINARY_DIR}/cooked/fixpspps.pl
  ${CMAKE_CURRENT_BINARY_DIR}/cooked/fixscribeps.pl
  ${CMAKE_CURRENT_BINARY_DIR}/cooked/fixtpps.pl
  ${CMAKE_CURRENT_BINARY_DIR}/cooked/fixwfwps.pl
  ${CMAKE_CURRENT_BINARY_DIR}/cooked/fixwpps.pl
  ${CMAKE_CURRENT_BINARY_DIR}/cooked/fixwwps.pl
  ${CMAKE_CURRENT_BINARY_DIR}/cooked/includeres.pl
  ${CMAKE_CURRENT_BINARY_DIR}/cooked/psmerge.pl
)

add_custom_target(scripts ALL
  DEPENDS ${cooked_scripts}
)

foreach(s ${scripts})
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/cooked/${s}
    COMMAND
	${PERL_EXE} ${CMAKE_CURRENT_SOURCE_DIR}/maketext
		INCLUDE=/texmf/psutils/base
		OS=XYZ
		PERL=perl
		${CMAKE_CURRENT_SOURCE_DIR}/${s}
		> ${CMAKE_CURRENT_BINARY_DIR}/cooked/${s}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${s}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM
  )
endforeach(s)

install(
  FILES ${cooked_scripts}
  DESTINATION ${texmfdir}/scripts/psutils/perl
)
