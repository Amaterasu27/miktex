/*
 * aconf.h
 *
 * Copyright 2002-2003 Glyph & Cog, LLC
 */

#ifndef ACONF_H
#define ACONF_H

#include <aconf2.h>

/*
 * Use A4 paper size instead of Letter for PostScript output.
 */
#cmakedefine A4_PAPER 1

/*
 * Do not allow text selection.
 */
#cmakedefine NO_TEXT_SELECT 1

/*
 * Include support for OPI comments.
 */
#cmakedefine OPI_SUPPORT 1

/*
 * Enable multithreading support.
 */
#cmakedefine MULTITHREADED 1

/*
 * Enable C++ exceptions.
 */
#cmakedefine USE_EXCEPTIONS 1

/*
 * Enable word list support.
 */
#cmakedefine TEXTOUT_WORD_LIST 1

/*
 * Use fixed point (instead of floating point) arithmetic.
 */
#cmakedefine USE_FIXEDPOINT 1

/*
 * Directory with the Xpdf app-defaults file.
 */
#cmakedefine APPDEFDIR 1

/*
 * Full path for the system-wide xpdfrc file.
 */
#cmakedefine SYSTEM_XPDFRC 1

/*
 * Various include files and functions.
 */
#cmakedefine HAVE_DIRENT_H 1
#cmakedefine HAVE_SYS_NDIR_H 1
#cmakedefine HAVE_SYS_DIR_H 1
#cmakedefine HAVE_NDIR_H 1
#cmakedefine HAVE_SYS_SELECT_H 1
#cmakedefine HAVE_SYS_BSDTYPES_H 1
#cmakedefine HAVE_STRINGS_H 1
#cmakedefine HAVE_BSTRING_H 1
#cmakedefine HAVE_POPEN 1
#cmakedefine HAVE_MKSTEMP 1
#cmakedefine HAVE_MKSTEMPS 1
#cmakedefine SELECT_TAKES_INT 1
#cmakedefine HAVE_FSEEKO 1
#cmakedefine HAVE_FSEEK64 1
#cmakedefine _FILE_OFFSET_BITS 1
#cmakedefine _LARGE_FILES 1
#cmakedefine _LARGEFILE_SOURCE 1
#cmakedefine HAVE_XTAPPSETEXITFLAG 1

/*
 * This is defined if using libXpm.
 */
#cmakedefine HAVE_X11_XPM_H 1

/*
 * This is defined if using t1lib.
 */
#cmakedefine HAVE_T1LIB_H 1

/*
 * One of these is defined if using FreeType 2.
 */
#cmakedefine HAVE_FREETYPE_H 1
#cmakedefine HAVE_FREETYPE_FREETYPE_H 1

/*
 * This is defined if using libpaper.
 */
#cmakedefine HAVE_PAPER_H 1

/*
 * Enable support for loading plugins.
 */
#cmakedefine ENABLE_PLUGINS 1

/*
 * Defined if the Splash library is avaiable.
 */
#cmakedefine HAVE_SPLASH 1

/*
 * Enable support for CMYK output.
 */
#cmakedefine SPLASH_CMYK 1

#endif
