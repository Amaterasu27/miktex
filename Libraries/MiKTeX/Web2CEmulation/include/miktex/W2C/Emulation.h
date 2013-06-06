/* miktex/W2C/Emulation.h: Web2C emulation			-*- C++ -*-

   Copyright (C) 2010-2013 Christian Schenk

   This file is part of the MiKTeX W2CEMU Library.

   The MiKTeX W2CEMU Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX W2CEMU Library is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX W2CEMU Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(AA9CDF05506A4A07827B1144C4755C06)
#define AA9CDF05506A4A07827B1144C4755C06

#include <miktex/Core/Core>
#include <miktex/Core/Definitions>
#include <miktex/Core/Debug>
#include <miktex/Core/IntegerTypes>
#include <miktex/Core/Version>
#include <miktex/KPSE/Emulation>

#include "pre.h"

/* _________________________________________________________________________

   c-auto.h
   _________________________________________________________________________ */

#define WEB2CVERSION " (" MIKTEX_BANNER_STR ")"

/* _________________________________________________________________________

   w2c/config.h
   _________________________________________________________________________ */

#include "../../w2c/config.h"

/* _________________________________________________________________________

   lib/lib.h
   _________________________________________________________________________ */

#include "../../lib/lib.h"

/* _________________________________________________________________________

   texmfmp.h
   _________________________________________________________________________ */

#define	secondsandmicros(s, m) \
  MiKTeX::Web2C::GetSecondsAndMicros(&(s), &(m))

#if defined(THEAPP)
#  define generic_synctex_get_current_name() \
  xstrdup(THEAPP.GetFQNameOfFile())
#endif

/* _________________________________________________________________________

   cpascal.h
   _________________________________________________________________________ */

#if defined(__cplusplus)
template<class T> T * addressof(T & x) { return (&x); }
#else
#define addressof(x) (&(x))
#endif

#define decr(x) --(x)

#define incr(x) ++(x)

#define halfp(i) ((i) >> 1)

#define ISDIRSEP IS_DIR_SEP

#define libcfree free

#define odd(x) ((x) & 1)

#define round(x) miktex_zround((double) (x))

#define xmallocarray(type, size) \
  ((type*)xmalloc(((size) + 1) * sizeof(type)))

#define xreallocarray(ptr, type, size) \
  ((type*)xrealloc(ptr, ((size) + 1) * sizeof(type)))

#define nil ((void*)0)

#define kpsevarvalue kpse_var_value

#define kpseinitprog kpse_init_prog

#define kpsesetprogramenabled kpse_set_program_enabled

#define kpsepkformat kpse_pk_format

#define kpsesrccmdline kpse_src_cmdline

#define kpsemaketexdiscarderrors kpse_make_tex_discard_errors

typedef double real;

#define intcast(x) ((integer)(x))

#define stringcast(x) ((char *) (x))

#define ucharcast(x) ((unsigned char)(x))

/* _________________________________________________________________________

   Prototypes
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEXWEB2C_BEGIN_NAMESPACE;

MIKTEXW2CCEEAPI(void)
GetSecondsAndMicros (/*[out]*/ int * pSeconds,
		     /*[out]*/ int * pMicros);

MIKTEXWEB2C_END_NAMESPACE;
#endif

#if defined(__cplusplus)
MIKTEX_BEGIN_EXTERN_C_BLOCK
#endif

MIKTEXW2CCEEAPI(integer)
miktex_zround (/*[in]*/ double r);

#if defined(__cplusplus)
MIKTEX_END_EXTERN_C_BLOCK
#endif


/* _________________________________________________________________________ */

#endif
