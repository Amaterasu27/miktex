%% tex-fallback.ch: fall back to TeX 3.14159
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
% limbo
% _____________________________________________________________________________

@x
% Version 3.141592 fixed \xleaders, glueset, weird alignments (December 2002).
@y
@z
 
@x
% A reward of $327.68 will be paid to the first finder of any remaining bug.
@y
% A reward of $327.68 will be paid to the first finder of any remaining bug,
% not counting changes introduced after August 1989.
@z
 
@x
\def\hangg#1 {\hang\hbox{#1 }}
@y
@z

% _____________________________________________________________________________
%
% [1.2]
% _____________________________________________________________________________
@x
@^Zabala Salelles, Ignacio Andr\'es@>
@y
@^Zabala Salelles, Ignacio Andres@>
@z

@x
@d banner=='This is TeX, Version 3.141592' {printed when \TeX\ starts}
@y
@d banner=='This is TeX, Version 3.14159' {printed when \TeX\ starts}
@z
 
% _____________________________________________________________________________
%
% [1.3]
% _____________________________________________________________________________

@x
Incidentally, \PASCAL's standard |round| function can be problematical,
because it disagrees with the IEEE floating-point standard.
Many implementors have
therefore chosen to substitute their own home-grown rounding procedure.

@y
@z

% _____________________________________________________________________________
%
% [1.15]
% _____________________________________________________________________________

@x
`|exit|' just before the `\ignorespaces|end|\unskip' of a procedure in
@y
`|exit|:' just before the `\ignorespaces|end|\unskip' of a procedure in
@z

% _____________________________________________________________________________
%
% [5.71]
% _____________________________________________________________________________

@x
begin update_terminal; {now the user sees the prompt for sure}
@y
begin update_terminal; {Now the user sees the prompt for sure}
@z

% _____________________________________________________________________________
%
% [15.207]
% _____________________________________________________________________________

@x
@d ignore=9 {characters to ignore ( \.{\^\^@@} )}
@y
@d ignore=9 {characters to ignore ( \.{\^\^J} )}
@z

@x
@d active_char=13 {characters that invoke macros ( \.{\char`\~} )}
@y
@d active_char=13 {characters that invoke macros ( \.{\^\^[} )}
@z

% _____________________________________________________________________________
%
% [17.220]
% _____________________________________________________________________________

@x
\yskip\hangg 1) |eqtb[active_base..(hash_base-1)]| holds the current
@y
\yskip\hang 1) |eqtb[active_base..(hash_base-1)]| holds the current
@z

@x
\yskip\hangg 2) |eqtb[hash_base..(glue_base-1)]| holds the current
@y
\yskip\hang 2) |eqtb[hash_base..(glue_base-1)]| holds the current
@z

@x
\yskip\hangg 3) |eqtb[glue_base..(local_base-1)]| holds the current
@y
\yskip\hang 3) |eqtb[glue_base..(local_base-1)]| holds the current
@z

@x
\yskip\hangg 4) |eqtb[local_base..(int_base-1)]| holds the current
@y
\yskip\hang 4) |eqtb[local_base..(int_base-1)]| holds the current
@z

@x
\yskip\hangg 5) |eqtb[int_base..(dimen_base-1)]| holds the current
@y
\yskip\hang 5) |eqtb[int_base..(dimen_base-1)]| holds the current
@z

@x
\yskip\hangg 6) |eqtb[dimen_base..eqtb_size]| holds the current equivalents
@y
\yskip\hang 6) |eqtb[dimen_base..eqtb_size]| holds the current equivalents
@z

% _____________________________________________________________________________
%
% [17.221]
% _____________________________________________________________________________

@x
\yskip\hangg 1) The |eq_level| (a quarterword) is the level of grouping at
@y
\yskip\hang 1) The |eq_level| (a quarterword) is the level of grouping at
@z

@x
\yskip\hangg 2) The |eq_type| (another quarterword) specifies what kind of
@y
\yskip\hang 2) The |eq_type| (another quarterword) specifies what kind of
@z

@x
\yskip\hangg 3) The |equiv| (a halfword) is the current equivalent value.
@y
\yskip\hang 3) The |equiv| (a halfword) is the current equivalent value.
@z

% _____________________________________________________________________________
%
% [19.268]
% _____________________________________________________________________________

@x
\yskip\hangg 1) If |save_type(p)=restore_old_value|, then
@y
\yskip\hang 1) If |save_type(p)=restore_old_value|, then
@z

@x
\yskip\hangg 2) If |save_type(p)=restore_zero|, then |save_index(p)|
@y
\yskip\hang 2) If |save_type(p)=restore_zero|, then |save_index(p)|
@z

@x
\yskip\hangg 3) If |save_type(p)=insert_token|, then |save_index(p)|
@y
\yskip\hang 3) If |save_type(p)=insert_token|, then |save_index(p)|
@z

@x
\yskip\hangg 4) If |save_type(p)=level_boundary|, then |save_level(p)|
@y
\yskip\hang 4) If |save_type(p)=level_boundary|, then |save_level(p)|
@z

% _____________________________________________________________________________
%
% [22.300]
% _____________________________________________________________________________

@x
\yskip\hangg 1) If there is frequent access to the top entry, and if the
@y
\yskip\hang 1) If there is frequent access to the top entry, and if the
@z

@x
\yskip\hangg 2) If there is infrequent top access, the entire stack contents
@y
\yskip\hang 2) If there is infrequent top access, the entire stack contents
@z

% _____________________________________________________________________________
%
% [23.325]
% _____________________________________________________________________________

@x
begin while (state=token_list)and(loc=null)and(token_type<>v_template) do
@y
begin while (state=token_list)and(loc=null) do
@z

% _____________________________________________________________________________
%
% [25.390]
% _____________________________________________________________________________

@x
while (state=token_list)and(loc=null)and(token_type<>v_template) do
  end_token_list; {conserve stack space}
@y
while (state=token_list)and(loc=null) do end_token_list; {conserve stack space}
@z

% _____________________________________________________________________________
%
% [29.511]
% _____________________________________________________________________________

@x
\.{webmac.tex}.
@y
\.{webhdr.tex}.
@z

@x
The following procedures don't allow spaces to be part of
file names; but some users seem to like names that are spaced-out.
System-dependent changes to allow such things should probably
be made with reluctance, and only when an entire file name that
includes spaces is ``quoted'' somehow.

@y
@z

% _____________________________________________________________________________
%
% [30.544]
% _____________________________________________________________________________

@x
\yskip\hangg|tag=0| (|no_tag|) means that |remainder| is unused.\par
\hangg|tag=1| (|lig_tag|) means that this character has a ligature/kerning
@y
\yskip\hang|tag=0| (|no_tag|) means that |remainder| is unused.\par
\hang|tag=1| (|lig_tag|) means that this character has a ligature/kerning
@z

@x
\hangg|tag=2| (|list_tag|) means that this character is part of a chain of
@y
\hang|tag=2| (|list_tag|) means that this character is part of a chain of
@z

@x
\hangg|tag=3| (|ext_tag|) means that this character code represents an
@y
\hang|tag=3| (|ext_tag|) means that this character code represents an
@z

% _____________________________________________________________________________
%
% [32.619]
% _____________________________________________________________________________

@x
@!cur_glue:real; {glue seen so far}
@!cur_g:scaled; {rounded equivalent of |cur_glue| times the glue ratio}
begin cur_g:=0; cur_glue:=float_constant(0);
this_box:=temp_ptr; g_order:=glue_order(this_box);
@y
begin this_box:=temp_ptr; g_order:=glue_order(this_box);
@z

% _____________________________________________________________________________
%
% [32.625]
% _____________________________________________________________________________

@x
begin g:=glue_ptr(p); rule_wd:=width(g)-cur_g;
@y
begin g:=glue_ptr(p); rule_wd:=width(g);
@z

@x
      begin cur_glue:=cur_glue+stretch(g);
      vet_glue(float(glue_set(this_box))*cur_glue);
@y
      begin vet_glue(float(glue_set(this_box))*stretch(g));
@z

@x
      cur_g:=round(glue_temp);
@y
      rule_wd:=rule_wd+round(glue_temp);
@z

@x
      begin cur_glue:=cur_glue-shrink(g);
      vet_glue(float(glue_set(this_box))*cur_glue);
      cur_g:=round(glue_temp);
      end;
@y
    begin vet_glue(float(glue_set(this_box))*shrink(g));
      rule_wd:=rule_wd-round(glue_temp);
    end;
@z

@x
rule_wd:=rule_wd+cur_g;
@y
@z

% _____________________________________________________________________________
%
% [32.627]
% _____________________________________________________________________________

@x
  else  begin lx:=lr div (lq+1);
@y
  else  begin lx:=(2*lr+lq+1) div (2*lq+2); {round|(lr/(lq+1))|}
@z

% _____________________________________________________________________________
%
% [32.629]
% _____________________________________________________________________________

@x
@!cur_glue:real; {glue seen so far}
@!cur_g:scaled; {rounded equivalent of |cur_glue| times the glue ratio}
begin cur_g:=0; cur_glue:=float_constant(0);
this_box:=temp_ptr; g_order:=glue_order(this_box);
@y
begin this_box:=temp_ptr; g_order:=glue_order(this_box);
@z

% _____________________________________________________________________________
%
% [32.634]
% _____________________________________________________________________________

@x
begin g:=glue_ptr(p); rule_ht:=width(g)-cur_g;
@y
begin g:=glue_ptr(p); rule_ht:=width(g);
@z

@x
      begin cur_glue:=cur_glue+stretch(g);
      vet_glue(float(glue_set(this_box))*cur_glue);
@y
      begin vet_glue(float(glue_set(this_box))*stretch(g));
@z

@x
      cur_g:=round(glue_temp);
@y
      rule_ht:=rule_ht+round(glue_temp);
@z

@x
      begin cur_glue:=cur_glue-shrink(g);
      vet_glue(float(glue_set(this_box))*cur_glue);
      cur_g:=round(glue_temp);
      end;
@y
    begin vet_glue(float(glue_set(this_box))*shrink(g));
    rule_ht:=rule_ht-round(glue_temp);
    end;
@z

@x
rule_ht:=rule_ht+cur_g;
@y
@z

% _____________________________________________________________________________
%
% [32.636]
% _____________________________________________________________________________

@x
  else  begin lx:=lr div (lq+1);
@y
  else  begin lx:=(2*lr+lq+1) div (2*lq+2); {round|(lr/(lq+1))|}
@z

% _____________________________________________________________________________
%
% [33.655]
% _____________________________________________________________________________

@x
to make a deletion.
@^inner loop@>
@y
to make a deletion.
@z

% _____________________________________________________________________________
%
% [34.680]
% _____________________________________________________________________________

@x
nodes, math nodes, ligature nodes,
@y
nodes, math nodes, ligature nodes, mark nodes, insert nodes, adjust nodes,
@z

% _____________________________________________________________________________
%
% [36.721]
% _____________________________________________________________________________

@x
  if r<>null then if link(r)=null then if not is_char_node(r) then
   if type(r)=kern_node then {unneeded italic correction}
@y
  if r<>null then if link(r)=null then
   if not is_char_node(r) then if type(r)=kern_node then
@z

% _____________________________________________________________________________
%
% [37.789]
% _____________________________________________________________________________

@x
to another alignment is being scanned, or when no alignment preamble is active.
@y
to another alignment is being scanned.
@z

@x
begin if (scanner_status=aligning) or (cur_align=null) then
@y
begin if scanner_status=aligning then
@z

% _____________________________________________________________________________
%
% [37.812]
% _____________________________________________________________________________

@x
@^dirty \PASCAL@>
@y
@^dirty Pascal@>
@z

% _____________________________________________________________________________
%
% [38.850]
% _____________________________________________________________________________

@x
compute the length of the $l\mskip1mu$th line of the current paragraph. Furthermore,
@y
compute the length of the $l\,$th line of the current paragraph. Furthermore,
@z

% _____________________________________________________________________________
%
% [41.905]
% _____________________________________________________________________________

@x
ligature and a kern between `fl' and `y', then $m=2$, $t=2$, and $y_1$ will
@y
ligature and a kern between `fl' and `y', then $m=2$, $y=2$, and $y_1$ will
@z

% _____________________________________________________________________________
%
% [41.911]
% _____________________________________________________________________________

@x
qi(2),qi(6):begin cur_r:=rem_byte(q); {\.{\?=:}, \.{\?=:>}}
@y
qi(2),qi(6):begin cur_r:=rem_byte(q); {\.{\?=:}. \.{\?=:>}}
@z

% _____________________________________________________________________________
%
% [46.1030]
% _____________________________________________________________________________

@x
@^inner loop@>
@y
@z

% _____________________________________________________________________________
%
% [46.1035]
% _____________________________________________________________________________

@x
@^dirty \PASCAL@>
@y
@^dirty Pascal@>
@z

% _____________________________________________________________________________
%
% [46.1039]
% _____________________________________________________________________________

@x
@^inner loop@>
@y
@z

% _____________________________________________________________________________
%
% [46.1041]
% _____________________________________________________________________________

@x
@^inner loop@>
@y
@z

% _____________________________________________________________________________
%
% [47.1131]
% _____________________________________________________________________________

@x
A devious user might force an |endv| command to occur just about anywhere;
we must defeat such hacks.

@y
@z

@x
begin base_ptr:=input_ptr; input_stack[base_ptr]:=cur_input;
while (input_stack[base_ptr].index_field<>v_template) and
      (input_stack[base_ptr].loc_field=null) and
      (input_stack[base_ptr].state_field=token_list) do decr(base_ptr);
if (input_stack[base_ptr].index_field<>v_template) or
      (input_stack[base_ptr].loc_field<>null) or
      (input_stack[base_ptr].state_field<>token_list) then
  fatal_error("(interwoven alignment preambles are not allowed)");
@.interwoven alignment preambles...@>
 if cur_group=align_group then
@y
begin if cur_group=align_group then
@z

% _____________________________________________________________________________
%
% [48.1145]
% _____________________________________________________________________________

@x
{now we are in vertical mode, working on the list that will contain the display}
@y
{Now we are in vertical mode, working on the list that will contain the display}
@z

% _____________________________________________________________________________
%
% [50.1326]
% _____________________________________________________________________________

@x
to prevent them from appearing again.
@y
to prevent them appearing again.
@z

% _____________________________________________________________________________
%
% [50.1328]
% _____________________________________________________________________________

@x
print_int(year); print_char(".");
@y
print_int(year mod 100); print_char(".");
@z
