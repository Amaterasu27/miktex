/* miktex/Core/First.h:						-*- C++ -*-

   Copyright (C) 1996-2008 Christian Schenk

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

#if ! defined(E2D546821BA244C1A028468F9A733B20)
#define E2D546821BA244C1A028468F9A733B20

#if defined(_MSC_VER)
#  include <crtdbg.h>
#endif

#if defined(_MSC_VER) && defined(__cplusplus)

#undef _SECURE_SCL_THROWS
#define _SECURE_SCL_THROWS 1

namespace MiKTeX {
  namespace Debug {
    void
#if defined(EAD86981C92C904D808A5E6CEC64B90E)
    __declspec(dllexport)
#else
    __declspec(dllimport)
#endif
    __cdecl
    OnThrowStdException ();
  }
}

#include <exception>

#if ! defined(_THROW)
#  error _THROW macro is not defined in <exception>
#endif

#undef _THROW

#define _THROW(x, y)				\
  (						\
    MiKTeX::Debug::OnThrowStdException (),	\
    throw x (y),				\
    0						\
  )

#endif /* Microsoft C++ */

#endif
