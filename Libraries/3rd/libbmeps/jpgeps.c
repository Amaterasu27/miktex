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


#line 27 "jpgeps.ctr"


#if HAVE_JPEGLIB_H
#include <jpeglib.h>
#endif

#if HAVE_SETJMP_H
#include <setjmp.h>
#endif

#if BITS_IN_JSAMPLE == 12

static int get_value(int i)
{
  int back;
  back = (i >> 4) & 0xFF;
  return back;
}

#else

#if BITS_IN_JSAMPLE > 8

#define get_value(i) (((i) >> (BITS_IN_JSAMPLE-8)) & 0xFF)

#else

#define get_value(i) (((int)i) & 0xFF)

#endif

#endif

#if HAVE_JPEGLIB_H

static int had_error = 0;
static int state     = 0;

#if HAVE_SETJMP_H
static jmp_buf errjump[4];
#endif
/* if HAVE_SETJMP_H */

static void error_exit_replacement(j_common_ptr cinfo)
{
  
  had_error = 1;
  if(state) {
#if HAVE_SETJMP_H
    
    longjmp(errjump[(state-1)],1);
#endif
  }
  
}

#endif
/* if HAVE_JPEGLIB_H */

static int
jpg_run(FILE *out, FILE *in, char *name, unsigned long *w, unsigned long *h, int cmd)
{
  int back = 0;
#if HAVE_JPEGLIB_H
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

  had_error = state = 0; 
  cinfo.err = jpeg_std_error(&jerr);
  jerr.error_exit = error_exit_replacement;
  jpeg_create_decompress(&cinfo);
  state = 1;
#if HAVE_SETJMP_H
  if(setjmp(errjump[0]) == 0) {
#endif
  jpeg_stdio_src(&cinfo, in);
  if(had_error == 0) {
  jpeg_read_header(&cinfo, TRUE);
  if(had_error == 0) {
  jpeg_start_decompress(&cinfo);
  if(had_error == 0) {
#if HAVE_SETJMP_H
  if(setjmp(errjump[1]) == 0) {
#endif
  state = 2;
  switch(cmd) {
    case 0: {
      back = 1;
      *w = ((unsigned long)(cinfo.output_width));
      *h = ((unsigned long)(cinfo.output_height));
      jpeg_abort((struct jpeg_common_struct *)(&cinfo));
    } break;
    case 1: {
      unsigned long w, h;
      w = cinfo.output_width;
      h = cinfo.output_height;
      back = 1;
      bmeps_bb(out, w, h);
      jpeg_abort((struct jpeg_common_struct *)(&cinfo));
    } break;
    case 2: {
      unsigned long w, h;
      w = cinfo.output_width;
      h = cinfo.output_height;
      if(had_error == 0) {
      bmeps_header(out, name, w, h);
#if HAVE_SETJMP_H
      if(setjmp(errjump[2]) == 0) {
#endif
      state = 3;
      if(bmeps_get_draft()) { 
	bmeps_draft(out, w, h);
	back = 1;
	jpeg_abort((struct jpeg_common_struct *)(&cinfo));
      } else {
	size_t sz;
	JSAMPLE *row; 
	sz = cinfo.output_width * cinfo.output_components * sizeof(JSAMPLE);
	row = malloc(sz); 
	if(row) {
	  JSAMPROW color_map0;
	  JSAMPROW color_map1;
	  JSAMPROW color_map2;
	  int      what_to_do, index;
	  unsigned long x, y; 
	  bmeps_begin_image(out, w, h);
#if HAVE_SETJMP_H
	  if(setjmp(errjump[3]) == 0) {
#endif
	  state = 4;
	  color_map0 = color_map1 = color_map2 = NULL;
	  what_to_do = 0;
	  if(cinfo.quantize_colors) {
	    what_to_do = 1;
	    color_map0 = (cinfo.colormap)[0];
	    if(cinfo.out_color_space == JCS_GRAYSCALE) {
	      what_to_do = 2;
	    } else {
	      color_map1 = (cinfo.colormap)[1];
	      color_map2 = (cinfo.colormap)[2];
	    }
	  }
	  for(y = 0UL; y < h; y++) { 
	    if(had_error == 0) {
	      jpeg_read_scanlines(&cinfo, &row, 1);
	    } 
            for(x = 0UL; x < cinfo.output_width; x++) {
	      switch(what_to_do) {
		case 1: {	/* color mapped */
		  index = row[x] & 0xFF;
		  bmeps_add_rgb(
		    get_value(color_map0[index]),
		    get_value(color_map1[index]),
		    get_value(color_map2[index])
		  );
		} break;
		case 2: {	/* gray mapped */
		  index = row[x] & 0xFF;
		  bmeps_add_gray(
		    get_value(color_map0[index])
		  );
		} break;
		default : {	/* normal */
		  bmeps_add_rgb(
		    get_value(row[3UL*x]),
		    get_value(row[3UL*x+1UL]),
		    get_value(row[3UL*x+2UL])
		  );
		} break;
	      }
	    }
	  }
	  back = 1;
#if HAVE_SETJMP_H
	  } /* if(setjmp(errjump[3]) == 0) */ else { 
	  }
#endif
	  state = 3;
	  bmeps_end_image(out);
	  free(row); 
	  jpeg_finish_decompress(&cinfo);
	} else { 
	  jpeg_abort((struct jpeg_common_struct *)(&cinfo));
	}
      }
#if HAVE_SETJMP_H
      } /* if(setjmp(errjump[2]) == 0) */ else { 
      }
#endif
      state = 2;
      bmeps_footer(out);
      } /* if(had_error == 0) */
    } break;
  }
#if HAVE_SETJMP_H
  } /* if(setjmp(errjump[1]) == 0) */ else { 
    jpeg_abort((struct jpeg_common_struct *)(&cinfo));
  }
#endif
  state = 1;
  } /* if(had_error == 0) */
  } /* if(had_error == 0) */
  } /* if(had_error == 0) */
#if HAVE_SETJMP_H
  } else { 
  }
#endif
  state = 0;
  jpeg_destroy_decompress(&cinfo);
  if(had_error) { back = 0; }
  had_error = state = 0;
  
#endif
  return back;
}

int bmeps_jpg_wh(FILE *in, unsigned long *w, unsigned long *h)
{
  int back = 0;
  if(w && h) {
    back = jpg_run(NULL, in, NULL, w, h, 0);
  }
  return back;
}

int bmeps_jpg_bb(FILE *out, FILE *in, char *name)
{
  int back = 0;
  if(out) {
  if(in) {
    bmeps_configure();
    back = jpg_run(out, in, name, NULL, NULL, 1);
  }
  }
  return back;
}

int bmeps_jpg(FILE *out, FILE *in, char *name)
{
  int back = 0;
  if(out) {
  if(in) {
    bmeps_configure();
    back = jpg_run(out, in, name, NULL, NULL, 2);
  }
  }
  return back;
}


#ifndef LINT
static char sccs_id[] = {
  "@(#)08/17/04\t1.11\tjpgeps.ctr\t(krause) JPEG to EPS conversion module"
};
#endif

