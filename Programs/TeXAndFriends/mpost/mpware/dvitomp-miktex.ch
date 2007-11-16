%% dvitomp-miktex.ch: WEB change file for DVItoMP
%% 
%% Copyright (C) 1998-2007 Christian Schenk
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
% [1.3]
% _____________________________________________________________________________

@x
procedure initialize; {this procedure gets things started properly}
@y
function stringcast : c4p_string; forward;
@<Define |parse_arguments|@>
procedure initialize; {this procedure gets things started properly}
@z

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
@!ASCII_code=" ".."~"; {a subrange of the integers}
@y
@!ASCII_code=0..255; {a subrange of the integers}
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
@d last_text_char=255 {ordinal number of the largest element of |text_char|}
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
% [5.32]
% _____________________________________________________________________________

@x
@!local_only:array [0..max_fonts] of boolean; {|font_num| meaningless?}
@y
@!local_only:array [0..max_fnums] of boolean; {|font_num| meaningless?}
@z

% _____________________________________________________________________________
%
% [5.43]
% _____________________________________________________________________________

@x
  font_warn('Checksum mismatch for ')(ff)
@.Checksum mismatch@>
@y
  begin err_print('DVItoMP warning: Checksum mismatch for ');
@.Checksum mismatch@>
  err_print_font(ff);
  if history=spotless then history:=cksum_trouble;
  end
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
% [9.75]
% _____________________________________________________________________________

@x
print_ln('vardef _s(expr _t,_f,_m,_x,_y)=');
print_ln('  addto _p also _t infont _f scaled _m shifted (_x,_y); enddef;');
@y
print_ln('vardef _s(expr _t,_f,_m,_x,_y)(text _c)=');
print_ln('  addto _p also _t infont _f scaled _m shifted (_x,_y) _c; enddef;');
@z

% _____________________________________________________________________________
%
% [9.78]
% _____________________________________________________________________________

@x
  print_ln(',_n',str_f:1,',',m:1:5,',',x:1:4,',',y:1:4,');');
@y
  print(',_n',str_f:1,',',m:1:5,',',x:1:4,',',y:1:4,',');@/
  @<Print a \.{withcolor} specifier if appropriate@>@/
  print_ln(');');
@z

% _____________________________________________________________________________
%
% [9.79]
% _____________________________________________________________________________

@x
    print_ln('vardef _r(expr _a,_w) =');
    print_ln('  addto _p doublepath _a withpen pencircle scaled _w enddef;');
@y
    print_ln('vardef _r(expr _a,_w)(text _t) =');
    print_ln('  addto _p doublepath _a withpen pencircle scaled _w _t enddef;');
@z

@x
  print_ln('_r((',xx1:1:4,',',yy1:1:4,')..(',xx2:1:4,',',yy2:1:4,
      '), ',ww:1:4,');');
@y
  print('_r((',xx1:1:4,',',yy1:1:4,')..(',xx2:1:4,',',yy2:1:4,
      '), ',ww:1:4,',');@/
  @<Print a \.{withcolor} specifier if appropriate@>@/
  print_ln(');');
@z

% _____________________________________________________________________________
%
% [10.88]
% _____________________________________________________________________________

@x
@p procedure do_push;
@y
@p @<Declare procedures to handle color commands@>
procedure do_push;
@z

% _____________________________________________________________________________
%
% [10.94]
% _____________________________________________________________________________

@x
four_cases(xxx1): for k:=1 to p do
    down_the_drain:=get_byte;
@y
four_cases(xxx1): do_xxx(p);
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
if (history > cksum_trouble) then
begin
  c4p_exit (history);
end;

end.
@z

% _____________________________________________________________________________
%
% [12.103] System-dependent changes
% _____________________________________________________________________________

@x
This section should be replaced, if necessary, by changes to the program
that are necessary to make \.{DVItoMP} work at a particular installation.
It is usually best to design your change file so that all changes to
previous sections preserve the section numbering; then everybody's version
will be consistent with the printed program. More extensive changes,
which introduce new sections, can be inserted here; then only the index
itself will get a new section number.
@^system dependencies@>
@y
@* Color support.
These changes support \.{dvips}-style ``\.{color push NAME}'' and
``\.{color pop}'' specials. We store a list of named colors, sorted by
name, and decorate the relevant drawing commands with ``\.{withcolor
(r,g,b)}'' specifiers while a color is defined.

@ A constant bounding the size of the named-color array.

@<Constants...@> =
@!max_named_colors=100; {maximum number of distinct named colors}

@ Then we declare a record for color types.

@<Types...@> =
c_string = ^char;
@!named_color_record=record@;@/
  @!name:c_string; {color name}
  @!value:c_string; {text to pass to MetaPost}
  end;

@ Declare the named-color array itself.

@<Globals...@> =
@!named_colors: array[1..max_named_colors] of named_color_record;
  {stores information about named colors, in sorted order by name}
@!num_named_colors:integer; {number of elements of |named_colors| that are valid}

@ This function, used only during initialization, defines a named color.

@<Define |parse_arguments|@> =
procedure def_named_color(n,v: c_string);
  begin
    if num_named_colors = max_named_colors then
      abort('too many named color definitions')
    else if (num_named_colors > 0)
            and (strcmp(n, named_colors[num_named_colors].name) <= 0)
    then
      abort('named colors added out of alphabetical order');
    incr(num_named_colors);
    named_colors[num_named_colors].name := n;
    named_colors[num_named_colors].value := v
  end;

@ During the initialization phase, we define values for all the named
colors defined in \.{colordvi.tex}. CMYK-to-RGB conversion by GhostScript.

@<Set initial values@> =
num_named_colors := 0;
def_named_color('Apricot', '(1.0, 0.680006, 0.480006)');
def_named_color('Aquamarine', '(0.180006, 1.0, 0.7)');
def_named_color('Bittersweet', '(0.760012, 0.0100122, 0.0)');
def_named_color('Black', '(0.0, 0.0, 0.0)');
def_named_color('Blue', '(0.0, 0.0, 1.0)');
def_named_color('BlueGreen', '(0.15, 1.0, 0.669994)');
def_named_color('BlueViolet', '(0.1, 0.05, 0.960012)');
def_named_color('BrickRed', '(0.719994, 0.0, 0.0)');
def_named_color('Brown', '(0.4, 0.0, 0.0)');
def_named_color('BurntOrange', '(1.0, 0.489988, 0.0)');
def_named_color('CadetBlue', '(0.380006, 0.430006, 0.769994)');
def_named_color('CarnationPink', '(1.0, 0.369994, 1.0)');
def_named_color('Cerulean', '(0.0600122, 0.889988, 1.0)');
def_named_color('CornflowerBlue', '(0.35, 0.869994, 1.0)');
def_named_color('Cyan', '(0.0, 1.0, 1.0)');
def_named_color('Dandelion', '(1.0, 0.710012, 0.160012)');
def_named_color('DarkOrchid', '(0.6, 0.2, 0.8)');
def_named_color('Emerald', '(0.0, 1.0, 0.5)');
def_named_color('ForestGreen', '(0.0, 0.880006, 0.0)');
def_named_color('Fuchsia', '(0.45, 0.00998169, 0.919994)');
def_named_color('Goldenrod', '(1.0, 0.9, 0.160012)');
def_named_color('Gray', '(0.5, 0.5, 0.5)');
def_named_color('Green', '(0.0, 1.0, 0.0)');
def_named_color('GreenYellow', '(0.85, 1.0, 0.310012)');
def_named_color('JungleGreen', '(0.0100122, 1.0, 0.480006)');
def_named_color('Lavender', '(1.0, 0.519994, 1.0)');
def_named_color('LimeGreen', '(0.5, 1.0, 0.0)');
def_named_color('Magenta', '(1.0, 0.0, 1.0)');
def_named_color('Mahogany', '(0.65, 0.0, 0.0)');
def_named_color('Maroon', '(0.680006, 0.0, 0.0)');
def_named_color('Melon', '(1.0, 0.539988, 0.5)');
def_named_color('MidnightBlue', '(0.0, 0.439988, 0.569994)');
def_named_color('Mulberry', '(0.640018, 0.0800061, 0.980006)');
def_named_color('NavyBlue', '(0.0600122, 0.460012, 1.0)');
def_named_color('OliveGreen', '(0.0, 0.6, 0.0)');
def_named_color('Orange', '(1.0, 0.389988, 0.130006)');
def_named_color('OrangeRed', '(1.0, 0.0, 0.5)');
def_named_color('Orchid', '(0.680006, 0.360012, 1.0)');
def_named_color('Peach', '(1.0, 0.5, 0.3)');
def_named_color('Periwinkle', '(0.430006, 0.45, 1.0)');
def_named_color('PineGreen', '(0.0, 0.75, 0.160012)');
def_named_color('Plum', '(0.5, 0.0, 1.0)');
def_named_color('ProcessBlue', '(0.0399878, 1.0, 1.0)');
def_named_color('Purple', '(0.55, 0.139988, 1.0)');
def_named_color('RawSienna', '(0.55, 0.0, 0.0)');
def_named_color('Red', '(1.0, 0.0, 0.0)');
def_named_color('RedOrange', '(1.0, 0.230006, 0.130006)');
def_named_color('RedViolet', '(0.590018, 0.0, 0.660012)');
def_named_color('Rhodamine', '(1.0, 0.180006, 1.0)');
def_named_color('RoyalBlue', '(0.0, 0.5, 1.0)');
def_named_color('RoyalPurple', '(0.25, 0.1, 1.0)');
def_named_color('RubineRed', '(1.0, 0.0, 0.869994)');
def_named_color('Salmon', '(1.0, 0.469994, 0.619994)');
def_named_color('SeaGreen', '(0.310012, 1.0, 0.5)');
def_named_color('Sepia', '(0.3, 0.0, 0.0)');
def_named_color('SkyBlue', '(0.380006, 1.0, 0.880006)');
def_named_color('SpringGreen', '(0.739988, 1.0, 0.239988)');
def_named_color('Tan', '(0.860012, 0.580006, 0.439988)');
def_named_color('TealBlue', '(0.119994, 0.980006, 0.640018)');
def_named_color('Thistle', '(0.880006, 0.410012, 1.0)');
def_named_color('Turquoise', '(0.15, 1.0, 0.8)');
def_named_color('Violet', '(0.210012, 0.119994, 1.0)');
def_named_color('VioletRed', '(1.0, 0.189988, 1.0)');
def_named_color('White', '(1.0, 1.0, 1.0)');
def_named_color('WildStrawberry', '(1.0, 0.0399878, 0.610012)');
def_named_color('Yellow', '(1.0, 1.0, 0.0)');
def_named_color('YellowGreen', '(0.560012, 1.0, 0.260012)');
def_named_color('YellowOrange', '(1.0, 0.580006, 0.0)');

@ Color commands get a separate warning procedure. |warn| sets |history :=
warning_given|, which causes a nonzero exit status; but color errors are
trivial and should leave the exit status zero.

@d color_warn(#)==begin err_print_ln('DVItoMP warning: ',#); if history < warning_given then history := cksum_trouble; end

@ The |do_xxx| procedure handles DVI specials (defined with the
|xxx1...xxx4| commands).

@<Declare procedures to handle color commands@> =
procedure do_xxx(p: integer);
label 9999; {exit procedure}
const bufsiz = 256; {FIXME: Fixed size buffer.}
var buf: packed array[0..bufsiz] of eight_bits;
    l, r, m, k, len: integer;
    found: boolean;
begin
  len := 0;
  while (p > 0) and (len < bufsiz) do begin
    buf[len] := get_byte;
    decr(p); incr(len);
  end;
  @<Check whether |buf| contains a color command; if not, |goto 9999|@>
  if p > 0 then begin
     color_warn('long "color" special ignored'); goto 9999; end;
  if @<|buf| contains a color pop command@> then begin
     @<Handle a color pop command@>
  end else if @<|buf| contains a color push command@> then begin
     @<Handle a color push command@>
  end else begin
     color_warn('unknown "color" special ignored'); goto 9999; end;
9999: for k := 1 to p do down_the_drain := get_byte;
end;

@ 

@<Check whether |buf| contains a color command; if not, |goto 9999|@> =
if (len <= 5)
   or (buf[0] <> "c")
   or (buf[1] <> "o")
   or (buf[2] <> "l")
   or (buf[3] <> "o")
   or (buf[4] <> "r")
   or (buf[5] <> " ")
  then goto 9999;

@ 

@<|buf| contains a color push command@> =
(len >= 11) and (buf[6] = "p") and (buf[7] = "u") and (buf[8] = "s") and (buf[9] = "h") and (buf[10] = " ")

@ 

@<|buf| contains a color pop command@> =
(len = 9) and (buf[6] = "p") and (buf[7] = "o") and (buf[8] = "p")

@ The \.{color push} and \.{pop} commands imply a color stack, so we need a
global variable to hold that stack.

@<Constants...@> =
max_color_stack_depth=10; {maximum depth of saved color stack}

@ Here's the actual stack variables.

@<Globals...@> =
color_stack_depth: integer; {current depth of saved color stack}
color_stack: array[1..max_color_stack_depth] of c_string; {saved color stack}

@ Initialize the stack to empty.

@<Set initial values@> =
color_stack_depth := 0;

@ \.{color pop} just pops the stack.

@<Handle a color pop command@> =
finish_last_char;
if color_stack_depth > 0 then begin
  libc_free(color_stack[color_stack_depth]);
  decr(color_stack_depth)
end
else begin
  color_warn('color stack underflow')
end;

@ \.{color push} pushes a color onto the stack.

@<Handle a color push command@> =
finish_last_char;
if color_stack_depth >= max_color_stack_depth then
  abort('color stack overflow');
incr(color_stack_depth);
{ I don't know how to do string operations in Pascal. }
{ Skip over extra spaces after 'color push'. }
l := 11;
while (l < len - 1) and (buf[l] = " ") do incr(l);
if @<|buf[l]| contains an rgb command@> then begin
  @<Handle a color push rgb command@>
end else if @<|buf[l]| contains a cmyk command@> then begin
  @<Handle a color push cmyk command@>
end else if @<|buf[l]| contains a gray command@> then begin
  @<Handle a color push gray command@>
end else begin
  @<Handle a named color push command@>
end;

@ 

@<|buf[l]| contains an rgb command@> =
(l + 4 < len)
and (buf[l]   = "r")
and (buf[l+1] = "g")
and (buf[l+2] = "b")
and (buf[l+3] = " ")

@

@<Handle a color push rgb command@> =
l := l + 4;
while (l < len) and (buf[l] = " ") do incr(l);
{ Remove spaces at end of buf }
while (len > l) and (buf[len - 1] = " ") do decr(len);
color_stack[color_stack_depth]:=xmalloc_char_array(len-l+3);
k := 0;
@<Copy |buf[l]| to |color_stack[color_stack_depth][k]| in tuple form@>

@ 

@<|buf[l]| contains a gray command@> =
(l + 5 < len)
and (buf[l]   = "g")
and (buf[l+1] = "r")
and (buf[l+2] = "a")
and (buf[l+3] = "y")
and (buf[l+4] = " ")

@

@<Handle a color push gray command@> =
l := l + 5;
while (l < len) and (buf[l] = " ") do incr(l);
{ Remove spaces at end of buf }
while (len > l) and (buf[len - 1] = " ") do decr(len);
color_stack[color_stack_depth]:=xmalloc_char_array(len-l+9);
strcpy(color_stack[color_stack_depth],'white*');
k := 6;
@<Copy |buf[l]| to |color_stack[color_stack_depth][k]| in tuple form@>

@ 

@<|buf[l]| contains a cmyk command@> =
(l + 5 < len)
and (buf[l]   = "c")
and (buf[l+1] = "m")
and (buf[l+2] = "y")
and (buf[l+3] = "k")
and (buf[l+4] = " ")

@

@<Handle a color push cmyk command@> =
l := l + 5;
while (l < len) and (buf[l] = " ") do incr(l);
{ Remove spaces at end of buf }
while (len > l) and (buf[len - 1] = " ") do decr(len);
color_stack[color_stack_depth]:=xmalloc_char_array(len-l+7);
strcpy(color_stack[color_stack_depth],'cmyk');
k := 4;
@<Copy |buf[l]| to |color_stack[color_stack_depth][k]| in tuple form@>

@

@<Copy |buf[l]| to |color_stack[color_stack_depth][k]| in tuple form@> =
color_stack[color_stack_depth][k] := "(";
incr(k);
while l < len do
  if buf[l] = " " then begin
    color_stack[color_stack_depth][k] := ",";
    while (l < len) and (buf[l] = " ") do incr(l);
    incr(k);
  end else begin
    color_stack[color_stack_depth][k] := buf[l];
    incr(l);
    incr(k);
  end;
color_stack[color_stack_depth][k] := ")";
color_stack[color_stack_depth][k+1] := chr(0)

@ Binary-search the |named_colors| array, then push the found color onto
the stack.

@<Handle a named color push command@> =
for k := l to len - 1 do begin
  buf[k - l] := xchr[buf[k]];
end;
buf[len - l] := 0;
len := len - l;
l := 1; r := num_named_colors;
found := false;
while (l <= r) and not found do begin
  m := (l + r) / 2;
  k := strcmp(stringcast(buf), named_colors[m].name);
  if k = 0 then begin
    color_stack[color_stack_depth]:=xstrdup(named_colors[m].value);
    found := true;
  end else if k < 0 then
    r := m - 1
  else
    l := m + 1;
end;
if not found then begin
   color_warn('non-hardcoded color "',stringcast(buf),'" in "color push" command');
   color_stack[color_stack_depth]:=xstrdup(stringcast(buf));
end;

@ Last but not least, this code snippet prints a \.{withcolor} specifier
for the top of the color stack, if the stack is nonempty.

@<Print a \.{withcolor} specifier if appropriate@> =
if color_stack_depth > 0 then begin
  print(' withcolor ');
  fputs(color_stack[color_stack_depth], mpx_file);
end;

@z
