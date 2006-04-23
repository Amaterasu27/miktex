/* char.h: MiKTeX text model					-*- C++ -*-

   Copyright (C) 1996-2006 Christian Schenk

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

/// @file char.h
/// @brief MiKTeX text model.

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(FA57A6CA5892114495469589C1AE54C0_)
#define FA57A6CA5892114495469589C1AE54C0_

#if defined(_MSC_VER) || defined(HAVE_TCHAR_H)
#  include <tchar.h>
#endif

#if defined(MIKTEX_UNICODE)
typedef wchar_t MIKTEXCHAR;
typedef unsigned wchar_t MIKTEXUCHAR;
typedef wint_t MIKTEXCHARINT;
#define MIKTEXTEXT(x) L##x
#define MIKTEXEOF WEOF
#else
/// The generic character type.
typedef char MIKTEXCHAR;
/// The generic unsigned character type.
typedef unsigned char MIKTEXUCHAR;
/// The generic character integer type.
typedef int MIKTEXCHARINT;
/// Converts a single-byte string/character literal into the generic
/// string/character counterpart.
#define MIKTEXTEXT(x) x
/// The generic end-of-file character.
#define MIKTEXEOF EOF
#endif

#if defined(MIKTEX_UNICODE)
#  error Unimplemented: A2T_
#else
#  define A2T_(x) x
#endif

#endif
