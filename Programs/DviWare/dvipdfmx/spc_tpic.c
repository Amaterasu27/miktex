/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/spc_tpic.c,v 1.2 2005/07/03 20:02:29 csc Exp $

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

#include "pdfresource.h"
#include "pdfparse.h"
#include "pdfdoc.h"

#include "pdfcolor.h"
#include "pdfdraw.h"
#include "pdfdev.h"

#include "specials.h"
#include "spc_util.h"
#include "spc_tpic.h"

/*
 * Following "constant" converts milli-inches to
 * device (in this case PDF stream) coordinates.
 */

#define MI2DEV (0.072/pdf_dev_scale())

static struct
{
  double     pen_size;
  int        fill_shape;
  double     fill_color;
  pdf_coord *path;
  long       path_length;
  long       max_path_length;
} tp_gstate = {
  1.0, 0, 0.0, NULL, 0, 0
};

static void
tpic_clear_state (void) 
{
  if (tp_gstate.path) {
    RELEASE(tp_gstate.path);
    tp_gstate.path = NULL;
  }
  tp_gstate.path_length     = 0;
  tp_gstate.max_path_length = 0;
  tp_gstate.fill_shape = 0;
  tp_gstate.fill_color = 0.0;
}

#define TPIC_DEFAULT_FILL_MODE 0

static double default_fill_color = 0.5;
static int    tp_fill_mode = TPIC_DEFAULT_FILL_MODE;

void
tpic_set_fill_mode (int mode)
{
  tp_fill_mode = mode;
}

int
spc_tpic_at_begin_page (void)
{
  return 0;
}

int
spc_tpic_at_end_page (void)
{
  if (tp_gstate.path_length > 0) {
    WARN("Unflushed tpic path at end of the page.");
  }

  return 0;
}

int
spc_tpic_at_begin_document (void)
{
  int  pdf_ver;

  pdf_ver = pdf_get_version();
  if (tp_fill_mode) {
    if (pdf_ver < 4) {
      WARN("Tpic shading support requires PDF version 1.4.");
      tp_fill_mode = 0;
    }
  }

  return 0;
}

int
spc_tpic_at_end_document (void)
{
  if (tp_gstate.path_length > 0) {
    WARN("Unflushed tpic path at end of the document.");
  }
  tpic_clear_state();

  return 0;
}

/*
 * We can include line-width, dash pattern and other graphic state parameters
 * in ExtGState dictionary. But doing that is not efficient.
 * "g" is gray level.
 */
static void
set_shading (double g)
{
  pdf_obj *gs_obj, *gs_ref;
  long     res_id;
  char     resname[6]; /* GsXX */
  char     wbuf[32];
  int      alpha, len = 0;

  /*
   * The semantics of a fill_color of 0.0 or 0.5 will be to
   * use current painting color known to dvipdfm.
   */
  if (tp_fill_mode != 1) {
    if (g != 0.0) {
      len = sprintf(wbuf, " %.2f g", g);
    }
  } else {
    if (g == 1.0)
      pdf_doc_add_page_content(" 1 g", 4);
    else {
      alpha = ROUND(255.0*(1.0-g), 1.0);
      sprintf(resname, "GsT%02X", alpha);

      res_id = pdf_findresource("ExtGState", resname);
      if (res_id >= 0) {
	gs_ref = pdf_get_resource_reference(res_id);
      } else {
	/* Acrobat 5/Linux not working...
	 * Why???
	 */
	gs_obj = pdf_new_dict();
	/* Type, BM, ... optional (omitted) */
	pdf_add_dict(gs_obj,
		     pdf_new_name("ca"), pdf_new_number(ROUND(alpha/255.0, 0.01)));
	res_id = pdf_defineresource("ExtGState", resname,
				    gs_obj, PDF_RES_FLUSH_IMMEDIATE);
	gs_ref = pdf_get_resource_reference(res_id);
      }
      pdf_doc_add_page_resource("ExtGState", resname, gs_ref);
      len = sprintf(wbuf, " /%s gs 0 g", resname);
    }
  }
  pdf_doc_add_page_content(wbuf, len);
}


static void
show_path (int hidden) 
{
  if (tp_gstate.fill_shape) {
    set_shading(tp_gstate.fill_color);
  }

  if (!hidden) {
    if (tp_gstate.fill_shape)
      pdf_dev_flushpath('b', PDF_FILL_RULE_NONZERO);
    else {
      pdf_dev_flushpath('S', PDF_FILL_RULE_NONZERO);
    }
  } else {
    /*
     * Acrobat claims 'Q' as illegal operation when there are unfinished
     * path (a path without path-painting operator applied)?
     */
    if (tp_gstate.fill_shape)
      pdf_dev_flushpath('f', PDF_FILL_RULE_NONZERO);
    else {
      pdf_dev_newpath();
    }
  }
  tp_gstate.fill_color = 0.0;
  tp_gstate.fill_shape = 0;
}

#define CLOSED_PATH() (tp_gstate.path[0].x == tp_gstate.path[tp_gstate.path_length-1].x && \
		       tp_gstate.path[0].y == tp_gstate.path[tp_gstate.path_length-1].y)

static int
flush_path (struct spc_env *spe, struct spc_arg *args,
	    int hidden, double dash_dot)
{
  double  dash[2];
  int     i;

  /* Make pen_size == 0 equivalent to hidden */
  if (tp_gstate.pen_size == 0.0) {
    hidden = 1;
  }

  if (tp_gstate.path_length <= 1) {
    tpic_clear_state();
    spe->errmsg = (char *) "Not enough points...";
    return -1;
  }

  if (!hidden || tp_gstate.fill_shape) {
    pdf_dev_gsave();

    pdf_dev_setlinecap (1);
    pdf_dev_setlinejoin(1);
    if (tp_gstate.pen_size != 0.0) {
      pdf_dev_setlinewidth(tp_gstate.pen_size);
    }
    if (dash_dot > 0.0) {
      dash[0] = dash_dot * 72.0;
      pdf_dev_setdash(1, dash, 0);
    } else if (dash_dot < 0.0) {
      dash[0] =  tp_gstate.pen_size;
      dash[1] = -dash_dot * 72.0;
      pdf_dev_setdash(2, dash, 0);
    }

    pdf_dev_moveto(spe->x_user + tp_gstate.path[0].x,
		   spe->y_user - tp_gstate.path[0].y);
    for (i = 0;
	 i < tp_gstate.path_length; i++) {
      pdf_dev_lineto(spe->x_user + tp_gstate.path[i].x,
		     spe->y_user - tp_gstate.path[i].y);
    } 
    /* Shading is applied only to closed path. */
    if (!CLOSED_PATH()) {
      tp_gstate.fill_shape = 0;
    }
    show_path(hidden);

    pdf_dev_grestore();
  }
  tpic_clear_state();

  return 0;
}

/*
 * Accroding to
 * "Tpic: Pic for TEX", Tim Morgan, Original by Brian Kernighan, p.20:
 * 
 *  A spline is a smooth curve guided by a set of straight lines just
 *  like the line above. It begins at the same place, ends at the same
 *  place, and in between is tangent to the mid-point of each guiding
 *  line. The syntax for a spline is identical to a (path) line except
 *  for using spline instead of line.
 *
 * Spline is not a curve drawn by spline-fitting points p0, p1, ..., pn,
 * defined by tpic special "pa" command. Instead, a path defined by set
 * of points p0, p1, ... is guiding line mentioned above.
 *
 * Dvipsk draws them as a straight line from p0 to q1 = (p0 + p1)/2,
 * followed by a quadratic B-spline curve with starting point q1, (off-
 * curve) control point p1, end point q2 = (p1 + p2)/2, ..., and a
 * straight line from qn to pn.
 */

static int
spline_path (struct spc_env *spe, struct spc_arg *args, double dash_dot)
{
  double  values[6];
  int     i;

  /* Spline is meaningless for path length of less than 3 */
  if (tp_gstate.path_length <= 2) {
    tpic_clear_state();
    spe->errmsg = (char *) "Not enough points...";
    return -1;
  }

  pdf_dev_gsave();

  pdf_dev_setmiterlimit(1.4);
  if (tp_gstate.pen_size > 0.0) {
    pdf_dev_setlinewidth(tp_gstate.pen_size);
  }
  if (dash_dot > 0.0) {
    values[0] = dash_dot * 72.0;
    pdf_dev_setdash(1, values, 0);
  } else if (dash_dot < 0.0) {
    values[0] =  tp_gstate.pen_size;
    values[1] = -dash_dot * 72.0;
    pdf_dev_setdash(2, values, 0);
    pdf_dev_setlinecap(1);
  }

  pdf_dev_moveto(spe->x_user + tp_gstate.path[0].x,
		 spe->y_user - tp_gstate.path[0].y);
  values[0] = spe->x_user + 0.5*(tp_gstate.path[0].x + tp_gstate.path[1].x);
  values[1] = spe->y_user - 0.5*(tp_gstate.path[0].y + tp_gstate.path[1].y);
  pdf_dev_lineto(values[0], values[1]);

  for (i = 1; i < tp_gstate.path_length - 1; i++) {
    /* B-spline control points */

    values[0] = spe->x_user + 0.5 * (tp_gstate.path[i-1].x + tp_gstate.path[i].x);
    values[1] = spe->y_user - 0.5 * (tp_gstate.path[i-1].y + tp_gstate.path[i].y);
    values[2] = spe->x_user + tp_gstate.path[i].x;
    values[3] = spe->y_user - tp_gstate.path[i].y;
    values[4] = spe->x_user + 0.5 * (tp_gstate.path[i].x + tp_gstate.path[i+1].x);
    values[5] = spe->y_user - 0.5 * (tp_gstate.path[i].y + tp_gstate.path[i+1].y);

    pdf_dev_bspline(values[0], values[1],
		    values[2], values[3], values[4], values[5]);
  } 

  pdf_dev_lineto(spe->x_user + tp_gstate.path[i].x,
		 spe->y_user - tp_gstate.path[i].y);

  if (!CLOSED_PATH()) {
    tp_gstate.fill_shape = 0;
  }
  show_path(0);

  pdf_dev_grestore();

  tpic_clear_state();

  return 0;
}

static int
arc (struct spc_env *spe, struct spc_arg *args, int hidden) 
{
  double values[6];

  if (tp_gstate.pen_size == 0.0) {
    hidden = 1;
  }

  if (spc_util_read_numbers(values, 6, spe, args) != 6) {
    tpic_clear_state();
    spe->errmsg = (char *) "Not enough parameters.";
    return -1;
  }

  if (!hidden || tp_gstate.fill_shape) {
    values[0] *= MI2DEV;
    values[1] *= MI2DEV;
    values[2] *= MI2DEV;
    values[3] *= MI2DEV;
    values[4] *= 180.0 / 3.1415926;
    values[5] *= 180.0 / 3.1415926;

    pdf_dev_gsave();

    pdf_dev_setlinecap(1);
    if (tp_gstate.pen_size > 0.0) {
      pdf_dev_setlinewidth(tp_gstate.pen_size);
    }
    pdf_dev_arcn(spe->x_user + values[0],
		 spe->y_user - values[1],
		 values[2], values[3], -values[4], -values[5]);

    if ((int) (fabs(values[4] - values[5])+0.5) < 360) {
      tp_gstate.fill_shape = 0;
    }

    show_path(hidden);

    pdf_dev_grestore();
  }
  tpic_clear_state();

  return 0;
}

static int
spc_handler_tpic_pn (struct spc_env *spe, struct spc_arg *args)
{
  char  *number;

  ASSERT(spe && args);

  skip_white(&args->curptr, args->endptr);
  number = parse_number(&args->curptr, args->endptr);
  if (!number) {
    spe->errmsg = (char *) "Invalid pen size.";
    return -1;
  }
  tp_gstate.pen_size = atof(number) * MI2DEV;
  RELEASE(number);

  return 0;
}

static int
spc_handler_tpic_pa (struct spc_env *spe, struct spc_arg *args)
{
  double values[2];

  ASSERT(spe && args);

  skip_white(&args->curptr, args->endptr);
  if (spc_util_read_numbers(values, 2, spe, args) != 2) {
    spe->errmsg = (char *) "Missing numbers.";
    return -1;
  }

  if (tp_gstate.path_length >= tp_gstate.max_path_length) {
    tp_gstate.max_path_length += 256;
    tp_gstate.path = RENEW(tp_gstate.path, tp_gstate.max_path_length, pdf_coord);
  }
  tp_gstate.path[tp_gstate.path_length].x = values[0] * MI2DEV;
  tp_gstate.path[tp_gstate.path_length].y = values[1] * MI2DEV;
  tp_gstate.path_length += 1;

  return 0;
}

static int
spc_handler_tpic_fp (struct spc_env *spe, struct spc_arg *args)
{
  ASSERT(spe && args);

  return flush_path(spe, args, 0, 0.0);
}

static int
spc_handler_tpic_ip (struct spc_env *spe, struct spc_arg *args)
{
  ASSERT(spe && args);

  return flush_path(spe, args, 1, 0.0);
}

static int
spc_handler_tpic_da (struct spc_env *spe, struct spc_arg *args)
{
  char  *token;
  double dash = 0.0;

  ASSERT(spe && args);

  skip_white(&args->curptr, args->endptr);
  token = parse_number(&args->curptr, args->endptr);
  if (token) {
    dash = atof(token);
    RELEASE(token);
  }

  return flush_path(spe, args, 0, dash);
}

static int
spc_handler_tpic_dt (struct spc_env *spe, struct spc_arg *args)
{
  char  *token;
  double dash = 0.0;

  ASSERT(spe && args);

  skip_white(&args->curptr, args->endptr);
  token = parse_number(&args->curptr, args->endptr);
  if (token) {
    dash = -atof(token);
    RELEASE(token);
  }

  return flush_path(spe, args, 0, dash);
}

static int
spc_handler_tpic_sp (struct spc_env *spe, struct spc_arg *args)
{
  char  *token;
  double dash = 0.0;

  ASSERT(spe && args);

  skip_white(&args->curptr, args->endptr);
  token = parse_number(&args->curptr, args->endptr);
  if (token) {
    dash = atof(token);
    RELEASE(token);
  }

  return spline_path(spe, args, dash);
}

static int
spc_handler_tpic_ar (struct spc_env *spe, struct spc_arg *args)
{
  ASSERT(spe && args);

  return arc(spe, args, 0);
}

static int
spc_handler_tpic_ia (struct spc_env *spe, struct spc_arg *args)
{
  ASSERT(spe && args);

  return arc(spe, args, 1);
}

static int
spc_handler_tpic_sh (struct spc_env *spe, struct spc_arg *args)
{
  char *number;

  ASSERT(spe && args);

  tp_gstate.fill_shape = 1;
  tp_gstate.fill_color = default_fill_color;

  skip_white(&args->curptr, args->endptr);
  number = parse_number(&args->curptr, args->endptr);
  if (number) {
    tp_gstate.fill_color = 1.0 - atof(number);
    RELEASE(number);
  }

  if (tp_gstate.fill_color > 1.0) {
    tp_gstate.fill_color = 1.0;
  }
  if (tp_gstate.fill_color < 0.0) {
    tp_gstate.fill_color = 0.0;
  }

  return 0;
}

static int
spc_handler_tpic_wh (struct spc_env *spe, struct spc_arg *args)
{
  ASSERT(spe && args);

  tp_gstate.fill_shape = 1;
  tp_gstate.fill_color = 1.0;

  return 0;
}

static int
spc_handler_tpic_bk (struct spc_env *spe, struct spc_arg *args)
{
  ASSERT(spe && args);

  tp_gstate.fill_shape = 1;
  tp_gstate.fill_color = 0.0;

  return 0;
}

static int
spc_handler_tpic_tx (struct spc_env *spe, struct spc_arg *args)
{
  long num = 0, den = 0;

  ASSERT(spe && args);

  tp_gstate.fill_shape = 1;

  while (args->curptr++ < args->endptr) {
    switch (*(args->curptr++)) {
    case '0':
      num += 0;
    case '1': case '2': case '4': case '8':
      num += 1;
      break;
    case '3': case '5': case '6': case '9':
    case 'a': case 'A': case 'c': case 'C':
      num += 2;
      break;
    case '7': case 'b': case 'B': case 'd':
    case 'D':
      num += 3;
      break;
    case 'f': case 'F':
      num += 4;
      break;
    default:
      break;
    }
    den += 16;
  }
  if (den != 0) {
    default_fill_color = 1.0 - (float) (num)/(den);
  } else {
    default_fill_color = 0.5;
  }

  return 0;
}

static struct spc_handler tpic_handlers[] = {
  {"pn", spc_handler_tpic_pn},
  {"pa", spc_handler_tpic_pa},
  {"fp", spc_handler_tpic_fp},
  {"ip", spc_handler_tpic_ip},
  {"da", spc_handler_tpic_da},
  {"dt", spc_handler_tpic_dt},
  {"sp", spc_handler_tpic_sp},
  {"ar", spc_handler_tpic_ar},
  {"ia", spc_handler_tpic_ia},
  {"sh", spc_handler_tpic_sh},
  {"wh", spc_handler_tpic_wh},
  {"bk", spc_handler_tpic_bk},
  {"tx", spc_handler_tpic_tx}
};

int
spc_tpic_check_special (const char *buffer, long size)
{
  char *p, *endptr;
  int   i, keylen;

  p      = (char *) buffer;
  endptr = p + size;

  skip_white(&p, endptr);
  if (p >= endptr) {
    return 0;
  }

  size = (long) (endptr - p);
  for (i = 0;
       i < sizeof(tpic_handlers)/sizeof(struct spc_handler); i++) {
    keylen = strlen(tpic_handlers[i].key);
    if (size == keylen &&
	!strncmp(p, tpic_handlers[i].key, keylen)) {
      return 1;
    } else if (size >= keylen + 1 &&
	       !strncmp(p, tpic_handlers[i].key, keylen) &&
	       isspace(p[keylen])) {
      return 1;
    }
  }

  return 0;
}

int
spc_tpic_setup_handler (struct spc_handler *handle,
			struct spc_env *spe, struct spc_arg *args)
{
  char  *key;
  int    i, keylen;

  ASSERT(handle && spe && args);

  skip_white(&args->curptr, args->endptr);

  key = args->curptr;
  while (args->curptr < args->endptr &&
	 isalpha(args->curptr[0])) {
    args->curptr++;
  }

  keylen = (int) (args->curptr - key);
  if (keylen < 1) {
    spe->errmsg = (char *) "Could not find tpic special.";
    return -1;
  }

  for (i = 0;
       i < sizeof(tpic_handlers)/sizeof(struct spc_handler); i++) {
    if (keylen == strlen(tpic_handlers[i].key) &&
	!strncmp(key, tpic_handlers[i].key, keylen)) {

      skip_white(&args->curptr, args->endptr);
      args->command = (char *) tpic_handlers[i].key;

      handle->key   = (char *) "tpic:";
      handle->exec  = tpic_handlers[i].exec;

      return 0;
    }
  }

  return -1;
}
