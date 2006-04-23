/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/spc_html.c,v 1.2 2005/07/03 20:02:29 csc Exp $

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ctype.h>

#include "system.h"
#include "mem.h"
#include "error.h"

#include "dvi.h"

#include "pdfparse.h"

#include "pdfdraw.h"
#include "pdfdev.h"

#include "pdfdoc.h"

#include "specials.h"
#include "spc_util.h"

#include "spc_html.h"


#define HREF 0
#define NAME 1

static pdf_obj *link_dict    = NULL;
static char    *base_url     = NULL;
static int      pending_type = -1;

int
spc_html_at_begin_page (void)
{
  return 0;
}

int
spc_html_at_end_page (void)
{
  return 0;
}

int
spc_html_at_begin_document (void)
{
  link_dict    = NULL;
  base_url     = NULL;
  pending_type = -1;

  return 0;
}

int
spc_html_at_end_document (void)
{
  if (base_url) {
    RELEASE(base_url);
  }
  if (pending_type >= 0 || link_dict) {
    WARN("Unbalanced HTML tag found.");
  }
  if (link_dict) {
    pdf_release_obj(link_dict);
  }

  pending_type = -1;
  base_url     = NULL;
  link_dict    = NULL;

  return 0;
}


static void
downcase (char *s)
{
  while (*s != 0) {
    if (*s >= 'A' && *s <= 'Z') {
      *s = (*s - 'A') + 'a';
    }
    s++;
  }
}

static int
html_make_link_dict (struct spc_env *spe, struct spc_arg *args,
		     const char *name)
{
  pdf_obj  *color;

  if (link_dict) {
    spe->errmsg = (char *) "Attempt to nest links.";
    return -1;
  }
  if (!name) {
    spe->errmsg = (char *) "No URI specified.";
    return -1;
  }

  link_dict = pdf_new_dict();
  pdf_add_dict(link_dict,
	       pdf_new_name("Type"),    pdf_new_name ("Annot"));
  pdf_add_dict(link_dict,
	       pdf_new_name("Subtype"), pdf_new_name ("Link"));

  color = pdf_new_array ();
  pdf_add_array(color, pdf_new_number(0.0));
  pdf_add_array(color, pdf_new_number(0.0));
  pdf_add_array(color, pdf_new_number(1.0));
  pdf_add_dict(link_dict, pdf_new_name("C"), color);

  if (name[0] == '#' &&
      !base_url && strlen(name) > 1) {
    pdf_add_dict(link_dict,
		 pdf_new_name("Dest"),
		 pdf_new_string(name + 1, strlen(name + 1)));
  } else { /* Assume this is URL */
    char     *url;
    int       len;
    pdf_obj  *action;

    len = strlen(name);
    if (base_url) {
      len += strlen(base_url);
    }
    url = NEW(len + 1, char);
    if (base_url) {
      strcpy(url, base_url);
      strcat(url, name);
    } else {
      strcpy(url, name);
    }

    action = pdf_new_dict();
    pdf_add_dict(action,
		 pdf_new_name("Type"),
		 pdf_new_name("Action"));
    pdf_add_dict(action,
		 pdf_new_name("S"),
		 pdf_new_name("URI"));
    pdf_add_dict(action,
		 pdf_new_name("URI"),
		 pdf_new_string(url, len));
    pdf_add_dict(link_dict,
		 pdf_new_name("A"),
		 pdf_ref_obj(action));
    pdf_release_obj(action);
    RELEASE(url);
  }

  pdf_doc_begin_annot(link_dict);
  dvi_tag_depth();

  return 0;
}

static int
html_make_dest (struct spc_env *spe, struct spc_arg *args,
		const char *name)
{
  int        error;
  pdf_obj   *array, *page_ref;
  pdf_coord  cp;

  cp.x = spe->x_user; cp.y = spe->y_user;
  pdf_dev_transform(&cp, NULL);

  page_ref = pdf_doc_this_page_ref();
  if (!page_ref) {
    spe->errmsg = (char *) "No currentpage...";
    return -1;
  }

  array = pdf_new_array();
  pdf_add_array(array, page_ref);
  pdf_add_array(array, pdf_new_name("XYZ"));
  pdf_add_array(array, pdf_new_null());
  pdf_add_array(array, pdf_new_number(cp.y + 24.0));
  pdf_add_array(array, pdf_new_null());

  error = pdf_doc_add_names("Dests",
			    name, strlen(name),
			    pdf_ref_obj(array));
  if (error) {
    spe->errmsg = (char *) "Failed to add named destination.";
  }
  pdf_release_obj(array);

  return error;
}

static int
spc_handler_html_begin_anchor (struct spc_env *spe, struct spc_arg *args)
{
  int    error = 0;
  char  *attr, *value;

  ASSERT(spe && args);

  if (pending_type >= 0 || link_dict) {
    spe->errmsg = (char *) "Nested html anchors.";
    return -1;
  }

  skip_white(&args->curptr, args->endptr);
  while (!error &&
	 args->curptr < args->endptr) {

    parse_key_val(&args->curptr, args->endptr, &attr, &value);
    if (!attr || !value) {
      if (attr)
	RELEASE(attr);
      if (value)
	RELEASE(value);
      spe->errmsg = (char *) "Missing attr-name=attr-value pair.";
      return -1;
    }

    downcase(attr);
    if (!strcmp(attr, "href")) {
      error = html_make_link_dict(spe, args, value);
      pending_type = HREF;
    } else if (!strcmp(attr, "name")) {
      error = html_make_dest(spe, args, value);
      pending_type = NAME;
    } else {
      /* Ignore */
    }

    RELEASE(attr);
    RELEASE(value);

    skip_white(&args->curptr, args->endptr);
  }
  if (error)
    return error;

  if (pending_type < 0 && !link_dict) {
    spe->errmsg = (char *) "Unknown anchor type.";
    return -1;
  }

  return 0;
}

static int
spc_handler_html_end_anchor (struct spc_env *spe, struct spc_arg *args)
{
  ASSERT(spe && args);

  switch (pending_type) {
  case HREF:
    if (link_dict) {
      dvi_untag_depth();
      pdf_doc_end_annot();
      pdf_release_obj(link_dict);
      link_dict = NULL;
      pending_type = -1;
    } else {
      spe->errmsg = (char *) "Ending anchor tag without starting tag!";
      return -1;
    }
    break;
  case NAME:
    pending_type = -1;
    break;
  default:
    spe->errmsg = (char *) "No corresponding opening tag.";
    return -1;
    break;
  }

  return 0;
}

static int
spc_handler_html_base (struct spc_env *spe, struct spc_arg *args)
{
  char  *attr, *value;

  ASSERT(spe && args);

  skip_white(&args->curptr, args->endptr);

  while (args->curptr < args->endptr) {

    attr = value = NULL;
    parse_key_val(&args->curptr, args->endptr, &attr, &value);
    if (!attr || !value) {
      if (attr)
	RELEASE(attr);
      if (value)
	RELEASE(value);
      spe->errmsg = (char *) "Syntax error in html tag.";
      return -1;
    }

    if (!strcmp(attr, "href")) {
      if (base_url)
	RELEASE(base_url);

      base_url = NEW(strlen(value) + 1, char);
      strcpy(base_url, value);
    }

    RELEASE(attr);
    RELEASE(value);

    skip_white(&args->curptr, args->endptr);
  }

  return 0;
}

#define HTML_ELEM_NAME_MAX 32

/* Read tag without attributes and <>.
 * a, /a, img, ...
 */
static int
read_html_tag (char *tag,
	       struct spc_env *spe, struct spc_arg *args)
{
  int  pos = 0, ch;

  skip_white(&args->curptr, args->endptr);
  if (args->curptr >= args->endptr) {
    spe->errmsg = (char *) "No valid HTML element found.";
    return -1;
  }

  if (args->curptr[0] == '/') {
    tag[pos++] = '/';
    args->curptr++;
  }

  skip_white(&args->curptr, args->endptr);

  while (args->curptr < args->endptr &&
	 !isspace(args->curptr[0])) {
    ch = args->curptr[0];
    if (ch >= 'A' && ch <= 'Z') {
      ch = ch - 'A' + 'a';
    } else if (ch == '/') {
      break;
    } else if (!isalpha(ch) && !isdigit(ch)) {
      spe->errmsg = (char *) "Invalid character in HTML tag.";
      return -1;
    }

    if (pos >= HTML_ELEM_NAME_MAX) {
      spe->errmsg = (char *) "HTML element name too long.";
      return -1;
    }

    tag[pos++] = ch;
    args->curptr++;
  }
  tag[pos] = '\0';

  return 0;
}

static int
spc_handler_html_image (struct spc_env *spe, struct spc_arg *args)
{
  ASSERT(spe && args);

  spe->errmsg = (char *) "IMG tag not yet supported...";
  return -1;
}


static struct spc_handler html_handlers[] = {
  {"a"  ,  spc_handler_html_begin_anchor},
  {"/a" ,  spc_handler_html_end_anchor},
  {"img",  spc_handler_html_image},
  {"base", spc_handler_html_base}
};

int
spc_html_check_special (const char *buffer, long size)
{
  char  *p, *endptr;

  p      = (char *) buffer;
  endptr = p + size;

  skip_white(&p, endptr);
  size   = (long) (endptr - p);
  if (size >= strlen("html:") &&
      !strncmp(p, "html:", strlen("html:"))) {
    return 1;
  }

  return 0;
}


int
spc_html_setup_handler (struct spc_handler *handle,
			struct spc_env *spe, struct spc_arg *args)
{
  int    error = 0;
  char  *p, tag[HTML_ELEM_NAME_MAX + 1];
  int    i;

  ASSERT(handle && spe && args);

  skip_white(&args->curptr, args->endptr);
  if (args->curptr + strlen("html:") > args->endptr ||
      strncmp(args->curptr, "html:", strlen("html:"))) {
    spe->errmsg = (char *) "No html: special found.";
    return -1;
  }

  args->curptr += strlen("html:");

  skip_white(&args->curptr, args->endptr);
  if (args->curptr >= args->endptr) {
    spe->errmsg = (char *) "No valid HTML tag found.";
    return -1;
  } else if (args->curptr[0] != '<' ) {
    spe->errmsg = (char *) "HTML tag not starting with a '<' ?";
    return -1;
  }
  args->curptr++;

  /* Set endptr to last '>'. */
  p = args->endptr;
  while (--p > args->curptr) {
    if (p[0] == '>')
      break;
  }
  if (p < args->curptr) {
    spe->errmsg = (char *) "HTML tag not properly terminated...";
    return -1;
  }
  args->endptr = p;

  error = read_html_tag(tag, spe, args);
  if (error)
    return error;

  for (i = 0;
       i < sizeof(html_handlers)/sizeof(struct spc_handler); i++) {
    if (!strcmp(tag, html_handlers[i].key)) {

      args->command  = (char *) html_handlers[i].key;
      skip_white(&args->curptr, args->endptr);

      handle->key    = (char *) "html:";
      handle->exec   = html_handlers[i].exec;

      return 0;
    }
  }

  return -1;
}
