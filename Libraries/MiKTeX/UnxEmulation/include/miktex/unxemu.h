/* miktex/unxemu.h: Unx emulation				-*- C++ -*-

   Copyright (C) 2007-2010 Christian Schenk

   This file is part of the MiKTeX UNXEMU Library.

   The MiKTeX UNXEMU Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX UNXEMU Library is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX UNXEMU Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#include <miktex/Core/Definitions>
#include <miktex/Core/Core.h>
#include <string.h>
#include <sys/stat.h>
#include <malloc.h>
#include <float.h>
#include <direct.h>
#include <process.h>
#if 0
#include <WinSock2.h> /* timeval */
#endif

// DLL import/export switch
#if ! defined(D2A2BA842ACE40C6A8A17A9358F2147E)
#  if defined(_MSC_VER)
#    define MIKTEXUNXEXPORT __declspec(dllimport)
#  else
#    define MIKTEXUNXEXPORT
#  endif
#endif

// API decoration for exported functions
#define MIKTEXUNXCEEAPI(type) MIKTEXUNXEXPORT type MIKTEXCEECALL

/* _________________________________________________________________________

   sys/time.h
   _________________________________________________________________________ */

#define gettimeofday miktex_gettimeofday

#if defined(__cplusplus)
extern "C" {
#endif

MIKTEXUNXCEEAPI(int)
miktex_gettimeofday(/*[in]*/ struct timeval * ptv, void * pNull);

#if defined(__cplusplus)
}
#endif

/* _________________________________________________________________________

   stdio.h
   _________________________________________________________________________ */

#define pclose _pclose
#define popen _popen
#define snprintf _snprintf

/* _________________________________________________________________________

   ctype.h
   _________________________________________________________________________ */

#define isblank(c) ((c) == ' ' || (c) == '\t')

/* _________________________________________________________________________

   strings.h
   _________________________________________________________________________ */

#define bzero(ptr, len) memset(ptr, 0, len)

#if defined(__cplusplus)
#  define strcasecmp(s1, s2) MiKTeX::Core::StringCompare(s1, s2, true)
#else
#  define strcasecmp(s1, s2) miktex_strcasecmp(s1, s2)
#endif

#if defined(__cplusplus)
#  define strncasecmp(s1, s2, n) MiKTeX::Core::StringCompare(s1, s2, n, true)
#else
#  define strncasecmp(s1, s2, n) miktex_strncasecmp(s1, s2, n)
#endif

#if defined(__cplusplus)
extern "C" {
#endif

MIKTEXUNXCEEAPI(int)
miktex_strncasecmp (/*[in]*/ const char * lpsz1,
		    /*[in]*/ const char * lpsz2,
		    /*[in]*/ size_t	  n);

#if defined(__cplusplus)
}
#endif

/* _________________________________________________________________________

   unistd.h
   _________________________________________________________________________ */

#define F_OK 0
#define W_OK 2
#define R_OK 4
#define X_OK W_OK

#if ! defined(getcwd)
#  define getcwd _getcwd
#endif

#if ! defined(getpid)
#  define getpid _getpid
#endif

#if ! defined(rmdir)
#  define rmdir _rmdir
#endif

/* _________________________________________________________________________

   alloca.h
   _________________________________________________________________________ */

#if ! defined(alloca)
#  define alloca _malloca
#endif

/* _________________________________________________________________________

   sys/stat.h
   _________________________________________________________________________ */

#define S_ISDIR(m) (((m) & _S_IFDIR) != 0)
#define S_ISREG(m) (((m) & _S_IFREG) != 0)

/* _________________________________________________________________________

   math.h
   _________________________________________________________________________ */

#define rint(x) ( (double) ((int)((x) + 0.5)) )

#if ! defined(finite)
#  define finite _finite
#endif

#if ! defined(isnan)
#  define isnan _isnan
#endif

/* _________________________________________________________________________

   dirent.h
   _________________________________________________________________________ */

#if defined(__cplusplus)
extern "C" {
#endif

struct DIR_;
struct WDIR_;

typedef struct DIR_ DIR;
typedef struct WDIR_ WDIR;

struct dirent
{
  int d_ino;
  char d_name[260];
};

struct wdirent
{
  int d_ino;
  wchar_t d_name[260];
};

MIKTEXUNXCEEAPI(int)
closedir (/*[in]*/ DIR * pDir);

MIKTEXUNXCEEAPI(DIR *)
opendir (/*[in]*/ const char * lpszPath);

MIKTEXUNXCEEAPI(struct dirent *)
readdir (/*[in]*/ DIR * pDir);

MIKTEXUNXCEEAPI(int)
wclosedir (/*[in]*/ WDIR * pDir);

MIKTEXUNXCEEAPI(WDIR *)
wopendir (/*[in]*/ const wchar_t * lpszPath);

MIKTEXUNXCEEAPI(struct wdirent *)
wreaddir (/*[in]*/ WDIR * pDir);

#if defined(__cplusplus)
}
#endif
