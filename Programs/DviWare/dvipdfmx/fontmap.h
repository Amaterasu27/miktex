/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/fontmap.h,v 1.3 2005/07/03 20:02:28 csc Exp $
    
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

#ifndef _FONTMAP_H_
#define _FONTMAP_H_

#define FONTMAP_RMODE_REPLACE  0
#define FONTMAP_RMODE_APPEND  '+'
#define FONTMAP_RMODE_REMOVE  '-'

#define FONTMAP_OPT_REMAP   (1 << 0)
#define FONTMAP_OPT_NOEMBED (1 << 1)
#define FONTMAP_OPT_VERT    (1 << 2)

#define FONTMAP_STYLE_NONE       0
#define FONTMAP_STYLE_BOLD       1
#define FONTMAP_STYLE_ITALIC     2
#define FONTMAP_STYLE_BOLDITALIC 3

/* Options */
typedef struct fontmap_opt {
  double slant, extend;
  long   mapc,  flags;

  char  *otl_tags;

  /* For PK font support.
   * Entries for PK font mapping automatically inserted
   * by DVI interpreter.
   */
  double design_size;

  /* CIDFont option */
  char  *charcoll;
  int    index;
  int    style;
} fontmap_opt;

typedef struct fontmap_rec {
  char *map_name;  /* for subfont support... */
  char *sfd_name;  /* for subfont support... */

  char *font_name;
  char *enc_name;

  fontmap_opt opt;

  int    deleted;   /* pdfTeX compatibility??? */
} fontmap_rec;

extern void         pdf_fontmap_set_verbose   (void);

extern void         pdf_init_fontmaps         (void);
extern void         pdf_clear_fontmaps        (void);
extern void         pdf_close_fontmaps        (void);

extern void         pdf_init_fontmap_record   (fontmap_rec *mrec);
extern void         pdf_clear_fontmap_record  (fontmap_rec *mrec);

extern void         pdf_load_fontmap_file     (const char  *filename, int mode);
extern int          pdf_read_fontmap_line     (fontmap_rec *mrec,
					       const char  *mline, long mline_strlen);

extern int          pdf_append_fontmap_record (fontmap_rec *mrec);
extern int          pdf_remove_fontmap_record (const char  *tex_name);
extern int          pdf_insert_fontmap_record (fontmap_rec *mrec);
extern fontmap_rec *pdf_lookup_fontmap_record (const char  *tex_name);

/* With SFD.... */
extern fontmap_rec *pdf_lookup_fontmap_record2 (const char *tex_name, int *sfd_id);

#endif /* _FONTMAP_H_ */
