/*
#!/bin/sh
#
# ppmmargin - add a margin to a portable anymap
#
# Copyright (C) 1991 by Jef Poskanzer.
#
# Permission to use, copy, modify, and distribute this software and its
# documentation for any purpose and without fee is hereby granted, provided
# that the above copyright notice appear in all copies and that both that
# copyright notice and this permission notice appear in supporting
# documentation.  This software is provided "as is" without express or
# implied warranty.
*/

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "pnm.h"

static char tempdir[_MAX_PATH];
static char tmp1[_MAX_PATH];
static char tmp2[_MAX_PATH];
static char tmp3[_MAX_PATH];
static char tmp4[_MAX_PATH];
static char color[100];
static char size[100];
static char buf[4096];

#define usage() fprintf (stderr, \
		 "usage: %s [-white|-black|-color <colorspec>] <size> [pnmfile]\n", \
                 argv[0])

int
main (int	argc,
      char **	argv)

{
  int i;
  FILE * infile;
  FILE * tmp1file;
  size_t n;

  _setmode (_fileno (stdin), _O_BINARY);
  _setmode (_fileno (stdout), _O_BINARY);

  /*
tmp1=/tmp/pnmm1$$
tmp2=/tmp/pnmm2$$
tmp3=/tmp/pnmm3$$
tmp4=/tmp/pnmm4$$
rm -f $tmp1 $tmp2 $tmp3 $tmp4
  */
  GetTempPath (_MAX_PATH, tempdir);
  GetTempFileName (tempdir, "m1", 0, tmp1); 
  GetTempFileName (tempdir, "m2", 0, tmp2); 
  GetTempFileName (tempdir, "m3", 0, tmp3); 
  GetTempFileName (tempdir, "m4", 0, tmp4); 
  DeleteFile (tmp1);
  DeleteFile (tmp2);
  DeleteFile (tmp3);
  DeleteFile (tmp4);

  /*
color="-gofigure"
  */
  strcpy (color, "-gofigure");

  /*
# Parse args.
while true ; do
    case "$1" in
	-w* )
	color="-white"
	shift
	;;
	-b* )
	color="-black"
	shift
	;;
	-c* )
	shift
	if [ ! ${1-""} ] ; then
	    echo "usage: $0 [-white|-black|-color <colorspec>] <size> [pnmfile]" 1>&2
	    exit 1
	fi
	color="$1"
	shift
	;;
	-* )
	echo "usage: $0 [-white|-black|-color <colorspec>] <size> [pnmfile]" 1>&2
	exit 1
	;;
	* )
	break
	;;
    esac
done
  */
  i = 1;
  while (i < argc)
    {
      if (strncmp (argv[i], "-w", 2) == 0)
	strcpy (color, "-white");
      else if (strncmp (argv[i], "-b", 2) == 0)
	strcpy (color, "-black");
      else if (strncmp (argv[i], "-c", 2) == 0)
	{
	  i += 1;
	  if (i == argc)
	    {
	      usage ();
	      exit (1);
	    }
	  strcpy (color, argv[i]);
	}
      else if (argv[i][0] == '-')
	{
	  usage ();
	  exit (1);
	}
      else
	break;
      i += 1;
    }

  /*
if [ ! ${1-""} ] ; then
    echo "usage: $0 [-white|-black|-color <colorspec>] <size> [pnmfile]" 1>&2
    exit 1
fi
size="$1"
shift
  */
  if (i == argc)
    {
      usage ();
      exit (1);
    }
  strcpy (size, argv[i]);
  i += 1;

  /*
if [ ${2-""} ] ; then
    echo "usage: $0 [-white|-black|-color <colorspec>] <size> [pnmfile]" 1>&2
    exit 1
fi
  */
  if (i + 1 <= argc)
    {
      usage ();
      exit (1);
    }

  /*
# Capture input file in a tmp file, in case it's a pipe.
cat $@ > $tmp1
  */
  if (i < argc)
    infile = fopen (argv[i], "rb");
  else
    infile = stdin;
  if (infile == 0)
    {
      exit (1);
    }
  tmp1file = fopen (tmp1, "wb");
  if (tmp1file == 0)
    {
      exit (1);
    }
  while ((n = fread (buf, 1, sizeof (buf), infile)) > 0)
    fwrite (buf, 1, n, tmp1file);
  fclose (tmp1file);
  if (infile != stdin)
    fclose (infile);

  /*
# Construct spacer files.
case "$color" in
    -gofigure )
    pnmcut 0 0 1 1 $tmp1 | pnmtile $size 1 > $tmp2
    ;;
    -white | -black )
    pbmmake $color $size 1 > $tmp2
    ;;
    * )
    ppmmake $color $size 1 > $tmp2
    ;;
esac
pnmflip -rotate90 $tmp2 > $tmp3
  */
  if (strcmp (color, "-gofigure") == 0)
    sprintf (buf, "pnmcut 0 0 1 1 %s | pnmtile %s 1 > %s", tmp1, size, tmp2);
  else if (strcmp (color, "-white") == 0 || strcmp (color, "-black") == 0)
    sprintf (buf, "pbmmake %s %s 1 > %s", color, size, tmp2);
  else
    sprintf (buf, "ppmmake %s %s 1 > %s", color, size, tmp2);
  system (buf);
  sprintf (buf, "pnmflip -rotate90 %s > %s", tmp2, tmp3);
  system (buf);

  /*
# Cat things together.
pnmcat -lr $tmp2 $tmp1 $tmp2 > $tmp4
pnmcat -tb $tmp3 $tmp4 $tmp3
  */
  sprintf (buf, "pnmcat -lr %s %s %s > %s", tmp2, tmp1, tmp2, tmp4);
  system (buf);
  sprintf (buf, "pnmcat -tb %s %s %s", tmp3, tmp4, tmp3);
  system (buf);

  /*
# All done.
rm -f $tmp1 $tmp2 $tmp3 $tmp4
  */
  DeleteFile (tmp1);
  DeleteFile (tmp2);
  DeleteFile (tmp3);
  DeleteFile (tmp4);

  return (0);
}
