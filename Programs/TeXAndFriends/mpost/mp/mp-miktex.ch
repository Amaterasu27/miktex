%% mp-miktex.ch: MiKTeX change file for METAPOST
%% 
%% Copyright (C) 1998-2006 Christian Schenk
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
@d start_of_MP=1 {go here when \MP's variables are initialized}
@d end_of_MP=9998 {go here to close files and terminate gracefully}
@d final_end=9999 {this label marks the ending of the program}
@y
@d start_of_MP=1 {go here when \MF's variables are initialized}
@d end_of_MP=9998 {go here to close files and terminate gracefully}
@d final_end=9999 {this label marks the ending of the program}
@d goto_fin_end==c4p_throw(final_end)
@d goto_eof_MP==c4p_throw(end_of_MP)
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
@!mem_max=30000; {greatest index in \MP's internal |mem| array;
@y
@!mem_max_def=1048576; {greatest index in \MP's internal |mem| array;
@z

@x
@!max_internal=100; {maximum number of internal quantities}
@y
@!max_internal=300; {maximum number of internal quantities}
@z

@x
@!buf_size=500; {maximum number of characters simultaneously present in
@y
@!buf_size_def=70000; {maximum number of characters simultaneously present in
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
@!stack_size=30; {maximum number of simultaneous input sources}
@y
@!stack_size_def=300; {maximum number of simultaneous input sources}
@z

@x
@!max_read_files=4; {maximum number of simultaneously open \&{readfrom} files}
@y
@!max_read_files=30; {maximum number of simultaneously open \&{readfrom} files}
@z

@x
@!max_strings=2500; {maximum number of strings; must not exceed |max_halfword|}
@y
@!max_strings_def=70000; {maximum number of strings; must not exceed |max_halfword|}
@z

@x
@!string_vacancies=9000; {the minimum number of characters that should be
@y
@!string_vacancies_def=70000; {the minimum number of characters that should be
@z

@x
@!pool_size=32000; {maximum number of characters in strings, including all
@y
@!pool_size_def=100000; {maximum number of characters in strings, including all
@z

@x
@!font_max=50; {maximum font number for included text fonts}
@y
@!font_max=200; {maximum font number for included text fonts}
@z

@x
@!font_mem_size=10000; {number of words for \.{TFM} information for text fonts}
@y
@!font_mem_size_def=100000; {number of words for \.{TFM} information for text fonts}
@z

@x
@!file_name_size=40; {file names shouldn't be longer than this}
@y
@!file_name_size=259; {file names shouldn't be longer than this}
@!file_name_size_plus_one=260; {one more for the string terminator}
@z

@x
@!ps_tab_name='MPlib:PSFONTS.MAP                     ';
  {string of length |file_name_size|; locates font name translation table}
@y
@!ps_tab_name='psfonts.map';
  {locates font name translation table}
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
@!max_font_dimen=50; {maximum number of \&{fontdimen} parameters}
@z

% _____________________________________________________________________________
%
% [1.11]
% _____________________________________________________________________________

@x
@d mem_min=0 {smallest index in the |mem| array, must not be less
@y
@d mem_min_def=0 {smallest index in the |mem| array, must not be less
@z

@x
@d mem_top==30000 {largest index in the |mem| array dumped by \.{INIMP};
@y
@d mem_top_def==1048576 {largest index in the |mem| array dumped by \.{INIMP};
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

@x
@d max_write_files=4 {maximum number of simultaneously open \&{write} files}
@y
@d max_write_files=30 {maximum number of simultaneously open \&{write} files}
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
begin reset(f,name_of_file,'/O'); b_open_in:=reset_OK(f);
@y
begin b_open_in:=false
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

% _____________________________________________________________________________
%
% [3.27]
% _____________________________________________________________________________

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
% [4.62]
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
% [4.64]
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
% [4.65]
% _____________________________________________________________________________

@x
@!init @!pool_file:alpha_file; {the string-pool file output by \.{TANGLE}}
tini
@y
@!pool_file:alpha_file; {the string-pool file output by \.{TANGLE}}
@z

% _____________________________________________________________________________
%
% [4.66]
% _____________________________________________________________________________

@x
name_of_file:=pool_name; {we needn't set |name_length|}
if a_open_in(pool_file) then
@y
miktex_get_pool_file_name(name_of_file);
if miktex_open_pool_file(pool_file) then
@z

@x
else  bad_pool('! I can''t read MP.POOL.')
@.I can't read MP.POOL@>
@y
else begin
  wake_up_terminal;
  write_ln(term_out, '! I can''t read MF.POOL.');
@.I can't read MP.POOL@>
  get_strings_started:=false;
  return;
end
@z

% _____________________________________________________________________________
%
% [5.69] \[5] On-line and off-line printing
% _____________________________________________________________________________

@x
@!term_offset : 0..max_print_line;
  {the number of characters on the current terminal line}
@!file_offset : 0..max_print_line;
  {the number of characters on the current file line}
@y
@!term_offset : 0..max_print_line_def;
  {the number of characters on the current terminal line}
@!file_offset : 0..max_print_line_def;
  {the number of characters on the current file line}
@z

@x
@!trick_buf:array[0..error_line] of ASCII_code; {circular buffer for
@y
@!trick_buf:array[0..error_line_def] of ASCII_code; {circular buffer for
@z

% _____________________________________________________________________________
%
% [5.71]
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
% [5.81]
% _____________________________________________________________________________

@x
var @!k:0..buf_size; {index into |buffer|}
@y
var @!k:0..buf_size_def; {index into |buffer|}
@z

% _____________________________________________________________________________
%
% [6.83]
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
% [6.84]
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
% [6.91]
% _____________________________________________________________________________

@x
begin goto end_of_MP;
@y
begin goto_eof_MP;
@z

% _____________________________________________________________________________
%
% [6.92]
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
% [6.94]
% _____________________________________________________________________________

@x
  begin print_nl("You want to edit file ");
@.You want to edit file x@>
  print(input_stack[file_ptr].name_field);
  print(" at line "); print_int(true_line);@/
@y
  begin miktex_remember_edit_info (input_stack[file_ptr].name_field, true_line);
@z

% _____________________________________________________________________________
%
% [7.111]
% _____________________________________________________________________________

 @x
@d halfp(#)==(#) div 2
 @y
@d halfp(#)==miktex_halfp(#)
 @z

% _____________________________________________________________________________
%
% [7.122]
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
% [7.124]
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
% [7.127]
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
% [7.129]
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
% [9.168] \[9] Packed data
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
% [10.175]
% _____________________________________________________________________________

@x
@!mem : array[mem_min..mem_max] of memory_word; {the big dynamic storage area}
@y
@!mem : array[mem_min_def..mem_max_def] of memory_word; {the big dynamic storage area}
@z

% _____________________________________________________________________________
%
% [10.188]
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
% [11.193]
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
% [12.212]
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
% [12.217]
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
% [13.228]
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
% [18.301]
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
% [18.302]
% _____________________________________________________________________________

@x
@!k,@!n:0..path_size; {current and final knot numbers}
@y
@!k,@!n:0..path_size_def; {current and final knot numbers}
@z

% _____________________________________________________________________________
%
% [18.305]
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
% [18.306]
% _____________________________________________________________________________

@x
var @!k:0..path_size; {current knot number}
@y
var @!k:0..path_size_def; {current knot number}
@z

% _____________________________________________________________________________
%
% [23.528]
% _____________________________________________________________________________

@x
bisect_stack:array[0..bistack_size] of integer;
bisect_ptr:0..bistack_size;
@y
bisect_stack:array[0..bistack_size_def] of integer;
bisect_ptr:0..bistack_size_def;
@z

% _____________________________________________________________________________
%
% [23.533]
% _____________________________________________________________________________

@x
@!uv,@!xy:0..bistack_size; {pointers to the current packets of interest}
@y
@!uv,@!xy:0..bistack_size_def; {pointers to the current packets of interest}
@z

% _____________________________________________________________________________
%
% [27.583]
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
% [27.586]
% _____________________________________________________________________________

@x
@!line_stack : array[0..max_in_open] of integer; {the line number for each file}
@y
@!line_stack : array[0..max_in_open] of integer; {the line number for each file}
@!full_source_filename_stack : array[0..max_in_open] of str_number;
@z

% _____________________________________________________________________________
%
% [27.588]
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
% [27.589]
% _____________________________________________________________________________

@x
var @!k:0..stack_size; {an index into the input stack}
@y
var @!k:0..stack_size_def; {an index into the input stack}
@z

% _____________________________________________________________________________
%
% [27.590]
% _____________________________________________________________________________

@x
@!file_ptr:0..stack_size; {shallowest level shown by |show_context|}
@y
@!file_ptr:0..stack_size_def; {shallowest level shown by |show_context|}
@z

% _____________________________________________________________________________
%
% [27.597]
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
% [29.628]
% _____________________________________________________________________________

@x
var @!k:0..buf_size; {an index into |buffer|}
@y
var @!k:0..buf_size_def; {an index into |buffer|}
@z

% _____________________________________________________________________________
%
% [29.646]
% _____________________________________________________________________________

@x
var @!k:0..buf_size; {an index into |buffer|}
@y
var @!k:0..buf_size_def; {an index into |buffer|}
@z

% _____________________________________________________________________________
%
% [31.670]
% _____________________________________________________________________________

@x
@!k:0..param_size; {the total number of parameters}
@y
@!k:0..param_size_def; {the total number of parameters}
@z

% _____________________________________________________________________________
%
% [35.747]
% _____________________________________________________________________________

@x
@d MP_area=="MPinputs:"
@.MPinputs@>
@d MF_area=="MFinputs:"
@.MFinputs@>
@d MP_font_area=="TeXfonts:"
@.TeXfonts@>
@y
@d MP_area==""
@.MPinputs@>
@d MF_area==""
@.MFinputs@>
@d MP_font_area==""
@.TeXfonts@>
@z

% _____________________________________________________________________________
%
% [35.749]
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
% [35.752]
% _____________________________________________________________________________

@x
for k:=name_length+1 to file_name_size do name_of_file[k]:=' ';
@y
name_of_file[ name_length + 1 ]:= chr(0);
@z

%% **   753  ******************************************************************

@x
MP_mem_default:='MPlib:plain.mem';
@y
miktex_get_default_dump_file_name(MP_mem_default);
@z

% _____________________________________________________________________________
%
% [35.754]
% _____________________________________________________________________________

@x
if mem_default_length>file_name_size then bad:=20;
@y
do_nothing;
@z

% _____________________________________________________________________________
%
% [35.756]
% _____________________________________________________________________________

@x
@p procedure pack_buffered_name(@!n:small_number;@!a,@!b:integer);
var @!k:integer; {number of positions filled in |name_of_file|}
@!c: ASCII_code; {character being packed}
@!j:integer; {index into |buffer| or |MP_mem_default|}
begin if n+b-a+1+mem_ext_length>file_name_size then
  b:=a+file_name_size-n-1-mem_ext_length;
k:=0;
for j:=1 to n do append_to_name(xord[MP_mem_default[j]]);
for j:=a to b do append_to_name(buffer[j]);
for j:=mem_default_length-mem_ext_length+1 to mem_default_length do
  append_to_name(xord[MP_mem_default[j]]);
if k<=file_name_size then name_length:=k@+else name_length:=file_name_size;
for k:=name_length+1 to file_name_size do name_of_file[k]:=' ';
end;
@y
@z

% _____________________________________________________________________________
%
% [35.757]
% _____________________________________________________________________________

@x
var @!j:0..buf_size; {the first space after the file name}
@y
var @!j:0..buf_size_def; {the first space after the file name}
@z

@x
  pack_buffered_name(0,loc,j-1); {try first without the system file area}
  if w_open_in(mem_file) then goto found;
  pack_buffered_name(mem_area_length,loc,j-1);
    {now try the system mem file area}
  if w_open_in(mem_file) then goto found;
@y
  c4p_memcpy(name_of_file,file_name_size,c4p_ptr(buffer[loc]),j-loc);
  name_of_file[j-loc+1]:=chr(0);
  if miktex_open_mem_file(mem_file) then goto found;
@z

@x
  wterm_ln('Sorry, I can''t find that mem file;',' will try PLAIN.');
@y
  wterm_ln('Sorry, I can''t find that mem file;',' will try default.');
@z

@x
pack_buffered_name(mem_default_length-mem_ext_length,1,0);
if not w_open_in(mem_file) then
@y
c4p_strcpy(name_of_file,file_name_size_plus_one,MP_mem_default);
if not miktex_open_mem_file(mem_file) then
@z

@x
  wterm_ln('I can''t find the PLAIN mem file!');
@y
  wterm_ln('I can''t find the default mem file!');
@z

% _____________________________________________________________________________
%
% [35.764]
% _____________________________________________________________________________

@x
var @!k:0..buf_size; {index into |buffer|}
@y
var @!k:0..buf_size_def; {index into |buffer|}
@z

% _____________________________________________________________________________
%
% [35.766]
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
% [35.768]
% _____________________________________________________________________________

@x
print(mem_ident); print("  ");
print_int(round_unscaled(internal[day])); print_char(" ");
months:='JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC';
@y
miktex_print_miktex_banner(log_file);
print(mem_ident); print("  ");
print_int(round_unscaled(internal[day])); print_char(" ");
c4p_arrcpy(months,'JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC');
@z

% _____________________________________________________________________________
%
% [35.771]
% _____________________________________________________________________________

@x
done: name:=a_make_name_string(cur_file);
@<Update the string reference counts for |in_name| and |in_area|@>;
if job_name=0 then
  begin job_name:=cur_name; str_ref[job_name]:=max_str_ref;
@y
done: name:=a_make_name_string(cur_file);
full_source_filename_stack[in_open]:=miktex_make_full_name_string;
@<Update the string reference counts for |in_name| and |in_area|@>;
if job_name=0 then
  begin
    job_name:=miktex_get_job_name;
    if job_name=0 then job_name:=cur_name;
    str_ref[job_name]:=max_str_ref;
@z

@x
if term_offset+length(name)>max_print_line-2 then print_ln
else if (term_offset>0)or(file_offset>0) then print_char(" ");
print_char("("); incr(open_parens); print(name); update_terminal;
@y
if term_offset+length(full_source_filename_stack[in_open])>max_print_line-2 then print_ln
else if (term_offset>0)or(file_offset>0) then print_char(" ");
print_char("("); incr(open_parens); print(full_source_filename_stack[in_open]); update_terminal;
@z

% _____________________________________________________________________________
%
% [35.772]
% _____________________________________________________________________________

@x
flush_string(name); name:=cur_name; cur_name:=0
@y
@z

% _____________________________________________________________________________
%
% [35.775]
% _____________________________________________________________________________

@x
for k:=old_name_length+1 to file_name_size do @+old_file_name[k]:=' ';
@y
for k:=old_name_length+1 to file_name_size do @+old_file_name[k]:=chr(0);
@z

% _____________________________________________________________________________
%
% [35.777]
% _____________________________________________________________________________

@x
name:=a_make_name_string(cur_file);
@y
name:=a_make_name_string(cur_file);
full_source_filename_stack[in_open]:=miktex_make_full_name_string;
@z

% _____________________________________________________________________________
%
% [35.778]
% _____________________________________________________________________________

@x
copy_old_name(name)
{System-dependent code should be added here}
@y
copy_old_name(name);
miktex_run_makempx (old_file_name,name_of_file);
@z

% _____________________________________________________________________________
%
% [36.796]
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
% [41.1067]
% _____________________________________________________________________________

@x
@!n:0..max_print_line; {amount of macro text to show}
@y
@!n:0..max_print_line_def; {amount of macro text to show}
@z

% _____________________________________________________________________________
%
% [41.1090]
% _____________________________________________________________________________

%% <fixme>variable add_type isn't set in:
%%   @<Abandon edges command because there's no variable@>
%% this causes a debug assertion if /RTC1 is specified</fixme>

@x
get_x_next; var_flag:=sep; scan_primary;
@y
add_type:=0;
get_x_next; var_flag:=sep; scan_primary;
@z

% _____________________________________________________________________________
%
% [42.1131]
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
% [43.1179]
% _____________________________________________________________________________

@x
font_info:array[0..font_mem_size] of memory_word;
  {height, width, and depth data}
next_fmem:0..font_mem_size; {next unused entry in |font_info|}
@y
font_info:array[0..font_mem_size_def] of memory_word;
  {height, width, and depth data}
next_fmem:0..font_mem_size_def; {next unused entry in |font_info|}
@z

% _____________________________________________________________________________
%
% [43.1180]
% _____________________________________________________________________________

@x
char_base:array[font_number] of 0..font_mem_size;
  {base address for |char_info|}
width_base:array[font_number] of 0..font_mem_size;
  {index for zeroth character width}
height_base:array[font_number] of 0..font_mem_size;
  {index for zeroth character height}
depth_base:array[font_number] of 0..font_mem_size;
@y
char_base:array[font_number] of 0..font_mem_size_def;
  {base address for |char_info|}
width_base:array[font_number] of 0..font_mem_size_def;
  {index for zeroth character width}
height_base:array[font_number] of 0..font_mem_size_def;
  {index for zeroth character height}
depth_base:array[font_number] of 0..font_mem_size_def;
@z

% _____________________________________________________________________________
%
% [43.1183]
% _____________________________________________________________________________

@x
@!i,@!ii:0..font_mem_size; {|font_info| indices}
@!jj:0..font_mem_size; {counts bytes to be ignored}
@y
@!i,@!ii:0..font_mem_size_def; {|font_info| indices}
@!jj:0..font_mem_size_def; {counts bytes to be ignored}
@z

% _____________________________________________________________________________
%
% [43.1192]
% _____________________________________________________________________________

@x
@ @<Open |tfm_infile| for input@>=
file_opened:=false;
str_scan_file(fname);
if cur_area="" then cur_area:=MP_font_area;
if cur_ext="" then cur_ext:=".tfm";
pack_cur_name;
if not b_open_in(tfm_infile) then goto bad_tfm;
file_opened:=true
@y
@ @<Open |tfm_infile| for input@>=
file_opened:=false;
str_scan_file(fname);
if cur_area="" then cur_area:=MP_font_area;
if cur_ext="" then cur_ext:=".tfm";
pack_cur_name;
file_opened:=miktex_open_tfm_file(tfm_infile, name_of_file);
if not file_opened then goto bad_tfm;
@z

% _____________________________________________________________________________
%
% [43.1199]
% _____________________________________________________________________________

@x
begin name_of_file:=ps_tab_name;
@y
begin c4p_strcpy(name_of_file, file_name_size_plus_one, ps_tab_name);
@z

@x
if a_open_in(ps_tab_file) then
@y
if miktexopenpstabfile(ps_tab_file, name_of_file) then
@z

% _____________________________________________________________________________
%
% [44.1246]
% _____________________________________________________________________________

@x
var @!k:0..font_mem_size; {an index into |font_info|}
@y
var @!k:0..font_mem_size_def; {an index into |font_info|}
@z

% _____________________________________________________________________________
%
% [44.1249]
% _____________________________________________________________________________

@x
@!p:0..font_mem_size; {|font_info| index for the current character}
@y
@!p:0..font_mem_size_def; {|font_info| index for the current character}
@z

% _____________________________________________________________________________
%
% [44.1253]
% _____________________________________________________________________________

@x
var @!p:0..font_mem_size; {|font_info| index for the current character}
@y
var @!p:0..font_mem_size_def; {|font_info| index for the current character}
@z

% _____________________________________________________________________________
%
% [45.1284]
% _____________________________________________________________________________

@x
@!init procedure store_mem_file;
@y
procedure store_mem_file;
@z

@x
tini
@y
@z

% _____________________________________________________________________________
%
% [45.1285]
% _____________________________________________________________________________

@x
@d off_base=6666 {go here if the mem file is unacceptable}
@y
@d off_base=6666 {go here if the mem file is unacceptable}
@d start_loading=7777
@z

@x
label done,off_base,exit;
var @!k:integer; {all-purpose index}
@y
label done,off_base,exit,start_loading;
var @!k:integer; {all-purpose index}
allow_makemem:boolean;
@z

@x
begin @<Undump constants for consistency check@>;
@y
begin
  allow_makemem := true;
start_loading:
  @<Undump constants for consistency check@>;
@z

@x
off_base: wake_up_terminal;
@y
off_base:
  if (allow_makemem) then begin
    allow_makemem := false;
    w_close (mem_file);
    { <fixme>asserting that |name_of_file| wasn't overridden</fixme> }
    if (miktex_open_mem_file(mem_file, true)) then goto start_loading;
  end
  wake_up_terminal;
@z

% _____________________________________________________________________________
%
% [45.1298]
% _____________________________________________________________________________

@x
print_int(round_unscaled(internal[year]) mod 100); print_char(".");
@y
print_int(round_unscaled(internal[year])); print_char(".");
@z

% _____________________________________________________________________________
%
% [46.1302]
% _____________________________________________________________________________

@x
@p begin @!{|start_here|}
@y
@p begin @!{|start_here|}
c4p_begin_try_block(final_end);
c4p_begin_try_block(end_of_MP);
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
end_of_MP: close_files_and_terminate;
final_end: ready_already:=0;
@y
end_of_MP: c4p_end_try_block(end_of_MP); close_files_and_terminate;
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
% [46.1303]
% _____________________________________________________________________________

@x
    print(log_name); print_char(".");
    end;
  end;
@y
    print(log_name); print_char(".");
    end;
  end;
print_ln;
miktex_invoke_editor_if_necessary;
@z

% _____________________________________________________________________________
%
% [46.1309]
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
% [46.1310]
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
% [49.1315] \[49] Index
% _____________________________________________________________________________

@x
@* \[49] Index.
@y
@ @<Declare \MiKTeX\ procedures@>=

function make_fraction(@!p,@!q:integer):fraction; forward;
function miktex_c_style_error_messages_p : boolean; forward;
function miktex_get_interaction : integer; forward;
function miktex_get_job_name : str_number; forward;
function miktex_halt_on_error_p : boolean; forward;
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
@!bistack_size : integer;
@!buf_size : integer;
@!error_line : integer;
@!font_mem_size : integer;
@!half_error_line : integer;
@!lig_table_size : integer;
@!max_print_line : integer;
@!max_strings : integer;
@!mem_max : integer;
@!mem_min : integer;
@!mem_top : integer;
@!param_size : integer;
@!path_size : integer;
@!pool_size : integer;
@!stack_size : integer;
@!string_vacancies : integer;

@* \[49] Index.
@z
