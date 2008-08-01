/* MiKTeX/UI/MFC/Prototypes:					-*- C++ -*-

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

#include <miktex/PackageManager/PackageManager>

// DLL import/export switch
#if ! defined(MIKTEXUI__77F15D3E_7C53_4571_B9D9_2E57DAD53055__)
#  define MIKTEXUIEXPORT __declspec(dllimport)
#endif

// API decoration for exported member functions
#define MIKTEXUITHISAPI(type) MIKTEXUIEXPORT type MIKTEXTHISCALL

#define MIKUI_MFC_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace UI {				\
      namespace MFC {

#define MIKUI_MFC_END_NAMESPACE			\
      }						\
    }						\
  }

MIKUI_MFC_BEGIN_NAMESPACE;

/* _________________________________________________________________________

   Prototypes 
   _________________________________________________________________________ */

MIKTEXUIEXPORT
void
MIKTEXCEECALL
InitializeFramework ();

const unsigned int YES = 1;
const unsigned int NO = 2;
const unsigned int DONTASKAGAIN = 4;

MIKTEXUIEXPORT
unsigned int
MIKTEXCEECALL
InstallPackageMessageBox
(/*[in]*/ HWND						hwndParent,
 /*[in]*/ class MiKTeX::Packages::PackageManager *	pManager,
 /*[in]*/ const char *					lpszPackageName,
 /*[in]*/ const char *					lpszTrigger);

MIKTEXUIEXPORT
bool
MIKTEXCEECALL
ProxyAuthenticationDialog (/*[in]*/ HWND hwndParent);

MIKUI_MFC_END_NAMESPACE;
