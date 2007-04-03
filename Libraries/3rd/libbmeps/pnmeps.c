/*
Copyright (c) 2000-2005, Dirk Krause
All rights reserved.

Redistribution and use in source and binary forms,
with or without modification, are permitted provided
that the following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.
* Redistributions in binary form must reproduce the above 
  opyright notice, this list of conditions and the following
  disclaimer in the documentation and/or other materials
  provided with the distribution.
* Neither the name of the Dirk Krause nor the names of
  its contributors may be used to endorse or promote
  products derived from this software without specific
  prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/

#include "bmeps.h"
#include "bmepsoe.h"
#include "bmepsco.h"

#if HAVE_PNM_H
#include <pnm.h>
#endif


#line 46 "pnmeps.ctr"


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

static char error_in_pnm[] =
{ "ERROR: Failed to read PNM file!" };

static char *error_not_a_pnm_file[] = {
  error_in_pnm,
  "The file is possibly corrupted or not a NetPBM file.",
  NULL
};

int bmeps_pnm_wh(FILE *in, unsigned long *w, unsigned long *h)
{
  int back = 0;
#if HAVE_PNM_H
  int cols = 0, rows = 0, format = 0;
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
  unsigned long w = 0UL, h = 0UL;
  
  if(out && in) {
    if(bmeps_pnm_wh(in, &w, &h)) {
      if(w && h) {
        back = 1;
        bmeps_bb(out, w, h);
      } else {
        bmeps_show_error_text(out, error_not_a_pnm_file);
      }
    } else {
      bmeps_show_error_text(out, error_not_a_pnm_file);
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
        if(w && h) {
	  back = 1;
	  bmeps_header(out, (name ? name : default_name), w, h);
	  bmeps_draft(out, w, h);
	  bmeps_footer(out);
	} else {
	  bmeps_show_error_text(out, error_not_a_pnm_file);
	}
      } else {
        bmeps_show_error_text(out, error_not_a_pnm_file);
      }
    } else {		    /* real picture */
      
      array = pnm_readpnm(in,&cols,&rows,&maxxelval,&format);
      
      w = cols; h = rows;
      if((array) && (w) && (h)) {
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
	/* + 2005/03/21, set exit code on success */
	back = 1;
	/* - 2005/03/21 */
      } else {
        bmeps_show_error_text(out, error_not_a_pnm_file);
	
      }
    }
  } 
#endif
  return back;
}


#ifndef LINT
static char sccsId[] = 
{ "@(#)pnmeps.ctr 1.15 07/18/06 (krause)\tPNM to EPS conversion" };
#endif

