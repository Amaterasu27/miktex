/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/pdfximage.c,v 1.2 2005/07/03 20:02:29 csc Exp $
    
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
#include <math.h>
#include <string.h>

#include "system.h"
#include "mem.h"
#include "error.h"

#include "mfileio.h"

#include "numbers.h"

#include "pdflimits.h"
#include "pdfobj.h"

#include "pdfdoc.h"

#include "pdfdev.h"
#include "pdfdraw.h"

#include "pdfparse.h"

#include "epdf.h"
#include "mpost.h"
#include "psimage.h"
#include "pngimage.h"
#include "jpegimage.h"
#include "bmpimage.h"

#include "pdfximage.h"

struct pdf_ximage {
  char *ident;
  char *filename;
  int   subtype;

  char  res_name[16];

  void *info;

  pdf_obj *reference;
  pdf_obj *resource;
};

#define PDF_XIMAGE_ALLOC_SIZE  5

static int verbose = 0;

void
pdf_ximage_set_verbose (void)
{
  verbose++;
}

static struct
{
  int  count;
  int  capacity;
  struct pdf_ximage *ximages;
} ximage_cache = {
  0, 0, NULL
};

static void
pdf_init_ximage_struct (pdf_ximage *ximage)
{
  ximage->ident    = NULL;
  ximage->filename = NULL;
  ximage->subtype  = -1;
  memset(ximage->res_name, 0, 16);
  ximage->info      = NULL;
  ximage->reference = NULL;
  ximage->resource  = NULL;
}

void
pdf_init_images (void)
{
  ASSERT(ximage_cache.ximages == NULL);

  ximage_cache.count    = 0;
  ximage_cache.capacity = 0;
  ximage_cache.ximages  = NULL;
}

void
pdf_close_images (void)
{
  int   i;

  for (i = 0; i < ximage_cache.count; i++) {
    struct pdf_ximage *p;

    p = &ximage_cache.ximages[i];
    if (p->ident)
      RELEASE(p->ident);
    p->ident = NULL;
    if (p->filename)
      RELEASE(p->filename);
    p->filename = NULL;
    if (p->resource)
      pdf_release_obj(p->resource);
    p->resource = NULL;
    if (p->reference)
      pdf_release_obj(p->reference);
    p->reference = NULL;
    if (p->info) {
      if (p->subtype == PDF_XOBJECT_TYPE_IMAGE) {
	RELEASE((ximage_info *) p->info);
      } else {
	RELEASE((xform_info *) p->info);
      }
    }
    p->info = NULL;
  }

  RELEASE(ximage_cache.ximages);
  ximage_cache.ximages = NULL;
  ximage_cache.count = ximage_cache.capacity = 0;

  psimage_close(); /* release distiller template */
}

static void
scale_image (transform_info *p, const ximage_info *image_info)
{
  double xscale, yscale, xy_ratio;

  xy_ratio = (double) image_info->width / image_info->height;
  /* width and height overwrite scale. */
  if ((p->flags & INFO_HAS_WIDTH) &&
      (p->flags & INFO_HAS_HEIGHT)) {
    xscale = p->width ;  /* 72dpi */
    yscale = p->height; /* 72dpi */
  } else if (p->flags & INFO_HAS_WIDTH) {
    xscale = p->width;  /* 72dpi */
    yscale = p->width  / xy_ratio;
  } else if (p->flags & INFO_HAS_HEIGHT) {
    yscale = p->height;
    xscale = p->height * xy_ratio;
  } else {
    xscale = image_info->width  * 72.0 / 100.0;
    yscale = image_info->height * 72.0 / 100.0;
  }
  xscale *= p->xscale;
  yscale *= p->yscale;

  /*
   * Raster images are always scaled to width/height, not clipped.
   */
  p->bbox.llx = 0.0;
  p->bbox.lly = 0.0;
  p->bbox.urx = xscale;
  p->bbox.ury = yscale;

  p->xscale   = xscale;
  p->yscale   = yscale;

  if (verbose) {
    /* This does not take account of transformation matrix. */
    MESG("[dpi:%dx%d]",
	 (int) ROUND(72.0 * image_info->width  / p->xscale, 1.0),
	 (int) ROUND(72.0 * image_info->height / p->yscale, 1.0));
  }
}

static void
scale_form (transform_info *p, const xform_info *form_info)
{
  double xscale, yscale;
  double width, height;

  xscale = p->xscale;
  yscale = p->yscale;

  if (!(p->flags & INFO_HAS_USER_BBOX)) {
    p->bbox.llx = form_info->bbox.llx;
    p->bbox.lly = form_info->bbox.lly;
    p->bbox.urx = form_info->bbox.urx;
    p->bbox.ury = form_info->bbox.ury;
  }

  width  = p->bbox.urx - p->bbox.llx;
  height = p->bbox.ury - p->bbox.lly;

  if (p->flags & INFO_HAS_WIDTH) {
    xscale = p->width  / width;
    if (!(p->flags & INFO_HAS_HEIGHT)) {
      yscale = xscale;
    }
  }
  if (p->flags & INFO_HAS_HEIGHT) {
    yscale = (p->depth + p->height) / height;
    if (!(p->flags & INFO_HAS_WIDTH)) {
      xscale = yscale;
    }
  }
    
  p->xscale = xscale;
  p->yscale = yscale;
}

#define IMAGE_TYPE_UNKNOWN -1
#define IMAGE_TYPE_PDF  0
#define IMAGE_TYPE_JPEG 1
#define IMAGE_TYPE_PNG  2
#define IMAGE_TYPE_MPS  4
#define IMAGE_TYPE_EPS  5
#define IMAGE_TYPE_BMP  6

static int
source_image_type (FILE *fp)
{
  int image_type;

  rewind(fp);
  /*
   * Make sure we check for PS *after* checking for MP since
   * MP is a special case of PS.
   */
  if (check_for_jpeg(fp)) {
    image_type = IMAGE_TYPE_JPEG;
  }
#ifdef HAVE_LIBPNG
  else if (check_for_png(fp)) {
    image_type = IMAGE_TYPE_PNG;
  }
#endif
  else if (check_for_bmp(fp)) {
    image_type = IMAGE_TYPE_BMP;
  } else if (check_for_pdf(fp)) {
    image_type = IMAGE_TYPE_PDF;
  } else if (check_for_mp(fp)) {
    image_type = IMAGE_TYPE_MPS;
  } else if (check_for_ps(fp)) {
    image_type = IMAGE_TYPE_EPS;
  } else {
    image_type = IMAGE_TYPE_UNKNOWN;
  }
  rewind(fp);

  return image_type;
}

static int
load_image (const char *ident,
	    const char *fullname, int image_type, FILE  *fp)
{
  int         xobj_id;
  pdf_ximage *ximage;

  xobj_id = ximage_cache.count;
  if (ximage_cache.count >= ximage_cache.capacity) {
    ximage_cache.capacity += PDF_XIMAGE_ALLOC_SIZE;
    ximage_cache.ximages   = RENEW(ximage_cache.ximages,
				   ximage_cache.capacity, pdf_ximage);
  }
  ximage = &ximage_cache.ximages[xobj_id];

  pdf_init_ximage_struct(ximage);

  switch (image_type) {
  case IMAGE_TYPE_JPEG:
    if (verbose)
      MESG("[JPEG]");
    if (jpeg_include_image(ximage, fp) < 0)
      return -1;
    ximage->subtype  = PDF_XOBJECT_TYPE_IMAGE;
    break;
#ifdef HAVE_LIBPNG
  case IMAGE_TYPE_PNG:
    if (verbose)
      MESG("[PNG]");
    if (png_include_image(ximage, fp) < 0)
      return -1;
    ximage->subtype  = PDF_XOBJECT_TYPE_IMAGE;
    break;
#endif
  case IMAGE_TYPE_BMP:
    if (verbose)
      MESG("[BMP]");
    if (bmp_include_image(ximage, fp) < 0)
      return -1;
    ximage->subtype  = PDF_XOBJECT_TYPE_IMAGE;
    break;
  case IMAGE_TYPE_PDF:
    if (verbose)
      MESG("[PDF]");
    if (pdf_include_page(ximage, fp) < 0)
      return -1;
    ximage->subtype  = PDF_XOBJECT_TYPE_FORM;
    break;
  case IMAGE_TYPE_EPS:
    if (verbose)
      MESG("[PS]");
    if (ps_include_page(ximage, fullname) < 0)
      return -1;
    ximage->subtype  = PDF_XOBJECT_TYPE_FORM;
    break;
  default:
    if (verbose)
      MESG("[UNKNOWN]");
    if (ps_include_page(ximage, fullname) < 0)
      return -1;
    ximage->subtype  = PDF_XOBJECT_TYPE_FORM;
  }

  ximage->filename = NEW(strlen(ident)+1, char);
  ximage->ident    = NEW(strlen(ident)+1, char);
  strcpy(ximage->filename, ident);
  strcpy(ximage->ident,    ident);

  switch (ximage->subtype) {
  case PDF_XOBJECT_TYPE_IMAGE:
    sprintf(ximage->res_name, "Im%d", xobj_id);
    break;
  case PDF_XOBJECT_TYPE_FORM:
    sprintf(ximage->res_name, "Fm%d", xobj_id);
    break;
  default:
    ERROR("Unknown XObject subtype: %d", ximage->subtype);
    return -1;
  }

  ximage_cache.count++;

  return xobj_id;
}

int
pdf_ximage_findresource (const char *ident)
{
  int         xobj_id;
  pdf_ximage *ximage;
  char       *fullname;
  int         image_type;
  FILE       *fp;

  for (xobj_id = 0; xobj_id < ximage_cache.count; xobj_id++) {
    ximage = &ximage_cache.ximages[xobj_id];
    if (ximage->ident &&
	!strcmp(ident, ximage->ident)) {
      return xobj_id;
    }
  }

  fullname = kpse_find_pict(ident);
  if (!fullname) {
    WARN("Error locating image file \"%s\"", ident);
    return -1;
  }
  fp = MFOPEN(fullname, FOPEN_RBIN_MODE);
  if (!fp) {
    WARN("Error opening image file \"%s\"", fullname);
    RELEASE(fullname);
    return -1;
  }
  if (verbose) {
    MESG("(Image:%s", ident);
    if (verbose > 1)
      MESG("[%s]", fullname);
  }

  image_type = source_image_type(fp);
  switch (image_type) {
  case IMAGE_TYPE_MPS:
    if (verbose)
      MESG("[MPS]");
    xobj_id = mps_include_page(ident, fp);
    break;
  default:
    xobj_id = load_image(ident, fullname, image_type, fp);
    break;
  }
  RELEASE(fullname);
  MFCLOSE(fp);
  if (verbose)
    MESG(")");

  if (xobj_id < 0) {
    WARN("pdf: image inclusion failed for \"%s\".", ident);
    return -1;
  }

  return xobj_id;
}

void
pdf_ximage_init_form_info (xform_info *info)
{
  info->flags    = 0;
  info->bbox.llx = 0;
  info->bbox.lly = 0;
  info->bbox.urx = 0;
  info->bbox.ury = 0;
  info->matrix.a = 1.0;
  info->matrix.b = 0.0;
  info->matrix.c = 0.0;
  info->matrix.d = 1.0;
  info->matrix.e = 0.0;
  info->matrix.f = 0.0;
}

void
pdf_ximage_init_image_info (ximage_info *info)
{
  info->flags  = 0;
  info->width  = 0;
  info->height = 0;
  info->bits_per_component = 0;
  info->num_components = 0;
  info->min_dpi = 0;
}

void
pdf_ximage_set_image (pdf_ximage *ximage, void *image_info, pdf_obj *resource)
{
  pdf_obj *dict;
  ximage_info *info;

  if (!PDF_OBJ_STREAMTYPE(resource))
    ERROR("Image XObject must be stream type.");

  ximage->subtype = PDF_XOBJECT_TYPE_IMAGE;

  info = NEW(1, ximage_info);
  memcpy(info, image_info, sizeof(ximage_info));
  ximage->info = info;
  ximage->reference = pdf_ref_obj(resource);

  dict = pdf_stream_dict(resource);
  pdf_add_dict(dict, pdf_new_name("Type"),    pdf_new_name("XObject"));
  pdf_add_dict(dict, pdf_new_name("Subtype"), pdf_new_name("Image"));
  pdf_add_dict(dict, pdf_new_name("Width"),   pdf_new_number(info->width));
  pdf_add_dict(dict, pdf_new_name("Height"),  pdf_new_number(info->height));
  pdf_add_dict(dict, pdf_new_name("BitsPerComponent"),
	       pdf_new_number(info->bits_per_component));

  pdf_release_obj(resource); /* Caller don't know we are using reference. */
  ximage->resource  = NULL;
}

void
pdf_ximage_set_form (pdf_ximage *ximage,  void *info, pdf_obj *resource)
{
  ximage->subtype   = PDF_XOBJECT_TYPE_FORM;

  ximage->info = NEW(1, xform_info);
  memcpy(ximage->info, info, sizeof(xform_info));
  ximage->reference = pdf_ref_obj(resource);
  pdf_release_obj(resource); /* Caller don't know we are using reference. */
  ximage->resource  = NULL;
}

#define CHECK_ID(n) do {\
  if ((n) < 0 || (n) >= ximage_cache.count) {\
    ERROR("Invalid XObject ID: %d", (n));\
  }\
} while (0)
#define GET_IMAGE(n) (&(ximage_cache.ximages[(n)]))

pdf_obj *
pdf_ximage_get_reference (int xobj_id)
{
  pdf_ximage *ximage;

  CHECK_ID(xobj_id);

  ximage = GET_IMAGE(xobj_id);
  if (!ximage->reference) {
    ximage->reference = pdf_ref_obj(ximage->resource);
  }

  return pdf_link_obj(ximage->reference);
}

int
pdf_ximage_defineresource (const char *ident,
			   int subtype, void *info, pdf_obj *resource)
{
  int xobj_id;
  pdf_ximage *ximage;

  xobj_id = ximage_cache.count;
  if (ximage_cache.count >= ximage_cache.capacity) {
    ximage_cache.capacity += PDF_XIMAGE_ALLOC_SIZE;
    ximage_cache.ximages   = RENEW(ximage_cache.ximages,
				   ximage_cache.capacity, pdf_ximage);
  }
  ximage = GET_IMAGE(xobj_id);

  pdf_init_ximage_struct(ximage);
  if (ident) {
    ximage->ident = NEW(strlen(ident) + 1, char);
    strcpy(ximage->ident, ident);
  }

  switch (subtype) {
  case PDF_XOBJECT_TYPE_IMAGE:
    pdf_ximage_set_image(ximage, info, resource);
    ximage->subtype = PDF_XOBJECT_TYPE_IMAGE;
    sprintf(ximage->res_name, "Im%d", xobj_id);
    break;
  case PDF_XOBJECT_TYPE_FORM:
    pdf_ximage_set_form (ximage, info, resource);
    ximage->subtype = PDF_XOBJECT_TYPE_FORM;
    sprintf(ximage->res_name, "Fm%d", xobj_id);
    break;
  default:
    ERROR("Unknown XObject subtype: %d", subtype);
  }
  ximage_cache.count++;

  return xobj_id;
}

int
pdf_ximage_scale_image (int xobj_id, transform_info *p)
{
  pdf_ximage  *ximage;

  CHECK_ID(xobj_id);

  ximage = GET_IMAGE(xobj_id);

  switch (ximage->subtype) {
  case PDF_XOBJECT_TYPE_IMAGE:
    scale_image(p, ximage->info);
    break;
  case PDF_XOBJECT_TYPE_FORM:
    scale_form (p, ximage->info);
    break;
  default:
    ERROR("Unknown XObject type: %d", ximage->subtype);
    return -1;
  }

  return 0;
}

char *
pdf_ximage_get_resname (int xobj_id)
{
  pdf_ximage  *ximage;

  CHECK_ID(xobj_id);

  ximage = GET_IMAGE(xobj_id);

  return ximage->res_name;
}
