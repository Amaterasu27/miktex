/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/dpxfile.c,v 1.4 2005/07/03 20:02:28 csc Exp $
    
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

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "system.h"
#include "mem.h"
#include "error.h"
#include "dpxutil.h"
#include "mfileio.h"

#include "dpxfile.h"


static int verbose = 0;

void
dpx_file_set_verbose (void)
{
  verbose++;
}

/* For testing MIKTEX enabled compilation */
#if defined(TESTCOMPILE) && !defined(MIKTEX)
#  define MIKTEX        1
#  define PATH_SEP_CHR  '/'
#  define _MAX_PATH     256

static int
miktex_get_acrobat_font_dir (char *buf)
{
  strcpy(buf, "/usr/share/ghostscript/Resource/Font/");
  return  1;
}

static int
miktex_find_file (const char *filename, const char *dirlist, char *buf)
{
  int    r = 0;
  char  *fqpn;

  fqpn = kpse_path_search(dirlist, filename, 0);
  if (!fqpn)
    return  0;
  if (strlen(fqpn) > _MAX_PATH)
    r = 0;
  else {
    strcpy(buf, fqpn);
    r = 1;
  }
  RELEASE(fqpn);

  return  r;
}

static int
miktex_find_app_input_file (const char *progname, const char *filename, char *buf)
{
  int    r = 0;
  char  *fqpn;

  kpse_reset_program_name(progname);
  fqpn = kpse_find_file  (filename, kpse_program_text_format, false);
  kpse_reset_program_name("dvipdfm");

  if (!fqpn)
    return  0;
  if (strlen(fqpn) > _MAX_PATH)
    r = 0;
  else {
    strcpy(buf, fqpn);
    r = 1;
  }
  RELEASE(fqpn);

  return  r;
}

static int
miktex_find_psheader_file (const char *filename, char *buf)
{
  int    r;
  char  *fqpn;

  fqpn = kpse_find_file(filename, kpse_tex_ps_header_format, 0);

  if (!fqpn)
    return  0;
  if (strlen(fqpn) > _MAX_PATH)
    r = 0;
  else {
    strcpy(buf, fqpn);
    r = 1;
  }
  RELEASE(fqpn);

  return  r; 
}

#endif /* TESTCOMPILE */

#ifdef  MIKTEX
#ifndef PATH_SEP_CHR
#  define PATH_SEP_CHR '\\'
#endif
static char  _tmpbuf[_MAX_PATH+1];
#endif /* MIKTEX */

#if  1
static char *
ensuresuffix (const char *basename, const char *sfx)
{
  char  *q, *p;

  p = NEW(strlen(basename) + strlen(sfx) + 1, char);
  strcpy(p, basename);
  q = strrchr(p, '.');
  if (!q && sfx[0])
    strcat(p, sfx);

  return  p;
}
#endif

#ifdef  MIKTEX
static char *
dpx_find__app__xyz (const char *filename,
                    const char *suffix, int is_text)
{
  char  *fqpn = NULL;
  int    r;
  char  *q;

  q = ensuresuffix(filename, suffix);
  r = miktex_find_app_input_file("dvipdfm", q, _tmpbuf);
  if (!r && strcmp(q, filename))
    r = miktex_find_app_input_file("dvipdfm", filename, _tmpbuf);
  if (r) {
    fqpn = NEW(strlen(_tmpbuf) + 1, char);
    strcpy(fqpn, _tmpbuf);
  }
  RELEASE(q);

  return  fqpn;
}

static char *
dpx_foolsearch (const char  *foolname,
                const char  *filename,
                int          is_text)
{
  char  *fqpn = NULL;
  int    r;

  r = miktex_find_app_input_file(foolname, filename, _tmpbuf);
  if (r) {
    fqpn = NEW(strlen(_tmpbuf) + 1, char);
    strcpy(fqpn, _tmpbuf);
  }

  return  fqpn;
}
#else /* TEXK */
#if  defined(__TDS_VERSION__) && __TDS_VERSION__ >= 0x200406L
#  define TDS11DOC "http://www.tug.org/ftp/tex/tds-1.1/tds.html#Fonts"
static void
insistupdate (const char      *filename,
              const char      *fqpn,
              const char      *foolname,
              kpse_file_format_type foolformat,
              kpse_file_format_type realformat)
{
  kpse_format_info_type *fif;
  kpse_format_info_type *fir;
  if (verbose < 1)
    return;
  fif = &kpse_format_info[foolformat];
  fir = &kpse_format_info[realformat];
  WARN("File name=\"%s\" format=\"%s\" might be found in different location than I expected:",
       filename, fir->type);
  WARN(">>   %s", fqpn);
  WARN(">> Please adjust your TEXMF as conformant with:");
  WARN(">>   " TDS11DOC);
  WARN(">> I searched it with fooling kpathsea as progname=\"%s\" format=\"%s\".",
       foolname, fif->type);
  WARN(">> Default search path for this format file is:");
  WARN(">>   %s", fir->default_path);
  WARN(">> Please read \"README\" file.");
}
#endif /* TDS 1.1 */

static char *
dpx_find__app__xyz (const char *filename,
                    const char *suffix, int is_text)
{
  char  *fqpn = NULL;
  char  *q;

  q    = ensuresuffix(filename, suffix);
  fqpn = kpse_find_file(q,
                         (is_text ?
                            kpse_program_text_format : kpse_program_binary_format), 0);
  if (!fqpn && strcmp(q, filename))
    fqpn = kpse_find_file(filename,
                           (is_text ?
                              kpse_program_text_format : kpse_program_binary_format), 0);   
  RELEASE(q);

  return  fqpn;
}

static char *
dpx_foolsearch (const char  *foolname,
                const char  *filename,
                int          is_text)
{
  char  *fqpn = NULL;

  kpse_reset_program_name(foolname);
  fqpn = kpse_find_file  (filename,
                          (is_text ?
                              kpse_program_text_format :
                              kpse_program_binary_format),
                          false);
  kpse_reset_program_name("dvipdfm");

  return  fqpn;
}
#endif /* MIKTEX */

static char *dpx_find_fontmap_file  (const char *filename);
static char *dpx_find_sfd_file      (const char *filename);
static char *dpx_find_cmap_file     (const char *filename);
static char *dpx_find_enc_file      (const char *filename);
static char *dpx_find_opentype_file (const char *filename);
static char *dpx_find_truetype_file (const char *filename);
static char *dpx_find_type1_file    (const char *filename);
static char *dpx_find_iccp_file     (const char *filename);

FILE *
dpx_open_file (const char *filename, int type)
{
  FILE  *fp   = NULL;
  char  *fqpn = NULL;

  switch (type) {
  case DPX_RES_TYPE_FONTMAP:
    fqpn = dpx_find_fontmap_file(filename);
    break;
  case DPX_RES_TYPE_T1FONT:
    fqpn = dpx_find_type1_file(filename);
    break;
  case DPX_RES_TYPE_TTFONT:
    fqpn = dpx_find_truetype_file(filename);
    break;
  case DPX_RES_TYPE_OTFONT:
    fqpn = dpx_find_opentype_file(filename);
    break;
  case DPX_RES_TYPE_PKFONT:
    break;
  case DPX_RES_TYPE_CMAP:
    fqpn = dpx_find_cmap_file(filename);
    break;
  case DPX_RES_TYPE_ENC:
    fqpn = dpx_find_enc_file(filename);
    break;
  case DPX_RES_TYPE_SFD:
    fqpn = dpx_find_sfd_file(filename);
    break;
  case DPX_RES_TYPE_ICCPROFILE:
    fqpn = dpx_find_iccp_file(filename);
    break;
  default:
    ERROR("Unknown resource type: %d", type);
    break;
  }
  if (fqpn) {
    fp = MFOPEN(fqpn, FOPEN_RBIN_MODE);
    RELEASE(fqpn);
  }

  return  fp;
}

static char *
dpx_find_iccp_file (const char *filename)
{
  char  *fqpn = NULL;

  fqpn = dpx_find__app__xyz(filename, "", 0);
  if (fqpn || strrchr(filename, '.'))
    return  fqpn;

  fqpn = dpx_find__app__xyz(filename, ".icc", 0);
  if (fqpn)
    return  fqpn;

  fqpn = dpx_find__app__xyz(filename, ".icm", 0);

  return  fqpn;
}


static char *
dpx_find_fontmap_file (const char *filename)
{
  char  *fqpn = NULL;
  char  *q;

  q = ensuresuffix(filename, ".map");
#ifdef  MIKTEX
  fqpn = dpx_find__app__xyz(q, ".map", 1);
#else /* !MIKTEX */
  fqpn = kpse_find_file(q, kpse_fontmap_format, 0);
  if (!fqpn) {
    fqpn = dpx_find__app__xyz(q, ".map", 1);
#if  defined(__TDS_VERSION__) && __TDS_VERSION__ >= 0x200406L
    if (fqpn)
      insistupdate(q, fqpn, "dvipdfm",
                   kpse_program_text_format, kpse_fontmap_format); 
#endif
  }
#endif /* MIKETEX */
  RELEASE(q);

  return  fqpn;
}


static char *
dpx_find_cmap_file (const char *filename)
{
  char  *fqpn = NULL;
  static const char *fools[] = {
    "cmap", "dvipdfm", NULL
  };
  int    i;

#if  defined(MIKTEX)
  /* Find in Acrobat's Resource/CMap dir */
  {
    char  _acrodir[_MAX_PATH+1];
    char  *q;
    int    r;

    memset(_acrodir, 0, _MAX_PATH+1);
    r = miktex_get_acrobat_font_dir(_acrodir);
    if (r &&
        strlen(_acrodir) > strlen("Font")) {
      /* ....\Font\ */
      q = strrchr(_acrodir, PATH_SEP_CHR);
      if (q && q[1] == '\0')
        q[0] = '\0';
      q = strrchr(_acrodir, PATH_SEP_CHR);
      if (q && !strcmp(q + 1, "Font")) {
        sprintf(q, "%cCMap%c", PATH_SEP_CHR, PATH_SEP_CHR);
        r = miktex_find_file(filename, _acrodir, _tmpbuf);
        if (r) {
          fqpn = NEW(strlen(_tmpbuf) + 1, char);
          strcpy(fqpn, _tmpbuf);
        }
      }
    }
    memset(_tmpbuf, 0, _MAX_PATH+1);
  }
#else
#if  defined(__TDS_VERSION__) && __TDS_VERSION__ >= 0x200406L  
  fqpn = kpse_find_file(filename, kpse_cmap_format, 0); 
#endif  
#endif

  for (i = 0; !fqpn && fools[i]; i++) { 
    fqpn = dpx_foolsearch(fools[i], filename, 1);
#ifndef  MIKTEX
#if  defined(__TDS_VERSION__) && __TDS_VERSION__ >= 0x200406L
    if (fqpn)
      insistupdate(filename, fqpn, fools[i],
                   kpse_program_text_format, kpse_cmap_format); 
#endif
#endif
  }

  return  fqpn;
}

/* Search order:
 *   SFDFONTS (TDS 1.1)
 *   ttf2pk   (text file)
 *   ttf2tfm  (text file)
 *   dvipdfm  (text file)   
 */
static char *
dpx_find_sfd_file (const char *filename)
{
  char  *fqpn = NULL;
  char  *q;
  static const char *fools[] = {
    "ttf2pk", "ttf2tfm", "dvipdfm", NULL
  };
  int    i;

  q    = ensuresuffix(filename, ".sfd");
#ifndef  MIKTEX
#if  defined(__TDS_VERSION__) && __TDS_VERSION__ >= 0x200406L
  fqpn = kpse_find_file(q, kpse_sfd_format, 0);
#endif
#endif /* !MIKTEX */

  for (i = 0; !fqpn && fools[i]; i++) { 
    fqpn = dpx_foolsearch(fools[i], q, 1);
#ifndef  MIKTEX
#if  defined(__TDS_VERSION__) && __TDS_VERSION__ >= 0x200406L
    if (fqpn)
      insistupdate(filename, fqpn, fools[i],
                   kpse_program_text_format, kpse_sfd_format); 
#endif
#endif
  }
  RELEASE(q);

  return  fqpn;
}

static char *
dpx_find_enc_file (const char *filename)
{
  char  *fqpn = NULL;
  char  *q;
  static const char *fools[] = {
    "dvips", "dvipdfm", NULL
  };
  int    i;

  q = ensuresuffix(filename, ".enc");
#ifdef  MIKTEX
  if (miktex_find_psheader_file(q, _tmpbuf)) {
    fqpn = NEW(strlen(_tmpbuf) + 1, char);
    strcpy(fqpn, _tmpbuf);
  }
#else
# if defined(__TDS_VERSION__) && __TDS_VERSION__ >= 0x200406L
  fqpn = kpse_find_file(filename, kpse_enc_format, 0);
# else
  fqpn = kpse_find_file(filename, kpse_tex_ps_header_format, 0);
# endif
#endif /* MIKTEX */

  for (i = 0; !fqpn && fools[i]; i++) { 
    fqpn = dpx_foolsearch(fools[i], q, 1);
#ifndef  MIKTEX
#if  defined(__TDS_VERSION__) && __TDS_VERSION__ >= 0x200406L
    if (fqpn)
      insistupdate(filename, fqpn, fools[i],
                   kpse_program_text_format, kpse_enc_format); 
#endif
#endif
  }
  RELEASE(q);

  return  fqpn;
}

static char *
dpx_find_type1_file (const char *filename)
{
  char  *fqpn = NULL;

  fqpn = kpse_find_file(filename, kpse_type1_format, 0);

  return  fqpn;
}

static char *
dpx_find_truetype_file (const char *filename)
{
  char  *fqpn = NULL;

  fqpn = kpse_find_file(filename, kpse_truetype_format, 0);

  return  fqpn;
}


static char *
dpx_find_opentype_file (const char *filename)
{
  char  *fqpn = NULL;
  char  *q;
  static const char *fools[] = {
    "dvipdfm", NULL
  };
  int    i;

  q = ensuresuffix(filename, ".otf");
#ifndef MIKTEX
#if  defined(__TDS_VERSION__) && __TDS_VERSION__ >= 0x200406L
  fqpn = kpse_find_file(q, kpse_opentype_format, 0);
#endif
#endif

  for (i = 0; !fqpn && fools[i]; i++) {
    fqpn = dpx_foolsearch(fools[i], q, 0);
#ifndef  MIKTEX
#if  defined(__TDS_VERSION__) && __TDS_VERSION__ >= 0x200406L
    if (fqpn)
      insistupdate(filename, fqpn, fools[i],
                   kpse_program_binary_format, kpse_opentype_format); 
#endif
#endif
  }
  RELEASE(q);

  return  fqpn;
}

