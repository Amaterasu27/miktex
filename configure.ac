# configure.ac:
#
# Copyright (C) 2005-2006 Christian Schenk
# 
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
# 
# This file is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this file; if not, write to the Free Software Foundation,
# 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

AC_PREREQ([2.59])

# Initialize autotools.  This is the place were we specify the MiKTeX
# version.
#
# Anatomy of the MiKTeX version string: depending on the release
# state, the version string has one of the following formats:
#
# for a general availability release:  MAJOR.MINOR.J2000
#            for a release candidate:  MAJOR.MINOR.J2000-rc-RCNUM
#                 for a beta release:  MAJOR.MINOR.J2000-beta-BETANUM
#             for a snapshot release:  MAJOR.MINOR.J2000-snapshot
#
# Legend:
#    MAJOR   the major version number
#    MINOR   the minor version number
#    J2000   days since Jan 1, 2000
#    RCNUM   release candidate number
#  BETANUM   beta number
AC_INIT([MiKTeX Tools],[2.5.2331-beta-8])

# write configuration settings into ./config.h
AC_CONFIG_HEADERS([config.h])

# install auxiliary in ./config/
AC_CONFIG_AUX_DIR([config])

# places where autotools look for m4 files
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_MACRO_DIR([Libraries/3rd/gl/m4])

# initialize automake:
# - MiKTeX is a "foreign" (non-GNU) project
# - the API doc file names are rather long
# - use a tar that can handle extra long file names
AM_INIT_AUTOMAKE([foreign filename-length-max=255 tar-ustar])

# early gnulib initialization
AC_PROG_CC
gl_EARLY

# initialize libtool
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

# initialize C++ support
AC_PROG_CXX

# split the MiKTeX version string into parts
AX_SPLIT_VERSION
AC_SUBST([MIKTEX_MAJOR_VERSION])
AC_SUBST([MIKTEX_MINOR_VERSION])
AC_SUBST([MIKTEX_J2000_VERSION])
AC_SUBST([MIKTEX_RELEASE_STATE])
AC_SUBST([MIKTEX_RELEASE_NUMBER])

# define the MiKTeX version string
if test $MIKTEX_RELEASE_STATE -eq 0; then
  MIKTEX_VERSION_STR="$MIKTEX_MAJOR_VERSION.$MIKTEX_MINOR_VERSION"
fi
if test $MIKTEX_RELEASE_STATE -eq 1; then
  MIKTEX_VERSION_STR="$MIKTEX_MAJOR_VERSION.$MIKTEX_MINOR_VERSION RC $MIKTEX_RELEASE_NUMBER"
fi
if test $MIKTEX_RELEASE_STATE -eq 2; then
  MIKTEX_VERSION_STR="$MIKTEX_MAJOR_VERSION.$MIKTEX_MINOR_VERSION Beta $MIKTEX_RELEASE_NUMBER"
fi
if test $MIKTEX_RELEASE_STATE -eq 4; then
  MIKTEX_VERSION_STR="$MIKTEX_MAJOR_VERSION.$MIKTEX_MINOR_VERSION.$MIKTEX_J2000_VERSION snapshot"
fi
AC_SUBST([MIKTEX_VERSION_STR])

# define the MiKTEX series string
MIKTEX_SERIES_STR="$MIKTEX_MAJOR_VERSION.$MIKTEX_MINOR_VERSION"
AC_SUBST([MIKTEX_SERIES_STR])

# initialize gnulib
gl_SOURCE_BASE(Libraries/3rd/gl)
gl_M4_BASE(Libraries/3rd/gl/m4)
gl_LIB(libgl)
gl_MODULES(canonicalize setenv)
gl_INIT

# extra gnulib features
gl_AC_TYPE_UINTMAX_T

# define the MIKTEX preprocessor symbol
AC_DEFINE([MIKTEX],[1],[Defined if we are using MiKTeX])

# turn off assert()ions
if test $MIKTEX_RELEASE_STATE -lt 4; then
  AC_DEFINE([NDEBUG],[1],[Defined if assert() shall do nothing])
fi

# find TEXMF root directories; invade Web2C territory, if kpsewhich
# is available
AC_CHECK_PROG([KPSEWHICH],[kpsewhich],[kpsewhich], [no])
if test -z "$MIKTEX_ROOTS" ; then
  if test "$KPSEWHICH" = "kpsewhich" ; then
    MIKTEX_ROOTS="`kpsewhich --expand-path \\$TEXMF`"
  fi
fi
if test -z "$MIKTEX_INSTALLROOT" ; then
  if test "$KPSEWHICH" = "kpsewhich" ; then
    MIKTEX_INSTALLROOT="`kpsewhich --expand-var \\$TEXMFLOCAL`"
  fi
fi
AC_SUBST([MIKTEX_DATAROOT])
AC_SUBST([MIKTEX_INSTALLROOT])
AC_SUBST([MIKTEX_ROOTS])

# specify Makefiles to be written
AC_CONFIG_FILES([BuildUtilities/etc/Makefile])
AC_CONFIG_FILES([Documentation/Makefile])
AC_CONFIG_FILES([Doxyfile])
AC_CONFIG_FILES([Libraries/3rd/bzip2/Makefile])
AC_CONFIG_FILES([Libraries/3rd/gl/Makefile])
AC_CONFIG_FILES([Libraries/3rd/libexpat/Makefile])
AC_CONFIG_FILES([Libraries/3rd/libmd5/Makefile])
AC_CONFIG_FILES([Libraries/3rd/libmspack/Makefile])
AC_CONFIG_FILES([Libraries/3rd/libpopt/Makefile])
AC_CONFIG_FILES([Libraries/3rd/zlib/Makefile])
AC_CONFIG_FILES([Libraries/MiKTeX/App/Makefile])
AC_CONFIG_FILES([Libraries/MiKTeX/Core/Makefile])
AC_CONFIG_FILES([Libraries/MiKTeX/Core/test/Makefile])
AC_CONFIG_FILES([Libraries/MiKTeX/Core/test/files/Makefile])
AC_CONFIG_FILES([Libraries/MiKTeX/Core/test/fndb/Makefile])
AC_CONFIG_FILES([Libraries/MiKTeX/Core/test/mmap/Makefile])
AC_CONFIG_FILES([Libraries/MiKTeX/Core/test/pathname/Makefile])
AC_CONFIG_FILES([Libraries/MiKTeX/Core/test/process/Makefile])
AC_CONFIG_FILES([Libraries/MiKTeX/Core/test/scartchdir/Makefile])
AC_CONFIG_FILES([Libraries/MiKTeX/Core/test/thread/Makefile])
AC_CONFIG_FILES([Libraries/MiKTeX/PackageManager/Makefile])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([Programs/MiKTeX/PackageManager/mpm/Makefile])
AC_CONFIG_FILES([Programs/MiKTeX/initexmf/Makefile])
AC_CONFIG_FILES([Programs/MiKTeX/mthelp/Makefile])
AC_CONFIG_FILES([miktex-config])

# specify other config files to be written
AC_CONFIG_FILES([Documentation/version.ent])
AC_CONFIG_FILES([Libraries/MiKTeX/Core/include/miktex/version.h])

# check availability of header files
AC_CHECK_HEADERS([dirent.h])
AC_CHECK_HEADERS([float.h])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([utime.h])
AC_CHECK_HEADERS([sys/mman.h])
AC_CHECK_HEADERS([sys/statvfs.h])
AC_CHECK_HEADERS([sys/utime.h])
AC_CHECK_HEADERS([sys/utsname.h])
AC_CHECK_HEADERS([sys/wait.h])

# check to see if struct dirent has the d_type member
AC_CHECK_MEMBER([struct dirent.d_type],
		[AC_DEFINE([HAVE_STRUCT_DIRENT_D_TYPE],
		           [1],
                           [Define if struct dirent has member d_type])],
                ,
		[#include <dirent.h>])

# check availability of some special functions
AC_FUNC_FORK
AC_FUNC_MMAP
AG_CHECK_UNAME_SYSCALL

# check availability of some other functions
AC_CHECK_FUNCS([confstr])
AC_CHECK_FUNCS([geteuid])
AC_CHECK_FUNCS([getuid])
AC_CHECK_FUNCS([memcmp])
AC_CHECK_FUNCS([memmove])
AC_CHECK_FUNCS([setreuid])
AC_CHECK_FUNCS([setuid])
AC_CHECK_FUNCS([statvfs])
AC_CHECK_FUNCS([strerror])
AC_CHECK_FUNCS([utime])
AC_CHECK_FUNCS([chown])

# check availability of C++ extensions
AC_CXX_HAVE_STD_EXT_HASH_SET
AC_CXX_HAVE_GNU_EXT_HASH_SET
AC_CXX_HAVE_GLOBAL_HASH_SET
AC_CXX_HAVE_STD_EXT_HASH_MAP
AC_CXX_HAVE_GNU_EXT_HASH_MAP
AC_CXX_HAVE_GLOBAL_HASH_MAP

# check availability of the pthread library
ACX_PTHREAD

# check availability of libCURL
LIBCURL_CHECK_CONFIG

# write all files
AC_OUTPUT
