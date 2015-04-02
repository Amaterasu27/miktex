/* lzma-miktex.cpp:						-*- C++ -*-

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

#include "StdAfx.h"

#include "../../../Common/MyInitGuid.h"

#include "LZMADecoder.h"

#if defined(MIKTEX_LZMA_SHARED) && defined(_MSC_VER)
#  define LZMAEXPORT __declspec(dllexport)
#else
#  define LZMAEXPORT
#endif

#define CA7C1BB6_EF7C_4F72_ADDD_8975CD8C84EB_
#include "include/miktex/lzma.h"

/* _________________________________________________________________________

   MiKTeX::LZMA::CreateDecoder
   _________________________________________________________________________ */

LZMAAPI(void *)
MiKTeX::LZMA::CreateDecoder ()
{
  NCompress::NLZMA::CDecoder * pDecoder = new NCompress::NLZMA::CDecoder();
  static_cast<ICompressCoder*>(pDecoder)->AddRef ();
  return (pDecoder);
}
