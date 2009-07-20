%% tex-miktex-hash.ch:
%% 
%% Copyright (C) 1991-2009 Christian Schenk
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
% [1.12]
% _____________________________________________________________________________

@x
@d hash_size=2100 {maximum number of control sequences; it should be at most
  about |(mem_max-mem_min)/10|}
@y
@d hash_size=110000 {maximum number of control sequences; it should be at most
  about |(mem_max-mem_min)/10|}
@z

@x
@d hash_prime=1777 {a prime number equal to about 85\pct! of |hash_size|}
@y
@d hash_prime=93503 {a prime number equal to about 85\pct! of |hash_size|}
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

