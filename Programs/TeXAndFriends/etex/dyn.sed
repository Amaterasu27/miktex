## dyn.sed:
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
s/^\([_A-Za-z0-9]*\) \(m_eofseen\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_grpstack\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
s/^\([_A-Za-z0-9]*\) \(m_ifstack\)\[ [A-Za-z0-9]* \];$/\1 *\2;/
