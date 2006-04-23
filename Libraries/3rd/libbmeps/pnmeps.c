/* 
 * libbmeps - Bitmap to EPS conversion library
 * Copyright (C) 2000 - Dirk Krause
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * In this package the copy of the GNU Library General Public License
 * is placed in file COPYING.
 */

#include "bmeps.h"
#include "bmepsoe.h"
#include "bmepsco.h"

#if HAVE_PNM_H
#include <pnm.h>
#endif

static int is_configured = 0;

static char default_name[] = { "noname.pnm" } ;
static char argv_name[] = { "bmeps" } ;
static char *argv_for_init[2];
static int  pnm_argc;

static int normalize(int val, int oldmax, int newmax)
{
  int back;
  long l1, l2, l3, l4;
  back = val;
  if(oldmax != newmax) {
    l1 = val; l2 = newmax; l3 = oldmax;
    l4 = (l1 * l2) / l3;
    back = (int)l4;
  }
  return back;
}

static void do_configure(void)
{
  if(!is_configured) {
    is_configured = 1;
    argv_for_init[0] = argv_name;
    argv_for_init[1] = NULL;
    pnm_argc         = 1;
#if HAVE_PNM_H
    pnm_init(&pnm_argc, argv_for_init);
#endif
  }
}

int bmeps_pnm_wh(FILE *in, unsigned long *w, unsigned long *h)
{
  int back = 0;
#if HAVE_PNM_H
  int cols, rows, format;
  xelval maxval;

  bmeps_configure();
  if(in && w && h) {
    do_configure();
    cols = rows = format = 0;
    pnm_readpnminit(in, &cols, &rows, &maxval, &format);
    if(cols && rows) {
      *w = cols; *h = rows;
      back = 1;
    }
  }
#endif
  return back;
}

int bmeps_pnm_bb(FILE *out, FILE *in, char *name)
{
  int back = 0;
#if HAVE_PNM_H
  unsigned long w, h;
  if(out && in) {
    if(bmeps_pnm_wh(in, &w, &h)) {
      back = 1;
      bmeps_bb(out, w, h);
    }
  }
#endif
  return back;
}

int bmeps_pnm(FILE *out, FILE *in, char *name)
{
  int back = 0;
#if HAVE_PNM_H
  int rows, cols, format, maxval, x, y;
  int r, g, b;
  unsigned long w, h;
  xelval maxxelval, xval;
  xel    xe, *xelptr, **array, **rowptr;

  if(out && in && name) {
    bmeps_configure();
    do_configure();
    if(bmeps_get_draft()) { /* draft placeholder only */
      if(bmeps_pnm_wh(in, &w, &h)) {
	back = 1;
	bmeps_header(out, (name ? name : default_name), w, h);
	bmeps_draft(out, w, h);
	bmeps_footer(out);
      }
    } else {		    /* real picture */
      array = pnm_readpnm(in,&cols,&rows,&maxxelval,&format);
      w = cols; h = rows;
      if(array) {
	maxval = (int)maxxelval;
	switch( PNM_FORMAT_TYPE(format) ) {
	  case PPM_TYPE : {	/* possibly colored */
	    bmeps_header(out, (name ? name : default_name), w, h);
	    bmeps_begin_image(out, w, h);
	    rowptr = array;
	    for(y = 0; y < rows; y++) {
	      xelptr = *rowptr;
	      for(x = 0; x < cols; x++) {
		xe = *xelptr;
		xval = PPM_GETR(xe);
		r = (int)xval;
		xval = PPM_GETG(xe);
		g = (int)xval;
		xval = PPM_GETB(xe);
		b = (int)xval;
		r = normalize(r, maxval, 255);
		g = normalize(g, maxval, 255);
		b = normalize(b, maxval, 255);
		bmeps_add_rgb(r,g,b);
		xelptr++;
	      }
	      rowptr++;
	    }
	    bmeps_end_image(out);
	    bmeps_footer(out);
	  } break;
	  default : {		/* no colors */
	    BmepsConfiguration oldcfg, newcfg;
	    bmeps_get_cfg(&oldcfg);
	    bmeps_copy_cfg(&newcfg, &oldcfg);
	    newcfg.col = 0;
	    bmeps_set_cfg(&newcfg);
	    bmeps_header(out, (name ? name : default_name), w, h);
	    bmeps_begin_image(out, w, h);
	     rowptr = array;
	     for(y = 0; y < rows; y++) {
	       xelptr = *rowptr;
	       for(x = 0; x < cols; x++) {
		 xe = *xelptr;
		 xval = PNM_GET1(xe);
		 r = (int)xval;
		 r = normalize(r, maxval, 255);
		 bmeps_add_gray(r);
		 xelptr++;
	       }
	       rowptr++;
	     }
	    bmeps_end_image(out);
	    bmeps_footer(out);
	    bmeps_set_cfg(&oldcfg);
	  } break;
	}
	pnm_freearray(array, rows);
      }
    }
  }
#endif
  return back;
}


#ifndef LINT
static char sccsId[] = 
{ "@(#)pnmeps.ctr 1.10 08/17/04 (krause)\tPNM to EPS conversion" };
#endif

