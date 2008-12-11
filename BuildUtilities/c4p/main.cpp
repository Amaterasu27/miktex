/* main.c: Pascal-to-C Translator				-*- C++ -*-

   Copyright (C) 1991-2007 Christian Schenk

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

#include <tchar.h>

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

int yyparse ();
extern int yylineno;
extern _TCHAR * yytext;

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
yyerror (/*[in]*/ const _TCHAR * s)
{
  _ftprintf (stderr,
	     _T("%s:%ld: parse error before `%s'\n"),
	     pascal_file_name.c_str(),
	     yylineno,
	     yytext);
  exit (1);
}

/* _________________________________________________________________________

   c4p_error
   _________________________________________________________________________ */

void
c4p_error (/*[in]*/ const _TCHAR *	fmt,
	   /*[in]*/			...)
{
  _ftprintf (stderr,
	     _T("%s:%ld: "),
	     pascal_file_name.c_str(),
	     yylineno);
  va_list ap;
  va_start (ap, fmt);
  _vftprintf (stderr, fmt, ap);
  va_end (ap);
  fputc (_T('\n'), stderr);
  exit (2);
}

/* _________________________________________________________________________

   c4p_warning
   _________________________________________________________________________ */

void
c4p_warning (/*[in]*/ const _TCHAR *	fmt,
	     /*[in]*/			...)
{
  if (verbose_flag)
    {
      _ftprintf (stderr,
		 _T("\n%s:%ld: warning: "),
		 pascal_file_name.c_str(), 
		 yylineno);
      va_list ap;
      va_start (ap, fmt);
      _vftprintf (stderr, fmt, ap);
      va_end (ap);
      fputc (_T('\n'), stderr);
    }
}

/* _________________________________________________________________________

   banner
   _________________________________________________________________________ */

void
banner ()
{
  _tprintf (_T("This is C4P version %s\n"), VER_FILEVERSION_STR);
}

/* _________________________________________________________________________

   usage
   _________________________________________________________________________ */

void
usage ()
{
  _putts (_T("\
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

/* _________________________________________________________________________

   longopts
   _________________________________________________________________________ */

namespace {
  const struct option longopts[] =
  {
    _T("auto-exit-label"), required_argument, 0, OPT_AUTO_EXIT_LABEL,
    _T("base-class"), required_argument, 0, OPT_BASE_CLASS,
    _T("chars-are-unsigned"), no_argument, 0, OPT_CHARS_ARE_UNSIGNED,
    _T("class"), required_argument, 0, OPT_CLASS,
    _T("class-include"), required_argument, 0, OPT_CLASS_INCLUDE,
    _T("c-plus-plus"), no_argument, 0, _T('C'),
    _T("declare-c-type"), required_argument, 0, OPT_DECLARE_C_TYPE,
    _T("def-filename"), required_argument, 0, OPT_DEF_FILENAME,
    _T("dll"), no_argument, 0, OPT_DLL,
    _T("entry-name"), required_argument, 0, OPT_ENTRY_NAME,
    _T("fast-var"), required_argument, 0, _T('f'),
    _T("header-file"), required_argument, 0, OPT_HEADER_FILE,
    _T("help"), no_argument, 0, _T('h'),
    _T("include-filename"), required_argument, 0, _T('i'),
    _T("lines"), required_argument, 0, _T('l'),
    _T("one"), optional_argument, 0, _T('1'),
    _T("output-prefix"), required_argument, 0, _T('p'),
    _T("rename"), required_argument, 0, _T('r'),
    _T("using-namespace"), required_argument, 0, OPT_USING_NAMESPACE,
    _T("var-name-prefix"), required_argument, 0, OPT_VAR_NAME_PREFIX,
    _T("var-struct"), required_argument, 0, OPT_VAR_STRUCT,
    _T("var-struct-class"), required_argument, 0, OPT_VAR_STRUCT_CLASS,
    { _T("var-struct-base-class"), required_argument, 0,
      OPT_VAR_STRUCT_BASE_CLASS, },
    _T("verbose"), no_argument, 0, _T('v'),
    _T("version"), no_argument, 0, _T('V'),
    0, 0, 0, 0,
  };
}

/* _________________________________________________________________________

   option_handler
   _________________________________________________________________________ */

void
option_handler (/*[in]*/ int		argc,
		/*[in]*/ _TCHAR **	argv)
{
  myname = argv[0];
  max_lines_per_c_file = 1000;
  auto_exit_label = -1;

  int opt;
  string new_name;

  optind = 0;
  while ((opt = getopt_long(argc, argv, _T("C1:hi:l:r:Vf:"), longopts, 0))
	 != EOF)
    {
      switch (opt)
	{
	case _T('h'):
	  usage ();
	  exit (0);
	  break;
	case _T('v'):
	  verbose_flag = true;
	  break;
	case OPT_CLASS:
	  class_name = optarg;
	  class_name_scope = optarg;
	  class_name_scope += _T("::");
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
	case _T('C'):
	  c_ext = _T(".cc");
	  break;
	case _T('i'):
	  include_filename = optarg;
	  break;
	case _T('l'):
	  max_lines_per_c_file = static_cast<unsigned>(atoi(optarg));
	  break;
	case _T('r'):
	  new_name = _T("c4p_");
	  new_name += optarg;
	  new_mapping (optarg, new_name.c_str());
	  break;
	case _T('f'):
	  new_fast_var (optarg);
	  break;
	case _T('p'):
	  c_file_name = optarg;
	  break;
	case _T('V'):
	  banner ();
	  exit (0);
	  break;
	case _T('1'):
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
      var_struct_class_name = _T("c4pdata");
    }

  if (entry_name.length() == 0)
    {
      entry_name = _T("c4pmain");
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
main (/*[in]*/ int		argc,
      /*[in]*/ _TCHAR **	argv)
{
  symtab_init ();

  option_handler (argc, argv);

  if (pascal_file_name.length() == 0)
    {
      _ftprintf (stderr, _T("%s: no input file specified\n"), myname.c_str());
      exit (4);
    }

  if (freopen(pascal_file_name.c_str(), _T("r"), stdin) == 0)
    {
      _ftprintf (stderr,
		 _T("%s: can't open %s\n"),
		 myname.c_str(),
		 pascal_file_name.c_str());
      exit (5);
    }

  yyparse ();

  return (0);
}
