%% xetex-miktex.ch:
%% 
%% Copyright (C) 2007 Christian Schenk
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
% [1.16]
% _____________________________________________________________________________

@x
@d empty=0 {symbolic name for a null constant}
@y
@d empty=0 {symbolic name for a null constant}
@d nil == null_ptr
@z

% _____________________________________________________________________________
%
% [3.26]
% _____________________________________________________________________________

@x
@!name_of_file:^UTF8_code; {we build filenames in utf8 to pass to the OS}
@y
@!name_of_file:packed array[1..file_name_size] of UTF8_code; {we build filenames in utf8 to pass to the OS}
@z

% _____________________________________________________________________________
%
% [3.33]
% _____________________________________________________________________________

@x
@d t_open_in==term_in:=i@&nput
@y
@d t_open_in==miktex_open_in
@z

% _____________________________________________________________________________
%
% [4.51]
% _____________________________________________________________________________

@x
name_of_file := xmalloc_array (UTF8_code, name_length + 1);
@y
@z

% _____________________________________________________________________________
%
% [5.58]
% _____________________________________________________________________________

@x
term_and_log: begin wterm(xchr[s]); wlog(xchr[s]);
@y
term_and_log: begin wterm(chr(s)); wlog(chr(s));
@z

@x
log_only: begin wlog(xchr[s]); incr(file_offset);
@y
log_only: begin wlog(chr(s)); incr(file_offset);
@z

@x
term_only: begin wterm(xchr[s]); incr(term_offset);
@y
term_only: begin wterm(chr(s)); incr(term_offset);
@z

@x
othercases write(write_file[selector],xchr[s])
@y
othercases write(write_file[selector],chr(s))
@z

% _____________________________________________________________________________
%
% [5.64]
% _____________________________________________________________________________

@x
if translate_filename then begin
  wterm(' (WARNING: translate-file "');
  fputs(translate_filename, stdout);
  wterm_ln('" ignored)');
end;
@y
@z

% _____________________________________________________________________________
%
% [8.114]
% _____________________________________________________________________________

@x
if (max_font_max<min_halfhalfword)or(max_font_max>max_halfhalfword) then bad:=15;
@y
if (max_font_max<min_quarterword)or(max_font_max>max_quarterword) then bad:=15;
@z

% _____________________________________________________________________________
%
% [8.116]
% _____________________________________________________________________________

@x
@!halfhalfword  = min_halfhalfword..max_halfhalfword; {1/2 of a halfword}
@y
@z

@x
  2: (@!b0:halfhalfword; @!b1:halfhalfword);
@y
  2: (@!b0:quarterword; @!b1:quarterword);
@z

@x
  4: (@!qqqq:four_quarters);
@y
  4: (@!qqqq:four_quarters);
  5: (@!ptr:void_pointer);
@z

% _____________________________________________________________________________
%
% [10.149]
% _____________________________________________________________________________

@x
    native_glyph_info_ptr(dest) := xmalloc_array(char, glyph_count * native_glyph_info_size);
@y
    native_glyph_info_ptr(dest) := xmalloc_char_array(glyph_count * native_glyph_info_size);
@z

% _____________________________________________________________________________
%
% [17.240]
% _____________________________________________________________________________

@x
@d web2c_int_pars=web2c_int_base+3 {total number of web2c's integer parameters}
@y
@d miktex_int_pars=miktex_int_base+3 {total number of \MiKTeX's integer parameters}
@z

@x
@d int_pars=miktex_int_pars {total number of integer parameters}
@#
@d etex_int_base=tex_int_pars {base for \eTeX's integer parameters}
@y
@d etex_int_base=miktex_int_pars {base for \eTeX's integer parameters}
@z

% _____________________________________________________________________________
%
% [22.308]
% _____________________________________________________________________________

@x
@!input_file : ^unicode_file;
@y
@!input_file : array[1..sup_max_in_open] of unicode_file;
@z

% _____________________________________________________________________________
%
% [29.523]
% _____________________________________________________________________________

@x
    if (str_pool[j]=" ") then must_quote:=true
@y
    if (str_pool[j]=" " or str_pool[j]="*") then must_quote:=true
@z

@x
    if (str_pool[j]=" ") then must_quote:=true
@y
    if (str_pool[j]=" " or str_pool[j]="*") then must_quote:=true
@z

@x
    if (str_pool[j]=" ") then must_quote:=true
@y
    if (str_pool[j]=" " or str_pool[j]="*") then must_quote:=true
@z

@x
    print(str_pool[j]);
@y
    if str_pool[j]="*" then
      print_char(" ")
    else if str_pool[j]="?" then
      print_char("~")
    else
      print(str_pool[j]);
@z

@x
    print(str_pool[j]);
@y
    if str_pool[j]="*" then
      print_char(" ")
    else if str_pool[j]="?" then
      print_char("~")
    else
      print(str_pool[j]);
@z

@x
    print(str_pool[j]);
@y
    if str_pool[j]="*" then
      print_char(" ")
    else if str_pool[j]="?" then
      print_char("~")
    else
      print(str_pool[j]);
@z

% _____________________________________________________________________________
%
% [29.524]
% _____________________________________________________________________________

@x
name_of_file:= xmalloc_array (UTF8_code, (length(a)+length(n)+length(e))*3+1);
@y
@z

% _____________________________________________________________________________
%
% [29.528]
% _____________________________________________________________________________

@x
name_of_file := xmalloc_array (UTF8_code, n+(b-a+1)+format_ext_length+1);
@y
@z

% _____________________________________________________________________________
%
% [29.530]
% _____________________________________________________________________________

@x
function a_make_name_string(var f:alpha_file):str_number;
@y
function a_make_name_string:str_number;
@z

% _____________________________________________________________________________
%
% [29.539]
% _____________________________________________________________________________

@x
log_name:=a_make_name_string(log_file);
@y
log_name:=a_make_name_string;
@z

% _____________________________________________________________________________
%
% [29.541]
% _____________________________________________________________________________

@x
if translate_filename then begin
  wlog_cr;
  wlog(' (WARNING: translate-file "');
  fputs(translate_filename, log_file);
  wlog('" ignored)');
  end;
end
@y
@z

% _____________________________________________________________________________
%
% [29.542]
% _____________________________________________________________________________

@x
var temp_str: str_number;
@y
var temp_str: str_number;
k: integer;
@z

@x
  if open_in_name_ok(stringcast(name_of_file+1))
     and u_open_in(cur_file, kpse_tex_format, XeTeX_default_input_mode, XeTeX_default_input_encoding) then
@y
  if u_open_in(cur_file, XeTeX_default_input_mode, XeTeX_default_input_encoding) then
@z

@x
done: name:=a_make_name_string(cur_file);
@y
done: name:=a_make_name_string;
@z

% _____________________________________________________________________________
%
% [30.569]
% _____________________________________________________________________________

@x
if b_open_in(tfm_file) then begin
@y
if miktex_open_tfm_file(tfm_file,name_of_file) then begin
@z

% _____________________________________________________________________________
%
% [32.626]
% _____________________________________________________________________________

@x
          if q = p then break
@y
          if q = p then c4p_break_loop
@z

% _____________________________________________________________________________
%
% [32.650]
% _____________________________________________________________________________

@x
  print_nl("Output written on "); print(output_file_name);
@y
  print_nl("Output written on "); print_file_name(0, output_file_name, 0);
@z

% _____________________________________________________________________________
%
% [35.714]
% _____________________________________________________________________________

@x
    if ot_assembly_ptr<>nil then
@y
    if is_null_ptr(ot_assembly_ptr) then
@z

% _____________________________________________________________________________
%
% [35.716]
% _____________________________________________________________________________

@x
  if ot_assembly_ptr<>nil then goto found;
@y
  if is_null_ptr(ot_assembly_ptr) then goto found;
@z

% _____________________________________________________________________________
%
% [49.1289]
% _____________________________________________________________________________

@x
var c:0..1; {1 for \.{\\openin}, 0 for \.{\\closein}}
@y
var c:0..1; {1 for \.{\\openin}, 0 for \.{\\closein}}
k:integer;
@z

@x
  if open_in_name_ok(stringcast(name_of_file+1))
     and u_open_in(read_file[n], kpse_tex_format, XeTeX_default_input_mode, XeTeX_default_input_encoding) then
@y
  if u_open_in(read_file[n], XeTeX_default_input_mode, XeTeX_default_input_encoding) then
@z

% _____________________________________________________________________________
%
% [50.1321]
% _____________________________________________________________________________

@x
format_engine:=xmalloc_array(char,x+4);
@y
@z

% _____________________________________________________________________________
%
% [50.1322]
% _____________________________________________________________________________

@x
format_engine:=xmalloc_array(char, x);
@y
@z

% _____________________________________________________________________________
%
% [50.1337]
% _____________________________________________________________________________

@x
font_mapping:=xmalloc_array(void_pointer, font_max);
font_layout_engine:=xmalloc_array(void_pointer, font_max);
font_flags:=xmalloc_array(char, font_max);
font_letter_space:=xmalloc_array(scaled, font_max);
font_bc:=xmalloc_array(UTF16_code, font_max);
font_ec:=xmalloc_array(UTF16_code, font_max);
@y
@z

% _____________________________________________________________________________
%
% [51.1346]
% _____________________________________________________________________________

@x
  setup_bound_var (15000)('max_strings')(max_strings);
@y
@z

@x
  input_file:=xmalloc_array (unicode_file, max_in_open);
@y
@z

% _____________________________________________________________________________
%
% [51.1347]
% _____________________________________________________________________________

@x
    print(log_name); print_char(".");
@y
    print_file_name(0, log_name, 0); print_char(".");
@z

% _____________________________________________________________________________
%
% [51.1351]
% _____________________________________________________________________________

@x
  font_mapping:=xmalloc_array(void_pointer, font_max);
  font_layout_engine:=xmalloc_array(void_pointer, font_max);
  font_flags:=xmalloc_array(char, font_max);
  font_letter_space:=xmalloc_array(scaled, font_max);
  font_bc:=xmalloc_array(UTF16_code, font_max);
  font_ec:=xmalloc_array(UTF16_code, font_max);
@y
@z

% _____________________________________________________________________________
%
% [53.1372]
% _____________________________________________________________________________

@x
@d total_pic_node_size(#) == (pic_node_size + (pic_path_length(#) + sizeof(memory_word) - 1) div sizeof(memory_word))
@y
@d total_pic_node_size(#) == (pic_node_size + (pic_path_length(#) + sizeof(mem[0]) - 1) div sizeof(mem[0]))
@z

% _____________________________________________________________________________
%
% [53.1388]
% _____________________________________________________________________________

@x
      if name_of_file then libc_free(name_of_file);
      name_of_file := xmalloc(cur_length * 3 + 2);
      k := 0;
      for d:=0 to cur_length-1 do append_to_name(str_pool[str_start_macro(str_ptr)+d]);
      name_of_file[k+1] := 0;
      system(name_of_file + 1);
@y
      miktex_write18(miktex_get_string_at(str_start_macro(str_ptr)));
@z

% _____________________________________________________________________________
%
% [53.1400]
% _____________________________________________________________________________

@x
		new_whatsit(pic_node, pic_node_size + (strlen(pic_path) + sizeof(memory_word) - 1) div sizeof(memory_word));
@y
		new_whatsit(pic_node, pic_node_size + (strlen(pic_path) + sizeof(mem[0]) - 1) div sizeof(mem[0]));
@z

% _____________________________________________________________________________
%
% [55.1635]
% _____________________________________________________________________________

@x
@ Define Web2C compatibility functions.
@y
@ Define XeTeX compatibility functions.

@<Declare \MiKTeX\ functions@>=

function cast_to_integer : integer; forward;@t\2@>@/
function cast_to_ptr : void_pointer; forward;@t\2@>@/
function cast_to_ushort : integer; forward;@t\2@>@/
function is_null_ptr : boolean; forward;@t\2@>@/
function linebreak_next : integer; forward;@t\2@>@/
function miktex_etex_p: boolean; forward;@t\2@>@/
function null_ptr : void_pointer; forward;@t\2@>@/
function xmalloc_char_array : void_pointer; forward;@t\2@>@/

@ Define Web2C compatibility functions.
@z

% _____________________________________________________________________________
%
% [57.1657]
% _____________________________________________________________________________

@x
	l := native_node_size + (n * sizeof(UTF16_code) + sizeof(memory_word) - 1) div sizeof(memory_word);
@y
	l := native_node_size + (n * sizeof(name_of_file16[0]) + sizeof(mem[0]) - 1) div sizeof(mem[0]);
@z

@x
	font_engine := find_native_font(name_of_file + 1, actual_size);
@y
	font_engine := find_native_font(name_of_file, actual_size);
@z
