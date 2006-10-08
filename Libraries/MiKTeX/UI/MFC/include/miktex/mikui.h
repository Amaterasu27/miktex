/* miktex/mikui.h:						-*- C++ -*-

   Copyright (C) 2000-2006 Christian Schenk

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

#include <miktex/mpm.h>

#if ! defined(MIKTEXUI__77F15D3E_7C53_4571_B9D9_2E57DAD53055__)
#  define MIKTEXUIEXPORT __declspec(dllimport)
#endif

#define MIKTEXUICALL __stdcall
#define MIKTEXUIAPI(type) MIKTEXUIEXPORT type MIKTEXUICALL

#define MIKUI_BEGIN_NAMESPACE			\
  namespace MiKTeX {				\
    namespace UI {

#define MIKUI_END_NAMESPACE			\
    }						\
  }

MIKUI_BEGIN_NAMESPACE;


/* _________________________________________________________________________

   Protos
   _________________________________________________________________________ */

MIKTEXUIAPI(void)
InitializeFramework ();

const unsigned int YES = 1;
const unsigned int NO = 2;
const unsigned int DONTASKAGAIN = 4;

MIKTEXUIAPI(unsigned int)
InstallPackageMessageBox
(/*[in]*/ HWND						hwndParent,
 /*[in]*/ class MiKTeX::Packages::PackageManager *	pManager,
 /*[in]*/ const MIKTEXCHAR *				lpszPackageName,
 /*[in]*/ const MIKTEXCHAR *				lpszTrigger);

MIKTEXUIAPI(bool)
ProxyAuthenticationDialog (/*[in]*/ HWND hwndParent);

MIKUI_END_NAMESPACE;
