/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/specials.c,v 1.2 2005/07/03 20:02:29 csc Exp $

    This is dvipdfmx, an eXtended version of dvipdfm by Mark A. Wicks.

    Copyright (C) 2002 by Jin-Hwan Cho and Shunsaku Hirata,
    the dvipdfmx project team <dvipdfmx@project.ktug.or.kr>
    
    Copyright (C) 1998, 1999 by Mark A. Wicks <mwicks@kettering.edu>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*/

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "system.h"
#include "mem.h"
#include "error.h"
#include "numbers.h"

#include "dvi.h"

#include "pdflimits.h"

#include "pdfobj.h"
#include "pdfparse.h"
#include "pdfdoc.h"
#include "pdfnames.h"

#include "pdfdraw.h"
#include "pdfdev.h"

#include "spc_pdfm.h"
#include "spc_tpic.h"
#include "spc_html.h"
#include "spc_misc.h"
#include "spc_color.h"
#include "spc_dvips.h"

#include "specials.h"

static int verbose = 0;
void
spc_set_verbose (void)
{
  verbose++;
}

static struct ht_table *named_objects = NULL;

static int
is_an_int (const char *s)
{
  if (!s || !*s)
    return 0;
  if (*s == '+' || *s == '-')
    s++;
  while (*s) {
    if (!isdigit(*s))
      return 0;
    s++;
  }

  return 1;
}

static struct {
  const char *key;
  double      units;
  int         is_true_unit;
} unit_in_bps[] = {
  {"pt",     (72.0/72.27), 0},
  {"in",     (72.0),       0},
  {"cm",     (72.0/2.54),  0},
  {"mm",     (72.0/25.4),  0},
  {"bp",     1.0,          0},
  {"truept", (72.0/72.27), 1},
  {"truein", (72.0),       1},
  {"truecm", (72.0/2.54),  1},
  {"truemm", (72.0/25.4),  1},
  {"truebp", 1.0,          1}
};
#define NUM_KNOWN_UNITS (sizeof(unit_in_bps)/sizeof(unit_in_bps[0]))

static double
parse_one_unit (char **start, char *end)
{
  double result = -1.0;
  char  *unit_str;
  int    i;
  double mag;

  skip_white(start, end);
  unit_str = parse_c_ident(start, end);
  if (!unit_str) {
    WARN("Expecting a unit here (e.g., in, cm, pt).");
    dump(*start, end);
  } else {
    for (i = 0; i < NUM_KNOWN_UNITS; i++) {
      if (!strcmp(unit_in_bps[i].key, unit_str)) {
	result = unit_in_bps[i].units;
	break;
      }
    }
    if (i >= NUM_KNOWN_UNITS) {
      WARN("Unknown unit of measurement \"%s\".", unit_str);
      dump(*start, end);
    } else {
      if (unit_in_bps[i].is_true_unit) {
	mag = dvi_tell_mag();
	if (mag == 0.0) {
	  result = unit_in_bps[i].units;
	} else {
	  result = unit_in_bps[i].units / mag;
	}
      } else {
	result = unit_in_bps[i].units;
      }
    }
    RELEASE(unit_str);
  }

  return result;
}

double
parse_length (char **start, char *end)
{
  double val = 0.0, one_bp = 0.0;
  char  *nextptr;

  /* Our units does not start with "e". */
  val = strtod(*start, &nextptr);
  if (nextptr == *start) {
    ERROR("Error in length specification: %s", *start);
  } else if (nextptr < end && nextptr != NULL) {
    one_bp = parse_one_unit(&nextptr, end);
    if (one_bp <= 0.0)
      ERROR("Unknown unit specified in: %s", nextptr);
    else if (nextptr != NULL &&
	     nextptr < end && !isspace(*nextptr) &&
	     !strchr("([</,", *nextptr)) {
      ERROR("Unknown unit specified in: %s", *start);
    }
  } else {
    one_bp = 1.0;
  }

  *start = nextptr;
  return (val * one_bp);
}

#define K_XPOS      0
#define K_YPOS      1
#define K_THISPAGE  2
#define K_PREVPAGE  3
#define K_NEXTPAGE  4
#define K_PAGES     5
#define K_NAMES     6
#define K_RESOURCES 7
#define K_CATALOG   8
#define K_DOCINFO   9

static struct {
  const char *key_str;
  int         key_id ;
} reserved_keys[] = {
  {"xpos",      K_XPOS},
  {"ypos",      K_YPOS},
  {"thispage",  K_THISPAGE},
  {"prevpage",  K_PREVPAGE},
  {"nextpage",  K_NEXTPAGE},
  {"pages",     K_PAGES},
  {"names",     K_NAMES},
  {"resources", K_RESOURCES},
  {"catalog",   K_CATALOG},
  {"docinfo",   K_DOCINFO},
  {NULL, -1}
};

static int
get_reserved_key (const char *key)
{
  int  i;

  for (i = 0; reserved_keys[i].key_str; i++) {
    if (!strcmp(reserved_keys[i].key_str, key)) {
      return reserved_keys[i].key_id;
    }
  }

  return -1;
}

/*
 * The following routine returns copies, not the original object.
 */
pdf_obj *
spc_lookup_reference (const char *key)
{
  pdf_obj   *value = NULL;
  pdf_coord  cp;
  int        key_id;

  ASSERT(named_objects);
  ASSERT(key && key[0] != '\0');

  key_id = get_reserved_key(key);

  switch (key_id) {
  case K_XPOS:
    cp.x = dvi_dev_xpos(); cp.y = 0.0;
    pdf_dev_transform(&cp, NULL);
    value = pdf_new_number(ROUND(cp.x, .01));
    break;
  case K_YPOS:
    cp.x = 0.0; cp.y = dvi_dev_ypos();
    pdf_dev_transform(&cp, NULL);
    value = pdf_new_number(ROUND(cp.y, .01));
    break;
  case K_THISPAGE:
    value = pdf_doc_this_page_ref();
    break;
  case K_PREVPAGE:
    value = pdf_doc_prev_page_ref();
    break;
  case K_NEXTPAGE:
    value = pdf_doc_next_page_ref();
    break;
  case K_PAGES:
    value = pdf_ref_obj(pdf_doc_page_tree());
    break;
  case K_NAMES:
    value = pdf_ref_obj(pdf_doc_names());
    break;
  case K_RESOURCES:
    value = pdf_ref_obj(pdf_doc_current_page_resources());
    break;
  case K_CATALOG:
    value = pdf_ref_obj(pdf_doc_catalog());
    break;
  case K_DOCINFO:
    value = pdf_ref_obj(pdf_doc_docinfo());
    break;
  default:
    if (strlen(key) > 4 &&
	!strncmp(key, "page", 4) && is_an_int(key + 4)) {
      value = pdf_doc_ref_page(atoi(key + 4));
    } else {
      value = pdf_names_lookup_reference(named_objects, key, strlen(key));
    }
    break;
  }

  if (!value) {
    ERROR("Object reference %s not exist.", key);
  }

  return value;
}

pdf_obj *
spc_lookup_object (const char *key)
{
  pdf_obj   *value = NULL;
  pdf_coord  cp;
  int        key_id;

  ASSERT(named_objects);
  ASSERT(key && key[0] != '\0');

  key_id = get_reserved_key(key);

  /* First check for builtins first */
  switch (key_id) {
  case K_XPOS:
    cp.x = dvi_dev_xpos(); cp.y = 0.0;
    pdf_dev_transform(&cp, NULL);
    value = pdf_new_number(ROUND(cp.x, .01));
    break;
  case K_YPOS:
    cp.x = 0.0; cp.y = dvi_dev_ypos();
    pdf_dev_transform(&cp, NULL);
    value = pdf_new_number(ROUND(cp.y, .01));
    break;
  case K_THISPAGE:
    value = pdf_doc_this_page();
    break;
  case K_PAGES:
    value = pdf_doc_page_tree();
    break;
  case K_NAMES:
    value = pdf_doc_names();
    break;
  case K_RESOURCES:
    value = pdf_doc_current_page_resources();
    break;
  case K_CATALOG:
    value = pdf_doc_catalog();
    break;
  case K_DOCINFO:
    value = pdf_doc_docinfo();
    break;
  default:
    value = pdf_names_lookup_object(named_objects, key, strlen(key));
    break;
  }

  if (!value) {
    ERROR("Object reference %s not exist.", key);
  }

  return value;
}

void
spc_push_object (const char *key, pdf_obj *value)
{
  int  error = 0;

  ASSERT(value);

  if (PDF_OBJ_INDIRECTTYPE(value)) {
    pdf_names_add_reference(named_objects,
			    key, strlen(key), value);
  } else {
    error = pdf_names_add_object(named_objects,
                                 key, strlen(key), value);
    if (!error) {
      pdf_obj *obj_ref = pdf_names_lookup_reference(named_objects,
                                                    key, strlen(key));
      if (obj_ref)
        pdf_release_obj(obj_ref);
    }
  }

  return;
}

void
spc_flush_object (const char *key)
{
  pdf_names_close_object(named_objects, key, strlen(key));
}

void
spc_clear_objects (void)
{
  pdf_delete_name_tree(&named_objects);
  named_objects = pdf_new_name_tree();
}


static int
spc_handler_unknown (struct spc_env *spe,
		  struct spc_arg *args)
{
  ASSERT(spe && args);

  spe->errmsg  = (char *) "Unknown special ignored.";
  args->curptr = args->endptr;

  return -1;
}

static void
init_special (struct spc_handler *special,
	      struct spc_env *spe,
	      struct spc_arg *args,
	      const char *p, long size,
	      double x_user, double y_user, double mag)
{

  special->key  = NULL;
  special->exec = (spc_handler_fn_ptr) &spc_handler_unknown;

  spe->x_user = x_user;
  spe->y_user = y_user;
  spe->mag    = mag;
  spe->errmsg = NULL;

  args->curptr = (char *) p;
  args->endptr = args->curptr + size;

  return;
}

static void
print_error (const char *spc_type,
	     struct spc_env *spe, struct spc_arg *args)
{
  ASSERT(spe && args);

  if (spe->errmsg) {
    WARN("Interpreting special command %s (%s) failed with:",
	 args->command, spc_type);
    MESG("\n");
    MESG(spe->errmsg);
    MESG("\n");
  }

  return;
}

static void
check_garbage (struct spc_arg *args)
{
  ASSERT(args);

  if (args->curptr >= args->endptr)
    return;

  skip_white(&args->curptr, args->endptr);
  if (args->curptr < args->endptr) {
    WARN("Unparsed material at end of special ignored.");
    dump(args->curptr, args->endptr);
  }

  return;
}

static struct {
  const char  *key;
  int (*bodhk_func) ();
  int (*eodhk_func) ();
  int (*bophk_func) ();
  int (*eophk_func) ();
  int (*check_func) (const char *, long);
  int (*setup_func) (struct spc_handler *, struct spc_env *, struct spc_arg *);
} known_specials[] = {

  {"pdf:",
   spc_pdfm_at_begin_document,
   spc_pdfm_at_end_document,
   spc_pdfm_at_begin_page,
   spc_pdfm_at_end_page,
   spc_pdfm_check_special,
   spc_pdfm_setup_handler
  },

  {"ps:",
   spc_dvips_at_begin_document,
   spc_dvips_at_end_document,
   spc_dvips_at_begin_page,
   spc_dvips_at_end_page,
   spc_dvips_check_special,
   spc_dvips_setup_handler
  },

  {"color",
   spc_color_at_begin_document,
   spc_color_at_end_document,
   spc_color_at_begin_page,
   spc_color_at_end_page,
   spc_color_check_special,
   spc_color_setup_handler
  },

  {"tpic",
   spc_tpic_at_begin_document,
   spc_tpic_at_end_document,
   spc_tpic_at_begin_page,
   spc_tpic_at_end_page,
   spc_tpic_check_special,
   spc_tpic_setup_handler
  },

  {"html:",
   spc_html_at_begin_document,
   spc_html_at_end_document,
   spc_html_at_begin_page,
   spc_html_at_end_page,
   spc_html_check_special,
   spc_html_setup_handler
  },

  {"unknown",
   spc_misc_at_begin_document,
   spc_misc_at_end_document,
   spc_misc_at_begin_page,
   spc_misc_at_end_page,
   spc_misc_check_special,
   spc_misc_setup_handler
  },

  {NULL} /* end */
};

int
spc_exec_at_begin_page (void)
{
  int  error = 0;
  int  i;

  for (i = 0; known_specials[i].key != NULL; i++) {
    if (known_specials[i].bophk_func) {
      error = known_specials[i].bophk_func();
    }
  }

  return error;
}

int
spc_exec_at_end_page (void)
{
  int  error = 0;
  int  i;

  for (i = 0; known_specials[i].key != NULL; i++) {
    if (known_specials[i].eophk_func) {
      error = known_specials[i].eophk_func();
    }
  }

  return error;
}

int
spc_exec_at_begin_document (void)
{
  int  error = 0;
  int  i;

  ASSERT(!named_objects);

  named_objects = pdf_new_name_tree();

  for (i = 0; known_specials[i].key != NULL; i++) {
    if (known_specials[i].bodhk_func) {
      error = known_specials[i].bodhk_func();
    }
  }

  return error;
}

int
spc_exec_at_end_document (void)
{
  int  error = 0;
  int  i;

  for (i = 0; known_specials[i].key != NULL; i++) {
    if (known_specials[i].eodhk_func) {
      error = known_specials[i].eodhk_func();
    }
  }

  if (named_objects) {
    pdf_delete_name_tree(&named_objects);
  }

  return error;
}

int
spc_exec_special (const char *buffer, long size,
		  double x_user, double y_user, double mag)
{
  int    error = -1;
  int    i, found;
  struct spc_env     spe;
  struct spc_arg     args;
  struct spc_handler special;

  if (verbose > 3) {
    dump((char *) buffer, (char *) buffer + size);
  }

  init_special(&special, &spe, &args, buffer, size, x_user, y_user, mag);

  for (i = 0; known_specials[i].key != NULL; i++) {
    found = known_specials[i].check_func(buffer, size);
    if (found) {
      error = known_specials[i].setup_func(&special, &spe, &args);
      if (!error) {
	error = special.exec(&spe, &args);
      }
      if (error) {
	print_error(known_specials[i].key, &spe, &args);
      }
      break;
    }
  } 

  check_garbage(&args);

  return error;
}

