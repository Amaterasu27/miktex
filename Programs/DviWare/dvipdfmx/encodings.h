/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/encodings.h,v 1.1 2003/12/27 18:03:16 csc Exp $
    
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

#ifndef _ENCODINGS_H_
#define _ENCODINGS_H_

typedef struct Encoding Encoding;

extern void      Encoding_set_verbose (void);

extern Encoding *Encoding_new     (void);
extern void      Encoding_flush   (Encoding *encoding);
extern void      Encoding_release (Encoding *encoding);

extern pdf_obj  *Encoding_get_resource (Encoding *encoding);

extern int       Encoding_is_ASL_charset (Encoding *encoding);
extern int       Encoding_is_predefined  (Encoding *encoding);

extern void      Encoding_set_name  (Encoding *encoding, char *name);
extern char     *Encoding_get_name  (Encoding *encoding);

extern void      Encoding_set_encoding (Encoding *encoding,
					char **encoding_vec, char *base_encname);
extern char    **Encoding_get_encoding (Encoding *encoding);

extern void      Encoding_cache_init  (void);
extern void      Encoding_cache_close (void);
extern Encoding *Encoding_cache_get   (int encoding_id);
extern int       Encoding_cache_find  (const char *enc_name);

/******************************* COMPATIBILITY ******************************/

extern void     encoding_set_verbose (void);

extern int      get_encoding       (const char *enc_name);
extern void     encoding_flush_all (void);
extern pdf_obj *encoding_ref       (int encoding_id);
extern char    *encoding_glyph     (int encoding_id, unsigned code);

#endif /* _ENCODINGS_H_ */
