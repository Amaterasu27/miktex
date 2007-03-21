%% pdftex-miktex.ch:
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
program TEX; {all file names are defined dynamically}
@y
program PDFTEX; {all file names are defined dynamically}
@z

% _____________________________________________________________________________
%
% [12.181]
% _____________________________________________________________________________

@x
@!k:integer; {index into |mem|, |eqtb|, etc.}
@y
@!k:integer; {index into |mem|, |eqtb|, etc.}
@!font_k:integer; {index into |font_base|, etc.}
@z

% _____________________________________________________________________________
%
% [19.286]
% _____________________________________________________________________________

@x
@!j:0..buf_size; {index into |buffer|}
@y
@!j:0..buf_size_def; {index into |buffer|}
@z

% _____________________________________________________________________________
%
% [27.449]
% _____________________________________________________________________________

% todo

@x
    if shell_enabled_p then cur_val := 1
    else cur_val := 0;
@y
    cur_val := 0;
@z

% _____________________________________________________________________________
%
% [34.676]
% _____________________________________________________________________________

% todo

@x
        pdf_mem := xrealloc_array(pdf_mem, integer, pdf_mem_size);
@y
        miktex_reallocate (pdf_mem, pdf_mem_size);
@z

% _____________________________________________________________________________
%
% [35.684]
% _____________________________________________________________________________

% todo

@x
        pdf_os_buf := xrealloc_array(pdf_os_buf, eight_bits, pdf_os_buf_size);
@y
        miktex_reallocate(pdf_os_buf, pdf_os_buf_size);
@z

% _____________________________________________________________________________
%
% [36.691]
% _____________________________________________________________________________

% fix pointer comparism

@x
               (pdf_font_map[k] = pdf_font_map[f]) and
@y
               (miktex_ptr_equal(pdf_font_map[k], pdf_font_map[f])) and
@z

% _____________________________________________________________________________
%
% [37.695]
% _____________________________________________________________________________

@x
@!obj_tab:^obj_entry;
@y
@!obj_tab:array [0..0] of obj_entry;
@z

% _____________________________________________________________________________
%
% [37.697]
% _____________________________________________________________________________

% todo

@x
        dest_names := xrealloc_array(dest_names, dest_name_entry, dest_names_size);
@y
        miktex_reallocate(dest_names, dest_names_size);
@z

@x
        obj_tab := xrealloc_array(obj_tab, obj_entry, obj_tab_size);
@y
        miktex_reallocate(obj_tab, obj_tab_size);
@z

% _____________________________________________________________________________
%
% [38.703]
% _____________________________________________________________________________

@x
@!pdf_font_type: ^eight_bits; {the type of font}
@!pdf_font_attr: ^str_number; {pointer to additional attributes}
@y
@!pdf_font_type: array[0..0] of eight_bits; {the type of font}
@!pdf_font_attr: array[0..0] of str_number; {pointer to additional attributes}
@z

% _____________________________________________________________________________
%
% [38.706]
% _____________________________________________________________________________

 @x
@!pdf_font_map: ^fm_entry_ptr; {pointer into AVL tree of font mappings}
 @y
@!pdf_font_map: array[0..0] of fm_entry_ptr; {pointer into AVL tree of font mappings}
 @z
  
% _____________________________________________________________________________
%
% [38.708]
% _____________________________________________________________________________

@x
@d vf_byte == getc(vf_file) {get a byte from\.{VF} file}
@y
@d vf_byte == get_byte(vf_file) {get a byte from\.{VF} file}
@z

@x
@!vf_packet_base: ^integer; {base addresses of character packets from virtual fonts}
@y
@!vf_packet_base: array[0..0] of integer; {base addresses of character packets from virtual fonts}
@z

@x
@!vf_default_font: ^internal_font_number; {default font in a \.{VF} file}
@!vf_local_font_num: ^internal_font_number; {number of local fonts in a \.{VF} file}
@y
@!vf_default_font: array[0..0] of internal_font_number;
@!vf_local_font_num: array[0..0] of internal_font_number;
@z

@x
@!vf_e_fnts: ^integer; {external font numbers}
@!vf_i_fnts: ^internal_font_number; {corresponding internal font numbers}
@y
@!vf_e_fnts: array[0..0] of integer; {external font numbers}
@!vf_i_fnts: array[0..0] of internal_font_number; {corresponding internal font numbers}
@z

% _____________________________________________________________________________
%
% [38.722]
% _____________________________________________________________________________

@x
@ Some functions for processing character packets.
@y
@ Some functions for processing character packets.

@d char_done = 72
@z

% _____________________________________________________________________________
%
% [39.787]
% _____________________________________________________________________________

% todo

@x
if pdf_pk_mode <> null then begin
    kpse_init_prog('PDFTEX', fixed_pk_resolution,
                   make_cstring(tokens_to_string(pdf_pk_mode)), nil);
    flush_string;
end else
    kpse_init_prog('PDFTEX', fixed_pk_resolution, nil, nil);
if not kpse_var_value('MKTEXPK') then
    kpse_set_program_enabled (kpse_pk_format, 1, kpse_src_cmdline);
@y
@z

% _____________________________________________________________________________
%
% [39.801]
% _____________________________________________________________________________

@x
pdf_print("/Producer (pdfTeX-");
@y
pdf_print("/Producer (MiKTeX pdfTeX-");
@z

% _____________________________________________________________________________
%
% [40.813]
% _____________________________________________________________________________

@x
@!pdf_font_blink: ^internal_font_number; {link to base font (used for expanded fonts only)}
@!pdf_font_elink: ^internal_font_number; {link to expanded fonts (used for base fonts only)}
@!pdf_font_stretch: ^integer; {limit of stretching}
@!pdf_font_shrink: ^integer; {limit of shrinking}
@!pdf_font_step: ^integer;  {amount of one step of expansion}
@!pdf_font_expand_ratio: ^integer; {expansion ratio of a particular font}
@!pdf_font_auto_expand: ^boolean; {this font is auto-expanded?}
@!pdf_font_lp_base: ^integer; {base of left-protruding factor}
@!pdf_font_rp_base: ^integer; {base of right-protruding factor}
@!pdf_font_ef_base: ^integer; {base of font expansion factor}
@y
@!pdf_font_blink: array[0..0] of internal_font_number; {link to base font (used for expanded fonts only)}
@!pdf_font_elink: array[0..0] of internal_font_number; {link to expanded fonts (used for base fonts only)}
@!pdf_font_stretch: array[0..0] of integer; {limit of stretching}
@!pdf_font_shrink: array[0..0] of integer; {limit of shrinking}
@!pdf_font_step: array[0..0] of integer;  {amount of one step of expansion}
@!pdf_font_expand_ratio: array[0..0] of integer; {expansion ratio of a particular font}
@!pdf_font_auto_expand: array[0..0] of boolean; {this font is auto-expanded?}
@!pdf_font_lp_base: array[0..0] of integer; {base of left-protruding factor}
@!pdf_font_rp_base: array[0..0] of integer; {base of right-protruding factor}
@!pdf_font_ef_base: array[0..0] of integer; {base of font expansion factor}
@z

% _____________________________________________________________________________
%
% [57.1471]
% _____________________________________________________________________________

@x
@<Dump the string pool@>;
@y
dump_int(pdf_output);
dump_int(pdf_compress_level);
dump_int(pdf_decimal_digits);
dump_int(pdf_move_chars);
dump_int(pdf_image_resolution);
dump_int(pdf_pk_resolution);
dump_int(pdf_unique_resname);
{dump_int(pdf_option_pdf_minor_version);}
dump_int(pdf_option_always_use_pdfpagebox);
dump_int(pdf_option_pdf_inclusion_errorlevel);
dump_int(pdf_h_origin);
dump_int(pdf_v_origin);
dump_int(pdf_page_width);
dump_int(pdf_page_height);
@<Dump the string pool@>;
@z

% _____________________________________________________________________________
%
% [57.1472]
% _____________________________________________________________________________

@x
@<Undump the string pool@>;
@y
undump_int(pdf_output);   {undump |pdf_output| mode flag}
undump_int(pdf_compress_level);
undump_int(pdf_decimal_digits);
undump_int(pdf_move_chars);
undump_int(pdf_image_resolution);
undump_int(pdf_pk_resolution);
undump_int(pdf_unique_resname);
{undump_int(pdf_option_pdf_minor_version);}
undump_int(pdf_option_always_use_pdfpagebox);
undump_int(pdf_option_pdf_inclusion_errorlevel);
undump_int(pdf_h_origin);
undump_int(pdf_v_origin);
undump_int(pdf_page_width);
undump_int(pdf_page_height);
@<Undump the string pool@>;
@z

% _____________________________________________________________________________
%
% [58.1501]
% _____________________________________________________________________________

@x
main_control; {come to life}
@y
make_pdftex_banner;
main_control; {come to life}
@z

% _____________________________________________________________________________
%
% [58.1506]
% _____________________________________________________________________________

@x
pdf_init_map_file('pdftex.map');
@y
pdf_init_map_file('psfonts.map');
@z

% _____________________________________________________________________________
%
% [60.1609]
% _____________________________________________________________________________

@x
@!dest_names: ^dest_name_entry;
@y
@!dest_names: array [0..0] of dest_name_entry;
@z

% _____________________________________________________________________________
%
% [61.1641]
% _____________________________________________________________________________

@x
@!eof_seen : array[1..max_in_open] of boolean; {has eof been seen?}
@y
@!eof_seen : array[1..max_in_open_def] of boolean; {has eof been seen?}
@z

% _____________________________________________________________________________
%
% [61.1659]
% _____________________________________________________________________________

@x
@!save_pointer=0..save_size; {index into |save_stack|}
@y
@!save_pointer=0..save_size_def; {index into |save_stack|}
@z

% _____________________________________________________________________________
%
% [61.1660]
% _____________________________________________________________________________

@x
var p:0..nest_size; {index into |nest|}
@y
var p:0..nest_size_def; {index into |nest|}
@z

% _____________________________________________________________________________
%
% [61.1756]
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
% [61.1757]
% _____________________________________________________________________________

@x
var i:0..max_in_open; {index into |grp_stack|}
@y
var i:0..max_in_open_def; {index into |grp_stack|}
@z

% _____________________________________________________________________________
%
% [61.1759]
% _____________________________________________________________________________

@x
var i:0..max_in_open; {index into |if_stack|}
@y
var i:0..max_in_open_def; {index into |if_stack|}
@z

% _____________________________________________________________________________
%
% [61.1814]
% _____________________________________________________________________________

@x
@!init destroy_marks: for i:=top_mark_code to split_bot_mark_code do
  begin get_sa_ptr;
  if cur_ptr<>null then
    begin delete_token_ref(cur_ptr); put_sa_ptr(null);
    end;
  end;
tini
@y
@!destroy_marks: init for i:=top_mark_code to split_bot_mark_code do
  begin get_sa_ptr;
  if cur_ptr<>null then
    begin delete_token_ref(cur_ptr); put_sa_ptr(null);
    end;
  end;
tini
@z

% _____________________________________________________________________________
%
% [63.1861]
% _____________________________________________________________________________

@x
@* \[54] \MiKTeX-dependent changes.
@y
@* \[54] \MiKTeX-dependent changes.

@<Set init...@>=

for font_k := font_base to font_max do begin
    for k := 0 to 31 do begin
        pdf_char_used[font_k, k] := 0;
    end;
    pdf_font_size[font_k] := 0;
    pdf_font_num[font_k] := 0;
    pdf_font_map[font_k] := 0;
    pdf_font_type[font_k] := new_font_type;
    pdf_font_attr[font_k] := "";
    pdf_font_blink[font_k] := null_font;
    pdf_font_elink[font_k] := null_font;
    pdf_font_stretch[font_k] := null_font;
    pdf_font_shrink[font_k] := null_font;
    pdf_font_step[font_k] := 0;
    pdf_font_expand_ratio[font_k] := 0;
    pdf_font_auto_expand[font_k] := false;
    pdf_font_lp_base[font_k] := 0;
    pdf_font_rp_base[font_k] := 0;
    pdf_font_ef_base[font_k] := 0;
end;
@z

% _____________________________________________________________________________
%
% [63.1864]
% _____________________________________________________________________________

@x
@<Declare \MiKTeX\ functions@>=
@y
@<Declare \MiKTeX\ functions@>=

function get_nullstr: str_number;
begin
    get_nullstr := "";
end;

function miktex_ptr_equal:boolean; forward;@t\2@>@/
function colorstackused: integer; forward;@t\2@>@/
function miktex_halt_on_error_p : boolean; forward;@t\2@>@/
function is_quote_bad: boolean; forward;@t\2@>@/
function getllx: scaled; forward;@t\2@>@/
function getlly: scaled; forward;@t\2@>@/
function geturx: scaled; forward;@t\2@>@/
function getury: scaled; forward;@t\2@>@/
function matrixused: boolean; forward;@t\2@>@/
function packet_byte : eight_bits; forward;@t\2@>@/
function get_resname_prefix : str_number; forward;@t\2@>@/
@z
