/* Define to 1 if you have the `basename' function. */
#cmakedefine HAVE_BASENAME 1

/* Define to 1 if you have the `close' function. */
#cmakedefine HAVE_CLOSE 1

/* Define to 1 if you have the declaration of `tzname', and to 0 if you don't.
   */
#cmakedefine HAVE_DECL_TZNAME 1

/* Define to 1 if fseeko (and presumably ftello) exists and is declared. */
#cmakedefine HAVE_FSEEKO 1

/* Define to 1 if you have the `getenv' function. */
#cmakedefine HAVE_GETENV 1

/* Define to 1 if you have the <inttypes.h> header file. */
#cmakedefine HAVE_INTTYPES_H 1

/* Define to 1 if you have the `kpathsea' library (-lkpathsea). */
#cmakedefine HAVE_LIBKPATHSEA 1

/* Define if you have libpaper */
#cmakedefine HAVE_LIBPAPER 1

/* Define if you have libpng */
#cmakedefine HAVE_LIBPNG 1

/* Define to 1 if you have the <memory.h> header file. */
#cmakedefine HAVE_MEMORY_H 1

/* Define to 1 if you have the `mkstemp' function. */
#cmakedefine HAVE_MKSTEMP 1

/* Define to 1 if you have the `open' function. */
#cmakedefine HAVE_OPEN 1

/* Define to 1 if you have the <stdint.h> header file. */
#cmakedefine HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#cmakedefine HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#cmakedefine HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#cmakedefine HAVE_STRING_H 1

/* Define to 1 if `tm_zone' is member of `struct tm'. */
#cmakedefine HAVE_STRUCT_TM_TM_ZONE 1

/* Define to 1 if you have the `system' function. */
#cmakedefine HAVE_SYSTEM 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#cmakedefine HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#cmakedefine HAVE_SYS_TYPES_H 1

/* external timezone variable */
#cmakedefine HAVE_TIMEZONE 1

/* struct tz has tm_gmtoff */
#cmakedefine HAVE_TM_GMTOFF 1

/* Define to 1 if your `struct tm' has `tm_zone'. Deprecated, use
   `HAVE_STRUCT_TM_TM_ZONE' instead. */
#cmakedefine HAVE_TM_ZONE 1

/* Define to 1 if you don't have `tm_zone' but do have the external array
   `tzname'. */
#cmakedefine HAVE_TZNAME 1

/* Define to 1 if you have the <unistd.h> header file. */
#cmakedefine HAVE_UNISTD_H 1

/* Define if you have zlib and its headers */
#cmakedefine HAVE_ZLIB 1

/* Define if your zlib has the compress2 function */
#cmakedefine HAVE_ZLIB_COMPRESS2 1

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
#cmakedefine NO_MINUS_C_MINUS_O 1

/* Name of package */
#define PACKAGE "dvipdfmx"

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
#include "dvipdfmx-version.h"
#define VERSION MIKTEX_COMP_ORIG_VERSION_STR

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
#cmakedefine WORDS_BIGENDIAN

/* Number of bits in a file offset, on hosts where this is settable. */
#cmakedefine _FILE_OFFSET_BITS

/* Define to 1 to make fseeko visible on some hosts (e.g. glibc 2.2). */
#cmakedefine _LARGEFILE_SOURCE

/* Define for large files, on AIX-style hosts. */
#cmakedefine _LARGE_FILES

/* Define if your libkpathsea supports enc formats */
#define __TDS_VERSION__ 0x200406

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
#  define USE_MIKTEX_EXIT
#endif
