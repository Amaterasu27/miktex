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

#ifndef OE_H_INCLUDED
#define OE_H_INCLUDED

#include <stdio.h>

#if HAVE_CONFIG_H
#include "config.h"
#else
#define HAVE_ZLIB	1
#endif

#if	HAVE_ZLIB
#include <zlib.h>
#else
#error	"The zlib library is needed. Install this library first."
#endif

typedef struct {
  int mode;
  FILE *out;
  int textpos;
  unsigned long a85_value;
  int      a85_consumed;
  unsigned long a85_4;
  unsigned long a85_3;
  unsigned long a85_2;
  unsigned long a85_1;
  unsigned long a85_0;
  int rl_lastbyte;
  int *rl_buffer;
  int rl_bufused;
  int rl_state;
  int bit_value;
  int bit_consumed;
  z_stream flate_stream;
  Bytef *fl_i_buffer;
  Bytef *fl_o_buffer;
  uLong  fl_i_size;
  uLong  fl_o_size;
  uLong  fl_i_used;
  int    flate_rate;
} Output_Encoder;

#ifdef __cplusplus
extern "C" {
#endif

void oe_init(Output_Encoder *o, FILE *out, int mode, int rate, int *buf,
  Bytef *flib, size_t flis, Bytef *flob, size_t flos
);
void oe_byte_add(Output_Encoder *o, int b);
void oe_byte_flush(Output_Encoder *o);
void oe_bit_add(Output_Encoder *o, int b);
void oe_bit_flush(Output_Encoder *o);

#ifdef __cplusplus
}
#endif



#define OE_ASC85 1
#define OE_FLATE 2
#define OE_RL    4

#endif
/* #ifndef OE_H_INCLUDED */

