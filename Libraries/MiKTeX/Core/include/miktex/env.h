/* miktex/env.h: MiKTeX environment variables

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(B644C2380CA33546A66551BC11D5194C)
#define B644C2380CA33546A66551BC11D5194C

#include <miktex/char.h>

#define MIKTEX_ENV_STARTUP_FILE \
  MIKTEXTEXT("MIKTEX_STARTUPFILE")

#define MIKTEX_ENV_BIN_DIR \
  MIKTEXTEXT("MIKTEX_BINDIR")

#define MIKTEX_ENV_TRACE \
  MIKTEXTEXT("MIKTEX_TRACE")

#define MIKTEX_ENV_PACKAGE_LIST_FILE \
  MIKTEXTEXT("MIKTEX_PKGLISTFILE")

#define MIKTEX_ENV_CWD \
  MIKTEXTEXT("MIKTEX_CWDLIST")

#define MIKTEX_ENV_USER_DATA \
  MIKTEXTEXT("MIKTEX_USERDATA")

#define MIKTEX_ENV_COMMON_DATA \
  MIKTEXTEXT("MIKTEX_COMMONDATA")

#define MIKTEX_ENV_USER_CONFIG \
  MIKTEXTEXT("MIKTEX_USERCONFIG")

#define MIKTEX_ENV_COMMON_CONFIG \
  MIKTEXTEXT("MIKTEX_COMMONCONFIG")

#define MIKTEX_ENV_INSTALL \
  MIKTEXTEXT("MIKTEX_INSTALL")

#define MIKTEX_ENV_ROOTS \
  MIKTEXTEXT("MIKTEX_ROOTS")

#define MIKTEX_ENV_REPOSITORY \
  MIKTEXTEXT("MIKTEX_REPOSITORY")

#endif	/* miktex/env.h */
