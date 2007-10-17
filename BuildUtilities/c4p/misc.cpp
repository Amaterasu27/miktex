/* misc.c: C4P utilities					-*- C++ -*-

   Copyright (C) 1991-2001 Christian Schenk

   This file is part of C4P.

   C4P is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   C4P is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with C4P; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <climits>
#include <malloc.h>

#include "c4p.rc"
#include "common.h"

using namespace std;

namespace {
  unsigned c_file_count;
  unsigned c_file_number;
  FILE * name_file;
  std::string current_fast_vars;
}

/* _________________________________________________________________________

   generate_file_header
   _________________________________________________________________________ */

void
generate_file_header ()
{
  out_form ("/* generated from %s by C4P version %s */\n",
	    pascal_file_name.c_str(), VER_FILEVERSION_STR);
}

/* _________________________________________________________________________

   open_name_file
   _________________________________________________________________________ */

void
open_name_file ()
{
  char name_file_name[_MAX_FNAME];
  sprintf (name_file_name, "%s.nam", prog_symbol->s_repr);
#if 0
  if ((name_file = fopen(name_file_name, "w")) == 0)
    {
      c4p_error ("can't open manifest file %s", name_file_name);
    }
#endif
}

/* _________________________________________________________________________

   close_name_file
   _________________________________________________________________________ */

void
close_name_file ()
{
#if 0
  fclose (name_file);
#endif
}

/* _________________________________________________________________________

   open_header_file
   _________________________________________________________________________ */

void
open_header_file ()
{
  char szHeaderFileName[_MAX_FNAME];
  if (h_file_name.length() > 0)
    {
      strcpy (szHeaderFileName, h_file_name.c_str());
    }
  else if (one_c_file && c_file_name.length() > 0)
    {
      sprintf (szHeaderFileName, "%s.h", c_file_name.c_str());
    }
  else
    {
      sprintf (szHeaderFileName, "%s.h", prog_symbol->s_repr);
    }
  open_file (H_FILE_NUM, szHeaderFileName);
  redir_file (H_FILE_NUM);
  generate_file_header ();
  out_form ("#ifndef C4P_HEADER_GUARD_%s\n", prog_symbol->s_repr);
  out_form ("#define C4P_HEADER_GUARD_%s\n", prog_symbol->s_repr);
  out_s ("#ifdef HAVE_C4P_PRE_H\n");
  out_s ("#include \"c4p_pre.h\"\n");
  out_s ("#endif\n");
  out_s ("#include <MiKTeX/C4P/C4P>\n");
  out_s ("#ifdef __cplusplus\nusing namespace C4P;\n#endif\n");
  out_s ("#ifndef C4PEXTERN\n#define C4PEXTERN\n#endif\n");
  if (! def_filename.empty())
    {
      out_form ("#include <%s>\n", def_filename.c_str());
    }
  if (class_name.length() == 0)
    {
      out_form ("#ifdef __cplusplus\n");
      out_form ("extern \"C++\" {\n");
      out_form ("#endif\n");
    }
  else
    {
      if (class_include.length() > 0)
	{
	  out_form ("#include <%s>\n", class_include.c_str());
	}
      if (base_class_name.length() == 0)
	{
	  out_form ("class %s {\n", class_name.c_str(), base_class_name.c_str());
	}
      else
	{
	  out_form ("class %s : public %s {\n",
		    class_name.c_str(), base_class_name.c_str());
	}
      out_form ("public:\n");
    }
}

/* _________________________________________________________________________

   close_header_file
   _________________________________________________________________________ */

void
close_header_file ()
{
  redir_file (H_FILE_NUM);
  if (class_name.length() == 0)
    {
      out_form ("#ifdef __cplusplus\n");
      out_form ("}\n");
      out_form ("#endif\n");
    }
  else
    {
      out_form ("};\n");
    }
  out_form ("\n#endif\n");
  close_file (H_FILE_NUM);
}

/* _________________________________________________________________________

   open_def_file
   _________________________________________________________________________ */

void
open_def_file ()
{
  open_file (DEF_FILE_NUM, def_filename.c_str());
  redir_file (DEF_FILE_NUM);
  generate_file_header ();
  out_form ("#ifndef C4P_DEF_GUARD_%s\n", prog_symbol->s_repr);
  out_form ("#define C4P_DEF_GUARD_%s\n", prog_symbol->s_repr);
}

/* _________________________________________________________________________

   close_def_file
   _________________________________________________________________________ */

void
close_def_file ()
{
  redir_file (DEF_FILE_NUM);
  out_form ("\n#endif\n");
  close_file (DEF_FILE_NUM);
}

/* _________________________________________________________________________

   begin_new_c_file
   _________________________________________________________________________ */

void
begin_new_c_file (const char *file_name, int is_main)

{
  char cfile_name[_MAX_FNAME];

  sprintf (cfile_name, "%s%s", file_name, c_ext.length() > 0 ? c_ext.c_str() : ".c");

#if 0
  printf ("creating %s\n", cfile_name);
#endif

  if (c_file_count++ == 0)
    {
      open_file (C_FILE_NUM, cfile_name);
    }
  else
    {
      reopen_file (C_FILE_NUM, cfile_name);
    }

  redir_file (C_FILE_NUM);
  generate_file_header ();

  if (using_namespace.size() > 0)
    {
      out_form ("#ifdef __cplusplus\n");
      for (vector<string>::const_iterator it = using_namespace.begin();
	   it != using_namespace.end();
	   ++ it)
	{
	  int count = 0;
	  string s = *it;
	  do
	    {
	      string::size_type end = s.find("::");
	      out_form ("namespace %s { ", s.substr(0, end).c_str());
	      ++ count;
	      if (end == string::npos)
		{
		  s = "";
		}
	      else
		{
		  s = s.substr(end + 2);
		}
	    }
	  while (! s.empty());
	  for (; count != 0; -- count)
	    {
	      out_s (" }");
	    }
	  out_form ("using namespace %s;\n", it->c_str());
	}
      out_form ("#endif\n");
    }

  if (class_name.length() == 0)
    {
      out_form ("#define C4PEXTERN%s\n\n", is_main ? "" : " extern");
    }
  if (h_file_name.length() > 0)
    {
      out_form ("#include \"%s\"\n\n", h_file_name.c_str());
    }
  else if (c_file_name.length() > 0)
    {
      out_form ("#include \"%s.h\"\n\n", c_file_name.c_str());
    }
  else
    {
      out_form ("#include \"%s.h\"\n\n", prog_symbol->s_repr);
    }
  if (include_filename.length() > 0)
    {
      out_form ("#include \"%s\"\n\n", include_filename.c_str());
    }

  out_s ("#ifdef _MSC_VER\n");
  out_s ("#pragma warning ( disable : 4102 4390)\n");
  out_s ("#endif\n\n");
}

/* _________________________________________________________________________

   check_c_file_size
   _________________________________________________________________________ */

void
check_c_file_size ()
{
  char name[_MAX_FNAME];

  if (c_file_count == 0
      || (!one_c_file && c_file_line_count + 20 > max_lines_per_c_file))
    {
      ++ c_file_number;
      if (one_c_file)
	{
	  if (c_file_name.length() > 0)
	    {
	      begin_new_c_file (c_file_name.c_str(), 1);
	      return;
	    }
	  else
	    {
	      sprintf (name, "%s", prog_symbol->s_repr);
	    }
	}
      else
	{
	  sprintf (name, "%s%03u",
		   (c_file_name.length() > 0 ? c_file_name.c_str() : prog_symbol->s_repr),
		   c_file_number);
	}
      begin_new_c_file (name, one_c_file);
    }
}

/* _________________________________________________________________________

   forget_fast_vars
   _________________________________________________________________________ */

void
forget_fast_vars ()
{
  current_fast_vars = "";
}

/* _________________________________________________________________________

   rememeber_fast_var
   _________________________________________________________________________ */

void
remember_fast_var (const char * s)
{
  if (strstr(current_fast_vars.c_str(), s) == 0)
    {
      current_fast_vars += ' ';
      current_fast_vars += s;
    }
}

/* _________________________________________________________________________

   declare_fast_var_macro
   _________________________________________________________________________ */

void
declare_fast_var_macro (unsigned routine_handle)
{
  char * buf = reinterpret_cast<char*>(_alloca(current_fast_vars.length() + 1));
  strcpy (buf, current_fast_vars.c_str());
  char * v = strtok(buf, " ");
  redir_file (H_FILE_NUM);
  out_form ("\n#define C4P_FAST_VARS_%u ", routine_handle);
  macroizing = true;
  while (v)
    {
      symbol_t *sym = lookup(v);
      if (sym)
	{
	  out_form ("register %s %s_c4p_fast_%s_%u = %s;\n",
		    (sym->s_translated_type ? sym->s_translated_type : "FIXME"),
		    (sym->s_type == ARRAY_NODE ? "*" : ""),
		    sym->s_repr,
		    routine_handle,
		    sym->s_repr);
	}
      v = strtok(0, " ");
    }
  macroizing = false;
  out_s ("\n");
  redir_file (C_FILE_NUM);
}

/* _________________________________________________________________________

   begin_routine
   _________________________________________________________________________ */

void
begin_routine (prototype_node *	proto,
	       unsigned		handle)
{
  check_c_file_size ();
  out_s ("\n");
  ++ block_level;
  mark_symbol_table ();
  mark_string_table ();
  mark_type_table ();
  out_form ("#if defined (C4P_OPT_%s) && ! defined (C4P_OPT_%u)\n",
	    proto->name->s_repr, handle);
  out_form ("#define C4P_OPT_%u C4P_OPT_%s\n", handle, proto->name->s_repr);
  out_s ("#endif\n");
  out_form ("#ifdef C4P_OPT_%u\n", handle);
  out_form ("#pragma optimize (C4P_OPT_%u, on)\n", handle);
  out_s ("#endif\n");
  out_form ("#ifdef C4P_NOOPT_%s\n", proto->name->s_repr);
  out_form ("#pragma optimize (\"\", off)\n");
  out_s ("#endif\n");
  generate_routine_head (proto);
  out_s ("\n");
  out_s ("{\n");
  ++ curly_brace_level;
  if (n_fast_vars)
    {
      out_form ("C4P_FAST_VARS_%u\n", handle);
      forget_fast_vars ();
    }
#if 0
  fprintf (name_file, "%u %s\n", handle, proto -> name -> s_repr);
#endif
  auto_exit_label_flag = (auto_exit_label >= 0);
}

/* _________________________________________________________________________

   end_routine
   _________________________________________________________________________ */

void
end_routine (unsigned handle)
{
  unmark_type_table ();
  unmark_string_table ();
  unmark_symbol_table ();
  -- block_level;
  -- curly_brace_level;
  if (auto_exit_label_flag)
    {
      out_form ("\nc4p_L%ld: ;\n", auto_exit_label);
      auto_exit_label_flag = false;
    }
  out_s ("}\n");
  if (n_fast_vars)
    {
      declare_fast_var_macro (handle);
    }
  out_s ("#pragma optimize (\"\", on)\n");
}

/* _________________________________________________________________________

   strcpye
   _________________________________________________________________________ */

char *
strcpye (char *		s1,
	 const char *	s2)
{
  while ((*s1++ = *s2++) != 0)
    {
      ;
    }
  return (--s1);
}
