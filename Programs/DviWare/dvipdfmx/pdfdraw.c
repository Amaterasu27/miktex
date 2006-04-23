/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/pdfdraw.c,v 1.2 2005/07/03 20:02:28 csc Exp $
    
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

#include <math.h>

#include "system.h"
#include "error.h"
#include "mem.h"
#include "mfileio.h"

#include "numbers.h"

#include "pdfdoc.h"
#include "pdfdev.h"
#include "pdfcolor.h"

#include "pdfdraw.h"


struct path_element
{
  char type;
  pdf_coord p[3];
};

typedef struct pdf_path
{
  int num_paths;
  int max_paths;
  struct path_element *path;
} pdf_path;

#define GS_FLAG_CURRENTPOINT_SET (1 << 0)

typedef struct pdf_gstate
{
  pdf_coord   cp;

  pdf_tmatrix matrix;   /* cm,  - */

  pdf_color   strokecolor;
  pdf_color   fillcolor;

  struct {
    int     num_dash;
    double  pattern[PDF_DASH_SIZE_MAX];
    double  offset;
  } linedash;           /* d,  D  */

  double    linewidth;  /* w,  LW */

  int       linecap;    /* J,  LC */
  int       linejoin;   /* j,  LJ */
  double    miterlimit; /* M,  ML */

  /* internal */
  pdf_path  path;
  int       flags;
} pdf_gstate;

static struct gstates
{
  int        next;
  pdf_gstate gstates[PDF_GSAVE_MAX];
} gs_stack = {
  0
};

#define CURRENT_GS()     (&gs_stack.gstates[gs_stack.next-1])
#define CURRENT_POINT()  (&gs_stack.gstates[gs_stack.next-1].cp)
#define CURRENT_PATH()   (&gs_stack.gstates[gs_stack.next-1].path)
#define CURRENT_MATRIX() (&gs_stack.gstates[gs_stack.next-1].matrix)

static char fmt_buf[1024];

static void
pd_initpath (pdf_path *p)
{
  ASSERT(p);

  p->num_paths = 0;
  p->max_paths = 0;
  p->path      = NULL;
}

static void
pd_clearpath (pdf_path *p)
{
  ASSERT(p);

  p->num_paths = 0;
}

static void
pd_cleanpath (pdf_path *p)
{
  ASSERT(p);

  if (p->path)
    RELEASE(p->path);
  p->path = NULL;
  p->num_paths = 0;
  p->max_paths = 0;
}

static int
pd_copypath (pdf_path *dst, pdf_path *src)
{
  int  i;

  ASSERT(dst && src);

  if (dst->max_paths < src->num_paths) {
    if (dst->path)
      RELEASE(dst->path);
    dst->max_paths = src->num_paths;
    dst->path = NEW(dst->max_paths, struct path_element);
  }
  dst->num_paths = src->num_paths;

  for (i = 0; i < src->num_paths; i++) {
    struct path_element *pe1, *pe2;

    pe1 = &(dst->path[i]);
    pe2 = &(src->path[i]);
    pe1->type = pe2->type;
    pe1->p[0].x = pe2->p[0].x; pe1->p[0].y = pe2->p[0].y;
    pe1->p[1].x = pe2->p[1].x; pe1->p[1].y = pe2->p[1].y;
    pe1->p[2].x = pe2->p[2].x; pe1->p[2].y = pe2->p[2].y;
  }

  return 0;
}

#define PATH_ALLOC_SIZE 16

static struct path_element *
pd_nextpe (void)
{
  pdf_path  *p;
  pdf_coord *cp;
  struct path_element *pe;

  p  = CURRENT_PATH();
  cp = CURRENT_POINT();

  if (p->num_paths + 2 > p->max_paths) {
    p->max_paths += PATH_ALLOC_SIZE;
    p->path       = RENEW(p->path, p->max_paths, struct path_element);
  }

  if (p->num_paths == 0) {
    pe = &p->path[p->num_paths++];
    pe->type = 'm';
    pe->p[0].x = cp->x;
    pe->p[0].y = cp->y;
  } else {
    pe = &p->path[p->num_paths-1];
    switch (pe->type) {
    case 'm':
      if (pe->p[0].x != cp->x)
	pe->p[0].x = cp->x;
      if (pe->p[0].y != cp->y)
	pe->p[0].y = cp->y;
      break;
    case 'l':
      if (pe->p[0].x != cp->x ||
	  pe->p[0].y != cp->y) {
	pe = &p->path[p->num_paths++];
	pe->type = 'm';
	pe->p[0].x = cp->x;
	pe->p[0].y = cp->y;
      }
      break;
    case 'c':
      if (pe->p[2].x != cp->x ||
	  pe->p[2].y != cp->y) {
	pe = &p->path[p->num_paths++];
	pe->type = 'm';
	pe->p[0].x = cp->x;
	pe->p[0].y = cp->y;
      }
      break;
    case 'y': case 'v':
      if (pe->p[1].x != cp->x ||
	  pe->p[1].y != cp->y) {
	pe = &p->path[p->num_paths++];
	pe->type = 'm';
	pe->p[0].x = cp->x;
	pe->p[0].y = cp->y;
      }
      break;
    }
  }

  return &p->path[p->num_paths++];
}

static void
pd_transform_path (pdf_path *p, const pdf_tmatrix *M)
{
  struct path_element *pe;
  int    n = 0, i;

  ASSERT(p && M);

  for (i = 0; i < p->num_paths; i++) {
    pe = &(p->path[i]);
    switch (pe->type) {
    case 'm': n = 1; break;
    case 'l': n = 1; break;
    case 'c': n = 3; break;
    case 'v': n = 2; break;
    case 'y': n = 2; break;
    case 'h': n = 0; break;
    default:
      ERROR("Unknown path type: %d", pe->type);
    }
    while (n-- > 0) {
      pdf_dev_transform(&(pe->p[n]), M);
    }
  }
}

static void
init_gstate (pdf_gstate *gs)
{
  ASSERT(gs);

  gs->cp.x     = 0.0;
  gs->cp.y     = 0.0;

  gs->matrix.a = 1.0;
  gs->matrix.b = 0.0;
  gs->matrix.c = 0.0;
  gs->matrix.d = 1.0;
  gs->matrix.e = 0.0;
  gs->matrix.f = 0.0;

  pdf_color_graycolor(&gs->strokecolor, 0.0);
  pdf_color_graycolor(&gs->fillcolor  , 0.0);

  gs->linedash.num_dash = 0;
  gs->linedash.offset   = 0;
  gs->linecap    = 0;
  gs->linejoin   = 0;
  gs->linewidth  = 1.0;
  gs->miterlimit = 10.0;

  pd_initpath(&gs->path);
  gs->flags      = 0;

  return;
}

void
pdf_dev_init_gstates (void)
{
  init_gstate(&gs_stack.gstates[0]);
  gs_stack.next = 1;
}

void
pdf_dev_clean_gstates (void)
{
  pdf_gstate *gs;

  if (gs_stack.next > 1) {
    WARN("Graphics state stack depth not 0 at end of document.");
  }
  while (--gs_stack.next >= 0) {
    gs = &gs_stack.gstates[gs_stack.next];
    pd_cleanpath(&gs->path);
  }
}

int
pdf_dev_newpath (void)
{
  pdf_path *p;

  p = CURRENT_PATH();
  if (p && p->num_paths > 0) {
    pd_clearpath(p);
    pdf_doc_add_page_content(" n", 2);
  }

  return 0;
}

int
pdf_dev_moveto (double x, double y)
{
  pdf_coord  *cp;

  cp = CURRENT_POINT();
  cp->x = x;  cp->y = y;

  return 0;
}

int
pdf_dev_rmoveto (double x, double y)
{
  pdf_coord  *cp;

  cp = CURRENT_POINT();
  cp->x += x;  cp->y += y;

  return 0;
}

int
pdf_dev_currentpoint (pdf_coord *p)
{
  pdf_coord  *cp;

  ASSERT(p);

  cp = CURRENT_POINT();
  p->x = cp->x; p->y = cp->y;

  return 0;
}

int
pdf_dev_currentmatrix (pdf_tmatrix *M)
{
  ASSERT(M);

  pdf_copymatrix(M, CURRENT_MATRIX());

  return 0;
}

int
pdf_dev_concat (const pdf_tmatrix *M)
{
  pdf_tmatrix *ctm;
  int          len = 0;
  pdf_tmatrix  M_i;

  ASSERT(M);

  if (fabs((M->a)*(M->d) - (M->b)*(M->c)) < 1.0e-7) {
    WARN("Non invertible transformation matrix.");
    return -1;
  }

  pdf_copymatrix(&M_i, M);
  pdf_invertmatrix(&M_i);

  fmt_buf[len++] = ' ';
  len += pdf_sprint_matrix(fmt_buf + len, M);
  fmt_buf[len++] = ' ';
  fmt_buf[len++] = 'c';
  fmt_buf[len++] = 'm';
  pdf_doc_add_page_content(fmt_buf, len);

  ctm = CURRENT_MATRIX();
  pdf_concatmatrix(ctm, M);

  pd_transform_path(CURRENT_PATH(),  &M_i);
  pdf_dev_transform(CURRENT_POINT(), &M_i);

  return 0;
}

void
pdf_make_tmatrix (pdf_tmatrix *M,
		  double xscale, double yscale, double rotate,
		  double x_user, double y_user)
{
  double  c, s;

  ASSERT(M);

  c = cos(rotate);
  s = sin(rotate);
  M->a =  xscale * c; M->b =  xscale * s;
  M->c = -yscale * s; M->d =  yscale * c;
  M->e = (1.0 - M->a) * x_user - M->c * y_user;
  M->f = (1.0 - M->d) * y_user - M->b * x_user;
}

static void
copy_gstate (pdf_gstate *gs1, pdf_gstate *gs2)
{
  int   i;

  ASSERT(gs1 && gs2);

  gs1->cp.x = gs2->cp.x;
  gs1->cp.y = gs2->cp.y;

  pdf_copymatrix(&gs1->matrix, &gs2->matrix);

  /* TODO:
   * Path should be linked list and gsave only
   * record starting point within path rather than
   * copying whole path.
   */
  pd_copypath(&gs1->path, &gs2->path);

  gs1->linedash.num_dash = gs2->linedash.num_dash;
  for (i = 0; i < gs2->linedash.num_dash; i++) {
    gs1->linedash.pattern[i] = gs2->linedash.pattern[i];
  }
  gs1->linedash.offset = gs2->linedash.offset;

  gs1->linecap    = gs2->linecap;
  gs1->linejoin   = gs2->linejoin;
  gs1->linewidth  = gs2->linewidth;
  gs1->miterlimit = gs2->miterlimit;

  pdf_color_copycolor(&gs1->fillcolor  , &gs2->fillcolor);
  pdf_color_copycolor(&gs1->strokecolor, &gs2->strokecolor);

  return;
}

int
pdf_dev_gsave (void)
{
  pdf_gstate *next, *curr;

  if (gs_stack.next >= PDF_GSAVE_MAX) {
    WARN("Too many gsaves.");
    return -1;
  }

  curr = CURRENT_GS();
  next = &(gs_stack.gstates[gs_stack.next]);
  gs_stack.next++;

  init_gstate(next);
  copy_gstate(next, curr);

  pdf_doc_add_page_content(" q", 2);

  return 0;
}

int
pdf_dev_current_depth (void)
{
  return gs_stack.next - 1; /* 0 means initial state */
}

void
pdf_dev_grestore_to (int depth)
{
  pdf_gstate  *gs;

  ASSERT(depth >= 0);

  if (gs_stack.next > depth + 1) {
    WARN("Closing pending transformations at end of page/XObject.");
  }

  while (gs_stack.next > depth + 1) {
    pdf_doc_add_page_content(" Q", 2);
    gs = CURRENT_GS();
    pd_cleanpath(&gs->path);
    gs_stack.next--;
  }
  pdf_dev_reset();

  return;
}

int
pdf_dev_grestore (void)
{
  pdf_gstate *gs;

  if (gs_stack.next <= 0) {
    WARN("Too many grestores.");
    return -1;
  }
  gs = CURRENT_GS();
  pd_cleanpath(&gs->path);
  gs_stack.next--;

  pdf_doc_add_page_content(" Q", 2);

  pdf_dev_reset_fonts();

  return 0;
}

int
pdf_dev_setmiterlimit (double mlimit)
{
  pdf_gstate *gs;
  int         len = 0;

  gs = CURRENT_GS();

  if (gs->miterlimit != mlimit) {
    fmt_buf[len++] = ' ';
    len += pdf_sprint_length(fmt_buf + len, mlimit);
    fmt_buf[len++] = ' ';
    fmt_buf[len++] = 'M';
    pdf_doc_add_page_content(fmt_buf, len);
    gs->miterlimit = mlimit;
  }

  return 0;
}

int
pdf_dev_setlinecap (int capstyle)
{
  pdf_gstate *gs;
  int         len;

  gs = CURRENT_GS();

  if (gs->linecap != capstyle) {
    len = sprintf(fmt_buf, " %d J", capstyle);
    pdf_doc_add_page_content(fmt_buf, len);
    gs->linecap = capstyle;
  }

  return 0;
}

int
pdf_dev_setlinejoin (int joinstyle)
{
  pdf_gstate *gs;
  int         len;

  gs = CURRENT_GS();

  if (gs->linejoin != joinstyle) {
    len = sprintf(fmt_buf, " %d j", joinstyle);
    pdf_doc_add_page_content(fmt_buf, len);
    gs->linejoin = joinstyle;
  }

  return 0;
}

int
pdf_dev_setlinewidth (double width)
{
  pdf_gstate *gs;
  int         len = 0;

  gs = CURRENT_GS();

  if (gs->linewidth != width) {
    fmt_buf[len++] = ' ';
    len += pdf_sprint_length(fmt_buf + len, width);
    fmt_buf[len++] = ' ';
    fmt_buf[len++] = 'w';
    pdf_doc_add_page_content(fmt_buf, len);
    gs->linewidth = width;
  }

  return 0;
}

int
pdf_dev_setdash (int count, double *pattern, double offset)
{
  pdf_gstate *gs;
  int   i, len = 0;

  gs = CURRENT_GS();

  gs->linedash.num_dash = count;
  gs->linedash.offset   = offset;
  pdf_doc_add_page_content(" [", 2);
  for (i = 0; i < count; i++) {
    fmt_buf[0] = ' ';
    len = pdf_sprint_length (fmt_buf + 1, pattern[i]);
    pdf_doc_add_page_content(fmt_buf, len + 1);
    gs->linedash.pattern[i] = pattern[i];
  }
  pdf_doc_add_page_content("] ", 2);
  len = pdf_sprint_length (fmt_buf, offset);
  pdf_doc_add_page_content(fmt_buf, len);
  pdf_doc_add_page_content(" d", 2);

  return 0;
}


/*
 *  x y width height re
 * 
 * is equivalent to
 *
 *  x y m
 *  (x + width) y l
 *  (x + width) (y + height) l
 *  x (y + height) l
 *  h
 */
static int
pd_is_rect (const pdf_path *p, int ignore_rule)
{
  struct path_element *pe0, *pe1, *pe2, *pe3, *pe4;

  if (p->num_paths == 5) {
    pe0 = &(p->path[0]);
    pe1 = &(p->path[1]);
    pe2 = &(p->path[2]);
    pe3 = &(p->path[3]);
    pe4 = &(p->path[4]);
    if (pe0->type == 'm' &&
	pe1->type == 'l' &&
	pe2->type == 'l' &&
	pe3->type == 'l' &&
	pe4->type == 'h') {
      if (pe1->p[0].y - pe0->p[0].y == 0 &&
	  pe2->p[0].x - pe1->p[0].x == 0 &&
	  pe3->p[0].y - pe2->p[0].y == 0) {
	if (pe1->p[0].x - pe0->p[0].x
	    == pe2->p[0].x - pe3->p[0].x) {
	  return 1;
	}
	/* Winding number is different but ignore it here. */
      } else if (ignore_rule &&
		 pe1->p[0].x - pe0->p[0].x == 0 &&
		 pe2->p[0].y - pe1->p[0].y == 0 &&
		 pe3->p[0].x - pe2->p[0].x == 0) {
	if (pe1->p[0].y - pe0->p[0].y
	    == pe2->p[0].y - pe3->p[0].y) {
	  return 1;
	}
      }
    }
  }

  return 0;
}

static int
pd_flushpath (pdf_path  *p, int ignore_rule)
{
  struct path_element *pe, *pe1;
  int    i, len = 0;

  if (pd_is_rect(p, ignore_rule)) {
    pdf_rect rect; /* FIXME */

    pe  = &(p->path[0]);
    pe1 = &(p->path[2]);

    rect.llx = pe->p[0].x;
    rect.lly = pe->p[0].y;
    rect.urx = pe1->p[0].x - pe->p[0].x; /* width...  */
    rect.ury = pe1->p[0].y - pe->p[0].y; /* height... */

    fmt_buf[len++] = ' ';
    len += pdf_sprint_rect(fmt_buf + len, &rect);
    fmt_buf[len++] = ' ';
    fmt_buf[len++] = 'r';
    fmt_buf[len++] = 'e';
    pdf_doc_add_page_content(fmt_buf, len);

    return 0;
  }

  for (i = 0; i < p->num_paths; i++) {

    pe  = &(p->path[i]);
    len = 0;

    switch (pe->type) {
    case 'm': case 'l':
      fmt_buf[len++] = ' ';
      len += pdf_sprint_coord(fmt_buf + len, &pe->p[0]);
      fmt_buf[len++] = ' ';
      fmt_buf[len++] = pe->type;
      break;
    case 'v':	/* first point replicated */
    case 'y':	/* last  point replicated */
      fmt_buf[len++] = ' ';
      len += pdf_sprint_coord(fmt_buf + len, &pe->p[0]);
      fmt_buf[len++] = ' ';
      len += pdf_sprint_coord(fmt_buf + len, &pe->p[1]);
      fmt_buf[len++] = ' ';
      fmt_buf[len++] = pe->type;
      break;
    case 'c':
      fmt_buf[len++] = ' ';
      len += pdf_sprint_coord(fmt_buf + len, &pe->p[0]);
      fmt_buf[len++] = ' ';
      len += pdf_sprint_coord(fmt_buf + len, &pe->p[1]);
      fmt_buf[len++] = ' ';
      len += pdf_sprint_coord(fmt_buf + len, &pe->p[2]);
      fmt_buf[len++] = ' ';
      fmt_buf[len++] = pe->type;
      break;
    case 'h': /* Closepath */
      fmt_buf[len++] = ' ';
      fmt_buf[len++] = 'h';
      break;
    default:
      ERROR("Unknown path type: %d", pe->type);
      break;
    }
    pdf_doc_add_page_content(fmt_buf, len);
  }

  return 0;
}

int
pdf_dev_clip (void)
{
  pdf_path *p;

  p = CURRENT_PATH();

  if (p->num_paths < 1) {
    return -1;
  }

  graphics_mode();
  pd_flushpath(p, 0);
  pdf_doc_add_page_content(" W", 2);

  return 0;
}

int
pdf_dev_eoclip (void)
{
  pdf_path *p;

  p = CURRENT_PATH();

  if (p->num_paths < 1) {
    return -1;
  }

  graphics_mode();
  pd_flushpath(p, 0);
  pdf_doc_add_page_content(" W*", 3);

  return 0;
}

int
pdf_dev_flushpath (char p_op, int fill_rule)
{
  pdf_gstate *gs;
  pdf_path   *p;
  char        opstr[16];
  int         len = 0;

  p  = CURRENT_PATH();
  if (p->num_paths < 1) {
    WARN("No currentpath.");
    return -1;
  }
  graphics_mode();

  opstr[len++] = ' ';

  switch (p_op) {
  case 'f': case 'F': /* fill */
    opstr[len++] = 'f';
    if (fill_rule == PDF_FILL_RULE_EVENODD)
      opstr[len++] = '*';
    break;
  case 's': /* closepath stroke */
    /* We do not update currentpoint since
     * currentpoint is undefined after painting.
     */
    opstr[len++] = 's';
    break;
  case 'S': /* stroke */
    opstr[len++] = 'S';
    break;
  case 'b': /* closepath fill stroke */
    opstr[len++] = 'b';
    if (fill_rule == PDF_FILL_RULE_EVENODD)
      opstr[len++] = '*';
    break;
  case 'B': /* fill stroke */
    opstr[len++] = 'B';
    if (fill_rule == PDF_FILL_RULE_EVENODD)
      opstr[len++] = '*';
    break;
  default:
    ERROR("Invalid path painting operator: %c", p_op);
    return -1;
    break;
  }
  pd_flushpath(p, 1);
  pdf_doc_add_page_content(opstr, len);

  pd_clearpath(p);

  gs = CURRENT_GS();
  gs->flags &= ~GS_FLAG_CURRENTPOINT_SET;

  return 0;
}

int
pdf_dev_lineto (double x, double y)
{
  struct path_element *pe;
  pdf_coord *cp;

  cp = CURRENT_POINT();

  pe = pd_nextpe();
  pe->type   = 'l';
  pe->p[0].x = cp->x = x;
  pe->p[0].y = cp->y = y;

  return 0;
}

int
pdf_dev_rlineto (double x, double y)
{
  pdf_coord *cp;

  cp = CURRENT_POINT();

  return pdf_dev_lineto(x + cp->x, y + cp->y);
}

int
pdf_dev_curveto (double x0, double y0,
		 double x1, double y1,
		 double x2, double y2)
{
  struct path_element *pe;
  pdf_coord *cp;

  cp = CURRENT_POINT();

  pe = pd_nextpe();
  if (x0 == cp->x && y0 == cp->y) {
    pe->type = 'v';
    pe->p[0].x = x1;
    pe->p[0].y = y1;
    pe->p[1].x = cp->x = x2;
    pe->p[1].y = cp->y = y2;
  } else if (x1 == x2 && y1 == y2) {
    pe->type = 'y';
    pe->p[0].x = x0;
    pe->p[0].y = y0;
    pe->p[1].x = cp->x = x1;
    pe->p[1].y = cp->y = y1;
  } else {
    pe->type = 'c';
    pe->p[0].x = x0;
    pe->p[0].y = y0;
    pe->p[1].x = x1;
    pe->p[1].y = y1;
    pe->p[2].x = cp->x = x2;
    pe->p[2].y = cp->y = y2;
  }

  return 0;
}

int
pdf_dev_rcurveto (double x0, double y0,
		  double x1, double y1,
		  double x2, double y2)
{
  pdf_coord  *cp;

  cp = CURRENT_POINT();

  return pdf_dev_curveto(x0 + cp->x, y0 + cp->y,
			 x1 + cp->x, y1 + cp->y,
			 x2 + cp->x, y2 + cp->y);
}

int
pdf_dev_closepath (void)
{			  
  pdf_path  *p;
  pdf_coord *cp;
  int        i;
  struct path_element *pe;

  p  = CURRENT_PATH();
  cp = CURRENT_POINT();

  pe = NULL;
  for (i = p->num_paths - 1; i >= 0; i--) {
    pe = &p->path[i];
    if (pe->type == 'm')
      break;
  }
  if (!pe || i < 0) {
    WARN("Invalid path...");
    return -1;
  }
  if (pe->p[0].x != cp->x ||
      pe->p[0].y != cp->y) {
    pe = pd_nextpe();
    pe->type = 'h';
    cp->x    = pe->p[0].x;
    cp->y    = pe->p[0].y;
  }

  return 0;
}

/* Modify M itself */
void
pdf_invertmatrix (pdf_tmatrix *M)
{
  pdf_tmatrix M_i;  
  double det;

  ASSERT(M);

  det = (M->a) * (M->d) - (M->b) * (M->c);
  if (det == 0.0) {
    ERROR("Determinant exactly zero in pdf_invertmatrix()");
    return;
  }

  M_i.a =  (M->d) / det; M_i.b = -(M->b) / det;
  M_i.c = -(M->c) / det; M_i.d =  (M->a) / det;
  M_i.e = ((M->b) * (M->f) - (M->d) * (M->e)) / det;
  M_i.f = ((M->e) * (M->c) - (M->a) * (M->f)) / det;

  pdf_copymatrix(M, &M_i);

  return;
}

void
pdf_dev_dtransform (pdf_coord *p, const pdf_tmatrix *M)
{
  pdf_tmatrix *ctm;
  double       x, y;

  ASSERT(p);

  x = p->x; y = p->y;
  if (M) {
    p->x = x * M->a + y * M->c;
    p->y = x * M->b + y * M->d;
  } else {
    ctm = CURRENT_MATRIX();
    p->x = x * ctm->a + y * ctm->c;
    p->y = x * ctm->b + y * ctm->d;
  }
}

void
pdf_dev_idtransform (pdf_coord *p, const pdf_tmatrix *M)
{
  pdf_tmatrix M_i;

  ASSERT(p);

  if (M)
    pdf_copymatrix(&M_i, M);
  else {
    pdf_copymatrix(&M_i, CURRENT_MATRIX());
  }
  pdf_invertmatrix  (&M_i);
  pdf_dev_dtransform(p, &M_i);
}

void
pdf_dev_transform (pdf_coord *p, const pdf_tmatrix *M)
{
  pdf_tmatrix *ctm;
  double       x, y;

  ASSERT(p);

  x = p->x; y = p->y;
  if (M) {
    p->x = x * M->a + y * M->c + M->e;
    p->y = x * M->b + y * M->d + M->f;
  } else {
    ctm = CURRENT_MATRIX();
    p->x = x * ctm->a + y * ctm->c + ctm->e;
    p->y = x * ctm->b + y * ctm->d + ctm->f;
  }
}

void
pdf_dev_itransform (pdf_coord *p, const pdf_tmatrix *M)
{
  pdf_tmatrix M_i;

  ASSERT(p);

  if (M)
    pdf_copymatrix(&M_i, M);
  else {
    pdf_copymatrix(&M_i, CURRENT_MATRIX());
  }
  pdf_invertmatrix (&M_i);
  pdf_dev_transform(p, &M_i);
}

static int
append_arc (double c_x , double c_y,
	    double r_x , double r_y,
	    double ang0, double ang1, int dir)
{
  pdf_tmatrix M;
  pdf_path   *p;
  pdf_coord   p0, p1, p2, *cp;
  int         i, n_steps;
  double      delta, v[6];

  cp = CURRENT_POINT();

  if (dir >= 0) {
    while (ang1 < ang0)
      ang0 -= 360;
  } else {
    while (ang1 > ang0)
      ang1 -= 360;
  }

  ang0 *= 3.1415926/180;
  ang1 *= 3.1415926/180;

#define MAX_ANG_STEP 1.0
  if (dir >= 0) {
    n_steps = (int) ((ang1 - ang0)/MAX_ANG_STEP) + 1;
  } else {
    n_steps = (int) ((ang0 - ang1)/MAX_ANG_STEP) + 1;
  }
  delta = (ang1 - ang0) / n_steps;

  M.a =  cos(delta); M.b = sin(delta);
  M.c = -sin(delta); M.d = cos(delta);
  M.e = 0.0;
  M.f = 0.0;

  p0.x = cos(ang0); p0.y = sin(ang0);

  p1.x = p0.x - delta / 3.0 * p0.y;
  p1.y = p0.y + delta / 3.0 * p0.x;
  p2.x = p0.x + delta / 3.0 * p0.y;
  p2.y = p0.y - delta / 3.0 * p0.x;

  v[0] = c_x + r_x * p0.x;
  v[1] = c_y + r_y * p0.y;
  p = CURRENT_PATH();
  if (p->num_paths == 0) {
    pdf_dev_moveto(v[0], v[1]);
  } else if (cp->x != v[0] || cp->y != v[1]) {
    pdf_dev_lineto(v[0], v[1]);
  }

  pdf_dev_transform(&p2, &M);
  pdf_dev_transform(&p0, &M);
  for (i = 0; i < n_steps; i++) {
    v[0] = c_x + r_x * p1.x;
    v[1] = c_y + r_y * p1.y;
    v[2] = c_x + r_x * p2.x;
    v[3] = c_y + r_y * p2.y;
    v[4] = c_x + r_x * p0.x;
    v[5] = c_y + r_y * p0.y;
    pdf_dev_curveto(v[0], v[1], v[2], v[3], v[4], v[5]);
    pdf_dev_transform(&p0, &M);
    pdf_dev_transform(&p1, &M);
    pdf_dev_transform(&p2, &M);
  }

  return 0;
}

int
pdf_dev_arc (double c_x , double c_y,
	     double r_x , double r_y,
	     double ang0, double ang1)
{
  return append_arc(c_x, c_y, r_x, r_y, ang0, ang1,  1);
}

int
pdf_dev_arcn (double c_x , double c_y,
	      double r_x , double r_y,
	      double ang0, double ang1)
{
  return append_arc(c_x, c_y, r_x, r_y, ang0, ang1, -1);
}

/* Required by Tpic */
int
pdf_dev_bspline (double x0, double y0,
		 double x1, double y1, double x2, double y2)
{
  double  v[6];

  v[0] = x0 + 2.0 * (x1 - x0) / 3.0;
  v[1] = y0 + 2.0 * (y1 - y0) / 3.0;
  v[2] = x1 + (x2 - x1) / 3.0;
  v[3] = y1 + (y2 - y1) / 3.0;
  v[4] = x2;
  v[5] = y2;

  return pdf_dev_curveto(v[0], v[1], v[2], v[3], v[4], v[5]);
}

