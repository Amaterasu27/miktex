%%%  common-miktec.ch: CWEB change file
%%%  
%%%  Copyright (C) 2000-2006 Christian Schenk
%%%  
%%%  This file is free software; you can redistribute it and/or modify it
%%%  under the terms of the GNU General Public License as published by the
%%%  Free Software Foundation; either version 2, or (at your option) any
%%%  later version.
%%%  
%%%  This file is distributed in the hope that it will be useful, but
%%%  WITHOUT ANY WARRANTY; without even the implied warranty of
%%%  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
%%%  General Public License for more details.
%%%  
%%%  You should have received a copy of the GNU General Public License
%%%  along with This file; if not, write to the Free Software Foundation,
%%%  59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

% _____________________________________________________________________________
%
% [3.9]
% _____________________________________________________________________________

@x
    if ((*(k++) = c) != ' ') limit = k;
@y
    if ((*(k++) = c) != ' ' && c != '\r') limit = k;
@z

% _____________________________________________________________________________
%
% [3.10]
% _____________________________________________________________________________

@x
@d max_file_name_length 60
@y
@d max_file_name_length 260
@z

% _____________________________________________________________________________
%
% [6.70]
% _____________________________________________________________________________

@x
        else if (*s=='/') dot_pos=NULL,name_pos=++s;
@y
#if defined(MIKTEX_WINDOWS)
        else if (*s=='/' || *s=='\\' || *s == ':') dot_pos=NULL,name_pos=++s;
#else
        else if (*s=='/') dot_pos=NULL,name_pos=++s;
#endif
@z

@x
  if (found_change<=0) strcpy(change_file_name,"/dev/null");
@y
#if defined(MIKTEX_WINDOWS)
  if (found_change<=0) strcpy(change_file_name,"nul");
#else
  if (found_change<=0) strcpy(change_file_name,"/dev/null");
#endif
@z
