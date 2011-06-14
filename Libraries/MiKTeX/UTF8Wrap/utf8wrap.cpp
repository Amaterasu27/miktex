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
  //MIKTEX_ASSERT (Utils::IsUTF8(lpszUtf8));
  int len = MultiByteToWideChar(
    CP_UTF8,
    MB_ERR_INVALID_CHARS,
    lpszUtf8,
    -1,
    0,
    0);
  if (len <= 0)
  {
    //FATAL_WINDOWS_ERROR ("MultiByteToWideChar", 0);
    return (0);
  }
  wchar_t * lpszWideChar = reinterpret_cast<wchar_t*>(malloc(len * sizeof(wchar_t)));
  len = MultiByteToWideChar(
    CP_UTF8,
    MB_ERR_INVALID_CHARS,
    lpszUtf8,
    -1,
    lpszWideChar,
    len);
  if (len <= 0)
  {
    free (lpszWideChar);
    //FATAL_WINDOWS_ERROR ("MultiByteToWideChar", 0);
    return (0);
  }
  return (lpszWideChar);
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
