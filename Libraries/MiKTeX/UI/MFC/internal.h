/* internal.h: internal definitions				-*- C++ -*-

   Copyright (C) 2000-2007 Christian Schenk

   This file is part of MiKTeX UI Library.

   MiKTeX UI Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   MiKTeX UI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX UI Library; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#define MIKTEXUIEXPORT __declspec(dllexport)

#define MIKTEXUI__77F15D3E_7C53_4571_B9D9_2E57DAD53055__
#include <miktex/mikui.h>
#include <miktex/ErrorDialog.h>

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace MiKTeX::UI;
using namespace std;

typedef basic_ostringstream<MIKTEXCHAR> otstringstream;

/* _________________________________________________________________________

   Error Macros
   _________________________________________________________________________ */

#define INVALID_ARGUMENT(function, param)				\
  FATAL_MIKTEX_ERROR (function, T_("Invalid argument."), param)

#define UNEXPECTED_CONDITION(function)				\
  FATAL_MIKTEX_ERROR (function, T_("Unexpected condition."), 0)

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  Session::FatalMiKTeXError (miktexFunction,				\
			     traceMessage,				\
			     lpszInfo,					\
			     T_(__FILE__),				\
			     __LINE__)

#define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,		\
			      lpszInfo,			\
			      T_(__FILE__),		\
			      __LINE__)

#define CHECK_WINDOWS_ERROR(windowsFunction, lpszInfo)	\
  if (::GetLastError() != ERROR_SUCCESS)		\
    {							\
      FATAL_WINDOWS_ERROR (windowsFunction, lpszInfo);	\
    }

/* _________________________________________________________________________

   Helper Macros
   _________________________________________________________________________ */

#define T_(x) MIKTEXTEXT(x)

/* _________________________________________________________________________

   IsWindowsVista
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
inline
bool
IsWindowsVista ()
{
  return ((GetVersion() & 0xff) >= 6);
}
#endif

/* _________________________________________________________________________

   AutoResource
   _________________________________________________________________________ */

template <class HandleType,
	  class Destroyer,
	  HandleType InvalidHandleValue = 0>
class AutoResource
{
public:
  AutoResource ()
  {
    handle = InvalidHandleValue;
  }

public:
  AutoResource (/*[in]*/ HandleType handle)
    : handle (handle)
  {
  }

public:
  ~AutoResource ()
  {
    try
      {
	if (handle != InvalidHandleValue)
	  {
	    Destroyer() (handle);
	  }
      }
    catch (const exception &)
      {
      }
  }

public:
  HandleType
  Get ()
    const
  {
    return (handle);
  }

  // deprecated: automatic type conversion; use Get() instead
public:
  operator HandleType ()
    const
  {
    return (Get());
  }

public:
  HandleType *
  operator & ()
  {
    return (&handle);
  }

private:
  HandleType handle;
};

/* _________________________________________________________________________

   AutoErrorMode
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class SetErrorMode_
{
public:
  void
  operator() (/*[in]*/ unsigned int mode)
  {
    SetErrorMode (mode);
  }
};

typedef AutoResource<unsigned int, SetErrorMode_> AutoErrorMode;
#endif

/* _________________________________________________________________________

   AutoGlobal
   _________________________________________________________________________ */

#if defined(MIKTEX_WINDOWS)
class GlobalUnlock_
{
public:
  void
  operator() (/*[in]*/ HGLOBAL hMem)
  {
    GlobalUnlock (hMem);
  }
};

typedef AutoResource<void *, GlobalUnlock_> AutoGlobal;
#endif
