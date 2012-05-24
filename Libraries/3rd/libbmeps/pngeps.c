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

#include "bmepsco.h"
#include "pngeps.h"
#include "bmeps.h"
#include "bmepsoe.h"


#line 43 "pngeps.ctr"


#if HAVE_ZLIB
#if HAVE_LIBPNG

#include <png.h>
#if defined(MIKTEX)
#  include <pnginfo.h>
#  include <pngstruct.h>
#endif
#include <math.h>

#if HAVE_SETJMP_H
#include <setjmp.h>
#endif


#if SIZEOF_LONG == 8
/* MSB for unsigned long in hex notation */
#define MSBULI 0x8000000000000000UL
/* MSB for unsigned long in double notation */
#define MSBULD 9223372036854775808.0
/* 0xFFFFFFFFFFFFFFFFUL in double notation */
#define MAXULD 18446744073709551615.0
/* MSB for long in hex notation */
#define MSBLI  0x4000000000000000L
/* MSB for long in double notation */
#define MSBLD  4611686018427387904.0
/* 0x7FFFFFFFFFFFFFFFL in double notation */
#define MAXLD  9223372036854775807.0
#define MAXLI  0x7FFFFFFFFFFFFFFFL
/* number of bits in unsigned long */
#define BITSUL 64
/* number of bits in long */
#define BITSL  63
/* negativest long number */
#if OLD_VERSION_NEGLI
#define NEGLI  (-9223372036854775808L)
#else
#define NEGLI  0x8000000000000000L
#endif
#define NEGLD  (-9223372036854775808.0)
#else
#define MSBULI 0x80000000UL
#define MSBULD 2147483648.0
#define MAXULD 4294967295.0
#define MSBLI  0x40000000L
#define MSBLD  1073741824.0
#define MAXLD  2147483647.0
#define MAXLI  0x7FFFFFFFL
#define BITSUL 32
#define BITSL  31
#if OLD_VERSION_NEGLI
#define NEGLI  (-2147483648L)
#else
#define NEGLI  0x80000000L
#endif
#define NEGLD  (-2147483648.0)
#endif



static char default_name[] = { "noname.png" };


/*
static int a2t(int x, int alpha, int trans, int aldual, int altrig)
{
  int back = 0;
  $? "+ a2t x=%d alpha=%d trans=%d dual=%d tr=%d", x, alpha, trans, aldual, altrig
  if(alpha) {
    if(!trans) {
      back = 255 - x;
    }
    if(aldual) {
      if(altrig) {
	if(back > 0) { back = 255; }
      } else {
	if(back < 255) { back = 0; }
      }
    }
  } $? "- a2t %d", back
  return back;
}
*/

static double
st_ul_to_double(unsigned long l)
{
  double back = 0.0;
  double testd; unsigned long testval; int trials;
  
  testd = MSBULD; testval = MSBULI; trials = BITSUL;
  while(trials--) {
    if(l & testval) { back += testd; }
    testd = testd / 2.0; testval = testval / 2UL;
  } 
  return back;
}

double x_rint(double d)
{
  double back = 0.0;
#if HAVE_RINT
  back = rint(d);
#else
  if(d >= 0.0) {
    back = floor(d + 0.5);
  } else {
    back = 0.0 - x_rint(0.0 - d);
  }
#endif
  
  return back;
}

static unsigned long
st_double_to_ul(double d)
{
  unsigned long back = 0UL;
  double xval, testd; unsigned long testval; int trials;
  
  if(d >= 0.0) {
    xval = x_rint(d); 
    if(xval > MAXULD) {
    } else {
      testd = MSBULD; testval = MSBULI; trials = BITSUL;
      while(trials--) {
	if(xval >= testd) {
	  back |= testval;
	  xval -= testd;
	}
	testd = testd / 2.0; testval = testval / 2UL;
      }
    }
  } else {
    back = st_double_to_ul(0.0 - d);
  } 
  return back;
}

static int new_bit_depth(int alt, int newbits, int altbits)
{
  int back = 0;
  unsigned long a, b, nb, ab;
  
  a = alt; nb = newbits; ab = altbits;
  b = (((2UL ^ nb)-1UL)*(a)) / ((2UL ^ ab) - 1UL);
  back = b;
  
  return back;
}

static int ntsc(int red, int green, int blue)
{
  int back = 0;
  unsigned long r, g, b, bck;
  
  r = red; g = green; b = blue;
  bck = ((54UL * r) + (183UL * g) + (19UL * b)) / 256UL;
  back = bck;
  
  return back;
}

static int mix_colors(int fg, int bg, int alpha, int tr)
{
  int back = 0;
  unsigned long f, b, a, bck;
  
  if(tr) { a = (255UL - alpha) ; }
  else   { a = alpha; }
  f = fg; b = bg;
  bck = ((a * f) + ((255UL - a) * b)) / 255UL;
  back = bck; 
  return back;
}


static char error_while_reading_png[] =
{ "ERROR: Failed to read PNG file!" };

static char error_file_possibly_damaged[] =
{ "The input file is possibly damaged or not a PNG file." };

static char *error_failed_to_create_read_struct[] = {
  error_while_reading_png,
  "The PNG library failed to create a PNG read structure.",
  error_file_possibly_damaged,
  NULL
};

static char *error_failed_to_create_info_struct[] = {
  error_while_reading_png,
  "The PNG library failed to create a PNG info structure.",
  error_file_possibly_damaged,
  NULL
};

static char *error_not_enough_memory[] = {
  error_while_reading_png,
  "The PNG library failed to allocate memory for the bitmap data.",
  NULL
};

static char *error_not_a_png_file[] = {
  error_while_reading_png,
  error_file_possibly_damaged,
  NULL
};

static int
png_run(FILE *out, FILE *in, char *name, unsigned long *w, unsigned long *h, int cmd)
{
  int back = 0;
  png_structp pp;
  png_infop   pi;
  unsigned long	wi;	/* width */
  unsigned long	he;	/* height */
  unsigned long bbwi;
  unsigned long bbhe;
  unsigned long x, y;
  int		bd;	/* bit depth */
  int		ct, cu;	/* color type */
  int		it;	/* interlace type */
  int		zt;	/* compression type */
  int		ft;	/* filter type */
  int		ch; 	/* channels */
  int		alpha;
  int		trans;
  int		altrig;
  int		rowbytes;
  int		mix;	/* mix foreground and background */
  int		specbg;	/* specified background from command line */
  int		bg_red, bg_green, bg_blue;
  int           usr;
  png_bytep	row, *rows = NULL, *rowp;
  png_color_16	bg;
  png_color_16p	bgp;
  png_uint_32	ppm, xppm, yppm;
  double xscale;
  double yscale;

  ppm = xppm = yppm = 0UL; usr = 0;
  xscale = yscale = 1.0;
  if(in) {
    alpha = bmeps_get_alpha();
    trans = bmeps_get_trans();
    altrig = bmeps_get_altrig();
    mix = bmeps_get_mix();
    specbg = bmeps_get_specbg();
    bg_red = bmeps_get_bg_red();
    bg_green = bmeps_get_bg_green();
    bg_blue = bmeps_get_bg_blue();
    usr = bmeps_get_usr();
    rewind(in);
    pp = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(pp) { 
      pi = png_create_info_struct(pp);
      if(pi) { 
#if HAVE_SETJMP_H
#  if defined(MIKTEX)
	if(setjmp(pp->longjmp_buffer) == 0) {
#  else
        if(setjmp(pp->jmpbuf) == 0) {
#  endif
#endif
	  png_init_io(pp, in); 
	  png_read_info(pp, pi); 
	  png_get_IHDR(pp, pi, &wi, &he, &bd, &ct, &it, &zt, &ft);
	  bbwi = wi; bbhe = he;
	  
	  
	  
	  
	  
	  
	  
	  cu = ct;
	  ch = png_get_channels(pp, pi);
	  
	  switch(cmd) {
	    case 0: {
	      if(out) {
	        ppm  = png_get_pixels_per_meter(pp, pi);
	        xppm = png_get_x_pixels_per_meter(pp, pi);
	        yppm = png_get_y_pixels_per_meter(pp, pi);
	        if(usr) {
	          if(!((xppm) && (yppm))) {
		    if(xppm) {
		      yppm = xppm;
		    } else {
		      if(yppm) {
		        xppm = yppm;
		      } else {
		        if(ppm) {
		          xppm = yppm = ppm;
		        } else {
		          usr = 0;
		        }
		      }
		    }
		  }
	        }
	        if(usr) {
	          double xwidth, yheight;
		  xscale  = (st_ul_to_double(xppm)*0.0127)/36.0;
		  yscale  = (st_ul_to_double(yppm)*0.0127)/36.0;
		  xwidth  = xscale * st_ul_to_double(wi);
		  yheight = yscale * st_ul_to_double(he);
		  xwidth  = ceil(xwidth);
		  yheight = ceil(yheight);
		  bbwi    = st_double_to_ul(xwidth);
		  bbhe    = st_double_to_ul(yheight);
		  xscale  = xwidth / st_ul_to_double(wi);
		  yscale  = yheight / st_ul_to_double(he);
	        }
	        bmeps_header(
		  out, (name ? name : default_name),
		  bbwi, bbhe
	        );
	        if(usr) {
	          fprintf(out, "gsave\n");
	          fprintf(out, "%% scaling (resolution chunk in PNG)\n");
	          fprintf(out, "%lg %lg scale\n", xscale, yscale);
		  bmeps_set_gsave_done();
	        }
	        if(bmeps_get_draft()) {
		  back = 1;
		  bmeps_draft(out, wi, he);
	        } else {
		  int need_expansion;
		  int need_strip;
		  int need_pack;
		  need_expansion = 0;
		  need_strip = 0;
		  need_pack  = 0;
		  if((ct == PNG_COLOR_TYPE_PALETTE) && (bd <= 8)) {
		    need_expansion = 1;
		  }
		  if((ct == PNG_COLOR_TYPE_GRAY) && (bd < 8)) {
		    need_expansion = 1;
		  }
		  if(png_get_valid(pp, pi, PNG_INFO_tRNS)) {
		    need_expansion = 1;
		  }
		  if(bd > 8) {
		    need_strip = 1;
		  } else {
		    if(bd < 8) {
		      need_pack = 1;
		    }
		  }
		  if(need_expansion) {
		    png_set_expand(pp);
		  }
		  if(need_strip) {
		    png_set_strip_16(pp);
		  }
		  if(need_pack) {
		    png_set_packing(pp);
		  }
		  bgp = &bg;
		  bg.red = bg_red;
		  bg.green = bg_green;
		  bg.blue = bg_blue;
		  bg.gray = ntsc(bg.red, bg.green, bg.blue);
		  bg.index = 0;
		  
		  if(!(alpha)) {
		    
		    if(png_get_bKGD(pp, pi, &bgp)) {
		      png_set_background(pp,bgp,PNG_BACKGROUND_GAMMA_FILE,1,1.0);
		    } else {
		      png_set_background(pp,&bg,PNG_BACKGROUND_GAMMA_SCREEN,0,1.0);
		    }
		  } else { 
		    if(png_get_bKGD(pp, pi, &bgp) && (!specbg)) {
		      
		      if(ct & PNG_COLOR_MASK_PALETTE) {
		        png_colorp ptr; int num; 
		        if(png_get_PLTE(pp, pi, &ptr, &num)) {
			  
		          if(bgp->index < num) { 
			    if(bd != 8) { 
			      bg.red = new_bit_depth(ptr[bgp->index].red, 8, bd);
			      bg.green = new_bit_depth(
			        ptr[bgp->index].green, 8, bd
			      );
			      bg.blue = new_bit_depth(
			        ptr[bgp->index].blue, 8, bd
			      );
			      bg.gray = ntsc(
			        bg.red, bg.green, bg.blue
			      );
			    } else { 
			      bg.red = ptr[bgp->index].red;
			      bg.green = ptr[bgp->index].green;
			      bg.blue = ptr[bgp->index].blue;
			      bg.gray = ntsc(bg.red, bg.green, bg.blue);
			    }
		          } else { 
		            bg.red = bg_red;
		            bg.green = bg_green;
		            bg.blue = bg_blue;
		            bg.gray = ntsc(bg.red, bg.green, bg.blue);
			    bg.index = 0;
		          }
		        } else { 
		          bg.red = bg_red;
		          bg.green = bg_green;
		          bg.blue = bg_blue;
		          bg.gray = ntsc(bg.red, bg.green, bg.blue);
		          bg.index = 0;
		        }
		      } else { 
		        if(bd != 8) { 
		          bg.red = new_bit_depth( bgp->red, 8, bd);
		          bg.blue = new_bit_depth( bgp->blue, 8, bd);
		          bg.green = new_bit_depth( bgp->green, 8, bd);
		          bg.gray = new_bit_depth( bgp->gray, 8, bd);
		        } else { 
		          bg.red = bgp->red;
		          bg.blue = bgp->blue;
		          bg.green = bgp->green;
		          bg.gray = bgp->gray;
		        }
		        bg.index = 0;
		      }
		    } else { 
		      bg.red = bg_red;
		      bg.green = bg_green;
		      bg.blue = bg_blue;
		      bg.gray = ntsc(bg.red, bg.green, bg.blue);
		      bg.index = 0;
		    }
		  }
		  /* place gamma correction here */
		  /* place interlace handling here */
		  png_read_update_info(pp, pi);
		  ch = png_get_channels(pp, pi);
		  ct = png_get_color_type(pp, pi);
		  
		  
		  rowbytes = png_get_rowbytes(pp, pi);
		  rows = (png_bytep *)malloc(he*sizeof(png_bytep));
		  if(rows) {
		    back = 1; rowp = rows;
		    for(y = 0; y < he; y++) {
		      *rowp = NULL;
		      row = (png_bytep)malloc(rowbytes*sizeof(png_byte));
		      if(row) {
		        *rowp = row;
		      } else {
		        back = 0;
		      }
		      rowp++;
		    }
		    if(back) {
		      png_read_image(pp, rows);
		      if((ct & PNG_COLOR_MASK_ALPHA) && alpha) {
		        
		        bmeps_set_trans(1);
		      } else { 
		      }
		      bmeps_begin_image(
		        out, wi, he
		      );
		      rowp = rows;
		      for(y = 0; y < he; y++) {
		        row = *(rowp++);
		        for(x = 0; x < wi; x++) {
			  if((ct & PNG_COLOR_MASK_ALPHA) && alpha) {
			    
			    bmeps_add_trans(
			      255-row[(x+1UL)*((unsigned long)ch)-1UL]
			    );
			  }
			  if(ct & PNG_COLOR_MASK_COLOR) {
			    
			    if((ct & PNG_COLOR_MASK_ALPHA) && alpha && mix) {
			      /* 4 lines for debugging only */
			      static int firstrun = 1;
			      if(firstrun) { 
			      }
			      firstrun = 0;
			      bmeps_add_rgb(
			        mix_colors(
				  row[x*((unsigned long)ch)],
				  bg.red,
				  row[(x+1UL)*((unsigned long)ch)-1UL],
				  trans
			        ),
			        mix_colors(
				  row[x*((unsigned long)ch)+1UL],
				  bg.green,
				  row[(x+1UL)*((unsigned long)ch)-1UL],
				  trans
  
			        ),
			        mix_colors(
				  row[x*((unsigned long)ch)+2UL],
				  bg.blue,
				  row[(x+1UL)*((unsigned long)ch)-1UL],
				  trans
			        )
			      );
			    } else {
			      /* 4 lines for debugging only */
			      static int firstrun = 1;
			      if(firstrun) { 
			      }
			      firstrun = 0;
			      bmeps_add_rgb(
			        row[x*((unsigned long)ch)],
			        row[x*((unsigned long)ch)+1UL],
			        row[x*((unsigned long)ch)+2UL]
			      );
			    }
			  } else {
			    if((ct & PNG_COLOR_MASK_ALPHA) && alpha && mix) {
			      /* 4 lines for debugging only */
			      static int firstrun = 1;
			      if(firstrun) { 
			      }
			      firstrun = 0;
			      bmeps_add_gray(
			        mix_colors(
				  row[x*((unsigned long)ch)],
				  bg.gray,
				  row[(x+1UL)*((unsigned long)ch)-1UL],
				  trans
			        )
			      );
			    } else {
			      /* 4 lines for debugging only */
			      static int firstrun = 1;
			      if(firstrun) { 
			      }
			      firstrun = 0;
			      
			      bmeps_add_gray(
			        row[x*((unsigned long)ch)]
			      );
			    }
			  }
		        }
		      }
		      bmeps_end_image(out);
		      
		    } else {
		      bmeps_show_error_text(out, error_not_enough_memory);
		    }
		    /* done with rows */
		    rowp = rows;
		    for(y = 0; y < he; y++) {
		      row = *rowp;
		      if(row) { free(row); }
		      *(rowp++) = NULL;
		    }
		    free(rows);
		  } else { 
		    bmeps_show_error_text(out, error_not_enough_memory);
		  }
	        }
	        bmeps_footer(out);
	      }
	    } break;
	    case 1: {
	      if(out) {
	        back = 1;
	        bmeps_bb(out, wi, he);
	      }
	    } break;
	    case 2: {
	      if(w && h) {
	        back = 1;
	        *w = wi;
	        *h = he;
	      }
	    } break;
	  }
	  /* done with it */
#if HAVE_SETJMP_H
        } else {
	  /* failed to read PNG */
	  bmeps_show_error_text(out, error_not_a_png_file);
          if(rows) {
	    /* done with rows */
	    rowp = rows;
	    for(y = 0; y < he; y++) {
	      row = *rowp;
	      if(row) { free(row); }
	      *(rowp++) = NULL;
	    }
	    free(rows);
	  }
	}
#endif
	png_destroy_info_struct(pp, &pi);
      } else { 
        bmeps_show_error_text(out, error_failed_to_create_info_struct);
      }
      png_destroy_read_struct(&pp, NULL, NULL);
    } else { 
      bmeps_show_error_text(out, error_failed_to_create_read_struct);
    }
  }
  return back;
}

#else
/* HAVE_LIBPNG */

static char *error_no_pnglib[] = {
  "ERROR: bmeps was compiled without libpng support!",
  "This library is needed to read PNG images.",
  NULL
};

static int
png_run(FILE *out, FILE *in, char *name, unsigned long *w, unsigned long *h, int cmd)
{
  return 0;
}

#endif
/* HAVE_LIBPNG */

#else
/* HAVE_ZLIB */

static char *error_no_zlib[] = {
  "ERROR: bmeps was compiled without zlib support!",
  "This library is needed as the PNG file format uses flate compression.",
  NULL
};

static int
png_run(FILE *out, FILE *in, char *name, unsigned long *w, unsigned long *h, int cmd)
{
  bmeps_show_error_text(out, error_no_zlib);
  return 0;
}

#endif
/* HAVE_ZLIB */

int bmeps_png(FILE *out, FILE *in, char *name)
{
  int back;
  if(out && in) {
    bmeps_configure();
    back = png_run(out, in, name, NULL, NULL, 0);
  }
  return back;
}

int bmeps_png_bb(FILE *out, FILE *in, char *name)
{
  int back;
  if(out && in) {
    bmeps_configure();
    back = png_run(out, in, name, NULL, NULL, 1);
  }
  return back;
}

int bmeps_png_wh(FILE *in, unsigned long *w, unsigned long *h)
{
  int back = 0;
  if(w && h && in) {
    bmeps_configure();
    back = png_run(NULL, in, NULL, w, h, 2);
  }
  return back;
}


#ifndef LINT
static char sccs_id[] = {
  "@(#)07/18/06\t1.22\tpngeps.ctr\t(krause) PNG to EPS conversion module"
};
#endif

