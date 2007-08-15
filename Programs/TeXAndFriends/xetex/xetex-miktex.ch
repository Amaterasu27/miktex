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
@d t_open_in==do_nothing
@z

% _____________________________________________________________________________
%
% [4.51]
% _____________________________________________________________________________

@x
name_of_file := xmalloc_array (UTF8_code, name_length + 1);
@y
name_of_file := xmalloc_array (UTF8_code, name_length + 1);
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
@y
@z

% _____________________________________________________________________________
%
% [18.268]
% _____________________________________________________________________________

@x
@!j:0..buf_size; {index into |buffer|}
@y
@!j:0..sup_buf_size; {index into |buffer|}
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
% [30.569]
% _____________________________________________________________________________

@x
if b_open_in(tfm_file) then begin
  file_opened:=true
@y
if not miktex_open_tfm_file(tfm_file,name_of_file) then abort;
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
      name_of_file := miktex_reallocate(name_of_file, cur_length * 3 + 2);
      k := 0;
      for d:=0 to cur_length-1 do append_to_name(str_pool[str_start_macro(str_ptr)+d]);
      name_of_file[k+1] := 0;
      miktex_write18(name_of_file);
      name_of_file := miktex_reallocate(name_of_file, file_name_size_plus_one);
@z

% _____________________________________________________________________________
%
% [54.1442]
% _____________________________________________________________________________

@x
@!save_pointer=0..save_size; {index into |save_stack|}
@y
@!save_pointer=0..sup_save_size; {index into |save_stack|}
@z
