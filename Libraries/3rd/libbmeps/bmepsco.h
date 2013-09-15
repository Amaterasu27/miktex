/*
Copyright (c) 2000-2005, Dirk Krause
All rights reserved.

Redistribution and use in source and binary forms,
with or without modification, are permitted provided
that the following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.
* Redistributions in binary form must reproduce the above 
  opyright notice, this list of conditions and the following
  disclaimer in the documentation and/or other materials
  provided with the distribution.
* Neither the name of the Dirk Krause nor the names of
  its contributors may be used to endorse or promote
  products derived from this software without specific
  prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/

#ifndef BMEPSCO_INC
#define BMEPSCO_INC 1

#if HAVE_CONFIG_H
#include <config.h>
#else
#ifndef HAVE_ZLIB
#define HAVE_ZLIB		1
#endif
#ifndef HAVE_LIBPNG
#define HAVE_LIBPNG		1
#endif
#ifndef HAVE_JPEGLIB_H
#define HAVE_JPEGLIB_H		1
#endif
#ifndef HAVE_TIFF_H
#if LZW_PATENT_EXPIRED
#define HAVE_TIFF_H             1
#else
#define HAVE_TIFF_H		0
#endif
#endif
#ifndef HAVE_PNM_H
#define HAVE_PNM_H		1
#endif
#if _MSC_VER
/* + MSVC */
#ifndef HAVE_STRICMP
#define HAVE_STRICMP		0
#endif
#ifndef HAVE__STRICMP
#define HAVE__STRICMP		1
#endif
#ifndef HAVE_UNLINK
#define HAVE_UNLINK		0
#endif
#ifndef HAVE__UNLINK
#define HAVE__UNLINK		1
#endif
#ifndef HAVE_GETPID
#define HAVE_GETPID		0
#endif
#ifndef HAVE__GETPID
#define HAVE__GETPID		1
#endif
#ifndef HAVE_PROCESS_H
#define HAVE_PROCESS_H		1
#endif
#ifndef HAVE_STDLIB_H
#define HAVE_STDLIB_H		1
#endif
#ifndef HAVE_SYS_TYPES_H
#define HAVE_SYS_TYPES_H	1
#endif
#ifndef HAVE_STRING_H
#define HAVE_STRING_H		1
#endif
#ifndef HAVE_MALLOC_H
#define HAVE_MALLOC_H		1
#endif
#if defined(MIKTEX)
#undef HAVE_SETJMP_H
#define HAVE_SETJMP_H 0
#else
#ifndef HAVE_SETJMP_H
#define HAVE_SETJMP_H		1
#endif
#endif
#ifndef HAVE__SETMODE
#define HAVE__SETMODE		1
#endif
#ifndef HAVE_SETMODE
#define HAVE_SETMODE		0
#endif
#ifndef HAVE__FILENO
#define HAVE__FILENO		1
#endif
#ifndef HAVE_FILENO
#define HAVE_FILENO		0
#endif
#ifndef HAVE_FCNTL_H
#define HAVE_FCNTL_H		1
#endif
#ifndef HAVE_IO_H
#define HAVE_IO_H		1
#endif
#ifndef HAVE_DOSWIN_SETMODE
#define HAVE_DOSWIN_SETMODE	1
#endif
/* - MSVC */
#else
/* + !MSVC */
#if __BORLANDC__
/* + BC */
#ifndef HAVE_STRICMP
#define HAVE_STRICMP		1
#endif
#ifndef HAVE_DOSWIN_SETMODE
#define HAVE_DOSWIN_SETMODE	1
#endif
/* - BC */
#else
/* + !MSVC && !BC */
#ifndef HAVE_STRCASECMP
#define HAVE_STRCASECMP		1
#endif
/* - !MSVC && !BC */
#endif
/* - !MSVC */
#endif
#ifndef HAVE_UNLINK
#define HAVE_UNLINK		1
#endif
#ifndef HAVE_GETPID
#define HAVE_GETPID		1
#endif
#ifndef HAVE_SYS_TYPES_H
#define HAVE_SYS_TYPES_H	1
#endif
#ifndef HAVE_STRING_H
#define HAVE_STRING_H		1
#endif
#ifndef HAVE_STDLIB_H
#define HAVE_STDLIB_H		1
#endif
#ifndef HAVE_MALLOC_H
#define HAVE_MALLOC_H		1
#endif
#ifndef HAVE_SETJMP_H
#define HAVE_SETJMP_H		1
#endif
#endif




#include <stdio.h>

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_MALLOC_H
#include <malloc.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#endif
#if HAVE_FCNTL_H
#include <fcntl.h>
#endif
#if HAVE_IO_H
#include <io.h>
#endif

#endif
/* ifndef BMEPSCO_INC */

