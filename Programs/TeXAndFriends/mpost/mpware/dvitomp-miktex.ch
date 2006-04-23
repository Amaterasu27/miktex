%% dvitomp-miktex.ch: WEB change file for DVItoMP
%% 
%% Copyright (C) 1998-2005 Christian Schenk
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
% [1.5]
% _____________________________________________________________________________

@x
@!virtual_space=10000;
@y
@!virtual_space=100000;
@z

@x
@!name_length=50; {a file name shouldn't be longer than this}
@y
@!name_length=260; {a file name shouldn't be longer than this}
@!namez_length=261;
@z

% _____________________________________________________________________________
%
% [1.10]
% _____________________________________________________________________________

@x
begin goto final_end;
@y
begin c4p_throw(final_end)
@z

% _____________________________________________________________________________
%
% [2.12]
% _____________________________________________________________________________

@x
@d text_char == char {the data type of characters in text files}
@d first_text_char=0 {ordinal number of the smallest element of |text_char|}
@d last_text_char=127 {ordinal number of the largest element of |text_char|}
@y
@d text_char == char {the data type of characters in text files}
@d first_text_char=0 {ordinal number of the smallest element of |text_char|}
@d last_text_char=127 {ordinal number of the largest element of |text_char|}
@z

% _____________________________________________________________________________
%
% [2.14]
% _____________________________________________________________________________

@x
begin rewrite(mpx_file);
@y
begin if (c4pargc <> 3) then
 abort('Usage: dvitomp inputfile outputfile');
c4p_fopen(mpx_file,c4p_argv[2],c4p_w_mode,true);
rewrite(mpx_file);
@z

% _____________________________________________________________________________
%
% [4.19]
% _____________________________________________________________________________

@x
begin reset(dvi_file);
if eof(dvi_file) then abort('DVI file not found');
@y
begin if (c4pargc <> 3) then
 abort('Usage: dvitomp inputfile outputfile');
c4p_fopen(dvi_file,c4p_argv[1],c4p_rb_mode,true);
reset(dvi_file);
@z

@x
begin reset(tfm_file,cur_name);
open_tfm_file:=(not eof(tfm_file));
@y
begin open_tfm_file:=miktex_open_tfm_file(tfm_file,cur_name);
@z

@x
begin reset(vf_file,cur_name);
open_vf_file:=(not eof(vf_file));
@y
begin open_vf_file:=miktex_open_vf_file(vf_file,cur_name);
@z

% _____________________________________________________________________________
%
% [4.20]
% _____________________________________________________________________________

@x
@!cur_name:packed array[1..name_length] of char; {external name,
@y
@!cur_name:packed array[1..namez_length] of char; {external name,
@z

% _____________________________________________________________________________
%
% [8.61]
% _____________________________________________________________________________

@x
@d default_directory_name=='TeXfonts:' {change this to the correct name}
@d default_directory_name_length=9 {change this to the correct length}
@y
@d default_directory_name==''
@d default_directory_name_length=0
@z

@x
@<Glob...@>=
@!default_directory:packed array[1..default_directory_name_length] of char;
@y
@z

% _____________________________________________________________________________
%
% [8.62]
% _____________________________________________________________________________

@x
default_directory:=default_directory_name;
@y
do_nothing;
@z

% _____________________________________________________________________________
%
% [8.63]
% _____________________________________________________________________________

@x
for k:=1 to name_length do cur_name[k]:=' ';
@y
for k:=1 to name_length do cur_name[k]:=chr(0);
@z

@x
if area_length[f]=0 then
  begin for k:=1 to default_directory_name_length do
    cur_name[k]:=default_directory[k];
  l:=default_directory_name_length;
  end
else l:=0;
@y
l:=0;
@z

@x
  if l+3>name_length then
@y
  if l+4>name_length then
@z

@x
  if (names[k]>="a")and(names[k]<="z") then
      cur_name[l]:=xchr[names[k]-@'40]
  else cur_name[l]:=xchr[names[k]];
@y
  cur_name[l]:=xchr[names[k]];
@z

@x
cur_name[l+1]:='.'; cur_name[l+2]:='V'; cur_name[l+3]:='F'
@y
cur_name[l+1]:='.'; cur_name[l+2]:='v'; cur_name[l+3]:='f';
cur_name[l+4]:=chr(0)
@z

% _____________________________________________________________________________
%
% [8.64]
% _____________________________________________________________________________

@x
cur_name[l+2]:='T'; cur_name[l+3]:='F'; cur_name[l+4]:='M'
@y
cur_name[l+2]:='t'; cur_name[l+3]:='f'; cur_name[l+4]:='m';
cur_name[l+5]:=chr(0)
@z

% _____________________________________________________________________________
%
% [11.98] The main program
% _____________________________________________________________________________

@x
@p begin initialize; {get all variables initialized}
@y
@p begin
c4p_begin_try_block(final_end);
initialize; {get all variables initialized}
@z

@x
print_ln(banner);
@y
print(banner);
miktex_print_miktex_banner(mpx_file);
write_ln(mpx_file);
@z

@x
final_end:end.
@y
final_end:
c4p_end_try_block(final_end);
end.
@z
