/* miktex/utf8wrap.h: Unx emulation				-*- C++ -*-

   Copyright (C) 2011 Christian Schenk

   This file is part of the MiKTeX UTF8Wrap Library.

   The MiKTeX UTF8Wrap Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX UTF8Wrap Library is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX UTF8Wrap Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(B2E524AD08FE4528BE66DC6ED9CF716C)
#define B2E524AD08FE4528BE66DC6ED9CF716C

#include <miktex/Core/Definitions.h>

#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/utime.h>

// DLL import/export switch
#if ! defined(BF56453E041E4B58A0EA455A65DD28B1)
#  if defined(_MSC_VER)
#    define MIKTEXUTF8WRAPEXPORT __declspec(dllimport)
#  else
#    define MIKTEXUTF8WRAPEXPORT
#  endif
#endif

// API decoration for exported functions
#define MIKTEXUTF8WRAPCEEAPI(type) MIKTEXUTF8WRAPEXPORT type MIKTEXCEECALL

#if defined(__cplusplus)
extern "C" {
#endif

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_access (/*[in]*/ const char * lpszFileName, /*[in]*/ int mode);

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_chdir (/*[in]*/ const char * lpszDirectoryName);

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_chmod (/*[in]*/ const char * lpszFileName, /*[in]*/ int mode);

MIKTEXUTF8WRAPCEEAPI(FILE *)
miktex_utf8_fopen (/*[in]*/ const char * lpszFileName, /*[in]*/ const char * lpszMode);

MIKTEXUTF8WRAPCEEAPI(char *)
miktex_utf8_getcwd (/*[out]*/ char * lpszDirectoryName, size_t maxSize)

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_mkdir (/*[in]*/ const char * lpszDirectoryName);

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_open (/*[in]*/ const char * lpszFileName, /*[in]*/ int flags, ...);

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_rmdir (/*[in]*/ const char * lpszDirectoryName);

struct miktex_utf8_stat
{
  _dev_t     st_dev;
  _ino_t     st_ino;
  unsigned short st_mode;
  short      st_nlink;
  short      st_uid;
  short      st_gid;
  _dev_t     st_rdev;
  _off_t     st_size;
  time_t st_atime;
  time_t st_mtime;
  time_t st_ctime;
};

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_stat (/*[in]*/ const char * lpszFileName, /*[out]*/ struct miktex_utf8_stat * pStat);

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_unlink (/*[in]*/ const char * lpszFileName);

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_utime (/*[in]*/ const char * lpszFileName, /*[in]*/ const struct utimbuf * pTime);

#if defined(__cplusplus)
}
#endif

#if ! defined(BF56453E041E4B58A0EA455A65DD28B1)
#  define _access miktex_utf8_access
#  define access miktex_utf8_access
#  define chdir miktex_utf8_chdir
#  define _chmod miktex_utf8_chmod
#  define chmod miktex_utf8_chmod
#  define fopen miktex_utf8_fopen
#  define getcwd miktex_utf8_getcwd
#  define _mkdir miktex_utf8_mkdir
#  define mkdir miktex_utf8_mkdir
#  define _open miktex_utf8_open
#  define open miktex_utf8_open
//#  define _stat miktex_utf8_stat
#  define rmdir miktex_utf8_rmdir
#  define stat miktex_utf8_stat
#  define unlink miktex_utf8_unlink
#  define _unlink miktex_utf8_unlink
#  define utime miktex_utf8_utime
#endif

#endif
