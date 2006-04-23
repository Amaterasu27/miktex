%% opl2ofm-miktex.ch: WEB change file for OPL2OFM
%% 
%% Copyright (C) 1998-2005 Christian Schenk
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
  begin print_ln(banner);@/
@y
  begin print(banner);
  miktex_print_miktex_banner(output);
  write_ln;@/
@z

% _____________________________________________________________________________
%
% [2.6]
% _____________________________________________________________________________

@x
reset(pl_file);
@y
if (c4pargc < 2) then begin
  write_ln ('Usage: opl2ofm inputfile outputfile'); c4p_exit(1);
end;
c4p_fopen(pl_file,c4p_argv[1],c4p_r_mode,true);reset(pl_file);
@z

% _____________________________________________________________________________
%
% [2.16]
% _____________________________________________________________________________

@x
rewrite(tfm_file);
@y
if (c4pargc <> 3) then begin
  write_ln ('Usage: opl2ofm inputfile outputfile'); c4p_exit(1);
end;
c4p_fopen(tfm_file,c4p_argv[2],c4p_wb_mode,true);rewrite(tfm_file);
@z

% _____________________________________________________________________________
%
% [8.126]
% _____________________________________________________________________________

@x
ASCII_04:=' !"#$%&''()*+,-./0123456789:;<=>?';@/
ASCII_10:='@@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_';@/
ASCII_14:='`abcdefghijklmnopqrstuvwxyz{|}~ ';@/
HEX:='0123456789ABCDEF';@/
MBL_string:='MBL'; RI_string:='RI '; RCE_string:='RCE';
@y
c4p_arrcpy(ASCII_04,' !"#$%&''()*+,-./0123456789:;<=>?');@/
c4p_arrcpy(ASCII_10,'@@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_');@/
c4p_arrcpy(ASCII_14,'`abcdefghijklmnopqrstuvwxyz{|}~ ');@/
c4p_arrcpy(HEX,'0123456789ABCDEF');@/
c4p_arrcpy(MBL_string,'MBL'); c4p_arrcpy(RI_string,'RI ');@/
c4p_arrcpy(RCE_string,'RCE');
@z
