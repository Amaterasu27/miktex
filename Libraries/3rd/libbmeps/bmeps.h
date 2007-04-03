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

#ifndef BMEPS_INC
#define BMEPS_INC 1

#include <stdio.h>

typedef struct {
  int psl; int col; int a85; int rl; int fl;
  int alpha; int trans; int altrig;
  int mix; int specbg; int bg_red; int bg_green; int bg_blue; int dsc_show;
  int shp; int dic; int vmr;
  int usr;
} BmepsConfiguration;

#ifdef __cplusplus
extern "C" {
#endif

void bmeps_setup(
  int psl, int col, int a85, int rl, int fl,
  int alpha, int trans, int altrig,
  int mix, int specbg, int bg_red, int bg_green, int bg_blue, int dsc_show,
  int shp, int dic, int vmr, int usr
);
void bmeps_cfg(char *options);
void bmeps_set_draft(int dr);
int  bmeps_get_pslevel(void);
int  bmeps_get_colored(void);
int  bmeps_get_enc_a85(void);
int  bmeps_get_enc_rl(void);
int  bmeps_get_enc_fl(void);
int  bmeps_get_draft(void);
int  bmeps_get_alpha(void);
int  bmeps_get_trans(void);
int  bmeps_get_altrig(void);
int  bmeps_get_dsc_comm(void);
void bmeps_configure(void);
void bmeps_version(FILE *out);
int bmeps_get_mix(void);
int bmeps_get_specbg(void);
int bmeps_get_bg_red(void);
int bmeps_get_bg_green(void);
int bmeps_get_bg_blue(void);
int bmeps_get_showpage(void);
int bmeps_get_dictionary(void);
int bmeps_get_vmreclaim(void);
int bmeps_get_usr(void);
int bmeps_get_app(void);
int bmeps_get_verb(void);
void bmeps_set_app(int);
void bmeps_set_verb(int);
void bmeps_show_error_text(FILE *, char **);

/*
	Support function used by handler functions
*/
void bmeps_bb(FILE *out, unsigned long w, unsigned long h);
void bmeps_header(FILE *out, char *name, unsigned long w, unsigned long h);
void bmeps_begin_image(FILE *out, unsigned long w, unsigned long h);
void bmeps_add_rgb(int red, int green, int blue);
void bmeps_add_gray(int gray);
void bmeps_add_trans(int alpha);
void bmeps_end_image(FILE *out);
void bmeps_draft(FILE *out, unsigned long w, unsigned long h);
void bmeps_footer(FILE *out);
void bmeps_set_trans(int flag);
void bmeps_set_usr(int fl);

void bmeps_get_cfg(BmepsConfiguration *bcp);
void bmeps_set_cfg(BmepsConfiguration *bcp);
void bmeps_copy_cfg(BmepsConfiguration *d, BmepsConfiguration *s);

char *bmeps_tmpname(void);
void bmeps_delete(char *filename);

/*
	Invocation points
*/
int  bmeps_run(FILE *out, FILE *in, char *name);
int  bmeps_run_bb(FILE *out, FILE *in, char *name);
int  bmeps_wh(FILE *in, char *name, unsigned long *w, unsigned long *h);
int  bmeps_can_handle(char *name);

/*	
	Handler functions invoked by the above functions
*/
int  bmeps_png(FILE *out, FILE *in, char *name);
int  bmeps_png_bb(FILE *out, FILE *in, char *name);
int  bmeps_png_wh(FILE *in, unsigned long *w, unsigned long *h);

/*
  gsave already printed when adjusting the resolution
*/
void bmeps_set_gsave_done(void);

#ifdef __cplusplus
}
#endif
/* #ifdef __cplusplus */

#define MAXPSLEVEL 3

#endif
/* BMEPS_INC */

