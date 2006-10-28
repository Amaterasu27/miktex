## miktex-version.cmake: define the MiKTeX version number
##
## Copyright (C) 2006 Christian Schenk
## 
## This file is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
## 
## This file is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this file; if not, write to the Free Software Foundation,
## 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

## major version number
set(MIKTEX_MAJOR_VERSION	2)

## minor version number
set(MIKTEX_MINOR_VERSION	6)

## days since January 1, 2000
set(MIKTEX_J2000_VERSION	2492)

## the release number; only meaningful for rc/beta builds
set(MIKTEX_RELEASE_NUM		1)

## the release state; one of:
##   0 (general availability release)
##   1 (release candidate)
##   2 (beta release)
##   4 (snapshot release)
set(MIKTEX_RELEASE_STATE 4)

if(MIKTEX_RELEASE_STATE EQUAL 0)
elseif(MIKTEX_RELEASE_STATE EQUAL 1)
elseif(MIKTEX_RELEASE_STATE EQUAL 2)
elseif(MIKTEX_RELEASE_STATE EQUAL 4)
else(MIKTEX_RELEASE_STATE EQUAL 4)
  message("${MIKTEX_RELEASE_STATE} is an unknown release state.")
  message("Falling back to release state 4 (snapshot).")
  set(MIKTEX_RELEASE_STATE
      4
      CACHE STRING
      "MiKTeX release state; one of 0 (general availability release), 1 (release candidate), 2 (beta release) and 4 (snapshot release)."
      FORCE)
endif(MIKTEX_RELEASE_STATE EQUAL 0)