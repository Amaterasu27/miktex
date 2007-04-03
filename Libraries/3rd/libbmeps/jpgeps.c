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


#line 42 "jpgeps.ctr"


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

static char error_failed_to_read_jpg[] =
{ "ERROR: Failed to read JPEG file!" };

static char *error_failed_to_use_input_file[] = {
  error_failed_to_read_jpg,
  "The input file can not be used as data source for JPEG decompression.",
  NULL
};

static char *error_failed_read_jpeg_header[] = {
  error_failed_to_read_jpg,
  "Failed to read JPEG header from file.",
  NULL
};

static char *error_failed_to_start_decompression[] = {
  error_failed_to_read_jpg,
  "Failed to start JPEG decompression.",
  NULL
};

static char *error_not_enough_memory[] = {
  error_failed_to_read_jpg,
  "Failed to allocate memory for image data.",
  NULL
};

static char *error_while_reading_jpeg[] = {
  error_failed_to_read_jpg,
  "Error while reading JPEG data.",
  NULL
};

static char *error_failed_to_read_jpeg[] = {
  error_failed_to_read_jpg,
  "The JPEG library routines failed to read the data from file.",
  "The file is possibly corrupted or not a JPEG file.",
  NULL
};

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
	  } else {
	    if(cinfo.out_color_space == JCS_GRAYSCALE) {
	      what_to_do = 3;
	    }
	  }
	  for(y = 0UL; y < h; y++) { 
	    if(had_error == 0) {
	      jpeg_read_scanlines(&cinfo, &row, 1);
	    }
	    
	    if(had_error == 0) {
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
		  case 3: {	/* pure gray */
		    bmeps_add_gray(get_value(row[x]));
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
	    } else {
	      bmeps_show_error_text(out, error_while_reading_jpeg);
	    }
	  }
	  back = 1;
#if HAVE_SETJMP_H
	  } /* if(setjmp(errjump[3]) == 0) */ else { 
	    bmeps_show_error_text(out, error_failed_to_read_jpeg);
	  }
#endif
	  state = 3;
	  bmeps_end_image(out);
	  free(row); 
	  jpeg_finish_decompress(&cinfo);
	} else { 
	  bmeps_show_error_text(out, error_not_enough_memory);
	  jpeg_abort((struct jpeg_common_struct *)(&cinfo));
	}
      }
#if HAVE_SETJMP_H
      } /* if(setjmp(errjump[2]) == 0) */ else { 
        bmeps_show_error_text(out, error_failed_to_read_jpeg);
      }
#endif
      state = 2;
      bmeps_footer(out);
      } /* if(had_error == 0) */
    } break;
  }
#if HAVE_SETJMP_H
  } /* if(setjmp(errjump[1]) == 0) */ else { 
    bmeps_show_error_text(out, error_failed_to_read_jpeg);
    jpeg_abort((struct jpeg_common_struct *)(&cinfo));
  }
#endif
  state = 1;
  } /* if(had_error == 0) */ else {
    bmeps_show_error_text(out, error_failed_to_start_decompression);
  }
  } /* if(had_error == 0) */ else {
    bmeps_show_error_text(out, error_failed_read_jpeg_header);
  }
  } /* if(had_error == 0) */ else {
    bmeps_show_error_text(out, error_failed_to_use_input_file);
  }
#if HAVE_SETJMP_H
  } else { 
    bmeps_show_error_text(out, error_failed_to_read_jpeg);
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
  "@(#)08/25/06\t1.15\tjpgeps.ctr\t(krause) JPEG to EPS conversion module"
};
#endif

