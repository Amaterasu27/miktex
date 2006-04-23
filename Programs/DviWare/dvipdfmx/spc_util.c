/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/spc_util.c,v 1.2 2005/07/03 20:02:29 csc Exp $
    
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

#include "pdfdev.h"
#include "pdfparse.h"
#include "pdfcolor.h"

#include "specials.h"

#include "spc_util.h"

int
spc_util_read_numbers (double *values, int num_values,
		       struct spc_env *spe, struct spc_arg *args)
{
  int   count;
  char *number;

  for (count = 0;
       count < num_values &&
	 args->curptr < args->endptr; count++) {
    number = parse_number(&args->curptr, args->endptr);
    if (!number) {
      break;
    } else {
      values[count] = atof(number);
      RELEASE(number);
    }
  }

  return count;
}

static int
read_colorsp_colorspec (pdf_color *colorspec,
			struct spc_env *spe, struct spc_arg *args)
{
  int     error = 0;
  char   *token;
  double  values[4];
  int     num_components;

  token = parse_ident(&args->curptr, args->endptr);
  if (!token) {
    spe->errmsg = (char *) "No color specified.";
    return -1;
  }

  skip_white(&args->curptr, args->endptr);
  if (!strcmp(token, "rgb")) { /* Handle rgb color */
    num_components = spc_util_read_numbers(values, 3, spe, args);
    if (num_components != 3) {
      spe->errmsg = (char *) "Invalid value for RGB color specification.";
      error = -1;
    } else {
      pdf_color_rgbcolor(colorspec, values[0], values[1], values[2]);
    }
  } else if (!strcmp(token, "cmyk")) { /* Handle cmyk color */
    num_components = spc_util_read_numbers(values, 4, spe, args);
    if (num_components != 4) {
      spe->errmsg = (char *) "Invalid value for CMYK color specification.";
      error = -1;
    } else {
      pdf_color_cmykcolor(colorspec,
			  values[0], values[1], values[2], values[3]);
    }
  } else if (!strcmp(token, "gray")) { /* Handle gray */
    num_components = spc_util_read_numbers(values, 1, spe, args);
    if (num_components != 1) {
      spe->errmsg = (char *) "Invalid value for gray color specification.";
      error = -1;
    } else {
      pdf_color_graycolor(colorspec, values[0]);
    }
  } else if (!strcmp(token, "hsb")) {
    spe->errmsg = (char *) "HSB color not supported yet.";
    error = -1;
  } else { /* Must be a "named" color */
    error = pdf_color_namedcolor(colorspec, token);
    if (error) {
      spe->errmsg = (char *) "Unknown color name.";
    }
  }
  RELEASE(token);

  return error;
}

static int
read_pdfmxsp_colorspec (pdf_color *colorspec,
			struct spc_env *spe, struct spc_arg *args)
{
  pdf_obj  *color;
  int       num_components, i;

  skip_white(&args->curptr, args->endptr);

  color = parse_pdf_object(&args->curptr, args->endptr);
  if (color == NULL) {
    return -1;
  }

  switch (PDF_OBJ_TYPEOF(color)) {
  case PDF_ARRAY:
    num_components = pdf_array_length(color);
    for (i = 0; i < num_components; i++) {
      if (!PDF_OBJ_NUMBERTYPE(pdf_get_array(color, i))) {
	WARN("A number expected for color component value.");
	pdf_release_obj(color);
	return -1;
      }
    }

    switch (num_components) {
    case 1:
      pdf_color_graycolor(colorspec,
			  pdf_number_value(pdf_get_array(color, 0)));
      break;
    case 3:
      pdf_color_rgbcolor(colorspec,
			 pdf_number_value(pdf_get_array(color, 0)),
			 pdf_number_value(pdf_get_array(color, 1)),
			 pdf_number_value(pdf_get_array(color, 2)));
      break;
    case 4:
      pdf_color_cmykcolor(colorspec,
			  pdf_number_value(pdf_get_array(color, 0)),
			  pdf_number_value(pdf_get_array(color, 1)),
			  pdf_number_value(pdf_get_array(color, 2)),
			  pdf_number_value(pdf_get_array(color, 3)));
      break;
    default:
      WARN("Expecting either RGB or CMYK color array.");
      break;
    }
    break;
  case PDF_NUMBER:
    pdf_color_graycolor(colorspec,
			pdf_number_value(pdf_get_array(color, 0)));
    break;
  default:
    WARN("Expecting a number or an array for color specification.");
    pdf_release_obj(color);
    return -1;
  }
  pdf_release_obj(color);

  return 0;
}

int
spc_util_read_colorspec (pdf_color *colorspec,
			 struct spc_env *spe, struct spc_arg *args, int colorsp)
{
  int  error = 0;

  ASSERT(colorspec && spe && args);

  skip_white(&args->curptr, args->endptr);
  if (args->curptr >= args->endptr) {
    return -1;
  }

  if (colorsp) {
    error = read_colorsp_colorspec(colorspec, spe, args);
  } else {
    error = read_pdfmxsp_colorspec(colorspec, spe, args);
  }

  return error;
}


#define HOFFSET 1
#define VOFFSET 2
#define HSIZE   3
#define VSIZE   4
#define HSCALE  5
#define VSCALE  6
#define ANGLE   7
#define CLIP    8
#define LLX     9
#define LLY    10
#define URX    11
#define URY    12
#define RWI    13
#define RHI    14

static struct keys
{
  const char *key;
  int id;
} keys[] = {
  {"hoffset", HOFFSET},
  {"voffset", VOFFSET},
  {"hsize",   HSIZE},
  {"vsize",   VSIZE},
  {"hscale",  HSCALE},
  {"vscale",  VSCALE},
  {"angle",   ANGLE},
  {"clip",    CLIP},
  {"llx",     LLX},
  {"lly",     LLY},
  {"urx",     URX},
  {"ury",     URY},
  {"rwi",     RWI},
  {"rhi",     RHI},
};

static int
guess_key (const char *key)
{
  int   i;

  for (i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
    if (!strcmp (key, keys[i].key))
      break;
  }

  if (i == sizeof(keys)/sizeof(keys[0]))
    return -1;

  return keys[i].id;
}

static int
is_a_number (const char *s)
{
  if (!s || !*s)
    return 0;

  if (*s == '+' || *s == '-')
    s++;
  while (*s) {
    if (!isdigit(*s))
      break;
    s++;
  }
  if (*s) {
    if (*s != '.')
      return 0;
    s++;
    while (*s) {
      if (!isdigit(*s))
	return 0;
      s++;
    }
  }

  return 1;
}

static int
read_dvipssp_dimension (transform_info *p,
			struct spc_env *spe, struct spc_arg *args)
{
  skip_white(&args->curptr, args->endptr);
  while (args->curptr < args->endptr) {
    char  *key = NULL, *value = NULL;
    int    key_id;

    parse_key_val(&args->curptr, args->endptr, &key, &value);
    if (!key) {
      if (value)
	RELEASE(value);
      return -1;
    }

    key_id = guess_key(key);

    if (!value) {
      switch (key_id) {
      case CLIP:
	p->flags |= INFO_DO_CLIP;
	break;
      default:
	RELEASE(key);
	spe->errmsg = (char *) "Missing a number...";
	return -1;
      }
      RELEASE(key);
      continue;
    }
    
    if (!is_a_number(value)) {
      RELEASE(key);
      RELEASE(value);
      spe->errmsg = (char *) "Expecting a number...";
      return -1;
    }

    switch (key_id) {
      case HOFFSET:
	p->xoffset = atof(value);
	break;
      case VOFFSET:
	p->yoffset = atof(value);
	break;
      case HSIZE:
	p->width  = atof(value);
	p->flags |= INFO_HAS_WIDTH;
	break;
      case VSIZE:
	p->height = atof(value);
	p->flags |= INFO_HAS_HEIGHT;
	break;
      case HSCALE:
	p->xscale = atof(value) / 100.0;
	break;
      case VSCALE:
	p->yscale = atof(value) / 100.0;
	break;
      case ANGLE:
	p->rotate = atof(value) * M_PI / 180.0;
	break;
      case LLX:
	p->bbox.llx = atof(value);
	p->flags   |= INFO_HAS_USER_BBOX;
	break;
      case LLY:
	p->bbox.lly = atof(value);
	p->flags   |= INFO_HAS_USER_BBOX;
	break;
      case URX:
	p->bbox.urx = atof(value);
	p->flags   |= INFO_HAS_USER_BBOX;
	break;
      case URY:
	p->bbox.ury = atof(value);
	p->flags   |= INFO_HAS_USER_BBOX;
	break;
      case RWI:
	p->width  = atof(value) / 10.0;
	p->flags |= INFO_HAS_WIDTH;
	break;
      case RHI:
	p->height = atof(value) / 10.0;
	p->flags |= INFO_HAS_HEIGHT;
	break;
      default:
	RELEASE(key);
	RELEASE(value);
	spe->errmsg = (char *) "Unknown dimension/transformation keyword...";
	return -1;
	break;
    }
    RELEASE(key);
    RELEASE(value);
    skip_white(&args->curptr, args->endptr);
  }

  return 0;
}

/*
 * The following must be consecutive and
 * starting at 0.  Order must match
 * dimensions array below
 */

#define WIDTH   0
#define HEIGHT  1
#define DEPTH   2
#define SCALE   3
#define XSCALE  4
#define YSCALE  5
#define ROTATE  6
#define BBOX    7
#define XOFFSET 8
#define YOFFSET 9

static struct {
  const char *key_str;
  int         key_id;
} dimensions[] = {
  {"width",   WIDTH},
  {"height",  HEIGHT},
  {"depth",   DEPTH},
  {"scale",   SCALE},
  {"xscale",  XSCALE},
  {"yscale",  YSCALE},
  {"rotate",  ROTATE},
  {"bbox",    BBOX},
  {"xoffset", XOFFSET},
  {"yoffset", YOFFSET}
};
#define NUM_DIMENSION_KEYS (sizeof(dimensions)/sizeof(dimensions[0]))

static int
parse_one_dim_word (char **start, char *end)
{
  int   key_id = -1;
  char *key_str;
  int   i;

  skip_white(start, end);
  key_str = parse_ident(start, end);
  if (!key_str) {
    return -1;
  }

  for (i = 0; i < NUM_DIMENSION_KEYS; i++) {
    if (!strcmp(dimensions[i].key_str, key_str)) {
      key_id = dimensions[i].key_id;
      break;
    }
  }
  RELEASE(key_str);

  return key_id;
}

static int
read_pdfmxsp_dimension (transform_info *p,
			struct spc_env *spe, struct spc_arg *args)
{
  double values[4], scale;
  int    has_scale, has_xscale, has_yscale;

  has_xscale = has_yscale = has_scale = 0;

  skip_white(&args->curptr, args->endptr);
  while (args->curptr < args->endptr &&
	 isalpha(args->curptr[0])) {
    int  key;

    key  =  parse_one_dim_word(&args->curptr, args->endptr);
    skip_white(&args->curptr, args->endptr);
    switch (key) {
    case WIDTH:
      p->width  = parse_length(&args->curptr, args->endptr);
      p->flags |= INFO_HAS_WIDTH;
      break;
    case HEIGHT:
      p->height = parse_length(&args->curptr, args->endptr);
      p->flags |= INFO_HAS_HEIGHT;
      break;
    case DEPTH:
      p->depth = parse_length(&args->curptr, args->endptr);
      p->flags |= INFO_HAS_HEIGHT;
      break;
    case SCALE:
      if (spc_util_read_numbers(&scale, 1, spe, args) != 1) {
	spe->errmsg = (char *) "Missing a number after \"scale\".";
	return -1;
      }
      has_scale = 1;
      break;
    case XSCALE:
      if (spc_util_read_numbers(&p->xscale, 1, spe, args) != 1) {
	spe->errmsg = (char *) "Missing a number after \"xscale\".";
	return -1;
      }
      has_xscale = 1;
      break;
    case YSCALE:
      if (spc_util_read_numbers(&p->yscale, 1, spe, args) != 1) {
	spe->errmsg = (char *) "Missing a number after \"yscale\".";
	return -1;
      }
      has_yscale = 1;
      break;
    case ROTATE:
      if (spc_util_read_numbers(&p->rotate, 1, spe, args) != 1) {
	spe->errmsg = (char *) "Missing a number after \"rotate\".";
	return -1;
      }
      p->rotate *= M_PI / 180.0;
      break;
    case BBOX:
      if (spc_util_read_numbers(values, 4, spe, args) != 4) {
	spe->errmsg = (char *) "Expecting four numbers following \"bbox\".";
	return -1;
      }
      p->bbox.llx = values[0];
      p->bbox.lly = values[1];
      p->bbox.urx = values[2];
      p->bbox.ury = values[3];
      p->flags   |= INFO_HAS_USER_BBOX;
      break;
    case XOFFSET:
      p->xoffset = parse_length(&args->curptr, args->endptr);
      break;
    case YOFFSET:
      p->yoffset = parse_length(&args->curptr, args->endptr);
      break;
    default:
      spe->errmsg = (char *) "Invalid dimention/transformation key...";
      return -1;
      break;
    }
    skip_white(&args->curptr, args->endptr);
  }

  if (has_xscale && (p->flags & INFO_HAS_WIDTH)) {
    spe->errmsg = (char *) "Can't supply both width and xscale.";
    return -1;
  } else if (has_yscale &&
	     (p->flags & INFO_HAS_HEIGHT)) {
    spe->errmsg = (char *) "Can't supply both height/depth and yscale.";
    return -1;
  } else if (has_scale) {
    if (has_xscale || has_yscale) {
      spe->errmsg = (char *) "Can't supply overall scale along with axis scales.";
      return -1;
    } else {
      p->xscale = scale;
      p->yscale = scale;
    }
  }

  return 0;
}

int
spc_util_read_dimension (transform_info *p,
			 struct spc_env *spe, struct spc_arg *args,
			 int dvipssp)
{
  ASSERT(p && spe && args);

  if (dvipssp) {
    return read_dvipssp_dimension(p, spe, args);
  } else {
    return read_pdfmxsp_dimension(p, spe, args);
  }

  return -1;
}


#ifndef isodigit
#define isodigit(c) ((c) >= '0' && (c) <= '7')
#endif
static int
parse_escape_char (char **pp, char *endptr,
		   const char *escape_str, unsigned char *ch)
{
  int  skip = 0;

  (*pp)++;

  switch (**pp) {
  case 'n': *ch = '\n'; (*pp)++; break;
  case 'r': *ch = '\r'; (*pp)++; break;
  case 't': *ch = '\t'; (*pp)++; break;
  case 'b': *ch = '\b'; (*pp)++; break;
  case 'f': *ch = '\f'; (*pp)++; break;
    /*
     * An end-of-line marker preceded by a backslash must be ignored.
     */
  case '\n':
    skip = 1; (*pp)++;
    break;
  case '\r':
    skip = 1; (*pp)++;
    if (*pp < endptr && **pp == '\n')
      (*pp)++;
    break;
  default:
    if (strchr(escape_str, **pp)) {
      *ch = **pp;
      (*pp)++;
    } else if (isodigit(**pp)) {
      int i = 3, val = 0;

      /* Don't forget isodigit() is a macro. */
      while (i-- > 0 &&
	     *pp < endptr && isodigit(**pp))
        val = (val << 3) + (*((*pp)++) - '0');
#if 0
      /* Not sure how to handle this. */
      if (val > 255)
	skip = 1;
#endif
      *ch = (unsigned char) (val & 0xff);
    } else {
      /* Ignore only backslash. */
      *ch = (unsigned char) **pp;
      (*pp)++;
      skip = 0;
    }
  }

  return skip ? 0 : 1;
}

#define QUOTECHAR '"'

char *
parse_c_string (char **pp, char *endptr)
{
  unsigned char *result = NULL;
  char   *p;
  int     len = 0;

  p = *pp;

  skip_white(&p, endptr);
  if (p >= endptr || p[0] != QUOTECHAR) {
    WARN("Could not find a C string.");
    return NULL;
  }

  p++;
  result = NEW(endptr - p + 1, unsigned char);
  while (p < endptr && p[0] != QUOTECHAR) {
    if (p[0] == '\\' &&
	parse_escape_char(&p, endptr, "\\\"", &(result[len])))
      len++;
    else {
      result[len++] = p[0];
      p++;
    }
  }

  if (p >= endptr || p[0] != QUOTECHAR) {
    WARN("Unbalanced quote or truncated strong.");
    RELEASE(result);
    return NULL;
  }

  result[len] = 0;
  *pp = p + 1;
  return (char *) result;
}

void
parse_key_val (char **start, char *end, char **key, char **val) 
{
  *key = *val = NULL;

  skip_white(start, end);
  if ((*key = parse_c_ident(start, end))) {
    skip_white(start, end);
    if (*start < end && **start == '=') {

      (*start)++;
      skip_white(start, end);
      if (*start < end) {
        if (**start == QUOTECHAR) {
	  *val = parse_c_string(start, end);
        } else {
	  *val = parse_val_ident(start, end);
	}
      }

    }
  }
}
