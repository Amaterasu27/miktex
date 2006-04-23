## dyn.sed: make dynamic arrays
## 
## Copyright (C) 1998-2005 Christian Schenk
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
s/^\([_a-zA-Z0-9]*\) \(m_pdffonttype\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdffontattr\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdffontblink\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdffontelink\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdffontstretch\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdffontshrink\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdffontstep\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdfcharused\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdfcharmap\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdffontsize\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdffontnum\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdffontmap\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdffontexpandratio\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdffontautoexpand\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_vfpacketbase\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_vfdefaultfont\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_vflocalfontnum\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_vfefnts\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_vfifnts\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdfmem\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_objtab\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdffontlpbase\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdffontrpbase\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_pdffontefbase\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_destnames\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
