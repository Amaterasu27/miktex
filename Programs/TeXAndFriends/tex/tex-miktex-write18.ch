%% tex-miktex-write18.ch: implement write18
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

%% ////////////////////////////////////////////////////////////////////////////
%% //                                                                        //
%% //                      INSPIRED BY WEB2C'S TEX.CH                        //
%% //                                                                        //
%% ////////////////////////////////////////////////////////////////////////////

% _____________________________________________________________________________
%
% [53.1350]
% _____________________________________________________________________________

@x
  else if cur_val>15 then cur_val:=16;
@y
  else if cur_val>15 and cur_val<> 18 then cur_val:=16;
@z

% _____________________________________________________________________________
%
% [53.1370]
% _____________________________________________________________________________

@x
procedure write_out(@!p:pointer);
var old_setting:0..max_selector; {holds print |selector|}
@y
procedure write_out(@!p:pointer);
var old_setting:0..max_selector; {holds print |selector|}
@!d:integer;
@!clobbered:boolean;
@z

@x
if write_open[j] then selector:=j
@y
if j=18 then selector:=new_string
else if write_open[j] then selector:=j
@z

@x
flush_list(def_ref); selector:=old_setting;
@y
flush_list(def_ref);
if j=18 then
  begin if (tracing_online<=0) then
    selector:=log_only  {Show what we're doing in the log file.}
  else selector:=term_and_log;  {Show what we're doing.}
  {If the log file isn't open yet, we can only send output to the terminal.
   Calling |open_log_file| from here seems to result in bad data in the log.}
  if not log_opened then selector:=term_only;
  print_nl("system(");
  for d:=0 to cur_length-1 do
    begin {|print| gives up if passed |str_ptr|, so do it by hand.}
    print(so(str_pool[str_start[str_ptr]+d])); {N.B.: not |print_char|}
    end;
  print(")...");
  if miktex_write18_p then
    begin str_room(1); append_char(0); {Append a null byte to the expansion.}
    clobbered:=false;
    for d:=0 to cur_length-1 do {Convert to external character set.}
      begin str_pool[str_start[str_ptr]+d]:=xchr[str_pool[str_start[str_ptr]+d]];
      if (str_pool[str_start[str_ptr]+d]=null_code)
         and (d<cur_length-1) then clobbered:=true;
        {minimal checking: NUL not allowed in argument string of |system|()}
      end;
    if clobbered then print("clobbered")
    else begin {We have the string; run system(3). We don't have anything
            reasonable to do with the return status, unfortunately discard it.}
      miktex_write18(miktex_get_string_at(str_start[str_ptr]));
      print("executed");
      end;
    end
  else begin print("disabled");
  end;
  print_char("."); print_nl(""); print_ln;
  pool_ptr:=str_start[str_ptr];  {erase the string}
end;
selector:=old_setting;
@z

% _____________________________________________________________________________
%
% [54.1379]
% _____________________________________________________________________________

@x
@* \[54/MiKTeX] System-dependent changes for MiKTeX.
@y
@* \[54/MiKTeX] System-dependent changes for MiKTeX.

@ Forward declaration of \MiKTeX\ functions.

@<Declare \MiKTeX\ functions@>=

function miktex_write18_p : boolean; forward;@t\2@>@/

@ To be able to determine whether \.{\\write18} is enabled from within
\TeX\ we also implement \.{\\eof18}.  We sort of cheat by having an
additional route |scan_four_bit_int_or_18| which is the same as
|scan_four_bit_int| except it also accepts the value 18.

@<Declare procedures that scan restricted classes of integers@>=
procedure scan_four_bit_int_or_18;
begin scan_int;
if (cur_val<0)or((cur_val>15)and(cur_val<>18)) then
  begin print_err("Bad number");
@.Bad number@>
  help2("Since I expected to read a number between 0 and 15,")@/
    ("I changed this one to zero."); int_error(cur_val); cur_val:=0;
  end;
end;
@z
