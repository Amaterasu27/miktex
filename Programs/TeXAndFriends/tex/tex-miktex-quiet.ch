%% tex-miktex-quiet.ch: implement quiet mode
%% 
%% Copyright (C) 1991-2006 Christian Schenk
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
% [5.61]
% _____________________________________________________________________________

@x
wterm(banner);
miktex_print_miktex_banner(term_out);
if format_ident=0 then print_ln
else  begin slow_print(format_ident); print_ln;
  end;
@y
if (not miktex_get_quiet_flag) then begin
  wterm(banner);
  miktex_print_miktex_banner(term_out);
  if format_ident=0 then print_ln
  else  begin slow_print(format_ident); print_ln;
    end;
end;
@z
