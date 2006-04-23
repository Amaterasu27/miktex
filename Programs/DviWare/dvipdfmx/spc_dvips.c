/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/spc_dvips.c,v 1.2 2005/07/03 20:02:29 csc Exp $
    
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

#include "system.h"
#include "mem.h"
#include "error.h"

#include "dpxfile.h"

#include "pdfparse.h"

#include "pdfdoc.h"

#include "psimage.h"
#include "mpost.h"

#include "pdfximage.h"
#include "pdfdraw.h"
#include "pdfcolor.h"
#include "pdfdev.h"

#include "specials.h"
#include "spc_util.h"

#include "spc_dvips.h"


static int    block_pending = 0;
static double pending_x     = 0.0;
static double pending_y     = 0.0;
static int    position_set  = 0;


/* =filename ... */
static int
spc_handler_ps_file (struct spc_env *spe, struct spc_arg *args)
{
  int            form_id;
  char          *filename;
  transform_info p;
  int            quoted = 0;

  ASSERT(spe && args);

  skip_white(&args->curptr, args->endptr);
  if (args->curptr + 1 >= args->endptr ||
      args->curptr[0] != '=') {
    spe->errmsg = (char *) "No filename specified.";
    return -1;
  }
  args->curptr++;

  if (args->curptr[0] == '\"') {
    args->curptr++;
    quoted = 1;
  }
  filename = parse_val_ident(&args->curptr, args->endptr);
  if (!filename) {
    spe->errmsg = (char *) "Expecting filename but not found...";
    return -1;
  }
  if (quoted) {
    if (filename[strlen(filename)-1] == '\"') {
      filename[strlen(filename)-1] = '\0';
    } else {
      spe->errmsg = (char *) "Invalid value.";
      RELEASE(filename);
      return -1;
    }
  }

  transform_info_clear(&p);
  if (spc_util_read_dimension(&p, spe, args, 1) < 0) {
    RELEASE(filename);
    return -1;
  }

  form_id = pdf_ximage_findresource(filename);
  if (form_id < 0) {
    spe->errmsg = (char *) "Failed to open image file...";
    RELEASE(filename);
    return -1;
  }
  RELEASE(filename);

  pdf_dev_put_image(form_id, &p, spe->x_user, spe->y_user);

  return 0;
}

static int
spc_handler_ps_plotfile (struct spc_env *spe, struct spc_arg *args)
{
  int            error = 0;
  int            form_id;
  char          *filename;
  transform_info p;

  ASSERT(spe && args);

  skip_white(&args->curptr, args->endptr);
  filename = parse_val_ident(&args->curptr, args->endptr);
  if (!filename) {
    spe->errmsg = (char *) "Expecting filename but not found...";
    return -1;
  }

  form_id = pdf_ximage_findresource(filename);
  if (form_id < 0) {
    spe->errmsg = (char *) "Could not find image...";
    error = -1;
  } else {
    transform_info_clear(&p);
    p.yscale = -1.0; p.xscale = 1.0;
#if 0
    /* I don't know how to treat this... */
    pdf_dev_put_image(form_id, &p,
		      block_pending ? pending_x : spe->x_user,
		      block_pending ? pending_y : spe->y_user);
#endif
    pdf_dev_put_image(form_id, &p, 0, 0);
  }
  RELEASE(filename);

  return error;
}

static int
spc_handler_ps_literal (struct spc_env *spe, struct spc_arg *args)
{
  int     error = 0;
  int     st_depth, gs_depth;
  double  x_user, y_user;

  ASSERT(spe && args);

  if (args->curptr >= args->endptr) {
    spe->errmsg = (char *) "No literal PS code found.";
    return -1;
  }

  if (args->curptr + strlen(":[begin]") <= args->endptr &&
      !strncmp(args->curptr, ":[begin]", strlen(":[begin]"))) {
    block_pending++;
    position_set = 1;

    x_user = pending_x = spe->x_user;
    y_user = pending_y = spe->y_user;
    args->curptr += strlen(":[begin]");
  } else if (args->curptr + strlen(":[end]") <= args->endptr &&
	     !strncmp(args->curptr, ":[end]", strlen(":[end]"))) {
    if (block_pending <= 0) {
      spe->errmsg = (char *) "No corresponding ::[begin] found.";
      return -1;
    }
    block_pending--;

    position_set = 0;

    x_user = pending_x;
    y_user = pending_y;
    args->curptr += strlen(":[end]");
  } else if (args->curptr < args->endptr &&
	     args->curptr[0] == ':') {
    x_user = position_set ? pending_x : spe->x_user;
    y_user = position_set ? pending_y : spe->y_user;
    args->curptr++;
  } else {
    position_set = 1;
    x_user = pending_x = spe->x_user;
    y_user = pending_y = spe->y_user;
  }

  skip_white(&args->curptr, args->endptr);
  if (args->curptr < args->endptr) {

    st_depth = mps_stack_depth();
    gs_depth = pdf_dev_current_depth();

    error = mps_exec_inline(&args->curptr,
			    args->endptr,
			    x_user, y_user);
    if (error) {
      spe->errmsg =
	(char *) "Interpreting PS code failed!!! Output might be broken!!!";
      pdf_dev_grestore_to(gs_depth);
    }

    if (st_depth != mps_stack_depth()) {
      WARN("Stack not empty after execution of inline PostScript code.");
      WARN("Your macro package makes some assumption on internal behaviour of DVI drivers.");
      WARN("It may not compatible with dvipdfmx.");
    }
  }

  return error;
}

static int
spc_handler_ps_default (struct spc_env *spe, struct spc_arg *args)
{
  int  error;
  int  st_depth, gs_depth;

  ASSERT(spe && args);

  pdf_dev_gsave();

  st_depth = mps_stack_depth();
  gs_depth = pdf_dev_current_depth();

  error = mps_exec_inline(&args->curptr,
			  args->endptr,
			  spe->x_user, spe->y_user);
  if (error) {
    spe->errmsg =
      (char *) "Interpreting PS code failed!!! Output might be broken!!!";
  }
  if (st_depth != mps_stack_depth()) {
    WARN("Stack not empty after execution of inline PostScript code.");
    WARN("Your macro package makes some assumption on internal behaviour of DVI drivers.");
    WARN("It may not compatible with dvipdfmx.");
  }

  pdf_dev_grestore_to(gs_depth);
  pdf_dev_grestore();

  return error;
}

static struct spc_handler dvips_handlers[] = {
  {"PSfile",        spc_handler_ps_file},
  {"psfile",        spc_handler_ps_file},
  {"ps: plotfile",  spc_handler_ps_plotfile}, /* FIXME */
  {"PS: plotfile",  spc_handler_ps_plotfile}, /* FIXME */
  {"PS:",           spc_handler_ps_literal},
  {"ps:",           spc_handler_ps_literal},
  {"\" ",           spc_handler_ps_default}
};

int
spc_dvips_at_begin_page (void)
{
  return 0;
}

int
spc_dvips_at_end_page (void)
{
  return 0;
}

int
spc_dvips_at_begin_document (void)
{
  return 0;
}

int
spc_dvips_at_end_document (void)
{
  return 0;
}

int
spc_dvips_check_special (const char *buffer, long size)
{
  char *p, *endptr;
  int   i;

  p      = (char *) buffer;
  endptr =  p + size;

  skip_white(&p, endptr);
  if (p >= endptr) {
    return 0;
  }

  size = (long) (endptr - p);
  for (i = 0;
       i < sizeof(dvips_handlers)/sizeof(struct spc_handler); i++) {
    if (size >= strlen(dvips_handlers[i].key) &&
	!strncmp(p, dvips_handlers[i].key,
		 strlen(dvips_handlers[i].key))) {
      return 1;
    }
  }

  return 0;
}

int 
spc_dvips_setup_handler (struct spc_handler *handle,
			 struct spc_env *spe, struct spc_arg *args)
{
  char *key;
  int   i, keylen;

  ASSERT(handle && spe && args);

  skip_white(&args->curptr, args->endptr);

  key = args->curptr;
  while (args->curptr < args->endptr &&
	 isalpha(args->curptr[0])) {
    args->curptr++;
  }
  /* ps:: --> : */
  if (args->curptr < args->endptr &&
      args->curptr[0] == ':') {
    args->curptr++;
  }

  keylen = (int) (args->curptr - key);
  if (keylen < 1) {
    spe->errmsg = (char *) "No ps special found.";
    return -1;
  }

  for (i = 0;
       i < sizeof(dvips_handlers) / sizeof(struct spc_handler); i++) {
    if (keylen == strlen(dvips_handlers[i].key) &&
	!strncmp(key, dvips_handlers[i].key, keylen)) {

      skip_white(&args->curptr, args->endptr);

      args->command = (char *) dvips_handlers[i].key;

      handle->key  = (char *) "ps:";
      handle->exec = dvips_handlers[i].exec;

      return 0;
    }
  }

  return -1;
}
