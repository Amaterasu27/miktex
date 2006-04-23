/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/spc_color.c,v 1.2 2005/07/03 20:02:29 csc Exp $
    
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

#include "pdfdev.h"
#include "pdfparse.h"
#include "pdfcolor.h"

#include "specials.h"

#include "spc_util.h"
#include "spc_color.h"

int
spc_color_at_begin_page (void)
{
  return 0;
}

int
spc_color_at_end_page (void)
{
  return 0;
}

int
spc_color_at_begin_document (void)
{
  return 0;
}

int
spc_color_at_end_document (void)
{
  return 0;
}

static int
spc_handler_color_push (struct spc_env *spe, struct spc_arg *args)
{
  int        error;
  pdf_color  colorspec;

  error = spc_util_read_colorspec(&colorspec, spe, args, 1);
  if (!error) {
    pdf_color_push(); /* save currentcolor */
    pdf_dev_setcolor(&colorspec, 0);
    pdf_dev_setcolor(&colorspec, 1);
  }

  return error;
}

static int
spc_handler_color_pop  (struct spc_env *spe, struct spc_arg *args)
{
  pdf_color_pop();

  return 0;
}

static int
spc_handler_color_default (struct spc_env *spe, struct spc_arg *args)
{
  int        error;
  pdf_color  colorspec;

  error = spc_util_read_colorspec(&colorspec, spe, args, 1);
  if (!error) {
    pdf_color_set_default(&colorspec);
    pdf_dev_setcolor(&colorspec, 0);
    pdf_dev_setcolor(&colorspec, 1);
  }

  return error;
}


int
spc_color_check_special (const char *buffer, long size)
{
  char *p, *endptr;

  p      = (char *) buffer;
  endptr = p + size;

  skip_white(&p, endptr);
  size   = (long) (endptr - p);
  if (size >= strlen("color") &&
      !strncmp(p, "color", strlen("color"))) {
    return 1;
  }

  return 0;
}

extern int
spc_color_setup_handler (struct spc_handler *handle,
			 struct spc_env *spe, struct spc_arg *args)
{
  char  *p, *q;

  ASSERT(handle && spe && args);

  skip_white(&args->curptr, args->endptr);
  if (args->curptr + strlen("color") > args->endptr ||
      strncmp(args->curptr, "color", strlen("color"))) {
    spe->errmsg = (char *) "No valid key for color special found.";
    return -1;
  }

  args->curptr += strlen("color");

  skip_white(&args->curptr, args->endptr);

  p = args->curptr;
  q = parse_c_ident(&p, args->endptr);
  if (!q)
    return -1;

  if (!strcmp(q, "push")) {
    args->command = (char *) "push";
    handle->exec  = &spc_handler_color_push;
    args->curptr = p;
  } else if (!strcmp(q, "pop")) {
    args->command = (char *) "pop";
    handle->exec = &spc_handler_color_pop;
    args->curptr = p;
  } else {
    /* Is this really allowed??? */
    args->command = (char *) "";
    handle->exec = &spc_handler_color_default;
  }
  RELEASE(q);

  skip_white(&args->curptr, args->endptr);

  return  0;
}

