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
% [1.2]
% _____________________________________________________________________________

@x
@d banner==pdfeTeX_banner
@y
@d banner==pdfTeX_banner
@z

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
% [18.240]
% _____________________________________________________________________________

@x
@d frozen_null_font=frozen_control_sequence+11
  {permanent `\.{\\nullfont}'}
@y
@d frozen_null_font=frozen_control_sequence+12
  {permanent `\.{\\nullfont}'}
@z

% _____________________________________________________________________________
%
% [19.286]
% _____________________________________________________________________________

@x
@!j:0..buf_size; {index into |buffer|}
@y
@!j:0..sup_buf_size; {index into |buffer|}
@z

% _____________________________________________________________________________
%
% [27.449]
% _____________________________________________________________________________

@x
    if shell_enabled_p then cur_val := 1
    else cur_val := 0;
@y
    if miktex_write18_p then cur_val := 1
    else cur_val := 0;
@z

% _____________________________________________________________________________
%
% [31.602]
% _____________________________________________________________________________

@x
      begin i := char_tag(char_info(f)(c));
@y
      begin i := char_tag(orig_char_info(f)(c));
@z

% _____________________________________________________________________________
%
% [34.671]
% _____________________________________________________________________________

@x
@d is_valid_char(#)==((font_bc[f] <= #) and (# <= font_ec[f]) and
                      char_exists(char_info(f)(#)))
@y
@d is_valid_char(#)==((font_bc[f] <= #) and (# <= font_ec[f]) and
                      char_exists(orig_char_info(f)(#)))
@z

@x
function get_charwidth(f: internal_font_number; c: eight_bits): scaled;
begin
    if is_valid_char(c) then
        get_charwidth := char_width(f)(char_info(f)(c))
    else
        get_charwidth := 0;
end;

function get_charheight(f: internal_font_number; c: eight_bits): scaled;
begin
    if is_valid_char(c) then
        get_charheight := char_height(f)(height_depth(char_info(f)(c)))
    else
        get_charheight := 0;
end;

function get_chardepth(f: internal_font_number; c: eight_bits): scaled;
begin
    if is_valid_char(c) then
        get_chardepth := char_depth(f)(height_depth(char_info(f)(c)))
    else
        get_chardepth := 0;
end;
@y
function get_charwidth(f: internal_font_number; c: eight_bits): scaled;
begin
    if is_valid_char(c) then
        get_charwidth := char_width(f)(orig_char_info(f)(c))
    else
        get_charwidth := 0;
end;

function get_charheight(f: internal_font_number; c: eight_bits): scaled;
begin
    if is_valid_char(c) then
        get_charheight := char_height(f)(height_depth(orig_char_info(f)(c)))
    else
        get_charheight := 0;
end;

function get_chardepth(f: internal_font_number; c: eight_bits): scaled;
begin
    if is_valid_char(c) then
        get_chardepth := char_depth(f)(height_depth(orig_char_info(f)(c)))
    else
        get_chardepth := 0;
end;
@z

% _____________________________________________________________________________
%
% [34.675]
% _____________________________________________________________________________

@x
pdf_mem_size := inf_pdf_mem_size; {allocated size of |pdf_mem| array}
@y
@z

% _____________________________________________________________________________
%
% [34.676]
% _____________________________________________________________________________

@x
        pdf_mem := xrealloc_array(pdf_mem, integer, pdf_mem_size);
@y
        miktex_reallocate (pdf_mem, pdf_mem_size);
@z

% _____________________________________________________________________________
%
% [35.679]
% _____________________________________________________________________________

@x
pdf_os_buf_size := inf_pdf_os_buf_size;
@y
@z

% _____________________________________________________________________________
%
% [35.684]
% _____________________________________________________________________________

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
% [37.696]
% _____________________________________________________________________________

@x
obj_tab_size := inf_obj_tab_size; {allocated size of |obj_tab| array}
dest_names_size := inf_dest_names_size; {allocated size of |dest_names| array}
@y
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
% [38.711]
% _____________________________________________________________________________

@x
    i := getc(vf_file);
@y
    i := get_byte(vf_file);
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
% [39.726]
% _____________________________________________________________________________

@x
label reswitch, move_past, fin_rule, next_p;
@y
label reswitch, move_past, fin_rule, next_p, found, continue;
@z

@x (MLTeX) l. 17462
  if is_valid_char(c) then
      output_one_char(c)
  else
      char_warning(f, c);
  cur_h:=cur_h+char_width(f)(char_info(f)(c));
@y
  if is_valid_char(c) then begin
      output_one_char(c);
      cur_h:=cur_h+char_width(f)(char_info(f)(c));
      goto continue;
  end;
  if mltex_enabled_p then
    @<(\pdfTeX) Output a substitution, |goto continue| if not possible@>;
continue:
@z

% _____________________________________________________________________________
%
% [39.735]
% _____________________________________________________________________________

@x
@ The |pdf_vlist_out| routine is similar to |pdf_hlist_out|, but a bit simpler.
@y
@ @<(\pdfTeX) Output a substitution, |goto continue| if not possible@>=
  begin
  @<Get substitution information, check it, goto |found|
  if all is ok, otherwise goto |continue|@>;
found: @<Print character substition tracing log@>;
  @<(\pdfTeX) Rebuild character using substitution information@>;
  end

@ @<(\pdfTeX) Rebuild character using substitution information@>=
  base_x_height:=x_height(f);
  base_slant:=slant(f)/float_constant(65536);
@^real division@>
  accent_slant:=base_slant; {slant of accent character font}
  base_width:=char_width(f)(ib_c);
  base_height:=char_height(f)(height_depth(ib_c));
  accent_width:=char_width(f)(ia_c);
  accent_height:=char_height(f)(height_depth(ia_c));
  @/{compute necessary horizontal shift (don't forget slant)}@/
  delta:=round((base_width-accent_width)/float_constant(2)+
            base_height*base_slant-base_x_height*accent_slant);
@^real multiplication@>
@^real addition@>
  @/{1. For centering/horizontal shifting insert a kern node.}@/
  cur_h:=cur_h+delta;
  @/{2. Then insert the accent character possibly shifted up or down.}@/
  if ((base_height<>base_x_height) and (accent_height>0)) then
    begin {the accent must be shifted up or down}
    cur_v:=base_line+(base_x_height-base_height);
    output_one_char(accent_c);
    cur_v:=base_line;
    end
  else begin
    output_one_char(accent_c);
    end;
  cur_h:=cur_h+accent_width;
  @/{3. For centering/horizontal shifting insert another kern node.}@/
  cur_h:=cur_h+(-accent_width-delta);
  @/{4. Output the base character.}@/
  output_one_char(base_c);
  cur_h:=cur_h+base_width;

@ The |pdf_vlist_out| routine is similar to |pdf_hlist_out|, but a bit simpler.
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
% [56.1429]
% _____________________________________________________________________________

@x
    if s>0 then
      begin if s=font_size[f] then goto common_ending;
      end
    else if font_size[f]=xn_over_d(font_dsize[f],-s,1000) then
      goto common_ending;
@y
    if pdf_font_step[f] = 0 then begin
       if s>0 then
         begin if s=font_size[f] then goto common_ending;
         end
       else if font_size[f]=xn_over_d(font_dsize[f],-s,1000) then
         goto common_ending;
    end
@z

% _____________________________________________________________________________
%
% [57.1499]
% _____________________________________________________________________________

@x
pdf_mem := xrealloc_array(pdf_mem, integer, pdf_mem_size);
@y
miktex_reallocate(pdf_mem, pdf_mem_size);
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
% [61.1641]
% _____________________________________________________________________________

@x
@!eof_seen : array[1..max_in_open] of boolean; {has eof been seen?}
@y
@!eof_seen : ^boolean; {has eof been seen?}
@z

% _____________________________________________________________________________
%
% [61.1659]
% _____________________________________________________________________________

@x
@!save_pointer=0..save_size; {index into |save_stack|}
@y
@!save_pointer=0..sup_save_size; {index into |save_stack|}
@z

% _____________________________________________________________________________
%
% [61.1660]
% _____________________________________________________________________________

@x
var p:0..nest_size; {index into |nest|}
@y
var p:0..sup_nest_size; {index into |nest|}
@z

% _____________________________________________________________________________
%
% [61.1756]
% _____________________________________________________________________________

@x
@!grp_stack : array[0..max_in_open] of save_pointer; {initial |cur_boundary|}
@!if_stack : array[0..max_in_open] of pointer; {initial |cond_ptr|}
@y
@!grp_stack : ^save_pointer; {initial |cur_boundary|}
@!if_stack : ^pointer; {initial |cond_ptr|}
@z

% _____________________________________________________________________________
%
% [61.1757]
% _____________________________________________________________________________

@x
var i:0..max_in_open; {index into |grp_stack|}
@y
var i:0..sup_max_in_open; {index into |grp_stack|}
@z

% _____________________________________________________________________________
%
% [61.1759]
% _____________________________________________________________________________

@x
var i:0..max_in_open; {index into |if_stack|}
@y
var i:0..sup_max_in_open; {index into |if_stack|}
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
    pdf_font_kn_bs_base[font_k] := 0;
    pdf_font_st_bs_base[font_k] := 0;
    pdf_font_sh_bs_base[font_k] := 0;
    pdf_font_kn_bc_base[font_k] := 0;
    pdf_font_kn_ac_base[font_k] := 0;
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
