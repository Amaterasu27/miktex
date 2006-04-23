%% etex-miktex.ch:
%% 
%% Copyright (C) 1998-2005 Christian
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
% [18.264]
% _____________________________________________________________________________

@x
@!j:0..buf_size; {index into |buffer|}
@y
@!j:0..buf_size_def; {index into |buffer|}
@z

% _____________________________________________________________________________
%
% [54.1394]
% _____________________________________________________________________________

@x
@!eof_seen : array[1..max_in_open] of boolean; {has eof been seen?}
@y
@!eof_seen : array[1..max_in_open_def] of boolean; {has eof been seen?}
@z

% _____________________________________________________________________________
%
% [54.1412]
% _____________________________________________________________________________

@x
@!save_pointer=0..save_size; {index into |save_stack|}
@y
@!save_pointer=0..save_size_def; {index into |save_stack|}
@z

% _____________________________________________________________________________
%
% [54.1413]
% _____________________________________________________________________________

@x
var p:0..nest_size; {index into |nest|}
@y
var p:0..nest_size_def; {index into |nest|}
@z

% _____________________________________________________________________________
%
% [54.1506]
% _____________________________________________________________________________

@x
@!grp_stack : array[0..max_in_open] of save_pointer; {initial |cur_boundary|}
@!if_stack : array[0..max_in_open] of pointer; {initial |cond_ptr|}
@y
@!grp_stack : array[0..max_in_open_def] of save_pointer; {initial |cur_boundary|}
@!if_stack : array[0..max_in_open_def] of pointer; {initial |cond_ptr|}
@z

% _____________________________________________________________________________
%
% [54.1507]
% _____________________________________________________________________________

@x
var i:0..max_in_open; {index into |grp_stack|}
@y
var i:0..max_in_open_def; {index into |grp_stack|}
@z

% _____________________________________________________________________________
%
% [54.1509]
% _____________________________________________________________________________

@x
var i:0..max_in_open; {index into |if_stack|}
@y
var i:0..max_in_open_def; {index into |if_stack|}
@z

% _____________________________________________________________________________
%
% [54.1564]
% _____________________________________________________________________________

@x
@!init destroy_marks: for i:=top_mark_code to split_bot_mark_code do
@y
destroy_marks: for i:=top_mark_code to split_bot_mark_code do
@z

@x
tini
@y
@z
