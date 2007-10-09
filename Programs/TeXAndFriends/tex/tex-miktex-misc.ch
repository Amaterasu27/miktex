%% tex-miktex-misc.ch:
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
@t\4@>@<Declare \MiKTeX\ functions@>@/
@#
procedure initialize; {this procedure gets things started properly}
@z

% _____________________________________________________________________________
%
% [1.6]
% _____________________________________________________________________________

@x
@d final_end=9999 {this label marks the ending of the program}
@y
@d final_end=9999 {this label marks the ending of the program}
@d goto_end_of_TEX==c4p_throw(end_of_TEX) {\MiKTeX: throw \Cplusplus\ exception}
@d goto_final_end==c4p_throw(final_end) {\MiKTeX: throw \Cplusplus\ exception}
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
the codewords `$|init|\ldots|tini|$'.

\MiKTeX: we use the function |miktex_is_init_program| to distinguish
between the two variants.

@d init== @+if miktex_is_init_program then@+begin
@d tini== @+end;
@z

% _____________________________________________________________________________
%
% [1.11]
% _____________________________________________________________________________

@x
in production versions of \TeX.
@.INITEX@>
@^system dependencies@>
@y
in production versions of \TeX.
@.INITEX@>
@^system dependencies@>

\MiKTeX: some parameters are global variables. For example, |mem_max| is
an integer variable.
@z

@x
@!mem_max=30000; {greatest index in \TeX's internal |mem| array;
@y
@!sup_mem_max=1048576; {greatest index in \TeX's internal |mem| array;
@z

@x
@!mem_min=0; {smallest index in \TeX's internal |mem| array;
@y
@!inf_mem_min=0; {smallest index in \TeX's internal |mem| array;
@z

@x
@!buf_size=500; {maximum number of characters simultaneously present in
@y
@!sup_buf_size=66000; {maximum number of characters simultaneously present in
@z

@x
@!error_line=72; {width of context lines on terminal error messages}
@y
@!sup_error_line=72; {width of context lines on terminal error messages}
@z

@x
@!half_error_line=42; {width of first lines of contexts in terminal
@y
@!sup_half_error_line=42; {width of first lines of contexts in terminal
@z

@x
@!max_print_line=79; {width of longest text lines output; should be at least 60}
@y
@!sup_print_line=79; {width of longest text lines output; should be at least 60}
@z

@x
@!stack_size=200; {maximum number of simultaneous input sources}
@y
@!sup_stack_size=300; {maximum number of simultaneous input sources}
@z

@x
@!max_in_open=6; {maximum number of input files and error insertions that
@y
@!sup_max_in_open=20; {maximum number of input files and error insertions that
@z

@x
@!font_max=75; {maximum internal font number; must not exceed |max_quarterword|
@y
@!sup_font_max=1000; {maximum internal font number; must not exceed |max_quarterword|
@z

@x
@!font_mem_size=20000; {number of words of |font_info| for all fonts}
@y
@!sup_font_mem_size=131068; {number of words of |font_info| for all fonts}
@z

@x
@!param_size=60; {maximum number of simultaneous macro parameters}
@y
@!sup_param_size=500; {maximum number of simultaneous macro parameters}
@z

@x
@!nest_size=40; {maximum number of semantic levels simultaneously active}
@y
@!sup_nest_size=200; {maximum number of semantic levels simultaneously active}
@z

@x
@!max_strings=3000; {maximum number of strings; must not exceed |max_halfword|}
@y
@!sup_max_strings=70000; {maximum number of strings; must not exceed |max_halfword|}
@z

@x
@!string_vacancies=8000; {the minimum number of characters that should be
@y
@!sup_string_vacancies=170000; {the minimum number of characters that should be
@z

@x
@!pool_size=32000; {maximum number of characters in strings, including all
@y
@!sup_pool_size=200000; {maximum number of characters in strings, including all
@z

@x
@!save_size=600; {space for saving values outside of current group; must be
@y
@!sup_save_size=70000; {space for saving values outside of current group; must be
@z

@x
@!dvi_buf_size=800; {size of the output buffer; must be a multiple of 8}
@y
@!dvi_buf_size=8192; {size of the output buffer; must be a multiple of 8}
@z

@x
@!file_name_size=40; {file names shouldn't be longer than this}
@y
@!file_name_size=259; {file names shouldn't be longer than this}
@!file_name_size_plus_one=260; {one more for the string terminator}
@z

% _____________________________________________________________________________
%
% [1.12]
% _____________________________________________________________________________

@x
@d mem_bot=0 {smallest index in the |mem| array dumped by \.{INITEX};
@y
@d mem_bot_def=0 {smallest index in the |mem| array dumped by \.{INITEX};
@z

@x
@d mem_top==30000 {largest index in the |mem| array dumped by \.{INITEX};
@y
@d mem_top_def==1048576 {largest index in the |mem| array dumped by \.{INITEX};
@z

@x
@d font_base=0 {smallest internal font number; must not be less
@y
@d max_font_max=5000 {maximum number of internal fonts; this can be
                      increased, but |hash_size+max_font_max|
                      should not exceed 29000.}
@d font_base=0 {smallest internal font number; must not be less
@z

@x
@d hash_size=2100 {maximum number of control sequences; it should be at most
@y
@d hash_size=60000 {maximum number of control sequences; it should be at most
@z

@x
@d hash_prime=1777 {a prime number equal to about 85\pct! of |hash_size|}
@y
@d hash_prime=51001 {a prime number equal to about 85\pct! of |hash_size|}
@z

% _____________________________________________________________________________
%
% [2.20]
%
% Our chars are unsigned (VC++ option /J), i.e., the followin isn't necessary.
%
% _____________________________________________________________________________

 @x
@!xord: array [text_char] of ASCII_code;
 @y
@!xord: array [eight_bits] of ASCII_code;
 @z

% _____________________________________________________________________________
%
% [2.22]
% _____________________________________________________________________________

@x
@d carriage_return=@'15 {ASCII code used at end of line}
@y
@d carriage_return=@'15 {ASCII code used at end of line}
@d tabulator=9 {\MiKTeX: ASCII code used as tabulator}
@d form_feed=12 {\MiKTeX: ASCII code used between pages}
@z

% _____________________________________________________________________________
%
% [2.23]
% _____________________________________________________________________________

@x
To get the most ``permissive'' character set, change |' '| on the
right of these assignment statements to |chr(i)|.
@^character set dependencies@>
@^system dependencies@>
@y
To get the most ``permissive'' character set, change |' '| on the
right of these assignment statements to |chr(i)|.
@^character set dependencies@>
@^system dependencies@>

\MiKTeX: we use the most ``permissive'' character set.
@z

@x
@<Set init...@>=
for i:=0 to @'37 do xchr[i]:=' ';
for i:=@'177 to @'377 do xchr[i]:=' ';
@y
@<Set init...@>=
do_nothing;
@z

% _____________________________________________________________________________
%
% [2.24]
% _____________________________________________________________________________

@x
codes below @'40 in case there is a coincidence.
@y
codes below @'40 in case there is a coincidence.

\MiKTeX: we use the routine |miktex_initialize_char_tables| to initialize
|xord|.
@z

@x
@<Set init...@>=
for i:=first_text_char to last_text_char do xord[chr(i)]:=invalid_code;
for i:=@'200 to @'377 do xord[xchr[i]]:=i;
for i:=0 to @'176 do xord[xchr[i]]:=i;
@y
@<Set init...@>=
miktex_initialize_char_tables;
@z

% _____________________________________________________________________________
%
% [3.26]
% _____________________________________________________________________________

@x
|name_of_file|.
@^system dependencies@>
@y
|name_of_file|.
@^system dependencies@>

\MiKTeX: reserve an extra byte for the terminating null character.
@z

@x
@!name_of_file:packed array[1..file_name_size] of char;@;@/
@y
@!name_of_file:packed array[1..file_name_size_plus_one] of char;@;@/
@z

% _____________________________________________________________________________
%
% [3.27]
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
begin a_open_in:=miktex_open_input_file(f);
@z

@x
begin rewrite(f,name_of_file,'/O'); a_open_out:=rewrite_OK(f);
@y
begin a_open_out:=miktex_open_output_file(f, true);
@z

@x
begin reset(f,name_of_file,'/O'); b_open_in:=reset_OK(f);
@y
begin b_open_in:=false {\MiKTeX\ does not need this function}
@z

@x
begin rewrite(f,name_of_file,'/O'); b_open_out:=rewrite_OK(f);
@y
begin b_open_out:=miktex_open_output_file(f, false);
@z

@x
begin reset(f,name_of_file,'/O'); w_open_in:=reset_OK(f);
@y
begin w_open_in:=false {\MiKTeX\ does not need this function}
@z

@x
begin rewrite(f,name_of_file,'/O'); w_open_out:=rewrite_OK(f);
@y
begin w_open_out:=miktex_open_output_file(f, false);
@z

% _____________________________________________________________________________
%
% [3.28]
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
% [3.30]
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
% [3.31]
% _____________________________________________________________________________

@x
finer tuning is often possible at well-developed \PASCAL\ sites.
@^inner loop@>
@y
finer tuning is often possible at well-developed \PASCAL\ sites.
@^inner loop@>

\MiKTeX: we use our own line-reader.
@z

@x
@p function input_ln(var f:alpha_file;@!bypass_eoln:boolean):boolean;
  {inputs the next line or returns |false|}
var last_nonblank:0..buf_size; {|last| with trailing blanks removed}
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
@p function input_ln(var f:alpha_file;@!bypass_eoln:boolean):boolean;
  forward;@t\2@>@/
@z

% _____________________________________________________________________________
%
% [3.33]
% _____________________________________________________________________________

@x
in \ph. The `\.{/I}' switch suppresses the first |get|.
@^system dependencies@>
@y
in \ph. The `\.{/I}' switch suppresses the first |get|.
@^system dependencies@>

\MiKTeX: standard input and output streams were prepared
by the \C4P\ runtime library.
@z

@x
@d t_open_in==reset(term_in,'TTY:','/O/I') {open the terminal for text input}
@d t_open_out==rewrite(term_out,'TTY:','/O') {open the terminal for text output}
@y
@d t_open_in==term_in:=i@&nput
@d t_open_out==term_out:=output
@z

% _____________________________________________________________________________
%
% [3.34]
% _____________________________________________________________________________

@x
some instruction to the operating system.  The following macros show how
these operations can be specified in \ph:
@^system dependencies@>
@y
some instruction to the operating system.
@^system dependencies@>
@z

@x
@d clear_terminal == break_in(term_in,true) {clear the terminal input buffer}
@y
@d clear_terminal == break(term_in) {clear the terminal input buffer}
@z

% _____________________________________________________________________________
%
% [3.35]
% _____________________________________________________________________________

@x
  begin write_ln(term_out,'Buffer size exceeded!'); goto final_end;
@y
  begin write_ln(term_out,'Buffer size exceeded!');
  goto_final_end; {\MiKTeX: throw a \Cplusplus\ exception}
@z

% _____________________________________________________________________________
%
% [3.37]
% _____________________________________________________________________________

@x
if the system permits them.
@^system dependencies@>
@y
if the system permits them.
@^system dependencies@>

\MiKTeX: we possibly use the routine |miktex_initialize_buffer| to get the
first input line.
@z

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
% [4.38]
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
@^character set dependencies@>
@^system dependencies@>
@y
@^character set dependencies@>
@^system dependencies@>

\MiKTeX: we use the function |miktex_is_printable| to test a character.
@z

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
else  bad_pool('! I can''t read TEX.POOL.')
@.I can't read TEX.POOL@>
@y
else begin
  wake_up_terminal;
  write_ln(term_out, '! I can''t read TEX.POOL.');
@.I can't read TEX.POOL@>
  get_strings_started:=false;
  return;
end
@z

% _____________________________________________________________________________
%
% [5.54]
% _____________________________________________________________________________

@x
@!term_offset : 0..max_print_line;
  {the number of characters on the current terminal line}
@!file_offset : 0..max_print_line;
  {the number of characters on the current file line}
@!trick_buf:array[0..error_line] of ASCII_code; {circular buffer for
@y
@!term_offset : 0..sup_print_line;
  {the number of characters on the current terminal line}
@!file_offset : 0..sup_print_line;
  {the number of characters on the current file line}
@!trick_buf:array[0..sup_error_line] of ASCII_code; {circular buffer for
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
if format_ident=0 then wterm_ln(' (no format preloaded)')
else  begin slow_print(format_ident); print_ln;
  end;
@y
miktex_print_miktex_banner(term_out); {\MiKTeX: append the \MiKTeX\ version information}
if format_ident=0 then print_ln {\MiKTeX: eliminate misleading `(no format preloaded)'.}
else  begin slow_print(format_ident); print_ln;
  end;
@z

% _____________________________________________________________________________
%
% [5.71]
% _____________________________________________________________________________

@x
var k:0..buf_size; {index into |buffer|}
@y
var k:0..sup_buf_size; {index into |buffer|}
@z

% _____________________________________________________________________________
%
% [6.73]
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
% [6.74]
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
% [6.81]
% _____________________________________________________________________________

@x
begin goto end_of_TEX;
@y
begin goto_end_of_TEX; {\MiKTeX: throw a \Cplusplus\ exception}
@z

% _____________________________________________________________________________
%
% [6.82]
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
% [6.84]
% _____________________________________________________________________________

@x
  begin print_nl("You want to edit file ");
@.You want to edit file x@>
  slow_print(input_stack[base_ptr].name_field);
  print(" at line "); print_int(line);
@y
  begin miktex_remember_edit_info (input_stack[base_ptr].name_field, line);
@z

% _____________________________________________________________________________
%
% [7.109]
% _____________________________________________________________________________

@x
@!glue_ratio=real; {one-word representation of a glue expansion factor}
@y
@!glue_ratio=longreal; {one-word representation of a glue expansion factor;
\MiKTeX: we use |longreal| instead of |real|}
@z

% _____________________________________________________________________________
%
% [8.110]
% _____________________________________________________________________________

@x
@d min_quarterword=0 {smallest allowable value in a |quarterword|}
@d max_quarterword=255 {largest allowable value in a |quarterword|}
@d min_halfword==0 {smallest allowable value in a |halfword|}
@d max_halfword==65535 {largest allowable value in a |halfword|}
@y
@d min_quarterword=0 {smallest allowable value in a |quarterword|}
@d max_quarterword==65535 {largest allowable value in a |quarterword|}
@d min_halfword==0 {smallest allowable value in a |halfword|}
@d max_halfword==@"3FFFFFFF {largest allowable value in a |halfword|}
@z

% _____________________________________________________________________________
%
% [8.111]
% _____________________________________________________________________________

@x
if (font_base<min_quarterword)or(font_max>max_quarterword) then bad:=15;
if font_max>font_base+256 then bad:=16;
@y
if (max_font_max<min_halfword)or(max_font_max>max_halfword) then bad:=15;
if font_max>font_base+max_font_max then bad:=16;
@z

% _____________________________________________________________________________
%
% [8.112]
%
% The original definitions of qi and qo do not work well with the MLTeX
% extensions.
%
% _____________________________________________________________________________

@x
@d qi(#)==#+min_quarterword
  {to put an |eight_bits| item into a quarterword}
@d qo(#)==#-min_quarterword
  {to take an |eight_bits| item out of a quarterword}
@d hi(#)==#+min_halfword
  {to put a sixteen-bit item into a halfword}
@d ho(#)==#-min_halfword
  {to take a sixteen-bit item from a halfword}
@y
@d qi(#)==#
  {to put an |eight_bits| item into a quarterword}
@d qo(#)==#
  {to take an |eight_bits| item out of a quarterword}
@d hi(#)==#
  {to put a sixteen-bit item into a halfword}
@d ho(#)==#
  {to take a sixteen-bit item from a halfword}
@z

% _____________________________________________________________________________
%
% [9.116]
% _____________________________________________________________________________

@x
@!mem : array[mem_min..mem_max] of memory_word; {the big dynamic storage area}
@y
@!mem : array[inf_mem_min..sup_mem_max] of memory_word; {the big dynamic storage area}
@z

% _____________________________________________________________________________
%
% [9.126]
% _____________________________________________________________________________

@x
begin if hi_mem_min-lo_mem_max>=1998 then t:=lo_mem_max+1000
@y
begin if hi_mem_min-lo_mem_max>=1022 then t:=lo_mem_max+512
@z

% _____________________________________________________________________________
%
% [9.131]
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
% [10.144]
% _____________________________________________________________________________

% font numbers can be > 255

@x
@p function new_ligature(@!f,@!c:quarterword; @!q:pointer):pointer;
@y
@p function new_ligature(@!f:internal_font_number; @!c:quarterword;
                         @!q:pointer):pointer;
@z
% _____________________________________________________________________________
%
% [12.186]
%
% Don't worry about strange floating point values.
%
% _____________________________________________________________________________

@x
  if abs(mem[p+glue_offset].int)<@'4000000 then print("?.?")
  else if abs(g)>float_constant(20000) then
@y
  { The Unix |pc| folks removed this restriction with a remark that
    invalid bit patterns were vanishingly improbable, so we follow
    their example without really understanding it.
  |if abs(mem[p+glue_offset].int)<@'4000000 then print('?.?')|
  |else| }
  if fabs(g)>float_constant(20000) then
@z

% _____________________________________________________________________________
%
% [16.213]
% _____________________________________________________________________________

@x
@!nest:array[0..nest_size] of list_state_record;
@!nest_ptr:0..nest_size; {first unused location of |nest|}
@!max_nest_stack:0..nest_size; {maximum of |nest_ptr| when pushing}
@y
@!nest:array[0..sup_nest_size] of list_state_record;
@!nest_ptr:0..sup_nest_size; {first unused location of |nest|}
@!max_nest_stack:0..sup_nest_size; {maximum of |nest_ptr| when pushing}
@z

% _____________________________________________________________________________
%
% [16.218]
% _____________________________________________________________________________

@x
var p:0..nest_size; {index into |nest|}
@y
var p:0..sup_nest_size; {index into |nest|}
@z

% _____________________________________________________________________________
%
% [17.222]
% _____________________________________________________________________________

@x
@d undefined_control_sequence=frozen_null_font+257 {dummy location}
@y
@d undefined_control_sequence=frozen_null_font+max_font_max+1 {dummy location}
@z

% _____________________________________________________________________________
%
% [17.230]
% _____________________________________________________________________________

@x
@d cur_font_loc=box_base+256 {internal font number outside math mode}
@y
@d cur_font_loc=box_base+max_font_max {internal font number outside math mode}
@z

% _____________________________________________________________________________
%
% [17.241]
% _____________________________________________________________________________

@x
begin time:=12*60; {minutes since midnight}
day:=4; {fourth day of the month}
month:=7; {seventh month of the year}
year:=1776; {Anno Domini}
@y
begin time:=c4p_hour*60+c4p_minute; {minutes since midnight}
day:=c4p_day; {day of month}
month:=c4p_month; {month of year}
year:=c4p_year; {Anno Domini}
@z

% _____________________________________________________________________________
%
% [18.264]
% _____________________________________________________________________________

@x
@p @!init procedure primitive(@!s:str_number;@!c:quarterword;@!o:halfword);
@y
@p procedure primitive(@!s:str_number;@!c:quarterword;@!o:halfword);
@z

@x
tini
@y
@z

% _____________________________________________________________________________
%
% [19.271]
% _____________________________________________________________________________

@x
@!save_stack : array[0..save_size] of memory_word;
@!save_ptr : 0..save_size; {first unused entry on |save_stack|}
@!max_save_stack:0..save_size; {maximum usage of save stack}
@y
@!save_stack : array[0..sup_save_size] of memory_word;
@!save_ptr : 0..sup_save_size; {first unused entry on |save_stack|}
@!max_save_stack:0..sup_save_size; {maximum usage of save stack}
@z

@x
@!cur_boundary: 0..save_size; {where the current level begins}
@y
@!cur_boundary: 0..sup_save_size; {where the current level begins}
@z

% _____________________________________________________________________________
%
% [22.301]
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
% [22.304]
% _____________________________________________________________________________

@x
@!in_open : 0..max_in_open; {the number of lines in the buffer, less one}
@!open_parens : 0..max_in_open; {the number of open text files}
@!input_file : array[1..max_in_open] of alpha_file;
@!line : integer; {current line number in the current source file}
@!line_stack : array[1..max_in_open] of integer;
@y
@!in_open : 0..sup_max_in_open; {the number of lines in the buffer, less one}
@!open_parens : 0..sup_max_in_open; {the number of open text files}
@!input_file : array[1..sup_max_in_open] of alpha_file;
@!line : integer; {current line number in the current source file}
@!line_stack : array[1..sup_max_in_open] of integer;
@!source_filename_stack : array[1..sup_max_in_open] of str_number;
@!full_source_filename_stack : array[1..sup_max_in_open] of str_number;
@z

% _____________________________________________________________________________
%
% [22.308]
% _____________________________________________________________________________

@x
@!param_stack:array [0..param_size] of pointer;
  {token list pointers for parameters}
@!param_ptr:0..param_size; {first unused entry in |param_stack|}
@y
@!param_stack:array [0..sup_param_size] of pointer;
  {token list pointers for parameters}
@!param_ptr:0..sup_param_size; {first unused entry in |param_stack|}
@z

% _____________________________________________________________________________
%
% [22.310]
% _____________________________________________________________________________

@x
@!base_ptr:0..stack_size; {shallowest level shown by |show_context|}
@y
@!base_ptr:0..sup_stack_size; {shallowest level shown by |show_context|}
@z

% _____________________________________________________________________________
%
% [22.315]
% _____________________________________________________________________________

@x
@!i:0..buf_size; {index into |buffer|}
@!j:0..buf_size; {end of current line in |buffer|}
@!l:0..half_error_line; {length of descriptive information on line 1}
@!m:integer; {context information gathered for line 2}
@!n:0..error_line; {length of line 1}
@y
@!i:0..sup_buf_size; {index into |buffer|}
@!j:0..sup_buf_size; {end of current line in |buffer|}
@!l:0..sup_half_error_line; {length of descriptive information on line 1}
@!m:integer; {context information gathered for line 2}
@!n:0..sup_error_line; {length of line 1}
@z

% _____________________________________________________________________________
%
% [23.328]
% _____________________________________________________________________________

@x
line_stack[index]:=line; start:=first; state:=mid_line;
@y
source_filename_stack[index]:=0;
full_source_filename_stack[index]:=0;
line_stack[index]:=line; start:=first; state:=mid_line;
@z

% _____________________________________________________________________________
%
% [23.331]
% _____________________________________________________________________________

@x
state:=new_line; start:=1; index:=0; line:=0; name:=0;
@y
state:=new_line; start:=1; index:=0; line:=0; name:=0;
source_filename_stack[1]:=0;
full_source_filename_stack[1]:=0;
@z

% _____________________________________________________________________________
%
% [24.341]
% _____________________________________________________________________________

@x
var k:0..buf_size; {an index into |buffer|}
@y
var k:0..sup_buf_size; {an index into |buffer|}
@z

% _____________________________________________________________________________
%
% [24.363]
% _____________________________________________________________________________

@x
var k:0..buf_size; {an index into |buffer|}
@y
var k:0..sup_buf_size; {an index into |buffer|}
@z

% _____________________________________________________________________________
%
% [25.366]
% _____________________________________________________________________________

@x
@!j:0..buf_size; {index into |buffer|}
@y
@!j:0..sup_buf_size; {index into |buffer|}
@z

% _____________________________________________________________________________
%
% [26.413]
% _____________________________________________________________________________

@x
@!p:0..nest_size; {index into |nest|}
@y
@!p:0..sup_nest_size; {index into |nest|}
@z

% _____________________________________________________________________________
%
% [28.501]
% _____________________________________________________________________________

@x
if_eof_code: begin scan_four_bit_int; b:=(read_open[cur_val]=closed);
  end;
@y
if_eof_code: begin scan_four_bit_int_or_18;
  if cur_val=18 then b:=not miktex_write18_p
  else b:=(read_open[cur_val]=closed);
  end;
@z

% _____________________________________________________________________________
%
% [29.514]
% _____________________________________________________________________________

@x
@d TEX_area=="TeXinputs:"
@.TeXinputs@>
@d TEX_font_area=="TeXfonts:"
@.TeXfonts@>
@y
@d TEX_area==""
@.TeXinputs@>
@d TEX_font_area==""
@.TeXfonts@>
@z

% _____________________________________________________________________________
%
% [29.515]
% _____________________________________________________________________________

@x
begin area_delimiter:=0; ext_delimiter:=0;
@y
begin area_delimiter:=0; ext_delimiter:=0;
quoted_filename:=false;
@z

% _____________________________________________________________________________
%
% [29.516]
% _____________________________________________________________________________

@x
begin if c=" " then more_name:=false
else  begin str_room(1); append_char(c); {contribute |c| to the current string}
  if (c=">")or(c=":") then
@y
begin
if c="""" then begin {\MiKTeX: allow quoted file names}
  quoted_filename:=not quoted_filename;
  more_name:=true;
  c4p_return;
end;
if (c=" " or c=tabulator) and (not quoted_filename) then more_name:=false
else  begin
  str_room(1); append_char(c); {contribute |c| to the current string}
  if (c="/") then
@z

% _____________________________________________________________________________
%
% [29.518]
% _____________________________________________________________________________

@x
begin slow_print(a); slow_print(n); slow_print(e);
@y
begin miktex_print_filename (a, n, e);
@z

% _____________________________________________________________________________
%
% [29.519]
% _____________________________________________________________________________

@x
for k:=name_length+1 to file_name_size do name_of_file[k]:=' ';
@y
name_of_file[ name_length + 1 ]:= chr(0); {\MiKTeX: 0-terminate the file name}
@z

% _____________________________________________________________________________
%
% [29.521]
% _____________________________________________________________________________

@x
TEX_format_default:='TeXformats:plain.fmt';
@y
miktex_get_default_dump_file_name (TEX_format_default);
@z

% _____________________________________________________________________________
%
% [29.522]
% _____________________________________________________________________________

@x
if format_default_length>file_name_size then bad:=31;
@y
do_nothing;
@z

% _____________________________________________________________________________
%
% [29.523]
% _____________________________________________________________________________

@x
since the error will be detected in another way when a strange file name
isn't found.
@^system dependencies@>
@y
since the error will be detected in another way when a strange file name
isn't found.
@^system dependencies@>

\MiKTeX: we do not need this routine.
@z

@x
@p procedure pack_buffered_name(@!n:small_number;@!a,@!b:integer);
var k:integer; {number of positions filled in |name_of_file|}
@!c: ASCII_code; {character being packed}
@!j:integer; {index into |buffer| or |TEX_format_default|}
begin if n+b-a+1+format_ext_length>file_name_size then
  b:=a+file_name_size-n-1-format_ext_length;
k:=0;
for j:=1 to n do append_to_name(xord[TEX_format_default[j]]);
for j:=a to b do append_to_name(buffer[j]);
for j:=format_default_length-format_ext_length+1 to format_default_length do
  append_to_name(xord[TEX_format_default[j]]);
if k<=file_name_size then name_length:=k@+else name_length:=file_name_size;
for k:=name_length+1 to file_name_size do name_of_file[k]:=' ';
end;
@y
@z

% _____________________________________________________________________________
%
% [29.524]
% _____________________________________________________________________________

@x
var j:0..buf_size; {the first space after the format file name}
@y
var j:0..sup_buf_size; {the first space after the format file name}
@z

@x
  pack_buffered_name(0,loc,j-1); {try first without the system file area}
  if w_open_in(fmt_file) then goto found;
  pack_buffered_name(format_area_length,loc,j-1);
    {now try the system format file area}
  if w_open_in(fmt_file) then goto found;
@y
  c4p_memcpy(name_of_file,file_name_size,c4p_ptr(buffer[loc]),j-loc);
  name_of_file[j-loc+1]:=chr(0);
  if miktex_open_format_file(fmt_file) then goto found;
@z

@x
  wterm_ln('Sorry, I can''t find that format;',' will try PLAIN.');
@y
  wterm_ln('Sorry, I can''t find that format;',' will try default.');
@z

@x
pack_buffered_name(format_default_length-format_ext_length,1,0);
if not w_open_in(fmt_file) then
@y
c4p_strcpy(name_of_file,file_name_size_plus_one,TEX_format_default);
if not miktex_open_format_file(fmt_file) then
@z

@x
  wterm_ln('I can''t find the PLAIN format file!');
@y
  wterm_ln('I can''t find the default format file!');
@z

% _____________________________________________________________________________
%
% [29.526]
% _____________________________________________________________________________

@x
  if not more_name(cur_chr) then goto done;
@y
  {If |cur_chr| is a space and we're not scanning a token list, check
   whether we're at the end of the buffer. Otherwise we end up adding
   spurious spaces to file names in some cases.}
  if (cur_chr=" ") and (state<>token_list) and (loc>limit) then goto done;
  if not more_name(cur_chr) then goto done;
@z

% _____________________________________________________________________________
%
% [29.530]
% _____________________________________________________________________________

@x
var k:0..buf_size; {index into |buffer|}
@y
var k:0..sup_buf_size; {index into |buffer|}
@z

@x
if e=".tex" then show_context;
@y
if (e=".tex") or (e="") then show_context;
@z

% _____________________________________________________________________________
%
% [29.532]
% _____________________________________________________________________________

@x
  while not b_open_out(dvi_file) do
@y
  while (not miktex_open_dvi_file (dvi_file)) do
@z

% _____________________________________________________________________________
%
% [29.534]
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
% [29.536]
% _____________________________________________________________________________

@x
slow_print(format_ident); print("  ");
print_int(day); print_char(" ");
months:='JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC';
@y
miktex_print_miktex_banner(log_file);
slow_print(format_ident); print("  ");
print_int(day); print_char(" ");
c4p_arrcpy(months,'JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC');
@z

% _____________________________________________________________________________
%
% [29.537]
% _____________________________________________________________________________

@x
if cur_ext="" then cur_ext:=".tex";
@y
@z

@x
  if cur_area="" then
    begin pack_file_name(cur_name,TEX_area,cur_ext);
    if a_open_in(cur_file) then goto done;
    end;
@y
@z

@x
  prompt_file_name("input file name",".tex");
@y
  prompt_file_name("input file name","");
@z

@x
done: name:=a_make_name_string(cur_file);
@y
done: name:=a_make_name_string(cur_file);
source_filename_stack[in_open]:=name;
full_source_filename_stack[in_open]:=miktex_make_full_name_string;
@z

@x
if job_name=0 then
  begin job_name:=cur_name; open_log_file;
@y
if job_name=0 then
  begin job_name:=miktex_get_job_name; open_log_file;
@z

@x
if term_offset+length(name)>max_print_line-2 then print_ln
else if (term_offset>0)or(file_offset>0) then print_char(" ");
print_char("("); incr(open_parens); slow_print(name); update_terminal;
@y
if term_offset+length(full_source_filename_stack[in_open])>max_print_line-2
then print_ln
else if (term_offset>0)or(file_offset>0) then print_char(" ");
print_char("("); incr(open_parens);
miktex_print_filename(full_source_filename_stack[in_open]); update_terminal;
@z

@x
if name=str_ptr-1 then {we can conserve string pool space now}
  begin flush_string; name:=cur_name;
  end;
@y
@z

% _____________________________________________________________________________
%
% [30.548]
% _____________________________________________________________________________

@x
@!internal_font_number=font_base..font_max; {|font| in a |char_node|}
@!font_index=0..font_mem_size; {index into |font_info|}
@y
@!internal_font_number=font_base..max_font_max; {|font| in a |char_node|}
@!font_index=0..sup_font_mem_size; {index into |font_info|}
@z

% _____________________________________________________________________________
%
% [30.563]
% _____________________________________________________________________________

@x
if not b_open_in(tfm_file) then abort;
@y
if not miktex_open_tfm_file(tfm_file,name_of_file) then abort;
@z

% _____________________________________________________________________________
%
% [32.592] \[32] Shipping pages out
% _____________________________________________________________________________

@x
@!c,@!f:quarterword; {character and font in current |char_node|}
@y
@!c:quarterword;
@!f:internal_font_number;
@z

% _____________________________________________________________________________
%
% [32.597]
% _____________________________________________________________________________

@x
var k:dvi_index;
begin for k:=a to b do write(dvi_file,dvi_buf[k]);
@y
begin c4p_buf_write(dvi_file,c4p_ptr(dvi_buf[a]),b-a+1);
@z

% _____________________________________________________________________________
%
% [32.602]
% _____________________________________________________________________________

@x
begin dvi_out(fnt_def1);
dvi_out(f-font_base-1);@/
@y
begin if (f<=256+font_base) then
  begin dvi_out(fnt_def1);
  dvi_out(f-font_base-1);
  end
else begin dvi_out(fnt_def1+1);
  dvi_out((f-font_base-1) div @'400);
  dvi_out((f-font_base-1) mod @'400);
  end;
@z

% _____________________________________________________________________________
%
% [32.621]
% _____________________________________________________________________________

@x
else  begin dvi_out(fnt1); dvi_out(f-font_base-1);
  end;
@y
else if(f<=256+font_base) then
  begin dvi_out (fnt1); dvi_out(f-font_base-1);
  end
else begin dvi_out(fnt1+1);
  dvi_out((f-font_base-1) div @'400);
  dvi_out((f-font_base-1) mod @'400);
  end;
@z

% _____________________________________________________________________________
%
% [32.642]
% _____________________________________________________________________________

@x
  print_nl("Output written on "); slow_print(output_file_name);
@y
  print_nl("Output written on "); miktex_print_filename(output_file_name);
@z

@x
  b_close(dvi_file);
@y
  miktex_close_dvi_file(dvi_file);
@z

% _____________________________________________________________________________
%
% [49.1244]
% _____________________________________________________________________________

@x
var p:0..nest_size; {index into |nest|}
@y
var p:0..sup_nest_size; {index into |nest|}
@z

% _____________________________________________________________________________
%
% [49.1275]
% _____________________________________________________________________________

@x
  if cur_ext="" then cur_ext:=".tex";
@y
@z

% _____________________________________________________________________________
%
% [50.1302]
% _____________________________________________________________________________

@x
@!init procedure store_fmt_file;
@y
procedure store_fmt_file;
@z

@x
tini
@y
@z

% _____________________________________________________________________________
%
% [50.1303]
% _____________________________________________________________________________

@x
@d bad_fmt=6666 {go here if the format file is unacceptable}
@y
@d bad_fmt=6666 {go here if the format file is unacceptable}
@d start_loading=7777
@z

@x
label bad_fmt,exit;
var j,@!k:integer; {all-purpose indices}
@y
label bad_fmt,exit,start_loading;
var j,@!k:integer; {all-purpose indices}
allow_makefmt:boolean;
@z

@x
begin @<Undump constants for consistency check@>;
@y
begin
  allow_makefmt := true;
start_loading:
  @<Undump constants for consistency check@>;
@z

@x
bad_fmt: wake_up_terminal;
@y
bad_fmt:
  if (allow_makefmt) then begin
    allow_makefmt := false;
    w_close (fmt_file);
    if (miktex_open_format_file(fmt_file, true)) then goto start_loading;
  end
  wake_up_terminal;
@z

% _____________________________________________________________________________
%
% [50.1312]
% _____________________________________________________________________________

@x
repeat for k:=p to q+1 do undump_wd(mem[k]);
@y
repeat c4p_mget(fmt_file, c4p_ptr(mem[p]), (q+1)-(p)+1);
@z

@x
for k:=p to lo_mem_max do undump_wd(mem[k]);
@y
c4p_mget(fmt_file, c4p_ptr(mem[p]), (lo_mem_max)-(p)+1);
@z

@x
for k:=hi_mem_min to mem_end do undump_wd(mem[k]);
@y
c4p_mget(fmt_file, c4p_ptr(mem[hi_mem_min]), (mem_end)-(hi_mem_min)+1);
@z

% _____________________________________________________________________________
%
% [50.1317]
% _____________________________________________________________________________

@x
for j:=k to k+x-1 do undump_wd(eqtb[j]);
@y
c4p_mget(fmt_file, c4p_ptr(eqtb[k]), (k+x-1)-(k)+1);
@z

% _____________________________________________________________________________
%
% [50.1319]
% _____________________________________________________________________________

%@x
for p:=hash_used+1 to undefined_control_sequence-1 do undump_hh(hash[p]);
%@y
c4p_mget(fmt_file, c4p_ptr(hash[hash_used+1]), (undefined_control_sequence-1)-(hash_used+1)+1);
%@z

% _____________________________________________________________________________
%
% [50.1326]
% _____________________________________________________________________________

@x
dump_int(interaction); dump_int(format_ident); dump_int(69069);
@y
if miktex_get_interaction >= 0 then dump_int(error_stop_mode)
else dump_int(interaction);
dump_int(format_ident); dump_int(69069);
@z

% _____________________________________________________________________________
%
% [51.1332]
% _____________________________________________________________________________

@x
@p begin @!{|start_here|}
@y
@p begin @!{|start_here|}
c4p_begin_try_block(final_end);
c4p_begin_try_block(end_of_TEX);
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
end_of_TEX: close_files_and_terminate;
final_end: ready_already:=0;
@y
end_of_TEX: c4p_end_try_block(end_of_TEX); close_files_and_terminate;
final_end: c4p_end_try_block(final_end); ready_already:=0;
miktex_free_memory;
miktex_on_texmf_finish_job;
if (history = error_message_issued) or (history = fatal_error_stop) then
begin
  c4p_exit (1); {\MiKTeX: throw an exception}
end;
@z

% _____________________________________________________________________________
%
% [51.1333]
% _____________________________________________________________________________

@x
    slow_print(log_name); print_char(".");
    end;
  end;
@y
    miktex_print_filename(log_name); print_char(".");
    end;
  end;
print_ln;
miktex_invoke_editor_if_necessary;
@z

% _____________________________________________________________________________
%
% [51.1336]
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
% [51.1337]
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
% [54.1379]
% _____________________________________________________________________________

@x
@* \[54] System-dependent changes.
This section should be replaced, if necessary, by any special
modifications of the program
that are necessary to make \TeX\ work at a particular installation.
It is usually best to design your change file so that all changes to
previous sections preserve the section numbering; then everybody's version
will be consistent with the published program. More extensive changes,
which introduce new sections, can be inserted here; then only the index
itself will get a new section number.
@^system dependencies@>
@y
@* \[54] \MiKTeX-dependent changes.

@ Print an error message like a C compiler would do.

@<Error handling procedures@>=

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

@ Forward declaration of \MiKTeX\ functions.

@<Declare \MiKTeX\ functions@>=

function miktex_is_init_program : boolean; forward;@t\2@>@/
function miktex_c_style_error_messages_p : boolean; forward;@t\2@>@/
function miktex_halt_on_error_p : boolean; forward;@t\2@>@/
function miktex_make_full_name_string : str_number; forward;@t\2@>@/
function miktex_get_job_name : str_number; forward;@t\2@>@/
function miktex_get_interaction : integer; forward;@t\2@>@/

@ Define \MiKTeX\ constants.

@<Constants in the outer block@>=

@!const_font_base=font_base;

@ Define \MiKTeX\ variables.

@<Global variables@>=
@!buf_size: integer;
@!error_line: integer;
@!font_max: integer;
@!font_mem_size: integer;
@!half_error_line: integer;
@!max_in_open: integer;
@!max_print_line: integer;
@!max_strings: integer;
@!mem_bot: integer;
@!mem_max: integer;
@!mem_min: integer;
@!mem_top: integer;
@!nest_size: integer;
@!param_size: integer;
@!pool_size: integer;
@!quoted_filename: boolean;
@!save_size: integer;
@!stack_size: integer;
@!string_vacancies: integer;
@z
