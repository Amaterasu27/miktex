/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/spc_misc.c,v 1.2 2005/07/03 20:02:29 csc Exp $
    
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

#include <string.h>

#include "system.h"
#include "mem.h"
#include "error.h"

#include "mfileio.h"

#include "pdfparse.h"
#include "pdfobj.h"

#include "pdfcolor.h"
#include "pdfdraw.h"
#include "pdfximage.h"
#include "pdfdev.h"

#include "mpost.h"

#include "specials.h"

#include "spc_util.h"
#include "spc_misc.h"

int
spc_misc_at_begin_page (void)
{
  return 0;
}

int spc_misc_at_end_page (void)
{
  return 0;
}

int
spc_misc_at_begin_document (void)
{
  return 0;
}

int
spc_misc_at_end_document (void)
{
  return 0;
}

static int
spc_handler_background (struct spc_env *spe, struct spc_arg *args)
{
  int        error;
  pdf_color  colorspec;

  error = spc_util_read_colorspec(&colorspec, spe, args, 1);
  if (!error) {
    pdf_color_set_bgcolor(&colorspec);
  }

  return error;
}

static int
spc_handler_postscriptbox (struct spc_env *spe, struct spc_arg *args)
{
  int            form_id;
  transform_info form_info;
  char           filename[256];
  double         width = 0.0, height = 0.0;
  FILE          *fp;
  char          *fullname;
  char           wbuf[256], *p, *endptr;

  ASSERT(spe && args);

  if (args->curptr >= args->endptr) {
    spe->errmsg = (char *) "No width/height/filename given.";
    return -1;
  }

  memset(filename, 0, 256);
  if (sscanf(args->curptr, "{%lfpt}{%lfpt}{%256[^}]}",
	     &width, &height, filename) != 3) {
    spe->errmsg = (char *) "Unrecogzied postscriptbox special.";
    return -1;
  }

  fullname = kpse_find_pict(filename);
  if (!fullname) {
    spe->errmsg = (char *) "Image file not found.";
    return -1;
  }

  fp = MFOPEN(fullname, FOPEN_R_MODE);
  RELEASE(fullname);

  if (!fp) {
    spe->errmsg = (char *) "Could not open image file.";
    return -1;
  }

  transform_info_clear(&form_info);

  form_info.width  = width  * 72.0 / 72.27;
  form_info.height = height * 72.0 / 72.27;
  form_info.flags |= (INFO_HAS_WIDTH|INFO_HAS_HEIGHT);

  for (;;) {
    p     = mfgets(wbuf, 256, fp);
    if (!p) {
      break;
    }
    endptr = p + strlen(wbuf);

    if (mps_scan_bbox(&p, endptr, &form_info.bbox) >= 0) {
      form_info.flags |= INFO_HAS_USER_BBOX;
      break;
    }
  }
  MFCLOSE(fp);

  if (!(form_info.flags & INFO_HAS_USER_BBOX)) {
    spe->errmsg = (char *) "Could not find BoundingBox...";
    return -1;
  }

  form_id = pdf_ximage_findresource(filename);
  if (form_id < 0) {
    spe->errmsg = (char *) "Failed to load image file...";
    return -1;
  }

  pdf_dev_put_image(form_id, &form_info,
		    spe->x_user, spe->y_user);
  args->curptr = args->endptr;

  return 0;
}

static int
spc_handler_null (struct spc_env *spe, struct spc_arg *args)
{
  args->curptr = args->endptr;

  return 0;
}

static struct spc_handler misc_handlers[] = {
  {"background",    spc_handler_background},
  {"postscriptbox", spc_handler_postscriptbox},
  {"landscape",     spc_handler_null}, /* handled at bop */
  {"papersize",     spc_handler_null}, /* handled at bop */
  {"src:",          spc_handler_null}, /* simply ignore  */
  {"om:",           spc_handler_null}  /* simply ignore  */
};


int
spc_misc_check_special (const char *buffer, long size)
{
  char  *p, *endptr;
  int    i;

  p      = (char *) buffer;
  endptr = p + size;

  skip_white(&p, endptr);
  size   = (long) (endptr - p);
  for (i = 0;
       i < sizeof(misc_handlers)/sizeof(struct spc_handler); i++) {
    if (size >= strlen(misc_handlers[i].key) &&
	!strncmp(p, misc_handlers[i].key,
		 strlen(misc_handlers[i].key))) {
      return 1;
    }
  }

  return 0;
}

int
spc_misc_setup_handler (struct spc_handler *handle,
			struct spc_env *spe, struct spc_arg *args)
{
  char  *key;
  int    i, keylen;

  ASSERT(handle && spe && args);

  skip_white(&args->curptr, args->endptr);

  key = args->curptr;
  while (args->curptr < args->endptr &&
	 isalpha(args->curptr[0])) {
    args->curptr++;
  }

  if (args->curptr < args->endptr &&
      args->curptr[0] == ':') {
    args->curptr++;
  }

  keylen = (int) (args->curptr - key);
  if (keylen < 1) {
    spe->errmsg = (char *) "No valid key for special found.";
    return -1;
  }

  for (i = 0;
       i < sizeof(misc_handlers)/sizeof(struct spc_handler); i++) {
    if (keylen == strlen(misc_handlers[i].key) &&
	!strncmp(key, misc_handlers[i].key, keylen)) {

      skip_white(&args->curptr, args->endptr);

      args->command = (char *) misc_handlers[i].key;

      handle->key   = (char *) "???:";
      handle->exec  = misc_handlers[i].exec;

      return 0;
    }
  }

  return -1;
}
