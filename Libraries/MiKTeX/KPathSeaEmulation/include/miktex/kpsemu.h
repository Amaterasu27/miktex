/* miktex/kpsemu.h: Web2C/KPSE emulation			-*- C++ -*-

   Copyright (C) 2000-2007 Christian Schenk

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

#if defined(_MSC_VER)
#  include <io.h>
#  include <fcntl.h>
#endif

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <miktex/core.h>

#if ! defined(D0A4167033297F40884B97769F47801C)
#  if defined(_MSC_VER)
#    define MIKTEXKPSEXPORT __declspec(dllimport)
#  else
#    define MIKTEXKPSEXPORT
#  endif
#endif

#if defined(_MSC_VER)
#  define MIKTEXKPSCALL __stdcall
#else
#  define MIKTEXKPSCALL
#endif

#define MIKTEXKPSAPI(type) MIKTEXKPSEXPORT type MIKTEXKPSCALL
#define MIKTEXKPSDATA(type) MIKTEXKPSEXPORT type

#define MIKTEX_BEGIN_EXTERN_C_BLOCK extern "C" {
#define MIKTEX_END_EXTERN_C_BLOCK }

#define MIKTEXKPSE_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace KPSE {

#define MIKTEXKPSE_END_NAMESPACE		\
    }						\
  }

#define MIKTEXWEB2C_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace Web2C {

#define MIKTEXWEB2C_END_NAMESPACE		\
    }						\
  }

/* _________________________________________________________________________

   Macros
   _________________________________________________________________________ */

#define FOPEN_R_MODE MIKTEXTEXT("r")
#define FOPEN_W_MODE MIKTEXTEXT("w")
#define FOPEN_RBIN_MODE MIKTEXTEXT("rb")
#define FOPEN_WBIN_MODE MIKTEXTEXT("wb")
#define FOPEN_ABIN_MODE MIKTEXTEXT("ab")

#if defined(MIKTEX_WINDOWS)
# define DIR_SEP_STRING MIKTEXTEXT("\\")
#else
# define DIR_SEP_STRING MIKTEXTEXT("/")
#endif

/* _________________________________________________________________________

   P?H and P?C Macros
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

/* _________________________________________________________________________

   FATAL Macros
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

/* _________________________________________________________________________

   DEBUG Macros
   _________________________________________________________________________ */

#define KPSE_DEBUG_SET(x)

/* _________________________________________________________________________

   Emulation Macros
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
#  define KPSEDLL __declspec(dllimport)
#else
#  define KPSEDLL
#endif

#define KPSEVERSION MIKTEXTEXT("kpathsea version 3.3.2")

#if defined(__cplusplus)
#  define FILESTRCASEEQ(s1, s2) (MiKTeX::Core::PathName::Compare(s1, s2) == 0)
#else
#  define FILESTRCASEEQ(s1, s2) (miktex_pathcmp(s1, s2) == 0)
#endif

#define ISALNUM(c) isalnum(c)

#define ISXDIGIT(c) isxdigit(c)

#define IS_DIR_SEP(c) ((c) == MIKTEXTEXT('\\') || (c) == MIKTEXTEXT('/'))

#define MAKE_TEX_PK_BY_DEFAULT 1

#define NAME_BEGINS_WITH_DEVICE(name) (isalpha(name[0]) && (name[1] == ':'))

#if defined(MIKTEX_WINDOWS)
#  define SET_BINARY(f) _setmode((f), _O_BINARY)
#else
#  define SET_BINARY(f)
#endif

#if defined(__cplusplus)
#  define STRCASECMP(s1, s2) MiKTeX::Core::StringCompare(s1, s2, true)
#  define strcasecmp(s1, s2) MiKTeX::Core::StringCompare(s1, s2, true)
#else
#  define STRCASECMP(s1, s2) miktex_strcasecmp(s1, s2)
#  define strcasecmp(s1, s2) miktex_strcasecmp(s1, s2)
#endif

#define STREQ(s1, s2) ((s1) && (s2) && (strcmp(s1, s2) == 0))

#define TOLOWER(c) tolower(c)

#define XRETALLOC(addr, n, t) \
  ((addr) = (t *) xrealloc(addr, (n) * sizeof(t)))

#define XTALLOC(n, t) ((t *) xmalloc((n) * sizeof(t)))

#define concat(s1, s2)							\
  strcat(strcpy((MIKTEXCHAR *)xmalloc(strlen(s1) + strlen(s2) + 1),	\
                (s1)),							\
	 (s2))

#define concat3(s1, s2, s3)					\
  strcat(strcat(strcpy((MIKTEXCHAR *)xmalloc(strlen(s1)		\
                                             + strlen(s2)	\
					     + strlen(s3)	\
                                             + 1),		\
                       (s1)),					\
		(s2)),						\
         (s3))

#define find_suffix(name) MiKTeX::KPSE::FindSuffix(name)

#define isblank(c) ((c) == ' ' || (c) == '\t')

#define kpse_set_program_enabled(fmt, value, level)

#define kpse_bug_address miktex_kpse_bug_address

#define kpse_fallback_resolutions_string \
  miktex_kpse_fallback_resolutions_string

#define kpse_absolute_p(path) \
  MiKTeX::Core::Utils::IsAbsolutePath(path)

#define kpse_var_expand(str) (str)

#define kpathsea_debug miktex_kpathsea_debug

#define kpathsea_version_string miktex_kpathsea_version_string

#if defined(__cplusplus)
#  define kpse_bitmap_tolerance MiKTeX::KPSE::BitmapTolerance
#else
#  define kpse_bitmap_tolerance miktex_kpse_bitmap_tolerance
#endif

#if defined(__cplusplus)
#  define kpse_find_file MiKTeX::KPSE::FindFile
#else
#  define kpse_find_file miktex_kpse_find_file
#endif

#define kpse_tex_hush(what) 0

#if defined(__cplusplus)
#  define kpse_find_glyph MiKTeX::KPSE::FindGlyph
#else
#  define kpse_find_glyph miktex_kpse_find_glyph
#endif

#define kpse_find_ofm(name) miktex_kpse_find_file(name, kpse_ofm_format, 1)

#define kpse_find_pict(name) miktex_kpse_find_file(name, kpse_pict_format, 1)

#define kpse_find_pk(font_name, dpi, glyph_file) \
  miktex_kpse_find_glyph (font_name, dpi, kpse_pk_format, glyph_file)

#define kpse_find_vf(name) miktex_kpse_find_file (name, kpse_vf_format, 0)

#define kpse_find_tex(name)  kpse_find_file(name, kpse_tex_format, true)

#define kpse_find_tfm(name) miktex_kpse_find_file(name, kpse_tfm_format, 1)

#if defined(__cplusplus)
#  define kpse_init_prog MiKTeX::KPSE::InitProg
#else
#  define kpse_init_prog miktex_kpse_init_prog
#endif

#if defined(__cplusplus)
#  define kpse_magstep_fix MiKTeX::KPSE::MagStepFix
#else
#  define kpse_magstep_fix miktex_kpse_magstep_fix
#endif

#define kpse_make_tex_discard_errors miktex_kpse_make_tex_discard_errors

#if defined(__cplusplus)
#  define kpse_open_file MiKTeX::KPSE::OpenFile
#else
#  define kpse_open_file miktex_kpse_open_file
#endif

#define kpse_set_progname(argv0) (program_invocation_name = argv0)

#define kpse_set_program_name(argv0, progname) \
  (program_invocation_name = progname)

#if defined(__cplusplus)
#  define kpse_var_value MiKTeX::KPSE::VarValue
#else
#  define kpse_var_value miktex_kpse_var_value
#endif

#define open_input(f_ptr, filefmt, fopen_mode) \
  MiKTeX::Web2C::OpenInput(nameoffile, f_ptr, filefmt, fopen_mode)

#define program_invocation_name miktex_program_invocation_name

#if ! defined(rindex)
#  define rindex strrchr
#endif

#define	secondsandmicros(s, m) MiKTeX::Web2C::GetSecondsAndMicros(&s, &m)

#define versionstring miktex_web2c_version_string

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

#define xfseek MiKTeX::KPSE::FSeek

#define xftell MiKTeX::KPSE::FTell

#if defined(__cplusplus)
#  define xmalloc MiKTeX::KPSE::Malloc
#else
#  define xmalloc miktex_malloc
#endif

#if defined(__cplusplus)
#  define xputenv MiKTeX::Core::Utils::SetEnvironmentString
#else
#  define xputenv miktex_putenv
#endif

#if defined(__cplusplus)
#  define xrealloc MiKTeX::KPSE::Realloc
#else
#  define xrealloc miktex_realloc
#endif

#if defined(__cplusplus)
#  define xstrdup MiKTeX::KPSE::StrDup
#else
#  define xstrdup miktex_strdup
#endif

#define xmallocarray(type,size) ((type*)xmalloc((size+1)*sizeof(type)))
#define xreallocarray(ptr,type,size) ((type*)xrealloc(ptr,(size+1)*sizeof(type)))

#define recorder_record_input(fname)
#define recorder_record_output(fname)

/* _________________________________________________________________________

   Basic Types
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEX_BEGIN_EXTERN_C_BLOCK
#endif

typedef MIKTEXCHAR * string;
typedef const MIKTEXCHAR * const_string;

#if ! defined(NO_BOOLEAN)
#  if defined(__cplusplus)
#    define boolean bool
#  else
#    if ! defined(false) && ! defined(true)
typedef enum { false = 0, true = 1 } MIKTEX_TEXK_BOOLEAN;
#    endif
#    define boolean unsigned char
#  endif
#endif

typedef int integer;

#if defined(__cplusplus)
MIKTEX_END_EXTERN_C_BLOCK
#endif

/* _________________________________________________________________________

   Kpathsea Types
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEX_BEGIN_EXTERN_C_BLOCK
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

typedef enum
{
  kpse_glyph_source_normal,
  kpse_glyph_source_alias,
  kpse_glyph_source_maketex,
  kpse_glyph_source_fallback
} kpse_glyph_source_type;

typedef struct
{
  MIKTEXCHAR *			name;
  unsigned			dpi;
  kpse_file_format_type		format;
  kpse_glyph_source_type	source;
} kpse_glyph_file_type;		

#if defined(__cplusplus)
MIKTEX_END_EXTERN_C_BLOCK
#endif

/* _________________________________________________________________________

   Prototypes (KPSE Namespace)
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEXKPSE_BEGIN_NAMESPACE;

MIKTEXKPSAPI(const MIKTEXCHAR *)
BaseName (/*[in]*/ const MIKTEXCHAR * lpszFileName);

MIKTEXKPSAPI(int)
BitmapTolerance (/*[in]*/ double	dpi1,
		 /*[in]*/ double	dpi2);

MIKTEXKPSAPI(void)
FClose (/*[in]*/ FILE *			stream,
	/*[in]*/ const MIKTEXCHAR *	fileName);

MIKTEXKPSAPI(MIKTEXCHAR *)
FindFile (/*[in]*/ const MIKTEXCHAR *		lpszFileName,
	  /*[in]*/ kpse_file_format_type	format,
	  /*[in]*/ int				mustExist);

MIKTEXKPSAPI(MIKTEXCHAR *)
FindGlyph (/*[in]*/ MIKTEXCHAR *		lpszFontName,
	   /*[in]*/ unsigned			dpi,
	   /*[in]*/ kpse_file_format_type	format,
	   /*[out]*/ kpse_glyph_file_type *	glyph_file);

MIKTEXKPSAPI(MIKTEXCHAR *)
FindSuffix (const MIKTEXCHAR * lpszPath);

MIKTEXKPSAPI(FILE *)
FOpen (/*[in]*/ const MIKTEXCHAR *	lpszName,
       /*[in]*/ const MIKTEXCHAR *	lpszMode);

MIKTEXKPSAPI(FILE *)
TryFOpen (/*[in]*/ const MIKTEXCHAR *	lpszName,
	  /*[in]*/ const MIKTEXCHAR *	lpszMode);

MIKTEXKPSAPI(int)
FSeek (/*[in]*/ FILE *			stream,
       /*[in]*/ long			offset,
       /*[in]*/ int			where,
       /*[in]*/ const MIKTEXCHAR *	lpszFileName);

MIKTEXKPSAPI(long)
FTell (/*[in]*/ FILE *			stream,
       /*[in]*/ const MIKTEXCHAR *	lpszFileName);

MIKTEXKPSAPI(void)
InitProg (/*[in]*/ const MIKTEXCHAR *	lpszPrefix,
	  /*[in]*/ unsigned		dpi,
	  /*[in]*/ const MIKTEXCHAR *	lpszMode,
	  /*[in]*/ const MIKTEXCHAR *	lpszFallback);

MIKTEXKPSAPI(unsigned)
MagStepFix (/*[in]*/ unsigned	dpi,
	    /*[in]*/ unsigned	bdpi,
	    /*[out]*/ int *	pRet);

MIKTEXKPSAPI(void *)
Malloc (/*[in]*/ size_t size);

MIKTEXKPSAPI(FILE *)
OpenFile (/*[in]*/ const MIKTEXCHAR *		lpszFileName,
	  /*[in]*/ kpse_file_format_type	format);

MIKTEXKPSAPI(void)
PutEnv (/*[in]*/ const MIKTEXCHAR * lpszVarName,
	/*[in]*/ const MIKTEXCHAR * lpszValue);

MIKTEXKPSAPI(void *)
Realloc (/*[in]*/ void *	ptr,
	 /*[in]*/ size_t	size);

MIKTEXKPSAPI(MIKTEXCHAR *)
StrDup (/*[in]*/ const MIKTEXCHAR *	lpsz);

MIKTEXKPSAPI(MIKTEXCHAR *)
VarValue (/*[in]*/ const MIKTEXCHAR * lpszVarName);

MIKTEXKPSE_END_NAMESPACE;
#endif

/* _________________________________________________________________________

   Prototypes (Web2C Namespace)
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEXWEB2C_BEGIN_NAMESPACE;

MIKTEXKPSAPI(void)
GetSecondsAndMicros (/*[out]*/ int * pSeconds,
		     /*[out]*/ int * pMicros);

MIKTEXKPSAPI(int)
OpenInput (/*[in]*/ const MIKTEXCHAR *		lpszFileName,
	   /*[in]*/ FILE **			ppfile,
	   /*[in]*/ kpse_file_format_type	format,
	   /*[in]*/ const MIKTEXCHAR *		lpszMode);

MIKTEXWEB2C_END_NAMESPACE;
#endif

/* _________________________________________________________________________

   Prototypes (C)
   _________________________________________________________________________ */

#if defined(__cplusplus)
MIKTEX_BEGIN_EXTERN_C_BLOCK
#endif

MIKTEXKPSAPI(const MIKTEXCHAR *)
miktex_basename (/*[in]*/ const MIKTEXCHAR * lpszFileName);

MIKTEXKPSAPI(void)
miktex_fclose (/*[in]*/ FILE *			stream,
	       /*[in]*/ const MIKTEXCHAR *	fileName);

MIKTEXKPSAPI(FILE *)
miktex_fopen (/*[in]*/ const MIKTEXCHAR *	lpszName,
	      /*[in]*/ const MIKTEXCHAR *	lpszMode);

MIKTEXKPSAPI(int)
miktex_kpse_bitmap_tolerance (/*[in]*/ double	dpi1,
			      /*[in]*/ double	dpi2);

MIKTEXKPSAPI(MIKTEXCHAR *)
miktex_kpse_find_file (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
		       /*[in]*/ kpse_file_format_type	format,
		       /*[in]*/ int			mustExist);

MIKTEXKPSAPI(MIKTEXCHAR *)
miktex_kpse_find_glyph (/*[in]*/ MIKTEXCHAR *			lpszFontName,
			/*[in]*/ unsigned			dpi,
			/*[in]*/ kpse_file_format_type		format,
			/*[out]*/ kpse_glyph_file_type *	glyph_file);

MIKTEXKPSAPI(void)
miktex_kpse_init_prog (/*[in]*/ const MIKTEXCHAR *	lpszPrefix,
		       /*[in]*/ unsigned		dpi,
		       /*[in]*/ const MIKTEXCHAR *	lpszMode,
		       /*[in]*/ const MIKTEXCHAR *	lpszFallback);

MIKTEXKPSAPI(unsigned)
miktex_kpse_magstep_fix (/*[in]*/ unsigned	dpi,
			 /*[in]*/ unsigned	bdpi,
			 /*[out]*/ int *	pRet);

MIKTEXKPSAPI(FILE *)
miktex_kpse_open_file (/*[in]*/ const MIKTEXCHAR *	lpszFileName,
		       /*[in]*/ kpse_file_format_type	format);

MIKTEXKPSAPI(MIKTEXCHAR *)
miktex_kpse_var_value (/*[in]*/ const MIKTEXCHAR * lpszVarName);

MIKTEXKPSAPI(LPVOID)
miktex_malloc (/*[in]*/ size_t size);

MIKTEXKPSAPI(LPVOID)
miktex_realloc (/*[in]*/ void *	ptr,
		/*[in]*/ size_t	size);

MIKTEXKPSAPI(void)
miktex_putenv (/*[in]*/ const MIKTEXCHAR * lpszVarName,
	       /*[in]*/ const MIKTEXCHAR * lpszValue);

MIKTEXKPSAPI(MIKTEXCHAR *)
miktex_strdup (/*[in]*/ const MIKTEXCHAR * s);

MIKTEXKPSAPI(int)
miktex_strcasecmp (/*[in]*/ const MIKTEXCHAR * lpsz1,
		   /*[in]*/ const MIKTEXCHAR * lpsz2);

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
MIKTEXKPSDATA(MIKTEXCHAR *)
miktex_kpse_bug_address;

extern
MIKTEXKPSDATA(const MIKTEXCHAR *)
miktex_kpse_fallback_resolutions_string;

extern
MIKTEXKPSDATA(int)
miktex_kpse_make_tex_discard_errors;

extern
MIKTEXKPSDATA(MIKTEXCHAR *)
miktex_kpathsea_version_string;

extern
MIKTEXKPSDATA(MIKTEXCHAR *)
miktex_web2c_version_string;

extern
MIKTEXKPSDATA(MIKTEXCHAR *)
miktex_program_invocation_name;

extern
MIKTEXKPSDATA(unsigned)
miktex_kpathsea_debug;

#if defined(__cplusplus)
MIKTEX_END_EXTERN_C_BLOCK
#endif

/* _________________________________________________________________________ */

#undef MIKTEX_BEGIN_EXTERN_C_BLOCK
#undef MIKTEX_END_EXTERN_C_BLOCK
