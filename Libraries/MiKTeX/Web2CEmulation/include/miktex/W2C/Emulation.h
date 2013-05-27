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

#define WEB2CVERSION " (" MIKTEX_BANNER_STR ")"

/* _________________________________________________________________________

   w2c/config.h
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEX_BEGIN_EXTERN_C_BLOCK
#endif

typedef int integer;

typedef MIKTEX_INT64 longinteger;

#define LONGINTEGER_TYPE long long
#define LONGINTEGER_PRI "ll"

MIKTEXW2CEXPORT MIKTEXNORETURN void MIKTEXCEECALL
miktex_uexit (int status);

#define uexit miktex_uexit

MIKTEXW2CEXPORT MIKTEXNORETURN void MIKTEXCEECALL
miktex_usagehelp (/*[in]*/ const char **	lpszLines,
		  /*[in]*/ const char *		lpszBugEmail);

#define usagehelp miktex_usagehelp

#if defined(__cplusplus)
MIKTEX_END_EXTERN_C_BLOCK
#endif

/* _________________________________________________________________________

   web2c/lib.h
   _________________________________________________________________________ */

#if defined(__cplusplus)
#define open_input(f_ptr, filefmt, fopen_mode) \
  MiKTeX::Web2C::OpenInput(nameoffile + 1, f_ptr, filefmt, fopen_mode)
#else
#define open_input(f_ptr, filefmt, fopen_mode) \
  UNIMPLEMENTED_miktex_web2c_open_input(f_ptr, filefmt, fopen_mode)
#endif

#define close_file(f) ((f) != 0 ? (void)fclose(f) : (void)0)

#define versionstring miktex_web2c_version_string

#define recorder_change_filename(new_name)

#define recorder_record_input(fname)

#define recorder_record_output(fname)

#define recorder_enabled miktex_web2c_recorder_enabled

#if defined(THEAPP)
#  define output_directory (THEAPP.GetOutputDirectory().Empty() ? 0 : THEAPP.GetOutputDirectory().Get())
#else
#  define output_directory miktex_web2c_output_directory
#endif

#define fullnameoffile miktex_web2c_fullnameoffile

#define setupboundvariable(var, var_name, dflt) \
  miktex_setupboundvariable(var, var_name, dflt)

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

#define ucharcast(x) ((unsigned char)(x))

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

#if defined(__cplusplus)
MIKTEX_BEGIN_EXTERN_C_BLOCK
#endif

MIKTEXW2CCEEAPI(integer)
miktex_zround (/*[in]*/ double r);

MIKTEXW2CCEEAPI(void)
miktex_setupboundvariable (/*[in]*/ integer *      pVar,
			   /*[in]*/ const char *   lpszVarName,
			   /*[in]*/ integer	      dflt);

#if defined(__cplusplus)
MIKTEX_END_EXTERN_C_BLOCK
#endif

/* _________________________________________________________________________

   Variables
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEX_BEGIN_EXTERN_C_BLOCK
#endif

extern
MIKTEXW2CDATA(string)
miktex_web2c_fullnameoffile;

extern
MIKTEXW2CDATA(const_string)
miktex_web2c_output_directory;

extern
MIKTEXW2CDATA(boolean)
miktex_web2c_recorder_enabled;

extern
MIKTEXW2CDATA(const_string)
miktex_web2c_version_string;

#if defined(__cplusplus)
MIKTEX_END_EXTERN_C_BLOCK
#endif

/* _________________________________________________________________________ */

#endif
