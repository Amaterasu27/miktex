%% mp-miktex.ch: MiKTeX change file for METAPOST
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

%% ////////////////////////////////////////////////////////////////////////////
%% //                                                                        //
%% //                       INSPIRED BY WEB2C'S MF.CH                        //
%% //                                                                        //
%% ////////////////////////////////////////////////////////////////////////////

@x
\def\PASCAL{Pascal}
@y
\def\PASCAL{Pascal}
\def\C4P{C4P}
\def\Cplusplus{C++}
\def\MiKTeX{MiK\TeX}
@z

% WEAVE: print changes only.

@x
  \def\?##1]{\hbox to 1in{\hfil##1.\ }}
  }
@y 83
  \def\?##1]{\hbox{Changes to \hbox to 1em{\hfil##1}.\ }}
  }
\let\maybe=\iffalse
@z

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
the codewords `$|init|\ldots|tini|$'.

@d init== {change this to `$\\{init}\equiv\.{@@\{}$' in the production version}
@d tini== {change this to `$\\{tini}\equiv\.{@@\}}$' in the production version}
@y
the codewords `$|init|\ldots|tini|$' for declarations and by the codewords
`$|Init|\ldots|Tini|$' for executable code.  This distinction is helpful for
implementations where a run-time switch differentiates between the two
versions of the program.

@d init=={ifdef('INIMF')}
@d tini=={endif('INIMF')}
@d Init==init if miktex_is_init_program then begin
@d Tini==end;@+tini
@f Init==begin
@f Tini==end
@z

% _____________________________________________________________________________
%
% [1.11]
% _____________________________________________________________________________

@x
@!mem_max=30000; {greatest index in \MP's internal |mem| array;
  must be strictly less than |max_halfword|;
  must be equal to |mem_top| in \.{INIMP}, otherwise |>=mem_top|}
@y
@z

@x
@!max_internal=100; {maximum number of internal quantities}
@y
@!max_internal=300; {maximum number of internal quantities}
@z

@x
@!buf_size=500; {maximum number of characters simultaneously present in
  current lines of open files; must not exceed |max_halfword|}
@y
@!inf_buf_size=500;
@!sup_buf_size=30000000;
@z

@x
@!error_line=72; {width of context lines on terminal error messages}
@y
@!inf_error_line=40;
@!sup_error_line=65535;
@z

@x
@!half_error_line=42; {width of first lines of contexts in terminal
  error messages; should be between 30 and |error_line-15|}
@y
@!inf_half_error_line=30;
@!sup_half_error_line=65535;
@z

@x
@!max_print_line=79; {width of longest text lines output; should be at least 60}
@y
@!inf_max_print_line=40;
@!sup_max_print_line=65535;
@z

@x
@!stack_size=30; {maximum number of simultaneous input sources}
@y
@!inf_stack_size=30;
@!sup_stack_size=65535;
@z

@x
@!max_read_files=4; {maximum number of simultaneously open \&{readfrom} files}
@y
@!max_read_files=30; {maximum number of simultaneously open \&{readfrom} files}
@z

@x
@!max_strings=2500; {maximum number of strings; must not exceed |max_halfword|}
@y
@!inf_max_strings=2500;
@!sup_max_strings=300000;
@z

@x
@!string_vacancies=9000; {the minimum number of characters that should be
  available for the user's identifier names and strings,
  after \MP's own error messages are stored}
@y
@!inf_string_vacancies=9000;
@!sup_string_vacancies=40000000;
@z

@x
@!pool_size=32000; {maximum number of characters in strings, including all
  error messages and help texts, and the names of all identifiers;
  must exceed |string_vacancies| by the total
  length of \MP's own strings, which is currently about 22000}
@y
@!inf_pool_size=32000;
@!sup_pool_size=40000000;
@!inf_pool_free=1000;
@!sup_pool_free=40000000;
@z

@x
@!font_max=50; {maximum font number for included text fonts}
@y
@!font_max=200; {maximum font number for included text fonts}
@z

@x
@!font_mem_size=10000; {number of words for \.{TFM} information for text fonts}
@y
@!inf_font_mem_size=10000;
@!sup_font_mem_size=300000;
@z

@x
@!file_name_size=40; {file names shouldn't be longer than this}
@y
@!file_name_size=259; {file names shouldn't be longer than this}
@!file_name_size_plus_one=260; {one more}
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
@!inf_path_size=300;
@!sup_path_size=300000;
@z

@x
@!bistack_size=785; {size of stack for bisection algorithms;
  should probably be left at this value}
@y
@!inf_bistack_size=785;
@!sup_bistack_size=300000;
@z

@x
@!header_size=100; {maximum number of \.{TFM} header words, times~4}
@y
@!header_size=100; {maximum number of \.{TFM} header words, times~4}
@z

@x
@!lig_table_size=5000; {maximum number of ligature/kern steps, must be
  at least 255 and at most 32510}
@y
@!inf_lig_table_size=5000;
@!sup_lig_table_size=32510;
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
% [1.11]
% _____________________________________________________________________________

@x
@d mem_top==30000 {largest index in the |mem| array dumped by \.{INIMP};
  must be substantially larger than |mem_min|
  and not greater than |mem_max|}
@y
@z

@x
@d hash_size=2100 {maximum number of symbolic tokens,
  must be less than |max_halfword-3*param_size|}
@y
@d hash_size=9500 {maximum number of symbolic tokens,
  must be less than |max_halfword-3*param_size|}
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
@z

@x
@d max_write_files=4 {maximum number of simultaneously open \&{write} files}
@y
@d max_write_files=30 {maximum number of simultaneously open \&{write} files}
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
@^character set dependencies@>
@^system dependencies@>

@<Set init...@>=
for i:=0 to @'37 do xchr[i]:=' ';
for i:=@'177 to @'377 do xchr[i]:=' ';
@y
@^character set dependencies@>
@^system dependencies@>

@d tab = @'11 { ASCII horizontal tab }
@d form_feed = @'14 { ASCII form feed }

@<Set init...@>=
{Initialize |xchr| to the identity mapping.}
for i:=0 to @'37 do xchr[i]:=i;
for i:=@'177 to @'377 do xchr[i]:=i;
@z

% _____________________________________________________________________________
%
% [2.23]
% _____________________________________________________________________________

@x
for i:=first_text_char to last_text_char do xord[chr(i)]:=@'177;
for i:=@'200 to @'377 do xord[xchr[i]]:=i;
for i:=0 to @'176 do xord[xchr[i]]:=i;
@y
miktex_initialize_char_tables;
@z

% _____________________________________________________________________________
%
% [3.25]
% _____________________________________________________________________________

@x
|name_of_file|.
@^system dependencies@>
@y
|name_of_file|.
@^system dependencies@>

\MiKTeX: reserve an extra char slot for the nul char.
@z

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
|name_of_file| could be opened.
@y
|name_of_file| could be opened.

\MiKTeX: we use our own functions to open files.
@z

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
@!buffer:array[0..sup_buf_size] of ASCII_code; {lines of characters being read}
@!first:0..sup_buf_size; {the first unused position in |buffer|}
@!last:0..sup_buf_size; {end of the line just input to |buffer|}
@!max_buf_stack:0..sup_buf_size; {largest index used in |buffer|}
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
% [4.37]
% _____________________________________________________________________________

@x
@!pool_pointer = 0..pool_size; {for variables that point into |str_pool|}
@!str_number = 0..max_strings; {for variables that point into |str_start|}
@y
@!pool_pointer = 0..sup_pool_size; {for variables that point into |str_pool|}
@!str_number = 0..sup_max_strings; {for variables that point into |str_start|}
@z

% _____________________________________________________________________________
%
% [5.68]
% _____________________________________________________________________________

@x
@!term_offset : 0..max_print_line;
  {the number of characters on the current terminal line}
@!file_offset : 0..max_print_line;
  {the number of characters on the current file line}
@y
@!term_offset : 0..sup_max_print_line;
  {the number of characters on the current terminal line}
@!file_offset : 0..sup_max_print_line;
  {the number of characters on the current file line}
@z

@x
@!trick_buf:array[0..error_line] of ASCII_code; {circular buffer for
@y
@!trick_buf:array[0..sup_error_line] of ASCII_code; {circular buffer for
@z

% _____________________________________________________________________________
%
% [5.80]
% _____________________________________________________________________________

@x
var @!k:0..buf_size; {index into |buffer|}
@y
var @!k:0..sup_buf_size; {index into |buffer|}
@z

% _____________________________________________________________________________
%
% [6.82]
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
% [6.83]
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
% [6.90]
% _____________________________________________________________________________

@x
begin goto end_of_MP;
@y
begin goto_eof_MP;
@z

% _____________________________________________________________________________
%
% [6.91]
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
% [6.93]
% _____________________________________________________________________________

@x
line ready to be edited. But such an extension requires some system
wizardry, so the present implementation simply types out the name of the
file that should be
edited and the relevant line number.
@^system dependencies@>

There is a secret `\.D' option available when the debugging routines haven't
been commented~out.
@^debugging@>
@y
line ready to be edited.
We do this by calling the external procedure |call_edit| with a pointer to
the filename, its length, and the line number.
However, here we just set up the variables that will be used as arguments,
since we don't want to do the switch-to-editor until after \MF\ has closed
its files.
@^system dependencies@>

There is a secret `\.D' option available when the debugging routines have
not been commented out.
@^debugging@>
@d edit_file==input_stack[file_ptr]
@z

@x
"E": if file_ptr>0 then
  begin print_nl("You want to edit file ");
@.You want to edit file x@>
  print(input_stack[file_ptr].name_field);
  print(" at line "); print_int(true_line);@/
  interaction:=scroll_mode; jump_out;
@y
"E": if file_ptr>0 then
    begin
    edit_name_start:=str_start[edit_file.name_field];
    edit_name_length:=str_start[edit_file.name_field+1] -
    		      str_start[edit_file.name_field];
    edit_line:=true_line;
    jump_out;
@z

% _____________________________________________________________________________
%
% [7.110]
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
% [7.123]
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
% [7.126]
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
% [7.128]
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
% [9.167]
% _____________________________________________________________________________

@x
@d min_quarterword=0 {smallest allowable value in a |quarterword|}
@d max_quarterword=255 {largest allowable value in a |quarterword|}
@d min_halfword==0 {smallest allowable value in a |halfword|}
@d max_halfword==65535 {largest allowable value in a |halfword|}
@y
@d min_quarterword=0 {smallest allowable value in a |quarterword|}
@d max_quarterword=255 {largest allowable value in a |quarterword|}
@d min_halfword==0 {smallest allowable value in a |halfword|}
@d max_halfword==@"FFFFFFF {largest allowable value in a |halfword|}
@z

% _____________________________________________________________________________
%
% [10.173]
% _____________________________________________________________________________

@x
@!mem : array[mem_min..mem_max] of memory_word; {the big dynamic storage area}
@y
@!mem : ^memory_word; {the big dynamic storage area}
@z

% _____________________________________________________________________________
%
% [11.192]
% _____________________________________________________________________________

@x
@!debug @!free: packed array [mem_min..mem_max] of boolean; {free cells}
@t\hskip1em@>@!was_free: packed array [mem_min..mem_max] of boolean;
@y
@!debug @!free: packed array [0..1] of boolean; {free cells}
@t\hskip1em@>@!was_free: packed array [0..1] of boolean;
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
char_class[tab]:=space_class;
char_class[form_feed]:=space_class;
@z

% _____________________________________________________________________________
%
% [18.301]
% _____________________________________________________________________________

@x
@!delta_x,@!delta_y,@!delta:array[0..path_size] of scaled; {knot differences}
@!psi:array[1..path_size] of angle; {turning angles}
@y
@!delta_x:array[0..sup_path_size] of scaled; {knot differences}
@!delta_y:array[0..sup_path_size] of scaled; {knot differences}
@!delta:array[0..sup_path_size] of scaled; {knot differences}
@!psi:array[1..sup_path_size] of angle; {turning angles}
@z

% _____________________________________________________________________________
%
% [18.302]
% _____________________________________________________________________________

@x
@!k,@!n:0..path_size; {current and final knot numbers}
@y
@!k,@!n:0..sup_path_size; {current and final knot numbers}
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
@!theta:array[0..sup_path_size] of angle; {values of $\theta_k$}
@!uu:array[0..sup_path_size] of fraction; {values of $u_k$}
@!vv:array[0..sup_path_size] of angle; {values of $v_k$}
@!ww:array[0..sup_path_size] of fraction; {values of $w_k$}
@z

% _____________________________________________________________________________
%
% [18.306]
% _____________________________________________________________________________

@x
var @!k:0..path_size; {current knot number}
@y
var @!k:0..sup_path_size; {current knot number}
@z

% _____________________________________________________________________________
%
% [23.528]
% _____________________________________________________________________________

@x
bisect_stack:array[0..bistack_size] of integer;
bisect_ptr:0..bistack_size;
@y
bisect_stack:array[0..sup_bistack_size] of integer;
bisect_ptr:0..sup_bistack_size;
@z

% _____________________________________________________________________________
%
% [23.533]
% _____________________________________________________________________________

@x
@!uv,@!xy:0..bistack_size; {pointers to the current packets of interest}
@y
@!uv,@!xy:0..sup_bistack_size; {pointers to the current packets of interest}
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
@!input_stack : array[0..sup_stack_size] of in_state_record;
@!input_ptr : 0..sup_stack_size; {first unused location of |input_stack|}
@!max_in_stack: 0..sup_stack_size; {largest value of |input_ptr| when pushing}
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
@!param_stack:array [0..0] of pointer;
  {token list pointers for parameters}
@!param_ptr:integer; {first unused entry in |param_stack|}
@z

% _____________________________________________________________________________
%
% [27.589]
% _____________________________________________________________________________

@x
var @!k:0..stack_size; {an index into the input stack}
@y
var @!k:0..sup_stack_size; {an index into the input stack}
@z

% _____________________________________________________________________________
%
% [27.590]
% _____________________________________________________________________________

@x
@!file_ptr:0..stack_size; {shallowest level shown by |show_context|}
@y
@!file_ptr:0..sup_stack_size; {shallowest level shown by |show_context|}
@z

% _____________________________________________________________________________
%
% [27.597]
% _____________________________________________________________________________

@x
@!i:0..buf_size; {index into |buffer|}
@y
@!i:0..sup_buf_size; {index into |buffer|}
@z

@x
@!n:0..error_line; {length of line 1}
@y
@!n:0..sup_error_line; {length of line 1}
@z

% _____________________________________________________________________________
%
% [29.628]
% _____________________________________________________________________________

@x
var @!k:0..buf_size; {an index into |buffer|}
@y
var @!k:0..sup_buf_size; {an index into |buffer|}
@z

% _____________________________________________________________________________
%
% [29.646]
% _____________________________________________________________________________

@x
var @!k:0..buf_size; {an index into |buffer|}
@y
var @!k:0..sup_buf_size; {an index into |buffer|}
@z

% _____________________________________________________________________________
%
% [31.670]
% _____________________________________________________________________________

@x
@!k:0..param_size; {the total number of parameters}
@y
@!k:integer; {the total number of parameters}
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
In C, the default paths are specified separately.
@z

% _____________________________________________________________________________
%
% [35.748]
% _____________________________________________________________________________

@x
area_delimiter:=-1; ext_delimiter:=-1;
@y
area_delimiter:=-1; ext_delimiter:=-1; quoted_filename:=false;
@z

% _____________________________________________________________________________
%
% [35.749]
% _____________________________________________________________________________

@x
begin if c=" " then more_name:=false
else  begin if (c=">")or(c=":") then
@y
begin
if c="""" then begin
  quoted_filename:=not quoted_filename;
  more_name:=true;
  end
else if ((c=" ")or(c=tab)) and stop_at_space and (not quoted_filename) then
  more_name:=false
else begin
  if c="/" then
@z

@x
  else if (c=".")and(ext_delimiter<0) then
@y
  else if (c=".") then
@z

% _____________________________________________________________________________
%
% [35.750]
% _____________________________________________________________________________

@x
procedure end_name;
var a,@!n,@!e:pool_pointer; {length of area, name, and extension}
begin e:=pool_ptr-str_start[str_ptr]; {total length}
@y
procedure end_name;
var a,@!n,@!e:pool_pointer; {length of area, name, and extension}
@!j,@!s,@!t:pool_pointer; {running indices}
@!must_quote:boolean; {whether we need to quote a string}
begin str_room(6); {room for quotes}
if area_delimiter<>-1 then begin
  {maybe quote |cur_area|}
  must_quote:=false;
  s:=str_start[str_ptr];
  t:=str_start[str_ptr]+area_delimiter+1;
  j:=s;
  while (not must_quote) and (j<>t) do begin
    must_quote:=str_pool[j]=" "; incr(j);
    end;
  if must_quote then begin
    for j:=pool_ptr-1 downto t do str_pool[j+2]:=str_pool[j];
    str_pool[t+1]:="""";
    for j:=t-1 downto s do str_pool[j+1]:=str_pool[j];
    str_pool[s]:="""";
    if ext_delimiter<>-1 then ext_delimiter:=ext_delimiter+2;
    area_delimiter:=area_delimiter+2;
    pool_ptr:=pool_ptr+2;
    end;
  s:=str_start[str_ptr]+area_delimiter+1;
  end
else begin
  s:=str_start[str_ptr];
  end;
{maybe quote |cur_name|}
if ext_delimiter=-1 then t:=pool_ptr else t:=str_start[str_ptr]+ext_delimiter;
must_quote:=false;
j:=s;
while (not must_quote) and (j<>t) do begin
  must_quote:=str_pool[j]=" "; incr(j);
  end;
if must_quote then begin
  for j:=pool_ptr-1 downto t do str_pool[j+2]:=str_pool[j];
  str_pool[t+1]:="""";
  for j:=t-1 downto s do str_pool[j+1]:=str_pool[j];
  str_pool[s]:="""";
  if ext_delimiter<>-1 then ext_delimiter:=ext_delimiter+2;
  pool_ptr:=pool_ptr+2;
  end;
if ext_delimiter<>-1 then begin
  {maybe quote |cur_ext|}
  s:=str_start[str_ptr]+ext_delimiter;
  t:=pool_ptr;
  must_quote:=false;
  j:=s;
  while (not must_quote) and (j<>t) do begin
    must_quote:=str_pool[j]=" "; incr(j);
    end;
  if must_quote then begin
    str_pool[t+1]:="""";
    for j:=t-1 downto s do str_pool[j+1]:=str_pool[j];
    str_pool[s]:="""";
    pool_ptr:=pool_ptr+2;
    end;
  end;
e:=pool_ptr-str_start[str_ptr]; {total length}
@z

% _____________________________________________________________________________
%
% [35.751]
% _____________________________________________________________________________

@x
begin print(a); print(n); print(e);
@y
var must_quote: boolean; {whether to quote the filename}
@!j:pool_pointer; {index into |str_pool|}
begin
must_quote:=false;
if a>0 then begin
  j:=str_start[a];
  while (not must_quote) and (j<>str_stop(a)) do begin
    must_quote:=str_pool[j]=" "; incr(j);
  end;
end;
if n>0 then begin
  j:=str_start[n];
  while (not must_quote) and (j<>str_stop(n)) do begin
    must_quote:=str_pool[j]=" "; incr(j);
  end;
end;
if e>0 then begin
  j:=str_start[e];
  while (not must_quote) and (j<>str_stop(e)) do begin
    must_quote:=str_pool[j]=" "; incr(j);
  end;
end;
if must_quote then print("""");
if a>0 then
  for j:=str_start[a] to str_stop(a)-1 do
    if so(str_pool[j])<>"""" then
      print(so(str_pool[j]));
if n>0 then
  for j:=str_start[n] to str_stop(n)-1 do
    if so(str_pool[j])<>"""" then
      print(so(str_pool[j]));
if e>0 then
  for j:=str_start[e] to str_stop(e)-1 do
    if so(str_pool[j])<>"""" then
      print(so(str_pool[j]));
if must_quote then print("""");
@z

% _____________________________________________________________________________
%
% [35.752]
% _____________________________________________________________________________

@x
@d append_to_name(#)==begin c:=#; incr(k);
  if k<=file_name_size then name_of_file[k]:=xchr[c];
  end
@y
@d append_to_name(#)==begin c:=#; if not (c="""") then begin incr(k);
  if k<=file_name_size then name_of_file[k]:=xchr[c];
  end end
@z

@x
for k:=name_length+1 to file_name_size do name_of_file[k]:=' ';
@y
name_of_file[ name_length + 1 ]:= chr(0); {\MiKTeX: 0-terminate the file name}
@z

% _____________________________________________________________________________
%
% [35.753]
% _____________________________________________________________________________

@x
@d mem_default_length=15 {length of the |MP_mem_default| string}
@d mem_area_length=6 {length of its area part}
@y
@d mem_area_length=0 {no fixed area in C}
@z

@x
@!MP_mem_default:packed array[1..mem_default_length] of char;
@y
@!MP_mem_default:packed array[1..file_name_size_plus_one] of char;
@!mem_default_length: integer;
@z

% _____________________________________________________________________________
%
% [35.754]
% _____________________________________________________________________________

@x
MP_mem_default:='MPlib:plain.mem';
@y
miktex_get_default_dump_file_name(MP_mem_default);
mem_default_length:=c4pstrlen(MP_mem_default);
@z

% _____________________________________________________________________________
%
% [35.755]
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
for k:=name_length+1 to file_name_size do name_of_file[k]:=' ';
@y
name_of_file[ name_length + 1 ]:= chr(0); {\MiKTeX: 0-terminate the file name}
@z

% _____________________________________________________________________________
%
% [35.757]
% _____________________________________________________________________________

@x
var @!j:0..buf_size; {the first space after the file name}
@y
var @!j:0..sup_buf_size; {the first space after the file name}
@z

@x
  pack_buffered_name(0,loc,j-1); {try first without the system file area}
  if w_open_in(mem_file) then goto found;
  pack_buffered_name(mem_area_length,loc,j-1);
    {now try the system mem file area}
  if w_open_in(mem_file) then goto found;
@y
  pack_buffered_name(0,loc,j-1);
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
% [35.759]
% _____________________________________________________________________________

@x
@p procedure scan_file_name;
label done;
begin begin_name;
while buffer[loc]=" " do incr(loc);
@y
@p procedure scan_file_name;
label done;
begin begin_name;
while (buffer[loc]=" ")or(buffer[loc]=tab) do incr(loc);
@z

% _____________________________________________________________________________
%
% [35.764]
% _____________________________________________________________________________

@x
var @!k:0..buf_size; {index into |buffer|}
@y
var @!k:0..sup_buf_size; {index into |buffer|}
@z

% _____________________________________________________________________________
%
% [35.765]
% _____________________________________________________________________________

@x
@ @<Scan file name in the buffer@>=
begin begin_name; k:=first;
while (buffer[k]=" ")and(k<last) do incr(k);
@y
@ @<Scan file name in the buffer@>=
begin begin_name; k:=first;
while ((buffer[k]=" ")or(buffer[k]=tab))and(k<last) do incr(k);
@z

% _____________________________________________________________________________
%
% [35.766]
% _____________________________________________________________________________

@x
@!k:0..buf_size; {index into |months| and |buffer|}
@!l:0..buf_size; {end of first input line}
@y
@!k:0..sup_buf_size; {index into |months| and |buffer|}
@!l:0..sup_buf_size; {end of first input line}
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
% [35.769]
% _____________________________________________________________________________

@x
begin pack_file_name(cur_name,cur_area,ext);
in_name:=cur_name; in_area:=cur_area;
if a_open_in(cur_file) then try_extension:=true
else begin if str_vs_str(ext,".mf")=0 then in_area:=MF_area
  else in_area:=MP_area;
  pack_file_name(cur_name,in_area,ext);
  try_extension:=a_open_in(cur_file);
  end;
@y
{|cur_ext| will be \.{two} if the filename was \.{one.two}.}
begin pack_file_name(cur_name,cur_area,cur_ext);
in_name:=cur_name; in_area:=cur_area;
{The extension is not relevant for determining whether we're allowed to
 open the file.}
if str_vs_str(ext,".mf")=0 then
  try_extension:=miktex_open_metafont_file(cur_file, name_of_file)
else try_extension:=a_open_in(cur_file);
@z

% _____________________________________________________________________________
%
% [35.771]
% _____________________________________________________________________________

@x
@p procedure start_input; {\MP\ will \.{input} something}
label done;
@y
@p procedure start_input; {\MP\ will \.{input} something}
label done;
var j:integer;
@z

@x
  if cur_ext="" then
    if try_extension(".mp") then goto done
    else if try_extension("") then goto done
    else if try_extension(".mf") then goto done
    else do_nothing
  else if try_extension(cur_ext) then goto done;
@y
    if try_extension(".mp") then goto done
    else if try_extension(".mf") then goto done
    else do_nothing;
@z

@x
done: name:=a_make_name_string(cur_file);
@y
done: name:=a_make_name_string(cur_file);
full_source_filename_stack[in_open]:=miktex_make_full_name_string;
@z

@x
  begin job_name:=cur_name; str_ref[job_name]:=max_str_ref;
@y
  begin
    j:=1;
    begin_name;
    stop_at_space:=false;
    while (j<=name_length)and(more_name(name_of_file[j])) do
      incr(j);
    stop_at_space:=true;
    end_name;
    job_name:=miktex_get_job_name;
    str_ref[job_name] := max_str_ref;
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
% [35.776]
% _____________________________________________________________________________

@x
@!old_file_name : packed array[1..file_name_size] of char;
  {analogous to |name_of_file|}
@y
@!old_file_name : packed array[1..file_name_size_plus_one] of char;
  {analogous to |name_of_file|}
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
% [41.1071]
% _____________________________________________________________________________

@x
@!n:0..max_print_line; {amount of macro text to show}
@y
@!n:0..sup_max_print_line; {amount of macro text to show}
@z

% _____________________________________________________________________________
%
% [41.1103]
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
% [42.1144]
% _____________________________________________________________________________

@x
@!char_remainder:array[eight_bits] of 0..lig_table_size; {the |remainder| byte}
@y
@!char_remainder:array[eight_bits] of 0..sup_lig_table_size; {the |remainder| byte}
@z

@x
@!lig_kern:array[0..lig_table_size] of four_quarters; {the ligature/kern table}
@y
@!lig_kern:array[0..sup_lig_table_size] of four_quarters; {the ligature/kern table}
@z

@x
@!skip_table:array[eight_bits] of 0..lig_table_size; {local label status}
@y
@!skip_table:array[eight_bits] of 0..sup_lig_table_size; {local label status}
@z

@x
@!bch_label:0..lig_table_size; {left boundary starting location}
@!ll,@!lll:0..lig_table_size; {registers used for lig/kern processing}
@!label_loc:array[0..256] of -1..lig_table_size; {lig/kern starting addresses}
@y
@!bch_label:0..sup_lig_table_size; {left boundary starting location}
@!ll,@!lll:0..sup_lig_table_size; {registers used for lig/kern processing}
@!label_loc:array[0..256] of -1..sup_lig_table_size; {lig/kern starting addresses}
@z

% _____________________________________________________________________________
%
% [42.1182]
% _____________________________________________________________________________

@x
print_nl("Font metrics written on "); print(metric_file_name); print_char(".");
@y
print_nl("Font metrics written on ");
print_file_name(0,metric_file_name,0);
print_char(".");@y
@z

% _____________________________________________________________________________
%
% [43.1192]
% _____________________________________________________________________________

@x
font_info:array[0..font_mem_size] of memory_word;
  {height, width, and depth data}
@y
font_info:array[0..sup_font_mem_size] of memory_word;
  {height, width, and depth data}
@z

@x
@!mp_font_map: ^fm_entry_ptr; {pointer into AVL tree of font mappings}
@y
@!mp_font_map: array[font_number] of fm_entry_ptr; {pointer into AVL tree of font mappings}
@z

@x
next_fmem:0..font_mem_size; {next unused entry in |font_info|}
@y
next_fmem:0..sup_font_mem_size; {next unused entry in |font_info|}
@z

% _____________________________________________________________________________
%
% [43.1193]
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
char_base:array[font_number] of 0..sup_font_mem_size;
  {base address for |char_info|}
width_base:array[font_number] of 0..sup_font_mem_size;
  {index for zeroth character width}
height_base:array[font_number] of 0..sup_font_mem_size;
  {index for zeroth character height}
depth_base:array[font_number] of 0..sup_font_mem_size;
@z

% _____________________________________________________________________________
%
% [43.1196]
% _____________________________________________________________________________

@x
@!i,@!ii:0..font_mem_size; {|font_info| indices}
@!jj:0..font_mem_size; {counts bytes to be ignored}
@y
@!i,@!ii:0..sup_font_mem_size; {|font_info| indices}
@!jj:0..sup_font_mem_size; {counts bytes to be ignored}
@z

% _____________________________________________________________________________
%
% [43.1205]
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
if cur_ext="" then cur_ext:=".tfm";
pack_cur_name;
file_opened:=miktex_open_tfm_file(tfm_infile, name_of_file);
if not file_opened then goto bad_tfm;
@z

% _____________________________________________________________________________
%
% [43.1218]
% _____________________________________________________________________________

@x
mp_font_map:=xmalloc_array(fm_entry_ptr,font_max);
@y
@z

% _____________________________________________________________________________
%
% [43.1220]
% _____________________________________________________________________________

@x
begin name_of_file:=ps_tab_name;
@y
begin
  c4p_strcpy(name_of_file, file_name_size_plus_one, ps_tab_name);
@z

@x
if a_open_in(ps_tab_file) then
@y
if miktexopenpstabfile(ps_tab_file, name_of_file) then
@z

% _____________________________________________________________________________
%
% [44.1233]
% _____________________________________________________________________________

@x
  print(first_file_name);
@y
  print_file_name(0,first_file_name,0);
@z

@x
    print(last_file_name);
@y
    print_file_name(0,last_file_name,0);
@z

% _____________________________________________________________________________
%
% [44.1268]
% _____________________________________________________________________________

@x
var @!k:0..font_mem_size; {an index into |font_info|}
@y
var @!k:0..sup_font_mem_size; {an index into |font_info|}
@z

% _____________________________________________________________________________
%
% [44.1271]
% _____________________________________________________________________________

@x
@!p:0..font_mem_size; {|font_info| index for the current character}
@y
@!p:0..sup_font_mem_size; {|font_info| index for the current character}
@z

% _____________________________________________________________________________
%
% [44.1275]
% _____________________________________________________________________________

@x
var @!p:0..font_mem_size; {|font_info| index for the current character}
@y
var @!p:0..sup_font_mem_size; {|font_info| index for the current character}
@z

% _____________________________________________________________________________
%
% [45.1319]
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
% [45.1320]
% _____________________________________________________________________________

@x
@d dump_wd(#)==begin mem_file^:=#; put(mem_file);@+end
@d dump_int(#)==begin mem_file^.int:=#; put(mem_file);@+end
@d dump_hh(#)==begin mem_file^.hh:=#; put(mem_file);@+end
@d dump_qqqq(#)==begin mem_file^.qqqq:=#; put(mem_file);@+end
@y
@d dump_things(#)==miktex_dump(mem_file, #)
@d dump_wd(#)==miktex_dump(mem_file, #)
@d dump_int(#)==miktex_dump_int(mem_file, #)
@d dump_hh(#)==miktex_dump(mem_file, #)
@d dump_qqqq(#)==miktex_dump(mem_file, #)
@z

% _____________________________________________________________________________
%
% [45.1321]
% _____________________________________________________________________________

@x
@d undump_wd(#)==begin get(mem_file); #:=mem_file^;@+end
@d undump_int(#)==begin get(mem_file); #:=mem_file^.int;@+end
@d undump_hh(#)==begin get(mem_file); #:=mem_file^.hh;@+end
@d undump_qqqq(#)==begin get(mem_file); #:=mem_file^.qqqq;@+end
@y
@d undump_things(#)==miktex_undump(mem_file, #)
@d undump_checked_things(#)==miktex_undump(mem_file, #)
@d undump_upper_check_things(#)==miktex_undump(mem_file, #)
@d undump_wd(#)==miktex_undump(mem_file, #)
@d undump_int(#)==miktex_undump_int(mem_file, #)
@d undump_hh(#)==miktex_undump(mem_file, #)
@d undump_qqqq(#)==miktex_undump(mem_file, #)
@z

% _____________________________________________________________________________
%
% [45.1322]
% _____________________________________________________________________________

@x
dump_int(@$);@/
@y
dump_int(@"4D694B50); {"PKiM"}
dump_int(@$);@/
dump_int(main_memory);@/
@<Dump |xord|, |xchr|, and |xprn|@>;
@z

% _____________________________________________________________________________
%
% [45.1323]
% _____________________________________________________________________________

@x
x:=mem_file^.int;
if x<>@$ then goto off_base; {check that strings are the same}
undump_int(x);
if x<>mem_min then goto off_base;
undump_int(x);
if x<>mem_top then goto off_base;
@y
undump_int(x);
if x<>@"4D694B50 then goto off_base; {not a mem file}
undump_int(x);
if x<>@$ then goto off_base; {check that strings are the same}
undump_int(x);
if x<>main_memory then goto off_base;
@<Undump |xord|, |xchr|, and |xprn|@>;
undump_int(x);
if x<>mem_min then goto off_base;
{Dynamic allocation \`a la \.{mf.ch}.}
undump_int(mem_top);
if mem_max < mem_top then mem_max:=mem_top; {Use at least what we dumped.}
if mem_min+1100>mem_top then goto off_base;
mem_max := mem_top + extra_mem_top;
mem:=miktex_reallocate(mem, mem_max - mem_min + 1);
@z

% _____________________________________________________________________________
%
% [45.1325]
% _____________________________________________________________________________

@x
undump_size(0)(pool_size)('string pool size')(pool_ptr);
undump_size(0)(max_strings-1)('max strings')(max_str_ptr);
@y
undump_size(0)(sup_pool_size-pool_free)('string pool size')(pool_ptr);
if pool_size < pool_ptr + pool_free then
  pool_size := pool_ptr+pool_free;
undump_size(0)(sup_max_strings)('max strings')(max_str_ptr);
@/
str_ref:=miktex_reallocate(str_ref, max_strings);
next_str:=miktex_reallocate(next_str, max_strings);
str_start:=miktex_reallocate(str_start, max_strings);
str_pool:=miktex_reallocate(str_pool, pool_size);
@z

% _____________________________________________________________________________
%
% [45.1331]
% _____________________________________________________________________________

@x
undump_int(x);@+if (x<>69073)or eof(mem_file) then goto off_base
@y
undump_int(x);@+if (x<>69073)or not eof(mem_file) then goto off_base
@z

% _____________________________________________________________________________
%
% [45.1332]
% _____________________________________________________________________________

@x
print_int(round_unscaled(internal[year]) mod 100); print_char(".");
@y
print_int(round_unscaled(internal[year])); print_char(".");
@z

% _____________________________________________________________________________
%
% [46.1336]
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
@!init if not get_strings_started then goto final_end;
init_tab; {initialize the tables}
init_prim; {call |primitive| for each primitive}
init_str_use:=str_ptr; init_pool_ptr:=pool_ptr;@/
max_str_ptr:=str_ptr; max_pool_ptr:=pool_ptr;
fix_date_and_time;
@y
@!init if miktex_is_init_program then begin
if not get_strings_started then goto final_end;
init_tab; {initialize the tables}
init_prim; {call |primitive| for each primitive}
init_str_use:=str_ptr; init_pool_ptr:=pool_ptr;@/
max_str_ptr:=str_ptr; max_pool_ptr:=pool_ptr;
fix_date_and_time;
end;
@z

@x
ready_already:=314159;
@y
ready_already:=314159;
miktex_on_texmf_initialize;
@z

@x
mp_init_map_file(troff_mode);
@y
mp_init_map_file(0);
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
% [46.1337]
% _____________________________________________________________________________

@x
    print(log_name); print_char(".");
    end;
  end;
@y
    print_file_name(0,log_name,0); print_char(".");
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
% [46.1342]
% _____________________________________________________________________________

@x
  begin @!init store_mem_file; return;@+tini@/
@y
  begin @!Init store_mem_file; return;@+Tini@/
@z

% _____________________________________________________________________________
%
% [46.1343]
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
% [46.1344]
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
% [48.1349]
% _____________________________________________________________________________

@x
This section should be replaced, if necessary, by any special
modification of the program
that are necessary to make \MP\ work at a particular installation.
It is usually best to design your change file so that all changes to
previous sections preserve the section numbering; then everybody's version
will be consistent with the published program. More extensive changes,
which introduce new sections, can be inserted here; then only the index
itself will get a new section number.
@^system dependencies@>
@y
@ @<Declare \MiKTeX\ procedures@>=

function make_fraction(@!p,@!q:integer):fraction; forward;
function miktex_c_style_error_messages_p : boolean; forward;
function miktex_enable_eightbit_chars_p : boolean; forward;@t\2@>@/
function miktex_get_interaction : integer; forward;
function miktex_get_job_name : str_number; forward;
function miktex_halt_on_error_p : boolean; forward;
function miktex_have_tcx_file_name : boolean; forward;
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
@!bistack_size:integer; {size of stack for bisection algorithms;
  should probably be left at this value}
@!buf_size:integer;
@!dummy_xord:ASCII_code;
@!dummy_xchr:text_char;
@!dummy_xprn:ASCII_code;
@!edit_line:integer; {what line to start editing at}
@!edit_name_length:integer;
@!edit_name_start:pool_pointer; {where the filename to switch to starts}
@!error_line:integer;
@!extra_mem_top:integer; {|mem_max:=mem_top+extra_mem_top| except in \.{INIMP}}
@!font_mem_size:integer; {number of words for \.{TFM} information for text fonts}
@!half_error_line:integer;
@!lig_table_size:integer;
@!main_memory:integer; {total memory words allocated in initex}
@!max_print_line:integer; {width of longest text lines output; should be at least 60}
@!max_strings:integer; {maximum number of strings; must not exceed |max_halfword|}
@!mem_max:integer; {greatest index in \MP's internal |mem| array;
  must be strictly less than |max_halfword|;
  must be equal to |mem_top| in \.{INIMP}, otherwise |>=mem_top|}
@!mem_top:integer; {largest index in the |mem| array dumped by \.{INIMP};
  must be substantially larger than |mem_min|
  and not greater than |mem_max|}
@!quoted_filename:boolean; {current filename is quoted}
@!param_size:integer; {maximum number of simultaneous macro parameters}
@!path_size:integer; {maximum number of knots between breakpoints of a path}
@!pool_free:integer;{minimum pool space free after format loaded}
@!pool_size:integer; {maximum number of characters in strings, including all
  error messages and help texts, and the names of all identifiers;
  must exceed |string_vacancies| by the total
  length of \MF's own strings, which is currently about 22000}
@!stack_size:integer; {maximum number of simultaneous input sources}
@!stop_at_space:boolean; {whether |more_name| returns false for space}
@!string_vacancies:integer;
@!xprn:array[ASCII_code] of ASCII_code; {use \.{\^\^} notation?}

@ @<Set init...@>=
edit_name_start:=0;
stop_at_space:=true;

@ Dumping the |xord|, |xchr|, and |xprn| arrays.  We dump these always
in the format, so a TCX file loaded during format creation can set a
default for users of the format.

@<Dump |xord|, |xchr|, and |xprn|@>=
dump_things(xord[0], 256);
dump_things(xchr[0], 256);
dump_things(xprn[0], 256);

@ Undumping the |xord|, |xchr|, and |xprn| arrays.  This code is more
complicated, because we want to ensure that a TCX file specified on
the command line will override whatever is in the format.  Since the
tcx file has already been loaded, that implies throwing away the data
in the format.  Also, if no |translate_filename| is given, but
|eight_bit_p| is set we have to make all characters printable.

@<Undump |xord|, |xchr|, and |xprn|@>=
if miktex_have_tcx_file_name then begin
  for k:=0 to 255 do undump_things(dummy_xord, 1);
  for k:=0 to 255 do undump_things(dummy_xchr, 1);
  for k:=0 to 255 do undump_things(dummy_xprn, 1);
  end
else begin
  undump_things(xord[0], 256);
  undump_things(xchr[0], 256);
  undump_things(xprn[0], 256);
  if miktex_enable_eightbit_chars_p then
    for k:=0 to 255 do
      xprn[k]:=1;
end;
@z
