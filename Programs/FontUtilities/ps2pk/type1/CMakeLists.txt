## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006 Christian Schenk
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

add_definitions(
  -DHAVE_PROTOTYPES
  -DDKPATHSEA
)

set(ps2pktype1_sources
  Xstuff.h
  arith.c
  arith.h
  blues.h
  cluts.h
  curves.c
  curves.h
  digit.h
  ffilest.h
  font.h
  fontfcn.c
  fontfcn.h
  fontfile.h
  fontmisc.h
  fonts.h
  fontstruct.h
  fontxlfd.h
  fsmasks.h
  hdigit.h
  hints.c
  hints.h
  lines.c
  lines.h
  objects.c
  objects.h
  paths.c
  paths.h
  pictures.h
  regions.c
  regions.h
  scanfont.c
  spaces.c
  spaces.h
  strokes.h
  t1funcs.c
  t1hdigit.h
  t1imager.h
  t1info.c
  t1intf.h
  t1io.c
  t1snap.c
  t1stdio.h
  t1stub.c
  token.c
  token.h
  tokst.h
  trig.h
  type1.c
  types.h
  util.c
  util.h
)

add_library(ps2pktype1 STATIC ${ps2pktype1_sources})
