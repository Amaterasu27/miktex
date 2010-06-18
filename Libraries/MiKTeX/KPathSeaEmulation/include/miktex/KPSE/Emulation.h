/* miktex/KPSE/Emulation.h: kpathsea emulation			-*- C++ -*-

   Copyright (C) 2000-2010 Christian Schenk

   This file is part of the MiKTeX KPSEMU Library.

   The MiKTeX KPSEMU Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX KPSEMU Library is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX KPSEMU Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(DF03C93E_3151_47A6_AEEE_6CFC30D01E42)
#define DF03C93E_3151_47A6_AEEE_6CFC30D01E42

#if defined(_MSC_VER)
#  include <io.h>
#  include <fcntl.h>
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <miktex/Core/Core>
#include <miktex/Core/Debug>

// DLL import/export switch
#if ! defined(D0A4167033297F40884B97769F47801C)
#  if defined(_MSC_VER)
#    define MIKTEXKPSEXPORT __declspec(dllimport)
#  else
#    define MIKTEXKPSEXPORT
#  endif
#endif

// API decoration for exported functions and data
#define MIKTEXKPSCEEAPI(type) MIKTEXKPSEXPORT type MIKTEXCEECALL
#define MIKTEXKPSDATA(type) MIKTEXKPSEXPORT type

#define MIKTEXKPSE_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace KPSE {

#define MIKTEXKPSE_END_NAMESPACE		\
    }						\
  }

/* _________________________________________________________________________

   config.h
   _________________________________________________________________________ */

#if defined (MIKTEX_WINDOWS)
#  define DEV_NULL "NUL"
#else
#  define DEV_NULL "/dev/null"
#endif

/* _________________________________________________________________________

   c-fopen.h
   _________________________________________________________________________ */

#ifndef FOPEN_R_MODE
#  define FOPEN_R_MODE "r"
#endif

#ifndef FOPEN_W_MODE
#  define FOPEN_W_MODE "w"
#endif

#ifndef FOPEN_RBIN_MODE
#  define FOPEN_RBIN_MODE "rb"
#endif

#ifndef FOPEN_WBIN_MODE
#  define FOPEN_WBIN_MODE "wb"
#endif

#ifndef FOPEN_ABIN_MODE
#  define FOPEN_ABIN_MODE "ab"
#endif

#if defined(MIKTEX_WINDOWS)
#  define SET_BINARY(f) _setmode(f, _O_BINARY)
#else
#  define SET_BINARY(f)
#endif

/* _________________________________________________________________________

   c-memstr.h
   _________________________________________________________________________ */

#if ! defined(HAVE_RINDEX) && ! defined(rindex)
#  define rindex(s, c) strrchr(s, c)
#endif

/* _________________________________________________________________________

   confdefs.h
   _________________________________________________________________________ */

#define KPSEVERSION "kpathsea version 3.3.2"

/* _________________________________________________________________________

   paths.h
   _________________________________________________________________________ */

#define DEFAULT_BIBTEX_CSFILE "88591lat.csf"

/* _________________________________________________________________________

   c-pathch.h
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
#  define DIR_SEP_STRING "\\"
#else
#  define DIR_SEP_STRING "/"
#endif

#if defined(MIKTEX_WINDOWS)
#  define IS_DIR_SEP(ch) ((ch) == '\\' || (ch) == '/')
#else
#  define IS_DIR_SEP(ch) ((ch) == '/')
#endif

#if defined(MIKTEX_WINDOWS)
#  define NAME_BEGINS_WITH_DEVICE(name) \
    ((name)[0] != 0 && isascii((name)[0]) && isalpha((name)[0]) && ((name)[1] == ':'))
#else
#  define NAME_BEGINS_WITH_DEVICE(name) 0
#endif

/* _________________________________________________________________________

   Obsolete
   _________________________________________________________________________ */

#define AA(args) args

#define P1H(x1) (x1)
#define P2H(p1,p2) (p1, p2)
#define P3H(p1,p2,p3) (p1, p2, p3)
#define P4H(p1,p2,p3,p4) (p1, p2, p3, p4)
#define P5H(p1,p2,p3,p4,p5) (p1, p2, p3, p4, p5)
#define P6H(p1,p2,p3,p4,p5,p6) (p1, p2, p3, p4, p5, p6)
#define P7H(p1,p2,p3,p4,p5,p6,p7) (p1, p2, p3, p4, p5, p6, p7)
#define P8H(p1,p2,p3,p4,p5,p6,p7,p8) (p1, p2, p3, p4, p5, p6, p7, p8)
#define P9H(p1,p2,p3,p4,p5,p6,p7,p8,p9) (p1, p2, p3, p4, p5, p6, p7, p8, p9)

#define P1C(t1, v1) (t1 v1)
#define P2C(t1, v1, t2, v2) (t1 v1, t2 v2)
#define P3C(t1, v1, t2, v2, t3, v3) (t1 v1, t2 v2, t3 v3)
#define P4C(t1, v1, t2, v2, t3, v3, t4, v4) (t1 v1, t2 v2, t3 v3, t4 v4)
#define P5C(t1, v1, t2, v2, t3, v3, t4, v4, t5, v5) \
  (t1 v1, t2 v2, t3 v3, t4 v4, t5 v5)
#define P6C(t1, v1, t2, v2, t3, v3, t4, v4, t5, v5, t6, v6)	\
  (t1 v1, t2 v2, t3 v3, t4 v4, t5 v5, t6 v6)
#define P7C(t1, v1, t2, v2, t3, v3, t4, v4, t5, v5, t6, v6, t7, v7)	\
  (t1 v1, t2 v2, t3 v3, t4 v4, t5 v5, t6 v6, t7 v7)
#define P8C(t1, v1, t2, v2, t3, v3, t4, v4, t5, v5, t6, v6, t7, v7, t8, v8) \
  (t1 v1, t2 v2, t3 v3, t4 v4, t5 v5, t6 v6, t7 v7, t8 v8)
#define P9C(t1,v1, t2,v2, t3,v3, t4,v4, t5,v5, t6,v6, t7,v7, t8,v8, t9,v9) \
  (t1 v1, t2 v2, t3 v3, t4 v4, t5 v5, t6 v6, t7 v7, t8 v8, t9 v9)

#define PVAR1H(p1) (p1, ...)

#define PVAR1C(t1, n1, ap) \
  (t1 n1, ...) { va_list ap; va_start (ap, n1);

#if defined(__cplusplus)
#  define STRCASECMP(s1, s2) MiKTeX::Core::StringCompare(s1, s2, true)
#else
#  define STRCASECMP(s1, s2) miktex_strcasecmp(s1, s2)
#endif

/* _________________________________________________________________________

   lib.h
   _________________________________________________________________________ */

#define START_FATAL()						\
  do {								\
    fprintf (stderr, "%s: fatal: ", program_invocation_name);

#define END_FATAL()				\
    fputs (".\n", stderr);			\
    throw (1);					\
  } while (0)

#define FATAL(str)							\
  START_FATAL (); fputs (str, stderr); END_FATAL ()

#define FATAL1(str, e1)							\
  START_FATAL (); fprintf (stderr, str, e1); END_FATAL ()

#define FATAL2(str, e1, e2)						\
  START_FATAL (); fprintf (stderr, str, e1, e2); END_FATAL ()

#define FATAL3(str, e1, e2, e3)						\
  START_FATAL (); fprintf (stderr, str, e1, e2, e3); END_FATAL ()

#define FATAL4(str, e1, e2, e3, e4)					\
  START_FATAL (); fprintf (stderr, str, e1, e2, e3, e4); END_FATAL ()

#define FATAL5(str, e1, e2, e3, e4, e5)					\
  START_FATAL (); fprintf (stderr, str, e1, e2, e3, e4, e5); END_FATAL ()

#define FATAL6(str, e1, e2, e3, e4, e5, e6)				\
  START_FATAL (); fprintf (stderr, str, e1, e2, e3, e4, e5, e6); END_FATAL ()

#define FATAL_PERROR(str)				\
  do {							\
    fprintf (stderr, "%s: ", program_invocation_name);	\
    perror (str);					\
    throw (1);						\
  } while (0)

#if defined(__cplusplus)
#  define FILESTRCASEEQ(s1, s2) (MiKTeX::Core::PathName::Compare(s1, s2) == 0)
#else
#  define FILESTRCASEEQ(s1, s2) (miktex_pathcmp(s1, s2) == 0)
#endif

#define STREQ(s1, s2) ((s1) != 0 && (s2) != 0 && (strcmp(s1, s2) == 0))

#define XRETALLOC(addr, n, t) \
  ((addr) = (t*)xrealloc(addr, (n) * sizeof(t)))

#define XTALLOC(n, t) ((t*)xmalloc((n) * sizeof(t)))

#define concat concatn
#define concat3 concatn

#if defined(__cplusplus)
#  define concatn MiKTeX::KPSE::Concat
#else
#  define concatn miktex_concat
#endif

#define find_suffix(name) MiKTeX::KPSE::FindSuffix(name)

#if defined(__cplusplus)
#  define xbasename MiKTeX::KPSE::BaseName
#else
#  define xbasename miktex_basename
#endif

#if defined(__cplusplus)
#  define xfclose MiKTeX::KPSE::FClose
#else
#  define xfclose miktex_fclose
#endif

#if defined(__cplusplus)
#  define xfopen MiKTeX::KPSE::FOpen
#else
#  define xfopen miktex_fopen
#endif

#define xfseek MiKTeX::KPSE::FSeek64

#define xftell MiKTeX::KPSE::FTell64

#if defined(__cplusplus)
#  define xmalloc(size) MiKTeX::Debug::Malloc(size, __FILE__, __LINE__)
#else
#  define xmalloc(size) miktex_core_malloc(size, __FILE__, __LINE__)
#endif

#if defined(__cplusplus)
#  define xrealloc(ptr, size) \
	MiKTeX::Debug::Realloc(ptr, size, __FILE__, __LINE__)
#else
#  define xrealloc(ptr, size) \
	miktex_core_realloc(ptr, size, __FILE__, __LINE__)
#endif

#if defined(__cplusplus)
#  define xstrdup(str) MiKTeX::Debug::StrDup(str, __FILE__, __LINE__)
#else
#  define xstrdup(str) miktex_core_strdup(str, __FILE__, __LINE__)
#endif

#if defined(__cplusplus)
#  define xputenv MiKTeX::Core::Utils::SetEnvironmentString
#else
#  define xputenv miktex_putenv
#endif

/* _________________________________________________________________________

   c-type.h
   _________________________________________________________________________ */

#define ISALNUM(c) (isascii(c) && isalnum(c))

#define ISXDIGIT(c) (isascii(c) && isxdigit(c))

#define TOLOWER(c) ((isascii(c) && isupper(c)) ? tolower(c) : (c))

#define ISBLANK(c) (isascii(c) && ((c) == ' ' || (c) == '\t'))

/* _________________________________________________________________________

   debug.h
   _________________________________________________________________________ */

#define KPSE_DEBUG_SET(bit)

/* _________________________________________________________________________

   c-proto.h
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
#  define KPSEDLL __declspec(dllimport)
#else
#  define KPSEDLL
#endif

/* _________________________________________________________________________

   c-auto.h
   _________________________________________________________________________ */

#define MAKE_TEX_PK_BY_DEFAULT 1

/* _________________________________________________________________________

   tex-file.h
   _________________________________________________________________________ */

#define kpse_set_program_enabled(fmt, value, level)

#define kpse_reset_program_name(progname)

#if defined(__cplusplus)
#  define kpse_find_file MiKTeX::KPSE::FindFile
#else
#  define kpse_find_file miktex_kpse_find_file
#endif

#define kpse_find_ofm(name) kpse_find_file(name, kpse_ofm_format, 1)

#define kpse_find_pict(name) kpse_find_file(name, kpse_pict_format, 1)

#define kpse_find_tex(name)  kpse_find_file(name, kpse_tex_format, 1)

#define kpse_find_tfm(name) kpse_find_file(name, kpse_tfm_format, 1)

#if defined(__cplusplus)
#  define kpse_open_file MiKTeX::KPSE::OpenFile
#else
#  define kpse_open_file miktex_kpse_open_file
#endif

#if defined(__cplusplus)
#define kpse_in_name_ok(fname) \
  (MiKTeX::Core::Utils::IsSafeFileName(fname, true) ? 1 : 0)
#else
#define kpse_in_name_ok(fname) \
  miktex_kpse_in_name_ok(fname)
#endif

#if defined(__cplusplus)
#define kpse_out_name_ok(fname) \
  (MiKTeX::Core::Utils::IsSafeFileName(fname, false) ? 1 : 0)
#else
#define kpse_out_name_ok(fname) \
  miktex_kpse_out_name_ok(fname)
#endif

/* _________________________________________________________________________

   types.h
   _________________________________________________________________________ */

#define kpse_bug_address miktex_kpse_bug_address

#define kpse_fallback_resolutions_string \
  miktex_kpse_fallback_resolutions_string

#define kpathsea_debug miktex_kpathsea_debug

#define kpse_make_tex_discard_errors miktex_kpse_make_tex_discard_errors

#undef program_invocation_name
#if defined(__cplusplus)
#   define program_invocation_name MiKTeX::KPSE::GetProgramInvocationName()
#else
#  define program_invocation_name miktex_get_program_invocation_name()
#endif

#if defined(__cplusplus)
MIKTEX_BEGIN_EXTERN_C_BLOCK
#endif

typedef char * string;
typedef const char * const_string;

#if ! defined(HAVE_BOOLEAN)
#  define HAVE_BOOLEAN
#  if defined(MIKTEX_WINDOWS)
     // shadow definition in rpcndr.h
#    define boolean int
#  else
     typedef int boolean;
#  endif
#  if ! defined(__cplusplus)
#    if ! defined(true)
#      define true 1
#      define false 0
#    endif
#  endif
#endif

typedef enum
{
  kpse_gf_format,
  kpse_pk_format,
  kpse_any_glyph_format,
  kpse_tfm_format, 
  kpse_afm_format, 
  kpse_base_format, 
  kpse_bib_format, 
  kpse_bst_format, 
  kpse_cnf_format,
  kpse_db_format,
  kpse_fmt_format,
  kpse_fontmap_format,
  kpse_mem_format,
  kpse_mf_format, 
  kpse_mfpool_format, 
  kpse_mft_format, 
  kpse_mp_format, 
  kpse_mppool_format, 
  kpse_mpsupport_format,
  kpse_ocp_format,
  kpse_ofm_format, 
  kpse_opl_format,
  kpse_otp_format,
  kpse_ovf_format,
  kpse_ovp_format,
  kpse_pict_format,
  kpse_tex_format,
  kpse_texdoc_format,
  kpse_texpool_format,
  kpse_texsource_format,
  kpse_tex_ps_header_format,
  kpse_troff_font_format,
  kpse_type1_format, 
  kpse_vf_format,
  kpse_dvips_config_format,
  kpse_ist_format,
  kpse_truetype_format,
  kpse_type42_format,
  kpse_web2c_format,
  kpse_program_text_format,
  kpse_program_binary_format,
  kpse_miscfonts_format,
  kpse_web_format,
  kpse_cweb_format,
  kpse_enc_format,
  kpse_cmap_format,
  kpse_sfd_format,
  kpse_opentype_format,
  kpse_pdftex_config_format,
  kpse_lig_format,
  kpse_texmfscripts_format,
  kpse_last_format
} kpse_file_format_type;


#if defined(__cplusplus)
MIKTEX_END_EXTERN_C_BLOCK
#endif

/* _________________________________________________________________________

   absolute.h
   _________________________________________________________________________ */

#if 0
#define kpse_absolute_p(filename, relative_ok)	  \
  (MIKTEX_ASSERT(! (relative_ok)),		  \
   MiKTeX::Core::Utils::IsAbsolutePath(filename))
#else
#define kpse_absolute_p(filename, relative_ok)	  \
   MiKTeX::Core::Utils::IsAbsolutePath(filename)
#endif

/* _________________________________________________________________________

   variable.h
   _________________________________________________________________________ */

#define kpse_var_expand(src) (src)

#if defined(__cplusplus)
#  define kpse_var_value MiKTeX::KPSE::VarValue
#else
#  define kpse_var_value miktex_kpse_var_value
#endif

/* _________________________________________________________________________

   version.h
   _________________________________________________________________________ */

#define kpathsea_version_string miktex_kpathsea_version_string

/* _________________________________________________________________________

   tex-glyph.h
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEX_BEGIN_EXTERN_C_BLOCK
#endif

typedef enum
{
  kpse_glyph_source_normal,
  kpse_glyph_source_alias,
  kpse_glyph_source_maketex,
  kpse_glyph_source_fallback
} kpse_glyph_source_type;

typedef struct
{
  char *			name;
  unsigned			dpi;
  kpse_file_format_type		format;
  kpse_glyph_source_type	source;
} kpse_glyph_file_type;		

#if defined(__cplusplus)
MIKTEX_END_EXTERN_C_BLOCK
#endif

#if defined(__cplusplus)
#  define kpse_bitmap_tolerance MiKTeX::KPSE::BitmapTolerance
#else
#  define kpse_bitmap_tolerance miktex_kpse_bitmap_tolerance
#endif

#if defined(__cplusplus)
#  define kpse_find_glyph MiKTeX::KPSE::FindGlyph
#else
#  define kpse_find_glyph miktex_kpse_find_glyph
#endif

#define kpse_find_pk(font_name, dpi, glyph_file) \
  kpse_find_glyph(font_name, dpi, kpse_pk_format, glyph_file)

#define kpse_find_vf(name) kpse_find_file(name, kpse_vf_format, 0)


/* _________________________________________________________________________

   tex-hush.h
   _________________________________________________________________________ */

#define kpse_tex_hush(what) 0

/* _________________________________________________________________________

   proginit.h
   _________________________________________________________________________ */

#if defined(__cplusplus)
#  define kpse_init_prog MiKTeX::KPSE::InitProg
#else
#  define kpse_init_prog miktex_kpse_init_prog
#endif

/* _________________________________________________________________________

   magstep.h
   _________________________________________________________________________ */

#if defined(__cplusplus)
#  define kpse_magstep_fix MiKTeX::KPSE::MagStepFix
#else
#  define kpse_magstep_fix miktex_kpse_magstep_fix
#endif

/* _________________________________________________________________________

   progname.h
   _________________________________________________________________________ */

#if defined(__cplusplus)
#  define kpse_set_program_name(argv0, progname) \
     MiKTeX::KPSE::SetProgramName(argv0, progname)
#else
#  define kpse_set_program_name(argv0, progname) \
     miktex_kpse_set_program_name(argv0, progname)
#endif

#define kpse_set_progname(argv0) kpse_set_program_name(argv0, 0)

/* _________________________________________________________________________

   Prototypes (C++)
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEXKPSE_BEGIN_NAMESPACE;

MIKTEXKPSCEEAPI(const char *)
BaseName (/*[in]*/ const char * lpszFileName);

MIKTEXKPSCEEAPI(int)
BitmapTolerance (/*[in]*/ double	dpi1,
		 /*[in]*/ double	dpi2);

MIKTEXKPSCEEAPI(void)
FClose (/*[in]*/ FILE *		stream,
	/*[in]*/ const char *	fileName);

MIKTEXKPSCEEAPI(char *)
FindFile (/*[in]*/ const char *			lpszFileName,
	  /*[in]*/ kpse_file_format_type	format,
	  /*[in]*/ int				mustExist);

MIKTEXKPSCEEAPI(char *)
FindGlyph (/*[in]*/ const char *		lpszFontName,
	   /*[in]*/ unsigned			dpi,
	   /*[in]*/ kpse_file_format_type	format,
	   /*[out]*/ kpse_glyph_file_type *	glyph_file);

MIKTEXKPSCEEAPI(char *)
FindSuffix (const char * lpszPath);

MIKTEXKPSCEEAPI(FILE *)
FOpen (/*[in]*/ const char *	lpszName,
       /*[in]*/ const char *	lpszMode);

MIKTEXKPSCEEAPI(FILE *)
TryFOpen (/*[in]*/ const char *	lpszName,
	  /*[in]*/ const char *	lpszMode);

MIKTEXKPSCEEAPI(int)
FSeek (/*[in]*/ FILE *		stream,
       /*[in]*/ long		offset,
       /*[in]*/ int		where,
       /*[in]*/ const char *	lpszFileName);

MIKTEXKPSCEEAPI(int)
FSeek64 (/*[in]*/ FILE *	stream,
	 /*[in]*/ MIKTEX_INT64	offset,
	 /*[in]*/ int		where,
	 /*[in]*/ const char *	lpszFileName);

MIKTEXKPSCEEAPI(long)
FTell (/*[in]*/ FILE *		stream,
       /*[in]*/ const char *	lpszFileName);

MIKTEXKPSCEEAPI(MIKTEX_INT64)
FTell64 (/*[in]*/ FILE *	stream,
	 /*[in]*/ const char *	lpszFileName);

MIKTEXKPSCEEAPI(const char *)
GetProgramInvocationName ();

MIKTEXKPSCEEAPI(void)
InitProg (/*[in]*/ const char *	lpszPrefix,
	  /*[in]*/ unsigned	dpi,
	  /*[in]*/ const char *	lpszMode,
	  /*[in]*/ const char *	lpszFallback);

MIKTEXKPSCEEAPI(char *)
Concat (/*[in]*/ const char * lpsz1, ...);

MIKTEXKPSCEEAPI(unsigned)
MagStepFix (/*[in]*/ unsigned	dpi,
	    /*[in]*/ unsigned	bdpi,
	    /*[out]*/ int *	pRet);

MIKTEXKPSCEEAPI(void *)
Malloc (/*[in]*/ size_t		size,
	/*[in]*/ const char *	lpszFileName,
	/*[in]*/ int		line);

MIKTEXKPSCEEAPI(FILE *)
OpenFile (/*[in]*/ const char *			lpszFileName,
	  /*[in]*/ kpse_file_format_type	format);

MIKTEXKPSCEEAPI(void)
PutEnv (/*[in]*/ const char * lpszVarName,
	/*[in]*/ const char * lpszValue);

MIKTEXKPSCEEAPI(void *)
Realloc (/*[in]*/ void *	ptr,
	 /*[in]*/ size_t	size,
	 /*[in]*/ const char *	lpszFileName,
	 /*[in]*/ int		line);

MIKTEXKPSCEEAPI(void)
SetProgramName (/*[in]*/ const char *	lpszArgv0,
		/*[in]*/ const char *	lpszProgramName);

MIKTEXKPSCEEAPI(char *)
StrDup (/*[in]*/ const char *	lpsz,
	/*[in]*/ const char *	lpszFileName,
	/*[in]*/ int		line);

MIKTEXKPSCEEAPI(char *)
VarValue (/*[in]*/ const char * lpszVarName);

MIKTEXKPSE_END_NAMESPACE;
#endif

/* _________________________________________________________________________

   C Wrappers
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEX_BEGIN_EXTERN_C_BLOCK
#endif

MIKTEXKPSCEEAPI(const char *)
miktex_basename (/*[in]*/ const char * lpszFileName);

MIKTEXKPSCEEAPI(char *)
miktex_concat (/*[in]*/ const char * lpsz1, ...);

MIKTEXKPSCEEAPI(void)
miktex_fclose (/*[in]*/ FILE *			stream,
	       /*[in]*/ const char *	fileName);

MIKTEXKPSCEEAPI(FILE *)
miktex_fopen (/*[in]*/ const char *	lpszName,
	      /*[in]*/ const char *	lpszMode);

MIKTEXKPSCEEAPI(const char *)
miktex_get_program_invocation_name ();

MIKTEXKPSCEEAPI(int)
miktex_kpse_bitmap_tolerance (/*[in]*/ double	dpi1,
			      /*[in]*/ double	dpi2);

MIKTEXKPSCEEAPI(char *)
miktex_kpse_find_file (/*[in]*/ const char *		lpszFileName,
		       /*[in]*/ kpse_file_format_type	format,
		       /*[in]*/ int			mustExist);

MIKTEXKPSCEEAPI(char *)
miktex_kpse_find_glyph (/*[in]*/ const char *			lpszFontName,
			/*[in]*/ unsigned			dpi,
			/*[in]*/ kpse_file_format_type		format,
			/*[out]*/ kpse_glyph_file_type *	glyph_file);

MIKTEXKPSCEEAPI(int)
miktex_kpse_in_name_ok (/*[in]*/ const char *lpszFileName);

MIKTEXKPSCEEAPI(void)
miktex_kpse_init_prog (/*[in]*/ const char *	lpszPrefix,
		       /*[in]*/ unsigned	dpi,
		       /*[in]*/ const char *	lpszMode,
		       /*[in]*/ const char *	lpszFallback);

MIKTEXKPSCEEAPI(unsigned)
miktex_kpse_magstep_fix (/*[in]*/ unsigned	dpi,
			 /*[in]*/ unsigned	bdpi,
			 /*[out]*/ int *	pRet);

MIKTEXKPSCEEAPI(FILE *)
miktex_kpse_open_file (/*[in]*/ const char *		lpszFileName,
		       /*[in]*/ kpse_file_format_type	format);

MIKTEXKPSCEEAPI(int)
miktex_kpse_out_name_ok (/*[in]*/ const char *lpszFileName);

MIKTEXKPSCEEAPI(void)
miktex_kpse_set_program_name (/*[in]*/ const char *	lpszArgv0,
			      /*[in]*/ const char *	lpszProgramName);

MIKTEXKPSCEEAPI(char *)
miktex_kpse_var_value (/*[in]*/ const char * lpszVarName);

MIKTEXKPSCEEAPI(void *)
miktex_malloc (/*[in]*/ size_t		size,
	       /*[in]*/ const char *	lpszFileName,
	       /*[in]*/ int		line);

MIKTEXKPSCEEAPI(void *)
miktex_realloc (/*[in]*/ void *		ptr,
		/*[in]*/ size_t		size,
		/*[in]*/ const char *	lpszFileName,
		/*[in]*/ int		line);

MIKTEXKPSCEEAPI(void)
miktex_putenv (/*[in]*/ const char * lpszVarName,
	       /*[in]*/ const char * lpszValue);

MIKTEXKPSCEEAPI(char *)
miktex_strdup (/*[in]*/ const char *	s,
	       /*[in]*/ const char *	lpszFileName,
	       /*[in]*/ int		line);

MIKTEXKPSCEEAPI(int)
miktex_strcasecmp (/*[in]*/ const char * lpsz1,
		   /*[in]*/ const char * lpsz2);

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
MIKTEXKPSDATA(char *)
miktex_kpse_bug_address;

extern
MIKTEXKPSDATA(const char *)
miktex_kpse_fallback_resolutions_string;

extern
MIKTEXKPSDATA(int)
miktex_kpse_make_tex_discard_errors;

extern
MIKTEXKPSDATA(char *)
miktex_kpathsea_version_string;

extern
MIKTEXKPSDATA(unsigned)
miktex_kpathsea_debug;

#if defined(__cplusplus)
MIKTEX_END_EXTERN_C_BLOCK
#endif

/* _________________________________________________________________________ */

#endif
