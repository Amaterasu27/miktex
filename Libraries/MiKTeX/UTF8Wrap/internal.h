/* internal.h: internal definitions				-*- C++ -*-

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

#if defined(_MSC_VER) && defined(MIKTEX_UTF8WRAP_SHARED)
#  define MIKTEXUTF8WRAPEXPORT __declspec(dllexport)
#else
#  define MIKTEXUTF8WRAPEXPORT
#endif

#define BF56453E041E4B58A0EA455A65DD28B1
#include "miktex/utf8wrap.h"

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define MIKTEXSTATICFUNC(type) static type
#define MIKTEXINTERNALFUNC(type) type
#define MIKTEXINTERNALVAR(type) type

