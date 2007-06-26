/*  $Header: /home/cvsroot/dvipdfmx/src/spc_color.c,v 1.5 2005/07/30 11:44:18 hirata Exp $
    
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
#include "dpxutil.h"

#include "pdfdev.h"
#include "pdfcolor.h"

#include "specials.h"

#include "spc_util.h"
#include "spc_color.h"


/* Color stack is actually placed into pdfcolor.c.
 * The reason why we need to place stack there is
 * that we must reinstall color after grestore and
 * other operations that can change current color
 * implicitely.
 */

int
spc_color_at_begin_page (void)
{
  return  0;
}

int
spc_color_at_end_page (void)
{
  return  0;
}

int
spc_color_at_begin_document (void)
{
  return  0;
}

int
spc_color_at_end_document (void)
{
  return  0;
}

static int
spc_handler_color_push (struct spc_env *spe, struct spc_arg *args)
{
  int        error = 0;
  pdf_color  colorspec;

  pdf_color_push(); /* save currentcolor */
  if (args->curptr < args->endptr) {
    error = spc_util_read_colorspec(spe, &colorspec, args, 1);
    if (!error) {
      pdf_dev_setcolor(&colorspec, 0);
      pdf_dev_setcolor(&colorspec, 1);
    }
  }
  
  return  error;
}

static int
spc_handler_color_pop  (struct spc_env *spe, struct spc_arg *args)
{
  pdf_color_pop();

  return  0;
}

/* _FIXME_ ... I don't understand this well.
 * Dvips's implementation is to clear color stack and then
 * push color?
 */
static int
spc_handler_color_default (struct spc_env *spe, struct spc_arg *args)
{
  int        error;
  pdf_color  colorspec;

  error = spc_util_read_colorspec(spe, &colorspec, args, 1);
  if (!error) {
    pdf_color_clear();
    pdf_dev_setcolor(&colorspec, 0);
    pdf_dev_setcolor(&colorspec, 1);
    pdf_color_push(); /* save currentcolor */
  }

  return  error;
}


/* This is from color special? */
#include "pdfdoc.h"

static int
spc_handler_background (struct spc_env *spe, struct spc_arg *args)
{
  int        error;
  pdf_color  colorspec;

  error = spc_util_read_colorspec(spe, &colorspec, args, 1);
  if (!error)
    pdf_doc_set_bgcolor(&colorspec);

  return  error;
}


#ifndef ISBLANK
#define ISBLANK(c) ((c) == ' ' || (c) == '\t' || (c) == '\v')
#endif
static void 
skip_blank (char **pp, char *endptr)
{
  char  *p = *pp;
  for ( ; p < endptr && ISBLANK(*p); p++);
  *pp = p;
}

int
spc_color_check_special (const char *buf, long len)
{
  int   r = 0;
  char *q, *p, *endptr;

  p      = (char *) buf;
  endptr = p + len;

  skip_blank(&p, endptr);
  q = parse_c_ident(&p, endptr);
  if (!q)
    return  0;
  else if (!strcmp(q, "color"))
    r = 1;
  else if (!strcmp(q, "background")) {
    r = 1;
  }
  RELEASE(q);

  return  r;
}

extern int
spc_color_setup_handler (struct spc_handler *sph,
			 struct spc_env *spe, struct spc_arg *ap)
{
  char  *p, *q;

  ASSERT(sph && spe && ap);

  skip_blank(&ap->curptr, ap->endptr);
  q = parse_c_ident(&ap->curptr, ap->endptr);
  if (!q)
    return  -1;
  skip_blank(&ap->curptr, ap->endptr);

  if (!strcmp(q, "background")) {
    ap->command = (char *) "background";
    sph->exec   = &spc_handler_background;
    RELEASE(q);
  } else if (!strcmp(q, "color")) { /* color */
    RELEASE(q);
    p = ap->curptr;

    q = parse_c_ident(&p, ap->endptr);
    if (!q)
      return  -1;
    else if (!strcmp(q, "push")) {
      ap->command = (char *) "push";
      sph->exec   = &spc_handler_color_push;
      ap->curptr  = p;
    } else if (!strcmp(q, "pop")) {
      ap->command = (char *) "pop";
      sph->exec   = &spc_handler_color_pop;
      ap->curptr  = p;
    } else { /* cmyk, rgb, ... */
      ap->command = (char *) "";
      sph->exec   = &spc_handler_color_default;
    }
    RELEASE(q);
  } else {
    spc_warn(spe, "Not color/background special?");
    RELEASE(q);
    return  -1;
  }

  skip_blank(&ap->curptr, ap->endptr);
  return  0;
}
