/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/tt_build.c,v 1.1 2003/12/27 18:03:16 csc Exp $
    
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
 * Subsetting glyf, updating loca, hmtx, ...
 *
 */

#include "config.h"

#include "system.h"
#include "mem.h"
#include "error.h"

#include "sfnt.h"
#include "tt_table.h"
#include "tt_build.h"

#define NUM_GLYPH_LIMIT        65535
#define TABLE_DATA_ALLOC_SIZE  40960
#define GLYPH_ARRAY_ALLOC_SIZE 256

USHORT
tt_add_to_used (struct tt_glyphs *g, USHORT gid)
{
  USHORT idx;

  for (idx = 0; idx < g->num_glyphs; idx++) {
    if (gid == (g->used_glyphs)[idx].gid)
      return idx;
  }
  
  if (g->num_glyphs >= NUM_GLYPH_LIMIT)
    ERROR("Too many glyphs: %u (limit: %u)", g->num_glyphs, NUM_GLYPH_LIMIT);

  /* grow used_glyphs array */
  if (g->num_glyphs >= g->max_glyphs) {
    g->max_glyphs += GLYPH_ARRAY_ALLOC_SIZE;
    g->used_glyphs = RENEW(g->used_glyphs, g->max_glyphs, struct glyph_desc);
    memset(g->used_glyphs+g->num_glyphs, 0, (g->max_glyphs-g->num_glyphs)*sizeof(struct glyph_desc));
  }

  (g->used_glyphs)[idx].gid = gid;
  g->num_glyphs += 1;
  return idx;
}

/*
 * Initialization
 */
struct tt_glyphs *
tt_build_init (void)
{
  struct tt_glyphs *g;

  g = NEW(1, struct tt_glyphs);

  g->num_glyphs  = 0;
  g->max_glyphs  = 0;
  g->emsize      = 1;
  g->used_glyphs = NULL;

  tt_add_to_used(g, 0);

  return g;
}

void
tt_build_finish (struct tt_glyphs *g)
{
  if (g) {
    if (g->used_glyphs)
      RELEASE(g->used_glyphs);
    RELEASE(g);
  }
}

static USHORT
map_gid (struct tt_glyphs *g, USHORT gid)
{
  USHORT idx;

  for (idx = 0; idx < g->num_glyphs; idx++) {
    if (gid == (g->used_glyphs)[idx].gid)
      return idx;
  }

  return tt_add_to_used(g, gid);
}

/*
 * Update hmtx and loca tables.
 */
static int
build_other (sfnt *sfont, struct tt_glyphs *g, USHORT num_hm, ULONG last_offset)
{
  char  *hmtx_table_data = NULL, *loca_table_data = NULL;
  ULONG  hmtx_table_size, loca_table_size;
  USHORT default_advw, advw, last_advw; /* Advance width */
  SHORT  default_lsb, lsb;              /* LSB width     */
  ULONG  hmtx_offset;
  USHORT old_gid;
  char  *p = NULL, *q = NULL;
  ULONG  offset;
  int    i;

  hmtx_table_size = (g->num_glyphs) * 4;  /* USHORT + SHORT */
  loca_table_size = (g->num_glyphs + 1) * 4;  /* ULONG, We use long format. */

  hmtx_table_data = p = NEW(hmtx_table_size, char);
  loca_table_data = q = NEW(loca_table_size, char);

  /*
   * Read AdvW and LSB of gid, last AdvW entry.
   */
  hmtx_offset  = sfnt_locate_table(sfont, "hmtx");
  default_advw = sfnt_get_ushort(sfont);
  default_lsb  = sfnt_get_short (sfont);
  sfnt_seek_set(sfont, hmtx_offset+4*(num_hm-1));
  last_advw    = sfnt_get_ushort(sfont);

  if ((g->used_glyphs)[0].gid != 0)
    ERROR("Unexpected error: First entry not gid = 0.");

  /*
   * gid = 0
   */
  offset = 0;
  p += sfnt_put_ushort(p, default_advw);
  p += sfnt_put_short (p, default_lsb);
  q += sfnt_put_ulong (q, offset);

  (g->used_glyphs)[0].width = default_advw;

  for ( i = 1; i < g->num_glyphs; i++) {
    old_gid = (g->used_glyphs)[i].gid;
    offset  = (g->used_glyphs)[i].offset;
    /*
     * Advance width and LSB
     */
    if (old_gid < num_hm) {
      sfnt_seek_set(sfont, hmtx_offset+old_gid*4);
      advw = sfnt_get_ushort(sfont);
      lsb  = sfnt_get_short(sfont);
    } else {
      advw = last_advw;
      sfnt_seek_set(sfont, hmtx_offset+4*num_hm+(old_gid-num_hm)*2);
      lsb  = sfnt_get_short(sfont);
    }
    p += sfnt_put_ushort(p, advw);
    p += sfnt_put_short (p, lsb);
    q += sfnt_put_ulong (q, offset);
    (g->used_glyphs)[i].width = advw;
  }
  q += sfnt_put_ulong(q, last_offset);

  sfnt_set_table(sfont, "hmtx", (char *) hmtx_table_data, hmtx_table_size);
  sfnt_set_table(sfont, "loca", (char *) loca_table_data, loca_table_size);

  return 0;
}

/*
 * TrueType outline data.
 */
#define ARG_1_AND_2_ARE_WORDS     (1 << 0)
#define ARGS_ARE_XY_VALUES        (1 << 1)
#define ROUND_XY_TO_GRID          (1 << 2)
#define WE_HAVE_A_SCALE           (1 << 3)
#define RESERVED                  (1 << 4)
#define MORE_COMPONENT            (1 << 5)
#define WE_HAVE_AN_X_AND_Y_SCALE  (1 << 6)
#define WE_HAVE_A_TWO_BY_TWO      (1 << 7)
#define WE_HAVE_INSTRUCTIONS      (1 << 8)
#define USE_MY_METRICS            (1 << 9)

int
tt_build_tables (sfnt *sfont, struct tt_glyphs *g)
{
  char *glyf_table_data = NULL;
  ULONG glyf_table_size, max_glyf_table_size;
  /* some information available from other TrueType table */
  struct tt_head_table *head = NULL;
  struct tt_hhea_table *hhea = NULL;
  struct tt_maxp_table *maxp = NULL;
  SHORT  indexToLocFormat;
  USHORT numGlyphs, numHMetrics;
  /* temp */
  ULONG currpos, *location;
  ULONG offset;
  USHORT i;

  if (sfont == NULL || sfont->stream == NULL)
    ERROR("File not opened.");

  if (sfont->type != SFNT_TYPE_TRUETYPE &&
      sfont->type != SFNT_TYPE_TTC)
    ERROR("Invalid font type");

  if (g->num_glyphs > NUM_GLYPH_LIMIT)
    ERROR("Too many glyphs.");

  /*
   * Read head, hhea, maxp, loca:
   *
   *   unitsPerEm       --> head
   *   numHMetrics      --> hhea
   *   indexToLocFormat --> head
   *   numGlyphs        --> maxp
   */
  head = tt_read_head_table(sfont);
  hhea = tt_read_hhea_table(sfont);
  maxp = tt_read_maxp_table(sfont);

  if (hhea->metricDataFormat != 0)
    ERROR("Unknown metricDataFormat.");

  g->emsize        = head->unitsPerEm;
  numHMetrics      = hhea->numberOfHMetrics;
  indexToLocFormat = head->indexToLocFormat;
  numGlyphs        = maxp->numGlyphs;

  sfnt_locate_table(sfont, "loca");
  location = NEW(numGlyphs+1, ULONG);

  switch (indexToLocFormat) {
  case 0:
    for (i = 0; i < numGlyphs + 1; i++)
      location[i] = 2*((ULONG) sfnt_get_ushort(sfont));
    break;
  case 1:
    for (i = 0; i < numGlyphs + 1; i++)
      location[i] = sfnt_get_ulong(sfont);
    break;
  default:
    ERROR("Unknown IndexToLocFormat.");
    break;
  }

  /*
   * Read glyf table.
   */

  offset = sfnt_locate_table(sfont, "glyf");

  /*
   * First allocate MIN_GLYPH_LEN*num_glyphs bytes. MIN_GLYPH_LEN
   * is a minimal size of TrueType glyph data; each glyphs consumes
   * at least 10 bytes. (number_of_counter and BoundingBox)
   */
#define MIN_GLYPH_LEN 10
  max_glyf_table_size = (g->num_glyphs) * MIN_GLYPH_LEN;
  glyf_table_data = NEW(max_glyf_table_size, char);

  /*
   * The num_glyphs may grow when composite glyph is found.
   * A component of glyph refered by a composite glyph is appended
   * to used_glyphs if it is not already registered in used_glyphs.
   * Glyph programs of composite glyphs are modified so that it
   * correctly refer to new gid of their components.
   */
  currpos = 0;
  for (i = 0; i < NUM_GLYPH_LIMIT; i++) {
    USHORT gid;     /* old gid */
    ULONG  loc, len;
    BYTE  *p, *end_ptr;
    SHORT  number_of_contours;
    int    padlen = 0;

    if (i >= g->num_glyphs) /* finished */
      break;

    gid = (g->used_glyphs)[i].gid;
    if (gid >= numGlyphs)
      ERROR("Invalid glyph index (gid %u)", gid);

    loc = location[gid];
    len = location[gid+1] - loc;
    (g->used_glyphs)[i].offset = currpos; /* new offset */

    if (len == 0) { /* Does not contains any data. */
      continue;
    } else if (len < MIN_GLYPH_LEN)
      ERROR("Invalid TrueType glyph data (gid %u).", gid);

    padlen = 4 - (len % 4);
    /* Need more space. */
    if (max_glyf_table_size < currpos + len + padlen) {
      max_glyf_table_size +=
	(len + padlen > TABLE_DATA_ALLOC_SIZE) ? len + padlen : TABLE_DATA_ALLOC_SIZE;
      glyf_table_data = RENEW(glyf_table_data, max_glyf_table_size, char);
    }

    p = glyf_table_data + currpos;
    end_ptr = p + len;
    memset(p, 0, len + padlen); /* Not sure, but maybe zero padded. */

    sfnt_seek_set(sfont, offset+loc);
    number_of_contours = sfnt_get_short(sfont);
    p += sfnt_put_short(p, number_of_contours);
    p += sfnt_read(p, 8, sfont); /* BoundingBox: FWord x 4 */

    sfnt_read(p, len - MIN_GLYPH_LEN, sfont); /* Read evrything else. */
    /*
     * Fix GIDs of composite glyphs.
     */
    if (number_of_contours < 0) {
      USHORT flags, cgid; /* flag, gid of a component */
      do {
	if (p >= end_ptr)
	  ERROR("Invalid TrueType glyph data (gid %u): %u bytes", gid, len);
	/*
	 * Flags and gid of component glyph are both USHORT.
	 */
	flags = ((*p) << 8)| *(p+1);
	p += 2;
	cgid  = ((*p) << 8)| *(p+1);
	if (cgid >= numGlyphs)
	  ERROR("Invalid gid (%u > %u) in composite glyph %u.", cgid, numGlyphs, gid);
	p += sfnt_put_ushort(p, map_gid(g, cgid)); /* Map */
	/*
	 * Just skip remaining part.
	 */
	p += (flags & ARG_1_AND_2_ARE_WORDS) ? 4 : 2;
	if (flags & WE_HAVE_A_SCALE) /* F2Dot14 */
	  p += 2;
	else if (flags & WE_HAVE_AN_X_AND_Y_SCALE) /* F2Dot14 x 2 */
	  p += 4;
	else if (flags & WE_HAVE_A_TWO_BY_TWO) /* F2Dot14 x 4 */
	  p += 8;
      } while (flags & MORE_COMPONENT);
      /*
       * TrueType instructions comes here:
       *  length_of_instruction (ushort)
       *  instruction (byte * length_of_instruction)
       */
    }
    currpos += len + padlen;
  }
  glyf_table_size = currpos;

  RELEASE(location);

  /*
   * Do not free xxx_data, sfnt_set_table just copy address.
   */
  sfnt_set_table(sfont, "glyf", glyf_table_data, glyf_table_size);

  /*
   * Update loca and hmtx table.
   */
  build_other(sfont, g, numHMetrics, currpos);

  head->checkSumAdjustment = 0;
  maxp->numGlyphs          = g->num_glyphs;
  hhea->numberOfHMetrics   = g->num_glyphs;
  head->indexToLocFormat   = 1; /* Forcing long format. */

  /* TODO */
  sfnt_set_table(sfont, "maxp", tt_pack_maxp_table(maxp), TT_MAXP_TABLE_SIZE);
  RELEASE(maxp);
  sfnt_set_table(sfont, "hhea", tt_pack_hhea_table(hhea), TT_HHEA_TABLE_SIZE);
  RELEASE(hhea);
  sfnt_set_table(sfont, "head", tt_pack_head_table(head), TT_HEAD_TABLE_SIZE);
  RELEASE(head);

  return 0;
}
