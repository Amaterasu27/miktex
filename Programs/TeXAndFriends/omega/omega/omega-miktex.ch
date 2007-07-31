%% omega-miktex.ch:
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
@d final_end=9999 {this label marks the ending of the program}
@y
@d final_end=9999 {this label marks the ending of the program}
@d goto_final_end==c4p_throw(final_end);
@d goto_eof_TEX==c4p_throw(end_of_TEX);
@z

% _____________________________________________________________________________
%
% [1.7]
% _____________________________________________________________________________

 @x
@d stat==@{ {change this to `$\\{stat}\equiv\null$' when gathering
  usage statistics}
@d tats==@t@>@} {change this to `$\\{tats}\equiv\null$' when gathering
  usage statistics}
 @y
@d stat==
@d tats==
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
@!mem_max=30000; {greatest index in \TeX's internal |mem| array;
@y
@!mem_max_def=1048576; {greatest index in \TeX's internal |mem| array;
@z

@x
@!mem_min=0; {smallest index in \TeX's internal |mem| array;
@y
@!mem_min_def=0; {smallest index in \TeX's internal |mem| array;
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
@!max_print_line_def=79; {width of longest text lines output; should be at least 60}
@z

@x
@!stack_size=200; {maximum number of simultaneous input sources}
@y
@!stack_size_def= 300; {maximum number of simultaneous input sources}
@z

@x
@!max_in_open=6; {maximum number of input files and error insertions that
@y
@!max_in_open_def=20; {maximum number of input files and error insertions that
@z

@x
@!font_max=65535; {maximum internal font number; must be at most |font_biggest|}
@y
@!font_max=65535; {maximum internal font number; must be at most |font_biggest|}
@z

@x
@!param_size=60; {maximum number of simultaneous macro parameters}
@y
@!param_size_def=500; {maximum number of simultaneous macro parameters}
@z

@x
@!nest_size=40; {maximum number of semantic levels simultaneously active}
@y
@!nest_size_def=200; {maximum number of semantic levels simultaneously active}
@z

@x
@!max_strings=3000; {maximum number of strings; must not exceed |max_halfword|}
@y
@!max_strings_def=100000; {maximum number of strings; must not exceed |max_halfword|}
@z

@x
@!string_vacancies=8000; {the minimum number of characters that should be
@y
@!string_vacancies_def=170000; {the minimum number of characters that should be
@z

@x
@!pool_size=32000; {maximum number of characters in strings, including all
@y
@!pool_size_def=200000; {maximum number of characters in strings, including all
@z

@x
@!save_size=600; {space for saving values outside of current group; must be
@y
@!save_size_def= 70000; {space for saving values outside of current group; must be
@z

@x
@!trie_size=8000; {space for hyphenation patterns; should be larger for
@y
@!trie_size_def=80000; {space for hyphenation patterns; should be larger for
@z

@x
@!trie_op_size=500; {space for ``opcodes'' in the hyphenation patterns}
@y
@!trie_op_size_def=2048; {space for ``opcodes'' in the hyphenation patterns}
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

@x
@!active_mem_size=2000; {number of words of |active_info| for active ocps}
@y
@!active_mem_size=2000; {number of words of |active_info| for active ocps}
@z

@x
@!ocp_maxint=@"10000000;
@y
@!ocp_maxint=@"10000000;
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
@d font_base=0 {smallest internal font number; must not be less
@z

@x
@d hash_size=65536 {maximum number of control sequences; it should be at most
@y
@d hash_size=65536 {maximum number of control sequences; it should be at most
@z

@x
@d hash_prime=55711 {a prime number equal to about 85\pct! of |hash_size|}
@y
@d hash_prime=55711 {a prime number equal to about 85\pct! of |hash_size|}
@z

@x
@d hyph_size=307 {another prime; the number of \.{\\hyphenation} exceptions}
@y
@d hyph_size=307 {another prime; the number of \.{\\hyphenation} exceptions}
@z

% _____________________________________________________________________________
%
% [2.22]
% _____________________________________________________________________________

@x
@d carriage_return=@'15 {ASCII code used at end of line}
@y
@d carriage_return=@'15 {ASCII code used at end of line}
@d tabulator=9 {ASCII code used as tabulator}
@d form_feed=12 {ASCII code used between pages}
@z

% _____________________________________________________________________________
%
% [2.23]
% _____________________________________________________________________________

 @x
@<Set init...@>=
for i:=0 to @'37 do xchr[i]:=' ';
for i:=@'177 to @'377 do xchr[i]:=' ';
 @y
@<Set init...@>=
for i:=0 to @'37 do xchr[i]:=chr(i);
for i:=@'177 to @'377 do xchr[i]:=chr(i);
 @z

% _____________________________________________________________________________
%
% [3.26]
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
% [3.27]
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
begin a_open_out:=c4p_try_fopen(f,name_of_file,c4p_w_mode);
@z

@x
begin reset(f,name_of_file,'/O'); b_open_in:=reset_OK(f);
@y
begin b_open_in:=false
@z

@x
begin rewrite(f,name_of_file,'/O'); b_open_out:=rewrite_OK(f);
@y
begin b_open_out:=c4p_try_fopen(f,name_of_file,c4p_wb_mode);
@z

@x
begin reset(f,name_of_file,'/O'); w_open_in:=reset_OK(f);
@y
begin w_open_in:=false
@z

@x
begin rewrite(f,name_of_file,'/O'); w_open_out:=rewrite_OK(f);
@y
begin w_open_out:=c4p_try_fopen(f,name_of_file,c4p_wb_mode);
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
% [3.30]
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
% [3.31]
% _____________________________________________________________________________

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
    buffer[last]:=f^; get(f); incr(last);
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
% [3.33]
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
% [3.34]
% _____________________________________________________________________________

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
  begin write_ln(term_out,'Buffer size exceeded!'); goto_final_end;
@z

% _____________________________________________________________________________
%
% [3.37]
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
% [4.38] \[4] String handling
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
if format_ident=0 then wterm_ln(' (no format preloaded)')
else  begin slow_print(format_ident); print_ln;
  end;
@y
miktex_print_miktex_banner(term_out);
if format_ident=0 then print_ln {|wterm_ln(' (no format preloaded)')|}
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
var k:0..buf_size_def; {index into |buffer|}
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
% [6.81]
% _____________________________________________________________________________

@x
begin goto end_of_TEX;
@y
begin goto_eof_TEX;
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
  interaction:=scroll_mode; jump_out;
@y
    begin edit_name_start:=str_start_ar[input_stack[base_ptr].name_field];
    edit_name_length:=str_start_ar[input_stack[base_ptr].name_field+1] -
                      str_start_ar[input_stack[base_ptr].name_field];
    edit_line:=line;
    jump_out;
@z

% _____________________________________________________________________________
%
% [7.109]
% _____________________________________________________________________________

@x
@!glue_ratio=real; {one-word representation of a glue expansion factor}
@y
@!glue_ratio=longreal; {one-word representation of a glue expansion factor}
@z

% _____________________________________________________________________________
%
% [8.110] \[8] Packed data
% _____________________________________________________________________________

@x
@d min_quarterword=0 {smallest allowable value in a |quarterword|}
@d max_quarterword=@"FFFF {largest allowable value in a |quarterword|}
@d min_halfword=0 {smallest allowable value in a |halfword|}
@d max_halfword=@"3FFFFFFF {largest allowable value in a |halfword|}
@y
@d min_quarterword=0 {smallest allowable value in a |quarterword|}
@d max_quarterword=@"FFFF {largest allowable value in a |quarterword|}
@d min_halfword=0 {smallest allowable value in a |halfword|}
@d max_halfword=@"3FFFFFFF {largest allowable value in a |halfword|}
@z

% _____________________________________________________________________________
%
% [8.111]
% _____________________________________________________________________________

@x
if font_max>font_base+@"10000 then bad:=16;
@y
if font_max>font_base+@"10000 then bad:=16;
@z

% _____________________________________________________________________________
%
% [9.116]
% _____________________________________________________________________________

@x
@!mem : array[mem_min..mem_max] of memory_word; {the big dynamic storage area}
@y
@!mem : array[mem_min_def..mem_max_def] of memory_word; {the big dynamic storage area}
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
% [9.127]
% _____________________________________________________________________________

 @x
  llink(t):=llink(q); rlink(llink(q)):=t;@/
 @y
  r:=llink(q);
  llink(t):=r; rlink(r):=t;@/
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
% [12.186]
% _____________________________________________________________________________

@x
  if abs(mem[p+glue_offset].int)<@'4000000 then print("?.?")
  else if abs(g)>float_constant(20000) then
@y
  if false and abs(mem[p+glue_offset].int)<@'4000000 then print("?.?")
  else if abs(g)>float_constant(20000) then
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
@!nest:array[0..nest_size_def] of list_state_record;
@!nest_ptr:0..nest_size_def; {first unused location of |nest|}
@!max_nest_stack:0..nest_size_def; {maximum of |nest_ptr| when pushing}
@z

% _____________________________________________________________________________
%
% [16.220]
% _____________________________________________________________________________

@x
var p:0..nest_size; {index into |nest|}
@y
var p:0..nest_size_def; {index into |nest|}
@z

% _____________________________________________________________________________
%
% [17.232]
% _____________________________________________________________________________

@x
@d cur_font_loc=box_base+number_regs {internal font number outside math mode}
@y
@d cur_font_loc=box_base+number_regs {internal font number outside math mode}
@z

% _____________________________________________________________________________
%
% [17.245]
% _____________________________________________________________________________

@x
begin time:=12*60; {minutes since midnight}
day:=4; {fourth day of the month}
month:=7; {seventh month of the year}
year:=1776; {Anno Domini}
@y
begin
set_new_eqtb_int(int_base + time_code,c4p_hour*60+c4p_minute);
set_new_eqtb_int(int_base + day_code,c4p_day);
set_new_eqtb_int(int_base + month_code,c4p_month);
set_new_eqtb_int(int_base + year_code,c4p_year);
@z

% _____________________________________________________________________________
%
% [18.269]
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
% [19.276]
% _____________________________________________________________________________

@x
@!save_stack : array[0..save_size] of memory_word;
@!save_ptr : 0..save_size; {first unused entry on |save_stack|}
@!max_save_stack:0..save_size; {maximum usage of save stack}
@y
@!save_stack : array[0..save_size_def] of memory_word;
@!save_ptr : 0..save_size_def; {first unused entry on |save_stack|}
@!max_save_stack:0..save_size_def; {maximum usage of save stack}
@z

@x
@!cur_boundary: 0..save_size; {where the current level begins}
@y
@!cur_boundary: 0..save_size_def; {where the current level begins}
@z

% _____________________________________________________________________________
%
% [22.306]
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

@x
@!in_open : 0..max_in_open; {the number of lines in the buffer, less one}
@!open_parens : 0..max_in_open; {the number of open text files}
@!input_file : array[1..max_in_open] of alpha_file;
@!input_file_mode : array[1..max_in_open] of halfword;
@!input_file_translation : array[1..max_in_open] of halfword;
@!line : integer; {current line number in the current source file}
@!line_stack : array[1..max_in_open] of integer;
@y
@!in_open : 0..max_in_open_def; {the number of lines in the buffer, less one}
@!open_parens : 0..max_in_open_def; {the number of open text files}
@!input_file : array[1..max_in_open_def] of alpha_file;
@!input_file_mode : array[1..max_in_open_def] of halfword;
@!input_file_translation : array[1..max_in_open_def] of halfword;
@!line : integer; {current line number in the current source file}
@!line_stack : array[1..max_in_open_def] of integer;
@!source_filename_stack : array[1..max_in_open_def] of str_number;
@!full_source_filename_stack : array[1..max_in_open_def] of str_number;
@z

% _____________________________________________________________________________
%
% [22.313]
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
% [22.315]
% _____________________________________________________________________________

@x
@!base_ptr:0..stack_size; {shallowest level shown by |show_context|}
@y
@!base_ptr:0..stack_size_def; {shallowest level shown by |show_context|}
@z

% _____________________________________________________________________________
%
% [22.320]
% _____________________________________________________________________________

@x
@!i:0..buf_size; {index into |buffer|}
@!j:0..buf_size; {end of current line in |buffer|}
@!l:0..half_error_line; {length of descriptive information on line 1}
@!m:integer; {context information gathered for line 2}
@!n:0..error_line; {length of line 1}
@y
@!i:0..buf_size_def; {index into |buffer|}
@!j:0..buf_size_def; {end of current line in |buffer|}
@!l:0..half_error_line_def; {length of descriptive information on line 1}
@!m:integer; {context information gathered for line 2}
@!n:0..error_line_def; {length of line 1}
@z

% _____________________________________________________________________________
%
% [24.346]
% _____________________________________________________________________________

@x
var k:0..buf_size; {an index into |buffer|}
@y
var k:0..buf_size_def; {an index into |buffer|}
@z

% _____________________________________________________________________________
%
% [24.368]
% _____________________________________________________________________________

@x
var k:0..buf_size; {an index into |buffer|}
@y
var k:0..buf_size_def; {an index into |buffer|}
@z

% _____________________________________________________________________________
%
% [25.371] \[25] Expanding the next token
% _____________________________________________________________________________

@x
@!j:0..buf_size; {index into |buffer|}
@y
@!j:0..buf_size_def; {index into |buffer|}
@z

% _____________________________________________________________________________
%
% [26.419]
% _____________________________________________________________________________

@x
@!p:0..nest_size; {index into |nest|}
@y
@!p:0..nest_size_def; {index into |nest|}
@z

% _____________________________________________________________________________
%
% [29.521]
% _____________________________________________________________________________

@x
@d TEX_area=="TeXinputs:"
@.TeXinputs@>
@d TEX_font_area=="TeXfonts:"
@.TeXfonts@>
@d OMEGA_ocp_area=="OmegaOCPs:"
@.OmegaOCPs@>
@y
@d TEX_area==""
@.TeXinputs@>
@d TEX_font_area==""
@.TeXfonts@>
@d OMEGA_ocp_area==""
@.OmegaOCPs@>
@z

% _____________________________________________________________________________
%
% [29.523]
% _____________________________________________________________________________

@x
begin if c=" " then more_name:=false
else  begin str_room(1); append_char(c); {contribute |c| to the current string}
  if (c=">")or(c=":") then
@y
begin if c=" " or c=tabulator then more_name:=false
else  begin str_room(1); append_char(c); {contribute |c| to the current string}
  if (c="/") then
@z

% _____________________________________________________________________________
%
% [29.526]
% _____________________________________________________________________________

@x
for k:=name_length+1 to file_name_size do name_of_file[k]:=' ';
@y
name_of_file[ name_length + 1 ]:= chr(0);
@z

% _____________________________________________________________________________
%
% [29.529]
% _____________________________________________________________________________

@x
if format_default_length>file_name_size then bad:=31;
@y
do_nothing;
@z

% _____________________________________________________________________________
%
% [29.530]
% _____________________________________________________________________________

@x
@p procedure pack_buffered_name(@!n:small_number;@!a,@!b:integer);
var k:integer; {number of positions filled in |name_of_file|}
@!c: ASCII_code; {character being packed}
@!j:integer; {index into |buffer| or |TEX_format_default|}
begin if n+b-a+1+format_ext_length>file_name_size then
  b:=a+file_name_size-n-1-format_ext_length;
k:=0;
for j:=1 to n do append_to_name(TEX_format_default[j]);
for j:=a to b do append_to_name(buffer[j]);
for j:=format_default_length-format_ext_length+1 to format_default_length do
  append_to_name(TEX_format_default[j]);
if k<=file_name_size then name_length:=k@+else name_length:=file_name_size;
for k:=name_length+1 to file_name_size do name_of_file[k]:=' ';
end;
@y
@z

% _____________________________________________________________________________
%
% [29.531]
% _____________________________________________________________________________

@x
var j:0..buf_size; {the first space after the format file name}
@y
var j:0..buf_size_def; {the first space after the format file name}
    i:0..buf_size_def;
@z

@x
  pack_buffered_name(0,loc,j-1); {try first without the system file area}
  if w_open_in(fmt_file) then goto found;
  pack_buffered_name(format_area_length,loc,j-1);
    {now try the system format file area}
  if w_open_in(fmt_file) then goto found;
@y
  for i:=loc to j-1 do name_of_file[i-loc+1]:=buffer[i];
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
% [29.537]
% _____________________________________________________________________________

@x
var k:0..buf_size; {index into |buffer|}
@y
var k:0..buf_size_def; {index into |buffer|}
@z

@x
if e=".tex" then show_context;
@y
if (e=".tex") or (e="") then show_context;
@z

% _____________________________________________________________________________
%
% [29.541]
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
% [29.543]
% _____________________________________________________________________________

@x
slow_print(format_ident); print_nl("");
print_int(day); print_char(" ");
months:='JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC';
@y
miktex_print_miktex_banner(log_file);
slow_print(format_ident); print_nl("");
print_int(day); print_char(" ");
c4p_arrcpy(months,'JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC');
@z

% _____________________________________________________________________________
%
% [29.544]
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
  if a_open_in(cur_file) then goto done;
@z

@x
  prompt_file_name("input file name",".tex");
@y
  prompt_file_name("input file name","");
@z

@x
done: name:=a_make_name_string(cur_file);
if job_name=0 then
  begin job_name:=cur_name; open_log_file;
@y
done: name:=a_make_name_string(cur_file);
source_filename_stack[in_open]:=name;
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
  begin flush_string; name:=cur_name;
  end;
@y
@z

% _____________________________________________________________________________
%
% [30.570]
% _____________________________________________________________________________

@x
if aire="" then pack_file_name(nom,TEX_font_area,".ofm")
else pack_file_name(nom,aire,".ofm");
if not b_open_in(tfm_file) then abort;
@y
if aire="" then pack_file_name(nom,TEX_font_area,"")
else pack_file_name(nom,aire,"");
if not miktex_open_xfm_file(tfm_file,name_of_file) then abort;
@z

% _____________________________________________________________________________
%
% [30.576]
% _____________________________________________________________________________

@x
    fget; read_sixteen(font_info(f)(k).hh.lhfield);
@y
    fget; read_sixteen(font_info(f)(k).hh.lh);
@z

% _____________________________________________________________________________
%
% [30.605]
% _____________________________________________________________________________

@x
pack_file_name(nom,aire,".ocp");
if not b_open_in(ocp_file) then ocp_abort("opening file");
@y
pack_file_name(nom,aire,".ocp");
if not miktex_open_ocp_file(ocp_file,name_of_file) then ocp_abort("opening file");
@z

% _____________________________________________________________________________
%
% [30.610]
% _____________________________________________________________________________

@x
@!ocp_list_info:array[ocp_list_index] of memory_word;
@y
@!ocp_list_info:array[0..0] of memory_word;
@z

@x
@!ocp_lstack_info:array[ocp_lstack_index] of memory_word;
@y
@!ocp_lstack_info:array[0..0] of memory_word;
@z

% _____________________________________________________________________________
%
% [32.629]
% _____________________________________________________________________________

@x
var k:dvi_index;
begin for k:=a to b do write(dvi_file,dvi_buf[k]);
@y
begin c4p_buf_write(dvi_file,c4p_ptr(dvi_buf[a]),b-a+1);
@z

% _____________________________________________________________________________
%
% [37.806]
% _____________________________________________________________________________

 @x
if every_cr<>null then begin_token_list(every_cr,every_cr_text);
 @y
if (insert_src_special_every_cr and head<>tail) then insert_src_special;
if every_cr<>null then begin_token_list(every_cr,every_cr_text);
 @z

% _____________________________________________________________________________
%
% [37.831]
% _____________________________________________________________________________

 @x
if every_cr<>null then begin_token_list(every_cr,every_cr_text);
 @y
if (insert_src_special_every_cr) then insert_src_special;
if every_cr<>null then begin_token_list(every_cr,every_cr_text);
 @z

% _____________________________________________________________________________
%
% [42.954]
% _____________________________________________________________________________

@x
@!trie_pointer=0..trie_size; {an index into |trie|}
@y
@!trie_pointer=0..trie_size_def; {an index into |trie|}
@z

% _____________________________________________________________________________
%
% [42.955]
% _____________________________________________________________________________

@x
@!hyf_distance:array[1..trie_op_size] of small_number; {position |k-j| of $n_j$}
@!hyf_num:array[1..trie_op_size] of small_number; {value of $n_j$}
@!hyf_next:array[1..trie_op_size] of quarterword; {continuation code}
@!op_start:array[0..biggest_lang] of 0..trie_op_size;
           {offset for current language}
@y
@!hyf_distance:array[1..trie_op_size_def] of small_number; {position |k-j| of $n_j$}
@!hyf_num:array[1..trie_op_size_def] of small_number; {value of $n_j$}
@!hyf_next:array[1..trie_op_size_def] of quarterword; {continuation code}
@!op_start:array[0..biggest_lang] of 0..trie_op_size_def;
           {offset for current language}
@z

% _____________________________________________________________________________
%
% [43.976] \[43] Initializing the hyphenation tables
% _____________________________________________________________________________

@x
@!init @<Declare procedures for preprocessing hyphenation patterns@>@;
tini
@y
@<Declare procedures for preprocessing hyphenation patterns@>@;
@z

% _____________________________________________________________________________
%
% [43.977]
% _____________________________________________________________________________

@x
@!init@! trie_op_hash:array[-trie_op_size..trie_op_size] of 0..trie_op_size;
@y
@! trie_op_hash:array[0..0] of 0..trie_op_size_def;
@z

@x
@!trie_op_lang:array[1..trie_op_size] of 0..biggest_lang;
  {language part of a hashed quadruple}
@!trie_op_val:array[1..trie_op_size] of quarterword;
  {opcode corresponding to a hashed quadruple}
@!trie_op_ptr:0..trie_op_size; {number of stored ops so far}
tini
@y
@!trie_op_lang:array[1..trie_op_size_def] of 0..biggest_lang;
  {language part of a hashed quadruple}
@!trie_op_val:array[1..trie_op_size_def] of quarterword;
  {opcode corresponding to a hashed quadruple}
@!trie_op_ptr:0..trie_op_size_def; {number of stored ops so far}
@z

% _____________________________________________________________________________
%
% [43.978]
% _____________________________________________________________________________

@x
var h:-trie_op_size..trie_op_size; {trial hash location}
@!u:quarterword; {trial op code}
@!l:0..trie_op_size; {pointer to stored data}
@y
var h:-trie_op_size_def..trie_op_size_def; {trial hash location}
@!u:quarterword; {trial op code}
@!l:0..trie_op_size_def; {pointer to stored data}
@z

@x
loop@+  begin l:=trie_op_hash[h];
@y
loop@+  begin l:=trie_op_hash[h + trie_op_size];
@z

@x
    trie_op_lang[trie_op_ptr]:=cur_lang; trie_op_hash[h]:=trie_op_ptr;
@y
    trie_op_lang[trie_op_ptr]:=cur_lang; trie_op_hash[h + trie_op_size]:=trie_op_ptr;
@z

% _____________________________________________________________________________
%
% [43.979]
% _____________________________________________________________________________

@x
  trie_op_hash[j]:=op_start[trie_op_lang[j]]+trie_op_val[j]; {destination}
for j:=1 to trie_op_ptr do while trie_op_hash[j]>j do
  begin k:=trie_op_hash[j];@/
@y
  trie_op_hash[j + trie_op_size]:=op_start[trie_op_lang[j]]+trie_op_val[j]; {destination}
for j:=1 to trie_op_ptr do while trie_op_hash[j + trie_op_size]>j do
  begin k:=trie_op_hash[j + trie_op_size];@/
@z

@x
  trie_op_hash[j]:=trie_op_hash[k]; trie_op_hash[k]:=k;
@y
  trie_op_hash[j + trie_op_size]:=trie_op_hash[k + trie_op_size]; trie_op_hash[k + trie_op_size]:=k;
@z

% _____________________________________________________________________________
%
% [43.980]
% _____________________________________________________________________________

@x
for k:=-trie_op_size to trie_op_size do trie_op_hash[k]:=0;
@y
for k:=-trie_op_size to trie_op_size do trie_op_hash[k + trie_op_size]:=0;
@z

% _____________________________________________________________________________
%
% [43.981]
% _____________________________________________________________________________

@x
@!init @!trie_c:packed array[trie_pointer] of ASCII_code;
@y
@!trie_c:packed array[trie_pointer] of ASCII_code;
@z

@x
tini
@y
@z

% _____________________________________________________________________________
%
% [43.984]
% _____________________________________________________________________________

@x
@!init@!trie_taken:packed array[1..trie_size] of boolean;
@y
@!trie_taken:packed array[1..trie_size_def] of boolean;
@z

@x
tini
@y
@z

% _____________________________________________________________________________
%
% [46.1068]
% _____________________________________________________________________________

 @x
@<Append character |cur_chr|...@>=
adjust_space_factor;@/
 @y
@<Append character |cur_chr|...@>=
if (head=tail and mode>0) then begin
  if (insert_src_special_auto) then append_src_special;
end;
adjust_space_factor;@/
 @z

% _____________________________________________________________________________
%
% [47.1118]
% _____________________________________________________________________________

 @x
  if every_vbox<>null then begin_token_list(every_vbox,every_vbox_text);
 @y
  if (insert_src_special_every_vbox) then insert_src_special;
  if every_vbox<>null then begin_token_list(every_vbox,every_vbox_text);
 @z

 @x
  if every_hbox<>null then begin_token_list(every_hbox,every_hbox_text);
 @y
  if (insert_src_special_every_hbox) then insert_src_special;
  if every_hbox<>null then begin_token_list(every_hbox,every_hbox_text);
 @z

% _____________________________________________________________________________
%
% [47.1126]
% _____________________________________________________________________________

 @x
if indented then begin
  tail:=new_null_box; link(link(head)):=tail; width(tail):=par_indent;@+
  end;
 @y
if indented then begin
  tail:=new_null_box; link(link(head)):=tail; width(tail):=par_indent;
  if (insert_src_special_every_par) then insert_src_special;@+
  end;
 @z

% _____________________________________________________________________________
%
% [47.1131]
% _____________________________________________________________________________

 @x fixme: etex conflict
  else line_break(widow_penalty);
 @y
  else begin
    if (insert_src_special_every_parend) then insert_src_special;
    line_break(widow_penalty);
  end;
 @z

% _____________________________________________________________________________
%
% [48.1180]
% _____________________________________________________________________________

 @x
if every_math<>null then begin_token_list(every_math,every_math_text);
 @y
if (insert_src_special_every_math) then insert_src_special;
if every_math<>null then begin_token_list(every_math,every_math_text);
 @z

% _____________________________________________________________________________
%
% [48.1186]
% _____________________________________________________________________________

 @x
if every_display<>null then begin_token_list(every_display,every_display_text);
 @y
if (insert_src_special_every_display) then append_src_special;
if every_display<>null then begin_token_list(every_display,every_display_text);
 @z

% _____________________________________________________________________________
%
% [48.1208]
% _____________________________________________________________________________

 @x
  if every_vbox<>null then begin_token_list(every_vbox,every_vbox_text);
 @y
  if (insert_src_special_every_vbox) then insert_src_special;
  if every_vbox<>null then begin_token_list(every_vbox,every_vbox_text);
 @z

% _____________________________________________________________________________
%
% [49.1285]
% _____________________________________________________________________________

@x
var p:0..nest_size; {index into |nest|}
@y
var p:0..nest_size_def; {index into |nest|}
@z

% _____________________________________________________________________________
%
% [49.1316]
% _____________________________________________________________________________

@x
  if cur_ext="" then cur_ext:=".tex";
@y
@z

% _____________________________________________________________________________
%
% [50.1343]
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
% [50.1344]
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
    { <fixme>asserting that |name_of_file| wasn't overridden</fixme> }
    if (miktex_open_format_file(fmt_file, true)) then goto start_loading;
  end
  wake_up_terminal;
@z

% _____________________________________________________________________________
%
% [50.1352]
% _____________________________________________________________________________

@x
for k:=p to lo_mem_max do undump_wd(mem[k]);
@y
c4p_mget (fmt_file, c4p_ptr(mem[p]), lo_mem_max - p + 1);
@z

@x
for k:=hi_mem_min to mem_end do undump_wd(mem[k]);
@y
c4p_mget (fmt_file, c4p_ptr(mem[hi_mem_min]), mem_end - hi_mem_min + 1);
@z

% _____________________________________________________________________________
%
% [50.1354]
% _____________________________________________________________________________

@x
dump_int(par_loc); dump_int(write_loc);@/
@y
dump_int(par_loc); dump_int(write_loc); dump_int(special_loc);@/
@z

% _____________________________________________________________________________
%
% [50.1355]
% _____________________________________________________________________________

@x
undump(hash_base)(frozen_control_sequence)(write_loc);@/
@y
undump(hash_base)(frozen_control_sequence)(write_loc);@/
undump(hash_base)(frozen_control_sequence)(special_loc);
special_token:=cs_token_flag+special_loc;@/
@z

% _____________________________________________________________________________
%
% [51.1378]
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
  c4p_exit (1);
end;
@z

% _____________________________________________________________________________
%
% [51.1379]
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
                         str_start_ar[log_name], length(log_name))
  else
    miktex_invoke_editor(edit_name_start,edit_name_length,edit_line)
end;
@z

% _____________________________________________________________________________
%
% [51.1382]
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
% [53.1390]
% _____________________________________________________________________________

@x
primitive("special",extension,special_node);@/
@y
primitive("special",extension,special_node);@/
special_loc:=cur_val; special_token:=cs_token_flag+special_loc;@/
@z

% _____________________________________________________________________________
%
% [55.1467]
% _____________________________________________________________________________

@x
  if name_of_file then libc_free (name_of_file);
  otp_ext_str:=ocp_external(otp_input_ocp);
  otp_ext_str_arg:=ocp_external_arg(otp_input_ocp);
  name_of_file := xmalloc (4 + length(otp_ext_str) + length(otp_ext_str_arg));
@y
  otp_ext_str:=ocp_external(otp_input_ocp);
  otp_ext_str_arg:=ocp_external_arg(otp_input_ocp);
@z

% _____________________________________________________________________________
%
% [56.1486] \[55] Index
% _____________________________________________________________________________

@x
@* \[55] Index.
@y

@ @<Declare action procedures for use by |main_control|@>=

procedure insert_src_special;
var toklist, p, q : pointer;
begin
  if (source_filename_stack[in_open] > 0 and miktex_is_new_source (source_filename_stack[in_open], line)) then begin
    toklist := get_avail;
    p := toklist;
    info(p) := special_token;
    link(p) := get_avail; p := link(p);
    info(p) := left_brace_token+"{";
    q := str_toks (miktex_make_src_special (source_filename_stack[in_open], line));
    link(p) := link(temp_head);
    p := q;
    link(p) := get_avail; p := link(p);
    info(p) := right_brace_token+"}";
    ins_list (toklist);
    miktex_remember_source_info (source_filename_stack[in_open], line);
  end;
end;

procedure append_src_special;
var p : pointer;
begin
  if (source_filename_stack[in_open] > 0 and miktex_is_new_source (source_filename_stack[in_open], line)) then begin
    new_whatsit (special_node, write_node_size);
    write_stream(tail) := null;
    def_ref := get_avail;
    token_ref_count(def_ref) := null;
    str_toks (miktex_make_src_special (source_filename_stack[in_open], line));
    link(def_ref) := link(temp_head);
    write_tokens(tail) := def_ref;
    miktex_remember_source_info (source_filename_stack[in_open], line);
  end;
end;

@ @<Error handling procedures@>=

procedure print_c_style_error_message (@!s:str_number);
  var k : integer;
begin
  print_nl ("");
  print (full_source_filename_stack[in_open]);
  print (":");
  print_int (line);
  print (": ");
  print (s);
end;

@ @<Declare \MiKTeX\ procedures@>=

function miktex_is_init_program : boolean; forward;
function miktex_c_style_error_messages_p : boolean; forward;
function miktex_halt_on_error_p : boolean; forward;
function miktex_make_full_name_string : str_number; forward;
function miktex_get_job_name : str_number; forward;

@ @<Constants in the outer block@>=

@!const_font_base=font_base;

@ @<Global variables@>=
@!edit_name_start: pool_pointer; {where the filename to switch to starts}
@!edit_name_length,@!edit_line: integer; {what line to start editing at}
@!mem_min : integer;
@!mem_max : integer;
@!mem_bot : integer;
@!mem_top : integer;
@!buf_size : integer;
@!error_line : integer;
@!half_error_line : integer;
@!max_print_line : integer;
@!stack_size : integer;
@!max_in_open : integer;
@!param_size : integer;
@!nest_size : integer;
@!max_strings : integer;
@!string_vacancies : integer;
@!pool_size : integer;
@!save_size : integer;
@!trie_size : integer;
@!trie_op_size : integer;

@!special_loc:pointer;
@!special_token:halfword;

@!insert_src_special_auto : boolean;
@!insert_src_special_every_par : boolean;
@!insert_src_special_every_parend : boolean;
@!insert_src_special_every_cr : boolean;
@!insert_src_special_every_math : boolean;
@!insert_src_special_every_hbox : boolean;
@!insert_src_special_every_vbox : boolean;
@!insert_src_special_every_display : boolean;
	
@!ocp_buf_size : integer;
@!ocp_stack_size : integer;

@!ocp_list_info_size : integer;
@!ocp_lstack_size : integer;
@!ocp_list_list_size : integer;

@!font_tables: array[0..0, 0..0] of memory_word;
@!font_sort_tables: array[0..0, 0..0] of memory_word;
@!ocp_tables: array[0..0, 0..0] of integer;

@!k,l: 0..65535;

@ Initialize \MiKTeX\ variables.

@<Set init...@>=
edit_name_start:=0;

@* \[55] Index.
@z
