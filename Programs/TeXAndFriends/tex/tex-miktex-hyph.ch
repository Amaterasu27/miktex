%% tex-miktex-hyph.ch:
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
% [1.11]
% _____________________________________________________________________________

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
@d hyph_size=307 {another prime; the number of \.{\\hyphenation} exceptions}
@y
@d hyph_size=8191 {another prime; the number of \.{\\hyphenation} exceptions}
@z

% _____________________________________________________________________________
%
% [42.920]
% _____________________________________________________________________________

@x
@!trie_pointer=0..trie_size; {an index into |trie|}
@y
@!trie_pointer=0..trie_size_def; {an index into |trie|}
@z

% _____________________________________________________________________________
%
% [42.921]
% _____________________________________________________________________________

@x
@!hyf_distance:array[1..trie_op_size] of small_number; {position |k-j| of $n_j$}
@!hyf_num:array[1..trie_op_size] of small_number; {value of $n_j$}
@!hyf_next:array[1..trie_op_size] of quarterword; {continuation code}
@!op_start:array[ASCII_code] of 0..trie_op_size; {offset for current language}
@y
@!hyf_distance:array[1..trie_op_size_def] of small_number; {position |k-j| of $n_j$}
@!hyf_num:array[1..trie_op_size_def] of small_number; {value of $n_j$}
@!hyf_next:array[1..trie_op_size_def] of quarterword; {continuation code}
@!op_start:array[ASCII_code] of 0..trie_op_size_def; {offset for current language}
@z

% _____________________________________________________________________________
%
% [43.942]
% _____________________________________________________________________________

@x
@!init @<Declare procedures for preprocessing hyphenation patterns@>@;
tini
@y
@<Declare procedures for preprocessing hyphenation patterns@>@;
@z

% _____________________________________________________________________________
%
% [43.943]
% _____________________________________________________________________________

@x
@!init@! trie_op_hash:array[-trie_op_size..trie_op_size] of 0..trie_op_size;
@y
@! trie_op_hash:array[0..0] of 0..trie_op_size_def;
@z

@x
@!trie_op_lang:array[1..trie_op_size] of ASCII_code;
  {language part of a hashed quadruple}
@!trie_op_val:array[1..trie_op_size] of quarterword;
  {opcode corresponding to a hashed quadruple}
@!trie_op_ptr:0..trie_op_size; {number of stored ops so far}
tini
@y
@!trie_op_lang:array[1..trie_op_size_def] of ASCII_code;
  {language part of a hashed quadruple}
@!trie_op_val:array[1..trie_op_size_def] of quarterword;
  {opcode corresponding to a hashed quadruple}
@!trie_op_ptr:0..trie_op_size_def; {number of stored ops so far}
@z
% _____________________________________________________________________________
%
% [43.944]
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
% [43.945]
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
% [43.946]
% _____________________________________________________________________________

@x
for k:=-trie_op_size to trie_op_size do trie_op_hash[k]:=0;
@y
for k:=-trie_op_size to trie_op_size do trie_op_hash[k + trie_op_size]:=0;
@z

% _____________________________________________________________________________
%
% [43.947]
% _____________________________________________________________________________

@x
@!init @!trie_c:packed array[trie_pointer] of packed_ASCII_code;
@y
@!trie_c:packed array[trie_pointer] of packed_ASCII_code;
@z

@x
tini
@y
@z

% _____________________________________________________________________________
%
% [43.950]
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
% [54.1379]
% _____________________________________________________________________________

@x
@* \[54] MiK\TeX-dependent changes.
@y
@* \[54] MiK\TeX-dependent changes.

@ @<Global variables@>=
@!trie_size : integer;
@!trie_op_size : integer;
@z
