/* miktex/W2C/Emulation.h: Web2C emulation			-*- C++ -*-

   Copyright (C) 2010 Christian Schenk

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
#include <miktex/Core/Debug>
#include <miktex/Core/IntegerTypes>
#include <miktex/KPSE/Emulation>

// DLL import/export switch
#if ! defined(B96BCD894353492A9CF685C84B9AB5E0)
#  if defined(_MSC_VER)
#    define MIKTEXW2CEXPORT __declspec(dllimport)
#  else
#    define MIKTEXW2CEXPORT
#  endif
#endif

// API decoration for exported functions and data
#define MIKTEXW2CCEEAPI(type) MIKTEXW2CEXPORT type MIKTEXCEECALL
#define MIKTEXW2CDATA(type) MIKTEXW2CEXPORT type

#define MIKTEXWEB2C_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace Web2C {

#define MIKTEXWEB2C_END_NAMESPACE		\
    }						\
  }

/* _________________________________________________________________________

   c-auto.h
   _________________________________________________________________________ */

#define WEB2CVERSION " MIKTEXW2C"

/* _________________________________________________________________________

   web2c/lib.h
   _________________________________________________________________________ */

#define open_input(f_ptr, filefmt, fopen_mode) \
  MiKTeX::Web2C::OpenInput(nameoffile + 1, f_ptr, filefmt, fopen_mode)

#define versionstring \
  miktex_web2c_version_string

#define recorder_record_input(fname)

#define recorder_record_output(fname)

/* _________________________________________________________________________

   texmfmp.h
   _________________________________________________________________________ */

#define	secondsandmicros(s, m) \
  MiKTeX::Web2C::GetSecondsAndMicros(&(s), &(m))

/* _________________________________________________________________________

   cpascal.h
   _________________________________________________________________________ */

#define xmallocarray(type, size) \
  ((type*)xmalloc(((size) + 1) * sizeof(type)))

#define xreallocarray(ptr, type, size) \
  ((type*)xrealloc(ptr, ((size) + 1) * sizeof(type)))

/* _________________________________________________________________________

   w2c/config.h
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEX_BEGIN_EXTERN_C_BLOCK
#endif

typedef int integer;

typedef MIKTEX_INT64 longinteger;

#if defined(__cplusplus)
MIKTEX_END_EXTERN_C_BLOCK
#endif

/* _________________________________________________________________________

   Prototypes
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEXWEB2C_BEGIN_NAMESPACE;

MIKTEXW2CCEEAPI(void)
GetSecondsAndMicros (/*[out]*/ int * pSeconds,
		     /*[out]*/ int * pMicros);

MIKTEXW2CCEEAPI(int)
OpenInput (/*[in,out]*/ char *			lpszFileName,
	   /*[in]*/ FILE **			ppfile,
	   /*[in]*/ kpse_file_format_type	format,
	   /*[in]*/ const char *		lpszMode);

MIKTEXWEB2C_END_NAMESPACE;
#endif

/* _________________________________________________________________________

   Variables
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEX_BEGIN_EXTERN_C_BLOCK
#endif

extern
MIKTEXW2CDATA(const char *)
miktex_web2c_version_string;

#if defined(__cplusplus)
MIKTEX_END_EXTERN_C_BLOCK
#endif

/* _________________________________________________________________________ */

#endif
