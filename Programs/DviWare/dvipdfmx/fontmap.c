/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/fontmap.c,v 1.3 2005/07/03 20:02:28 csc Exp $
    
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

#include "mem.h"
#include "error.h"

#include "dpxfile.h"

#include "pdfparse.h"

/* Hash */
#include "dpxutil.h"

#include "fontmap.h"

#include "subfont.h"

/* CIDFont */
static char *strip_options (const char *map_name, fontmap_opt *opt);

static int verbose = 0;

void
pdf_init_fontmap_record (fontmap_rec *mrec) 
{
  ASSERT(mrec);

  mrec->map_name  = NULL;
  mrec->sfd_name  = NULL;

  mrec->font_name = NULL;
  mrec->enc_name  = NULL;

  mrec->opt.slant  = 0.0;
  mrec->opt.extend = 1.0;
  mrec->opt.flags  = 0;
  mrec->opt.mapc   = 0;

  mrec->opt.design_size = -1.0;

  mrec->opt.otl_tags = NULL;
  mrec->opt.index    = 0;
  mrec->opt.charcoll = NULL;
  mrec->opt.style    = FONTMAP_STYLE_NONE;

  mrec->deleted   = 0;
}

void
pdf_clear_fontmap_record (fontmap_rec *mrec)
{
  ASSERT(mrec);

  if (mrec->map_name)
    RELEASE(mrec->map_name);
  if (mrec->sfd_name)
    RELEASE(mrec->sfd_name);
  if (mrec->enc_name)
    RELEASE(mrec->enc_name);
  if (mrec->font_name)
    RELEASE(mrec->font_name);

  if (mrec->opt.otl_tags)
    RELEASE(mrec->opt.otl_tags);
  if (mrec->opt.charcoll)
    RELEASE(mrec->opt.charcoll);
  pdf_init_fontmap_record(mrec);
}

static void CDECL
hval_free (void *value)
{
  fontmap_rec *mrec;

  mrec = (fontmap_rec *) value;
  pdf_clear_fontmap_record(mrec);
  RELEASE(mrec);
}

#define ENABLE_MAP_RECORD(r) {\
  (r)->deleted = 0;\
}
#define DISABLE_MAP_RECORD(r) {\
  (r)->deleted = 1;\
}

static char *
chop_sfd_name (const char *tex_name, char **sfd_name)
{
  char  *fontname;
  char  *p, *q;
  int    len;

  p = strchr((char *) tex_name, '@');
  if (!p ||
      p[1] == '\0' || p == tex_name) {
    *sfd_name = NULL;
    return NULL;
  }

  p++;
  q = strchr(p, '@');
  if (q[1] != '\0') {
    *sfd_name = NULL;
    return NULL;
  }

  len = strlen(tex_name) - strlen(p) - 1;
  fontname = NEW(len+1, char);
  strncpy(fontname, tex_name, len);
  fontname[len] = '\0';

  len = strlen(p) - 1; /* excludes '@' */
  *sfd_name = NEW(len+1, char);
  strncpy(*sfd_name, p, len);
  (*sfd_name)[len] = '\0';

  return fontname;
}

static void
fill_in_defaults (fontmap_rec *mrec, const char *tex_name)
{
  char *fontname, *sfd_name;

  if (mrec->enc_name && 
      (!strcmp(mrec->enc_name, "default") ||
       !strcmp(mrec->enc_name, "none"))) {
    RELEASE(mrec->enc_name);
    mrec->enc_name = NULL;
  }
  if (mrec->font_name && 
      (!strcmp(mrec->font_name, "default") ||
       !strcmp(mrec->font_name, "none"))) {
    RELEASE(mrec->font_name);
    mrec->font_name = NULL;
  }

  fontname = chop_sfd_name(tex_name, &sfd_name);
  /* We *must* fill font_name either explicitly or by default */
  if (fontname && sfd_name) {
    if (!mrec->font_name) {
      /* In the case of subfonts, the base name (before the character '@')
       * will be used as a font_name by default.
       * Otherwise tex_name will be used as a font_name by default.
       */
      mrec->font_name = fontname ;
    } else {
      RELEASE(fontname);
    }
    mrec->sfd_name  = sfd_name ;
  } else if (!mrec->font_name) {
    mrec->font_name = NEW(strlen(tex_name)+1, char);
    strcpy(mrec->font_name, tex_name);
    mrec->sfd_name  = NULL;
  }

  mrec->map_name = NEW(strlen(tex_name)+1, char);
  strcpy(mrec->map_name, tex_name);

#ifndef WITHOUT_COMPAT
  if (mrec->sfd_name && mrec->enc_name &&
      !mrec->opt.charcoll) {
    if ((!strcmp(mrec->enc_name, "Identity-H") ||
	 !strcmp(mrec->enc_name, "Identity-V"))
	&&
	(strstr(mrec->sfd_name, "Uni")  ||
	 strstr(mrec->sfd_name, "UBig") ||
	 strstr(mrec->sfd_name, "UBg")  ||
	 strstr(mrec->sfd_name, "UGB")  ||
	 strstr(mrec->sfd_name, "UKS")  ||
	 strstr(mrec->sfd_name, "UJIS"))) {
      mrec->opt.charcoll = NEW(strlen("UCS")+1, char);
      strcpy(mrec->opt.charcoll, "UCS");
    }
  }
#endif /* WITHOUT_COMPAT */

  return;
}

static int
fontmap_parse_mapdef (fontmap_rec *mrec,
		      const char *tex_name, char *mdef, char *endptr)
{
  /*
   * Parse record line in map file.  First two fields (after TeX font
   * name) are position specific.  Arguments start at the first token
   * beginning with a  '-'.
   */

  skip_white(&mdef, endptr);
  if (mdef < endptr && mdef[0] != '-') { /* May be NULL */
    mrec->enc_name = parse_ident(&mdef, endptr);
    skip_white(&mdef, endptr);
  }
  if (mdef < endptr && mdef[0] != '-') { /* May be NULL */
    /* parse_val_ident() allows inclusion of '/' */
    mrec->font_name = parse_val_ident(&mdef, endptr);
    skip_white(&mdef, endptr);
  }

  if (mrec->font_name) {
    char  *tmp;

    tmp = strip_options(mrec->font_name, &mrec->opt);
    if (tmp) {
      RELEASE(mrec->font_name);
      mrec->font_name = tmp;
    }
  }

  skip_white(&mdef, endptr);
  /* Parse any remaining arguments */ 
  while (mdef + 1 < endptr && mdef[0] == '-') {
    char mopt, *number;

    mopt = mdef[1];
    switch (mopt) {
    case '%':
      mdef = endptr;
      break;
    case 'i':  /* TTC index */
      mdef += 2;
      skip_white(&mdef, endptr);
      number = parse_number(&mdef, endptr);
      if (!number) {
	WARN("Missing TTC index number...");
	return -1;
      }
      mrec->opt.index = atoi(number);
      if (mrec->opt.index < 0) {
	WARN("Invalid TTC index number: %d", mrec->opt.index);
	mrec->opt.index = 0;
      }
      RELEASE(number);
      break;

    case 's': /* Slant option */
      mdef += 2;
      skip_white(&mdef, endptr);
      number = parse_number(&mdef, endptr);
      if (!number) {
	WARN("Missing slant value...");
	return -1;
      }
      mrec->opt.slant = atof(number);
      RELEASE(number);
      break;
    case 'e': /* Extend option */
      mdef += 2;
      skip_white(&mdef, endptr);
      number = parse_number(&mdef, endptr);
      if (!number) {
	WARN("Missing extend value...");
	return -1;
      }
      mrec->opt.extend = atof(number);
      RELEASE(number);
      break;
    case 'r': /* Remap option */
      mdef += 2;
      skip_white(&mdef, endptr);
      mrec->opt.flags |= FONTMAP_OPT_REMAP;
      break;
    case 'm': /* Map single bytes char 0xab to double byte char 0xcdab  */
      {
	char *nextptr;
	long  v;

	mdef += 2;
	skip_white(&mdef, endptr);
	if (mdef + 4 <= endptr &&
	    mdef[0] == '<' && mdef[3] == '>') {
	  mdef++;
	  v = strtol(mdef, &nextptr, 16);
	  nextptr++;
	} else {
	  v = strtol(mdef, &nextptr, 0);
	}

	if (nextptr == mdef ||
	    (nextptr < endptr && !isspace(nextptr[0]))) {
	  WARN("Invalid value for option \"-m\": -->%s<--", mdef);
	  return -1;
	}
	if (mrec->opt.mapc < 0)
	  mrec->opt.mapc = 0;
	mrec->opt.mapc |= ((v << 8) & 0x0000ff00L);
	mdef = nextptr;
      }
      break;

    case 'w': /* Writing mode (for unicode encoding) */
      if (!mrec->enc_name ||
	  strcmp(mrec->enc_name, "unicode"))
	WARN("Fontmap option \"-w\" meaningless for encoding other than \"unicode\".");
      mdef += 2;
      skip_white(&mdef, endptr);
      number = parse_number(&mdef, endptr);
      if (!number) {
	WARN("Missing wmode value...", tex_name);
	return -1;
      }
      if (atoi(number) == 1)
	mrec->opt.flags |= FONTMAP_OPT_VERT;
      RELEASE(number);
      break;

    default:
      WARN("Unrecognized font map option: -->%s<--", mdef);
      return -1;
      break;
    }
    skip_white(&mdef, endptr);
  }
  fill_in_defaults(mrec, tex_name);

  return 0;
}


void
pdf_fontmap_set_verbose (void)
{
  verbose++;
}

static struct ht_table *fontmap = NULL;

static int
fontmap_invalid (fontmap_rec *mrec)
{
  if (!mrec)
    return 1;
  if (!mrec->map_name)
    return 1;
  if (!mrec->font_name)
    return 1;

  return 0;
}

int
pdf_append_fontmap_record (fontmap_rec *mrec)
{
  fontmap_rec *duplicate;

  if (fontmap_invalid(mrec)) {
    WARN("Invalid fontmap record...");
    return -1;
  }

  duplicate = ht_lookup_table(fontmap,
			      mrec->map_name, strlen(mrec->map_name));
  if (!duplicate) {
    ht_append_table(fontmap,
		    mrec->map_name, strlen(mrec->map_name), mrec);
  }

  return 0;
}

int
pdf_remove_fontmap_record (const char *map_name)
{
  ht_remove_table(fontmap, map_name, strlen(map_name), hval_free);

  return 0;
}

int
pdf_insert_fontmap_record (fontmap_rec *mrec)
{
  if (fontmap_invalid(mrec)) {
    WARN("Invalid fontmap record...");
    return -1;
  }

  ht_insert_table(fontmap,
		  mrec->map_name, strlen(mrec->map_name),
		  mrec, hval_free);

  return 0;
}

int
pdf_read_fontmap_line (fontmap_rec *mrec, const char *mline, long mline_len)
{
  int    retval;
  char  *tex_name;
  char  *p, *endptr;


  ASSERT(mrec);

  p      = (char *) mline;
  endptr = p + mline_len;

  skip_white(&p, endptr);
  if (p >= endptr)
    return -1;

  tex_name = parse_ident(&p, endptr);
  if (!tex_name)
    return -1;

  retval = fontmap_parse_mapdef(mrec, tex_name, p, endptr);
  RELEASE(tex_name);

  return retval;
}

static int
is_pdfm_mapline (const char *mline) /* NULL terminated. */
{
  char  *p, *endptr;

  p      = (char *) mline;
  endptr = p + strlen(mline);

  skip_white(&p, endptr);
  while (p < endptr) {
    switch (p[0]) {
    case '<':  case '\"': case '\'':
      return 0;
      break;
    case '-':
      return 1;
      break;
    }
    while (p < endptr && !isspace(p[0])) p++;
    skip_white(&p, endptr);
  }

  return 1;
}

static long
delete_records (FILE *fp, long num_lines)
{
  long  count = 0;
  char *map_name;
  char *p, *endptr;

  while (num_lines-- > 0 &&
	 (p = mfgets(work_buffer, WORK_BUFFER_SIZE, fp)) != NULL) {
    endptr = p + strlen(work_buffer);
    skip_white(&p, endptr);
    map_name = p;
    while (p < endptr && !isspace(p[0])) p++;
    if (p > map_name) {
      p[0] = '\0';
      WARN("Deleting fontmap record for \"%s\"", map_name);
      pdf_remove_fontmap_record(map_name);
      count++;
    }
  }

  return count;
}

void
pdf_load_fontmap_file (const char *filename, int mode)
{
  fontmap_rec *mrec;
  FILE        *fp;
  char        *p = NULL, *endptr;
  long         llen, lpos  = 0;

  ASSERT(filename);
  ASSERT(fontmap) ;

  if (verbose)
    MESG("<FONTMAP:%s", filename);

  fp = DPXFOPEN(filename, DPX_RES_TYPE_FONTMAP);
  if (!fp) {
    WARN("Couldn't open the font map file \"%s\".", filename);
    return;
  }

  while ((p = mfgets(work_buffer, WORK_BUFFER_SIZE, fp)) != NULL) {

    lpos++;
    llen   = strlen(work_buffer);
    endptr = p + llen;

    skip_white(&p, endptr);

    if (!is_pdfm_mapline(p)) {
      WARN("This .map file looks like using dvips format.");
      WARN("-- Current input buffer is: %s", p);
      WARN("-- Reading fontmap file stopped at: file=\"%s\", line=%d.",
	   filename, lpos);
      rewind(fp);
      delete_records(fp, lpos - 1);
      break;
    }

    if (p < endptr && p[0] != '%') {
      mrec = NEW(1, fontmap_rec);
      pdf_init_fontmap_record(mrec);
      if (pdf_read_fontmap_line(mrec, p, llen) < 0) {
	pdf_clear_fontmap_record(mrec);
	RELEASE(mrec);
	WARN("Invalid map record in fontmap file.");
	WARN("-- Current input buffer is: %s", p);
	WARN("-- Reading fontmap file stopped at: file=\"%s\", line=%d.",
	     filename, lpos);
	rewind(fp);
	delete_records(fp, lpos - 1);
	break;
      } else {
	switch (mode) {
	case FONTMAP_RMODE_REPLACE:
	  pdf_insert_fontmap_record(mrec);
	  break;
	case FONTMAP_RMODE_APPEND:
	  pdf_append_fontmap_record(mrec);
	  break;
	case FONTMAP_RMODE_REMOVE:
	  pdf_remove_fontmap_record(mrec->map_name);
	  pdf_clear_fontmap_record (mrec);
	  RELEASE(mrec);
	  break;
	default:
	  pdf_clear_fontmap_record (mrec);
	  RELEASE(mrec);
	  break;
	}
      }
    }

  }
  DPXFCLOSE(fp);

  if (verbose)
    MESG(">");
}

fontmap_rec *
pdf_lookup_fontmap_record (const char *map_name)
{
  fontmap_rec *mrec = NULL;

  if (fontmap && map_name)
    mrec = ht_lookup_table(fontmap, map_name, strlen(map_name));

  return mrec;
}

/* This must take care about subfonts... */
fontmap_rec *
pdf_lookup_fontmap_record2 (const char *map_name, int *sfd_id)
{
  fontmap_rec *mrec = NULL;

  *sfd_id = -1; /* by default */

  if (fontmap && map_name) {
    mrec = ht_lookup_table(fontmap, map_name, strlen(map_name));
    if (!mrec) { /* Possibly subfont */
      struct ht_iter iter;

      if (ht_set_iter(fontmap, &iter) < 0)
	mrec = NULL;
      else {
	do {
	  char  *key;
	  int    keylen, prfx_len;

	  key  = ht_iter_getkey(&iter, &keylen);
	  mrec = ht_iter_getval(&iter);
	  if (mrec->sfd_name) {
	    prfx_len = keylen - strlen(mrec->sfd_name) - 2;
	    if (!memcmp(map_name, key, prfx_len)) {
              *sfd_id = load_sfd_record(mrec->sfd_name, (char *)map_name + prfx_len);
	      if (*sfd_id >= 0)
	        break;
            }
	  }
	  mrec = NULL;
	} while (ht_iter_next(&iter) >= 0);
      }
      ht_clear_iter(&iter);
    }
    if (mrec && mrec->deleted)
      return NULL;
  }

  return mrec;
}

void
pdf_init_fontmaps (void)
{
  fontmap = NEW(1, struct ht_table);
  ht_init_table(fontmap);
}

void
pdf_close_fontmaps (void)
{
  if (fontmap) {
    ht_clear_table(fontmap, hval_free);
    RELEASE(fontmap);
  }
  fontmap = NULL;
}

void
pdf_clear_fontmaps (void)
{
  pdf_close_fontmaps();
}

/* CIDFont options
 *
 * FORMAT:
 *
 *   (:int:)?!?string(/string)?(,string)?
 */

static char *
substr (char **str, char stop)
{
  char *sstr, *endptr;

  endptr = strchr(*str, stop);
  if (!endptr || endptr == *str)
    return NULL;
  sstr = NEW(endptr-(*str)+1, char);
  memcpy(sstr, *str, endptr-(*str));
  sstr[endptr-(*str)] = '\0';

  *str = endptr+1;
  return sstr;
}

#include <ctype.h>
#define CID_MAPREC_CSI_DELIM '/'

static char *
strip_options (const char *map_name, fontmap_opt *opt)
{
  char *font_name;
  char *p, *next = NULL;
  int   have_csi = 0, have_style = 0;

  ASSERT(opt);

  p = (char *) map_name;
  font_name      = NULL;
  opt->charcoll  = NULL;
  opt->index     = 0;
  opt->style     = FONTMAP_STYLE_NONE;
  opt->flags     = 0;

  if (*p == ':' && isdigit(*(p+1))) {
    opt->index = (int) strtoul(p+1, &next, 10);
    if (*next == ':')
      p = next + 1;      
    else {
      opt->index = 0;
    }
  }
  if (*p == '!') { /* no-embedding */
    if (*(++p) == '\0')
      ERROR("Invalid map record: %s (--> %s)", map_name, p);
    opt->flags |= FONTMAP_OPT_NOEMBED;
  }

  if ((next = strchr(p, CID_MAPREC_CSI_DELIM)) != NULL) {
    if (next == p)
      ERROR("Invalid map record: %s (--> %s)", map_name, p);
    font_name = substr(&p, CID_MAPREC_CSI_DELIM);
    have_csi  = 1;
  } else if ((next = strchr(p, ',')) != NULL) {
    if (next == p)
      ERROR("Invalid map record: %s (--> %s)", map_name, p);
    font_name = substr(&p, ',');
    have_style = 1;
  } else {
    font_name = NEW(strlen(p)+1, char);
    strcpy(font_name, p);
  }

  if (have_csi) {
    if ((next = strchr(p, ',')) != NULL) {
      opt->charcoll = substr(&p, ',');
      have_style = 1;
    } else if (p[0] == '\0') {
      ERROR("Invalid map record: %s.", map_name);
    } else {
      opt->charcoll = NEW(strlen(p)+1, char);
      strcpy(opt->charcoll, p);
    }
  }

  if (have_style) {
    if (!strncmp(p, "BoldItalic", 10)) {
      if (*(p+10))
	ERROR("Invalid map record: %s (--> %s)", map_name, p);
      opt->style = FONTMAP_STYLE_BOLDITALIC;
    } else if (!strncmp(p, "Bold", 4)) {
      if (*(p+4))
	ERROR("Invalid map record: %s (--> %s)", map_name, p);
      opt->style = FONTMAP_STYLE_BOLD;
    } else if (!strncmp(p, "Italic", 6)) {
      if (*(p+6))
	ERROR("Invalid map record: %s (--> %s)", map_name, p);
      opt->style = FONTMAP_STYLE_ITALIC;
    }
  }

  return font_name;
}
