/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/spc_pdfm.c,v 1.2 2005/07/03 20:02:29 csc Exp $

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

#include <ctype.h>

#include "system.h"
#include "mem.h"
#include "error.h"
#include "mfileio.h"

#include "numbers.h"

#include "dvi.h"
#include "fontmap.h"
#include "dpxutil.h"

#include "pdflimits.h"

#include "pdfobj.h"
#include "pdfparse.h"

#include "pdfdoc.h"

#include "pdfximage.h"
#include "pdfdraw.h"
#include "pdfcolor.h"
#include "pdfdev.h"

#include "specials.h"

#include "spc_util.h"
#include "spc_pdfm.h"

static pdf_obj *pending_annot_dict = NULL;
static int      lowest_level  = 255; /* current min level of outline */

/* PLEASE REMOVE THIS */
struct resource_map {
  int   type;
  int   res_id;
};
static struct ht_table *resourcemap = NULL;

static struct tounicode {
  int       cmap_id;
  pdf_obj  *taintkeys; /* An array of PDF names. */
} tounicode_data = {
  -1,
  NULL
};

int
spc_pdfm_at_begin_page (void)
{
  return 0;
}

int
spc_pdfm_at_end_page (void)
{
  return 0;
}

int
spc_pdfm_at_begin_document (void)
{
  static const char *default_taintkeys[] = {
    "Title",   "Author",   "Subject", "Keywords",
    "Creator", "Producer", "Contents", "Subj",
    "TU",      "T",        "TM",        NULL /* EOD */
  };
  int  i;

  pending_annot_dict = NULL;
  lowest_level       = 255;

  resourcemap = NEW(1, struct ht_table);
  ht_init_table(resourcemap);

  tounicode_data.taintkeys = pdf_new_array();
  for (i = 0; default_taintkeys[i] != NULL; i++) {
    pdf_add_array(tounicode_data.taintkeys,
		  pdf_new_name(default_taintkeys[i]));
  }

  return 0;
}

/* PLEASE REMOVE THIS */
static void CDECL
hval_free (void *hval)
{
  struct resource_map *map;

  map = (struct resource_map *) hval;
  RELEASE(map);

  return;
}

int
spc_pdfm_at_end_document (void)
{
  if (pending_annot_dict) {
    WARN("Unbalanced bann and eann found.");
    pdf_release_obj(pending_annot_dict);
  }
  lowest_level = 255;
  pending_annot_dict = NULL;

  if (resourcemap) {
    ht_clear_table(resourcemap, hval_free);
    RELEASE(resourcemap);
  }
  resourcemap = NULL;

  if (tounicode_data.taintkeys)
    pdf_release_obj(tounicode_data.taintkeys);
  tounicode_data.taintkeys = NULL;

  return 0;
}


static int
spc_handler_pdfm_bop (struct spc_env *spe, struct spc_arg *args)
{
  if (args->curptr < args->endptr) {
    pdf_doc_set_bop_content(args->curptr,
			    (long) (args->endptr - args->curptr));
  }

  args->curptr = args->endptr;

  return 0;
}

static int
spc_handler_pdfm_eop (struct spc_env *spe, struct spc_arg *args)
{
  if (args->curptr < args->endptr) {
    pdf_doc_set_eop_content(args->curptr,
			    (long) (args->endptr - args->curptr));
  }

  args->curptr = args->endptr;

  return 0;
}

/* Why should we have this kind of things?
 */
static int
pdf_colon_put_space_at_resources (struct spc_env *spe, struct spc_arg *args)
{
  pdf_obj  *dict;
  pdf_obj  *category, *resname;
  pdf_obj  *resource, *resources;
  pdf_obj  *catnames, *resnames;
  int       i, j, num_cats, num_ress;

  skip_white(&args->curptr, args->endptr);

  if (args->curptr >= args->endptr) {
    spe->errmsg = (char *) "Missing PDF object.";
    return -1;
  }

  dict = parse_pdf_dict(&args->curptr, args->endptr);
  if (!dict) {
    spe->errmsg = (char *) "Missing resource dictionary.";
    return -1;
  }

  catnames = pdf_dict_keys(dict);
  num_cats = pdf_array_length(catnames);
  for (i = 0; i < num_cats; i++) {
    category  = pdf_get_array(catnames, i);
    resources = pdf_lookup_dict(dict, pdf_name_value(category));
    if (PDF_OBJ_DICTTYPE(resources)) {
      resnames  = pdf_dict_keys(resources);
      num_ress  = pdf_array_length(resnames);
      for (j = 0; j < num_ress; j++) {
	resname  = pdf_get_array(resnames, j);
	resource = pdf_lookup_dict(resources, pdf_name_value(resname));
	pdf_doc_add_page_resource(pdf_name_value(category),
				  pdf_name_value(resname),
				  pdf_ref_obj(resource));
      }
      pdf_release_obj(resnames);
    } else if (PDF_OBJ_INDIRECTTYPE(resources)) { /* Can't handle properly */
      pdf_obj *tmp;

      tmp = pdf_new_dict();
      pdf_add_dict(tmp, pdf_link_obj(category), pdf_link_obj(resources));
      pdf_merge_dict(pdf_doc_current_page_resources(), tmp);
      pdf_release_obj(tmp);
    } else {
      WARN("Skipping invalid object for page resource...");
    }
  }
  pdf_release_obj(catnames);
  pdf_release_obj(dict);

  return 0;
}

/* Think what happens if user does
 *
 *  pdf:put @resources << /Font << >> >>
 * 
 */
static int
spc_handler_pdfm_put (struct spc_env *spe, struct spc_arg *args)
{
  pdf_obj  *obj;
  pdf_obj  *value;
  char     *ident;

  skip_white(&args->curptr, args->endptr);

  ident = parse_opt_ident(&args->curptr, args->endptr);
  if (!ident) {
    spe->errmsg = (char *) "Missing object reference identifier.";
    return -1;
  }

  if (!strcmp(ident, "resources")) {
    RELEASE(ident);
    return pdf_colon_put_space_at_resources(spe, args);
  }

  obj = spc_lookup_object(ident);
  if (!obj) {
    spe->errmsg = (char *) "Specified object not exist.";
    RELEASE(ident);
    return -1;
  }
  RELEASE(ident);

  skip_white(&args->curptr, args->endptr);

  switch (PDF_OBJ_TYPEOF(obj)) {
  case PDF_DICT:
    value = parse_pdf_dict(&args->curptr, args->endptr);
    if (value) {
      if (PDF_OBJ_DICTTYPE(value)) {
	pdf_merge_dict(obj, value);
      } else {
	spe->errmsg = (char *) "Invalid type: expecting a dictionary.";
	pdf_release_obj(value);
	return -1;
      }
      pdf_release_obj(value);
    } else {
      spe->errmsg = (char *) "Missing a PDF object.";
      return -1;
    }
    break;
  case PDF_STREAM:
    value = parse_pdf_object(&args->curptr, args->endptr);
    if (value) {
      pdf_obj *stream_dict;

      if (PDF_OBJ_DICTTYPE(value)) {
	stream_dict = pdf_stream_dict(obj);
	pdf_merge_dict(stream_dict, value);
      } else if (PDF_OBJ_STREAMTYPE(value)) {
	spe->errmsg = (char *) "PDF stream not supported yet.";
	pdf_release_obj(value);
	return -1;
      } else {
	spe->errmsg = (char *) "Invalid type: expecting a dictionary or stream.";
	pdf_release_obj(value);
	return -1;
      }
      pdf_release_obj(value);
    }
    break;
  case PDF_ARRAY:
    while (args->curptr < args->endptr) {
      value = parse_pdf_object(&args->curptr, args->endptr);
      if (!value) {
	break;
      }
      pdf_add_array(obj, value);
      skip_white(&args->curptr, args->endptr);
    }
    break;
  default:
    spe->errmsg = (char *) "Invalid destination object type.";
    return -1;
    break;
  }

  return 0;
}


static int
spc_handler_pdfm_pagesize (struct spc_env *spe, struct spc_arg *args)
{
  /* Handled at BOP */
  args->curptr = args->endptr;

  return 0;
}

#include "cmap.h"

static int
modified_string (CMap *cmap, pdf_obj *instring)
{
#define WBUF_SIZE 4096
  unsigned char  wbuf[WBUF_SIZE];
  unsigned char *obufcur;
  unsigned char *inbufcur;
  long inbufleft, obufleft;

  if (!cmap || !instring)
    return 0;

  inbufleft = pdf_string_length(instring);
  inbufcur  = pdf_string_value(instring);

  /* Unicode BOM might be found. */
  if (inbufleft > 1 &&
      *inbufcur == 0xfe && *(inbufcur+1) == 0xff)
    return 0;

  wbuf[0] = 0xfe; wbuf[1] = 0xff;
  obufcur   = wbuf + 2;
  obufleft  = WBUF_SIZE - 2;

  CMap_decode(cmap,
	      (const unsigned char **)&inbufcur, &inbufleft,
	      &obufcur, &obufleft);

  if (inbufleft > 0) {
    return -1;
  }

  pdf_set_string(instring, wbuf, WBUF_SIZE - obufleft);

  return 0;
}


static int modstrings (pdf_obj *key, pdf_obj *value, void *pdata);

static int CDECL
modstrings (pdf_obj *key, pdf_obj *value, void *pdata)
{
  int      retval = 0; /* continue */
  int      i, num_keys;
  CMap    *cmap;
  char    *key_str;
  pdf_obj *taintkey;
  struct tounicode *conv_data;

  if (!PDF_OBJ_NAMETYPE(key)) { /* Shouldn't happen */
    return -1;
  }

  conv_data = (struct tounicode *) pdata;

  if (!conv_data ||
      conv_data->cmap_id < 0 || !conv_data->taintkeys)
    return -1;

  if (PDF_OBJ_STRINGTYPE(value)) {
    num_keys = pdf_array_length(conv_data->taintkeys);
    key_str  = pdf_name_value(key);
    cmap     = CMap_cache_get (conv_data->cmap_id);
    for (i = 0; i < num_keys; i++) {
      taintkey = pdf_get_array(conv_data->taintkeys, i);
      if (taintkey &&
	  !strcmp(key_str, pdf_name_value(taintkey))) {
#if 0
	/* debug */
	WARN("Trying to modify string value of dictionary entry /%s.",
	     key_str);
#endif
	retval = modified_string(cmap, value);
	if (retval < 0) { /* error occured... */
	  WARN("Failed to convert input string to UTF16...");
	}
	break;
      }
    }
  } else if (PDF_OBJ_DICTTYPE(value)) {
    retval = pdf_foreach_dict(value, modstrings, pdata);
  }

  return retval;
}

static pdf_obj *
my_parse_pdf_dict (char **pp, char *endptr)
{
  pdf_obj  *dict;
  CMap     *cmap;
  char     *cmap_name;

  if (tounicode_data.cmap_id < 0)
    return parse_pdf_dict(pp, endptr);

  cmap = CMap_cache_get(tounicode_data.cmap_id);
  if (!cmap) { /* Should be error. */
    WARN("ToUnicode mapping not found: cmap_id=%d",
	 tounicode_data.cmap_id);
    return NULL;
  }

  cmap_name = CMap_get_name(cmap);
  /* Shift-JIS like encoding may contain backslash in 2nd byte. */
  if (strstr(cmap_name, "RKSJ") ||
      strstr(cmap_name, "B5")   ||
      strstr(cmap_name, "GBK")  ||
      strstr(cmap_name, "KSC")) {
    dict = parse_pdf_tainted_dict(pp, endptr);
  } else {
    dict = parse_pdf_dict(pp, endptr);
  }
  if (dict) {
    pdf_foreach_dict(dict, modstrings, &tounicode_data);
  }

  return dict;
}

static int
spc_handler_pdfm_annot (struct spc_env *spe, struct spc_arg *args)
{
  pdf_obj   *annot_dict;
  pdf_rect   rect;
  char      *ident;
  pdf_coord  cp;
  transform_info info;

  skip_white(&args->curptr, args->endptr);
  if (args->curptr[0] == '@') {
    ident = parse_opt_ident(&args->curptr, args->endptr);
  } else {
    ident = NULL;
  }

  transform_info_clear(&info);
  if (spc_util_read_dimension(&info, spe, args, 0) < 0) {
    if (ident)
      RELEASE(ident);
    return -1;
  }

  if ((info.flags & INFO_HAS_USER_BBOX) &&
      ((info.flags & INFO_HAS_WIDTH) || (info.flags & INFO_HAS_HEIGHT))) {
    spe->errmsg = (char *) "You can't specify both bbox and width/height.";
    if (ident)
      RELEASE(ident);
    return -1;
  }

  annot_dict = my_parse_pdf_dict(&args->curptr, args->endptr);
  if (!annot_dict) {
    spe->errmsg = (char *) "Could not find dictionary object.";
    return -1;
  } else if (!PDF_OBJ_DICTTYPE(annot_dict)) {
    spe->errmsg = (char *) "Invalid type: not dictionary object.";
    if (ident)
      RELEASE(ident);
    pdf_release_obj(annot_dict);
    return -1;
  }

  if (info.flags & INFO_HAS_USER_BBOX) {
    rect.llx = info.bbox.llx;
    rect.lly = info.bbox.lly;
    rect.urx = info.bbox.urx;
    rect.ury = info.bbox.ury;
  } else {
    cp.x = spe->x_user; cp.y = spe->y_user;
    pdf_dev_transform(&cp, NULL);
    rect.llx = cp.x;
    rect.lly = cp.y - spe->mag * info.depth;
    rect.urx = rect.llx + spe->mag * info.width;
    rect.ury = rect.lly + spe->mag * info.height;
  }

  /* Order is important... */
  if (ident)
    spc_push_object(ident, pdf_link_obj(annot_dict));
  /* This add reference. */
  pdf_doc_add_annot(pdf_doc_current_page_number(), &rect, annot_dict);

  if (ident) {
    spc_flush_object(ident);
    RELEASE(ident);
  }
  pdf_release_obj(annot_dict);

  return 0;
}

static int
spc_handler_pdfm_bann (struct spc_env *spe, struct spc_arg *args)
{
  if (pending_annot_dict) {
    spe->errmsg = (char *) "Can't begin an annotation when one is pending.";
    return -1;
  }

  skip_white(&args->curptr, args->endptr);
  pending_annot_dict = my_parse_pdf_dict(&args->curptr, args->endptr);
  if (!pending_annot_dict) {
    spe->errmsg = (char *) "Ignoring annotation with invalid dictionary.";
    return -1;
  } else if (!PDF_OBJ_DICTTYPE(pending_annot_dict)) {
    spe->errmsg = (char *) "Invalid type: not a dictionary object.";
    pdf_release_obj(pending_annot_dict);
    pending_annot_dict = NULL;
    return -1;
  }

  pdf_doc_begin_annot(pending_annot_dict);
  dvi_tag_depth(); /* Tell dvi interpreter to handle line-break. */

  /*
   * If this object has a named reference, we file it away for
   * later.  Otherwise we release it.
   */
  /* Did someone modified this leaving the above comment unchanged? */

  return 0;
}

static int
spc_handler_pdfm_eann (struct spc_env *spe, struct spc_arg *args)
{
  if (!pending_annot_dict) {
    spe->errmsg = (char *) "Tried to end an annotation without starting one!";
    return -1;
  }

  dvi_untag_depth();
  pdf_doc_end_annot();
  pdf_release_obj(pending_annot_dict);
  pending_annot_dict = NULL;

  return 0;
}

static int
spc_handler_pdfm_bgcolor (struct spc_env *spe, struct spc_arg *args)
{
  int       error;
  pdf_color colorspec;

  error = spc_util_read_colorspec(&colorspec, spe, args, 0);
  if (error) {
    spe->errmsg = (char *) "No valid color specified?";
  } else {
    pdf_color_set_bgcolor(&colorspec);
  }

  return error;
}

static int
spc_handler_pdfm_bcolor (struct spc_env *spe, struct spc_arg *args)
{
  int       error;
  pdf_color colorspec;

  error = spc_util_read_colorspec(&colorspec, spe, args, 0);
  if (error) {
    spe->errmsg = (char *) "No valid color specified?";
  } else {
    pdf_color_push(); /* save currentcolor */
    pdf_dev_setcolor(&colorspec, 0);
    pdf_dev_setcolor(&colorspec, 1);
  }

  return error;
}

static int
spc_handler_pdfm_scolor (struct spc_env *spe, struct spc_arg *args)
{
  int       error;
  pdf_color colorspec;

  error = spc_util_read_colorspec(&colorspec, spe, args, 0);
  if (error) {
    spe->errmsg = (char *) "No valid color specified?";
  } else {
    pdf_color_set_default(&colorspec);
    pdf_dev_setcolor(&colorspec, 0);
    pdf_dev_setcolor(&colorspec, 1);
  }

  return error;
}

static int
spc_handler_pdfm_ecolor (struct spc_env *spe, struct spc_arg *args)
{
  pdf_color_pop();

  return 0;
}

static int
spc_handler_pdfm_btrans (struct spc_env *spe, struct spc_arg *args)
{
  pdf_tmatrix      M;
  transform_info   info;

  transform_info_clear(&info);
  if (spc_util_read_dimension(&info, spe, args, 0) < 0) {
    return -1;
  }

  pdf_make_tmatrix(&M,
		   info.xscale, info.yscale,
		   info.rotate, spe->x_user, spe->y_user); 
  pdf_dev_gsave();
  pdf_dev_concat(&M);

  return 0;
}

static int
spc_handler_pdfm_etrans (struct spc_env *spe, struct spc_arg *args)
{
  pdf_dev_grestore();

  /*
   * Unfortunately, the following line is necessary in case
   * of a font or color change inside of the save/restore pair.
   * Anything that was done there must be redone, so in effect,
   * we make no assumptions about what fonts. We act like we are
   * starting a new page.
   */
  pdf_dev_reset();

  return 0;
}

static int
spc_handler_pdfm_tounicode (struct spc_env *spe, struct spc_arg *args)
{
  char    *cmap_name;
  pdf_obj *taintkeys, *tmp;
  int      num_keys;

  skip_white(&args->curptr, args->endptr);
  if (args->curptr >= args->endptr) {
    spe->errmsg = (char *) "No options supplied.";
    return -1;
  }

  if (args->curptr[0] == '/') {
    tmp = parse_pdf_object(&args->curptr, args->endptr);
    if (!tmp) {
      spe->errmsg = (char *) "Missing ToUnicode mapping name...";
      return -1;
    } else if (!PDF_OBJ_NAMETYPE(tmp)) {
      pdf_release_obj(tmp);
      spe->errmsg = (char *) "Missing ToUnicode mapping name...";
      return -1;
    }
    cmap_name = pdf_name_value(tmp);
  } else {
    tmp = NULL;
    cmap_name = parse_ident(&args->curptr, args->endptr);
    if (!cmap_name) {
      spe->errmsg = (char *) "Missing ToUnicode mapping name...";
      return -1;
    }
  }

  if (!strcmp(cmap_name, "null")) {
    tounicode_data.cmap_id = -1;
  } else {
    tounicode_data.cmap_id = CMap_cache_find(cmap_name);
  }
  if (tmp) {
    pdf_release_obj(tmp);
  } else {
    RELEASE(cmap_name);
  }

  skip_white(&args->curptr, args->endptr);
  if (args->curptr < args->endptr) {

    taintkeys = parse_pdf_object(&args->curptr, args->endptr);
    if (!taintkeys) {
      spe->errmsg = (char *) "Expecting PDF array object for key list.";
      return -1;
    } else if (PDF_OBJ_NULLTYPE(taintkeys)) {
      pdf_release_obj(taintkeys);
      if (tounicode_data.taintkeys)
	pdf_release_obj(tounicode_data.taintkeys);
      tounicode_data.taintkeys = NULL;
    } else if (!PDF_OBJ_ARRAYTYPE(taintkeys)) {
      pdf_release_obj(taintkeys);
      spe->errmsg = (char *) "Expecting PDF array object for key list.";
      return -1;
    }

    num_keys = pdf_array_length(taintkeys);
    while (num_keys-- > 0) {
      tmp = pdf_get_array(taintkeys, num_keys);
      if (!PDF_OBJ_NAMETYPE(tmp)) {
	pdf_release_obj(taintkeys);
	spe->errmsg = (char *) "Expecting PDF name object for keys.";
	return -1;
      }
    }
    if (tounicode_data.taintkeys)
      pdf_release_obj(tounicode_data.taintkeys);

    tounicode_data.taintkeys = taintkeys;

  }

  return 0;
}

static int
spc_handler_pdfm_outline (struct spc_env *spe, struct spc_arg *args)
{
  pdf_obj   *item_dict, *tmp;
  int        level, is_open;
  int        current_depth;

  skip_white(&args->curptr, args->endptr);

  tmp = parse_pdf_object(&args->curptr, args->endptr);
  if (!tmp) {
    spe->errmsg = (char *) "Missing number or something.";
    return -1;
  } else if (!PDF_OBJ_NUMBERTYPE(tmp)) {
    pdf_release_obj(tmp);
    spe->errmsg = (char *) "Expecting number for object level.";
    return -1;
  }

  item_dict = NULL;
  is_open   = -1;

  level = (int) pdf_number_value(tmp);
  pdf_release_obj(tmp);

  /*
   * What is this? Starting at level 3 and can go down to level 1?
   *
   * Here is the original comment:
   *  Make sure we know where the starting level is
   */
  lowest_level = MIN(lowest_level, level);

  level  +=  1 - lowest_level;

  skip_white(&args->curptr, args->endptr);
  if (args->curptr < args->endptr &&
      args->curptr[0] != '<') {
    char  *opt;
    /*
     * Should we use key-value (open true/open false) pair here?
     */
    opt = parse_ident(&args->curptr, args->endptr);
    if (!opt) {
      spe->errmsg = (char *) "Unknown option...";
      return -1;
    } else if (!strcmp(opt, "opened")) {
      is_open = 1;
    } else if (!strcmp(opt, "closed")) {
      is_open = 0;
    } else {
      spe->errmsg = (char *) "Unknown option...";
      RELEASE(opt);
      return -1;
    }
    RELEASE(opt);
  }

  item_dict = my_parse_pdf_dict(&args->curptr, args->endptr);
  if (!item_dict) {
    spe->errmsg = (char *) "Ignoring invalid dictionary.";
    return -1;
  }
  current_depth = pdf_doc_bookmarks_depth();
  if (current_depth > level) {
    while (current_depth-- > level)
      pdf_doc_bookmarks_up();
  } else if (current_depth < level) {
    while (current_depth++ < level)
      pdf_doc_bookmarks_down();
  }

  pdf_doc_bookmarks_add(item_dict, is_open);

  return 0;
}

static int
spc_handler_pdfm_article (struct spc_env *spe, struct spc_arg *args)
{
  char    *ident;
  pdf_obj *info_dict;

  skip_white (&args->curptr, args->endptr);

  ident = parse_opt_ident(&args->curptr, args->endptr);
  if (!ident) {
    spe->errmsg = (char *) "Article name expected.";
    return -1;
  }

  info_dict = my_parse_pdf_dict(&args->curptr, args->endptr);
  if (!info_dict) {
    RELEASE(ident);
    spe->errmsg = (char *) "Ignoring invalid dictionary.";
    return -1;
  }

  pdf_doc_begin_article(ident, pdf_link_obj(info_dict));
  spc_push_object(ident, info_dict);
  RELEASE(ident);

  return 0;
}

static int
spc_handler_pdfm_bead (struct spc_env *spe, struct spc_arg *args)
{
  pdf_obj       *article;
  pdf_obj       *article_info;
  char          *article_name;
  pdf_rect       rect;
  long           page_no;
  transform_info info;
  pdf_coord      cp;

  skip_white(&args->curptr, args->endptr);

  if (args->curptr[0] != '@') {
    spe->errmsg = (char *) "Article reference expected.";
    return -1;
  }

  article_name = parse_ident(&args->curptr, args->endptr);
  if (!article_name) {
    spe->errmsg = (char *) "Article reference expected.";
    return -1;
  }

  /* If okay so far, try to get a bounding box */
  transform_info_clear(&info);
  if (spc_util_read_dimension(&info, spe, args, 0) < 0) {
    RELEASE(article_name);
    return -1;
  }

  if ((info.flags & INFO_HAS_USER_BBOX) &&
      ((info.flags & INFO_HAS_WIDTH) || (info.flags & INFO_HAS_HEIGHT))) {
    spe->errmsg = (char *) "You can't specify both bbox and width/height.";
    RELEASE(article_name);
    return -1;
  }

  if (info.flags & INFO_HAS_USER_BBOX) {
    rect.llx = info.bbox.llx;
    rect.lly = info.bbox.lly;
    rect.urx = info.bbox.urx;
    rect.ury = info.bbox.ury;
  } else {
    cp.x = spe->x_user; cp.y = spe->y_user;
    pdf_dev_transform(&cp, NULL);
    rect.llx = cp.x;
    rect.lly = cp.y - spe->mag * info.depth;
    rect.urx = rect.llx + spe->mag * info.width;
    rect.ury = rect.lly + spe->mag * info.height;
  }

  skip_white(&args->curptr, args->endptr);
  if (args->curptr[0] != '<') {
    article_info = pdf_new_dict();
  } else {
    article_info = my_parse_pdf_dict(&args->curptr, args->endptr);
    if (!article_info) {
      spe->errmsg = (char *) "Error in dictionary.";
      RELEASE(article_name);
      return -1;
    }
  }

  /* Does this article exist yet */
  article = spc_lookup_object(article_name);
  if (article) {
    pdf_merge_dict (article, article_info);
    pdf_release_obj(article_info);
  } else {
    pdf_doc_begin_article(article_name, pdf_link_obj(article_info));
    spc_push_object(article_name, article_info);
  }
  page_no = pdf_doc_current_page_number();
  pdf_doc_add_bead(article_name, NULL, page_no, &rect);

  return 0;
}

static int
spc_handler_pdfm_image (struct spc_env *spe, struct spc_arg *args)
{
  int            xobj_id;
  char          *ident;
  pdf_obj       *fspec;
  transform_info info;
  struct resource_map *rmap;

  skip_white(&args->curptr, args->endptr);
  if (args->curptr[0] == '@') {
    ident = parse_opt_ident(&args->curptr, args->endptr);
    /* PLEASE REMOVE THIS */
    rmap  = ht_lookup_table(resourcemap, ident, strlen(ident));
    if (rmap) {
      spe->errmsg = (char *) "Duplicated reference name for image found.";
      RELEASE(ident);
      return -1;
    }
  } else {
    ident = NULL;
  }

  transform_info_clear(&info);
  if (spc_util_read_dimension(&info, spe, args, 0) < 0) {
    if (ident)
      RELEASE(ident);
    return -1;
  }

  skip_white(&args->curptr, args->endptr);
  fspec = parse_pdf_object(&args->curptr, args->endptr);
  if (!fspec) {
    spe->errmsg = (char *) "Missing filename string.";
    if (ident)
      RELEASE(ident);
    return -1;
  } else if (!PDF_OBJ_STRINGTYPE(fspec)) {
    spe->errmsg = (char *) "Missing filename string.";
    pdf_release_obj(fspec);
    return -1;
  }

  xobj_id = pdf_ximage_findresource(pdf_string_value(fspec));
  if (xobj_id < 0) {
    spe->errmsg = (char *) "Could not find image resource.";
    pdf_release_obj(fspec);
    if (ident)
      RELEASE(ident);
    return -1;
  }

  pdf_dev_put_image(xobj_id, &info, spe->x_user, spe->y_user);

  if (ident) {
    /* PLEASE REMOVE THIS */
    rmap = NEW(1, struct resource_map);
    rmap->type   = 0;
    rmap->res_id = xobj_id;
    spc_push_object(ident, pdf_ximage_get_reference(xobj_id));
    ht_append_table(resourcemap, ident, strlen(ident), rmap);
    RELEASE(ident);
  }

  pdf_release_obj(fspec);

  return 0;
}

/* Use do_names instead. */
static int
spc_handler_pdfm_dest (struct spc_env *spe, struct spc_arg *args)
{
  pdf_obj *name;
  pdf_obj *array;

  skip_white(&args->curptr, args->endptr);

  name = parse_pdf_object(&args->curptr, args->endptr);
  if (!name) {
    spe->errmsg = (char *) "PDF string expected but not found.";
    return -1;
  } else if (!PDF_OBJ_STRINGTYPE(name)) {
    spe->errmsg = (char *) "PDF string expected but not found.";
    pdf_release_obj(name);
    return -1;
  }

  array = parse_pdf_object(&args->curptr, args->endptr);
  if (!array) {
    spe->errmsg = (char *) "Destination not specified.";
    pdf_release_obj(name);
    return -1;
  }  else if (!PDF_OBJ_ARRAYTYPE(array)) {
    spe->errmsg = (char *) "Destination must be an array.";
    pdf_release_obj(name);
    pdf_release_obj(array);
    return -1;
  }

  if (pdf_doc_add_names("Dests",
			pdf_string_value (name),
			pdf_string_length(name), array) < 0) {
    spe->errmsg = (char *) "Failed to add Dest entry...";
    pdf_release_obj(array);
    pdf_release_obj(name);
    return -1;
  }
  pdf_release_obj(name);

  return 0;
}

static int
spc_handler_pdfm_names (struct spc_env *spe, struct spc_arg *args)
{
  pdf_obj *category, *key, *value, *tmp;
  int      i, size;

  category = parse_pdf_object(&args->curptr, args->endptr);
  if (!category) {
    spe->errmsg = (char *) "PDF name expected but not found.";
    return -1;
  } else if (!PDF_OBJ_NAMETYPE(category)) {
    spe->errmsg = (char *) "PDF name expected but not found.";
    pdf_release_obj(category);
    return -1;
  }

  tmp = parse_pdf_object(&args->curptr, args->endptr);
  if (!tmp) {
    spe->errmsg = (char *) "PDF object expected but not found.";
    pdf_release_obj(category);
    return -1;
  } else if (PDF_OBJ_ARRAYTYPE(tmp)) {
    size = pdf_array_length(tmp);
    if (size % 2 != 0) {
      spe->errmsg = (char *) "Array size not multiple of 2.";
      pdf_release_obj(category);
      pdf_release_obj(tmp);
      return -1;
    }

    for (i = 0; i < size / 2; i++) {
      key   = pdf_get_array(tmp, 2 * i);
      value = pdf_get_array(tmp, 2 * i + 1);
      if (!PDF_OBJ_STRINGTYPE(key)) {
	spe->errmsg = (char *) "Name tree key must be string.";
	pdf_release_obj(category);
	pdf_release_obj(tmp);
	return -1;
      } else if (pdf_doc_add_names(pdf_name_value(category),
				   pdf_string_value (key),
				   pdf_string_length(key),
				   pdf_link_obj(value)) < 0) {
	spe->errmsg = (char *) "Failed to add Name tree entry...";
	pdf_release_obj(category);
	pdf_release_obj(tmp);
	return -1;
      }
    }
    pdf_release_obj(tmp);
  } else if (PDF_OBJ_STRINGTYPE(tmp)) {
    key   = tmp;
    value = parse_pdf_object(&args->curptr, args->endptr);
    if (!value) {
      pdf_release_obj(category);
      pdf_release_obj(key);
      spe->errmsg = (char *) "PDF object expected but not found.";
      return -1;
    }
    if (pdf_doc_add_names(pdf_name_value(category),
			  pdf_string_value (key),
			  pdf_string_length(key),
			  value) < 0) {
      spe->errmsg = (char *) "Failed to add Name tree entry...";
      pdf_release_obj(category);
      pdf_release_obj(key);
      return -1;
    }
    pdf_release_obj(key);
  } else {
    pdf_release_obj(tmp);
    pdf_release_obj(category);
    spe->errmsg = (char *) "Invalid object type.";
    return -1;
  }

  pdf_release_obj(category);

  return 0;
}

static int
spc_handler_pdfm_mapline (struct spc_env *spe, struct spc_arg *args)
{
  fontmap_rec *mrec;
  char        *map_name;
  long         len;

  skip_white(&args->curptr, args->endptr);
  if (args->curptr >= args->endptr)
    return -1;

  len = (long) (args->endptr - args->curptr);
  switch (args->curptr[0]) {
  case '-':
    args->curptr++;
    map_name = parse_ident(&args->curptr, args->endptr);
    if (map_name) {
      pdf_remove_fontmap_record(map_name);
      RELEASE(map_name);
      args->curptr = args->endptr;
    } else {
      spe->errmsg = (char *) "Missing font name.";
      return -1;
    }
    break;
  case '+':
    args->curptr++;
    len--;
    mrec = NEW(1, fontmap_rec);
    pdf_init_fontmap_record(mrec);
    if (pdf_read_fontmap_line(mrec, args->curptr, len) < 0) {
      pdf_clear_fontmap_record(mrec);
      RELEASE(mrec);
      spe->errmsg = (char *) "Invalid fontmap entry.";
      return -1;
    } else {
      pdf_append_fontmap_record(mrec);
      args->curptr = args->endptr;
    }
    break;
  default:
    mrec = NEW(1, fontmap_rec);
    pdf_init_fontmap_record(mrec);
    if (pdf_read_fontmap_line(mrec, args->curptr, len) < 0) {
      pdf_clear_fontmap_record(mrec);
      RELEASE(mrec);
      spe->errmsg = (char *) "Invalid fontmap entry.";
      return -1;
    } else {
      pdf_insert_fontmap_record(mrec);
      args->curptr = args->endptr;
    }
    break;
  }

  return 0;
}

static int
spc_handler_pdfm_mapfile (struct spc_env *spe, struct spc_arg *args)
{
  char  *mapfile;
  int    mode;

  skip_white(&args->curptr, args->endptr);
  if (args->curptr >= args->endptr)
    return 0;

  switch (args->curptr[0]) {
  case '-':
    mode = FONTMAP_RMODE_REMOVE;
    args->curptr++;
    break;
  case '+':
    mode = FONTMAP_RMODE_APPEND;
    args->curptr++;
    break;
  default:
    mode = FONTMAP_RMODE_REPLACE;
    break;
  }

  mapfile = parse_ident(&args->curptr, args->endptr);
  if (!mapfile) {
    spe->errmsg = (char *) "No fontmap specified.";
    return -1;
  } else {
    pdf_load_fontmap_file(mapfile, mode);
  }

  return 0;
}

static int
spc_handler_pdfm_docinfo (struct spc_env *spe, struct spc_arg *args)
{
  pdf_obj *docinfo, *dict;

  dict = my_parse_pdf_dict(&args->curptr, args->endptr);
  if (!dict) {
    spe->errmsg = (char *) "Dictionary expected but not found.";
    return -1;
  }

  docinfo = pdf_doc_docinfo();
  pdf_merge_dict(docinfo, dict);
  pdf_release_obj(dict);

  return 0;
}

static int
spc_handler_pdfm_docview (struct spc_env *spe, struct spc_arg *args)
{
  pdf_obj *catalog, *docview;
  pdf_obj *pref_old, *pref_add;

  docview = my_parse_pdf_dict(&args->curptr, args->endptr);
  if (!docview) {
    spe->errmsg = (char *) "Dictionary expected but not found.";
    return -1;
  }

  catalog  = pdf_doc_catalog();
  /* Avoid overriding whole ViewerPreferences */
  pref_old = pdf_lookup_dict(catalog, "ViewerPreferences");
  pref_add = pdf_lookup_dict(docview, "ViewerPreferences");
  if (pref_old && pref_add) {
    pdf_merge_dict (pref_old, pref_add);
    pdf_remove_dict(docview, "ViewerPreferences");
  }
  pdf_merge_dict (catalog, docview);
  pdf_release_obj(docview);

  return 0;
}

static int
spc_handler_pdfm_close (struct spc_env *spe, struct spc_arg *args)
{
  char *ident;

  skip_white(&args->curptr, args->endptr);
  ident = parse_opt_ident(&args->curptr, args->endptr);
  if (ident) {
    spc_flush_object(ident);
    RELEASE(ident);
  } else { /* Close all? */
    spc_clear_objects();
  }

  return 0;
}

static int
spc_handler_pdfm_object (struct spc_env *spe, struct spc_arg *args)
{
  char    *ident;
  pdf_obj *object;

  skip_white(&args->curptr, args->endptr);
  ident = parse_opt_ident(&args->curptr, args->endptr);
  if (!ident) {
    spe->errmsg = (char *) "Could not find a reference name.";
    return -1;
  }

  object = parse_pdf_object(&args->curptr, args->endptr);
  if (!object) {
    RELEASE(ident);
    spe->errmsg = (char *) "Could not find an object.";
    return -1;
  } else {
    spc_push_object(ident, object);
  }
  RELEASE(ident);

  return 0;
}

static int
spc_handler_pdfm_content (struct spc_env *spe, struct spc_arg *args)
{
  long  len;

  skip_white(&args->curptr, args->endptr);
  if (args->curptr < args->endptr) {
    len = sprintf(work_buffer, " q 1 0 0 1 %.2f %.2f cm ", spe->x_user, spe->y_user);
    pdf_doc_add_page_content(work_buffer, len);
    len = (long) (args->endptr - args->curptr);
    pdf_doc_add_page_content(args->curptr, len);
    pdf_doc_add_page_content(" Q", 2);
  }
  args->curptr = args->endptr;

  return 0;
}

static int
spc_handler_pdfm_literal (struct spc_env *spe, struct spc_arg *args)
{
  int       direct = 0;
  pdf_coord cp;

  cp.x = spe->x_user; cp.y = spe->y_user;
  pdf_dev_transform(&cp, NULL);

  skip_white(&args->curptr, args->endptr);
  while (args->curptr < args->endptr) {
    if (args->curptr + 7 <= args->endptr &&
	!strncmp(args->curptr, "reverse", 7)) {
      cp.x *= -1.0; cp.y *= -1.0;
      args->curptr += 7;
    } else if (args->curptr + 6 <= args->endptr &&
	       !strncmp(args->curptr, "direct", 6)) {
      direct      = 1;
      args->curptr += 6;
    } else {
      break;
    }
    skip_white(&args->curptr, args->endptr);
  }

  skip_white(&args->curptr, args->endptr);

#if 0
  /* THIS IS BROKEN */
  /* This breaks the assumption that x_user and y_user representing
   * current position in user-space. All subsequent drawing operation
   * will not work correctly if you do translation here.
   */
  if (!direct)
    pdf_dev_set_origin(cp.x, cp.y);
#endif 
  pdf_doc_add_page_content(" ", 1);

  if (args->curptr < args->endptr) {
    pdf_doc_add_page_content(args->curptr, (long) (args->endptr - args->curptr));
  }

  args->curptr = args->endptr;

  return 0;
}

/*
 * FSTREAM: Create a PDF stream object from an existing file.
 *
 *  pdf: fstream @objname (filename) [PDF_DICT]
 */
static int
spc_handler_pdfm_fstream (struct spc_env *spe, struct spc_arg *args)
{
  pdf_obj *fstream;
  long     stream_len, nb_read;
  char    *ident, *filename, *fullname;
  pdf_obj *tmp;
  FILE    *fp;

  skip_white(&args->curptr, args->endptr);

  ident = parse_opt_ident(&args->curptr, args->endptr);
  if (!ident) {
    spe->errmsg = (char *) "Could not find a reference name.";
    return -1;
  }

  skip_white(&args->curptr, args->endptr);

  tmp = parse_pdf_object(&args->curptr, args->endptr);
  if (!tmp) {
    spe->errmsg = (char *) "Missing filename string.";
    RELEASE(ident);
    return -1;
  } else if (!PDF_OBJ_STRINGTYPE(tmp)) {
    spe->errmsg = (char *) "Missing filename string.";
    pdf_release_obj(tmp);
    RELEASE(ident);
    return -1;
  }

  filename = pdf_string_value(tmp);
  if (!filename) {
    spe->errmsg = (char *) "Missing filename.";
    RELEASE(ident);
    return -1;
  }

  MESG("(FILE:%s", filename);

  fstream  = pdf_new_stream(STREAM_COMPRESS);

#ifdef MIKTEX
  if (!miktex_find_app_input_file("dvipdfm", filename, work_buffer))
    fullname = NULL;
  else {
    fullname = work_buffer;
  }
#else /* !MIKTEX */
  fullname = kpse_find_file(filename, kpse_program_binary_format, 0);
#endif
  pdf_release_obj(tmp);
  if (!fullname) {
    spe->errmsg = (char *) "Could not find file.";
    RELEASE(ident);
    pdf_release_obj(fstream);
    return -1;
  }

  fp = MFOPEN(fullname, FOPEN_RBIN_MODE);
  if (!fp) {
    spe->errmsg = (char *) "Could not open file.";
    RELEASE(ident);
    pdf_release_obj(fstream);
    return -1;
  }

  stream_len = 0;
  while ((nb_read =
	  fread(work_buffer, sizeof(char), WORK_BUFFER_SIZE, fp)) > 0) {
    pdf_add_stream(fstream, work_buffer, nb_read);
    stream_len += nb_read;
  }

  MFCLOSE(fp);

  /*
   * Optional dict.
   *
   *  TODO: check Length, Filter...
   */
  skip_white(&args->curptr, args->endptr);

  if (args->curptr[0] == '<') {
    pdf_obj *stream_dict;

    stream_dict = pdf_stream_dict(fstream);

    tmp = parse_pdf_dict(&args->curptr, args->endptr);
    if (!tmp) {
      spe->errmsg = (char *) "Parsing dictionary failed.";
      RELEASE(ident);
      pdf_release_obj(fstream);
      return -1;
    }
    if (pdf_lookup_dict(tmp, "Length")) {
      WARN("Please don't use /Length key...");
      pdf_remove_dict(tmp, "Length");
    } else if (pdf_lookup_dict(tmp, "Filter")) {
      WARN("Please don't use /Filter key...");
      pdf_remove_dict(tmp, "Filter");
    }
    pdf_merge_dict(stream_dict, tmp);
    pdf_release_obj(tmp);
  }

  /* Users should explicitly close this. */
  spc_push_object(ident, fstream);
  RELEASE(ident);

  MESG(")");

  return 0;
}

/* Grab page content */
static int
spc_handler_pdfm_bform (struct spc_env *spe, struct spc_arg *args)
{
  int             xobj_id;
  char           *ident;
  pdf_rect        cropbox;
  transform_info  info;

  skip_white(&args->curptr, args->endptr);

  ident = parse_opt_ident(&args->curptr, args->endptr);
  if (!ident) {
    spe->errmsg = (char *) "A form XObject must have name.";
    return -1;
  }

  transform_info_clear(&info);
  if (spc_util_read_dimension(&info, spe, args, 0) < 0) {
    RELEASE(ident);
    return -1;
  }

#if 0
  /* The following code can't be removed.
   *
   * A XForm with zero dimension result in a non-invertible transformation
   * matrix. And it may result in unpredictable behaviour. It might be an
   * error in Acrobat. Bounding box with zero dimention may cause division
   * by zero.
   */
  /* Removed because ConTeXt sometimes uses an empty object. */
#endif
#if 1
  if (info.width == 0.0 ||
      info.depth + info.height == 0.0) {
    spe->errmsg = (char *) "Bounding box has a zero dimension.";
    RELEASE(ident);
    return -1;
  }
#endif

  if (info.flags & INFO_HAS_USER_BBOX) {
    cropbox.llx = info.bbox.llx;
    cropbox.lly = info.bbox.lly;
    cropbox.urx = info.bbox.urx;
    cropbox.ury = info.bbox.ury;
  } else {
    cropbox.llx = spe->x_user;
    cropbox.lly = spe->y_user - info.depth;
    cropbox.urx = spe->x_user + info.width;
    cropbox.ury = spe->y_user + info.height;
  }

  xobj_id = pdf_doc_begin_grabbing(ident, spe->x_user, spe->y_user, &cropbox);
  if (xobj_id < 0) {
    RELEASE(ident);
    spe->errmsg = (char *) "Couldn't start form object.";
    return -1;
  }

  spc_push_object(ident, pdf_ximage_get_reference(xobj_id));
  RELEASE(ident);

  return 0;
}

static int
spc_handler_pdfm_eform (struct spc_env *spe, struct spc_arg *args)
{
  pdf_obj   *garbage;
  pdf_obj   *resource;

  skip_white(&args->curptr, args->endptr);

  if (args->curptr < args->endptr) {
    /* An extra dictionary after exobj must be merged to /Resources???
     * Please use pdf:put @resources (before pdf:exobj) instead.
     */
    garbage = parse_pdf_dict(&args->curptr, args->endptr);
    if (garbage) {
      resource = pdf_doc_current_page_resources();
      pdf_merge_dict(resource, garbage);
      WARN("Garbage after special pdf:exobj.");
      pdf_release_obj(garbage);
    }
  }
  pdf_doc_end_grabbing();

  return 0;
}

static int
spc_handler_pdfm_uxobj (struct spc_env *spe, struct spc_arg *args)
{
  int            xobj_id;
  char          *ident;
  transform_info info;
  struct resource_map *rmap;

  skip_white(&args->curptr, args->endptr);

  ident = parse_opt_ident(&args->curptr, args->endptr);
  if (!ident) {
    spe->errmsg = (char *) "No XObject specified.";
    return -1;
  }

  transform_info_clear(&info);
  if (args->curptr < args->endptr) {
    if (spc_util_read_dimension(&info, spe, args, 0) < 0) {
      RELEASE(ident);
      return -1;
    }
  }

  /* Dvipdfmx was suddenly changed to use file name to identify
   * external images. We can't use ident to find image resource
   * here.
   */
  rmap = ht_lookup_table(resourcemap, ident, strlen(ident));
  if (rmap) {
    xobj_id = rmap->res_id;
  } else {
    xobj_id = pdf_ximage_findresource(ident);
  }

  if (xobj_id < 0) {
    RELEASE(ident);
    spe->errmsg = (char *) "Specified object doesn't exist.";
    return -1;
  }

  pdf_dev_put_image(xobj_id, &info, spe->x_user, spe->y_user);
  RELEASE(ident);

  return 0;
}

static int
spc_handler_pdfm_link (struct spc_env *spe, struct spc_arg *args)
{
  dvi_link_annot(1);

  return 0;
}

static int
spc_handler_pdfm_nolink (struct spc_env *spe, struct spc_arg *args)
{
  dvi_link_annot(0);

  return 0;
}

/* Load image but not place. */
static int
spc_handler_pdfm_loadimage (struct spc_env *spe, struct spc_arg *args)
{
  int      xobj_id;
  char    *ident;
  pdf_obj *fspec;
  struct resource_map *rmap;

  skip_white(&args->curptr, args->endptr);

  ident = parse_opt_ident(&args->curptr, args->endptr);
  if (!ident) {
    spe->errmsg = (char *) "Resource identifier not given.";
    return -1;
  }

  /* PLEASE REMOVE THIS */
  rmap  = ht_lookup_table(resourcemap, ident, strlen(ident));
  if (rmap) {
    spe->errmsg = (char *) "Duplicated reference name for resource found.";
    RELEASE(ident);
    return -1;
  }

  skip_white(&args->curptr, args->endptr);
  fspec = parse_pdf_object(&args->curptr, args->endptr);
  if (!fspec) {
    spe->errmsg = (char *) "Missing resource filename string.";
    RELEASE(ident);
    return -1;
  } else if (!PDF_OBJ_STRINGTYPE(fspec)) {
    spe->errmsg = (char *) "Missing resource filename string.";
    pdf_release_obj(fspec);
    RELEASE(ident);
    return -1;
  }

  xobj_id = pdf_ximage_findresource(pdf_string_value(fspec));
  pdf_release_obj(fspec);
  if (xobj_id < 0) {
    spe->errmsg = (char *) "Could not load/find image resource.";
    RELEASE(ident);
    return -1;
  }

  /* PLEASE REMOVE THIS */
  rmap = NEW(1, struct resource_map);
  rmap->type   = 0;
  rmap->res_id = xobj_id;
  ht_append_table(resourcemap, ident, strlen(ident), rmap);
  spc_push_object(ident, pdf_ximage_get_reference(xobj_id));

  RELEASE(ident);

  return 0;
}

static int
load_colorspace (struct spc_env *spe, struct spc_arg *args,
		 const char *ident, pdf_obj *resourcedef)
{
  int      res_id;
  pdf_obj *subtype;
  pdf_obj *fspec;
  struct resource_map *rmap;

  if (!PDF_OBJ_ARRAYTYPE(resourcedef) ||
      pdf_array_length(resourcedef) < 2) {
    spe->errmsg = (char *) "Invalid value for resource definition.";
    return -1;
  }

  subtype = pdf_get_array(resourcedef, 0);
  if (!subtype || !PDF_OBJ_NAMETYPE(subtype) ||
      strcmp("ICCBased", pdf_name_value(subtype))) {
    spe->errmsg = (char *) "Only /ICCBased currently supported.";
    return -1;
  }

  fspec = pdf_get_array(resourcedef, 1);
  if (!fspec || !PDF_OBJ_STRINGTYPE(fspec)) {
    spe->errmsg = (char *) "Missing resource filename.";
    return -1;
  }

  res_id = pdf_colorspace_load_ICCBased(ident, pdf_string_value(fspec));
  if (res_id < 0) {
    spe->errmsg = (char *) "Could not load/find resource.";
    return -1;
  }

  /* PLEASE REMOVE THIS */
  rmap = NEW(1, struct resource_map);
  rmap->type   = 1;
  rmap->res_id = res_id;
  ht_append_table(resourcemap, ident, strlen(ident), rmap);
  spc_push_object(ident, pdf_get_colorspace_reference(res_id));

  return 0;
}

/* Load resources
 *
 *  pdf:load @ident /Category resource_definition
 *
 *   pdf:load @foo /ColorSpace [/ICCBased (filename)]
 */
static int
spc_handler_pdfm_load (struct spc_env *spe, struct spc_arg *args)
{
  int      error = 0;
  char    *ident;
  pdf_obj *category, *resourcedef;

  skip_white(&args->curptr, args->endptr);

  ident = parse_opt_ident(&args->curptr, args->endptr);
  if (!ident) {
    spe->errmsg = (char *) "Resource identifier not given.";
    return -1;
  }

  /* PLEASE REMOVE THIS */
  if (ht_lookup_table(resourcemap, ident, strlen(ident))) {
    spe->errmsg = (char *) "Duplicated reference name for resource found.";
    RELEASE(ident);
    return -1;
  }

  category = parse_pdf_object(&args->curptr, args->endptr);
  if (!category) {
    spe->errmsg = (char *) "Missing resource category name.";
    RELEASE(ident);
    return -1;
  } else if (!PDF_OBJ_NAMETYPE(category)) {
    spe->errmsg = (char *) "Missing resource category name.";
    pdf_release_obj(category);
    RELEASE(ident);
    return -1;
  }

  resourcedef = parse_pdf_object(&args->curptr, args->endptr);
  if (!resourcedef) {
    spe->errmsg = (char *) "No resource definition found.";
    pdf_release_obj(category);
    RELEASE(ident);
    return -1;
  }

  if (!strcmp("ColorSpace", pdf_name_value(category))) {
    error = load_colorspace(spe, args, ident, resourcedef);
  } else {
    spe->errmsg = (char *) "Only resource category /ColorSpace supported.";
    error = -1;
  }
  pdf_release_obj(resourcedef);
  pdf_release_obj(category);

  RELEASE(ident);

  return error;
}

static struct spc_handler pdfm_handlers[] = {
  {"annotation", spc_handler_pdfm_annot},
  {"annotate",   spc_handler_pdfm_annot},
  {"annot",      spc_handler_pdfm_annot},
  {"ann",        spc_handler_pdfm_annot},

  {"outline",    spc_handler_pdfm_outline},
  {"out",        spc_handler_pdfm_outline},

  {"article",    spc_handler_pdfm_article},
  {"art",        spc_handler_pdfm_article},

  {"bead",       spc_handler_pdfm_bead},
  {"thread",     spc_handler_pdfm_bead},

  {"destination", spc_handler_pdfm_dest}, 
  {"dest",        spc_handler_pdfm_dest},


  {"object",      spc_handler_pdfm_object},
  {"obj",         spc_handler_pdfm_object},


  {"docinfo",     spc_handler_pdfm_docinfo},
  {"docview",     spc_handler_pdfm_docview},

  {"content",     spc_handler_pdfm_content},
  {"put",         spc_handler_pdfm_put},
  {"close",       spc_handler_pdfm_close},
  {"bop",         spc_handler_pdfm_bop},
  {"eop",         spc_handler_pdfm_eop},

  {"image",       spc_handler_pdfm_image},
  {"img",         spc_handler_pdfm_image},
  {"epdf",        spc_handler_pdfm_image},

  {"link",        spc_handler_pdfm_link},
  {"nolink",      spc_handler_pdfm_nolink},

  {"begincolor",  spc_handler_pdfm_bcolor},
  {"bcolor",      spc_handler_pdfm_bcolor},
  {"bc",          spc_handler_pdfm_bcolor},

  {"setcolor",    spc_handler_pdfm_scolor},
  {"scolor",      spc_handler_pdfm_scolor},
  {"sc",          spc_handler_pdfm_scolor},

  {"endcolor",    spc_handler_pdfm_ecolor},
  {"ecolor",      spc_handler_pdfm_ecolor},
  {"ec",          spc_handler_pdfm_ecolor},

  {"begingray",   spc_handler_pdfm_bcolor},
  {"bgray",       spc_handler_pdfm_bcolor},
  {"bg",          spc_handler_pdfm_bcolor},

  {"endgray",     spc_handler_pdfm_ecolor},
  {"egray",       spc_handler_pdfm_ecolor},
  {"eg",          spc_handler_pdfm_ecolor},

  {"bgcolor",     spc_handler_pdfm_bgcolor},
  {"bgc",         spc_handler_pdfm_bgcolor},
  {"bbc",         spc_handler_pdfm_bgcolor},
  {"bbg",         spc_handler_pdfm_bgcolor},

  {"pagesize",    spc_handler_pdfm_pagesize},

  {"bannot",      spc_handler_pdfm_bann},
  {"beginann",    spc_handler_pdfm_bann},
  {"bann",        spc_handler_pdfm_bann},

  {"eannot",      spc_handler_pdfm_eann},
  {"endann",      spc_handler_pdfm_eann},
  {"eann",        spc_handler_pdfm_eann},

  {"btrans",         spc_handler_pdfm_btrans},
  {"begintransform", spc_handler_pdfm_btrans},
  {"begintrans",     spc_handler_pdfm_btrans},
  {"bt",             spc_handler_pdfm_btrans},

  {"etrans",         spc_handler_pdfm_etrans},
  {"endtransform",   spc_handler_pdfm_etrans},
  {"endtrans",       spc_handler_pdfm_etrans},
  {"et",             spc_handler_pdfm_etrans},

  {"bform",          spc_handler_pdfm_bform},
  {"beginxobj",      spc_handler_pdfm_bform},
  {"bxobj",          spc_handler_pdfm_bform},

  {"eform",          spc_handler_pdfm_eform},
  {"endxobj",        spc_handler_pdfm_eform},
  {"exobj",          spc_handler_pdfm_eform},

  {"usexobj",        spc_handler_pdfm_uxobj},
  {"uxobj",          spc_handler_pdfm_uxobj},

  {"tounicode",  spc_handler_pdfm_tounicode},
  {"literal",    spc_handler_pdfm_literal},
  {"fstream",    spc_handler_pdfm_fstream},
  {"names",      spc_handler_pdfm_names},
  {"mapline",    spc_handler_pdfm_mapline},
  {"mapfile",    spc_handler_pdfm_mapfile},

  {"loadimage",  spc_handler_pdfm_loadimage},
  {"load",       spc_handler_pdfm_load}
};

int
spc_pdfm_check_special (const char *buffer, long size)
{
  char  *p, *endptr;

  p      = (char *) buffer;
  endptr = p + size;

  skip_white(&p, endptr);

  if (p + strlen("pdf:") <= endptr &&
      !strncmp(p, "pdf:", strlen("pdf:"))) {
    return 1;
  }

  return 0;
}

int
spc_pdfm_setup_handler (struct spc_handler *handle,
			struct spc_env *spe, struct spc_arg *args)
{
  char  *key;
  int    i, keylen;

  ASSERT(handle && spe && args);

  skip_white(&args->curptr, args->endptr);
  if (args->curptr + strlen("pdf:") > args->endptr ||
      strncmp(args->curptr, "pdf:", strlen("pdf:"))) {
    spe->errmsg = (char *) "No pdf special found.";
    return -1;
  }

  args->curptr += strlen("pdf:");

  skip_white(&args->curptr, args->endptr);

  key = args->curptr;
  while (args->curptr < args->endptr &&
	 isalpha(args->curptr[0])) {
    args->curptr++;
  }

  keylen = (int) (args->curptr - key);
  if (keylen < 1) {
    spe->errmsg = (char *) "No pdf special command found.";
    return -1;
  }

  for (i = 0;
       i < sizeof(pdfm_handlers) / sizeof(struct spc_handler); i++) {
    if (keylen == strlen(pdfm_handlers[i].key) &&
	!strncmp(key, pdfm_handlers[i].key, keylen)) {

      skip_white(&args->curptr, args->endptr);

      args->command = (char *) pdfm_handlers[i].key;
      handle->key   = (char *) "pdf:";
      handle->exec  = pdfm_handlers[i].exec;

      return 0;
    }
  }

  return -1;
}
