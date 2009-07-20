%% miktex-adapter-2.ch:
%% 
%% Copyright (C) 2007-2009 Christian Schenk
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
% [1.2]
% _____________________________________________________________________________

@x
@d eTeX_banner=='This is e-TeX, Version 3.1415926',eTeX_version_string
  {printed when \eTeX\ starts}
@y
@d eTeX_banner_k=='This is e-TeXk, Version 3.1415926',eTeX_version_string
@d eTeX_banner=='This is e-TeX, Version 3.1415926',eTeX_version_string
  {printed when \eTeX\ starts}
@z

@x
@d TeX_banner=='This is TeX, Version 3.1415926' {printed when \TeX\ starts}
@y
@d TeX_banner_k=='This is TeXk, Version 3.1415926' {printed when \TeX\ starts}
@d TeX_banner=='This is TeX, Version 3.1415926' {printed when \TeX\ starts}
@z

@x
@d banner==eTeX_banner
@y
@d banner==eTeX_banner
@d banner_k==eTeX_banner_k
@z

% _____________________________________________________________________________
%
% [2.23]
% _____________________________________________________________________________

@x
for i:=0 to 127 do mubyte_cswrite[i]:=null;
@y
for i:=0 to 128 do mubyte_cswrite[i]:=null;
@z

% _____________________________________________________________________________
%
% [3.24]
% _____________________________________________________________________________

@x
miktex_initialize_char_tables;
@y
for i:=first_text_char to last_text_char do xord[chr(i)]:=invalid_code;
for i:=@'200 to @'377 do xord[xchr[i]]:=i;
for i:=0 to @'176 do xord[xchr[i]]:=i;
{Set |xprn| for printable ASCII, unless |eight_bit_p| is set.}
for i:=0 to 255 do xprn[i]:=(eight_bit_p or ((i>=" ")and(i<="~")));

{The idea for this dynamic translation comes from the patch by
 Libor Skarvada \.{<libor@@informatics.muni.cz>}
 and Petr Sojka \.{<sojka@@informatics.muni.cz>}. I didn't use any of the
 actual code, though, preferring a more general approach.}

{This updates the |xchr|, |xord|, and |xprn| arrays from the provided
 |translate_filename|.  See the function definition in \.{texmfmp.c} for
 more comments.}
if translate_filename then read_tcx_file;
@z

% _____________________________________________________________________________
%
% [3.26]
% _____________________________________________________________________________

@x
@!name_of_file:packed array[1..file_name_size] of char;@;@/
  {on some systems this may be a \&{record} variable}
@y
@!name_of_file:^text_char;
@z

% _____________________________________________________________________________
%
% [3.30]
% _____________________________________________________________________________

@x
@!buffer:array[0..sup_buf_size] of ASCII_code; {lines of characters being read}
@y
@!buffer:^ASCII_code; {lines of characters being read}
@z

% _____________________________________________________________________________
%
% [3.32]
% _____________________________________________________________________________

@x
is considered an output file the file variable is |term_out|.
@^system dependencies@>

@<Glob...@>=
@!term_in:alpha_file; {the terminal as an input file}
@!term_out:alpha_file; {the terminal as an output file}
@y
is considered an output file the file variable is |term_out|.
@^system dependencies@>

@d term_in==stdin {the terminal as an input file}

@<Glob...@>=
@!term_out:alpha_file; {the terminal as an output file}
@!bound_default:integer; {temporary for setup}
@z

% _____________________________________________________________________________
%
% [29.51]
% _____________________________________________________________________________

@x
miktex_get_pool_file_name(name_of_file);
if miktex_open_pool_file(pool_file) then
  begin c:=false;
  repeat @<Read one string, but return |false| if the
    string memory space is getting too tight for comfort@>;
  until c;
  a_close(pool_file); get_strings_started:=true;
  end
else begin
  wake_up_terminal;
  write_ln(term_out, '! I can''t read TEX.POOL.');
@.I can't read TEX.POOL@>
  get_strings_started:=false;
  return;
end
@y
name_length := strlen (pool_name);
name_of_file := xmalloc_array (ASCII_code, name_length + 1);
strcpy (stringcast(name_of_file+1), pool_name); {copy the string}
if a_open_in (pool_file, kpse_texpool_format) then
  begin c:=false;
  repeat @<Read one string, but return |false| if the
    string memory space is getting too tight for comfort@>;
  until c;
  a_close(pool_file); get_strings_started:=true;
  end
else  bad_pool('! I can''t read ', pool_name, '; bad path?')
@.I can't read TEX.POOL@>
@z

% _____________________________________________________________________________
%
% [4.52]
% _____________________________________________________________________________

@x
begin if eof(pool_file) then bad_pool('! TEX.POOL has no check sum.');
@y
begin if eof(pool_file) then bad_pool('! ', pool_name, ' has no check sum.');
@z

@x
read(pool_file,m,n); {read two digits of string length}
@y
read(pool_file,m); read(pool_file,n); {read two digits of string length}
@z

@x
    bad_pool('! TEX.POOL line doesn''t begin with two digits.');
@y
    bad_pool('! ', pool_name, ' line doesn''t begin with two digits.');
@z

% _____________________________________________________________________________
%
% [4.53]
% _____________________________________________________________________________

@x
  bad_pool('! TEX.POOL check sum doesn''t have nine digits.');
@y
  bad_pool('! ', pool_name, ' check sum doesn''t have nine digits.');
@z

@x
done: if a<>@$ then bad_pool('! TEX.POOL doesn''t match; TANGLE me again.');
@y
done: if a<>@$ then
  bad_pool('! ', pool_name, ' doesn''t match; tangle me again (or fix the path).');
@z

% _____________________________________________________________________________
%
% [5.61]
% _____________________________________________________________________________

@x
  if format_ident=0 then print_ln {\MiKTeX: eliminate misleading `(no format preloaded)'.}
  else  begin slow_print(format_ident); print_ln;
    end;
@y
  if format_ident=0 then print_ln {\MiKTeX: eliminate misleading `(no format preloaded)'.}
  else  begin slow_print(format_ident); print_ln;
    end;
if translate_filename then begin
  wterm(' (');
  fputs(translate_filename, stdout);
  wterm_ln(')');
end;
@z

% _____________________________________________________________________________
%
% [8.110]
% _____________________________________________________________________________

@x
@d min_quarterword=0 {smallest allowable value in a |quarterword|}
@d max_quarterword==255 {largest allowable value in a |quarterword|}
@d min_halfhalfword==-@"8000
@d max_halfhalfword==@"7FFF
@d min_halfword==-@"FFFFFFF {smallest allowable value in a |halfword|}
@d max_halfword==@"FFFFFFF {largest allowable value in a |halfword|}
@y
@d min_quarterword=0 {smallest allowable value in a |quarterword|}
@d max_quarterword=255 {largest allowable value in a |quarterword|}
@d min_halfword==-@"FFFFFFF {smallest allowable value in a |halfword|}
@d max_halfword==@"FFFFFFF {largest allowable value in a |halfword|}
@z

% _____________________________________________________________________________
%
% [17.236]
% _____________________________________________________________________________

@x
@d mubyte_in_code=miktex_int_base+3 {if positive then reading mubytes is active}
@d mubyte_out_code=miktex_int_base+4 {if positive then printing mubytes is active}
@d mubyte_log_code=miktex_int_base+5 {if positive then print mubytes to log and terminal}
@d spec_out_code=miktex_int_base+6 {if positive then print specials by mubytes}
@d miktex_int_pars=miktex_int_base+7 {total number of \MiKTeX's integer parameters}
@y
@d mubyte_in_code=web2c_int_base+3 {if positive then reading mubytes is active}
@d mubyte_out_code=web2c_int_base+4 {if positive then printing mubytes is active}
@d mubyte_log_code=web2c_int_base+5 {if positive then print mubytes to log and terminal}
@d spec_out_code=web2c_int_base+6 {if positive then print specials by mubytes}
@d web2c_int_pars=web2c_int_base+7 {total number of web2c's integer parameters}
@z

% _____________________________________________________________________________
%
% [18.264]
% _____________________________________________________________________________

@x
@!j:0..sup_buf_size; {index into |buffer|}
@y
@!j:0..buf_size; {index into |buffer|}
@z

% _____________________________________________________________________________
%
% [22.304]
% _____________________________________________________________________________

@x
@!input_file : array[1..sup_max_in_open] of alpha_file;
@y
@!input_file : ^alpha_file;
@z

% _____________________________________________________________________________
%
% [24.341]
% _____________________________________________________________________________

@x
@!i,@!j: 0..sup_buf_size; {more indexes for encTeX}
@y
@!i,@!j: 0..buf_size; {more indexes for encTeX}
@z

% _____________________________________________________________________________
%
% [29.518]
% _____________________________________________________________________________

@x
    must_quote:=str_pool[j]=" " or str_pool[j]="*"; incr(j);
@y
    must_quote:=str_pool[j]=" "; incr(j);
@z

@x
    must_quote:=str_pool[j]=" " or str_pool[j]="*"; incr(j);
@y
    must_quote:=str_pool[j]=" "; incr(j);
@z

@x
    must_quote:=str_pool[j]=" " or str_pool[j]="*"; incr(j);
@y
    must_quote:=str_pool[j]=" "; incr(j);
@z

@x
      if str_pool[j]="*" then
        print_char(" ")
      else if str_pool[j]="?" then
        print_char("~")
      else
        print(so(str_pool[j]));
@y
      print(so(str_pool[j]));
@z

@x
      if str_pool[j]="*" then
        print_char(" ")
      else if str_pool[j]="?" then
        print_char("~")
      else
        print(so(str_pool[j]));
@y
      print(so(str_pool[j]));
@z

@x
      if str_pool[j]="*" then
        print_char(" ")
      else if str_pool[j]="?" then
        print_char("~")
      else
        print(so(str_pool[j]));
@y
      print(so(str_pool[j]));
@z

% _____________________________________________________________________________
%
% [29.519]
% _____________________________________________________________________________

@x
for j:=str_start[a] to str_start[a+1]-1 do append_to_name(so(str_pool[j]));
@y
name_of_file:= xmalloc_array (ASCII_code, length(a)+length(n)+length(e)+1);
for j:=str_start[a] to str_start[a+1]-1 do append_to_name(so(str_pool[j]));
@z

% _____________________________________________________________________________
%
% [29.523]
% _____________________________________________________________________________

@x
for j:=1 to n do append_to_name(xord[TEX_format_default[j]]);
@y
name_of_file := xmalloc_array (ASCII_code, n+(b-a+1)+format_ext_length+1);
for j:=1 to n do append_to_name(xord[TEX_format_default[j]]);
@z

% _____________________________________________________________________________
%
% [29.532]
% _____________________________________________________________________________

@x
  while (not miktex_open_dvi_file (dvi_file)) do
@y
  while not b_open_out(dvi_file) do
@z

% _____________________________________________________________________________
%
% [29.534]
% _____________________________________________________________________________

@x
    if miktex_have_tcx_file_name then
@y
    if translate_filename then
@z

% _____________________________________________________________________________
%
% [29.536]
% _____________________________________________________________________________

@x
if eTeX_ex then
  begin; wlog_cr; wlog('entering extended mode');
  end;
end
@y
if eTeX_ex then
  begin; wlog_cr; wlog('entering extended mode');
  end;
end
if translate_filename then begin
  wlog_cr;
  wlog(' (');
  fputs(translate_filename, log_file);
  wlog(')');
  end;
end
@z

% _____________________________________________________________________________
%
% [29.537]
% _____________________________________________________________________________

@x
  if a_open_in(cur_file) then goto done;
@y
  if open_in_name_ok(stringcast(name_of_file+1))
     and a_open_in(cur_file, kpse_tex_format) then
    goto done;
@z

% _____________________________________________________________________________
%
% [30.563]
% _____________________________________________________________________________

@x
if not miktex_open_tfm_file(tfm_file,name_of_file) then abort;
@y
if not b_open_in(tfm_file) then abort;
@z

% _____________________________________________________________________________
%
% [32.642]
% _____________________________________________________________________________

@x
  miktex_close_dvi_file(dvi_file);
@y
  b_close(dvi_file);
@z

% _____________________________________________________________________________
%
% [46.1034]
% _____________________________________________________________________________

@x
  if (miktex_insert_src_special_auto) then append_src_special;
@y
  if (insert_src_special_auto) then append_src_special;
@z

% _____________________________________________________________________________
%
% [49.1275]
% _____________________________________________________________________________

@x
  if a_open_in(read_file[n]) then read_open[n]:=just_open;
@y
  if open_in_name_ok(stringcast(name_of_file+1))
     and a_open_in(read_file[n], kpse_tex_format) then
    read_open[n]:=just_open;
@z

% _____________________________________________________________________________
%
% [49.1302]
% _____________________________________________________________________________

@x
@!w: four_quarters; {four ASCII codes}
@y
@!format_engine: ^text_char;
@z

% _____________________________________________________________________________
%
% [50.1303]
% _____________________________________________________________________________

@x
@!w: four_quarters; {four ASCII codes}
@y
@!format_engine: ^text_char;
@z

% _____________________________________________________________________________
%
% [50.1307]
% _____________________________________________________________________________

@x
dump_int(@$);@/
@y
format_engine:=xmalloc_array(text_char,x+4);
dump_int(@$);@/
@z

% _____________________________________________________________________________
%
% [50.1308]
% _____________________________________________________________________________

@x
undump_int(x);
if x<>@"4D694B54 then goto bad_fmt; {not a format file}
undump_int(x);
if x<>@$ then goto bad_fmt; {check that strings are the same}
@y
undump_int(x);
if x<>@"4D694B54 then goto bad_fmt; {not a format file}
format_engine:=xmalloc_array(text_char, x);
undump_int(x);
if x<>@$ then goto bad_fmt; {check that strings are the same}
@z

% _____________________________________________________________________________
%
% [50.1323]
% _____________________________________________________________________________

@x
begin {Allocate the font arrays}
@y
begin {Allocate the font arrays}
font_bc:=xmalloc_array(eight_bits, font_max);
font_ec:=xmalloc_array(eight_bits, font_max);
@z

% _____________________________________________________________________________
%
% [50.1327]
% _____________________________________________________________________________

@x
undump_int(x);
if (x<>69069)or not eof(fmt_file) then goto bad_fmt
@y
undump_int(x);
if (x<>69069)or feof(fmt_file) then goto bad_fmt
@z

% _____________________________________________________________________________
%
% [51.1332]
% _____________________________________________________________________________

@x
miktex_process_command_line_options;
@y
miktex_process_command_line_options;
  setup_bound_var (15000)('max_strings')(max_strings);
@z

@x
miktex_allocate_memory;
@y
miktex_allocate_memory;
  buffer:=xmalloc_array (ASCII_code, buf_size);
  input_file:=xmalloc_array (alpha_file, max_in_open);
@z

% _____________________________________________________________________________
%
% [51.1337]
% _____________________________________________________________________________

@x
    if miktex_have_tcx_file_name then begin
@y
    if translate_filename then begin
@z

@x
@!init
if trie_not_ready then begin {initex without format loaded}
  trie_root:=0; trie_c[0]:=si(0); trie_ptr:=0;
end;
tini@/
@y
@!init
if trie_not_ready then begin {initex without format loaded}
  trie_root:=0; trie_c[0]:=si(0); trie_ptr:=0;
  {Allocate and initialize font arrays}
  font_bc:=xmalloc_array(eight_bits, font_max);
  font_ec:=xmalloc_array(eight_bits, font_max);
  param_base[null_font]:=-1;
end;
tini@/
@z

% _____________________________________________________________________________
%
% [53.1370]
% _____________________________________________________________________________

 @x
      miktex_write18(miktex_get_string_at(str_start[str_ptr]));
 @y
      system(conststringcast(address_of(str_pool[str_start[str_ptr]])));
 @z

% _____________________________________________________________________________
%
% [58.1618]
% _____________________________________________________________________________

@x
if miktex_have_tcx_file_name then begin
@y
if translate_filename then begin
@z

@x
  if miktex_enable_eightbit_chars_p then
@y
  if eight_bit_p then
@z

@x
@ The boolean variable |enctex_p| is set by \MiKTeX\ according to the given
@y
@ The boolean variable |enctex_p| is set by web2c according to the given
@z

% _____________________________________________________________________________
%
% [55.1633]
% _____________________________________________________________________________

@x
@ Declare system-dependent enctex functions.
@<Declare \MiKTeX\ functions@>=
function miktex_enctex_p : boolean; forward;
function miktex_have_tcx_file_name : boolean; forward;

@y
@z

% _____________________________________________________________________________
%
% [55.1634]
% _____________________________________________________________________________

@x
enctex_p:=miktex_enctex_p;
@y
@z
