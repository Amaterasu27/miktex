## omega_dyn.sed: make dynamic arrays
## 
## Copyright (C) 1998-2007 Christian Schenk
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
s/^\([_a-zA-Z0-9]*\) \(m_fonttables\)\(\[ [a-zA-Z0-9]* \]\)\[ [a-zA-Z0-9]* \];$/\1 **\2;/
s/^\([_a-zA-Z0-9]*\) \(m_ocptables\)\(\[ [a-zA-Z0-9]* \]\)\[ [a-zA-Z0-9]* \];$/\1 **\2;/
s/^\([_a-zA-Z0-9]*\) \(m_strstartar\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_ocplistinfo\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_ocplstackinfo\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_ocplistlist\)\[ [a-zA-Z0-9]* \];$/\1 *\2;/
s/^\([_a-zA-Z0-9]*\) \(m_fontsorttables\)\(\[ [a-zA-Z0-9]* \]\)\[ [a-zA-Z0-9]* \];$/\1 **\2;/
s/^\([_A-Za-z0-9]*\) \(m_trieophash\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_trieoplang\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_trieopval\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_hyfdistance\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_hyfnum\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_hyfnext\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_trie\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
