/* trace.h:

   Copyright (C) 1996-2007 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(F0010C34E9B32544A57E69B47E7722FB)
#define F0010C34E9B32544A57E69B47E7722FB

#include <miktex/char.h>

#define MIKTEX_TRACE_ACCESS MIKTEXTEXT("access")
#define MIKTEX_TRACE_CONFIG MIKTEXTEXT("config")
#define MIKTEX_TRACE_CORE MIKTEXTEXT("core")
#define MIKTEX_TRACE_CURL MIKTEXTEXT("curl")
#define MIKTEX_TRACE_DIB MIKTEXTEXT("dib")
#define MIKTEX_TRACE_DVIBITMAP MIKTEXTEXT("dvibitmap")
#define MIKTEX_TRACE_DVICOLOR MIKTEXTEXT("dvicolor")
#define MIKTEX_TRACE_DVIFILE MIKTEXTEXT("dvifile")
#define MIKTEX_TRACE_DVIGC MIKTEXTEXT("dvigc")
#define MIKTEX_TRACE_DVIHYPERTEX MIKTEXTEXT("dvihypertex")
#define MIKTEX_TRACE_DVIPAGE MIKTEXTEXT("dvipage")
#define MIKTEX_TRACE_DVIPKBITMAP MIKTEXTEXT("dvipkbitmap")
#define MIKTEX_TRACE_DVIPKCHAR MIKTEXTEXT("dvipkchar")
#define MIKTEX_TRACE_DVIPKFONT MIKTEXTEXT("dvipkfont")
#define MIKTEX_TRACE_DVISEARCH MIKTEXTEXT("dvisearch")
#define MIKTEX_TRACE_DVITFM MIKTEXTEXT("dvitfm")
#define MIKTEX_TRACE_DVIVFCHAR MIKTEXTEXT("dvivfchar")
#define MIKTEX_TRACE_DVIVFONT MIKTEXTEXT("dvivfont")
#define MIKTEX_TRACE_ENV MIKTEXTEXT("env")
#define MIKTEX_TRACE_ERROR MIKTEXTEXT("error")
#define MIKTEX_TRACE_EXTRACTOR MIKTEXTEXT("extractor")
#define MIKTEX_TRACE_FILES MIKTEXTEXT("files")
#define MIKTEX_TRACE_FILESEARCH MIKTEXTEXT("filesearch")
#define MIKTEX_TRACE_FNDB MIKTEXTEXT("fndb")
#define MIKTEX_TRACE_FONTINFO MIKTEXTEXT("fontinfo")
#define MIKTEX_TRACE_MMAP MIKTEXTEXT("mmap")
#define MIKTEX_TRACE_MPM MIKTEXTEXT("mpm")
#define MIKTEX_TRACE_MTPRINT MIKTEXTEXT("mtprint")
#define MIKTEX_TRACE_PACKAGES MIKTEXTEXT("packages")
#define MIKTEX_TRACE_PROCESS MIKTEXTEXT("process")
#define MIKTEX_TRACE_TEMPFILE MIKTEXTEXT("tempfile")
#define MIKTEX_TRACE_TIME MIKTEXTEXT("time")
#define MIKTEX_TRACE_VALUES MIKTEXTEXT("values")
#define MIKTEX_TRACE_YAP MIKTEXTEXT("yap")

#endif
