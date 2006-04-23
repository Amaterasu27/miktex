/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/dvipdfmx.c,v 1.9 2005/07/03 20:02:28 csc Exp $
    
    This is DVIPDFMx, an eXtended version of DVIPDFM by Mark A. Wicks.

    Copyright (C) 2002-2003 by Jin-Hwan Cho and Shunsaku Hirata,
    the DVIPDFMx project team <dvipdfmx@project.ktug.or.kr>
    
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

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#include "system.h"
#include "mem.h"
#include "dpxfile.h"

#include "dvi.h"

#include "pdfdoc.h"
#include "pdfdev.h"
#include "pdfparse.h"
#include "pdfencrypt.h"

#include "spc_tpic.h"
#include "spc_util.h"
#include "spc_pdfm.h"
#include "specials.h"

#include "psimage.h"
#include "mpost.h"

#include "fontmap.h"
#include "pdffont.h"
#include "cid.h"

extern void error_cleanup (void);

static int verbose = 0;

static int mp_mode = 0;

static long opt_flags = 0;
#define OPT_TPIC_TRANSPARENT_FILL (1 << 1)
#define OPT_CIDFONT_FIXEDPITCH    (1 << 2)
#define OPT_FONTMAP_FIRST_MATCH   (1 << 3)

static char   ignore_colors = 0;
static double annot_grow    = 0.0;
static int    bookmark_open = 0;
static double mag           = 1.0;
static int    font_dpi      = 600;
static int    really_quiet  = 0;
/*
 * Precision is essentially limited to 0.01pt.
 * See, dev_set_string() in pdfdev.c.
 */
static int pdfdecimaldigits = 2;

/* Encryption */
static int do_encryption    = 0;
static unsigned key_bits    = 40;
static unsigned permission  = 0x003C;

/* Page device */
static double paper_width  = 595.0;
static double paper_height = 842.0;
static double x_offset = 72.0;
static double y_offset = 72.0;
static int  default_papersize = 1;
static char landscape_mode    = 0;

#ifdef HAVE_LIBPAPER
#include <paper.h>
#else /* !HAVE_LIBPAPER */
struct paper {
  const char* name;
  double pswidth, psheight;
};

#ifndef DEFAULT_PAPER_NAME
#define DEFAULT_PAPER_NAME "a4"
#endif

static const struct paper paperspecs[] = {
  {"letter",  612.00,  792.00},
  {"legal" ,  612.00, 1008.00},
  {"ledger", 1224.00,  792.00},
  {"tabloid", 792.00, 1224.00},
  {"a6",      297.64,  420.91},
  {"a5",      420.91,  595.27},
  {"a4",      595.27,  841.82},
  {"a3",      841.82, 1190.16},
  /*
   * The B series paper formats were first introduced in Japanese
   * patch (jpatch). The size of b6, ..., b5var paper is JIS paper
   * size for this reason. Please modify the following line or use
   * libpaper if you need ISO paper sizes.
   */
#ifdef ISO_PAPERSIZE
  {"b6",  354,  499},
  {"b5",  501,  709},
  {"b4",  709, 1002},
  {"b3", 1002, 1418},
#else
  {"b6",      364.25,  515.91},
  {"b5",      515.91,  728.50},
  {"b4",      728.50, 1031.81},
  {"b3",     1031.81, 1457.00},
  {"b5var",   515.91,  651.97},
#endif
  /* -- */
  {NULL, 0, 0},
};

#define paperinit()
#define paperdone()
#define paperpswidth(p)    (((p) && (p)->name) ? p->pswidth  : 0.0)
#define paperpsheight(p)   (((p) && (p)->name) ? p->psheight : 0.0)
#define papername(p)       (((p) && (p)->name) ? p->name : NULL)
#define paperfirst()       &(paperspecs[0])
#define papernext(p)       ((((p)+1) && ((p)+1)->name) ? (p+1) : NULL)

#define defaultpapername() DEFAULT_PAPER_NAME
#define systempapername()  DEFAULT_PAPER_NAME

#define defaultpapersizefile() NULL
#define systempapersizefile()  NULL

static const struct paper *
paperinfo (const char *ppspec)
{
  const struct paper *ppinfo;

  ppinfo = paperfirst();
  while (ppinfo && papername(ppinfo)) {
    if (!strcasecmp(ppspec, papername(ppinfo)))
      break;
    ppinfo = papernext(ppinfo);
  }

  return ((ppinfo && papername(ppinfo)) ? ppinfo : NULL);
}

#endif /* !HAVE_LIBPAPER */

static void paperconf (void)
{
  const struct paper *ppinfo;

  fprintf(stdout, "Supported paper format:\n");
  ppinfo = paperfirst();
  while (ppinfo && papername(ppinfo)) {
    fprintf(stdout, "  %s: %g %g\n",
	    papername(ppinfo), paperpswidth(ppinfo), paperpsheight(ppinfo));
    ppinfo = papernext(ppinfo);
  }
  if (systempapername() != NULL)
    fprintf(stdout, "System default: %s\n", systempapername());
  else if (defaultpapername() != NULL)
    fprintf(stdout, "System default: %s\n", defaultpapername());

  return;
}

static int
get_mediasize (pdf_rect *rect, const char *ppspec)
{
  const struct paper *ppinfo;

  ppinfo = paperinfo(ppspec);
  if (ppinfo && papername(ppinfo)) {
    rect->urx = paperpswidth(ppinfo);
    rect->ury = paperpsheight(ppinfo);
  } else {
    char  *start, *end, *comma;

    start = (char *) ppspec;
    comma = strchr(start, ',');
    end   = start + strlen(start);
    if (!comma)
      return -1;
    rect->urx = parse_length(&start, comma);
    start = comma + 1;
    rect->ury = parse_length(&start, end);
  }

  rect->llx = rect->lly = 0.0;
  return ((rect->urx <= 0.0 || rect->ury <= 0.0) ? -1 : 0);
}

char *dvi_filename = NULL, *pdf_filename = NULL;

static void
set_default_pdf_filename(void)
{
  const char *dvi_base;

  dvi_base = xbasename(dvi_filename);
  if (mp_mode &&
      strlen(dvi_base) > 4 &&
      !strncmp(".mps", dvi_base + strlen(dvi_base) - 4, 4)) {
    pdf_filename = NEW(strlen(dvi_base)+1, char);
    strncpy(pdf_filename, dvi_base, strlen(dvi_base) - 4);
    pdf_filename[strlen(dvi_base)-4] = '\0';
  } else if (strlen(dvi_base) > 4 &&
	     !strncmp(".dvi", dvi_base+strlen(dvi_base)-4, 4)) {
    pdf_filename = NEW(strlen(dvi_base)+1, char);
    strncpy(pdf_filename, dvi_base, strlen(dvi_base)-4);
    pdf_filename[strlen(dvi_base)-4] = '\0';
  } else {
    pdf_filename = NEW(strlen(dvi_base)+5, char);
    strcpy(pdf_filename, dvi_base);
  }

  strcat (pdf_filename, ".pdf");
}

static void
usage (void)
{
  fprintf (stdout, "\nThis is %s-%s by the DVIPDFMx project team,\n", PACKAGE, VERSION);
  fprintf (stdout, "an extended version of dvipdfm-0.13.2c developed by Mark A. Wicks.\n");
  fprintf (stdout, "\nCopyright (C) 2002-2005 by the DVIPDFMx project team\n");
  fprintf (stdout, "\nThis is free software; you can redistribute it and/or modify\n");
  fprintf (stdout, "it under the terms of the GNU General Public License as published by\n");
  fprintf (stdout, "the Free Software Foundation; either version 2 of the License, or\n");
  fprintf (stdout, "(at your option) any later version.\n");
  fprintf (stdout, "\nUsage: dvipdfmx [options] dvifile\n");
  fprintf (stdout, "-c \t\tIgnore color specials (for B&W printing)\n");
  fprintf (stdout, "-d number\tSet PDF decimal digits (0-5) [2]\n");
  fprintf (stdout, "-l \t\tLandscape mode\n");
#if 0
  /* Not supported */
  fprintf (stdout, "-e \t\tDisable partial font embedding [default is enabled]\n");
#endif
  fprintf (stdout, "-f filename\tSet font map file name [t1fonts.map]\n");
  fprintf (stdout, "-m number\tSet additional magnification\n");
  fprintf (stdout, "-o filename\tSet output file name [dvifile.pdf]\n");
  fprintf (stdout, "-p papersize\tSet papersize [a4]\n");
  fprintf (stdout, "-r resolution\tSet resolution (in DPI) for raster fonts [600]\n");
  fprintf (stdout, "-s pages\tSelect page ranges (-)\n");
  fprintf (stdout, "-t \t\tEmbed thumbnail images\n");
  fprintf (stdout, "-x dimension\tSet horizontal offset [1.0in]\n");
  fprintf (stdout, "-y dimension\tSet vertical offset [1.0in]\n");
  fprintf (stdout, "-z number  \tSet zlib compression level (0-9) [9]\n");

  fprintf (stdout, "-v         \tBe verbose\n");
  fprintf (stdout, "-vv        \tBe more verbose\n");
  fprintf (stdout, "-C number\tSpecify miscellaneous option flags [0]:\n");
  fprintf (stdout, "\t\t  0x0001 reserved\n");
  fprintf (stdout, "\t\t  0x0002 Use semi-transparent filling for tpic shading command,\n");
  fprintf (stdout, "\t\t         instead of opaque gray color. (requires PDF 1.4)\n");
  fprintf (stdout, "\t\t  0x0004 Treat all CIDFont as fixed-pitch font.\n");
  fprintf (stdout, "\t\t  0x0008 Do not replace duplicate fontmap entries.\n");
  fprintf (stdout, "\t\tPositive values are always ORed with previously given flags.\n");
  fprintf (stdout, "\t\tAnd negative values replace old values.\n");
  fprintf (stdout, "-O number\tSet maximum depth of open bookmark items [0]\n");
  fprintf (stdout, "-P number\tSet permission flags for PDF encryption [0x003C]\n");
  fprintf (stdout, "-S         \tEnable PDF encryption\n");
  fprintf (stdout, "-T         \tEmbed thumbnail images. Remove images files when finished.\n");
  fprintf (stdout, "-M         \tExperimental mps-to-pdf mode\n");
  fprintf (stdout, "-V number\tSet PDF minor version [3]\n");
  fprintf (stdout, "\nAll dimensions entered on the command line are \"true\" TeX dimensions.\n");
  fprintf (stdout, "Argument of \"-s\" lists physical page ranges separated by commas, e.g., \"-s 1-3,5-6\"\n");
  fprintf (stdout, "Papersize is specified by paper format (e.g., \"a4\") or by w<unit>,h<unit> (e.g., \"20cm,30cm\").\n");
  fprintf (stdout, "Please type \"dvipdfmx -vh\" for the list of available paper format.\n\n");
  if (verbose) {
    paperconf();
  }
  fprintf (stdout, "\n");

  exit(1);
}

struct page_label
{
  char  type[2];
  char *prefix;
  unsigned long  start;
};

struct page_range 
{
  long first, last;
  struct page_label *label;
} *page_ranges = NULL;

int num_page_ranges = 0;
int max_page_ranges = 0;

static struct page_label *
parse_pagelabel (char **start, char *end)
{
  struct page_label *label;
  char *next;
  int   len;

  if (*start >= end)
    return NULL;

  label = NEW(1, struct page_label);
  label->type[0] = '\0';
  label->type[1] = '\0';
  label->start   = 1;
  label->prefix  = NULL;

  switch (**start) {
  case 'D':
  case 'r': case 'R':
  case 'a': case 'A':
    label->type[0] = **start;
    *start += 1;
    break;
  case ':':
    break;
  default:
    WARN("Invalid page label specified.");
    *start += 1;
    break;
  }
  if (*start < end && **start == ':') {
    *start += 1;
    label->start = strtoul(*start, &next, 10);
    if (next == *start)
      label->start = 1;
    else
      *start = next;
  }
  if (*start < end && **start == ':') {
    *start += 1;
    len = end - (*start);
    if (len > 0) {
      label->prefix = NEW(len + 1, char);
      memcpy(label->prefix, *start, len);
      label->prefix[len] = '\0';
    }
  }

  *start = end;
  return label;
}

static void
select_pages (char *start, char *end)
{
  char *number;

  while (start < end) {
    /* Enlarge page range table if necessary */
    if (num_page_ranges >= max_page_ranges) {
      max_page_ranges += 4;
      page_ranges = RENEW (page_ranges, max_page_ranges, struct page_range);
    }

    page_ranges[num_page_ranges].first = 0;
    page_ranges[num_page_ranges].last  = 0;
    page_ranges[num_page_ranges].label = NULL;

    skip_white(&start, end);
    number = parse_unsigned(&start, end); /* Can't be signed. */
    if (number) {
      page_ranges[num_page_ranges].first = atoi(number) - 1;
      page_ranges[num_page_ranges].last  = page_ranges[num_page_ranges].first;
      RELEASE(number);
    }

    skip_white(&start, end);
    if (*start == '-') {
      start++;
      skip_white(&start, end);
      page_ranges[num_page_ranges].last = -1;
      if (start < end &&
	  (number = parse_unsigned(&start, end))) {
	page_ranges[num_page_ranges].last = atoi(number) - 1;
	RELEASE(number);
      }
    } else {
      page_ranges[num_page_ranges].last = page_ranges[num_page_ranges].first;
    }

    skip_white(&start, end);
    if (*start == '/') {
      char *next;

      start++;
      next = strchr(start, ',');
      if (!next)
	next = end;
      page_ranges[num_page_ranges].label = parse_pagelabel(&start, next);
    }
    num_page_ranges++;

    skip_white(&start, end);
    if (start < end && *start == ',') {
      start += 1;
    } else {
      skip_white (&start, end);
      if (start < end)
	ERROR("Bad page range specification: %s", start);
    }
  }
}

#define POP_ARG() {argv += 1; argc -= 1;}
/* It doesn't work as expected (due to dvi filename). */
#define CHECK_ARG(n,m) if (argc < (n) + 1) {\
  fprintf (stderr, "\nMissing %s after \"-%c\".\n", (m), *flag);\
  usage();\
}
 
static void
do_args (int argc, char *argv[])
{
  while (argc > 0 && *argv[0] == '-') {
    char *flag, *nextptr;

    for (flag = argv[0] + 1; *flag != 0; flag++) {
      switch (*flag) {
      case 'D':
	CHECK_ARG(1, "PS->PDF conversion command line template");
	set_distiller_template(argv[1]);
	POP_ARG();
	break;
      case 'r':
	CHECK_ARG(1, "bitmap font dpi");
	font_dpi = atoi(argv[1]);
	if (font_dpi <= 0)
	  ERROR("Invalid bitmap font dpi specified: %s", argv[1]);
	POP_ARG();
	break;
      case 'm':
	CHECK_ARG(1, "magnification value");
	mag = strtod(argv[1], &nextptr);
	if (mag < 0.0 || nextptr == argv[1])
	  ERROR("Invalid magnification specifiied: %s", argv[1]);
	POP_ARG();
	break;
      case 'g':
	CHECK_ARG(1, "annotation \"grow\" amount");
	nextptr = argv[1];
	annot_grow = parse_length(&nextptr, nextptr + strlen(nextptr));
	POP_ARG();
	break;
      case 'x':
	CHECK_ARG(1, "horizontal offset value");
	nextptr = argv[1];
	x_offset = parse_length(&nextptr, nextptr + strlen(nextptr));
	POP_ARG();
	break;
      case 'y':
	CHECK_ARG(1, "vertical offset value");
	nextptr = argv[1];
	y_offset = parse_length(&nextptr, nextptr + strlen(nextptr));
	POP_ARG();
	break;
      case 'o':
	CHECK_ARG(1, "output file name");
	pdf_filename = NEW (strlen(argv[1])+1,char);
	strcpy(pdf_filename, argv[1]);
	POP_ARG();
	break;
      case 's':
	CHECK_ARG(1, "page selection specification");
	select_pages(argv[1], argv[1] + strlen(argv[1]));
	POP_ARG();
	break;
#ifndef NO_THUMBNAIL
      case 'T':
	pdf_doc_enable_thumbnails(1); /* remove after... */
	break;
      case 't':
	pdf_doc_enable_thumbnails(0);
	break;
#endif /* !NO_THUMBNAIL */
      case 'p':
	CHECK_ARG(1, "paper format/size");
	{
	  pdf_rect rect;

	  if (get_mediasize(&rect, argv[1]) < 0) {
	    ERROR("Unknown papersize format: \"%s\"", argv[1]);
	  }
	  paper_width  = rect.urx;
	  paper_height = rect.ury;
	  default_papersize = 0;
	}
	POP_ARG();
	break;
      case 'c':
	ignore_colors = 1;
	break;
      case 'l':
	landscape_mode = 1;
	break;
      case 'f':
	CHECK_ARG(1, "fontmap file name");
	if (opt_flags & OPT_FONTMAP_FIRST_MATCH)
	  pdf_load_fontmap_file(argv[1], FONTMAP_RMODE_APPEND);
	else
	  pdf_load_fontmap_file(argv[1], FONTMAP_RMODE_REPLACE);
	POP_ARG();
	break;
      case 'e':
	WARN("dvipdfm \"-e\" option not supported.");
	break;
      case 'q':
	really_quiet = 1;
	break;
      case 'v':
	verbose++;
	break;
      case 'V':
	{
	  int ver_minor;

	  if (isdigit(*(flag+1))) {
	    flag++;
	    ver_minor = atoi(flag);
	  } else {
	    CHECK_ARG(1, "PDF minor version number");
	    ver_minor = atoi(argv[1]);
	    POP_ARG();
	  }
	  if (ver_minor < 3 || ver_minor > 5) {
	    WARN("PDF version 1.%d not supported. (1.3 used instead)", ver_minor);
	    ver_minor = 3;
	  }
	  pdf_set_version((unsigned) ver_minor);
	}
	break;
      case 'z': 
	{
	  int level;

	  if (isdigit(*(flag+1))) {
	    flag++;
	    level = atoi(flag);
	  } else {
	    CHECK_ARG(1, "compression level");
	    level = atoi(argv[1]);
	    POP_ARG();
	  }
	  pdf_set_compression(level);
	}
	break;
      case 'd': 
	if (isdigit(*(flag+1))) {
	  flag++;
	  pdfdecimaldigits = atoi(flag);
	} else {
	  CHECK_ARG(1, "number of fractional digits");
	  pdfdecimaldigits = atoi(argv[1]);
	  POP_ARG();
	}
	break;
      case 'S':
        do_encryption = 1;
	break;
      case 'K': 
	CHECK_ARG(1, "encryption key bits");
	key_bits = (unsigned) atoi(argv[1]);
	if (key_bits < 40 || key_bits > 128)
	  ERROR("Invalid encryption key bits specified: %s", argv[1]);
	POP_ARG();
	break;
      case 'P': 
	CHECK_ARG(1, "encryption permission flag");
	permission = (unsigned) strtoul(argv[1], &nextptr, 0);
	if (nextptr == argv[1])
	  ERROR("Invalid encryption permission flag: %s", argv[1]);
	POP_ARG();
	break;
      case 'O':
	/* Bookmark open level */
	CHECK_ARG(1, "bookmark open level");
	bookmark_open = atoi(argv[1]);
	POP_ARG();
	break;
      case 'M':
	mp_mode = 1;
	break;
      case 'C':
	CHECK_ARG(1, "a number");
	{
	  long flags;

	  flags = (unsigned) strtol(argv[1], &nextptr, 0);
	  if (nextptr == argv[1])
	    ERROR("Invalid flag: %s", argv[1]);
	  if (flags < 0)
	    opt_flags  = -flags;
	  else
	    opt_flags |=  flags;
	}
	POP_ARG();
	break;
      default:
	fprintf (stderr, "Unknown option in \"%s\"", flag);
	usage();
      }
    }
    POP_ARG();
  }

  if (!really_quiet) {
    int i;

    for (i = 0; i < verbose; i++) {
      dvi_set_verbose();
      pdf_fontmap_set_verbose();
      pdf_dev_set_verbose();
      pdf_doc_set_verbose();
      pdf_enc_set_verbose();
      pdf_obj_set_verbose();
      dpx_file_set_verbose();
    }
  }

  if (argc > 1) {
    fprintf(stderr, "Multiple dvi filenames?");
    usage();
  } else if (argc > 0) {
    /*
     * The only legitimate way to have argc == 0 here is
     * is do_args was called from config file.  In that case, there is
     * no dvi file name.  Check for that case .
     */
    if (!mp_mode && strncmp(".dvi", argv[0] + strlen(argv[0]) - 4, 4)) {
      dvi_filename = NEW(strlen(argv[0]) + 5, char);
      strcpy(dvi_filename, argv[0]);
      strcat(dvi_filename, ".dvi");
    } else {
      dvi_filename = NEW(strlen(argv[0]) + 1, char);
      strcpy(dvi_filename, argv[0]);
    }
  }
}

static void
cleanup (void)
{
  if (dvi_filename)
    RELEASE(dvi_filename);
  if (pdf_filename)
    RELEASE(pdf_filename);
  if (page_ranges) {
    int i;

    for (i = 0; i < num_page_ranges; i++) {
      if (page_ranges[i].label) {
	if (page_ranges[i].label->prefix)
	  RELEASE(page_ranges[i].label->prefix);
	RELEASE(page_ranges[i].label);
      }
    }
    RELEASE(page_ranges);
  }
}

static const char *default_config_file = "dvipdfmx.cfg";

static void
read_config_file (const char *config)
{
  char *fullname;
  char *start, *end, *option;
  FILE *fp;

#ifdef MIKTEX
  if (!miktex_find_app_input_file("dvipdfm", config, fullname = work_buffer))
    return;
#else
  if ((fullname = kpse_find_file(config, kpse_program_text_format, 1)) == NULL)
    return;
#endif

  if (!(fp = MFOPEN (fullname, FOPEN_R_MODE))) {
    WARN("Could not open config file \"%s\".", fullname);
    return;
  }
  while ((start = mfgets (work_buffer, WORK_BUFFER_SIZE, fp)) != NULL) {
    char *argv[2];
    int   argc;

    argc = 0;
    end = work_buffer + strlen(work_buffer);
    skip_white (&start, end);
    if (start >= end)
      continue;
    /* Build up an argument list as if it were passed on the command
       line */
    if ((option = parse_ident (&start, end))) {
      argc = 1;
      argv[0] = NEW (strlen(option)+2, char);
      strcpy (argv[0]+1, option);
      RELEASE (option);
      *argv[0] = '-';
      skip_white (&start, end);
      if (start < end) {
        argc += 1;
        if (*start == '"') {
          argv[1] = parse_c_string (&start, end);
        }
        else
          argv[1] = parse_ident (&start, end);
      }
    }
    do_args (argc, argv);
    while (argc > 0) {
      RELEASE (argv[--argc]);
    }
  }
  if (fp)
    MFCLOSE(fp);
}

static void
system_default (void)
{
  pdf_rect rect;

  if (systempapername() != NULL &&
      get_mediasize(&rect, systempapername()) >= 0) {
    paper_width  = rect.urx;
    paper_height = rect.ury;
  } else if (defaultpapername() != NULL &&
             get_mediasize(&rect, defaultpapername())) {
    paper_width  = rect.urx;
    paper_height = rect.ury;
  }
}

void
error_cleanup (void)
{
  pdf_error_cleanup();
  if (pdf_filename) {
    remove(pdf_filename);
    fprintf(stderr, "\nOutput file removed.\n");
  }
}

#define SWAP(v1,v2) do {\
   double _tmp = (v1);\
   (v1) = (v2);\
   (v2) = _tmp;\
 } while (0)

static void
do_dvi_pages (void)
{
  long     page_no, page_count, i, step;
  double   page_width, page_height;
  pdf_rect mediabox;
  int      has_label;

  spc_exec_at_begin_document();

  if (num_page_ranges == 0) {
    if (!page_ranges) {
      page_ranges = NEW(1, struct page_range);
      max_page_ranges = 1;
    }
    page_ranges[0].first = 0;
    page_ranges[0].last  = -1; /* last page */
    page_ranges[0].label = NULL;
    num_page_ranges = 1;
  }

  page_width  = paper_width;
  page_height = paper_height;
  page_count  = 0;
  has_label   = 0;

  mediabox.llx = 0.0;
  mediabox.lly = 0.0;
  mediabox.urx = paper_width;
  mediabox.ury = paper_height;

  pdf_doc_set_creator (dvi_comment());
  pdf_doc_set_mediabox(0, &mediabox); /* Root node */

  for (i = 0; i < num_page_ranges; i++) {
    if (page_ranges[i].last < 0)
      page_ranges[i].last += dvi_npages();

    if (page_ranges[i].label || has_label) {
      struct page_label *label;

      has_label = 1;
      label = page_ranges[i].label;
      if (!label)
	pdf_doc_set_pagelabel(page_count,
			      NULL, NULL, 0, 1); /* reset */
      else if (label->prefix) {
	pdf_doc_set_pagelabel(page_count,
			      label->type,
			      label->prefix, strlen(label->prefix),
			      label->start);
      } else {
	pdf_doc_set_pagelabel(page_count,
			      label->type,
			      NULL, 0, label->start);
      }

    }
    step    = (page_ranges[i].first <= page_ranges[i].last) ? 1 : -1;
    page_no = page_ranges[i].first;
    for (;;) {
      if (page_no < dvi_npages()) {
	double w, h;
	char   lm;

	MESG("[%d", page_no+1);

	/* Users want to change page size even after page is started! */
	w = page_width; h = page_height; lm = landscape_mode;
	dvi_scan_paper_size(page_no, &w, &h, &lm);
	if (lm != landscape_mode) {
	  SWAP(w, h);
	  landscape_mode = lm;
	}
	if (page_width  != w || page_height != h) {
	  page_width  = w;
	  page_height = h;
	}
	if (page_width  != paper_width ||
	    page_height != paper_height) {
	  mediabox.llx = 0.0;
	  mediabox.lly = 0.0;
	  mediabox.urx = page_width;
	  mediabox.ury = page_height;
	  pdf_doc_set_mediabox(page_count+1, &mediabox);
	}
	dvi_do_page(page_no,
		    page_width, page_height, x_offset, y_offset);
	page_count++;
	MESG("]");
      }

      if (step > 0 &&
	  page_no >= page_ranges[i].last)
	break;
      else if (step < 0 &&
	       page_no <= page_ranges[i].last)
	break;
      else {
	page_no += step;
      }
    }
  }

  if (page_count < 1) {
    ERROR("No pages fall in range!");
  }

  spc_exec_at_end_document();
}

static void
do_mps_pages (void)
{
  FILE *mps_file;
  char *suffix;

  suffix = strstr(dvi_filename, ".dvi");
  if (suffix && strlen(suffix) == strlen(".dvi"))
    suffix[0] = '\0';

  mps_file = MFOPEN(dvi_filename, FOPEN_RBIN_MODE);
  if (!mps_file)
    ERROR("Could not open file: %s", dvi_filename);
  mps_do_page(mps_file);
  MFCLOSE(mps_file);
}


/* TODO: MetaPost mode */
#if defined(MIKTEX)
#  define main Main
#endif
int CDECL
main (int argc, char *argv[]) 
{
  double dvi2pts;

  if (argc < 2) {
    fprintf(stderr, "No dvi filename specified.");
    usage();
    return 1;
  }

#ifdef MIKTEX
  miktex_initialize();
#else
  kpse_set_program_name(argv[0], "dvipdfm");
#endif

  paperinit();
  system_default();

  argv+=1;
  argc-=1;

  pdf_init_fontmaps(); /* This must come before parsing options... */

  /* Process config file, if any */
  if (default_config_file) {
    read_config_file(default_config_file);
  }

  default_papersize = 1; /* reset */
  do_args (argc, argv);

  if (really_quiet)
    shut_up();

#ifndef MIKTEX
  kpse_init_prog("", font_dpi, NULL, NULL);
  kpse_set_program_enabled(kpse_pk_format, true, kpse_src_texmf_cnf);
#endif
  pdf_font_set_dpi(font_dpi);

  if (!dvi_filename) {
    WARN("No dvi filename specified.");
    usage();
    return 1;
  }

  /* Check for ".dvi" at end of argument name */
  if (pdf_filename == NULL)
    set_default_pdf_filename();

  MESG("%s -> %s\n", dvi_filename, pdf_filename);

  if (do_encryption) {
    pdf_enc_set_passwd(key_bits, permission, dvi_filename, pdf_filename);
    if (key_bits > 40)
      pdf_set_version(4);
  }

  if (mp_mode) {
    x_offset = 0.0;
    y_offset = 0.0;
    dvi2pts  = 0.01; /* dvi2pts controls accuracy. */
  } else {
    /* Dependency between DVI and PDF side is rather complicated... */
    dvi2pts = dvi_init(dvi_filename, mag);
    if (dvi2pts == 0.0)
      ERROR("dvi_init() failed!");

    dvi_scan_paper_size(0, &paper_width, &paper_height, &landscape_mode);
    if (landscape_mode) {
      SWAP(paper_width, paper_height);
    }
  }

  /* Set default paper size here so that all page's can inherite it.
   * annot_grow:    Margin of annotation.
   * bookmark_open: Miximal depth of open bookmarks.
   */
  pdf_open_document(pdf_filename,
                    paper_width, paper_height, annot_grow, bookmark_open);

  /* Ignore_colors placed here since
   * they are considered as device's capacity.
   */
  pdf_init_device(dvi2pts, pdfdecimaldigits, ignore_colors);

  if (opt_flags & OPT_CIDFONT_FIXEDPITCH)
    CIDFont_set_flags(CIDFONT_FORCE_FIXEDPITCH);

  /* Please move this to spc_init_specials(). */
  if (opt_flags & OPT_TPIC_TRANSPARENT_FILL)
    tpic_set_fill_mode(1);

  if (mp_mode) {
    do_mps_pages();
  } else {
    do_dvi_pages();
  }

  /* Order of close... */
  pdf_close_device  ();
  /* pdf_close_document flushes XObject (image) and other resources. */
  pdf_close_document();

  pdf_close_fontmaps(); /* pdf_font may depend on fontmap. */

  if (!mp_mode)
    dvi_close();

  MESG("\n");
  cleanup();

  paperdone();
#ifdef MIKTEX
  miktex_uninitialize ();
#endif

  return 0;
}
