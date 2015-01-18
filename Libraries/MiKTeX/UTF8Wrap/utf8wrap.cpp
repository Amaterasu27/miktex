/* utf8wrap.cpp:

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

#include <Windows.h>
#include <cstdarg>
#include <exception>
#include <vector>
#include <fcntl.h>
#include "internal.h"

using namespace std;

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
      throw std::exception("MultiByteToWideChar() failed for some reason");
    }
  wchar_t * lpszWideChar = reinterpret_cast<wchar_t*>(malloc(len * sizeof(wchar_t)));
  if (lpszWideChar == 0)
    {
      throw std::exception("out of memory");
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
      throw std::exception("MultiByteToWideChar() failed for some reason");
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
      throw std::exception("WideCharToMultiByte() failed for some reason");
    }
  char * lpszUtf8 = reinterpret_cast<char*>(malloc(len * sizeof(char)));
  if (lpszUtf8 == 0)
    {
      throw std::exception("out of memory");
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
      throw std::exception("WideCharToMultiByte() failed for some reason");
    }
  return (lpszUtf8);
};

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
    : lpszUtf8(WideCharToUTF8(lpszWideChar))
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
  return (_wopen(UW_(lpszFileName), flags, pmode));
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

   miktex_utf8_remove
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_remove (/*[in]*/ const char * lpszFileName)
{
  return (_wremove(UW_(lpszFileName)));
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
      throw std::exception("out of memory");
    }
  if (_wgetcwd(lpszWideChar, maxSize) == 0)
    {
      free (lpszWideChar);
      return (0);
    }
  Utf8Buffer utf8 (lpszWideChar);
  free (lpszWideChar);
  if (strlen(utf8.Get()) >= maxSize)
    {
      throw std::exception("buffer too small");
    }      
  strcpy (lpszDirectoryName, utf8.Get());
  return (lpszDirectoryName);
}

/* _________________________________________________________________________

   miktex_utf8_utime
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_utime (/*[in]*/ const char *		lpszFileName,
		   /*[in]*/ const struct utimbuf *	pTime)
{
  struct _utimbuf buf;
  buf.actime = pTime->actime;
  buf.modtime = pTime->modtime;
  return (_wutime(UW_(lpszFileName), &buf));
}

/* _________________________________________________________________________

   miktex_utf8_rename
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_rename (/*[in]*/ const char * lpszOld, /*[in]*/ const char * lpszNew)
{
  return (_wrename(UW_(lpszOld), UW_(lpszNew)));
}

/* _________________________________________________________________________

   miktex_utf8_spawnvp
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(intptr_t)
miktex_utf8_spawnvp (/*[in]*/ int mode,
		     /*[in]*/ const char * lpszFileName,
		     /*[in]*/ const char * const * argv)
{
  vector<wstring> wideArguments;
  int count;
  for (count = 0; argv[count] != 0; ++ count)
  {
    wideArguments.push_back (UW_(argv[count]));
  }
  vector<const wchar_t *> wargv;
  wargv.reserve (count + 1);
  for (int idx = 0; idx < count; ++ idx)
  {
    wargv.push_back (wideArguments[idx].c_str());
  }
  wargv.push_back (0);
  return (_wspawnvp(mode, UW_(lpszFileName), &wargv[0]));
}

/* _________________________________________________________________________

   miktex_utf8_system
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int)
miktex_utf8_system (/*[in]*/ const char * lpszCommand)
{
  return (_wsystem(lpszCommand == 0 ? 0 : UW_(lpszCommand)));
}

/* _________________________________________________________________________

   miktex_utf8_popen
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(FILE *)
miktex_utf8_popen (/*[in]*/ const char * lpszCommand,
		   /*[in]*/ const char * lpszMode)
{
  return (_wpopen(UW_(lpszCommand), UW_(lpszMode)));
}

/* _________________________________________________________________________

   GetConsoleHandle
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(HANDLE) GetConsoleHandle(/*[in]*/ FILE * pFile)
{
  bool isStdout = fileno(pFile) == fileno(stdout);
  bool isStderr = fileno(pFile) == fileno(stderr);
  if (_isatty(fileno(pFile)) && (isStdout || isStderr))
  {
    return isStdout ? GetStdHandle(STD_OUTPUT_HANDLE) : GetStdHandle(STD_ERROR_HANDLE);
  }
  else
  {
    return INVALID_HANDLE_VALUE;
  }
}

/* _________________________________________________________________________

   NonUtf8ConsoleWarning
   _________________________________________________________________________ */

#define MIKTEX_UTF8_CONSOLE_WARNING 0

#if MIKTEX_UTF8_CONSOLE_WARNING
class NonUtf8ConsoleWarning
{
public:
  NonUtf8ConsoleWarning()
    : pConsole(0)
  {
  }
public:
  ~NonUtf8ConsoleWarning()
  {
    const char * envvar = "MIKTEX_UTF8_SUPPRESS_CONSOLE_WARNING";
    if (pConsole != 0 && getenv(envvar) == 0)
    {
      fprintf(pConsole, "\
\n\n\
*** Warning:\n\
***\n\
*** Some characters could not be printed because the console's active code\n\
*** page is not UTF-8.\n\
***\n\
*** See http://miktex.org/howto/utf8-console for more information.\n\
***\n\
*** You can suppress this warning by setting the environment variable\n\
*** %s\n",
           "MIKTEX_UTF8_SUPPRESS_CONSOLE_WARNING");
    }
  }
public:
  void Emit(/*[in]*/ FILE * pConsole)
  {
    this->pConsole = pConsole;
  }
private:
  FILE * pConsole;
};
#endif

/* _________________________________________________________________________

   miktex_utf8_fputc

   FIXME: not thread-safe
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int) miktex_utf8_fputc (/*[in]*/ int ch, /*[in]*/ FILE * pFile)
{
  HANDLE hConsole = GetConsoleHandle(pFile);
  if (hConsole == INVALID_HANDLE_VALUE)
  {
    return fputc(ch, pFile);
  }
  if (GetConsoleOutputCP() != CP_UTF8)
  {
#if MIKTEX_UTF8_CONSOLE_WARNING
    static NonUtf8ConsoleWarning warning;
    if ((ch & 0x80) != 0)
    {
      warning.Emit(pFile);
      ch = '?';
    }
#endif
    return fputc(ch, pFile);
  }
  static int remaining = 0;
  static char buf[5];
  static int bufidx = 0;
  if (bufidx > 4)
  {
    throw std::exception("invalid UTF-8 byte sequence");
  }
  buf[bufidx++] = ch;
  if ((ch & 0x80) == 0)
  {
    remaining = 0;
  }
  else if ((ch & 0xc0) == 0x80)
  {
    if (remaining == 0)
    {
      throw std::exception("invalid UTF-8 byte sequence");
    }
    -- remaining;
  }
  else if ((ch & 0xe0) == 0xc0)
  {
    remaining = 1;
  }
  else if ((ch & 0xf0) == 0xe0)
  {
    remaining = 2;
  }
  else if ((ch & 0xf8) == 0xf0)
  {
    remaining = 3;
  }
  if (remaining == 0)
  {
    buf[bufidx] = 0;
    bufidx = 0;
    if (fputs(buf, pFile) < 0)
    {
      ch = EOF;
    }
  }
  return ch;
}

/* _________________________________________________________________________

   miktex_utf8_putc
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int) miktex_utf8_putc (/*[in]*/ int ch, /*[in]*/ FILE * pFile)
{
  return miktex_utf8_fputc(ch, pFile);
}

/* _________________________________________________________________________

   miktex_utf8_putchar
   _________________________________________________________________________ */

MIKTEXUTF8WRAPCEEAPI(int) miktex_utf8_putchar (/*[in]*/ int ch)
{
  return miktex_utf8_fputc(ch, stdout);
}

/* _________________________________________________________________________

   miktex_utf8_fprintf
   _________________________________________________________________________ */

#if MIKTEX_UTF8_CONSOLE_WARNING
MIKTEXUTF8WRAPCEEAPI(int) miktex_utf8_fprintf (/*[in]*/ FILE * pFile, /*[in]*/ const char * lpszFormat, ...)
{
  HANDLE hConsole = GetConsoleHandle(pFile);
  va_list ap;
  va_start(ap, lpszFormat);
  int ret;
  if (hConsole == INVALID_HANDLE_VALUE || GetConsoleOutputCP() == CP_UTF8)
  {
    ret = vfprintf(pFile, lpszFormat, ap);
  }
  else
  {
    int n = _vscprintf(lpszFormat, ap);
    if (n >= 0)
    {
      char * pBuffer = new char[n+1];
      n = vsprintf_s(pBuffer, n + 1, lpszFormat, ap);
      if (n >= 0)
      {
        // TODO: check buffer and warn if it contains UTF-8 bytes
        n = fputs(pBuffer, pFile);
      }
      delete [] pBuffer;
    }
  }
  va_end(ap);
  return ret;
}
#endif
