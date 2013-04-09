/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
#undef AC_APPLE_UNIVERSAL_BUILD

/* The normal alignment of `double', in bytes. */
#if defined(MIKTEX_WINDOWS)
#define ALIGNOF_DOUBLE 8
#else
#undef ALIGNOF_DOUBLE
#endif

/* Use libxml2 instead of Expat */
#undef ENABLE_LIBXML2

/* Additional font directories */
#undef FC_ADD_FONTS

/* Architecture prefix to use for cache file names */
#undef FC_ARCHITECTURE

/* System font directory */
#undef FC_DEFAULT_FONTS

/* Define to nothing if C supports flexible array members, and to 1 if it does
   not. That way, with a declaration like `struct s { int n; double
   d[FLEXIBLE_ARRAY_MEMBER]; };', the struct hack can be used with pre-C99
   compilers. When computing the size of such an object, don't use 'sizeof
   (struct s)' as it overestimates the size. Use 'offsetof (struct s, d)'
   instead. Don't use 'offsetof (struct s, d[0])', as this doesn't work with
   MSVC and with C++ compilers. */
#define FLEXIBLE_ARRAY_MEMBER

/* Define to 1 if you have the `chsize' function. */
#undef HAVE_CHSIZE

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#undef HAVE_DIRENT_H

/* Define to 1 if you have the <dlfcn.h> header file. */
#undef HAVE_DLFCN_H

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
#undef HAVE_DOPRNT

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the `fstatfs' function. */
#undef HAVE_FSTATFS

/* Define to 1 if you have the `fstatvfs' function. */
#undef HAVE_FSTATVFS

/* Define to 1 if you have the `ftruncate' function. */
#undef HAVE_FTRUNCATE

/* FT_Bitmap_Size structure includes y_ppem field */
#define HAVE_FT_BITMAP_SIZE_Y_PPEM 1

/* Define to 1 if you have the `FT_Get_BDF_Property' function. */
#define HAVE_FT_GET_BDF_PROPERTY 1

/* Define to 1 if you have the `FT_Get_Next_Char' function. */
#define HAVE_FT_GET_NEXT_CHAR 1

/* Define to 1 if you have the `FT_Get_PS_Font_Info' function. */
#define HAVE_FT_GET_PS_FONT_INFO 1

/* Define to 1 if you have the `FT_Get_X11_Font_Format' function. */
#undef HAVE_FT_GET_X11_FONT_FORMAT

/* Define to 1 if you have the `FT_Has_PS_Glyph_Names' function. */
#define HAVE_FT_HAS_PS_GLYPH_NAMES 1

/* Define to 1 if you have the `FT_Select_Size' function. */
#undef HAVE_FT_SELECT_SIZE

/* Define to 1 if you have the `geteuid' function. */
#undef HAVE_GETEUID

/* Define to 1 if you have the `getopt' function. */
#define HAVE_GETOPT 1

/* Define to 1 if you have the `getopt_long' function. */
#define HAVE_GETOPT_LONG 1

/* Define to 1 if you have the `getpagesize' function. */
#undef HAVE_GETPAGESIZE

/* Define to 1 if you have the `getuid' function. */
#undef HAVE_GETUID

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Define to 1 if you have the `link' function. */
#undef HAVE_LINK

/* Define to 1 if you have the `lrand48' function. */
#undef HAVE_LRAND48

/* Define to 1 if you have the `memmove' function. */
#undef HAVE_MEMMOVE

/* Define to 1 if you have the <memory.h> header file. */
#undef HAVE_MEMORY_H

/* Define to 1 if you have the `memset' function. */
#undef HAVE_MEMSET

/* Define to 1 if you have the `mkstemp' function. */
#undef HAVE_MKSTEMP

/* Define to 1 if you have a working `mmap' system call. */
#undef HAVE_MMAP

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
#undef HAVE_NDIR_H

/* Define to 1 if you have the `posix_fadvise' function. */
#undef HAVE_POSIX_FADVISE

/* Define to 1 if you have the `rand' function. */
#define HAVE_RAND 1

/* Define to 1 if you have the `random' function. */
#undef HAVE_RANDOM

/* Define to 1 if you have the `random_r' function. */
#undef HAVE_RANDOM_R

/* Define to 1 if you have the `rand_r' function. */
#undef HAVE_RAND_R

/* Define to 1 if you have the `regcomp' function. */
#undef HAVE_REGCOMP

/* Define to 1 if you have the `regerror' function. */
#undef HAVE_REGERROR

/* Define to 1 if you have the `regexec' function. */
#undef HAVE_REGEXEC

/* Define to 1 if you have the <regex.h> header file. */
#undef HAVE_REGEX_H

/* Define to 1 if you have the `regfree' function. */
#undef HAVE_REGFREE

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#undef HAVE_STDLIB_H

/* Define to 1 if you have the `strchr' function. */
#undef HAVE_STRCHR

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#undef HAVE_STRING_H

/* Define to 1 if you have the `strrchr' function. */
#undef HAVE_STRRCHR

/* Define to 1 if you have the `strtol' function. */
#undef HAVE_STRTOL

/* Define to 1 if `d_type' is a member of `struct dirent'. */
#undef HAVE_STRUCT_DIRENT_D_TYPE

/* Define to 1 if `f_flags' is a member of `struct statfs'. */
#undef HAVE_STRUCT_STATFS_F_FLAGS

/* Define to 1 if `f_fstypename' is a member of `struct statfs'. */
#undef HAVE_STRUCT_STATFS_F_FSTYPENAME

/* Define to 1 if `f_basetype' is a member of `struct statvfs'. */
#undef HAVE_STRUCT_STATVFS_F_BASETYPE

/* Define to 1 if `f_fstypename' is a member of `struct statvfs'. */
#undef HAVE_STRUCT_STATVFS_F_FSTYPENAME

/* Define to 1 if you have the `sysconf' function. */
#undef HAVE_SYSCONF

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
#undef HAVE_SYS_DIR_H

/* Define to 1 if you have the <sys/mount.h> header file. */
#undef HAVE_SYS_MOUNT_H

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
#undef HAVE_SYS_NDIR_H

/* Define to 1 if you have the <sys/param.h> header file. */
#undef HAVE_SYS_PARAM_H

/* Define to 1 if you have the <sys/statfs.h> header file. */
#undef HAVE_SYS_STATFS_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#undef HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/types.h> header file. */
#undef HAVE_SYS_TYPES_H

/* Define to 1 if you have the <sys/vfs.h> header file. */
#undef HAVE_SYS_VFS_H

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* Define to 1 if you have the `vprintf' function. */
#undef HAVE_VPRINTF

/* Can use #warning in C files */
#undef HAVE_WARNING_CPP_DIRECTIVE

/* Use xmlparse.h instead of expat.h */
#undef HAVE_XMLPARSE_H

/* Define to 1 if you have the `XML_SetDoctypeDeclHandler' function. */
#undef HAVE_XML_SETDOCTYPEDECLHANDLER

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#undef LT_OBJDIR

/* Name of package */
#undef PACKAGE

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#undef PACKAGE_NAME

/* Define to the full name and version of this package. */
#undef PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the home page for this package. */
#undef PACKAGE_URL

/* Define to the version of this package. */
#undef PACKAGE_VERSION

/* The size of `void *', as computed by sizeof. */
#if defined(MIKTEX_WINDOWS)
#if defined(_WIN64)
#  define SIZEOF_VOID_P 8
#else
#  define SIZEOF_VOID_P 4
#endif
#else
#undef SIZEOF_VOID_P
#endif

/* Define to 1 if you have the ANSI C header files. */
#undef STDC_HEADERS

/* Use iconv. */
#undef USE_ICONV

/* Use regex */
#undef USE_REGEX

/* Version number of package */
#undef VERSION

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
#  undef WORDS_BIGENDIAN
# endif
#endif

/* Define to empty if `const' does not conform to ANSI C. */
#undef const

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#undef inline
#endif

/* Define to `int' if <sys/types.h> does not define. */
#undef pid_t

#include "config-fixups.h"

#if defined(MIKTEX) && ! defined(DONT_USE_MIKTEX_EXIT)
#if defined(MIKTEX_WINDOWS)
#  define MIKTEX_USE_UTF8_FILE_NAMES 1
#endif
#  define USE_MIKTEX_EXIT
#  undef USE_FCSTAT_WORKAROUND
#endif
