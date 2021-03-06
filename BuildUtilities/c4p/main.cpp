/* main.c: Pascal-to-C Translator				-*- C++ -*-

   Copyright (C) 1991-2009 Christian Schenk

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
#include <cstdarg>
#include <cstdlib>
#include <climits>

#include <getopt.h>

#include "common.h"

#include "c4p-version.h"

using namespace std;

namespace {
  string myname;
}

bool auto_exit_label_flag;
bool chars_are_unsigned;
bool dll_flag;
bool macroizing;
bool one_c_file;
bool verbose_flag;
int auto_exit_label;
int n_fast_vars;
string base_class_name;
string c_ext;
string c_file_name;
string class_include;
string class_name;
string class_name_scope;
string def_filename;
string entry_name;
string h_file_name;
string include_filename;
string pascal_file_name;
vector<string> using_namespace;
string var_name_prefix;
string var_struct_base_class_name;
string var_struct_class_name;
string var_struct_name;
unsigned c_file_line_count;
unsigned curly_brace_level;
unsigned extra_indent;
unsigned max_lines_per_c_file;
string name_space;

int yyparse ();
extern int yylineno;
extern char * yytext;

/* _________________________________________________________________________

   yywrap
   _________________________________________________________________________ */

int
yywrap ()
{
  return (1);
}

/* _________________________________________________________________________

   yyerror
   _________________________________________________________________________ */

void
yyerror (/*[in]*/ const char * s)
{
  fprintf (stderr,
	   T_("%s:%ld: parse error before `%s'\n"),
	   pascal_file_name.c_str(),
	   yylineno,
	   yytext);
  exit (1);
}

/* _________________________________________________________________________

   c4p_error
   _________________________________________________________________________ */

void
c4p_error (/*[in]*/ const char *	fmt,
	   /*[in]*/			...)
{
  fprintf (stderr,
	   "%s:%ld: ",
	   pascal_file_name.c_str(),
	   yylineno);
  va_list ap;
  va_start (ap, fmt);
  vfprintf (stderr, fmt, ap);
  va_end (ap);
  fputc ('\n', stderr);
  exit (2);
}

/* _________________________________________________________________________

   c4p_warning
   _________________________________________________________________________ */

void
c4p_warning (/*[in]*/ const char *	fmt,
	     /*[in]*/			...)
{
  if (verbose_flag)
    {
      fprintf (stderr,
	       T_("\n%s:%ld: warning: "),
	       pascal_file_name.c_str(), 
	       yylineno);
      va_list ap;
      va_start (ap, fmt);
      vfprintf (stderr, fmt, ap);
      va_end (ap);
      fputc ('\n', stderr);
    }
}

/* _________________________________________________________________________

   banner
   _________________________________________________________________________ */

void
banner ()
{
  printf (T_("This is C4P version %s\n"), MIKTEX_COMPONENT_VERSION_STR);
}

/* _________________________________________________________________________

   usage
   _________________________________________________________________________ */

void
usage ()
{
  puts (T_("\
Usage: c4p [options] filename\n\
Options:\n\
  -1 FILENAME, --one FILENAME\n\
  --auto-exit-label=NUM\n\
  --base-class=CLASS\n\
  --chars-are-unsigned\n\
  --class=CLASS\n\
  --class-include=FILENAME\n\
  --entry-name=NAME\n\
  --declare-c-type=NAME\n\
  --var-name-prefix=PREFIX\n\
  --var-struct=NAME\n\
  --var-struct-base-class=NAME\n\
  --var-struct-class=NAME\n\
  -C, --c-plus-plus\n\
  --header-file FILE\n\
  -h, --help\n\
  -i, --include-filename FILENAME\n\
  -l NUM, --lines NUM\n\
  -p FILENAME, --output-prefix FILENAME\n\
  -r NAME, --rename NAME\n\
  -f VAR, --fast-var VAR\n\
  -V, --version"));
}

#define OPT_DLL 1
#define OPT_CLASS 2
#define OPT_BASE_CLASS 3
#define OPT_CLASS_INCLUDE 4
#define OPT_VAR_STRUCT 5
#define OPT_DEF_FILENAME 6
#define OPT_HEADER_FILE 7
#define OPT_AUTO_EXIT_LABEL 8
#define OPT_VAR_STRUCT_CLASS 9
#define OPT_VAR_STRUCT_BASE_CLASS 10
#define OPT_ENTRY_NAME 11
#define OPT_USING_NAMESPACE 12
#define OPT_VAR_NAME_PREFIX 13
#define OPT_CHARS_ARE_UNSIGNED 14
#define OPT_DECLARE_C_TYPE 15
#define OPT_NAMESPACE 16

/* _________________________________________________________________________

   longopts
   _________________________________________________________________________ */

namespace {
  const struct option longopts[] =
  {
    T_("auto-exit-label"), required_argument, 0, OPT_AUTO_EXIT_LABEL,
    T_("base-class"), required_argument, 0, OPT_BASE_CLASS,
    T_("chars-are-unsigned"), no_argument, 0, OPT_CHARS_ARE_UNSIGNED,
    T_("class"), required_argument, 0, OPT_CLASS,
    T_("class-include"), required_argument, 0, OPT_CLASS_INCLUDE,
    T_("c-plus-plus"), no_argument, 0, 'C',
    T_("declare-c-type"), required_argument, 0, OPT_DECLARE_C_TYPE,
    T_("def-filename"), required_argument, 0, OPT_DEF_FILENAME,
    T_("dll"), no_argument, 0, OPT_DLL,
    T_("entry-name"), required_argument, 0, OPT_ENTRY_NAME,
    T_("fast-var"), required_argument, 0, 'f',
    T_("header-file"), required_argument, 0, OPT_HEADER_FILE,
    T_("help"), no_argument, 0, 'h',
    T_("include-filename"), required_argument, 0, 'i',
    T_("lines"), required_argument, 0, 'l',
    T_("namespace"), required_argument, 0, OPT_NAMESPACE,
    T_("one"), optional_argument, 0, '1',
    T_("output-prefix"), required_argument, 0, 'p',
    T_("rename"), required_argument, 0, 'r',
    T_("using-namespace"), required_argument, 0, OPT_USING_NAMESPACE,
    T_("var-name-prefix"), required_argument, 0, OPT_VAR_NAME_PREFIX,
    T_("var-struct"), required_argument, 0, OPT_VAR_STRUCT,
    T_("var-struct-class"), required_argument, 0, OPT_VAR_STRUCT_CLASS,
    { T_("var-struct-base-class"), required_argument, 0,
      OPT_VAR_STRUCT_BASE_CLASS, },
    T_("verbose"), no_argument, 0, 'v',
    T_("version"), no_argument, 0, 'V',
    0, 0, 0, 0,
  };
}

/* _________________________________________________________________________

   option_handler
   _________________________________________________________________________ */

void
option_handler (/*[in]*/ int		argc,
		/*[in]*/ char **	argv)
{
  myname = argv[0];
  max_lines_per_c_file = 1000;
  auto_exit_label = -1;

  int opt;
  string new_name;

  optind = 0;
  while ((opt = getopt_long(argc, argv, "C1:hi:l:r:Vf:", longopts, 0))
	 != EOF)
    {
      switch (opt)
	{
	case 'h':
	  usage ();
	  exit (0);
	  break;
	case 'v':
	  verbose_flag = true;
	  break;
	case OPT_CLASS:
	  class_name = optarg;
	  class_name_scope = optarg;
	  class_name_scope += "::";
	  break;
	case OPT_DEF_FILENAME:
	  def_filename = optarg;
	  break;
	case OPT_BASE_CLASS:
	  base_class_name = optarg;
	  break;
	case OPT_CHARS_ARE_UNSIGNED:
	  chars_are_unsigned = true;
	  break;
	case OPT_CLASS_INCLUDE:
	  class_include = optarg;
	  break;
	case OPT_DECLARE_C_TYPE:
	  new_type (optarg, UNKNOWN_TYPE, 0, 0);
	  break;
	case OPT_DLL:
	  dll_flag = true;
	  break;
	case OPT_ENTRY_NAME:
	  entry_name = optarg;
	  break;
	case OPT_VAR_STRUCT:
	  var_struct_name = optarg;
	  break;
	case OPT_VAR_STRUCT_CLASS:
	  var_struct_class_name = optarg;
	  break;
	case OPT_VAR_STRUCT_BASE_CLASS:
	  var_struct_base_class_name = optarg;
	  break;
	case OPT_HEADER_FILE:
	  h_file_name = optarg;
	  break;
	case OPT_AUTO_EXIT_LABEL:
	  auto_exit_label = atoi(optarg);
	  break;
	case OPT_USING_NAMESPACE:
	  using_namespace.push_back (optarg);
	  break;
	case OPT_VAR_NAME_PREFIX:
	  var_name_prefix = optarg;
	  break;
	case OPT_NAMESPACE:
	  name_space = optarg;
	  break;
	case 'C':
	  c_ext = ".cc";
	  break;
	case 'i':
	  include_filename = optarg;
	  break;
	case 'l':
	  max_lines_per_c_file = static_cast<unsigned>(atoi(optarg));
	  break;
	case 'r':
	  new_name = "c4p_";
	  new_name += optarg;
	  new_mapping (optarg, new_name.c_str());
	  break;
	case 'f':
	  new_fast_var (optarg);
	  break;
	case 'p':
	  c_file_name = optarg;
	  break;
	case 'V':
	  banner ();
	  exit (0);
	  break;
	case '1':
	  one_c_file = true;
	  if (optarg)
	    {
	      c_file_name = optarg;
	    }
	  break;
	default:
	  usage ();
	  exit (1);
	}
    }
  
  if (var_struct_class_name.length() == 0)
    {
      var_struct_class_name = "c4pdata";
    }

  if (entry_name.length() == 0)
    {
      entry_name = "c4pmain";
    }

  if (optind == argc - 1)
    {
      pascal_file_name = argv[optind];
    }
}

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int	argc,
      /*[in]*/ char **	argv)
{
  symtab_init ();

  option_handler (argc, argv);

  if (pascal_file_name.length() == 0)
    {
      fprintf (stderr, T_("%s: no input file specified\n"), myname.c_str());
      exit (4);
    }

  if (freopen(pascal_file_name.c_str(), "r", stdin) == 0)
    {
      fprintf (stderr,
	       T_("%s: can't open %s\n"),
	       myname.c_str(),
	       pascal_file_name.c_str());
      exit (5);
    }

  yyparse ();

  return (0);
}
