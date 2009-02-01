## dyn.sed: make dynamic arrays
## 
## Copyright (C) 1991-2009 Christian Schenk
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
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]bisectstack\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]buffer\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]c4p_free\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]delta\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]deltax\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]deltay\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]fontinfo\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]inputstack\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]ligkern\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]mem\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]nextstr\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]paramstack\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]psi\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]strpool\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]strref\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]strstart\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]theta\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]trickbuf\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]uu\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]vv\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]wasfree\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
s/^C4PEXTERN \([_A-Za-z0-9]*\) \([gm_]ww\)\[ [A-Za-z0-9]* \];$/C4PEXTERN \1 *\2;/
