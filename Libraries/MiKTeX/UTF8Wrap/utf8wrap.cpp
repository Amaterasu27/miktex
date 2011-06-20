/* utf8wrap.cpp:

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

#include <Windows.h>
#include <cstdarg>
#include <fcntl.h>
#include "internal.h"

/* _________________________________________________________________________

   UTF8ToWideChar
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(wchar_t *)
UTF8ToWideChar (/*[in]*/ const char * lpszUtf8)
{
  int len = MultiByteToWideChar(CP_UTF8,
				MB_ERR_INVALID_CHARS,
				lpszUtf8,
				-1,
				0,
				0);
  if (len <= 0)
    {
      throw fixme;
    }
  wchar_t * lpszWideChar = reinterpret_cast<wchar_t*>(malloc(len * sizeof(wchar_t)));
  if (lpszWideChar == 0)
    {
      throw fixme;
    }
  len = MultiByteToWideChar(CP_UTF8,
			    MB_ERR_INVALID_CHARS,
			    lpszUtf8,
			    -1,
			    lpszWideChar,
			    len);
  if (len <= 0)
    {
      free (lpszWideChar);
      throw fixme;
    }
  return (lpszWideChar);
}

/* _________________________________________________________________________

   WideCharToUTF8
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(char *)
WideCharToUTF8 (/*[in]*/ const wchar_t * lpszWideChar)
{
  int len = WideCharToMultiByte(CP_UTF8,
				0,
				lpszWideChar,
				-1,
				0,
				0,
				0,
				0);
  if (len <= 0)
    {
      throw fixme;
    }
  char * lpszUtf8 = reinterpret_cast<char*>(malloc(len * sizeof(char)));
  if (lpszUtf8 == 0)
    {
      throw fixme;
    }
  len = WideCharToMultiByte(CP_UTF8,
			    0,
			    lpszWideChar,
			    -1,
			    lpszUtf8,
			    len,
			    0,
			    0);
  if (len <= 0)
    {
      free (lpszUtf8);
      throw fixme;
    }
  return (lpszUtf8);
  }
}

/* _________________________________________________________________________

   WideCharBuffer
   _________________________________________________________________________ */

class WideCharBuffer
{
public:
  WideCharBuffer (/*[in]*/ const char * lpszUtf8)
    : lpszWideChar(UTF8ToWideChar(lpszUtf8))
  {
  }
public:
  ~WideCharBuffer ()
  {
    if (lpszWideChar != 0)
    {
      free (lpszWideChar);
      lpszWideChar = 0;
    }
  }
public:
  wchar_t * Get() { return (lpszWideChar); }
private:
  wchar_t * lpszWideChar;
};

#define UW_(x) WideCharBuffer(x).Get()

/* _________________________________________________________________________

   Utf8Buffer
   _________________________________________________________________________ */

class Utf8Buffer
{
public:
  Utf8Buffer (/*[in]*/ const wchar_t * lpszWideChar)
    : lpszUtf8(WideCharToUtf8(lpszWideChar))
  {
  }
public:
  ~Utf8Buffer ()
  {
    if (lpszUtf8 != 0)
    {
      free (lpszUtf8);
      lpszUtf8 = 0;
    }
  }
public:
  char * Get() { return (lpszUtf8); }
private:
  char * lpszUtf8;
};

#define WU_(x) Utf8Buffer(x).Get()

/* _________________________________________________________________________

   miktex_utf8_fopen
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(FILE *)
miktex_utf8_fopen (/*[in]*/ const char * lpszFileName, /*[in]*/ const char * lpszMode)
{
  return (_wfopen(UW_(lpszFileName), UW_(lpszMode)));
}

/* _________________________________________________________________________

   miktex_utf8_open
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_open (/*[in]*/ const char * lpszFileName, /*[in]*/ int flags, ...)
{
  int pmode = 0;
  if ((flags & _O_CREAT) != 0)
  {
    va_list ap;
    va_start(ap, flags);
    pmode = va_arg(ap, int);
    va_end(ap);
  }
  return (_wopen(UW_(lpszFileName), pmode));
}

/* _________________________________________________________________________

   miktex_utf8_stat
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_stat (/*[in]*/ const char * lpszFileName, /*[out]*/ struct miktex_utf8_stat * pStat)
{
  struct _stat statbuf;
  int result = _wstat(UW_(lpszFileName), &statbuf);
  pStat->st_atime = statbuf.st_atime;
  pStat->st_ctime = statbuf.st_ctime;
  pStat->st_dev = statbuf.st_dev;
  pStat->st_gid = statbuf.st_gid;
  pStat->st_ino = statbuf.st_ino;
  pStat->st_mode = statbuf.st_mode;
  pStat->st_mtime = statbuf.st_mtime;
  pStat->st_nlink = statbuf.st_nlink;
  pStat->st_rdev = statbuf.st_rdev;
  pStat->st_size = statbuf.st_size;
  pStat->st_uid = statbuf.st_uid;
  return (result);
}

/* _________________________________________________________________________

   miktex_utf8_unlink
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_unlink (/*[in]*/ const char * lpszFileName)
{
  return (_wunlink(UW_(lpszFileName)));
}

/* _________________________________________________________________________

   miktex_utf8_access
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_access (/*[in]*/ const char * lpszFileName, /*[in]*/ int mode)
{
  return (_waccess(UW_(lpszFileName), mode));
}

/* _________________________________________________________________________

   miktex_utf8_chmod
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_chmod (/*[in]*/ const char * lpszFileName, /*[in]*/ int mode)
{
  return (_wchmod(UW_(lpszFileName), mode));
}

/* _________________________________________________________________________

   miktex_utf8_mkdir
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_mkdir (/*[in]*/ const char * lpszDirectoryName)
{
  return (_wmkdir(UW_(lpszDirectoryName)));
}

/* _________________________________________________________________________

   miktex_utf8_rmdir
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_rmdir (/*[in]*/ const char * lpszDirectoryName)
{
  return (_wrmdir(UW_(lpszDirectoryName)));
}

/* _________________________________________________________________________

   miktex_utf8_chdir
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_chdir (/*[in]*/ const char * lpszDirectoryName)
{
  return (_wchdir(UW_(lpszDirectoryName)));
}

/* _________________________________________________________________________

   miktex_utf8_getcwd
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(char *)
miktex_utf8_getcwd (/*[out]*/ char *	lpszDirectoryName,
		    size_t		maxSize)
{
  wchar_t * lpszWideChar = reinterpret_cast<wchar_t*>(malloc(maxSize * sizeof(wchar_t)));
  if (lpszWideChar == 0)
    {
      throw fixme;
    }
  if (_wgetcwd(lpszWideChar, maxSize) == 0)
    {
      free (lpszWideChar);
      return (0);
    }
  Utf8Buffer utf8 (lpszWideChar);
  free (lpszWideChar);
  if (wcslen(utf8.Get()) >= maxSize)
    {
      throw fixme;
    }      
  wcscpy (lpszDirectoryName, utf8.Get());
  return (lpszDirectoryName);
}

/* _________________________________________________________________________

   miktex_utf8_utime
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_utime (/*[in]*/ const char *		lpszFileName,
		   /*[in]*/ const struct utimbuf *	pTime)
{
  return (_wutime(UW_(lpszFileName), pTime));
}
