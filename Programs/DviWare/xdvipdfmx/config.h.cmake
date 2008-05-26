/* Define if you have Mac OS X ApplicationServices */
#cmakedefine HAVE_APP_SERVICES

/* Define to 1 if you have the `basename' function. */
#cmakedefine HAVE_BASENAME

/* Define to 1 if you have the `close' function. */
#cmakedefine HAVE_CLOSE

/* Define to 1 if you have the declaration of `tzname', and to 0 if you don't.
   */
#cmakedefine HAVE_DECL_TZNAME

/* Define to 1 if you have the <dlfcn.h> header file. */
#cmakedefine HAVE_DLFCN_H

/* Define if you have freetype2 and its headers */
#cmakedefine HAVE_FREETYPE2

/* Define to 1 if fseeko (and presumably ftello) exists and is declared. */
#cmakedefine HAVE_FSEEKO

/* Define to 1 if you have the <ft2build.h> header file. */
#cmakedefine HAVE_FT2BUILD_H

/* Define to 1 if you have the `getenv' function. */
#cmakedefine HAVE_GETENV

/* Define to 1 if you have the <inttypes.h> header file. */
#cmakedefine HAVE_INTTYPES_H

/* Define if you have libfontconfig */
#cmakedefine HAVE_LIBFONTCONFIG

/* Define to 1 if you have the `freetype' library (-lfreetype). */
#cmakedefine HAVE_LIBFREETYPE

/* Define to 1 if you have the `kpathsea' library (-lkpathsea). */
#cmakedefine HAVE_LIBKPATHSEA

/* Define if you have libpaper */
#cmakedefine HAVE_LIBPAPER

/* Define if you have libpng */
#cmakedefine HAVE_LIBPNG 1

/* Define to 1 if you have the `z' library (-lz). */
#cmakedefine HAVE_LIBZ

/* Define to 1 if you have the <memory.h> header file. */
#cmakedefine HAVE_MEMORY_H

/* Define to 1 if you have the `mkstemp' function. */
#cmakedefine HAVE_MKSTEMP

/* Define to 1 if you have the `open' function. */
#cmakedefine HAVE_OPEN

/* Define to 1 if you have the <stdint.h> header file. */
#cmakedefine HAVE_STDINT_H

/* Define to 1 if you have the <stdlib.h> header file. */
#cmakedefine HAVE_STDLIB_H

/* Define to 1 if you have the <strings.h> header file. */
#cmakedefine HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#cmakedefine HAVE_STRING_H

/* Define to 1 if `tm_zone' is member of `struct tm'. */
#cmakedefine HAVE_STRUCT_TM_TM_ZONE

/* Define to 1 if you have the `system' function. */
#cmakedefine HAVE_SYSTEM

/* Define to 1 if you have the <sys/stat.h> header file. */
#cmakedefine HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/types.h> header file. */
#cmakedefine HAVE_SYS_TYPES_H

/* external timezone variable */
#cmakedefine HAVE_TIMEZONE


/* struct tz has tm_gmtoff */
#cmakedefine HAVE_TM_GMTOFF

/* Define to 1 if your `struct tm' has `tm_zone'. Deprecated, use
   `HAVE_STRUCT_TM_TM_ZONE' instead. */
#cmakedefine HAVE_TM_ZONE

/* Define to 1 if you don't have `tm_zone' but do have the external array
   `tzname'. */
#cmakedefine HAVE_TZNAME

/* Define to 1 if you have the <unistd.h> header file. */
#cmakedefine HAVE_UNISTD_H

/* Define if you have zlib and its headers */
#cmakedefine HAVE_ZLIB 1

/* Define if your zlib has the compress2 function */
#cmakedefine HAVE_ZLIB_COMPRESS2

/* Define to 1 if you have the <zlib.h> header file. */
#cmakedefine HAVE_ZLIB_H

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
#cmakedefine NO_MINUS_C_MINUS_O

/* Name of package */
#define PACKAGE "xdvipdfmx"

/* Define to the address where bug reports for this package should be sent. */
#cmakedefine PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#cmakedefine PACKAGE_NAME

/* Define to the full name and version of this package. */
#cmakedefine PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#cmakedefine PACKAGE_TARNAME

/* Define to the version of this package. */
#cmakedefine PACKAGE_VERSION

/* The size of `char', as computed by sizeof. */
#cmakedefine SIZEOF_CHAR

/* The size of `int', as computed by sizeof. */
#cmakedefine SIZEOF_INT

/* The size of `long', as computed by sizeof. */
#cmakedefine SIZEOF_LONG

/* The size of `short', as computed by sizeof. */
#cmakedefine SIZEOF_SHORT

/* Define to 1 if you have the ANSI C header files. */
#cmakedefine STDC_HEADERS

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
#cmakedefine TM_IN_SYS_TIME

/* Version number of package */
#include "xdvipdfmx-version.h"
#define VERSION  MIKTEX_COMPONENT_ORIGINAL_VERSION_STR

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
#cmakedefine WORDS_BIGENDIAN

/* Define if part of a teTeX/TeX live build. */
#define XETEX 1

/* Define if part of a teTeX/TeX live build for Mac OS X. */
#cmakedefine XETEX_MAC

/* Number of bits in a file offset, on hosts where this is settable. */
#cmakedefine _FILE_OFFSET_BITS

/* Define to 1 to make fseeko visible on some hosts (e.g. glibc 2.2). */
#cmakedefine _LARGEFILE_SOURCE

/* Define for large files, on AIX-style hosts. */
#cmakedefine _LARGE_FILES

/* Define if your libkpathsea supports enc formats */
#cmakedefine __TDS_VERSION__

/* Define to empty if `const' does not conform to ANSI C. */
#cmakedefine const

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#cmakedefine inline
#endif

/* Define to `unsigned int' if <sys/types.h> does not define. */
#cmakedefine size_t

#if defined(MIKTEX)
#  define MIKTEX_2_4_COMPAT 1
#endif