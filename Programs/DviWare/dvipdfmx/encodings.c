/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/encodings.c,v 1.3 2005/07/03 20:02:28 csc Exp $
    
    This is dvipdfmx, an eXtended version of dvipdfm by Mark A. Wicks.

    Copyright (C) 2002 by Jin-Hwan Cho and Shunsaku Hirata,
    the dvipdfmx project team <dvipdfmx@project.ktug.or.kr>
    
    Copyright (C) 1998, 1999 by Mark A. Wicks <mwicks@kettering.edu>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*/

#include <string.h>
#include <stdio.h>

#include "mem.h"
#include "error.h"
#include "mfileio.h"
#include "pdfparse.h"
#include "dpxutil.h"

#include "pdfobj.h"
#include "encodings.h"

#define _ENCODINGS_C_
#include "asl_charset.h"
#include "encodings_p.h"
#undef  _ENCODINGS_C_


#define ENCODING_DEBUG_STR "ENC"
#define ENCODING_DEBUG     3

static FILE    *open_enc_file  (const char *filename);
static int      is_ASL_charset (char **encoding);

static pdf_obj *make_encoding_differences (char **encoding, char **baseenc);

static unsigned char __verbose = 0;

void
Encoding_set_verbose (void)
{
  __verbose++;
}

/*
 * ident:  File name, e.g., 8a.enc.
 * name:   Name of encoding, StandardEncoding, TeXBase1Encoding, ...
 * glyphs: List of encoded glyphs (name).
 * res:    PDF dictionary object representing encoding.
 * ref:    Indirect reference of dict.
 */

/*
 * flags:
 *   IS_PREDEFINED:
 *     Encoding is one of the MacRomanEncoding, MacExpertEncoding, and
 *     WinAnsiEncoding.
 *   IS_ASL_CHARSET:
 *     Encoded glyphs are only form Adobe Standard Latin Character Set.
 *     Fonts only uses encodings of this flag set can be "nonsymbolic".
 */
#define FLAG_IS_PREDEFINED  (1 << 0)
#define FLAG_IS_ASL_CHARSET (1 << 1)

struct Encoding
{
  char   *ident;
  char   *name;
  int     flags;
  char   *glyphs[256];
  /* PDF Objects */
  pdf_obj *res;
  pdf_obj *ref;
};

Encoding *
Encoding_new (void)
{
  Encoding *encoding;

  encoding = NEW(1, struct Encoding);
  encoding->ident  = NULL;
  encoding->name   = NULL;
  encoding->flags  = 0;
  memset(encoding->glyphs, 0, 256*sizeof(char *));
  encoding->res = NULL;
  encoding->ref = NULL;

  return encoding;
}

void
Encoding_flush (Encoding *encoding)
{
  if (encoding) {
    if (encoding->res) pdf_release_obj(encoding->res);
    if (encoding->ref) pdf_release_obj(encoding->ref);
    encoding->res   = NULL;
    encoding->ref   = NULL;
    encoding->flags = 0;
  }
}

void
Encoding_release (Encoding *encoding)
{
  if (encoding) {
    int code;
    if (encoding->ref) ERROR("%s: Object not flushed.", ENCODING_DEBUG_STR);
    if (encoding->res) pdf_release_obj(encoding->res);
    if (encoding->ident) RELEASE(encoding->ident);
    if (encoding->name)  RELEASE(encoding->name);
    for (code = 0; code < 256; code++)
      if (encoding->glyphs[code])
	RELEASE(encoding->glyphs[code]);
    RELEASE(encoding);
  }
}

/*
 * The original dvipdfm describes as:
 *
 *  Some software doesn't like BaseEncoding key (e.g., FastLane) 
 *  so this code is commented out for the moment.  It may reemerge in the
 *  future
 *
 * and the line for BaseEncoding is commented out.
 *
 * I'm not sure why this happens. But maybe BaseEncoding key causes problems
 * when the font is Symbol font or TrueType font.
 */

void
Encoding_set_encoding (Encoding *encoding, char **encoding_vec, char *base_encname)
{
  char   **base_encvec = NULL;
  pdf_obj *differences;

  ASSERT(encoding);
  ASSERT(encoding_vec);

  if (encoding->ref) {
    WARN("%s: Object already have a label... flushing", ENCODING_DEBUG_STR);
    Encoding_flush(encoding);
  }

  encoding->flags = 0;
  if (!encoding->res) {
    encoding->res = pdf_new_dict();
    pdf_add_dict(encoding->res, pdf_new_name("Type"), pdf_new_name("Encoding"));
  }

  {
    int code;

    for (code = 0; code < 256; code++) {
      if (encoding->glyphs[code])
	RELEASE(encoding->glyphs[code]);
      if (encoding_vec[code])
	encoding->glyphs[code] = strdup(encoding_vec[code]);
      else
	encoding->glyphs[code] = NULL;
    }
  }

  if (base_encname) {
    if (!strcmp(base_encname, "WinAnsiEncoding"))
      base_encvec = (char **) WinAnsiEncoding;
    else if (!strcmp(base_encname, "MacRomanEncoding"))
      base_encvec = (char **) MacRomanEncoding;
    else if (!strcmp(base_encname, "MacExpertEncoding"))
      base_encvec = (char **) MacExpertEncoding;
    else
      ERROR("%s: Unknown encoding %s.", ENCODING_DEBUG_STR, base_encname);
  } else
    base_encvec = NULL;

  if (is_ASL_charset(encoding->glyphs)) {
    encoding->flags |= FLAG_IS_ASL_CHARSET;
    /* Dvipdfmx default setting. */
    if (!base_encname) {
      base_encvec  = (char **) WinAnsiEncoding; 
      base_encname = "WinAnsiEncoding";
    }
  }

  differences = make_encoding_differences(encoding->glyphs, base_encvec);
  if (base_encname)
    pdf_add_dict(encoding->res, pdf_new_name("BaseEncoding"), pdf_new_name(base_encname));
  pdf_add_dict(encoding->res, pdf_new_name("Differences"),  differences);

  return;
}

char **
Encoding_get_encoding (Encoding *encoding)
{
  ASSERT(encoding);

  return encoding->glyphs;
}

pdf_obj *
Encoding_get_resource (Encoding *encoding)
{
  ASSERT(encoding);

  if (!encoding->ref)
    encoding->ref = pdf_ref_obj(encoding->res);

  return pdf_link_obj(encoding->ref);
}

int
Encoding_is_predefined (Encoding *encoding)
{
  ASSERT(encoding);

  return (encoding->flags & FLAG_IS_PREDEFINED) ? 1 : 0;
}

int
Encoding_is_ASL_charset (Encoding *encoding)
{
  ASSERT(encoding);

  return (encoding->flags & FLAG_IS_ASL_CHARSET) ? 1 : 0;
}

void
Encoding_set_name (Encoding *encoding, char *name)
{
  ASSERT(encoding);

  if (encoding->name)
    RELEASE(encoding->name);
  if (name)
    encoding->name = strdup(name);
  else
    encoding->name = NULL;
}

char *
Encoding_get_name (Encoding *encoding)
{
  ASSERT(encoding);

  return encoding->name;
}

static int CDECL
glycmp (const void *pv1, const void *pv2)
{
  char *v1, *v2;

  v1 = (char *) pv1;
  v2 = *((char **) pv2);

  return strcmp(v1, v2);
}

static int
is_ASL_charset (char **encoding)
{
  int code;

  for (code = 0; code < 256; code++) {
    if (encoding[code] && strcmp(encoding[code], ".notdef")) {
      if (!bsearch(encoding[code], ASL_Charset, ASL_CHARSET_MAX, sizeof(char *), glycmp))
	return 0;
    }
  }

  return 1;
}

static pdf_obj *
make_encoding_differences (char **encoding, char **baseenc)
{
  pdf_obj *differences = NULL;
  int      code;
  int      skipping = 1;

  ASSERT(encoding);
  /*
   *  Write all entries (except .notdef) if baseenc is unknown.
   */
  differences = pdf_new_array();
  for (code = 0; code < 256; code++) {
    if (!encoding[code] || !strcmp(".notdef", encoding[code]))
      skipping = 1;
    else if (!baseenc || !baseenc[code] ||
	     strcmp(baseenc[code], encoding[code])) {
      /*
       * Difference found.
       */
      if (skipping)
	pdf_add_array(differences, pdf_new_number(code));
      pdf_add_array(differences, pdf_new_name(encoding[code]));
      skipping = 0;
    } else
      skipping = 1;
  }

  return differences;
}

static int
Encoding_read (Encoding *encoding, const char *filename)
{
  FILE    *fp;
  char    *encoding_name;
  pdf_obj *encoding_array = NULL;
  char    *wbuf, *start, *end, *vec[256];
  int      code, fsize;

  if (!encoding || !filename)
    return -1;

  fp = open_enc_file(filename);
  if (!fp)
    return -1;
  /*
   * file_size do seek_end witout saving current position and
   * do rewind.
   */
  fsize = file_size(fp);

  wbuf = NEW(fsize+1, char); 
  fread(wbuf, sizeof(char), fsize, fp);
  MFCLOSE(fp);

  start = wbuf; end = wbuf+fsize; *end  = '\0';
  skip_white(&start, end);
  /*
   * Skip comment lines.
   */
  while (start < end && *start == '%') {
    skip_line (&start, end);
    skip_white(&start, end);
  }
  if (*start == '/') {
    pdf_obj *tmp = parse_pdf_name(&start, end);
    encoding_name = strdup(pdf_name_value(tmp));
    pdf_release_obj(tmp);
  } else
    encoding_name = NULL;
  skip_white(&start, end);
  encoding_array = parse_pdf_array(&start, end);
  RELEASE(wbuf);
  if (!encoding_array)
    return -1;

  if (encoding_name) {
    Encoding_set_name(encoding, encoding_name);
    RELEASE(encoding_name);
  }

  for (code = 0; code < 256; code++)
    vec[code] = pdf_name_value(pdf_get_array(encoding_array, code));
  Encoding_set_encoding(encoding, vec, NULL);
  pdf_release_obj(encoding_array);

  return 0;
}

#define CHECK_ID(n) do {\
                        if (! __cache)\
                           ERROR("%s: Not initialized.", ENCODING_DEBUG_STR);\
                        if ((n) < 0 || (n) >= __cache->num)\
                           ERROR("%s: Invalid ID %d", ENCODING_DEBUG_STR, (n));\
                    } while (0)

#define CACHE_ALLOC_SIZE 16u

struct EncodingCache
{
  int num;
  int max;
  struct Encoding **encodings;
};
static struct EncodingCache *__cache;

void
Encoding_cache_init (void)
{
  if (__cache)
    ERROR("%s: Already initialized.", ENCODING_DEBUG_STR);

  if (__verbose > ENCODING_DEBUG)
    MESG("\n%s: Initialize.\n", ENCODING_DEBUG_STR);

  __cache = NEW(1, struct EncodingCache);
  __cache->max = MAX(CACHE_ALLOC_SIZE, 3);
  __cache->num = 0;
  __cache->encodings = NEW(__cache->max, struct Encoding *);

  /*
   * PDF Predefined Encodings
   */
  {
    Encoding *encoding;
    int       code;

    /*
     * strdup ...
     */
    __cache->encodings[0] = encoding = Encoding_new();
    encoding->ident = strdup("WinAnsiEncoding");
    encoding->name  = strdup("WinAnsiEncoding");
    encoding->flags = (FLAG_IS_PREDEFINED|FLAG_IS_ASL_CHARSET);
    encoding->res   = pdf_new_name("WinAnsiEncoding");
    for (code = 0; code < 256; code++)
      encoding->glyphs[code] = strdup(WinAnsiEncoding[code]);

    __cache->encodings[1] = encoding = Encoding_new();
    encoding->ident = strdup("MacRomanEncoding");
    encoding->name  = strdup("MacRomanEncoding");
    encoding->flags = (FLAG_IS_PREDEFINED|FLAG_IS_ASL_CHARSET);
    encoding->res   = pdf_new_name("MacRomanEncoding");
    for (code = 0; code < 256; code++)
      encoding->glyphs[code] = strdup(MacRomanEncoding[code]);

    __cache->encodings[2] = encoding = Encoding_new();
    encoding->ident = strdup("MacExpertEncoding");
    encoding->name  = strdup("MacExpertEncoding");
    encoding->flags = FLAG_IS_PREDEFINED;
    encoding->res   = pdf_new_name("MacExpertEncoding");
    for (code = 0; code < 256; code++)
      encoding->glyphs[code] = strdup(MacExpertEncoding[code]);

    __cache->num = 3;
  }
}

void
Encoding_cache_close (void)
{
  if (__cache) {
    int id;
    for (id = 0; id < __cache->num; id++) {
      Encoding *encoding = __cache->encodings[id];
      if (encoding) {
	Encoding_flush(encoding);
	Encoding_release(encoding);
      }
    }
    RELEASE(__cache->encodings);
    RELEASE(__cache);
    __cache = NULL;
  }

  if (__verbose > ENCODING_DEBUG)
    MESG("%s: Close\n", ENCODING_DEBUG_STR);
}

Encoding *
Encoding_cache_get (int encoding_id)
{
  CHECK_ID(encoding_id);
  return __cache->encodings[encoding_id];
}

int
Encoding_cache_find (const char *enc_name)
{
  int       enc_id;
  Encoding *encoding;

  if (!__cache)
    Encoding_cache_init();
  ASSERT(__cache);

  for (enc_id = 0; enc_id < __cache->num; enc_id++) {
    encoding = __cache->encodings[enc_id];
    ASSERT(encoding);
    if (!strcmp(enc_name, encoding->ident))
      return enc_id;
    else if (!strcmp(enc_name, Encoding_get_name(encoding)))
      return enc_id;
  }

  if (__verbose > ENCODING_DEBUG)
    MESG("\n%s: Encoding \"%s\" not found in cache (total=%d).\n",
	 ENCODING_DEBUG_STR, enc_name, __cache->num);

  if (__verbose)
    MESG("(ENC:%s", enc_name);

  enc_id   = __cache->num;
  encoding = Encoding_new();
  /*
   * ident should be in different place...
   */
  encoding->ident = strdup(enc_name);
  if (Encoding_read(encoding, enc_name) < 0) {
    Encoding_release(encoding);
    ERROR("%s: Could not read encoding file \"%s\".", ENCODING_DEBUG_STR, enc_name);
    return -1;
  }

  if (__verbose > 1 && Encoding_get_name(encoding))
    MESG("[%s]", Encoding_get_name(encoding));

  if (__cache->num >= __cache->max) {
    __cache->max      += CACHE_ALLOC_SIZE;
    __cache->encodings = RENEW(__cache->encodings, __cache->max, struct Encoding *);
  }
  __cache->encodings[enc_id] = encoding;
  (__cache->num)++;

  if (__verbose) MESG(")");

  return enc_id;
}


/******************************* COMPATIBILITY ******************************/

void
encoding_set_verbose(void)
{
  Encoding_set_verbose();
}

int
get_encoding (const char *enc_name)
{
  return Encoding_cache_find(enc_name);
}

pdf_obj *
encoding_ref (int encoding_id) 
{
  return Encoding_get_resource(Encoding_cache_get(encoding_id));
}

void
encoding_flush_all (void) 
{
  Encoding_cache_close();
}

char *
encoding_glyph (int encoding_id, unsigned code) 
{
  Encoding *encoding;

  encoding = Encoding_cache_get(encoding_id);

  return encoding->glyphs[code];
}


/*
 * This should be removed from this file.
 */

static FILE *
open_enc_file (const char *filename)
{
  char *fullname, *altname;

  altname = NEW(strlen(filename)+5, char);
  sprintf(altname, "%s.enc", filename, ".enc");
#ifdef MIKTEX
  fullname = work_buffer;
  if (!miktex_find_psheader_file(filename, fullname) &&
      !miktex_find_app_input_file("dvipdfm", filename, fullname) &&
      !miktex_find_psheader_file(altname, fullname) &&
      !miktex_find_app_input_file("dvipdfm", altname, fullname)) {
    ERROR("Could not find encoding file: %s", filename);
  }
#else
  if ((fullname = kpse_find_file(filename,
				 kpse_tex_ps_header_format, 1)) == NULL &&
      (fullname = kpse_find_file(filename,
				 kpse_program_text_format,  1)) == NULL &&
      (fullname = kpse_find_file(altname,
				 kpse_tex_ps_header_format, 1)) == NULL &&
      (fullname = kpse_find_file(altname,
				 kpse_program_text_format,  1)) == NULL)
    ERROR("Could not find encoding file: %s", filename) ;
#endif
  RELEASE(altname);

  if (__verbose > 1)
    MESG("[%s]", __verbose > 1 ? fullname : filename);

  return MFOPEN(fullname, FOPEN_R_MODE);
}

