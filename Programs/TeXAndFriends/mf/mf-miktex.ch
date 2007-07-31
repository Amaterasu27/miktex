%% mf-miktex.ch: WEB change file for METAFONT
%% 
%% Copyright (C) 1991-2007 Christian Schenk
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
% [1.4]
% _____________________________________________________________________________

@x
procedure initialize; {this procedure gets things started properly}
@y
@t\4@>@<Declare \MiKTeX\ procedures@>@/
@#
procedure initialize; {this procedure gets things started properly}
@z

% _____________________________________________________________________________
%
% [1.6]
% _____________________________________________________________________________

@x
@d start_of_MF=1 {go here when \MF's variables are initialized}
@d end_of_MF=9998 {go here to close files and terminate gracefully}
@d final_end=9999 {this label marks the ending of the program}
@y
@d start_of_MF=1 {go here when \MF's variables are initialized}
@d end_of_MF=9998 {go here to close files and terminate gracefully}
@d final_end=9999 {this label marks the ending of the program}
@d goto_fin_end==c4p_throw(final_end)
@d goto_eof_MF==c4p_throw(end_of_MF)
@z

% _____________________________________________________________________________
%
% [1.8]
% _____________________________________________________________________________

@x
@d init== {change this to `$\\{init}\equiv\.{@@\{}$' in the production version}
@d tini== {change this to `$\\{tini}\equiv\.{@@\}}$' in the production version}
@y
@d init== if miktex_is_init_program then begin
@d tini== end;
@z

% _____________________________________________________________________________
%
% [1.11]
% _____________________________________________________________________________

@x
@!mem_max=30000; {greatest index in \MF's internal |mem| array;
@y
@!mem_max_def=1048576; {greatest index in \MF's internal |mem| array;
@z

@x
@!max_internal=100; {maximum number of internal quantities}
@y
@!max_internal=300; {maximum number of internal quantities}
@z

@x
@!buf_size=500; {maximum number of characters simultaneously present in
@y
@!buf_size_def=66000; {maximum number of characters simultaneously present in
@z

@x
@!error_line=72; {width of context lines on terminal error messages}
@y
@!error_line_def=72; {width of context lines on terminal error messages}
@z

@x
@!half_error_line=42; {width of first lines of contexts in terminal
@y
@!half_error_line_def=42; {width of first lines of contexts in terminal
@z

@x
@!max_print_line=79; {width of longest text lines output; should be at least 60}
@y
@!max_print_line_def=72; {width of longest text lines output; should be at least 60}
@z

@x
@!screen_width=768; {number of pixels in each row of screen display}
@y
@!screen_width=768; {number of pixels in each row of screen display}
@z

@x
@!screen_depth=1024; {number of pixels in each column of screen display}
@y
@!screen_depth=1024; {number of pixels in each column of screen display}
@z

@x
@!stack_size=30; {maximum number of simultaneous input sources}
@y
@!stack_size_def=300; {maximum number of simultaneous input sources}
@z

@x
@!max_strings=2000; {maximum number of strings; must not exceed |max_halfword|}
@y
@!max_strings_def=70000; {maximum number of strings; must not exceed |max_halfword|}
@z

@x
@!string_vacancies=8000; {the minimum number of characters that should be
@y
@!string_vacancies_def=74000; {the minimum number of characters that should be
@z

@x
@!pool_size=32000; {maximum number of characters in strings, including all
@y
@!pool_size_def=100000; {maximum number of characters in strings, including all
@z

@x
@!move_size=5000; {space for storing moves in a single octant}
@y
@!move_size_def=70000; {space for storing moves in a single octant}
@z

@x
@!max_wiggle=300; {number of autorounded points per cycle}
@y
@!max_wiggle_def=70000; {number of autorounded points per cycle}
@z

@x
@!gf_buf_size=800; {size of the output buffer, must be a multiple of 8}
@y
@!gf_buf_size=8192; {size of the output buffer, must be a multiple of 8}
@z

@x
@!file_name_size=40; {file names shouldn't be longer than this}
@y
@!file_name_size=259; {file names shouldn't be longer than this}
@!file_name_size_plus_one=260; {one more for the string terminator}
@z

@x
@!path_size=300; {maximum number of knots between breakpoints of a path}
@y
@!path_size_def=70000; {maximum number of knots between breakpoints of a path}
@z

@x
@!bistack_size=785; {size of stack for bisection algorithms;
@y
@!bistack_size_def=70000; {size of stack for bisection algorithms;
@z

@x
@!header_size=100; {maximum number of \.{TFM} header words, times~4}
@y
@!header_size=100; {maximum number of \.{TFM} header words, times~4}
@z

@x
@!lig_table_size=5000; {maximum number of ligature/kern steps, must be
@y
@!lig_table_size_def=15000; {maximum number of ligature/kern steps, must be
@z

@x
@!max_kerns=500; {maximum number of distinct kern amounts}
@y
@!max_kerns=2500; {maximum number of distinct kern amounts}
@z

@x
@!max_font_dimen=50; {maximum number of \&{fontdimen} parameters}
@y
@!max_font_dimen=60; {maximum number of \&{fontdimen} parameters}
@z

% _____________________________________________________________________________
%
% [1.12]
% _____________________________________________________________________________

@x
@d mem_min=0 {smallest index in the |mem| array, must not be less
@y
@d mem_min_def=0 {smallest index in the |mem| array, must not be less
@z

@x
@d mem_top==30000 {largest index in the |mem| array dumped by \.{INIMF};
@y
@d mem_top_def==1048576 {largest index in the |mem| array dumped by \.{INIMF};
@z

@x
@d hash_size=2100 {maximum number of symbolic tokens,
@y
@d hash_size=9500 {maximum number of symbolic tokens,
@z

@x
@d hash_prime=1777 {a prime number equal to about 85\pct! of |hash_size|}
@y
@d hash_prime=7919 {a prime number equal to about 85\pct! of |hash_size|}
@z

@x
@d max_in_open=6 {maximum number of input files and error insertions that
@y
@d max_in_open=20 {maximum number of input files and error insertions that
@z

@x
@d param_size=150 {maximum number of simultaneous macro parameters}
@y
@d param_size_def=150 {maximum number of simultaneous macro parameters}
@z

% _____________________________________________________________________________
%
% [2.19]
% _____________________________________________________________________________

@x
@d first_text_char=0 {ordinal number of the smallest element of |text_char|}
@y
@d tabulator=@'9
@d form_feed=@'14
@d first_text_char=0 {ordinal number of the smallest element of |text_char|}
@z

% _____________________________________________________________________________
%
% [2.20]
% _____________________________________________________________________________

@x
@!xord: array [text_char] of ASCII_code;
@y
@!xord: array [0..255] of ASCII_code;
@z

% _____________________________________________________________________________
%
% [2.22]
% _____________________________________________________________________________

@x
for i:=0 to @'37 do xchr[i]:=' ';
for i:=@'177 to @'377 do xchr[i]:=' ';
@y
do_nothing;
@z

% _____________________________________________________________________________
%
% [2.23]
% _____________________________________________________________________________

@x
@<Set init...@>=
for i:=first_text_char to last_text_char do xord[chr(i)]:=@'177;
for i:=@'200 to @'377 do xord[xchr[i]]:=i;
for i:=0 to @'176 do xord[xchr[i]]:=i;
@y
@<Set init...@>=
miktex_initialize_char_tables;
@z

% _____________________________________________________________________________
%
% [3.25]
% _____________________________________________________________________________

@x
@!name_of_file:packed array[1..file_name_size] of char;@;@/
  {on some systems this may be a \&{record} variable}
@y
@!name_of_file:packed array[1..file_name_size_plus_one] of char;@;@/
  {on some systems this may be a \&{record} variable}
@z

% _____________________________________________________________________________
%
% [3.26]
% _____________________________________________________________________________

@x
begin reset(f,name_of_file,'/O'); a_open_in:=reset_OK(f);
@y
begin if not miktex_open_input_file(f) then a_open_in:=false
   else a_open_in:=true;
@z

@x
begin rewrite(f,name_of_file,'/O'); a_open_out:=rewrite_OK(f);
@y
begin a_open_out:=miktex_open_output_file(f, true);
@z

@x
begin rewrite(f,name_of_file,'/O'); b_open_out:=rewrite_OK(f);
@y
begin b_open_out:=miktex_open_output_file(f, false);
@z

@x
begin reset(f,name_of_file,'/O'); w_open_in:=reset_OK(f);
@y
begin w_open_in:=false
@z

@x
begin rewrite(f,name_of_file,'/O'); w_open_out:=rewrite_OK(f);
@y
begin w_open_out:=miktex_open_output_file(f, false);
@z

@x
begin close(f);
@y
begin miktex_close_file(f);
@z

@x
begin close(f);
@y
begin miktex_close_file(f);
@z

@x
begin close(f);
@y
begin miktex_close_file(f);
@z

% _____________________________________________________________________________
%
% [3.29]
% _____________________________________________________________________________

@x
@!buffer:array[0..buf_size] of ASCII_code; {lines of characters being read}
@!first:0..buf_size; {the first unused position in |buffer|}
@!last:0..buf_size; {end of the line just input to |buffer|}
@!max_buf_stack:0..buf_size; {largest index used in |buffer|}
@y
@!buffer:array[0..buf_size_def] of ASCII_code; {lines of characters being read}
@!first:0..buf_size_def; {the first unused position in |buffer|}
@!last:0..buf_size_def; {end of the line just input to |buffer|}
@!max_buf_stack:0..buf_size_def; {largest index used in |buffer|}
@z

% _____________________________________________________________________________
%
% [3.30]
% _____________________________________________________________________________

@x
@p function input_ln(var @!f:alpha_file;@!bypass_eoln:boolean):boolean;
  {inputs the next line or returns |false|}
var @!last_nonblank:0..buf_size; {|last| with trailing blanks removed}
begin if bypass_eoln then if not eof(f) then get(f);
  {input the first character of the line into |f^|}
last:=first; {cf.\ Matthew 19\thinspace:\thinspace30}
if eof(f) then input_ln:=false
else  begin last_nonblank:=first;
  while not eoln(f) do
    begin if last>=max_buf_stack then
      begin max_buf_stack:=last+1;
      if max_buf_stack=buf_size then
        @<Report overflow of the input buffer, and abort@>;
      end;
    buffer[last]:=xord[f^]; get(f); incr(last);
    if buffer[last-1]<>" " then last_nonblank:=last;
    end;
  last:=last_nonblank; input_ln:=true;
  end;
end;
@y
@p function input_ln(var f:alpha_file;@!bypass_eoln:boolean):boolean;forward;
@z

% _____________________________________________________________________________
%
% [3.32]
% _____________________________________________________________________________

@x
@d t_open_in==reset(term_in,'TTY:','/O/I') {open the terminal for text input}
@d t_open_out==rewrite(term_out,'TTY:','/O') {open the terminal for text output}
@y
@d t_open_in==term_in:=i@&nput
@d t_open_out==term_out:=output
@z

% _____________________________________________________________________________
%
% [3.33]
% _____________________________________________________________________________

@x
@d clear_terminal == break_in(term_in,true) {clear the terminal input buffer}
@y
@d clear_terminal == break(term_in) {clear the terminal input buffer}
@z

% _____________________________________________________________________________
%
% [3.34]
% _____________________________________________________________________________

@x
  begin write_ln(term_out,'Buffer size exceeded!'); goto final_end;
@y
  begin write_ln('Buffer size exceeded!'); goto_fin_end;
@z

% _____________________________________________________________________________
%
% [3.36]
% _____________________________________________________________________________

@x
loop@+begin wake_up_terminal; write(term_out,'**'); update_terminal;
@y
loop@+begin
  if (c4p_plen > 0) then miktex_initialize_buffer
  else begin
    wake_up_terminal; write(term_out,'**'); update_terminal;
@z

@x
  loc:=first;
@y
    end;
  loc:=first;
@z

% _____________________________________________________________________________
%
% [4.37] \[4] String handling
% _____________________________________________________________________________

@x
@!pool_pointer = 0..pool_size; {for variables that point into |str_pool|}
@!str_number = 0..max_strings; {for variables that point into |str_start|}
@y
@!pool_pointer = 0..pool_size_def; {for variables that point into |str_pool|}
@!str_number = 0..max_strings_def; {for variables that point into |str_start|}
@z

% _____________________________________________________________________________
%
% [4.47]
% _____________________________________________________________________________

@x
@p @!init function get_strings_started:boolean; {initializes the string pool,
@y
@p function get_strings_started:boolean; {initializes the string pool,
@z

@x
tini
@y
@z

% _____________________________________________________________________________
%
% [4.49]
% _____________________________________________________________________________

@x
@<Character |k| cannot be printed@>=
  (k<" ")or(k>"~")
@y
@<Character |k| cannot be printed@>=
  not miktex_is_printable (k)
@z

% _____________________________________________________________________________
%
% [4.50]
% _____________________________________________________________________________

@x
@!init @!pool_file:alpha_file; {the string-pool file output by \.{TANGLE}}
tini
@y
@!pool_file:alpha_file; {the string-pool file output by \.{TANGLE}}
@z

% _____________________________________________________________________________
%
% [4.51]
% _____________________________________________________________________________

@x
name_of_file:=pool_name; {we needn't set |name_length|}
if a_open_in(pool_file) then
@y
miktex_get_pool_file_name(name_of_file);
if miktex_open_pool_file(pool_file) then
@z

@x
else  bad_pool('! I can''t read MF.POOL.')
@.I can't read MF.POOL@>
@y
else begin
  wake_up_terminal;
  write_ln(term_out, '! I can''t read MF.POOL.');
@.I can't read MF.POOL@>
  get_strings_started:=false;
  return;
end
@z

% _____________________________________________________________________________
%
% [5.54] \[5] On-line and off-line printing
% _____________________________________________________________________________

@x
@!term_offset : 0..max_print_line;
  {the number of characters on the current terminal line}
@!file_offset : 0..max_print_line;
  {the number of characters on the current file line}
@!trick_buf:array[0..error_line] of ASCII_code; {circular buffer for
@y
@!term_offset : 0..max_print_line_def;
  {the number of characters on the current terminal line}
@!file_offset : 0..max_print_line_def;
  {the number of characters on the current file line}
@!trick_buf:array[0..error_line_def] of ASCII_code; {circular buffer for
@z

% _____________________________________________________________________________
%
% [5.56]
% _____________________________________________________________________________

@x
@d wterm(#)==write(term_out,#)
@d wterm_ln(#)==write_ln(term_out,#)
@d wterm_cr==write_ln(term_out)
@y
@d wterm(#)==miktex_termout(#)
@d wterm_ln(#)==miktex_termout_ln(#)
@d wterm_cr==miktex_termout_ln
@z

% _____________________________________________________________________________
%
% [5.61]
% _____________________________________________________________________________

@x
wterm(banner);
if base_ident=0 then wterm_ln(' (no base preloaded)')
else  begin slow_print(base_ident); print_ln;
  end;
@y
if (not miktex_get_quiet_flag) then begin
  wterm(banner);
  miktex_print_miktex_banner(term_out);
  if base_ident=0 then print_ln {|wterm_ln(' (no base preloaded)')|}
  else  begin slow_print(base_ident); print_ln;
    end;
end;
@z

% _____________________________________________________________________________
%
% [5.66]
% _____________________________________________________________________________

@x
var @!k:0..buf_size; {index into |buffer|}
@y
var @!k:0..buf_size_def; {index into |buffer|}
@z

% _____________________________________________________________________________
%
% [6.68]
% _____________________________________________________________________________

@x
  print_nl("! "); print(#);
@y
  if (miktex_c_style_error_messages_p and not terminal_input) then
    print_c_style_error_message (#)
  else begin print_nl("! "); print(#) end;
@z

% _____________________________________________________________________________
%
% [6.69]
% _____________________________________________________________________________

@x
@ @<Set init...@>=interaction:=error_stop_mode;
@y
@ @<Set init...@>=
if miktex_get_interaction >= 0 then
  interaction:=miktex_get_interaction
else
  interaction:=error_stop_mode;
@z

% _____________________________________________________________________________
%
% [6.76]
% _____________________________________________________________________________

@x
begin goto end_of_MF;
@y
begin goto_eof_MF;
@z

% _____________________________________________________________________________
%
% [6.77]
% _____________________________________________________________________________

@x
print_char("."); show_context;
@y
if not miktex_c_style_error_messages_p then begin
  print_char("."); show_context
end;
if (miktex_halt_on_error_p) then begin
  history:=fatal_error_stop; jump_out;
end;
@z

% _____________________________________________________________________________
%
% [6.79]
% _____________________________________________________________________________

@x
  begin print_nl("You want to edit file ");
@.You want to edit file x@>
  slow_print(input_stack[file_ptr].name_field);
  print(" at line "); print_int(line);@/
  interaction:=scroll_mode; jump_out;
@y
  begin edit_name_start:=str_start[input_stack[file_ptr].name_field];
    edit_name_length:=str_start[input_stack[file_ptr].name_field+1] -
                      str_start[input_stack[file_ptr].name_field];
    edit_line:=line;
    jump_out;
@z

% _____________________________________________________________________________
%
% [7.107]
% _____________________________________________________________________________

@x
@p function make_fraction(@!p,@!q:integer):fraction;
var @!f:integer; {the fraction bits, with a leading 1 bit}
@!n:integer; {the integer part of $\vert p/q\vert$}
@!negative:boolean; {should the result be negated?}
@!be_careful:integer; {disables certain compiler optimizations}
begin if p>=0 then negative:=false
else  begin negate(p); negative:=true;
  end;
if q<=0 then
  begin debug if q=0 then confusion("/");@;@+gubed@;@/
@:this can't happen /}{\quad \./@>
  negate(q); negative:=not negative;
  end;
n:=p div q; p:=p mod q;
if n>=8 then
  begin arith_error:=true;
  if negative then make_fraction:=-el_gordo@+else make_fraction:=el_gordo;
  end
else  begin n:=(n-1)*fraction_one;
  @<Compute $f=\lfloor 2^{28}(1+p/q)+{1\over2}\rfloor$@>;
  if negative then make_fraction:=-(f+n)@+else make_fraction:=f+n;
  end;
end;
@y
@z

% _____________________________________________________________________________
%
% [7.109]
% _____________________________________________________________________________

@x
@p function take_fraction(@!q:integer;@!f:fraction):integer;
var @!p:integer; {the fraction so far}
@!negative:boolean; {should the result be negated?}
@!n:integer; {additional multiple of $q$}
@!be_careful:integer; {disables certain compiler optimizations}
begin @<Reduce to the case that |f>=0| and |q>0|@>;
if f<fraction_one then n:=0
else  begin n:=f div fraction_one; f:=f mod fraction_one;
  if q<=el_gordo div n then n:=n*q
  else  begin arith_error:=true; n:=el_gordo;
    end;
  end;
f:=f+fraction_one;
@<Compute $p=\lfloor qf/2^{28}+{1\over2}\rfloor-q$@>;
be_careful:=n-el_gordo;
if be_careful+p>0 then
  begin arith_error:=true; n:=el_gordo-p;
  end;
if negative then take_fraction:=-(n+p)
else take_fraction:=n+p;
end;
@y
@z

% _____________________________________________________________________________
%
% [7.112]
% _____________________________________________________________________________

@x
@p function take_scaled(@!q:integer;@!f:scaled):integer;
var @!p:integer; {the fraction so far}
@!negative:boolean; {should the result be negated?}
@!n:integer; {additional multiple of $q$}
@!be_careful:integer; {disables certain compiler optimizations}
begin @<Reduce to the case that |f>=0| and |q>0|@>;
if f<unity then n:=0
else  begin n:=f div unity; f:=f mod unity;
  if q<=el_gordo div n then n:=n*q
  else  begin arith_error:=true; n:=el_gordo;
    end;
  end;
f:=f+unity;
@<Compute $p=\lfloor qf/2^{16}+{1\over2}\rfloor-q$@>;
be_careful:=n-el_gordo;
if be_careful+p>0 then
  begin arith_error:=true; n:=el_gordo-p;
  end;
if negative then take_scaled:=-(n+p)
else take_scaled:=n+p;
end;
@y
@z

% _____________________________________________________________________________
%
% [7.114]
% _____________________________________________________________________________

 @x
@p function make_scaled(@!p,@!q:integer):scaled;
var @!f:integer; {the fraction bits, with a leading 1 bit}
@!n:integer; {the integer part of $\vert p/q\vert$}
@!negative:boolean; {should the result be negated?}
@!be_careful:integer; {disables certain compiler optimizations}
begin if p>=0 then negative:=false
else  begin negate(p); negative:=true;
  end;
if q<=0 then
  begin debug if q=0 then confusion("/");@+gubed@;@/
@:this can't happen /}{\quad \./@>
  negate(q); negative:=not negative;
  end;
n:=p div q; p:=p mod q;
if n>=@'100000 then
  begin arith_error:=true;
  if negative then make_scaled:=-el_gordo@+else make_scaled:=el_gordo;
  end
else  begin n:=(n-1)*unity;
  @<Compute $f=\lfloor 2^{16}(1+p/q)+{1\over2}\rfloor$@>;
  if negative then make_scaled:=-(f+n)@+else make_scaled:=f+n;
  end;
end;
 @y
 @z

% _____________________________________________________________________________
%
% [9.153] \[9] Packed data
% _____________________________________________________________________________

@x
@d min_quarterword=0 {smallest allowable value in a |quarterword|}
@d max_quarterword=255 {largest allowable value in a |quarterword|}
@d min_halfword==0 {smallest allowable value in a |halfword|}
@d max_halfword==65535 {largest allowable value in a |halfword|}
@y
@d min_quarterword=0 {smallest allowable value in a |quarterword|}
@d max_quarterword=1023 {largest allowable value in a |quarterword|}
@d min_halfword==0 {smallest allowable value in a |halfword|}
@d max_halfword==@"3FFFFFFF {largest allowable value in a |halfword|}
@z

% _____________________________________________________________________________
%
% [10.159]
% _____________________________________________________________________________

@x
@!mem : array[mem_min..mem_max] of memory_word; {the big dynamic storage area}
@y
@!mem : array[mem_min_def..mem_max_def] of memory_word; {the big dynamic storage area}
@z

% _____________________________________________________________________________
%
% [10.173]
% _____________________________________________________________________________

@x
@p @!init procedure sort_avail; {sorts the available variable-size nodes
@y
@p procedure sort_avail; {sorts the available variable-size nodes
@z

@x
tini
@y
@z

% _____________________________________________________________________________
%
% [11.178]
% _____________________________________________________________________________

@x
@!debug @!free: packed array [mem_min..mem_max] of boolean; {free cells}
@t\hskip1em@>@!was_free: packed array [mem_min..mem_max] of boolean;
@y
@!debug @!free: packed array [mem_min_def..mem_max_def] of boolean; {free cells}
@t\hskip1em@>@!was_free: packed array [mem_min_def..mem_max_def] of boolean;
@z

% _____________________________________________________________________________
%
% [12.194]
% _____________________________________________________________________________

@x
begin internal[time]:=12*60*unity; {minutes since midnight}
internal[day]:=4*unity; {fourth day of the month}
internal[month]:=7*unity; {seventh month of the year}
internal[year]:=1776*unity; {Anno Domini}
@y
begin internal[time]:=(c4p_hour*60+c4p_minute)*unity; {minutes since midnight}
internal[day]:=c4p_day*unity; {day of month}
internal[month]:=c4p_month*unity; {month of year}
internal[year]:=c4p_year*unity; {Anno Domini}
@z

% _____________________________________________________________________________
%
% [12.199]
% _____________________________________________________________________________

@x
for k:=127 to 255 do char_class[k]:=invalid_class;
@y
for k:=127 to 255 do char_class[k]:=invalid_class;
char_class[tabulator]:=space_class;
char_class[form_feed]:=space_class;
@z

% _____________________________________________________________________________
%
% [13.210]
% _____________________________________________________________________________

@x
@p @!init procedure primitive(@!s:str_number;@!c:halfword;@!o:halfword);
@y
@p procedure primitive(@!s:str_number;@!c:halfword;@!o:halfword);
@z

@x
tini
@y
@z

% _____________________________________________________________________________
%
% [18.279]
% _____________________________________________________________________________

@x
@!delta_x,@!delta_y,@!delta:array[0..path_size] of scaled; {knot differences}
@!psi:array[1..path_size] of angle; {turning angles}
@y
@!delta_x:array[0..path_size_def] of scaled; {knot differences}
@!delta_y:array[0..path_size_def] of scaled; {knot differences}
@!delta:array[0..path_size_def] of scaled; {knot differences}
@!psi:array[1..path_size_def] of angle; {turning angles}
@z

% _____________________________________________________________________________
%
% [18.280]
% _____________________________________________________________________________

@x
@!k,@!n:0..path_size; {current and final knot numbers}
@y
@!k,@!n:0..path_size_def; {current and final knot numbers}
@z

% _____________________________________________________________________________
%
% [18.283]
% _____________________________________________________________________________

@x
@!theta:array[0..path_size] of angle; {values of $\theta_k$}
@!uu:array[0..path_size] of fraction; {values of $u_k$}
@!vv:array[0..path_size] of angle; {values of $v_k$}
@!ww:array[0..path_size] of fraction; {values of $w_k$}
@y
@!theta:array[0..path_size_def] of angle; {values of $\theta_k$}
@!uu:array[0..path_size_def] of fraction; {values of $u_k$}
@!vv:array[0..path_size_def] of angle; {values of $v_k$}
@!ww:array[0..path_size_def] of fraction; {values of $w_k$}
@z

% _____________________________________________________________________________
%
% [18.284]
% _____________________________________________________________________________

@x
var @!k:0..path_size; {current knot number}
@y
var @!k:0..path_size_def; {current knot number}
@z

% _____________________________________________________________________________
%
% [19.308]
% _____________________________________________________________________________

@x
@!move:array[0..move_size] of integer; {the recorded moves}
@!move_ptr:0..move_size; {the number of items in the |move| list}
@y
@!move:array[0..move_size_def] of integer; {the recorded moves}
@!move_ptr:0..move_size_def; {the number of items in the |move| list}
@z

@!move:array[0..move_size] of integer; {the recorded moves}
@!move_ptr:0..move_size; {the number of items in the |move| list}

% _____________________________________________________________________________
%
% [19.309]
% _____________________________________________________________________________

@x
@!bisect_stack:array[0..bistack_size] of integer;
@!bisect_ptr:0..bistack_size;
@y
@!bisect_stack:array[0..bistack_size_def] of integer;
@!bisect_ptr:0..bistack_size_def;
@z

% _____________________________________________________________________________
%
% [19.321]
% _____________________________________________________________________________

@x
var@!k:1..move_size; {index into |move|}
@y
var@!k:1..move_size_def; {index into |move|}
@z

% _____________________________________________________________________________
%
% [20.357]
% _____________________________________________________________________________

@x
@!j,@!jj:0..move_size; {indices into |move|}
@y
@!j,@!jj:0..move_size_def; {indices into |move|}
@z

% _____________________________________________________________________________
%
% [20.378]
% _____________________________________________________________________________

@x
var @!delta:0..move_size; {extent of |move| data}
@!k:0..move_size; {index into |move|}
@y
var @!delta:0..move_size_def; {extent of |move| data}
@!k:0..move_size_def; {index into |move|}
@z

% _____________________________________________________________________________
%
% [21.426]
% _____________________________________________________________________________

@x
var @!k:0..max_wiggle; {runs through the list of inputs}
@y
var @!k:0..max_wiggle_def; {runs through the list of inputs}
@z

% _____________________________________________________________________________
%
% [21.427]
% _____________________________________________________________________________

@x
@!before,@!after:array[0..max_wiggle] of scaled; {data for |make_safe|}
@!node_to_round:array[0..max_wiggle] of pointer; {reference back to the path}
@!cur_rounding_ptr:0..max_wiggle; {how many are being used}
@!max_rounding_ptr:0..max_wiggle; {how many have been used}
@y
@!before:array[0..max_wiggle_def] of scaled; {data for |make_safe|}
@!after:array[0..max_wiggle_def] of scaled; {data for |make_safe|}
@!node_to_round:array[0..max_wiggle_def] of pointer; {reference back to the path}
@!cur_rounding_ptr:0..max_wiggle_def; {how many are being used}
@!max_rounding_ptr:0..max_wiggle_def; {how many have been used}
@z

% _____________________________________________________________________________
%
% [21.440]
% _____________________________________________________________________________

@x
@!k:0..max_wiggle; {runs through before-and-after values}
@y
@!k:0..max_wiggle_def; {runs through before-and-after values}
@z

% _____________________________________________________________________________
%
% [24.507]
% _____________________________________________________________________________

@x
@!env_move:array[0..move_size] of integer;
@y
@!env_move:array[0..move_size_def] of integer;
@z

% _____________________________________________________________________________
%
% [24.511]
% _____________________________________________________________________________

@x
@!smooth_bot,@!smooth_top:0..move_size; {boundaries of smoothing}
@y
@!smooth_bot,@!smooth_top:0..move_size_def; {boundaries of smoothing}
@z

% _____________________________________________________________________________
%
% [26.557]
% _____________________________________________________________________________

@x
@!uv,@!xy:0..bistack_size; {pointers to the current packets of interest}
@y
@!uv,@!xy:0..bistack_size_def; {pointers to the current packets of interest}
@z

% _____________________________________________________________________________
%
% [27.564] \[27] Online graphic output
% _____________________________________________________________________________

@x
begin init_screen:=false;
@y
begin init_screen:=miktex_init_screen(screen_width, screen_depth);
@z

@x
begin @!init wlog_ln('Calling UPDATESCREEN');@+tini {for testing only}
@y
begin @!init wlog_ln('Calling UPDATESCREEN');@+tini {for testing only}
miktex_update_screen;
@z

% _____________________________________________________________________________
%
% [27.567]
% _____________________________________________________________________________

@x
var @!r:screen_row;
@!c:screen_col;
begin @{@+for r:=top_row to bot_row-1 do
  for c:=left_col to right_col-1 do
    screen_pixel[r,c]:=white;@+@}@/
@y
begin
  miktex_blank_rectangle(left_col, right_col, top_row, bot_row);
@z

% _____________________________________________________________________________
%
% [27.568]
% _____________________________________________________________________________

@x
var @!k:screen_col; {an index into |a|}
@!c:screen_col; {an index into |screen_pixel|}
begin @{ k:=0; c:=a[0];
repeat incr(k);
  repeat screen_pixel[r,c]:=b; incr(c);
  until c=a[k];
  b:=black-b; {$|black|\swap|white|$}
  until k=n;@+@}@/
@y
var @!k:screen_col; {an index into |a|}
begin
  miktex_paint_row(r, b, a, n);
@z

% _____________________________________________________________________________
%
% [31.628]
% _____________________________________________________________________________

@x
@!input_stack : array[0..stack_size] of in_state_record;
@!input_ptr : 0..stack_size; {first unused location of |input_stack|}
@!max_in_stack: 0..stack_size; {largest value of |input_ptr| when pushing}
@y
@!input_stack : array[0..stack_size_def] of in_state_record;
@!input_ptr : 0..stack_size_def; {first unused location of |input_stack|}
@!max_in_stack: 0..stack_size_def; {largest value of |input_ptr| when pushing}
@z

% _____________________________________________________________________________
%
% [31.631]
% _____________________________________________________________________________

@x
@!line_stack : array[1..max_in_open] of integer;
@y
@!line_stack : array[1..max_in_open] of integer;
@!source_filename_stack : array[1..max_in_open] of str_number;
@!full_source_filename_stack : array[1..max_in_open] of str_number;
@z

% _____________________________________________________________________________
%
% [31.633]
% _____________________________________________________________________________

@x
@!param_stack:array [0..param_size] of pointer;
  {token list pointers for parameters}
@!param_ptr:0..param_size; {first unused entry in |param_stack|}
@y
@!param_stack:array [0..param_size_def] of pointer;
  {token list pointers for parameters}
@!param_ptr:0..param_size_def; {first unused entry in |param_stack|}
@z

% _____________________________________________________________________________
%
% [31.634]
% _____________________________________________________________________________

@x
@!file_ptr:0..stack_size; {shallowest level shown by |show_context|}
@y
@!file_ptr:0..stack_size_def; {shallowest level shown by |show_context|}
@z

% _____________________________________________________________________________
%
% [31.641]
% _____________________________________________________________________________

@x
@!i:0..buf_size; {index into |buffer|}
@y
@!i:0..buf_size_def; {index into |buffer|}
@z

@x
@!n:0..error_line; {length of line 1}
@y
@!n:0..error_line_def; {length of line 1}
@z

% _____________________________________________________________________________
%
% [33.667]
% _____________________________________________________________________________

@x
var @!k:0..buf_size; {an index into |buffer|}
@y
var @!k:0..buf_size_def; {an index into |buffer|}
@z

% _____________________________________________________________________________
%
% [33.682]
% _____________________________________________________________________________

@x
var @!k:0..buf_size; {an index into |buffer|}
@y
var @!k:0..buf_size_def; {an index into |buffer|}
@z

% _____________________________________________________________________________
%
% [34.697]
% _____________________________________________________________________________

@x
@!k:0..param_size; {the total number of parameters}
@y
@!k:0..param_size_def; {the total number of parameters}
@z

% _____________________________________________________________________________
%
% [38.769]
% _____________________________________________________________________________

@x
@d MF_area=="MFinputs:"
@y
@d MF_area==""
@z

% _____________________________________________________________________________
%
% [38.771]
% _____________________________________________________________________________

@x
begin if c=" " then more_name:=false
else  begin if (c=">")or(c=":") then
@y
begin if c=" " or c=tabulator then more_name:=false
else  begin if (c="/") then
@z

% _____________________________________________________________________________
%
% [38.774]
% _____________________________________________________________________________

@x
for k:=name_length+1 to file_name_size do name_of_file[k]:=' ';
@y
name_of_file[ name_length + 1 ]:= chr(0);
@z

% _____________________________________________________________________________
%
% [38.775]
% _____________________________________________________________________________

@x
MF_base_default:='MFbases:plain.base';
@y
miktex_get_default_dump_file_name(MF_base_default);
@z

% _____________________________________________________________________________
%
% [38.776]
% _____________________________________________________________________________

@x
if base_default_length>file_name_size then bad:=41;
@y
do_nothing;
@z

% _____________________________________________________________________________
%
% [38.778]
% _____________________________________________________________________________

@x
@p procedure pack_buffered_name(@!n:small_number;@!a,@!b:integer);
var @!k:integer; {number of positions filled in |name_of_file|}
@!c: ASCII_code; {character being packed}
@!j:integer; {index into |buffer| or |MF_base_default|}
begin if n+b-a+1+base_ext_length>file_name_size then
  b:=a+file_name_size-n-1-base_ext_length;
k:=0;
for j:=1 to n do append_to_name(xord[MF_base_default[j]]);
for j:=a to b do append_to_name(buffer[j]);
for j:=base_default_length-base_ext_length+1 to base_default_length do
  append_to_name(xord[MF_base_default[j]]);
if k<=file_name_size then name_length:=k@+else name_length:=file_name_size;
for k:=name_length+1 to file_name_size do name_of_file[k]:=' ';
end;
@y
@z

% _____________________________________________________________________________
%
% [38.779]
% _____________________________________________________________________________

@x
var @!j:0..buf_size; {the first space after the file name}
@y
var @!j:0..buf_size_def; {the first space after the file name}
@z

@x
  pack_buffered_name(0,loc,j-1); {try first without the system file area}
  if w_open_in(base_file) then goto found;
  pack_buffered_name(base_area_length,loc,j-1);
    {now try the system base file area}
  if w_open_in(base_file) then goto found;
@y
  c4p_memcpy(name_of_file,file_name_size,c4p_ptr(buffer[loc]),j-loc);
  name_of_file[j-loc+1]:=chr(0);
  if miktex_open_base_file(base_file) then goto found;
@z

@x
  wterm_ln('Sorry, I can''t find that base;',' will try PLAIN.');
@y
  wterm_ln('Sorry, I can''t find that base;',' will try default.');
@z

@x
pack_buffered_name(base_default_length-base_ext_length,1,0);
if not w_open_in(base_file) then
@y
c4p_strcpy(name_of_file,file_name_size_plus_one,MF_base_default);
if not miktex_open_base_file(base_file) then
@z

@x
  wterm_ln('I can''t find the PLAIN base file!');
@y
  wterm_ln('I can''t find the default base file!');
@z

% _____________________________________________________________________________
%
% [38.786]
% _____________________________________________________________________________

@x
var @!k:0..buf_size; {index into |buffer|}
@y
var @!k:0..buf_size_def; {index into |buffer|}
@z

% _____________________________________________________________________________
%
% [38.788]
% _____________________________________________________________________________

@x
@!k:0..buf_size; {index into |months| and |buffer|}
@!l:0..buf_size; {end of first input line}
@y
@!k:0..buf_size_def; {index into |months| and |buffer|}
@!l:0..buf_size_def; {end of first input line}
@z

% _____________________________________________________________________________
%
% [38.790]
% _____________________________________________________________________________

@x
slow_print(base_ident); print("  ");
print_int(round_unscaled(internal[day])); print_char(" ");
months:='JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC';
@y
miktex_print_miktex_banner(log_file);
slow_print(base_ident); print("  ");
print_int(round_unscaled(internal[day])); print_char(" ");
c4p_arrcpy(months,'JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC');
@z

% _____________________________________________________________________________
%
% [38.793]
% _____________________________________________________________________________

@x
done: name:=a_make_name_string(cur_file); str_ref[cur_name]:=max_str_ref;
if job_name=0 then
  begin job_name:=cur_name; open_log_file;
@y
done: name:=a_make_name_string(cur_file); str_ref[cur_name]:=max_str_ref;
full_source_filename_stack[in_open]:=miktex_make_full_name_string;
if job_name=0 then
  begin job_name:=miktex_get_job_name; open_log_file;
@z

@x
if term_offset+length(name)>max_print_line-2 then print_ln
else if (term_offset>0)or(file_offset>0) then print_char(" ");
print_char("("); incr(open_parens); slow_print(name); update_terminal;
@y
if term_offset+length(full_source_filename_stack[in_open])>max_print_line-2 then print_ln
else if (term_offset>0)or(file_offset>0) then print_char(" ");
print_char("("); incr(open_parens); slow_print(full_source_filename_stack[in_open]); update_terminal;
@z

@x
if name=str_ptr-1 then {we can conserve string pool space now}
  begin flush_string(name); name:=cur_name;
  end;
@y
@z

% _____________________________________________________________________________
%
% [39.807]
% _____________________________________________________________________________

@x
if s<>"" then
  begin print_nl("! "); print(s);
@.!\relax@>
  end;
@y
if s<>"" then
  begin
  if (miktex_c_style_error_messages_p and not terminal_input) then
    print_c_style_error_message (s)
  else begin print_nl("! "); print(s);end;
@.!\relax@>
  end;
@z

% _____________________________________________________________________________
%
% [44.1046]
% _____________________________________________________________________________

@x
@!n:0..max_print_line; {amount of macro text to show}
@y
@!n:0..max_print_line_def; {amount of macro text to show}
@z

% _____________________________________________________________________________
%
% [45.1096]
% _____________________________________________________________________________

@x
@!char_remainder:array[eight_bits] of 0..lig_table_size; {the |remainder| byte}
@y
@!char_remainder:array[eight_bits] of 0..lig_table_size_def; {the |remainder| byte}
@z

@x
@!lig_kern:array[0..lig_table_size] of four_quarters; {the ligature/kern table}
@y
@!lig_kern:array[0..lig_table_size_def] of four_quarters; {the ligature/kern table}
@z

@x
@!skip_table:array[eight_bits] of 0..lig_table_size; {local label status}
@y
@!skip_table:array[eight_bits] of 0..lig_table_size_def; {local label status}
@z

@x
@!bch_label:0..lig_table_size; {left boundary starting location}
@!ll,@!lll:0..lig_table_size; {registers used for lig/kern processing}
@!label_loc:array[0..256] of -1..lig_table_size; {lig/kern starting addresses}
@y
@!bch_label:0..lig_table_size_def; {left boundary starting location}
@!ll,@!lll:0..lig_table_size_def; {registers used for lig/kern processing}
@!label_loc:array[0..256] of -1..lig_table_size_def; {lig/kern starting addresses}
@z

% _____________________________________________________________________________
%
% [47.1154]
% _____________________________________________________________________________

@x
var k:gf_index;
begin for k:=a to b do write(gf_file,gf_buf[k]);
@y
begin c4p_buf_write(gf_file,c4p_ptr(gf_buf[a]),b-a+1);
@z

% _____________________________________________________________________________
%
% [48.1186]
% _____________________________________________________________________________

@x
@!init procedure store_base_file;
@y
procedure store_base_file;
@z

@x
tini
@y
@z

% _____________________________________________________________________________
%
% [48.1187]
% _____________________________________________________________________________

@x
@d off_base=6666 {go here if the base file is unacceptable}
@y
@d off_base=6666 {go here if the base file is unacceptable}
@d start_loading=7777
@z

@x
label off_base,exit;
var @!k:integer; {all-purpose index}
@y
label off_base,exit,start_loading;
var @!k:integer; {all-purpose index}
allow_makebase:boolean;
@z

@x
begin @<Undump constants for consistency check@>;
@y
begin
  allow_makebase := true;
start_loading:
  @<Undump constants for consistency check@>;
@z

@x
off_base: wake_up_terminal;
@y
off_base:
  if (allow_makebase) then begin
    allow_makebase := false;
    w_close (base_file);
    { <fixme>asserting that |name_of_file| wasn't overridden</fixme> }
    if (miktex_open_base_file(base_file, true)) then goto start_loading;
  end
  wake_up_terminal;
@z

% _____________________________________________________________________________
%
% [48.1198]
% _____________________________________________________________________________

@x
dump_int(start_sym); dump_int(interaction); dump_int(base_ident);
@y
dump_int(start_sym);
if miktex_get_interaction >= 0 then dump_int(error_stop_mode)
else dump_int(interaction);
dump_int(base_ident);
@z

% _____________________________________________________________________________
%
% [49.1204]
% _____________________________________________________________________________

@x
@p begin @!{|start_here|}
@y
@p begin @!{|start_here|}
c4p_begin_try_block(final_end);
c4p_begin_try_block(end_of_MF);
miktex_on_texmf_start_job;
miktex_process_command_line_options;
miktex_allocate_memory;
@z

@x
    '---case ',bad:1);
@y
    '---case ',bad);
@z

@x
ready_already:=314159;
@y
ready_already:=314159;
miktex_on_texmf_initialize;
@z

@x
end_of_MF: close_files_and_terminate;
final_end: ready_already:=0;
@y
end_of_MF: c4p_end_try_block(end_of_MF); close_files_and_terminate;
final_end: c4p_end_try_block(final_end); ready_already:=0;
miktex_free_memory;
miktex_on_texmf_finish_job;
if (history = error_message_issued) or (history = fatal_error_stop) then
begin
  c4p_exit (1);
end;
@z

% _____________________________________________________________________________
%
% [49.1205]
% _____________________________________________________________________________

@x
    slow_print(log_name); print_char(".");
    end;
  end;
@y
    slow_print(log_name); print_char(".");
    end;
  end;
print_ln;
if (edit_name_start<>0) and (interaction>batch_mode) then begin
  if log_opened then
    miktex_invoke_editor(edit_name_start,edit_name_length,edit_line,
                         str_start[log_name], length(log_name))
  else
    miktex_invoke_editor(edit_name_start,edit_name_length,edit_line)
end;
@z

% _____________________________________________________________________________
%
% [49.1210]
% _____________________________________________________________________________

@x
@!init procedure init_prim; {initialize all the primitives}
@y
procedure init_prim; {initialize all the primitives}
@z

@x
tini
@y
@z

% _____________________________________________________________________________
%
% [50.1212] \[50] Debugging
% _____________________________________________________________________________

@x
@<Initialize the print |selector|...@>;
@y
if miktex_get_interaction >= 0 then
  interaction:=miktex_get_interaction;
@<Initialize the print |selector|...@>;
@z

% _____________________________________________________________________________
%
% [51.1214] \[51] System-dependent changes
% _____________________________________________________________________________

@x
@* \[52] Index.
@y
@ @<Declare \MiKTeX\ procedures@>=

function make_fraction(@!p,@!q:integer):fraction; forward;
function miktex_c_style_error_messages_p : boolean; forward;
function miktex_get_interaction : integer; forward;
function miktex_get_job_name : str_number; forward;
function miktex_get_quiet_flag : boolean; forward;
function miktex_halt_on_error_p : boolean; forward;
function miktex_init_screen : boolean; forward;
function miktex_is_init_program : boolean; forward;
function miktex_make_full_name_string : str_number; forward;
function take_fraction(@!p,@!q:integer):fraction; forward;
function take_scaled(@!p,@!q:integer):scaled; forward;
{function make_scaled(@!p,@!q:integer):scaled; forward;}

@ @<Error handling procedures@>=

procedure print_c_style_error_message (@!s:str_number);
  var k : integer;
      old_interaction : batch_mode..error_stop_mode;
begin
  old_interaction := interaction;
  interaction := nonstop_mode;
  @<Initialize the print |selector| based on |interaction|@>;
  if log_opened then selector:=selector+2;
  print_nl ("");
  print (full_source_filename_stack[in_open]);
  print (":");
  print_int (line);
  print (": ");
  print (s);
  interaction := old_interaction;
  @<Initialize the print |selector| based on |interaction|@>;
  if log_opened then selector:=selector+2;
end;

@ @<Global variables@>=
@!edit_name_start: pool_pointer; {where the filename to switch to starts}
@!edit_name_length,@!edit_line: integer; {what line to start editing at}
@!bistack_size : integer;
@!buf_size : integer;
@!error_line : integer;
@!half_error_line : integer;
@!lig_table_size : integer;
@!max_print_line : integer;
@!max_strings : integer;
@!mem_max : integer;
@!mem_min : integer;
@!mem_top : integer;
@!max_wiggle : integer;
@!move_size : integer;
@!param_size : integer;
@!path_size : integer;
@!pool_size : integer;
@!stack_size : integer;
@!string_vacancies : integer;

@ @<Set init...@>=
edit_name_start:=0;

@* \[52] Index.
@z
