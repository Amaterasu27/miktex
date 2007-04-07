/* miktex/unxemu.h: Unx emulation				-*- C++ -*-

   Copyright (C) 2007 Christian Schenk

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

#if ! defined(D2A2BA842ACE40C6A8A17A9358F2147E)
#  if defined(_MSC_VER)
#    define MIKTEXUNXEXPORT __declspec(dllimport)
#  else
#    define MIKTEXUNXEXPORT
#  endif
#endif

#if defined(_MSC_VER)
#  define MIKTEXUNXCALL __stdcall
#else
#  define MIKTEXUNXCALL
#endif

#define MIKTEXUNXAPI(type) MIKTEXUNXEXPORT type MIKTEXUNXCALL
#define MIKTEXUNXDATA(type) MIKTEXUNXEXPORT type

#include <sys/stat.h>

#define S_ISDIR(m) (((m) & _S_IFDIR) != 0)

#define F_OK 0
#define W_OK 2
#define R_OK 4
#define X_OK W_OK

#if defined(__cplusplus)
extern "C" {
#endif

struct DIR_;

typedef struct DIR_ DIR;

struct dirent
{
  int d_ino;
  char d_name[260];
};

MIKTEXUNXAPI(int)
closedir (/*[in]*/ DIR * pDir);

MIKTEXUNXAPI(DIR *)
opendir (/*[in]*/ const char * lpszPath);

MIKTEXUNXAPI(struct dirent *)
readdir (/*[in]*/ DIR * pDir);

#if defined(__cplusplus)
}
#endif
