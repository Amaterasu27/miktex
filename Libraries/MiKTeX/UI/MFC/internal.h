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
#include <miktex/UI/MFC/Prototypes>
#include <miktex/UI/MFC/ErrorDialog>

#include <miktex/UI/UI>

using namespace MiKTeX::Core;
using namespace MiKTeX::Packages;
using namespace MiKTeX::UI::MFC;
using namespace MiKTeX::UI;
using namespace std;

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
			     __FILE__,					\
			     __LINE__)

#define FATAL_WINDOWS_ERROR(windowsfunction, lpszInfo)	\
  Session::FatalWindowsError (windowsfunction,		\
			      lpszInfo,			\
			      __FILE__,			\
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

#if ! defined(UNICODE)
#  error UNICODE required
#endif

#define TU_(x) MiKTeX::Core::CharBuffer<char>(x).GetBuffer()
#define UT_(x) MiKTeX::Core::CharBuffer<wchar_t>(x).GetBuffer()

/* _________________________________________________________________________

   USE_MY_RESOURCES
   _________________________________________________________________________ */

class USE_MY_RESOURCES
{
public:
  USE_MY_RESOURCES ()
    : defaultResourceHandle (AfxGetResourceHandle())
  {
    extern AFX_EXTENSION_MODULE MikuiDLL;
    AfxSetResourceHandle (MikuiDLL.hModule);
  }

public:
  ~USE_MY_RESOURCES ()
  {
    AfxSetResourceHandle (defaultResourceHandle);
  }

private:
  HINSTANCE defaultResourceHandle;
};

#define BEGIN_USE_MY_RESOURCES() { USE_MY_RESOURCES useMyResources;
#define END_USE_MY_RESOURCES() }
