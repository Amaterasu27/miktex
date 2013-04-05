%% miktex-adapter-0.ch:
%% 
%% Copyright (C) 2013 Christian Schenk
%% 
%% This file is free software; you can redistribute it and/or modify it
%% under the terms of the GNU General Public License as published by the
%% Free Software Foundation; either version 2, or (at your option) any
%% later version.
%% 
%% This file is distributed in the hope that it will be useful, but
%% WITHOUT ANY WARRANTY; without even the implied warranty of
%% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
%% General Public License for more details.
%% 
%% You should have received a copy of the GNU General Public License
%% along with This file; if not, write to the Free Software Foundation,
%% 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

% _____________________________________________________________________________
%
% [2.24]
% _____________________________________________________________________________

@x
for i:=0 to @'176 do xord[xchr[i]]:=i;
@y
for i:=first_text_char to last_text_char do xord[chr(i)]:=invalid_code;
for i:=@'200 to @'377 do xord[xchr[i]]:=i;
for i:=0 to @'176 do xord[xchr[i]]:=i;
@z

% _____________________________________________________________________________
%
% [8.110]
% _____________________________________________________________________________

@x
@d max_quarterword=@"FFFF {largest allowable value in a |quarterword|}
@d min_halfword==-@"FFFFFFF {smallest allowable value in a |halfword|}
@d max_halfword==@"3FFFFFFF {largest allowable value in a |halfword|}
@y
@d max_quarterword=255 {largest allowable value in a |quarterword|}
@d min_halfword==0 {smallest allowable value in a |halfword|}
@d max_halfword==65535 {largest allowable value in a |halfword|}
@z

% _____________________________________________________________________________
%
% [15.217]
% _____________________________________________________________________________

@x
@d shorthand_def=97 {code definition ( \.{\\chardef}, \.{\\countdef}, etc.~)}
@y
@d shorthand_def=95 {code definition ( \.{\\chardef}, \.{\\countdef}, etc.~)}
@z

% _____________________________________________________________________________
%
% [17.238]
% _____________________________________________________________________________

@x
@d int_base=math_code_base+number_usvs {beginning of region 5}
@y
@d int_base=math_code_base+256 {beginning of region 5}
@z

% _____________________________________________________________________________
%
% [36.773]
% _____________________________________________________________________________

@x
      begin c:=rem_byte(cur_i); i:=char_info(cur_f)(c);
@y
    begin c:=rem_byte(cur_i); i:=char_info(cur_f)(c);
@z
