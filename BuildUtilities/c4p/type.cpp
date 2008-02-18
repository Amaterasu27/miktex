/* type.c: type table						-*- C++ -*-

   Copyright (C) 1991-2008 Christian Schenk

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

#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <climits>

#include "common.h"
#include "gram.h"

union common_type_node
{
  record_node rec;		/* Record */
  field_list_node fl;		/* Field list */
  variant_field_list_node vfl;	/* Variant field list */
  variant_node var;		/* Variant */
  record_section_node rs;	/* Record section */
  subrange_node sub;		/* Subrange */
  array_node arr;		/* Array */
  pointer_node ptr;		/* Pointer */
  declarator_node dcl;		/* Declarator */
  named_type_node nt;		/* Named Type */
  parameter_node par;		/* Parameter */
  prototype_node pro;		/* Prototype */
  file_node fil;		/* File */
};

#define REC ((record_node*)type_ptr)
#define FL ((field_list_node*)type_ptr)
#define VFL ((variant_field_list_node*)type_ptr)
#define V ((variant_node*)type_ptr)
#define RS ((record_section_node*)type_ptr)
#define SUB ((subrange_node*)type_ptr)
#define ARR ((array_node*)type_ptr)
#define PTR ((pointer_node*)type_ptr)
#define DCL ((declarator_node*)type_ptr)
#define NT ((named_type_node*)type_ptr)
#define PAR ((parameter_node*)type_ptr)
#define PRO ((prototype_node*)type_ptr)
#define FIL ((file_node*)type_ptr)

namespace {
  common_type_node type_table[TYPE_TABLE_SIZE];
  unsigned type_table_top;
  unsigned type_table_mark;
}

/* _________________________________________________________________________

   Malloc
   _________________________________________________________________________ */

void *
Malloc (unsigned type)
{
  if (type_table_top == TYPE_TABLE_SIZE)
    {
      c4p_error ("internal error: must increase `TYPE_TABLE_SIZE'");
    }

  common_type_node * p_common_type_node = &type_table[type_table_top++];

  switch (type)
    {
    case POINTER_NODE:
      return (&p_common_type_node->ptr);
    case RECORD_NODE:
      return (&p_common_type_node->rec);
    case FIELD_LIST_NODE:
      return (&p_common_type_node->fl);
    case VARIANT_FIELD_LIST_NODE:
      return (&p_common_type_node->vfl);
    case VARIANT_NODE:
      return (&p_common_type_node->var);
    case RECORD_SECTION_NODE:
      return (&p_common_type_node->rs);
    case SUBRANGE_NODE:
      return (&p_common_type_node->sub);
    case ARRAY_NODE:
      return (&p_common_type_node->arr);
    case DECLARATOR_NODE:
      return (&p_common_type_node->dcl);
    case NAMED_TYPE_NODE:
      return (&p_common_type_node->nt);
    case PARAMETER_NODE:
      return (&p_common_type_node->par);
    case PROTOTYPE_NODE:
      return (&p_common_type_node->pro);
    case FILE_NODE:
      return (&p_common_type_node->fil);
    default:
      c4p_error ("internal error: unknown type node");
      return (0);
    }
}

/* _________________________________________________________________________

   new_type_node
   _________________________________________________________________________ */

void *
new_type_node (pascal_type	what_kind,
				...)
{
  va_list ap;
  va_start (ap, what_kind);

  void * type_ptr = Malloc(what_kind);

  switch (what_kind)
    {
    case NAMED_TYPE_NODE:
      NT->name = va_arg(ap, symbol_t *);
      break;
    case SUBRANGE_NODE:
      SUB->lower_bound = va_arg(ap, long);
      SUB->upper_bound = va_arg(ap, long);
      break;
    case ARRAY_NODE:
      ARR->lower_bound = va_arg(ap, long);
      ARR->upper_bound = va_arg(ap, long);
      ARR->component_type = UNKNOWN_TYPE;
      break;
    case POINTER_NODE:
      PTR->component_type = va_arg(ap, pascal_type);
      PTR->component_type_ptr = va_arg(ap, void *);
      PTR->array_node_ptr
	= reinterpret_cast<array_node*>(new_type_node(ARRAY_NODE, 0, LONG_MAX));
      PTR->array_node_ptr->component_type = PTR->component_type;
      PTR->array_node_ptr->component_type_ptr = PTR->component_type_ptr;
      break;
    case RECORD_NODE:
      REC->field_list = va_arg(ap, field_list_node *);
      break;
    case FIELD_LIST_NODE:
      FL->fixed_part = va_arg(ap, record_section_node *);
      FL->variant_part = va_arg(ap, variant_node *);
      break;
    case VARIANT_FIELD_LIST_NODE:
      VFL->fixed_part = va_arg(ap, record_section_node *);
      VFL->variant_part = va_arg(ap, variant_node *);
      if (VFL->fixed_part->next == 0)
	VFL->pseudo_name = 0;
      else
	VFL->pseudo_name = new_pseudo_symbol();
      VFL->next = 0;
      break;
    case RECORD_SECTION_NODE:
      RS->name = va_arg(ap, declarator_node *);
      RS->type = va_arg(ap, pascal_type);
      RS->type_ptr = va_arg(ap, void *);
      RS->next = 0;
      break;
    case VARIANT_NODE:
      V->variant_field_list = va_arg(ap, variant_field_list_node *);
      V->pseudo_name = new_pseudo_symbol();
      break;
    case PROTOTYPE_NODE:
      PRO->name = va_arg(ap, symbol_t *);
      PRO->formal_parameter = va_arg(ap, parameter_node *);
      PRO->result_type = va_arg(ap, symbol_t *);
      break;
    case PARAMETER_NODE:
      PAR->name = va_arg(ap, const char *);
      PAR->type_symbol = 0;
      PAR->by_reference = false;
      PAR->next = 0;
      break;
    case DECLARATOR_NODE:
      DCL->name = va_arg(ap, symbol_t *);
      DCL->next = 0;
      break;
    case FILE_NODE:
      FIL->type = va_arg(ap, pascal_type);
      FIL->type_ptr = va_arg(ap, void *);
      break;
    default:
      c4p_error ("internal error: new_type_node: unknown node type: %u", what_kind);
      break;
    }
  va_end (ap);
  return (type_ptr);
}

/* _________________________________________________________________________

   out_dimensions
   _________________________________________________________________________ */

void
out_dimensions (array_node * arr)

{
  unsigned type;
  do
    {
      out_form ("[ %ld ]", arr->upper_bound - arr->lower_bound + 1);
      type = arr->component_type;
      arr = reinterpret_cast<array_node*>(arr->component_type_ptr);
    }
  while (type == ARRAY_NODE);
}

/* _________________________________________________________________________

   flatten_type
   _________________________________________________________________________ */

pascal_type
flatten_type (pascal_type	type,
	      void *		type_ptr,
	      void **		type_ptr_ptr)
{
  pascal_type flattened_type = type;
  if (type == NAMED_TYPE_NODE)
    {
      if (NT->name->s_kind != TYPE_IDENTIFIER)
	{
	  c4p_error ("`%s' is not a type identifier", NT->name->s_repr);
	}
      flattened_type = NT->name->s_type;
      if (type_ptr_ptr != 0)
	{
	  *type_ptr_ptr = NT->name->s_type_ptr;
	}
    }
  else if (type == ARRAY_NODE)
    {
      ARR->component_type =
	flatten_type(ARR->component_type, ARR->component_type_ptr,
		     &ARR->component_type_ptr);
    }
  else if (type == POINTER_NODE)
    {
      PTR->component_type =
	flatten_type(PTR->component_type, PTR->component_type_ptr,
		     &PTR->component_type_ptr);
      PTR->array_node_ptr->component_type = PTR->component_type;
      PTR->array_node_ptr->component_type_ptr = PTR->component_type_ptr;
    }
  else
    {
      if (type == FILE_NODE)
	{
	  FIL->type = flatten_type(FIL->type, FIL->type_ptr, &FIL->type_ptr);
	}
    }
  return (flattened_type);
}

/* _________________________________________________________________________

   translate_type
   _________________________________________________________________________ */

const char *
translate_type (pascal_type	type,
		const void *	type_ptr)
{
  const char * ret = 0;
  switch (type)
    {
    case NAMED_TYPE_NODE:
      ret = (NT->name->s_translated_type
	     ? NT->name->s_translated_type
	     : NT->name->s_repr);
      out_s (ret);
      break;
    case BOOLEAN_TYPE:
      NT->name->s_repr = "C4P_boolean";
      out_s ("C4P_boolean");
      break;
    case CHARACTER_TYPE:
      NT->name->s_repr = "char";
      out_s ("char");
      break;
    case INTEGER_TYPE:
      NT->name->s_repr = "C4P_integer";
      out_s ("C4P_integer");
      break;
    case LONG_INTEGER_TYPE:
      NT->name->s_repr = "C4P_longinteger";
      out_s ("C4P_longinteger");
      break;
    case REAL_TYPE:
      NT->name->s_repr = "float";
      out_s ("float");
      break;
    case LONG_REAL_TYPE:
      NT->name->s_repr = "double";
      out_s ("double");
      break;
    case ARRAY_NODE:
      ret = translate_type (ARR->component_type, ARR->component_type_ptr);
      break;
    case POINTER_NODE:
#if 1
      ret = translate_type (PTR->component_type, PTR->component_type_ptr);
#else
      ret = translate_type (PTR->component_type, PTR->component_type_ptr);
#endif
      break;
    case SUBRANGE_NODE:
      ret = subrange (SUB->lower_bound, SUB->upper_bound);
      out_s (ret);
      break;
    case RECORD_NODE:
      out_s ("struct {\n");
      ++ curly_brace_level;
      translate_type (FIELD_LIST_NODE, REC->field_list);
      -- curly_brace_level;
      out_s ("}");
      break;
    case FIELD_LIST_NODE:
      if (FL->fixed_part != 0)
	{
	  translate_type (RECORD_SECTION_NODE, FL->fixed_part);
	}
      if (FL->variant_part != 0)
	{
	  translate_type (VARIANT_NODE, FL->variant_part);
	}
      break;
    case RECORD_SECTION_NODE:
      declare_var_list (RS->name, FIELD_IDENTIFIER, UINT_MAX,
			RS->type, RS->type_ptr);
      out_s (";\n");
      if (RS->next != 0)
	{
	  translate_type (RECORD_SECTION_NODE, RS->next);
	}
      break;
    case VARIANT_NODE:
      out_s ("union {\n");
      ++ curly_brace_level;
      if (V->variant_field_list != 0)
	{
	  translate_type (VARIANT_FIELD_LIST_NODE, V->variant_field_list);
	}
      -- curly_brace_level;
      out_form ("} %s;\n", V->pseudo_name->s_repr);
      break;
    case VARIANT_FIELD_LIST_NODE:
      if (VFL->pseudo_name != 0)
	{
	  out_s ("struct {\n");
	  ++ curly_brace_level;
	}
      if (VFL->fixed_part != 0)
	{
	  translate_type (RECORD_SECTION_NODE, VFL->fixed_part);
	}
      if (VFL->variant_part != 0)
	{
	  translate_type (VARIANT_NODE, VFL->variant_part);
	}
      if (VFL->pseudo_name != 0)
	{
	  -- curly_brace_level;
	  out_form ("} %s;\n", VFL->pseudo_name->s_repr);
	}
      if (VFL->next != 0)
	{
	  translate_type (VARIANT_FIELD_LIST_NODE, VFL->next);
	}
      break;
    case FILE_NODE:
      if (FIL->type == CHARACTER_TYPE
	  || (FIL->type == NAMED_TYPE_NODE
	      && (((named_type_node *) FIL->type_ptr)->name->s_type
		  == CHARACTER_TYPE)))
	{
	  out_s ("C4P_text");
	}
      else
	{
	  out_s ("C4P_FILE_STRUCT(");
	  translate_type (FIL->type, FIL->type_ptr);
	  out_s (")");
	}
      FIL->type = flatten_type(FIL->type, FIL->type_ptr, &FIL->type_ptr);
      break;
    default:
      c4p_error ("internal error: translate_type: unknown node type: %u", type);
    }
  return (ret);
}

/* _________________________________________________________________________

   generate_routine_head
   _________________________________________________________________________ */

void
generate_routine_head (prototype_node * proto)
{
  out_s ("\n");
  out_form ("%s%s\n%s%s (",
	    ((class_name_scope.length() > 0
	      && proto->result_type != 0
	      && ! (proto->result_type->s_flags & S_PREDEFINED))
	     ? class_name_scope.c_str()
	     : ""),
	    (proto->result_type == 0
	     ? "void"
	     : (proto->result_type->s_translated_type
		? proto->result_type->s_translated_type
		: proto->result_type->s_repr)),
	    class_name_scope.c_str(),
	    proto->name->s_repr);
  redir_file (H_FILE_NUM);
  out_form ("%s%s %s (",
	    (one_c_file ? "" : ""), /* static */
	    (proto->result_type == 0
	     ? "void"
	     : (proto->result_type->s_translated_type
		? proto->result_type->s_translated_type
		: proto->result_type->s_repr)),
	    proto->name->s_repr);
  redir_file (C_FILE_NUM);
  parameter_node * par = proto->formal_parameter;
  if (par == 0)
    {
      out_s ("void)\n");
      redir_file (H_FILE_NUM);
      out_s ("void);\n");
      redir_file (C_FILE_NUM);
      return;
    }
  symbol_t * type_symbol;
  symbol_t * param_symbol;
  while (par != 0)
    {
      type_symbol = par->type_symbol;
      param_symbol = new_symbol_instance (par->name);
      param_symbol->s_kind = PARAMETER_IDENTIFIER;
      param_symbol->s_block_level = 1;
      param_symbol->s_type = type_symbol->s_type;
      param_symbol->s_type_ptr = type_symbol->s_type_ptr;
      param_symbol->s_type = flatten_type(param_symbol->s_type,
					  param_symbol->s_type_ptr,
					  &param_symbol->s_type_ptr);
      out_form ("%s %s%s",
		(type_symbol->s_translated_type
		 ? type_symbol->s_translated_type
		 : type_symbol->s_repr),
		par->by_reference ? "& " : "",
		par->name);
      redir_file (H_FILE_NUM);
      out_form ("%s %s%s",
		(type_symbol->s_translated_type
		 ? type_symbol->s_translated_type
		 : type_symbol->s_repr),
		par->by_reference ? "& " : "",
		par->name);
      redir_file (C_FILE_NUM);
      if (par->by_reference)
	{
	  param_symbol->s_flags |= S_BY_REFERENCE;
	}
      par = par->next;
      if (par != 0)
	{
	  out_s (",  ");
	  redir_file (H_FILE_NUM);
	  out_s (",  ");
	  redir_file (C_FILE_NUM);
	}
    }

  out_s (")\n");
  redir_file (H_FILE_NUM);
  out_s (");\n");
  redir_file (C_FILE_NUM);
}

/* _________________________________________________________________________

   declare_var_list
   _________________________________________________________________________ */

void
declare_var_list (declarator_node *	vars,
		  unsigned		kind,
		  unsigned		block_level,
		  pascal_type		type,
		  void *		type_ptr)
{
  declarator_node * start = vars;
  const char * translated_type = translate_type(type, type_ptr);
  out_s (" ");
  while (vars != 0)
    {
      if (is_fast_var (vars->name->s_repr))
	{
	  vars->name->s_flags |= S_FAST;
	}
      if (type == POINTER_NODE)
	{
	  out_s ("*");
	}
      if (block_level == 0 && kind == VARIABLE_IDENTIFIER)
	{
	  out_s (var_name_prefix.c_str());
	}
      out_s (vars->name->s_repr);
      if (type == ARRAY_NODE)
	{
	  out_dimensions (reinterpret_cast<array_node*>(type_ptr));
	}
      vars->name->s_translated_type = translated_type;
      vars = vars->next;
      if (vars != 0)
	{
	  out_s (", ");
	}
    }
  type = flatten_type(type, type_ptr, &type_ptr);
  vars = start;
  while (vars != 0)
    {
      vars->name = define_symbol(vars->name, kind, block_level,
				 type, type_ptr, 0);
      vars = vars->next;
    }
}

/* _________________________________________________________________________

   define_type
   _________________________________________________________________________ */

void
define_type (symbol_t *		type_symbol,
	     unsigned		block_level,
	     pascal_type	type,
	     void *		type_ptr)
{
  translate_type (type, type_ptr);
  out_s (" ");
#if 1				// experimental
  if (type == POINTER_NODE)
    {
      out_s ("*");
    }
#endif
  out_s (type_symbol->s_repr);
  if (type == ARRAY_NODE)
    {
      out_dimensions (reinterpret_cast<array_node*>(type_ptr));
    }
  if (type == NAMED_TYPE_NODE)
    {
      if (NT->name->s_kind != TYPE_IDENTIFIER)
	{
	  c4p_error ("`%s' is no a type identifier", NT->name->s_repr);
	}
      type = NT->name->s_type;
      type_ptr = NT->name->s_type_ptr;
    }
  define_symbol (type_symbol, TYPE_IDENTIFIER, block_level, type, type_ptr, 0);
}

/* _________________________________________________________________________

   search_field_name
   _________________________________________________________________________ */

symbol_t *
search_field_name (const char *	name,
		   pascal_type	type,
		   void *	type_ptr,
		   char *	prefix)
{
  symbol_t * ret;
  char * next_prefix;

  switch (type)
    {
    case FIELD_LIST_NODE:
      ret = search_field_name(name,
			      RECORD_SECTION_NODE, FL->fixed_part, prefix);
      if (ret != 0)
	{
	  return (ret);
	}
      return (search_field_name(name, VARIANT_NODE,
				FL->variant_part, prefix));
    case RECORD_SECTION_NODE:
      while (type_ptr != 0)
	{
	  ret = search_field_name(name, DECLARATOR_NODE, RS->name, prefix);
	  if (ret != 0)
	    {
	      return (ret);
	    }
	  type_ptr = RS->next;
	}
      return (0);
    case VARIANT_NODE:
      if (type_ptr == 0)
	{
	  return (0);
	}
      if (V->pseudo_name != 0)
	{
	  next_prefix = strcpye(prefix, ".");
	  next_prefix = strcpye(next_prefix, V->pseudo_name->s_repr);
	}
      else
	{
	  next_prefix = 0;
	}
      ret = search_field_name(name, VARIANT_FIELD_LIST_NODE,
			      V->variant_field_list, next_prefix);
      if (ret != 0)
	{
	  return (ret);
	}
      *prefix = 0;
      return (0);
    case VARIANT_FIELD_LIST_NODE:
      while (type_ptr != 0)
	{
	  if (VFL->pseudo_name != 0)
	    {
	      next_prefix = strcpye(prefix, ".");
	      next_prefix = strcpye(next_prefix, VFL->pseudo_name->s_repr);
	    }
	  else
	    {
	      next_prefix = 0;
	    }
	  ret = search_field_name(name, RECORD_SECTION_NODE,
				  VFL->fixed_part, next_prefix);
	  if (ret != 0)
	    {
	      return (ret);
	    }
	  ret = search_field_name(name, VARIANT_NODE,
				  VFL->variant_part, next_prefix);
	  if (ret != 0)
	    {
	      return (ret);
	    }
	  type_ptr = VFL->next;
	}
      *prefix = 0;
      return (0);
    case DECLARATOR_NODE:
      while (type_ptr != 0)
	{
	  if (strcmp(name, DCL->name->s_repr) == 0)
	    {
	      return (DCL->name);
	    }
	  type_ptr = DCL->next;
	}
      return (0);

    default:
      c4p_error ("internal error: search_field_name: unknown type: %u", type);
      return (0);
    }
}

/* _________________________________________________________________________

   translate_field_name
   _________________________________________________________________________ */

pascal_type
translate_field_name (const char *	name,
		      record_node *	rec,
		      void **		field_type_ptr_ptr)
{
  char prefix[100];
  symbol_t *sym;
  prefix[0] = 0;
  sym = search_field_name(name, FIELD_LIST_NODE, rec->field_list, prefix);
  if (sym == 0)
    {
      c4p_error ("`%s' is not a field identifier", name);
    }
  *field_type_ptr_ptr = sym->s_type_ptr;
  out_form ("%s.%s", prefix, name);
  return (sym->s_type);
}

/* _________________________________________________________________________

   mark_type_table
   _________________________________________________________________________ */

void
mark_type_table ()
{
  type_table_mark = type_table_top;
}

/* _________________________________________________________________________

   unmark_type_table
   _________________________________________________________________________ */

void
unmark_type_table ()
{
  type_table_top = type_table_mark;
}

/* _________________________________________________________________________

   coerce
   _________________________________________________________________________ */

pascal_type
coerce (pascal_type	type_1,
	pascal_type	type_2)
{
  pascal_type t1 = type_1, t2 = type_2;

  if (type_1 == SUBRANGE_NODE)
    {
      type_1 = INTEGER_TYPE;
    }
  if (type_2 == SUBRANGE_NODE)
    {
      type_2 = INTEGER_TYPE;
    }
  if (type_1 == type_2)
    {
      return (type_1);
    }
  if (type_1 == REAL_TYPE && type_2 == LONG_REAL_TYPE)
    {
      return (LONG_REAL_TYPE);
    }
  if (type_2 == REAL_TYPE && type_1 == LONG_REAL_TYPE)
    {
      return (LONG_REAL_TYPE);
    }
  if (type_1 == INTEGER_TYPE && type_2 == REAL_TYPE)
    {
      return (REAL_TYPE);
    }
  if (type_1 == INTEGER_TYPE && type_2 == LONG_REAL_TYPE)
    {
      return (LONG_REAL_TYPE);
    }
  if (type_1 == INTEGER_TYPE && type_2 == CHARACTER_TYPE)
    {
      return (INTEGER_TYPE);
    }
  if (type_1 == INTEGER_TYPE && type_2 == BOOLEAN_TYPE)
    {
      return (INTEGER_TYPE);
    }
  if (type_2 == INTEGER_TYPE && type_1 == REAL_TYPE)
    {
      return (REAL_TYPE);
    }
  if (type_2 == INTEGER_TYPE && type_1 == LONG_REAL_TYPE)
    {
      return (LONG_REAL_TYPE);
    }
  if (type_2 == INTEGER_TYPE && type_1 == CHARACTER_TYPE)
    {
      return (INTEGER_TYPE);
    }
  if (type_2 == INTEGER_TYPE && type_1 == BOOLEAN_TYPE)
    {
      return (INTEGER_TYPE);
    }
#if 0
  c4p_error ("can't coerce type %u to type %u", t1, t2);
#else
  c4p_warning ("can't coerce type %u to type %u", t1, t2);
  return (UNKNOWN_TYPE);
#endif
}
