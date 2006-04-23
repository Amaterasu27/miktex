/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/truetype.c,v 1.2 2005/07/03 20:02:29 csc Exp $
    
    This is dvipdfmx, an eXtended version of dvipdfm by Mark A. Wicks.

    Copyright (C) 2002 by Jin-Hwan Cho and Shunsaku Hirata,
    the dvipdfmx project team <dvipdfmx@project.ktug.or.kr>
    
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

/*
 * OpenType GSUB support is not available yet.
 *
 *  Required features: onum (oldstyle digits), c2sc/smcp, and more.
 *
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <ctype.h>
#include "system.h"

#include "mem.h"
#include "error.h"
#include "dpxfile.h"

#include "numbers.h"
#include "dpxutil.h"

#include "pdfobj.h"
#include "pdfresource.h"
#include "pdffont.h"

#include "pdfencoding.h"
#include "unicode.h"
#include "agl.h"

/* TrueType */
#include "sfnt.h"
#include "tt_cmap.h"
#include "tt_table.h"
#include "tt_glyf.h"
#include "tt_post.h"
#include "tt_gsub.h"
#include "tt_aux.h"

#include "truetype.h"

#define TTFONT_DEBUG_STR "TTFont"
#define TTFONT_DEBUG     3

int
pdf_font_open_truetype (pdf_font *font)
{
  sfnt     *sfont;
  pdf_obj  *fontdict, *descriptor, *tmp;
  int       encoding_id, embedding;
  char     *fontname, *ident;
  int       fontname_len;
  FILE     *fp;

  ASSERT(font);

  ident = pdf_font_get_ident(font);

  fp = DPXFOPEN(ident, DPX_RES_TYPE_TTFONT);
  if (!fp)
    return -1;

  sfont = sfnt_open(fp);
  if (!sfont) {
    ERROR("Could not open TrueType font: %s", ident);
    DPXFCLOSE(fp);
    return -1;
  } else if (sfont->type != SFNT_TYPE_TRUETYPE ||
	     sfnt_read_table_directory(sfont, 0) < 0) {
    sfnt_close(sfont);
    DPXFCLOSE(fp);
    return -1;
  }

  pdf_font_set_subtype (font, PDF_FONT_FONTTYPE_TRUETYPE);

  fontdict    = pdf_font_get_resource  (font);
  descriptor  = pdf_font_get_descriptor(font);
  encoding_id = pdf_font_get_encoding  (font);

  embedding   = pdf_font_get_flag(font, PDF_FONT_FLAG_NOEMBED) ? 0 : 1;

  tmp = tt_get_fontdesc(sfont, &embedding, 1);
  if (!tmp) {
    ERROR("Could not obtain neccesary font info.");
    sfnt_close(sfont);
    DPXFCLOSE(fp);
    return -1;
  }
  pdf_merge_dict (descriptor, tmp);
  pdf_release_obj(tmp);

  if (!embedding && encoding_id >= 0) {
    ERROR("Custum encoding not allowed for non-embedded TrueType font.");
    sfnt_close(sfont);
    return -1;
  }
  if (!embedding) {
    pdf_font_set_flags(font, PDF_FONT_FLAG_NOEMBED);
  }

  fontname = NEW(PDF_NAME_LEN_MAX + 1, char);
  memset(fontname, 0, PDF_NAME_LEN_MAX + 1);
  fontname_len = tt_get_ps_fontname(sfont, fontname, PDF_NAME_LEN_MAX);
  if (fontname_len == 0) {
    fontname_len = MIN(strlen(ident), PDF_NAME_LEN_MAX);
    strncpy(fontname, ident, fontname_len);
    fontname[fontname_len] = '\0';
  }
  pdf_font_set_fontname(font, fontname);
  RELEASE(fontname);

  sfnt_close(sfont);
  DPXFCLOSE(fp);

  /*
   * We use MacRoman as "default" encoding.
   */
  if (encoding_id < 0)
    pdf_add_dict(fontdict,
		 pdf_new_name("Encoding"),
		 pdf_new_name("MacRomanEncoding"));
  else {
    if (pdf_encoding_is_predefined(encoding_id))
      pdf_add_dict(fontdict,
		   pdf_new_name("Encoding"),
		   pdf_new_name(pdf_encoding_get_name(encoding_id)));
  }

  return 0;
}

/*
 * The 'name' table should be preserved since it contains copyright
 * information, but it might cause problem when there are invalid
 * table entries (wrongly encoded text which is often the case in
 * CJK fonts). Acrobat does not use 'name' table. Unicode TrueType
 * fonts may have 10K bytes 'name' table...
 *
 * We preserve the 'OS/2' table too, since it contains the license
 * information. PDF applications should use this table to decide
 * whether the font is embedded only for the purpose of preview &
 * printing. Otherwise, we must encrypt the document. Acrobat does
 * not use 'OS/2' table, though...
 */
static struct
{
  const char *name;
  int   must_exist;
} required_table[] = {
  {"OS/2", 1}, {"head", 1}, {"hhea", 1}, {"loca", 1}, {"maxp", 1},
  {"name", 1}, {"glyf", 1}, {"hmtx", 1}, {"fpgm", 0}, {"cvt ", 0},
  {"prep", 0}, {"cmap", 1}, {NULL, 0}
};

static void
do_widths (pdf_font *font, double *widths)
{
  pdf_obj  *fontdict, *tmp_array;
  char     *usedchars;
  int       code, firstchar, lastchar;

  fontdict  = pdf_font_get_resource (font);
  usedchars = pdf_font_get_usedchars(font);

  firstchar = 255; lastchar = 0;
  for (code = 0; code < 256; code++) {
    if (usedchars[code]) {
      if (code < firstchar) firstchar = code;
      if (code > lastchar)  lastchar  = code;
    }
  }
  if (firstchar > lastchar) {
    WARN("No glyphs actually used???");
    return;
  }

  tmp_array = pdf_new_array();
  for (code = firstchar; code <= lastchar; code++) {
    if (usedchars[code])
      pdf_add_array(tmp_array,
		    pdf_new_number(ROUND(widths[code], 1)));
    else {
      pdf_add_array(tmp_array, pdf_new_number(0.0));
    }
  }
  if (pdf_array_length(tmp_array) > 0) {
    pdf_add_dict(fontdict,
		 pdf_new_name("Widths"), pdf_ref_obj(tmp_array));
  }
  pdf_release_obj(tmp_array);
  pdf_add_dict(fontdict,
	       pdf_new_name("FirstChar"), pdf_new_number(firstchar));
  pdf_add_dict(fontdict,
	       pdf_new_name("LastChar"),  pdf_new_number(lastchar));

  return;
}

static int verbose = 0;

#define PDFUNIT(v) ((double) (ROUND(1000.0*(v)/(glyphs->emsize), 1)))

/*
 * There are several issues in TrueType font support in PDF.
 * How PDF viewers select TrueType cmap table is not so clear.
 * Most reliable way seem to reencode font and sort glyphs as
 * charcode == gid and to use Mac-Roman format 0 subtable.
 * It does not work with encodings that uses full 256 range since
 * GID = 0 is reserved for .notdef, so GID = 256 is not accessible.
 */
static void
do_builtin_encoding (pdf_font *font, sfnt *sfont)
{
  char             *fontname, *usedchars;
  struct tt_glyphs *glyphs;
  char             *cmap_table;
  tt_cmap          *ttcm;
  int               code, count;
  double            widths[256];

  ttcm = tt_cmap_read(sfont, TT_MAC, TT_MAC_ROMAN);
  if (!ttcm) {
    ERROR("Cannot read Mac-Roman TrueType cmap table");
  }

  usedchars  = pdf_font_get_usedchars (font);
  fontname   = pdf_font_get_fontname  (font);

  cmap_table = NEW(274, char);
  memset(cmap_table, 0, 274);
  sfnt_put_ushort(cmap_table,    0);            /* Version  */
  sfnt_put_ushort(cmap_table+2,  1);            /* Number of subtables */
  sfnt_put_ushort(cmap_table+4,  TT_MAC);       /* Platform ID */
  sfnt_put_ushort(cmap_table+6,  TT_MAC_ROMAN); /* Encoding ID */
  sfnt_put_ulong (cmap_table+8,  12);           /* Offset   */
  sfnt_put_ushort(cmap_table+12, 0);            /* Format   */
  sfnt_put_ushort(cmap_table+14, 262);          /* Length   */
  sfnt_put_ushort(cmap_table+16, 0);            /* Language */

  glyphs = tt_build_init();

  if (verbose > 2)
    MESG("[glyphs:/.notdef");

  count = 1; /* .notdef */
  for (code = 0; code < 256; code++) {
    unsigned short new_gid, gid = 0;

    if (!usedchars[code])
      continue;

    if (verbose > 2)
      MESG("/.c0x%02x", code);

    gid = tt_cmap_lookup(ttcm, (unsigned short) code);
    if (gid == 0) {
      WARN("Character 0x%02x missing in font \"%s\".", code, fontname);
      new_gid = 0;
    } else {
      new_gid = tt_find_glyph(glyphs, gid);
      if (new_gid == 0) {
	new_gid = tt_add_glyph(glyphs, gid, count); /* count returned. */
      }
    }
    cmap_table[18+code] = new_gid;
    count++;
  }
  tt_cmap_release(ttcm);

  if (verbose > 2)
    MESG("]");

  if (tt_build_tables(sfont, glyphs) < 0)
    ERROR("Could not created FontFile stream.");

  for (code = 0; code < 256; code++) {
    if (usedchars[code]) {
      USHORT idx;

      idx = tt_get_index(glyphs, (USHORT) cmap_table[18+code]);
      widths[code] = PDFUNIT(glyphs->gd[idx].advw);
    } else {
      widths[code] = 0.0;
    }
  }
  do_widths(font, widths);

  if (verbose > 1) 
    MESG("[%d glyphs]", glyphs->num_glyphs);

  tt_build_finish(glyphs);

  sfnt_set_table(sfont, "cmap", cmap_table, 274);
}


static USHORT
find_glyph_paranoiac (const char *glyphname,
		      tt_cmap *ttcm, otl_gsub *gsub, sfnt *sfont)
{
  USHORT    gid;
  agl_name *agln;
  char     *otlfeat;

  gid  = 0;
  agln = agl_lookup_list(glyphname);
  while (agln && gid == 0) {
    if (agln->n_components > 1) {
      WARN("Glyph \"%s\" inaccessible (composite character).", agln->name);
      agln = agln->alternate;
      continue;
    }

    if (agln->suffix) {
      WARN("Glyph \"%s\" has suffix: %s.%s",
	   glyphname, agln->name, agln->suffix);
      otlfeat = (char *) agl_suffix_to_otltag(agln->suffix);
      if (!otlfeat) {
	WARN("No OTL GSUB feature for .%s, suffix ignored.", agln->suffix);
      } else if (otl_gsub_select(gsub, "*", "*", otlfeat) < 0) {
	if (verbose)
	  WARN("Trying to load OTL GSUB for \"*.*.%4s\"...", otlfeat);
	if (otl_gsub_add_feat(gsub, "*", "*", otlfeat, sfont) < 0) {
	  otlfeat = NULL;
	}
      }

      gid = find_glyph_paranoiac(agln->name, ttcm, gsub, sfont);
      if (gid > 0 && otlfeat &&
	  otl_gsub_select(gsub, "*", "*", otlfeat) >= 0) {
	if (otl_gsub_apply(gsub, &gid) < 0) {
	  WARN("No substitution... suffix dropped.");
	} else {
	  WARN("Glyph substituted: %s -> %s.%s (gid=%u)",
	       glyphname, agln->name, agln->suffix, gid);
	}
      }
    } else {
      gid = tt_cmap_lookup(ttcm, agln->unicodes[0]); 
    }
    agln = agln->alternate;
  }

  return gid;
}

static void
do_custum_encoding (pdf_font *font, sfnt *sfont)
{
  char                  *usedchars, *fontname;
  int                    encoding_id;
  struct tt_glyphs      *glyphs;
  struct tt_post_table  *post;
  char                  *cmap_table;
  int                    code, count;
  tt_cmap               *ttcm;
  otl_gsub              *gsub;
  char                 **enc_vec;
  double                 widths[256];

  encoding_id = pdf_font_get_encoding (font);
  fontname    = pdf_font_get_fontname (font);
  usedchars   = pdf_font_get_usedchars(font);

  enc_vec  = pdf_encoding_get_encoding(encoding_id);

  post     = tt_read_post_table(sfont);
  ttcm     = tt_cmap_read(sfont, TT_WIN, TT_WIN_UCS4);
  if (!ttcm) {
    ttcm   = tt_cmap_read(sfont, TT_WIN, TT_WIN_UNICODE);
  }
  gsub     = otl_gsub_new();

  ASSERT(enc_vec);

  if (!post && !ttcm) {
    ERROR("No post table or Unicode cmap found in font: %s", fontname);
  }

  cmap_table = NEW(274, char);
  memset(cmap_table, 0, 274);
  sfnt_put_ushort(cmap_table,    0);            /* Version  */
  sfnt_put_ushort(cmap_table+2,  1);            /* Number of subtables */
  sfnt_put_ushort(cmap_table+4,  TT_MAC);       /* Platform ID */
  sfnt_put_ushort(cmap_table+6,  TT_MAC_ROMAN); /* Encoding ID */
  sfnt_put_ulong (cmap_table+8,  12);           /* Offset   */
  sfnt_put_ushort(cmap_table+12, 0);            /* Format   */
  sfnt_put_ushort(cmap_table+14, 262);          /* Length   */
  sfnt_put_ushort(cmap_table+16, 0);            /* Language */

  glyphs = tt_build_init();

  if (verbose > 2)
    MESG("[glyphs:/.notdef");

  count = 1; /* +1 for .notdef */
  for (code = 0; code < 256; code++) {
    USHORT new_gid, gid = 0;

    if (!usedchars[code]) {
      continue;
    }

    if (!enc_vec[code] || !strcmp(enc_vec[code], ".notdef")) {
      WARN("Character mapped to .notdef used. (char: 0x%02X, font: %s, encoding: %s)",
	   code, fontname, pdf_encoding_get_name(encoding_id));
      WARN("Maybe incorrect encoding specified?");
      new_gid = 0;
    } else {
      if (verbose > 2)
	MESG("/%s", enc_vec[code]);
      /*
       * First we try glyph name to GID mapping using post table if post table
       * is available. If post table is not available or glyph is not listed 
       * in the post table, then we try Unicode if Windows-Unicode TrueType
       * cmap is available.
       */
      if (post) {
	gid = tt_lookup_post_table(post, enc_vec[code]);
      }
      if (gid == 0 && ttcm) {
	if (agl_name_is_unicode(enc_vec[code])) {
	  /* This doesn't handle uniXXXX.sc */
	  gid = tt_cmap_lookup(ttcm,
			       agl_name_convert_unicode(enc_vec[code]));
	} else {
	  gid = find_glyph_paranoiac(enc_vec[code], ttcm, gsub, sfont);
	}
      }

      /*
       * Older versions of gs had problem with glyphs (other than .notdef)
       * mapped to gid = 0.
       */
      if (gid == 0 && enc_vec[code]) {
	WARN("Glyph \"%s\" missing in font \"%s\".", enc_vec[code], fontname);
      } else if (enc_vec[code]) {
	if (verbose > 1) {
	  MESG("Glyph \"%s\" at code=0x%02x, gid=%u\n", enc_vec[code], code, gid);
	}
      }
      new_gid = tt_find_glyph(glyphs, gid);
      if (new_gid == 0) {
	new_gid = tt_add_glyph(glyphs, gid, count); /* count returned. */
	count++;
      }
    }
    cmap_table[18 + code] = new_gid;
  }
  otl_gsub_release(gsub);
  tt_cmap_release (ttcm);
  tt_release_post_table(post);

  if (verbose > 2)
    MESG("]");

  if (tt_build_tables(sfont, glyphs) < 0)
    ERROR("Could not created FontFile stream.");

  for (code = 0; code < 256; code++) {
    if (usedchars[code]) {
      USHORT idx;

      idx = tt_get_index(glyphs, (USHORT) cmap_table[18+code]);
      widths[code] = PDFUNIT(glyphs->gd[idx].advw);
    } else {
      widths[code] = 0.0;
    }
  }
  do_widths(font, widths);

  if (verbose > 1) 
    MESG("[%d glyphs]", glyphs->num_glyphs);

  tt_build_finish(glyphs);

  sfnt_set_table(sfont, "cmap", cmap_table, 274);

  return;
}

int
pdf_font_load_truetype (pdf_font *font)
{
  pdf_obj *fontdict, *descriptor;
  pdf_obj *fontfile;
  char    *fontname, *ident;
  FILE    *fp;
  sfnt    *sfont;
  int      encoding_id, i;

  ASSERT(font);

  verbose = pdf_font_get_verbose();

  if (!pdf_font_is_in_use(font)) {
    return 0;
  }

  encoding_id = pdf_font_get_encoding  (font);
  fontdict    = pdf_font_get_resource  (font);
  descriptor  = pdf_font_get_descriptor(font);
  ident       = pdf_font_get_ident     (font);
  fontname    = pdf_font_get_fontname  (font);

  if (encoding_id >= 0) {
    if (!pdf_lookup_dict(fontdict, "ToUnicode"))
      pdf_attach_ToUnicode_CMap(fontdict,
                                encoding_id,
			      pdf_font_get_usedchars(font));
  }

  if (pdf_font_get_flag(font, PDF_FONT_FLAG_NOEMBED)) {
    return 0;
  }

  fp = DPXFOPEN(ident, DPX_RES_TYPE_TTFONT);
  if (!fp) {
    ERROR("Unable to open file: %s", ident);
  }
  sfont = sfnt_open(fp);
  if (!sfont) {
    ERROR("Unable to open file: %s", ident);
    DPXFCLOSE(fp);
    return -1;
  }
  if (sfont->type != SFNT_TYPE_TRUETYPE || 
      sfnt_read_table_directory(sfont, 0) < 0) {
    ERROR("Not TrueType font ?");
    sfnt_close(sfont);
    DPXFCLOSE(fp);
    return -1;
  }

  /*
   * Create new TrueType cmap table with MacRoman encoding.
   */
  if (encoding_id < 0)
    do_builtin_encoding(font, sfont);
  else {
    do_custum_encoding(font, sfont);
  }

  /*
   * TODO: post table?
   */

  for (i = 0; required_table[i].name != NULL; i++) {
    if (sfnt_require_table(sfont,
			   required_table[i].name,
			   required_table[i].must_exist) < 0) {
      ERROR("TrueType table \"%s\" does not exist.",
	    required_table[i].name);
      sfnt_close(sfont);
      DPXFCLOSE(fp);
      return -1;
    }
  }

  /*
   * FontFile2
   */
  fontfile = sfnt_create_FontFile_stream(sfont);
  if (!fontfile)
    ERROR("Could not created FontFile stream for \"%s\".", ident);

  if (verbose > 1) {
    MESG("[%ld bytes]", pdf_stream_length(fontfile));
  }

  pdf_add_dict(descriptor,
	       pdf_new_name("FontFile2"), pdf_ref_obj(fontfile));
  pdf_release_obj(fontfile);

  sfnt_close(sfont);
  DPXFCLOSE(fp);

  return 0;
}
