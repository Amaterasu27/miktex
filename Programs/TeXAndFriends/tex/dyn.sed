## dyn.sed: make dynamic arrays
## 
## Copyright (C) 1991-2007 Christian Schenk
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
s/^\([_A-Za-z0-9]*\) \(m_mem\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_buffer\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_trickbuf\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_inputstack\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_inputfile\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_inputfilemode\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_inputfiletranslation\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_linestack\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_fontinfo\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_fontcheck\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_fontsize\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_fontdsize\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_fontparams\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_fontname\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_fontarea\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_fontbc\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_fontec\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_fontglue\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_fontused\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_hyphenchar\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_skewchar\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_bcharlabel\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_fontbchar\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_fontfalsebchar\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_charbase\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_widthbase\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_heightbase\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_depthbase\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_italicbase\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_ligkernbase\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_kernbase\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_extenbase\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_parambase\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_paramstack\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_nest\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_strstart\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_strpool\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_savestack\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_trietaken\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_triec\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_trieo\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_triel\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_trier\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_triehash\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_sourcefilenamestack\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_fullsourcefilenamestack\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
