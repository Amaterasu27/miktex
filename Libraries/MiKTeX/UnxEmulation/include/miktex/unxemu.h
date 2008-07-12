/* miktex/unxemu.h: Unx emulation				-*- C++ -*-

   Copyright (C) 2007-2008 Christian Schenk

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
#include <sys/stat.h>

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


#define S_ISDIR(m) (((m) & _S_IFDIR) != 0)

#define F_OK 0
#define W_OK 2
#define R_OK 4
#define X_OK W_OK

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
