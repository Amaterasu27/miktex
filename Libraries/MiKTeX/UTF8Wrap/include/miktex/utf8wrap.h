/* miktex/utf8wrap.h: Unx emulation				-*- C++ -*-

   Copyright (C) 2011-2015 Christian Schenk

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

/* todo:
   getenv()
   putenv()
 */
#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(B2E524AD08FE4528BE66DC6ED9CF716C)
#define B2E524AD08FE4528BE66DC6ED9CF716C

#include <miktex/Core/Definitions.h>

#if defined(__cplusplus)
#include <cstdio>
#include <cstdlib>
#else
#include <stdio.h>
#include <stdlib.h>
#endif

#include <direct.h>
#include <io.h>
#include <process.h>
#include <sys/stat.h>
#include <sys/utime.h>

// DLL import/export switch
#if ! defined(BF56453E041E4B58A0EA455A65DD28B1)
#  if defined(MIKTEX_UTF8WRAP_SHARED) && defined(_MSC_VER)
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

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_fputc (/*[in]*/ int ch, /*[in]*/ FILE * pFile);

MIKTEXUTF8WRAPCEEAPI(char *)
miktex_utf8_getcwd (/*[out]*/ char * lpszDirectoryName, size_t maxSize);

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_mkdir (/*[in]*/ const char * lpszDirectoryName);

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_open (/*[in]*/ const char * lpszFileName, /*[in]*/ int flags, ...);

MIKTEXUTF8WRAPCEEAPI(FILE *)
miktex_utf8_popen (/*[in]*/ const char * lpszCommand,
		   /*[in]*/ const char * lpszMode);

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_putc (/*[in]*/ int ch, /*[in]*/ FILE *);

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_putchar (/*[in]*/ int ch);

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_rename (/*[in]*/ const char * lpszOld, /*[in]*/ const char * lpszNew);

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_remove (/*[in]*/ const char * lpszFileName);

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_rmdir (/*[in]*/ const char * lpszDirectoryName);

MIKTEXUTF8WRAPCEEAPI(intptr_t)
miktex_utf8_spawnvp (/*[in]*/ int mode,
		     /*[in]*/ const char * lpszFileName,
		     /*[in]*/ const char * const * argv);

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
miktex_utf8_system (/*[in]*/ const char * lpszCommand);

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_unlink (/*[in]*/ const char * lpszFileName);

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_utime (/*[in]*/ const char * lpszFileName, /*[in]*/ const struct utimbuf * pTime);

#if defined(__cplusplus)
}
#endif

#if MIKTEX_UTF8_WRAP_ALL
#  if ! defined(MIKTEX_UTF8_WRAP__ACCESS)
#    define MIKTEX_UTF8_WRAP__ACCESS 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_ACCESS)
#    define MIKTEX_UTF8_WRAP_ACCESS 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_CHDIR)
#    define MIKTEX_UTF8_WRAP_CHDIR 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP__CHMOD)
#    define MIKTEX_UTF8_WRAP__CHMOD 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_CHMOD)
#    define MIKTEX_UTF8_WRAP_CHMOD 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_FOPEN)
#    define MIKTEX_UTF8_WRAP_FOPEN 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_FPUTC)
#    define MIKTEX_UTF8_WRAP_FPUTC 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_GETCWD)
#    define MIKTEX_UTF8_WRAP_GETCWD 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP__MKDIR)
#    define MIKTEX_UTF8_WRAP__MKDIR 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_MKDIR)
#    define MIKTEX_UTF8_WRAP_MKDIR 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP__OPEN)
#    define MIKTEX_UTF8_WRAP__OPEN 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_OPEN)
#    define MIKTEX_UTF8_WRAP_OPEN 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP__POPEN)
#    define MIKTEX_UTF8_WRAP__POPEN 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_POPEN)
#    define MIKTEX_UTF8_WRAP_POPEN 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_PUTC)
#    define MIKTEX_UTF8_WRAP_PUTC 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_PUTCHAR)
#    define MIKTEX_UTF8_WRAP_PUTCHAR 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_RENAME)
#    define MIKTEX_UTF8_WRAP_RENAME 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_REMOVE)
#    define MIKTEX_UTF8_WRAP_REMOVE 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_RMDIR)
#    define MIKTEX_UTF8_WRAP_RMDIR 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP__SPAWNVP)
#    define MIKTEX_UTF8_WRAP__SPAWNVP 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_SPAWNVP)
#    define MIKTEX_UTF8_WRAP_SPAWNVP 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP__STAT)
#    define MIKTEX_UTF8_WRAP__STAT 0
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_STAT)
#    define MIKTEX_UTF8_WRAP_STAT 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP__SYSTEM)
#    define MIKTEX_UTF8_WRAP__SYSTEM 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_SYSTEM)
#    define MIKTEX_UTF8_WRAP_SYSTEM 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP__UNLINK)
#    define MIKTEX_UTF8_WRAP__UNLINK 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_UNLINK)
#    define MIKTEX_UTF8_WRAP_UNLINK 1
#  endif
#  if ! defined(MIKTEX_UTF8_WRAP_UTIME)
#    define MIKTEX_UTF8_WRAP_UTIME 1
#  endif
#endif

#if MIKTEX_UTF8_WRAP__ACCESS
#  define _access miktex_utf8_access
#endif

#if MIKTEX_UTF8_WRAP_ACCESS
#  define access miktex_utf8_access
#endif

#if MIKTEX_UTF8_WRAP_CHDIR
#  define chdir miktex_utf8_chdir
#endif

#if MIKTEX_UTF8_WRAP__CHMOD
#  define _chmod miktex_utf8_chmod
#endif

#if MIKTEX_UTF8_WRAP_CHMOD
#  define chmod miktex_utf8_chmod
#endif

#if MIKTEX_UTF8_WRAP_FOPEN
#  define fopen miktex_utf8_fopen
#endif

#if MIKTEX_UTF8_WRAP_FPUTC
#  define fputc miktex_utf8_fputc
#endif

#if MIKTEX_UTF8_WRAP_GETCWD
#  define getcwd miktex_utf8_getcwd
#endif

#if MIKTEX_UTF8_WRAP__MKDIR
#  define _mkdir miktex_utf8_mkdir
#endif

#if MIKTEX_UTF8_WRAP_MKDIR
#  define mkdir miktex_utf8_mkdir
#endif

#if MIKTEX_UTF8_WRAP__OPEN
#  define _open miktex_utf8_open
#endif

#if MIKTEX_UTF8_WRAP_OPEN
#if defined(__cplusplus)
inline int open(const char * lpszFileName, int flags)
{
  return miktex_utf8_open(lpszFileName, flags);
}
#else
#define open miktex_utf8_open
#endif
#endif

#if MIKTEX_UTF8_WRAP__POPEN
#  define _popen miktex_utf8_popen
#endif

#if MIKTEX_UTF8_WRAP_POPEN
#  define popen miktex_utf8_popen
#endif

#if MIKTEX_UTF8_WRAP_PUTC
#  undef putc
#  define putc miktex_utf8_putc
#endif

#if MIKTEX_UTF8_WRAP_PUTCHAR
#  undef putchar
#  define putchar miktex_utf8_putchar
#endif

#if MIKTEX_UTF8_WRAP_RENAME
#  define rename miktex_utf8_rename
#endif

#if MIKTEX_UTF8_WRAP_REMOVE
#if defined(__cplusplus)
inline int remove(const char * lpszFileName)
{
  return miktex_utf8_remove(lpszFileName);
}
#else
#define remove miktex_utf8_remove
#endif
#endif

#if MIKTEX_UTF8_WRAP_RMDIR
#  define rmdir miktex_utf8_rmdir
#endif

#if MIKTEX_UTF8_WRAP__SPAWNVP
#  define _spawnvp miktex_utf8_spawnvp
#endif

#if MIKTEX_UTF8_WRAP_SPAWNVP
#  define spawnvp miktex_utf8_spawnvp
#endif

#if MIKTEX_UTF8_WRAP__STAT
#  define _stat miktex_utf8_stat
#endif

#if MIKTEX_UTF8_WRAP_STAT
#  define stat miktex_utf8_stat
#endif

#if MIKTEX_UTF8_WRAP__SYSTEM
#  define _system miktex_utf8_system
#endif

#if MIKTEX_UTF8_WRAP_SYSTEM
#  define system miktex_utf8_system
#endif

#if MIKTEX_UTF8_WRAP__UNLINK
#  define _unlink miktex_utf8_unlink
#endif

#if MIKTEX_UTF8_WRAP_UNLINK
#  define unlink miktex_utf8_unlink
#endif

#if MIKTEX_UTF8_WRAP_UTIME
#  define utime miktex_utf8_utime
#endif

#endif
