/* urls.h: hard-coded urls

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

#if ! defined(BCD264408F6CC94AB371F96954CBDA98_)
#define BCD264408F6CC94AB371F96954CBDA98_

#include <miktex/char.h>
#include <miktex/version.h>

#define MIKTEX_URL_WWW_NO_SLASH MIKTEXTEXT("http://miktex.org")

#define MIKTEX_URL_WWW MIKTEX_URL_WWW_NO_SLASH MIKTEXTEXT("/")

#define MIKTEX_URL_WWW_GIVE_BACK \
  MIKTEX_URL_WWW MIKTEXTEXT("giveback.aspx")

#if 1
#  define MIKTEX_URL_WWW_REGISTRATION MIKTEX_URL_WWW_GIVE_BACK
#else
#  define MIKTEX_URL_WWW_REGISTRATION \
  MIKTEX_URL_WWW MIKTEXTEXT("Registration.aspx")
#endif

#define MIKTEX_URL_WWW_KNOWN_ISSUES \
  MIKTEX_URL_WWW MIKTEXTEXT(MIKTEX_SERIES_STR) MIKTEXTEXT("/Issues.aspx")

#define MIKTEX_URL_WWW_SUPPORT MIKTEX_URL_WWW MIKTEXTEXT("Support.aspx")

#endif
