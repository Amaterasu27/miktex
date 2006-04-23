%%%  common-miktec.ch: CWEB change file
%%%  
%%%  Copyright (C) 2000-2001 Christian Schenk <cs@miktex.org>
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

%% **    70  ******************************************************************

@x
        else if (*s=='/') dot_pos=NULL,name_pos=++s;
@y
        else if (*s=='/' || *s=='\\' || *s == ':') dot_pos=NULL,name_pos=++s;
@z

@x
  if (found_change<=0) strcpy(change_file_name,"/dev/null");
@y
  if (found_change<=0) strcpy(change_file_name,"nul");
@z
