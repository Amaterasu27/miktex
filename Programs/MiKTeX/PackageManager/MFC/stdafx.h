/* stdafx.h:							-*- C++ -*-

   Copyright (C) 2002-2007 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if ! defined(VC_EXTRALEAN)
#  define VC_EXTRALEAN
#endif

#if ! defined(WINVER)
#  define WINVER 0x0600
#endif

#if ! defined(_WIN32_WINNT)
#  define _WIN32_WINNT 0x0600
#endif						

#if ! defined(_WIN32_WINDOWS)
#  define _WIN32_WINDOWS 0x0600
#endif

#if ! defined(_WIN32_IE)
#  define _WIN32_IE 0x0700
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#define _AFX_ALL_WARNINGS

#include <afxcview.h>
#include <afxdlgs.h>
#include <afxdtctl.h>
#include <afxext.h>
#include <afxmt.h>
#include <afxwin.h>

#if ! defined(_AFX_NO_AFXCMN_SUPPORT)
#  include <afxcmn.h>
#endif

#include "mpm-version.h"

#include <miktex/core.h>
#include <miktex/mpm.h>
#include <miktex/paths.h>
#include <miktex/win/DllProc.h>

#pragma warning (push, 1)
#pragma warning (disable: 4702)
#include <map>
#include <vector>
#include <string>
#pragma warning (pop)

#include <miktex/ErrorDialog.h>
#include <miktex/ProgressDialog.h>
#include <miktex/PropSheetPackage.h>
#include <miktex/SiteWizSheet.h>
#include <miktex/UpdateDialog.h>

using namespace MiKTeX;
using namespace MiKTeX::Packages;
using namespace MiKTeX::UI;
using namespace MiKTeX::Core;
using namespace std;
