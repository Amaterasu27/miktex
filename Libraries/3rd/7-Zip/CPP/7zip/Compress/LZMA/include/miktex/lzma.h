/* lzma.h:							-*- C++ -*-

   Copyright (C) 1999-2007 Igor Pavlov
   Copyright (C) 2007 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.
   
   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */


#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(ONCE_E12B043C_9DFC_43ED_82FE_4367D658F8CC_)
#define ONCE_E12B043C_9DFC_43ED_82FE_4367D658F8CC_

#if ! defined(CA7C1BB6_EF7C_4F72_ADDD_8975CD8C84EB_)
#  if ! defined(MIKTEX_STATIC) && defined(_MSC_VER)
#    define LZMAEXPORT __declspec(dllimport)
#  else
#    define LZMAEXPORT
#  endif
#endif

#if defined(_MSC_VER)
#  define LZMACALL __cdecl
#else
#  define LZMACALL
#endif

#define LZMAAPI(type) LZMAEXPORT type LZMACALL

#define LZMA_BEGIN_NAMESPACE			\
  namespace MiKTeX {				\
    namespace LZMA {

#define LZMA_END_NAMESPACE			\
    }						\
  }

LZMA_BEGIN_NAMESPACE;

LZMAAPI(void *)
CreateDecoder ();

LZMA_END_NAMESPACE;

#undef LZMA_BEGIN_NAMESPACE
#undef LZMA_END_NAMESPACE

#endif
