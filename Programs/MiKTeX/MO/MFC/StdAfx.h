/* StdAfx.h: pre-compiled header stuff				-*- C++ -*-

   Copyright (C) 2000-2007 Christian Schenk

   This file is part of MiKTeX Options.

   MiKTeX Options is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   MiKTeX Options is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MiKTeX Options; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#pragma once

#if ! defined(_SECURE_ATL)
#  define _SECURE_ATL 1
#endif

#if ! defined(VC_EXTRALEAN)
#  define VC_EXTRALEAN
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#define _AFX_ALL_WARNINGS

#include <afxwin.h>
#include <afxext.h>
#include <afxdtctl.h>
#include <afxmt.h>
#include <afxtempl.h>

#include <HtmlHelp.h>

#include <shlwapi.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#  include <afxcmn.h>
#endif

#include "mo-version.h"

#include <MiKTeX/Core/Core>
#include <MiKTeX/Core/Debug>
#include <miktex/paths.h>
#include <miktex/reg.h>
#include <MiKTeX/Core/win/DllProc>

#include <memory>
#include <set>
#include <string>
#include <vector>

#include <MiKTeX/UI/MFC/ErrorDialog>
#include <MiKTeX/UI/MFC/PackageTreeCtrl>
#include <MiKTeX/UI/MFC/ProgressDialog>
#include <MiKTeX/UI/MFC/SiteWizSheet>
#include <MiKTeX/UI/MFC/TextViewerDialog>
#include <MiKTeX/UI/MFC/UpdateDialog>

#if defined(_UNICODE)
#  if defined(_M_IX86)
#    pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  elif defined(_M_IA64)
#    pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  elif defined(_M_X64)
#    pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  else
#    pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  endif
#endif

using namespace MiKTeX;
using namespace MiKTeX::Packages;
using namespace MiKTeX::UI::MFC;
using namespace MiKTeX::Core;
using namespace std;
